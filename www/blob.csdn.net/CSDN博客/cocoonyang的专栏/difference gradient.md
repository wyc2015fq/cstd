
# difference gradient - cocoonyang的专栏 - CSDN博客


2017年11月24日 11:46:49[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：128



```python
#include <math.h>
```
```python
#include <stdlib.h>
```
```python
#include <stdio.h>
```
```python
void
```
```python
show_vector(
```
```python
float
```
```python
*A,
```
```python
int
```
```python
n) {
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < n; i++) {
```
```python
printf
```
```python
(
```
```python
"%2.5f \t"
```
```python
, A[i]);
    }
```
```python
printf
```
```python
(
```
```python
"\n"
```
```python
);
}
```
```python
float
```
```python
func(
```
```python
float
```
```python
* x )
{
```
```python
float
```
```python
result =
```
```python
0.0
```
```python
;
    result = x[
```
```python
0
```
```python
] * x[
```
```python
0
```
```python
] * x[
```
```python
0
```
```python
] + x[
```
```python
1
```
```python
] *x[
```
```python
1
```
```python
];
```
```python
return
```
```python
result;
}
```
```python
/*
  Calculating gradient of a function with different methods
*/
```
```python
void
```
```python
forward()
{
```
```python
int
```
```python
n =
```
```python
2
```
```python
;
```
```python
float
```
```python
x[] = {
```
```python
0
```
```python
,
```
```python
0
```
```python
};
```
```python
float
```
```python
pf[] = {
```
```python
0
```
```python
,
```
```python
0
```
```python
};
```
```python
float
```
```python
dx =
```
```python
0.001
```
```python
;
```
```python
float
```
```python
f = func( x );
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < n; i++ )
   {
       x[i] += dx;
```
```python
float
```
```python
newF = func( x );
```
```python
float
```
```python
diff = newF - f;
       pf[i] = diff/dx;
       x[i] -= dx; 
   }
```
```python
printf
```
```python
(
```
```python
"forward pf =  \n"
```
```python
);
   show_vector( pf, n );
}
```
```python
void
```
```python
twoside()
{
```
```python
int
```
```python
n =
```
```python
2
```
```python
;
```
```python
float
```
```python
x[] = {
```
```python
0
```
```python
,
```
```python
0
```
```python
};
```
```python
float
```
```python
pf[] = {
```
```python
0
```
```python
,
```
```python
0
```
```python
};
```
```python
float
```
```python
dx =
```
```python
0.01
```
```python
;
```
```python
float
```
```python
f = func( x );
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < n; i++ )
   {
       x[i] += dx;
```
```python
float
```
```python
F_forward = func( x );
       x[i] -=
```
```python
2
```
```python
*dx;
```
```python
float
```
```python
F_backward = func( x );
       x[i] += dx;
```
```python
float
```
```python
diff = F_forward - F_backward ;
       pf[i] = diff/(
```
```python
2
```
```python
*dx);
   }
```
```python
printf
```
```python
(
```
```python
"two side pf =  \n"
```
```python
);
   show_vector( pf, n );
}
```
```python
void
```
```python
fourthorder()
{
```
```python
int
```
```python
n =
```
```python
2
```
```python
;
```
```python
float
```
```python
x[] = {
```
```python
0
```
```python
,
```
```python
0
```
```python
};
```
```python
float
```
```python
pf[] = {
```
```python
0
```
```python
,
```
```python
0
```
```python
};
```
```python
float
```
```python
dx =
```
```python
0.01
```
```python
;
```
```python
float
```
```python
f = func( x );
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < n; i++ )
   {
```
```python
float
```
```python
x0 = x[i];
       x[i] = x0 + dx;
```
```python
float
```
```python
F_forward_1 = func( x );
       x[i] = x0 +
```
```python
2
```
```python
*dx;
```
```python
float
```
```python
F_forward_2 = func( x );
       x[i] = x0 - dx;
```
```python
float
```
```python
F_backward_1 = func( x );
       x[i] = x0 -
```
```python
2
```
```python
*dx;
```
```python
float
```
```python
F_backward_2 = func( x );
       x[i] = x0;
```
```python
float
```
```python
diff =
```
```python
2
```
```python
* F_forward_2 +
```
```python
16
```
```python
*F_forward_1 -
```
```python
16
```
```python
F_backward_1 -
```
```python
2
```
```python
* F_backward_2 ;
       pf[i] = diff/(
```
```python
4
```
```python
*dx);
   }
```
```python
printf
```
```python
(
```
```python
"fourth order pf =  \n"
```
```python
);
   show_vector( pf, n );   
}
```
```python
void
```
```python
main()
{
    forward();
    twoside();
    fourthorder();
```
```python
return
```
```python
;
}
```

