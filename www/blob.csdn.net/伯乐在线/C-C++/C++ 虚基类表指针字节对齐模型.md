# C++ 虚基类表指针字节对齐模型 - 文章 - 伯乐在线
原文出处： [chengonghao](http://blog.csdn.net/chengonghao/article/details/51679743)
关于虚基类表指针/虚函数表指针这些类里面的“隐藏成员”在结构里面是如何进行字节对齐的这个问题困惑了我48个小时。虽然网上也有很多关于虚继承、虚函数表、虚基类等内存布局的一些文章，但是基本上谈的都是大致的布局，什么地方有什么，按照什么顺序来排放等这些泛泛而谈的东西，好像都没有把字节对齐方面考虑进去的文章，或许他们都没有考虑到一些复杂层次的类继承关系，从而字节对齐的问题也没有成为问题，也自然而然地会被忽略掉，或者得出错误的对齐规则结论。
也许是我一开始就走错了路，在错误中越走越远，尝尽了各种曲扭的解决方案来回波折。我原以为我对字节对齐的规则已经很熟悉了，而恰恰是这仅仅几个字节的东西却困惑了我两三天，所以我觉得有必要将这个东西单独写一篇笔记，以记录我的研究成果和各种憋屈的时光。我们的目标是，精确到每一个字节~！
在Windows 里面微软是使用了虚基类表的方式来实现类的虚继承的，简单来讲就是当有虚继承的时候会在类里面加入一个“隐藏成员”虚基类表指针，这个指针就是指向虚基类表的了。我们要讨论的就是这个隐藏成员的字节级别的布局，其他的不在这里讨论。从现在开始我会一步一步地让事情变得更加复杂。
先看下面这片代码。在这里我使用了一个空类K，不要被这个东西所迷惑，我使用这个空类的目的主要是为了让它产生虚基类表指针而又不引入虚基类成员变量，这样我就可以少叙述一些关于虚基类成员排放的东西，而将焦点聚集在引入的那个虚基类表指针之上。这个空类虽然有点特殊，但是在这里它其他的东西和正常的类一样，不要纠结这个。还有，代码我直接指定了对齐参数，是为了不引起混乱。


```
#include"stdafx.h"  
#pragma pack(8)  
class K{};  
   
class B :virtual K{  
public:  
       int b;  
       B() :b(0xbbbbbbbb){}  
};  
   
   
int _tmain(int argc, _TCHAR* argv[])  
{  
       B aa;  
       return 0;  
}
```
我们可以在IDE下打断点，用内存查看窗口轻易地观察到对象bb的内存布局情况，这个大家基本上都应该猜测得到大致的布局，就是在类B的开始插入了一个隐藏成员，虚基类表指针，然后才到B的成员b，因为虚基类没有成员所以其他的就不需要理了，这就是我选空类的原因，少说点话~。在VS2013下我抓到的布局是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/12/5c9b006af3e5ea26ef0a71be1bbe942b.png)
恩，和我们猜测的一样。先是放了虚基类表指针然后才到 B 的成员。看到这里我们视乎可以得出点小结论，就是加入的隐藏成员，虚基类表指针，就是相当于在类里面加入一个正常的指针成员变量一样，即可以把类B看成是这样的等价布局：


```
class B { void *vb_ptr; int b;}
```
这样的话字节对齐等各方面都还算是合理的。
那么我们小改一下看下能推翻这个模型么。我们在B里面加一个成员变量double。变成这样：


```
#include "stdafx.h"  
#pragma pack(8)  
class K{};  
   
class B :virtual K{  
public:  
   int b;  
   double b2;  
   B() :b(0xbbbbbbbb), b2(0){}  
};  
   
int _tmain(int argc, _TCHAR* argv[])  
{  
   B aa;  
   return 0;  
}
```
按照以上的理论，我们新加入的double类型的成员变量b2刚好会紧紧的排列在成员b的后面，因为字节对齐刚好合适。但是不幸的是抓到的实际内存布局是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/12/e6d27388ca818351700e1cd64900795a.png)
你看，原来在vb_ ptr和成员b之间没有填充字节的，现在却有了4个字节的填充，而b2和b之间的4字节填充倒还可以勉强理解得过去。所以之前的推论肯定是不正确的了。按照现在的布局和之前的布局貌似还可以这样牵强地理解，虚基类表指针不和B是一个结构的，B是一个结构，虚基类表指针自己一个结构，那么就等价于这样的布局：


