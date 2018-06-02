

/// <summary>
/// Static class Matrix. Defines a set of extension methods
/// that operates mainly on multidimensional arrays and vectors.
/// </summary>

int IsNaN(double x) {
  return _isnan(x);
}

// 矩阵乘法运算函数
int matrix_mutiply_AB(int row_c, int col_c, int col_a, const double* A, int al, const double* B, int bl, double* C, int cl)
{
  int i, j, k;
  
  for (i = 0; i < row_c; i++) {
    for (k = 0; k < col_a; k++) {
      for (j = 0; j < col_c; j++) {
        C[i * cl + j] += A[i * al + k] * B[k * bl + j];
      }
    }
  }
  
  return 0;
}
int matrix_mutiply_ATB(int row_c, int col_c, int col_a, const double* A, int al, const double* B, int bl, double* C, int cl)
{
  int i, j, k;
  
  for (k = 0; k < col_a; k++) {
    for (i = 0; i < row_c; i++) {
      for (j = 0; j < col_c; j++) {
        C[i * cl + j] += A[k * al + i] * B[k * bl + j];
      }
    }
  }
  
  return 0;
}

//n阶实对称方阵的cholesky分解
//A=r*r'
//只用了A的下三角
//计算出的r存在A的下三角
//成功返回0，失败返回非0值
int cholesky_decompose(int n, double* A, int al)
{
  int i, j, k;
  double sum;
  
  for (i = 0; i < n; i++) { //按行计算
    for (j = 0; j < i + 1; j++) { //只算下三角
      sum = A[i * al + j];
      
      for (k = 0; k < j; k++) {
        sum -= A[i * al + k] * A[j * al + k];
      }
      
      if (i == j) {
        if (sum < 0) {
          return (-1);
        }
        else {
          A[i * al + i] = sqrt(sum);
        }
      }
      else {
        A[i * al + j] = sum / A[j * al + j];
      }
    }
    
    for (j = i + 1; j < n; j++) { //上三角清零
      A[i * al + j] = 0;
    }
  }
  
  return (0);
}

// #region Comparison and Rounding
/// <summary>
///   Compares two matrices for equality, considering an acceptance threshold.
/// </summary>
static bool dm_IsEqual(double* a, int al, double* b, int bl, double threshold, IDIM asz)
{
  int i, j;
    for (i = 0; i < asz.len[0]; i++)
    {
      for (j = 0; j < asz.len[1]; j++)
        {
            double x = a[i*al+j], y = b[i*bl+j];

            if (fabs(x - y) > threshold || (IsNaN(x) ^ IsNaN(y)))
                return false;
        }
    }
    return true;
}

/// <summary>
///   Compares two matrices for equality, considering an acceptance threshold.
/// </summary>
static bool fm_IsEqual(float* a, int al, float* b, int bl, float threshold, IDIM asz)
{
  int i, j;
    for (i = 0; i < asz.len[0]; i++)
    {
      for (j = 0; j < asz.len[1]; j++)
        {
            float x = a[i*al+j], y = b[i*bl+j];

            if (fabs(x - y) > threshold || (IsNaN(x) ^ IsNaN(y)))
                return false;
        }
    }
    return true;
}

/// <summary>
///   Compares two vectors for equality, considering an acceptance threshold.
/// </summary>
static bool dv_IsEqual(double* a, double* b, double threshold, int an)
{
  int i;
    for (i = 0; i < an; i++)
    {
        if (fabs(a[i] - b[i]) > threshold)
            return false;
    }
    return true;
}

/// <summary>
///   Compares each member of a vector for equality with a scalar value x.
/// </summary>
static bool dvx_IsEqual(double* a, double x, int an)
{
  int i;
    for (i = 0; i < an; i++)
    {
        if (a[i] != x)
            return false;
    }
    return true;
}

/// <summary>
///   Compares each member of a matrix for equality with a scalar value x.
/// </summary>
static bool dmx_IsEqual(double* a, int al, double x, int n, IDIM asz)
{
  int i, j;
    for (i = 0; i < asz.len[0]; i++)
    {
      for (j = 0; j < asz.len[1]; j++)
        {
            if (a[i*al+j] != x)
                return false;
        }
    }
    return true;
}

/// <summary>
///   Compares two matrices for equality.
/// </summary>
static bool mem2_IsEqual(const void* a0, int al, const void* b0, int bl, IDIM asz)
{
    int i, j;
    const uchar* a = (const uchar*)a0;
    const uchar* b = (const uchar*)b0;
    for (i = 0; i < asz.len[0]; i++)
    {
        for (j = 0; j < asz.len[1]; j++)
        {
            if (a[i*al+j]!=b[i*bl+j])
                return false;
        }
    }
    return true;
}

/// <summary>Compares two vectors for equality.</summary>
static bool mem_IsEqual(const void* a0, const void* b0, int an)
{
  int i;
    const uchar* a = (const uchar*)a0;
    const uchar* b = (const uchar*)b0;
    for (i = 0; i < an; i++)
    {
        if (a[i]!=b[i])
            return false;
    }
    return true;
}



double d_dot(int n, const double* A, const double* B)
{
  int i;
  double sum = 0;
  
  for (i = 0; i < n; ++i) {
    sum += A[i] * B[i];
  }
  
  return sum;
}



// #region Matrix Algebra

static int mem2_Copy(const void* a0, int al, void* b0, int bl, IDIM asz, int cn) {
  int i, h = asz.len[0], w = asz.len[1];
  typedef uint8 arrtype;
  const arrtype* a = (const arrtype*)a0;
  arrtype* b = (arrtype*)b0;
  al*=cn, bl*=cn;
  w*=cn;
  for (i=0; i<h; ++i) {
    memcpy(b+i*bl, a+i*al, w);
  }
  return 0;
}

//#include "cv/cxmatrix.inl"
/// <summary>
///   Gets the transpose of a matrix.
/// </summary>
/// <param name="value">A matrix.</param>
/// <returns>The transpose of matrix m.</returns>
static int mem_Transpose(const void* a0, int al, void* b0, int bl, IDIM asz, int cn)
{
    int i, j, h = asz.len[0], w = asz.len[1];
    switch(cn) {
    case 1:
      {
        typedef uint8 arrtype;
#include "mem_Transpose_impl.inl"
      }
      break;
    case 2:
      {
        typedef uint16 arrtype;
#include "mem_Transpose_impl.inl"
      }
      break;
    case 3:
      {
        typedef struct {uint8 t[3];} arrtype;
#include "mem_Transpose_impl.inl"
      }
      break;
    case 4:
      {
        typedef uint32 arrtype;
#include "mem_Transpose_impl.inl"
      }
      break;
    case 8:
      {
        typedef struct {uint32 t[2];} arrtype;
#include "mem_Transpose_impl.inl"
      }
      break;
    case 16:
      {
        typedef struct {uint32 t[4];} arrtype;
#include "mem_Transpose_impl.inl"
      }
    break;
    default:ASSERT(0);break;
    }

    return 0;
}


/// <summary>
///   Multiplies two matrices.
/// </summary>
/// <param name="a">The left matrix.</param>
/// <param name="b">The right matrix.</param>
/// <param name="r">The matrix to store results.</param>
static void dmm_Multiply(const double* a, int al, const double* b, int bl, double* r, int rl, IDIM rsz, int n)
{
  int i, j, k;
  typedef double arrtype;
  double* Bcolj = MALLOC(arrtype, n);
  
  for (j = 0; j < rsz.len[1]; j++)
  {
    for (k = 0; k < n; k++) {
      Bcolj[k] = b[k*bl+j];
    }
    
    for (i = 0; i < rsz.len[0]; i++)
    {
      const arrtype* Arowi = a+i*al;
      double s = 0;
      for (k = 0; k < n; k++) {
        s += Arowi[k] * Bcolj[k];
      }
      r[i*rl+j] = s;
    }
  }
  FREE(Bcolj);
}

