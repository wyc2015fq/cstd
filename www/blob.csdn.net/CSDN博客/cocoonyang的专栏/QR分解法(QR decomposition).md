# QR分解法(QR decomposition) - cocoonyang的专栏 - CSDN博客





2017年11月19日 10:08:09[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：2283








QR decomposition divides a m by n matrix $A$ into a product of an orthogonal matrix $Q$ and an upper triangular matrix $R$:



$ A = Q R $

Thus  


$ Ax = b => QRx = b => Q^{T}QRx = Q^{T}b => Rx = Q^{T}b$

QR decomposition can be implemented by several algorithms, such as Gram–Schmidt process, Householder transformations, or Givens rotations. Gram-Schmidt procedure is a sequence of multiplications of A from the right by upper triangular matrices. Householde decomposition $A$ into $QR$ with orthogonal matrices.  As orthogonal transformations are stable, using Householder triangularization and back-substitution to slove $Ax = b$ is backward stable[9][10].
## Householder transformation

Householder transformation reflects a vector $u$ about a hyperplane which orthogonal to a vector $v$ which is called Householder vector[8].  


$ u^{f} = u - 2vv^{T}u$

The basic idea of Householder reflection for QR decomposition is to find a linear transformation that changes vector $u$ into a vector which collinear to $e_{i}$, $u^{f} = ||u||e_{i}$, thus all but one entries of the vector $u$ can be eliminated.  It means that finding a hyperplane that bisect $u$  and $||u||e_{i}$. The orthogonal vector of the hyperplane is: 


$ v = u - ||u||e_{i}$

The transformation matrix is: 


$ H = I - 2 \frac{vv^{T}}{v^{T}v} $

Householder QR decomposition code :
```
# include <math.h>
# include <stdio.h>
# include <stdlib.h>

void showVector(float* A, int n)
{
    // Checking function parameters
    if((null == A) || (n < 0))
    {
        return;
    }
    for (int i = 0; i < n; i++)
    {
        printf("%2.5f ", A[i]);
    }
    printf("\n");
}

void showMatrix(float *A, int m, int n)
{
    // Checking function parameters
    if((null == A) || (m < 0) || (n < 0))
    {
        return;
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%2.5f ", A[i * n + j]);
        printf("\n");
    }
}

/*****************************
 * Householder transformation function 
 *  inputs:
 *      A - m by n matrix
 *      m - row of matrix A 
 *      n - column of matrix A
 *      offset - the offset 
 *      b - the left side of the linear functions
 *****************************/
void h(float* A, int m, int n, int offset, float* b )
{
    // Checking function parameters
    if((null == A) || (null == b) || (m < 0) || (n < 0))
    {
        return;
    }
    //  A -- m by n matrix
    //       m >= n
    int len = m - offset;
    if(( n - offset ) <= 1)
    {
         return;
    }
    // Temporary vector pointers
    float* x = null;
    float* u = null;
    float* I = null;
    float* A1 = null;
    float* newB = null;

    x = (float*) malloc(len * sizeof(float));
    if(null == x)
    {
        goto CLEAN;
    }
    for (int i = 0; i < len; i++)
    {
        x[i] = A[(offset + i) * n + offset];
    }

    // Logging 
    printf("x = ");
    showVector(x, len);

    float xNormal = snrm2(len, x, 1);
    u = (float*) malloc(len * sizeof(float));
    if(null == u)
    {
        goto CLEAN;
    }   
    for (int i = 0; i < len; i++)
    {
        u[i] = 0.0;
    }
    u[0] = pow(-1, offset) *xNormal;

    // Logging 
    printf("||x||e = ");
    showVector(u, len);

    saxpy(len,  1, x, 1, u, 1);

    // Logging 
    printf("v = x - ||x||e = ");
    showVector(u, len);

    float vNormal = snrm2(len, u, 1);
    vNormal =  vNormal * vNormal;

    // Logging 
    printf("c = %f \n", 2/vNormal);

    I = (float*) malloc(len * len * sizeof(float));
    if(null == I)
    {
        goto CLEAN;
    }   
    for (int i = 0; i < len * len; i++)
    {
        I[i] = 0.0;
    }
    for (int i = 0; i < len; i++)
    {
        I[i * len + i] = 1.0;
    }

    for (int i = 0; i < len  ; i++)
    {
        for (int j = 0; j < len; j++)
        {
            I[i * len + j] -= 2 * u[i] * u[j] / vNormal;
        }
    }

    // Logging 
    printf("I - c vvT = \n");
    showMatrix(I, len);

    A1 = (float*) malloc((m-offset) * (n-offset) * sizeof(float));
    if(null == Al)
    {
        goto CLEAN;
    }
    for (int i = 0; i < m-offset; i++)
    {
        for (int j = 0; j < n-offset; j++)
        {
            A1[i*(n-offset)+j] = 0.0;
            for( int k = 0; k < len; k++)
            {
                A1[i*(n-offset)+j] += I[i*(m-offset) + k]* A[(k+offset)*(n)+j+offset];
            }
        }
    }

    // Logging 
    printf("A1  = \n");
    showMatrix(A1, (m-offset), (n-offset));


    for (int i = offset; i < m; i++)
    {
        for (int j = offset; j < n; j++)
        {
            A[i*n+j]  = A1[(i-offset)*(n-offset)+(j-offset)];
        }
    }

    newB = (float*) malloc( m*sizeof(float))
    if(null == newB)
    {
        goto CLEAN;
    }
    for( int i = 0; i < m; i++ )
    {
        newB[i] = 0.0;
    }

    for(int i = 0; i < len; i++ )
    {
      float tmp = 0.0;
      for( int j = 0; j < len; j++ )
      {
             tmp += I[i * len + j] * b[j+offset];
      }
     newB[i] = tmp;
    }
    for(int i = 0; i < len; i++ )
    {
        b[i+offset] = newB[i];
    }

    // Clean temporary memory
    CLEAN:
    free( newB );
    newB = 0;
    free( A1 );
    A1 = 0; 
    free( I );
    I = 0;
    free( u );
    u = 0;
    free( x );
    x = 0;  
    return;
}

/*****************************
 * Householder transformation method testing scenario
 *  A is a 5x3 matrix
 *  b is a vector
 *****************************/
void testHouseholder()
{
    int m = 5;
    int n = 3;
    float A[] =
    {0.8147, 0.0975, 0.1576,
    0.9058, 0.2785, 0.9706,
    0.1270, 0.5469, 0.9572,
    0.9134, 0.9575, 0.4854,
    0.6324, 0.9649, 0.8003};
    float b[] = {1.0698, 2.1549, 1.6311, 2.3563, 2.3976};

    // Logging 
    printf("A  = \n");
    showMatrix(A, m, n);

    for (int i = 0; i < n; i++)
    {
        h(A, m, n, i, b);

        // Logging 
        printf("A  = \n");
        showMatrix(A, m, n);
    }
}

int main( int argc, char* argv[] )
{
    testHouseholder();
    return 0;
}
```

A case study[7] results:

```
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
-0.49267 -0.54776 -0.07680 -0.55235 -0.38243 
-0.54776 0.79899 -0.02818 -0.20269 -0.14034 
-0.07680 -0.02818 0.99605 -0.02842 -0.01968 
-0.55235 -0.20269 -0.02842 0.79560 -0.14151 
-0.38243 -0.14034 -0.01968 -0.14151 0.90202 
A1  = 
-1.65365 -1.14047 -1.25698 
0.00000 -0.17579 0.45150 
0.00000 0.48320 0.88442 
0.00000 0.49940 -0.03806 
0.00000 0.64773 0.43788 
A  = 
-1.65365 -1.14047 -1.25698 
0.00000 -0.17579 0.45150 
0.00000 0.48320 0.88442 
0.00000 0.49940 -0.03806 
0.00000 0.64773 0.43788 

x = -0.17579 0.48320 0.49940 0.64773 
||x||e = -0.96609 0.00000 0.00000 0.00000 
v = x - ||x||e = -1.14189 0.48320 0.49940 0.64773 
c = 0.906478 
I - c vvT = 
-0.18196 0.50016 0.51692 0.67046 
0.50016 0.78835 -0.21874 -0.28371 
0.51692 -0.21874 0.77393 -0.29322 
0.67046 -0.28371 -0.29322 0.61968 
A1  = 
0.96609 0.63411 
-0.00000 0.80714 
-0.00000 -0.11792 
-0.00000 0.33430 
A  = 
-1.65365 -1.14047 -1.25698 
0.00000 0.96609 0.63411 
0.00000 -0.00000 0.80714 
0.00000 -0.00000 -0.11792 
0.00000 -0.00000 0.33430 

x = 0.80714 -0.11792 0.33430 
||x||e = 0.88156 0.00000 0.00000 
v = x - ||x||e = 1.68870 -0.11792 0.33430 
c = 0.671733 
I - c vvT = 
-0.91559 0.13376 -0.37921 
0.13376 0.99066 0.02648 
-0.37921 0.02648 0.92493 
A1  = 
-0.88156 
-0.00000 
0.00000 
A  = 
-1.65365 -1.14047 -1.25698 
0.00000 0.96609 0.63411 
0.00000 -0.00000 -0.88156 
0.00000 -0.00000 -0.00000 
0.00000 -0.00000 0.00000
```

> 
[1] [http://qucs.sourceforge.net/tech/node99.html](http://qucs.sourceforge.net/tech/node99.html)

  [2] [https://en.wikipedia.org/wiki/System_of_linear_equations](https://en.wikipedia.org/wiki/System_of_linear_equations)

  [3] [https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix](https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix)

  [4] [http://fourier.eng.hmc.edu/e176/lectures/NM/node10.html](http://fourier.eng.hmc.edu/e176/lectures/NM/node10.html)

  [5] [http://www.math.iit.edu/~fass/477577_Chapter_4.pdf](http://www.math.iit.edu/~fass/477577_Chapter_4.pdf)

  [6] [http://www.aaronschlegel.com/qr-decomposition-householder-reflections/](http://www.aaronschlegel.com/qr-decomposition-householder-reflections/)

  [7] [http://www.math.usm.edu/lambers/mat610/sum10/lecture9.pdf](http://www.math.usm.edu/lambers/mat610/sum10/lecture9.pdf)

  [8] Gene H Golub, Charles F. Van Loan. Matrix Computation. 

  [9] [http://terminus.sdsu.edu/SDSU/Math543_s2010/Lectures/12/lecture-static.pdf](http://terminus.sdsu.edu/SDSU/Math543_s2010/Lectures/12/lecture-static.pdf)

  [10] [https://www-old.math.gatech.edu/academic/courses/core/math2601/Web-notes/3num.pdf](https://www-old.math.gatech.edu/academic/courses/core/math2601/Web-notes/3num.pdf)