```
class temp{ void* vb_ ptr;  B tempB;};
```
为什么是上面这种布局，不懂的需要看另一篇博客：[C/C++字节对齐](http://blog.csdn.net/chengonghao/article/details/51674166)
简单的解释如下：
> 
在 temp 类里面
1.   vb_ptr 是一个指针，大小为 4 字节；
2.   tempB 是一个结构体，tempB 中又包含 int 类型（占 4 字节）的变量 b ；
3.   以及double 类型（占 8 字节）的变量 b2；
class 或 struct 类型的有效对齐参数是指它的成员中，有效对齐参数最大的那个值，对于 temp 类而言，有效对齐参数是 8 ，也就是 tempB 中的 b2 变量（double类型）占用的字节大小。
按照上述模型的话就可以满足上面的两种情况了，而且好像怎么变化B里面的成员，这个模型都可以很好的等价其对齐规则。这个结论当然也是错误的，一般情况其实也就到这里了，但是我说过要让事情变得复杂。


```
#include"stdafx.h"  
#pragma pack(8)  
class K{};  
   
class A {  
public:  
       int a;  
       A() :a(0xaaaaaaaa){}  
};  
   
class B :virtual K, A{  
public:  
       int b;  
       B() :b(0xbbbbbbbb){}  
};  
  
  
   
int _tmain(int argc, _TCHAR* argv[])  
{  
       A aa;  
       return 0;  
}
```
我们把代码改成如上那样。让B在虚继承的时候还多继承了一个A，而且是实继承A。这个派生类B的对象模型估计很多人也猜得对，按照对象布局规则，先是实基类A，然后是虚基类表指针，然后是成员b。实际上内存布局确实也是这样：
![](http://jbcdn2.b0.upaiyun.com/2016/12/8a6e457ac9e6e9f8852bba677558dce6.png)


```
class temp{ A tempA; void* vb_ ptr;  B tempB;};
```
但是后面，我们会慢慢的进入噩梦。小改一下代码。把类A的成员变量a的类型改成char类型。3个类现在变成这样，指定对齐仍然是8字节对齐，和VC默认的一样。


```
#include "stdafx.h"  
#pragma pack(8)  
class K{};  
   
class A {  
public:  
  char a;   // int->char  
  A() :a(0xaaaaaaaa){}  
};  
   
class B :virtual K, A{  
public:  
  int b;  
  B() :b(0xbbbbbbbb){}  
};  
   
int _tmain(int argc, _TCHAR* argv[])  
{  
  B bb;  
  return 0;  
}
```
在IDE下抓取到的B的对象bb的内存布局是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/12/aac3c4049f3effcd906d9910e9a20d7f.png)
怎么样，虚函数表指针后面居然填充了4个字节，看起来貌似完全没有必要在和B的成员变量b之间填充4个字节，就算是b直接排放在vb_prt之后也是满足整体的对齐规则的。这样看来，虚基类表指针的对齐规则貌似和排在它之前的对象又有点联系。
我们把指定的对齐参数改成4字节对齐。看下这填充的4个字节会消失掉么。理论上它应该消失，因为填充的字节数必须小于指定的对齐参数的。


```
#include"stdafx.h"  
#pragma pack(4) //pack(8)->pack(4)  
class K{};  
   
class A {  
public:  
       char a;  //int->char  
       A() :a(0xaaaaaaaa){}  
};  
   
class B :virtualK, A{  
public:  
       int b;  
       B() :b(0xbbbbbbbb){}  
};  
   
int _tmain(int argc, _TCHAR* argv[])  
{  
       B bb;  
       return 0;  
}
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/4e1ef4b6c4e95de06c22bef45d05ca7f.png)
可以看到，字节的填充完全没有变化，这确实是违背了对齐规则的，这种情况不应该发生~！。这也是苦恼我很久的地方。然而，还有更加苦恼的地方。
把对齐改回8字节对齐，然后再把B的成员b的类型改成double。


```
#include"stdafx.h"  
#pragma pack(8)  
class K{};  
   
class A {  
public:  
       char a;  //int->char  
       A() :a(0xaaaaaaaa){}  
};  
   
class B :virtualK, A{  
public:  
       double b;//int->double  
       B() :b(0xbbbbbbbb){}  
};  
   
int _tmain(int argc, _TCHAR* argv[])  
{  
       B bb;  
       return 0;  
}
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/e5e0fb46ce015f74af5cf16ef899833e.png)
把b的类型由int改成了double之后，虚基类表指针和B的成员b之间居然填充了8个字节。实在是匪夷所思了。我尝试了很多猜测，很多看似马上就对得上号了的模型，但是最终都被自己举各种复杂的例子给推翻了~···当然，我想到的那些对齐模型都是很曲扭的，其实连我自己也不相信会是那样的~···
后来，不知道怎么的灵感就来了。
像虚基类表指针和虚函数表指针这些类里面必要的时候会出现的“隐藏成员变量”它们的对齐规则可以总结为一句话：
> 
隐藏成员的加入不能影响在其后的成员的对齐。
这句话怎么理解呢？要想不影响后面的成员的对齐规则和填充的字节，那么因为隐藏成员而插入的总体字节长度就必须是结构里面的各个成员中有效对齐参数最大的那个的整数倍。这样，后面的成员才可以“无视”隐藏成员的存在进行对齐和填充。隐藏的成员确确实实也属于类的成员。
这个结论我已经测试了很多种情况了，都是可以的，我也相信像这种言简意赅的结论才会是正确的那一个，其实我早该想到，而且微软的编译器这样安排也确实很合情合理。我之前的各种憋屈结论立马变成浮云……
我们先按照这个规则小试牛刀一下。
比如最匪夷所思的这个：


