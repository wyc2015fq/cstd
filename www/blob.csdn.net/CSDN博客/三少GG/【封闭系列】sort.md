# 【封闭系列】sort - 三少GG - CSDN博客
2011年07月15日 11:04:30[三少GG](https://me.csdn.net/scut1135)阅读数：923
1. sort/////////////////////////*下面是别人的东西*////头文件：#include <algorithm>using namespace std;**1.默认的sort函数是按升序排**。对应于1）sort(a,a+n);   //两个参数分别为待排序数组的首地址和尾地址2.可以自己写一个cmp函数，按特定意图进行排序。对应于2）例如：int cmp( const int &a, const int &b ){    if( a > b )       return 1;    else       return 0;}sort(a,a+n,cmp);是对数组a降序排序又如：int cmp( const POINT &a, const POINT &b ){    if( a.x < b.x )       return 1;    else       if( a.x == b.x ){          if( a.y < b.y )             return 1;          else             return 0;        }       else          return 0;}sort(a,a+n,cmp);是先按x升序排序，若x值相等则按y升序排参考资料：1.[http://it.china-b.com/cxsj/vc/20090612/101895_1.html](http://it.china-b.com/cxsj/vc/20090612/101895_1.html)C++标准24章有一个小节叫“Sorting and related operations”。它包含了很多对已序区间进行的操作，和三个排序用泛型算法：sort()，stable_sort()，和partial_sort()。前两个，sort()和stable_sort()，本质上有相同的接口：同通常的STL泛型算法一样，传入指明了需要排序的区间的Random Access Iterator。 同样，作为可选项，你也能提供第三个参数以指明如何比较元素：第三个参数是一个functor，接受两个参数（x和y），在x应该位于y之前时返回true。所以，举例来说，如果v是一个int的vector：std::sort(v.begin()， v.end());将以升序来排序它。要改为降序，你需要提供应该不同的比较方法：std::sort(v.begin()， v.end()， std::greaterint());2.[http://blog.csdn.net/hqwang4/article/details/5623795](http://blog.csdn.net/hqwang4/article/details/5623795)
sort采用的是成熟的"快速排序算法"(目前大部分STL版本已经不是采用简单的快速排序，而是结合内插排序算法)。可以保证很好的平均性能、复杂度为n*log(n)。
qsort()的速度远远慢于<algorithm>中的sort()。主要原因就是那第4个参数，因为编译器不可能内联通过函数指针传递的函数，所以在qsort()中每次判断都会真正地调用一次比较函数。而在sort()中传递的是“仿函数”，这是一个行为类似函数的对象（通过重载operator()实现），而当“调用”它时，编译器可以轻易内联其代码，于是可以省略大量的函数调用，节约时间。
**注意：不能对char型数据进行排序，如果非要对char型数据进行排序，则需要将char型数据转换为string类型的数据，才能进行排序。**
**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**
**2.  STL vector**
c.end() // 指向迭代器中末端元素的下一个，指向一个不存在元素。   
3.
