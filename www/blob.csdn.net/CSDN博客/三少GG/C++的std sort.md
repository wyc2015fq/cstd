# C++的std::sort - 三少GG - CSDN博客
2012年06月13日 11:08:51[三少GG](https://me.csdn.net/scut1135)阅读数：3197
最近算法作业经常需要排序。偶是一个很懒的人，于是一直用C++的sort进行排序～～～不少同志对此心存疑虑，所以今天就写一写sort的用法。
声明：此用法是从某大牛的程序中看到的，其实偶只是拿来用，不知所以然，飘走~~~~~
MSDN中的定义：
```
template<class RanIt>
    void **sort**(RanIt first, RanIt last); //--> 1）
template<class RanIt, class Pred>
    void **sort**(RanIt first, RanIt last, Pred pr); //--> 2）
```
头文件：
#include <algorithm>
using namespace std;
1.**默认的sort函数是按升序排。对应于1）**
sort(a,a+n); //两个参数分别为待排序数组的首地址和尾地址
2.可以自己写一个cmp函数，按特定意图进行排序。对应于2）
例如：
int cmp( const int &a, const int &b ){
if( a > b )
return 1;
else
return 0;
}
sort(a,a+n,cmp);
是对数组a降序排序
又如：
int cmp( const POINT &a, const POINT &b ){
if( a.x < b.x )
return 1;
else
if( a.x == b.x ){
if( a.y < b.y )
return 1;
else
return 0;
}
else
return 0;
}
sort(a,a+n,cmp);
是先按x升序排序，若x值相等则按y升序排
与此类似的还有C中的qsort，以下同附上qsort的使用方法：
#include <stdlib.h>
格式 qsort(*array_name*,*data_number*,sizeof(*data_type*),*compare_function_name*) (void*)bsearch (*pointer_to_key_word*,*array_name*,*find_number*,
sizeof(*data_type*),*compare_function_name*)
e.g.
int Cmp(const void*a,const void *b)
{
int*pa=(int*)a,*pb=(int*)b;
if(*pa>*pb) return 1;
else if (*pa==*pb) return 0;
else return -1;
}
qsort(data,N,sizeof(int),Cmp); // 对int型数组进行快速排序（非降序排列）
p=(int*)bsearch(&a,data,n,sizeof(int),Cmp);
以上。希望对各位有所帮助。
PS：FJNU OJ上是禁止使用qsort滴~~~~
+++++++++++++++++++++++
1.
《Effective STL》里这些话可能对你也有用处： 
item 31 
　　“我们总结一下你的排序选择： 
　　 ● 如果你需要在vector、string、deque或数组上进行完全排序，你可以使用sort或stable_sort。
　　 ● 如果你有一个vector、string、deque或数组，你只需要排序前n个元素，应该用partial_sort。 
　　 ● 如果你有一个vector、string、deque或数组，你需要鉴别出第n个元素或你需要鉴别出最前的n个元素，而不用知道它们的顺序，nth_element是你应该注意和调用的。
　　 ● 如果你需要把标准序列容器的元素或数组分隔为满足和不满足某个标准，你大概就要找partition或stable_partition。 
　　 ● 如果你的数据是在list中，你可以直接使用partition和stable_partition，你可以使用list的sort来代替sort和stable_sort。如果你需要partial_sort或nth_element提供的效果，你就必须间接完成这个任务，但正如我在上面勾画的，会有很多选择。
　　另外，你可以通过把数据放在标准关联容器中的方法以保持在任何时候东西都有序。你也可能会考虑标准非STL容器priority_queue，它也可以总是保持它的元素有序。”
++++++++++++++++++++++++++
2.
sort()和qsort()
有区别的，应该是sort函数的效率更高吧。
qsort(数组名,数组长度,数组中每个元素大小,compare);     compare函数的写法决定了排序是升序还是降序。需要#include<stdlib.h>
例如：
int compare(const void*a,const void*b){return *(*int)a-*(int*)b;}  
示例：qsort(a,10,sizeof(int),compare)    //假设已定义了整型数组a[10]
升序排序的写法，如果是：return *(*int)b-*(int*)a   就是降序，不论是什么类型的数组，该函数的形式都是这样的
sort(数组名,数组末地址,compare)    //若不写compare则默认升序排列，需要#include<algorithm>
例如：
sort(a,a+10);        //将数组a以升序排序，假设先前定义了a[10]并输入了数据 
其实升序还是降序没有多大关系，无所谓了，在输出语句做很小的改动就ok了！
