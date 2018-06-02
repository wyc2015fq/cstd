
//#include "list"

template <typename T>
struct MatIterator_ {
  typedef MatIterator_ self;

  T* p;
  int s, c;
  int i;
  MatIterator_() : p(0), s(0), c(0), i(0) {}
  MatIterator_(T* p0, int s0, int c0, int i0) : p(p0), s(s0), c(c0), i(i0) {}
  MatIterator_(const MatIterator_& o) : p(o.p), s(o.s), c(o.c), i(o.i) {}
  self& operator++()  {++i; return (*this); }
  self operator++(int) {self me=*this; return me;}
};
template <typename T>
struct MatConstIterator_  {
  typedef MatConstIterator_ self;

  T* p;
  int s, c;
  int i;
  MatConstIterator_() : p(0), s(0), c(0), i(0) {}
  MatConstIterator_(T* p0, int s0, int c0, int i0) : p(p0), s(s0), c(c0), i(i0) {}
  MatConstIterator_(const MatIterator_& o) : p(o.p), s(o.s), c(o.c), i(o.i) {}
  self& operator++()  {++i; return (*this); }
  self operator++(int) {self me=*this; return me;}
};

template <typename T>
struct Mat_ {
  T* data;
  union{ int h; int height; int rows;};
  union{ int w; int width; int cols;};
  int step;
  int cn;
  Mat_() : data(NULL) {}
  Mat_(int row, int col, T* data, int step) : data(NULL) {}
  void init(int row, int cols, T* data2 = NULL, int step2 = 0) {
    rows = row;
    cols = col;
    if (data2) {
      MYREALLOC(data, row*col);
      MEMCPY2D(data, step, data2, step2, rows, cols);
    }
  }
  MatIterator_<T> begin() {return MatIterator_<T>(data, step, cn, 0);}
  MatConstIterator_<T> begin() const {return MatConstIterator_<T>(data, step, cn, 0);}
};

int test_Mat() {
  Mat_<float> m;
  MatIterator_<float> it = m.begin();
  ++it;
  return 0;
}

struct Mat {
  uchar* data;
  union{ int h; int height; int rows;};
  union{ int w; int width; int cols;};
  int step, c;
  TypeId type;
  int depth() {
    return type;
  }
  int channel() {
    return CC_MAT_CN(this);
  }
  template <typename T> inline
  MatIterator_<T> begin() {return MatIterator_<T>(data, step, channel(), 0);}
  template <typename T> inline
  MatConstIterator_<T> begin() const {return MatConstIterator_<T>(data, step, channel(), 0);}
  template <typename T> inline
  T& at(int row, int col) {return *(T*)(data + row*step+col*c);}
  template <typename T> inline
  const T& at(int row, int col) const {return *(T*)(data + row*step+col*c);}
  template <typename T> inline
  T& at(int index) {ASSERT(c*w==step);return *(T*)(data + index*c);}
  template <typename T> inline
  const T& at(int index) const {ASSERT(c*w==step);return *(T*)(data + index*c);}
  //static diag
};