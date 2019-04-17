# C++虚函数与纯虚函数的区别 - 一世豁然的专栏 - CSDN博客





2015年09月15日 16:55:09[一世豁然](https://me.csdn.net/Explorer_day)阅读数：377标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C/C++/Java](https://blog.csdn.net/Explorer_day/article/category/2276819)









本博客转载于：[http://www.cnblogs.com/fzhe/archive/2013/01/02/2842513.html](http://www.cnblogs.com/fzhe/archive/2013/01/02/2842513.html)







今天在做一道面试题的时候遇到一个纯虚函数的概念，就翻了下书，查资料，对比下虚函数，现在整理一下和大家分享：

什么是虚函数？

那些被virtual关键字修饰的[成员函数](http://baike.baidu.com/view/2258939.htm)，就是虚函数。虚函数的作用，用专业术语来解释就是实现[多态性](http://baike.baidu.com/view/261058.htm)（Polymorphism），多态性是将接口与实现进行分离；用形象的语言来解释就是实现以共同的方法，但因个体差异而采用不同的策略。

虚函数声明如下：virtual ReturnType FunctionName(Parameter)；

虚函数必须实现，如果不实现，编译器将报错，错误提示为：


error LNK****: unresolved external symbol "public: virtual void __thiscall


ClassName::virtualFunctionName(void)"



为什么要用纯虚函数？

在很多情况下，[基类](http://baike.baidu.com/view/535539.htm)本身生成对象是不合情理的。例如，动物作为一个[基类](http://baike.baidu.com/view/535539.htm)可以派生出老虎、孔雀等子类，但动物本身生成对象明显不合常理。为了解决这个问题，方便使用类的多态性，引入了纯虚函数的概念，将函数定义为纯虚函数（方法：virtual
 ReturnType Function()= 0;），则编译器要求在派生类中必须予以重写以实现多态性。同时含有纯虚拟函数的类称为抽象类，它不能生成对象。



在什么情况下使用纯虚函数(pure vitrual function)?




1，当想在基类中抽象出一个方法，且该基类只做能被继承，而不能被实例化；




2，这个方法必须在派生类(derived class)中被实现；




   如果满足以上两点，可以考虑将该方法申明为pure virtual function.




我们来举个例子，我们先定义一个形状的类(Cshape)，但凡是形状我们都要求其能显示自己。所以我们定义了一个类如下：




class CShape




{




    virtual void Show(){};




};




但没有CShape这种形状，因此我们不想让CShape这个类被实例化，我们首先想到的是将Show函数的定义（实现）部分删除如下：




class CShape

{

    virtual void Show();

};

当我们使用下面的语句实例化一个CShape时：





CShape cs;  //这是我们不允许的，但仅用上面的代码是可以通过编译（但link时失败）。







怎么样避免一个CShape被实例化，且在编译时就被发现？




答案是：使用pure virtual funcion.




我们再次修改CShape类如下：




class CShape

{

public:

    virtual void Show()=0;

};

这时在实例化CShape时就会有以下报错信息：

error C2259: 'CShape' : cannot instantiate abstract class due to following members:

warning C4259: 'void __thiscall CShape::Show(void)' : pure virtual function was not defined




我们再来看看被继承的情况,我们需要一个CPoint2D的类，它继承自CShape.他必须实现基类(CShape)中的Show()方法。

   其实使用最初的本意是让每一个派生自CShape的类，都要实现Show()方法，但时常我们可能在一个派生类中忘记了实现Show(),为了避免这种情况，pure virtual funcion发挥作用了。

   我们看以下代码：

class CPoint2D:public CShape

{

public:

 CPoint2D()

 {

  printf("CPoint2D ctor is invoked\n");

 };

 void Msg()

 {

  printf("CPoint2D.Msg() is invoked\n");

 };



};



当我们实例化CPoint2D时，在编译时(at the compiling)也会出现如下的报错:


error C2259: 'CShape' : cannot instantiate abstract class due to following members:

warning C4259: 'void __thiscall CShape::Show(void)' : pure virtual function was not defined

   如上，我们预防了在派生类中忘记实现基类方法。









