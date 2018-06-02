
/// <summary>
///	  QR decomposition for a rectangular matrix.
/// </summary>
/// <remarks>
/// <para>
///   For an m-by-n matrix <c>A</c> with <c>m >= n</c>, the QR decomposition
///   is an m-by-n orthogonal matrix <c>Q</c> and an n-by-n upper triangular
///   matrix <c>R</c> so that <c>A = Q * R</c>.</para>
/// <para>
///   The QR decomposition always exists, even if the matrix does not have
///   full rank, so the constructor will never fail. The primary use of the
///   QR decomposition is in the least squares solution of nonsquare systems
///   of simultaneous linear equations.
///   This will fail if <see cref="FullRank"/> returns <see langword="false"/>.</para>  
/// </remarks>
/// 

//private double[,] qr;
//private double[] Rdiag;

/// <summary>Constructs a QR decomposition.</summary>	
/// <param name="a">The matrix A to be decomposed.</param>
/// <param name="transpose">True if the decomposition should be performed on
/// the transpose of A rather than A itself, false otherwise. Default is false.</param>
int QrDecomposition(double* a, int al, bool transpose, double* qr, int qrl, double* Rdiag, IDIM asz)
{
    int i, j, k, rows, cols;
    IDIM qrsz;
    if (a == NULL)
    {
        ASSERT(0 && "Matrix cannot be NULL.");
    }

    if ((!transpose && asz.len[0] < asz.len[1]) || (transpose && asz.len[1] < asz.len[0]))
    {
        ASSERT(0 && "Matrix has more columns than rows.");
    }

    qrsz = iDIM(asz.len[transpose], asz.len[!transpose]);
    if (transpose) {
      mem_Transpose(a, al, qr, qrl, asz, sizeof(double));
    } else {
      mem2_Copy(a, al, qr, qrl, asz, sizeof(double));
    }

    rows = qrsz.len[0];
    cols = qrsz.len[1];
    //Rdiag = new double[cols];

    for (k = 0; k < cols; k++)
    {
        // Compute 2-norm of k-th column without under/overflow.
        double nrm = 0;
        for (i = k; i < rows; i++)
        {
            nrm = Hypotenuse(nrm, qr[i*qrl+k]);
        }

        if (nrm != 0.0)
        {
            // Form k-th Householder vector.
            if (qr[k*qrl+k] < 0)
            {
                nrm = -nrm;
            }

            for (i = k; i < rows; i++)
            {
                qr[i*qrl+k] /= nrm;
            }

            qr[k*qrl+k] += 1.0;

            // Apply transformation to remaining columns.
            for (j = k + 1; j < cols; j++)
            {
                double s = 0.0;

                for (i = k; i < rows; i++)
                {
                    s += qr[i*qrl+k] * qr[i*qrl+j];
                }

                s = -s / qr[k*qrl+k];

                for (i = k; i < rows; i++)
                {
                    qr[i*qrl+j] += s * qr[i*qrl+k];
                }
            }
        }

        Rdiag[k] = -nrm;
    }
    return 0;
}

/// <summary>Shows if the matrix <c>A</c> is of full rank.</summary>
/// <a>The a is <see langword="true"/> if <c>R</c>, and hence <c>A</c>, has full rank.</a>
bool FullRank(const double* Rdiag, IDIM qrsz)
{
  int i;
    int columns = qrsz.len[1];
    
    for (i = 0; i < columns; i++)
    {
      if (Rdiag[i] == 0)
      {
        return false;
      }
    }
    
    return true;
  
}

/// <summary>Returns the upper triangular factor <c>R</c>.</summary>
int qrUpperTriangularFactor(const double* qr, int qrl, const double* Rdiag, double* x, int xl, IDIM qrsz)
{
  int i, j;
    int n = qrsz.len[1];
    //double[,] x = new double[n, n];
    
    for (i = 0; i < n; i++)
    {
      for (j = 0; j < n; j++)
      {
        if (i < j)
        {
          x[i*xl+j] = qr[i*qrl+j];
        }
        else if (i == j)
        {
          x[i*xl+j] = Rdiag[i];
        }
        else
        {
          x[i*xl+j] = 0.0;
        }
      }
    }
    
    return 0;
}

