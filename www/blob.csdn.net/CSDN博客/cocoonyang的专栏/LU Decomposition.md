# LU Decomposition - cocoonyang的专栏 - CSDN博客





2017年11月19日 10:12:03[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：161








对于线性方程组  


$A x = b$

高斯消元法是通过线性变换将满阵$A$变换成一个上三角阵，然后可使用迭代法求解该方程组。 从第一列开始，将矩阵对角元素以下的各元素变换成零，然后逐次到最后一列。从而将满阵$A$ 变换成一个上三角阵$U$. 这个过程等价与用一系列下三角阵左乘$A$:  


$L_{m-1} \cdots L_{1} A = U$

令 


$L^{-1} = L_{m-1} \cdots L_{1} $

则 


$L^{-1} A = U  => A = LU$

由此得到$A$ 的 $LU$ 因子分解.
代码样例[[1]](https://en.wikipedia.org/wiki/LU_decomposition)

```
/* INPUT: A - array of pointers to rows of a square matrix having dimension N
 *        Tol - small tolerance number to detect failure when the matrix is near degenerate
 * OUTPUT: Matrix A is changed, it contains both matrices L-E and U as A=(L-E)+U such that P*A=L*U.
 *        The permutation matrix is not stored as a matrix, but in an integer vector P of size N+1 
 *        containing column indexes where the permutation matrix has "1". The last element P[N]=S+N, 
 *        where S is the number of row exchanges needed for determinant computation, det(P)=(-1)^S    
 */
int LUPDecompose(double **A, int N, double Tol, int *P) {
    int i, j, k, imax; 
    double maxA, *ptr, absA;
    for (i = 0; i <= N; i++)
        P[i] = i; //Unit permutation matrix, P[N] initialized with N
    for (i = 0; i < N; i++) {
        maxA = 0.0;
        imax = i;
        for (k = i; k < N; k++)
            if ((absA = fabs(A[k][i])) > maxA) { 
                maxA = absA;
                imax = k;
            }
        if (maxA < Tol) return 0; //failure, matrix is degenerate
        if (imax != i) {
            //pivoting P
            j = P[i];
            P[i] = P[imax];
            P[imax] = j;
            //pivoting rows of A
            ptr = A[i];
            A[i] = A[imax];
            A[imax] = ptr;
            //counting pivots starting from N (for determinant)
            P[N]++;
        }
        for (j = i + 1; j < N; j++) {
            A[j][i] /= A[i][i];
            for (k = i + 1; k < N; k++)
                A[j][k] -= A[j][i] * A[i][k];
        }
    }
    return 1;  //decomposition done 
}
/* INPUT: A,P filled in LUPDecompose; b - rhs vector; N - dimension
 * OUTPUT: x - solution vector of A*x=b
 */
void LUPSolve(double **A, int *P, double *b, int N, double *x) {
    for (int i = 0; i < N; i++) {
        x[i] = b[P[i]];
        for (int k = 0; k < i; k++)
            x[i] -= A[i][k] * x[k];
    }
    for (int i = N - 1; i >= 0; i--) {
        for (int k = i + 1; k < N; k++)
            x[i] -= A[i][k] * x[k];
        x[i] = x[i] / A[i][i];
    }
}
/* INPUT: A,P filled in LUPDecompose; N - dimension
 * OUTPUT: IA is the inverse of the initial matrix
 */
void LUPInvert(double **A, int *P, int N, double **IA) {  
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            if (P[i] == j) 
                IA[i][j] = 1.0;
            else
                IA[i][j] = 0.0;
            for (int k = 0; k < i; k++)
                IA[i][j] -= A[i][k] * IA[k][j];
        }
        for (int i = N - 1; i >= 0; i--) {
            for (int k = i + 1; k < N; k++)
                IA[i][j] -= A[i][k] * IA[k][j];
            IA[i][j] = IA[i][j] / A[i][i];
        }
    }
}
/* INPUT: A,P filled in LUPDecompose; N - dimension. 
 * OUTPUT: Function returns the determinant of the initial matrix
 */
double LUPDeterminant(double **A, int *P, int N) {
    double det = A[0][0];
    for (int i = 1; i < N; i++)
        det *= A[i][i];
    if ((P[N] - N) % 2 == 0)
        return det; 
    else
        return -det;
}
```




