#Code by Mudit
#Date: 21/09/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess

# Given
tau = 40
V0 = 1

# Time values
t = np.linspace(0, 200, 1000)

# RC step response
Vc = V0 * (1 - np.exp(-t / tau))

# 95% response
t_95 = -tau * np.log(0.05)
Vc_95 = 0.95 * V0

# Plot
plt.plot(t, Vc, label=r"$V_C(t)=V_0(1-e^{-t/\tau})$")

plt.scatter(t_95, Vc_95, s=60,
            label=f"95% point: t = {t_95:.2f} s")

plt.axhline(Vc_95, linestyle="--")
plt.axvline(t_95, linestyle="--")

plt.xlabel("Time (s)")
plt.ylabel("Capacitor Voltage ($V_0$)")
plt.title("RC Circuit Step Response")
plt.grid(True)
plt.legend()

# Save as PDF
filename = "rc_step_response.pdf"
plt.savefig(filename, format="pdf", bbox_inches="tight")

# Automatically open the PDF in Android
subprocess.run(["termux-open", filename])

# Print answer
print(f"95% response time = {t_95:.2f} s")
print(f"Rounded answer = {round(t_95)} s")
