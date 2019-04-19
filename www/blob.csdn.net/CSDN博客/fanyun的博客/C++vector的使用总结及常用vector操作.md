# C++vector的使用总结及常用vector操作 - fanyun的博客 - CSDN博客
2017年02月26日 21:11:48[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：61722
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**一、C++ vector类为内置数组提供了一种替代表示，与string类一样 vector 类是随标准 C++引入的标准库的一部分，使用时需包含头文件：**
#include <vector>
**二、C++ vector类有两种使用方式：**
**第一种：STL方式**
vector< string > text;
1. 我们向 vector 中插入元素，而不再是索引元素，以及向元素赋值，例如 push_back()操作，就是在 vector 的后面插入一个元素下面的 while 循环从标准输入读入一个字符串序列并每次将一个字符串插入到 vector 中  
string word; 
while ( cin >> word ) { 
text.push_back( word ); 
// ... 
}
虽然我们仍可以用下标操作符来迭代访问元素  
cout << "words read are: \n"; 
for ( int ix = 0; ix < text.size(); ++ix ) 
      cout << text[ ix ] << ' '; 
cout << endl; 
但是 更典型的做法是使用vector 操作集中的begin()和 end()所返回的迭代器 iterator  
对 ：
cout << "words read are: \n"; 
for ( vector<string>::iterator it = text.begin(); 
    it != text.end(); ++it ) 
           cout << *it<< ' '; 
cout << endl 
iterator 是标准库中的类，它具有指针的功能 
*it; 
对迭代器解引用，并访问其指向的实际对象  
++it;
向前移动迭代器 it 使其指向下一个元素  
2. 注意 不要混用这两种习惯用法， 例如，下面的定义  
vector< int > ivec; 
定义了一个空vector 再写这样的语句  
ivec[ 0 ] = 1024; 
就是错误的 ，因为 ivec 还没有第一个元素，我们只能索引 vector 中已经存在的元素 size()操作返回 vector 包含的元素的个数。
3. 类似地 当我们用一个给定的大小定义一个vector 时，例如  ：
vector<int> ia( 10 ); 
任何一个插入操作都将增加vector 的大小，而不是覆盖掉某个现有的元素，这看起来好像是很显然的，但是下面的错误在初学者中并不少见：
const int size = 7; 
int ia[ size ] = { 0, 1, 1, 2, 3, 5, 8 }; 
vector< int > ivec( size ); 
for ( int ix = 0; ix < size; ++ix ) 
    ivec.push_back( ia[ ix ]); 
程序结束时ivec 包含 14 个元素, ia 的元素从第八个元素开始插入。
**第二种：类数组使用**
1. 定义一个已知长度的 vector ：
vector< int > ivec( 10 );  //类似数组定义int ia[10 ];
可以通过ivec[索引号] 来访问元素
使用 if ( ivec.empty() ) 判断是否是空，ivec.size()判断元素个数。
2. vector的元素被初始化为与其类型相关的缺省值：算术和指针类型的缺省值是 0，对于class 类型，缺省值可通过调用这类的缺省构造函数获得，我们还可以为每个元素提供一个显式的初始值来完成初始化，例如  
vector< int > ivec( 10, -1 ); 
定义了 ivec 它包含十个int型的元素每个元素都被初始化为-1 
对于内置数组我们可以显式地把数组的元素初始化为一组常量值，例如： 
int ia[ 6 ] = { -2, -1, 0, 1, 2, 1024 };
我们不能用同样的方法显式地初始化 vector ，但是可以将 vector 初始化为一个已有数组的全部或一部分，只需指定希望被用来初始化 vector 的数组的开始地址以及数组最末元的下一位置来实现，例如：  
// 把 ia 的 6 个元素拷贝到 ivec 中 
vector< int > ivec( ia, ia+6 );  
被传递给ivec 的两个指针标记了用来初始化对象的值的范围，第二个指针总是指向要拷贝的末元素的下一位置，标记出来的元素范围也可以是数组的一个子集，例如 :
// 拷贝 3 个元素 ia[2],ia[3], ia[4] 
vector< int > ivec( &ia[ 2 ], &ia[ 5 ] );
3. 与内置数组不同 vector 可以被另一个 vector 初始化或被赋给另一个 vector 例如  
vector< string > svec; 
void init_and_assign() 
{ 
    // 用另一个 vector 初始化一个 vector 
    vector< string > user_names( svec ); 
    // ... 
    // 把一个 vector 拷贝给另一个 vector 
    svec = user_names; 
}
** 三、vector使用总结：**
1.vector的初始化：可以有五种方式,举例说明如下：
（1） vector<int> a(10); //定义了10个整型元素的向量（尖括号中为元素类型名，它可以是任何合法的数据类型），但没有给出初值，其值是不确定的。
（2）vector<int>a(10,1); //定义了10个整型元素的向量,且给出每个元素的初值为1
（3）vector<int>a(b); //用b向量来创建a向量，整体复制性赋值
（4）vector<int>a(b.begin(),b.begin+3); //定义了a值为b中第0个到第2个（共3个）元素
（5）intb[7]={1,2,3,4,5,9,8};vector<int> a(b,b+7); //从数组中获得初值
2.vector对象的几个重要操作，举例说明如下：
（1）a.assign(b.begin(), b.begin()+3);//b为向量，将b的0~2个元素构成的向量赋给a
（2）a.assign(4,2);//是a只含4个元素，且每个元素为2
（3）a.back();//返回a的最后一个元素
（4）a.front();//返回a的第一个元素
（5）a[i]; //返回a的第i个元素，当且仅当a[i]存在2013-12-07
（6）a.clear();//清空a中的元素
（7）a.empty();//判断a是否为空，空则返回ture,不空则返回false
（8）a.pop_back();//删除a向量的最后一个元素
（9）a.erase(a.begin()+1,a.begin()+3);//删除a中第1个（从第0个算起）到第2个元素，也就是说删除的元素从a.begin()+1算起（包括它）一直到a.begin()+3（不包括它）
（10）a.push_back(5);//在a的最后一个向量后插入一个元素，其值为5
（11）a.insert(a.begin()+1,5);//在a的第1个元素（从第0个算起）的位置插入数值5，如a为1,2,3,4，插入元素后为1,5,2,3,4
（12）a.insert(a.begin()+1,3,5);//在a的第1个元素（从第0个算起）的位置插入3个数，其值都为5
（13）a.insert(a.begin()+1,b+3,b+6);//b为数组，在a的第1个元素（从第0个算起）的位置插入b的第3个元素到第5个元素（不包括b+6），如b为1,2,3,4,5,9,8，插入元素后为1,4,5,9,2,3,4,5,9,8
（14）a.size();//返回a中元素的个数；
（15）a.capacity();//返回a在内存中总共可以容纳的元素个数
（16）a.rezize(10);//将a的现有元素个数调至10个，多则删，少则补，其值随机
（17）a.rezize(10,2);//将a的现有元素个数调至10个，多则删，少则补，其值为2
（18）a.reserve(100);//将a的容量（capacity）扩充至100，也就是说现在测试a.capacity();的时候返回值是100.这种操作只有在需要给a添加大量数据的时候才 显得有意义，因为这将避免内存多次容量扩充操作（当a的容量不足时电脑会自动扩容，当然这必然降低性能） 
（19）a.swap(b);//b为向量，将a中的元素和b中的元素进行整体性交换
（20）a==b; //b为向量，向量的比较操作还有!=,>=,<=,>,<
3.顺序访问vector的几种方式，举例说明如下：
1、向向量a中添加元素
```cpp
vector<int> a;
for(int i=0;i<10;i++)
a.push_back(i);
```
2、也可以从数组中选择元素向向量中添加
```cpp
int a[6]={1,2,3,4,5,6};
vector<int> b；
for(int i=1;i<=4;i++)
b.push_back(a[i]);
```
3、也可以从现有向量中选择元素向向量中添加
```cpp
int a[6]={1,2,3,4,5,6};
vector<int> b;
vector<int> c(a,a+4);
for(vector<int>::iterator it=c.begin();it<c.end();it++)
b.push_back(*it);
```
4、也可以从文件中读取元素向向量中添加
```cpp
ifstream in("data.txt");
vector<int> a;
for(int i; in>>i)
a.push_back(i);
```
5、易犯错误
```cpp
vector<int> a;
for(int i=0;i<10;i++)
a[i]=i;
//这种做法以及类似的做法都是错误的。下标只能用于获取已存在的元素，而现在的a[i]还是空的对象
```
（2）从向量中读取元素
1、通过下标方式读取
```cpp
int a[6]={1,2,3,4,5,6};
vector<int> b(a,a+4);
for(int i=0;i<=b.size()-1;i++)
cout<<b[i]<<" ";
```
2、通过遍历器方式读取
```cpp
int a[6]={1,2,3,4,5,6};
vector<int> b(a,a+4);
for(vector<int>::iterator it=b.begin();it!=b.end();it++)
cout<<*it<<" ";
```
四、几种重要的算法，使用时需要包含头文件：
#include<algorithm>
（1）sort(a.begin(),a.end()); //对a中的从a.begin()（包括它）到a.end()（不包括它）的元素进行从小到大排列
（2）reverse(a.begin(),a.end()); //对a中的从a.begin()（包括它）到a.end()（不包括它）的元素倒置，但不排列，如a中元素为1,3,2,4,倒置后为4,2,3,1
（3）copy(a.begin(),a.end(),b.begin()+1); //把a中的从a.begin()（包括它）到a.end()（不包括它）的元素复制到b中，从b.begin()+1的位置（包括它）开始复制，覆盖掉原有元素
（4）find(a.begin(),a.end(),10); //在a中的从a.begin()（包括它）到a.end()（不包括它）的元素中查找10，若存在返回其在向量中的位置
五、其它使用举例：
（1）把一个vector中元素追加到另外一个容器vector，示例程序如下：
#include <vector>
#include <iostream>
template <typename type> void printvector(std::vector<type> const &v, std::ostream &os = std::cout)
{
    for (typename std::vector<type>::size_type sz = 0; sz != v.size(); ++sz)
        os << v[sz] << " ";
    os << std::endl;
}
int main()
{
    int a[5] = {1, 2, 3, 4, 5};
    std::vector<int> v1(a, a + 3), v2(a + 3, a + 5);
    printvector(v1); // 结果是1 2 3
    printvector(v2); // 结果是4 5
    v1.insert(v1.end(), v2.begin(), v2.end()); // 把v2加到v1末尾
    printvector(v1); // 结果是1 2 3 4 5
    return 0;
}
 （2）类和结构体存vector入实例:由于vector只允许一个占位，所以才将struct塞进vector，以弥补vector的不足。
```cpp
#include "stdafx.h"
#include <vector>
#include <string>
using namespace std;
class AClass
{
public:
    int num;
    string name;
};
struct AStruct 
{
    int num;
    string name;
};
void TestStruct()
{
 //类的使用
    AClass Ac;
    vector<AClass> vc;
    Ac.num=10;
    Ac.name="name";
    vc.push_back(Ac);
    AClass d;  
    for (vector<AClass>::iterator it=vc.begin();it<vc.end();++it)  
    {  
        d=*it;  
        cout<<d.num<<endl; 
    } 
 //结构体的使用
    AStruct As;
    vector<AStruct> vs;
    As.num=10;
    As.name="name";
    vs.push_back(As);
    AStruct ds;  
    for (vector<AStruct>::iterator it=vs.begin();it<vs.end();++it)  
    {  
        ds=*it;  
        cout<<ds.num<<endl; 
    } 
}
void TestPoint()
{
 //类的使用
    AClass *Ac=new AClass;
    vector<AClass *> vc;
    Ac->num=10;
    Ac->name="name";
    vc.push_back(Ac);
    AClass *d;  
    for (vector<AClass*>::iterator it=vc.begin();it<vc.end();++it)  
    {  
        d=*it;  
        cout<<d->num<<endl; 
    } 
}
int _tmain(int argc, _TCHAR* argv[])
{
    TestStruct();
    TestPoint();
    int n;
    cin>>n;
    return 0;
}
```
（3）C++中将两个vector中的值整合到另一个vector中，vecB和vecA中有相同的struct个数，现在想将每个vecA中的每个a的值传给vecC中c1，每个vecB中的每个b的值传给vecC中c2，也就是将两个容器中的内容整合到新的容器C中
```cpp
struct A
{
int a;
};
vector<A> vecA;
struct B
{
int b;
};
vector<B> vecB;
struct C
{
int c1;
int c2;
};
vector<C> vecC;
```
容器C和A、B中元素类型不同，迭代器类型就不同，所以不能用容器算法，使用迭代器遍历赋值，合并代码如下：第一种方法：
```cpp
for(vecA::const_iterator itA = vecA.begin(), VecB::const_iterator itB = vecB.begin();
        itA != vecA.end() && itB != vecB.end(); itA++, itB++){
        C c;
        c.c1 = (*itA).a;
        c.c2 = (*itB).b;
        vecC.push_back(c);
}
```
第二种方法：
```cpp
void MergeVector(vector<A> &vectorA,vector<B> &vectorB,vector<C> &vectorC)
{
vector<A>::iterator pva;
vector<B>::iterator pvb;
vector<C>::iterator pvc;
pva = vectorA.begin();
pvb = vectorB.begin();
pvc = vectorC.begin();
while(pva!=vectorA.end())
{
*pvc->c1 = *pva->a;
*pvc->c2 = *pvb->b;
pva++;
pvb++;
pvc++;
}
}
```
（4）建立两个int类型的向量vector,利用merge算法合并，再用sort算法对合并后算法排序
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
vector<int> merge(vector<int> ,vector<int> );
int main()
{
vector<int> v1;
v1.push_back(4);
v1.push_back(6);
v1.push_back(2);
vector<int> v2;
v2.push_back(3);
v2.push_back(1);
v2.push_back(5);
vector<int> v3=merge(v1,v2);
sort(v3.begin(),v3.end());
for(vector<int>::iterator it=v3.begin();it!=v3.end();++it){
cout<<*it<<endl;
}
}
vector<int> merge(vector<int> v1,vector<int> v2)
{
v1.insert(v1.end(),v2.begin(),v2.end());
return v1;
}
```

