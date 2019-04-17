# C++ 容器详解 - 安得广厦千万间的博客 - CSDN博客





2019年03月13日 14:55:10[独-](https://me.csdn.net/qq_41713256)阅读数：31








vector、set、multiset、map和multimap

一：vector

在使用它时, 需要包含头文件 **#include<vector>**

vector 容器与数组相比其优点在于它能够根据需要随时自动调整自身的大小以便容下所要放入的元素。此外, vector 也提供了许多的方法来对自身进行操作

**二、向量的声明及初始化**

    vector 型变量的声明以及初始化的形式也有许多, 常用的有以下几种形式



```
vector<int> a ;                                //声明一个int型向量a
        vector<int> a(10) ;                            //声明一个初始大小为10的向量
        vector<int> a(10, 1) ;                         //声明一个初始大小为10且初始值都为1的向量
        vector<int> b(a) ;                             //声明并用向量a初始化向量b
        vector<int> b(a.begin(), a.begin()+3) ;        //将a向量中从第0个到第2个(共3个)作为向量b的初始值
```



```
int n[] = {1, 2, 3, 4, 5} ;
        vector<int> a(n, n+5) ;              //将数组n的前5个元素作为向量a的初值
        vector<int> a(&n[1], &n[4]) ;        //将n[1] - n[4]范围内的元素作为向量a的初值
        
三、元素的输入及访问
    元素的输入和访问可以像操作普通的数组那样, 用cin>>进行输入, cout<<a[n]这样进行输出:
    示例:
```





```
#include<iostream>
      #include<vector>
  
      using namespace std ;
  
      int main()
      {
          vector<int> a(10, 0) ;      //大小为10初值为0的向量a
  
         //对其中部分元素进行输入
         cin >>a[2] ;
         cin >>a[5] ;
         cin >>a[6] ;
 
         //全部输出
         int i ;
         for(i=0; i<a.size(); i++)
             cout<<a[i]<<" " ;
 
         return 0 ;
     }
```





```
//全部输出
    vector<int>::iterator t ;
    for(t=a.begin(); t!=a.end(); t++)
        cout<<*t<<" " ;

四、向量的基本操作
```

```
a.size()                 //获取向量中的元素个数
a.empty()                //判断向量是否为空
a.clear()                //清空向量中的元素
a = b ;                 //将b向量复制到a向量中
b.swap(a) ;            //a向量与b向量进行交换

比较
    保持 ==、!=、>、>=、<、<= 的惯有含义 ;
    如: a == b ;    //a向量与b向量比较, 相等则返回1


插入 - insert
     a.insert(a.begin(), 1000);            //将1000插入到向量a的起始位置前
        
     a.insert(a.begin(), 3, 1000) ;        //将1000分别插入到向量元素位置的0-2处(共3个元素)
        
     b.insert(b.begin(), a.begin(), a.end()) ;    //将a.begin(), a.end()之间的全部元素插入 
                                                    到b.begin()前
删除 - erase
     b.erase(b.begin()) ;                     //将起始位置的元素删除
     b.erase(b.begin(), b.begin()+3) ;        //将(b.begin(), b.begin()+3)之间的元素删除
```
` `
1. 在C++中的详细说明


c.push_back(elem)  // 在尾部加入一个数据。

c.pop_back()       // 删除最后一个数据。

c.rbegin()         // 传回一个逆向队列的第一个数据。

c.rend()           // 传回一个逆向队列的最后一个数据的下一个位置。

c.size()           // 返回容器中实际数据的个数。

c.begin()     // 传回迭代器中的第一个数据地址。

c.clear()     // 移除容器中所有数据。

c.empty()     // 判断容器是否为空。

c.end()       // 指向迭代器中末端元素的下一个，指向一个不存在元素。

c.erase(pos)  // 删除pos位置的数据，传回下一个数据的位置。

c.erase(beg,end)  //删除[beg,end)区间的数据，传回下一个数据的位置。


c.insert(pos,elem)    // 在pos位置插入一个elem拷贝，传回新数据位置。

c.insert(pos,n,elem)  // 在pos位置插入n个elem数据。无返回值。

c.insert(pos,beg,end) // 在pos位置插入在[beg,end)区间的数据。无返回值。


c1.swap(c2)

swap(c1,c2)        // 将c1和c2元素互换。同上操作。


二：set和multiset

set的特性是，所有元素都会根据元素的键值自动排序，set不允许两个元素有相同的键值。

**set的各成员函数列表如下:**

1. begin()--返回指向第一个元素的迭代器

2. clear()--清除所有元素

3. count()--返回某个值元素的个数

4. empty()--如果集合为空，返回true

5. end()--返回指向最后一个元素的迭代器

7. erase()--删除集合中的元素

8. find()--返回一个指向被查找到元素的迭代器

10. insert()--在集合中插入元素

11. lower_bound()--返回指向大于（或等于）某值的第一个元素的迭代器

18. upper_bound()--返回大于某个值元素的迭代器

14. rbegin()--返回指向集合中最后一个元素的反向迭代器

15. rend()--返回指向集合中第一个元素的反向迭代器

16. size()--集合中元素的数目

17. swap()--交换两个集合变量

```
#include<set>
#include<iostream>
using namespace std;
int main()
{
    int i;
    int arr[5] = {0,1,2,3,4};
    set<int> iset(arr,arr+5);

    iset.insert(5);
    cout<<"size:"<<iset.size()<<endl;
    cout<<"3 count = "<<iset.count(3)<<endl;
    iset.erase(1);

    set<int>::iterator ite1 = iset.begin();
    set<int>::iterator ite2 = iset.end();
    for(;ite1!=ite2;ite1++)
    {
        cout<<*ite1;
    }
    cout<<endl;

    ite1 = iset.find(3);
    if(ite1!=iset.end())
        cout<<"3 found"<<endl;

    ite1 = iset.find(1);
    if(ite1!=iset.end())
        cout<<"1 not found"<<endl;
}
```



multiset 和set最大的区别就是，它可以插入重复的元素，

如果删除的话，相同的也一起删除了；

如果 erase(  )的参数类型是一个迭代器，那么即使multiset里面又多个相同的值的时候也只会删除一个

否则就会全部删除。

举个例子：

```
multiset<int>s;
s.insert(1)；
s.insert(1);

s.erase( s.find(1) ); // 只会删除一个1
 
s.erase( 1 );    // 所有的1 都会删去
```

如果查找的话，返回该元素的迭代器的位置，若有相同，返回第一个元素的地址；

其他使用和set基本类似。

三：map和multimap

Map是STL的一个关联容器，它提供一对一（其中第一个可以称为关键字，每个关键字只能在map中出现一次，第二个可能称为该关键字的值）的数据；map是根据关键字升序的。

map有三种插入数据的方法

```
map<int, string> mapStudent;

mapStudent.insert(pair<int, string>(1, "student_one")); 
mapStudent.insert(map<int, string>::value_type (1, "student_one"));  
mapStudent[1] = "student_one";
```

前两种是没有区别的，但是最后一种就不一样了

mapStudent.insert(map<int, string>::value_type (1, "student_one"));

mapStudent.insert(map<int, string>::value_type (1, "student_two"));

上面这两条语句执行后，map中1这个关键字对应的值是“student_one”，第二条语句并没有生效

但是如果是用第三种的话：

mapStudent[1] = "student_one";       mapStudent[1] = "student_two";

map中1的这个关键字会被覆盖。

map 的遍历

迭代器形式

```
map<int, string> mapStudent;  
  
    mapStudent.insert(pair<int, string>(1, "student_one"));  
  
    mapStudent.insert(pair<int, string>(2, "student_two"));  
  
    mapStudent.insert(pair<int, string>(3, "student_three"));  
  
    map<int, string>::reverse_iterator iter;  
  
    for(iter = mapStudent.rbegin(); iter != mapStudent.rend(); iter++)  
  
        cout<<iter->first<<"  "<<iter->second<<endl;
```

数组形式

```
map<int, string> mapStudent;  
  
    mapStudent.insert(pair<int, string>(1, "student_one"));  
  
    mapStudent.insert(pair<int, string>(2, "student_two"));  
  
    mapStudent.insert(pair<int, string>(3, "student_three"));  
  
    cout<<mapStudent[1]<<endl;
```



要判定一个数据（关键字）是否在map中出现的方法比较多，这里标题虽然是数据的查找，在这里将穿插着大量的map基本用法。

这里给出三种数据查找方法



第一种：用count函数来判定关键字是否出现，其缺点是无法定位数据出现位置,由于map的特性，一对一的映射关系，就决定了count函数的返回值只有两个，要么是0，要么是1，出现的情况，当然是返回1了



第二种：用find函数来定位数据出现位置，它返回的一个迭代器，当数据出现时，它返回数据所在位置的迭代器，如果map中没有要查找的数据，它返回的迭代器等于end函数返回的迭代器。

**查找map中是否包含某个关键字条目用find**()**方法，传入的参数是要查找的key，在这里需要提到的是begin**()**和end**()两个成员，

分别代表map对象中第一个条目和最后一个条目，这两个数据的类型是iterator.

```
#include <map>  
#include <string>  
#include <iostream>    
using namespace std;    
int main()  
  
{  
  
    map<int, string> mapStudent;    
    mapStudent.insert(pair<int, string>(1, "student_one"));   
    mapStudent.insert(pair<int, string>(2, "student_two"));    
    mapStudent.insert(pair<int, string>(3, "student_three"));    
    map<int, string>::iterator iter;    
    iter = mapStudent.find(1);    
    if(iter != mapStudent.end())    
       cout<<"Find, the value is "<<iter->second<<endl;    
    else    
       cout<<"Do not Find"<<endl;      
    return 0;  
}
```



第三种：这个方法用来判定数据是否出现，是显得笨了点，但是，我打算在这里讲解



lower_bound函数用法，这个函数用来返回要查找关键字的下界(是一个迭代器)



upper_bound函数用法，这个函数用来返回要查找关键字的上界(是一个迭代器)



例如：map中已经插入了1，2，3，4的话，如果lower_bound(2)的话，返回的2，而upper-bound（2）的话，返回的就是3



Equal_range函数返回一个pair，pair里面第一个变量是Lower_bound返回的迭代器，pair里面第二个迭代器是Upper_bound返回的迭代器，如果这两个迭代器相等的话，则说明map中不出现这个关键字，

程序说明

```
#include <map>    
#include <string>    
#include <iostream>    
using namespace std;    
int main()   
{    
    map<int, string> mapStudent;    
    mapStudent[1] = "student_one";    
    mapStudent[3] = "student_three";    
    mapStudent[5] = "student_five";    
    map<int, string>::iterator iter;    
    iter = mapStudent.lower_bound(1);    
    //返回的是下界1的迭代器    
    cout<<iter->second<<endl;    
    iter = mapStudent.lower_bound(2);   
    //返回的是下界3的迭代器    
    cout<<iter->second<<endl;    
    iter = mapStudent.lower_bound(3);    
    //返回的是下界3的迭代器    
    cout<<iter->second<<endl;    
    iter = mapStudent.upper_bound(2);    
    //返回的是上界3的迭代器    
    cout<<iter->second<<endl;    
    iter = mapStudent.upper_bound(3);    
    //返回的是上界5的迭代器    
    cout<<iter->second<<endl;    
    pair<map<int, string>::iterator, map<int, string>::iterator> mappair;    
    mappair = mapStudent.equal_range(2);    
    if(mappair.first == mappair.second)    
        cout<<"Do not Find"<<endl;    
    else    
        cout<<"Find"<<endl;    
    mappair = mapStudent.equal_range(3);    
    if(mappair.first == mappair.second)    
        cout<<"Do not Find"<<endl;    
    else    
        cout<<"Find"<<endl;    
    return 0;  
}
```



map 删除

erase（iterator it);//通过一个条目对象删除

erase（iterator first，iterator last）//删除一个范围

erase( const Key &key );//通过关键字删除



map和multimap的区别 跟set与multiset区别一样的

multimap 的成员函数 erase() 有 3 个版本：

以待删除兀素的迭代器作为参数，这个函数没有返回值；

以一个键作为参数，它会删除容器中所有含这个键的元素，返回容器中被移除元素的个数；

接受两个迭代器参数，它们指定了容器中的一段元素，这个范围内的所有元素都会被删除，这个函数返回的迭代器指向最后一个被删除元素的后一个位置






