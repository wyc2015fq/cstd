# difference gradient - cocoonyang的专栏 - CSDN博客





2017年11月24日 11:46:49[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：130








```cpp
#include <math.h> 
#include <stdlib.h>
#include <stdio.h>

void show_vector(float *A, int n) {
    for (int i = 0; i < n; i++) {
            printf("%2.5f \t", A[i]);
    }
    printf("\n");
}

float func( float* x )
{
    float result = 0.0;
    result = x[0] * x[0] * x[0] + x[1] *x[1];
    return result;
}

/*
  Calculating gradient of a function with different methods
*/
void forward()
{
   int n = 2;

   float x[] = { 0, 0};
   float pf[] = { 0, 0};
   float dx = 0.001;
   float f = func( x );


   for( int i = 0; i < n; i++ )
   {
       x[i] += dx;
       float newF = func( x );
       float diff = newF - f;
       pf[i] = diff/dx;
       x[i] -= dx; 
   }
   printf( "forward pf =  \n" );
   show_vector( pf, n );

}
void twoside()
{
   int n = 2;

   float x[] = { 0, 0};
   float pf[] = { 0, 0};
   float dx = 0.01;
   float f = func( x );


   for( int i = 0; i < n; i++ )
   {
       x[i] += dx;
       float F_forward = func( x );
       x[i] -= 2*dx; 
       float F_backward = func( x );
       x[i] += dx;
       float diff = F_forward - F_backward ;
       pf[i] = diff/(2*dx);
   }
   printf( "two side pf =  \n" );
   show_vector( pf, n );
}
void fourthorder()
{
   int n = 2;

   float x[] = { 0, 0};
   float pf[] = { 0, 0};
   float dx = 0.01;
   float f = func( x );


   for( int i = 0; i < n; i++ )
   {
       float x0 = x[i];
       x[i] = x0 + dx;
       float F_forward_1 = func( x );

       x[i] = x0 + 2*dx;
       float F_forward_2 = func( x );

       x[i] = x0 - dx; 
       float F_backward_1 = func( x );

       x[i] = x0 - 2*dx; 
       float F_backward_2 = func( x );
       x[i] = x0;

       float diff = 2* F_forward_2 + 16*F_forward_1 - 16 F_backward_1 - 2* F_backward_2 ;
       pf[i] = diff/(4*dx);
   }
   printf( "fourth order pf =  \n" );
   show_vector( pf, n );   
}
void main()
{
    forward();
    twoside();
    fourthorder();
    return;
}
```



