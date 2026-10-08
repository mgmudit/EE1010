
//read vector from file
node *array(char *str, int *n)
{

FILE *fp; //file pointer
double val;//for reading file data
node *head, *temp;//head of the array

fp = fopen(str, "r");//open file
*n=0;
head = (node *)malloc(sizeof(node));
temp = head;

while(fscanf(fp,"%lf",&temp->data) != EOF)
{
temp->next=(node *)malloc(sizeof(node));
temp->next->next = NULL;
temp  = temp->next;
++*n;
}

fclose(fp);
 return head;

}
