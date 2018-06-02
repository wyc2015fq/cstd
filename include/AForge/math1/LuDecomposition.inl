
/// <summary>
///   LU decomposition of a rectangular matrix.
/// </summary>
/// <remarks>
///   <para>
///     For an m-by-n matrix <c>A</c> with <c>m >= n</c>, the LU decomposition is an m-by-n
///     unit lower triangular matrix <c>L</c>, an n-by-n upper triangular matrix <c>U</c>,
///     and a permutation vector <c>piv</c> of length m so that <c>A(piv) = L*U</c>.
///     If m &lt; n, then <c>L</c> is m-by-m and <c>U</c> is m-by-n.</para>
///   <para>
///     The LU decomposition with pivoting always exists, even if the matrix is
///     singular, so the constructor will never fail.  The primary use of the
///     LU decomposition is in the solution of square systems of simultaneous
///     linear equations. This will fail if <see cref="LuNonSingular"/> returns
///     <see langword="false"/>.
///   </para>
/// </remarks>
/// 

// double* lu;
// int pivotSign;
// int* pivotVector;


/// <summary>Construct a LU decomposition.</summary>	
/// <param name="a">The matrix A to be decomposed.</param>
/// <param name="transpose">True if the decomposition should be performed on
/// the transpose of A rather than A itself, false otherwise. Default is false.</param>
int LuDecomposition(const double* a, int al, bool transpose, double* lu, int lul, int* pivotVector, IDIM asz)
{
  typedef double arrtype;
    IDIM lusz;
    int i, j, k, p, rows=0, cols=0;
    int pivotSign=0;
    double* LUcolj=0;

    if ((!transpose && asz.len[0] < asz.len[1]) || (transpose && asz.len[1] < asz.len[0]))
    {
        ASSERT("Matrix has more cols than rows.");
    }

    lusz = iDIM(asz.len[transpose], asz.len[!transpose]);

    if (transpose) {
      mem_Transpose(a, al, lu, lul, asz, sizeof(arrtype));
    } else {
      mem2_Copy(a, al, lu, lul, asz, sizeof(arrtype));
    }
    rows = lusz.len[0];
    cols = lusz.len[1];

    //pivotVector = new int[rows];

    for (i = 0; i < rows; i++) {
        pivotVector[i] = i;
    }

    pivotSign = 1;
    LUcolj = MALLOC(double, rows);

    // Outer loop.
    for (j = 0; j < cols; j++)
    {
        // Make a copy of the j-th column to localize references.
        for (i = 0; i < rows; i++)
            LUcolj[i] = lu[i*lul+j];

        // Apply previous transformations.
        for (i = 0; i < rows; i++)
        {
            // Most of the time is spent in the following dot product.
            int kmax = MIN(i, j);
            double s = 0.0;
            for (k = 0; k < kmax; k++)
                s += lu[i*lul+k] * LUcolj[k];
            lu[i*lul+j] = LUcolj[i] -= s;
        }

        // Find pivot and exchange if necessary.
        p = j;
        for (i = j + 1; i < rows; i++)
        {
            if (fabs(LUcolj[i]) > fabs(LUcolj[p]))
                p = i;
        }

        if (p != j)
        {
            int v;
            for (k = 0; k < cols; k++)
            {
                double t = lu[p*lul+k];
                lu[p*lul+k] = lu[j*lul+k];
                lu[j*lul+k] = t;
            }

            v = pivotVector[p];
            pivotVector[p] = pivotVector[j];
            pivotVector[j] = v;

            pivotSign = -pivotSign;
        }

        // Compute multipliers.

        if (j < rows && lu[j*lul+j] != 0.0)
        {
            for (i = j + 1; i < rows; i++)
                lu[i*lul+j] /= lu[j*lul+j];
        }
    }
    return 0;
}

