//Functions created by
// G V V Sharma
// October 27, 2023
// Revised November 27, 2023

//vector data structure
typedef struct list
{
double data;
struct list *next;
}sadish;

//matrix data structure
typedef struct tree
{	
sadish *vector;
struct tree *next;
}avyuh;

//Function declaration
sadish *ListVecopy(sadish *a);//copy vector
sadish *Vecind(sadish *a,int n);
void printVec(sadish *head);
sadish *loadVec(FILE *fp, int n);
avyuh *createList(int m,int n);//create m x n matrix array
void printList(avyuh *p);//print matrix
avyuh *loadList(char *str,int m,int n);//load matrix from file
avyuh *Listcol(avyuh *a, int n);//extract nth column of matrix
sadish *colVec(avyuh *a, int n);//extract nth column of matrix
avyuh *transposeList(avyuh *a);//transpose of a
//End function declaration

//Matrix transpose 
avyuh *transposeList(avyuh *a){
	int i=0;//dummy integer
	avyuh *head =(avyuh *)malloc(sizeof(avyuh)); 
	avyuh *b=head;
	sadish *temp, *bvec;
	head->next = NULL;

//extract column vector
		
	for (temp= a->vector; temp!=NULL;  temp= temp->next){
		bvec= colVec(a,i);
		b->vector = bvec;
		i++;
		if(temp->next !=NULL){
		b->next = (avyuh *)malloc(sizeof(avyuh));
		b->next->next=NULL;
		b= b->next;
	}

	}
return head;
	}

//create vector
sadish *createVec( int n)
{

int i =0;//dummy integer
sadish *head,*temp;//head of the array
head = (sadish *)malloc(sizeof(sadish));
head->next = NULL;
temp  = head;
for (i=0; i < n; i++)
{
	if (i< n-1){
temp->next = (sadish *)malloc(sizeof(sadish));
temp->next->next= NULL;
temp  = temp->next;
	}
}

 return head;

}

//create matrix
avyuh *createList(int m,int n)
{
	avyuh *a, *temp;//matrix head
	int i=0;//dummy integer
a = (avyuh *)malloc(sizeof(avyuh));
a->next = NULL;
temp  = a;
for (i = 0; i < m; i++)
{
	temp->vector = createVec(n);
	if (i< m-1){
	temp->next = (avyuh *)malloc(sizeof(avyuh));
	temp->next->next = NULL; 
	temp = temp->next; 
	}
}
 
 return a;
}

//Extract address from vector
sadish *Vecind(sadish *a,int n){
	sadish *temp=a;
	int i=0;//dummy integer
	for (i=0; i < n; i++){
		temp = temp->next;
	}
	return temp;
}

//Extract column from matrix
avyuh *Listcol(avyuh *a, int n){
	avyuh *head= (avyuh *)malloc(sizeof(avyuh)), *alist;
	head->vector= colVec(a,n);
	head->next= NULL;
return head;
}
sadish *colVec(avyuh *a, int n){
	int i = 0,j = 0;//dummy integers
	int m = 2;
	//avyuh *head= (avyuh *)malloc(sizeof(avyuh)), *alist;
	sadish *head=(sadish *)malloc(sizeof(sadish)),*temp, *btemp;
	btemp = head;
	head->next= NULL;
	for (avyuh *alist = a; alist != NULL; alist= alist->next){
		temp = Vecind(alist->vector,n);//getting address of the nth column
		btemp->data = temp->data;
	if(alist->next !=NULL){
		btemp->next = (sadish *)malloc(sizeof(sadish));
		btemp->next->next=NULL;
		btemp= btemp->next;
	}
	}
return head;
}

//load matrix from file
avyuh *loadList(char *str, int m, int n){
	avyuh *head, *temp;//matrix head
	int i=0;//dummy integer
	FILE *fp;
fp = fopen(str, "r");//open file
head = (avyuh *)malloc(sizeof(avyuh));
head->next = NULL;
temp  = head;
for (i = 0; i < m; i++)
{
	temp->vector = loadVec(fp, n);
	if (i< m-1){
	temp->next = (avyuh *)malloc(sizeof(avyuh));
	temp->next->next = NULL; 
	temp = temp->next; 
	}
}
fclose(fp);
return head;
}

//load vector from file
sadish *loadVec(FILE *fp, int n)
{

int i =0;//dummy integer
double val;//for reading file data
sadish *head,*temp;//head of the array
head = (sadish *)malloc(sizeof(sadish));
temp  = head;
head->next = NULL;
for (i=0; i < n; i++)
{
fscanf(fp,"%lf",&temp->data);
	if (i< n-1){
temp->next = (sadish *)malloc(sizeof(sadish));
temp->next->next= NULL;
temp  = temp->next;
	}
}

 return head;

}
//Function for printing an array
void printVec(sadish *head)
{
	for (sadish *temp=head; temp !=NULL; temp= temp->next){
		printf("%lf ",temp->data);
	}
	printf("\n");
}

void printList(avyuh *head)
{
	for (avyuh *temp=head; temp !=NULL; temp= temp->next){
		printVec(temp->vector);
	fflush(stdout);
    }
}
//copy a vector
sadish *ListVecopy(sadish *a){
	sadish *c= (sadish *)malloc(sizeof(sadish)), *head;
	head = c; 
	head->next = NULL;
	for(sadish *tempa=a;tempa!=NULL;tempa=tempa->next){
                c->data = tempa->data;
		if(tempa->next!=NULL){
			c->next = (sadish *)malloc(sizeof(sadish));
			c->next->next=NULL;
			c= c->next;
		}
	}
	return head;
}
