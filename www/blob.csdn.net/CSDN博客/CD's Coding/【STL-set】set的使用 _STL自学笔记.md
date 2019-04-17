# 【STL-set】set的使用 _STL自学笔记 - CD's Coding - CSDN博客





2015年07月16日 17:19:25[糖果天王](https://me.csdn.net/okcd00)阅读数：694








## 前言

啊啊啊，我还真是喜欢STL呢，明知道不能过度依赖它，但是就觉得好好用啊好方便啊怎么办……

啃饼干啃STL，今天又啃了一个set。

set是一个很棒的数据结构，度娘说：

“STL 对这个序列可以进行查找，插入删除序列中的任意一个元素，而完成这些操作的时间同这个序列中元素个数的对数成比例关系，并且当[游标](http://baike.baidu.com/view/176618.htm)指向一个已删除的元素时，删除操作无效。而一个经过更正的和更加实际的定义应该是：一个集合(set)是一个容器，它其中所包含的元素的值是唯一的。这在收集一个数据的具体值的时候是有用的。集合中的元素按一定的顺序排列，并被作为集合中的实例。一个集合通过一个[链表](http://baike.baidu.com/view/549479.htm)来组织，在插入操作和删除操作上比向量(vector)快，但查找或添加末尾的元素时会有些慢。具体实现采用了[红黑树](http://baike.baidu.com/view/133754.htm)的平衡二叉树的数据结构。”

然后又有如下解释:


|set是STL中一种标准关联容器（vector,list,string,deque都是序列容器，而set，multiset，map，multimap是标准关联容器），它底层使用平衡的搜索树——红黑树实现，插入删除操作时仅仅需要指针操作节点即可完成，不涉及到内存移动和拷贝，所以效率比较高。set，顾名思义是“集合”的意思，在set中元素都是唯一的，而且默认情况下会对元素自动进行升序排列，支持集合的交(set_intersection),差(set_difference) 并(set_union)，对称差(set_symmetric_difference) 等一些集合上的操作，如果需要集合中的元素允许重复那么可以使用multiset|
|----|

叫人如何不动心？于是来学学看了~

鉴于本人学习STL的方法是先拼命看啊看啊看，然后再敲啊敲啊敲，所以还是从SourceCode来看看比较符合我的胃口——




## 代码理解

### set的初始化

首先是裸set模板，用int的太没诚意了，用个<string,int>型的结构体吧~



```cpp
#include<set>
#include<iostream>
#include<algorithm>
using namespace std;

struct Info  
{  
    string name;  
    double score;  
    bool operator < (const Info &a) const // Custom cmp_Rule 
    {  
        //Desc:<  Asc:>  
        return a.score < score;  
    }  
};  


int main()
{
	set<Info> s;  
    Info info;  

    info.name = "Jack";  
    info.score = 80;  
    s.insert(info);  
    info.name = "Tom";  
    info.score = 99;  
    s.insert(info);  
    info.name = "Steven";  
    info.score = 60;  
    s.insert(info);  
	s.erase(s.find(info));
	
    set<Info>::iterator it;  
    for(it = s.begin(); it != s.end(); it++)  
        cout << (*it).name << " : " << (*it).score << endl;   
	return 0;
}

/* 
运行结果： 
Tom : 99 
Jack : 80 
*/
```
可见set会自动维护其中元素的顺序，可以通过重载操作符的方式自定义排序的规则，



此外，erase删除某个元素时需要传参指向这个被删除的元素的迭代器；寻找是否存在某个元素，如果存在则获取其迭代位置的迭代器则使用find()函数。

说到erase，也可以传入两个参数作为前后指针删除其中的元素，代码如下：



```cpp
// set::lower_bound/upper_bound
#include <iostream>
#include <set>

int main ()
{
  std::set<int> myset;
  std::set<int>::iterator itlow,itup;

  for (int i=1; i<10; i++) myset.insert(i*10); // 10 20 30 40 50 60 70 80 90

  itlow=myset.lower_bound (30);                //       ^
  itup=myset.upper_bound (60);                 //                   ^

  myset.erase(itlow,itup);                     // 10 20 70 80 90

  std::cout << "myset contains:";
  for (std::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```




### 关于等号的操作符重载



```cpp
// assignment operator with sets
#include <iostream>
#include <set>

int main ()
{
  int myints[]={ 12,82,37,64,15 };
  std::set<int> first (myints,myints+5);   // set with 5 ints
  std::set<int> second;                    // empty set

  second = first;                          // now second contains the 5 ints
  first = std::set<int>();                 // and first is empty

  std::cout << "Size of first: " << int (first.size()) << '\n';
  std::cout << "Size of second: " << int (second.size()) << '\n';
  return 0;
}
```


如果只是像上述一样把first和second交换包含元素的话，其实swap也就够了：





```cpp
// swap sets
#include <iostream>
#include <set>

main ()
{
  int myints[]={12,75,10,32,20,25};
  std::set<int> first (myints,myints+3);     // 10,12,75
  std::set<int> second (myints+3,myints+6);  // 20,25,32

  first.swap(second);

  std::cout << "first contains:";
  for (std::set<int>::iterator it=first.begin(); it!=first.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "second contains:";
  for (std::set<int>::iterator it=second.begin(); it!=second.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```




### 黑科技之equal_elements

此外，看到一个特别方便的函数叫做equal_elements()，作用是返回一个pair，pair的first指向lower_bound，pair的second指向upper_bound。



```cpp
// set::equal_elements
#include <iostream>
#include <set>

int main ()
{
  std::set<int> myset;

  for (int i=1; i<=5; i++) myset.insert(i*10);   // myset: 10 20 30 40 50

  std::pair<std::set<int>::const_iterator,std::set<int>::const_iterator> ret;
  ret = myset.equal_range(30);

  std::cout << "the lower bound points to: " << *ret.first << '\n';
  std::cout << "the upper bound points to: " << *ret.second << '\n';

  return 0;
}
```
是不是特别方便呢？~









### 存疑

get_allocator() 的用法及意义


```cpp
// set::get_allocator
#include <iostream>
#include <set>

int main ()
{
  std::set<int> myset;
  int * p;
  unsigned int i;

  // allocate an array of 5 elements using myset's allocator:
  p=myset.get_allocator().allocate(5);

  // assign some values to array
  for (i=0; i<5; i++) p[i]=(i+1)*10;

  std::cout << "The allocated array contains:";
  for (i=0; i<5; i++) std::cout << ' ' << p[i];
  std::cout << '\n';

  myset.get_allocator().deallocate(p,5);

  return 0;
}
```



### 关于集合间逻辑关系操作

四种常用函数标准用法：



```cpp
#include <algorithm>
#include <iostream>
#include <set>
using namespace std;
 
int main()
{
       int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
       set<int> S( a, a + 9 );
      
       int b[] = { 3, 6, 8, 9 };
       set<int> S2( b, b + 4 );
      
       set<int>::iterator site;
 
       set<int> Su;
       set<int> Si;
       set<int> Sd;
       set<int> Ssd;
      
       //交集
       set_intersection( S.begin(), S.end(),
                                   S2.begin(), S2.end(),
                                   inserter( Si, Si.begin() ) );
                                  
       //并集
       set_union( S.begin(), S.end(),
                     S2.begin(), S2.end(),
                        inserter( Su, Su.begin() ) );
                       
       //差集
       set_difference( S.begin(), S.end(),
                                S2.begin(), S2.end(),
                                   inserter( Sd, Sd.begin() ) );
      
       //对称差集
       set_symmetric_difference( S.begin(), S.end(),
                                                S2.begin(), S2.end(),
                                                 inserter( Ssd, Ssd.begin() ) );
                                                
      
       site = Si.begin();
       cout<<"the intersection of S and S2 is : ";
       while( site != Si.end() )
       {
              cout<< *site <<" ";
              ++ site;
       }
       cout<<endl;
      
       site = Su.begin();
       cout<<"the union of S and S2 is : ";
       while( site != Su.end() )
       {
              cout<< *site <<" ";
              ++ site;
       }
       cout<<endl;
      
       site = Sd.begin();
       cout<<"the difference of S and S2 is : ";
       while( site != Sd.end() )
       {
              cout<< *site <<" ";
              ++ site;
       }
       cout<<endl;
      
       site = Ssd.begin();
       cout<<"the symmetric difference of S and S2 is : ";
       while( site != Ssd.end() )
       {
              cout<< *site <<" ";
              ++ site;
       }
       cout<<endl;
      
       return 0;
}
```

输出：

the intersection of S and S2 is : 3 6 8

the union of S and S2 is : 0 1 2 3 4 5 6 7 8 9

the difference of S and S2 is : 0 1 2 4 5 7

the symmetric difference of S and S2 is : 0 1 2 4 5 7 9




详解用法：



```cpp
#include<set>
#include<iterator>
#include<iostream>
#include<algorithm>
using namespace std;

set<int> eg1,eg2;

int main()
{
	//插入
	eg1.insert(1);
	eg1.insert(100);
	eg1.insert(5);
	eg1.insert(1);//元素1因为已经存在所以set中不会再次插入1
	eg1.insert(10);
	eg1.insert(9);
	
	//遍历set，可以发现元素是有序的
	set<int>::iterator set_iter=eg1.begin();
	cout<<"Set named eg1:"<<endl;
	for(;set_iter!=eg1.end();set_iter++) cout<<*set_iter<<" ";	cout<<endl;
	
	//使用size()函数可以获得当前元素个数
	cout<<"Now there are "<<eg1.size()<<" elements in the set eg1"<<endl;
	if(eg1.find(200)==eg1.end())	//find()函数可以查找元素是否存在
	   cout<<"200 isn't in the set eg1"<<endl;
	
	for(int i=6;i<15;i++) eg2.insert(i);
	cout<<"Set named eg2:"<<endl;
	for(set_iter=eg2.begin();set_iter!=eg2.end();set_iter++) cout<<*set_iter<<" ";	cout<<endl;
	
	//获得两个set的并
	set<int> eg3; 	cout<<"Union:";
	set_union(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<set<int> >(eg3,eg3.begin()));//注意第五个参数的形式
	copy(eg3.begin(),eg3.end(),ostream_iterator<int>(cout," "));
	cout<<endl;
	
	//获得两个set的交，注意进行集合操作之前接收结果的set要调用clear()函数清空一下
	eg3.clear();
	set_intersection(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<set<int> >(eg3,eg3.begin()));
	cout<<"Intersection:";
	copy(eg3.begin(),eg3.end(),ostream_iterator<int>(cout," "));
	cout<<endl;
	
	//获得两个set的差
	eg3.clear();
	set_difference(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<set<int> >(eg3,eg3.begin()));
	cout<<"Difference:";
	copy(eg3.begin(),eg3.end(),ostream_iterator<int>(cout," "));
	cout<<endl;
	
	//获得两个set的对称差，也就是假设两个集合分别为A和B那么对称差为AUB-A∩B
	eg3.clear();
	set_symmetric_difference(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<set<int> >(eg3,eg3.begin()));
	copy(eg3.begin(),eg3.end(),ostream_iterator<int>(cout," "));
	cout<<endl;
	
	return 0;
}
```
输出:

Set named eg1:

1 5 9 10 100

Now there are 5 elements in the set eg1

200 isn't in the set eg1

Set named eg2:

6 7 8 9 10 11 12 13 14

Union:1 5 6 7 8 9 10 11 12 13 14 100

Intersection:9 10

Difference:1 5 100

1 5 6 7 8 11 12 13 14 100












