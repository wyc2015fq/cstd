
# 初窥c++11：从move语义到右值引用 - 3-Number - CSDN博客


2017年12月06日 09:49:59[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：86


转载理由：一直听C++11特性，没认真学习过，学习学习；
装载于：http://blog.csdn.net/booirror/article/details/24503461
技术人员往往比较傲慢，写c语言的鄙视c++，写c++的鄙视Java，写Java的鄙视c\#，要是程序员吵起来怎么办，一个流行的办法就是去github约架，比赛写tokenizer和grammar analyzer啊，写不出来滚出程序界啊，哈哈。说到github，不得不说去github找份好代码真不容易，大量“坏味道”的代码充斥其中，大有劣币驱逐良币之势。

网上C++11的文章已经汗牛充栋了，早在06，07年那时就有人写这方面的了。我为什么这么晚才写呢？因为我学C++还不到一年，惭愧。


背景

为什么需要move语义，或者说增加move语义能给c++带来什么？运行效率是主要原因。c++重视运行效率，在不失程序抽象的基础上，想尽办法榨尽CPU的每一滴油水。@陈硕有一篇blog正确地实现了最简单的string类，其中的拷贝构造函数如下:

String(const String& rhs)
: data_(new char[rhs.size() + 1])
{
strcpy(data_, rhs.c_str());
}

这里进行了内存分配和拷贝数据，如果rhs是个临时对象，要是能将rhs的数据“move”到data_岂不是提高了运行效率，想想自己编程时，类似情况经常出现，如果这些临时内存都能利用上，真是令人兴奋的事。要知道，对游戏而言，流畅性是至关重要的。

move语义

这时，move语义出场了，拷贝数据时，有一个const T&版的，也不要忘了move语义版的函数。因此，c++11还有个move拷贝构造函数：
String(String&& rhs)
: data_(rhs.data_)
{
rhs.data_ = nullptr;
}

此时有些语法你看不懂没关系，但是这个版本没有了new和strcpy。观察发现，move语义可分为两个要求：
1.引用传递
2.可以修改该引用变量
好了，要想实现这样的move语义，仅靠之前的c++语法并不好实现。看下面的代码
int foo = 3;
int getFoo() { return foo;}
int main()
{
int& a = getFoo(); // error
const int& b = getFoo();//ok
}

getFoo()返回个临时对象，他是个右值，因此不能赋给int&，否则就可以修改内容了。如果返回一个函数内的临时变量的引用，去操作他也是不安全的，因为函数返回后内存被收回了。一种可行的做法时，发明新的语法来支持move语义，也就是右值引用(rvalue reference)。

右值引用

上面代码String&& rhs就表示一个右值引用， 传统的引用是String&写法。现在，我们可以放心地写下面的代码：
int && a = getFoo();      //ok
const int && b = getFoo();//ok

左值，右值的概念最初是从《c primer plus》里看到的，在这里，我们应该这么理解左值和右值
1.左值应该是像变量样的对象
2.右值则是临时性的对象

正如标题所言，本文仅仅是初窥之作，如有错误，谢谢指正

