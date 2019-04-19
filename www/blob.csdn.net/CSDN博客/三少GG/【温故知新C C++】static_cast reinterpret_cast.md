# 【温故知新C/C++】static_cast||reinterpret_cast - 三少GG - CSDN博客
2011年10月10日 22:42:55[三少GG](https://me.csdn.net/scut1135)阅读数：624标签：[class																[object																[c																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
                
.
## [static_cast、dynamic_cast reinterpret_cast和const_cast](http://blog.sina.com.cn/s/blog_4a84e45b0100f57m.html)
关于强制类型转换的问题，很多书都讨论过，写的最详细的是C++
 之父的《C++ 的设计和演化》。**最好的解决方法就是不要使用C风格的强制类型转换，而是使用标准C++的类型转换符：static_cast, dynamic_cast。标准C++中有四个类型转换符：static_cast、dynamic_cast、reinterpret_cast、和const_cast。**下面对它们一一进行介绍。
**static_cast**
用法：static_cast < type-id > ( expression_r_r )
该运算符把expression_r_r转换为type-id类型，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法：
用于类层次结构中基类和子类之间指针或引用的转换。进行上行转换（把子类的指针或引用转换成基类表示）是安全的；进行下行转换（把基类指针或引用转换成子类表示）时，由于没有动态类型检查，所以是不安全的。
用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
把空指针转换成目标类型的空指针。
把任何类型的表达式转换成void类型。
注意：static_cast不能转换掉expression_r_r的const、volitale、或者__unaligned属性。
**dynamic_cast**
用法：dynamic_cast < type-id > ( expression_r_r )
该运算符把expression_r_r转换成type-id类型的对象。Type-id必须是类的指针、类的引用或者void *；如果type-id是类指针类型，那么expression_r_r也必须是一个指针，如果type-id是一个引用，那么expression_r_r也必须是一个引用。
dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。
在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。
class B{
public:
int m_iNum;
virtual void foo();
};
class D:public B{
public:
char *m_szName[100];
};
void func(B *pb){
D *pd1 = static_cast<D *>(pb);
D *pd2 = dynamic_cast<D *>(pb);
}
在上面的代码段中，如果pb指向一个D类型的对象，pd1和pd2是一样的，并且对这两个指针执行D类型的任何操作都是安全的；但是，如果pb指向的是一个 B类型的对象，那么pd1将是一个指向该对象的指针，对它进行D类型的操作将是不安全的（如访问m_szName），而pd2将是一个空指针。另外要注意：B要有虚函数，否则会编译出错；static_cast则没有这个限制。这是由于运行时类型检查需要运行时类型信息，而这个信息存储在类的虚函数表（关于虚函数表的概念，详细可见<Inside
 c++ object model>）中，只有定义了虚函数的类才有虚函数表，没有定义虚函数的类是没有虚函数表的。
另外，dynamic_cast还支持交叉转换（cross cast）。如下代码所示。
class A{
public:
int m_iNum;
virtual void f(){}
};
class B:public A{
};
class D:public A{
};
void foo(){
B *pb = new B;
pb->m_iNum = 100;
D *pd1 = static_cast<D *>(pb); //copile error
D *pd2 = dynamic_cast<D *>(pb); //pd2 is NULL
delete pb;
}
在函数foo中，使用static_cast进行转换是不被允许的，将在编译时出错；而使用 dynamic_cast的转换则是允许的，结果是空指针。
**reinpreter_cast**
用法：reinpreter_cast<type-id> (expression_r_r)
type-id必须是一个**指针**、引用、算术类型、**函数指针或者成员指针**。**它可以把一个指针转换成一个整数**，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。
该运算符的用法比较多。
**const_cast**
用法：const_cast<type_id> (expression_r_r)
该运算符用来修改类型的const或volatile属性。除了const 或volatile修饰之外， type_id和expression_r_r的类型是一样的。
常量指针被转化成非常量指针，并且仍然指向原来的对象；常量引用被转换成非常量引用，并且仍然指向原来的对象；常量对象被转换成非常量对象。
Voiatile和const类试。举如下一例：
class B{
public:
int m_iNum;
}
void foo(){
const B b1;
b1.m_iNum = 100; //comile error
B b2 = const_cast<B>(b1);
b2. m_iNum = 200; //fine
}
上面的代码编译时会报错，因为b1是一个常量对象，不能对它进行改变；使用const_cast把它转换成一个常量对象，就可以对它的数据成员任意改变。注意：b1和b2是两个不同的对象。
最容易理解的解释：
**dynamic_cast: 通常在基类和派生类之间转换时使用；const_cast: 主要针对const和volatile的转换. static_cast: 一般的转换，如果你不知道该用哪个，就用这个。 reinterpret_cast: 用于进行没有任何关联之间的转换，比如一个字符指针转换为一个整形数。**
++++++++++++++++
