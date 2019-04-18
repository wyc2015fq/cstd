# C++必备STL的高频应用实例（string、vector、set、list、map） - 心纯净，行致远 - CSDN博客





2018年11月16日 00:01:57[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：95








# string

### 1/4、常用操作

```cpp
//11152245
#include <iostream>
#include <string>
using namespace std;

int main(){
	string s1;	//初始化字符串，空字符串 
	string s2=s1;	//s2为s1的副本 
	string s3="i am yasuo";	//直接初始化并赋值 
	string s4(10,'a');	//s4存的字符串是aaaaaaaaaa
	string s5(s4);	//s5为s4的副本
	string s6("i am ali");
	string s7=string(6,'c'); 
	
	cout<<"s6.size="<<s7.size() <<endl;
	cout<<(s2.empty() ?"this is a empty string":"this is not a empty string")<<endl; 
} 
/*
output:
s6.size=6
this is a empty string

--------------------------------
Process exited after 0.785 seconds with return value 0
请按任意键继续. . .
*/
```

### 2/4、迭代器iterator在string的应用

```cpp
//11152303
#include <iostream>
#include <string>
using namespace std;

int main(){
	string str("hello world");
	//使用iterator遍历字符串 
	for(string::const_iterator it=str.begin() ;it!=str.end() ;it++){
		cout<<*it<<endl;
	}
} 
/*
output:
h
e
l
l
o

w
o
r
l
d

--------------------------------
Process exited after 0.6435 seconds with return value 0
请按任意键继续. . .

*/
```

### 3/4、string的查找子串等功能

```cpp
//11152310
#include <iostream>
#include <string>
using namespace std;

int main(){
	string str("hello world");
	cout<<str.find("ll",0)<<endl;
	if(str.find("aa",0)==string::npos )
		cout<<"找不到该子串"<<endl; 
} 
/*
2
找不到该子串

--------------------------------
Process exited after 0.6054 seconds with return value 0
请按任意键继续. . .

*/
```

### 4/4、to_string

```cpp
#include <iostream>
#include <string>
using namespace std;

int main(){
	string s1=to_string(123);	//将数字123转成字符串 
	cout<<s1<<endl;
	string s2=to_string(98.67);	//将数字98.67转成字符串 
	cout<<s2<<endl;
	cout<<s1+s2<<endl;	//连接s1,s2
	//如果想用printf输出string,要加一个.c_str() 
	printf("%s\n",(s1+s2).c_str());
	return 0;
}
/*
output:
123
98.670000
12398.670000
12398.670000

———————————— 
请按任意键继续. . .
*/
```

# vector

### 1/2、迭代器iterator在vector的应用

```cpp
//11152310
#include <iostream>
#include <vector>
using namespace std;

int main(){
	vector<int> v1;
	//存入vector 
	for(int i=0;i<10;i++)	v1.push_back(i);
	//输出vector，但是注意要知道vector的长度，此方法不太好 
	for(int i=0;i<10;i++)	cout<<v1[i]; 
	
	cout<<endl;
	
	//顺序输出通法。使用iterator迭代器输出 ，默认顺序输出 
	for(vector<int>::iterator it=v1.begin() ;it!=v1.end() ;it++ )
		cout<<*it;
	
	cout<<endl;
	
	//反向输出通法。使用reverse_iterator迭代器逆序输出 
	for(vector<int>::reverse_iterator it=v1.rbegin() ;it!=v1.rend() ;it++ )
		cout<<*it;
	
}
/*
0123456789
0123456789
9876543210
--------------------------------
Process exited after 0.4554 seconds with return value 0
请按任意键继续. . .
*/
```

### 2/2、vector常用操作

```cpp
#include <iostream>
#include <vector>
#include <string>
 
using namespace std;
 
//template <typename T>
//void showvector(vector<T> v)
//{
//    for (vector<T>::iterator it = v.begin(); it != v.end(); it++)
//    {
//        cout << *it;
//    }
//    cout << endl;
//}
 
int main()
{
    vector<string> v6(7,"s");
    v6.resize(3);  //重新调整vector容量大小
    for (vector<string>::iterator it = v6.begin(); it != v6.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
 
    vector<int> v5(10,6); //列表初始化,注意使用的是花括号
    cout << v5.front() << endl; //访问第一个元素
    cout << v5.back() << endl; //访问最后一个元素
 
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.pop_back(); //删除最后一个元素
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.push_back(6); //加入一个元素并把它放在最后
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.insert(v5.begin()+1,9); //在第二个位置插入新元素
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.erase(v5.begin() + 3);  //删除第四个元素
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.insert(v5.begin() + 1, 7,8); //连续插入7个8
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.clear(); //清除所有内容
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
 	cout << endl;
    system("pause");
    return 0;
} 

/*
sss
6
6
6666666666
666666666
6666666666
69666666666
6966666666
68888888966666666

请按任意键继续. . .

*/
```

注意第一部分的注释为什么不能通过模板函数直接在下面使用呢，参考[C++ STL快速入门](https://blog.csdn.net/zhanshen112/article/details/84033994)

实际上是可以的，只不过需要将compilator改成支持C++11,另外在迭代器iterator之前的容器之前需要使用typename。具体可参照我的另一篇博客：[如何在DevC++中使用C++11语法？](https://blog.csdn.net/zhanshen112/article/details/84146502)

# set

vector可以类比数组，但是增加了很多易用的函数，这一部分包括后面的容器要注意整理一下。

**而set又类似于vector，最重要的区别是set内元素会自动有序升序排列且没有重复的元素。**

### 1/2、set常用操作

```cpp
#include <iostream>
#include <set>
#include <string>
using namespace std;
 
template <typename T>
void showset(set<T> v)
{
    for (typename std::set<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
 
int main()
{
    set<int> s1={9,8,1,2,3,4,5,5,5,6,7,7}; //自动排序，从小到大,剔除相同项
    showset(s1);
    set<string> s2{"hello","sysy","school","hello"}; //字典序排序
    showset(s2);
    s1.insert(9); //有这个值了，do nothing
    showset(s1);
    s2.insert("aaa"); //没有这个字符串，添加并且排序
    showset(s2);
    
    system("pause");
    return 0;
} 

/*
123456789
helloschoolsysy
123456789
aaahelloschoolsysy
请按任意键继续. . .
*/
```

### 2/2、set常用操作

```cpp
#include <iostream>
#include <set>
using namespace std;

int main(){
	set<int> s;	//定义一个空集合s
	s.insert(1);
	cout<<*(s.begin())<<endl;	//s.begin()是集合第一个元素的指针 
	for(int i=0;i<6;i++)	s.insert(i) ;
	//iterator迭代器 
	for(auto it=s.begin();it!=s.end() ;it++ )	cout<<*it<<" ";
	//s.find(a)在s集合中查找值为a元素，一直查找到s.end。s.end是s中最后一个元素的下一位 
	cout<<endl<<(s.find(2)!=s.end())<<endl;	//查到返回，查不到返回0 
	cout<<(s.find(10)!=s.end())<<endl;
	s.erase(1) ;
	cout<<(s.find(1)!=s.end())<<endl;
	return 0;
} 
/*
output:
1
0 1 2 3 4 5
1
0
0
*/
```

# list

list就是链表，在C语言中我们想使用链表都是自己去实现的，实现起来倒不难，但是如果有现成的高效的链表可以使用的话，我们就不需要重复造轮子了。STL就提供了list容器给我们。

**list是一个双向链表，而单链表对应的容器则是foward_list。**

**list即双向链表的优点是插入和删除元素都比较快捷，缺点是不能随机访问元素。**

初始化方式就大同小异了，跟vector基本一样。要想用list先加个头文件list。

### 1/2、list常用操作

```cpp
#include <iostream>
#include <list>
#include <string>
 
using namespace std;
 
template <typename T>
void showlist(list<T> v)
{
    for (typename list<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
 
int main()
{
    list<int> l1{ 1,2,3,4,5,5,6,7,7 };
    showlist(l1);
    list<double> l2;
    list<char> l3(10);
    list<int> l4(5, 10); //将元素都初始化为10
    showlist(l4);
 
    
    system("pause");
    return 0;
} 

/*
output:
123455677
1010101010
请按任意键继续. . .
*/
```

### 2/2、list的排序函数list.sort()

```cpp
#include <iostream>
#include <list>
#include <string>
using namespace std;
 
template <typename T>
void showlist(list<T> v)
{
    for (typename list<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
 
int main()
{
    list<int> l1{ 8,5,7,6,1,2,3,4,5,5,6,7,7 };
    l1.sort();
    showlist(l1);

    system("pause");
    return 0;
} 

/*
output:
1234555667778
请按任意键继续. . .
*/
```

# map

map运用了哈希表地址映射的思想，也就是key-value的思想，来实现的。

首先给出map最好用也最最常用的用法例子，就是用字符串作为key去查询操作对应的value。

要使用map得先加个头文件map。

### 1/2、map常用操作

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

void showmap(map<string, int> v)
{
    for (map<string, int>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << it->first << "  " << it->second << endl;  //注意用法，不是用*it来访问了。first表示的是key，second存的是value
    }
    cout << endl;
}
 
int main()
{
    map<string, int> m1; //<>里的第一个参数表示key的类型,第二个参数表示value的类型
    m1["Kobe"] = 100;
    m1["James"] = 99;
    m1["Curry"] = 98;
 
    string s("Jordan");
    m1[s] = 90;
 
    cout << m1["Kobe"] << endl;
    cout << m1["Jordan"] << endl;
    cout << m1["Durant"] << endl; //不存在这个key，就显示0
 
    m1.erase("Curry");//通过关键字来删除
    showmap(m1);
    m1.insert(pair<string, int>("Harris", 89)); //也可以通过insert函数来实现增加元素
    showmap(m1);
    m1.clear(); //清空全部
 
 
    system("pause");
    return 0;
}

/*
output:
100
90
0
Durant  0
James  99
Jordan  90
Kobe  100

Durant  0
Harris  89
James  99
Jordan  90
Kobe  100

请按任意键继续. . .

*/
```

### 1/2、map常用操作

```cpp
#include <iostream>
#include <map>
using namespace std;

int main(){
	map<string,int> m;
	m["hello"]=2;
	cout<<m["hello"]<<endl;
	//key不存在，返回0 
	cout<<m["world"]<<endl;
	m["world"]=3;
	//使用迭代器遍历，key用it->first，value用it->second获取 
	for(auto it=m.begin() ;it!=m.end() ;it++)
		cout<<it->first<<" "<<it->second<<endl;
	//访问第一个键值对 
	cout<<m.begin()->first<<" "<<m.begin()->second<<endl;
	//访问最后一个键值对 
	cout<<m.rbegin()->first<<" "<<m.rbegin()->second<<endl;
	//输出map元素个数 
	cout<<m.size() <<endl;
	return 0;
}
/*
output:
2
0
hello 2
world 3
hello 2
world 3
2

--------------------------------
Process exited after 0.3415 seconds with return value 0
请按任意键继续. . .
*/
```

# stack

1/1、stack常用操作

```cpp
#include <iostream>
#include <stack> 
using namespace std;
int main(){
	stack<int> s;	//定义空栈 
	for(int i=0;i<8;i++)
		s.push(i) ;	//元素压栈 
	cout<<s.top() <<endl;	//访问栈顶元素 
	cout<<s.size() <<endl;	//访问栈的大小 
	s.pop() ;	//移除栈顶元素 
	return 0;
}
/*
output:
7
8

———————————— 
请按任意键继续. . .
*/
```

# 注意

**如果不使用template模板函数，就不需要使用C++11编译器，另外也不需要在迭代器iterator中使用typename。也就是说，不适用template的iterator不需要特殊进行设置。**