/// <summary>
///   Multiplies a row vector and a matrix.
/// </summary>
/// <param name="a">A row vector.</param>
/// <param name="b">A matrix.</param>
/// <returns>The product of the multiplication of the row vector and the matrix.</returns>
static int dvm_Multiply(const double* a, const double* b, int bl, double* r, IDIM bsz)
{
  int j, k;
  for (j = 0; j < bsz.len[1]; j++) {
    r[j] = 0;
  }
  for (j = 0; j < bsz.len[1]; j++) {
    for (k = 0; k < bsz.len[0]; k++) {
      r[j] += a[k] * b[k*bl+j];
    }
  }

  return 0;
}

/// <summary>
///   Multiplies a matrix and a vector (a*bT).
/// </summary>
/// <param name="a">A matrix.</param>
/// <param name="b">A column vector.</param>
/// <returns>The product of the multiplication of matrix a and column vector b.</returns>
static int dmv_Multiply(const double* a, int al, const double* b, double* r, IDIM asz)
{
    int i, j;
    for (i = 0; i < asz.len[0]; i++) {
      for (j = 0; j < asz.len[1]; j++) {
          r[i] += a[i*al+j] * b[j];
      }
    }

    return 0;
}

/// <summary>
///   Multiplies a matrix by a scalar.
/// </summary>
/// <param name="a">A matrix.</param>
/// <param name="x">A scalar.</param>
/// <returns>The product of the multiplication of matrix a and scalar x.</returns>
static int dms_Multiply(const double* a, int al, double x, double* r, int rl, IDIM asz)
{
    int i, j, rows, cols;
    rows = asz.len[0], cols = asz.len[1];

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = a[i*al+j] * x;

    return 0;
}

/// <summary>
///   Multiplies a vector by a scalar.
/// </summary>
/// <param name="a">A vector.</param>
/// <param name="x">A scalar.</param>
/// <returns>The product of the multiplication of vector a and scalar x.</returns>
static int dvs_Multiply(const double* a, double x, double* r, int an)
{
    int i;
    for (i = 0; i < an; i++)
        r[i] = a[i] * x;

    return 0;
}

// #region Division
/// <summary>
///   Divides a vector by a scalar.
/// </summary>
/// <param name="a">A vector.</param>
/// <param name="x">A scalar.</param>
/// <returns>The division quotient of vector a and scalar x.</returns>
static int dvs_Divide(const double* a, double x, double* r, int an)
{
    int i;
    for (i = 0; i < an; i++)
        r[i] = a[i] / x;

    return 0;
}

typedef struct {uint8 t[3];} uint24;

/// <summary>
///   Creates a index vector.
/// </summary>
static int Indexes(int from, int to, int* r)
{
  int i;
  for (i = 0; i < to - from; i++)
    r[i] = from++;
  return 0;
}

// #region Subsection and elements selection

/// <summary>Returns a sub matrix extracted from the current matrix.</summary>
/// <param name="data">The matrix to return the submatrix from.</param>
/// <param name="startRow">Start row index</param>
/// <param name="endRow">End row index</param>
/// <param name="startColumn">Start column index</param>
/// <param name="endColumn">End column index</param>
/// <remarks>
///   Routine adapted from Lutz Roeder's Mapack for .NET, September 2000.
/// </remarks>
static int mem2ss_Submatrix(const void* a0, int al, int startRow, int startCol, void* x0, int xl, IDIM xsz, int cn)
{
  int i, j;
#define MEM2_SUBMATRIX(arrtype)   { const arrtype* a = (const arrtype*)a0; arrtype* x = (arrtype*)x0; \
  for (i = 0; i < xsz.len[0]; i++){ for (j = 0; j < xsz.len[1]; j++){ \
  x[i*xl+j] = a[(i + startRow)*xl+(j + startCol)];  } } }

    //T* X = new T[endRow - startRow + 1, endColumn - startColumn + 1];
#include "mem2_Submatrix_impl.inl"
#undef MEM2_SUBMATRIX

    return 0;
}

/// <summary>Returns a sub matrix extracted from the current matrix.</summary>
/// <param name="a">The matrix to return the submatrix from.</param>
/// <param name="rowIndexes">Array of row indices. Pass NULL to select all indices.</param>
/// <param name="colIndexes">Array of column indices. Pass NULL to select all indices.</param>
/// <remarks>
///   Routine adapted from Lutz Roeder's Mapack for .NET, September 2000.
/// </remarks>
static int mem2vv_Submatrix(const void* a0, int al, const int* rowIndexes, const int* colIndexes, void* x0, int xl, IDIM xsz, int cn)
{
  int i, j;

#define MEM2_SUBMATRIX(arrtype)   { const arrtype* a = (const arrtype*)a0; arrtype* x = (arrtype*)x0; \
  for (i = 0; i < xsz.len[0]; i++) { for (j = 0; j < xsz.len[1]; j++) { \
  x[i*xl+j] = a[rowIndexes[i]*al+colIndexes[j]]; }  } }

  //T* X = new T[endRow - startRow + 1, endColumn - startColumn + 1];
#include "mem2_Submatrix_impl.inl"
#undef MEM2_SUBMATRIX

  return 0;
}

/// <summary>Returns a sub matrix extracted from the current matrix.</summary>
/// <param name="a">The matrix to return the submatrix from.</param>
/// <param name="rowIndexes">Array of row indices</param>
/// <remarks>
///   Routine adapted from Lutz Roeder's Mapack for .NET, September 2000.
/// </remarks>
// int mem2ss_Submatrix(const void* a0, int al, int startRow, int startCol, void* x0, int xl, IDIM xsz, int cn)
//static int mem2vv_Submatrix(const void* a0, int al, const int* rowIndexes, const int* colIndexes, void* x0, int xl, IDIM xsz, int cn)
static int mem2vs_Submatrix(const void* a0, int al, const int* rowIndexes, int startCol, void* x0, int xl, IDIM xsz, int cn)
{
  int i, j;
  
#define MEM2_SUBMATRIX(arrtype)   { const arrtype* a = (const arrtype*)a0; arrtype* x = (arrtype*)x0; \
  for (i = 0; i < xsz.len[0]; i++) { for (j = 0; j < xsz.len[1]; j++) { \
  x[i*xl+j] = a[rowIndexes[i]*al+startCol+j]; }  } }
  
#include "mem2_Submatrix_impl.inl"
#undef MEM2_SUBMATRIX

    return 0;
}

static int mem2sv_Submatrix(const void* a0, int al, int startRow, const int* colIndexes, void* x0, int xl, IDIM xsz, int cn)
{
  int i, j;
  
#define MEM2_SUBMATRIX(arrtype)   { const arrtype* a = (const arrtype*)a0; arrtype* x = (arrtype*)x0; \
  for (i = 0; i < xsz.len[0]; i++) { for (j = 0; j < xsz.len[1]; j++) { \
  x[i*xl+j] = a[(startRow+i)*al+colIndexes[j]]; }  } }
  
#include "mem2_Submatrix_impl.inl"
#undef MEM2_SUBMATRIX
  
  return 0;
}

/// <summary>Returns a subvector extracted from the current vector.</summary>
/// <param name="a">The vector to return the subvector from.</param>
/// <param name="indexes">Array of indices.</param>
/// <remarks>
///   Routine adapted from Lutz Roeder's Mapack for .NET, September 2000.
/// </remarks>
static int memv_Subvector(const void* a0, const int* indexes, void* x0, int xn, int cn)
{
#define MEM2_SUBMATRIX(arrtype)   { int i; const arrtype* a = (const arrtype*)a0; arrtype* x = (arrtype*)x0; \
  for (i = 0; i < xn; i++) { x[i] = a[indexes[i]]; } }

#include "mem2_Submatrix_impl.inl"
#undef MEM2_SUBMATRIX
    return 0;
}

