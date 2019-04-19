# c++析构函数 - Koma Hub - CSDN博客
2019年03月28日 21:42:25[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：14
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
原文：[https://www.cnblogs.com/puyangsky/p/5319470.html](https://www.cnblogs.com/puyangsky/p/5319470.html)
**Table of Contents**
[一、定义](#%E4%B8%80%E3%80%81%E5%AE%9A%E4%B9%89)
[二、用法](#%E4%BA%8C%E3%80%81%E7%94%A8%E6%B3%95)
[1.普通用法](#1.%E6%99%AE%E9%80%9A%E7%94%A8%E6%B3%95)
[用法二：](#%E7%94%A8%E6%B3%95%E4%BA%8C%EF%BC%9A)
# **一、定义**
1. 作用：对象消亡时，自动被调用，用来释放对象占用的空间
2.特点:
   (1) 名字与类名相同
   (2) 在前面需要加上"~"
   (3) 无参数，无返回值
   (4) 一个类最多只有一个析构函数
   (5) 不显示定义析构函数会调用缺省析构函数
# **二、用法**
## 1.普通用法
**代码：**
```cpp
class Test
{
    int id;
public:
    Test(int i)
    {
        id = i;
    }
    ~Test()
    {
        cout<<"ID: "<<id<<" destruction function is invoked!"<<endl;
    };
};
int main()
{
    Test t0(0);                   //栈中分配   
    Test t1[3]{1,1,1};  　　　　　　//栈中分配，数组型对象
    Test *t2 = new Test(2);       //堆中分配
    delete t2;
    Test *t3 = new Test[3]{3,3,3};//堆中分配
    delete []t3;
    cout<<"------End of Main-------"<<endl;
    return 0;
}
```
**分析：**
在main函数中创建了t0,t1,t2,t3几个对象，这里先说一下**C++创建对象的三种不同方式**：
1、Test p1(1);                               //栈中分配内存
2、Test p2 = Test(2);　　　　　　　 //栈中分配内存，跟方法1相同，是方法1的完整模式
3、Test *p3 = new Test(3);　　　　 //堆中分配内存
方法1、2中都是在栈中分配内存，在栈中内存由系统自动的去分配和释放，而使用new创建的指针对象是在堆中分配内存，当不需要该对象时，需要我们手动的去释放，否则会造成内存泄漏。
在上述程序中，t0和t1都是栈中的对象，在程序结束时由系统来释放，因此出现在“----End of Main”之后。
t2，t3是new出来的堆中对象，所以需要手动的delete释放，因此出现在最前面。
另外有一点发现，就是**栈中对象的释放顺序，是后定义的先释放**，经过几次验证也如此，我想这恰好应征了栈的后进先出的特征。
## 用法二：
代码：
```cpp
class Test
{
    int id;
public:
    Test(int i)
    {
        id = i;
    }
    ~Test()
    {
        cout<<"ID: "<<id<<" destruction function is invoked!"<<endl;
    };
};
Test t0(0);　　                      //最先创建的对象，最后释放
void Func()
{
    static Test t1(1);               //创建静态对象，会在整个程序结束时自动释放
    Test t2(2);　　　　　　　　　　　　 //在Func结束时自动释放
    cout<<"-----Func-----"<<endl;
}
int main()
{
    Test t3(3);
    t3 = 10;                         //类型转换构造函数，这里会创建临时对象，将int型转成Test类型对象，在赋值结束后，临时变量销毁
    cout<<"------Begin of Main-------"<<endl;
    {
        Test t4(4);                  //花括号代表作用域，不需要等到main方法结束就释放了
    }
    Func();                          //进入Func函数
    cout<<"------End of Main-------"<<endl;
    return 0;
}
```
分析过程都在代码2的注释中。
