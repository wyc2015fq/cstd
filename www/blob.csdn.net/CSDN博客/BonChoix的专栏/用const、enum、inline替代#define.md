# 用const、enum、inline替代#define - BonChoix的专栏 - CSDN博客





2012年10月28日 07:49:37[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：3485








       在C语言程序设计中，define被大量地使用，用来定义常量或者宏，以减少代码重复及提高可维护性。但在C++中，引进了const、enum、inline后，大多数使用define的地方都可以用它们来代替（当然#ifndef、#define、#endif这种条件编译除外）。以下是几点提倡用const、enum、inline而不用define的理由：

       1. 定义常量

           如果用#define来定义常量，比如#define N 100，在程序在所有用到N地方，编译过程中直接被替换为100，在符号表中并不存在N这个变量，因此对于调试有时可能会造成麻烦。此外，N没有类型可言，因此对于C、C++这种强类型的语言，并不够安全。相反，定义常量最好的方法是用const，如果是定义int，则上述最好用const int N = 100。

       2. 类内部常量

           对这种情况，用#define是无法实现类内部常量的。因为define没有作用域的概念，只要#define定义后，后面的编译过程任何地方都有效。因此要定义类内部专属的常量，只能用const。

           顺便指出下，如果是定义针对类所有实体都共有的常量，则该常量应该同时为static。可以这样定义：

```cpp
class X
{
public:
	......
	
private:
	const static int N = 100;
};
```

          注意，这时给出的形式仅仅是N的声明式，而非定义式！真正的定义式应该位于cpp文件中，形如：

```cpp
const static int X::N = 100;
```

          但是，对于static、且仅仅为int型的常量，才可以像上面那样直接在类中声明！如果在程序中不取它们的地址的话，则可以不给出定义式。如果取地址，则必须保证在cpp文件中给出定义式！

         (注：《Effective C++，3rd》上作者确实说不可取地址，不过我在VS2010上测试后发现还是可以取地址的.....在gcc编译器(确切说是g++)上明确表示不可取地址，未定义！不得不说，经常发现微软的编译器确认挺奇葩的，不止一次两次。。。。）

         对于其他非int 的类型，即使是const static，也不允许直接在类里面给出初始化的声明。如下则为非法的：

```cpp
class X
{
public:
	......
	
private:
	const static float N = 100.f;
};
```



       3. 针对旧式编译器

       旧式的编译器有可能不支持在类内部直接初始化static int型的常量，但有时候又需要用类内的常量来声明数组，如下情况：

```cpp
class X
{
public:
	......
	
private:
	const static int N = 5;
	int array[N];
};
```

此时可以利用enum常量为代替const int 来巧妙地实现。

```cpp
class X
{
public:
	......
	
private:
	enum { N = 5 };
	int array[N];
};
```

此时的enum常量可以充当int来被使用。但注意的是，enum常量N不可取地址！

       4. 宏的使用

        用define来定义宏，存在一系列的问题：

        首先，宏不具有任务类型检查，因此不够安全。

        其次，定义宏时，必需为所有的实参加上小括号，否则在调用时可能遇到麻烦。比如：

```cpp
#define MUL(a,b) a*b
MUL(a+1,b+1);
```

调用MUL(a+b,b+1)时，实际情况是a+1*b+1，即先计算1*b，再把a和1加进来。因此编译器在预处理时，仅仅是把宏进行原封不动地替换。因此正确写法是：

```cpp
#define MUL(a,b) (a)*(b)
```

        但即使是这种情况，还是会有一些不可思议的事情发生。比如：

```cpp
#define Square(a) (a)*(a)
int a = 5;
Square(a++);
```

       执行完Square后，我想绝大多数人的本意肯定是a = 6，但实际却是7，因为a被自增了两次：（a++)*(a++)。用inline函数，即可以像函数一样满足我们的本意：

```cpp
template<typename T>
inline T Square(const T &a)
{
	return a*a;
}
```

       inline函数既可以像函数一样有实现我们的需求，同时又可以像宏一样，不需要函数调用。因此这种情况下，应该尽可能用inline函数来代替宏的使用。



       小结：对于单纯常量，最好以const对象或enum替换#define。

                   对于形似函数的宏，最好改用inline函数替换#define。



       （见《Effective C++，3rd》，条款2）



