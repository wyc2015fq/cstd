# 【剑指offer】题2：实现单件(singleton)模式 - xiaxzhou的博客 - CSDN博客





2017年06月09日 15:58:43[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：227








#### **实现单件(singleton)模式**
- 单件： 

> 
不允许自动生成对象、不允许赋值、拷贝对象：屏蔽构造函数、拷贝构造函数、赋值运算符


- 加锁并两次判断 

> **互斥量：Mutex** +**区域锁：socpedlock**


  区域锁即使用**RAII**模式（Resource Acquisition Is Initialization）：把锁封装到一个对象里面。 

  锁的初始化(mutex.lock())放到构造函数， 

  锁的释放( mutex.unlock())放到析构函数。 


  这样当锁离开作用域时，析构函数会自动释放锁。即使运行时抛出异常，由于析构函数仍然会自动运行，所以锁仍然能自动释放。(**Effective C++ 条款13**) 


  (参考:[http://blog.csdn.net/zouxinfox/article/details/5848519](http://blog.csdn.net/zouxinfox/article/details/5848519)) 


  注：互斥量与二元信号量类似，但是互斥量的加锁和解锁进程必须是**同一个线程**- 内存释放：使用对象管理资源： 

> 
**new**的对象在程序结束时也**不会**自动**调用析构函数**析构，是系统强制回收了内存。而普通对象，静态对象在程序结束时会自动调用析构函数完成析构。


- 静态成员函数的定义、静态成员变量需要类外定义

> 
注：静态成员变量在类中仅仅是声明，没有定义，所以要在类的外面定义，实际上是给静态成员变量分配内存 

  （参考：[https://my.oschina.net/u/1537391/blog/219432](https://my.oschina.net/u/1537391/blog/219432)）


#### **模板singleton类：**

```
template<typename T>
class singleton
{
private:
    singleton(){};
    singleton(const singleton&);
    singleton& operator=(const singleton&);

    static T* instance;
    static Mutex mutex_;

public:

    class AutoRelease
    {
    public:
        ~AutoRelease()
        {
            if (GetInstance() != NULL)
            {
                delete GetInstance();
                GetInstance() = NULL;
            }
        }
    };

    static T*& GetInstance()
    {
        if (instance==NULL)
        {
            scopedlock raii(mutex_);
            if (instance == NULL)
            {
                instance = new T;
                static AutoRelease ar;
            }
        }
        return instance;
    }
};

template<T>
T* singleton<T>::instance = NULL;

template<T>
Mutex singleton<T>::mutex_;
```

#### **singleton类：**

```
class singleton
{
private:
    singleton(){};
    singleton(const singleton&);
    singleton& operator=(const singleton&);

    static singleton* instance;
    static Mutex mutex_;

public:

    class AutoRelease
    {
    public:
        ~AutoRelease()
        {
            if (GetInstance() != NULL)
            {
                delete GetInstance();
                GetInstance() = NULL;
            }
        }
    };

    static singleton*& GetInstance()
    {
        if (instance==NULL)
        {
            scopedlock raii(mutex_);
            if (instance == NULL)
            {
                instance = new singleton;
                static AutoRelease ar;
            }
        }
        return instance;
    }
};

singleton* singleton::instance = NULL;

Mutex singleton::mutex_;
```

#### 懒汉式、饿汉式?

[http://www.cnblogs.com/kkgreen/archive/2011/09/05/2166868.html](http://www.cnblogs.com/kkgreen/archive/2011/09/05/2166868.html)

首先，懒汉式是典型的以时间换取空间的例子，就是每次获取实例时都要进行判断，看是否要创建实例，浪费判断时间。当然如果一直没有人用的话，就不会创建实例，则是节约空间。而饿汉式是典型的以空间换取时间，就是说当类装载的时候，就创建出一个实例，不管你用不用它，然后每次调用时就不用判断了，节省了运行时间。

#### 单例模式的坏处?

1、扩展困难 

首先，如果是private的构造函数，会导致无法继承。 

其次，如果改为protect：**GetInstance静态函数没有办法生成子类的实例。如果要拓展，只有重写那个类**。
2、隐式使用引起类结构不清晰。比如有时候，你并不知道某个类A是单例类，当你读类B的时候，你可能先看它头文件，或者类视图里的内容，从这里你无法知道A和B 关系，因为B类在实现的时候才使用A类的那个所谓的GetInstance函数，读不到这行，你就不会知道B类对A类的依赖关系。

3、导致程序内存泄露的问题。很多人只是调用了GetInstance生成唯一的实例，却永远new被封装在GetInstance里忘了去释放内存（使用RAII解决）






