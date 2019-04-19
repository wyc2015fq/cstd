# STL中list的使用（理论） - maopig的专栏 - CSDN博客
2011年09月14日 20:22:40[maopig](https://me.csdn.net/maopig)阅读数：10058
                
STL中的list就是一双向链表，可高效地进行插入删除元素。现总结一下它的操作。
文中所用到两个list对象c1,c2分别有元素c1(10,20,30) c2(40,50,60)。还有一个list<int>::iterator citer用来指向c1或c2元素。
list对象的声明构造()：
A.      list<int>c0;                //空链表
B.      list<int>c1(3);            //建一个含三个默认值是0的元素的链表
C.      list<int>c2(5,2);            //建一个含五个元素的链表，值都是２
D.      list<int>c4(c2);            //建一个c2的copy链表
E.      list<int>c5(c1.begin(),c1.end());  
//c5含c1一个区域的元素[_First, _Last)。
1.      assign()分配值，有两个重载：
c1.assign(++c2.begin(), c2.end()) //c1现在为(50,60)。
c1.assing(7,4) //c1中现在为7个4,c1(4,4,4,4,4,4,4)。
2.      back()返回最后一元素的引用：
int i=c1.back(); //i=30
const int i=c2.back(); //i=60且不可修改
3.      begin()返回第一个元素的指针(iterator)
citer=c1.begin();    // *citer=10
list<int>::const_iterator cciter=c1.begin(); //*cciter=10且为const。
4.      clear()删除所有元素
c1.clear();  //c1为空 c1.size为０；
5.      empty()判断是否链表为空
bool B=c1.empty(); //若c1为空B=true；否则B=false;
6.      end()返回最后一个元素的下一位置的指针(list为空时end()=begin())
citer=c1.end(); //*(--citer)=30;
同begin()返回一个常指针，不能修改其中元素。
7.      erase()删除一个元素或一个区域的元素(两个重载)
c1.erase(c1.begin()); // c1现为(20,30);
c1.erase(++c1.begin(),c1.end()); //　c1现为(10);
8.      front() 返回第一个元素的引用：
int i=c1.front(); //i=10;
const int i=c1.front(); //i=10且不可修改。
9.      insert()在指定位置插入一个或多个元素(三个重载)：
c1.insert(++c1.begin(),100);  //c1(10,100,20,30)
c1.insert(c1.begin(),2,200); //c1(200,200,20,30);
c1.insert(++c1.begin(),c2.begin(),--c2.end());
//c1(10,40,50,20,30);
10.    max_size()返回链表最大可能长度(size_type就是int型)：
list<int>::size_type i=c1.max_size(); //i=1073741823
11.    merge()合并两个链表并使之默认升序(也可改)：
c2.merge(c1);  //c1现为空;c2现为c2(10,20,30,40,50,60)
c2.merge(c1,greater<int>()); //同上，但c2现为降序
12.    pop_back()删除链表尾的一个元素
c1.pop_back() //c1(10,20);
13.    pop_front()删除链表头的一元素
c1.pop_front()　//c1(20,30)
14.    push_back()增加一元素到链表尾
c1.push_back(100) //c1(10,20,30,100)
15.    push_front()增加一元素到链表头
c1.push_front(100) //c1(100,10,20,30)
16.    rbegin()返回链表最后一元素的后向指针(reverse_iterator or const)
list<int>::reverse_iterator riter=c1.rbegin(); //*riter=30
17.    rend()返回链表第一元素的下一位置的后向指针
list<int>::reverse_iterator riter=c1.rend(); // *(--riter)=10
18.    remove()删除链表中匹配值的元素(匹配元素全部删除)
c1.remove(10);    //c1(20,30)
19.    remove_if()删除条件满足的元素(会遍历一遍链表)
c1.remove_if( is_odd<int> () ); //c1(10,20,30)　
//is_odd自己写(表奇数)　
20.    resize()重新定义链表长度(两重载)：
c1.resize(4) //c1(10,20,30,0)用默认值填补
c1.resize(4,100) //c1(10,20,30,100)用指定值填补
21.    reverse()反转链表:
c1.reverse(); //c1(30,20,10)
22.    size()返回链表中元素个数
list<int>::size_type i=c1.size(); //i=3
23.    sort()对链表排序，默认升序(可自定义)
c1.sort(); //c1(10,20,30)
c1.sort(great<int>()); //c1(30,20,10)
24.    splice()对两个链表进行结合(三个重载)
c1.splice(++c1.begin(),c2); 
//c1(10,40,50,60,20,30) c2为空 全合并
c1.splice(++c1.begin(),c2,++c2.begin()); 
//c1(10,50,20,30) ; c2(40,60)　指定元素合并
c1.splice(++c1.begin(),c2,++c2.begin(),c2.end()); 
//c1(10,50,60,20,30); c2(40) 指定范围合并
25.    swap()交换两个链表(两个重载)
c1.swap(c2); //c1(40,50,60);
swap(c1,c2); //c1(40,50,60)
26.    unique()删除相邻重复元素(断言已经排序，因为它不会删除不相邻的相同元素)
c1.unique(); 
//假设c1开始(-10,10,10,20,20,-10)则之后为c1(-10,10,20,-10)
c1.unique(mypred); //自定义谓词

**C++ Lists（[链表](http://wenwen.soso.com/z/Search.e?sp=S%E9%93%BE%E8%A1%A8&ch=w.search.yjjlink&cid=w.search.yjjlink)）**
Lists将元素按顺序储存在链表中. 与 [向量](http://wenwen.soso.com/z/Search.e?sp=S%E5%90%91%E9%87%8F&ch=w.search.yjjlink&cid=w.search.yjjlink)(vectors)相比, 它允许快速的插入和删除，但是随机访问却比较慢.
assign() 给list[赋值](http://wenwen.soso.com/z/Search.e?sp=S%E8%B5%8B%E5%80%BC&ch=w.search.yjjlink&cid=w.search.yjjlink)
back() 返回最后一个元素 
begin() 返回指向第一个元素的[迭代器](http://wenwen.soso.com/z/Search.e?sp=S%E8%BF%AD%E4%BB%A3%E5%99%A8&ch=w.search.yjjlink&cid=w.search.yjjlink)
clear() 删除所有元素 
empty() 如果list是空的则返回true 
end() 返回末尾的迭代器 
erase() 删除一个元素 
front() 返回第一个元素 
get_allocator() 返回list的配置器 
insert() 插入一个元素到list中 
max_size() 返回list能容纳的最大元素数量 
merge() 合并两个list 
pop_back() 删除最后一个元素 
pop_front() 删除第一个元素 
push_back() 在list的末尾添加一个元素 
push_front() 在list的头部添加一个元素 
rbegin() 返回指向第一个元素的逆向迭代器 
remove() 从list删除元素 
remove_if() 按指定条件删除元素 
rend() 指向list末尾的逆向迭代器 
resize() 改变list的大小 
reverse() 把list的元素倒转 
size() 返回list中的元素个数 
sort() 给list排序 
splice() 合并两个list 
swap() 交换两个list 
unique() 删除list中重复的元素
**附List用法实例：**
#include <iostream>
#include <list>
#include <numeric>
#include <algorithm>
using namespace std;
//创建一个list容器的实例LISTINT
typedef list<int> LISTINT;
//创建一个list容器的实例LISTCHAR
typedef list<char> LISTCHAR;
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
    //使用STL的accumulate(累加)[算法](http://wenwen.soso.com/z/Search.e?sp=S%E7%AE%97%E6%B3%95&ch=w.search.yjjlink&cid=w.search.yjjlink)
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
#include <iostream>
#include <list>
using namespace std;
typedef list<int> INTLIST;
//从前向后显示list队列的全部元素
void put_list(INTLIST list, char *name)
{
    INTLIST::iterator plist;
    cout << "The contents of " << name << " : ";
    for(plist = list.begin(); plist != list.end(); plist++)
        cout << *plist << " ";
    cout<<endl;
}
//测试list容器的功能
void main(void)
{
//list1对象初始为空
    INTLIST list1;   
    //list2对象最初有10个值为6的元素 
    INTLIST list2(10,6); 
    //list3对象最初有3个值为6的元素 
    INTLIST list3(list2.begin(),--list2.end()); 
    //声明一个名为i的双向迭代器
    INTLIST::iterator i;
    //从前向后显示各list对象的元素
    put_list(list1,"list1");
    put_list(list2,"list2");
    put_list(list3,"list3");
//从list1序列后面添加两个元素
list1.push_back(2);
list1.push_back(4);
cout<<"list1.push_back(2) and list1.push_back(4):"<<endl;
    put_list(list1,"list1");
//从list1序列前面添加两个元素
list1.push_front(5);
list1.push_front(7);
cout<<"list1.push_front(5) and list1.push_front(7):"<<endl;
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
cout<<"list1.pop_front() and list1.pop_back():"<<endl;
    put_list(list1,"list1");
//清除list1中的第2个元素
list1.erase(++list1.begin());
cout<<"list1.erase(++list1.begin()):"<<endl;
    put_list(list1,"list1");
//对list2赋值并显示
list2.assign(8,1);
cout<<"list2.assign(8,1):"<<endl;
    put_list(list2,"list2");
//显示序列的[状态信息](http://wenwen.soso.com/z/Search.e?sp=S%E7%8A%B6%E6%80%81%E4%BF%A1%E6%81%AF&ch=w.search.yjjlink&cid=w.search.yjjlink)
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
list1.splice(++list1.begin(), list3);
    put_list(list1,"list1");
    put_list(list3,"list3"); 
}
**补充：STL标准函数find进行vector 、list链表查找**
#include <vector>
#include <algorithm>
#include <iostream>
class example
{
public:
example(int val)
{
i = val;
}
bool operator==(example const & rhs)
{
return (i == rhs.i) ? true : false;
}
private:
int i;
};
using namespace std;
int main(void)
{
vector<example> ve;
ve.push_back(1);
vector<example>::iterator it;
example elem(1);
it = find(ve.begin(), ve.end(), elem);
cout<<boolalpha<<(*it == elem);
}