```
#include"stdafx.h"  
#pragma pack(4) //pack(8)->pack(4)  
class K{};  
   
class A {  
public:  
       char a;  //int->char  
       A() :a(0xaaaaaaaa){}  
};  
   
class B :virtualK, A{  
public:  
       int b;  
       B() :b(0xbbbbbbbb){}  
};  
   
int _tmain(int argc, _TCHAR* argv[])  
{  
       B bb;  
       return 0;  
}
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/97136eadd41d6efd79723cad0729fcc9.png)
类A的大小只占一个字节，这已经是它的完整的结构，a和虚基类表指针之间的那3个字节是因为虚基类表指针本身需要4字节对齐而引入的填充字节，在虚基类表指针之后，因为填充的3个字节和vb_ptr本身的4个字节加起来才是7个字节，还不是结构B中的有效对齐参数最大的那个的整数倍，B中成员的最大对齐参数是int b 或者是隐藏成员虚基类表指针本身的4字节对齐，这其实也是类B本身的有效对齐参数。所以还得在vb_ptr后面再补一个字节，这样后面的成员才能相当于无视掉这个隐藏参数带来的对齐影响。而后面那3个框出来的CC，其实是成员b需要4字节对齐而引入的字节填充，所以看起来就像是填充了4个字节而已。其实它并没有违反字节对齐的基本规则。
还有其他几个也是可以一样的推导出来。
虚函数表指针也是一个道理的，只不过虚函数表指针会简单一些，因为在虚函数表前面，要么没有成员，要么肯定就是已经是4字节对齐了，不会出现像虚基类表指针那样的前面补几个字节后面补几个字节这种情况。比如下面那样：


```
#include"stdafx.h"  
#pragma pack(8)  
class A {  
public:  
       char a;  
       virtual void funA(){}  
       A() :a(0xaaaaaaaa){}  
};  
   
   
int _tmain(int argc, _TCHAR* argv[])  
{  
       A aa;  
       return 0;  
}
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/3caaf94db84b72b8583c93dba62f5db2.png)
类A有虚函数，所以会有一个隐藏的成员，虚函数表指针，放在类对象的最开始的地方，因为类A的最大对齐参数就是隐藏成员本身的对齐参数4了，因隐藏成员的加入而引入的总共字节数就自然是它的整数倍了，所以就没必要再隐藏成员后面填充字节了，然后就是成员变量a紧紧跟着在后面，因为A本身也要自身对齐，按照它的成员中最大的那个有效对齐作为自己的自身对齐，也就是隐藏成员虚函数指针，所以A本身要按照4字节对齐，所以在a后面要填充3个字节才满足对齐规则。


```
#include"stdafx.h"  
#pragma pack(8)  
class C {  
public:  
       int c;  
       double c2;  
       virtual void funC(){}  
       C() :c(0xaaaaaaaa), c2(0){}  
};  
   
   
int _tmain(int argc, _TCHAR* argv[])  
{  
       C aa;  
       return 0;  
}
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/0b3c82b390a62474295bc15c77739ce9.png)
C类也是差不多，但是C有一个double成员，它的有效对齐是8字节的，是C中各个成员最大的一个，所以隐藏成员引入的字节要是8的整数倍。所以虚函数指针要至少填充4个字节然后加上它本身的4字节才能满足要求。
