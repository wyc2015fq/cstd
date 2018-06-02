
//#include "math/expr.inl"

struct _move_op {};

#if 0
template<typename T> struct CPoint_;
template<typename T> struct CSize_;
template<typename T> struct CRect_;

template<typename T> struct CPoint_
{
public:
    typedef T value_type;

    // various constructors
    CPoint_();
    CPoint_(T _x, T _y);
    CPoint_(const CPoint_& pt);
    //CPoint_(const CSize_<T>& sz);
    //CPoint_(const Vec<T, 2>& v);

    CPoint_& operator = (const CPoint_& pt);
    //! conversion to another data type
    template<typename _Tp2> operator CPoint_<_Tp2>() const;

    //! conversion to the old-style C structures
    //operator Vec<T, 2>() const;

    //! dot product
    T dot(const CPoint_& pt) const;
    //! dot product computed in double-precision arithmetics
    double ddot(const CPoint_& pt) const;
    //! cross-product
    double cross(const CPoint_& pt) const;
    //! checks whether the point is inside the specified rectangle
    int inside(const CRect_<T>& r) const;

    T x, y; //< the point coordinates
};

template<typename T> struct CSize_
{
public:
    typedef T value_type;

    //! various constructors
    CSize_();
    CSize_(T _width, T _height);
    CSize_(const CSize_& sz);
    CSize_(const CPoint_<T>& pt);

    CSize_& operator = (const CSize_& sz);
    //! the area (width*height)
    T area() const;

    //! conversion of another data type.
    template<typename _Tp2> operator CSize_<_Tp2>() const;

    T width, height; // the width and the height
};

template<typename T> struct CRect_
{
public:
    typedef T value_type;

    //! various constructors
    CRect_();
	CRect_(T _x, T _y, T _width, T _height) : x(_x), y(_y), width(_width), height(_height) {}
	CRect_(const CRect_& r) : x(r.x), y(r.y), width(r.width), height(r.height) {}
    CRect_(const CPoint_<T>& org, const CSize_<T>& sz);
    CRect_(const CPoint_<T>& pt1, const CPoint_<T>& pt2);

    CRect_& operator = ( const CRect_& r );
    //! the top-left corner
	CPoint_<T> tl() const { return CPoint_<T>(x, y); }
    //! the bottom-right corner
    CPoint_<T> br() const { return CPoint_<T>(x+width, y+height); }

    //! size (width, height) of the rectangle
    CSize_<T> size() const;
    //! area (width*height) of the rectangle
	T area() const { return width*height; }

    //! conversion to another data type
    template<typename _Tp2> operator CRect_<_Tp2>() const;

    //! checks whether the rectangle contains the point
    int contains(const CPoint_<T>& pt) const {
    return x <= pt.x && pt.x < x + width && y <= pt.y && pt.y < y + height; }

	static CRect_ and(CRect_ a, CRect_ b)
	{
		int x1 = MAX(a.x, b.x);
		int y1 = MAX(a.y, b.y);
		a.width = MIN(a.x + a.width, b.x + b.width) - x1;
		a.height = MIN(a.y + a.height, b.y + b.height) - y1;
		a.x = x1;
		a.y = y1;
		if (a.width <= 0 || a.height <= 0) {
			a.x = a.y = a.width = a.height = 0;
		}
		return a;
	}

	static CRect_ or(CRect_ a, CRect_ b)
	{
		int x1 = MIN(a.x, b.x);
		int y1 = MIN(a.y, b.y);
		a.width = MAX(a.x + a.width, b.x + b.width) - x1;
		a.height = MAX(a.y + a.height, b.y + b.height) - y1;
		a.x = x1;
		a.y = y1;
		return a;
	}
	CRect_ operator & (const CRect_& b) {
		return and (*this, b);
	}
	CRect_ operator | (const CRect_& b) {
		return or (*this, b);
	}

    T x, y, width, height; //< the top-left corner, as well as width and height of the rectangle
};
//typedef CPoint_<int> CPoint2i;
//typedef CPoint_<float> CPoint2f;
//typedef CPoint_<double> CPoint2d;
//typedef CPoint2i CPoint;
typedef CSize_<int> CSize2i;
typedef CSize_<float> CSize2f;
typedef CSize_<double> CSize2d;
//typedef CSize2i CSize;
typedef CRect_<int> CRect2i;
typedef CRect_<float> CRect2f;
typedef CRect_<double> CRect2d;
//typedef CRect2i CRect;

