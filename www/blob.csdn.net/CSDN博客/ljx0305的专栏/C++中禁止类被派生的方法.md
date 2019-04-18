# C++中禁止类被派生的方法 - ljx0305的专栏 - CSDN博客
2008年11月09日 21:57:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：576标签：[c++																[delete																[文档																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
                 在我的那篇“[C++中虚析构函数的作用](http://blog.csdn.net/starlee/archive/2006/03/09/619827.aspx)”中我说明了为什么作为基类的类的析构函数必须是虚函数，同时也指出：为了避免产生虚函数表，如果类不是基类的话，析构函数就不需要声明为虚函数。
    但是，我们不能预料用户的行为，你不敢肯定用户是否会从你的类去派生自己的类。如果用户以一个基类指针去删除一个派生类的对象，就会发生派生类的析构函数不被调用的情况。这样做的危险性我想大家都知道。当然，你可以在类的说明文档中，甚至是在类的头文件中，说明你写的类不能作为基类。可是，谁又能保证，用户会仔细阅读这些说明呢？
    所以，我们最好的方法就是禁止类的派生。如果用户从你的类去派生自己的类，那么在编译阶段他就会知道这样做是错的，从而避免可能发生在运行阶段的析构函数不被调用的危险。而禁止类派生的方法就是把构造函数声明为私有的。例如下面的类就不能被派生：
class ClxNotBase
{
public:
~ClxNotBase();
private:
    ClxNotBase();
    ClxNotBase(const ClxNotBase& rhs);
};
    如果用户从类ClxNotBase派生了一个类，那么在编译阶段他就会得到一个不能访问私有成员函数的错误信息。
    当然，你肯定会说：如果把类的构造函数声明为私有的，那么我们就无法构造这个类的对象，那我要这个类还有什么用呢？
    是的，你说的很对。不过，我们可以用很简单的方法来解决这个问题。下面是修改过的类ClxNotBase：
class ClxNotBase
{
public:
~ClxNotBase();
static ClxNotBase * NewlxNotBase();
static ClxNotBase * NewlxNotBase(const ClxNotBase& rhs);
private:
    ClxNotBase();
    ClxNotBase(const ClxNotBase& rhs);
};
ClxNotBase * ClxNotBase::NewlxNotBase()
{
//  调用真正的构造函数
returnnew ClxNotBase();
}
ClxNotBase * ClxNotBase::NewlxNotBase(const ClxNotBase& rhs) 
{ 
//  调用真正的拷贝构造函数
returnnew ClxNotBase(rhs); 
}
    用户在要使用类ClxNotBase的时候，就可以调用伪构造函数NewlxNotBase来生成对象。当然，每个伪构造函数都调用了new，这就意味着用户必须在使用完类ClxNotBase的对象后都必须调用delete。但是，释放不用的资源是每个C++程序员的基本素质，这个我们就不用强调了。而且，现在有了智能指针auto_ptr，可以自动删除所指的对象，如果用户知道用智能指针的话，那就更好了。下面是一个例子：
auto_ptr<ClxNotBase> p(ClxNotBase::NewlxNotBase());
    在这种情况下，就不用考虑delete对象的问题。在对象离开作用域的时候，智能指针会自动删除其所指的对象。
引用：[http://blog.csdn.net/starlee/archive/2006/03/14/623881.aspx](http://blog.csdn.net/starlee/archive/2006/03/14/623881.aspx)
