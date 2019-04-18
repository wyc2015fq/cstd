# C++ vector类详解 - xcw0754 - 博客园
# [C++ vector类详解](https://www.cnblogs.com/xcw0754/p/4227238.html)
### 转自http://blog.csdn.net/whz_zb/article/details/6827999
### vector简介
       vector是STL中最常见的容器，它是一种顺序容器，支持随机访问。vector是一块连续分配的内存，从数据安排的角度来讲，和数组极其相似，不同的地方就是：数组是静态分配空间，一旦分配了空间的大小，就不可再改变了；而vector是动态分配空间，随着元素的不断插入，它会按照自身的一套机制不断扩充自身的容量。
        vector的扩充机制：按照容器现在容量的一倍进行增长。vector容器分配的是一块连续的内存空间，每次容器的增长，并不是在原有连续的内存空间后再进行简单的叠加，而是重新申请一块更大的新内存，并把现有容器中的元素逐个复制过去，然后销毁旧的内存。这时原有指向旧内存空间的迭代器已经失效，所以当操作容器时，迭代器要及时更新。
### vector的数据结构
        vector数据结构，采用的是连续的线性空间，属于线性存储。他采用3个迭代器_First、_Last、_End来指向分配来的线性空间的不同范围，下面是声明3个迭代器变量的源代码。
template<class _Ty, class _A= allocator< _Ty> > class vector{     ...     protected:     iterator _First, _Last, _End; };
_First指向使用空间的头部，_Last指向使用空间大小（size）的尾部，_End指向使用空间容量（capacity）的尾部。例如：
int data[6]={3,5,7,9,2,4}; vector<int> vdata(data, data+6); vdata.push_back(6); ...
vector初始化时，申请的空间大小为6，存放下了data中的6个元素。当向vdata中插入第7个元素“6”时，vector利用自己的扩充机制重新申请空间，数据存放结构如图1所示：
![](http://hi.csdn.net/attachment/201110/1/0_13174454572eN4.gif)
图1 扩充后的vector内存结构
        简单描述一下。当插入第7个元素“6”时，vector发现自己的空间不够了，于是申请新的大小为12的内存空间（自增一倍），并将前面已有数据复制到新空间的前部，然后插入第7个元素。此时_Last迭代器指向最后一个有效元素，而_End迭代器指向vector的最后有效空间位置。我们利用vector的成员函数size可以获得当前vector的大小，此时为7；利用capacity成员函数获取当前vector的容量，此时为12。
vector容器类型
   vector容器是一个模板类，可以存放任何类型的对象（但必须是同一类对象）。vector对象可以在运行时高效地添加元素，并且vector中元素是连续存储的。
vector的构造
函数原型：
template<typename T>
   explicit vector();                                 // 默认构造函数，vector对象为空
   explicit vector(size_type n, const T& v = T());    // 创建有n个元素的vector对象
   vector(const vector& x);
   vector(const_iterator first, const_iterator last);
注：vector容器内存放的所有对象都是经过初始化的。如果没有指定存储对象的初始值，那么对于内置类型将用0初始化，对于类类型将调用其默认构造函数进行初始化（如果有其它构造函数而没有默认构造函数，那么此时必须提供元素初始值才能放入容器中）。
举例：
vector<string> v1;         // 创建空容器，其对象类型为string类
vector<string> v2(10);     // 创建有10个具有初始值（即空串）的string类对象的容器
vector<string> v3(5, "hello"); // 创建有5个值为“hello”的string类对象的容器
vector<string> v4(v3.begin(), v3.end());  // v4是与v3相同的容器（完全复制）
**vector的操作（下面的函数都是成员函数）**
bool empty() const;                    // 如果为容器为空，返回true；否则返回false
size_type max_size() const;            // 返回容器能容纳的最大元素个数
size_type size() const;                // 返回容器中元素个数  
size_type capacity() const;            // 容器能够存储的元素个数，有：capacity() >= size()  
void reserve(size_type n);             // 确保capacity() >= n
void resize(size_type n, T x = T());   // 确保返回后，有：size() == n；如果之前size()<n，那么用元素x的值补全。
reference front();                     // 返回容器中第一个元素的引用（容器必须非空）
const_reference front() const;                   
reference back();                      // 返回容器中最后一个元素的引用（容器必须非空）
const_reference back() const;
reference operator[](size_type pos);   // 返回下标为pos的元素的引用（下标从0开始；如果下标不正确，则属于未定义行为。
const_reference operator[](size_type pos) const; 
reference at(size_type pos);           // 返回下标为pos的元素的引用；如果下标不正确，则抛出异常out_of_range
const_reference at(size_type pos) const;
void push_back(const T& x);            // 向容器末尾添加一个元素          
void pop_back();                       // 弹出容器中最后一个元素（容器必须非空）
// 注：下面的插入和删除操作将发生元素的移动（为了保持连续存储的性质），所以之前的迭代器可能失效
iterator insert(iterator it, const T& x = T());        // 在插入点元素之前插入元素（或者说在插入点插入元素）
void insert(iterator it, size_type n, const T& x);     // 注意迭代器可能不再有效（可能重新分配空间）
void insert(iterator it, const_iterator first, const_iterator last);
iterator erase(iterator it);           // 删除指定元素，并返回删除元素后一个元素的位置（如果无元素，返回end()）
iterator erase(iterator first, iterator last); // 注意：删除元素后，删除点之后的元素对应的迭代器不再有效。
void clear() const;                    // 清空容器，相当于调用erase( begin(), end())
void assign(size_type n, const T& x = T());   // 赋值，用指定元素序列替换容器内所有元素
void assign(const_iterator first, const_iterator last);
const_iterator begin() const;          // 迭代序列
iterator begin();
const_iterator end() const;
iterator end();
const_reverse_iterator rbegin() const;
reverse_iterator rbegin();
const_reverse_iterator rend() const; 
reverse_iterator rend();
vector对象的比较（非成员函数）
   针对vector对象的比较有六个比较运算符：operator==、operator!=、operator<、operator<=、operator>、operator>=。
   其中，对于operator==和operator!=，如果vector对象拥有相同的元素个数，并且对应位置的元素全部相等，则两个vector对象相等；否则不等。
   对于operator<、operator<=、operator>、operator>=，采用字典排序策略比较。
注：其实只需要实现operator==和operator!=就可以了，其它可以根据这两个实现。因为，operator!= (lhs, rhs) 就是 !(lhs == rhs)，operator<=(lhs, rhs) 就是 !(rhs < lhs)，operator>(lhs, rhs) 就是 (rhs < lhs)，operator>=（lhs, rhs) 就是 !(lhs, rhs)。
vector类的迭代器
   vector类的迭代器除了支持通用的前缀自增运算符外，还支持算术运算：it + n、it - n、it2 - it1。注意it2 - it1返回值为difference_type（signed类型）。
   注意，任何改变容器大小的操作都可能造成以前的迭代器失效。