#else

typedef CSize CSize2i;
typedef ISIZE CSize2i;
typedef FSIZE CSize2f;
typedef DSIZE CSize2d;

//typedef CSize2i CSize;
typedef CRect CRect2i;
typedef CRectd CRect2f;
typedef CRectd CRect2d;

CRect operator & (const CRect& a, const CRect& b) {
	return cRectAnd(a, b);
}
CRect operator | (const CRect& a, const CRect& b) {
	return cRectOr(a, b);
}

CRectd operator & (const CRectd& a, const CRectd& b) {
	return cRectAndd(a, b);
}
CRectd operator | (const CRectd& a, const CRectd& b) {
	return cRectOrd(a, b);
}

int contains(const CRect& rt, const CPoint& pt) {
  return rt.x <= pt.x && pt.x < rt.x + rt.width && rt.y <= pt.y && pt.y < rt.y + rt.height;
}
int inside(const CPoint& pt, const CRect& rt) {
	return contains(rt, pt);
}

#endif

#if 0
template <typename T>
struct ImgIterator_ {
  typedef ImgIterator_ self;

  T* p;
  int s, c;
  int i;
  ImgIterator_() : p(0), s(0), c(0), i(0) {}
  ImgIterator_(T* p0, int s0, int c0, int i0) : p(p0), s(s0), c(c0), i(i0) {}
  ImgIterator_(const ImgIterator_& o) : p(o.p), s(o.s), c(o.c), i(o.i) {}
  self& operator++()  {++i; return (*this); }
  self operator++(int) {self me=*this; return me;}
  T& operator* () {}
};

template <typename T>
struct ImgConstIterator_ : public ImgIterator_<T> {
  ImgConstIterator_() : p(0), s(0), c(0), i(0) {}
  ImgConstIterator_(T* p0, int s0, int c0, int i0) : p(p0), s(s0), c(c0), i(i0) {}
  ImgConstIterator_(const ImgIterator_& o) : p(o.p), s(o.s), c(o.c), i(o.i) {}
};
#endif


matx33d operator*(const matx33d& a, const matx33d& b) {
  matx33d c;
  matmul3(c.val, a.val, b.val);
  return c;
}
vec3d operator*(const vec3d& a, double b) {
  vec3d c;
  c.val[0] = a.val[0]*b;
  c.val[1] = a.val[1]*b;
  c.val[2] = a.val[2]*b;
  return c;
}
CPoint3f operator*(const CPoint3f& a, double b) {
  CPoint3f c;
  c.x = a.x*b;
  c.y = a.y*b;
  c.z = a.z*b;
  return c;
}
CPoint3f operator+(const CPoint3f& a, const CPoint3f& b) {
  CPoint3f c;
  c.x = a.x + b.x;
  c.y = a.y + b.y;
  c.z = a.z + b.z;
  return c;
}
CPoint3f operator-(const CPoint3f& a, const CPoint3f& b) {
  CPoint3f c;
  c.x = a.x - b.x;
  c.y = a.y - b.y;
  c.z = a.z - b.z;
  return c;
}
//#include "core/1matx.hpp"

