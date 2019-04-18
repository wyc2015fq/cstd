# STL容器（二）——map - bigfacesafdasgfewgf - CSDN博客





2014年10月13日 19:14:41[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：637








**关于map容器的使用方法**

    在STL一个中有一个很重要的容器：map。map容器提供了一个键值对：key, value. 例如：我们可以把一个字符作为key，其在字符串中出现的次数是value，这样子我们就可以把一个字符串中统计字符出现的问题放在map容器中来解决。

***1. map的基本操作***

    map需要头文件： #include<map>

    定义：map<string, int> myMap;  这里指定了map中键值对的类型，以及生成对象的名称myMap; 



***2. map的插入***


    插入数 ：



```cpp
myMap["a"]   =   1; 
myMap.insert(map<string,   int>::value_type("b",2)); 
myMap.insert(pair<string,int>("c",3)); 
myMap.insert(make_pair<string,int>("d",4));
```

***3. 查找数据：***


我们需要使用find函数，这个是map容器最擅长的一个功能，因为map查找的时间复杂度只有O(log n). find(k)方法简单地返回键值为k的元素的迭代器。如果没有元素的键值为k，则返回map的end()迭代器；如果有，则返回迭代器指针，一般为it。由于map是按照键key递增顺序排列的，故使用map查找的复杂度只有O(logN)。这才是map最主要的优势，也是面试map中最常问到的知识点。

如下面这个例子：



```cpp
#include<iostream>
#include<map>
#include<algorithm>
using namespace std;


int main()
{
//    freopen("test.txt", "r", stdin); 

    int N; 
    while(scanf("%d", &N))
    {
        map<int, int>number; 
        map<int, int>::iterator iter; 

        int m; 
        while(N--)
        {
            scanf("%d", &m); 
            iter=number.find(m);
            if(iter==number.end()) 
                number.insert(map<int, int>::value_type(m, 1)); 
            else
                iter->second++; 
        }

        for(iter=number.begin(); iter!=number.end(); iter++)
        {
            cout<<iter->first<<' '<<iter->second<<endl; 
        }
    }

    return 0; 
}
```

一定要记住find()的返回情况：如果查找到，则返回找到的指针位置；如果没有查找到，返回指针指向end()。还有重要的是，map查找的复杂度为O(logN)。



***4. 删除数据***

在map中不能修改key，如果修改，只能删除。

map中还有一个常用的就是erase()。比如我们需要清楚clientId=10的键值对，那么我们只需要对clients调用erase：clients.erase(clients.find(10)); 这里需要注意的是erase()的输入还是find返回的指针。如果我们还想清除clientId从1到10的键值对，我们可以使用：clients.erase(client.find(1), clients.find(10));
 不过当find()返回的end()，那么这样调用erase()方法则是一个严重的错误，会对map造成破坏操作。




***5. 下标方式***

关于map的插入操作，前面的例子中已经涉及了insert()。其实，map还有一种更为简单的插入操作方法，即使用下标。

例如，我们需要在上面定义的clients的map中插入一个key=110的键值对，那么我们可以使用如下方法：




myMap["a"]=1; 




所以我们可以简单地使用下标的方式来实现插入操作。其实，使用下标的方法来插入有一个比较好玩的过程：首先需要检查key=k是否已经在map里面，如果没有这样的key，则我们就直接插入；如果已经存在这样的key，那么是做覆盖处理的。显然，使用下标来插入就会包含这样的一个判断过程，如果要插入较多的键值对，那么必然会影响程序的效率，故当我们需要批量插入许多个键值对时，我们就使用insert()。但是需要注意的是，如果key已经存在了map中，下标[]方式反而比insert()的效率更加高而且更加美观简洁！所以，我们在对map实现插入操作的时候，会综合两种方式来用，即我们首先判断key是否已经存在于map中了，如果已经存在就用下标[]的方式，如果不存在，则用insert()的方法。

不仅仅可以用于插入操作，我们也可以通过键key检索出value。而且，我们用[]还可以统计信息，例如下面这个例子：

bookMap[book]+=count; 

但是，正如前面说的，下标的方法需要一个先检索再赋值的过程。如果map中已经存在该key，那么这样做可行；如果本来不存在，需要插入新的key，这样的速度会收到影响。



***6. map的大小***



在往map里面插入了数据，我们怎么知道当前已经插入了多少数据呢，可以用size函数，用法如下：

Int nSize = mapStudent.size();


***7. map的排序***

    map中默认是对key排序的，所以如果需要按key的大小排序，直接输出即可；但是，大多数情况，我们需要对value排序。例如：统计字符出现的次数，每个人得票数等等问题，这样问题最后都是需要按照value排序输出的。很可惜，map中没有一个函数可以直接对value实现sort操作。但是，我们利用vector里的sort作用，来间接对map按照value排序。




其实，为了实现快速查找，map内部本身就是按序存储的（比如红黑树）。在我们插入<key, value>键值对时，就会按照key的大小顺序进行存储。这也是作为key的类型必须能够进行<运算比较的原因。现在我们用string类型作为key，因此，我们的存储就是按学生姓名的字典排序储存的。


【参考代码】



```cpp
#include<map>  
#include<string>  
#include<iostream>  
using namespace std;  
  
typedef pair<string, int> PAIR;  
  
ostream& operator<<(ostream& out, const PAIR& p) {  
  return out << p.first << "\t" << p.second;  
}  
  
int main() {  
  map<string, int> name_score_map;  
  name_score_map["LiMin"] = 90;   
  name_score_map["ZiLinMi"] = 79;   
  name_score_map["BoB"] = 92;   
  name_score_map.insert(make_pair("Bing",99));  
  name_score_map.insert(make_pair("Albert",86));  
  for (map<string, int>::iterator iter = name_score_map.begin();  
       iter != name_score_map.end();  
       ++iter) {  
    cout << *iter << endl;  
  }  
  return 0;  
 }
```






【运行结果】

![](https://img-my.csdn.net/uploads/201211/30/1354288047_7951.jpg)






大家都知道map是stl里面的一个模板类，现在我们来看下map的定义：



```cpp
template < class Key, class T, class Compare = less<Key>,  
           class Allocator = allocator<pair<const Key,T> > > class map;
```



它有四个参数，其中我们比较熟悉的有两个: Key 和 Value。第四个是 Allocator，用来定义存储分配模型的，此处我们不作介绍。



现在我们重点看下第三个参数： class Compare = less<Key> 


这也是一个class类型的，而且提供了默认值 less<Key>。 less是stl里面的一个函数对象，那么什么是函数对象呢？


所谓的函数对象：即调用操作符的类，其对象常称为函数对象（function object），它们是行为类似函数的对象。表现出一个函数的特征，就是通过“对象名+(参数列表)”的方式使用一个 类，其实质是对operator()操作符的重载。


现在我们来看一下less的实现：






```cpp
template <class T> struct less : binary_function <T,T,bool> {  
  bool operator() (const T& x, const T& y) const  
    {return x<y;}  
};
```






它是一个带模板的struct，里面仅仅对()运算符进行了重载，实现很简单，但用起来很方便，这就是函数对象的优点所在。stl中还为四则运算等常见运算定义了这样的函数对象，与less相对的还有greater：


```cpp
template <class T> struct greater : binary_function <T,T,bool> {  
  bool operator() (const T& x, const T& y) const  
    {return x>y;}  
};
```

map这里指定less作为其默认比较函数(对象)，所以我们通常如果不自己指定Compare，map中键值对就会按照Key的less顺序进行组织存储，因此我们就看到了上面代码输出结果是按照学生姓名的字典顺序输出的，即string的less序列。





我们可以在定义map的时候，指定它的第三个参数Compare，比如我们把默认的less指定为greater：


【参考代码】









```cpp
#include<map>  
#include<string>  
#include<iostream>  
using namespace std;  
  
typedef pair<string, int> PAIR;  
  
ostream& operator<<(ostream& out, const PAIR& p) {  
  return out << p.first << "\t" << p.second;  
}  
  
int main() {  
  map<string, int, greater<string> > name_score_map;  
  name_score_map["LiMin"] = 90;   
  name_score_map["ZiLinMi"] = 79;   
  name_score_map["BoB"] = 92;   
  name_score_map.insert(make_pair("Bing",99));  
  name_score_map.insert(make_pair("Albert",86));  
  for (map<string, int>::iterator iter = name_score_map.begin();  
       iter != name_score_map.end();  
       ++iter) {  
    cout << *iter << endl;  
  }  
  return 0;  
}
```





【运行结果】

![](https://img-my.csdn.net/uploads/201211/30/1354287870_9567.jpg)






现在知道如何为map指定Compare类了，如果我们想自己写一个compare的类，让map按照我们想要的顺序来存储，比如，按照学生姓名的长短排序进行存储，那该怎么做呢？




其实很简单，只要我们自己写一个函数对象，实现想要的逻辑，定义map的时候把Compare指定为我们自己编写的这个就ok啦。





- 
```cpp
struct CmpByKeyLength {  
  bool operator()(const string& k1, const string& k2) {  
    return k1.length() < k2.length();  
  }  
};
```








是不是很简单！这里我们不用把它定义为模板，直接指定它的参数为string类型就可以了。


【参考代码】






```cpp
int main() {  
  map<string, int, CmpByKeyLength> name_score_map;  
  name_score_map["LiMin"] = 90;   
  name_score_map["ZiLinMi"] = 79;   
  name_score_map["BoB"] = 92;   
  name_score_map.insert(make_pair("Bing",99));  
  name_score_map.insert(make_pair("Albert",86));  
  for (map<string, int>::iterator iter = name_score_map.begin();  
       iter != name_score_map.end();  
       ++iter) {  
    cout << *iter << endl;  
  }  
  return 0;  
}
```


【运行结果】



![](https://img-my.csdn.net/uploads/201211/30/1354287915_4835.jpg)






在第一部分中，我们借助map提供的参数接口，为它指定相应Compare类，就可以实现对map按Key排序，是在创建map并不断的向其中添加元素的过程中就会完成排序。


现在我们想要从map中得到学生按成绩的从低到高的次序输出，该如何实现呢？换句话说，该如何实现Map的按Value排序呢？


        第一反应是利用stl中提供的sort算法实现，这个想法是好的，不幸的是，sort算法有个限制，利用sort算法只能对序列容器进行排序，就是线性的（如vector，list，deque）。map也是一个集合容器，它里面存储的元素是pair，但是它不是线性存储的（前面提过，像红黑树），所以利用sort不能直接和map结合进行排序。


       虽然不能直接用sort对map进行排序，那么我们可不可以迂回一下，把map中的元素放到序列容器（如vector）中，然后再对这些元素进行排序呢？这个想法看似是可行的。要对序列容器中的元素进行排序，也有个必要条件：就是容器中的元素必须是可比较的，也就是实现了<操作的。那么我们现在就来看下map中的元素满足这个条件么？




    那么我们如何实现对pair按value进行比较呢？ 第一种：是最原始的方法，写一个比较函数；  第二种：刚才用到了，写一个函数对象。这两种方式实现起来都比较简单。



```cpp
typedef pair<string, int> PAIR;  
  
bool cmp_by_value(const PAIR& lhs, const PAIR& rhs) {  
  return lhs.second < rhs.second;  
}  
  
struct CmpByValue {  
  bool operator()(const PAIR& lhs, const PAIR& rhs) {  
    return lhs.second < rhs.second;  
  }  
};
```



我们看下sort算法，是不是也像map一样，可以让我们自己指定元素间如何进行比较呢？




```cpp
template <class RandomAccessIterator>  
  void sort ( RandomAccessIterator first, RandomAccessIterator last );  
  
template <class RandomAccessIterator, class Compare>  
  void sort ( RandomAccessIterator first, RandomAccessIterator last, Compare comp );
```
sort算法和map一样，也可以让我们指定元素间如何进行比较，即指定Compare。







```cpp
int main() {  
  map<string, int> name_score_map;  
  name_score_map["LiMin"] = 90;  
  name_score_map["ZiLinMi"] = 79;  
  name_score_map["BoB"] = 92;  
  name_score_map.insert(make_pair("Bing",99));  
  name_score_map.insert(make_pair("Albert",86));  
 //把map中元素转存到vector中   
  vector<PAIR> name_score_vec(name_score_map.begin(), name_score_map.end());  
  sort(name_score_vec.begin(), name_score_vec.end(), CmpByValue());  
 // sort(name_score_vec.begin(), name_score_vec.end(), cmp_by_value);  
  for (int i = 0; i != name_score_vec.size(); ++i) {  
    cout << name_score_vec[i] << endl;  
  }  
  return 0;  
}
```

*8. map和multimap*




map要求key必须是唯一的，而且要是递增的顺序。如果key不唯一，那么map就不能处理的。这时我们就需要multimap，multimap允许key可以重复。

multimap不支持[]操作，而map支持；

multimap的insert()返回的只是一个迭代器，没有bool，而map的insert返回的是(iterator, bool)元素对。

其余的方面，multimap和map都挺类似的。










