# STL中map, list, and pair的学习笔记 - bigfacesafdasgfewgf - CSDN博客





2014年05月05日 14:53:00[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2237








STL中map, list, and pair的学习笔记




在做题目的时候遇到了STL标准库中的一些容器，所以才有了这篇学习笔记。目前只遇到了map, list, and pair这三个，日后要是还遇到了其他的常用容器，再过来补充。




首先还是尊重前辈的成果，把参考的文章地址先置顶于正文中。

[http://www.cnblogs.com/fangyukuan/archive/2010/09/21/1832364.html](http://www.cnblogs.com/fangyukuan/archive/2010/09/21/1832364.html)


[http://blog.csdn.net/nupt123456789/article/details/8120397](http://blog.csdn.net/nupt123456789/article/details/8120397)


[http://baike.baidu.com/subview/332356/10428592.htm?fr=aladdin](http://baike.baidu.com/subview/332356/10428592.htm?fr=aladdin)

[http://www.cnblogs.com/skynet/archive/2010/06/18/1760518.html](http://www.cnblogs.com/skynet/archive/2010/06/18/1760518.html)





首先，我们简单介绍一下STL。STL是标准模版库，它是一些容器的集合，这些容器有list，vector，set, map等等。STL是C++的一部分，所以在编写C++代码的时候，可以直接使用STL模版库。

STL主要有如下六个部分组成：容器containers，迭代器iterators，空间配置器allocators，配接器adaptors，算法algorithms，仿函数funtors。




其中，在容器部分常用的头文件包括：<vector>, <list>, <deque>, <set>, <map>, <stack>, <queue>等等。下面做一下简单的介绍

vector：连续存储的元素；

list：由节点组成的双向链表，每个结点包含着一个元素；

set：由节点组成的红黑树，每个节点都包含着一个元素，节点之间以某种作用于元素对的谓词排列，没有两个不同的元素能够拥有相同的次序；

stack：后进先出的值的排列；

map：由｛key，value｝对组成的集合；

……

这次用到的是list和map。




下面我们先介绍list：

STL中的list其实就是一个双向链表，可以高效地插入、删除里面的元素。而且需要注意的是，list不支持随机访问，所以不能使用at或者operator之类的操作。













![](https://img-blog.csdn.net/20140505155515484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140505155558031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


那么如何构建一个list链表呢？



```cpp
#include<iostream>
#include<list>
#include<string>
using namespace std;

class Student{  
private:  
    int ID;  
    string Name;  
public:  
    Student(int ID,string Name)  
    {  
        this->ID=ID;  
        this->Name=Name;  
    }  
    int getID()  
    {  
        return ID;  
    }  
    string getName()  
    {  
        return Name;  
    }  
}; 

int main()
{
	//建立一个空list
	list<double> L0;
	cout<<"The size of L0 is: "<<L0.size()<<endl;

	//建立一个有n个元素的list，其中的元素都是为空
	list<double> L1(5);
	cout<<"The size of L1 is: "<<L1.size()<<endl;

	//建立一个list，并且初始化
	list<double> L2(5, 1.1);
	cout<<"L2: ";
	list<double>::iterator it;//在输出的时候，需要用到迭代器
	for(it=L2.begin(); it!=L2.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	//构建一个list，其元素由一个数组赋值
	double a[8]={3.45, 67, 10, 0.67, 8.99, 9.78, 6.77, 34.677};
	list<double> L3(a, a+5);//注意，不是值形式，而是地址形式
	cout<<"L3: ";
	for(it=L3.begin(); it!=L3.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	//构建一个list，其由另一个list来赋值
	list<double> L4(L3);
	cout<<"L4: ";
	for(it=L4.begin(); it!=L4.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	//构建一个list，先不初始化，过后再赋值，要用到assign函数
	list<double>L5;
	L5.assign(5, 10.2);//第一个输入是个数，第二个数是值
	cout<<"L5: ";
	for(it=L5.begin(); it!=L5.end(); ++it)
		cout<<*it<<" ";
	cout<<endl;

	//构建一个list，里面的数据类型是自己定义的数据类型
	list<Student>L6;
    Student stu1(1,"pu");  
    Student stu2(2,"njupt"); 
    L6.push_back(stu1);//增加一个元素到list的尾部  
    L6.push_back(stu2);  
    list<Student>::iterator iter_stu; //其实只要使用到list里面每个元素的时候，都要用到迭代器 
    cout << "L6: "<<endl;  
    for (iter_stu=L6.begin();iter_stu!=L6.end();iter_stu++)  
    {  
        cout<<"ID:"<<iter_stu->getID()<<" Name:"<<iter_stu->getName()<<endl; //指针指向student类里面的成员函数 
    } 

	return 0;

}
```


上面就是构建一个list，并且使用各种方法初始化赋值的实例。


其实的对于list的删除、修改等操作以后遇到了再学习。而且，师门师兄们都说：list在实际中用到较少，知道它的工作原理即可，不要太注重运用。




下面总结更为重要的map的用法，这部分我想和pair一起介绍。

我们首先通过一段代码来感受一下map的魅力。



```cpp
#include<iostream>
#include <utility>
#include<map>

using namespace std;

int main () 
{
  map<int, int> my_map;
  pair <int,int> one;
  pair <int,int> two;

  one = make_pair (10,20);
  two = make_pair (11,30);

  cout << "one: " << one.first << ", " << one.second << "\n";
  cout << "two: " << two.first << ", " << two.second << "\n";

  my_map.insert(one);
  my_map.insert(two);

  map<int,int>::iterator it = my_map.begin();
  while(it != my_map.end())
  {
          cout<<"my_map:"<<it->first<<", "<<it->second<<endl;
          ++it;
  }
  return 0;
}
```


我们发现：在定义一个map或者是pair的时候，用到的形式和定义一个list差不多。不一样的是，给pair赋值需要用到make_pair函数；给map赋值的时候，需要用到my_map.insert()函数。


这些都是我们对map和pair的初步影响。而map的真正魅力还在于查找功能，它查找的复杂度是log(n)。




map是一个键值对，即(key, value)对。map提供了基于key的快速检索能力，在一个map中key是唯一的，map提供的是双向迭代器，即有从前往后的iterator，也有从后往前的reverse_iterator。

map要求按照key递增有序，map的迭代器也要死递增的。如果对于元素不需要保持递增有序，那么可以使用hash_map。

map中的key一定也是唯一的，如果需要插入一个key值一样的键值对，那么我们可以使用multimap。




关于如何定义一个map，在上面的示例中我们已经给出了。我们再通过一个比较复杂的例子来展示map的定义和赋值，如下：



```cpp
#include<iostream>
#include<map>
#include<string>
using namespace std;
 
typedef    int    clientId;//把int类型用ID别名来表示。
typedef struct{
    int scanRate;
    string socketAddr;
}clientInfo;//把这个结构用info来表示
 
int main(int argc,char** argv)
{
    typedef map<clientId,clientInfo> clientEdp;//把map的定义用edp来表示

    typedef map<clientId,clientInfo>::const_iterator iterator;//map的迭代器
 
    clientEdp clients;//定义一个map
    clientInfo client[10];//定义100个info
    char str[10];
    string strAddr("socket addr client ");
 
    for(int i=0;i<10;i++)
    {
        client[i].scanRate=i+1;    
        //convert int to char*
        itoa(i+1,str,10);//i+1是需要转换的数值，str是转换后字符串的存储，10是10进制
        //concatenate strAddr and str
        client[i].socketAddr=strAddr+str; //两个字符串相加
        cout<<client[i].socketAddr<<endl;
        clients.insert(
            make_pair(i+1,client[i]));  //上面结构client插入到clients这个map中,可以看到map中的key 就是递增的
    }
    //delete str;
    for(iterator it=clients.begin();it!=clients.end();it++)
    {
        cout<<"clientId:"<<it->first<<endl;//map的key
        cout<<"scanRate:"<<it->second.scanRate<<endl;//map的value
        cout<<"socketAddr:"<<it->second.socketAddr<<endl;//还是map的value
        cout<<endl;//注意，上面的second由于是个结构类型，故后面还可以用点.运算
    }
}
```


分析上例可知：map中的key一定要是唯一的，且是递增的；value可以是一个值，可以是一个结构。遍历map中的元素的时候，也是需要迭代器的。在迭代器中我们需要用到begin(), end()，在给map赋值的时候，需要用到insert()。




下面我们再详细介绍一下map中查找find的用法。

find(k)方法简单地返回键值为k的元素的迭代器。如果没有元素的键值为k，则返回map的end()迭代器；如果有，则返回迭代器指针，一般为it。由于map是按照键key递增顺序排列的，故使用map查找的复杂度只有O(logN)。这才是map最主要的优势，也是面试map中最常问到的知识点。下面我们通过一个实例来展示map查找功能的魅力：






```cpp
#include<iostream>
#include<map>
#include<string>
using namespace std;
 
typedef    int    clientId;
typedef struct{
    int scanRate;
    string socketAddr;
}clientInfo;
 
int main(int argc,char** argv)
{
    typedef map<clientId,clientInfo> clientEdp;
    typedef map<clientId,clientInfo>::const_iterator iterator;
 
    clientEdp clients;
    clientInfo client[100];
    char* str=new char[10];
    string strAddr("socket addr client ");
 
    for(int i=0;i<100;i++)
    {
        client[i].scanRate=i+1;    
        //convert int to char*
        itoa(i+1,str,10);
        //concatenate strAddr and str
        client[i].socketAddr=strAddr+str; 
        clients.insert(
            make_pair(i+1,client[i]));    
    }
    //delete str;
	int id=10;
    iterator it=clients.find(id);//查找，返回迭代器的指针
    if(it!=clients.end()){//如果不存在，则返回的迭代器指针指向end()
        cout<<"clientId: "<<id
            <<" exists in clients"<<endl;
    }
    else{
        cout<<"clientId: "<<id
            <<" doesn't exist in clients"<<endl;
    }  
}
```

一定要记住find()的返回情况：如果查找到，则返回找到的指针位置；如果没有查找到，返回指针指向end()。还有重要的是，map查找的复杂度为O(logN)。

map中还有一个常用的就是erase()。比如我们需要清楚clientId=10的键值对，那么我们只需要对clients调用erase：clients.erase(clients.find(10)); 这里需要注意的是erase()的输入还是find返回的指针。如果我们还想清除clientId从1到10的键值对，我们可以使用：clients.erase(client.find(1), clients.find(10));
 不过当find()返回的end()，那么这样调用erase()方法则是一个严重的错误，会对map造成破坏操作。




关于map的插入操作，前面的例子中已经涉及了insert()。其实，map还有一种更为简单的插入操作方法，即使用下标。

例如，我们需要在上面定义的clients的map中插入一个key=110的键值对，那么我们可以使用如下方法：



```cpp
clientInfo cltInfo;
cltInfo.scanRate=10;
cltInfo.socketAddr="110";
 
clients[110]=cltInfo;
```
所以我们可以简单地使用下标的方式来实现插入操作。其实，使用下标的方法来插入有一个比较好玩的过程：首先需要检查key=k是否已经在map里面，如果没有这样的key，则我们就直接插入；如果已经存在这样的key，那么是做覆盖处理的。显然，使用下标来插入就会包含这样的一个判断过程，如果要插入较多的键值对，那么必然会影响程序的效率，故当我们需要批量插入许多个键值对时，我们就使用insert()。但是需要注意的是，如果key已经存在了map中，下标[]方式反而比insert()的效率更加高而且更加美观简洁！所以，我们在对map实现插入操作的时候，会综合两种方式来用，即我们首先判断key是否已经存在于map中了，如果已经存在就用下标[]的方式，如果不存在，则用insert()的方法。


其实，下标[]的方法不仅仅可以用于插入操作，我们也可以通过键key检索出value。而且，我们用[]还可以统计信息，例如下面这个例子：



```cpp
#include<iostream>//其实，这个程序我一直没有运行出来结果……
#include<map>
#include<string>
using namespace std;
 
int main(int argc,char** argv)
{
    map<string,int> bookMap;
 
    string book;
    int count;
    int total=0;
    while(cin>>book>>count)
        bookMap[book]+=count;
 
    map<string,int>::iterator it;
    for(it=bookMap.begin();it!=bookMap.end();it++)
    {
        total+=it->second;
        cout<<it->first<<'\t'<<it->second<<endl;
    }
    cout<<"total count:"<<total<<endl;
}
```


这样，我们介绍的map就到这里了。其实，回过头来看，我们最需要记住的就是map查找的功能，其复杂度是O(logN)；还有就是使用insert()和下标[]方式各自的优势，以及[]对键值的索引作用。




map要求key必须是唯一的，而且要是递增的顺序。如果key不唯一，那么map就不能处理的。这时我们就需要multimap，multimap允许key可以重复。

multimap不支持[]操作，而map支持；

multimap的insert()返回的只是一个迭代器，没有bool，而map的insert返回的是(iterator, bool)元素对。

其余的方面，multimap和map都挺类似的。




这样我们就介绍完了map和multimap。


















































































