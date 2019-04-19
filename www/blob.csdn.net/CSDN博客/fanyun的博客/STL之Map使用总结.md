# STL之Map使用总结 - fanyun的博客 - CSDN博客
2017年05月31日 21:21:17[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：678
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
         Map是STL的标准容器之一，和Set一样，Map是一种关联容器。他提供一对一的数据处理方式，每个Map有两个关键字，第一个关键字在Map中是唯一的，也叫做Map的键；第二个关键字用来存储该关键字的值。Map内部和层次实现是一颗红黑数，是一种非平衡二叉树。这棵树具有对数据自动排序的功能，所以Map内部所有数据均是有序的。
**1. Map的初始化及销毁：**
(1). Map()创建了一个空的 map 对象 m ，元素的键值类型为 char ，元素的映照数据类型为 int ，键值的比较函数对象为  greater<char>
 map<char, int,greater<char> >  m;  // 两个 ">" 、">" 符号不要写在一起了，记得要空格
(2). map(constkey_compare& comp)指定一个比较函数对象comp 来创建 map 对象，内存分配器为默认值。
 map<T1,T2,less<T1>>  mapA;  //该容器是按键的升序方式排列元素。未指定函数对象，默认采用less<T1>函数对象。
    map<T1,T2,greater<T1>> mapB;   //该容器是按键的降序方式排列元素。
    less<T1>与greater<T1> 可以替换成其它的函数对象functor。
    可编写自定义函数对象以进行自定义类型的比较，使用方法与set构造时所用的函数对象一样。
    例如，下面的代码使用自定义的函数对象 strLess ，创建一个 map 容器对象 m 。该容器的键值类型为 const char* ，映照容器数据类型为 int 。
    // 定义字符串比较函数对象 strLess
    struct  strLess
    {
        bool operator() (const char* s1, constchar* s2) const
        {
            return strcmp(s1, s2)< 0 ;
        }
    };
    // 创建 map 容器对象 m
    map<const char*, int >   m(strLess());
(3). map(const map&)拷贝构造函数，用一个 map 容器的元素和比较函数，拷贝生成一个新的 map 容器对象。
    例如，下面代码利用 map 容器对象 m1 ，创建另一个容器对象 m2 .
// map <int ,char*>  m1;
    map<int, char*>  m2(m1);  // 使用默认的键值比较函数 less<int>
(4). map(InputIterator first,InputIterator last)用迭代器区间[first, last) 所指的数据，作为 map 容器的元素（包括键值和映照数据），创建一个 map 容器对象。
    例如，下面代码使用数组 pairArray 的 5 个 pair 对象，创建 map 容器对象 m 。
 pair<const int,char>  p1(1, 'a');
    pair<const int, char>  p1(2, 'b');
    pair<const int, char>  p1(3, 'c');
    pair<const int, char>  p1(4, 'd');
    pair<const int, char>  p1(5, 'e');
    pair<const int, char>  pairArray[] = {p1, p2, p3,p4, p5};
    // 创建 map 容器对象 m
    map<const int, char>  m(pairArray, pairArray +5);
(5). map(InputIterator first,InputIterator last, const key_compare& comp)    
    用迭代器区间[first, last)所指的数据和 comp 函数对象，创建一个 map 对象。
    例如，下面代码使用 greater<int> 函数对象，取代默认的 less<int> ，创建一个 map 对象 m 。
   map<constint, char, greater<const int> >   m(pairArray, pairArray+5,greater<const int>());
(6). 销毁Map
~Map（）；
**2. Map的操作函数使用总结**
(1) map c
产生一个空map/multimap
(2) map c(op)
以op为排序准则产生一个空map/multimap
(3)  map c1(c2)
产生某一个map/multimap的副本
 (4) map c(beg, end)
 以区间[beg, end)内的元素产生一个map/multimap
 (5) map c(beg, end, op)
 以op为排序准则，以区间[beg, end)内的元素产生一个map/multimap
 (6) c.size()
返回当前的元素数量
 (7) c.empty()
判断c是否为空
 (8) c.max_size()
返回可容纳的元素最大数值
 (9) c1 compare c2
 compare可为==，!=，<，>，<=和>=
 (10) c.count(elem)
 返回“键值等于key”的元素个数
 (11) c.find(elem)
返回“键值等于key”的第一个元素的迭代器，如果找不到就返回end()
 (12) c.lower_bound(key)
返回“键值为key”之元素的第一个可安插位置，也就是“键值>=key”的第一个元素位置
 (13) c.upper_bound(key)
 返回“键值为key”之元素的最后一个可安插位置，也就是“键值>key”的第一个元素位置
 (14) c.equal_range(key)
 返回“键值为key”之元素的第一个和最后一个可安插位置，也就是“键值==key”的元素区间lower_bound()、upper_bound()和equal_range()行为与set相应的函数十分相似，唯一不同的是：元素是一个key/value pair。
 (15) c1 = c2
将c2全部元素赋值给c1
 (16) c1.swap(c2)
将c1和c2元素互换
 (17) c.begin()
返回一个双向存取迭代器（将key视为常数），指向第一个元素
(18) c.end()
返回一个双向存取迭代器（将key视为常数），指向最后元素的下一个位置
(19) c.rbegin()
返回一个逆向迭代器，指向逆向迭代的第一个元素
(20) c.rend()
返回一个逆向迭代器，指向逆向迭代的最后元素的下一个位置
这里的元素是一个key/value pair（pair<const key, T>）。
(21) c.insert(elem)
插入一个elem副本并返回新元素的位置
(22) c.insert(it, elem)
安插elem副本返回新元素的位置（it是一个提示指出安插操作的搜寻起点）
(23) c.insert(beg, end)
插入区间[beg, end)内的所有元素副本，无返回值
(24) c.erase(elem)
移除“value与elem相等”的所有元素，返回被移除元素的个数
(25) c.erase(it)
移除it位置上的元素，无返回
(26) c.erase(beg, end)
移除[beg, end)区间内的所有元素，无返回
(27) c.clear()
将容器清空
代码示例：
```cpp
map 容器的元素插入
#pragma warning(disable:4786)
#include <map>
#include <iostream>
using namespace std;
int main()
{
    // 创建 map 容器对象 m
    map<const char*, float>  m;
    // 插入元素（水果名，单价），水果名位键值，单价为映照数据
    m["apple"] = 3.6f;
    m["orange"] = 3.2f;
    m["banana"] = 1.8f;
    m["pear"] = 2.3f;
    m["lichee"] = 6.3f;
    m.insert(pair<const char*, float>("何亮",100.001f) );
    // 打印元素
    cout << "苹果价格：" << m["apple"] << "元/斤 \n"; 
    cout << "桔子价格：" << m["orange"] << "元/斤 \n"; 
    cout << "香蕉价格：" << m["banana"] << "元/斤 \n"; 
    cout << "雪梨价格：" << m["pear"] << "元/斤 \n"; 
    cout << "荔枝价格：" << m["lichee"] << "元/斤 \n"; 
    cout << "pair插入  " << m["何亮"] << "元/斤\n";
    return 0;
}
```
```cpp
元素的删除
    与 set 集合容器一样，map 映照容器的删除函数，可删除某个迭代器位置上的元素、等于某键值的元素、一个迭代器区间上的元素和容器中的所有元素。
1.    void  erase(iterator position)    删除 position 所指的元素
2.    size_type  erase(const key_type& k)    删除键值为 K 的元素。对 map 容器来说，此函数总是返回值 1，因为 map 容器不会出现键值重复的元素值。
3.    void  erase(iterator first, iterator last)    删除 map 迭代器区间 [first, last) 上的所有元素
4.    void clear()     删除 map 容器的所有元素
    例如，下面的代码将上一例子 map 对象 m 中，"lichee, 6.3" 的元素数据删除
    m.erase("lichee");
```
```cpp
遍历 map 容器元素
    
/*    除了利用键值的数组方式来访问元素外，更一般可使用 map 容器的迭代器进行访问，通常需要用 begin 和 end 函数找出遍历开始的首元素和结束元素，然后通过迭代器的“++” 和“*” 操作，读取容器元素。
    下面的示例程序，将一批学生记录（学号为键值）插入到 map 容器对象 m 中，然后，利用前向迭代器打印出来。由于 map 容器的元素是通过转换为 pair 对象插入到容器的，因此，学生记录结构体中各个变量域，需要通过 pair 对象的 first 和 second 变量 指出来。（关于 pair ，可参考之前的讨论 set 容器的文章）
*/    
-------------------------------------------------------- 遍历 map 容器元素
#pragma warning(disable:4786)
#include <map>
#include <iostream>
using namespace std;
// 学生信息结构体
struct StudentInfo
{
    char* name;
    int year;
    char* addr;
};
// 学生记录结构体
struct StudentRecord  
{
    int id;  // 学号，作键值
    StudentInfo sf;  // 学生信息，作映照数据
};
int main()
{
    // 3个学生的记录
    StudentRecord srArray[] = {
        {1, "何亮",21,"武汉"},
        {2, "何小亮",20,"上海"},
        {3, "何大亮",22,"北京"}
    };
    // 创建 map 容器对象 m ，管理学生记录
    map<int, StudentInfo> m;
    // 装入 3 笔学生记录
    for (int j=0; j<3; j++)
        m[srArray[j].id] = srArray[j].sf;
    // 迭代器遍历元素
    map<int, StudentInfo>::iterator i, iend;
    iend = m.end();
    cout << "学号    " << "姓名    " << "年龄    " << "地址    " << endl;
    for (i=m.begin(); i!=iend; ++i)
        cout << (*i).first << '    ' << (*i).second.name << '    ' << (*i).second.year << '    ' << (*i).second.addr << endl;
    cout << endl;
    return 0;
}
```
```cpp
map 容器元素的搜索
/*    利用 map 容器提供的 find 函数，可搜索出具有某一键值的元素。map 容器的元素键值是唯一的。find 函数返回的迭代器值为搜索到的元素位置，如果该元素不存在，则返回一个 end 结束元素位置。
    下面的示例程序重写了 StudentRecord 结构体，以适于使用返回 pair 对象的 insert 函数将学生记录插入到 map 容器。 返回的 pair 对象的第二变量 second 中，判断是否插入成功（键值重复插入将失败）。然后，调用 find 函数，对学号为 2 的键值进行搜索，并将该学生的记录打印出来。
*/
-------------------------------------------------------- map 容器元素的搜索
#pragma warning(disable:4786)
#include <map>
#include <iostream>
using namespace std;
// 学生记录结构体
struct StudentRecord
{
    struct StudentInfo
    {
        char* name;
        int year;
        char* addr;
    };
    StudentRecord(int id_, char* name_, int year_, char* addr_)
    {
        id = id_;
        sf.name = name_;
        sf.year = year_;
        sf.addr = addr_;
    }
    int id;   // 学号，作键值
    StudentInfo sf;  // 其他信息
};
int main()
{
    // 创建 map 容器对象 m
    typedef map<int, StudentRecord::StudentInfo>  studentmap;
    studentmap m;
    pair<studentmap::iterator, bool>  p;
    // 插入第一条学生记录
    StudentRecord student1 = StudentRecord(1, "何亮1号", 21, "武汉");
    pair<int, StudentRecord::StudentInfo>  pairStudent1(student1.id, student1.sf);
    p = m.insert(pairStudent1);
    if (!p.second)
        cout << "插入学生记录失败:\n" 
        << student1.id << '    ' 
        << student1.sf.name << '    '
        << student1.sf.year << '    '
        << student1.sf.addr << '    '
        << endl << endl;
    // 插入第二条学生记录
    StudentRecord student2 = StudentRecord(2, "何亮2号", 22, "北京");
    pair<int, StudentRecord::StudentInfo>  pairStudent2(student2.id, student2.sf);
    p = m.insert(pairStudent2);
    if (!p.second)
        cout << "插入学生记录失败:\n" 
        << student2.id << '    ' 
        << student2.sf.name << '    '
        << student2.sf.year << '    '
        << student2.sf.addr << '    '
        << endl << endl;
    // 插入第三条学生记录
    StudentRecord student3 = StudentRecord(3, "何亮3号", 23, "上海");
    pair<int, StudentRecord::StudentInfo>  pairStudent3(student3.id, student3.sf);
    p = m.insert(pairStudent3);
    if (!p.second)
        cout << "插入学生记录失败:\n" 
        << student3.id << '    ' 
        << student3.sf.name << '    '
        << student3.sf.year << '    '
        << student3.sf.addr << '    '
        << endl << endl;
    // 插入键值重复的学生记录， 插入失败 
    StudentRecord student4 = StudentRecord(3, "何亮4号", 24, "广州");
    pair<int, StudentRecord::StudentInfo>  pairStudent4(student4.id, student4.sf);
    p = m.insert(pairStudent4);
    if (!p.second)
        cout << "插入学生记录失败:\n" 
        << student4.id << '    ' 
        << student4.sf.name << '    '
        << student4.sf.year << '    '
        << student4.sf.addr << '    '
        << endl << endl;
    // 记录搜索
    studentmap::iterator  i = m.find(2);
    cout << "搜索出学号为2的记录：\n"
        << (*i).first << '    '
        << (*i).second.name << '    '
        << (*i).second.year << '    '
        << (*i).second.addr << '    '
        << endl << endl;
    // 统计 map 容器 m 所有学生的个数
    if (m.empty())
        cout << "map 容器为空" << endl;
    else
        cout << "map 容器元素个数为：" << m.size() << endl << endl;
    return 0;
}
```
        map 容器区别于 set 容器的一个主要特性在于，map 是处理带有键值的记录型元素数据的快速插入、删除 和检索，而 set 则可看成是对单一数据的处理。map 将一个元素划分出键值部分，并按这个局部的键值指定整个元素的函数比较规则，来建立容器的数据分布。map 的元素键值是唯一的，不允许重复的元素键值插入。 set 和 map 都是泛型库对二叉树的一个泛化。
       map 缺点：map是比较高级的用法了，适合用 list , vector 就能完成的操作，就不需要用到 map 。
       map 优点：相比 set ，map 可处理带有键值的记录型数据的快速插入、删除和检索。检索的速度也是非常快的。
