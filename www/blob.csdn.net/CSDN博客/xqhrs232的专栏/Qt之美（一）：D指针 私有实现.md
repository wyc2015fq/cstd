# Qt之美（一）：D指针/私有实现 - xqhrs232的专栏 - CSDN博客
2016年12月05日 18:14:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：316
原文地址::[http://blog.csdn.net/zhu_xz/article/details/6035861](http://blog.csdn.net/zhu_xz/article/details/6035861)
相关文章
1、[Qt之美（二）：元对象](http://blog.csdn.net/zhu_xz/article/details/6048610)----[http://blog.csdn.net/zhu_xz/article/details/6048610](http://blog.csdn.net/zhu_xz/article/details/6048610)
2、[Qt之美（三）：隐式共享](http://blog.csdn.net/zhu_xz/article/details/6061201)----[http://blog.csdn.net/zhu_xz/article/details/6061201](http://blog.csdn.net/zhu_xz/article/details/6061201)

The English version is available at: [http://xizhizhu.blogspot.com/2010/11/beauty-of-qt-1-d-pointer-private.html](http://xizhizhu.blogspot.com/2010/11/beauty-of-qt-1-d-pointer-private.html)
相信不少刚开始阅读[Qt源代码](http://qt.gitorious.org/)的朋友在看到其中的Private类和诸如Q_D、Q_Q等宏时都会思考，为什么Qt要用这样一个设计模式呢？这样一段增加了不少复杂度的代码，到底有多大的好处呢？简单的说，这样的好处在于**保证代码的二进制兼容性**。
什么是二进制兼容性？大名鼎鼎的KDE项目是这样[介绍](http://techbase.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B)的：一个库是二进制兼容的，如果一个程序和某个库的某个版本动态链接，并且不需要重新编译，即可在安装有该库较新版本的环境中运行。为什么要保证二进制兼容性？如果不能保证库的二进制兼容性，就意味着每次发布新版本时，依赖该库的所有程序都必须重新编译才能正常运行。显然，这对于像Qt这样被广泛采用的库而言是完全不可接受的。关于二进制兼容性的更多信息，感兴趣的朋友可以参考下KDE
 TechBase上的[这篇文章](http://techbase.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B)，这里就不罗嗦了，仅仅和大家分享下具体的使用。
如果不使用D指针，那我们可能会有如下的一个类声明： 
**[cpp]**[view
 plain](http://blog.csdn.net/zhu_xz/article/details/6035861#)[copy](http://blog.csdn.net/zhu_xz/article/details/6035861#)
- class MyClass  
- {  
- public:  
-   MyClass();  
-   ~MyClass();  
- private:  
- int myVar;  
- };  
显然，这里的私有成员myVar是保证代码二进制兼容性的大敌，所以我们需要使用D指针，改写这个类：
**[cpp]**[view
 plain](http://blog.csdn.net/zhu_xz/article/details/6035861#)[copy](http://blog.csdn.net/zhu_xz/article/details/6035861#)
- class MyClassPrivate;  
- class MyClass  
- {  
- public:  
-   MyClass();  
-   ~MyClass();  
- private:  
-   MyClassPrivate * const d_ptr;  
-   Q_DECLARE_PRIVATE(MyClass);  
- };  
这里，我们定义了一个指针d_ptr指向私有实现类，然后用Q_DECLARE_PRIVATE宏来定义一些辅助函数和声明友元类：
**[cpp]**[view
 plain](http://blog.csdn.net/zhu_xz/article/details/6035861#)[copy](http://blog.csdn.net/zhu_xz/article/details/6035861#)
- #define Q_DECLARE_PRIVATE(Class) /
- inline Class##Private* d_func() { returnreinterpret_cast<Class##Private *>(qGetPtrHelper(d_ptr)); } /  
- inlineconst Class##Private* d_func() const { returnreinterpret_cast<const Class##Private *>(qGetPtrHelper(d_ptr)); } /  
- friendclass Class##Private;  
然后这个私有类的实现如下所示：
**[cpp]**[view
 plain](http://blog.csdn.net/zhu_xz/article/details/6035861#)[copy](http://blog.csdn.net/zhu_xz/article/details/6035861#)
- class MyClassPrivate  
- {  
- public:  
-   MyClassPrivate(MyClass *parent);  
- private:  
-   MyClass * const q_ptr;  
-   Q_DECLARE_PUBLIC(MyClass);  
- int myVar;  
- };  
这里的q_ptr指针就是指向公开的接口了，然后Q_DECLARE_PUBLIC宏则定义了辅助函数并声明了友元类：
**[cpp]**[view
 plain](http://blog.csdn.net/zhu_xz/article/details/6035861#)[copy](http://blog.csdn.net/zhu_xz/article/details/6035861#)
- #define Q_DECLARE_PUBLIC(Class)                                    /
- inline Class* q_func() { returnstatic_cast<Class *>(q_ptr); } /  
- inlineconst Class* q_func() const { returnstatic_cast<const Class *>(q_ptr); } /  
- friendclass Class;  
而我们还可以用Q_D和Q_Q两个宏来进一步简化访问：
**[cpp]**[view
 plain](http://blog.csdn.net/zhu_xz/article/details/6035861#)[copy](http://blog.csdn.net/zhu_xz/article/details/6035861#)
- #define Q_D(Class) Class##Private * const d = d_func()
- #define Q_Q(Class) Class * const q = q_func()
这就是Qt中D指针/私有实现的最基本使用方法。最后用一个比较完整的例子作为结尾;)
**[cpp]**[view
 plain](http://blog.csdn.net/zhu_xz/article/details/6035861#)[copy](http://blog.csdn.net/zhu_xz/article/details/6035861#)
- // myclass.h
- #ifndef MYCLASS_H
- #define MYCLASS_H
- #include <QtCore/QObject>
- class MyClassPrivate;  
- class MyClass: public QObject  
- {  
-   Q_OBJECT  
- public:  
-   MyClass(QObject *parent = 0);  
- virtual ~MyClass();  
- void dummyFunc();  
- signal:  
- void dummySignal();  
- private:  
-   MyClassPrivate * const d_ptr;  
-   Q_DECLARE_PRIVATE(MyClass);  
-   Q_DISABLE_COPY(MyClass);  
- };  
- #endif // MYCLASS_H
- // myclass.cpp
- #include "myclass.h"
- class MyClassPrivate  
- {  
- public:  
-   MyClassPrivate(MyClass *parent)  
-     : q_ptr(parent)  
-   {  
-   }  
- void foobar()  
-   {  
-     Q_Q(MyClass);  
-     emit q->dummySignal();  
-   }  
- private:  
-   MyClass * const q_ptr;  
-   Q_DECLARE_PUBLIC(MyClass);  
- };  
- MyClass::MyClass(QObject *parent)  
-   : QObject(parent)  
-   , d_ptr(new MyClassPrivate(this))  
- {  
- }  
- MyClass::~MyClass()  
- {  
-   Q_D(MyClass);  
- delete d;  
- }  
- void MyClass::dummyFunc()  
- {  
-   Q_D(MyClass);  
-   d->foobar();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