/// <summary>Returns a sub matrix extracted from the current matrix.</summary>
/// <param name="a">The vector to return the subvector from.</param>
/// <param name="i0">Starting index.</param>
/// <param name="i1">End index.</param>
/// <remarks>
///   Routine adapted from Lutz Roeder's Mapack for .NET, September 2000.
/// </remarks>
static int mems_Subvector(const void* a0, int i0, void* x0, int xn, int cn)
{
#define MEM2_SUBMATRIX(arrtype)  { int i; const arrtype* a = (const arrtype*)a0; arrtype* x = (arrtype*)x0; \
    for (i = 0; i < xn; i++) { x[i] = a[i0+i]; } }
    
#include "mem2_Submatrix_impl.inl"
#undef MEM2_SUBMATRIX
    return 0;
}

#include "mathTools.inl"
#include "LuDecomposition.inl"
#include "QrDecomposition.inl"

/// <summary>
///   Divides two matrices by multiplying A by the inverse of B.
/// </summary>
/// <param name="a">The first matrix.</param>
/// <param name="b">The second matrix (which will be inverted).</param>
/// <returns>The result from the division of a and b.</returns>
static int dmm_Divide(double* a, int al, double* b, int bl, double* r, int rl, IDIM asz, IDIM bsz)
{
  //return a.Multiply(b.Inverse());
  
  if (asz.len[0] == asz.len[1])
  {
    // Solve by LU Decomposition if matrix is square.
    enum {transpose=true};
    IDIM lusz = iDIM(asz.len[transpose], asz.len[!transpose]);
    double* lu = MALLOC(double, lusz.len[0]*lusz.len[1]);
    int* pivotVector = MALLOC(int, lusz.len[0]);
    int lul = lusz.len[1];
    LuDecomposition(b, bl, transpose, lu, lul, pivotVector, bsz);
    luSolveTranspose(a, al, lu, lul, pivotVector, r, rl, asz, lusz);
    FREE(lu);
    FREE(pivotVector);
    return 0;
  }
  else
  {
    // Solve by QR Decomposition if not.
    enum {transpose=true};
    IDIM qrsz = iDIM(asz.len[transpose], asz.len[!transpose]);
    double* qr = MALLOC(double, qrsz.len[0]*qrsz.len[1]);
    double* Rdiag = MALLOC(double, qrsz.len[1]);
    int qrl = qrsz.len[1];
    QrDecomposition(b, bl, true, qr, qrl, Rdiag, bsz);
    qrSolveTranspose(a, al, qr, qrl, Rdiag, r, rl, asz, qrsz);
    FREE(qr);
    FREE(Rdiag);
    return 0;
  }
}

/// <summary>
///   Divides a matrix by a scalar.
/// </summary>
/// <param name="a">A matrix.</param>
/// <param name="x">A scalar.</param>
/// <returns>The division quotient of matrix a and scalar x.</returns>
static int dms_Divide(const double* a, int al, double x, double* r, int rl, IDIM asz)
{
  int i, j;
    int rows = asz.len[0];
    int cols = asz.len[1];

    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        r[i*rl+j] = a[i*al+j] / x;
      }
    }

    return 0;
}

// #endregion

// #region Products
/// <summary>
///   Gets the inner product (scalar product) between two vectors (aT*b).
/// </summary>
/// <param name="a">A vector.</param>
/// <param name="b">A vector.</param>
/// <returns>The inner product of the multiplication of the vectors.</returns>
/// <remarks>
///    In mathematics, the dot product is an algebraic operation that takes two
///    equal-length sequences of numbers (usually coordinate vectors) and returns
///    a single number obtained by multiplying corresponding entries and adding up
///    those products. The name is derived from the dot that is often used to designate
///    this operation; the alternative name scalar product emphasizes the scalar
///    (rather than vector) nature of the result.
///    
///    The principal use of this product is the inner product in a Euclidean vector space:
///    when two vectors are expressed on an orthonormal basis, the dot product of their 
///    coordinate vectors gives their inner product.
/// </remarks>
static double InnerProduct(const double* a, const double* b, int an)
{
    double r = 0.0;
    //if (an != bn) ASSERT("Vector dimensions must match", "b");
    int i;
    for (i = 0; i < an; i++)
        r += a[i] * b[i];

    return r;
}

/// <summary>
///   Gets the outer product (matrix product) between two vectors (a*bT).
/// </summary>
/// <remarks>
///   In linear algebra, the outer product typically refers to the tensor
///   product of two vectors. The result of applying the outer product to
///   a pair of vectors is a matrix. The name contrasts with the inner product,
///   which takes as input a pair of vectors and produces a scalar.
/// </remarks>
static int OuterProduct(const double* a, const double* b, double* r, int rl, IDIM rsz)
{
  int i, j;

    for (i = 0; i < rsz.len[0]; i++)
        for (j = 0; j < rsz.len[1]; j++)
            r[i*rl+j] += a[i] * b[j];

    return 0;
}

/// <summary>
///   Vectorial product.
/// </summary>
/// <remarks>
///   The cross product, vector product or Gibbs vector product is a binary operation
///   on two vectors in three-dimensional space. It has a vector result, a vector which
///   is always perpendicular to both of the vectors being multiplied and the plane
///   containing them. It has many applications in mathematics, engineering and physics.
/// </remarks>
static int dVectorProduct(const double* a, const double* b, double* r)
{
    
      r[0] = a[1]*b[2] - a[2]*b[1];
      r[1] = a[2]*b[0] - a[0]*b[2];
      r[2] = a[0]*b[1] - a[1]*b[0];
    return 0;
}


// #endregion

// #region Addition and Subraction
/// <summary>
///   Adds two matrices.
/// </summary>
/// <param name="a">A matrix.</param>
/// <param name="b">A matrix.</param>
/// <returns>The sum of the two matrices a and b.</returns>
static int dmm_Add(const double* a, int al, const double* b, int bl, double* r, int rl, IDIM asz)
{
  int i, j;
    for (i = 0; i < asz.len[0]; i++)
        for (j = 0; j < asz.len[1]; j++)
            r[i*rl+j] = a[i*al+j] + b[i*bl+j];

    return 0;
}

/// <summary>
///   Adds a vector to a column or row of a matrix.
/// </summary>
/// <param name="a">A matrix.</param>
/// <param name="b">A vector.</param>
/// <param name="dimension">
///   Pass 0 if the vector should be added row-wise, 
///   or 1 if the vector should be added column-wise.
/// </param>
static int dmv_Add(const double* a, int al, const double* b, int dimension, double* r, int rl, IDIM asz)
{
    int i, j;
    int rows = asz.len[0];
    int cols = asz.len[1];

    if (dimension == 1)
    {
        //if (rows != bn) ASSERT("Length of B should equal the number of rows in A");
        
        for (i = 0; i < rows; i++)
          for (j = 0; j < cols; j++)
                r[i*rl+j] = a[i*al+j] + b[i];
    }
    else
    {
        //if (cols != bn) ASSERT("Length of B should equal the number of cols in A");

        for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
                r[i*rl+j] = a[i*al+j] + b[j];
    }

    return 0;
}

/// <summary>
///   Adds a vector to a column or row of a matrix.
/// </summary>
/// <param name="a">A matrix.</param>
/// <param name="b">A vector.</param>
/// <param name="dimension">The dimension to add the vector to.</param>
static int dmv_Subtract(const double* a, int al, const double* b, int dimension, double* r, int rl, IDIM asz)
{
    int i, j;
    int rows = asz.len[0];
    int cols = asz.len[1];
    if (dimension == 1)
    {
        //if (rows != bn) ASSERT("Length of B should equal the number of rows in A", "b");

        for (j = 0; j < cols; j++)
            for (i = 0; i < rows; i++)
                r[i*rl+j] = a[i*al+j] - b[i];
    }
    else
    {
        //if (cols != bn) ASSERT("Length of B should equal the number of cols in A", "b");

        for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
                r[i*rl+j] = a[i*al+j] - b[j];
    }

    return 0;
}

