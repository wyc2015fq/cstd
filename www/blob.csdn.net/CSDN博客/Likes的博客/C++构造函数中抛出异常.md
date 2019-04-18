# C++构造函数中抛出异常 - Likes的博客 - CSDN博客
2018年12月28日 18:01:45[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：32
构造函数中抛出异常会有怎样的影响呢？如下实验代码
```cpp
#include <iostream>
#include <stdexcept>
using namespace std;
class A
{
  public:
    A(int n){}
  ~A(){cout<<"A destroy"<<endl;}
};
class B
{
  public:
    B(int n):a(n)
    {
      throw runtime_error("error");
    }
  private:
    A a;
};
int main()
{
  try{
  B b(3);
  }
  catch(...)
  {
    cout << "catch" << endl;
  }
  return 0;
}
```
输出：
![](https://img-blog.csdnimg.cn/20181228175741916.png)
在catch异常前，调用了A的析构函数。
事实上：当构造函数发生异常时，如果已经初始化了成员对象，则会调用成员对象的析构函数。但发生异常的构造函数自己本身的对象不会调用析构函数。即在上述代码中B的构造函数发生异常，调用A的析构，不会调用B的析构。
