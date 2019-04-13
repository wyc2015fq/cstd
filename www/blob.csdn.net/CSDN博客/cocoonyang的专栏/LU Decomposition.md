
# LU Decomposition - cocoonyang的专栏 - CSDN博客


2017年11月19日 10:12:03[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：155


对于线性方程组

$$
A x = b
$$
高斯消元法是通过线性变换将满阵$A$变换成一个上三角阵，然后可使用迭代法求解该方程组。 从第一列开始，将矩阵对角元素以下的各元素变换成零，然后逐次到最后一列。从而将满阵$A$变换成一个上三角阵$U$. 这个过程等价与用一系列下三角阵左乘$A$:

$$
L_{m-1} \cdots L_{1} A = U
$$
令

$$
L^{-1} = L_{m-1} \cdots L_{1}
$$
则

$$
L^{-1} A = U  => A = LU
$$
由此得到$A$的$LU$因子分解.
代码样例[[1]](https://en.wikipedia.org/wiki/LU_decomposition)
```python
/* INPUT: A - array of pointers to rows of a square matrix having dimension N
 *        Tol - small tolerance number to detect failure when the matrix is near degenerate
 * OUTPUT: Matrix A is changed, it contains both matrices L-E and U as A=(L-E)+U such that P*A=L*U.
 *        The permutation matrix is not stored as a matrix, but in an integer vector P of size N+1 
 *        containing column indexes where the permutation matrix has "1". The last element P[N]=S+N, 
 *        where S is the number of row exchanges needed for determinant computation, det(P)=(-1)^S    
 */
```
```python
int
```
```python
LUPDecompose(
```
```python
double
```
```python
**A,
```
```python
int
```
```python
N,
```
```python
double
```
```python
Tol,
```
```python
int
```
```python
*P) {
```
```python
int
```
```python
i, j, k, imax;
```
```python
double
```
```python
maxA, *ptr, absA;
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
; i &lt;= N; i++)
        P[i] = i;
```
```python
//Unit permutation matrix, P[N] initialized with N
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
; i &lt; N; i++) {
        maxA =
```
```python
0.0
```
```python
;
        imax = i;
```
```python
for
```
```python
(k = i; k &lt; N; k++)
```
```python
if
```
```python
((absA =
```
```python
fabs
```
```python
(A[k][i])) &gt; maxA) { 
                maxA = absA;
                imax = k;
            }
```
```python
if
```
```python
(maxA &lt; Tol)
```
```python
return
```
```python
0
```
```python
;
```
```python
//failure, matrix is degenerate
```
```python
if
```
```python
(imax != i) {
```
```python
//pivoting P
```
```python
j = P[i];
            P[i] = P[imax];
            P[imax] = j;
```
```python
//pivoting rows of A
```
```python
ptr = A[i];
            A[i] = A[imax];
            A[imax] = ptr;
```
```python
//counting pivots starting from N (for determinant)
```
```python
P[N]++;
        }
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
; j &lt; N; j++) {
            A[j][i] /= A[i][i];
```
```python
for
```
```python
(k = i +
```
```python
1
```
```python
; k &lt; N; k++)
                A[j][k] -= A[j][i] * A[i][k];
        }
    }
```
```python
return
```
```python
1
```
```python
;
```
```python
//decomposition done
```
```python
}
```
```python
/* INPUT: A,P filled in LUPDecompose; b - rhs vector; N - dimension
 * OUTPUT: x - solution vector of A*x=b
 */
```
```python
void
```
```python
LUPSolve(
```
```python
double
```
```python
**A,
```
```python
int
```
```python
*P,
```
```python
double
```
```python
*b,
```
```python
int
```
```python
N,
```
```python
double
```
```python
*x) {
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
; i &lt; N; i++) {
        x[i] = b[P[i]];
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
; k &lt; i; k++)
            x[i] -= A[i][k] * x[k];
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
i = N -
```
```python
1
```
```python
; i &gt;=
```
```python
0
```
```python
; i--) {
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
k = i +
```
```python
1
```
```python
; k &lt; N; k++)
            x[i] -= A[i][k] * x[k];
        x[i] = x[i] / A[i][i];
    }
}
```
```python
/* INPUT: A,P filled in LUPDecompose; N - dimension
 * OUTPUT: IA is the inverse of the initial matrix
 */
```
```python
void
```
```python
LUPInvert(
```
```python
double
```
```python
**A,
```
```python
int
```
```python
*P,
```
```python
int
```
```python
N,
```
```python
double
```
```python
**IA) {
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
; j &lt; N; j++) {
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
; i &lt; N; i++) {
```
```python
if
```
```python
(P[i] == j) 
                IA[i][j] =
```
```python
1.0
```
```python
;
```
```python
else
```
```python
IA[i][j] =
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
; k &lt; i; k++)
                IA[i][j] -= A[i][k] * IA[k][j];
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
i = N -
```
```python
1
```
```python
; i &gt;=
```
```python
0
```
```python
; i--) {
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
k = i +
```
```python
1
```
```python
; k &lt; N; k++)
                IA[i][j] -= A[i][k] * IA[k][j];
            IA[i][j] = IA[i][j] / A[i][i];
        }
    }
}
```
```python
/* INPUT: A,P filled in LUPDecompose; N - dimension. 
 * OUTPUT: Function returns the determinant of the initial matrix
 */
```
```python
double
```
```python
LUPDeterminant(
```
```python
double
```
```python
**A,
```
```python
int
```
```python
*P,
```
```python
int
```
```python
N) {
```
```python
double
```
```python
det = A[
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
(
```
```python
int
```
```python
i =
```
```python
1
```
```python
; i &lt; N; i++)
        det *= A[i][i];
```
```python
if
```
```python
((P[N] - N) %
```
```python
2
```
```python
==
```
```python
0
```
```python
)
```
```python
return
```
```python
det;
```
```python
else
```
```python
return
```
```python
-det;
}
```

