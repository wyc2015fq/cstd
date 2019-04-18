# STL容器（三）——对map排序 - bigfacesafdasgfewgf - CSDN博客





2014年12月12日 09:18:53[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：3135标签：[map																[排序																[STL																[容器																[vector](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[Data Structure](https://blog.csdn.net/puqutogether/article/category/2595131)





**STL容器（三）——对map排序**



    对于map的排序问题，主要分为两部分：根据key排序；根据value排序。下面我们就分别说一下~




# 1. 根据key进行排序




map默认按照key进行升序排序，和输入的顺序无关。如果是int/double等数值型为key，那么就按照大小排列；如果是string类型，那么就按照字符串的字典序进行排列~（还记得之前说过的字典序吗？当时我们用到了next_permutation这个库函数！）下面我们展示一个例子，说明map中默认按照key升序排列的情况。




**Example 1: 默认按照key升序排序**






```cpp
#include<iostream>
#include<string>
#include<algorithm>
#include<map>
using namespace std; 

int main()
{
	map<string, int> scoreMap; 
	map<string, int>::iterator iter; 

	scoreMap["LiMin"] = 90; 
	scoreMap["ZZihsf"] = 95; 
	scoreMap["Kim"] = 100;
	scoreMap.insert(map<string, int>::value_type("Jack", 88)); 

	for(iter=scoreMap.begin(); iter!=scoreMap.end(); iter++)
		cout<<iter->first<<' '<<iter->second<<endl; 

	return 0; 
}
```



![](https://img-blog.csdn.net/20141212092343287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





上面结果表明，不过我们输入是怎样的顺序，map内部默认始终都是按照key升序排序的。




    那么自然而然就会问到这样一个问题：我不按照升序排列不行吗？我可以修改map的默认排序方式吗？我可以自己设计排序规则吗？这些问题的答案都是必须的~！




   首先透露给大家一个消息：我们在定义map类模板的时候不是只有两个参数吗~（map<string, int>）~~其实map一共有4个参数，后面省略的，或者说是默认的第三个参数就是关于排序规则的~~神奇吧！




    具体而言，它有四个参数，其中我们比较熟悉的有两个: Key 和 Value。第四个是 Allocator，用来定义存储分配模型的，此处我们不作介绍。现在我们重点看下第三个参数： class Compare = less<Key>


这也是一个class类型的，而且提供了默认值 less<Key>。 less是stl里面的一个函数对象，那么什么是函数对象呢？所谓的函数对象：即调用操作符的类，其对象常称为函数对象（function object），它们是行为类似函数的对象。表现出一个函数的特征，就是通过“对象名+(参数列表)”的方式使用一个 类，其实质是对operator()操作符的重载。现在我们来看一下less的实现：




```cpp
template <class T> struct less : binary_function <T,T,bool>
```

```cpp
{  
  bool operator() (const T& x, const T& y) const  
    {return x<y;}  
};
```


注意，这些都是它内部的内容，我们无需要自己编写！上面默认的是less，那么还有几种是可以供我们自由选择的，而且这些都不需要自己写函数对象，直接在定义map的时候，指明对第三个参数即可~

有个需要注意的，在指定compare类型的时候，需要加入key的类型作为参数！例如：greater<string>。




**Example 2: 按照key降序排序**






```cpp
#include<iostream>
#include<string>
#include<algorithm>
#include<map>
using namespace std; 

int main()
{
	//注意这里的greater<string>，其有个参数就是key的类型，比如这里就是string
	map<string, int, greater<string> > scoreMap;   
	map<string, int, greater<string> >::iterator iter; 

	scoreMap["LiMin"] = 90; 
	scoreMap["ZZihsf"] = 95; 
	scoreMap["Kim"] = 100;
	scoreMap.insert(map<string, int>::value_type("Jack", 88)); 

	for(iter=scoreMap.begin(); iter!=scoreMap.end(); iter++)
		cout<<iter->first<<' '<<iter->second<<endl; 

	return 0; 
}
```



![](https://img-blog.csdn.net/20141212100626000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





当然，默认的compare参数还有这样的几种：equal_to 相等；not_equal_to 不相等；less 小于；greater 大于；less_equal 小于等于；greater_equal 大于等于。不过其余的几种貌似不太用的到啊~




    那么怎么样自定义compare呢？这个就需要我们自己写一个函数对象了。如下：




**Example 3: 自定义比较规则**




我们可以自定义一个按照key中的string长度排序，程序如下：






```cpp
#include<iostream>
#include<string>
#include<algorithm>
#include<map>
using namespace std; 

struct cmp  //自定义比较规则
{
	bool operator() (const string& str1, const string& str2)
	{
		return str1.length() < str2.length(); 
	}
};

int main()
{
	map<string, int, cmp > scoreMap;  //这边调用cmp  
	map<string, int, cmp >::iterator iter; 

	scoreMap["LiMin"] = 90; 
	scoreMap["ZZihsf"] = 95; 
	scoreMap["Kim"] = 100;
	scoreMap.insert(map<string, int>::value_type("Jack", 88)); 

	for(iter=scoreMap.begin(); iter!=scoreMap.end(); iter++)
		cout<<iter->first<<' '<<iter->second<<endl; 

	return 0; 
}
```



![](https://img-blog.csdn.net/20141212102800062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





按照这个示例，我们还可以设计其他的比较规则，把比较规则写在cmp结构中，然后在定义map的时候调用即可~




# 2. 根据value排序




    其实更多的时候，我们需要对value进行排序，比如对学生成绩这个map进行排序的时候，我们不大可能需要名字排序，却很有可能需要按照分数进行排序。那么问题来了，我们是否可以按照上面那样子，简单的指明一个参数，或者自定义一个比较规则cmp即可呢？答案是失望的，map模板里面没有这个性能。。。




    究其原因，map是个关联容器，不是序列容器。像是一些序列容器list, vector都是可以排序的~咦，我们可不可以把map中的<key, value>对放在一个vector中呢？那样是否就可以直接用vetor的sort函数呢？这个想法不错~

    下面我们来详细说一下这个想法：




首先，map中的<key, value>是pair形式的，那么我们就可以把一个pair作为vector中的元素；

    然后，调用vetor容器中的sort函数，sort函数也是可以用户指定比较类型的。






```cpp
template <class RandomAccessIterator>  
  void sort ( RandomAccessIterator first, RandomAccessIterator last );  
  
template <class RandomAccessIterator, class Compare>  
  void sort ( RandomAccessIterator first, RandomAccessIterator last, Compare comp );
```



程序如下：






```cpp
#include<iostream>
#include<string>
#include<algorithm>
#include<map>
#include<vector>
using namespace std; 

typedef pair<string, int> PAIR; 

struct cmp  //自定义比较规则
{
	bool operator() (const PAIR& P1, const PAIR& P2)  //注意是PAIR类型，需要.firt和.second。这个和map类似
	{
		return P1.second < P2.second; 
	}
};


int main()
{
	map<string, int> scoreMap;  //这边调用cmp  
	map<string, int>::iterator iter; 

	scoreMap["LiMin"] = 90; 
	scoreMap["ZZihsf"] = 95; 
	scoreMap["Kim"] = 100;
	scoreMap.insert(map<string, int>::value_type("Jack", 88)); 

	vector<PAIR>scoreVector; 
	for(iter=scoreMap.begin(); iter!=scoreMap.end();iter++)  //这边本来是使用vector直接初始化的，当时由于vc 6.0 编译器问题，只能这样写，而且还有非法内存。。
		scoreVector.push_back(*iter); 
	//转化为PAIR的vector
	sort(scoreVector.begin(), scoreVector.end(), cmp());  //需要指定cmp

	for(int i=0; i<=scoreVector.size(); i++)  //也要按照vector的形式输出
		cout<< scoreVector[i].first<<' '<<scoreVector[i].second <<endl; 

	/*
	for(iter=scoreMap.begin(); iter!=scoreMap.end(); iter++)
		cout<<iter->first<<' '<<iter->second<<endl; 
	*/

	return 0; 
}
```



![](https://img-blog.csdn.net/20141212111332562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








    这样我们就把map的排序：按照key排序；按照value排序都说好了~](https://so.csdn.net/so/search/s.do?q=容器&t=blog)](https://so.csdn.net/so/search/s.do?q=STL&t=blog)](https://so.csdn.net/so/search/s.do?q=排序&t=blog)](https://so.csdn.net/so/search/s.do?q=map&t=blog)




