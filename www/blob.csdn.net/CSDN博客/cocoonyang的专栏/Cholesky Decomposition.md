# Cholesky Decomposition - cocoonyang的专栏 - CSDN博客





2017年07月20日 17:00:31[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1585








# Cholesky分解法

Cholesky分解法是将 Hermitian正定矩阵分解为一个下三角阵与其共轭转置阵的积。当线性方程组 Ax=b可用Cholesky分解法求解时，Cholesky分解法的求解效率大约是LU分解法的2倍[[1]](https://en.wikipedia.org/wiki/Cholesky_decomposition)。

```
#include <stdio.h> 
 #include <stdlib.h> 
 #include <math.h> 

const double ESP = 0.00001;

void forward_elimination(double** L, double* y, const double* b, int n)
{
    register int i, j;

    if (fabs(L[0][0]) < ESP)
    {
        return;
    }
    y[0] = b[0] / L[0][0];

    for (i = 1; i < n; i++)
    {
        y[i] = b[i];
        for (j = 0; j < i; j++)
        {
            y[i] -= L[i][j] * y[j];
        }
        // checking
        if (fabs(L[i][i]) < ESP)
        {
            return;
        }
        y[i] = y[i] / L[i][i];
    }

    //printf("y = [%f, %f, %f] \n", y[0], y[1], y[2]);
}

void backward_elimination(double** U, double* x, const double* y, int n)
{
    register int i, j;

    // checking
    if (fabs(U[n - 1][n - 1]) < ESP)
    {
        return;
    }
    x[n - 1] = y[n - 1] / U[n - 1][n - 1];
    for (i = n - 2; i >= 0; i--)
    {
        x[i] = y[i];
        for (j = i + 1; j < n; j++)
        {
            x[i] -= U[i][j] * x[j];
        }
        // checking
        if (fabs( U[i][i]) < ESP)
        {
            return;
        }
        x[i] = x[i] / U[i][i];
    }
}

void cholesky_solve(double** L, double* x, const double* b, int n)
{
    if ((0 == L) || (0 == x) || (0 == B) || (n <= 0))
    {
        return;
    }

    double* y = (double*) malloc(n * sizeof(double)); //new double[n];
    if (0 == y)
    {
        return;
    }

    /* Solve L * y = b */
    forward_elimination(L, y, b, n);
    /* Solve L^T * x = y */
    backward_elimination(L, x, y, n);

    free(y);
}
void display_matrix(double **data, int n)
{
    int i, j;
    printf("( \n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (i == n - 1 && j == n - 1)
                printf("%.4f",  data[i][j]);
            else
                printf("%.4f,",  data[i][j]);
        }
        printf("\n");
    }
    printf(")\n");
}
void test()
{
    int n = 3;

    double** m = (double**) malloc(3 * sizeof(double*));
    double a[] = { 1, 0, -5 };
    double b[] = { 0, 9, 0 };
    double c[] = { -5, 0, 11 };

    m[0] = a;
    m[1] = b;
    m[2] = c;

    double f[] = { 1, 1, 1 };
    double* x = (double*) malloc(n * sizeof(double));

    cholesky_solve(m, x, f, n);

    printf("A=\n");
    display_matrix(m,n);

    printf("L=\n");
    display_matrix(complete_cholesky_decompose(m),n);

    printf("x = [%f, %f, %f] \n", x[0], x[1], x[2]);

    free(m);
    free(x);
}

int main(int argc, char*argv[])
{
    test();
    return 0;
}
```

# Crout decomposition

“Crout matrix decomposition is an LU decomposition which decomposes a matrix into a lower triangular matrix (L), an upper triangular matrix (U) and, although not always needed, a permutation matrix (P).”[[2]](https://en.wikipedia.org/wiki/Crout_matrix_decomposition)

Implementations[[2]](https://en.wikipedia.org/wiki/Crout_matrix_decomposition)

```
void crout(double const **A, double **L, double **U, int n) {
    int i, j, k;
    double sum = 0;
    for (i = 0; i < n; i++) {
        U[i][i] = 1;
    }
    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            sum = 0;
            for (k = 0; k < j; k++) {
                sum = sum + L[i][k] * U[k][j];  
            }
            L[i][j] = A[i][j] - sum;
        }
        for (i = j; i < n; i++) {
            sum = 0;
            for(k = 0; k < j; k++) {
                sum = sum + L[j][k] * U[k][i];
            }
            if (L[j][j] == 0) {
                printf("det(L) close to 0!\n Can't divide by 0...\n");
                exit(EXIT_FAILURE);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
    }
}
```

> 
[1] [https://en.wikipedia.org/wiki/Cholesky_decomposition](https://en.wikipedia.org/wiki/Cholesky_decomposition)

  [2] [https://en.wikipedia.org/wiki/Crout_matrix_decomposition](https://en.wikipedia.org/wiki/Crout_matrix_decomposition)

  [3] [https://en.wikipedia.org/wiki/LU_decomposition](https://en.wikipedia.org/wiki/LU_decomposition)