/// <summary>
///   Adds two vectors.
/// </summary>
/// <param name="a">A vector.</param>
/// <param name="b">A vector.</param>
/// <returns>The addition of vector a to vector b.</returns>
static int dvv_Add(const double* a, const double* b, double* r, int an)
{
  int i;
    //if (an != bn) ASSERT("Vector lengths must match", "b");

    for (i = 0; i < an; i++)
        r[i] = a[i] + b[i];

    return 0;
}

/// <summary>
///   Subtracts two matrices.
/// </summary>
/// <param name="a">A matrix.</param>
/// <param name="b">A matrix.</param>
/// <returns>The subtraction of matrix b from matrix a.</returns>
static int dmm_Subtract(const double* a, int al, const double* b, int bl, double* r, int rl, IDIM asz)
{
  int i, j;
    //if (asz.len[0] != b.GetLength(0) || asz.len[1] != b.GetLength(1)) ASSERT("Matrix dimensions must match", "b");

  int rows = asz.len[0];
    int cols = asz.len[1];

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = a[i*al+j] - b[i*bl+j];

    return 0;
}

/// <summary>
///   Subtracts two vectors.
/// </summary>
/// <param name="a">A vector.</param>
/// <param name="b">A vector.</param>
/// <returns>The subtraction of vector b from vector a.</returns>
static int dvv_Subtract(const double* a, const double* b, double* r, int an)
{
  int i;
    //if (an != bn) ASSERT("Vector length must match", "b");

  for (i = 0; i < an; i++)
        r[i] = a[i] - b[i];

    return 0;
}

/// <summary>
///   Subtracts a scalar from a vector.
/// </summary>
/// <param name="a">A vector.</param>
/// <param name="b">A scalar.</param>
/// <returns>The subtraction of b from all elements in a.</returns>
static int dvs_Subtract(const double* a, double b, double* r, int an)
{
  int i;
    for (i = 0; i < an; i++)
        r[i] = a[i] - b;

    return 0;
}
// #endregion


/// <summary>
///   Multiplies a matrix by itself n times.
/// </summary>
static int Power(const double* a, int al, int k, double* r, int rl, int n)
{
  int i, bl;
  double* b=0;
  IDIM asz = iDIM(n, n);
  //if (!matrix.IsSquare()) ASSERT("Matrix must be square", "matrix");

  if (0>=k) {
    return 0;
  }

  b = MALLOC(double, n*n);
  bl = n;
    // TODO: This is a very naive implementation and should be optimized.
  mem2_Copy(a, al, b, bl, asz, sizeof(double));
  for (i = 1; i < k; i++) {
    dmm_Multiply(a, al, b, bl, r, rl, asz, n);
    mem2_Copy(r, rl, b, bl, asz, sizeof(double));
  }

  FREE(b);
  return 0;
}
// #endregion


double drandnext() {
  return rand()*1./RAND_MAX;
}

// #region Matrix Construction

/// <summary>
///   Creates a rows-by-cols matrix with uniformly distributed random data.
/// </summary>
static int dm_Random(int size, bool symmetric, double minValue, double maxValue, double* r, int rl)
{
    int i, j;
    if (!symmetric)
    {
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                r[i*rl+j] = drandnext() * (maxValue - minValue) + minValue;
    }
    else
    {
        for (i = 0; i < size; i++)
        {
            for (j = i; j < size; j++)
            {
                r[i*rl+j] = drandnext() * (maxValue - minValue) + minValue;
                r[j, i] = r[i*rl+j];
            }
        }
    }

    return 0;
}

/// <summary>
///   Creates a rows-by-cols matrix with uniformly distributed random data.
/// </summary>
static int dm_Random2(int rows, int cols, double minValue, double maxValue, double* r, int rl)
{
    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = drandnext() * (maxValue - minValue) + minValue;

    return 0;
}

/// <summary>
///   Creates a vector with uniformly distributed random data.
/// </summary>
static double* dv_Random(int size, double minValue, double maxValue, double* r)
{
    int i;
    for (i = 0; i < size; i++)
        r[i] = drandnext() * (maxValue - minValue) + minValue;

    return 0;
}

/// <summary>
///   Creates a magic square matrix.
/// </summary>
static int dMagic(int size, double* r, int rl)
{
    int i, j;
    if (size < 3)  ASSERT("The square size must be greater or equal to 3.");

    // First algorithm: Odd order
    if ((size % 2) == 1)
    {
        int a = (size + 1) / 2;
        int b = (size + 1);

        for (j = 0; j < size; j++)
            for (i = 0; i < size; i++)
                r[i*rl+j] = size * ((i + j + a) % size) + ((i + 2 * j + b) % size) + 1;
    }

    // Second algorithm: Even order (double)
    else if ((size % 4) == 0)
    {
        for (j = 0; j < size; j++)
            for (i = 0; i < size; i++)
                if (((i + 1) / 2) % 2 == ((j + 1) / 2) % 2)
                    r[i*rl+j] = size * size - size * i - j;
                else
                    r[i*rl+j] = size * i + j + 1;
    }

    // Third algorithm: Even order (single)
    else
    {
        int n = size / 2;
        int p = (size - 2) / 4;
        double t;

        double* a = MALLOC(double, n*n);
        int al = n;
        dMagic(n, a, al);

        for (j = 0; j < n; j++)
        {
            for (i = 0; i < n; i++)
            {
                double e = a[i*al+j];
                r[i*rl+j] = e;
                r[i*rl+(j + n)] = e + 2 * n * n;
                r[(i + n)*rl+j] = e + 3 * n * n;
                r[(i + n)*rl+(j + n)] = e + n * n;
            }
        }

        for (i = 0; i < n; i++)
        {
            // Swap M[i,j] and M[i+n,j]
            for (j = 0; j < p; j++)
            {
                t = r[i*rl+j];
                r[i*rl+j] = r[(i + n)*rl+j];
                r[(i + n)*rl+j] = t;
            }
            for (j = size - p + 1; j < size; j++)
            {
                t = r[i*rl+j];
                r[i*rl+j] = r[(i + n)*rl+j];
                r[(i + n)*rl+j] = t;
            }
        }

        // Continue swaping in the boundary
        t = r[p*rl+0];
        r[p*rl+0] = r[(p + n)*rl+0];
        r[(p + n)*rl+0] = t;

        t = r[p*rl+p];
        r[p*rl+p] = r[(p + n)*rl+p];
        r[(p + n)*rl+p] = t;
        FREE(a);
    }

    return 0; // return the magic square.
}

/// <summary>
///   Gets the diagonal vector from a matrix.
/// </summary>
/// <param name="m">A matrix.</param>
/// <returns>The diagonal vector from matrix m.</returns>
static int mv_Diagonal(const void* a0, int al, void* x0, int an, int cn)
{
#define MEM2_SUBMATRIX(arrtype) { const arrtype* a = (const arrtype*)a0; arrtype* x = (arrtype*)x0; \
  int i; for (i = 0; i < an; i++) { x[i] = a[i*al+i]; } }

#include "mem2_Submatrix_impl.inl"
#undef MEM2_SUBMATRIX
    return 0;
}

/// <summary>
///   Returns a square diagonal matrix of the given size.
/// </summary>
static int dms_Diagonal(int size, double value, double* m, int ml)
{
  int i;
    for (i = 0; i < size; i++)
        m[i*ml+i] = value;
    
    return 0;
}

/// <summary>
///   Return a square matrix with a vector of values on its diagonal.
/// </summary>
static int dmv_Diagonal(int size, const double* values, double* m, int ml)
{
  int i;
    for (i = 0; i < size; i++)
        m[i*ml+i] = values[i];

    return 0;
}

