# STL 之 vector 容器详解 - 文章 - 伯乐在线
原文出处： [Sam大叔](http://www.cnblogs.com/scandy-yuan/archive/2013/01/07/2849735.html)
vector 容器
vector是C++标准模版库(STL,Standard Template Library)中的部分内容。之所以认为是一个容器，是因为它能够像容器一样存放各种类型的对象，简单的说：vector是一个能够存放任意类型的动态数组，能够增加和压缩数据。
使用vector容器之前必须加上头文件：#include<vector>;;
vector属于std命名域的内容，因此需要通过命名限定：using std::vector;也可以直接使用全局的命名空间方式：using namespace std;
vector成员函数
c.push_back(elem)在尾部插入一个elem数据。

```
vector<int> v;
v.push_back(1);
```
c.pop_back()删除末尾的数据。

```
vector<int> v;
v.pop_back();
```
c.assign(beg,end)将[beg,end)一个左闭右开区间的数据赋值给c。

```
vector<int> v1,v2;
v1.push_back(10);
v1.push_back(20);
v2.push_back(30);
v2.assign(v1.begin(),v1.end());
```
c.assign (n,elem)将n个elem的拷贝赋值给c。

```
vector<int> v;
v.assign(5,10);//往v里放5个10
```
c.at(int index)传回索引为index的数据,如果index越界,抛出out_of_range异常。

```
vecto<int> v;
cout << v.at(2) << endl;//打印vector中下标是2的数据
```
c.begin()返回指向第一个数据的迭代器。
c.end()返回指向最后一个数据之后的迭代器。

```
vector<int> v;
v.push_back(1);
v.push_back(2);
v.push_back(3);
vector<int>::iterator it;
for(it = v.begin();it!=v.end();it++){
    cout << *it << "\t";
}
cout << endl;
```
c.rbegin()返回逆向队列的第一个数据,即c容器的最后一个数据。
c.rend()返回逆向队列的最后一个数据的下一个位置,即c容器的第一个数据再往前的一个位置。

```
vector<int> v;
v.push_back(1);
v.push_back(2);
v.push_back(3);
vector<int>::reverse_iterator it;
for(it = v.rbegin();it!=v.rend();it++){
    cout << *it << "\t";
}
cout << endl;
```
c.capacity()返回容器中数据个数,翻倍增长。

```
vector<int> v;
v.push_back(1);
cout << v.capacity() << endl;  // 1
v.push_back(2);
cout << v.capacity() << endl;  // 2
v.push_back(3);
cout << v.capacity() << endl; // 4
```
c.clear()移除容器中的所有数据。

```
vector<int>::iterator it;
for(it = v.begin();it!=v.end();it++){
    cout << *it << "\t";
}
v.clear();
for(it = v.begin();it!=v.end();it++){
    cout << *it << "\t";
}
cout << endl;
```
c.empty()判断容器是否为空。

```
vector<int> v;
v.push_back(1);
v.push_back(2);
v.push_back(3);
if(!v.empty()){
    cout << "v is not empty!" << endl;        
}
```
c.erase(pos)删除pos位置的数据,传回下一个数据的位置。

```
vector<int> v;
v.push_back(1);
v.push_back(2);
v.push_back(3);
v.erase(v.begin());
```
c.erase(beg,end)删除[beg,end)区间的数据，传回下一个数据的位置。

```
vector<int> v;
v.push_back(1);
v.push_back(2);
v.push_back(3);
v.erase(v.begin(),v.end());
```
c.front()返回第一个数据。
c.back()传回最后一个数据,不检查这个数据是否存在。

```
vector<int> v;
v.push_back(1);
v.push_back(2);
v.push_back(3);
if(!vec.empty()){
    cout << “the first number is:” << v.front() << endl;
    cout << “the last number is:” << v.back() << endl;
}
```
c.insert(pos,elem) 在pos位置插入一个elem的拷贝,返回插入的值的迭代器。
c.insert(pos,n,elem)在pos位置插入n个elem的数据,无返回值。
c.insert(pos,beg,end)在pos位置插入在[beg,end)区间的数据,无返回值。

```
vector<int> v;
v.insert(v.begin(),10);
v.insert(v.begin(),2,20);
v.insert(v.begin(),v1.begin(),v1.begin()+2);
```
c.size()返回容器中实际数据的个数。
c.resize(num)重新指定队列的长度。（往往用来增加vector的长度,小->大 ok 大->小 没用！）
c.reserve()保留适当的容量。
针对resize()和reserver()做一点分析:
reserve是容器预留空间，但并不真正创建元素对象，在创建对象之前，不能引用容器内的元素，因此当加入新的元素时，需要用push_back()/insert()函数。
resize是改变容器的大小，并且创建对象，因此，调用这个函数之后，就可以引用容器内的对象了，因此当加入新的元素时，用operator[]操作符，或者用迭代器来引用元素对象。
再者，两个函数的形式是有区别的，reserve函数之后一个参数，即需要预留的容器的空间；resize函数可以有两个参数，第一个参数是容器新的大小，第二个参数是要加入容器中的新元素，如果这个参数被省略，那么就调用元素对象的默认构造函数。
reserve只是保证vector的空间大小(capacity)最少达到它的参数所指定的大小n。在区间[0, n)范围内，如果下标是index，vector[index]这种访问有可能是合法的，也有可能是非法的，视具体情况而定。
resize和reserve接口的共同点是它们都保证了vector的空间大小(capacity)最少达到它的参数所指定的大小。
c.max_size()返回容器能容量的最大数量。
c1.swap(c2)将c1和c2交换。
swap(c1,c2)同上。

```
vector<int> v1,v2,v3;
v1.push_back(10);
v2.swap(v1);
swap(v3,v1);
```
vector<type>c;创建一个空的vector容器。
vector<type> c1(c2);复制一个vector。
vector<type> c(n);创建一个vector,含有n个数据，数据均以缺省构造产生,即全0；
vector<type> c(n,elem)创建一个vector,含有n个elem的拷贝数据。
vector<type> c(beg,end)创建一个以[beg,end)区间的vector。
~vector<type>() 销毁所有数据,施放内存。
压缩一个臃肿的 vector
很多时候大量的删除数据，或者通过使用reserver(),结果vector的空间远远大于实际的需要。所以需要压缩vector到它的实际大小。resize()能增加vector的大小。clear()仅仅移除容器内的数据,不能改变capacity()的大小,所以对vector进行压缩非常重要。
测试一下clear()函数：

```
//
//  vector.cpp
//  vector
//
//  Created by scandy_yuan on 13-1-7.
//  Copyright (c) 2013年 Sam. All rights reserved.
//
#include <iostream>
#include <vector>
using namespace std;
int main(int argc, const char * argv[])
{
    // insert code here...
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    vector<int>::iterator it;
    cout << "clear before:" << " ";
    for(it=v.begin();it!=v.end();it++){
        cout << *it << "\t";
    }
    cout << endl;
    cout << "clear before capacity:" << v.capacity() << endl;
    v.clear();
    cout << "after clear:" << " ";
    for(it=v.begin();it!=v.end();it++){
        cout << *it << "\t";
    }
    cout << endl;
    cout << "after clear capacity:" << v.capacity() << endl;
    return 0;
}
```
结果：

```
clear before: 1    2    3    
clear before capacity:4
after clear: 
after clear capacity:4
```
为什么这里打印的capacity()的结果是4不做详细解释,请参考上面关于capacity的介绍。 通过结果，我们可以看到clear()之后数据全部清除了,但是capacity()依旧是4。
假设：我们通过原本的vector来创建一个新的vector,让我们看看将会发生什么？

```
//
//  vector.cpp
//  vector
//
//  Created by scandy_yuan on 13-1-7.
//  Copyright (c) 2013年 Sam. All rights reserved.
//
#include <iostream>
#include <vector>
using namespace std;
int main(int argc, const char * argv[])
{
    // insert code here...
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    cout << "v.capacity()" << v.capacity() << endl;
    
    vector<int> v1(v);
    cout << "v1.capacity()" << v1.capacity() << endl;
    return 0;
}
```
结果：

```
v.capacity()4
v1.capacity()3
```
可以看出,v1的capacity()是v的实际大小，因此可以达到压缩vector的目的。但是我们不想新建一个,我们想在原本的vector(即v)上进行压缩,那么借鉴上面的方式思考另一种方式。
假设:我们通过swap函数把v1交换回v,看看会发生什么？

```
//
//  vector.cpp
//  vector
//
//  Created by scandy_yuan on 13-1-7.
//  Copyright (c) 2013年 Sam. All rights reserved.
//
#include <iostream>
#include <vector>
using namespace std;
int main(int argc, const char * argv[])
{
    // insert code here...
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    cout << "v.capacity()" << v.capacity() << endl;
    
    vector<int> v1(v);
    cout << "v1.capacity()" << v1.capacity() << endl;
    
    v.swap(v1);
    cout << "v.swap(v1).capacity()" << v.capacity() << endl;
    return 0;
}
```
结果：

```
v.capacity()4
v1.capacity()3
v.swap(v1).capacity()3
```
可以看出,v.capacity()变成了3,目的达到。但是代码给人感觉繁琐臃肿,我们从新考虑一种新的写法,采用匿名对象来代替v1这个中间对象：vector (v).swap(v);
测试：

```
//
//  vector.cpp
//  vector
//
//  Created by scandy_yuan on 13-1-7.
//  Copyright (c) 2013年 Sam. All rights reserved.
//
#include <iostream>
#include <vector>
using namespace std;
int main(int argc, const char * argv[])
{
    // insert code here...
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    cout << "v.capacity()" << v.capacity() << endl;
    
    vector<int> (v).swap(v);
    cout << "v.capacity()" << v.capacity() << endl;
    return 0;
}
```
结果：

```
v.capacity()4
v.capacity()3
```
可以看到 v.capacity()由4编程了3,目的达到。
之前没有关注C++11,感谢@egmkang,确实在C++11中已经提供了shrink_to_fit()函数实现vector的压缩。
如下：


```
#include <iostream>
#include <vector>
int main()
{
    std::vector<int> v;
    std::cout << "Default-constructed capacity is " << v.capacity() << '\n';
    v.resize(100);
    std::cout << "Capacity of a 100-element vector is " << v.capacity() << '\n';
    v.clear();
    std::cout << "Capacity after clear() is " << v.capacity() << '\n';
    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << v.capacity() << '\n';
}
```
结果：

```
Default-constructed capacity is 0
Capacity of a 100-element vector is 100
Capacity after clear() is 100
Capacity after shrink_to_fit() is 0
```
