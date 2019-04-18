# C++多重继承要慎用! - 文章 - 伯乐在线
原文出处： [bourneli](http://www.cnblogs.com/bourneli/archive/2011/12/28/2305264.html)
#### 本文目的
前几天在写程序时，发现一个多重继承类，调用virtual函数会出现一个问题，该问题比较**隐晦**（因为不会引起程序core dump等严重的效果，我是很偶然的在单元测试中发现的），不容易定位，但是如果出现，可能对程序逻辑会带来致命的问题。
#### 一个例子


```
#include <iostream>
using namespace std;
 
class Base1{
public:
    virtual void foo1() {};
};
 
class Base2{
public:
    virtual void foo2() {};
};
 
class MI : public Base1, public Base2{
public:
    virtual void foo1 () {cout << "MI::foo1" << endl;}
    virtual void foo2 () {cout << "MI::foo2" << endl;}
};
 
int main(){
    MI oMI;
 
    Base1* pB1 =  &oMI;
    pB1->foo1();
   
    Base2* pB2 = (Base2*)(pB1); // 指针强行转换，没有偏移
    pB2->foo2();
     
    pB2 = dynamic_cast<Base2*>(pB1); // 指针动态转换,dynamic_cast帮你偏移
    pB2->foo2();
 
    return 0;
}
```
你会认为屏幕上会输出什么？是下面的结果吗？
![5](http://jbcdn2.b0.upaiyun.com/2016/12/c2c0af4aaa6229078540b86b868289f3.jpg)
这样认为没有什么不对的，因为C++的多态性保证用父类指针可以正确的找到子类实现，并调用。所以会有上面的输出。
但是，现实却不是这样，下面是真实的输出：
![clip_image002](http://jbcdn2.b0.upaiyun.com/2016/12/d8a7ede0669c16004b635c51c0853a00.jpg)
（以上实现在VC 2005和Linux Gcc 4.1.2效果一致）
#### 为什么
为什么会出现上面的情况呢，上面代码中的注释部分也许解释了，这里再来详细的来分析一下。
首先，C++使用一种称之为vtable（google “vtable” for more details）的东西实现virtual函数多态调用。vtable每个类中都有一个，该类的所有对象公用，由编译器帮你生成，只要有virtual函数的类，均会有vtable。在继承过程中，由于类Base1和类Base2都有vtable，所以类MI继承了两个vtable。简单的分析一下对象oMI内存结构，如下：
![6](http://jbcdn2.b0.upaiyun.com/2016/12/0056ad5bd354bf8dfbaf06adcd667966.jpg)
其实很简单，就两个vtable的指针，0和4代表相对地址，指针地址大小为4。
pB1的值为0（pB1 == 0），所以调用“pB1->foo1()”时，可以正确的找到MI::fool这个函数执行。
但是当使用强行转换，将pB1转给pB2，那么实质上pB2的值也是0（pB2 == 0），当调用“pB2->foo2()”时，无法在第一个vtalbe中找到对应的函数，**但是却不报错，而是选择执行函数****MI::foo1**，不知道为什么会有这种行为，但是这种行为却十分恶心，导致结果无法预期的（最后调用的函数会与函数申明的循序有关），不太会引起注意，使得bug十分隐晦。
*可以设想，当一个有复杂的业务逻辑的程序，而类似这种函数调用和指针强行转换分布在不同的函数或模块中，可想而知，bug**定位十分困难。*
当使用动态转换时，也就是“pB2 = **dynamic_cast**(pB1)”，dynamic_cast函数会根据尖括号中的类型进行指针偏移，所以pB2的值为4（pB2 == 4），这样调用“pB2->foo2()”就会按照期望的方式执行。
#### 结论
上面的现象在单继承中是不会出现的，因为只有一个vtable（子类的virtual函数会自动追加到第一个父类的vtable的结尾）。所以不会出现上面的现象，而多重继承却出现了上面的想象，所以需要注意以下两点:
1. 多重继承需要慎用
2. 类型转换尽量采用c++内置的类型转换函数，而不要强行转换
#### 参考资料
- C++虚函数表解析：[http://blog.csdn.net/haoel/article/details/1948051](http://blog.csdn.net/haoel/article/details/1948051)
- Wiki vtable: [http://en.wikipedia.org/wiki/Virtual_method_table](http://en.wikipedia.org/wiki/Virtual_method_table)
