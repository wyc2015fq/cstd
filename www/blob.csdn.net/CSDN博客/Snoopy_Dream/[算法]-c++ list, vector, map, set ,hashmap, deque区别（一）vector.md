# [算法] - c++ list, vector, map, set ,hashmap, deque区别（一）vector - Snoopy_Dream - CSDN博客





2018年12月26日 11:54:50[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：71
所属专栏：[编程感悟](https://blog.csdn.net/column/details/31618.html)









list不能随机访问，但vector可以，vector不能删除头部，而且插入删除的效率低。

**List封装了链表**,**Vector封装了数组**, list和vector得最主要的区别在于**vector使用连续内存存储的，他支持[]运算符**，而**list是以链表形式实现的，不支持[]。**

**Vector对于随机访问的速度很快**，但是对于**插入尤其是在头部插入元素速度很慢**，在尾部插入速度很快。

**List对于随机访问速度慢得多**，因为可能要遍历整个链表才能做到，但是**对于插入就快的多了**，不需要拷贝和移动数据，只需要改变指针的指向就可以了。另外对于新添加的元素，Vector有一套算法，而List可以任意加入。


**Map,Set **属于标准关联容器，使用了非常高效的**平衡检索二叉树**：**红黑树**，他的插入删除效率比其他序列容器高是因为不需要做内存拷贝和内存移动，而直接替换指向节点的指针即可。
**Set和Vector的区别**在于**Set不包含重复的数据**。

**Set和Map **的区别在于Set只含有Key，而Map有一个Key和Key所对应的Value两个元素。
**Map和Hash_Map **的区别是**Hash_Map使用了Hash算法来加快查找过程，但是需要更多的内存来存放这些Hash桶元**素，因此可以算得上是**采用空间来换取时间策略**。

### **vector**

    向量 相当于一个数组

    在内存中分配一块连续的内存空间进行存储。支持不指定vector大小的存储。STL内部实现时，首先分配一个非常大的内存空间预备进行存储，即capacituy（）函数返回的大小，当超过此分配的空间时再整体重新放分配一块内存存储，这给人以vector可以不指定vector即一个连续内存的大小的感觉。通常此默认的内存分配能完成大部分情况下的存储。

   优点：(1) 不指定一块内存大小的数组的连续存储，即可以像数组一样操作，但可以对此数组

               进行动态操作。通常体现在push_back() pop_back()

               (2) 随机访问方便，即支持[ ]操作符和vector.at()

               (3) 节省空间。

   缺点：(1) 在内部进行插入删除操作效率低。

               (2) 只能在vector的最后进行push和pop，**不能在vector的头**进行push和pop。

               (3) 当动态添加的数据超过vector默认分配的大小时要进行整体的重新分配、拷贝与释

                     放 


2 **list**
**双向链表**

    每一个结点都包括一个信息快Info、一个前驱指针Pre、一个后驱指针Post。可以不分配必须的内存大小方便的进行添加和删除操作。使用的是非连续的内存空间进行存储。

   优点：(1) **不使用连续内存完成动态操作。**

               (2) 在内部**方便的进行插入和删除操作**

               (3) 可在**两端进行push、pop**

   缺点：(1) **不能进行内部的随机访问**，即不支持[ ]操作符和vector.at()

               (2) **相对于verctor占用内存多**


3 **deque**

   双端队列 double-end queue
**deque是在功能上合并了vector和list。**

   优点：(1) 随机访问方便，即支持[ ]操作符和vector.at()

               (2) 在内部方便的进行插入和删除操作

               (3) 可在两端进行push、pop

   缺点：(1) 占用内存多

### **使用区别：**



     1 如果你需要高效的随即存取，而不在乎插入和删除的效率，使用vector 

     2 如果你需要大量的插入和删除，而不关心随即存取，则应使用list 

     3 如果你需要随即存取，而且关心两端数据的插入和删除，则应使用deque













C++STL中vector容器的用法 



http://xiamaogeng.blog.163.com/blog/static/1670023742010102494039234/



vector是C++标准模板库中的部分内容，它是一个多功能的，能够操作多种数据结构和算法的模板类和函数库。vector之所以被认为是一个容器，是因为它能够像容器一样存放各种类型的对象，简单地说vector是一个能够存放任意类型的动态数组，能够增加和压缩数据。为了可以使用vector，必须在你的头文件中包含下面的代码：

#include <vector>

vector属于std命名域的，因此需要通过命名限定，如下完成你的代码：

using std::vector;     vector<int> v;

或者连在一起，使用全名：

std::vector<int> v;

建议使用全局的命名域方式：

using namespace std;

1.vector的声明

   vector<ElemType> c;   创建一个空的vector

   vector<ElemType> c1(c2); 创建一个vector c1，并用c2去初始化c1

   vector<ElemType> c(n) ; 创建一个含有n个ElemType类型数据的vector;

   vector<ElemType> c(n,elem); 创建一个含有n个ElemType类型数据的vector,并全部初始化为elem;

   c.~vector<ElemType>(); 销毁所有数据,释放资源;

2.vector容器中常用的函数。(c为一个容器对象）

    c.push_back(elem);   在容器最后位置添加一个元素elem

    c.pop_back();            删除容器最后位置处的元素

    c.at(index);                返回指定index位置处的元素

    c.begin();                   返回指向容器最开始位置数据的指针

    c.end();                      返回指向容器最后一个数据单元的指针+1

    c.front();                     返回容器最开始单元数据的引用

    c.back();                     返回容器最后一个数据的引用

    c.max_size();              返回容器的最大容量

    c.size();                      返回当前容器中实际存放元素的个数

    c.capacity();               同c.size()

     c.resize();                   重新设置vector的容量

    c.reserve();                同c.resize()

    c.erase(p);               删除指针p指向位置的数据，返回下指向下一个数据位置的指针（迭代器）

    c.erase(begin,end)     删除begin,end区间的数据，返回指向下一个数据位置的指针（迭代器）

    c.clear();                    清除所有数据

    c.rbegin();                  将vector反转后的开始指针返回(其实就是原来的end-1)

    c.rend();                     将vector反转后的结束指针返回(其实就是原来的begin-1)

    c.empty();                   判断容器是否为空，若为空返回true，否则返回false

    c1.swap(c2);               交换两个容器中的数据

    c.insert(p,elem);          在指针p指向的位置插入数据elem,返回指向elem位置的指针       

    c.insert(p,n,elem);      在位置p插入n个elem数据，无返回值

    c.insert(p,begin,end) 在位置p插入在区间[begin,end)的数据，无返回值

3.vector中的操作

    operator[] 如： c.[i];

    同at()函数的作用相同，即取容器中的数据。



## [C++ STLList队列用法（实例）](http://www.cnblogs.com/madlas/articles/1364503.html)

http://www.cnblogs.com/madlas/articles/1364503.html

C++ STL List队列用法（实例）

```cpp
#include <iostream>
#include <list>
#include <numeric>
#include <algorithm>

using namespace std;

//创建一个list容器的实例LISTINT
typedef list<int> LISTINT;

//创建一个list容器的实例LISTCHAR
typedef list<char> LISTCHAR;

void main(void)
{
    //--------------------------
    //用list容器处理整型数据
    //--------------------------
    //用LISTINT创建一个名为listOne的list对象
    LISTINT listOne;
    //声明i为迭代器
    LISTINT::iterator i;

    //从前面向listOne容器中添加数据
    listOne.push_front (2);
    listOne.push_front (1);

    //从后面向listOne容器中添加数据
    listOne.push_back (3);
    listOne.push_back (4);

    //从前向后显示listOne中的数据
    cout<<"listOne.begin()--- listOne.end():"<<endl;
    for (i = listOne.begin(); i != listOne.end(); ++i)
        cout << *i << " ";
    cout << endl;

    //从后向后显示listOne中的数据
LISTINT::reverse_iterator ir;
    cout<<"listOne.rbegin()---listOne.rend():"<<endl;
    for (ir =listOne.rbegin(); ir!=listOne.rend();ir++) {
        cout << *ir << " ";
    }
    cout << endl;

    //使用STL的accumulate(累加)算法
    int result = accumulate(listOne.begin(), listOne.end(),0);
    cout<<"Sum="<<result<<endl;
    cout<<"------------------"<<endl;

    //--------------------------
    //用list容器处理字符型数据
    //--------------------------

    //用LISTCHAR创建一个名为listOne的list对象
    LISTCHAR listTwo;
    //声明i为迭代器
    LISTCHAR::iterator j;

    //从前面向listTwo容器中添加数据
    listTwo.push_front ('A');
    listTwo.push_front ('B');

    //从后面向listTwo容器中添加数据
    listTwo.push_back ('x');
    listTwo.push_back ('y');

    //从前向后显示listTwo中的数据
    cout<<"listTwo.begin()---listTwo.end():"<<endl;
    for (j = listTwo.begin(); j != listTwo.end(); ++j)
        cout << char(*j) << " ";
    cout << endl;

    //使用STL的max_element算法求listTwo中的最大元素并显示
    j=max_element(listTwo.begin(),listTwo.end());
    cout << "The maximum element in listTwo is: "<<char(*j)<<endl;
}
```

#include <iostream>

#include <list>

using namespace std;

typedef list<int> INTLIST;

//从前向后显示list队列的全部元素

void put_list(INTLISTlist, char *name)

{

    INTLIST::iterator plist;

    cout << "The contents of " << name << " : ";

    for(plist = list.begin(); plist != list.end(); plist++)

        cout << *plist << " ";

    cout<<endl;

}

//测试list容器的功能

void main(void)

{

//list1对象初始为空

    INTLIST list1;  

    //list2对象最初有10个值为6的元素

    INTLIST list2(10,6);

    //list3对象最初有3个值为6的元素

    INTLIST list3(list2.begin(),--list2.end());

    //声明一个名为i的双向迭代器

    INTLIST::iterator i;

    //从前向后显示各list对象的元素

    put_list(list1,"list1");

    put_list(list2,"list2");

    put_list(list3,"list3");


//从list1序列后面添加两个元素

list1.push_back(2);

list1.push_back(4);

cout<<"list1.push_back(2) andlist1.push_back(4):"<<endl;

    put_list(list1,"list1");

//从list1序列前面添加两个元素

list1.push_front(5);

list1.push_front(7);

cout<<"list1.push_front(5) andlist1.push_front(7):"<<endl;

    put_list(list1,"list1");

//在list1序列中间插入数据

list1.insert(++list1.begin(),3,9);

cout<<"list1.insert(list1.begin()+1,3,9):"<<endl;

    put_list(list1,"list1");

//测试引用类函数

cout<<"list1.front()="<<list1.front()<<endl;

cout<<"list1.back()="<<list1.back()<<endl;

//从list1序列的前后各移去一个元素

list1.pop_front();

list1.pop_back();

cout<<"list1.pop_front() andlist1.pop_back():"<<endl;

    put_list(list1,"list1");

//清除list1中的第2个元素

list1.erase(++list1.begin());

cout<<"list1.erase(++list1.begin()):"<<endl;

    put_list(list1,"list1");

//对list2赋值并显示

list2.assign(8,1);

cout<<"list2.assign(8,1):"<<endl;

    put_list(list2,"list2");

//显示序列的状态信息

cout<<"list1.max_size(): "<<list1.max_size()<<endl;

cout<<"list1.size(): "<<list1.size()<<endl;

cout<<"list1.empty(): "<<list1.empty()<<endl;

//list序列容器的运算

    put_list(list1,"list1");

    put_list(list3,"list3");

cout<<"list1>list3: "<<(list1>list3)<<endl;

cout<<"list1<list3: "<<(list1<list3)<<endl;

//对list1容器排序

list1.sort();

    put_list(list1,"list1");


//结合处理

list1.splice(++list1.begin(),list3);

    put_list(list1,"list1");

    put_list(list3,"list3");

}









[C++map 映照容器](http://www.cppblog.com/vontroy/archive/2010/05/16/115501.html)



http://www.cppblog.com/vontroy/archive/2010/05/16/115501.html



map映照容器的元素数据是一个键值和一个映照数据组成的，键值与映照数据之间具有一一映照的关系。

        map映照容器的数据结构是采用红黑树来实现的，插入键值的元素不允许重复，比较函数只对元素的键值进行比较，元素的各项数据可通过键值检索出来。

        使用map容器需要头文件包含语句“#include<map>”，map文件也包含了对multimap多重映照容器的定义。

**1、map创建、元素插入和遍历访问**创建map对象，键值与映照数据的类型由自己定义。在没有指定比较函数时，元素的插入位置是按键值由小到大插入到黑白树中去的，下面这个程序详细说明了如何操作map容器。

 1![](https://img-blog.csdnimg.cn/20181226115450871)#include <map>

 2![](https://img-blog.csdnimg.cn/20181226115450871)#include <string>

 3![](https://img-blog.csdnimg.cn/20181226115450871)#include <iostream>

 4![](https://img-blog.csdnimg.cn/20181226115450871)

 5![](https://img-blog.csdnimg.cn/20181226115450871)using std :: cout ;

 6![](https://img-blog.csdnimg.cn/20181226115450871)using std :: endl ;

 7![](https://img-blog.csdnimg.cn/20181226115450871)using std :: string ;

 8![](https://img-blog.csdnimg.cn/20181226115450871)using std :: map ;

 9![](https://img-blog.csdnimg.cn/20181226115450871)

10![](https://img-blog.csdnimg.cn/20181226115450871)int main()

11![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

12![](https://img-blog.csdnimg.cn/20181226115451144)     //定义map对象，当前没有任何元素

13![](https://img-blog.csdnimg.cn/20181226115451144)     map<string,float> m ;

14![](https://img-blog.csdnimg.cn/20181226115451144)

15![](https://img-blog.csdnimg.cn/20181226115451144)     //插入元素，按键值的由小到大放入黑白树中

16![](https://img-blog.csdnimg.cn/20181226115451144)     m["Jack"] = 98.5 ;

17![](https://img-blog.csdnimg.cn/20181226115451144)     m["Bomi"] = 96.0 ;

18![](https://img-blog.csdnimg.cn/20181226115451144)     m["Kate"] = 97.5 ;

19![](https://img-blog.csdnimg.cn/20181226115451144)

20![](https://img-blog.csdnimg.cn/20181226115451144)     //先前遍历元素

21![](https://img-blog.csdnimg.cn/20181226115451144)     map<string,float> :: iterator it ;

22![](https://img-blog.csdnimg.cn/20181226115451144)     for(it = m.begin() ; it != m.end() ; it ++)

23![](https://img-blog.csdnimg.cn/20181226115451472)![](https://img-blog.csdnimg.cn/20181226115451487)![](https://img-blog.csdnimg.cn/20181226115451125){

24![](https://img-blog.csdnimg.cn/20181226115451144)          cout << (*it).first << " : " << (*it).second << endl ;

25![](https://img-blog.csdnimg.cn/20181226115451524)     }

26![](https://img-blog.csdnimg.cn/20181226115451144)

27![](https://img-blog.csdnimg.cn/20181226115451144)     return 0 ;

28![](https://img-blog.csdnimg.cn/20181226115451631)}

29![](https://img-blog.csdnimg.cn/20181226115450871)

        运行结果：

                          Bomi ：96

                          Jack  ：98.5

                          Kate  ：97.5

        程序编译试，会产生代号为“warning C4786” 的警告， “4786” 是标记符超长警告的代号。可以在程序的头文件包含代码的前面使用"#pragma waring(disable:4786)" 宏语句，强制编译器忽略该警告。4786号警告对程序的正确性和运行并无影响。
**2、删除元素        map**映照容器的 erase() 删除元素函数，可以删除某个迭代器位置上的元素、等于某个键值的元素、一个迭代器区间上的所有元素，当然，也可使用clear()方法清空map映照容器。

        下面这个程序演示了删除map容器中键值为28的元素：

 1![](https://img-blog.csdnimg.cn/20181226115450871)#include <map>

 2![](https://img-blog.csdnimg.cn/20181226115450871)#include <string>

 3![](https://img-blog.csdnimg.cn/20181226115450871)#include <iostream>

 4![](https://img-blog.csdnimg.cn/20181226115450871)

 5![](https://img-blog.csdnimg.cn/20181226115450871)using std :: cout ;

 6![](https://img-blog.csdnimg.cn/20181226115450871)using std :: endl ;

 7![](https://img-blog.csdnimg.cn/20181226115450871)using std :: string ;

 8![](https://img-blog.csdnimg.cn/20181226115450871)using std :: map ;

 9![](https://img-blog.csdnimg.cn/20181226115450871)

10![](https://img-blog.csdnimg.cn/20181226115450871)int main()

11![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

12![](https://img-blog.csdnimg.cn/20181226115451144)    //定义map对象，当前没有任何元素

13![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char> m ;

14![](https://img-blog.csdnimg.cn/20181226115451144)    //插入元素，按键值的由小到大放入黑白树中

15![](https://img-blog.csdnimg.cn/20181226115451144)    m[25] = 'm' ;

16![](https://img-blog.csdnimg.cn/20181226115451144)    m[28] = 'k' ;

17![](https://img-blog.csdnimg.cn/20181226115451144)    m[10] = 'x' ;

18![](https://img-blog.csdnimg.cn/20181226115451144)    m[30] = 'a' ;

19![](https://img-blog.csdnimg.cn/20181226115451144)    //删除键值为28的元素

20![](https://img-blog.csdnimg.cn/20181226115451144)    m.erase(28) ;

21![](https://img-blog.csdnimg.cn/20181226115451144)    //向前遍历元素

22![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char> :: iterator it ;

23![](https://img-blog.csdnimg.cn/20181226115451144)    for(it = m.begin() ; it != m.end() ; it ++)

24![](https://img-blog.csdnimg.cn/20181226115451472)![](https://img-blog.csdnimg.cn/20181226115451487)![](https://img-blog.csdnimg.cn/20181226115451125){

25![](https://img-blog.csdnimg.cn/20181226115451144)        //输出键值与映照数据

26![](https://img-blog.csdnimg.cn/20181226115451144)        cout << (*it).first << " : " << (*it).second << endl ;

27![](https://img-blog.csdnimg.cn/20181226115451524)    }

28![](https://img-blog.csdnimg.cn/20181226115451144)    return 0 ;

29![](https://img-blog.csdnimg.cn/20181226115451631)}

30![](https://img-blog.csdnimg.cn/20181226115450871)

运行结果：

                     10 : x

                     25 : m

                     30 : a
**3、元素反向遍历**

      可以用反向迭代器reverse_iterator反向遍历map映照容器中的数据，它需要rbegin()方法和rend()方法指出反向遍历的起始位置和终止位置。

 1![](https://img-blog.csdnimg.cn/20181226115450871)#include <map>

 2![](https://img-blog.csdnimg.cn/20181226115450871)#include <string>

 3![](https://img-blog.csdnimg.cn/20181226115450871)#include <iostream>

 4![](https://img-blog.csdnimg.cn/20181226115450871)

 5![](https://img-blog.csdnimg.cn/20181226115450871)using std :: cout ;

 6![](https://img-blog.csdnimg.cn/20181226115450871)using std :: endl ;

 7![](https://img-blog.csdnimg.cn/20181226115450871)using std :: string ;

 8![](https://img-blog.csdnimg.cn/20181226115450871)using std :: map ;

 9![](https://img-blog.csdnimg.cn/20181226115450871)

10![](https://img-blog.csdnimg.cn/20181226115450871)int main()

11![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

12![](https://img-blog.csdnimg.cn/20181226115451144)    //定义map对象，当前没有任何元素

13![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char> m ;

14![](https://img-blog.csdnimg.cn/20181226115451144)    //插入元素，按键值的由小到大放入黑白树中

15![](https://img-blog.csdnimg.cn/20181226115451144)    m[25] = 'm' ;

16![](https://img-blog.csdnimg.cn/20181226115451144)    m[28] = 'k' ;

17![](https://img-blog.csdnimg.cn/20181226115451144)    m[10] = 'x' ;

18![](https://img-blog.csdnimg.cn/20181226115451144)    m[30] = 'a' ;

19![](https://img-blog.csdnimg.cn/20181226115451144)    //反向遍历元素

20![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char> :: reverse_iterator rit ;

21![](https://img-blog.csdnimg.cn/20181226115451144)    for( rit = m.rbegin() ; rit != m.rend() ; rit ++)

22![](https://img-blog.csdnimg.cn/20181226115451472)![](https://img-blog.csdnimg.cn/20181226115451487)![](https://img-blog.csdnimg.cn/20181226115451125){

23![](https://img-blog.csdnimg.cn/20181226115451144)        //输入键值与映照数据

24![](https://img-blog.csdnimg.cn/20181226115451144)        cout << (*rit).first << " : " << (*rit).second << endl ;

25![](https://img-blog.csdnimg.cn/20181226115451524)    }

26![](https://img-blog.csdnimg.cn/20181226115451144)    return 0 ;

27![](https://img-blog.csdnimg.cn/20181226115451631)}

28![](https://img-blog.csdnimg.cn/20181226115450871)

运行结果：

                  30 : a

                  28 : k

                  25 : m

                  10 : x
**4、元素的搜索**使用find()方法来搜索某个键值，如果搜索到了，则返回该键值所在的迭代器位置，否则，返回end()迭代器位置。由于map采用黑白树数据结构来实现，所以搜索速度是极快的。

       下面这个程序搜索键值为28的元素：

 1![](https://img-blog.csdnimg.cn/20181226115450871)#include <map>

 2![](https://img-blog.csdnimg.cn/20181226115450871)#include <string>

 3![](https://img-blog.csdnimg.cn/20181226115450871)#include <iostream>

 4![](https://img-blog.csdnimg.cn/20181226115450871)

 5![](https://img-blog.csdnimg.cn/20181226115450871)using std :: cout ;

 6![](https://img-blog.csdnimg.cn/20181226115450871)using std :: endl ;

 7![](https://img-blog.csdnimg.cn/20181226115450871)using std :: string ;

 8![](https://img-blog.csdnimg.cn/20181226115450871)using std :: map ;

 9![](https://img-blog.csdnimg.cn/20181226115450871)

10![](https://img-blog.csdnimg.cn/20181226115450871)int main()

11![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

12![](https://img-blog.csdnimg.cn/20181226115451144)    //定义map对象，当前没有任何元素

13![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char> m ;

14![](https://img-blog.csdnimg.cn/20181226115451144)    //插入元素，按键值的由小到大放入黑白树中

15![](https://img-blog.csdnimg.cn/20181226115451144)    m[25] = 'm' ;

16![](https://img-blog.csdnimg.cn/20181226115451144)    m[28] = 'k' ;

17![](https://img-blog.csdnimg.cn/20181226115451144)    m[10] = 'x' ;

18![](https://img-blog.csdnimg.cn/20181226115451144)    m[30] = 'a' ;

19![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char> :: iterator it ;

20![](https://img-blog.csdnimg.cn/20181226115451144)    it = m.find(28) ;

21![](https://img-blog.csdnimg.cn/20181226115451144)    if(it != m.end())  //搜索到该键值

22![](https://img-blog.csdnimg.cn/20181226115451144)            cout << (*it).first << " : " << ( *it ).second << endl ;

23![](https://img-blog.csdnimg.cn/20181226115451144)    else

24![](https://img-blog.csdnimg.cn/20181226115451144)            cout << "not found it" << endl ;

25![](https://img-blog.csdnimg.cn/20181226115451144)    return 0 ;

26![](https://img-blog.csdnimg.cn/20181226115451631)}

27![](https://img-blog.csdnimg.cn/20181226115450871)

**5、自定义比较函数**将元素插入到map中去的时候，map会根据设定的比较函数将该元素放到该放的节点上去。在定义map的时候，如果没有指定比较函数，那么采用默认的比较函数，即按键值由小到大的顺序插入元素。在很多情况下，需要自己编写比较函数。

        编写方法有两种。

        （1）如果元素不是结构体，那么，可以编写比较函数。下面这个程序编写的比较规则是要求按键值由大到小的顺序将元素插入到map中

 1![](https://img-blog.csdnimg.cn/20181226115450871)#include <map>

 2![](https://img-blog.csdnimg.cn/20181226115450871)#include <string>

 3![](https://img-blog.csdnimg.cn/20181226115450871)#include <iostream>

 4![](https://img-blog.csdnimg.cn/20181226115450871)

 5![](https://img-blog.csdnimg.cn/20181226115450871)using std :: cout ;

 6![](https://img-blog.csdnimg.cn/20181226115450871)using std :: endl ;

 7![](https://img-blog.csdnimg.cn/20181226115450871)using std :: string ;

 8![](https://img-blog.csdnimg.cn/20181226115450871)using std :: map ;

 9![](https://img-blog.csdnimg.cn/20181226115450871)

10![](https://img-blog.csdnimg.cn/20181226115450871)//自定义比较函数 myComp

11![](https://img-blog.csdnimg.cn/20181226115450871)struct myComp

12![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

13![](https://img-blog.csdnimg.cn/20181226115451144)    bool operator() (const int &a, const int &b)

14![](https://img-blog.csdnimg.cn/20181226115451472)![](https://img-blog.csdnimg.cn/20181226115451487)![](https://img-blog.csdnimg.cn/20181226115451125){

15![](https://img-blog.csdnimg.cn/20181226115451144)        if(a != b) return a > b ;

16![](https://img-blog.csdnimg.cn/20181226115451144)        else  return a > b ;

17![](https://img-blog.csdnimg.cn/20181226115451524)    }

18![](https://img-blog.csdnimg.cn/20181226115451631)} ;

19![](https://img-blog.csdnimg.cn/20181226115450871)

20![](https://img-blog.csdnimg.cn/20181226115450871)int main()

21![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

22![](https://img-blog.csdnimg.cn/20181226115451144)    //定义map对象，当前没有任何元素

23![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char> m ;

24![](https://img-blog.csdnimg.cn/20181226115451144)    //插入元素，按键值的由小到大放入黑白树中

25![](https://img-blog.csdnimg.cn/20181226115451144)    m[25] = 'm' ;

26![](https://img-blog.csdnimg.cn/20181226115451144)    m[28] = 'k' ;

27![](https://img-blog.csdnimg.cn/20181226115451144)    m[10] = 'x' ;

28![](https://img-blog.csdnimg.cn/20181226115451144)    m[30] = 'a' ;

29![](https://img-blog.csdnimg.cn/20181226115451144)    //使用前向迭代器中序遍历map

30![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char,myComp> :: iterator it ;

31![](https://img-blog.csdnimg.cn/20181226115451144)    for(it = m.begin() ; it != m.end() ; it ++)

32![](https://img-blog.csdnimg.cn/20181226115451144)            cout << (*it).first << " : " << (*it).second << endl ;

33![](https://img-blog.csdnimg.cn/20181226115451144)    return 0 ;

34![](https://img-blog.csdnimg.cn/20181226115451631)}

35![](https://img-blog.csdnimg.cn/20181226115450871)

运行结果：

                  30 ：a

                  28 ：k

                  25 ：m

                  10 ：x

       （2）如果元素是结构体，那么，可以直接把比较函数写在结构体内。下面的程序详细说明了如何操作：

 1![](https://img-blog.csdnimg.cn/20181226115450871)#include <map>

 2![](https://img-blog.csdnimg.cn/20181226115450871)#include <string>

 3![](https://img-blog.csdnimg.cn/20181226115450871)#include <iostream>

 4![](https://img-blog.csdnimg.cn/20181226115450871)

 5![](https://img-blog.csdnimg.cn/20181226115450871)using std :: cout ;

 6![](https://img-blog.csdnimg.cn/20181226115450871)using std :: endl ;

 7![](https://img-blog.csdnimg.cn/20181226115450871)using std :: string ;

 8![](https://img-blog.csdnimg.cn/20181226115450871)using std :: map ;

 9![](https://img-blog.csdnimg.cn/20181226115450871)

10![](https://img-blog.csdnimg.cn/20181226115450871)struct Info

11![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

12![](https://img-blog.csdnimg.cn/20181226115451144)    string name ;

13![](https://img-blog.csdnimg.cn/20181226115451144)    float score ;

14![](https://img-blog.csdnimg.cn/20181226115451144)    //重载 “<”操作符，自定义排列规则

15![](https://img-blog.csdnimg.cn/20181226115451144)    bool operator < (const Info &a) const

16![](https://img-blog.csdnimg.cn/20181226115451472)![](https://img-blog.csdnimg.cn/20181226115451487)![](https://img-blog.csdnimg.cn/20181226115451125){

17![](https://img-blog.csdnimg.cn/20181226115451144)        //按score由大到小排列。如果要由小到大排列，使用“>”号即可

18![](https://img-blog.csdnimg.cn/20181226115451144)        return a.score < score ;

19![](https://img-blog.csdnimg.cn/20181226115451524)    }

20![](https://img-blog.csdnimg.cn/20181226115451631)} ;

21![](https://img-blog.csdnimg.cn/20181226115450871)

22![](https://img-blog.csdnimg.cn/20181226115450871)int main()

23![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

24![](https://img-blog.csdnimg.cn/20181226115451144)    //定义map对象，当前没有任何元素

25![](https://img-blog.csdnimg.cn/20181226115451144)    map<Info, int> m ;

26![](https://img-blog.csdnimg.cn/20181226115451144)    //定义Info结构体变量

27![](https://img-blog.csdnimg.cn/20181226115451144)    Info info ;

28![](https://img-blog.csdnimg.cn/20181226115451144)    //插入元素，按键值的由小到大放入黑白树中

29![](https://img-blog.csdnimg.cn/20181226115451144)    info.name = "Jack" ;

30![](https://img-blog.csdnimg.cn/20181226115451144)    info.score = 60 ;

31![](https://img-blog.csdnimg.cn/20181226115451144)    m[info] = 25 ;

32![](https://img-blog.csdnimg.cn/20181226115451144)    info.name = "Bomi" ;

33![](https://img-blog.csdnimg.cn/20181226115451144)    info.score = 80 ;

34![](https://img-blog.csdnimg.cn/20181226115451144)    m[info] = 10 ;

35![](https://img-blog.csdnimg.cn/20181226115451144)    info.name = "Peti" ;

36![](https://img-blog.csdnimg.cn/20181226115451144)    info.score = 66.5 ;

37![](https://img-blog.csdnimg.cn/20181226115451144)    m[info] = 30 ;

38![](https://img-blog.csdnimg.cn/20181226115451144)    //使用前向迭代器中序遍历map

39![](https://img-blog.csdnimg.cn/20181226115451144)    map<Info,int> :: iterator it ;

40![](https://img-blog.csdnimg.cn/20181226115451144)    for(it = m.begin() ; it != m.end() ; it ++)

41![](https://img-blog.csdnimg.cn/20181226115451472)![](https://img-blog.csdnimg.cn/20181226115451487)![](https://img-blog.csdnimg.cn/20181226115451125){

42![](https://img-blog.csdnimg.cn/20181226115451144)            cout << (*it).second << " : " ;

43![](https://img-blog.csdnimg.cn/20181226115451144)            cout << ((*it).first).name << " : " << ((*it).first).score << endl ;

44![](https://img-blog.csdnimg.cn/20181226115451524)    }

45![](https://img-blog.csdnimg.cn/20181226115451144)    return 0 ;

46![](https://img-blog.csdnimg.cn/20181226115451631)}

47![](https://img-blog.csdnimg.cn/20181226115450871)

运行结果：

                  10 ：Bomi   80

                  30 ：Peti     66.5

                  25 ：Jack    60
**6、用map实现数字分离**

      对数字的各位进行分离，采用取余等数学方法是很耗时的。而把数字当成字符串，使用map的映照功能，很方便地实现了数字分离。下面这个程序将一个字符串中的字符当成数字，并将各位的数值相加，最后输出各位的和。

 1![](https://img-blog.csdnimg.cn/20181226115450871)#include <string>

 2![](https://img-blog.csdnimg.cn/20181226115450871)#include <map>

 3![](https://img-blog.csdnimg.cn/20181226115450871)#include <iostream>

 4![](https://img-blog.csdnimg.cn/20181226115450871)

 5![](https://img-blog.csdnimg.cn/20181226115450871)using std :: cout ;

 6![](https://img-blog.csdnimg.cn/20181226115450871)using std :: endl ;

 7![](https://img-blog.csdnimg.cn/20181226115450871)using std :: string ;

 8![](https://img-blog.csdnimg.cn/20181226115450871)using std :: map ;

 9![](https://img-blog.csdnimg.cn/20181226115450871)

10![](https://img-blog.csdnimg.cn/20181226115450871)int main()

11![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

12![](https://img-blog.csdnimg.cn/20181226115451144)    //定义map对象，当前没有任何元素

13![](https://img-blog.csdnimg.cn/20181226115451144)    map<char, int> m ;

14![](https://img-blog.csdnimg.cn/20181226115451144)

15![](https://img-blog.csdnimg.cn/20181226115451144)    //赋值：字符映射数字

16![](https://img-blog.csdnimg.cn/20181226115451144)    m['0'] = 0 ;

17![](https://img-blog.csdnimg.cn/20181226115451144)    m['1'] = 1 ;

18![](https://img-blog.csdnimg.cn/20181226115451144)    m['2'] = 2 ;

19![](https://img-blog.csdnimg.cn/20181226115451144)    m['3'] = 3 ;

20![](https://img-blog.csdnimg.cn/20181226115451144)    m['4'] = 4 ;

21![](https://img-blog.csdnimg.cn/20181226115451144)    m['5'] = 5 ;

22![](https://img-blog.csdnimg.cn/20181226115451144)    m['6'] = 6 ;

23![](https://img-blog.csdnimg.cn/20181226115451144)    m['7'] = 7 ;

24![](https://img-blog.csdnimg.cn/20181226115451144)    m['8'] = 8 ;

25![](https://img-blog.csdnimg.cn/20181226115451144)    m['9'] = 9 ;

26![](https://img-blog.csdnimg.cn/20181226115451472)![](https://img-blog.csdnimg.cn/20181226115451487)    /**//*上面的10条赋值语句可采用下面这个循环简化代码编写

27![](https://img-blog.csdnimg.cn/20181226115451144)    for(int j = 0 ; j < 10 ; j++)

28![](https://img-blog.csdnimg.cn/20181226115451144)    {

29![](https://img-blog.csdnimg.cn/20181226115451144)            m['0' + j] = j ;

30![](https://img-blog.csdnimg.cn/20181226115451144)    }

31![](https://img-blog.csdnimg.cn/20181226115451524)    */

32![](https://img-blog.csdnimg.cn/20181226115451144)    string sa, sb ;

33![](https://img-blog.csdnimg.cn/20181226115451144)    sa = "6234" ;

34![](https://img-blog.csdnimg.cn/20181226115451144)    int i ;

35![](https://img-blog.csdnimg.cn/20181226115451144)    int sum = 0 ;

36![](https://img-blog.csdnimg.cn/20181226115451144)    for ( i = 0 ; i < sa.length() ; i++ )

37![](https://img-blog.csdnimg.cn/20181226115451144)            sum += m[sa[i]] ;

38![](https://img-blog.csdnimg.cn/20181226115451144)    cout << "sum = " << sum << endl ;

39![](https://img-blog.csdnimg.cn/20181226115451144)    return 0 ;

40![](https://img-blog.csdnimg.cn/20181226115451631)}

41![](https://img-blog.csdnimg.cn/20181226115450871)

**7、数字映照字符的map写法**在很多情况下，需要实现将数字映射为相应的字符，看看下面的程序：

 1![](https://img-blog.csdnimg.cn/20181226115450871)#include <string>

 2![](https://img-blog.csdnimg.cn/20181226115450871)#include <map>

 3![](https://img-blog.csdnimg.cn/20181226115450871)#include <iostream>

 4![](https://img-blog.csdnimg.cn/20181226115450871)

 5![](https://img-blog.csdnimg.cn/20181226115450871)using std :: cout ;

 6![](https://img-blog.csdnimg.cn/20181226115450871)using std :: endl ;

 7![](https://img-blog.csdnimg.cn/20181226115450871)using std :: string ;

 8![](https://img-blog.csdnimg.cn/20181226115450871)using std :: map ;

 9![](https://img-blog.csdnimg.cn/20181226115450871)

10![](https://img-blog.csdnimg.cn/20181226115450871)int main()

11![](https://img-blog.csdnimg.cn/2018122611545197)![](https://img-blog.csdnimg.cn/20181226115451111)![](https://img-blog.csdnimg.cn/20181226115451125){

12![](https://img-blog.csdnimg.cn/20181226115451144)    //定义map对象，当前没有任何元素

13![](https://img-blog.csdnimg.cn/20181226115451144)    map<int, char> m ;

14![](https://img-blog.csdnimg.cn/20181226115451144)

15![](https://img-blog.csdnimg.cn/20181226115451144)    //赋值：字符映射数字

16![](https://img-blog.csdnimg.cn/20181226115451144)    m[0] = '0' ;

17![](https://img-blog.csdnimg.cn/20181226115451144)    m[1] = '1' ;

18![](https://img-blog.csdnimg.cn/20181226115451144)    m[2] = '2' ;

19![](https://img-blog.csdnimg.cn/20181226115451144)    m[3] = '3' ;

20![](https://img-blog.csdnimg.cn/20181226115451144)    m[4] = '4' ;

21![](https://img-blog.csdnimg.cn/20181226115451144)    m[5] = '5' ;

22![](https://img-blog.csdnimg.cn/20181226115451144)    m[6] = '6' ;

23![](https://img-blog.csdnimg.cn/20181226115451144)    m[7] = '7' ;

24![](https://img-blog.csdnimg.cn/20181226115451144)    m[8] = '8' ;

25![](https://img-blog.csdnimg.cn/20181226115451144)    m[9] = '9' ;

26![](https://img-blog.csdnimg.cn/20181226115451472)![](https://img-blog.csdnimg.cn/20181226115451487)    /**//*上面的10条赋值语句可采用下面这个循环简化代码编写

27![](https://img-blog.csdnimg.cn/20181226115451144)    for(int j = 0 ; j < 10 ; j++)

28![](https://img-blog.csdnimg.cn/20181226115451144)    {

29![](https://img-blog.csdnimg.cn/20181226115451144)            m[j] = '0' + j ;

30![](https://img-blog.csdnimg.cn/20181226115451144)    }

31![](https://img-blog.csdnimg.cn/20181226115451524)    */

32![](https://img-blog.csdnimg.cn/20181226115451144)    int n = 7 ;

33![](https://img-blog.csdnimg.cn/20181226115451144)    string s = "The number is " ;

34![](https://img-blog.csdnimg.cn/20181226115451144)    cout << s + m[n] << endl ;

35![](https://img-blog.csdnimg.cn/20181226115451144)    return 0 ;

36![](https://img-blog.csdnimg.cn/20181226115451631)}

37![](https://img-blog.csdnimg.cn/20181226115450871)

运行结果：

                  The number is 7

























准模板库就是类与函数模板的大集合。STL共有6种组件：容器，容器适配器，迭代器，算法，函数对象和函数适配器。

1、容器：

容器是用来存储和组织其他对象的对象。STL容器类的模板在标准头文件中定义。主要如下所示

![](http://hi.csdn.net/attachment/201108/24/0_1314184820YY5a.gif)

①序列容器

基本的序列容器是上面图中的前三类：

![](http://hi.csdn.net/attachment/201108/24/0_1314186467W5QH.gif)

关于三者的优缺点主要是：

A:vector<T>矢量容器：可以随机访问容器的内容，在序列末尾添加或删除对象，但是因为是从尾部删除，过程非常慢，因为必须移动插入或删除点后面的所有对象。

![](http://hi.csdn.net/attachment/201108/24/0_1314186799506c.gif)



矢量容器的操作：（自己以前有个表，贴出来大家看看）

![](http://hi.csdn.net/attachment/201108/24/0_13141871367aW5.gif)

其中的capacity表示容量，size是当前数据个数。矢量容器如果用户添加一个元素时容量已满，那么就增加当前容量的一半的内存，比如现在是500了，用户添加进第501个，那么他会再开拓250个，总共就750个了。所以矢量容器当你添加数据量很大的时候，需要注意这一点哦。。。



如果想用迭代器访问元素是比较简单的，使用迭代器输出元素的循环类似如下：



**[cpp]**[view plain](http://blog.csdn.net/timebomb/article/details/6716064#)[copy](http://blog.csdn.net/timebomb/article/details/6716064#)
- vector<int>::iterator表示矢量容器vector<int>的迭代器。。。  

**[cpp]**[view plain](http://blog.csdn.net/timebomb/article/details/6716064#)[copy](http://blog.csdn.net/timebomb/article/details/6716064#)
- for(vector<int>::iterator iter = number.begin(); iter<number.end(); iter++)//这里的iterator iter算是一个指针了  
-      cout << " " << *iter;  

当然也可以用我们自己的方法，但是感觉用上面的更好一些。





**[cpp]**[view plain](http://blog.csdn.net/timebomb/article/details/6716064#)[copy](http://blog.csdn.net/timebomb/article/details/6716064#)
- for(vector<int>::size_type i=0; i<number.size(); i++)  
-     cout << " " << number[i]  





排序矢量元素：

对矢量元素的排序可以使用<algorithm>头文件中定义的sort()函数模板来对一个矢量容器进行排序。但是有几点要求需要注意


- sort()函数模板用<运算符来排列元素的顺序，所以容器中对象必须可以进行<运算，如果是基本类型，可以直接调用sort()，如果是自定义对象，必须对<进行运算符重载
- 两个迭代器的指向必须是序列的第一个对象和最后一个对象的下一个位置。比如：sort(people.begin(), people.end());//这里两个参数就是迭代器的意思了



B:deque<T>容器：非常类似vector<T>，且支持相同的操作，但是它还可以在序列开头添加和删除。

![](http://hi.csdn.net/attachment/201108/24/0_1314186809iPmh.gif)



deque<T>双端队列容器与矢量容器基本类似，具有相同的函数成员，但是有点不同的是它支持从两端插入和删除数据，所以就有了两个函数：push_front和pop_front。并且有两个迭代器变量



**[cpp]**[view plain](http://blog.csdn.net/timebomb/article/details/6716064#)[copy](http://blog.csdn.net/timebomb/article/details/6716064#)
- <span style="font-size:18px;">#include <deque>  
- deque<int> data;//创建双端队列容器对象  
- deque<int>::iterator iter;//书序迭代器  
- deque<int>::reverse_iterator riter;//逆序迭代器。  
- //iter和riter是不同的类型</span>  





C:list<T>容器是双向链表，因此可以有效的在任何位置添加和删除。列表的缺点是不能随机访问内容，要想访问内容必须在列表的内部从头开始便利内容，或者从尾部开始。

![](http://hi.csdn.net/attachment/201108/24/0_13141868189Tjb.gif)





②关联容器

map<K, T>映射容器:K表示键，T表示对象，根据特定的键映射到对象，可以进行快速的检索。

有关它的创建以及查找的操作作如下总结



**[cpp]**[view plain](http://blog.csdn.net/timebomb/article/details/6716064#)[copy](http://blog.csdn.net/timebomb/article/details/6716064#)
- //创建映射容器  
- map<person, string> phonebook;  
- 
- //创建要存储的对象  
- pair<person, string> entry = pair<person, string>(person("mel", "Gibson"), "213 345 567");  
- 
- //插入对象  
- phonebook.insert(entry);//只要映射中没有相同的键，就可以插入entry  
- 
- //访问对象  
- string number = phonebook[person("mel", "Gibson")];//如果这个键不存在，会默认将这个键插入  
- 
- //如果不想在找不到的时候插入，可以先查找然后再检索  
- person key = person("mel", "Gibson");  
- map<person, string>::iterator iter = phonebook.find(key);//创建迭代器，就认为是指针就好了  
- 
- if(iter != phonebook.end())  
-     string  number = iter->second;  


![](http://hi.csdn.net/attachment/201108/25/0_1314242304rrS0.gif)







2、容器适配器：

容器适配器是包装了现有的STL容器类的模板类，提供了一个不同的、通常更有限制性的功能。具体如下所示

![](http://hi.csdn.net/attachment/201108/24/0_1314184993htb3.gif)



A:queue<T>队列容器：通过适配器实现先进先出的存储机制。我们只能向队列的末尾添加或从开头删除元素。push_back() pop_front()

代码：queue<string, list<string> > names;(这就是定义的一个适配器)是基于列表创建队列的。适配器模板的第二个类型形参指定要使用的底层序列容器，主要的操作如下

![](http://hi.csdn.net/attachment/201108/25/0_1314239716Pu0b.gif)

![](http://hi.csdn.net/attachment/201108/25/0_1314239724am2V.gif)

B:priority_queue<T>优先级队列容器：是一个队列，它的顶部总是具有最大或最高优先级。优先级队列容器与队列容器一个不同点是优先级队列容器不能访问队列后端的元素。

默认情况下，优先级队列适配器类使用的是矢量容器vector<T>，当然可以选择指定不同的序列容器作为基础，并选择一个备用函数对象来确定元素的优先级代码如下



**[cpp]**[view plain](http://blog.csdn.net/timebomb/article/details/6716064#)[copy](http://blog.csdn.net/timebomb/article/details/6716064#)
- priority_queue<int, deque<int>, greate<int>> numbers;  



C:stack<T>堆栈容器：其适配器模板在<stack>头文件中定义，默认情况下基于deque<T>容器实现向下推栈，即后进先出机制。只能访问最近刚刚进去的对象





**[cpp]**[view plain](http://blog.csdn.net/timebomb/article/details/6716064#)[copy](http://blog.csdn.net/timebomb/article/details/6716064#)
- <span style="font-size:18px;">//定义容器  
- stack<person> people;  
- //基于列表来定义堆栈  
- stack<string, list<string>> names;</span>  



基本操作如下：



![](http://hi.csdn.net/attachment/201108/25/0_1314240798j99W.gif)





3、迭代器：

具体它的意思还没怎么看明白，书上介绍迭代器的行为与指针类似，这里做个标记![奋斗](http://static.blog.csdn.net/xheditor/xheditor_emot/default/struggle.gif)，看看后面的例子再给出具体的解释

具体分为三个部分：输入流迭代器、插入迭代器和输出流迭代器。

![](http://hi.csdn.net/attachment/201108/24/0_1314185277vBvq.gif)、

![](http://hi.csdn.net/attachment/201108/24/0_1314185285BBW8.gif)

看这一章的内容看的我有点抑郁了都，摘段课本介绍的内容，还是可以帮助理解的

<iterator>头文件中定义了迭代器的几个模板：①流迭代器作为指向输入或输出流的指针，他们可以用来在流和任何使用迭代器或目的地之间传输数据。②插入迭代器可以将数据传输给一个基本序列容器。头文件中定义了两个流迭代器模板：istream_iterator<T>用于输入流，ostream_iterator<T>用于输出流。T表示从流中提取数据或写到流中的对象的类型。头文件还定义了三个插入模板：insert<T>, back_insert<T>和front_inset<T>。其中T也是指代序列容器中数据的类型。

输入流迭代器用下面的程序来说明下，可见具体注释



**[cpp]**[view plain](http://blog.csdn.net/timebomb/article/details/6716064#)[copy](http://blog.csdn.net/timebomb/article/details/6716064#)
- #include <iostream>    
- #include <vector>  
- #include <numeric>  
- #include <sstream>  
- 
- using namespace std;    
- 
- int main()  
- {  
-     //定义矢量容器  
-     vector<int> numbers;  
-     cout << "请输入整数值，以字母结束:";  
- 
-     //定义输入流迭代器。注意两个不同  
-     //1、numberInput(cin)是指定迭代器指向流cin  
-     //2、numbersEnd没有指定，是默认的，默认构造了一个end_of_stream的迭代器，它等价于调用end()  
-     istream_iterator<int> numbersInput(cin), numbersEnd;  
- 
-     //用户输入，直到输入的不是int类型或者终止时结束。   
-     while(numbersInput != numbersEnd)  
-         numbers.push_back(*numbersInput++);  
- 
-     cout << "打印输出：" << numbers.at(3) << endl;  
- 
- 
-     //如何指定输入流呢？  
- 
-     //确定字符串  
-     string data("2.1 3.6 36.5 26 34 25 2.9 63.8");  
- 
-     //指定data为输入流input。需要头文件<sstream>  
-     istringstream input(data);  
- 
-     //定义迭代器  
-     istream_iterator<double> begin(input), end;  
- 
-     //计算数值和。  
-     //acculumate为头文件<numeric>下定义的函数。  
-     //第一个参数是开始迭代器，第二个是终止迭代器(最后一个值的下一个)。第三个是和的初值，注意必须用0.0，用它确定数据类型是double  
-     cout << "打印数据的总和：" << accumulate(begin, end, 0.0) << endl;  
- }  

输出结果：

![](http://hi.csdn.net/attachment/201108/25/0_13142457813T0D.gif)

耽误时间太多。以后再写吧



4、算法：

算法是操作迭代器提供的一组对象的STL函数模板，对对象的一个操作，可以与前面的容器迭代器结合起来看。如下图介绍

![](http://hi.csdn.net/attachment/201108/24/0_1314185621ZBKE.gif)

5、函数对象：

函数对象是重载()运算符的类类型的对象。就是实现operator()()函数。

函数对象模板在<functional>头文件中定义，必要时我们也可以定义自己的函数对象。做个标记![奋斗](http://static.blog.csdn.net/xheditor/xheditor_emot/default/struggle.gif)，等有具体实例来进行进一步的解释。

6、函数适配器：

函数适配器是允许合并函数对象以产生一个更复杂的函数对象的函数模板。







Map是STL的一个关联容器，它提供一对一（其中第一个可以称为关键字，每个关键字只能在map中出现一次，第二个可能称为该关键字的值）的数据处理能力，由于这个特性，它完成有可能在我们处理一对一数据的时候，在编程上提供快速通道。这里说下map内部数据的组织，map内部自建一颗红黑树(一种非严格意义上的平衡二叉树)，这颗树具有对数据自动排序的功能，所以在map内部所有的数据都是有序的，后边我们会见识到有序的好处。

下面举例说明什么是一对一的数据映射。比如一个班级中，每个学生的学号跟他的姓名就存在着一一映射的关系，这个模型用map可能轻易描述，很明显学号用int描述，姓名用字符串描述(本篇文章中不用char *来描述字符串，而是采用STL中string来描述),下面给出map描述代码：

Map<int, string> mapStudent;

1.       map的构造函数

map共提供了6个构造函数，这块涉及到内存分配器这些东西，略过不表，在下面我们将接触到一些map的构造方法，这里要说下的就是，我们通常用如下方法构造一个map：

Map<int, string> mapStudent;

2.       数据的插入

在构造map容器后，我们就可以往里面插入数据了。这里讲三种插入数据的方法：

第一种：用insert函数插入pair数据，下面举例说明(以下代码虽然是随手写的，应该可以在VC和GCC下编译通过，大家可以运行下看什么效果，在VC下请加入这条语句，屏蔽4786警告 ＃pragma warning (disable:4786) )

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, “student_one”));

       mapStudent.insert(pair<int, string>(2, “student_two”));

       mapStudent.insert(pair<int, string>(3, “student_three”));

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<”   ”<<iter->second<<end;

}

}

第二种：用insert函数插入value_type数据，下面举例说明

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(map<int, string>::value_type (1, “student_one”));

       mapStudent.insert(map<int, string>::value_type (2, “student_two”));

       mapStudent.insert(map<int, string>::value_type (3, “student_three”));

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<”   ”<<iter->second<<end;

}

}

第三种：用数组方式插入数据，下面举例说明

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent[1] =  “student_one”;

       mapStudent[2] =  “student_two”;

       mapStudent[3] =  “student_three”;

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<”   ”<<iter->second<<end;

}

}

以上三种用法，虽然都可以实现数据的插入，但是它们是有区别的，当然了第一种和第二种在效果上是完成一样的，用insert函数插入数据，在数据的插入上涉及到集合的唯一性这个概念，即当map中有这个关键字时，insert操作是插入数据不了的，但是用数组方式就不同了，它可以覆盖以前该关键字对应的值，用程序说明

mapStudent.insert(map<int, string>::value_type (1, “student_one”));

mapStudent.insert(map<int, string>::value_type (1, “student_two”));

上面这两条语句执行后，map中1这个关键字对应的值是“student_one”，第二条语句并没有生效，那么这就涉及到我们怎么知道insert语句是否插入成功的问题了，可以用pair来获得是否插入成功，程序如下

Pair<map<int, string>::iterator, bool> Insert_Pair;

Insert_Pair = mapStudent.insert(map<int, string>::value_type (1, “student_one”));

我们通过pair的第二个变量来知道是否插入成功，它的第一个变量返回的是一个map的迭代器，如果插入成功的话Insert_Pair.second应该是true的，否则为false。

下面给出完成代码，演示插入成功与否问题

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

Pair<map<int, string>::iterator, bool> Insert_Pair;

       Insert_Pair ＝ mapStudent.insert(pair<int, string>(1, “student_one”));

       If(Insert_Pair.second == true)

       {

              Cout<<”Insert Successfully”<<endl;

       }

       Else

       {

              Cout<<”Insert Failure”<<endl;

       }

       Insert_Pair ＝ mapStudent.insert(pair<int, string>(1, “student_two”));

       If(Insert_Pair.second == true)

       {

              Cout<<”Insert Successfully”<<endl;

       }

       Else

       {

              Cout<<”Insert Failure”<<endl;

       }

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<”   ”<<iter->second<<end;

}

}

大家可以用如下程序，看下用数组插入在数据覆盖上的效果

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent[1] =  “student_one”;

       mapStudent[1] =  “student_two”;

       mapStudent[2] =  “student_three”;

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<”   ”<<iter->second<<end;

}

}

3.       map的大小

在往map里面插入了数据，我们怎么知道当前已经插入了多少数据呢，可以用size函数，用法如下：

Int nSize = mapStudent.size();

4.       数据的遍历

这里也提供三种方法，对map进行遍历

第一种：应用前向迭代器，上面举例程序中到处都是了，略过不表

第二种：应用反相迭代器，下面举例说明，要体会效果，请自个动手运行程序

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, “student_one”));

       mapStudent.insert(pair<int, string>(2, “student_two”));

       mapStudent.insert(pair<int, string>(3, “student_three”));

       map<int, string>::reverse_iterator  iter;

       for(iter = mapStudent.rbegin(); iter != mapStudent.rend(); iter++)

{

       Cout<<iter->first<<”   ”<<iter->second<<end;

}

}

第三种：用数组方式，程序说明如下

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, “student_one”));

       mapStudent.insert(pair<int, string>(2, “student_two”));

       mapStudent.insert(pair<int, string>(3, “student_three”));

       int nSize = mapStudent.size()

//此处有误，应该是 for(int nIndex = 1; nIndex <= nSize; nIndex++) 

//by rainfish

       for(int nIndex = 0; nIndex < nSize; nIndex++)

{

       Cout<<mapStudent[nIndex]<<end;

}

}

5.       数据的查找（包括判定这个关键字是否在map中出现）

在这里我们将体会，map在数据插入时保证有序的好处。

要判定一个数据（关键字）是否在map中出现的方法比较多，这里标题虽然是数据的查找，在这里将穿插着大量的map基本用法。

这里给出三种数据查找方法

第一种：用count函数来判定关键字是否出现，其缺点是无法定位数据出现位置,由于map的特性，一对一的映射关系，就决定了count函数的返回值只有两个，要么是0，要么是1，出现的情况，当然是返回1了

第二种：用find函数来定位数据出现位置，它返回的一个迭代器，当数据出现时，它返回数据所在位置的迭代器，如果map中没有要查找的数据，它返回的迭代器等于end函数返回的迭代器，程序说明

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, “student_one”));

       mapStudent.insert(pair<int, string>(2, “student_two”));

       mapStudent.insert(pair<int, string>(3, “student_three”));

       map<int, string>::iterator iter;

       iter = mapStudent.find(1);

if(iter != mapStudent.end())

{

       Cout<<”Find, the value is ”<<iter->second<<endl;

}

Else

{

       Cout<<”Do not Find”<<endl;

}

}

第三种：这个方法用来判定数据是否出现，是显得笨了点，但是，我打算在这里讲解

Lower_bound函数用法，这个函数用来返回要查找关键字的下界(是一个迭代器)

Upper_bound函数用法，这个函数用来返回要查找关键字的上界(是一个迭代器)

例如：map中已经插入了1，2，3，4的话，如果lower_bound(2)的话，返回的2，而upper-bound（2）的话，返回的就是3

Equal_range函数返回一个pair，pair里面第一个变量是Lower_bound返回的迭代器，pair里面第二个迭代器是Upper_bound返回的迭代器，如果这两个迭代器相等的话，则说明map中不出现这个关键字，程序说明

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent[1] =  “student_one”;

       mapStudent[3] =  “student_three”;

       mapStudent[5] =  “student_five”;

       map<int, string>::iterator  iter;

iter = mapStudent.lower_bound(2);

{

       //返回的是下界3的迭代器

       Cout<<iter->second<<endl;

}

iter = mapStudent.lower_bound(3);

{

       //返回的是下界3的迭代器

       Cout<<iter->second<<endl;

}



iter = mapStudent.upper_bound(2);

{

       //返回的是上界3的迭代器

       Cout<<iter->second<<endl;

}

iter = mapStudent.upper_bound(3);

{

       //返回的是上界5的迭代器

       Cout<<iter->second<<endl;

}



Pair<map<int, string>::iterator, map<int, string>::iterator> mapPair;

mapPair = mapStudent.equal_range(2);

if(mapPair.first == mapPair.second)

       {

       cout<<”Do not Find”<<endl;

}

Else

{

Cout<<”Find”<<endl;

}

mapPair = mapStudent.equal_range(3);

if(mapPair.first == mapPair.second)

       {

       cout<<”Do not Find”<<endl;

}

Else

{

Cout<<”Find”<<endl;

}

}

6.       数据的清空与判空

清空map中的数据可以用clear()函数，判定map中是否有数据可以用empty()函数，它返回true则说明是空map

7.       数据的删除

这里要用到erase函数，它有三个重载了的函数，下面在例子中详细说明它们的用法

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, “student_one”));

       mapStudent.insert(pair<int, string>(2, “student_two”));

       mapStudent.insert(pair<int, string>(3, “student_three”));



//如果你要演示输出效果，请选择以下的一种，你看到的效果会比较好

       //如果要删除1,用迭代器删除

       map<int, string>::iterator iter;

       iter = mapStudent.find(1);

       mapStudent.erase(iter);



       //如果要删除1，用关键字删除

       Int n = mapStudent.erase(1);//如果删除了会返回1，否则返回0



       //用迭代器，成片的删除

       //一下代码把整个map清空

       mapStudent.earse(mapStudent.begin(), mapStudent.end());

       //成片删除要注意的是，也是STL的特性，删除区间是一个前闭后开的集合



       //自个加上遍历代码，打印输出吧

}

8.       其他一些函数用法

这里有swap,key_comp,value_comp,get_allocator等函数，感觉到这些函数在编程用的不是很多，略过不表，有兴趣的话可以自个研究

9.       排序

这里要讲的是一点比较高深的用法了,排序问题，STL中默认是采用小于号来排序的，以上代码在排序上是不存在任何问题的，因为上面的关键字是int型，它本身支持小于号运算，在一些特殊情况，比如关键字是一个结构体，涉及到排序就会出现问题，因为它没有小于号操作，insert等函数在编译的时候过不去，下面给出两个方法解决这个问题

第一种：小于号重载，程序举例

#include <map>

#include <string>

Using namespace std;

Typedef struct tagStudentInfo

{

       Int      nID;

       String   strName;

}StudentInfo, *PStudentInfo;  //学生信息



Int main()

{

    int nSize;

       //用学生信息映射分数

       map<StudentInfo, int>mapStudent;

    map<StudentInfo, int>::iterator iter;

       StudentInfo studentInfo;

       studentInfo.nID = 1;

       studentInfo.strName = “student_one”;

       mapStudent.insert(pair<StudentInfo, int>(studentInfo, 90));

       studentInfo.nID = 2;

       studentInfo.strName = “student_two”;

mapStudent.insert(pair<StudentInfo, int>(studentInfo, 80));



for (iter=mapStudent.begin(); iter!=mapStudent.end(); iter++)

    cout<<iter->first.nID<<endl<<iter->first.strName<<endl<<iter->second<<endl;



}

以上程序是无法编译通过的，只要重载小于号，就OK了，如下：

Typedef struct tagStudentInfo

{

       Int      nID;

       String   strName;

       Bool operator < (tagStudentInfo const& _A) const

       {

              //这个函数指定排序策略，按nID排序，如果nID相等的话，按strName排序

              If(nID < _A.nID)  return true;

              If(nID == _A.nID) return strName.compare(_A.strName) < 0;

              Return false;

       }

}StudentInfo, *PStudentInfo;  //学生信息

第二种：仿函数的应用，这个时候结构体中没有直接的小于号重载，程序说明

#include <map>

#include <string>

Using namespace std;

Typedef struct tagStudentInfo

{

       Int      nID;

       String   strName;

}StudentInfo, *PStudentInfo;  //学生信息



Classs sort

{

       Public:

       Bool operator() (StudentInfo const &_A, StudentInfo const &_B) const

       {

              If(_A.nID < _B.nID) return true;

              If(_A.nID == _B.nID) return _A.strName.compare(_B.strName) < 0;

              Return false;

       }

};



Int main()

{

       //用学生信息映射分数

       Map<StudentInfo, int, sort>mapStudent;

       StudentInfo studentInfo;

       studentInfo.nID = 1;

       studentInfo.strName = “student_one”;

       mapStudent.insert(pair<StudentInfo, int>(studentInfo, 90));

       studentInfo.nID = 2;

       studentInfo.strName = “student_two”;

mapStudent.insert(pair<StudentInfo, int>(studentInfo, 80));

}

10.   另外

由于STL是一个统一的整体，map的很多用法都和STL中其它的东西结合在一起，比如在排序上，这里默认用的是小于号，即less<>，如果要从大到小排序呢，这里涉及到的东西很多，在此无法一一加以说明。

还要说明的是，map中由于它内部有序，由红黑树保证，因此很多函数执行的时间复杂度都是log2N的，如果用map函数可以实现的功能，而STL  Algorithm也可以完成该功能，建议用map自带函数，效率高一些。



