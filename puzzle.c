#include <stdio.h>


void print(int * a, int b){
   for(int i=0;i<5;i++){
     printf("%d,",a[i]);
  }
  printf("=%d\n",b);
}
int test(int* a){
  int d= (a[0]+(a[1]*a[2]*a[2]+a[3]*a[3]*a[3])-a[4]);
  return d;
}
void main()
{
  int x[]={2,3,5,7,9};

  int y[5];
  int a;
  for(int g=0;g<5;g++){
  for(int h=0;h<5;h++){
  if (g==h) continue;
  for(int i=0;i<5;i++){
  if (i==h||i==g) continue;
  for(int j=0;j<5;j++){
  if (j==h||j==g||j==i) continue;
  for(int k=0;k<5;k++){
  if (k==h||k==g||k==i||k==j) continue;
   y[0]=x[g];
   y[1]=x[h];
   y[2]=x[i];
   y[3]=x[j];
   y[4]=x[k];
  a=test(y);
  if (a==399)print(y,a);
  }
  }
  }
  }
  }
}
