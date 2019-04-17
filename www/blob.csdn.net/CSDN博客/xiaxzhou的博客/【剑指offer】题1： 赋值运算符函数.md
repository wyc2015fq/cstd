# 【剑指offer】题1： 赋值运算符函数 - xiaxzhou的博客 - CSDN博客





2017年06月08日 22:38:41[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：189







- 函数返回类型：

> 
令operator=返回reference to *this （**Effective C++ 条款10**）

- 函数参数：

> 
一般应该是常量引用类型

- 异常安全：

> 
在赋值运算符函数中使用swap 

  使用**copy and swap**技术处理自身赋值和赋值失败异常问题（**Effective C++ 条款11**）

copy and swap策略：为打算做修改的对象做一份副本，在副本上做一切必要的修改，若修改动作抛出异常，原对象会保持原样。待所有操作成功后，将修改过的副本与元对象在一个不抛出异常的swap中置换。（**Effective C++ 条款29**）

- 编写swap函数：（**Effective C++ 条款25**）

> - 提供一个公有swap成员函数
- 提供一个非成员swap函数，调用swap成员函数
- **调用swap**：每个swap调用都应该未加任何限定，确保包含using std::swap; 然后赤裸裸地调用swap。因为如果有类型特定的swap版本，调用会与之匹配，如果不存在类型特定版本，则会调用std的版本。


swap在algorithm头文件中，需要
`#include <algorithm>`
> 
注：`<iostream`>与`<iostream.h`>的区别: 

  .h是继承自C语言，未引入名字空间定义，所以可直接使用。 

  不加.h的是现在C++中规定的标准，C++中新定义的方法都是有名字空间的，需要使用using。 
```
#include <algorithm>
class MyClass
{
public:
    void swap(MyClass& rhs);

protected:
private:
    int s;
};

void swap(MyClass& lhs, MyClass& rhs)//公有swap
{
    lhs.swap(rhs);//调用成员swap
}

void MyClass::swap(MyClass& rhs)
{
    using std::swap;
    swap(s, rhs.s);
}
```
- 在赋值运算符中使用copy-swap策略

```
MyClass& MyClass::operator=(const MyClass & rhs)
{
    MyClass tmp(rhs);//copy
    swap(tmp);       //swap
    return *this;
}
```

可将参数直接作为副本，改进如下

```
MyClass& MyClass::operator=(MyClass  rhs)//copy
{
    swap(rhs);//swap
    return *this;
}
```





