# C++四种类型转换的关键字及其特点 - sinat_32043495的博客 - CSDN博客





2017年12月04日 10:15:35[LZXandTM](https://me.csdn.net/sinat_32043495)阅读数：106








static_cast　　

reinterpret_cast

dynamic_cast

const_cast



## 1.static_cast------运算符完成相关类型之间的转换



使用场景：如在同一类层次结构中的一个指针类型到另一个指针类型，整型到枚举类型，或者浮点型到整型等。

example 1:



class Base {};

class Derived : public Base {};

Base *a = new Base;

Derived *b = static_cast<Derived *>(a);

example 2:


double d = 3.14159265;
int i = static_cast<int>(d);


example 3:


class A 
{}; 
class B:public A 
{}; 
class C 
{}; 
int main() 
{ 
    A* a=new A; 
    B* b; 
    C* c; 
    b=static_cast<B>(a);  //编译不会报错, B类继承A类
    c=static_cast<B>(a);  //编译报错,C类与A类没有任何关系
    return 1; 
}




## 2.reinterpret_cast------处理互不相关类型之间的转换



使用场景：如从整型到指针，一种类型的指针到另一种类型的指针等,可以转化任何内置的数据类型为其他任何的数据类型，也可以转化任何指针类型为其他的类型。它甚至可以转化内置的数据类型为指针，无须考虑类型安全或者常量的情形。

example
 1:




class A {};

class B {};

A * a = new A;

B * b = reinterpret_cast<B *>(a);

example 2:


int a=10;

double* b=reinterpret_cast<double*>(a);

## 3.const_cast用来移除变量的const或volatile限定符。

不能在不同的种类间转换,它仅仅把一个它作用的表达式转换成常量,它可以使一个本来不是const类型的数据转换成const类型的,或者把const属性去掉.


example 1:


class C {};

const C *a = new C;

C *b = const_cast<C *>(a);



注：

volatile的作用是:作为指令关键字，确保本条指令不会因编译器的优化而省略，且要求每次直接读值.

简单地说就是防止编译器对代码进行优化.比如如下代码：


a=1;


a=2;


a=3;


a=4;

对外部硬件而言,上述四条语句分别表示不同的操作,会产生四种不同的动作，但是编译器却会对上述四条语句进行优化，认为只有a=4（即忽略前三条语句，只产生一条机器代码）.如果键入volatile,则编译器会逐一的进行编译并产生相应的机器代码(产生四条代码)


## 4.dynamic_cast------处理基类型到派生类型的转换

使用场景：基类必须有虚函数，即为多态时，可以转换
（1）其他三种都是编译时完成的,dynamic_cast是运行时处理的,运行时要进行类型检查;
（2）不能用于内置的基本数据类型的强制转换;
（3）dynamic_cast转换如果成功的话返回的是指向类的指针或引用,转换失败的话则会返回NULL;
（4）使用dynamic_cast进行转换的,基类中一定要有虚函数,否则编译不通过.B中需要检测有虚函数的原因：类中存在虚函数,就说明它有想要让基类指针或引用指向派生类对象的情况,此时转换才有意义.这是由于运行时类型检查需要运行时类型信息,而这个信息存储在类的虚函数表;

（5）在类的转换时，在类层次间进行上行转换时,dynamic_cast和static_cast的效果是一样的.在进行下行转换时,dynamic_cast具有类型检查的功能,比static_cast更安全.向上转换即为指向子类对象的向下转换,即将父类指针转化子类指针.向下转换的成功与否还与将要转换的类型有关,即要转换的指针指向的对象的实际类型与转换以后的对象类型一定要相同,否则转换失败。
参考例子:


example
 1:


#include<iostream> 
#include<cstring> 
using namespace std; 
class A 
{ 
    public: 
    virtual void f() 
    { 
         cout<<"hello"<<endl; 
      }; 
}; 

class B:public A 
{ 
    public: 
    void f() 
    { 
         cout<<"hello2"<<endl; 
     }; 
}; 

class C 
{ 
   void pp() 
   { 
       return; 
   } 
}; 

int fun() 
{ 
     return 1; 
} 


int main() 
{ 
    A* a1=new B;//a1是A类型的指针指向一个B类型的对象 
    A* a2=new A;//a2是A类型的指针指向一个A类型的对象 
    B* b; 
    C* c; 
    b=dynamic_cast<B*>(a1);//结果为not null，向下转换成功，a1之前指向的就是B类型的对象，所以可以转换成B类型的指针。 
    if(b==NULL) 
    { 
         cout<<"null"<<endl; 
    } 
    else
    { 
         cout<<"not null"<<endl; 
    } 
    b=dynamic_cast<B*>(a2);//结果为null，向下转换失败 
    if(b==NULL) 
    { 
         cout<<"null"<<endl; 
    } 
    else
    { 
         cout<<"not null"<<endl; 
    } 
    c=dynamic_cast<C*>(a);//结果为null，向下转换失败 
    if(c==NULL) 
    { 
         cout<<"null"<<endl; 
    } 
    else
    { 
         cout<<"not null"<<endl; 
    } 
    delete(a); 
    return 0; 
}

example
 2:


class Base{virtual dummy() {}; };  

class Derived: public Base{} ;  

Base *b1 = new Derived;  

Base *b2 = new Base;  

Derived *d1 = dynamic_cast<Derived *>(b1); // success

Derived *d2 = dynamic_cast<Derived *>(b2); // fails: return NULL




from：

1.http://blog.csdn.net/u010025211/article/details/48626687


2.https://www.cnblogs.com/junlinfeizixiao/p/6193412.html

3.http://blog.sina.com.cn/s/blog_4b20940201013ryp.html










