# STL容器（一）——List - bigfacesafdasgfewgf - CSDN博客





2014年10月19日 07:45:37[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：464








# STL容器之List

# 1. List的概念

    List是STL中的一个重要的容器，它是一个双向循环链表，也就是每个元素都知道其前面的元素和后面的元素，但是其又不支持元素的任意存取，只能逐个的push或者pop。list的优点是快速的插入和删除元素。list和vector的区别就在于两点：

- list可以对首元素操作，使用push_front和pop_front，而vector只能尾元素操作；
- list的迭代器不会出现失效的情况，list没有备份空间的概念，出入一个元素就申请一个元素的空间，迭代器不会失效。而vector会保留备份空间，在超过容量额度时重新全部分配内存，导致迭代器失效。

**2. 构建list**


    list可以在定义对象的时候就初始化，方式如下：





```cpp
int data[6]={3,5,7,9,2,4};  
list<int> lidata(data, data+6);  
lidata.push_back(6);  </span>
```



可以发现，初始化的时候可以确定list中有多少个元素，而且在初始化之后，我们也可以继续push进去元素。




其余的构建list方法也有：





```cpp
list<int> L1;  //空链表
list<int> L2(3);  //建一个含三个默认值是0的元素的链表
list<int> L3(5,2); //建一个含五个元素的链表，值都是2
list<int> L4(L2);  //链表L4中的元素复制了L2
list<int> L5(L4.begin(), L4.end());  //把L4中的[L4.begin(), L4.end())赋值给L5，用于区间赋值
```




关于list的用法，可以通过下面的实例进行了解：


```cpp
//假如也有c1(10, 20, 30), c2(40, 50, 60)

//assign()  分配值，有两个重载
c1.assign(++c2.begin(), c2.end()) //c1现在为(50,60)
c1.assing(7,4)  //c1中现在为7个4,c1(4,4,4,4,4,4,4)

//back()  返回最后一元素的引用
int i=c1.back();  //i=30

//begin()  返回第一个元素的指针(iterator)    和back略有区别
citer=c1.begin();    // *citer=10

//end()  返回最后一个元素的下一位置的指针(list为空时end()=begin())
citer=c1.end();   //*(--citer)=30;

//front()  返回第一个元素的引用
int i=c1.front();  //i=10;

//clear()  删除所有元素
c1.clear();   //c1为空  c1.size为0

//empty()  判断是否链表为空
bool B=c1.empty();  //若c1为空B=true  否则B=false

//erase()  删除一个元素或一个区域的元素(两个重载)
c1.erase(c1.begin());  // c1现为(20,30);
c1.erase(++c1.begin(), c1.end());   //　c1现为(10);

//insert()  在指定位置插入一个或多个元素(三个重载)
c1.insert(++c1.begin(),100);   //c1(10,100,20,30)
c1.insert(c1.begin(),2,200);  //c1(200,200,20,30);
c1.insert(++c1.begin(),c2.begin(),--c2.end());  //c1(10,40,50,20,30);

//max_size()  返回list最大可能长度(size_type就是int型)
list<int>::size_type i=c1.max_size();   //i=1073741823

// merge()  合并list链表并使之默认升序(也可改)
c2.merge(c1);   //c1现为空;c2现为c2(10,20,30,40,50,60)
c2.merge(c1,greater<int>());  //同上，但c2现为降序

//pop_back()  删除链表尾的一个元素
c1.pop_back()  //c1(10,20);

// pop_front()  删除链表头的一元素
c1.pop_front()　//c1(20,30)

// push_back()  增加一元素到链表尾
c1.push_back(100) //c1(10,20,30,100)

// push_front()  增加一元素到链表头
c1.push_front(100) //c1(100,10,20,30)

//remove()  删除链表中匹配值的元素(匹配元素全部删除)
c1.remove(10);     //c1(20,30)

//remove_if()  删除条件满足的元素(会遍历一遍链表)
c1.remove_if( is_odd<int> () ); //c1(10,20,30)  其中if谓词条件需要用户自定义

//resize()  重新定义链表长度(两重载)：
c1.resize(4)   //c1(10,20,30,0)用默认值填补
c1.resize(4,100)  //c1(10,20,30,100)用指定值填补

//reverse()  反转链表:
c1.reverse();  //c1(30,20,10)

//reverse()  反转链表:
c1.reverse();  //c1(30,20,10)

//size()  返回链表中元素个数
list<int>::size_type i=c1.size();  //i=3

//sort()  对链表排序，默认升序(可自定义)
c1.sort();  //c1(10,20,30)
c1.sort(great<int>()); //c1(30,20,10)

//splice()  对两个链表进行结合(三个重载)
c1.splice(++c1.begin(),c2);  //c1(10,40,50,60,20,30) c2为空 全合并
c1.splice(++c1.begin(),c2,++c2.begin());  //c1(10,50,20,30) ; c2(40,60)　指定元素合并
c1.splice(++c1.begin(),c2,++c2.begin(),c2.end());  //c1(10,50,60,20,30); c2(40) 指定范围合并

//swap()  交换两个链表(两个重载)
c1.swap(c2);  //c1(40,50,60)
swap(c1,c2);  //c1(40,50,60)

//unique()  删除相邻重复元素(它不会删除不相邻的相同元素)
c1.unique();  //假设c1开始(-10,10,10,20,20,-10), 则之后为c1(-10,10,20,-10)
c1.unique(mypred); //自定义谓词
```












































