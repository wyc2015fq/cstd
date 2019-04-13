
# Cholesky Decomposition - cocoonyang的专栏 - CSDN博客


2017年07月20日 17:00:31[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1571



# Cholesky分解法
Cholesky分解法是将 Hermitian正定矩阵分解为一个下三角阵与其共轭转置阵的积。当线性方程组 Ax=b可用Cholesky分解法求解时，Cholesky分解法的求解效率大约是LU分解法的2倍[[1]](https://en.wikipedia.org/wiki/Cholesky_decomposition)。
```python
#include <stdio.h>
```
```python
#include <stdlib.h>
```
```python
#include <math.h>
```
```python
const
```
```python
double
```
```python
ESP =
```
```python
0.00001
```
```python
;
```
```python
void
```
```python
forward_elimination(
```
```python
double
```
```python
** L,
```
```python
double
```
```python
* y,
```
```python
const
```
```python
double
```
```python
* b,
```
```python
int
```
```python
n)
{
```
```python
register
```
```python
int
```
```python
i, j;
```
```python
if
```
```python
(
```
```python
fabs
```
```python
(L[
```
```python
0
```
```python
][
```
```python
0
```
```python
]) &lt; ESP)
    {
```
```python
return
```
```python
;
    }
    y[
```
```python
0
```
```python
] = b[
```
```python
0
```
```python
] / L[
```
```python
0
```
```python
][
```
```python
0
```
```python
];
```
```python
for
```
```python
(i =
```
```python
1
```
```python
; i &lt; n; i++)
    {
        y[i] = b[i];
```
```python
for
```
```python
(j =
```
```python
0
```
```python
; j &lt; i; j++)
        {
            y[i] -= L[i][j] * y[j];
        }
```
```python
// checking
```
```python
if
```
```python
(
```
```python
fabs
```
```python
(L[i][i]) &lt; ESP)
        {
```
```python
return
```
```python
;
        }
        y[i] = y[i] / L[i][i];
    }
```
```python
//printf("y = [%f, %f, %f] \n", y[0], y[1], y[2]);
```
```python
}
```
```python
void
```
```python
backward_elimination(
```
```python
double
```
```python
** U,
```
```python
double
```
```python
* x,
```
```python
const
```
```python
double
```
```python
* y,
```
```python
int
```
```python
n)
{
```
```python
register
```
```python
int
```
```python
i, j;
```
```python
// checking
```
```python
if
```
```python
(
```
```python
fabs
```
```python
(U[n -
```
```python
1
```
```python
][n -
```
```python
1
```
```python
]) &lt; ESP)
    {
```
```python
return
```
```python
;
    }
    x[n -
```
```python
1
```
```python
] = y[n -
```
```python
1
```
```python
] / U[n -
```
```python
1
```
```python
][n -
```
```python
1
```
```python
];
```
```python
for
```
```python
(i = n -
```
```python
2
```
```python
; i &gt;=
```
```python
0
```
```python
; i--)
    {
        x[i] = y[i];
```
```python
for
```
```python
(j = i +
```
```python
1
```
```python
; j &lt; n; j++)
        {
            x[i] -= U[i][j] * x[j];
        }
```
```python
// checking
```
```python
if
```
```python
(
```
```python
fabs
```
```python
( U[i][i]) &lt; ESP)
        {
```
```python
return
```
```python
;
        }
        x[i] = x[i] / U[i][i];
    }
}
```
```python
void
```
```python
cholesky_solve(
```
```python
double
```
```python
** L,
```
```python
double
```
```python
* x,
```
```python
const
```
```python
double
```
```python
* b,
```
```python
int
```
```python
n)
{
```
```python
if
```
```python
((
```
```python
0
```
```python
== L) || (
```
```python
0
```
```python
== x) || (
```
```python
0
```
```python
== B) || (n &lt;=
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
double
```
```python
* y = (
```
```python
double
```
```python
*)
```
```python
malloc
```
```python
(n *
```
```python
sizeof
```
```python
(
```
```python
double
```
```python
));
```
```python
//new double[n];
```
```python
if
```
```python
(
```
```python
0
```
```python
== y)
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
/* Solve L * y = b */
```
```python
forward_elimination(L, y, b, n);
```
```python
/* Solve L^T * x = y */
```
```python
backward_elimination(L, x, y, n);
```
```python
free
```
```python
(y);
}
```
```python
void
```
```python
display_matrix(
```
```python
double
```
```python
**data,
```
```python
int
```
```python
n)
{
```
```python
int
```
```python
i, j;
```
```python
printf
```
```python
(
```
```python
"( \n"
```
```python
);
```
```python
for
```
```python
(i =
```
```python
0
```
```python
; i &lt; n; i++)
    {
```
```python
for
```
```python
(j =
```
```python
0
```
```python
; j &lt; n; j++)
        {
```
```python
if
```
```python
(i == n -
```
```python
1
```
```python
&& j == n -
```
```python
1
```
```python
)
```
```python
printf
```
```python
(
```
```python
"%.4f"
```
```python
,  data[i][j]);
```
```python
else
```
```python
printf
```
```python
(
```
```python
"%.4f,"
```
```python
,  data[i][j]);
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
printf
```
```python
(
```
```python
")\n"
```
```python
);
}
```
```python
void
```
```python
test()
{
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
double
```
```python
** m = (
```
```python
double
```
```python
**)
```
```python
malloc
```
```python
(
```
```python
3
```
```python
*
```
```python
sizeof
```
```python
(
```
```python
double
```
```python
*));
```
```python
double
```
```python
a[] = {
```
```python
1
```
```python
,
```
```python
0
```
```python
, -
```
```python
5
```
```python
};
```
```python
double
```
```python
b[] = {
```
```python
0
```
```python
,
```
```python
9
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
double
```
```python
c[] = { -
```
```python
5
```
```python
,
```
```python
0
```
```python
,
```
```python
11
```
```python
};
    m[
```
```python
0
```
```python
] = a;
    m[
```
```python
1
```
```python
] = b;
    m[
```
```python
2
```
```python
] = c;
```
```python
double
```
```python
f[] = {
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
};
```
```python
double
```
```python
* x = (
```
```python
double
```
```python
*)
```
```python
malloc
```
```python
(n *
```
```python
sizeof
```
```python
(
```
```python
double
```
```python
));
    cholesky_solve(m, x, f, n);
```
```python
printf
```
```python
(
```
```python
"A=\n"
```
```python
);
    display_matrix(m,n);
```
```python
printf
```
```python
(
```
```python
"L=\n"
```
```python
);
    display_matrix(complete_cholesky_decompose(m),n);
```
```python
printf
```
```python
(
```
```python
"x = [%f, %f, %f] \n"
```
```python
, x[
```
```python
0
```
```python
], x[
```
```python
1
```
```python
], x[
```
```python
2
```
```python
]);
```
```python
free
```
```python
(m);
```
```python
free
```
```python
(x);
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
*argv[])
{
    test();
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
# Crout decomposition
“Crout matrix decomposition is an LU decomposition which decomposes a matrix into a lower triangular matrix (L), an upper triangular matrix (U) and, although not always needed, a permutation matrix (P).”[[2]](https://en.wikipedia.org/wiki/Crout_matrix_decomposition)
Implementations[[2]](https://en.wikipedia.org/wiki/Crout_matrix_decomposition)
```python
void
```
```python
crout(
```
```python
double
```
```python
const
```
```python
**A,
```
```python
double
```
```python
**L,
```
```python
double
```
```python
**U,
```
```python
int
```
```python
n) {
```
```python
int
```
```python
i, j, k;
```
```python
double
```
```python
sum =
```
```python
0
```
```python
;
```
```python
for
```
```python
(i =
```
```python
0
```
```python
; i < n; i++) {
        U[i][i] =
```
```python
1
```
```python
;
    }
```
```python
for
```
```python
(j =
```
```python
0
```
```python
; j < n; j++) {
```
```python
for
```
```python
(i = j; i < n; i++) {
            sum =
```
```python
0
```
```python
;
```
```python
for
```
```python
(k =
```
```python
0
```
```python
; k < j; k++) {
                sum = sum + L[i][k] * U[k][j];  
            }
            L[i][j] = A[i][j] - sum;
        }
```
```python
for
```
```python
(i = j; i < n; i++) {
            sum =
```
```python
0
```
```python
;
```
```python
for
```
```python
(k =
```
```python
0
```
```python
; k < j; k++) {
                sum = sum + L[j][k] * U[k][i];
            }
```
```python
if
```
```python
(L[j][j] ==
```
```python
0
```
```python
) {
```
```python
printf
```
```python
(
```
```python
"det(L) close to 0!\n Can't divide by 0...\n"
```
```python
);
```
```python
exit
```
```python
(EXIT_FAILURE);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
    }
}
```
> [1]
> [https://en.wikipedia.org/wiki/Cholesky_decomposition](https://en.wikipedia.org/wiki/Cholesky_decomposition)

> [2]
> [https://en.wikipedia.org/wiki/Crout_matrix_decomposition](https://en.wikipedia.org/wiki/Crout_matrix_decomposition)

> [3]
> [https://en.wikipedia.org/wiki/LU_decomposition](https://en.wikipedia.org/wiki/LU_decomposition)


