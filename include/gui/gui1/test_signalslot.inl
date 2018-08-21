
#include <string>

struct A {
  struct event1 {
    int n;
    std::string str;
  };
  void start() {
    m_s1.emit(0);
    m_s2.emit("Hello C++11");
    event1 aa;
    aa.n = 100;
    aa.str = "Hello C++11";
    m_s3.emit(aa);
  }

  Signal<int> m_s1;  // 不带参数的信号
  Signal<std::string> m_s2;
  Signal<event1> m_s3;
};

struct B {
  virtual void func1(int n) {
    printf("B::func1\n");
  }

  virtual void func2(const std::string & str) {
    printf("B::func2 %s\n", str.c_str());
  }

  virtual void func3(A::event1 e) {
    printf("B::func3 %d %s\n", e.n, e.str.c_str());
  }
  static void func(const std::string & str) {
    printf("func %s\n", str.c_str());
  }
};

struct C : public B {
  void func1(int n) {
    printf("C::func1\n");
  }

  void func2(const std::string & str) {
    printf("C::func2 %s\n", str.c_str());
  }

  void func3(A::event1 e) {
    printf("C::func3 %d %s\n", e.n, e.str.c_str());
  }
};


int test_signalslot()
{
  A a;
  B b;
  C c;
  // 信号与槽绑定
  Slot* s1 = a.m_s1.connect2(&B::func1, &b);
  a.m_s2.connect2(&B::func2, (B*)&c);
  a.m_s3.connect2(&C::func3, &c);
  a.m_s2.connect(B::func);
  int n = 10;
#if _MSC_VER > 1300
  a.m_s2.connect([n](const std::string & str) {
    printf("lambda n:%d str: %s\n", n, str.c_str());
  });
#endif
  delete s1;
  a.start();
  return 0;
}