/// <summary>
///   Returns a matrix with all elements set to a given value.
/// </summary>
static int dm_Create(int rows, int cols, double value, double* m, int ml)
{
  int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            m[i*ml+j] = value;

    return 0;
}

/// <summary>
///   Expands a data vector given in summary form.
/// </summary>
/// <param name="data">A base vector.</param>
/// <param name="count">An array containing by how much each line should be replicated.</param>
/// <returns></returns>
static int  dExpand(const double* data, const int* count, int count_Length, double* expansion)
{
  int i, j, k=0;
    for (i = 0; i < count_Length; i++)
        for (j = 0; j < count[i]; j++)
            expansion[k++] = (data[i]);

    return 0;
}

/// <summary>
///   Returns the Identity matrix of the given size.
/// </summary>
static int dm_Identity(int size, double* m, int ml)
{
    return dms_Diagonal(size, 1.0, m, ml);
}

/// <summary>
///   Creates a centering matrix of size n x n in the form (I - 1n)
///   where 1n is a matrix with all entries 1/n.
/// </summary>
static int dm_Centering(int size, double* r, int rl)
{
    int i;
    dm_Create(size, size, -1.0 / size, r, rl);

    for (i = 0; i < size; i++)
        r[i*rl+i] = 1.0 - 1.0 / size;

    return 0;
}

/// <summary>
///   Creates an interval vector.
/// </summary>
static int iv_Interval(int from, int* r, int rn)
{
    int i;
    for (i = 0; i < rn; i++)
        r[i] = from++;
    return 0;
}

/// <summary>
///   Creates an interval vector.
/// </summary>
static int dv_Interval(double from, double stepSize, double* r, int rn)
{
  int i;
    for (i = 0; i < rn; i++)
        r[i] = from + i * stepSize;

    return 0;
}

/// <summary>
///   Combines two vectors horizontally.
/// </summary>
static int vv_Combine(const void* a1, int a1n, const void* a2, int a2n, void* r, int cn)
{
  memcpy(r, a1, a1n*cn);
  memcpy((uchar*)r+a1n*cn, a2, a2n*cn);
  return 0;
}

/// <summary>
///   Combine vectors horizontally.
/// </summary>
static int vn_Combine(const void** vectors, const int* vn, int n, void* r0, int cn)
{
    int i = 0, c;
    uchar* r = (uchar*)r0;
    for (i = 0; i < n; i++) {
      c = vn[i]*cn;
      memcpy(r, vectors[i], c);
      r += c;
    }

    return 0;
}

/// <summary>
///   Combines matrices vertically.
/// </summary>
static int mmn_Combine(const void** a, const IDIM* asz, const int* al, int n, void* r0, int rl, int cn)
{
    int i, rows = 0, cols = 0;
    uchar* r = (uchar*)r0;

    for (i = 0; i < n; i++)
    {
        rows += asz[i].len[0];
        if (asz[i].len[1] > cols)
            cols = asz[i].len[1];
    }

    for (i = 0; i < n; i++)
    {
        mem2_Copy(a[i], al[i], r, rl*cn, asz[i], cn);
        r += rl*cn;
    }

    return 0;
}

/// <summary>
///   Returns a new matrix without one of its columns.
/// </summary>
static int mm_RemoveColumn(const void* a0, int al, int index, void* r0, int rl, IDIM asz, int cn)
{
    int i, k0, k1;
    const uchar* a = (const uchar*)a0;
    uchar* r = (uchar*)r0;
    al*=cn, rl*=cn;
    k0 = index*cn;
    k1 = (asz.len[1]-1-index)*cn;
    for (i = 0; i < asz.len[0]; i++)
    {
        memcpy(r, a, k0);
        memcpy(r+k0, a+k0+cn, k1);
        a+=al, r+=rl;
    }
    return 0;
}

/// <summary>
///   Returns a new matrix with a given column vector inserted at a given index.
/// </summary>
static int mmm_InsertColumn(const void* a0, int al, const void* b0, int bl, int index, void* r0, int rl, IDIM asz, int cn)
{
  int i, k0, k1;
  const uchar* a = (const uchar*)a0;
  const uchar* b = (const uchar*)b0;
  uchar* r = (uchar*)r0;
  al*=cn, rl*=cn, bl*=cn;
  k0 = index*cn;
  k1 = (asz.len[1]-index)*cn;
  for (i = 0; i < asz.len[0]; i++)
  {
    memcpy(r, a, k0);
    memcpy(r+k0, b, cn);
    memcpy(r+k0+cn, a+k0, k1);
    a+=al, r+=rl, b+=bl;
  }
  
  return 0;
}

/// <summary>
///   Removes an element from a vector.
/// </summary>
static int v_RemoveAt(const void* a0, int index, void* r0, int an, int cn)
{
  int k0, k1;
  const uchar* a = (const uchar*)a0;
  uchar* r = (uchar*)r0;
  k0 = index*cn;
  k1 = (an-index)*cn;
  memcpy(r, a, k0);
  memcpy(r+k0, a+k0+cn, k1);
  return 0;
}


#if 00
/// <summary>
///   Gets the indices of all elements matching a certain criteria.
/// </summary>
/// <typeparam name="T">The type of the array.</typeparam>
/// <param name="a">The array to search inside.</param>
/// <param name="func">The search criteria.</param>
static int* Find(const* a, Func<T, bool> func)
{
    return Find(a, func, false);
}

/// <summary>
///   Gets the indices of all elements matching a certain criteria.
/// </summary>
/// <typeparam name="T">The type of the array.</typeparam>
/// <param name="a">The array to search inside.</param>
/// <param name="func">The search criteria.</param>
/// <param name="firstOnly">
///    Set to true to stop when the first element is
///    found, set to false to get all elements.
/// </param>
static int* Find<T>(T* a, Func<T, bool> func, bool firstOnly)
{
    List<int> idx = new List<int>();
    for (i = 0; i < datan; i++)
    {
        if (func(a[i]))
        {
            if (firstOnly)
                return new int* { i };
            else idx.Add(i);
        }
    }
    return idx.ToArray();
}

/// <summary>
///   Gets the indices of all elements matching a certain criteria.
/// </summary>
/// <typeparam name="T">The type of the array.</typeparam>
/// <param name="a">The array to search inside.</param>
/// <param name="func">The search criteria.</param>
static int* Find<T>(T* a, Func<T, bool> func)
{
    return Find(a, func, false);
}

/// <summary>
///   Gets the indices of all elements matching a certain criteria.
/// </summary>
/// <typeparam name="T">The type of the array.</typeparam>
/// <param name="a">The array to search inside.</param>
/// <param name="func">The search criteria.</param>
/// <param name="firstOnly">
///    Set to true to stop when the first element is
///    found, set to false to get all elements.
/// </param>
static int* Find<T>(T* a, Func<T, bool> func, bool firstOnly)
{
    List<int*> idx = new List<int*>();
    for (i = 0; i < asz.len[0]; i++)
    {
        for (j = 0; j < asz.len[1]; j++)
        {
            if (func(a[i*al+j]))
            {
                if (firstOnly)
                    return new int* { new int* { i, j } };
                else idx.Add(new int* { i, j });
            }
        }
    }
    return idx.ToArray();
}

/// <summary>
///   Applies a function to every element of the array.
/// </summary>
static TResult* Apply<TData, TResult>(TData* a, Func<TData, TResult> func)
{
    var r = new TResult[datan];
    for (i = 0; i < datan; i++)
        r[i] = func(a[i]);
    return 0;
}

/// <summary>
///   Applies a function to every element of a matrix.
/// </summary>
static TResult* Apply<TData, TResult>(TData* a, Func<TData, TResult> func)
{
    int rows = asz.len[0];
    int cols = asz.len[1];

    var r = new TResult[rows, cols];
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = func(a[i*al+j]);

    return 0;
}