/// <summary>Returns if the matrix is non-singular.</summary>
bool LuNonSingular(const double* lu, int lul, IDIM lusz)
{
    int j;
    for (j = 0; j < lusz.len[1]; j++)
        if (lu[j*lul+j] == 0)
            return false;
    return true;
    
}

/// <summary>Returns the determinant of the matrix.</summary>
double LuDeterminant(const double* lu, int lul, IDIM lusz, int pivotSign)
{
  int j;
        double determinant = pivotSign;
    
    if (lusz.len[0] != lusz.len[1]) {
        ASSERT(0 && "Matrix must be square.");
    }
        for (j = 0; j < lusz.len[1]; j++)
            determinant *= lu[j*lul+j];
        return determinant;
    
}

/// <summary>Returns the lower triangular factor <c>L</c> with <c>A=LU</c>.</summary>
int LuLowerTriangularFactor(const double* lu, int lul, double* X, int xl, IDIM lusz)
{
  int i, j;
        int rows = lusz.len[0];
        int cols = lusz.len[1];

        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                if (i > j)
                    X[i*xl+j] = lu[i*lul+j];
                else if (i == j)
                    X[i*xl+j] = 1.0;
                else
                    X[i*xl+j] = 0.0;
            }
        }

        return 0;
}

/// <summary>Returns the lower triangular factor <c>L</c> with <c>A=LU</c>.</summary>
int LuUpperTriangularFactor(const double* lu, int lul, double* X, int xl, IDIM lusz)
{
  int i, j;
        int rows = lusz.len[0];
        int cols = lusz.len[1];

        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                if (i <= j)
                    X[i*xl+j] = lu[i*lul+j];
                else
                    X[i*xl+j] = 0.0;
            }
        }
        return 0;
}

/// <summary>Returns the pivot permuation vector.</summary>

/// <summary>Solves a set of equation systems of type <c>A * X = I</c>.</summary>
int LuInverse(const double* lu, int lul, const int* pivotVector, double* X, int xl, IDIM lusz)
{
    int i, j, k, rows, cols, count;
    if (!LuNonSingular(lu, lul, lusz))
    {
        ASSERT(0 && "Matrix is singular");
    }

    rows = lusz.len[1];
    cols = lusz.len[1];
    count = rows;

    // Copy right hand side with pivoting
    //double[,] X = new double[rows, cols];
    for (i = 0; i < rows; i++)
    {
        int k = pivotVector[i];
        X[i*xl+k] = 1.0;
    }

    // Solve L*Y = B(piv,:)
    for (k = 0; k < cols; k++)
        for (i = k + 1; i < cols; i++)
            for (j = 0; j < count; j++)
                X[i*xl+j] -= X[k*xl+j] * lu[i*lul+k];

    // Solve U*X = I;
    for (k = cols - 1; k >= 0; k--)
    {
        for (j = 0; j < count; j++)
            X[k*xl+j] /= lu[k, k];

        for (i = 0; i < k; i++)
            for (j = 0; j < count; j++)
                X[i*xl+j] -= X[k*xl+j] * lu[i*lul+k];
    }

    return 0;
}

/// <summary>Solves a set of equation systems of type <c>A * X = B</c>.</summary>
/// <param name="a">Right hand side matrix with as many rows as <c>A</c> and any number of cols.</param>
/// <returns>Matrix <c>X</c> so that <c>L * U * X = B</c>.</returns>
int LuSolve(const double* a, int al, const double* lu, int lul, const int* pivotVector, double* x, int xl, IDIM asz, IDIM lusz)
{
    int i, j, k, count, cols;
    if (!LuNonSingular(lu, lul, lusz))
    {
        ASSERT(0 && "Matrix is singular");
    }
    if (asz.len[0] != lusz.len[0])
    {
      ASSERT(0 && "Invalid matrix dimensions.");
    }

    // Copy right hand side with pivoting
    count = asz.len[1];
    //double[,] X = a.Submatrix(pivotVector, 0, count - 1);
    mem2vs_Submatrix(a, al, pivotVector, 0, x, xl, asz, sizeof(double));

    cols = lusz.len[1];

    // Solve L*Y = B(piv,:)
    for (k = 0; k < cols; k++)
        for (i = k + 1; i < cols; i++)
            for (j = 0; j < count; j++)
                x[i*xl+j] -= x[k*xl+j] * lu[i*lul+k];

    // Solve U*X = Y;
    for (k = cols - 1; k >= 0; k--)
    {
        for (j = 0; j < count; j++)
            x[k*xl+j] /= lu[k*lul+k];

        for (i = 0; i < k; i++)
            for (j = 0; j < count; j++)
                x[i*xl+j] -= x[k*xl+j] * lu[i*lul+k];
    }

    return 0;
}