struct Img : public img_t {
  ~Img() {
	  imfree(this);}
  Img() {BZERO1(this);}
  Img(const Img& o) {BZERO1(this);imclone2(&o, this);}
  Img(Img& o, _move_op) {BZERO1(this);swap(o);}
  template <typename T, int M, int N> Img(const matx<T, M, N>& o) { BZERO1(this); cvSetMat(this, M, N, (TypeId)TypeIdTrais<T>::id); memcpy(data, o.val, M*N*sizeof(T));  }
  template <typename T, int M, int N> Img(const matx<T, M, N>& o, int type) {BZERO1(this);cvSetMat(this, M, N, (TypeId)type, 1, o.val); convertFrom(data, TypeIdTrais<T>::id); }
  Img(const img_t* o) {BZERO1(this);cvCloneMat(this, o);}
  Img(const img_t& o) {BZERO1(this);cvCloneMat(this, &o);}
  Img(const Img& o, const CRect2i& rt) {BZERO1(this);img_t sub = o.sub(rt); cvCloneMat(this, &sub);}
  Img(int row, int col, int typecn) {BZERO1(this);cvSetMat(this, row, col, CC_TYPECN_TYPE(typecn), CC_TYPECN_CN(typecn));}
  Img(int row, int col, int typecn, void* data, int step = CC_AUTOSTEP) {
    BZERO1(this);
    img_t im[1] = {0};
    cvInitMatHeader(im, row, col, CC_TYPECN_TYPE(typecn), CC_TYPECN_CN(typecn), data, step);
    cvCloneMat(this, im);
  }
  Img(int row, int col, int typecn, double val) {
    BZERO1(this);
    cvSetMat(this, row, col, CC_TYPECN_TYPE(typecn), CC_TYPECN_CN(typecn));
    cvSet(this, cRealScalar(val), NULL);
  }
  int is_cout() const {
    return CC_IS_MAT_CONT(this);
  }
  Img& operator = (const Img& o) {
    imclone2(&o, this);return *this; }
  Img& swap(Img& o) {
    img_t t = *this;
    memcpy(this, &o, sizeof(img_t));
    memcpy(&o, &t, sizeof(img_t));
    return *this;
  }
  //int type() const {return 1; }
  //TypeCnId type() const {return CC_MAT_TYPECN(this); }
  CSize size() const {return cSize(w, h); }
  int empty() const { return imempty(this); }
  TypeId depth() const {return tid;}
  int elemSize() const {return CC_TYPE_SIZE(tid); }
  Img col(int i) const {img_t c={0}; cvGetCol(this, &c, i); return c; }
  Img row(int i) const {img_t c={0}; cvGetRow(this, &c, i); return c; }
  Img colRange(int startcol, int endcol) const {img_t c={0}; cvGetCols(this, &c, startcol, endcol); return c; }
  Img rowRange(int startrow, int endrow) const {img_t c={0}; cvGetRows(this, &c, startrow, endrow); return c; }
  void convertTo(img_t& o, int type) const {cvConvertTo(this, &o, (TypeId)type); }
  void convertTo(void* data, int data_type) const {img_t o = cvMat2(h,w,(TypeId)data_type, CC_MAT_CN(this),data); cvConvertTo(this, &o, (TypeId)data_type); }
  void convertFrom(const void* data, int data_type) {img_t o = cvMat2(h,w,(TypeId)data_type, CC_MAT_CN(this),(void*)data); cvConvertTo(&o, this, (TypeId)data_type); }
  Img clone() const {return *this;}
  void create(int row, int col, int typecn) { cvSetZero(this, row, col, CC_TYPECN_TYPE(typecn), CC_TYPECN_CN(typecn)); }
  void create(CSize sz, int typecn) { cvSetZero(this, sz.height, sz.width, CC_TYPECN_TYPE(typecn), CC_TYPECN_CN(typecn)); }
  void create(int row, int col, int typecn, void* data, int step = CC_AUTOSTEP) {
    img_t im[1] = {0};
    if (data) { imfree(this); }
    cvInitMatHeader(im, row, col, CC_TYPECN_TYPE(typecn), CC_TYPECN_CN(typecn), data, step);
    cvCloneMat(this, im);
  }
  static Img diag(const Img& a) { Img c; cvSetDiag(&c, &a); return Img(c, _move_op()); }
  static Img eye(int row, int col, int typecn) { Img c; cvSetEye(&c, row, col, CC_TYPECN_TYPE(typecn), 1); return Img(c, _move_op()); }
  static Img zeros(int row, int col, int typecn) { Img c; cvSetZero(&c, row, col, CC_TYPECN_TYPE(typecn), 1); return Img(c, _move_op()); }
  void zero() { cvZeroMat(this); }
  static Img ones(int row, int col, int typecn) { Img c; cvSetTo(&c, row, col, CC_TYPECN_TYPE(typecn), 1); return Img(c, _move_op()); }
  Img t() const { Img c; cvT(this, &c); return Img(c, _move_op()); }
  Img reshape(int new_cn, int new_rows CC_DEFAULT(0)) const { return Img(cvReshape1(this, new_cn, new_rows)); }
  void setTo(double x, const Img& mask = Img()) { cvSet(this, cRealScalar(x), mask.empty() ? NULL : &mask); }
  void copyTo(img_t& c, const Img& mask) const { cvCopy(this, &c, mask.empty() ? NULL : &mask); }
  void copyTo(img_t& c) const { cvCopy(this, &c, NULL); }
  template <typename T, int M, int N>
  void copyTo(matx<T, M, N>& c) const { img_t im = cvMat1((int)M, (int)N, (TypeId)TypeIdTrais<T>::id, 1, c.val); cvCopy(this, &im, NULL); }
  img_t operator ()(int x, int y, int width, int height) {img_t c; cvGetSubRect1(this, &c, x, y, width, height); return c;}
  const img_t operator()(int x, int y, int width, int height) const {img_t c; cvGetSubRect1(this, &c, x, y, width, height); return c;}
  img_t sub(const CRect2i& r) {img_t c; cvGetSubRect1(this, &c, r.x, r.y, r.width, r.height); return c;}
  const img_t sub(const CRect2i& r) const {img_t c; cvGetSubRect1(this, &c, r.x, r.y, r.width, r.height); return c;}
  img_t operator ()(const CRect2i& r) {return sub(r);}
  const img_t operator ()(const CRect2i& r) const {return sub(r);}
  template <typename T> T* ptr(int i=0) {ASSERT(TypeIdTrais<T>::id == tid);return (T*)(data + i*step);}
  template <typename T> const T* ptr(int i=0) const {ASSERT(TypeIdTrais<T>::id == tid);return (const T*)(data + i*step);}
  
#if 0
  template <typename T> ImgIterator_<T> begin() {ASSERT(TypeIdTrais<T>::id == tid);return ImgIterator_<T>(data, step, channel(), 0);}
  template <typename T> ImgConstIterator_<T> begin() const {ASSERT(TypeIdTrais<T>::id == tid);return ImgConstIterator_<T>(data, step, channel(), 0);}
  template <typename T> ImgIterator_<T> end() {ASSERT(TypeIdTrais<T>::id == tid);return ImgIterator_<T>(data+rows*step, step, channel(), 0);}
  template <typename T> ImgConstIterator_<T> end() const {ASSERT(TypeIdTrais<T>::id == tid);return ImgConstIterator_<T>(data+rows*step, step, channel(), 0);}
#endif
  template <typename T> T& at(int row, int col) { ASSERT(TypeIdTrais<T>::id == tid); return *(T*)(data + row*step+col*c);}
  template <typename T> const T& at(int row, int col) const {ASSERT(TypeIdTrais<T>::id == tid);return *(T*)(data + row*step+col*c);}
  template <typename T> T& at(int index) {ASSERT(TypeIdTrais<T>::id == tid);ASSERT(c*w==step);return *(T*)(data + index*c);}
  template <typename T> const T& at(int index) const {ASSERT(TypeIdTrais<T>::id == tid);ASSERT(c*w==step);return *(T*)(data + index*c);}

