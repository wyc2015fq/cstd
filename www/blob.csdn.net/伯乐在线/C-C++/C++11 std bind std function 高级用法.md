# C++11 std::bind std::function 高级用法 - 文章 - 伯乐在线
原文出处： [eclipser1987](http://blog.csdn.net/eclipser1987/article/details/24406203)
从最基础的了解，std::bind和std::function


```
/*  
 * File:   main.cpp 
 * Author: Vicky.H 
 * Email:  eclipser@163.com 
 */  
#include <iostream>  
#include <functional>  
#include <typeinfo>  
#include <string.h>  
  
int add1(int i, int j, int k) {  
    return i + j + k;  
}  
  
  
class Utils {  
public:  
    Utils(const char* name) {  
        strcpy(_name, name);  
    }  
      
    void sayHello(const char* name) const {  
        std::cout << _name << " say: hello " << name << std::endl;  
    }  
      
    static int getId() {  
        return 10001;  
    }   
      
    int operator()(int i, int j, int k) const {  
        return i + j + k;  
    }  
      
private:  
    char _name[32];  
};  
  
  
/* 
 *  
 */  
int main(void) {  
      
    // 绑定全局函数  
    auto add2 = std::bind(add1, std::placeholders::_1, std::placeholders::_2, 10);  
    // 函数add2 = 绑定add1函数，参数1不变，参数2不变，参数3固定为10.  
    std::cout << typeid(add2).name() << std::endl;  
    std::cout << "add2(1,2) = " << add2(1, 2) << std::endl;  
      
    std::cout << "\n---------------------------" << std::endl;  
      
    // 绑定成员函数  
    Utils utils("Vicky");  
    auto sayHello = std::bind(&Utils::sayHello, utils/*调用者*/, std::placeholders::_1/*参数1*/);  
    sayHello("Jack");  
      
    auto sayHelloToLucy = std::bind(&Utils::sayHello, utils/*调用者*/, "Lucy"/*固定参数1*/);  
    sayHelloToLucy();  
      
    // 绑定静态成员函数  
    auto getId = std::bind(&Utils::getId);  
    std::cout << getId() << std::endl;  
      
    std::cout << "\n---------------------------" << std::endl;  
      
    // 绑定operator函数  
    auto add100 = std::bind(&Utils::operator (), utils, std::placeholders::_1, std::placeholders::_2, 100);  
    std::cout << "add100(1, 2) = " << add100(1, 2) << std::endl;  
      
    // 注意：无法使用std::bind()绑定一个重载函数  
      
    return 0;  
}
```


```
/*  
 * File:   main2.cpp 
 * Author: Vicky.H 
 * Email:  eclipser@163.com 
 */  
#include <iostream>  
#include <typeinfo>  
  
  
void sayHello() {  
    std::cout << "Hello world !" << std::endl;  
}  
  
int sum(int i, int j, int k) {  
    return i + j + k;  
}  
  
template <typename T>  
class Func {  
public:  
  
    Func(T fun) {  
        if (!fun) {  
            throw "fun nullptr";  
        }  
        _fun = fun;  
    }  
  
    template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>  
    R Call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {  
        return _fun(a1, a2, a3, a4, a5);  
    }  
  
    template<typename R, typename A1, typename A2, typename A3, typename A4>  
    R Call(A1 a1, A2 a2, A3 a3, A4 a4) {  
        return _fun(a1, a2, a3, a4);  
    }  
  
    template<typename R, typename A1, typename A2, typename A3>  
    R Call(A1 a1, A2 a2, A3 a3) {  
        return _fun(a1, a2, a3);  
    }  
  
    template<typename R, typename A1, typename A2>  
    R Call(A1 a1, A2 a2) {  
        return _fun(a1, a2);  
    }  
  
    template<typename R, typename A1>  
    R Call(A1 a1) {  
        return _fun(a1);  
    }  
  
    template<typename R>  
    R Call() {  
        return _fun();  
    }  
  
    void Call() {  
        _fun();  
    }  
  
private:  
    T _fun;  
};  
  
#include <functional>  
  
template<typename R = void, typename... Args>  
class Fn {  
public:  
    Fn(std::function<R(Args...)> fun) : _fun(fun) {  
    }  
      
    R operator()(Args... args) {  
        return _fun(args...);  
    }  
private:  
    std::function<R(Args...) > _fun;  
};  
  
/* 
 * 将函数注册到对象中，通过对象直接调用 
 */  
int main(void) {  
  
  
    Func<void(*)() > sayHelloFunc(sayHello);  
    sayHelloFunc.Call();  
  
  
    Func<int (*)(int, int, int) > sumFunc(sum);  
    std::cout << "sumFunc.Call<int>(1, 2, 3) : " << sumFunc.Call<int>(1, 2, 3) << std::endl;  
  
  
    std::cout << "\n---------------------------" << std::endl;  
  
    Fn<> sayHelloFn(sayHello);  
    sayHelloFn();  
      
    Fn<int, int, int, int> sumFn(sum);  
    std::cout << "sumFn(1, 2, 3) : " << sumFn(1, 2, 3) << std::endl;  
  
    std::cout << "\n---------------------------" << std::endl;  
  
    return 0;  
}
```
Hello world !
sumFunc.Call(1, 2, 3) : 6
—————————
Hello world !
sumFn(1, 2, 3) : 6
—————————
上面的例子非常有趣，使用了2种方案，将一个函数，注册到一个对象/仿函数中，并且通过一个对象/仿函数来直接调用调用。
例子显而易见的，第2种方案更佳简洁，并且对传递参数有明确的判断，当参数类型或数量不正确的时候，编译器将导致失败。
这种方案，可以将类的成员变量直接作为函数的参数使用，或者，如我：
http://blog.csdn.net/eclipser1987/article/details/23926395
这篇文章中，无法直接调用脚本函数类，有了好的解决办法。这个我将随后补充。


```
#include <list>  
#include <functional>  
  
template<typename... Args>  
class Fns  
{  
private:  
  
    std::list<std::function<void(Args...)> > _calls;  
  
public:  
  
    virtual ~Fns()  
    {  
        _calls.clear();  
    }  
  
    void connect(std::function<void(Args...)> fct)  
    {  
        _calls.push_back(fct);  
    }  
  
    template<typename Object>  
    void connect(Object* object, void (Object::*method)(Args...))  
    {  
        _calls.push_back([object,method](Args... args){(*object.*method)(args...);});  
    }  
  
    template<typename Object>  
    void connect(Object* object, void (Object::*method)(Args...) const)  
    {  
        _calls.push_back([object,method](Args... args){(*object.*method)(args...);});  
    }  
  
    template<typename Object>  
    void connect(const Object* object, void (Object::*method)(Args...) const)  
    {  
        _calls.push_back([object,method](Args... args){(*object.*method)(args...);});  
    }  
  
    void emit(Args... args)  
    {  
        for(auto call : _calls)  
            call(args...);  
    }  
};
```


```
#include <cstdio>  
#include "Signal.hpp"  
  
class Foo  
{  
public:  
  
    void bar(int x, int y)  
    {  
        printf("Foo::bar(%d, %d)\n", x, y);  
    }  
};  
  
void foobar(int x, int y)  
{  
    printf("foobar(%d, %d)\n", x, y);  
}  
  
int main(void)  
{  
    Foo foo;  
    Fns<int, int> s;  
  
    // Connect a function  
    s.connect(foobar);  
    // Connect a class method  
    s.connect(&foo, &Foo::bar);  
    // Create and connect some lambda expression  
    s.connect([&foo](int x, int y){   
        printf("lambda::"); foo.bar(x, y);   
    });  
    // Emit the signal !  
    s.emit(4, 2);  
    getchar();  
    return 0;  
}
```
foobar(4, 2)
Foo::bar(4, 2)
lambda::Foo::bar(4, 2)
