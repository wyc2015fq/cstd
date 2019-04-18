# C++ STL学习——vector - 文章 - 伯乐在线
原文出处： [乞力马扎罗的雪](http://blog.csdn.net/chenyufeng1991/article/details/52269300)
学过C++的人肯定会很熟悉STL标准模板库，STL其实就是封装了一系列的接口，供我们调用。很多函数或者算法的实现不需要我们从头开始写，大大提高我们的编程效率。这篇博客在简单介绍STL的情况下，会详细的来介绍vector的使用。
STL共有六大组件：
一。容器（Container）：是一种数据结构，如list,vector,deque,queue等，以模板类的方法提供，为了访问容器中的数据，可以使用由容器类提供的迭代器。
二。迭代器（Iterator）:提供了访问容器中对象的方法。
三。算法(Algorithm):用来操作容器中的数据的模板函数。
四。仿函数(Function object)。
五。迭代适配器（Adapter）。
六。控件适配器(allocator)。
我们这里主要会讨论的是容器、迭代器和算法。这篇博客我们先来谈谈vector。相关的示例代码上传至[https://github.com/chenyufeng1991/STL_vector](https://github.com/chenyufeng1991/STL_vector)  。vector对应的数据结构为数组，而且是动态数组，也就是说我们不必关心该数组事先定义的容量是多少，它的大小会动态增长。与数组类似的是，我们可以在末尾进行元素的添加和删除，也可以进行元素值的随机访问和修改。首先要引入头文件 #include <vector>。具体使用如下：
（1）vector的创建

C++
```
vector<int> vec1; // 创建一个空的vector
vector<int> vec2(vec1); // 创建一个vector vec2,并用vec1去初始化vec2
vector<int> vec3(10); // 创建一个含有n个数据的vector
vector<int> vec4(10,0); // 创建含有10个数据的vector，并全部初始化为0
```
构造函数进行了重载，可以用多种方式创建vector。
（2）push_back(),pop_back()添加删除元素

C++
```
// 在vector尾部添加元素  
vec1.push_back(4);  
vec1.push_back(6);  
vec1.push_back(8);  
vec1.push_back(1);  
vec1.push_back(2);  
PrintVector(vec1);  
  
// 在vector尾部删除元素  
vec1.pop_back();  
PrintVector(vec1);  
  
// 在vector头部添加元素，无法完成，因为vector的数据结构为数组，无法在头部插入元素，否则需要整个数组前移；  
  
// 在vector头部删除元素，无法完成，理由同上。
```
可以使用push_back()不断的在vector尾部添加元素，使用pop_back删除尾部元素。操作非常的方便，比我们直接用数组结构方便多了。
（3）[],at(),取某位置的元素值


C++
```
// 取vector中某位置的元素值
    cout << "在1位置的元素值为：" << vec1.at(1) << endl;
    cout << "在1位置的元素值为：" << vec1[1] << endl;
```
因为vector的数据结构就是一个数组，所以可以进行随机访问。
（4）begin(),end(),指向头元素、尾元素的指针


C++
```
void PrintVector(vector<int> ve)
{
    cout << "Vector中的数据为：";
    vector<int>::iterator veIterator;
    for (veIterator = ve.begin(); veIterator < ve.end(); veIterator++)
    {
        cout << *veIterator << " ";
    }
    cout << endl;
}
```
这里以打印vector中的所有元素来理解begin().end()。这里需要使用迭代器来进行遍历，也可以把迭代器理解为指针，把begin()，end()分别理解为头指针和尾指针。这样就能访问到vector中的每一个元素了。
（5）back(),front(),访问头部元素和尾部元素


C++
```
// 返回尾部数据的引用
cout << "尾部数据的值为：" << vec1.back() << endl;
// 返回头部数据的引用
cout << "头部数据的值为：" << vec1.front() << endl;
```
（6）max_size(),vector的最大容纳量；size():vector当前的元素个数。


C++
```
cout << "vector中的最大容量为：" << vec1.max_size() << endl;
cout << "vector中的元素个数为：" << vec1.size() << endl;
```
（7）empty()，判断vector是否为空


C++
```
cout << "vector是否为空：" << vec1.empty() << endl;
```
如果为空，返回1.否则返回0。
（8）swap():交换两个vector中的值。
（9）sort():对vector升序排序；reverse()：对vector降序排序。


C++
```
// 对vector进行升序排序
    sort(vec1.begin(), vec1.end());
    PrintVector(vec1);
    // 对vector进行降序排序
    reverse(vec1.begin(), vec1.end());
    PrintVector(vec1);
```
传递的参数是需要排序的范围，因为这里要对整个vector排序，所以参数分别指向头部和尾部。
（10）[],at()：修改元素


C++
```
// 修改vector中的某个值
vec1[2] = 99;
PrintVector(vec1);
vec1.at(3) = 88;
PrintVector(vec1);
```
（11）erase()：删除某个元素

C++
```
// 删除数组的某个元素
// 为什么要使用iterator来进行定位，因为数组如果要删除一个元素或者插入一个元素，会导致其他元素移动，所以不能直接进行删除
vector<int>::iterator vItera = vec1.begin();
vItera = vItera + 2;
vec1.erase(vItera);
PrintVector(vec1);
```
在删除元素时，要借用迭代器。
（12）insert(): 插入元素

C++
```
// vector插入某元素，要使用iterator来定位某个位置
vector<int>::iterator vInsert = vec1.begin();
vInsert = vInsert + 2;
vec1.insert(vInsert, 777);
PrintVector(vec1);
```
同样要借助迭代器。
（13）clear():清除所有元素

C++
```
// 清除所有数据
   vec1.clear();
   PrintVector(vec1);
   cout << "vector是否为空：" << vec1.empty() << endl;
```
执行以后就是一个空的vector，不包含任何元素。
关于vector的更详细介绍可以参考文档：[http://en.cppreference.com/w/cpp/container/vector](http://en.cppreference.com/w/cpp/container/vector)
