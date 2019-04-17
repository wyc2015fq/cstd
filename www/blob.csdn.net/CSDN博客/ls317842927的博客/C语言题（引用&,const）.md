# C语言题（引用&,const） - ls317842927的博客 - CSDN博客





2017年03月17日 12:38:26[ls317842927](https://me.csdn.net/ls317842927)阅读数：343








有如下程序段：

```cpp
class A
{
    int _a;
public:
    A(int a): _a(a)
    {
    }
    friend int f1(A &);
    friend int f2(const A &);
    friend int f3(A);
    friend int f4(const A);
};
```

以下调用哪个是错误的： 

A:  f1(0) 

B:  f2(0) 

C:  f3(0) 

D:  f4(0)
答案：A

可以将类A看做某个数据类型来理解，比如int 

int f1(A &) 可以理解为  int f1(int &a)  其中a是形参 

int f2(const A &)  可以理解为  int f2(const int &a) 

int f3(A)  可以理解为  int f3(int a) 

int f4(const A) 可以理解为  int f4(const int a)
&是引用符号，变量的引用就是变量的别名。 

用程序可以这样： 

int a=1; //这里是定义一个整形变量 

int &b=a;//声明b是整型变量a的别名 

int &c=b;//声明c是整型引用变量b的别名 

int &d=a;//声明d是整型变量a的别名 

引用初始化后不能再被重新声明为另一变量的别名 

即三娃子既然是指张三这个人，就不能让其他人也叫三娃子 

对于 int &a=b;这个例子来说，要输出a和b 的地址，肯定是相同的。 

即一个别名只能对应一个原始变量，但是一个原始变量可以有多个别名。
5、可以用const对引用加以限定，不允许直接改变该引用的值，但是可以改变原变量的值去改变引用的值； 

int i=5; 

const int &a=i; 

a=3;//错误，因为引用a是const int 类型不能直接改变引用的值 

但是可以这样修改： 

i=3; 

此时输出i和a都是3
6、可以用常量或表达式对引用进行初始化，但此时必须用const作声明。 

int i=5; 

const int &a=i+1; 

此时编译系统是这样处理的：生成一个临时变量，用来存放该表达式的值，引用是该临时变量的别名、系统将“const int &a=i+1;”转换为 

int temp=i+1; 

const int &a=temp; 

临时变量是在内部实现的，用户不能访问临时变量； 

用这种办法不仅可以用表达式对引用进行初始化，还可以用不同类型的变量对之初始化；如 

double d=3.1415926; 

const int &a=d; 

以上等价为： 

double d=3.1415926; 

int temp=d; 

const int &a=temp; 
**如果在上面不用const则会发生错误**

double d=3.1415926; 

int &a=d;//未加const，错误 

为什么？因为如果可以这样做，那么修改引用a的值(如a=3.56),则临时变量temp的值也变为3.56，即修改了临时变量temp的值，但不能修改变量d的值，这往往不是用户所希望的，即存在二义性。与其允许修改引用的值而不能实现用户的目的，还不如不允许修改引用的值。这就是C++规定对这类引用必须加const的原因。
参考：[int int& int * int**的区别、联系和用途（一）](http://blog.csdn.net/willian0621/article/details/12838157)

所以 

A：int f1(A &) 可以理解为  int f1(int &a)  。 

f1(0) 表示 int &a=0 

B：int f2(const A &)  可以理解为  int f2(const int &a) 

f2(0) 表示 const int &a=0 

由于红字以下的解释， 所以A错。B对。
& 本质是const指针，就是不可以再指向其他变量。 

const & 是双重const。

关于const： 

1、可以定义const常量，如const int Max = 100;  

2、指针使用CONST 

(1)指针本身是常量不可变，不可以再指向其他变量 

     char* const pContent; 
(2)指针所指向的内容是常量不可变 

     const char *pContent; 

(3)两者都不可变 

      const char* const pContent;  

3、函数中使用CONST
(1)const修饰函数参数 

a.传递过来的参数在函数内不可以改变(无意义，因为Var本身就是形参)

void function(const int Var);

b.参数指针所指内容为常量不可变

void function(const char* Var);

c.参数指针本身为常量不可变(也无意义，因为char* Var也是形参)

void function(char* const Var);

d.参数为引用，为了增加效率同时防止修改。修饰引用参数时：

void function(const Class& Var); //引用参数在函数内不可以改变

void function(const TYPE& Var); //引用参数在函数内为常量不可变

**这样的一个const引用传递和最普通的函数按值传递的效果是一模一样**的,他禁止对引用的对象的一切修改,唯一不同的是按值传递会先建立一个类对象的副本, 然后传递过去,而它直接传递地址,所以这种传递比按值传递更有效.另外只有引用的const传递可以传递一个临时对象,因为临时对象都是const属性, 且是不可见的,他短时间存在一个局部域中,所以不能使用指针,只有引用的const传递能够捕捉到这个家伙. 

参考：[关于C++ const 的全面总结](http://blog.csdn.net/Eric_Jo/article/details/4138548)











