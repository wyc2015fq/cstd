# 如何在DevC++中使用C++11语法？ - 心纯净，行致远 - CSDN博客





2018年11月16日 22:17:46[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：394








在用到DevC++编译C++中的容器时，出现了一个错误。如下：

![](https://img-blog.csdnimg.cn/20181116220221912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

E:\C编程学习\1024\11161242.cpp   ** [Error] in C++98 's1' must be initialized by constructor, not by '{...}'**

**代码没有任何问题，后来上网查证了一下，原来是DevC++的编译器没有支持C++11，因此只需要更改即可。**

**更改方法：**

工具->编译选项

![](https://img-blog.csdn.net/20161113193016166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

语言标准 设置ISO C++ 11

其实不更改，也可以正常实现代码功能，只是不能使用template T 的输出，也就是必须一个一个地写输出函数才行，就比如下面的代码使用迭代器iterator，一个个写输出函数也是没问题的。

```cpp
#include <iostream>
#include <vector>
#include <string>
 
using namespace std;
 
//template <typename T>
//void showvector(vector<T> v)
//{
//    for (vector<T>::iterator it = v.begin(); it != v.end(); it++)
//    {
//        cout << *it;
//    }
//    cout << endl;
//}
 
int main()
{
    vector<string> v6(7,"s");
    v6.resize(3);  //重新调整vector容量大小
    for (vector<string>::iterator it = v6.begin(); it != v6.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
 
    vector<int> v5(10,6); //列表初始化,注意使用的是花括号
    cout << v5.front() << endl; //访问第一个元素
    cout << v5.back() << endl; //访问最后一个元素
 
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.pop_back(); //删除最后一个元素
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.push_back(6); //加入一个元素并把它放在最后
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.insert(v5.begin()+1,9); //在第二个位置插入新元素
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.erase(v5.begin() + 3);  //删除第四个元素
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.insert(v5.begin() + 1, 7,8); //连续插入7个8
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    v5.clear(); //清除所有内容
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++)
    {
        cout << *it;
    }
 	cout << endl;
    system("pause");
    return 0;
} 

/*
sss
6
6
6666666666
666666666
6666666666
69666666666
6966666666
68888888966666666

请按任意键继续. . .

*/
```

但是，这样带来的典型问题就是代码不够优化，太冗杂。template的好处就是可以使输出函数的输入数据类型具有多态性。

下面时优化的代码：

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;
 
template <typename T>
void showvector(vector<T> v)
{
    for (typename vector<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
 
int main()
{
    vector<string> v6 = { "hi","my","name","is","lee" };
    v6.resize(3);  //重新调整vector容量大小
    showvector(v6);
 
    vector<int> v5 = { 1,2,3,4,5 }; //列表初始化,注意使用的是花括号
    cout << v5.front() << endl; //访问第一个元素
    cout << v5.back() << endl; //访问最后一个元素
 
    showvector(v5);
    v5.pop_back(); //删除最后一个元素
    showvector(v5);
    v5.push_back(6); //加入一个元素并把它放在最后
    showvector(v5);
    v5.insert(v5.begin()+1,9); //在第二个位置插入新元素
    showvector(v5);
    v5.erase(v5.begin() + 3);  //删除第四个元素
    showvector(v5);
    v5.insert(v5.begin() + 1, 7,8); //连续插入7个8
    showvector(v5);
    v5.clear(); //清除所有内容
    showvector(v5);
 
    system("pause");
    return 0;
} 
/*
output:
himyname
1
5
12345
1234
12346
192346
19246
188888889246

请按任意键继续. . .
 
*/
```

这里要注意showvector（）里面的迭代器iterator需要使用typename,否则会有一下的错误提醒：
`E:\C编程学习\1024\111622209.cpp	[Error] need 'typename' before 'std::vector<T>::iterator' because 'std::vector<T>' is a dependent scope`
不过根据提醒也能猜得出来。



