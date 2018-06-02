

#ifndef OPENCC_CORE_MATX_HPP
#define OPENCC_CORE_MATX_HPP

#ifndef __cplusplus
#  error matx.hpp header must be compiled as C++
#endif

//#include "cvdef.h"

#include "traits.hpp"
#include "saturate.hpp"

//! @addtogroup core_basic
//! @{

////////////////////////////// Small Matrix ///////////////////////////

/** @brief Template class for small matrices whose type and size are known at compilation time

If you need a more flexible type, use CvMat . The elements of the matrix M are accessible using the
M(i,j) notation. Most of the common matrix operations (see also @ref MatrixExpressions ) are
available. To do an operation on Matx that is not implemented, you can easily convert the matrix to
CvMat and backwards:
@code
    Matx33f m(1, 2, 3,
              4, 5, 6,
              7, 8, 9);
    cout << sum(CvMat(m*m.t())) << endl;
 @endcode
 */
template<typename _Tp, int cn> class Vec;
template<typename _Tp, int m, int n> class Matx
{
public:
    enum { type    = DataType<_Tp>::type,
           rows     = m,
           cols     = n,
           channels = rows*cols,
           typecn   = CC_MAKETYPECN(type, channels),
           shortdim = (m < n ? m : n)
         };

    typedef _Tp                           value_type;
    typedef Matx<_Tp, m, n>               mat_type;
    typedef Matx<_Tp, shortdim, 1> diag_type;

    //! default constructor
Matx()
{
    for(int i = 0; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0)
{
    val[0] = v0;
    for(int i = 1; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1)
{
    CC_StaticAssert(channels >= 2, "Matx should have at least 2 elements.");
    val[0] = v0; val[1] = v1;
    for(int i = 2; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2)
{
    CC_StaticAssert(channels >= 3, "Matx should have at least 3 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2;
    for(int i = 3; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
{
    CC_StaticAssert(channels >= 4, "Matx should have at least 4 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    for(int i = 4; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4)
{
    CC_StaticAssert(channels >= 5, "Matx should have at least 5 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    for(int i = 5; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5)
{
    CC_StaticAssert(channels >= 6, "Matx should have at least 6 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5;
    for(int i = 6; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6)
{
    CC_StaticAssert(channels >= 7, "Matx should have at least 7 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6;
    for(int i = 7; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7)
{
    CC_StaticAssert(channels >= 8, "Matx should have at least 8 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    for(int i = 8; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8)
{
    CC_StaticAssert(channels >= 9, "Matx should have at least 9 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8;
    for(int i = 9; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9)
{
    CC_StaticAssert(channels >= 10, "Matx should have at least 10 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8; val[9] = v9;
    for(int i = 10; i < channels; i++) val[i] = _Tp(0);
}



Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11)
{
    CC_StaticAssert(channels >= 12, "Matx should have at least 12 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8; val[9] = v9; val[10] = v10; val[11] = v11;
    for(int i = 12; i < channels; i++) val[i] = _Tp(0);
}


Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13)
{
    CC_StaticAssert(channels == 14, "Matx should have at least 14 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8; val[9] = v9; val[10] = v10; val[11] = v11;
    val[12] = v12; val[13] = v13;
}



Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13, _Tp v14, _Tp v15)
{
    CC_StaticAssert(channels >= 16, "Matx should have at least 16 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    val[8] = v8; val[9] = v9; val[10] = v10; val[11] = v11;
    val[12] = v12; val[13] = v13; val[14] = v14; val[15] = v15;
    for(int i = 16; i < channels; i++) val[i] = _Tp(0);
}


Matx(const _Tp* values)
{
    for( int i = 0; i < channels; i++ ) val[i] = values[i];
}

void all(_Tp alpha)
{
    for( int i = 0; i < m*n; i++ ) val[i] = alpha;
}


void zeros()
{
    all(0);
}


void ones()
{
    all(1);
}


void eye()
{
    mat_type& M = *this;
    for(int i = 0; i < shortdim; i++)
        M(i,i) = 1;
}

void diag(const typename diag_type& d)
{
    mat_type& M = *this;
    for(int i = 0; i < shortdim; i++)
        M(i,i) = d(i, 0);
}


_Tp dot(const mat_type& M) const
{
    _Tp s = 0;
    for( int i = 0; i < channels; i++ ) s += val[i]*M.val[i];
    return s;
}


double ddot(const mat_type& M) const
{
    double s = 0;
    for( int i = 0; i < channels; i++ ) s += (double)val[i]*M.val[i];
    return s;
}


template<typename T2>
inline operator Matx<T2, m, n>() const
{
    Matx<T2, m, n> M;
    for( int i = 0; i < m*n; i++ ) M.val[i] = saturate_cast<T2>(val[i]);
    return M;
}

template<int m1, int n1> inline
void reshape(Matx<_Tp, m1, n1>& dst) const
{
    CC_StaticAssert(m1*n1 == m*n, "Input and destnarion matrices must have the same number of elements");
    (const Matx<_Tp, m1, n1>&)*this;
}

template<int m1, int n1> inline
void get_minor(int i, int j, Matx<_Tp, m1, n1>& s) const
{
    CC_DbgAssert(0 <= i && i+m1 <= m && 0 <= j && j+n1 <= n);
    for( int di = 0; di < m1; di++ )
        for( int dj = 0; dj < n1; dj++ )
            s(di, dj) = (*this)(i+di, j+dj);
}


Matx<_Tp, 1, n> row(int i) const
{
    CC_DbgAssert((unsigned)i < (unsigned)m);
    return Matx<_Tp, 1, n>(&val[i*n]);
}


Matx<_Tp, m, 1> col(int j) const
{
    CC_DbgAssert((unsigned)j < (unsigned)n);
    Matx<_Tp, m, 1> v;
    for( int i = 0; i < m; i++ )
        v.val[i] = val[i*n + j];
    return v;
}


typename diag_type diag() const
{
    diag_type d;
    for( int i = 0; i < shortdim; i++ )
        d.val[i] = val[i*n + i];
    return d;
}


const _Tp& operator()(int i, int j) const
{
    CC_DbgAssert( (unsigned)i < (unsigned)m && (unsigned)j < (unsigned)n );
    return this->val[i*n + j];
}


_Tp& operator ()(int i, int j)
{
    CC_DbgAssert( (unsigned)i < (unsigned)m && (unsigned)j < (unsigned)n );
    return val[i*n + j];
}


const _Tp& operator ()(int i) const
{
    CC_StaticAssert(m == 1 || n == 1, "Single index indexation requires matrix to be a column or a row");
    CC_DbgAssert( (unsigned)i < (unsigned)(m+n-1) );
    return val[i];
}


_Tp& operator ()(int i)
{
    CC_StaticAssert(m == 1 || n == 1, "Single index indexation requires matrix to be a column or a row");
    CC_DbgAssert( (unsigned)i < (unsigned)(m+n-1) );
    return val[i];
}


mat_type& Add(const mat_type& a, const mat_type& b)
{
    for( int i = 0; i < channels; i++ )
        saturate_cast(val[i], a.val[i] + b.val[i]);
    return *this;
}


mat_type& Sub(const mat_type& a, const mat_type& b)
{
    for( int i = 0; i < channels; i++ )
        saturate_cast(val[i], a.val[i] - b.val[i]);
  return *this;
}

template<typename _T2> inline
mat_type& Scale(const mat_type& a, _T2 alpha)
{
    for( int i = 0; i < channels; i++ )
        saturate_cast(val[i], a.val[i] * alpha);
  return *this;
}


mat_type& Mul(const mat_type& a, const mat_type& b)
{
    for( int i = 0; i < channels; i++ )
        saturate_cast(val[i], a.val[i] * b.val[i]);
  return *this;
}


mat_type& Div(const mat_type& a, const mat_type& b)
{
    for( int i = 0; i < channels; i++ )
        saturate_cast(val[i], a.val[i] / b.val[i]);
  return *this;
}

template<typename T, typename Y> inline
mat_type& MatMul(const T& a, const Y& b) {
  enum {l = a.cols};
  for( int i = 0; i < m; i++ ) {
    for( int j = 0; j < n; j++ ) {
      _Tp s = 0;
      for( int k = 0; k < l; k++ )
        s += a(i, k) * b(k, j);
      val[i*n + j] = s;
    }
  }
  return *this;
}

template<int l> inline
mat_type& MatMul2(const Matx<_Tp, m, l>& a, const Matx<_Tp, l, n>& b)
{
  for( int i = 0; i < m; i++ ) {
    for( int j = 0; j < n; j++ ) {
      _Tp s = 0;
      for( int k = 0; k < l; k++ )
        s += a(i, k) * b(k, j);
      val[i*n + j] = s;
    }
  }
  return *this;
}


mat_type& T(const Matx<_Tp, n, m>& a)
{
    for( int i = 0; i < m; i++ ) {
      for( int j = 0; j < n; j++ ) {
        val[i*n + j] = a(j, i);
      }
    }
    return *this;
}


mat_type mul(const mat_type& a) const
{
    return mat_type().mul(*this, a);
}


mat_type div(const mat_type& a) const
{
    return mat_type().Div(*this, a);
}


mat_type t() const
{
    return mat_type().T(*this);
}

Vec<_Tp, n> solve(const Vec<_Tp, m>& rhs, int method) const
{
    Matx<_Tp, n, 1> x = solve((const Matx<_Tp, m, 1>&)(rhs), method);
    return (Vec<_Tp, n>&)(x);
}

template<typename _Tp, int m> static inline
double determinant(const Matx<_Tp, m, m>& a)
{
	return internal::Matx_DetOp<_Tp, m>()(a);
}

static inline
double trace(const mat_type& a)
{
	_Tp s = 0;
	for (int i = 0; i < MIN(m, n); i++)
		s += a(i, i);
	return s;
}

static inline
double norm(const mat_type& M)
{
	return std::sqrt(normL2Sqr<_Tp, double>(M.val, m*n));
}

static inline
double norm(const mat_type& M, int normType)
{
	switch (normType) {
	case NORM_INF:
		return (double)normInf<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n);
	case NORM_L1:
		return (double)normL1<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n);
	case NORM_L2SQR:
		return (double)normL2Sqr<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n);
	default:
	case NORM_L2:
		return std::sqrt((double)normL2Sqr<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n));
	}
}

template <typename _Tp2> inline
mat_type& operator += (const Matx<_Tp2, m, n>& b)
{
    mat_type& a = *this;
    for( int i = 0; i < m*n; i++ )
        a.val[i] = saturate_cast<_Tp1>(a.val[i] + b.val[i]);
    return a;
}

template <typename _Tp2> inline
mat_type& operator -= (const Matx<_Tp2, m, n>& b)
{
    mat_type& a = *this;
    for( int i = 0; i < m*n; i++ )
        a.val[i] = saturate_cast<_Tp1>(a.val[i] - b.val[i]);
    return a;
}

static inline
mat_type operator + (const mat_type& a, const mat_type& b)
{
    return mat_type().Add(a, b);
}

static inline
mat_type operator - (const mat_type& a, const mat_type& b)
{
    return mat_type().Sub(a, b);
}

inline
mat_type& operator *= (int alpha)
{
    mat_type& a = *this;
    for( int i = 0; i < m*n; i++ )
        saturate_cast(val[i], val[i] * alpha);
    return *this;
}

inline
mat_type& operator *= (float alpha)
{
    for( int i = 0; i < m*n; i++ )
        saturate_cast(val[i], val[i] * alpha);
    return *this;
}

inline
mat_type& operator *= (double alpha)
{
    mat_type& a = *this;
    for( int i = 0; i < m*n; i++ )
        saturate_cast(val[i], a.val[i] * alpha);
    return a;
}

inline
mat_type operator * (int alpha)
{
	mat_type& a = *this;
    return mat_type(a).Scale(alpha);
}

static inline
mat_type operator * (const mat_type& a, float alpha)
{
    return mat_type(a, alpha, Matx_ScaleOp());
}

static inline
mat_type operator * (const mat_type& a, double alpha)
{
    return mat_type(a, alpha, Matx_ScaleOp());
}

static inline
mat_type operator * (int alpha, const mat_type& a)
{
    return mat_type(a, alpha, Matx_ScaleOp());
}

static inline
mat_type operator * (float alpha, const mat_type& a)
{
    return mat_type(a, alpha, Matx_ScaleOp());
}

static inline
mat_type operator * (double alpha, const mat_type& a)
{
    return mat_type(a, alpha, Matx_ScaleOp());
}

static inline
mat_type operator - (const mat_type& a)
{
    return mat_type(a, -1, Matx_ScaleOp());
}

    _Tp val[m*n]; //< matrix elements
};

typedef Matx<float, 1, 2> Matx12f;
typedef Matx<double, 1, 2> Matx12d;
typedef Matx<float, 1, 3> Matx13f;
typedef Matx<double, 1, 3> Matx13d;
typedef Matx<float, 1, 4> Matx14f;
typedef Matx<double, 1, 4> Matx14d;
typedef Matx<float, 1, 6> Matx16f;
typedef Matx<double, 1, 6> Matx16d;

typedef Matx<float, 2, 1> Matx21f;
typedef Matx<double, 2, 1> Matx21d;
typedef Matx<float, 3, 1> Matx31f;
typedef Matx<double, 3, 1> Matx31d;
typedef Matx<float, 4, 1> Matx41f;
typedef Matx<double, 4, 1> Matx41d;
typedef Matx<float, 6, 1> Matx61f;
typedef Matx<double, 6, 1> Matx61d;

typedef Matx<float, 2, 2> Matx22f;
typedef Matx<double, 2, 2> Matx22d;
typedef Matx<float, 2, 3> Matx23f;
typedef Matx<double, 2, 3> Matx23d;
typedef Matx<float, 3, 2> Matx32f;
typedef Matx<double, 3, 2> Matx32d;

typedef Matx<float, 3, 3> Matx33f;
typedef Matx<double, 3, 3> Matx33d;

typedef Matx<float, 3, 4> Matx34f;
typedef Matx<double, 3, 4> Matx34d;
typedef Matx<float, 4, 3> Matx43f;
typedef Matx<double, 4, 3> Matx43d;

typedef Matx<float, 4, 4> Matx44f;
typedef Matx<double, 4, 4> Matx44d;
typedef Matx<float, 6, 6> Matx66f;
typedef Matx<double, 6, 6> Matx66d;

/** @brief  Comma-separated Matrix Initializer
*/
template<typename _Tp, int m, int n> class MatxCommaInitializer
{
public:
  typedef Matx<_Tp, m, n> mat_type;
    MatxCommaInitializer(mat_type* _mtx);
    template<typename T2> MatxCommaInitializer<_Tp, m, n>& operator , (T2 val);
    mat_type operator *() const;

    mat_type* dst;
    int idx;
};


/////////////////////// Vec (used as element of multi-channel images /////////////////////

/** @brief Template class for short numerical vectors, a partial case of Matx

This template class represents short numerical vectors (of 1, 2, 3, 4 ... elements) on which you
can perform basic arithmetical operations, access individual elements using [] operator etc. The
vectors are allocated on stack, as opposite to std::valarray, std::vector, CvMat etc., which
elements are dynamically allocated in the heap.

The template takes 2 parameters:
@tparam _Tp element type
@tparam cn the number of elements

In addition to the universal notation like Vec<float, 3>, you can use shorter aliases
for the most popular specialized variants of Vec, e.g. Vec3f ~ Vec<float, 3>.

It is possible to convert Vec\<T,2\> to/from Point_, Vec\<T,3\> to/from Point3_ , and Vec\<T,4\>
to CvScalar or Scalar_. Use operator[] to access the elements of Vec.

All the expected vector operations are also implemented:
-   v1 = v2 + v3
-   v1 = v2 - v3
-   v1 = v2 \* scale
-   v1 = scale \* v2
-   v1 = -v2
-   v1 += v2 and other augmenting operations
-   v1 == v2, v1 != v2
-   norm(v1) (euclidean norm)
The Vec class is commonly used to describe pixel types of multi-channel arrays. See CvMat for details.
*/
template<typename _Tp, int cn> class Vec : public Matx<_Tp, cn, 1>
{
public:
    typedef _Tp value_type;
    enum { type    = Matx<_Tp, cn, 1>::type,
           channels = cn,
           typecn     = CC_MAKETYPECN(type, channels)
         };

    typedef Matx<_Tp, cn, 1> base;
    //! default constructor

Vec() {}


Vec(_Tp v0)
    : base(v0) {}


Vec(_Tp v0, _Tp v1)
    : base(v0, v1) {}


Vec(_Tp v0, _Tp v1, _Tp v2)
    : base(v0, v1, v2) {}


Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
    : base(v0, v1, v2, v3) {}


Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4)
    : base(v0, v1, v2, v3, v4) {}


Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5)
    : base(v0, v1, v2, v3, v4, v5) {}


Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6)
    : base(v0, v1, v2, v3, v4, v5, v6) {}


Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7)
    : base(v0, v1, v2, v3, v4, v5, v6, v7) {}


Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8)
    : base(v0, v1, v2, v3, v4, v5, v6, v7, v8) {}


Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9)
    : base(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) {}


Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13)
    : base(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) {}


Vec(const _Tp* values)
    : base(values) {}

Vec(const Vec<_Tp, cn>& m)
    : base(m.val) {}

void all(_Tp alpha)
{
    for( int i = 0; i < cn; i++ ) val[i] = alpha;
}

void mul(const base& v)
{
    for( int i = 0; i < cn; i++ ) saturate_cast(val[i], val[i]*v.val[i]);
}

void cross(const base&) const
{
    CC_StaticAssert(cn == 3, "for arbitrary-size vector there is no cross-product defined");
    return Vec<_Tp, cn>();
}

template<typename T2> inline
operator Vec<T2, cn>() const
{
    Vec<T2, cn> v;
    for( int i = 0; i < cn; i++ ) v.val[i] = saturate_cast<T2>(this->val[i]);
    return v;
}

inline
const _Tp& operator [](int i) const
{
    CC_DbgAssert( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

inline
_Tp& operator [](int i)
{
    CC_DbgAssert( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

inline
const _Tp& operator ()(int i) const
{
    CC_DbgAssert( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

inline
_Tp& operator ()(int i)
{
    CC_DbgAssert( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

inline
Vec<_Tp, cn> normalize(const Vec<_Tp, cn>& v)
{
    double nv = norm(v);
    return v * (nv ? 1./nv : 0.);
}

};

/** @name Shorter aliases for the most popular specializations of Vec<T,n>
  @{
*/
typedef Vec<uchar, 2> Vec2b;
typedef Vec<uchar, 3> Vec3b;
typedef Vec<uchar, 4> Vec4b;

typedef Vec<short, 2> Vec2s;
typedef Vec<short, 3> Vec3s;
typedef Vec<short, 4> Vec4s;

typedef Vec<ushort, 2> Vec2w;
typedef Vec<ushort, 3> Vec3w;
typedef Vec<ushort, 4> Vec4w;

typedef Vec<int, 2> Vec2i;
typedef Vec<int, 3> Vec3i;
typedef Vec<int, 4> Vec4i;
typedef Vec<int, 6> Vec6i;
typedef Vec<int, 8> Vec8i;

typedef Vec<float, 2> Vec2f;
typedef Vec<float, 3> Vec3f;
typedef Vec<float, 4> Vec4f;
typedef Vec<float, 6> Vec6f;

typedef Vec<double, 2> Vec2d;
typedef Vec<double, 3> Vec3d;
typedef Vec<double, 4> Vec4d;
typedef Vec<double, 6> Vec6d;
/** @} */

/** @brief  Comma-separated Vec Initializer
*/
template<typename _Tp, int m> class VecCommaInitializer : public MatxCommaInitializer<_Tp, m, 1>
{
public:
    VecCommaInitializer(Vec<_Tp, m>* _vec);
    template<typename T2> VecCommaInitializer<_Tp, m>& operator , (T2 val);
    Vec<_Tp, m> operator *() const;
};

template<typename _Tp, int cn> static Vec<_Tp, cn> normalize(const Vec<_Tp, cn>& v);

//! @} core_basic

//! @cond IGNORED

///////////////////////////////////// helper classes /////////////////////////////////////



////////////////////////////////// Matx Implementation ///////////////////////////////////

template<typename _Tp, int m> inline
double Det(const Matx<_Tp, m, m>& a)
{
    Matx<_Tp, m, m> temp = a;
    double p = 1==m ? a(0, 0)
      : 2==m ? (a(0,0)*a(1,1) - a(0,1)*a(1,0))
      : 3==m ? (a(0,0)*(a(1,1)*a(2,2) - a(2,1)*a(1,2)) -
            a(0,1)*(a(1,0)*a(2,2) - a(2,0)*a(1,2)) +
            a(0,2)*(a(1,0)*a(2,1) - a(2,0)*a(1,1)))
      : LU(temp.val, m*sizeof(_Tp), m, 0, 0, 0);
    if( p == 0 )
        return p;
    for( int i = 0; i < m; i++ )
        p *= temp(i, i);
    return p;
}

template<typename _Tp>
Vec<_Tp, 2> inline conjugate(const Vec<_Tp, 2>& v)
{
    return Vec<_Tp, 2>(v[0], -v[1]);
}

template<typename _Tp> Vec<_Tp, 4> inline conjugate(const Vec<_Tp, 4>& v)
{
    return Vec<_Tp, 4>(v[0], -v[1], -v[2], -v[3]);
}

/////////////////////////////////// Vec Implementation ///////////////////////////////////



Vec2f conj(const Vec2f& x) 
{
    return conjugate(x);
}


Vec2d conj(const Vec2d& x) 
{
    return conjugate(x);
}


Vec4f conj(const Vec4f& x) 
{
    return conjugate(x);
}


Vec4d conj(const Vec4d& x) 
{
    return conjugate(x);
}

Vec3f cross(const Vec3f& s, const Vec3f& v)
{
    return Vec3f(s.val[1]*v.val[2] - s.val[2]*v.val[1],
                     s.val[2]*v.val[0] - s.val[0]*v.val[2],
                     s.val[0]*v.val[1] - s.val[1]*v.val[0]);
}

Vec3d cross(const Vec3d& s, const Vec3d& v)
{
    return Vec3d(s.val[1]*v.val[2] - s.val[2]*v.val[1],
                     s.val[2]*v.val[0] - s.val[0]*v.val[2],
                     s.val[0]*v.val[1] - s.val[1]*v.val[0]);
}


//! @endcond

///////////////////////////// Matx out-of-class operators ////////////////////////////////

//! @relates Matx
//! @{

template<typename _Tp, int m, int n> static inline
Vec<_Tp, m> operator * (const Matx<_Tp, m, n>& a, const Vec<_Tp, n>& b)
{
    return Vec<_Tp, m>(a.MatMul(b));
}

template<typename _Tp, int m, int n> static inline
bool operator == (const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b)
{
    for( int i = 0; i < m*n; i++ )
        if( a.val[i] != b.val[i] ) return false;
    return true;
}

template<typename _Tp, int m, int n> static inline
bool operator != (const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b)
{
    return !(a == b);
}

//! @}

////////////////////////////// Vec out-of-class operators ////////////////////////////////

//! @relates Vec
//! @{

template<typename _Tp1, typename _Tp2, int cn> static inline
Vec<_Tp1, cn>& operator += (Vec<_Tp1, cn>& a, const Vec<_Tp2, cn>& b)
{
    for( int i = 0; i < cn; i++ )
        a.val[i] = saturate_cast<_Tp1>(a.val[i] + b.val[i]);
    return a;
}

template<typename _Tp1, typename _Tp2, int cn> static inline
Vec<_Tp1, cn>& operator -= (Vec<_Tp1, cn>& a, const Vec<_Tp2, cn>& b)
{
    for( int i = 0; i < cn; i++ )
        a.val[i] = saturate_cast<_Tp1>(a.val[i] - b.val[i]);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator + (const Vec<_Tp, cn>& a, const Vec<_Tp, cn>& b)
{
    return Vec<_Tp, cn>(a, b, Matx_AddOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator - (const Vec<_Tp, cn>& a, const Vec<_Tp, cn>& b)
{
    return Vec<_Tp, cn>(a, b, Matx_SubOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator *= (Vec<_Tp, cn>& a, int alpha)
{
    for( int i = 0; i < cn; i++ )
        saturate_cast(a[i], a[i]*alpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator *= (Vec<_Tp, cn>& a, float alpha)
{
    for( int i = 0; i < cn; i++ )
        saturate_cast(a[i], a[i]*alpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator *= (Vec<_Tp, cn>& a, double alpha)
{
    for( int i = 0; i < cn; i++ )
        saturate_cast(a[i], a[i]*alpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator /= (Vec<_Tp, cn>& a, int alpha)
{
    double ialpha = 1./alpha;
    for( int i = 0; i < cn; i++ )
        saturate_cast(a[i], a[i]*ialpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator /= (Vec<_Tp, cn>& a, float alpha)
{
    float ialpha = 1.f/alpha;
    for( int i = 0; i < cn; i++ )
        saturate_cast(a[i], a[i]*ialpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn>& operator /= (Vec<_Tp, cn>& a, double alpha)
{
    double ialpha = 1./alpha;
    for( int i = 0; i < cn; i++ )
        saturate_cast(a[i], a[i]*ialpha);
    return a;
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator * (const Vec<_Tp, cn>& a, int alpha)
{
    return Vec<_Tp, cn>(a, alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator * (int alpha, const Vec<_Tp, cn>& a)
{
    return Vec<_Tp, cn>(a, alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator * (const Vec<_Tp, cn>& a, float alpha)
{
    return Vec<_Tp, cn>(a, alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator * (float alpha, const Vec<_Tp, cn>& a)
{
    return Vec<_Tp, cn>(a, alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator * (const Vec<_Tp, cn>& a, double alpha)
{
    return Vec<_Tp, cn>(a, alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator * (double alpha, const Vec<_Tp, cn>& a)
{
    return Vec<_Tp, cn>(a, alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator / (const Vec<_Tp, cn>& a, int alpha)
{
    return Vec<_Tp, cn>(a, 1./alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator / (const Vec<_Tp, cn>& a, float alpha)
{
    return Vec<_Tp, cn>(a, 1.f/alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator / (const Vec<_Tp, cn>& a, double alpha)
{
    return Vec<_Tp, cn>(a, 1./alpha, Matx_ScaleOp());
}

template<typename _Tp, int cn> static inline
Vec<_Tp, cn> operator - (const Vec<_Tp, cn>& a)
{
    Vec<_Tp,cn> t;
    for( int i = 0; i < cn; i++ ) t.saturate_cast(val[i], -a.val[i]);
    return t;
}

template<typename _Tp> inline Vec<_Tp, 4> operator * (const Vec<_Tp, 4>& v1, const Vec<_Tp, 4>& v2)
{
    Vec<_Tp, 4> x;
    saturate_cast(x[0], v1[0]*v2[0] - v1[1]*v2[1] - v1[2]*v2[2] - v1[3]*v2[3]);
    saturate_cast(x[1], v1[0]*v2[1] + v1[1]*v2[0] + v1[2]*v2[3] - v1[3]*v2[2]);
    saturate_cast(x[2], v1[0]*v2[2] - v1[1]*v2[3] + v1[2]*v2[0] + v1[3]*v2[1]);
    saturate_cast(x[3], v1[0]*v2[3] + v1[1]*v2[2] - v1[2]*v2[1] + v1[3]*v2[0]);
    return x;
}

template<typename _Tp> inline Vec<_Tp, 4>& operator *= (Vec<_Tp, 4>& v1, const Vec<_Tp, 4>& v2)
{
    v1 = v1 * v2;
    return v1;
}

#if 0

/*!
  traits
*/
template<typename _Tp, int m, int n> class DataType< mat_type >
{
public:
    typedef mat_type                               value_type;
    typedef Matx<typename DataType<_Tp>::work_type, m, n> work_type;
    typedef _Tp                                           channel_type;
    typedef value_type                                    vec_type;

    enum { generic_type = 0,
           type        = DataType<channel_type>::type,
           channels     = m * n,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8),
           typecn         = CC_MAKETYPECN(type, channels)
         };
};

template<typename _Tp, int cn> class DataType< Vec<_Tp, cn> >
{
public:
    typedef Vec<_Tp, cn>                               value_type;
    typedef Vec<typename DataType<_Tp>::work_type, cn> work_type;
    typedef _Tp                                        channel_type;
    typedef value_type                                 vec_type;

    enum { generic_type = 0,
           type        = DataType<channel_type>::type,
           channels     = cn,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8),
           typecn         = CC_MAKETYPECN(type, channels)
         };
};


template<typename _Tp, int m, int n, int l> static inline
mat_type operator * (const Matx<_Tp, m, l>& a, const Matx<_Tp, l, n>& b)
{
    return mat_type().MatMul(a, b);
}


//////////////////////////////// matx comma initializer //////////////////////////////////

template<typename _Tp, typename _T2, int m, int n> static inline
MatxCommaInitializer<_Tp, m, n> operator << (const mat_type& mtx, _T2 val)
{
    MatxCommaInitializer<_Tp, m, n> commaInitializer((mat_type*)&mtx);
    return (commaInitializer, val);
}

template<typename _Tp, int m, int n> inline
MatxCommaInitializer<_Tp, m, n>::MatxCommaInitializer(mat_type* _mtx)
    : dst(_mtx), idx(0)
{}

template<typename _Tp, int m, int n> template<typename _T2> inline
MatxCommaInitializer<_Tp, m, n>& MatxCommaInitializer<_Tp, m, n>::operator , (_T2 value)
{
    CC_DbgAssert( idx < m*n );
    dst->val[idx++] = saturate_cast(value);
    return *this;
}

template<typename _Tp, int m, int n> inline
mat_type MatxCommaInitializer<_Tp, m, n>::operator *() const
{
    CC_DbgAssert( idx == n*m );
    return *dst;
}


//////////////////////////////// matx comma initializer //////////////////////////////////


template<typename _Tp, typename _T2, int cn> static inline
VecCommaInitializer<_Tp, cn> operator << (const Vec<_Tp, cn>& vec, _T2 val)
{
    VecCommaInitializer<_Tp, cn> commaInitializer((Vec<_Tp, cn>*)&vec);
    return (commaInitializer, val);
}

template<typename _Tp, int cn> inline
VecCommaInitializer<_Tp, cn>::VecCommaInitializer(Vec<_Tp, cn>* _vec)
    : MatxCommaInitializer<_Tp, cn, 1>(_vec)
{}

template<typename _Tp, int cn> template<typename _T2> inline
VecCommaInitializer<_Tp, cn>& VecCommaInitializer<_Tp, cn>::operator , (_T2 value)
{
    CC_DbgAssert( this->idx < cn );
    this->dst->val[this->idx++] = saturate_cast(value);
    return *this;
}

template<typename _Tp, int cn> inline
Vec<_Tp, cn> VecCommaInitializer<_Tp, cn>::operator *() const
{
    CC_DbgAssert( this->idx == cn );
    return *this->dst;
}

#endif


#endif // OPENCC_CORE_MATX_HPP
