# 从一道题谈C++中构造函数调用构造函数 - ljx0305的专栏 - CSDN博客
2008年06月01日 21:09:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：451标签：[c++																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
[从一道题谈C++中构造函数调用构造函数](http://blog.csdn.net/fisher_jiang/archive/2008/05/23/2472153.aspx)
题目如下：问下列代码的打印结果为0吗？
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <cstdlib>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <iostream>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)usingnamespace std;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct CLS
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int m_i;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)    CLS( int i ) : m_i(i)...{}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CLS()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        CLS(0);      //这里相当于产生了一个匿名的临时CLS对象，该对象调用带有一个整型参数的构造函数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int main()
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CLS obj;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    cout << obj.m_i << endl;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    system("PAUSE");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
打印结果是不定的，不一定为0
代码奇怪的地方在于构造函数中调用了自己的另一个构造函数
我们知道，当定义一个对象时，会按顺序做2件事情：
1）分配好内存（非静态数据成员是未初始化的）
2）调用构造函数（构造函数的本意就是初始化非静态数据成员）
显然上面代码中，CLS obj；这里已经为obj分配了内存，然后调用默认构造函数，但是默认构造函数还未执行完，却调用了另一个构造函数，这样相当于产生了一个匿名的临时CLS对象，它调用CLS(int)构造函数，将这个匿名临时对象自己的数据成员m_i初始化为0；但是obj的数据成员并没有得到初始化。于是obj的m_i是未初始化的，因此其值也是不确定的
从这里，我们归纳如下：
1）在c++里，由于构造函数允许有默认参数，使得这种构造函数调用构造函数来重用代码的需求大为减少
2）如果仅仅为了一个构造函数重用另一个构造函数的代码，那么完全可以把构造函数中的公共部分抽取出来定义一个成员函数(推荐为private),然后在每个需要这个代码的构造函数中调用该函数即可
3）偶尔我们还是希望在类的构造函数里调用另一个构造函数，可以按下面方式做：
在构造函数里调用另一个构造函数的关键是让第二个构造函数在第一次分配好的内存上执行，而不是分配新的内存，这个可以用标准库的placement new做到：
    先看看标准库中placement new的定义
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)inline void*__cdecl operatornew(size_t, void*_P)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return (_P); 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
可见没有分配新的内存。
正确的方式：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)struct CLS
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int m_i;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)    CLS( int i ) : m_i(i)...{}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CLS()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)new (this)CLS(0);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
另： 若构造函数调用自身，则会出现无限递归调用，是不允许的
转自:http://blog.csdn.net/fisher_jiang/archive/2008/05/23/2472153.aspx