应用示例
1.vector 的数据的存入和输出：
#include<stdio.h>
#include<vector>
#include <iostream>
using namespace std;
void main()
{
   int i = 0;
    vector<int> v;
    for( i = 0; i < 10; i++ )
   {
             v.push_back( i );//把元素一个一个存入到vector中
   }
   for( i = 0; i < v.size(); i++ )//v.size() 表示vector存入元素的个数
   {
          cout << v[ i ] << "  "; //把每个元素显示出来
   }
   cout << endl;
} 
注：你也可以用v.begin()和v.end() 来得到vector开始的和结束的元素地址的指针位置。你也可以这样做：
vector<int>::iterator iter;
for( iter = v.begin(); iter != v.end(); iter++ )
{
    cout << *iter << endl;
}
2. 对于二维vector的定义。
1)定义一个10个vector元素，并对每个vector符值1-10。
#include<stdio.h>
#include<vector>
#include <iostream>
using namespace std;
void main()
{
 int i = 0, j = 0;
//定义一个二维的动态数组，有10行，每一行是一个用一个vector存储这一行的数据。
所以每一行的长度是可以变化的。之所以用到vector<int>(0)是对vector初始化，否则不能对vector存入元素。
 vector< vector<int> > Array( 10, vector<int>(0) ); 
for( j = 0; j < 10; j++ )
 {
  for ( i = 0; i < 9; i++ )
  {
   Array[ j ].push_back( i );
  }
 }
 for( j = 0; j < 10; j++ )
 {
  for( i = 0; i < Array[ j ].size(); i++ )
  {
   cout << Array[ j ][ i ] << "  ";
  }
  cout<< endl;
 }
}
2)定义一个行列都是变化的数组。
#include<stdio.h>
#include<vector>
#include <iostream>
using namespace std;
void main()
{
 int i = 0, j = 0;
 vector< vector<int> > Array;
 vector< int > line;
 for( j = 0; j < 10; j++ )
 {
  Array.push_back( line );//要对每一个vector初始化，否则不能存入元素。
  for ( i = 0; i < 9; i++ )
  {
   Array[ j ].push_back( i );
  }
 }
 for( j = 0; j < 10; j++ )
 {
  for( i = 0; i < Array[ j ].size(); i++ )
  {
   cout << Array[ j ][ i ] << "  ";
  }
  cout<< endl;
 }
}
3.综合一点的例子。
```
#include <iostream>
#include <cassert>
#include <vector>
using namespace std;
int main()
{
    vector<int> v(5, 1);
    vector<int> v2(v.begin(), v.end());
    
    assert(v == v2);
    
    cout<<"> Before operation"<<endl;
    for(vector<int>::const_iterator it = v.begin(); it < v.end(); ++it)
        cout<<*it<<endl;
    //v.insert(v.begin() + 3, 4, 2);
    v.insert(v.begin() + 3, 1, 2);
    v.insert(v.begin() + 4, 1, 3);
```
`    v.insert(v.begin() + 5, 1, 4);`
    v.insert(v.begin() + 6, 1, 5);