/// <summary>Returns the orthogonal factor <c>Q</c>.</summary>
int qrOrthogonalFactor(const double* qr, int qrl, double* x, int xl, IDIM qrsz)
{
  int i, j, k;
    int rows = qrsz.len[0];
    int cols = qrsz.len[1];
    //double[,] x = new double[rows, cols];
    
    for (k = cols - 1; k >= 0; k--)
    {
      for (i = 0; i < rows; i++)
      {
        x[i*xl+k] = 0.0;
      }
      
      x[k*xl+k] = 1.0;
      for (j = k; j < cols; j++)
      {
        if (qr[k*qrl+k] != 0)
        {
          double s = 0.0;
          
          for (i = k; i < rows; i++)
          {
            s += qr[i*qrl+k] * x[i*xl+j];
          }
          
          s = -s / qr[k*qrl+k];
          
          for (i = k; i < rows; i++)
          {
            x[i*xl+j] += s * qr[i*qrl+k];
          }
        }
      }
    }
    
    return 0;
}

/// <summary>Least squares solution of <c>A * x = B</c></summary>
/// <param name="a">Right-hand-side matrix with as many rows as <c>A</c> and any number of columns.</param>
/// <returns>A matrix that minimized the two norm of <c>Q * R * x - B</c>.</returns>
/// <exception cref="T:System.ArgumentException">Matrix row dimensions must be the same.</exception>
/// <exception cref="T:System.InvalidOperationException">Matrix is rank deficient.</exception>
int qrSolve(const double* a, int al, const double* qr, int qrl, const double* Rdiag, double* r, int rl, IDIM asz, IDIM qrsz)
{
    int i, j, k, m, n, count, xl;
    double* x=0;
    if (a == NULL) {
        ASSERT(0 && "Matrix cannot be NULL.");
    }

    if (asz.len[0] != qrsz.len[0]) {
        ASSERT(0 && "Matrix row dimensions must agree.");
    }

    if (!FullRank(Rdiag, qrsz)) {
        ASSERT(0 && "Matrix is rank deficient.");
    }

    // Copy right hand side
    count = asz.len[1];
    x = MALLOC(double, asz.len[0]*asz.len[1]);
    xl = asz.len[1];
    mem2_Copy(a, al, x, xl, asz, sizeof(double));
    //double[,] x = (double[,])a.Clone();
    m = qrsz.len[0];
    n = qrsz.len[1];

    // Compute Y = transpose(Q)*B
    for (k = 0; k < n; k++)
    {
        for (j = 0; j < count; j++)
        {
            double s = 0.0;

            for (i = k; i < m; i++)
                s += qr[i*qrl+k] * x[i*xl+j];

            s = -s / qr[k, k];

            for (i = k; i < m; i++)
                x[i*xl+j] += s * qr[i*qrl+k];
        }
    }

    // Solve R*x = Y;
    for (k = n - 1; k >= 0; k--)
    {
        for (j = 0; j < count; j++)
            x[k*xl+j] /= Rdiag[k];

        for (i = 0; i < k; i++)
            for (j = 0; j < count; j++)
                x[i*xl+j] -= x[k*xl+j] * qr[i*qrl+k];
    }

    //double[,] r = new double[n, count];
    for (i = 0; i < n; i++) {
      for (j = 0; j < count; j++) {
        r[i, j] = x[i*xl+j];
      }
    }
    
    FREE(x);
    return 0;
}

