# C++虚表，你搞懂了吗？ - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [iosxxoo](http://www.jobbole.com/members/iosxxoo) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 前言
我们说的虚表其实有很多种叫法：
- virtual method table（VMT）
- virtual function table(vftable)
- virtual call table
- dispatch table
- vtable
这些都是虚表的意思。虚表是一种利用程序语言实现的dynamic dispatch机制，或者说runtime method binding机制，也就是我们说的多态。
注：笔者在本文使用C++语言，并且统一用**vTable**来表示虚表。
## 虚函数
用virtual关键字修饰的函数就叫**虚函数**。
因为vTable(虚表)是C++利用runtime来实现多态的工具，所以我们需要借助**virtual关键字**将函数代码地址存入vTable来躲开静态编译期。这里我们先不深入探究，后面我会细说。
首先我们先来看一个没有虚函数，即没有用到vTable的例子：


```
#include <iostream>
#include <ctime>
using std::cout;
using std::endl;
struct Animal { void makeSound() { cout << "动物叫了" << endl; } };
struct Cow : public Animal { void makeSound() { cout << "牛叫了" << endl; } };
struct Pig : public Animal { void makeSound() { cout << "猪叫了" << endl; } };
struct Donkey : public Animal { void makeSound() { cout << "驴叫了" << endl; } };
int main(int argc, const char * argv[])
{
    srand((unsigned)time(0));
    int count = 4;
    while (count --) {
        Animal *animal = nullptr;
        switch (rand() % 3) {
            case 0:
                animal = new Cow;
                break;
            case 1:
                animal = new Pig;
                break;
            case 2:
                animal = new Donkey;
                break;
        }
        animal->makeSound();
        delete animal;
    }
    return 0;
}
```
程序中有一个`基类Animal`，它有一个`makeSound()函数`。有三个继承自Animal的子类，分别是`牛、猪、驴`，并且实现了自己的`makeSound()方法`。很简单的代码，是吧。
我们运行程序，你觉得输出结果会是什么呢？不错，这里会连续执行4次Animal的makeSound()方法，结果如下：
![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-30-14672741368548.jpg)
为什么？因为我们的基类Animal的makeSound()方法**没有使用Virtual**修饰，所以在静态编译时就makeSound()的实现就定死了。调用makeSound()方法时，编译器发现这是Animal指针，就会直接jump到makeSound()的代码段地址进行调用。
ok，那么我们把Animal的makeSound()改为虚函数，如下：


```
struct Animal { 
    virtual void makeSound() 
        { 
           cout << "动物叫了" << endl; 
        } 
};
```
运行会是怎样？如你所料，多态已经成功实现： ![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-30-14672741881975.jpg)
接下来就是大家最关心的部分，这是怎么回事？编译器到底做了什么？
## 虚表
为了说明方便，我们需要修改一下**基类Animal**的代码，不改变其他子类，修改如下：


```
struct Animal {
    virtual void makeSound() { cout << "动物叫了" << endl; }
    virtual void walk() {}
    void sleep() {}
};
struct Cow : public Animal { void makeSound() { cout << "牛叫了" << endl; } };
struct Pig : public Animal { void makeSound() { cout << "猪叫了" << endl; } };
struct Donkey : public Animal { void makeSound() { cout << "驴叫了" << endl; } };
```
首先我们需要知道几个关键点：
- 函数只要有virtual，我们就需要把它添加进vTable。
- 每个类(而不是类实例)都有自己的虚表，因此vTable就变成了vTables。
- 虚表存放的位置一般存放在模块的常量段中，从始至终都只有一份。[详情可在此参考](http://blog.csdn.net/houdy/article/details/1496161)
我们怎么理解？从本例来看，我们的Animal、Cow、Pig、Donkey类都有自己的虚表，并且虚表里都有两个地址指针指向makeSound()和walk()的函数地址。一个指针4个字节，因此每个vTable的大小都是8个字节。如图：
![vtable](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-30-vtable.png)
他们的虚表中记录着不同函数的地址值。可以看到Cow、Pig、Donkey**重写了makeSound()函数**但是没有重写walk()函数。因此在调用makeSound()时，就会直接jump到自己实现的code Address。而调用walk()时，则会jump到Animal父类walk的Code Address。
## 虚指针
现在我们已经知道虚表的数据结构了，那么我们在堆里实例化类对象时是怎么样调用到相应的函数的呢？这就要借助到虚指针了(vPointer)。
虚指针是类实例对象指向虚表的指针，存在于对象头部,大小为4个字节，比如我们的Donkey类的实例化对象数据结构就如下： ![](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-30-14672754961225.jpg)
我们修改main函数里的代码，如下：


```
int main(int argc, const char * argv[])
{
    int count = 2;
    while (count --) {
        Animal *animal = new Donkey;
        animal->makeSound();
        delete animal;
    }
    return 0;
}
```
我们在堆中生成了两个Donkey实例，运行结果如下：


```
驴叫了
驴叫了
Program ended with exit code: 0
```
没问题。然后我们再来看看堆里的结构，就变成了这样：
![donkey](http://7xtb7p.com1.z0.glb.clouddn.com/2016-06-30-donkey.png)
还有什么是这张图不能说明的呢？
Enjoy it :)
### 参考链接：
- [C++ vTable Preview](https://www.youtube.com/watch?v=aWDBwLzWrVM)
- [C++ vPointers and vTables](https://www.youtube.com/watch?v=Eaz0P_gJ9FE)
- [Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)
- [虚函数表放在哪里](http://blog.csdn.net/houdy/article/details/1496161)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/06/580ef09bfa7cd184dd604f79237a33ba.jpg)![](http://jbcdn2.b0.upaiyun.com/2016/06/2542128601c04ea21a4aa6fa8553811a.jpg)
