# 谈谈重载（overload）覆盖（override）与隐藏 - ljx0305的专栏 - CSDN博客
2009年03月12日 20:30:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：421
这三个概念都是与OO中的多态有关系的。如果单是区别重载与覆盖这两个概念是比较容易的，但是隐藏这一概念却使问题变得有点复杂了，下面说说它们的区别吧。
重载是指不同的函数使用相同的函数名，但是函数的参数个数或类型不同。调用的时候根据函数的参数来区别不同的函数。
覆盖（也叫重写）是指在派生类中重新对基类中的虚函数（注意是虚函数）重新实现。即函数名和参数都一样，只是函数的实现体不一样。
隐藏是指派生类中的函数把基类中相同名字的函数屏蔽掉了。隐藏与另外两个概念表面上看来很像，很难区分，其实他们的关键区别就是在多态的实现上。什么叫多态？简单地说就是一个接口，多种实现吧。
还是引用一下别人的代码来说明问题吧（引用自林锐的《高质量C/C++编程指南》）。
仔细看下面的代码：
class Derived : public Base
{
public:
virtualvoid f(float x){ cout << "Derived::f(float) " << x << endl; }
void g(int x){ cout << "Derived::g(int) " << x << endl; }
void h(float x){ cout << "Derived::h(float) " << x << endl; }
};
看出什么了吗？下面说明一下：
（
1）函数Derived::f(float)覆盖了Base::f(float)。 
（
2）函数Derived::g(int)隐藏了Base::g(float)，而不是重载。 
（
3）函数Derived::h(float)隐藏了Base::h(float)，而不是覆盖。 
嗯，概念大概明白了，但是在实际的编程中，我们会因此遇到什么问题呢？再看下面的代码：
void main(void)
{
Derivedd;
Base *pb = &d;
Derived *pd = &d;
// Good : behavior depends solely on type of the object
pb->f(3.14f);// Derived::f(float) 3.14 
pd->f(3.14f);// Derived::f(float) 3.14
// Bad : behavior depends on type of the pointer
pb->g(3.14f);// Base::g(float) 3.14 
pd->g(3.14f);// Derived::g(int) 3(surprise!)
// Bad : behavior depends on type of the pointer
pb->h(3.14f);
// Base::h(float) 3.14(surprise!) 
pd->h(3.14f);
// Derived::h(float) 3.14 
}
在第一种调用中，函数的行为取决于指针所指向的对象。在第二第三种调用中，函数的行为取决于指针的类型。所以说，隐藏破坏了面向对象编程中多态这一特性，会使得
OOP人员产生混乱。 
不过隐藏也并不是一无是处，它可以帮助编程人员在编译时期找出一些错误的调用。但我觉得还是应该尽量使用隐藏这一些特性，该加virtual时就加吧。
引用:http://blog.csdn.net/yanjun_1982/archive/2005/09/02/470405.aspx
