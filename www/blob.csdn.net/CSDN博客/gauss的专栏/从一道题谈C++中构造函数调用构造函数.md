# 从一道题谈C++中构造函数调用构造函数 - gauss的专栏 - CSDN博客
2013年01月04日 00:16:12[gauss](https://me.csdn.net/mathlmx)阅读数：159
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)

### 题目如下：问下列代码的打印结果为0吗？
#include <stdlib.h>
#include <iostream>
usingnamespace std;
struct CLS
{
int m_i;
    CLS( int i ) : m_i(i){}
    CLS()
    {
        CLS(0);
    }
};
int main()
{
    CLS obj;
    cout << obj.m_i << endl;
    system("PAUSE");
return0;
}
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
inline void*__cdecl operatornew(size_t, void*_P)
{
return (_P); 
} 
可见没有分配新的内存。
正确的方式：
struct CLS
{
int m_i;
    CLS( int i ) : m_i(i){}
    CLS()
    {
new (this)CLS(0);
    }
};
另： 若构造函数调用自身，则会出现无限递归调用，是不允许的
