# C 可变长参数 VS C++11 可变长模板 - 文章 - 伯乐在线
原文出处： [zj510](http://blog.csdn.net/zj510/article/details/36633603)
有些时候，我们定义一个函数，可能这个函数需要支持可变长参数，也就是说调用者可以传入任意个数的参数。比如C函数printf().
我们可以这么调用。


```
printf("name: %s, number: %d", "Obama", 1);
```
那么这个函数是怎么实现的呢？其实C语言支持可变长参数的。
我们举个例子，


```
double Sum(int count, ...)  
{  
    va_list ap;  
    double sum = 0;  
  
    va_start(ap, count);  
  
    for (int i = 0; i < count; ++i)  
    {  
        double arg = va_arg(ap, double);  
        sum += arg;  
    }  
  
    va_end(ap);  
  
    return sum;  
}
```
上面这个函数，接受变长参数，用来把所有输入参数累加起来。可以这么调：


```
double sum = Sum(4, 1.0, 2.0, 3.0, 4.0);
```
那么C语言的这个函数有什么问题呢？
1. 函数本身并不知道传进来几个参数，比如我现在多传一个参数，或者少传一个参数，那么函数本身是检测不到这个问题的。这就可能会导致未定义的错误。
2. 函数本身也不知道传进来的参数类型。以上面的例子，假如我把第二个参数1.0改成一个字符串，又如何？答案就是会得到未定义的错误，也就是不知道会发生什么。
3. 对于可变长参数，我们只能用__cdecl调用约定，因为只有调用者才知道传进来几个参数，那么也只有调用者才能维持栈平衡。如果是__stdcall,那么函数需要负责栈平衡，可是函数本身根本不知道有几个参数，函数调用结束后，根本不知道需要将几个参数pop out。（注：某些编译器如VS，如果用户写了个__stdcall的可变长参数函数，VS会自动转换成__cdecl的，当然这是编译器干的事情）
在C++语言里面，在C++11之前，C++也只是兼容了C的这种写法，而C++本身并没有更好的替代方案。其实对于C++这种强类型语言而言，C的这种可变长方案等于是开了个后门，函数居然不知道传进来的参数是什么类型。所以在C++11里面专门提供了对可变长参数的更现代化的支持，那就是可变长模板。
**模板参数包(template parameter pack)**


```
template<typename... A> class Car;
```
typename…就表示一个模板参数包。可以这么来实例化模板：


```
Car<int, char> car;
```
再来看一个更加具体的例子：


```
template<typename T1, typename T2> class Car{};  
template<typename... A> class BMW : public Car<A...>{};  
BMW<int, char> car;
```
A…称之为包扩展(pack extension)，包扩展是可以传递的。比如继承的时候，或者直接在函数参数里面传递。然后当编译器进行推导的时候，就会对这个包扩展进行展开，上面的例子，A…就展开成了int, char。C++11定义了可以展开包的几个地方：
1. 表达式
2. 初始化列表
3. 基类描述列表
4. 类成员初始化列表
5. 模板参数列表
6. 通用属性列表
7. lamda函数的捕捉列表
其他地方是不能展开的。
针对上面的例子，如果我们改成BMW car, 会如何呢？编译的时候就直接报错了，
Error 1 error C2977: ‘Car’ : too many template arguments d:studyconsoleapplication2variablelengthparametersvariablelengthparameters.cpp27 1 VariableLengthParameters
这是因为当展开的时候，A…变成了int, char, int了，可能基类根本就没有3个模板参数，所以推导就出错了。那如果这样的话，可变长参数还是啥意义呢？这等于每次的参数个数还是固定的啊。当然不会这么傻，其实C++11可以通过递归来实现真正的可变长的。看下面的代码。


```
template<typename... A> class BMW{};  
  
template<typename Head, typename... Tail>  
class BMW<Head, Tail...> : public BMW<Tail...>  
{  
public:  
    BMW()  
    {  
        printf("type: %s\n", typeid(Head).name());  
    }  
private:  
    Head head;  
};  
  
template<> class BMW<>{};  // 边界条件  
  
BMW<int, char, float> car;
```
如果我们运行这段代码，会发现构造函数被调用了3次。第一次得到的类型是float，第二次是char，第三次是int。这就好像模板实例化的时候层层展开了。实际上也就是这么一回事情。这里使用了C++模板的特化来实现了递归，每递归一次就得到一个类型。看一下对象car里面有什么：
![](http://jbcdn2.b0.upaiyun.com/2016/12/f0ba274dcac30f7fc4339f9777320d1c.png)
可以清晰的看到car里面有三个head。基类里面的head是float，第二个head是char，第三个head是int。
有了这个基础之后，我们就可以实现我们的可变长模板类了，std::tuple就是个很好的例子。可以看看它的源代码，这里就不再介绍了。
可变长模板不光可以用于类的定义，也可以用户函数模板。接下来，就用可变长参数来实现一个Sum函数，然后跟上面的C语言版本做对比。
**可变长模板实现Sum函数**
直接看代码：


```
template<typename T1, typename... T2> double Sum2(T1 p, T2... arg)  
{  
    double ret = p + Sum2(arg...);  
  
    return ret;  
}  
  
double Sum2()  // 边界条件  
{  
    return 0;  
}
```
在上面的代码里面，可以很清楚的看到递归。


```
double ret2 = Sum2(1.0, 2.0, 3.0, 4.0);
```
这条调用代码同样得到结果10.这样过程可以理解为，边界条件的函数先执行完毕，然后4.0的执行完毕，再3.0，2.0，1.0以此被执行完毕。一个典型的递归。
ok，那么跟C语言版本相比，又有哪些好处呢？
**变长模板优点**
之前提到的几个C语言版本的主要缺点：
1. 参数个数，那么对于模板来说，在模板推导的时候，就已经知道参数的个数了，也就是说在编译的时候就确定了，这样编译器就存在可能去优化代码。
2. 参数类型，推导的时候也已经确定了，模板函数就可以知道参数类型了。
3. 既然编译的时候就知道参数个数和参数类型了，那么调用约定也就没有限制了。
来实验一下第二点吧


```
int _tmain(int argc, _TCHAR* argv[])  
{  
    double ret1 = Sum(4, 1.0, 2.0, 3.0, 4.0, "abcd");  
    double ret2 = Sum2(1.0, 2.0, 3.0, 4.0, "abcd");  
      
      
    return 0;  
}
```
Sum是C语言版本，最后一个参数传了个字符串，但是Sum函数是无法检测这个错误的。结果也就是未定义。
Sum2是个模板函数，最后一个参数也是字符串，在编译的时候就报错了，
Error 1 error C2111: ‘+’ : pointer addition requires integral operandd:\study\consoleapplication2\variablelengthparameters\variablelengthparameters.cpp29
1 VariableLengthParameters
double无法和字符串相加，这样在编译的时候就告诉我们这个错误了，我们就可以修复它，但是C语言的版本不会报错，代码也就失控了，不知道会得到什么结果。
怎么样，变长模板比C语言的变长参数好一些吧。
所以，我们还是尽可能使用C++11的变长模板吧。
**最后一个问题，为什么使用变长参数呢？**有些人可能会问，是不是可以把所有的参数放到一个list里面，然后函数遍历整个list，再相加呢？good point，
如果所有的参数类型都一样，确实可以这么做，但是如果参数类型不一样呢？那怎么放到一个list里面？像C++这种强类型语言可能做不到吧，确实弱类型语言比如php，python等，确实可以这么做。根据我的理解，脚本语言等弱类型语言不需要变长参数吧，或者不重要。但是C++还是需要的，
用可变长模板就没这个问题了，就算参数类型不一样，只要对应的类型有对应的操作，就没问题。当然像上面的例子，如果没有重载+,那么编译的时候就报错，这不就是我们需要的吗?
附：


```
// VariableLengthParameters.cpp : Defines the entry point for the console application.  
//  
  
#include "stdafx.h"  
  
#include "stdarg.h"  
#include <typeinfo>  
  
double Sum(int count, ...)  
{  
    va_list ap;  
    double sum = 0;  
  
    va_start(ap, count);  
  
    for (int i = 0; i < count; ++i)  
    {  
        double arg = va_arg(ap, double);  
        sum += arg;  
    }  
  
    va_end(ap);  
  
    return sum;  
}  
  
template<typename T1, typename... T2> double Sum2(T1 p, T2... arg)  
{  
    double ret = p + Sum2(arg...);  
  
    return ret;  
}  
  
double Sum2()  
{  
    return 0;  
}  
  
  
template<typename... A> class BMW{};  
  
template<typename Head, typename... Tail>  
class BMW<Head, Tail...> : public BMW<Tail...>  
{  
public:  
    BMW()  
    {  
  
        printf("type: %s\n", typeid(Head).name());  
    }  
  
    Head head;  
};  
  
template<> class BMW<>{};  
  
BMW<int, char, float> car;  
  
int _tmain(int argc, _TCHAR* argv[])  
{  
    double ret1 = Sum(4, 1.0, 2.0, 3.0, 4.0);  
    double ret2 = Sum2(1.0, 2.0, 3.0, 4.0);  
      
      
    return 0;  
}
```
