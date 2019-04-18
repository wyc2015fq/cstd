# Singleton模式小探 - tkokof1的专栏 - CSDN博客

2010年07月25日 14:44:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：1075



Singleton模式小探

一.缘起

设计模式，这个多年前因为[《设计模式》](http://book.douban.com/subject/1052241/)这本书而开始名声鹊起的软件名词，相信在当今年代早已为相关人士耳熟能详，你可以像大多数人那样对他顶礼膜拜，你也可以如极少数人一般对其嗤之以鼻，但是有一点是可以肯定的，那就是你肯定曾经听说过他、接触过他甚至运用过他（什么，你不知道什么是“设计模式”？……好吧，我是武断了点，请点击[这里](http://en.wikipedia.org/wiki/Design_pattern_(computer_science))开始了解……）

说来有趣，我本人最初开始接触设计模式并非始于《设计模式》这部“[Gang of Four](http://en.wikipedia.org/wiki/Gang_of_Four_(software))”的经典，而是来自于一本讲述游戏编程的书：[《游戏编程All In One》](http://book.douban.com/subject/1706699/)，，而书中所涉及到的那个设计模式便是此次我想讲一讲的Singleton，在此不妨摘录一段相关代码，大家看的也直观些：

/* Mirus Input Manager class */

class mrInputManager

{

protected:

    /* DirectInput objects */

    LPDIRECTINPUT8 m_lpkDInput;

/* Singleton */

    static mrInputManager * m_pkSingleton;

public:

    /* Constructors / Destructor */

    mrInputManager (void);

    ~mrInputManager (void);

    /* Input devices manipulation routines */

    mrError32 Init (HINSTANCE hInstance);

    LPDIRECTINPUT8 GetInput (void);

 /* Singleton */

    static mrInputManager * GetSingleton (void);

};

当时书中并未提到这是什么设计模式，自己也并未有多少在意，只是后来在某次考试的时候，出乎预料的遇到了一个号位为“[桥接模式](http://en.wikipedia.org/wiki/Bridge_pattern)”的名词，这才幡然醒悟，觉得自己应该补习一下设计模式的相关知识了，这才翻了翻《设计模式》，进而也才了解到上面那段代码原来便是大名鼎鼎的Singleton模式 ：）后来自己在实际编码的过程中也遇到了不少需要运用Singleton的场合，于是对其的实现、复用等方面内容做了一些涉猎、思考和讨论，故而想到应该好好的总结一下，于是便有了这篇文章 ：）

二.探索

经典的《设计模式》对Singleton模式有如下的描述：

“对一些类来说，只有一个实例是很重要的。……我们怎么样才能保证一个类只有一个实例并且这个实例易于被访问呢？一个全局变量使得一个对象可以变访问，但他不能防止你实例化多个对象。一个更好的办法是，让类自身负责保存他的唯一实例。这个类可以保证没有其他实例可以被创建（通过截取创建新对象的请求），并且它可以提供一个访问该实例的方法。这就是Singleton模式” 

相当深刻的解说，虽然仅有短短数十字，但将Singleton模式出现的原因以及解决的途径讲的极透，快哉快哉 ：）

书中也颇为简洁的列出了一段C++的实现代码：

class Singleton

{

public:

    /* get the instance */

    static Singleton* Instance();

protected:

    /* avoid the instance creating */ 

    Singleton();

private:

    /* the instance pointer */

    static Singleton* _instance;

};

Singleton* Singleton::_instance = 0;

Singleton* Singleton::Instance()

{

    /* if _instance is 0,then create the instance */

    if( _instance == 0 )

        _instance = new Singleton;

    return _instance;

}

值得一提的是，除了上述实现之外，书中还另外给出了一种使用“注册”的Singleton实现方法，具体思路是将所有Singleton注册进一个“容器”（原书中是一个以“单件名、单件指针”值对为元素的链表），然后通过“容器”的接口来获取相应单件，有兴趣的朋友可以找出原著来看一看，在此为了简单起见，我就不赘述了。

（插曲：对于《设计模式》示例代码中普遍出现的以下划线（'_'）开头的变量，[Herb Sutter](http://en.wikipedia.org/wiki/Herb_Sutter)先生并不十分赞同，原因很简单，容易与库名冲突，具体可见[《Exceptional C++ Style》](http://book.douban.com/subject/1470842/)：） ）  

同样的，在[Rob大叔](http://en.wikipedia.org/wiki/Robert_Cecil_Martin)的名著[《敏捷软件开发：原则、模式与实践》](http://book.douban.com/subject/1140457/)中，同样对Singleton模式有很不错的诠释，让我们来看看相应的Singleton实现代码（Java）：

public class Singleton

{

    private static Singleton theInstance = null;

    private Singleton() {}

    public static Singleton Instance()

    {

        if( theInstance == null )

            theInstance = new Singleton();

        return theInstance;

    } 

}

相较之前《设计模式》中的C++实现源码，这个实现除了使用了Java语言，其他部分确实是一模一样，丝毫不差 ：）

不妨让我们来看看Rob大叔对于Singleton模式的一些评价：

Ø 优点：

      1.    跨平台

      2.    适用于任何类：只要把一个类的构造函数变成私有，并添加相应的静态函数和                变量，就可以使一个类变成Singleton。

      3.    可以透过派生类创建：不管基类是否是Singleton，其派生的子类都可以成为   Singleton。

      4.     延迟求值（Lazy evaluation）：如果Singleton从未使用过，则不会创建。

Ø 缺点：

      1.    摧毁方法未定义：没有好的方法去摧毁一个Singleton，或者解除其职责。

      2.    不能继承：从Singleton派生的类并不是Singleton。

      3.    效率问题：由于延迟求值的存在，每次调用Instance都需要执行if语句。

      4.    不透明性：Singleton的使用者必须明确所使用的类为单件类才能正确使用。

相当全面到位的评价，值得我们好好参考 ：）另外需要一提的是，Rob大叔在书中除了提到Singleton模式来实现单件以外，还提到了另外一个与Singleton异曲同工的设计模式：Monostate，基本思想是使用static变量的全局单例特性，有兴趣的朋友可以看看，不过由此我倒可以引出 [Scott Meyers](http://en.wikipedia.org/wiki/Scott_Meyers)先生在其著作[《More Effective C++》](http://book.douban.com/subject/1457891/)中实现的一种单件，原理同样也是使用了 static变量的全局单例特性，并且这种单件还被大家命名为了Meyers Singleton，让我们来看看：

class Thing

{

public:

  Thing &GetInstance()

  {

static Thing s_instance;

    return s_instance;

  }

private:

  Thing();

  Thing(Thing const &);

  Thing &operator =(Thing const &);

};

对于Singleton模式更加深入的讨论，则可以在[Alexandrescu](http://en.wikipedia.org/wiki/Andrei_Alexandrescu)先生的[《Modern C++ Design》](http://book.douban.com/subject/1119904/)中找到：

书中首先讲述了Singleton模式的缘起（当然，自然是《设计模式》 ：） ），提到“Singleton是一种经过该进的全局变量。他所带来的该进是，你无法产生第二个具有Singleton形态的对象”，同时也说到了Singleton模式的实现问题“描述十分简单，实现却很复杂”、“Singleton生命期的管理是实现Singleton时最伤脑筋的地方”。

考虑到：

Ø   1.由于C++静态变量初始化顺序难以控制，所以采用保存指针的方法以达到动    态赋值的目的。

Ø   2.为了防止返回值被用户以外的释放（delete），采用传回引用的方式。

Ø   3.为了避免用户手工创建Singleton对象，采用私有化构造函数和析构函数的做法。

Ø   4.为了防止Singleton对象之间的相互赋值，私有化Singleton的赋值函数（operator=）。

第一个版本的Singleton看起来应该是这个样子：

class Singleton

{

public:

    static Singleton& Instance();

    // other operations

private:

    static Singleton* pInstance_;

private:

    Singleton();

    Singleton( const Singleton& );

    Singleton& operator = ( const Singleton& );

    ~Singleton();  

};


接着，为了进一步解决所谓死引用（Dead Reference）的问题，即避免对于已经析构的Singleton进行访问，在Instance函数中添加了检查机制：

class Singleton

{

public:

    static Singleton& Instance()

    {

        if( !pInstance_ )

        {

            // check for dead reference

            if( destroyed_ )

            {

                OnDeadReference();

            }

            else

            {

                // call initialize

                Create();

            }

        }

        return *pInstance_;

    }

private:

    // create the new Singleton

    static void Create()

    {

        static Singleton theInstance;

        pInstance_ = &theInstance;

    } 

    static void OnDeadReference()

    {

        throw std::runtime_error( "Dead Reference Detected" );

    }

    virtual ~Singleton()

    {

        pInstance_ = 0;

        destroyed_ = true;

    }

    // data 

    static Singleton* pInstance_;

    static bool destroyed_;

    // disable 'tors/operator='

    // ...

};

代码中以 destroyed_ 这个变量来标记Singleton是否已经被释放，并在必要时使用OnDeadReference来处理不期望的死引用，方案则是简单的抛出一个运行时异常。

然后，为了进一步控制Singleton的生命周期，譬如在某些情况下，Singleton即使被摧毁了，也应该在访问时再度被创建（例如原书中提到的Log类），那么应该如何做呢？原书中使用了两种方式来达到这个目的：

一个便是作者称之为“Phoenix Singleton”的方案，即对于某个已经已经析构的Singleton，在检测到死引用时便重新创建：

class Singleton

{

public:

    static Singleton& Instance()

    {

        if( !pInstance_ )

        {

            // check for dead reference

            if( destroyed_ )

            {

                OnDeadReference();

            }

            else

            {

                // call initialize

                Create();

            }

        }

        return *pInstance_;

    }

private:

// added ...

    void KillPhoenixSingleton()

    {

        pInstance_->~Singleton();

    }

    static void OnDeadReference()

    {

        Create();

        // placement 'new'

        new(pInstance_) Singleton;

        // queue this destruction

        atexit( KillPhoenixSingleton );

        destroyed_ = false;

    }

    // create the new Singleton

    static void Create()

    {

        static Singleton theInstance;

        pInstance_ = &theInstance;

    } 

    virtual ~Singleton()

    {

        pInstance_ = 0;

        destroyed_ = true;

    }

    // data 

    static Singleton* pInstance_;

    static bool destroyed_;

    // disable 'tors/operator='

    // ...

};

黑体部分便是相较上一个版本修改的部分，其中使用了atexit函数注册KillPhoenixSingleton以达到进程结束时析构Singleton的目的。

另外一种能够更加精细控制Singleton生命周期的方案，是使用一个数值来记录Singleton的“寿命”，以此达到可以控制Singleton析构顺序的目的。具体的思路是使用一个数组来维护登记的Singleton（使用一个称为SetLongevity的模板函数），然后依据各个Singleton的“寿命”（一个名为longevity_的变量）来依次使用atexit注册对应的"析构"函数。当然，原理虽然并不复杂，但相关的实现代码却并不简易，在此出于简单起见，我并不打算完整的列出代码，有兴趣的朋友可以找出原书来看一看。

接着，作者又马不停蹄的步入了多线程的世界，讨论了如何在多线程的情况下来保证Singleton的唯一性。

最初的一个方案平淡无奇：

Singleton& Singleton::Instance()

{

    Lock guard(mutex_);

    If( !pInstance_ )

        pInstance_ = new Singleton;

    return *pInstance_; 

}

使用加锁机制来管理多线程的并发访问是多线程编程中常用的方法，但是此处的实现方式效率上存在着严重不足，因为不管pInstance_ 是否被创建，每次调用Instance()时都必须构造互斥体，这种代价在pInstance_已经创建时是完全不必要的，所以修改如下：

Singleton& Singleton::Instance()

{

    if( !pInstance_ )

    {

        Lock guard(mutex_);

        pInstance_ = new Singleton;

    }

    return *pInstance_; 

}

但是这个实现仍然存在问题，试想如果两个进程同时越过了if(!pInstance) 的“关卡”（这是完全有可能的），然后其中一个进程“加锁”并构造Singleton，然后顺利的返回，但接下来原先处于等待的进程也会进行同样的操作，即创建另外一份Singleton实体，然后不幸便就此产生了……

为了防止这种情况发生，作者使用了一个称为“双检测锁定（Double-checked Locking）”的技巧：

Singleton& Singleton::Instance()

{

    if( !pInstance_ )

    {

        Lock guard(mutex_);

If( !pInstance )

        {

            pInstance_ = new Singleton;

        }

    }

    return pInstance_; 

}

至此，多线程之间的并发访问问题也算是基本解决了 ：）

最后，作者仔细分析了Singleton模式的各项需求，并将这些需求悉心的分解于多个Policy之中，并实现了一个模板类SingletonHolder，由于相关实现代码比较复杂，在此仅列出相应的头文件声明，而具体的实现细节，有兴趣的朋友可以找出原书来翻一翻（提示：由于相关代码综合使用了譬如模板的模板参数等技法，阅读时要做好心理准备 ：） ）：

template

<

    class T,

    template<class> class CreationPolicy = CreatingUsingNew,

    template<class> class LifetimePolicy = DefaultLifetime,

    template<class> class ThreadingModel = SingleThreaded 

>

class SingletonHolder

{

public:

    static T& Instance();

private:

    // helper

    static void DestroySingleton();

    // protection

    SingletonHolder();

    SingletonHolder( const SingletonHolder& );

    SingletonHolder& operator = ( const SingletonHolder& );

    ~Singleton();

    // data

    typedef ThreadingModel<T>::VolatileType InstanceType;

    static InstanceType* pInstance_;

    static bool destroyed_;

};

另外值得一提的是，在[Matthew Wilson](http://en.wikipedia.org/wiki/Matthew_Wilson_(author))先生的[《Imperfect C++》](http://book.douban.com/subject/1470838/)一书中，对于上面的Singleton实现中存在的问题亦有一些讨论：在给不同Singleton类型的“寿命”进行赋值时，程序员很难有一个正确明晰的全局判断，而且当程序将来需要扩展的时候，由于可能增加新的Singleton，那么重新分配原先的Singleton“寿命”将成为一大难题……（原书中还提到了一些其他的Singleton实现方式，在此出于简单起见不再讲述，有兴趣的朋友可以找出原著来看一看 ：） ）

三.总结

好了，至此我们对于Singleton模式的探索也算是告一段落，不知你有什么感想，至少对于我而言，对于Singleton 模式也算是有了一点认识，既然如此，不如让我们循着上面的思路自己来实现一个简单的Singleton，我们当然不必像《modern C++ design》中那般考虑的如此周全和富于技巧，但同时也不能像很多著作中示例代码一般的简单直白，考虑到重复代码的复用，我决定采用模板的方式进行实现，当然相关代码远没有《modern C++ design》中那般的复杂：）：

/*!

  /file Singleton.h

  /author Hugo Yu

  /date 2010.7.25 12:00

  /brief   A Simple Template Imlementation Of Singleton Pattern(Single Thread)

         If You What to make some class to be Singleton,

         Please use Singleton<T>,and make sure that this 

         class can not be created, and has the default Constructor 

*/

#ifndef HUGOLIB_SINGLETON_H

#define HUGOLIB_SINGLETON_H

#include <stdexcept>

namespace HugoLib

{

template<typename T>

class Singleton

{

public:

    //! use this to access the instance

    static T& GetSingleton() 

    { 

        // if the Singleton has been destructored ...

        if( c_destroyed )

        {

            // handle the dead reference

            onDeadReference();     

        }

        // if the instance is not created 

        else if( !c_instance ) 

        {

            // create the instance

            createSingleton();    

        }

        return *c_instance;

    }

private:

    //! protection default constructor

    Singleton();

    //! protection copy constructor

    Singleton( const Singleton& );

    //! protection operator = function

    Singleton& operator = ( const Singleton& );

    //! create the instance and set the destuctor

    static void createSingleton() { c_instance = new T; atexit( deleteSingleton ); }

    //! handle the dead reference

    static void onDeadReference() { throw std::runtime_error( "Dead Singleton Reference Detected" ); }

    //! delete the singleton

    static void deleteSingleton() { c_instance->~T(); c_destroyed = true; }

private:

    //! the instance

    static T* c_instance;

    //! the destroyed flag

    static bool c_destroyed;

};

template<typename T>

T* Singleton<T>::c_instance = NULL;

template<typename T>

bool Singleton<T>::c_destroyed = false;

}// end of "HugoLib"

#endif

要使用这个Singleton类模板，你必须让你的类别拥有默认的私有构造函数，并且需要一个友元声明，就像这样：

/*!

    /file Singleton_Test_01.cpp

    /author Hugo Yu

    /date 2010.7.25 12:19

    /brief a test profram of Singleton Pattern

*/ 

#include <iostream>

using std::cout;

using std::endl;

#include "Singleton.h"

using HugoLib::Singleton;

class A

{

    friend class Singleton<A>;

public:

    void Print() { cout << "This is A" << endl; }

private:

    A() {};

    A( const A& ) {};

    A& operator = ( const A& ) { return *this; }

    ~A() {};

};

int main( void )

{

    Singleton<A>::GetSingleton().Print();

    system( "pause" );

    return EXIT_SUCCESS;

}

虽然仍然有些不足，但是对于一般的使用相信已经可以胜任了，至于更高层次的修改，我想就等到碰到之时再说吧 ：）



