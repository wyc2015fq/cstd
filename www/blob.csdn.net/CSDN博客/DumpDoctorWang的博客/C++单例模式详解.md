# C++单例模式详解 - DumpDoctorWang的博客 - CSDN博客





2019年03月02日 14:30:51[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：45











### 目录
- [一、单例模式概念以及优缺点](#_4)
- [二、懒汉式单例模式](#_29)
- [1、原始懒汉式单例模式](#1_30)
- [2、线程安全的单例模式](#2_109)
- [3、锁住初始化实例语句的方式](#3_151)
- [4、锁住初始化实例语句之后再次检查实例是否被创建](#4_192)
- [三、饿汉式单例](#_233)



参考文章：[设计模式（一）–深入单例模式（涉及线程安全问题）](https://blog.csdn.net/Jack__Frost/article/details/66475741)。代码全，解释清楚。

本篇博客编译时，需要使用C++11标准。


# 一、单例模式概念以及优缺点

（1）定义：

要求一个类只能生成一个对象，所有对象对它的依赖相同。

（2）优点：
- 只有一个实例，减少内存开支。应用在一个经常被访问的对象上。
- 减少系统的性能开销，应用启动时，直接产生一单例对象，用永久驻留内存的方式。
- 避免对资源的多重占用。
- 可在系统设置全局的访问点，优化和共享资源访问。

（3）缺点：
- 一般没有接口，扩展困难。原因：接口对单例模式没有任何意义；要求“自行实例化”，并提供单一实例，接口或抽象类不可能被实例化。（当然，单例模式可以实现接口、被继承，但需要根据系统开发环境判断）
- 单例模式对测试是不利的。如果单例模式没完成，是不能进行测试的。
- 单例模式与单一职责原则有冲突。原因：一个类应该只实现一个逻辑，而不关心它是否是单例，是不是要单例取决于环境；单例模式把“要单例”和业务逻辑融合在一个类。

（4）使用场景：
- 要求生成唯一序列化的环境.
- 项目需要的一个共享访问点或共享的数据点.
- 创建一个对象需要消耗资源过多的情况。如：要访问IO和 数据库等资源。
- 需要定义大量的静态常量和静态方法（如工具类）的环境。可以采用单例模式或者直接声明static的方式。

（5）注意事项：
- 类中其他方法，尽量是static。

# 二、懒汉式单例模式

## 1、原始懒汉式单例模式

懒汉式单例就是需要使用这个单例对象的时候才去创建这个单例对象。

```
#include <iostream>
#include <stdexcept>
#include <thread>
#include <mutex>

#ifdef WIN32
#include <windows.h>
#define SLEEP(x) Sleep(x)
#else
#include <unistd.h>
#define SLEEP(x) usleep(x*1000)
#endif

using namespace std;

//懒汉式单例
class Singleton {
private:
    static Singleton *singleton;
    Singleton() = default;
    Singleton(const Singleton& s) = default;
    Singleton& operator=(const Singleton& s) = default;
public:
    static Singleton* getInstance(){
        if (Singleton::singleton == nullptr){
            SLEEP(10);//休眠，模拟创建实例的时间
            singleton = new Singleton();
        }
        return singleton;
    }
};

// 必须在类外初始化
Singleton* Singleton::singleton = nullptr;
// 定义一个互斥锁
mutex m;

void print_address(){
    // 获取实例
    Singleton* singleton1 = Singleton::getInstance();
    // 打印singleton1地址
    m.lock(); // 锁住，保证只有一个线程在打印地址
    cout<<singleton1<<endl;
    m.unlock();// 解锁
}

int main(){
    thread threads[10];

    // 创建10个线程
    for (auto&t : threads)
        t = thread(print_address);
    // 对每个线程调用join，主线程等待子线程完成运行
    for (auto&t : threads)
        t.join();
}
```

运行结果：

```bash
0x7ff280000b20
0x7ff298000b20
0x7ff290000b20
0x7ff288000b20
0x7ff278000b20
0x7ff260000b20
0x7ff268000b20
0x7ff264000b20
0x7ff258000b20
0x7ff270000b20
```

**可以看出，结果里面有好几个不同地址的实例！** 所以，这种单例模式不是线程安全的。原因是，当几个线程同时执行到语句`if (Singleton::singleton == nullptr)`时，singleton都还没有被创建，所以就重复创建了几个实例。

## 2、线程安全的单例模式

为了编写线程安全的单例模式，可以锁住getInstance函数，保证同时只有一个线程访问getInstance函数(为了节省篇幅，相同的部分不在代码中再次给出)。

```
using namespace std;

//线程安全的懒汉式单例
mutex m1;
class Singleton {
private:
    static Singleton *singleton;
    Singleton() = default;
    Singleton(const Singleton& s) = default;
    Singleton& operator=(const Singleton& s) = default;
public:
    static Singleton* getInstance() {
        m1.lock(); // 加锁，保证只有一个线程在访问下面的语句
        if (Singleton::singleton == nullptr){
            SLEEP(10); //休眠，模拟创建实例的时间
            singleton = new Singleton();
        }
        m1.unlock();//解锁
        return singleton;
    }
};
```

运行输出：

```bash
0x7fcb80000b20
0x7fcb80000b20
0x7fcb80000b20
0x7fcb80000b20
0x7fcb80000b20
0x7fcb80000b20
0x7fcb80000b20
0x7fcb80000b20
0x7fcb80000b20
0x7fcb80000b20
```

可以发现，所有线程获取到的实例的地址都相同。整个程序中只有一个Singleton实例。原因是进入`getInstance`函数之后，立马锁住创建实例的语句，保证只有一个线程在访问创建实例的代码。

## 3、锁住初始化实例语句的方式

仅仅对创建实例的语句进行加锁，是否是线程安全的呢？

```
//线程安全的懒汉式单例
mutex m1;
class Singleton {
private:
    static Singleton *singleton;
    Singleton() = default;
    Singleton(const Singleton& s) = default;
    Singleton& operator=(const Singleton& s) = default;
public:
    static Singleton* getInstance() {
        if (Singleton::singleton == nullptr){
            SLEEP(100); //休眠，模拟创建实例的时间
            m1.lock();  // 加锁，保证只有一个线程在创建实例
            singleton = new Singleton();
            m1.unlock();//解锁
        }
        return singleton;
    }
};
```

运行输出：

```bash
0x7ff7f0000b20
0x7ff7e8000b20
0x7ff7f0000f50
0x7ff7ec000b20
0x7ff7e0000b20
0x7ff7ec000b40
0x7ff7f0000f70
0x7ff7e0000b40
0x7ff7f0000f90
0x7ff7ec000b60
```

**这种方式不是线程安全的。** 因为当线程同时执行到语句`if (Singleton::singleton == nullptr)`时，singleton都还没有被创建，故会条件为真，多个线程都会创建实例，尽管不是同时创建。

## 4、锁住初始化实例语句之后再次检查实例是否被创建

```
//线程安全的懒汉式单例
mutex m1;
class Singleton {
private:
    static Singleton *singleton;
    Singleton() = default;
    Singleton(const Singleton& s) = default;
    Singleton& operator=(const Singleton& s) = default;
public:
    static Singleton* getInstance() {
        if (Singleton::singleton == nullptr){
            SLEEP(100); //休眠，模拟创建实例的时间
            m1.lock();  // 加锁，保证只有一个线程在访问线程内的代码
            if (Singleton::singleton == nullptr) { //再次检查
                singleton = new Singleton();
            }
            m1.unlock();//解锁
        }
        return singleton;
    }
};
```

运行输出：

```bash
0x7f0bc4000b20
0x7f0bc4000b20
0x7f0bc4000b20
0x7f0bc4000b20
0x7f0bc4000b20
0x7f0bc4000b20
0x7f0bc4000b20
0x7f0bc4000b20
0x7f0bc4000b20
0x7f0bc4000b20
```

可以看出，这种方式是线程安全的。并且没有第2种代码简洁。

# 三、饿汉式单例

先实例化该单例类，而不是像之前一样初始化为空指针。

```
using namespace std;

//线程安全的饿汉式单例
class Singleton {
private:
    static Singleton *singleton;
    Singleton() try{
        // 构造本单利模式的代码
    }catch (exception& e){
        cout << e.what() << endl;
        // 在这里处理可能的异常情况
        throw;
    }

    Singleton(const Singleton& s) = default;
    Singleton& operator=(const Singleton& s) = default;
public:
    static Singleton* getInstance() {
        return singleton;
    }
};

// 必须在类外初始化
Singleton* Singleton::singleton = new Singleton();

// 定义一个互斥锁
mutex m;
```

运行输出：

```bash
0x56362e1fae70
0x56362e1fae70
0x56362e1fae70
0x56362e1fae70
0x56362e1fae70
0x56362e1fae70
0x56362e1fae70
0x56362e1fae70
0x56362e1fae70
0x56362e1fae70
```

可以看出singleton的实例确实只有一个。饿汉式单利会在程序开始之前就被创建，所以是线程安全的。由于创建的单例是在全局变量区，所以需要处理构造函数中可能出现的异常：

```
Singleton() try{
        // 构造本单利模式的代码
    }catch (exception& e){
        cout << e.what() << endl;
        // 在这里处理可能的异常情况
        throw;
    }
```

这里涉及到的知识是：[很少有人知道的c++中的try块函数](https://www.cnblogs.com/laodageblog/p/3615982.html)。

至于参考博客中提到的序列化与反序列化问题，不在本博客中做讨论。