  Img mul(const Img& b) const { Img c; bin_opt(CC_EXPR_MUL, this, &b, 0, &c); return Img(c, _move_op()); }
  Img inv(int method) { Img c; invert(this, &c, method); return Img(c, _move_op()); }
  double dot(const Img& b) const { return cvDotProduct(this, &b); }
  static void bin_opt(ExprType t, const img_t* a, const img_t* b, double d, img_t* c) {
    switch (t) {
    case CC_EXPR_ADD: cvAdd(a, b, c, NULL);break;
    case CC_EXPR_SUB: cvSub(a, b, c, NULL);break;
	case CC_EXPR_MUL: cvMatMul(a, b, c); break;
	case CC_EXPR_DIV: cvDiv(a, b, c); break;
    case CC_EXPR_ADDS: cvAddS_C1(a, d, c, NULL);break;
    case CC_EXPR_SUBS: cvSubS_C1(a, d, c, NULL);break;
    case CC_EXPR_MULS: cvMulS(a, d, c);break;
    case CC_EXPR_DIVS: cvMulS(a, 1./d, c);break;
    //case CC_EXPR_DIV: cvMulS(a, 1./d, c);break;
    case CC_EXPR_FREE: imfree(c);break;
    case CC_EXPR_COPY: cvCloneMat(c, a);break;
    case CC_EXPR_EQ: cvCmp(a, b, c, CC_CMP_EQ);break;
    case CC_EXPR_GT: cvCmp(a, b, c, CC_CMP_GT);break;
    case CC_EXPR_GE: cvCmp(a, b, c, CC_CMP_GE);break;
    case CC_EXPR_LT: cvCmp(a, b, c, CC_CMP_LT);break;
    case CC_EXPR_LE: cvCmp(a, b, c, CC_CMP_LE);break;
    case CC_EXPR_NE: cvCmp(a, b, c, CC_CMP_NE);break;

    case CC_EXPR_EQS: cvCmpS(a, d, c, CC_CMP_EQ);break;
    case CC_EXPR_GTS: cvCmpS(a, d, c, CC_CMP_GT);break;
    case CC_EXPR_GES: cvCmpS(a, d, c, CC_CMP_GE);break;
    case CC_EXPR_LTS: cvCmpS(a, d, c, CC_CMP_LT);break;
    case CC_EXPR_LES: cvCmpS(a, d, c, CC_CMP_LE);break;
    case CC_EXPR_NES: cvCmpS(a, d, c, CC_CMP_NE);break;
    default: ASSERT(0); break;
    }
  }
#ifdef _EXPR_INL_
  typedef expr<img_t, 64> ImgExpr;
  Img(const ImgExpr& s) {BZERO1(this); s.calc(this, bin_opt);}
  Img& operator = (const ImgExpr& s) {s.calc(this, bin_opt); return *this; }
#endif
  Img& operator /= (double b) {bin_opt(CC_EXPR_MULS, this, NULL, 1./b, this); return *this; }
  Img& operator *= (double b) {bin_opt(CC_EXPR_MULS, this, NULL, b, this); return *this; }
  Img& operator += (double b) {bin_opt(CC_EXPR_ADDS, this, NULL, b, this); return *this; }
  Img& operator -= (double b) {bin_opt(CC_EXPR_SUBS, this, NULL, b, this); return *this; }
  Img operator -() {Img c; cvSubRS(this, cRealScalar(0), &c, NULL);return Img(c, _move_op()); }
};

