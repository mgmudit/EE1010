//Functions created by
// G V V Sharma
// October 29, 2023
// Revised October 31, 2023
// Revised November 1, 2023
// Revised November 21, 2023
// Revised November 24, 2023

//Function declaration

avyuh *Listeigvec(avyuh *a, avyuh *lam);//eigenvector matrix 2x2
avyuh *Listunit(avyuh *a);//unit vector
avyuh *Listrow(avyuh *a, int k);// kth row
double Listdet(avyuh *a);//determinant of a 2x2 matrix
avyuh *Listeigval(avyuh *a);//eigenvalues of a 2x2 matrix
double Listrace(avyuh *c);//matrix trace
avyuh *Listeye(int k);//identity matrix
avyuh *Listbasis(int k);//standard basis vector of length k
avyuh *Listquad(double a,double b, double c);//roots of a quadratic equation
double Listnorm(avyuh *a);//norm of a vector
double Listdot(avyuh *a, avyuh * b);//inner product
double ListVecdot(sadish *a, sadish * b);//inner product
avyuh *Listsub(avyuh *a, avyuh *b);//subtract two matrices
avyuh *Listadd(avyuh *a, avyuh *b);//add two matrices
sadish *ListVecadd(sadish *a, sadish *b);//add two matrices
sadish *ListVecShift(sadish *a);//circulalry right shift vector
sadish *ListVecsub(sadish *a, sadish *b);//subtract two vectors
avyuh *Listscale(avyuh *a, double k);//scale matrix
sadish *Listvecscale(sadish *a, double k);//scale vector
avyuh *Listinv(avyuh *mat, int m);//invert an m  x m matrix, m <=3
avyuh *Listmul(avyuh *a, avyuh *b);//multiply matrices a and b
avyuh *rotList(double theta); //rotation matrix
avyuh *normVec(avyuh *a); //normal vector
avyuh *circulantList(avyuh *c);//circulant matrix
avyuh *Listsec(avyuh *a, avyuh * b,double k);//section formula
//End function declaration


//add two matrices
avyuh *Listadd(avyuh *a, avyuh *b){
	avyuh *c= (avyuh *)malloc(sizeof(avyuh)), *tempb = b, *head; 
	c->next = NULL;
	head = c;
	 for(avyuh *tempa=a;tempa!=NULL;tempa=tempa->next){
                c->vector = ListVecadd(tempa->vector,tempb->vector);
                tempb = tempb->next;
	if(tempa->next!=NULL){
		c->next = (avyuh *)malloc(sizeof(avyuh));
		c->next->next=NULL;
		c= c->next;
	}
	}
	return head;
}
//Sum of vectors
sadish *ListVecadd(sadish *a, sadish *b){
	sadish *head = (sadish *)malloc(sizeof(sadish)), *c, *tempa=a, *tempb=b;
	c = head; 
	head->next = NULL;
	for(sadish *tempa=a;tempa!=NULL;tempa=tempa->next){
                c->data = tempa->data+tempb->data;
                //tempa = tempa->next;
                tempb = tempb->next;
	if(tempa->next!=NULL){
		c->next = (sadish *)malloc(sizeof(sadish));
		c->next->next=NULL;
		c= c->next;
	}
	}
	return head;
}
//inner product
double ListVecdot(sadish *a, sadish *b){
	double val = 0;
	sadish *tempa=a, *tempb=b;
	while(tempa !=NULL){
		val += tempa->data*tempb->data;
		tempa = tempa->next;
		tempb = tempb->next;
	}
	return val;
}
double Listdot(avyuh *a, avyuh *b){
	return ListVecdot(a->vector, b->vector);
}
sadish *ListVecsub(sadish *a, sadish *b){
	sadish *head = (sadish *)malloc(sizeof(sadish)), *c, *tempb=b;
	c = head; 
	head->next = NULL;
	for(sadish *tempa=a;tempa!=NULL;tempa=tempa->next){
		c->data = tempa->data-tempb->data;
		tempb = tempb->next;
	if(tempa->next!=NULL){
		c->next = (sadish *)malloc(sizeof(sadish));
		c->next->next=NULL;
		c= c->next;
	}
	}
	return head;
}