/// <summary>Least squares solution of <c>x * A = B</c></summary>
/// <param name="a">Right-hand-side matrix with as many columns as <c>A</c> and any number of rows.</param>
/// <returns>A matrix that minimized the two norm of <c>x * Q * R - B</c>.</returns>
/// <exception cref="T:System.ArgumentException">Matrix column dimensions must be the same.</exception>
/// <exception cref="T:System.InvalidOperationException">Matrix is rank deficient.</exception>
int qrSolveTranspose(const double* a, int al, const double* qr, int qrl, const double* Rdiag, double* r, int rl, IDIM asz, IDIM qrsz)
{
  int i, j, k, count, m, n, xl;
    double* x = 0;
    if (a == NULL)
        ASSERT(0 && "Matrix cannot be NULL.");

    if (asz.len[1] != qrsz.len[0])
        ASSERT(0 && "Matrix row dimensions must agree.");

    if (!FullRank(Rdiag, qrsz))
        ASSERT(0 && "Matrix is rank deficient.");

    // Copy right hand side
    count = asz.len[0];
    x = MALLOC(double, asz.len[1]*asz.len[0]);
    xl = asz.len[0];
    mem_Transpose(a, al, x, xl, asz, sizeof(double));
    m = qrsz.len[0];
    n = qrsz.len[1];

    // Compute Y = transpose(Q)*B
    for (k = 0; k < n; k++)
    {
        for (j = 0; j < count; j++)
        {
            double s = 0.0;

            for (i = k; i < m; i++)
                s += qr[i*qrl+k] * x[i*xl+j];

            s = -s / qr[k, k];

            for (i = k; i < m; i++)
                x[i*xl+j] += s * qr[i*qrl+k];
        }
    }

    // Solve R*x = Y;
    for (k = n - 1; k >= 0; k--)
    {
        for (j = 0; j < count; j++)
            x[k*xl+j] /= Rdiag[k];

        for (i = 0; i < k; i++)
            for (j = 0; j < count; j++)
                x[i*xl+j] -= x[k*xl+j] * qr[i*qrl+k];
    }

    //double[,] r = new double[count, n];
    for (i = 0; i < n; i++) {
        for (j = 0; j < count; j++) {
            r[j*rl+i] = x[i*xl+j];
        }
    }
    FREE(x);
    return 0;
}

/// <summary>Least squares solution of <c>A * x = B</c></summary>
/// <param name="a">Right-hand-side matrix with as many rows as <c>A</c> and any number of columns.</param>
/// <returns>A matrix that minimized the two norm of <c>Q * R * x - B</c>.</returns>
/// <exception cref="T:System.ArgumentException">Matrix row dimensions must be the same.</exception>
/// <exception cref="T:System.InvalidOperationException">Matrix is rank deficient.</exception>
int qrSolve2(const double* a, const double* qr, int qrl, const double* Rdiag, double* r, int rl, int an, IDIM qrsz)
{
    int i, k, m, n;
    double* x=0;
    if (a == NULL)
        ASSERT(0 && "a");

    if (an != qrsz.len[0])
        ASSERT(0 && "Matrix row dimensions must agree.");

    if (!FullRank(Rdiag, qrsz))
        ASSERT(0 && "Matrix is rank deficient.");

    // Copy right hand side
    //double[] x = (double[])a.Clone();
    x = MALLOC(double, an);
    m = qrsz.len[0];
    n = qrsz.len[1];

    // Compute Y = transpose(Q)*B
    for (k = 0; k < n; k++)
    {
        double s = 0.0;

        for (i = k; i < m; i++)
            s += qr[i*qrl+k] * x[i];

        s = -s / qr[k, k];

        for (i = k; i < m; i++)
            x[i] += s * qr[i*qrl+k];
    }

    // Solve R*x = Y;
    for (k = n - 1; k >= 0; k--)
    {
        x[k] /= Rdiag[k];

        for (i = 0; i < k; i++) {
            x[i] -= x[k] * qr[i*qrl+k];
        }
    }
    
    FREE(x);
    return 0;
}

/// <summary>Least squares solution of <c>A * x = I</c></summary>
int qrInverse(const double* qr, int qrl, IDIM qrsz, const double* Rdiag, double* x, int xl)
{
    int i, j, k, m, n;
    if (!FullRank(Rdiag, qrsz))
        ASSERT(0 && "Matrix is rank deficient.");

    // Copy right hand side
    m = qrsz.len[0];
    n = qrsz.len[1];
    xl = m;
    //x = MALLOC(double, m*m);

    // Compute Y = transpose(Q)
    for (k = n - 1; k >= 0; k--)
    {
        for (i = 0; i < m; i++)
            x[k*xl+i] = 0.0;

        x[k*xl+k] = 1.0;
        for (j = k; j < n; j++)
        {
            if (qr[k, k] != 0)
            {
                double s = 0.0;

                for (i = k; i < m; i++)
                    s += qr[i*qrl+k] * x[j*xl+i];

                s = -s / qr[k, k];

                for (i = k; i < m; i++)
                    x[j*xl+i] += s * qr[i*qrl+k];
            }
        }
    }

    // Solve R*x = Y;
    for (k = n - 1; k >= 0; k--)
    {
      for (j = 0; j < m; j++) {
            x[k*xl+j] /= Rdiag[k];
      }

        for (i = 0; i < k; i++) {
          for (j = 0; j < m; j++) {
                x[i*xl+j] -= x[k*xl+j] * qr[i*qrl+k];
          }
        }
    }

    return 0;
}

