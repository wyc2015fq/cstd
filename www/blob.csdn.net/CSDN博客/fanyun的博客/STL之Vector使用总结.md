# STL之Vector使用总结 - fanyun的博客 - CSDN博客
2017年05月31日 21:20:08[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：422
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
         vector作为STL提供的标准容器之一，在我们的开发中应用广泛，避免了我们不必要的造轮子。vector又被称为向量，vector可以形象的描述为长度可以动态改变的数组，功能和数组较为相似。实际上更专业的描述为：vector是一个多功能的，能够操作多种数据结构和算法的模板类和函数库，vector之所以被认为是一个容器，是因为它能够像容器一样存放各种类型的对象，简单地说，vector是一个能够存放任意类型的动态数组，能够增加和压缩数据。实际上vector和数组很像，但是又有区别，比如，数组分配的是静态空间，一旦分配就无法改变。而vector分配的是动态空间，即vector无需指定容器大小，而是随着元素的
 增加，自动扩展的。分配的空间跟数组一样是连续的，实际上自增是通过new一个大的vector，替换原来小的vector，并把原来的vector里面元素全部移动到大的vector里面，最后将原来的空间释放掉。
**1.  Vector的初始化及销毁：**
(1).默认构造函数,长度为0的空向量
** vector<int> v1;**
(2).带有单个整形参数的构造函数,长度为20的空向量
** vector<int> v2(20); vector<int> v3(30,1);//长度为30，初始值为1的向量**
(3).复制构造函数,构造一个新的向量v4，作为已存在的向量v2的完全复制
** vector<int> v4(v2);**
(4).带两个常量参数的构造函数，产生初始值为一个区间的向量。区间由一个左闭右开的半开区间[first,last) 来指定
** vector<int>v5(first_Iter,end_Iter);**
(5).销毁所有数据，释放内存
**~vector <Elem>()**
```cpp
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    vector<int>::iterator iter;
    //第一种方式
    vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    cout<<"第一种方式的输出结果："<<endl;
    for(iter = v1.begin() ; iter != v1.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    //第二种方式
    vector<int> v2(v1);
    cout<<"第二种方式的输出结果："<<endl;
    for(iter = v2.begin() ; iter != v2.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    //第三种方式
    vector<int> v3(3);
    cout<<"第三种方式的输出结果："<<endl;
    for(iter = v3.begin() ; iter != v3.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    //第四种方式
    vector<int> v4(3,4);
    cout<<"第四种方式的输出结果："<<endl;
    for(iter = v4.begin() ; iter != v4.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    //第五种方式
    vector<int> v5(v1.begin(),v1.end()-1);
    cout<<"第五种方式的输出结果："<<endl;
    for(iter = v5.begin() ; iter != v5.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    //第六种方式
    int a[] = {1,2,3,4};
    vector<int> v6(a+1,a+2);
    cout<<"第六种方式的输出结果："<<endl;
    for(iter = v6.begin() ; iter != v6.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    //
    v6.~vector<int>();
    cout<<"释放内存后的结果是："<<endl;
    for(iter = v6.begin() ; iter != v6.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
    return 0;
}
```
**2.  Vector中其他函数的用法**
(1).v.assign(beg,end)  
将[beg; end)区间中的数据赋值给v
(2).**v.assign(n,elem)  **
传回索引idx所指的数据，如果idx越界，抛出
(3).v.at(idx)    
传回索引idx所指的数据，如果idx越界，抛出 , out_of_range
(4).v.begin()
传回迭代器重的一个数据
(5).v.capacity() 
返回容器中数据个数
(6).v.clear()      
移除容器中所有数据   
(7).v.empty()
判断容器是否为空
(8).v.end() 
指向迭代器中的最后一个数据地址            
(9). v.insert(pos,elem)
在pos位置插入一个elem拷贝，**传回新数据位置**（位置指传回地址值）
(10). v.insert(pos,beg,end)
在pos位置插入在[beg,end)区间的数据。**无返回值**
(11). v.insert(pos,n,elem)
在pos位置插入n个elem数据。**无返回值**
(12). v.erase(pos)
删除pos位置的数据，传回**下一个数据的位置**
(13). v.erase(beg,end)
删除[beg,end)区间的数据，传回**下一个数据的位置**
(14). **v.capacity() **
返回容器中数据个数
(15). v.size()
返回容器中实际数据的个数
(16). v.reserve()
保留适当的容量
(17).v.resize(num)
重新指定队列的长度。
(18). **v.max_size() **
返回容器中最大数据的数量
(19). v.rbegin()
传回一个逆向队列的第一个数据
(20). v.rend()
传回一个逆向队列的最后一个数据的下一个位置
(21).v.pop_back()
删除最后一个数据
(22). v. push_back(elem)
在尾部加入一个数据
(23).v.front()
传回第一个数据
(24). v.back()
传回最后一个数据，不检查这个数据是否存在
(25). V1.swap(c2)or swap(v1,v2)
将c1和c2元素互换
```cpp
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    vector<int> v1(4,1);
    vector<int>::iterator iter;
    cout<<"vector的size的值 : "<<v1.size()<<endl;
    cout<<"vector的capacity值 : "<<v1.capacity()<<endl;
    cout<<"vector的max_size的值 : "<<v1.max_size()<<endl;
    
    //使用reserve函数
    v1.reserve(6);
    cout<<endl;
    cout<<"vector的size的值 : "<<v1.size()<<endl;
    cout<<"vector的capacity值 : "<<v1.capacity()<<endl;
    cout<<"vector的max_size的值 : "<<v1.max_size()<<endl;
    cout<<"vector中的元素是 : ";
    for(iter = v1.begin() ; iter != v1.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl<<endl;
    //使用resize函数
    v1.resize(6,2);
    cout<<endl;
    cout<<"vector的size的值 : "<<v1.size()<<endl;
    cout<<"vector的capacity值 : "<<v1.capacity()<<endl;
    cout<<"vector的max_size的值 : "<<v1.max_size()<<endl;
    cout<<"vector中的元素是 : ";
    for(iter = v1.begin() ; iter != v1.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl<<endl;
    return 0;
}
```
