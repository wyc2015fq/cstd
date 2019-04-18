# 【C++】错误处理和异常处理 - 江南烟雨 - CSDN博客
2012年11月27日 18:21:46[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：16444
**C++语言本身或标准程序库所抛出的所有异常，都派生自基类exception。这是其他数个标准异常类别的基类，它们共同构成一个类体系：**
**![](https://img-my.csdn.net/uploads/201211/27/1354011666_6615.jpg)**
**图一 标准异常阶层体系**
这些标准异常类别分为三组：
（1）语言本身所支持的异常
此类异常用以支撑某些语言特性。主要包括：
bad_alloc：new操作失败会抛出。
bad_cast：执行期间加在一个引用上面的动态性型别转换操作失败时抛出。
bad_typeid：执行RTTI时，交给typeid的参数为零或空指针时抛出
bad_exception：非预期的异常
（2）C++标准程序库发出的异常
总是派生自logic_error。逻辑错误是由于程序内部逻辑而导致的错误。逻辑错误是可以避免的，且在程序开始执行之前，能够被检测到。
C++标准库中定义的逻辑错误如下：
```cpp
class logic_error : public exception {
public:
  explicit logic_error (const string& what_arg);
};
class invalid_argument : public logic_error {
public:
  explicit invalid_argument (const string& what_arg);
};
class out_of_range : public logic_error {
public:
  explicit out_of_range (const string& what_arg);
};
class length_error : public logic_error {
public:
  explicit length_error (const string& what_arg);
};
class domain_error : public logic_error {
public:
  explicit domain_error (const string& what_arg);
};
```
错误分类解释及举例：
domain_error：专业领域内的范畴 
invalid_argument：无效参数，比如讲bitset以char而非0或1进行初始化
length_error：可能超越了最大极限，比如对着某个字符串附加太多字符。 
out_of_range：参数不再预期范围内。例如在诸如array的容器或字符串string中采用一个错误索引。
（3）程序作用域之外发出的异常
总是派生自runtime_error，用来指出“不在程序范围内，且不容易回避”的事件。此类错误只在程序执行时才是可检测的。C++标准库中的定义如下：
```cpp
class runtime_error : public exception {
public:
  explicit runtime_error (const string& what_arg);
};
class range_error : public runtime_error {
public:
  explicit range_error (const string& what_arg);
};
class overflow_error : public runtime_error {
public:
  explicit overflow_error (const string& what_arg);
};
class underflow_error : public runtime_error {
public:
  explicit underflow_error (const string& what_arg);
};
```
range_error：内部计算时发生区间错误
overflow_error：算数运算时发生上溢
underflow_error：算数运算时发生下溢
实例代码：
```cpp
#include <iostream>
#include <string>
#include <bitset>
#include <typeinfo>
#include <vector>
#include <stdexcept>
using namespace std;
//自定义配置器，vector分配空间使用
template<class _Ty>
class stingyallocator : public allocator<_Ty>
{
public:
   template <class U>
    struct rebind {
          typedef stingyallocator<U> other;
    };
   size_t max_size( ) const
   {
         return 10;
   };
};
int main()
{
    //逻辑错误：out_of_range
    try {
      string str( "Micro" );
      string rstr( "soft" );
      str.append( rstr, 5, 3 );
      cout << str << endl;
   }
   catch ( exception &e ) {
      cerr << "Caught: " << e.what( ) << endl;
      cerr << "Type: " << typeid( e ).name( ) << endl << endl;
   };
   //逻辑错误：length_error
   try
   {
      vector<int, stingyallocator< int > > myv;
      for ( int i = 0; i < 11; i++ )
        myv.push_back( i );
   }
   catch ( exception &e )
   {
      cerr << "Caught " << e.what( ) << endl;
      cerr << "Type " << typeid( e ).name( ) << endl << endl;
   };
   //逻辑错误：invalid_argument
   try
   {
      bitset< 32 > bitset( string( "11001010101100001b100101010110000") );
   }
   catch ( exception &e )
   {
      cerr << "Caught " << e.what( ) << endl;
      cerr << "Type " << typeid( e ).name( ) << endl << endl;
   };
   //逻辑错误：domain_error
   try
   {
      throw domain_error( "Your domain is in error!" );
   }
   catch (exception &e)
   {
      cerr << "Caught: " << e.what( ) << endl;
      cerr << "Type: " << typeid(e).name( ) << endl << endl;
   };
   //运行时错误：range_error
    try
   {
      throw range_error( "The range is in error!" );
   }
   catch (exception &e)
   {
      cerr << "Caught: " << e.what( ) << endl;
      cerr << "Type: " << typeid( e ).name( ) << endl << endl << endl;
   };
   //运行时错误：underflow_error
   try
   {
      throw underflow_error( "The number's a bit small, captain!" );
   }
   catch ( exception &e ) {
      cerr << "Caught: " << e.what( ) << endl;
      cerr << "Type: " << typeid( e ).name( ) << endl << endl;
   };
    //运行时错误：overflow_error
    try
    {
        bitset< 33 > bitset;
        bitset[32] = 1;
        bitset[0] = 1;
        unsigned long x = bitset.to_ulong( );
    }
    catch(exception &e)
    {
        cerr << "Caught " << e.what() << endl;
        cerr << "Type: " << typeid(e).name() << endl << endl;
    }
    return 0;
}
```
运行结果（CodeBlocks）：
**![](https://img-my.csdn.net/uploads/201211/27/1354011802_2554.jpg)**
**参考资料： **
[**C++中异常类的使用方法**](http://blog.csdn.net/goodlixueyong/article/details/6533733)
**[Confused about std::runtime_error vs. std::logic_error](http://stackoverflow.com/questions/2924058/confused-about-stdruntime-error-vs-stdlogic-error)**