```
cout<<"> After insert"<<endl;
    for(vector<int>::size_type i = 0; i < v.size(); ++i)
        cout<<v[i]<<endl;
    
    vector<int>::iterator it1 = v.erase(v.begin() + 3, v.begin() + 6);//删除的是第4，5，6三个元素
    assert(*it1 == 5);
    cout<<"> After erase"<<endl;
    for(vector<int>::size_type j = 0; j != v.size(); ++j)
        cout<<v[j]<<endl;
    
    assert(v.begin() + v.size() == v.end());
    assert(v.end() - v.size() == v.begin());
    assert(v.begin() - v.end() == -vector<string>::difference_type(v.size()));
    
    return 0;
}
```
程序说明：上面第3个程序中用了三个循环输出容器中的元素，每个循环的遍历方式是不一样的。特别需要说明的是，第二个循环在条件判断中使用了size() 函数，而不是在循环之前先保存在变量中再使用。之所以这样做，有两个原因：其一，如果将来在修改程序时，在循环中修改了容器元素个数，这个循环仍然能很好 地工作，而如果先保存size()函数值就不正确了；其二，由于这些小函数（其实现只需要一条返回语句）基本上都被声明为inline，所以不需要考虑效率问题。
附录1
## vector是一个标准库中的容器，使用时需要包含#include <vector>头文件。
vector是一个类模板而不是一种数据类型，对它的定义，需要指定类型。
vector(向量)是 C++中的一种数据结构,确切的说是一个类.它相当于一个动态的数组,
当程序员无法知道自己需要的数组的规模多大时,用其来解决问题可以达到最大节约空间的目的.
**1、vector对象的定义和初始化**
vector类定义了好几种构造函数，并且与string类型相类似，如下所示：
|**操作调用方式**|**操作说明**|
|----|----|
|**Vector<T> v1;**|Vector保存类型为T的对象。默认构造函数v1为空|
|**Vector<T> v2(v1);**|V2是v1的一个副本|
|**Vector<T> v3(n , i);**|V3包含n个值为i的元素|
|**Vector<T> v4(n);**|V4含有值初始化的元素的n个副本|
注：对C++的容器来说，动态添加元素的效率，要比直接静态初始化元素的效率高
例1.1 :声明一个int向量以替代一维的数组:vector <int> a;(等于声明了一个int数组a[],大小没有指定,可以动态的向里面添加删除)。
例1.2: 用vector代替二维数组.其实只要声明一个一维数组向量即可,而一个数组的名字其实代表的是它的首地址,所以只要声明一个地址的向量即可,即:vector <int *> a.同理想用向量代替三维数组也是一样,vector <int**>a;再往上面依此类推.
备注:
在用vector的过程中我碰到了一个问题,特此列出讨论:
                     1)
                     vector <int > a;
                     int b = 5;
                     a.push_back(b);
                     此时若对b另外赋值时不会影响a[0]的值
                     2)
                    vector <int*> a;
                      int *b;
                      b= new int[4];
                      b[0]=0;
                      b[1]=1;
                      b[2]=2;
                      a.push_back(b);
                      delete b;          //释放b的地址空间
                      for(int i=0 ; i <3 ; i++)
                      {
                            cout<<a[0][i]<<endl;
                      }
                      此时输出的值并不是一开始b数组初始化的值,而是一些无法预计的值.
                     分析:根据1) 2)的结果,可以想到,在1)中,   往a向量中压入的是b的值,即a[0]=b,此时a[0]和b是存储在两个不同的地址中的.因此改变b的值不会影响a[0];而在2)中,因为是把一个地址(指针)压入向量a,即a[0]=b,因此释放了b的地址也就释放了a[0]的地址,因此a[0]数组中存放的数值也就不得而知了.       
