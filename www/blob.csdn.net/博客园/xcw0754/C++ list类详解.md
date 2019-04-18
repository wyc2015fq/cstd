# C++ list类详解 - xcw0754 - 博客园
# [C++ list类详解](https://www.cnblogs.com/xcw0754/p/4227240.html)
### 转自：http://blog.csdn.net/whz_zb/article/details/6831817
### 双向循环链表list
        list是双向循环链表，，每一个元素都知道前面一个元素和后面一个元素。在STL中，list和vector一样，是两个常被使用的容器。和vector不一样的是，list不支持对元素的任意存取。list中提供的成员函数与vector类似，不过list提供对表首元素的操作push_front、pop_front，这是vector不具备的。和vector另一点不同的是，list的迭代器不会存在失效的情况，他不像vector会保留备份空间，在超过容量额度时重新全部分配内存，导致迭代器失效；list没有备份空间的概念，出入一个元素就申请一个元素的空间，所以它的迭代器不会失效。还是举[《C++之vector》](http://our2848884.blog.163.com/blog/static/1468548342011669513996/)中的例子：
int data[6]={3,5,7,9,2,4};  list<int> lidata(data, data+6);  lidata.push_back(6);  ...
list初始化时，申请的空间大小为6，存放下了data中的6个元素，当向lidata插入第7个元素“6”时，list申请新的节点单元，插入到list链表中，数据存放结构如图1所示：
![](http://hi.csdn.net/attachment/201110/1/0_131745864571pS.gif)
图1 list的存储结构
        list每次增加一个元素，不存在重新申请内存的情况，它的成本是恒定的。而vector每当增加关键元素的时候，都需要重新申请新的更大的内存空间，会调用元素的自身的复制构造函数，存在构造成本。在销毁旧内存的时候，会调用析构函数，存在析构成本。所以在存储复杂类型和大量元素的情况下，list比vector更有优势！ 
    List是一个双向链表,双链表既可以向前又向后链接他的元素。
    List将元素按顺序储存在链表中. 与 向量(vector)相比, 它允许快速的插入和删除，但是随机访问却比较慢。
> 
> 
assign() 给list赋值 
> 
back() 返回最后一个元素 
> 
begin() 返回指向第一个元素的迭代器 
> 
clear() 删除所有元素 
> 
empty() 如果list是空的则返回true 
> 
end() 返回末尾的迭代器 
> 
erase() 删除一个元素 
> 
front() 返回第一个元素 
> 
get_allocator() 返回list的配置器 
> 
insert() 插入一个元素到list中 
> 
max_size() 返回list能容纳的最大元素数量 
> 
merge() 合并两个list 
> 
pop_back() 删除最后一个元素 
> 
pop_front() 删除第一个元素 
> 
push_back() 在list的末尾添加一个元素 
> 
push_front() 在list的头部添加一个元素 
> 
rbegin() 返回指向第一个元素的逆向迭代器 
> 
remove() 从list删除元素 
> 
remove_if() 按指定条件删除元素 
> 
rend() 指向list末尾的逆向迭代器 
> 
resize() 改变list的大小 
> 
reverse() 把list的元素倒转 
> 
size() 返回list中的元素个数 
> 
sort() 给list排序 
> 
splice() 合并两个list 
> 
> 
swap() 交换两个list 
> 
> 
unique() 删除list中重复的元素

 List使用实例1
#include <iostream>
#include <list>
#include <numeric>
#include <algorithm>
using namespace std;
//创建一个list容器的实例LISTINT
typedef list<int> LISTINT;
//创建一个list容器的实例LISTCHAR
typedef list<char> LISTCHAR;
int main(int argc, char *argv[])
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
return 0;
}
 List使用实例2
list: Linked list of variables, struct or objects. Insert/remove anywhere.
Two examples are given:
- The first STL example is for data type int
- The second for a list of class instances.
They are used to show a simple example and a more complex real world application.
1. Lets start with a simple example of a program using STL for a linked list:  
// Simple example uses type int
#include <iostream>
#include <list>
using namespace std;
int main()
{
   list<int> L;
   L.push_back(0);              // Insert a new element at the end
   L.push_front(0);             // Insert a new element at the beginning
   L.insert(++L.begin(),2);     // Insert "2" before position of first argument
                                            // (Place before second argument)
   L.push_back(5);
   L.push_back(6);
   list<int>::iterator i;
   for(i=L.begin(); i != L.end(); ++i) cout << *i << " ";
   cout << endl;
   return 0;
}
Compile: g++ example1.cpp
Run: ./a.out
Output: 0 2 0 5 6
2. The STL tutorials and texts seem to give simple examples which do not apply to the real world. The following example is for a doubly linked list. Since we are using a class and we are not using defined built-in C++ types we have included the following:
- To make this example more complete, a copy constructor has been included although the compiler will generate a member-wise one automatically if needed. This has the same functionality as the assignment operator (=).
- The assignment (=) operator must be specified so that sort routines can assign a new order to the members of the list.
- The "less than" (<) operator must be specified so that sort routines can determine if one class instance is "less than" another.
- The "equals to" (==) operator must be specified so that sort routines can determine if one class instance is "equals to" another. 
// Standard Template Library example using a class.
#include <iostream>
#include <list>
using namespace std;
// The List STL template requires overloading operators =, == and <.
//vc2005调试没有错（红色字体部分可去掉）、可用vc6.0却报错了“'operator <<' is ambiguous”（vc6.0的加上红色字体部分）
class AAA;
ostream &operator<<(ostream &output, const AAA &aaa);
class AAA
{
   friend ostream &operator<<(ostream &, const AAA &);
   public:
      int x;
      int y;
      float z;
      AAA();
      AAA(const AAA &);
      ~AAA(){};
      AAA &operator=(const AAA &rhs);
      int operator==(const AAA &rhs) const;
      int operator<(const AAA &rhs) const;
};
AAA::AAA()   // Constructor
{
   x = 0;
   y = 0;
   z = 0;
}
AAA::AAA(const AAA ©in)   // Copy constructor to handle pass by value.
{                             
   x = copyin.x;
   y = copyin.y;
   z = copyin.z;
}
ostream &operator<<(ostream &output, const AAA &aaa)
{
   output << aaa.x << ' ' << aaa.y << ' ' << aaa.z << endl;
   return output;
}
AAA& AAA::operator=(const AAA &rhs)
{
   this->x = rhs.x;
   this->y = rhs.y;
   this->z = rhs.z;
   return *this;
}
int AAA::operator==(const AAA &rhs) const
{
   if( this->x != rhs.x) return 0;
   if( this->y != rhs.y) return 0;
   if( this->z != rhs.z) return 0;
   return 1;
}
// This function is required for built-in STL list functions like sort
int AAA::operator<(const AAA &rhs) const
{
   if( this->x == rhs.x && this->y == rhs.y && this->z < rhs.z) return 1;
   if( this->x == rhs.x && this->y < rhs.y) return 1;
   if( this->x < rhs.x ) return 1;
   return 0;
}
int main()
{
   list<AAA> L;
   AAA Ablob ;
   Ablob.x=7;
   Ablob.y=2;
   Ablob.z=4.2355;
   L.push_back(Ablob);  // Insert a new element at the end
   Ablob.x=5;
   L.push_back(Ablob);  // Object passed by value. Uses default member-wise
                        // copy constructor
   Ablob.z=3.2355;
   L.push_back(Ablob); 
   Ablob.x=3;
   Ablob.y=7;
   Ablob.z=7.2355;
   L.push_back(Ablob); 
   list<AAA>::iterator i;
   for(i=L.begin(); i != L.end(); ++i) cout << (*i).x << " "; // print member
   cout << endl;      
   for(i=L.begin(); i != L.end(); ++i) cout << *i << " "; // print with overloaded operator
   cout << endl;
   cout << "Sorted: " << endl;
   L.sort();
   for(i=L.begin(); i != L.end(); ++i) cout << *i << " "; // print with overloaded operator
   cout << endl;
   return 0;
}
Output:
7 5 5 3 
7 2 4.2355
 5 2 4.2355
 5 2 3.2355
 3 7 7.2355
Sorted:
3 7 7.2355
 5 2 3.2355
 5 2 4.2355
 7 2 4.2355
STL中list的使用：
STL中的list就是一双向链表，可高效地进行插入删除元素。现总结一下它的操作。
文中所用到两个list对象c1,c2分别有元素c1(10,20,30)  c2(40,50,60)。还有一个list<int>::iterator citer用来指向c1或c2元素。
list对象的声明构造()：
A.      list<int>c0;                 //空链表
B.      list<int>c1(3);             //建一个含三个默认值是0的元素的链表
C.      list<int>c2(5,2);            //建一个含五个元素的链表，值都是２
D.      list<int>c4(c2);             //建一个c2的copy链表
E.       list<int>c5(c1.begin(),c1.end());  
//c5含c1一个区域的元素[_First, _Last)。
1.       assign()分配值，有两个重载：
c1.assign(++c2.begin(), c2.end()) //c1现在为(50,60)。
c1.assing(7,4)  //c1中现在为7个4,c1(4,4,4,4,4,4,4)。
2.       back()返回最后一元素的引用：
int i=c1.back();  //i=30
const int i=c2.back();  //i=60且不可修改
3.       begin()返回第一个元素的指针(iterator)
citer=c1.begin();    // *citer=10
list<int>::const_iterator cciter=c1.begin(); //*cciter=10且为const。
4.       clear()删除所有元素
c1.clear();   //c1为空  c1.size为０；
5.       empty()判断是否链表为空
bool B=c1.empty(); //若c1为空B=true；否则B=false;
6.       end()返回最后一个元素的下一位置的指针(list为空时end()=begin())
citer=c1.end(); //*(--citer)=30;
同begin()返回一个常指针，不能修改其中元素。
7.       erase()删除一个元素或一个区域的元素(两个重载)
c1.erase(c1.begin()); // c1现为(20,30);
c1.erase(++c1.begin(),c1.end()); //　c1现为(10);
8.       front() 返回第一个元素的引用：
int i=c1.front(); //i=10;
const int i=c1.front(); //i=10且不可修改。
9.       insert()在指定位置插入一个或多个元素(三个重载)：
c1.insert(++c1.begin(),100);   //c1(10,100,20,30)
c1.insert(c1.begin(),2,200);  //c1(200,200,20,30);
c1.insert(++c1.begin(),c2.begin(),--c2.end());
 //c1(10,40,50,20,30);
10.    max_size()返回链表最大可能长度(size_type就是int型)：
list<int>::size_type i=c1.max_size();  //i=1073741823
11.    merge()合并两个链表并使之默认升序(也可改)：
c2.merge(c1);   //c1现为空;c2现为c2(10,20,30,40,50,60)
c2.merge(c1,greater<int>()); //同上，但c2现为降序
12.    pop_back()删除链表尾的一个元素
c1.pop_back()  //c1(10,20);
13.    pop_front()删除链表头的一元素
c1.pop_front()　//c1(20,30)
14.    push_back()增加一元素到链表尾
c1.push_back(100) //c1(10,20,30,100)
15.    push_front()增加一元素到链表头
c1.push_front(100) //c1(100,10,20,30)
16.    rbegin()返回链表最后一元素的后向指针(reverse_iterator or const)
list<int>::reverse_iterator riter=c1.rbegin(); //*riter=30
17.    rend()返回链表第一元素的下一位置的后向指针
list<int>::reverse_iterator riter=c1.rend(); // *(--riter)=10
18.    remove()删除链表中匹配值的元素(匹配元素全部删除)
c1.remove(10);     //c1(20,30)
19.    remove_if()删除条件满足的元素(会遍历一遍链表)
c1.remove_if( is_odd<int> () ); //c1(10,20,30)　
//is_odd自己写(表奇数)　
20.    resize()重新定义链表长度(两重载)：
c1.resize(4)  //c1(10,20,30,0)用默认值填补
c1.resize(4,100) //c1(10,20,30,100)用指定值填补
21.    reverse()反转链表:
c1.reverse(); //c1(30,20,10)
22.    size()返回链表中元素个数
list<int>::size_type i=c1.size();  //i=3
23.    sort()对链表排序，默认升序(可自定义)
c1.sort();  //c1(10,20,30)
c1.sort(great<int>()); //c1(30,20,10)
24.    splice()对两个链表进行结合(三个重载)
c1.splice(++c1.begin(),c2); 
//c1(10,40,50,60,20,30) c2为空 全合并
c1.splice(++c1.begin(),c2,++c2.begin());
//c1(10,50,20,30) ; c2(40,60)　指定元素合并
c1.splice(++c1.begin(),c2,++c2.begin(),c2.end());
//c1(10,50,60,20,30); c2(40) 指定范围合并
25.    swap()交换两个链表(两个重载)
c1.swap(c2);  //c1(40,50,60);
swap(c1,c2);  //c1(40,50,60)
26.    unique()删除相邻重复元素(断言已经排序，因为它不会删除不相邻的相同元素)
c1.unique(); 
//假设c1开始(-10,10,10,20,20,-10)则之后为c1(-10,10,20,-10)
c1.unique(mypred); //自定义谓词
list 的使用
在使用list必须包括头文件#include <list>，
1)、如何定义一个list对象
#include <list>
int main (void)
{
 list<char > cList; //声明了list<char>模板类 的一个实例
}
2)、使用list的成员函数push_back和push_front插入一个元素到list中
cList. push_back(‘a’); //把一个对象放到一个list的后面
cList. push_front (‘b’); //把一个对象放到一个list的前面
3)、使用list的成员函数empty()判断list是否为空
if (cList.empty())
{
 printf(“this list is empty”);
}
4)、用list< char >::iterator得到指向list的指针
list< char>::iterator charIterator;
for(cIterator = cList.Begin();cIterator != cList.end();cIterator++)
{
 printf(“%c”, *cIterator);
} //输出list中的所有对象
说明：cList.Begin()和cList.end()函数返回指向list< char >::iterator的指针，由于list采用链表结构，因此它不支持随机存取，因此不能用cList.begin()+3来指向list中的第四个对象，vector和deque支持随机存取。
5）、用STL的通用算法count()来统计list中的元素个数
int cNum;
char ch = ’b’;
cNum = count(cList.Begin(), cList.end(), ch); //统计list中的字符b的个数
说明：在使用count()函数之前必须加入#include <algorithm> 
6）、用STL的通用算法count_if ()来统计list中的元素个数
const char c(‘c’);
class IsC
{
public:
 bool operator() ( char& ch )
 {
  return ch== c;
 }
};
int numC;
numC = count_if (cList.begin(), cList.end(),IsC());//统计c的数量；
说明：count_if() 带一个函数对象的参数,函数对象是一个至少带有一个operator()方法的类函数对象被约定为STL算法调用operator时返回true或false。它们根据这个来判定这个函数。举个例子会 说的更清楚些。count_if()通过传递一个函数对象来作出比count()更加复杂的评估以确定一个对象是否应该被记数。
7）、使用STL通用算法find()在list中查找对象
list<char >::iterator FindIterator;
FindIterator = find(cList.begin(), cList.end(), ‘c’);
If (FindIterator == cList.end())
{
 printf(“not find the char ‘c’!”);
}
else
{
 printf(“%c”, * FindIterator);
}
说明：如果没有找到指定的对象，就会返回cList.end()的值，找到了就返回一个指向对象iterator的指针。
8）、使用STL通用算法find_if()在list中查找对象
const char c(‘c’);
class c
{
public:
 bool operator() ( char& ch )
 {
  return ch== c;
 }
};
list<char>::iterator FindIterator
FindIterator = find_if (cList.begin(), cList.end(),IsC());//查找字符串c；
说明：如果没有找到指定的对象，就会返回cList.end()的值，找到了就返回一个指向对象iterator的指针。
9）、使用list的成员函数sort()排序
cList.sort();
10)、使用list的成员函数insert插入一个对象到list中
cList.insert(cLiset.end, ‘c’); ///在list末尾插入字符‘c’
char ch[3] ={‘a’, ‘b’, ‘c’};
cList.insert(cList.end, &ch[0], & ch[3] ); //插入三个字符到list中
说明：insert()函数把一个或多个元素插入到指出的iterator位置。元素将出现在 iterator指出的位置以前。
11)、如何在list中删除元素
cList.pop_front(); //删除第一个元素
cList.pop_back(); //删除最后一个元素
cList. Erase(cList.begin()); //使用iterator删除第一个元素；
cList. Erase(cList.begin(), cList.End()); //使用iterator删除所有元素；
cList.remove(‘c’); //使用remove函数删除指定的对象；
list<char>::iterator newEnd;
//删除所有的’c’ ,并返回指向新的list的结尾的iterator
newEnd = cList.remove(cList.begin(), cList.end(), ‘c’);
