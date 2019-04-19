# C++之List - LC900730的博客 - CSDN博客
2017年07月12日 20:37:35[lc900730](https://me.csdn.net/LC900730)阅读数：343
双向链表容器，可以高效插入与删除元素 
不可以随机存取元素，即不能at与[]； 
迭代器可以++，可以–，但是不能+5，-5等
list的删除元素是左闭右开。 
//插入元素 
链表的起始位置是0，那么插入元素3，就是原来的第3个就会变成第4个
```
insert(it,100);//那么插入后
list.clear();移除容器中所有数据
list,erase(beg,end);删除[beg,end)元素之间的数据，返回下一个数据的位置
list.erase(pos);//删除pos位置的数据，返回下一个数据的位置
lst.remove(elem);//删除容器中所有与elem匹配的元素
```
优先级队列：priority queue
set/multiset: 
set是一个容器，其中包含元素是唯一的，集合中的元素按照一定顺序排列，元素插入过程中是按规则插入的，所以不能指定插入位置。 
向set中插入多次相同的元素
set是红黑树实现的，平衡二叉树，插入效率比vector高 
如 
set1.insert(100); 
set1.insert(100); 
set1.insert(100); 
那么结果上实际只插入了一个100，默认排序规则是从小到大的。
仿函数
Set集合的元素排序
```cpp
set<int， less<int>>setIntA;       升序方式排列元素 
set<int ，greater<int>>setIntB;  降序方式排列元素 
set<int>setIntA;      相当于是set<int， less<int>>
```
less与greater里的int可以改成其他类型，主要是跟set中容纳的数据类型一致。
less<>与greater<>是什么？如果set不包含int类型，而是包含自定义类型，set容器如何排序？
less与greator是系统提供的函数对象，可以使用在基础类型上。但是如果是自定义的数据类型。 
要事实上解决两个问题，需要了解容器的函数对象，也叫伪函数，英文叫functor。
```cpp
void main93(){
    Student s1("s1",31);
    Student s1("s2",22);
    Student s1("s3",44);
    Student s1("s4",11);
    set<Student,FuncStu> set1;
    set1.insert(s1);
    set1.insert(s2);
    set1.insert(s3);
    set1.insert(s4);
    for(set<Student,FuncStu>::iterator it=set1.begin();it!=set1.end();it++){
        cout<<it->age<<endl;
    };
}
```
如果有2个年龄一样的，那么插入会成功吗？
### Pair
set的插入返回值,pair的使用：对组。
```cpp
set中insert的返回值是_Pairib,实际上是
typedef pair<iterator ,bool>_Pairib;
pair<set<Student,Func>::iterator,bool>>pair1=set1.insert(s1);
//判断
if(pair1.second==true){
    cout<<"插入成功"<<endl;
}else{
    cout<<"插入失败"<<endl;
}
```
查找 
=5的个数
```cpp
set1.count(5);//找到集合中元素为5的数量,对于set来说，要么是0，要么是1，对于multiset来说，值可能大于1；
set1.lower_bound(5);//大于等于5的迭代器位置
pair<iterator,iterator>;
_Pairii equal_range(const key_type &_Keyval);
typedef pair<iterator ,iterator> _Pairii(2个迭代器，就是2个ii)
set.equal_range(elem);//返回容器中与elem相等的上下限的两个迭代器，上限是闭区间，下限是开区间。
如set1.equal_range(5) ;//那么返回 5，6
//如果set中没有5这个元素，那么返回的是6，6
set中恰好没有该元素，就返回比该元素大的最靠近的该元素的迭代器，first与second都是
pair<<set<int>::iterator,<set<int>::iterator>> mypair=set1.equal_range(5);
//同类型 迭代器接收 
set<int>::iterator it1=mypair.first;
set<int>::iterator it2=mypair.second;
```
## Map
map是标准的关联式容器，一个map是一个键值对，提供了基于key的快速检索能力； 
map中key是唯一的，集合中的元素按照一定顺序排列，元素插入过程是按照排序规则插入的
```cpp
//插入元素
map<int,string>map1;
//方法1：
map1.insert(pair<int,string>(1,"teacher01"));
map1.insert(pair<int,string>(2,"teacher02"));
//方法2：
map1.insert(make_pair(3,"teacher04"));
map1.insert(make_pair(4,"teacher05"));
//方法3：
map1.insert(pair<int,string>::value_type(5,"teacher05"));
map1.insert(pair<int,string>::value_type(6,"teacher06"));
//方法4：
map1[7]="teacher08";
map1[8]="teacher09";
typedef pair<iterator,bool> _Pairib
前三种方式返回的是pair<iterator,bool>
//返回的是插入的迭代器位置，bool表示结果
   pair<map<int,string>::iterator,bool> mypair1 = map1.insert(pair<int,string>(1,"teacher"));
   pair<map<int,string>::iterator,bool> mypair2 = map1.insert(pair<int,string>(5,"teacher"));
   pair<map<int,string>::iterator,bool> mypair3 = map1.insert(pair<int,string>(5,"teacher"));
   cout<<mypair2.first->second<<endl;
   cout<<mypair2.first->first<<endl;
   cout<<mypair2.second<<endl;
   cout<<mypair3.first->second<<endl;
   cout<<mypair3.first->first<<endl;
   cout<<mypair3.second<<endl;
//第4中赋值方式，即[]赋值方式，如key已经存在，则会覆盖签名key名字相同的值
即map1[7]="teacher07";
再次执行map1[7]="teacher77"不会报错，会将值修改为teacher77
```
查找
```cpp
map<int ,string>::iterator  it1=map1.find(100);
如果没找到，返回的是map.end()；
if(it1==map1.end()){
    //没找到
}
map1.equal_range(5);//返回2个迭代器，形成一个pair
pair<map<int,string>::iterator,map<int,string>::iterator> mypair=map1.equal_range(5);
//第一个迭代器>=5的位置
//第二个迭代器=5的位置
if(mypair.first==map1.end()){
    cout<<"不存在"<<endl;
}else{
    cout<<mypair.first->first<<endl;
}
if(mypair.second==map1.end()){
    cout<<"不存在"<<endl;
}else{
    cout<<mypair.second->first<<endl;
}
```