/// <summary>
///   Applies a function to every element of the array.
/// </summary>
static void ApplyInPlace<TData>(TData* a, Func<TData, TData> func)
{
    for (i = 0; i < datan; i++)
        a[i] = func(a[i]);
}

/// <summary>
///   Applies a function to every element of the array.
/// </summary>
static void ApplyInPlace<TData>(TData* a, Func<TData, int, TData> func)
{
    for (i = 0; i < datan; i++)
        a[i] = func(a[i], i);
}

/// <summary>
///   Sorts the columns of a matrix by sorting keys.
/// </summary>
/// <param name="keys">The key value for each column.</param>
/// <param name="values">The matrix to be sorted.</param>
/// <param name="comparer">The comparer to use.</param>
static TValue* Sort<TKey, TValue>(TKey* keys, TValue* values, IComparer<TKey> comparer)
{
    int* indices = new int[keys.Length];
    for (i = 0; i < keys.Length; i++) indices[i] = i;

    Array.Sort<TKey, int>(keys, indices, comparer);

    return values.Submatrix(0, values.GetLength(0) - 1, indices);
}

#endif

/// <summary>
///   Returns true if a matrix is symmetric.
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
static bool dm_IsSymmetric(int n, const double* a, int al)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j <= i; j++)
        {
            if (a[i*al+j] != a[j*al+i])
                return false;
        }
    }
    return true;
}

/// <summary>
///   Gets the trace of a matrix.
/// </summary>
/// <remarks>
///   The trace of an n-by-n square matrix A is defined to be the sum of the
///   elements on the main diagonal (the diagonal from the upper left to the
///   lower right) of A.
/// </remarks>
static double dm_Trace(int n, const double* a, int al)
{
    int i;
    double trace = 0.0;
    for (i = 0; i < n; i++) {
        trace += a[i*al+i];
    }
    return trace;
}

#include "cv/matrix.inl"
/// <summary>
///   Gets the determinant of a matrix.
/// </summary>
static double Determinant(int n, const double* a, int al)
{
    //return new LuDecomposition(m).Determinant;
    return cvDet_64f(n, a, al);
}

/// <summary>
///    Gets whether a matrix is positive definite.
/// </summary>
static bool IsPositiveDefinite(int n, double* m, int al)
{
  ASSERT(0);
    //return new CholeskyDecomposition(m).PositiveDefinite;
  return 0;
}

/// <summary>Calculates a vector cumulative sum.</summary>
static int dv_CumulativeSum(int n, const double* value, double* sum)
{
    int i;
    sum[0] = value[0];
    for (i = 1; i < n; i++) {
        sum[i] += sum[i - 1] + value[i];
    }
    return 0;
}



/// <summary>Calculates the matrix Sum vector.</summary>
/// <param name="value">A matrix whose sums will be calculated.</param>
/// <param name="dimension">The dimension in which the cumulative sum will be calculated.</param>
/// <returns>Returns a vector containing the sums of each variable in the given matrix.</returns>
static int dm_CumulativeSum(const double* a, int al, int dimension, double* sum, int sl, IDIM asz)
{
    int i, j;
    SKIP_FILL(asz.len[0], asz.len[1], sum, sl, 1, 0);

    if (dimension == 1)
    {
        for (j = 0; j < asz.len[1]; j++) {
          sum[j] = a[j];
        }

        // for each row
        for (i = 1; i < asz.len[0]; i++)
        {
            // for each column
          for (j = 0; j < asz.len[1]; j++) {
            sum[i*sl+j] += sum[(i - 1)*sl+j] + a[i*al+j];
          }
        }
    }
    else if (dimension == 0)
    {
        for (j = 0; j < asz.len[0]; j++) {
          sum[j*sl] = a[j*al];
        }

        // for each column
        for (i = 1; i < asz.len[1]; i++)
        {
            // for each row
            for (j = 0; j < asz.len[0]; j++) {
                sum[j*sl+i] += sum[j*sl+(i - 1)] + a[j*al+i];
            }
        }
    }
    else
    {
        ASSERT(0 && "Invalid dimension");
    }

    return 0;
}

/// <summary>Calculates the matrix Sum vector.</summary>
/// <param name="value">A matrix whose sums will be calculated.</param>
/// <param name="dimension">The dimension in which the sum will be calculated.</param>
/// <returns>Returns a vector containing the sums of each variable in the given matrix.</returns>
static int dm_Sum(const double* a, int al, int dimension, double* sum, int sl, IDIM asz)
{
  double s=0;
#include "impl/m_Sum.inl"
  return 0;
}

/// <summary>Calculates the matrix Sum vector.</summary>
/// <param name="value">A matrix whose sums will be calculated.</param>
/// <param name="dimension">The dimension in which the sum will be calculated.</param>
/// <returns>Returns a vector containing the sums of each variable in the given matrix.</returns>
static int im_Sum(const int* a, int al, int dimension, int* sum, int sl, IDIM asz)
{
  int s=0;
#include "impl/m_Sum.inl"
  return 0;
}

/// <summary>
///   Gets the sum of all elements in a vector.
/// </summary>
static double dv_Sum(int n, const double* a)
{
    double sum = 0.0;
#include "impl/v_Sum.inl"
    return sum;
}

/// <summary>
///   Gets the sum of all elements in a vector.
/// </summary>
static int iv_Sum(int n, const int* a)
{
  int sum = 0;
#include "impl/v_Sum.inl"
  return sum;
}

/// <summary>
///   Gets the product of all elements in a vector.
/// </summary>
static double dv_Product(int n, const double* a)
{
    double product = 1.0;
#include "impl/v_Product.inl"
    return product;
}


/// <summary>
///   Gets the product of all elements in a vector.
/// </summary>
static int iv_Product(int n, const int* a)
{
  int product = 1;
#include "impl/v_Product.inl"
  return product;
}


/// <summary>
///   Gets the maximum element in a vector.
/// </summary>
static int dv_MaxIndex(int n, const double* a)
{
    int imax=-1;
#include "impl/v_Max.inl"
    return imax;
}
static int fv_MaxIndex(int n, const float* a)
{
  int imax=-1;
#include "impl/v_Max.inl"
  return imax;
}

/// <summary>
///   Gets the maximum element in a vector.
/// </summary>
static double dv_Max(int n, const double* a)
{
  int imax=dv_MaxIndex(n, a);
  ASSERT(n>0);
  return a[imax];
}

/// <summary>
///   Gets the minimum element in a vector.
/// </summary>
static int dv_MinIndex(int n, const double* a)
{
  int imin=-1;
#include "impl/v_Min.inl"
  return imin;
}

/// <summary>
///   Gets the minimum element in a vector.
/// </summary>
static double dv_Min(int n, const double* a)
{
  int imin=dv_MinIndex(n, a);
  ASSERT(n>0);
  return a[imin];
}

/// <summary>
///   Gets the maximum values accross one dimension of a matrix.
/// </summary>
static int dm_MaxIndex(const double* a, int al, int dimension, int* imax, IDIM asz)
{
#define __COMP(a, b)   ((a) > (b))
#include "impl/m_Max.inl"
#undef __COMP
    return 0;
}

/// <summary>
///   Gets the minimum values across one dimension of a matrix.
/// </summary>
static int dm_MinIndex(const double* a, int al, int dimension, int* imax, IDIM asz)
{
#define __COMP(a, b)   ((a) < (b))
#include "impl/m_Max.inl"
#undef __COMP
    return 0;
}

/// <summary>
///   Gets the range of the values in a vector.
/// </summary>
static IRANGE dv_RangeIndex(int n, const double* a)
{
#include "impl/v_Range.inl"
  return iRANGE(l, r);
}
static DRANGE dv_Range(int n, const double* a)
{
  IRANGE ra = dv_RangeIndex(n, a);
  return dRANGE(a[ra.l], a[ra.r]);
}