#ifdef _EXPR_INL_
typedef Img::ImgExpr ImgExpr;
#define IMG_OPT_DEF(OP, ET)  \
static inline ImgExpr operator OP (const Img& a, double b) { return ImgExpr(ET ## S, a, b); }\
static inline ImgExpr operator OP (ImgExpr& a, double b) { return ImgExpr(ET ## S, a, b); }\
static inline ImgExpr operator OP (double b, const Img& a) { return ImgExpr(ET ## S, a, b); }\
static inline ImgExpr operator OP (double b, ImgExpr& a) { return ImgExpr(ET ## S, a, b); }\
static inline ImgExpr operator OP (const Img& a, const Img& b) { return ImgExpr(ET, a, b); }\
static inline ImgExpr operator OP (ImgExpr& a, const Img& b) { return ImgExpr(ET, a, b); } \
static inline ImgExpr operator OP (ImgExpr& a, ImgExpr& b) { return ImgExpr(ET, a, b); }
#else
#define IMG_OPT_DEF(OP, ET)  \
static inline Img operator OP (const Img& a, double b) { Img c; Img::bin_opt(ET ## S, &a, 0, b, &c); return Img(c, _move_op()); }\
static inline Img operator OP (Img& a, double b) { Img c; Img::bin_opt(ET ## S, &a, 0, b, &c); return Img(c, _move_op()); }\
static inline Img operator OP (double b, const Img& a) { Img c; Img::bin_opt(ET ## S, &a, 0, b, &c); return Img(c, _move_op()); }\
static inline Img operator OP (double b, Img& a) { Img c; Img::bin_opt(ET ## S, &a, 0, b, &c); return Img(c, _move_op()); } \
static inline Img operator OP (Img& a, const Img& b) { Img c; Img::bin_opt(ET, &a, &b, 0, &c); return Img(c, _move_op()); }\
static inline Img operator OP (const Img& a, const Img& b) { Img c; Img::bin_opt(ET, &a, &b, 0, &c); return Img(c, _move_op()); }
#endif

IMG_OPT_DEF(==, CC_EXPR_EQ)
IMG_OPT_DEF(> , CC_EXPR_GT)
IMG_OPT_DEF(>=, CC_EXPR_GE)
IMG_OPT_DEF(< , CC_EXPR_LT)
IMG_OPT_DEF(<=, CC_EXPR_LE)
IMG_OPT_DEF(!=, CC_EXPR_NE)

IMG_OPT_DEF(+, CC_EXPR_ADD)
IMG_OPT_DEF(-, CC_EXPR_SUB)
IMG_OPT_DEF(*, CC_EXPR_MUL)
IMG_OPT_DEF(/, CC_EXPR_DIV)

#undef IMG_OPT_DEF


struct Svd {
  Img u, w, vt;//CC_SVD_MODIFY_A
  enum {
    MODIFY_A = 1,
      NO_UV    = 2,
      FULL_UV  = 4
  };
  Svd(Img a, int flags = MODIFY_A) {
    int m = a.h, n = a.w;
    cvSetMat(&u, m, m, a.tid);
    cvSetMat(&vt, n, n, a.tid);
    cvSetMat(&w, m, n, a.tid);
    SVD_compute(&a, &w, &u, &vt, flags);
    //cvT(&vt, &vt);
  }
};


void mulSpectrums(const Img& srcA, const Img& srcB, Img& dst, int flags, bool conjB) {
	mulSpectrums(&srcA, &srcB, &dst, flags, conjB);
}
double determinant(const Img& src) {
  return cvDet(&src);
}

void integral(const Img& src, Img& sum, int type) {
  integral(&src, &sum, (TypeId)type);
}
void integral(const Img& src, Img& sum, Img& sqsum, int type) {
  integral(&src, &sum, &sqsum, (TypeId)type);
}
void meanStdDev(const img_t* mat, CScalar& _mean, CScalar& _sdv, const img_t* mask = NULL) {
  meanStdDev(mat, _mean.val, _sdv.val, mask);
}
void meanStdDev(const Img& mat, CScalar& _mean, CScalar& _sdv) {
  meanStdDev(&mat, _mean.val, _sdv.val, NULL);
}
void meanStdDev(const Img& mat, CScalar& _mean, CScalar& _sdv, const Img& mask) {
  meanStdDev(&mat, _mean.val, _sdv.val, &mask);
}
CScalar mean(const Img& mat) {
	//return cvMean(&mat, NULL);
	return mean(&mat, NULL);
}
CScalar mean(const Img& mat, const Img& mask) {
	//return cvMean(&mat, &mask);
	return mean(&mat, &mask);
}
CScalar sum(const Img& mat) {
	//return cvSum(&mat);
	return sum(&mat);
}
CScalar trace(const Img& mat) {
  return cvTrace(&mat);
}

void minMaxIdx(const Img& mat, double* _minVal, double* _maxVal, int* minIdx = 0, int* maxIdx = 0) {
  //cvMinMaxIdx;
  CPoint _minLoc, _maxLoc;
  cvMinMaxLoc(&mat, _minVal, _maxVal, minIdx ? &_minLoc : NULL, maxIdx ? &_maxLoc : NULL);
  if (minIdx) {
    minIdx[0] = _minLoc.y;
    minIdx[1] = _minLoc.x;
  }
  if (maxIdx) {
    maxIdx[0] = _maxLoc.y;
    maxIdx[1] = _maxLoc.x;
  }
}

void minMaxLoc(const Img& mat, double* _minVal, double* _maxVal, int* minIdx = 0, int* maxIdx = 0) {
  //cvMinMaxIdx;
  minMaxIdx(mat, _minVal, _maxVal, minIdx, maxIdx);
}
void pow(const Img& src, double power, Img& dst) {
  cvPow(&src, &dst, power);
}

void Rodrigues_m2v(const Img& rotation_matrix, Img& rotation_vector) {
  //cvRodrigues2(&src, &dst, power);
  cvRodrigues2(&rotation_matrix, &rotation_vector, NULL);
}

int solve( const Img& src, const Img& src2, Img& dst, int method ) {
  return solve( &src, &src2, &dst, method );
}
int countNonZero(const Img& mat) {
  return cvCountNonZero(&mat);
}
void flip(const Img& src, Img& dst, int flip_mode) {
  cvFlip(&src, &dst, flip_mode);
}

void remap(const Img& src, Img& dst, const Img& mapx, const Img& mapy,
                int interpolation, int borderType = CC_BORDER_CONSTANT,
                const CScalar& borderValue = cScalarAll(0)) {
  remap(&src, &dst, &mapx, &mapy, interpolation, borderType, borderValue);
}

void hconcat(const Img& src1, const Img& src2, Img& dst) {
  hconcat(&src1, &src2, &dst);
}
//typedef Img Mat;

void exp(const Img& src, Img& dst) {
  cvExp(&src, &dst);
}
double norm(const Img& mat2, const Img& mat1, int normType CC_DEFAULT(CC_L2)) {
  return cvNorm(&mat2, &mat1, normType, NULL);
}
double norm(const Img& mat2, int normType CC_DEFAULT(CC_L2)) {
  return cvNorm(&mat2, &mat2, normType, NULL);
}

CC_IMPL void
matchTemplate(const Img& img, const Img& templ, Img& result, int method) {
  cvMatchTemplate(&img, &templ, &result, method);
}

CC_IMPL void inRange(const Img& src1, const Img& src2,
        const Img& src3, Img& dst) {
  cvInRange(&src1, &src2, &src3, &dst);
}
CC_IMPL void inRange(const Img& src1, double lower,
        double upper, Img& dst) {
  cvInRangeS(&src1, cRealScalar(lower), cRealScalar(upper), &dst);
}

#if 1
CC_IMPL void resize( const Img& src, Img& dst,
                    CSize dsize, double fx = 0, double fy = 0,
                    int interpolation = CC_INTER_LINEAR ) {
  cvResize(&src, &dst, dsize, fx, fy, interpolation);
}

void sepFilter2D(const Img& src, Img& dst, TypeId ddepth,
             const Img& kernelX, const Img& kernelY,
             CPoint anchor = cPoint(-1,-1),
             double delta = 0, CBorderTypes _border_mode = CC_BORDER_DEFAULT) {
  //sepFilter2D(&src, &dst, ddepth, &kernelX, &kernelY, anchor, delta, _border_mode);
}

int solvePnP( const Img& opoints, const Img& ipoints,
               const Img& cameraMatrix, const Img& distCoeffs,
               Img& rvec, Img& tvec, bool useExtrinsicGuess CC_DEFAULT(false), int flags CC_DEFAULT(CC_SOLVEPNP_ITERATIVE)) {
  return solvePnP1(&opoints, &ipoints, &cameraMatrix, &distCoeffs,
    &rvec, &tvec, useExtrinsicGuess, flags );
}
#endif

void vconcat(const Img& src1, const Img& src2, Img& dst) {
  vconcat(&src1, &src2, &dst);
}

// projectPoints

//cvProjectPoints

#if 0

CC_IMPL void
cvProjectPoints2(const Img& obj_points,
                  const Img& r_vec,
                  const Img& t_vec,
                  const Img& A,
                  const Img& dist_coeffs,
                  Img& img_points, Img& dpdr,
                  Img& dpdt, Img& dpdf,
                  Img& dpdc, Img& dpdk) {

}
#endif

CC_IMPL void circle(Img& img, CPoint center, int radius,
                    const CScalar color, int thickness = 1,
                    int lineType = CC_LINE_8, int shift = 0) {
  circle(&img, center, radius, color, thickness, lineType, shift);
  return ;
}

void rectangle( Img& img, CPoint pt1, CPoint pt2, const CScalar& color, int thickness, int lineType, int shift ) {
  rectangle(&img, pt1, pt2, color, thickness, lineType, shift);
  return ;
}
