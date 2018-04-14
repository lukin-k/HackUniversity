
#include <stdio.h>
#include <math.h>
void pika (int n)
{
	int i;
float k;

float p;
float N=8;
//scanf("k-->%f",&k);
//scanf("%d",&n);
//scanf("p-->%f",&p);
float pika[8]={4.5,5,6,3.3,4,8,1,2};

for(i=0;i<N;i++)
{
	
p+=pika[i]*exp(((2*M_PI*n*i)/N)*(-1));
//printf("(%f)",p);
}
p*=(1.0/8);
printf("%f",p);
}
void main (void)
{
	
int i=0,n=8;
for(i=0;i<n;i++)
{
	printf("\tC%d --> ",i);
	pika(i);
	printf("\n");
}

}