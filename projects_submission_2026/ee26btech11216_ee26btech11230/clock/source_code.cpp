/*
 * ============================================================================
 * DIGITAL CLOCK + STOPWATCH + COUNTDOWN TIMER (BOOLEAN BCD LOGIC ENGINES)
 * ============================================================================
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Shared 7447 Inputs: D4-D7 -> PIN_A..PIN_D
const uint8_t PIN_A = 4, PIN_B = 5, PIN_C = 6, PIN_D = 7;
// Digit Enables: D8-D13 -> sec1, sec10, min1, min10, hr1, hr10
const uint8_t EN[] = {8, 9, 10, 11, 12, 13};

const uint8_t MENU_BTN = A0;
const uint8_t UP_BTN = A1;
const uint8_t DOWN_BTN = A2;
const uint8_t SELECT_BTN = A3;
const uint8_t BUZZER = 2;
const uint8_t TOUCH_SENSOR = 3;

volatile bool displayEnabled = true;
volatile bool displayForcedOn = false;
bool lastTouchState = LOW;

// ---------------- CLOCK BOOLEAN STATES (BCD Representation) ----------------
int W1=0, X1=0, Y1=0, Z1=0; // Sec Ones (0-9)
int W2=0, X2=0, Y2=0;       // Sec Tens (0-5)
int W3=0, X3=0, Y3=0, Z3=0; // Min Ones (0-9)
int W4=0, X4=0, Y4=0;       // Min Tens (0-5)
int W5=0, X5=1, Y5=0, Z5=0; // Hr Ones  (Default 12 hr)
int W6=1, X6=0, Y6=0;       // Hr Tens

// ---------------- STOPWATCH BOOLEAN STATES (BCD Representation) ------------
int SW_W0=0, SW_X0=0, SW_Y0=0, SW_Z0=0; // cs ones
int SW_W1=0, SW_X1=0, SW_Y1=0, SW_Z1=0; // cs tens
int SW_W2=0, SW_X2=0, SW_Y2=0, SW_Z2=0; // sec ones
int SW_W3=0, SW_X3=0, SW_Y3=0;          // sec tens
int SW_W4=0, SW_X4=0, SW_Y4=0, SW_Z4=0; // min ones
int SW_W5=0, SW_X5=0, SW_Y5=0, SW_Z5=0; // min tens

// ---------------- TIMER BOOLEAN STATES (BCD Reverse Clock Representation) ----
int T_W0=0, T_X0=0, T_Y0=0, T_Z0=0; // Sec Ones (0-9)
int T_W1=0, T_X1=0, T_Y1=0, T_Z1=0; // Sec Tens (0-5)
int T_W2=0, T_X2=0, T_Y2=0, T_Z2=0; // Min Ones (0-9)
int T_W3=0, T_X3=0, T_Y3=0, T_Z3=0; // Min Tens (0-5)
int T_W4=0, T_X4=0, T_Y4=0, T_Z4=0; // Hr Ones  (0-9)
int T_W5=0, T_X5=0, T_Y5=0, T_Z5=0; // Hr Tens  (0-9)

// Date & Time auxiliary trackers
uint8_t day = 19, month = 8;
uint16_t year = 2026;
uint32_t lastClock = 0;

// Stopwatch Status
bool stopwatchRunning = false;
bool stopwatchMaxReached = false;
uint32_t lastStopwatchTick = 0;

// Timer
bool timerRunning = false;
uint32_t lastTimer = 0;
uint8_t timerSetHour = 0, timerSetMinute = 5, timerSetSecond = 0;

// Alarm
bool alarmEnabled = false;
bool alarmActive = false;
bool timerRinging = false;
bool timerPreviousDisplayState = false;
bool alarmDisplayWasOff = false;
bool alarmSnoozed = false;
uint8_t alarmHour = 7, alarmMinute = 0, alarmField = 0;
uint32_t alarmEnd = 0, alarmPatternTime = 0;
uint8_t alarmPatternStep = 0;

enum Screen : uint8_t {
  MAIN_MENU, CLOCK_SCREEN, STOPWATCH_SCREEN, TIMER_SCREEN,
  SET_TIME_SCREEN, SET_DATE_SCREEN, SET_TIMER_SCREEN, SET_ALARM_SCREEN
};

Screen screen = MAIN_MENU;
uint8_t menuSelection = 0, timeField = 0, dateField = 0, timerField = 0;
bool oledDirty = true;
uint32_t lastOLED = 0;

struct Button {
  uint8_t pin;
  uint8_t stable;
  uint8_t raw;
  uint32_t changed;
};

Button btn[] = {
  {MENU_BTN, HIGH, HIGH, 0},
  {UP_BTN, HIGH, HIGH, 0},
  {DOWN_BTN, HIGH, HIGH, 0},
  {SELECT_BTN, HIGH, HIGH, 0}
};

const uint16_t DEBOUNCE_MS = 18;

enum BeepType : uint8_t { B_NONE, B_SINGLE, B_MINUTE, B_HOUR, B_TIMER_MAX };
BeepType beepType = B_NONE;
uint8_t beepStep = 0;
bool beepOn = false;
uint32_t beepTime = 0;

bool timerAlarm = false;
uint8_t timerAlarmBeep = 0, timerAlarmCycle = 0;
bool timerAlarmOn = false;
uint32_t timerAlarmTime = 0;

volatile uint8_t segFront[6] = {0, 0, 0, 0, 0, 0};

// Function Prototypes
void forceDisplayOn();
void print2(uint16_t n);
void drawAlarmActive();
void drawSetAlarm();
void drawMainMenu();
void drawClock();
void drawStopwatch();
void drawTimer();
void drawSetTime();
void drawSetDate();
void drawSetTimer();

// ---------------- TIMER BCD LOGIC HELPERS ----------------
uint8_t getTimerSeconds() { return (T_W1 + T_X1*2 + T_Y1*4) * 10 + (T_W0 + T_X0*2 + T_Y0*4 + T_Z0*8); }
uint8_t getTimerMinutes() { return (T_W3 + T_X3*2 + T_Y3*4) * 10 + (T_W2 + T_X2*2 + T_Y2*4 + T_Z2*8); }
uint8_t getTimerHours()   { return (T_W5 + T_X5*2 + T_Y5*4 + T_Z5*8) * 10 + (T_W4 + T_X4*2 + T_Y4*4 + T_Z4*8); }

bool isTimerZero() {
  return (T_W0|T_X0|T_Y0|T_Z0|T_W1|T_X1|T_Y1|T_Z1|
          T_W2|T_X2|T_Y2|T_Z2|T_W3|T_X3|T_Y3|T_Z3|
          T_W4|T_X4|T_Y4|T_Z4|T_W5|T_X5|T_Y5|T_Z5) == 0;
}

void setTimerBCD(uint8_t h, uint8_t m, uint8_t s) {
  uint8_t s0 = s % 10, s1 = s / 10;
  uint8_t m0 = m % 10, m1 = m / 10;
  uint8_t h0 = h % 10, h1 = h / 10;

  T_W0 = (s0 & 1); T_X0 = (s0 & 2) >> 1; T_Y0 = (s0 & 4) >> 2; T_Z0 = (s0 & 8) >> 3;
  T_W1 = (s1 & 1); T_X1 = (s1 & 2) >> 1; T_Y1 = (s1 & 4) >> 2; T_Z1 = 0;

  T_W2 = (m0 & 1); T_X2 = (m0 & 2) >> 1; T_Y2 = (m0 & 4) >> 2; T_Z2 = (m0 & 8) >> 3;
  T_W3 = (m1 & 1); T_X3 = (m1 & 2) >> 1; T_Y3 = (m1 & 4) >> 2; T_Z3 = 0;

  T_W4 = (h0 & 1); T_X4 = (h0 & 2) >> 1; T_Y4 = (h0 & 4) >> 2; T_Z4 = (h0 & 8) >> 3;
  T_W5 = (h1 & 1); T_X5 = (h1 & 2) >> 1; T_Y5 = (h1 & 4) >> 2; T_Z5 = (h1 & 8) >> 3;
}

void decrementTimerBCDDigit(int d) {
  int A, B, C, D;
  switch (d) {
    case 0: { // Sec Ones 0–9
      A = !T_W0;
      B = (!T_X0 && !T_W0 && ((!T_Z0 && T_Y0)||(T_Z0 && !T_Y0))) || (!T_Z0 && T_W0 && T_X0); 
      C = (!T_Z0 && T_Y0 && (T_X0||T_W0)) || (T_Z0 && !T_X0 && !T_W0 && !T_Y0);
      D = !T_X0 && !T_Y0 && ((T_Z0 && T_W0) || (!T_Z0 && !T_W0));
      T_W0=A; T_X0=B; T_Y0=C; T_Z0=D;
    } break;
    case 1: { // Sec Tens 0–5 (Rolls from 0 to 5)
      if (T_W1==0 && T_X1==0 && T_Y1==0) {
        T_W1=1; T_X1=0; T_Y1=1; // Reset to 5
      } else {
        A = !T_W1;
        B = (T_Y1 && !T_X1 && !T_W1) || (!T_Y1 && T_X1 && T_W1);
        C = !T_X1 && ((T_Y1 && T_W1) || (!T_Y1 && !T_W1));
        T_W1=A; T_X1=B; T_Y1=C;
      }
    } break;
    case 2: { // Min Ones 0–9
      A = !T_W2;
      B = (!T_X2 && !T_W2 && ((!T_Z2 && T_Y2)||(T_Z2 && !T_Y2))) || (!T_Z2 && T_W2 && T_X2);
      C = (!T_Z2 && T_Y2 && (T_X2||T_W2)) || (T_Z2 && !T_X2 && !T_W2 && !T_Y2);
      D = !T_X2 && !T_Y2 && ((T_Z2 && T_W2) || (!T_Z2 && !T_W2));
      T_W2=A; T_X2=B; T_Y2=C; T_Z2=D;
    } break;
    case 3: { // Min Tens 0–5 (Rolls from 0 to 5)
      if (T_W3==0 && T_X3==0 && T_Y3==0) {
        T_W3=1; T_X3=0; T_Y3=1; // Reset to 5
      } else {
        A = !T_W3;
        B = (T_Y3 && !T_X3 && !T_W3) || (!T_Y3 && T_X3 && T_W3);
        C = !T_X3 && ((T_Y3 && T_W3) || (!T_Y3 && !T_W3));
        T_W3=A; T_X3=B; T_Y3=C;
      }
    } break;
    case 4: { // Hr Ones 0–9
      A = !T_W4;
      B = (!T_X4 && !T_W4 && ((!T_Z4 && T_Y4)||(T_Z4 && !T_Y4))) || (!T_Z4 && T_W4 && T_X4);
      C = (!T_Z4 && T_Y4 && (T_X4||T_W4)) || (T_Z4 && !T_X4 && !T_W4 && !T_Y4);
      D = !T_X4 && !T_Y4 && ((T_Z4 && T_W4) || (!T_Z4 && !T_W4));
      T_W4=A; T_X4=B; T_Y4=C; T_Z4=D;
    } break;
    case 5: { // Hr Tens 0–9
      A = !T_W5;
      B = (!T_X5 && !T_W5 && ((!T_Z5 && T_Y5)||(T_Z5 && !T_Y5))) || (!T_Z5 && T_W5 && T_X5);
      C = (!T_Z5 && T_Y5 && (T_X5||T_W5)) || (T_Z5 && !T_X5 && !T_W5 && !T_Y5);
      D = !T_X5 && !T_Y5 && ((T_Z5 && T_W5) || (!T_Z5 && !T_W5));
      T_W5=A; T_X5=B; T_Y5=C; T_Z5=D;
    } break;
  }
}

// ---------------- CLOCK BOOLEAN LOGIC HELPERS ----------------
void incrementDigit(int d){
  int A,B,C,D;
  switch(d){
    case 0: {
      A = !W1;
      B = (W1 && !X1 && !Z1)||(!W1 && X1);
      C = (!X1 && Y1)||(!W1 && Y1)||(W1 && X1 && !Y1);
      D = (!W1 && Z1)||(W1 && X1 && Y1);
      W1=A; X1=B; Y1=C; Z1=D;
    } break;
    case 1: {
      A = !W2;
      B = (W2 && !X2 && !Y2)||(!W2 && X2);
      C = (W2 && X2)||(!W2 && !X2 && Y2);
      W2=A; X2=B; Y2=C;
    } break;
    case 2: {
      A = !W3;
      B = (W3 && !X3 && !Z3)||(!W3 && X3);
      C = (!X3 && Y3)||(!W3 && Y3)||(W3 && X3 && !Y3);
      D = (!W3 && Z3)||(W3 && X3 && Y3);
      W3=A; X3=B; Y3=C; Z3=D;
    } break;
    case 3: {
      A = !W4;
      B = (W4 && !X4 && !Y4)||(!W4 && X4);
      C = (W4 && X4)||(!W4 && !X4 && Y4);
      W4=A; X4=B; Y4=C;
    } break;
    case 4: {
      if(X6==0){
        A = !W5;
        B = (W5 && !X5 && !Z5)||(!W5 && X5);
        C = (!X5 && Y5)||(!W5 && Y5)||(W5 && X5 && !Y5);
        D = (!W5 && Z5)||(W5 && X5 && Y5);
        W5=A; X5=B; Y5=C; Z5=D;
      } else {
        A = !W5;
        B = (W5 && !X5)||(!W5 && X5);
        W5=A; X5=B; Y5=0; Z5=0;
      }
    } break;
    case 5: {
      if(!(X6==0 && W6==1 && Y5==1)){
        A = !W6 && !X6;
        B = W6 && !X6;
        W6=A; X6=B; Y6=0;
      }
    } break;
  }
}

void decrementDigit(int d){
  int A,B,C,D;
  switch(d){
    case 0: {
      A = !W1;
      B = (!X1 && !W1 && ((!Z1 && Y1)||(Z1 && !Y1))) || (!Z1 && W1 && X1); 
      C = (!Z1 && Y1 && (X1||W1)) || (Z1 && !X1 && !W1 && !Y1);
      D = !X1 && !Y1 && ((Z1 && W1) || (!Z1 && !W1));
      W1=A; X1=B; Y1=C; Z1=D;
    } break;
    case 1: {
      A = !W2;
      B = (Y2 && !X2 && !W2) || (!Y2 && X2 && W2);
      C = !X2 && ((Y2 && W2) || (!Y2 && !W2));
      W2=A; X2=B; Y2=C;
    } break;
    case 2: {
      A = !W3;
      B = (!X3 && !W3 && ((!Z3 && Y3)||(Z3 && !Y3))) || (!Z3 && W3 && X3);
      C = (!Z3 && Y3 && (X3||W3)) || (Z3 && !X3 && !W3 && !Y3);
      D = !X3 && !Y3 && ((Z3 && W3) || (!Z3 && !W3));
      W3=A; X3=B; Y3=C; Z3=D;
    } break;
    case 3: {
      A = !W4;
      B = (Y4 && !X4 && !W4) || (!Y4 && X4 && W4);
      C = !X4 && ((Y4 && W4) || (!Y4 && !W4));
      W4=A; X4=B; Y4=C;
    } break;
    case 4: {
      if(X6==0){
        A = !W5;
        B = (!X5 && !W5 && ((!Z5 && Y5)||(Z5 && !Y5))) || (!Z5 && W5 && X5);
        C = (!Z5 && Y5 && (X5||W5)) || (Z5 && !X5 && !W5 && !Y5);
        D = !X5 && !Y5 && ((Z5 && W5) || (!Z5 && !W5));
        W5=A; X5=B; Y5=C; Z5=D;
      } else {
        A = !W5;
        B = (X5 && W5) || (!X5 && !W5);
        W5=A; X5=B; Y5=0; Z5=0;
      }
    } break;
    case 5: {
      if(!(X6==0 && W6==0 && Y5==1)){
        A = X6 && !W6;
        B = !X6 && !W6;
        W6=A; X6=B; Y6=0;
      }
    } break;
  }
}

// BCD Conversion Utilities
uint8_t getHours() { return (W6 + X6*2) * 10 + (W5 + X5*2 + Y5*4 + Z5*8); }
uint8_t getMinutes() { return (W4 + X4*2 + Y4*4) * 10 + (W3 + X3*2 + Y3*4 + Z3*8); }
uint8_t getSeconds() { return (W2 + X2*2 + Y2*4) * 10 + (W1 + X1*2 + Y1*4 + Z1*8); }

// Stopwatch BCD Getters
uint8_t getSWCentiseconds() { return (SW_W1 + SW_X1*2 + SW_Y1*4 + SW_Z1*8) * 10 + (SW_W0 + SW_X0*2 + SW_Y0*4 + SW_Z0*8); }
uint8_t getSWSeconds() { return (SW_W3 + SW_X3*2 + SW_Y3*4) * 10 + (SW_W2 + SW_X2*2 + SW_Y2*4 + SW_Z2*8); }
uint8_t getSWMinutes() { return (SW_W5 + SW_X5*2 + SW_Y5*4 + SW_Z5*8) * 10 + (SW_W4 + SW_X4*2 + SW_Y4*4 + SW_Z4*8); }

void resetStopwatchBCD() {
  SW_W0=0; SW_X0=0; SW_Y0=0; SW_Z0=0;
  SW_W1=0; SW_X1=0; SW_Y1=0; SW_Z1=0;
  SW_W2=0; SW_X2=0; SW_Y2=0; SW_Z2=0;
  SW_W3=0; SW_X3=0; SW_Y3=0;
  SW_W4=0; SW_X4=0; SW_Y4=0; SW_Z4=0;
  SW_W5=0; SW_X5=0; SW_Y5=0; SW_Z5=0;
}

// ---------------- HARDWARE MULTIPLEXING TIMER ISR ----------------
void setupMuxTimer() {
  DDRD |= 0xF0;
  DDRB |= 0x3F;
  PORTD &= 0x0F;
  PORTB &= 0xC0;

  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 7999;
  TCCR1B = _BV(WGM12) | _BV(CS10);
  TIMSK1 = _BV(OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  static uint8_t digit = 0;
  PORTB &= 0xC0;

  if (!displayEnabled && !displayForcedOn) {
    digit = (digit + 1) % 6;
    return;
  }

  uint8_t value = segFront[5 - digit] & 0x0F;
  PORTD = (PORTD & 0x0F) | (value << 4);
  PORTB |= _BV(digit);

  digit = (digit + 1) % 6;
}

void publishDigits(const uint8_t d[6]) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    for (uint8_t i = 0; i < 6; i++) segFront[i] = d[i];
  }
}

// ---------------- SETUP & MAIN LOOP ----------------
void setup() {
  pinMode(MENU_BTN, INPUT_PULLUP);
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(SELECT_BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(TOUCH_SENSOR, INPUT);
  noTone(BUZZER);

  Wire.begin();
  Wire.setClock(400000UL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {}
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.print(F("CLOCK"));
  display.display();

  uint32_t now = millis();
  lastClock = now;
  lastTimer = now;
  lastOLED = now;
  lastStopwatchTick = now;

  setTimerBCD(timerSetHour, timerSetMinute, timerSetSecond);

  for (uint8_t i = 0; i < 4; i++) {
    btn[i].stable = digitalRead(btn[i].pin);
    btn[i].raw = btn[i].stable;
    btn[i].changed = now;
  }

  setupMuxTimer();
}

void loop() {
  uint32_t now = millis();

  serviceTouch();
  serviceButtons(now);
  updateClock(now);
  updateStopwatch(now);
  updateTimer(now);
  serviceBuzzer(now);
  serviceTimerAlarm(now);
  serviceAlarm(now);
  updateSevenSegment();
  updateOLED(now);
}

// ---------------- CLOCK ENGINE WITH BOOLEAN LOGIC ----------------
void updateClock(uint32_t now) {
  uint32_t elapsed = now - lastClock;
  if (elapsed < 1000UL) return;

  uint32_t ticks = elapsed / 1000UL;
  lastClock += ticks * 1000UL;

  while (ticks--) {
    int A = !W1;
    int B = (W1 && !X1 && !Z1)||(!W1 && X1);
    int C = (!X1 && Y1)||(!W1 && Y1)||(W1 && X1 && !Y1);
    int D = (!W1 && Z1)||(W1 && X1 && Y1);
    W1=A; X1=B; Y1=C; Z1=D;

    if((W1|X1|Y1|Z1)==0){
      A = !W2;
      B = (W2 && !X2 && !Y2)||(!W2 && X2);
      C = (W2 && X2)||(!W2 && !X2 && Y2);
      W2=A; X2=B; Y2=C;

      if((W1|X1|Y1|Z1|W2|X2|Y2)==0){
        A = !W3;
        B = (W3 && !X3 && !Z3)||(!W3 && X3);
        C = (!X3 && Y3)||(!W3 && Y3)||(W3 && X3 && !Y3);
        D = (!W3 && Z3)||(W3 && X3 && Y3);
        W3=A; X3=B; Y3=C; Z3=D;

        if((W3|X3|Y3|Z3)==0){
          A = !W4;
          B = (W4 && !X4 && !Y4)||(!W4 && X4);
          C = (W4 && X4)||(!W4 && !X4 && Y4);
          W4=A; X4=B; Y4=C;
        }

        if((W3|X3|Y3|Z3|W4|X4|Y4)==0){
          if(X6==0){
            A = !W5;
            B = (W5 && !X5 && !Z5)||(!W5 && X5);
            C = (!X5 && Y5)||(!W5 && Y5)||(W5 && X5 && !Y5);
            D = (!W5 && Z5)||(W5 && X5 && Y5);
            W5=A; X5=B; Y5=C; Z5=D;
          } else {
            A = !W5;
            B = (W5 && !X5)||(!W5 && X5);
            W5=A; X5=B; Y5=0; Z5=0;
          }

          if((W5|X5|Y5|Z5)==0){
            A = !W6 && !X6;
            B = W6 && !X6;
            W6=A; X6=B; Y6=0;
            if(W6==0 && X6==0) incrementDate();
          }
          startBeep(B_HOUR);
        } else {
          startBeep(B_MINUTE);
        }
        checkAlarm();
        oledDirty = true;
      }
    }
  }
}

// ---------------- STOPWATCH ENGINE WITH BOOLEAN LOGIC ----------------
void updateStopwatch(uint32_t now) {
  if (!stopwatchRunning || stopwatchMaxReached) return;

  uint32_t elapsed = now - lastStopwatchTick;
  if (elapsed < 10UL) return;

  uint32_t ticks = elapsed / 10UL;
  lastStopwatchTick += ticks * 10UL;

  while (ticks--) {
    int A = !SW_W0;
    int B = (SW_W0 && !SW_X0 && !SW_Z0)||(!SW_W0 && SW_X0);
    int C = (!SW_X0 && SW_Y0)||(!SW_W0 && SW_Y0)||(SW_W0 && SW_X0 && !SW_Y0);
    int D = (!SW_W0 && SW_Z0)||(SW_W0 && SW_X0 && SW_Y0);
    SW_W0=A; SW_X0=B; SW_Y0=C; SW_Z0=D;

    if((SW_W0|SW_X0|SW_Y0|SW_Z0)==0) {
      A = !SW_W1;
      B = (SW_W1 && !SW_X1 && !SW_Z1)||(!SW_W1 && SW_X1);
      C = (!SW_X1 && SW_Y1)||(!SW_W1 && SW_Y1)||(SW_W1 && SW_X1 && !SW_Y1);
      D = (!SW_W1 && SW_Z1)||(SW_W1 && SW_X1 && SW_Y1);
      SW_W1=A; SW_X1=B; SW_Y1=C; SW_Z1=D;

      if((SW_W1|SW_X1|SW_Y1|SW_Z1)==0) {
        A = !SW_W2;
        B = (SW_W2 && !SW_X2 && !SW_Z2)||(!SW_W2 && SW_X2);
        C = (!SW_X2 && SW_Y2)||(!SW_W2 && SW_Y2)||(SW_W2 && SW_X2 && !SW_Y2);
        D = (!SW_W2 && SW_Z2)||(SW_W2 && SW_X2 && SW_Y2);
        SW_W2=A; SW_X2=B; SW_Y2=C; SW_Z2=D;

        if((SW_W2|SW_X2|SW_Y2|SW_Z2)==0) {
          A = !SW_W3;
          B = (SW_W3 && !SW_X3 && !SW_Y3)||(!SW_W3 && SW_X3);
          C = (SW_W3 && SW_X3)||(!SW_W3 && !SW_X3 && SW_Y3);
          SW_W3=A; SW_X3=B; SW_Y3=C;

          if((SW_W3|SW_X3|SW_Y3)==0) {
            A = !SW_W4;
            B = (SW_W4 && !SW_X4 && !SW_Z4)||(!SW_W4 && SW_X4);
            C = (!SW_X4 && SW_Y4)||(!SW_W4 && SW_Y4)||(SW_W4 && SW_X4 && !SW_Y4);
            D = (!SW_W4 && SW_Z4)||(SW_W4 && SW_X4 && SW_Y4);
            SW_W4=A; SW_X4=B; SW_Y4=C; SW_Z4=D;

            if((SW_W4|SW_X4|SW_Y4|SW_Z4)==0) {
              A = !SW_W5;
              B = (SW_W5 && !SW_X5 && !SW_Z5)||(!SW_W5 && SW_X5);
              C = (!SW_X5 && SW_Y5)||(!SW_W5 && SW_Y5)||(SW_W5 && SW_X5 && !SW_Y5);
              D = (!SW_W5 && SW_Z5)||(SW_W5 && SW_X5 && SW_Y5);
              SW_W5=A; SW_X5=B; SW_Y5=C; SW_Z5=D;

              if (getSWMinutes() == 99 && getSWSeconds() == 59 && getSWCentiseconds() == 99) {
                stopwatchRunning = false;
                stopwatchMaxReached = true;
                break;
              }
            }
          }
        }
      }
    }
  }
  oledDirty = true;
}

// ---------------- REVERSE CLOCK TIMER ENGINE WITH BOOLEAN BCD LOGIC ----------------
void updateTimer(uint32_t now) {
  if (!timerRunning) return;

  uint32_t elapsed = now - lastTimer;
  if (elapsed < 1000UL) return;

  uint32_t ticks = elapsed / 1000UL;
  lastTimer += ticks * 1000UL;

  while (ticks--) {
    if (isTimerZero()) {
      timerRunning = false;
      timerAlarm = true;
      timerPreviousDisplayState = displayEnabled;
      displayForcedOn = true;
      timerAlarmBeep = 0;
      timerAlarmCycle = 0;
      timerAlarmOn = false;
      timerAlarmTime = now;
      break;
    }

    // 1. Decrement Sec Ones (0-9)
    bool secOnesWasZero = (T_W0|T_X0|T_Y0|T_Z0) == 0;
    decrementTimerBCDDigit(0);

    if (secOnesWasZero) {
      // 2. Decrement Sec Tens (0-5)
      bool secTensWasZero = (T_W1|T_X1|T_Y1|T_Z1) == 0;
      decrementTimerBCDDigit(1);

      if (secTensWasZero) {
        // 3. Decrement Min Ones (0-9)
        bool minOnesWasZero = (T_W2|T_X2|T_Y2|T_Z2) == 0;
        decrementTimerBCDDigit(2);

        if (minOnesWasZero) {
          // 4. Decrement Min Tens (0-5)
          bool minTensWasZero = (T_W3|T_X3|T_Y3|T_Z3) == 0;
          decrementTimerBCDDigit(3);

          if (minTensWasZero) {
            // 5. Decrement Hr Ones (0-9)
            bool hrOnesWasZero = (T_W4|T_X4|T_Y4|T_Z4) == 0;
            decrementTimerBCDDigit(4);

            if (hrOnesWasZero) {
              // 6. Decrement Hr Tens (0-9)
              decrementTimerBCDDigit(5);
            }
          }
        }
      }
    }

    if (isTimerZero()) {
      timerRunning = false;
      timerAlarm = true;
      timerPreviousDisplayState = displayEnabled;
      displayForcedOn = true;
      timerAlarmBeep = 0;
      timerAlarmCycle = 0;
      timerAlarmOn = false;
      timerAlarmTime = now;
      break;
    }
  }
  oledDirty = true;
}

// ---------------- INPUT AND STATE MANAGEMENT ----------------
void setDisplayEnabled(bool on) {
  displayEnabled = on;
  if (!on) {
    display.clearDisplay();
    display.display();
    PORTB &= 0xC0;
  }
  oledDirty = true;
}

void serviceTouch() {
  uint8_t state = digitalRead(TOUCH_SENSOR);
  if (stopwatchRunning || stopwatchMaxReached || timerRunning || timerAlarm || alarmActive) {
    lastTouchState = state;
    return;
  }
  if (state == HIGH && lastTouchState == LOW) {
    setDisplayEnabled(!displayEnabled);
    startBeep(B_SINGLE);
  }
  lastTouchState = state;
}

bool pressed(Button &b, uint32_t now) {
  uint8_t r = digitalRead(b.pin);
  if (r != b.raw) { b.raw = r; b.changed = now; }
  if (r != b.stable && now - b.changed >= DEBOUNCE_MS) {
    b.stable = r;
    return r == LOW;
  }
  return false;
}

void serviceButtons(uint32_t now) {
  bool buttonPressed = false;
  if (pressed(btn[0], now)) { buttonPressed = true; startBeep(B_SINGLE); menuButton(); }
  if (pressed(btn[1], now)) { buttonPressed = true; startBeep(B_SINGLE); upButton(); }
  if (pressed(btn[2], now)) { buttonPressed = true; startBeep(B_SINGLE); downButton(); }
  if (pressed(btn[3], now)) { buttonPressed = true; startBeep(B_SINGLE); selectButton(); }

  if (buttonPressed && !displayEnabled && !alarmActive && !timerAlarm) {
    setDisplayEnabled(true);
  }
}

void menuButton() {
  screen = (screen == MAIN_MENU) ? CLOCK_SCREEN : MAIN_MENU;
  oledDirty = true;
}

void upButton() {
  switch (screen) {
    case MAIN_MENU: menuSelection = (menuSelection + 1) % 7; break;
    case STOPWATCH_SCREEN:
      if (!stopwatchRunning && !stopwatchMaxReached) {
        lastStopwatchTick = millis();
        stopwatchRunning = true;
      }
      break;
    case TIMER_SCREEN:
      if (!timerRunning) {
        uint8_t m = getTimerMinutes() + 1;
        uint8_t h = getTimerHours();
        if (m >= 60) { m = 0; h = (h >= 99) ? 99 : h + 1; }
        setTimerBCD(h, m, getTimerSeconds());
      }
      break;
    case SET_TIME_SCREEN: changeTime(1); break;
    case SET_DATE_SCREEN: changeDate(1); break;
    case SET_TIMER_SCREEN: changeTimer(1); break;
    case SET_ALARM_SCREEN: changeAlarm(1); break;
    default: break;
  }
  oledDirty = true;
}

void downButton() {
  if (alarmActive) { snoozeAlarm(); return; }
  switch (screen) {
    case MAIN_MENU: menuSelection = (menuSelection == 0) ? 6 : menuSelection - 1; break;
    case STOPWATCH_SCREEN:
      stopwatchRunning = false; 
      stopwatchMaxReached = false;
      resetStopwatchBCD();
      break;
    case TIMER_SCREEN:
      if (!timerRunning) {
        uint8_t m = getTimerMinutes();
        uint8_t h = getTimerHours();
        if (m > 0) { m--; }
        else if (h > 0) { h--; m = 59; }
        setTimerBCD(h, m, getTimerSeconds());
      }
      break;
    case SET_TIME_SCREEN: changeTime(-1); break;
    case SET_DATE_SCREEN: changeDate(-1); break;
    case SET_TIMER_SCREEN: changeTimer(-1); break;
    case SET_ALARM_SCREEN: changeAlarm(-1); break;
    default: break;
  }
  oledDirty = true;
}

void selectButton() {
  if (alarmActive) { stopAlarm(); return; }
  if (timerAlarm) {
    timerAlarm = false; timerAlarmOn = false; noTone(BUZZER); displayForcedOn = false;
    if (!timerPreviousDisplayState) setDisplayEnabled(false);
    oledDirty = true; return;
  }
  switch (screen) {
    case MAIN_MENU:
      if (menuSelection == 0) screen = CLOCK_SCREEN;
      else if (menuSelection == 1) screen = STOPWATCH_SCREEN;
      else if (menuSelection == 2) screen = TIMER_SCREEN;
      else if (menuSelection == 3) { timeField = 0; screen = SET_TIME_SCREEN; }
      else if (menuSelection == 4) { dateField = 0; screen = SET_DATE_SCREEN; }
      else if (menuSelection == 5) { timerField = 0; screen = SET_TIMER_SCREEN; }
      else { alarmField = 0; screen = SET_ALARM_SCREEN; }
      break;

    case STOPWATCH_SCREEN:
      if (stopwatchRunning) {
        stopwatchRunning = false;
      } else if (!stopwatchMaxReached) {
        lastStopwatchTick = millis();
        stopwatchRunning = true;
      }
      break;

    case TIMER_SCREEN:
      if (!isTimerZero()) { timerRunning = !timerRunning; lastTimer = millis(); }
      break;

    case SET_TIME_SCREEN:
      if (++timeField > 2) { timeField = 0; screen = CLOCK_SCREEN; }
      break;

    case SET_DATE_SCREEN:
      if (++dateField > 2) { dateField = 0; screen = CLOCK_SCREEN; }
      break;

    case SET_TIMER_SCREEN:
      if (++timerField > 2) {
        timerField = 0;
        setTimerBCD(timerSetHour, timerSetMinute, timerSetSecond);
        timerRunning = false; screen = TIMER_SCREEN;
      }
      break;

    case SET_ALARM_SCREEN:
      if (++alarmField > 2) { alarmField = 0; screen = MAIN_MENU; }
      break;
  }
  oledDirty = true;
}

void changeTime(int8_t a) {
  if (timeField == 0) {
    if (a > 0) { incrementDigit(4); if(W5==0&&X5==0&&Y5==0&&Z5==0) incrementDigit(5); }
    else { decrementDigit(4); }
  } else if (timeField == 1) {
    if (a > 0) { incrementDigit(2); if(W3==0&&X3==0&&Y3==0&&Z3==0) incrementDigit(3); }
    else { decrementDigit(2); }
  } else {
    if (a > 0) { incrementDigit(0); if(W1==0&&X1==0&&Y1==0&&Z1==0) incrementDigit(1); }
    else { decrementDigit(0); }
  }
  lastClock = millis();
}

void changeDate(int8_t a) {
  if (dateField == 0) {
    int maxD = daysInMonth(month, year);
    if (a > 0) day = (day >= maxD) ? 1 : day + 1;
    else day = (day <= 1) ? maxD : day - 1;
  } else if (dateField == 1) {
    if (a > 0) month = (month >= 12) ? 1 : month + 1;
    else month = (month <= 1) ? 12 : month - 1;
    uint8_t maxD = daysInMonth(month, year);
    if (day > maxD) day = maxD;
  } else {
    if (a > 0) year++;
    else if (year > 1) year--;
    uint8_t maxD = daysInMonth(month, year);
    if (day > maxD) day = maxD;
  }
}

void changeTimer(int8_t a) {
  if (timerField == 0)
    timerSetHour = (a > 0) ? ((timerSetHour >= 99) ? 0 : timerSetHour + 1) : ((timerSetHour == 0) ? 99 : timerSetHour - 1);
  else if (timerField == 1)
    timerSetMinute = (a > 0) ? ((timerSetMinute >= 59) ? 0 : timerSetMinute + 1) : ((timerSetMinute == 0) ? 59 : timerSetMinute - 1);
  else
    timerSetSecond = (a > 0) ? ((timerSetSecond >= 59) ? 0 : timerSetSecond + 1) : ((timerSetSecond == 0) ? 59 : timerSetSecond - 1);
}

void changeAlarm(int8_t a) {
  if (alarmField == 0) {
    alarmHour = (a > 0) ? ((alarmHour >= 23) ? 0 : alarmHour + 1) : ((alarmHour == 0) ? 23 : alarmHour - 1);
  } else if (alarmField == 1) {
    alarmMinute = (a > 0) ? ((alarmMinute >= 59) ? 0 : alarmMinute + 1) : ((alarmMinute == 0) ? 59 : alarmMinute - 1);
  } else {
    alarmEnabled = !alarmEnabled;
  }
}

// ---------------- TIMERS & BUZZERS ----------------
void startBeep(BeepType type) {
  beepType = type; beepStep = 0; beepOn = false; beepTime = millis();
}

void serviceBuzzer(uint32_t now) {
  if (beepType == B_NONE) return;
  uint8_t count = 1; uint16_t frequency = 2500; uint16_t duration = 60;

  if (beepType == B_MINUTE) { frequency = 2000; duration = 80; }
  else if (beepType == B_HOUR) { count = 2; frequency = 2500; duration = 500; }
  else if (beepType == B_TIMER_MAX) { count = 3; frequency = 2500; duration = 1000; }

  if (!beepOn) {
    if (beepStep >= count) { beepType = B_NONE; return; }
    uint16_t gap = (beepType == B_TIMER_MAX) ? 500U : 300U;
    if (beepStep > 0 && now - beepTime < gap) return;
    tone(BUZZER, frequency); beepOn = true; beepTime = now; return;
  }

  if (now - beepTime >= duration) {
    noTone(BUZZER); beepOn = false; beepTime = now; beepStep++;
    if (beepStep >= count) beepType = B_NONE;
  }
}

void serviceTimerAlarm(uint32_t now) {
  if (!timerAlarm) return;
  if (timerAlarmOn) {
    if (now - timerAlarmTime >= 500UL) {
      noTone(BUZZER); timerAlarmOn = false; timerAlarmTime = now; timerAlarmBeep++;
      if (timerAlarmBeep >= 3) {
        timerAlarmBeep = 0; timerAlarmCycle++;
        if (timerAlarmCycle >= 3) {
          timerAlarm = false; displayForcedOn = false;
          if (!timerPreviousDisplayState) setDisplayEnabled(false);
          return;
        }
      }
    }
    return;
  }

  if (timerAlarmBeep == 0 && timerAlarmCycle > 0) { if (now - timerAlarmTime < 600UL) return; }
  else if (timerAlarmBeep > 0) { if (now - timerAlarmTime < 300UL) return; }

  tone(BUZZER, 2500); timerAlarmOn = true; timerAlarmTime = now;
}

void checkAlarm() {
  if (!alarmEnabled || alarmActive) return;
  if (getHours() == alarmHour && getMinutes() == alarmMinute && getSeconds() == 0) {
    alarmDisplayWasOff = !displayEnabled; displayForcedOn = true;
    alarmActive = true; alarmSnoozed = false; alarmEnd = millis() + 120000UL;
    alarmPatternStep = 0; alarmPatternTime = millis();
    beepType = B_NONE; timerAlarm = false; noTone(BUZZER); oledDirty = true;
  }
}

void stopAlarm() {
  alarmActive = false; displayForcedOn = false; alarmSnoozed = false;
  noTone(BUZZER); screen = CLOCK_SCREEN;
  if (alarmDisplayWasOff) { setDisplayEnabled(false); alarmDisplayWasOff = false; }
  oledDirty = true;
}

void snoozeAlarm() {
  uint16_t total = (uint16_t)getHours() * 60U + getMinutes() + 5U;
  total %= 1440U;
  alarmHour = total / 60U; alarmMinute = total % 60U;
  alarmEnabled = true; alarmSnoozed = true; alarmActive = false; displayForcedOn = false;
  noTone(BUZZER); screen = CLOCK_SCREEN;
  if (alarmDisplayWasOff) { setDisplayEnabled(false); alarmDisplayWasOff = false; }
  oledDirty = true;
}

void serviceAlarm(uint32_t now) {
  if (!alarmActive) return;
  if (now >= alarmEnd) { snoozeAlarm(); return; }

  switch (alarmPatternStep) {
    case 0: tone(BUZZER, 2500); alarmPatternStep = 1; alarmPatternTime = now; break;
    case 1: if (now - alarmPatternTime >= 100UL) { noTone(BUZZER); alarmPatternStep = 2; alarmPatternTime = now; } break;
    case 2: if (now - alarmPatternTime >= 100UL) { tone(BUZZER, 2500); alarmPatternStep = 3; alarmPatternTime = now; } break;
    case 3: if (now - alarmPatternTime >= 100UL) { noTone(BUZZER); alarmPatternStep = 4; alarmPatternTime = now; } break;
    case 4: if (now - alarmPatternTime >= 250UL) { tone(BUZZER, 2500); alarmPatternStep = 5; alarmPatternTime = now; } break;
    case 5: if (now - alarmPatternTime >= 100UL) { noTone(BUZZER); alarmPatternStep = 6; alarmPatternTime = now; } break;
    case 6: if (now - alarmPatternTime >= 100UL) { tone(BUZZER, 2500); alarmPatternStep = 7; alarmPatternTime = now; } break;
    case 7: if (now - alarmPatternTime >= 100UL) { noTone(BUZZER); alarmPatternStep = 0; alarmPatternTime = now; } break;
  }
}

bool leap(uint16_t y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }
uint8_t daysInMonth(uint8_t m, uint16_t y) {
  static const uint8_t d[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  return (m == 2 && leap(y)) ? 29 : d[m - 1];
}

void incrementDate() {
  if (++day > daysInMonth(month, year)) {
    day = 1;
    if (++month > 12) { month = 1; year++; }
  }
}

void forceDisplayOn() {
  displayForcedOn = true;
  for (int i = 0; i < 6; i++) digitalWrite(EN[i], LOW);
}

// ---------------- DISPLAY DRIVER FUNCTIONS ----------------
void updateSevenSegment() {
  uint8_t d[6];

  if (screen == STOPWATCH_SCREEN) {
    uint8_t c = getSWCentiseconds();
    uint8_t s = getSWSeconds();
    uint8_t m = getSWMinutes();
    
    d[0] = c % 10; d[1] = c / 10;
    d[2] = s % 10; d[3] = s / 10;
    d[4] = m % 10; d[5] = m / 10;
  } else if (screen == TIMER_SCREEN) {
    uint8_t s = getTimerSeconds();
    uint8_t m = getTimerMinutes();
    uint8_t h = getTimerHours();
    d[0] = s % 10; d[1] = s / 10;
    d[2] = m % 10; d[3] = m / 10;
    d[4] = h % 10; d[5] = h / 10;
  } else if (screen == SET_TIMER_SCREEN) {
    d[0] = timerSetSecond % 10; d[1] = timerSetSecond / 10;
    d[2] = timerSetMinute % 10; d[3] = timerSetMinute / 10;
    d[4] = timerSetHour % 10;   d[5] = timerSetHour / 10;
  } else {
    d[0] = W1 + X1*2 + Y1*4 + Z1*8;
    d[1] = W2 + X2*2 + Y2*4;
    d[2] = W3 + X3*2 + Y3*4 + Z3*8;
    d[3] = W4 + X4*2 + Y4*4;
    d[4] = W5 + X5*2 + Y5*4 + Z5*8;
    d[5] = W6 + X6*2;
  }

  publishDigits(d);
}

void print2(uint16_t n) {
  if (n < 10) display.print('0');
  display.print(n);
}

void drawAlarmActive() {
  display.setTextSize(2);
  display.setCursor(20, 0);
  display.print(F("ALARM!"));

  display.setTextSize(2);
  display.setCursor(29, 24);
  print2(alarmHour); display.print(':'); print2(alarmMinute);

  display.setTextSize(1);
  display.setCursor(0, 55);
  display.print(F("C:SNOOZE  D:STOP"));
}

void drawSetAlarm() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("SET ALARM"));

  display.setTextSize(2);
  display.setCursor(17, 16);
  print2(alarmHour); display.print(':'); print2(alarmMinute);

  display.setTextSize(1);
  display.setCursor(0, 42);
  display.print(F("EDIT: "));
  if (alarmField == 0) display.print(F("HOUR"));
  else if (alarmField == 1) display.print(F("MINUTE"));
  else {
    display.print(F("ALARM "));
    display.print(alarmEnabled ? F("ON") : F("OFF"));
  }

  display.setCursor(0, 55);
  display.print(F("B/C CHANGE  D NEXT"));
}

void drawMainMenu() {
  display.setTextSize(1);

  const char *items[] = {
    "CLOCK", "STOPWATCH", "TIMER",
    "SET TIME", "SET DATE", "SET TIMER", "SET ALARM"
  };

  for (uint8_t i = 0; i < 7; i++) {
    display.setCursor(5, i * 9);
    display.print(i == menuSelection ? F("> ") : F("  "));
    display.print(items[i]);
  }
}

void drawClock() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("CLOCK"));

  display.setTextSize(2);
  display.setCursor(17, 15);
  print2(getHours()); display.print(':');
  print2(getMinutes()); display.print(':');
  print2(getSeconds());

  display.setTextSize(1);
  display.setCursor(27, 39);
  print2(day); display.print('/');
  print2(month); display.print('/');
  display.print(year);

  display.setCursor(0, 56);
  display.print(F("A:MENU"));
}

void drawStopwatch() {
  uint8_t c = getSWCentiseconds();
  uint8_t s = getSWSeconds();
  uint8_t m = getSWMinutes();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("STOPWATCH"));

  display.setTextSize(2);
  display.setCursor(10, 16);
  print2(m); display.print(':'); print2(s);
  display.print('.'); print2(c);

  display.setTextSize(1);
  display.setCursor(0, 42);
  if (stopwatchRunning) display.print(F("RUNNING"));
  else if (getSWCentiseconds() || getSWSeconds() || getSWMinutes()) display.print(F("PAUSED"));
  else display.print(F("STOPPED"));

  display.setCursor(0, 54);
  display.print(F("A:BK B:ST C:RS D:P/R"));

  if (stopwatchMaxReached) {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);

    const char *limitLine1 = "LIMIT";
    const char *limitLine2 = "REACHED!";

    int16_t x1, y1;
    uint16_t w1, h1;
    uint16_t w2, h2;

    display.getTextBounds(limitLine1, 0, 0, &x1, &y1, &w1, &h1);
    display.getTextBounds(limitLine2, 0, 0, &x1, &y1, &w2, &h2);

    display.setCursor((128 - w1) / 2, 25);
    display.print(limitLine1);

    display.setCursor((128 - w2) / 2, 45);
    display.print(limitLine2);
  }
}

void drawTimer() {
  uint8_t h = getTimerHours();
  uint8_t m = getTimerMinutes();
  uint8_t s = getTimerSeconds();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("COUNTDOWN TIMER"));

  display.setTextSize(2);
  display.setCursor(17, 16);
  print2(h); display.print(':'); print2(m); display.print(':'); print2(s);

  display.setTextSize(1);
  display.setCursor(0, 42);
  if (timerRunning) display.print(F("RUNNING"));
  else if (isTimerZero()) display.print(F("FINISHED"));
  else display.print(F("PAUSED"));

  display.setCursor(0, 54);
  display.print(F("A BACK B+1M C-1M D"));
}

void drawSetTime() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("EDIT TIME"));

  display.setTextSize(2);
  display.setCursor(17, 16);
  print2(getHours()); display.print(':'); print2(getMinutes()); display.print(':'); print2(getSeconds());

  display.setTextSize(1);
  display.setCursor(0, 42);
  display.print(F("EDIT: "));
  if (timeField == 0) display.print(F("HOUR"));
  else if (timeField == 1) display.print(F("MINUTE"));
  else display.print(F("SECOND"));

  display.setCursor(0, 55);
  display.print(F("B/D CHANGE  D NEXT"));
}

void drawSetDate() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("EDIT DATE"));

  display.setTextSize(2);
  display.setCursor(10, 16);
  print2(day); display.print('/'); print2(month);

  display.setTextSize(1);
  display.print('/'); display.print(year);

  display.setCursor(0, 42);
  display.print(F("EDIT: "));
  if (dateField == 0) display.print(F("DAY"));
  else if (dateField == 1) display.print(F("MONTH"));
  else display.print(F("YEAR"));

  display.setCursor(0, 55);
  display.print(F("B/C CHANGE  D NEXT"));
}

void drawSetTimer() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("SET TIMER"));

  display.setTextSize(2);
  display.setCursor(17, 16);
  print2(timerSetHour); display.print(':');
  print2(timerSetMinute); display.print(':');
  print2(timerSetSecond);

  display.setTextSize(1);
  display.setCursor(0, 42);
  display.print(F("EDIT: "));
  if (timerField == 0) display.print(F("HOUR"));
  else if (timerField == 1) display.print(F("MINUTE"));
  else display.print(F("SECOND"));

  display.setCursor(0, 55);
  display.print(F("B/C CHANGE  D NEXT"));
}

void updateOLED(uint32_t now) {
  if (!displayEnabled && !alarmActive && !timerAlarm) return;
  if (!oledDirty && now - lastOLED < 10UL) return;

  lastOLED = now; oledDirty = false;
  display.clearDisplay();

  if (alarmActive) { drawAlarmActive(); display.display(); return; }
  if (timerAlarm) { drawTimer(); display.display(); return; }

  switch (screen) {
    case MAIN_MENU: drawMainMenu(); break;
    case CLOCK_SCREEN: drawClock(); break;
    case STOPWATCH_SCREEN: drawStopwatch(); break;
    case TIMER_SCREEN: drawTimer(); break;
    case SET_TIME_SCREEN: drawSetTime(); break;
    case SET_DATE_SCREEN: drawSetDate(); break;
    case SET_TIMER_SCREEN: drawSetTimer(); break;
    case SET_ALARM_SCREEN: drawSetAlarm(); break;
  }
  display.display();
}
