# C++中几个值得分析的小问题 - 文章 - 伯乐在线
原文出处： [QingLiXueShi，2015-04-27](http://www.cnblogs.com/mengwang024/p/4458910.html?utm_source=tuicool)
下面3个小问题都是我认为C++ Beginner应该能够解答或辨别清楚的。希望我们能通过题目挖掘更多的信息，而不仅仅局限在解题。我最喜欢说的话：能力有限，所以作为抛砖引玉，希望共同讨论，指出错误。
另外，我都是碰到一个觉得有必要记录的问题，就写下来说说，所以每一篇内容可能不是单一主题。
**1、**先来看一道简单题目。有下面这个继承类：

```
class Person
{
public:
    void Walk()                //普通人的“走”
    {
        cout << "Person::Walk I am an Ordinary People." << endl;
    };        
};
class Student : public Person
{
public:
    void Walk()                //学生的“走”
    {
        cout << "Student::Walk I am a student." << endl;
    };        
};
```
你没看错Walk()是非虚函数。请解释下面代码：

```
Student s;
Person* pp = &s;
pp->Walk();
Student* ps= &s;
ps->Walk();
```
结果是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/12/ae2f2cf4bfd04e402579319f263a15e7.png)
**分析：**Walk()是非虚函数，被静态绑定所限制，所以pp、ps是什么类型就决定了调用的版本。这里，我还要说明的一点是：**明白接口继承和实现继承。声明一个non-virtual函数的目的是为了令derived class继承函数的接口及一份强制性实现**。所以，**绝不要重新定义继承而来的non-virtual函数**。
**2、**下面这个问题实质上也是静态绑定与动态绑定的问题，但看起来不那么明显。

```
class Shape
{
public:
    enum ShapeColor{Red, Green, Blue};                //形状颜色
    virtual void Draw(ShapeColor color = Red) const = 0;
};
class Circle : public Shape
{
public:
    virtual void Draw(ShapeColor color) const
    {
        cout << "I am Circle::Draw. ";
        cout << "My color = " << color << endl;
    }
};
class Rectangle : public Shape
{
public:
    virtual void Draw(ShapeColor color = Green) const            //缺省的参数值被更改了
    {
        cout << "I am Rectangle::Draw. ";
        cout << "My color = " << color << endl;
    }
};
```
我主要想说两个问题。
**（1）**当你下面这样调用时，请解释会发生什么情况。

```
Circle cr;            //(1) 编译不通过
cr.Draw();            
Shape *ps = &cr;    //(2)
ps->Draw();
```
没错，(1)通过对象调用而不指定参数是错误的，而(2)的结果是这样的：color = 0代表Red这你应该是知道的。
![](http://jbcdn2.b0.upaiyun.com/2016/12/5c9bfbe529fbea7df33002b9bd520327.png)
**分析：**通过对象调用是静态绑定，一定要指定参数值，因为静态绑定这个函数不从base class继承缺省参数值。动态绑定却可以从base class继承参数值。注意，这里我就不强调动态绑定和静态绑定的概念了，但下面这个一定是静态绑定：

```
Circle cr;
Circle *ps = &cr;    //这还是静态绑定，静态类型Circle *，编译不通过
ps->Draw();
```
**（2）**第二个我想说的问题，请解释下面的调用结果。

```
Shape* ps1 = new Rectangle;
ps1->Draw();
Shape* ps2 = new Circle;
ps2->Draw();
```
是这样令人可喜的结果：
![](http://jbcdn2.b0.upaiyun.com/2016/12/8e360c6d67c0aedd90617fd16a118be2.png)
你是说，你在Rectangle中已经将Draw的缺省值改为1（Green）了，怎么没效果？
**分析：**Rectangle::Draw的缺省参数值为GREEN，但ps2的静态类型为Shape*，所以此调用的缺省参数值来自Shape class。
如果你非要让Rectangle::Draw的参数有所改变，可以这样调用（提供参数）：

```
Shape* ps4 = new Rectangle;
ps4->Draw(Shape::Green);
Shape* ps5 = new Circle;
ps5->Draw(Shape::Green);
```
这个问题是想提醒你：**virtual函数是动态绑定，缺省参数值是静态绑定。所以，不应该重新定义这个缺省参数值。**
**3、**多重继承为什么会含有多个虚表指针而不是一个？
这道题是我看一位同学面试经验时，面试官提的，我试着回答一下，不知道在不在点子上，还请补充和指正。
答：多重继承下，**因为编译器对一个derived class实现了n-1个虚表，n表示上一层base class的个数**，当然假设每个base class都有至少有一个virtual函数，否则编译器是不会为其添加vptr和vtbl了。所以说有多少个虚表，自然就有多少个指针指向，而不是一个。
这样说我不知道合理不合理，可能面试官要问的点是“为什么需要多个虚表？一个虚表行不行？”
这个属于编译器厂商做的事情，标准并未规范。C++的父亲就做出过这样的一款编译器原型，通过增大vtbl的体积，每个slot上不只有一个指针，还有一个offset，用来调整this指针的指向。
**这样做的弊端是：**所有vtbl中的虚函数指针都包含这样一个offset，并且假设不需要调整this指向，调用时还是要做offset的加法操作，尽管offset此时为0。另外，vtbl中每个slot体积的膨胀。这些都是效率问题。
实际上，用来调整this的指向用的比较多的是trunk技术，必须以汇编编写才能获得高效率。另外，sun编译器就是把多个虚表连锁为1个，每个表格中含有下一个表格的指针（通过offset方式），这样就需要一个指针就好了。
理解能力有限，不知道问的是不是这么一回事？
