# 深入理解C++的动态绑定和静态绑定 - maopig的专栏 - CSDN博客
2011年05月19日 22:34:00[maopig](https://me.csdn.net/maopig)阅读数：569
为了支持c++的多态性，才用了动态绑定和静态绑定。理解他们的区别有助于更好的理解多态性，以及在编程的过程中避免犯错误。
需要理解四个名词：
1、对象的静态类型：对象在声明时采用的类型。是在编译期确定的。
2、对象的动态类型：目前所指对象的类型。是在运行期决定的。对象的动态类型可以更改，但是静态类型无法更改。
关于对象的静态类型和动态类型，看一个示例：
view plaincopy to clipboardprint?
class B   
{   
}   
class C : public B   
{   
}   
class D : public B   
{   
}   
D* pD = new D();//pD的静态类型是它声明的类型D*，动态类型也是D*   
B* pB = pD;//pB的静态类型是它声明的类型B*，动态类型是pB所指向的对象pD的类型D*   
C* pC = new C();   
pB = pC;//pB的动态类型是可以更改的，现在它的动态类型是C*  
class B
{
}
class C : public B
{
}
class D : public B
{
}
D* pD = new D();//pD的静态类型是它声明的类型D*，动态类型也是D*
B* pB = pD;//pB的静态类型是它声明的类型B*，动态类型是pB所指向的对象pD的类型D*
C* pC = new C();
pB = pC;//pB的动态类型是可以更改的，现在它的动态类型是C*
3、静态绑定：绑定的是对象的静态类型，某特性（比如函数）依赖于对象的静态类型，发生在编译期。
4、动态绑定：绑定的是对象的动态类型，某特性（比如函数）依赖于对象的动态类型，发生在运行期。
view plaincopy to clipboardprint?
class B   
{   
    void DoSomething();   
    virtual void vfun();   
}   
class C : public B   
{   
    void DoSomething();//首先说明一下，这个子类重新定义了父类的no-virtual函数，这是一个不好的设计，会导致名称遮掩；这里只是为了说明动态绑定和静态绑定才这样使用。   
    virtual void vfun();   
}   
class D : public B   
{   
    void DoSomething();   
    virtual void vfun();   
}   
D* pD = new D();   
B* pB = pD;  
class B
{
    void DoSomething();
    virtual void vfun();
}
class C : public B
{
    void DoSomething();//首先说明一下，这个子类重新定义了父类的no-virtual函数，这是一个不好的设计，会导致名称遮掩；这里只是为了说明动态绑定和静态绑定才这样使用。
    virtual void vfun();
}
class D : public B
{
    void DoSomething();
    virtual void vfun();
}
D* pD = new D();
B* pB = pD; 
让我们看一下，pD->DoSomething()和pB->DoSomething()调用的是同一个函数吗？
不是的，虽然pD和pB都指向同一个对象。因为函数DoSomething是一个no-virtual函数，它是静态绑定的，也就是编译器会在编译期根据对象的静态类型来选择函数。pD的静态类型是D*，那么编译器在处理pD->DoSomething()的时候会将它指向D::DoSomething()。同理，pB的静态类型是B*，那pB->DoSomething()调用的就是B::DoSomething()。
让我们再来看一下，pD->vfun()和pB->vfun()调用的是同一个函数吗？
是的。因为vfun是一个虚函数，它动态绑定的，也就是说它绑定的是对象的动态类型，pB和pD虽然静态类型不同，但是他们同时指向一个对象，他们的动态类型是相同的，都是D*，所以，他们的调用的是同一个函数：D::vfun()。
上面都是针对对象指针的情况，对于引用（reference）的情况同样适用。
指针和引用的动态类型和静态类型可能会不一致，但是对象的动态类型和静态类型是一致的。
D D;
D.DoSomething()和D.vfun()永远调用的都是D::DoSomething()和D::vfun()。
至于那些事动态绑定，那些事静态绑定，有篇文章总结的非常好:
我总结了一句话：只有虚函数才使用的是动态绑定，其他的全部是静态绑定。目前我还没有发现不适用这句话的，如果有错误，希望你可以指出来。
特别需要注意的地方
当缺省参数和虚函数一起出现的时候情况有点复杂，极易出错。我们知道，虚函数是动态绑定的，但是为了执行效率，缺省参数是静态绑定的。
view plaincopy to clipboardprint?
class B   
{   
 virtual void vfun(int i = 10);   
}   
class D : public B   
{   
 virtual void vfun(int i = 20);   
}   
D* pD = new D();   
B* pB = pD;   
pD->vfun();   
pB->vfun();  
class B
{
 virtual void vfun(int i = 10);
}
class D : public B
{
 virtual void vfun(int i = 20);
}
D* pD = new D();
B* pB = pD;
pD->vfun();
pB->vfun();
有上面的分析可知pD->vfun()和pB->vfun()调用都是函数D::vfun()，但是他们的缺省参数是多少？
分析一下，缺省参数是静态绑定的，pD->vfun()时，pD的静态类型是D*，所以它的缺省参数应该是20；同理，pB->vfun()的缺省参数应该是10。编写代码验证了一下，正确。
对于这个特性，估计没有人会喜欢。所以，永远记住：
“绝不重新定义继承而来的缺省参数（Never redefine function’s inherited default parameters value.）”
关于c++语言
目前我基本上都是在c++的子集“面向对象编程”下工作，对于更复杂的知识了解的还不是很多。即便如此，到目前为止编程时需要注意的东西已经很多，而且后面可能还会继续增多，这也许是很多人反对c++的原因。
c++是Google的四大官方语言之一。但是Google近几年确推出了go语言，而且定位是和c/c++相似。考虑这种情况，我认为可能是Google的程序员们深感c++的复杂，所以想开发一种c++的替代语言。有时间要了解一下go语言，看它在类似c++的问题上时如何取舍的。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/chgaowei/archive/2011/05/17/6427731.aspx](http://blog.csdn.net/chgaowei/archive/2011/05/17/6427731.aspx)
