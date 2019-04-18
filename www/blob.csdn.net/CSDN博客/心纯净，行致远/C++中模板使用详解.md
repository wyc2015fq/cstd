# C++中模板使用详解 - 心纯净，行致远 - CSDN博客





2018年11月16日 22:33:34[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：33标签：[C  																[template](https://so.csdn.net/so/search/s.do?q=template&t=blog)](https://so.csdn.net/so/search/s.do?q=C  &t=blog)
个人分类：[【C++】](https://blog.csdn.net/zhanshen112/article/category/7639351)








**1. ****模板的概念。**

我们已经学过**重载(Overloading)**，对重载函数而言,C++的检查机制能通过函数参数的不同及所属类的不同。正确的调用重载函数。例如，为求两个数的最大值，我们定义MAX()函数需要对不同的数据类型分别定义不同**重载(Overload)**版本。

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//函数1.

int max(int x,int y);
{return(x>y)?x:y ;}

//函数2.
float max( float x,float y){
return (x>y)? x:y ;}

//函数3.
double max(double x,double y)
{return (c>y)? x:y ;}
![复制代码](http://common.cnblogs.com/images/copycode.gif)

但如果在主函数中，我们分别定义了 char a,b; 那么在执行max(a,b);时 程序就会出错，因为我们没有定义char类型的重载版本。

现在，我们再重新审视上述的max()函数，它们都具有同样的功能，即求两个数的最大值，能否只写一套代码解决这个问题呢？这样就会避免因重载函数定义不 全面而带来的调用错误。为解决上述问题C++引入模板机制，**模板定义：模板就是实现代码重用机制的一种工具，它可以实现类型参数化，即把类型定义为参数， 从而实现了真正的代码可重用性。模版可以分为两类，一个是函数模版，另外一个是类模版。**

**2.   ****函数模板的写法**

函数模板的一般形式如下：

Template <class或者也可以用typename T>

返回类型 函数名（形参表）
{//函数定义体 }
**说明： template是一个声明模板的关键字，表示声明一个模板关键字class不能省略，如果类型形参多余一个 ，每个形参前都要加class <类型 形参表>可以包含基本数据类型可以包含类类型.**

请看以下程序:

![复制代码](http://common.cnblogs.com/images/copycode.gif)

//Test.cpp

#include <iostream>

using std::cout;

using std::endl;
![复制代码](http://common.cnblogs.com/images/copycode.gif)

//声明一个函数模版,用来比较输入的两个相同数据类型的参数的大小，class也可以被typename代替，

//T可以被任何字母或者数字代替。

![复制代码](http://common.cnblogs.com/images/copycode.gif)

template <class T>

T min(T x,T y)

{ return(x<y)?x:y;}

 

void main( )

{

     int n1=2,n2=10;

     double d1=1.5,d2=5.6;

     cout<< "较小整数:"<<min(n1,n2)<<endl;

     cout<< "较小实数:"<<min(d1,d2)<<endl;

     system("PAUSE");

}
![复制代码](http://common.cnblogs.com/images/copycode.gif)

程序运行结果：

![](https://images0.cnblogs.com/blog/83792/201306/25111512-57d81b0a0c1844828c243a2cea233f2d.jpg)



程序分析：main()函数中定义了两个整型变量n1 , n2 两个双精度类型变量d1 , d2然后调用min( n1, n2); 即实例化函数模板T min(T x, T y)其中Ｔ为int型，求出n1,n2中的最小值．同理调用min(d1,d2)时，求出d1,d2中的最小值．

**3. ****类模板的写法**

定义一个类模板：

Template < class或者也可以用typename T >
class类名｛
／／类定义．．．．．．
｝;
**说明：其中，template是声明各模板的关键字，表示声明一个模板，模板参数可以是一个，也可以是多个。**

例如：定义一个类模板：

![复制代码](http://common.cnblogs.com/images/copycode.gif)

// ClassTemplate.h
#ifndef ClassTemplate_HH
#define ClassTemplate_HH

template<typename T1,typename T2>

class myClass{

private:
     T1 I;
     T2 J;

public:
     myClass(T1 a, T2 b);//Constructor
     void show();
};


//这是构造函数
//注意这些格式

template <typename T1,typename T2>

myClass<T1,T2>::myClass(T1 a,T2 b):I(a),J(b){}

 

//这是void show();

template <typename T1,typename T2>

void myClass<T1,T2>::show()
{
     cout<<"I="<<I<<", J="<<J<<endl;
}
#endif


// Test.cpp

#include <iostream>
#include "ClassTemplate.h"
using std::cout;
using std::endl;

void main()
{
     myClass<int,int> class1(3,5);
     class1.show();

     myClass<int,char> class2(3,'a');
     class2.show();

     myClass<double,int> class3(2.9,10);
     class3.show();

     system("PAUSE");
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)

最后结果显示：

![](https://images0.cnblogs.com/blog/83792/201306/25111745-71640ae18db543d5ad8823f418e6ca79.jpg)

**4.非类型模版参数**

一般来说，非类型模板参数可以是常整数（包括枚举）或者指向外部链接对象的指针。

那么就是说，浮点数是不行的，指向内部链接对象的指针是不行的。

![复制代码](http://common.cnblogs.com/images/copycode.gif)

template<typename T, int MAXSIZE>
class Stack{
Private:
       T elems[MAXSIZE];
…
};

Int main()
{

       Stack<int, 20> int20Stack;
       Stack<int, 40> int40Stack;
…

};
![复制代码](http://common.cnblogs.com/images/copycode.gif)




