# C++ map key值为指针 - 在思索中前行！ - CSDN博客





2015年04月13日 21:24:25[_Tham](https://me.csdn.net/txl16211)阅读数：5501









## STL中map的key能否用char *呢？当然可以！



在程序中需要用到一个map，本来是这样写的，

map<string, int> mapStr;

为了追求效率，把string改成了char *，

map<char *, int> mapStr;

结果呢？

可想而知，每次放进去的是指针，

当查找的时候就出问题了，总是找不到。

因为key中存放的是指针，当然找不到了。

需要重载一下操作符，当查找时比较指针对应的字符串就可以了。

修改如下：

struct ptrCmp

{

    bool operator()( const char * s1, const char * s2 ) const

    {

        return strcmp( s1, s2 ) < 0;

    }

};

map<char *, int, ptrCmp> mapStr;

再进行查找就OK了！




===========================




## C++ map中key使用指针的问题
(2012-12-07 08:19:28)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)



|标签：### [map](http://search.sina.com.cn/?c=blog&q=map&by=tag)### [find失败](http://search.sina.com.cn/?c=blog&q=find%CA%A7%B0%DC&by=tag)### [find不到结果](http://search.sina.com.cn/?c=blog&q=find%B2%BB%B5%BD%BD%E1%B9%FB&by=tag)|分类：[调试](http://blog.sina.com.cn/s/articlelist_2757352693_9_1.html)|
|----|----|


       C++实际开发的过程会经常使用到map。map是一个key-value值对，key唯一，可以用find进行快速的查找。其时间复杂度为O(logN)，如果采用for循环进行遍历数据时间复杂度为O(N)。如果map中的数据量比较少时，采用find和for循环遍历的效率基本没有太大的区别，但是在实际的开发过程中，存储在map中的数据往往是大量的，这个时候map采用find方式效率比遍历效率高的多。


   确定采用find方式查找数据后，我们需要考虑存储map的空间复杂度，对于基础数据类型的数据（int char等）这里就不做讨论。本文讨论的是map中存储的数据结构struct情况。

1、如果map中的key为struct此时，需要先对struct进行操作符重载，关于这部分内容可以参考[C++
 重载操作符示例](http://blog.sina.com.cn/s/blog_a459dcf50101e6th.html)

  2、map中的key只能是对象，而不能是指针。（这一点尤为重要）。下面给出三个map定义进行说明：

std::map<NHSymbolkey,
 Stru_NHSymbol>*   pmapNHSymbolInfo1

   std::map<NHSymbolkey, Stru_NHSymbol*>*  pmapNHSymbolInfo2


std::map<NHSymbolkey*,
 Stru_NHSymbol*> * pmapNHSymbolInfo2

   其中，pmapNHSymbolInfo1、pmapNHSymbolInfo2中使用find正常，遍历也正常，pmapNHSymbolInfo3使用find查找不到对应的数据（数据已经存在，find不到，遍历可以找到）

原因：std::map<NHSymbolkey*,
 Stru_NHSymbol*>*  pmapNHSymbolInfo2在find的时候是根据指针进行查找的。而在数据insert时，数据都是new的，每次new出的地址是不一样的，在find数据时，根据地址查找结果就找不到数据。通过遍历是取出地址中内容一一比较，这样能够找到数据。

pmapNHSymbolInfo1、pmapNHSymbolInfo2两种方式都可以使用find方式查找数据，但是pmapNHSymbolInfo1中Stru_NHSymbol为对象，这样会使map占用空间比较大，pmapNHSymbolInfo2的Stru_NHSymbol为指针，存储时地址占用空间小，但是每次都是new处理来的，所有一定要记住使用完成后一定要delete，否则会出现内存泄露。




  3、 map插入数据2中方式比较
  

      std::map<NHSymbolkey, Stru_NHSymbol*>*  pmapNHSymbolInfo

      pmapNHSymbolInfo->insert(std::make_pair(pNHSymbolkey, pNHSymbol));该方式的key如果出现重复，则会插入数据失败；

      (*pmapNHSymbolInfo)[objNHSymbolkey] = pNHSymbol;该方式的key如果出现重复则直接覆盖掉原来的数据，永远不会出现插入失败的问题。



结论：C++
 map中key不要使用指针，请直接使用对象。







