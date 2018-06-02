template <int N>
struct fac {
  enum { value = N * fac < N - 1 >::value };
};
template <>
struct fac<1> {
  enum { value = 1 };
};
template <typename II> inline
II print(II f, II l)
{
  for (; f != l; ++f) {
    f->print();
  }
  return f;
}
class A
{
  int a;
  int b;
  char* ptr;
public:
  A(): a(1), b(2) {
    printf("In A()\n");
    //ptr = new char[6];
  }
  //A(const A& a1)  {      a = a1.a;  }
  ~A() {
    printf("In ~A()\n");
    //delete [] ptr;
  }
  void print() const {
    printf("a= %d b=%d\n", a, b);
  }
};
struct B {
  int a;
  int b;
};
inline void* __cdecl operator new(size_t, void* _P)
{
  return (_P);
}
template<class _T1> inline
void _Construct(_T1* _P, const _T1& _V)
{
  new((void*)_P) _T1(_V);
}
template <typename T>
struct vector {
  T* ptr;
  int n;
  vector() {
    ptr = NULL, n = 0;
  }
  ~vector() {
    resize(0);
  }
  void resize(int new_n) {
    int i;
    if (new_n == n) {
      return ;
    }
    if (new_n < n) {
      for (i = new_n; i < n; ++i) {
        (ptr + i)->~T();
      }
    }
    if (new_n > 0) {
      ptr = (T*)realloc(ptr, sizeof(T) * new_n);
    }
    else {
      if (ptr) {
        free(ptr);
      }
    }
    if (new_n > n) {
      for (i = n; i < new_n; ++i) {
        _Construct((ptr + i), T());
      }
    }
    n = new_n;
  }
  T& operator [](int i) {
    ASSERT(i < n && i >= 0);
    return ptr[i];
  }
  const T& operator [](int i) const {
    ASSERT(i < n && i >= 0);
    return ptr[i];
  }
  T* begin() {
    return ptr;
  }
  const T* begin() const {
    return ptr;
  }
  T* end() {
    return ptr + n;
  }
  const T* end() const {
    return ptr + n;
  }
  void print() const {
    ::print(begin(), end());
  }
};
// 实现matrix类使一下测试函数通过
#ifdef TEST1
int test_matrix()
{
  matrix<double> m1, m2, m3;
  // 矩阵生成
  m1.rand(1000, 1000); // 随机
  m1.one(); // 全1
  m1.indentity(); //单位矩阵
  // 矩阵打印
  m1.print();
  // 矩阵计算
  m2 = m1 * (m1 + m1) - m1;
  m2.trans(); //矩阵转置
  m2.inv(); //矩阵求逆
  return 0;
}
#endif // TEST
int test_cpp()
{
  int ff = fac<6>::value;
  vector<A> aa;
  aa.resize(10);
  aa.resize(6);
  aa.print();
  return 0;
}