/// <summary>
///   Gets the range of the values in a vector.
/// </summary>
static IRANGE iv_RangeIndex(int n, const int* a)
{
#include "impl/v_Range.inl"
  return iRANGE(l, r);
}
static DRANGE iv_Range(int n, const int* a)
{
  IRANGE ra = iv_RangeIndex(n, a);
  return dRANGE(a[ra.l], a[ra.r]);
}

/// <summary>
///   Gets the range of the values accross the columns of a matrix.
/// </summary>
static int dm_Range(const double* a, int al, DRANGE* ranges, IDIM asz)
{
#include "impl/m_Range.inl"
    return 0;
}
// 四舍五入到小数点后
double round1(double x, int k) {
  double pk, y;
  static const double pow10[]={1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
  ASSERT(k>=0 && k<countof(pow10));
  pk = pow10[k];
  y = (int)(x*pk + 0.5);
  y /= pk;
  return y;
}

// #region Rounding and discretization
/// <summary>
///   Rounds a double-precision floating-point matrix to a specified number of fractional digits.
/// </summary>
static int dm_Round(const double* a, int al, int decimals, double* r, int rl, IDIM asz)
{
    int i, j;
    for (i = 0; i < asz.len[0]; i++) {
      for (j = 0; j < asz.len[1]; j++) {
        r[i*rl+j] = round1(a[i*al+j], decimals);
      }
    }

    return 0;
}

#if 0
/// <summary>
///   Returns the largest integer less than or equal than to the specified 
///   double-precision floating-point number for each element of the matrix.
/// </summary>
static double* Floor(double* a)
{
    double* r = new double[asz.len[0], asz.len[1]];

    for (i = 0; i < asz.len[0]; i++)
        for (j = 0; j < asz.len[1]; j++)
            r[i*rl+j] = floor(a[i*al+j]);

    return 0;
}

/// <summary>
///   Returns the largest integer greater than or equal than to the specified 
///   double-precision floating-point number for each element of the matrix.
/// </summary>
static double* Ceiling(double* a)
{
    double* r = new double[asz.len[0], asz.len[1]];

    for (i = 0; i < asz.len[0]; i++)
        for (j = 0; j < asz.len[1]; j++)
            r[i*rl+j] = System.Math.Ceiling(a[i*al+j]);

    return 0;
}

/// <summary>
///   Rounds a double-precision floating-point number array to a specified number of fractional digits.
/// </summary>
static double* Round(double* value, int decimals)
{
    double* r = new double[value.Length];
    for (i = 0; i < r.Length; i++)
        r[i] = Math.Round(value[i], decimals);
    return 0;
}

/// <summary>
///   Returns the largest integer less than or equal than to the specified 
///   double-precision floating-point number for each element of the array.
/// </summary>
static double* Floor(double* value)
{
    double* r = new double[value.Length];
    for (i = 0; i < r.Length; i++)
        r[i] = floor(value[i]);
    return 0;
}

/// <summary>
///   Returns the largest integer greater than or equal than to the specified 
///   double-precision floating-point number for each element of the array.
/// </summary>
static double* Ceiling(double* value)
{
    double* r = new double[value.Length];
    for (i = 0; i < r.Length; i++)
        r[i] = Math.Ceiling(value[i]);
    return 0;
}

// #endregion


// #region Elementwise operations
/// <summary>
///   Elementwise absolute value.
/// </summary>
static int* Abs(int* value)
{
    int* r = new int[value.Length];
    for (i = 0; i < value.Length; i++)
        r[i] = fabs(value[i]);
    return 0;
}

/// <summary>
///   Elementwise absolute value.
/// </summary>
static double* Abs(double* value)
{
    double* r = new double[value.Length];
    for (i = 0; i < value.Length; i++)
        r[i] = fabs(value[i]);
    return 0;
}

/// <summary>
///   Elementwise absolute value.
/// </summary>
static double* Abs(double* value)
{
    int rows = value.GetLength(0);
    int cols = value.GetLength(1);

    double* r = new double[rows, cols];
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = fabs(value[i, j]);
    return 0;
}

/// <summary>
///   Elementwise absolute value.
/// </summary>
static int* Abs(int* value)
{
    int rows = value.GetLength(0);
    int cols = value.GetLength(1);

    int* r = new int[rows, cols];
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = fabs(value[i, j]);
    return 0;
}


/// <summary>
///   Elementwise Square root.
/// </summary>
static double* Sqrt(double* value)
{
    double* r = new double[value.Length];
    for (i = 0; i < value.Length; i++)
        r[i] = sqrt(value[i]);
    return 0;
}

/// <summary>
///   Elementwise Square root.
/// </summary>
static double* Sqrt(double* value)
{
    int rows = value.GetLength(0);
    int cols = value.GetLength(1);

    double* r = new double[rows, cols];
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = sqrt(value[i, j]);
    return 0;
}


/// <summary>
///   Elementwise power operation.
/// </summary>
/// <param name="x">A matrix.</param>
/// <param name="y">A power.</param>
/// <returns>Returns x elevated to the power of y.</returns>
static double* ElementwisePower(double* x, double y)
{
    double* r = new double[x.GetLength(0), x.GetLength(1)];

    for (i = 0; i < x.GetLength(0); i++)
        for (j = 0; j < x.GetLength(1); j++)
            r[i*rl+j] = pow(x[i, j], y);

    return 0;
}

/// <summary>
///   Elementwise power operation.
/// </summary>
/// <param name="x">A matrix.</param>
/// <param name="y">A power.</param>
/// <returns>Returns x elevated to the power of y.</returns>
static double* ElementwisePower(double* x, double y)
{
    double* r = new double[x.Length];

    for (i = 0; i < r.Length; i++)
        r[i] = pow(x[i], y);

    return 0;
}


/// <summary>
///   Elementwise divide operation.
/// </summary>
static double* ElementwiseDivide(double* a, double* b)
{
    double* r = new double[an];

    for (i = 0; i < an; i++)
        r[i] = a[i] / b[i];

    return 0;
}

/// <summary>
///   Elementwise divide operation.
/// </summary>
static double* ElementwiseDivide(double* a, double* b)
{
    int rows = asz.len[0];
    int cols = b.GetLength(1);

    double* r = new double[rows, cols];

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = a[i*al+j] / b[i*bl+j];

    return 0;
}

/// <summary>
///   Elementwise division.
/// </summary>
static double* ElementwiseDivide(double* a, double* b, int dimension)
{
    int rows = asz.len[0];
    int cols = asz.len[1];

    double* r = new double[rows, cols];

    if (dimension == 1)
    {
        if (cols != bn) ASSERT(
                "Length of B should equal the number of columns in A", "b");

        for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
                r[i*rl+j] = a[i*al+j] / b[j];
    }
    else
    {
        if (rows != bn) ASSERT(
                "Length of B should equal the number of rows in A", "b");

        for (j = 0; j < cols; j++)
            for (i = 0; i < rows; i++)
                r[i*rl+j] = a[i*al+j] / b[i];
    }
    return 0;
}


/// <summary>
///   Elementwise multiply operation.
/// </summary>
static double* ElementwiseMultiply(double* a, double* b)
{
    double* r = new double[an];

    for (i = 0; i < an; i++)
        r[i] = a[i] * b[i];

    return 0;
}

/// <summary>
///   Elementwise multiply operation.
/// </summary>
static double* ElementwiseMultiply(double* a, double* b)
{
    if (asz.len[0] != b.GetLength(0) || asz.len[1] != b.GetLength(1))
        ASSERT("Matrix dimensions must agree.", "b");

    int rows = asz.len[0];
    int cols = asz.len[1];

    double* r = new double[rows, cols];

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = a[i*al+j] * b[i*bl+j];

    return 0;
}

/// <summary>
///   Elementwise multiply operation.
/// </summary>
static int* ElementwiseMultiply(int* a, int* b)
{
    int* r = new int[an];

    for (i = 0; i < an; i++)
        r[i] = a[i] * b[i];

    return 0;
}

/// <summary>
///   Elementwise multiplication.
/// </summary>
static int* ElementwiseMultiply(int* a, int* b)
{
    if (asz.len[0] != b.GetLength(0) || asz.len[1] != b.GetLength(1))
        ASSERT("Matrix dimensions must agree.", "b");

    int rows = asz.len[0];
    int cols = asz.len[1];

    int* r = new int[rows, cols];

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            r[i*rl+j] = a[i*al+j] * b[i*bl+j];

    return 0;
}

/// <summary>
///   Elementwise multiplication.
/// </summary>
static double* ElementwiseMultiply(double* a, double* b, int dimension)
{
    int rows = asz.len[0];
    int cols = asz.len[1];

    double* r = new double[rows, cols];

    if (dimension == 1)
    {
        if (cols != bn) ASSERT(
                "Length of B should equal the number of columns in A", "b");

        for (i = 0; i < rows; i++)
            for (j = 0; j < cols; j++)
                r[i*rl+j] = a[i*al+j] * b[j];
    }
    else
    {
        if (rows != bn) ASSERT(
                "Length of B should equal the number of rows in A", "b");

        for (j = 0; j < cols; j++)
            for (i = 0; i < rows; i++)
                r[i*rl+j] = a[i*al+j] * b[i];
    }

    return 0;
}

// #endregion


// #region Conversions

/// <summary>
///   Converts a jagged-array into a multidimensional array.
/// </summary>
static T* ToMatrix<T>(T* array)
{
    int rows = array.Length;
    if (rows == 0) return new T[rows, 0];
    int cols = array[0].Length;

    T* m = new T[rows, cols];
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            m[i*ml+j] = array[i][j];

    return m;
}

/// <summary>
///   Converts an array into a multidimensional array.
/// </summary>
static T* ToMatrix<T>(T* array)
{
    T* m = new T[1, array.Length];
    for (i = 0; i < array.Length; i++)
        m[0, i] = array[i];

    return m;
}

/// <summary>
///   Converts a multidimensional array into a jagged-array.
/// </summary>
static T* ToArray<T>(T* matrix)
{
    int rows = matrix.GetLength(0);

    T* array = new T[rows]*;
    for (i = 0; i < rows; i++)
        array[i] = matrix.GetRow(i);

    return array;
}

/// <summary>
///   Converts a DataTable to a double* array.
/// </summary>
static double* ToMatrix(DataTable table, out string* columnNames)
{
    double* m = new double[table.Rows.Count, table.Columns.Count];
    columnNames = new string[table.Columns.Count];

    for (j = 0; j < table.Columns.Count; j++)
    {
        for (i = 0; i < table.Rows.Count; i++)
        {
            if (table.Columns[j].DataType == typeof(System.String))
            {
                m[i*ml+j] = Double.Parse((String)table.Rows[i][j], System.Globalization.CultureInfo.InvariantCulture);
            }
            else if (table.Columns[j].DataType == typeof(System.Boolean))
            {
                m[i*ml+j] = (Boolean)table.Rows[i][j] ? 1.0 : 0.0;
            }
            else
            {
                m[i*ml+j] = (Double)table.Rows[i][j];
            }
        }

        columnNames[j] = table.Columns[j].Caption;
    }
    return m;
}

/// <summary>
///   Converts a DataTable to a double* array.
/// </summary>
static double* ToMatrix(DataTable table)
{
    String* names;
    return ToMatrix(table, out names);
}

/// <summary>
///   Converts a DataTable to a double* array.
/// </summary>
static double* ToArray(DataTable table)
{
    double* m = new double[table.Rows.Count]*;

    for (i = 0; i < table.Rows.Count; i++)
    {
        m[i] = new double[table.Columns.Count];

        for (j = 0; j < table.Columns.Count; j++)
        {
            if (table.Columns[j].DataType == typeof(System.String))
            {
                m[i][j] = Double.Parse((String)table.Rows[i][j], System.Globalization.CultureInfo.InvariantCulture);
            }
            else if (table.Columns[j].DataType == typeof(System.Boolean))
            {
                m[i][j] = (Boolean)table.Rows[i][j] ? 1.0 : 0.0;
            }
            else
            {
                m[i][j] = (Double)table.Rows[i][j];
            }
        }
    }
    return m;
}

/// <summary>
///   Converts a DataColumn to a double* array.
/// </summary>
static double* ToArray(DataColumn column)
{
    double* m = new double[column.Table.Rows.Count];

    for (i = 0; i < m.Length; i++)
    {
        object b = column.Table.Rows[i][column];

        if (column.DataType == typeof(System.String))
        {
            m[i] = Double.Parse((String)b, System.Globalization.CultureInfo.InvariantCulture);
        }
        else if (column.DataType == typeof(System.Boolean))
        {
            m[i] = (Boolean)b ? 1.0 : 0.0;
        }
        else
        {
            m[i] = (Double)b;
        }
    }

    return m;
}
// #endregion


// #region Inverses and Linear System Solving
/// <summary>
///   Returns the LHS solution matrix if the matrix is square or the least squares solution otherwise.
/// </summary>
/// <remarks>
///   Please note that this does not check if the matrix is non-singular before attempting to solve.
/// </remarks>
static double* Solve(double* matrix, double* rightSide)
{
    if (matrix.GetLength(0) == matrix.GetLength(1))
    {
        // Solve by LU Decomposition if matrix is square.
        return new LuDecomposition(matrix).Solve(rightSide);
    }
    else
    {
        // Solve by QR Decomposition if not.
        return new QrDecomposition(matrix).Solve(rightSide);
    }
}

/// <summary>
///   Returns the LHS solution vector if the matrix is square or the least squares solution otherwise.
/// </summary>
/// <remarks>
///   Please note that this does not check if the matrix is non-singular before attempting to solve.
/// </remarks>
static double* Solve(double* matrix, double* rightSide)
{
    if (matrix.GetLength(0) == matrix.GetLength(1))
    {
        // Solve by LU Decomposition if matrix is square.
        return new LuDecomposition(matrix).Solve(rightSide);
    }
    else
    {
        // Solve by QR Decomposition if not.
        return new QrDecomposition(matrix).Solve(rightSide);
    }
}

/// <summary>
///   Computes the inverse of a matrix.
/// </summary>
static double* Inverse(double* matrix)
{
    if (matrix.GetLength(0) != matrix.GetLength(1))
        ASSERT("Matrix must be square", "matrix");

    return new LuDecomposition(matrix).Inverse();
}

/// <summary>
///   Computes the pseudo-inverse of a matrix.
/// </summary>
static double* PseudoInverse(double* matrix)
{
    return new SingularValueDecomposition(matrix, true, true, true).Inverse();
}
// #endregion


// #region Morphological operations
/// <summary>
///   Transforms a vector into a matrix of given dimensions.
/// </summary>
static T* Reshape<T>(T* array, int rows, int cols)
{
    T* r = new T[rows, cols];

    for (j = 0, k = 0; j < cols; j++)
        for (i = 0; i < rows; i++)
            r[i*rl+j] = array[k++];

    return 0;
}

/// <summary>
///   Transforms a vector into a single vector.
/// </summary>
static T* Reshape<T>(T* array, int dimension)
{
    int rows = array.GetLength(0);
    int cols = array.GetLength(1);

    T* r = new T[rows * cols];

    if (dimension == 1)
    {
        for (j = 0, k = 0; j < rows; j++)
            for (i = 0; i < cols; i++)
                r[k++] = array[j, i];
    }
    else
    {
        for (i = 0, k = 0; i < cols; i++)
            for (j = 0; j < rows; j++)
                r[k++] = array[j, i];
    }

    return 0;
}
// #endregion

//#if 0
#endif
