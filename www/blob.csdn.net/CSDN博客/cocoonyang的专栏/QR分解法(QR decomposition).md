
# QR分解法(QR decomposition) - cocoonyang的专栏 - CSDN博客


2017年11月19日 10:08:09[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：2265


QR decomposition divides a m by n matrix$A$into a product of an orthogonal matrix$Q$and an upper triangular matrix$R$:

$$
A = Q R
$$
Thus

$$
Ax = b => QRx = b => Q^{T}QRx = Q^{T}b => Rx = Q^{T}b
$$
QR decomposition can be implemented by several algorithms, such as Gram–Schmidt process, Householder transformations, or Givens rotations. Gram-Schmidt procedure is a sequence of multiplications of A from the right by upper triangular matrices. Householde decomposition$A$into$QR$with orthogonal matrices.  As orthogonal transformations are stable, using Householder triangularization and back-substitution to slove$Ax = b$is backward stable[9][10].
## Householder transformation
Householder transformation reflects a vector$u$about a hyperplane which orthogonal to a vector$v$which is called Householder vector[8].

$$
u^{f} = u - 2vv^{T}u
$$
The basic idea of Householder reflection for QR decomposition is to find a linear transformation that changes vector$u$into a vector which collinear to$e_{i}$,$u^{f} = ||u||e_{i}$, thus all but one entries of the vector$u$can be eliminated.  It means that finding a hyperplane that bisect$u$and$||u||e_{i}$. The orthogonal vector of the hyperplane is:

$$
v = u - ||u||e_{i}
$$
The transformation matrix is:

$$
H = I - 2 \frac{vv^{T}}{v^{T}v}
$$
Householder QR decomposition code :
```python
# include <math.h>
```
```python
# include <stdio.h>
```
```python
# include <stdlib.h>
```
```python
void
```
```python
showVector(
```
```python
float
```
```python
* A,
```
```python
int
```
```python
n)
{
```
```python
// Checking function parameters
```
```python
if
```
```python
((null == A) || (n <
```
```python
0
```
```python
))
    {
```
```python
return
```
```python
;
    }
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
; i < n; i++)
    {
```
```python
printf
```
```python
(
```
```python
"%2.5f "
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
void
```
```python
showMatrix(
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
m,
```
```python
int
```
```python
n)
{
```
```python
// Checking function parameters
```
```python
if
```
```python
((null == A) || (m <
```
```python
0
```
```python
) || (n <
```
```python
0
```
```python
))
    {
```
```python
return
```
```python
;
    }
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
; i < m; i++)
    {
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
j =
```
```python
0
```
```python
; j < n; j++)
```
```python
printf
```
```python
(
```
```python
"%2.5f "
```
```python
, A[i * n + j]);
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
}
```
```python
/*****************************
 * Householder transformation function 
 *  inputs:
 *      A - m by n matrix
 *      m - row of matrix A 
 *      n - column of matrix A
 *      offset - the offset 
 *      b - the left side of the linear functions
 *****************************/
```
```python
void
```
```python
h(
```
```python
float
```
```python
* A,
```
```python
int
```
```python
m,
```
```python
int
```
```python
n,
```
```python
int
```
```python
offset,
```
```python
float
```
```python
* b )
{
```
```python
// Checking function parameters
```
```python
if
```
```python
((null == A) || (null == b) || (m <
```
```python
0
```
```python
) || (n <
```
```python
0
```
```python
))
    {
```
```python
return
```
```python
;
    }
```
```python
//  A -- m by n matrix
```
```python
//       m >= n
```
```python
int
```
```python
len = m - offset;
```
```python
if
```
```python
(( n - offset ) <=
```
```python
1
```
```python
)
    {
```
```python
return
```
```python
;
    }
```
```python
// Temporary vector pointers
```
```python
float
```
```python
* x = null;
```
```python
float
```
```python
* u = null;
```
```python
float
```
```python
* I = null;
```
```python
float
```
```python
* A1 = null;
```
```python
float
```
```python
* newB = null;
    x = (
```
```python
float
```
```python
*)
```
```python
malloc
```
```python
(len *
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
));
```
```python
if
```
```python
(null == x)
    {
```
```python
goto
```
```python
CLEAN;
    }
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
; i < len; i++)
    {
        x[i] = A[(offset + i) * n + offset];
    }
```
```python
// Logging
```
```python
printf
```
```python
(
```
```python
"x = "
```
```python
);
    showVector(x, len);
```
```python
float
```
```python
xNormal = snrm2(len, x,
```
```python
1
```
```python
);
    u = (
```
```python
float
```
```python
*)
```
```python
malloc
```
```python
(len *
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
));
```
```python
if
```
```python
(null == u)
    {
```
```python
goto
```
```python
CLEAN;
    }
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
; i < len; i++)
    {
        u[i] =
```
```python
0.0
```
```python
;
    }
    u[
```
```python
0
```
```python
] =
```
```python
pow
```
```python
(-
```
```python
1
```
```python
, offset) *xNormal;
```
```python
// Logging
```
```python
printf
```
```python
(
```
```python
"||x||e = "
```
```python
);
    showVector(u, len);
    saxpy(len,
```
```python
1
```
```python
, x,
```
```python
1
```
```python
, u,
```
```python
1
```
```python
);
```
```python
// Logging
```
```python
printf
```
```python
(
```
```python
"v = x - ||x||e = "
```
```python
);
    showVector(u, len);
```
```python
float
```
```python
vNormal = snrm2(len, u,
```
```python
1
```
```python
);
    vNormal =  vNormal * vNormal;
```
```python
// Logging
```
```python
printf
```
```python
(
```
```python
"c = %f \n"
```
```python
,
```
```python
2
```
```python
/vNormal);
    I = (
```
```python
float
```
```python
*)
```
```python
malloc
```
```python
(len * len *
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
));
```
```python
if
```
```python
(null == I)
    {
```
```python
goto
```
```python
CLEAN;
    }
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
; i < len * len; i++)
    {
        I[i] =
```
```python
0.0
```
```python
;
    }
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
; i < len; i++)
    {
        I[i * len + i] =
```
```python
1.0
```
```python
;
    }
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
; i < len  ; i++)
    {
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
j =
```
```python
0
```
```python
; j < len; j++)
        {
            I[i * len + j] -=
```
```python
2
```
```python
* u[i] * u[j] / vNormal;
        }
    }
```
```python
// Logging
```
```python
printf
```
```python
(
```
```python
"I - c vvT = \n"
```
```python
);
    showMatrix(I, len);
    A1 = (
```
```python
float
```
```python
*)
```
```python
malloc
```
```python
((m-offset) * (n-offset) *
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
));
```
```python
if
```
```python
(null == Al)
    {
```
```python
goto
```
```python
CLEAN;
    }
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
; i < m-offset; i++)
    {
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
j =
```
```python
0
```
```python
; j < n-offset; j++)
        {
            A1[i*(n-offset)+j] =
```
```python
0.0
```
```python
;
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
k =
```
```python
0
```
```python
; k < len; k++)
            {
                A1[i*(n-offset)+j] += I[i*(m-offset) + k]* A[(k+offset)*(n)+j+offset];
            }
        }
    }
```
```python
// Logging
```
```python
printf
```
```python
(
```
```python
"A1  = \n"
```
```python
);
    showMatrix(A1, (m-offset), (n-offset));
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
i = offset; i < m; i++)
    {
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
j = offset; j < n; j++)
        {
            A[i*n+j]  = A1[(i-offset)*(n-offset)+(j-offset)];
        }
    }
    newB = (
```
```python
float
```
```python
*)
```
```python
malloc
```
```python
( m*
```
```python
sizeof
```
```python
(
```
```python
float
```
```python
))
```
```python
if
```
```python
(null == newB)
    {
```
```python
goto
```
```python
CLEAN;
    }
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
; i < m; i++ )
    {
        newB[i] =
```
```python
0.0
```
```python
;
    }
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
; i < len; i++ )
    {
```
```python
float
```
```python
tmp =
```
```python
0.0
```
```python
;
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
j =
```
```python
0
```
```python
; j < len; j++ )
      {
             tmp += I[i * len + j] * b[j+offset];
      }
     newB[i] = tmp;
    }
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
; i < len; i++ )
    {
        b[i+offset] = newB[i];
    }
```
```python
// Clean temporary memory
```
```python
CLEAN:
```
```python
free
```
```python
( newB );
    newB =
```
```python
0
```
```python
;
```
```python
free
```
```python
( A1 );
    A1 =
```
```python
0
```
```python
;
```
```python
free
```
```python
( I );
    I =
```
```python
0
```
```python
;
```
```python
free
```
```python
( u );
    u =
```
```python
0
```
```python
;
```
```python
free
```
```python
( x );
    x =
```
```python
0
```
```python
;
```
```python
return
```
```python
;
}
```
```python
/*****************************
 * Householder transformation method testing scenario
 *  A is a 5x3 matrix
 *  b is a vector
 *****************************/
```
```python
void
```
```python
testHouseholder()
{
```
```python
int
```
```python
m =
```
```python
5
```
```python
;
```
```python
int
```
```python
n =
```
```python
3
```
```python
;
```
```python
float
```
```python
A[] =
    {
```
```python
0.8147
```
```python
,
```
```python
0.0975
```
```python
,
```
```python
0.1576
```
```python
,
```
```python
0.9058
```
```python
,
```
```python
0.2785
```
```python
,
```
```python
0.9706
```
```python
,
```
```python
0.1270
```
```python
,
```
```python
0.5469
```
```python
,
```
```python
0.9572
```
```python
,
```
```python
0.9134
```
```python
,
```
```python
0.9575
```
```python
,
```
```python
0.4854
```
```python
,
```
```python
0.6324
```
```python
,
```
```python
0.9649
```
```python
,
```
```python
0.8003
```
```python
};
```
```python
float
```
```python
b[] = {
```
```python
1.0698
```
```python
,
```
```python
2.1549
```
```python
,
```
```python
1.6311
```
```python
,
```
```python
2.3563
```
```python
,
```
```python
2.3976
```
```python
};
```
```python
// Logging
```
```python
printf
```
```python
(
```
```python
"A  = \n"
```
```python
);
    showMatrix(A, m, n);
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
; i < n; i++)
    {
        h(A, m, n, i, b);
```
```python
// Logging
```
```python
printf
```
```python
(
```
```python
"A  = \n"
```
```python
);
        showMatrix(A, m, n);
    }
}
```
```python
int
```
```python
main(
```
```python
int
```
```python
argc,
```
```python
char
```
```python
* argv[] )
{
    testHouseholder();
```
```python
return
```
```python
0
```
```python
;
}
```
A case study[7] results:
```python
A  = 
0.81470 0.09750 0.15760 
0.90580 0.27850 0.97060 
0.12700 0.54690 0.95720 
0.91340 0.95750 0.48540 
0.63240 0.96490 0.80030
x = 0.81470 0.90580 0.12700 0.91340 0.63240 
||x||e = 1.65365 0.00000 0.00000 0.00000 0.00000 
v = x - ||x||e = 2.46835 0.90580 0.12700 0.91340 0.63240 
c = 0.244990 
I - c vvT = 
-
```
```python
0
```
```python
.
```
```python
49267
```
```python
-
```
```python
0
```
```python
.
```
```python
54776
```
```python
-
```
```python
0
```
```python
.
```
```python
076
```
```python
8
```
```python
0
```
```python
-
```
```python
0
```
```python
.
```
```python
55235
```
```python
-
```
```python
0
```
```python
.
```
```python
38243
```
```python
-
```
```python
0
```
```python
.
```
```python
54776
```
```python
0
```
```python
.
```
```python
79899
```
```python
-
```
```python
0
```
```python
.
```
```python
02
```
```python
818 -
```
```python
0
```
```python
.
```
```python
20269
```
```python
-
```
```python
0
```
```python
.
```
```python
14034
```
```python
-
```
```python
0
```
```python
.
```
```python
076
```
```python
8
```
```python
0
```
```python
-
```
```python
0
```
```python
.
```
```python
02
```
```python
818
```
```python
0
```
```python
.
```
```python
99605
```
```python
-
```
```python
0
```
```python
.
```
```python
02
```
```python
842 -
```
```python
0
```
```python
.
```
```python
01
```
```python
968
```
```python
-
```
```python
0
```
```python
.
```
```python
55235
```
```python
-
```
```python
0
```
```python
.
```
```python
20269
```
```python
-
```
```python
0
```
```python
.
```
```python
02
```
```python
842
```
```python
0
```
```python
.
```
```python
79560
```
```python
-
```
```python
0
```
```python
.
```
```python
14151
```
```python
-
```
```python
0
```
```python
.
```
```python
38243
```
```python
-
```
```python
0
```
```python
.
```
```python
14034
```
```python
-
```
```python
0
```
```python
.
```
```python
01
```
```python
968 -
```
```python
0
```
```python
.
```
```python
14151
```
```python
0
```
```python
.
```
```python
90202
```
```python
A1  = 
-
```
```python
1.65365
```
```python
-
```
```python
1.14047
```
```python
-
```
```python
1.25698
```
```python
0.00000 -0.17579 0.45150 
0.00000 0.48320 0.88442 
0.00000 0.49940 -0.03806 
0.00000 0.64773 0.43788 
A  = 
-
```
```python
1.65365
```
```python
-
```
```python
1.14047
```
```python
-
```
```python
1.25698
```
```python
0.00000 -0.17579 0.45150 
0.00000 0.48320 0.88442 
0.00000 0.49940 -0.03806 
0.00000 0.64773 0.43788 
x = -0.17579 0.48320 0.49940 0.64773 
||x||e = -0.96609 0.00000 0.00000 0.00000 
v = x - ||x||e = -1.14189 0.48320 0.49940 0.64773 
c = 0.906478 
I - c vvT = 
-
```
```python
0
```
```python
.
```
```python
18196
```
```python
0
```
```python
.
```
```python
50016
```
```python
0
```
```python
.
```
```python
51692
```
```python
0
```
```python
.
```
```python
67046
```
```python
0.50016 0.78835 -0.21874 -0.28371 
0.51692 -0.21874 0.77393 -0.29322 
0.67046 -0.28371 -0.29322 0.61968 
A1  = 
0.96609 0.63411 
-
```
```python
0
```
```python
.
```
```python
00000
```
```python
0
```
```python
.
```
```python
80714
```
```python
-
```
```python
0
```
```python
.
```
```python
00000
```
```python
-
```
```python
0
```
```python
.
```
```python
11792
```
```python
-
```
```python
0
```
```python
.
```
```python
00000
```
```python
0
```
```python
.
```
```python
33430
```
```python
A  = 
-
```
```python
1.65365
```
```python
-
```
```python
1.14047
```
```python
-
```
```python
1.25698
```
```python
0.00000 0.96609 0.63411 
0.00000 -0.00000 0.80714 
0.00000 -0.00000 -0.11792 
0.00000 -0.00000 0.33430 
x = 0.80714 -0.11792 0.33430 
||x||e = 0.88156 0.00000 0.00000 
v = x - ||x||e = 1.68870 -0.11792 0.33430 
c = 0.671733 
I - c vvT = 
-
```
```python
0
```
```python
.
```
```python
91559
```
```python
0
```
```python
.
```
```python
13376
```
```python
-
```
```python
0
```
```python
.
```
```python
37921
```
```python
0.13376 0.99066 0.02648 
-
```
```python
0
```
```python
.
```
```python
37921
```
```python
0
```
```python
.
```
```python
0264
```
```python
8
```
```python
0
```
```python
.
```
```python
92493
```
```python
A1  = 
-
```
```python
0
```
```python
.
```
```python
88156
```
```python
-
```
```python
0
```
```python
.
```
```python
00000
```
```python
0.00000 
A  = 
-
```
```python
1.65365
```
```python
-
```
```python
1.14047
```
```python
-
```
```python
1.25698
```
```python
0.00000 0.96609 0.63411 
0.00000 -0.00000 -0.88156 
0.00000 -0.00000 -0.00000 
0.00000 -0.00000 0.00000
```
> [1]
> [http://qucs.sourceforge.net/tech/node99.html](http://qucs.sourceforge.net/tech/node99.html)

> [2]
> [https://en.wikipedia.org/wiki/System_of_linear_equations](https://en.wikipedia.org/wiki/System_of_linear_equations)

> [3]
> [https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix](https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix)

> [4]
> [http://fourier.eng.hmc.edu/e176/lectures/NM/node10.html](http://fourier.eng.hmc.edu/e176/lectures/NM/node10.html)

> [5]
> [http://www.math.iit.edu/~fass/477577_Chapter_4.pdf](http://www.math.iit.edu/~fass/477577_Chapter_4.pdf)

> [6]
> [http://www.aaronschlegel.com/qr-decomposition-householder-reflections/](http://www.aaronschlegel.com/qr-decomposition-householder-reflections/)

> [7]
> [http://www.math.usm.edu/lambers/mat610/sum10/lecture9.pdf](http://www.math.usm.edu/lambers/mat610/sum10/lecture9.pdf)

> [8] Gene H Golub, Charles F. Van Loan. Matrix Computation.

> [9]
> [http://terminus.sdsu.edu/SDSU/Math543_s2010/Lectures/12/lecture-static.pdf](http://terminus.sdsu.edu/SDSU/Math543_s2010/Lectures/12/lecture-static.pdf)

> [10]
> [https://www-old.math.gatech.edu/academic/courses/core/math2601/Web-notes/3num.pdf](https://www-old.math.gatech.edu/academic/courses/core/math2601/Web-notes/3num.pdf)


