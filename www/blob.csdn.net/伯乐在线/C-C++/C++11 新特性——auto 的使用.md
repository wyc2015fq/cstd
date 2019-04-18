# C++11 新特性——auto 的使用 - 文章 - 伯乐在线
原文出处： [huang_xw](http://blog.csdn.net/huang_xw/article/details/8760403)
C++11中引入的auto主要有两种用途：自动类型推断和返回值占位。auto在C++98中的标识临时变量的语义，由于使用极少且多余，在C++11中已被删除。前后两个标准的auto，完全是两个概念。
#### 1. 自动类型推断
auto自动类型推断，用于从初始化表达式中推断出变量的数据类型。通过auto的自动类型推断，可以大大简化我们的编程工作。下面是一些使用auto的例子。


```
#include <vector>  
#include <map>  
  
using namespace std;  
  
int main(int argc, char *argv[], char *env[])  
{  
//  auto a;                 // 错误，没有初始化表达式，无法推断出a的类型  
//  auto int a = 10;        // 错误，auto临时变量的语义在C++11中已不存在, 这是旧标准的用法。  
  
    // 1. 自动帮助推导类型  
    auto a = 10;  
    auto c = 'A';  
    auto s("hello");  
  
    // 2. 类型冗长  
    map<int, map<int,int> > map_;  
    map<int, map<int,int>>::const_iterator itr1 = map_.begin();  
    const auto itr2 = map_.begin();  
    auto ptr = []()  
    {  
        std::cout << "hello world" << std::endl;  
    };  
  
    return 0;  
};  
  
// 3. 使用模板技术时，如果某个变量的类型依赖于模板参数，  
// 不使用auto将很难确定变量的类型（使用auto后，将由编译器自动进行确定）。  
template <class T, class U>  
void Multiply(T t, U u)  
{  
    auto v = t * u;  
}
```
#### 2. 返回值占位


```
template <typename T1, typename T2>  
auto compose(T1 t1, T2 t2) -> decltype(t1 + t2)  
{  
   return t1+t2;  
}  
auto v = compose(2, 3.14); // v's type is double
```
3.使用注意事项
①我们可以使用valatile，pointer（*），reference（&），rvalue reference（&&） 来修饰auto


```
auto k = 5;  
auto* pK = new auto(k);  
auto** ppK = new auto(&k);  
const auto n = 6;
```
②用auto声明的变量必须初始化


```
auto m; // m should be intialized
```
③auto不能与其他类型组合连用


```
auto int p; // 这是旧auto的做法。
```
④函数和模板参数不能被声明为auto


```
void MyFunction(auto parameter){} // no auto as method argument  
  
template<auto T> // utter nonsense - not allowed  
void Fun(T t){}
```
⑤定义在堆上的变量，使用了auto的表达式必须被初始化


```
int* p = new auto(0); //fine  
int* pp = new auto(); // should be initialized  
   
auto x = new auto(); // Hmmm ... no intializer  
     
auto* y = new auto(9); // Fine. Here y is a int*  
auto z = new auto(9); //Fine. Here z is a int* (It is not just an int)
```
⑥以为auto是一个占位符，并不是一个他自己的类型，因此不能用于类型转换或其他一些操作，如sizeof和typeid


```
int value = 123;  
auto x2 = (auto)value; // no casting using auto  
  
auto x3 = static_cast<auto>(value); // same as above
```
⑦定义在一个auto序列的变量必须始终推导成同一类型


```
auto x1 = 5, x2 = 5.0, x3='r';  // This is too much....we cannot combine like this
```
⑧auto不能自动推导成CV-qualifiers（constant & volatile qualifiers），除非被声明为引用类型


```
const int i = 99;  
auto j = i;       // j is int, rather than const int  
j = 100           // Fine. As j is not constant  
  
// Now let us try to have reference  
auto& k = i;      // Now k is const int&  
k = 100;          // Error. k is constant  
  
// Similarly with volatile qualifer
```
⑨auto会退化成指向数组的指针，除非被声明为引用


```
int a[9];  
auto j = a;  
cout<<typeid(j).name()<<endl; // This will print int*  
  
auto& k = a;  
cout<<typeid(k).name()<<endl; // This will print int [9]
```
