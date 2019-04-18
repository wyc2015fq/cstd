# C/C++学习笔记 - qq229873466的博客 - CSDN博客

2017年03月10日 11:19:38[qq229873466](https://me.csdn.net/qq229873466)阅读数：217
个人分类：[C/C++](https://blog.csdn.net/qq229873466/article/category/6778966)



# 1.static

静态全局变量：它只在定义它的源文件内有效，其他源文件无法访问它。和普通全局变量一样存放在.data段（已初始化）或者.bss段（未初始化）内。

静态局部变量：只初始化一次。（多线程编程使用要注意）

   1）位置：静态局部变量被编译器放在全局存储区.data（注意：不在.bss段内，原因见3）），所以它虽然是局部的，但是在程序的整个生命周期中存在。

   2）访问权限：静态局部变量只能被其作用域内的变量或函数访问。也就是说虽然它会在程序的整个生命周期中存在，由于它是static的，它不能被其他的函数和源文件访问。

   3）值：静态局部变量如果没有被用户初始化，则会被编译器自动赋值为0，以后每次调用静态局部变量的时候都用上次调用后的值。这个比较好理解，每次函数调用静态局部变量的时候都修改它然后离开，下次读的时候从全局存储区读出的静态局部变量就是上次修改后的值。

静态函数：C的静态函数可以模仿C++的私有函数（在一个文件中定义静态函数，另一个文件无法调用）。

# 2.extern

extern “C”这个声明的真实目的：实现C++与C及其它语言的混合编程。

按照C的编译和链接方式进行，避免不兼容C++中新的特性（如：函数重载）。

# 3.inline

内联函数主要是解决频繁调用小函数大量消耗栈空间的问题。可以替代宏，避免出现下面的错误：

```cpp
#define fun(x) (x)*(x)
fun(a++);
```

# 4.typedef

1.给数据类型取别名。

2.简化函数指针书写：

```cpp
void (*signal(int, void(*)(int)))(int); //等价于下面
typedef void (*HANDLER)(int);
HANDLER signal(int, HANDLER);
```

# 5.const

1.必须初始化，否则编译错误。

2.C默认是extern，C++默认文件内有效。

3.const：有数据类型，编译进行安全检查，可调试。

   define:宏，不考虑数据类型，没有安检，不能调试。

4.类常量成员不能在声明时初始化。

5.先忽略类型名(编译器解析的时候也是忽略类型名)，我们看const离哪个近。"近水楼台先得月"，离谁近就修饰谁。

```cpp
//判断时忽略括号中的类型
const (int) *p;   //const修饰*p，*p是指针指向的对象，不可变
(int) const *p；  //const修饰*p，*p是指针指向的对象，不可变
(int)*const p;   //const修饰p，p不可变，p指向的对象可变
const (int) *const p;  //前一个const修饰*p，后一个const修饰p，指针p和p指向的对象都不可变
void xxx() const // 成员函数的this不能改变
```

# 6.template

实例化是所有类型有相同代码。

```cpp
template<typename T>
XXX<T>::Y(){
   ...
}
```

特殊化是不同的类型有不同的代码。

```cpp
template<>
XXX<T1>::Y(){
   ...
}
XXX<T2>::Y(){
   ...
}
```

# 7.do...while(0)

1.消除goto语句。

```cpp
boolExecute()
{
   //分配资源
  int *p = new int" _msthash="1428145"_mstchunk="true">int *p = new int;
 
  bool bOk(true" _msthash="1428146" _mstchunk="true">boolbOk(true);
   do
   {
     //执行并进行错误处理
     bOk =func1();
     if(!bOk) break;
 
     bOk =func2();
    if(!bOk) break;
 
     bOk =func3();
     if(!bOk) break;
 
     // ..........
 
  }while(0);
 
   //释放资源
  delete p;
   p=NULL;
   return bOk;
}
```

2.宏定义中使用

```cpp
#define SAFE_DELETE(p) do{ delete p; p =NULL} while(0)
```

# 8.零长度数组

结构体中的零长度数组不占空间，只有GNU C有这种用法。

# 9.指针加1

```cpp
int a[2][2][3] = { { { 1, 2, 3 }, { 4, 5, 6 } }, { { 7, 8, 9 }, { 10, 11, 12 } } };
a+1 =  a + sizeof(a[0]); // { { 1, 2, 3 }, { 4, 5, 6 } }
&a + 1 = a + sizeof((&a)[0]); // { { { 1, 2, 3 }, { 4, 5, 6 } }, { { 7, 8, 9 }, { 10, 11, 12 } } }
```

# 10.C++对象访问私有函数

```cpp
class Base{
public:
    virtual void Say(){
        printf("Base::Say()\n");
    }
};
class Derived : public Base{
private:
    virtual void Say(){
        printf("Derived::Say()\n");
    }
};
Base *p = new Derived;
p->Say();
```

# 11.屏蔽warming C4127

当显示警告级别高于/W4时，若出现逻辑判断出现常量表达式时会出现C4127警告，如

```cpp
while(0) 
{}
```

应该改成

```cpp
while(void(0), 0) 
{}
```

# 12.返回常量的成员函数
为了防止返回值作为左值

# 13.编写继承父类的函数
在编写这种子类函数时最好前面加了virtual，以表示和子类独有函数的区别