**2、vector对象的操作**
vector标准库提供了许多类似于string对象的操作，如下所示是一部分：
|**操作调用方式**|**操作说明**|
|----|----|
|**v.empty()**|判断v是否为空|
|**v.size()**|返回v中元素的个数|
|**v.push_back(t)**|向v的末尾添加一个元素|
|**V[n]**|返回v中位置为n的元素|
|**V1 = v2**|把v1中元素替换为v2中元素副本|
|**V1==v2**|判断是否相等|
|**!=, <, <=, >, >=**|直接用于vector对象的相互比较|
1.push_back   在数组的最后添加一个数据
2.pop_back    去掉数组的最后一个数据 
3.at                得到编号位置的数据
4.begin           得到数组头的指针
5.end             得到数组的最后一个单元+1的指针
6．front        得到数组头的引用
7.back            得到数组的最后一个单元的引用
8.max_size     得到vector最大可以是多大
9.capacity       当前vector分配的大小
10.size           当前使用数据的大小
11.resize         改变当前使用数据的大小，如果它比当前使用的大，者填充默认值
12.reserve      改变当前vecotr所分配空间的大小
13.erase         删除指针指向的数据项
14.clear          清空当前的vector
15.rbegin        将vector反转后的开始指针返回(其实就是原来的end-1)
16.rend          将vector反转构的结束指针返回(其实就是原来的begin-1)
17.empty        判断vector是否为空
18.swap         与另一个vector交换数据
注，以下是一些需要注意的地方
Ø vector和string一样，长度、下标等类型是size_type，但是vector获取size_type时，需要指定类型，如vector<int>::size_type这样的方式
Ø vector的下标操作，例如v[i]，只能用于操作已经存在的元素，可以进行覆盖、获取等，但是不能通过v[i++]这种方式来给一个vector容器添加元素，该功能需要用push_back操作完成，下标不具备该功能
Ø C++程序员习惯优先使用！=而不是<来编写循环判断条件
附录2
## [C++ vector 排序](http://www.blue-dev.cn/?p=336)
C++中当 vector 中的数据类型为基本类型时我们调用std::sort函数很容易实现 vector中数据成员的升序和降序排序，然而当vector中的数据类型为自定义结构体类型时，我们该怎样实现升序与降序排列呢？有两种方法，下面的例子能很好的说明： 方法1：
我们直接来看代码吧，比较简单，容易理解：
#include “stdafx.h”
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;
struct AssessTypeInfo
{
unsigned int m_uiType; //类型ID
char m_szName[64]; //类型名称
unsigned int m_uiTotal; //总分数
bool operator < (const AssessTypeInfo& rhs ) const //升序排序时必须写的函数
{
return m_uiType < rhs.m_uiType;
}
bool operator > (const AssessTypeInfo& rhs ) const //降序排序时必须写的函数
{
return m_uiType > rhs.m_uiType;
}
}
int main()
{
vector<AssessTypeInfo > ctn ;
AssessTypeInfo a1;
a1.m_uiType=1;
AssessTypeInfo a2;
a2.m_uiType=2;
AssessTypeInfo a3;
a3.m_uiType=3;
ctn.push_back(a1);
ctn.push_back(a2);
ctn.push_back(a3);
//升序排序
sort(ctn.begin(), ctn.end(),less<AssessTypeInfo>()) ; //或者sort(ctn.begin(), ctn.end()) 默认情况为升序
for ( int i=0; i<3; i++ )
printf(”%d\n”,ctn[i].m_uiType);
//降序排序
sort(ctn.begin(), ctn.end(),greater<AssessTypeInfo>()) ;
for ( int i=0; i<3; i++ )
printf(”%d\n”,ctn[i].m_uiType);
return 0 ;
}
以上方法就可以实现升序排序，输出结果为 1 2 3
降序排序结果3 2 1。
方法2 ： 不修改结构体或类的定义部分，我们用函数对象来实现：
#include “stdafx.h”
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;
struct AssessTypeInfo
{
unsigned int m_uiType; //类型ID
char m_szName[64]; //类型名称
unsigned int m_uiTotal; //总分数
};
bool lessmark(const AssessTypeInfo& s1,const AssessTypeInfo& s2)
{
return s1.m_uiType < s2.m_uiType;
}
bool greatermark(const AssessTypeInfo& s1,const AssessTypeInfo& s2)
{
return s1.m_uiType > s2.m_uiType;
}
int main()
{
vector<AssessTypeInfo > ctn ;
AssessTypeInfo a1;
a1.m_uiType=1;
AssessTypeInfo a2;
a2.m_uiType=2;
AssessTypeInfo a3;
a3.m_uiType=3;
ctn.push_back(a1);
ctn.push_back(a2);
ctn.push_back(a3);
sort(ctn.begin(), ctn.end(),lessmark) ; //升序排序
for ( int i=0; i<3; i++ )
printf(”%d\n”,ctn[i].m_uiType);
sort(ctn.begin(), ctn.end(),greatermark) ; //降序排序
return 0 ;
}
以上方法就可以实现升序排序，输出结果为 1 2 3
降序排序结果3 2 1。
方法2是一种比较简单的方法。
以上两种方法您可根据您自己的需求选择，并且以上两种方法在VC++6.0环境下编译通过，也是自己在实践过程中的总结，如有不妥的地方，欢迎您指出，至于为什么这样使用，请参考 stl算法中sort 部分。
(6)C++ Vector排序
- vector< int > vi ;   
- vi.push_back(1);  
- vi.push_back(3);  
- vi.push_back(0);  
- sort(vi.begin() , vi.end()); /// /小到大  
- reverse(vi.begin(),vi.end()) /// 从大道小 
(7)顺序访问
- vector < int > vi ;   
- for( int i = 0 ; i < 10 ; i ++)  
- {  
- vector.push_back(i);  
- }   
- for(int i = 0 ; i < 10 ; i ++) /// 第一种调用方法  
- {  
- cout <<vector[i] <<" " ;   
- }  
- for(vector<int>::iterator it = vi.begin() ; 
- it !=vi.end() ; it++) ///第二种调用方法  
- {  
- cout << *it << " " ;  
- } 
(8)寻找
- vector < int > vi ;   
- for( int i = 0 ; i < 10 ; i ++)  
- {  
- vector.push_back(i);  
- }   
- vector < int >::interator it = find(vi.begin() , vi.end,3) ;  
- cout << *it << endl ; ///返回容器内找到值的位置。 
(9)使用数组对C++ Vector进行初始化
- int i[10] ={1,2,3,4,5,6,7,78,8} ;  
- ///第一种   
- vector<int> vi(i+1,i+3); ///从第2个元素到第三个元素  
- for(vector <int>::interator it = vi.begin() ; 
- it != vi.end() ; it++)  
- {  
- cout << *it <<" " ;   
- } 
(10) 结构体类型
- struct temp  
- {  
- public :  
- string str ;   
- public :  
- int id ;  
- }tmp  
- int main()  
- {  
- vector <temp> t ;   
- temp w1 ;   
- w1.str = "Hellowor" ;  
- w1.id = 1 ;   
- t.push_back(t1);  
- cout << w1.str << "," <<w1.id <<endl ;   
- return 0 ;   
- } 
- 