//subtract two matrices
avyuh *Listsub(avyuh *a, avyuh *b){
	avyuh *c= (avyuh *)malloc(sizeof(avyuh)),  *tempb = b, *head; 
	c->next = NULL;
	head = c;
	for(avyuh *tempa=a;tempa!=NULL;tempa=tempa->next){
		c->vector = ListVecsub(tempa->vector,tempb->vector);
		tempb = tempb->next;
	if(tempa->next!=NULL){
		c->next = (avyuh *)malloc(sizeof(avyuh));
		c->next->next=NULL;
		c= c->next;
	}
	}
	return head;
}

//norm of a vector
double Listnorm(avyuh *a){
	return sqrt(Listdot(a,a));
}

//rotation matrix
avyuh *rotList(double theta){ 
avyuh *head = createList(2,2), *temp;//create empty 2 x 2 matrix 
sadish *row1, *row2;
double c = cos(theta), s = sin(theta);

row1 = head->vector;
row2 = head->next->vector;
row1->data = c;
row1->next->data = -s;
row2->data = s;
row2->next->data = c;
return head;

}

//Matrix multiplication
avyuh *Listmul(avyuh *a, avyuh *b){
	avyuh *c=(avyuh *)malloc(sizeof(avyuh)),*atemp, *btemp;
	avyuh *head = c;
	sadish *cvec=(sadish *)malloc(sizeof(sadish));
	head->next = NULL;
	for(atemp=a; atemp !=NULL; atemp=atemp->next){
		cvec=(sadish *)malloc(sizeof(sadish));
		cvec->next = NULL;
		c->vector = cvec;  
		for(btemp=b; btemp !=NULL; btemp=btemp->next){
			cvec->data=ListVecdot(atemp->vector, btemp->vector);//inner product
		if(btemp->next!=NULL){
			cvec->next = (sadish *)malloc(sizeof(sadish));
			cvec->next->next=NULL;
			cvec= cvec->next;
		}
		}

		if(atemp->next!=NULL){
			c->next = (avyuh *)malloc(sizeof(avyuh));
			c->next->next=NULL;
			c= c->next;
		}
	}
return head;
}
//scale vector
sadish *Listvecscale(sadish *a, double k){
	sadish *head = (sadish *)malloc(sizeof(sadish)), *c;
	c = head; 
	head->next = NULL;
	for(sadish *tempa=a;tempa!=NULL;tempa=tempa->next){
		c->data = k*tempa->data;
	if(tempa->next!=NULL){
		c->next = (sadish *)malloc(sizeof(sadish));
		c->next->next=NULL;
		c= c->next;
	}
	}
	return head;
}
//scale matrix
avyuh *Listscale(avyuh *a, double k){
	avyuh *head = (avyuh *)malloc(sizeof(avyuh)), *c;
	c = head; 
	head->next = NULL;
	for(avyuh *tempa=a;tempa!=NULL;tempa=tempa->next){
		c->vector = Listvecscale(tempa->vector, k);
		//c->vector = Listvecscale(a->vector, k);
	if(tempa->next!=NULL){
		c->next = (avyuh *)malloc(sizeof(avyuh));
		c->next->next=NULL;
		c= c->next;
	}
	}
	return head;
}
//circulalry right shift vector
sadish *ListVecShift(sadish *a){
	sadish *tempa, *temp;
	sadish *head = ListVecopy(a);
	for(temp=head;temp->next->next!=NULL;temp=temp->next);
temp->next->next = head;
tempa = temp->next;
temp->next = NULL;
return tempa;
}
//circulant matrix
avyuh *circulantList(avyuh *a){
	avyuh *c=(avyuh *)malloc(sizeof(avyuh));
	avyuh *head = c;
	sadish *ctemp;
	ctemp=a->vector;
	head->vector=a->vector;
	for(sadish *temp=a->vector;temp->next!=NULL;temp=temp->next){
			c->next = (avyuh *)malloc(sizeof(avyuh));
			c->next->next=NULL;
			c= c->next;
		c->vector = ListVecShift(ctemp);
		ctemp=c->vector;
	}
	return head;
}
//roots of a quadratic equation
avyuh *Listquad(double a,double b, double c){
	avyuh *head=createList(1,2);//create 1 x 2 matrix 
	sadish *lam=head->vector;
	double D = sqrt(pow(b,2.0)-4*a*c);
	double den =2.0*a; 
lam->data = (-b+D)/den;
lam = lam->next; 
lam->data = (-b-D)/den;
return head;
}
//section formula for vectors
sadish *ListVecSec(sadish *a, sadish *b,double k){
	sadish *head=(sadish *)malloc(sizeof(sadish)), *tempb;
	head->next = NULL;
	for(sadish *c = head;a!=NULL;a=a->next){
		c->data= (k*b->data+a->data)/(k+1);
		b=b->next;
	if(a->next!=NULL){
		c->next = (sadish *)malloc(sizeof(sadish));
		c->next->next=NULL;
		c= c->next;
	}
	}
	return head;
}
//section formula for matrices
avyuh *Listsec(avyuh *a, avyuh *b, double k){
	avyuh *c= (avyuh *)malloc(sizeof(avyuh)), *tempb = b, *head; 
	c->next = NULL;
	head = c;
	 for(avyuh *tempa=a;tempa!=NULL;tempa=tempa->next){
                c->vector = ListVecSec(tempa->vector,tempb->vector, k);
                tempb = tempb->next;
	if(tempa->next!=NULL){
		c->next = (avyuh *)malloc(sizeof(avyuh));
		c->next->next=NULL;
		c= c->next;
	}
	}
	 return head;
}
//standard basis vector of length k
avyuh *Listbasis(int k){
	avyuh *head=(avyuh *)malloc(sizeof(avyuh));
	sadish *c = createVec(k);
	head->vector = c;
	head->next = NULL;
	for(int i=0; i < k; i++){
		c->data= 0;
		c= c->next;
	}
	head->vector->data = 1;
	return head;
}
//identity matrix
avyuh *Listeye(int k){
	avyuh *c=Listbasis(k);
return circulantList(c);
}
//matrix trace
double Listrace(avyuh *c){
	double temp=0;
	sadish *vec;
	for(int i=0;c!=NULL;c=c->next,i++){
		vec = c->vector;
		for(int k = 0; k < i; k++){
			vec = vec->next;
		}
			temp +=vec->data;
	}
	return temp;
}
//determinant of a 2x2 matrix
double Listdet(avyuh *a){
	double temp1, temp2;	
	temp1 = a->vector->data;
	temp2 = a->vector->next->data;
	a = a->next;
	temp2 = a->vector->data*temp2;
	temp1 = a->vector->next->data*temp1;
return temp1-temp2;
}
//eigenvalues of a 2x2 matrix
avyuh *Listeigval(avyuh *a){
	double b = -Listrace(a);
	double c = Listdet(a);
	return Listquad(1,b,c);
}
/*
//eigenvector matrix for a 2x2 matrix
avyuh *Listeigvec(avyuh *a, avyuh *lam){
//void Listeigvec(avyuh *a, avyuh *lam){
	avyuh *b1, *b2;
	avyuh *p1, *p2;
	avyuh *temp1, *temp2;
	avyuh *omat=rotList(M_PI/2);
	//A-lambda I
	b1 = Listadd(a,Listscale(Listeye(2),-lam->vector->data));
	b2 = Listadd(a,Listscale(Listeye(2),-lam->vector->next->data));
	//Extract 1st row
	temp1 = Listunit(Listrow(b1, 0));
	temp2 = Listunit(Listrow(b2, 0));
	//Find eigen vector
	temp1 = transposeList(Listmul(omat, temp1));
	temp2 = transposeList(Listmul(omat, temp2));
	return Listvstack(temp1,temp2);
}*/
// kth row
avyuh *Listrow(avyuh *a, int k){
	avyuh *c= (avyuh *)malloc(sizeof(avyuh));
	c->next = NULL;
	for(int i = 0; i <k; i++){
		a = a->next;
	}
	c->vector = ListVecopy(a->vector);
	return c;
}
//unit vector
avyuh *Listunit(avyuh *a){
	double k= Listnorm(a);
	avyuh *c=Listscale(a, 1/k);//scale vector
	return c;
}