/// <summary>Solves a set of equation systems of type <c>X * A = B</c>.</summary>
/// <param name="a">Right hand side matrix with as many cols as <c>A</c> and any number of rows.</param>
/// <returns>Matrix <c>X</c> so that <c>X * L * U = A</c>.</returns>
int luSolveTranspose(const double* a, int al, const double* lu, int lul, const int* pivotVector, double* X, int xl, IDIM asz, IDIM lusz)
{
  int i, j, k, cols, count;
    if (asz.len[0] != lusz.len[0])
    {
        ASSERT(0 && "Invalid matrix dimensions.");
    }
    
    if (!LuNonSingular(lu, lul, lusz))
    {
        ASSERT(0 && "Matrix is singular");
    }

    // Copy right hand side with pivoting
    //double[,] X = a.Submatrix(null, pivotVector);
    mem2sv_Submatrix(a, al, 0, pivotVector, X, xl, asz, sizeof(double));

    count = asz.len[0];
    cols = lusz.len[1];

    // Solve L*Y = B(piv,:)
    for (k = 0; k < cols; k++)
        for (i = k + 1; i < cols; i++)
            for (j = 0; j < count; j++)
                X[j*xl+i] -= X[j*xl+k] * lu[i*lul+k];

    // Solve U*X = Y;
    for (k = cols - 1; k >= 0; k--)
    {
        for (j = 0; j < count; j++)
            X[j*xl+k] /= lu[k*lul+k];

        for (i = 0; i < k; i++)
            for (j = 0; j < count; j++)
                X[j*xl+i] -= X[j*xl+k] * lu[i*lul+k];
    }

    return 0;
}

/// <summary>Solves a set of equation systems of type <c>A * X = B</c>.</summary>
/// <param name="a">Right hand side matrix with as many rows as <c>A</c> and any number of cols.</param>
/// <returns>Matrix <c>X</c> so that <c>L * U * X = B</c>.</returns>
double LuSolve_2(const double* a, int al, const double* lu, int lul, const int* pivotVector, double* x, int an, IDIM lusz)
{
    int i, j, rows, cols, count;
    double* b=0;
    if (an != lusz.len[0])
    {
        ASSERT(0 && "Invalid matrix dimensions.");
    }
    
    if (!LuNonSingular(lu, lul, lusz))
    {
        ASSERT(0 && "Matrix is singular");
    }

    // Copy right hand side with pivoting
    count = an;
    b = MALLOC(double, count);
    for (i = 0; i < count; i++) {
        b[i] = a[pivotVector[i]];
    }

    rows = lusz.len[1];
    cols = lusz.len[1];

    // Solve L*Y = B
    //double[] X = new double[count];
    for (i = 0; i < rows; i++)
    {
        x[i] = b[i];
        for (j = 0; j < i; j++)
            x[i] -= lu[i*lul+j] * x[j];
    }

    // Solve U*X = Y;
    for (i = rows - 1; i >= 0; i--)
    {
      //double sum = 0.0;
      for (j = cols - 1; j > i; j--) {
        x[i] -= lu[i*lul+j] * x[j];
      }
      x[i] /= lu[i*lul+i];
    }

    FREE(b);
    return 0;
}



