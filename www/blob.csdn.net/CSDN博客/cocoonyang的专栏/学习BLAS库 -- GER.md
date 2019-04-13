
# 学习BLAS库 -- GER - cocoonyang的专栏 - CSDN博客


2017年03月08日 15:52:22[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：480


**函数语法:**
XGER( M, N, ALPHA, X, INCX, Y, INCY, A, LDA)
**功能**：
GER performs the rank 1 operation
A := alpha*x*y' + A,

## C语言版SGER
C programming language version of SGER method with row primary order data storage.
**源代码**：
```python
/*
  Purpose:
    SGER computes A := alpha*x*y' + A.
  Discussion:
    SGER performs the rank 1 operation
      A := alpha*x*y' + A,
    where alpha is a scalar, x is an m element vector, y is an n element
    vector and A is an m by n matrix.
  Licensing:
    This code is distributed under the GNU LGPL license.
  Modified:
      2017
  Author:
    Original FORTRAN77 version by Jack Dongarra,  Jeremy Du Croz,
    Sven Hammarling,  Richard Hanson.
    This C version by Chunfeng Yang, John Burkardt.
  Parameters:
    Input, int M, the number of rows of the matrix A.
    0 <= M.
    Input, int N, the number of columns of the matrix A.
    0 <= N.
    Input, float ALPHA, the scalar multiplier.
    Input, float X[1+(M-1)*abs(INCX)], the first vector.
    Input, int INCX, the increment for elements of X.
    INCX must not be zero.
    Input, float Y[1+(N-1)*abs(INCY)], the second vector.
    Input, int INCY, the increment for elements of Y.
    INCY must not be zero.
    Input/output, float A[LDA*N].  On entry, the leading M by N
    part of the array contains the matrix of coefficients. On exit, A is
    overwritten by the updated matrix.
    Input, int LDA, the first dimension of A as declared
    in the calling program. max ( 1, M ) <= LDA.
*/
// row prinary order version
void cblas_sger ( int m, int n, float alpha, float x[], int incx, float y[],
  int incy, float a[], int lda )
{
	//int i;
	int info;
	int ix = 0;
	//int j;
	int jy = 0;
	int kx;
	int ky = 0;
	float temp = 0.0;
	/*
	 Test the input parameters.
	 */
	info = 0;
	if (m < 0)
	{
		info = 1;
	}
	else if (n < 0)
	{
		info = 2;
	}
	else if (incx == 0)
	{
		info = 5;
	}
	else if (incy == 0)
	{
		info = 7;
	}
	else if (lda < i4_max(1, m))
	{
		info = 9;
	}
	if (info != 0)
	{
		xerbla("SGER", info);
		return;
	}
	/*
	 Quick return if possible.
	 */
	if (m == 0 || n == 0 || alpha == 0.0)
	{
		return;
	}
	/*
	 Start the operations. In this version the elements of A are
	 accessed sequentially with one pass through A.
	 */
	if (0 < incx)
	{
		ix = 0;
	}
	else
	{
		ix = 0 - (m - 1) * incx;
	}
	if (incy == 1)
	{
		for (int i = 0; i < m; i++)
		{
			if (x[ix] != 0.0)
			{
				temp = alpha * x[ix];
				for (int j = 0; j < n; j++)
				{
					a[i * lda + j] = a[i * lda + j] + y[j] * temp;
				}
			}
			ix = ix + incx;
		}
	}
	else
	{
		if (0 < incy)
		{
			ky = 0;
		}
		else
		{
			ky = 0 - (m - 1) * incy;
		}
		for (int i = 0; i < m; i++)
		{
			if (x[ix] != 0.0)
			{
				temp = alpha * x[ix];
				jy = ky;
				for (int j = 0; j < n; j++)
				{
					a[i * lda + j] = a[i * lda + j] + y[jy] * temp;
					jy = jy + incy;
				}
			}
			ix = ix + incx;
		}
	}
	return;
}
```

Testing Code
```python
void testsger()
{
	int row = 4;
	int column = 4;
	float *A = (float*) malloc(row * column * sizeof(float));
	float *x = (float*) malloc(row * sizeof(float));
	float *y = (float*) malloc(column * sizeof(float));
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			A[i*row + j] = 0.0;
		}
		x[i] = 1.0 + i;
		y[i] = 1.0 + i;
	}
	cblas_sger(row/2, column/2, 1.0, x, 2, y, 2, &A[1*row+2], row);
	show_matrix(A, row, column);
	return;
}
```
Testing Results:
```python
A = 1.0 * X * Y 
X = 
[ 1,
  2 ,  
  3 ,   
  4 ]
Y = [1,2,3,4]
A:
0.000000 0.000000 0.000000 0.000000
0.000000 0.000000 1.000000 3.000000
0.000000 0.000000 3.000000 9.000000
0.000000 0.000000 0.000000 0.000000
```



