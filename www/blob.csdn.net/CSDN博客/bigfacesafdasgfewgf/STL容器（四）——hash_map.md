# STL容器（四）——hash_map - bigfacesafdasgfewgf - CSDN博客





2014年12月16日 09:13:07[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：922标签：[hash_map																[STL																[map																[容器																[查找](https://so.csdn.net/so/search/s.do?q=查找&t=blog)
个人分类：[Data Structure](https://blog.csdn.net/puqutogether/article/category/2595131)





**STL容器（四）——hash_map**



# 1. hash_map简介




    hash_map和前面介绍的map既有很多的联系，也要必须要了解的区别~

    hash_map的用法和map是一样的，提供了insert，size，count等操作，并且里面的元素也是以pair类型来存贮的。虽然对外部提供的函数和数据类型是一致的，但是其底层实现是完全不同的，map底层的数据结构是红黑树rb_tree；而hash_map却是哈希表hash_table来实现的。




    我们可以通过一段小程序来了解hash_map的用途：（详解看注释！）






```cpp
#include<iostream>
#include<ext/hash_map>  //注意！！这里一定要这样写，这个是hash_map的加载问题
#include<string>
using namespace std;
using namespace __gnu_cxx;  //加载hash_map的需要

int main()
{
	hash_map<int, string> stuInfo;
	hash_map<int, string>::iterator iter;

	//赋值功能
	stuInfo[5]="Jack";  //直接赋值
	stuInfo[3]="Eric";
	stuInfo.insert(pair<int, string> (4, "jason"));  //insert赋值，注意pair类型
	stuInfo.insert(hash_map<int, string>::value_type (2, "Amy"));

	for(iter=stuInfo.begin(); iter!=stuInfo.end(); iter++)  //输出
		cout<<iter->first<<' '<<iter->second<<endl;

    //查找功能
    iter=stuInfo.find(4);
    if(iter!=stuInfo.end())  //如果找到了，返回位置指针
        cout<<"Found! "<<iter->first<<' '<<iter->second<<endl;
    iter=stuInfo.find(1);
    if(iter==stuInfo.end())  //如果找不到，返回.end()指针
        cout<<"Not Found!"<<endl;

    //计数功能
    cout<<stuInfo.count(1)<<endl;  //没有这个key，计数为零
    cout<<stuInfo.count(2)<<endl;  //有了，只可能为1，后面继续赋值也没用
    stuInfo.insert(pair<int, string> (2, "Amy"));
    cout<<stuInfo.count(2)<<endl;

	return 0;
}
```



程序结果：

![](https://img-blog.csdn.net/20141216094303713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    hash_map是基于hash table（哈希表）。 



哈希表最大的优点：就是把数据的存储和查找消耗的时间大大降低，几乎可以看成是常数时间；而代价仅仅是消耗比较多的内存。然而在当前可利用内存越来越多的情况下，用空间换时间的做法是值得的。另外，编码比较容易也是它的特点之一。


基本原理：使用一个下标范围比较大的数组来存储元素。可以设计一个函数（哈希函数，也叫做散列函数），使得每个元素的关键字都与一个函数值（即数组下标，hash值）相对应，于是用这个数组单元来存储这个元素；也可以简单的理解为，按照关键字为每一个元素“分类”，然后将这个元素存储在相应“类”所对应的地方，称为桶。

但是，不能够保证每个元素的关键字与函数值是一一对应的，因此极有可能出现对于不同的元素，却计算出了相同的函数值，这样就产生了“冲突”，换句话说，就是把不同的元素分在了相同的“类”之中。 总的来说，“直接定址”与“解决冲突”是哈希表的两大特点。

hash_map，首先分配一大片内存，形成许多桶。是利用hash函数，对key进行映射到不同区域（桶）进行保存。




其插入过程是：


> 
得到key

通过hash函数得到hash值

得到桶号(一般都为hash值对桶数求模)

存放key和value在桶内。


其取值过程是:


> 
得到key

通过hash函数得到hash值

得到桶号(一般都为hash值对桶数求模)

比较桶的内部元素是否与key相等，若都不相等，则没有找到。

取出相等的记录的value。





    hash_map中直接地址用hash函数生成，解决冲突，用比较函数解决。这里可以看出，如果每个桶内部只有一个元素，那么查找的时候只有一次比较。当许多桶内没有值时，许多查询就会更快了(指查不到的时候).

由此可见，要实现哈希表, 和用户相关的是：hash函数和比较函数。这两个参数刚好是我们在使用hash_map时需要指定的参数。





# 2. 深入理解hash_map的源码




    如果是深入看我们前面一篇《STL容器（三）——对map的排序》的小伙伴们就会问了，map在定义的时候会有缺省，比如compare函数，那么hash_map是不是也有呢？答案是必须的啊~

    其实，hash_map里面缺省的不仅仅是compare函数，因为hash_map是基于hash table的，所有它还缺省了hash函数的类型。STL 中hash_map的声明形式如下：



```cpp
template <class _Key, class _Tp, class _HashFcn = hash<_Key>,
class _EqualKey = equal_to<_Key>,
class _Alloc = __STL_DEFAULT_ALLOCATOR(_Tp) >
class hash_map
{
...
}
```



**   先说一下hash函数的设置：**




hash< int>到底是什么样子？看看源码:

struct hash<int> {

size_t operator()(int __x) const { return __x; }

};

原来是个函数对象。在SGI STL中，提供了以下hash函数：


> 
struct hash<char*>

struct hash<const char*>

struct hash<char> 

struct hash<unsigned char> 

struct hash<signed char>

struct hash<short>

struct hash<unsigned short> 

struct hash<int> 

struct hash<unsigned int>

struct hash<long> 

struct hash<unsigned long> 


也就是说，如果你的key使用的是以上类型中的一种，你都可以使用缺省的hash函数。当然你自己也可以定义自己的hash函数。对于自定义变量，你只能如此，例如对于string，就必须自定义hash函数。





**    现在我们来看比较函数：**




    在map中的比较函数，需要提供less函数。如果没有提供，缺省的也是less< Key> 。在hash_map中，要比较桶内的数据和key是否相等，因此需要的是是否等于的函数:equal_to< Key> 。





//看看equal_to的定义：

template <class _Tp>

struct equal_to : public binary_function<_Tp,_Tp,bool>

{

bool operator()(const _Tp& __x, const _Tp& __y) const { return __x == __y; }

};



如果你使用一个自定义的数据类型，如struct mystruct, 或者const char* 的字符串，如何使用比较函数？使用比较函数，有两种方法. 第一种是：重载==操作符，利用equal_to;看看下面的例子：



struct mystruct{

int iID;

int len;

bool operator==(const mystruct & my) const{

return (iID==my.iID) && (len==my.len) ;

}

}; 



这样，就可以使用equal_to< mystruct>作为比较函数了。另一种方法就是使用函数对象。自定义一个比较函数体：

struct compare_str{

bool operator()(const char* p1, const char*p2) const{

return strcmp(p1,p2)==0;

}

}; 



有了compare_str，就可以使用hash_map了。





# 3. hash_map和map的区别

**构造函数：**hash_map需要hash函数，等于函数；map只需要比较函数(小于函数).

**存储结构：**hash_map采用hash表存储，map一般采用红黑树(RB Tree)实现。因此其memory数据结构是不一样的。





    那么合适使用hash_map，有何时使用map呢？




**具体情况具体选择！**




如果你favor时间效率，考虑hash_map；

如果你favor内存，考虑map；




但是，也要注意：hash_map 查找速度会比map快，而且查找速度基本和数据数据量大小，属于常数级别；而map的查找速度是log(n)级别。并不一定常数就比log(n)小，hash还有hash函数的耗时，而且hash的构造速度比较慢。




http://blog.chinaunix.net/uid-52437-id-2108567.html





http://blog.csdn.net/cws1214/article/details/9842679](https://so.csdn.net/so/search/s.do?q=容器&t=blog)](https://so.csdn.net/so/search/s.do?q=map&t=blog)](https://so.csdn.net/so/search/s.do?q=STL&t=blog)](https://so.csdn.net/so/search/s.do?q=hash_map&t=blog)




