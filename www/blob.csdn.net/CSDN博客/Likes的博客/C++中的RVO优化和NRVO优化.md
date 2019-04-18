# C++中的RVO优化和NRVO优化 - Likes的博客 - CSDN博客
2018年11月01日 16:02:28[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：73
RVO (return value optimization) 和NRVO (named return value optimization) 是C++在处理一个函数返回类对象并将返回值赋给另一个对象时，为了减少拷贝构造次数以及析构次数而采用的一种编译器优化技术。 
本篇博客以《深度探索C++对象模型》中例子来总结这两种优化技术。代码片段如下：
```cpp
class Point3d
{
public:
    int m_x;
    int m_y;
    int m_z;
public:
    Point3d(int x,int y,int z):m_x(x),m_y(y),m_z(z)
    {
        cout << "constructor"<<endl;
    }
    ~Point3d()
    {
        cout << "deconstructor"<<endl;
    }
    Point3d(const Ponint3d &other)
    {
        this.m_x = other.m_x;
        this.m_y = other.m_y;
        this.m_z = other.m_z;
        cout << "copy constructor"<<endl;
    }
    Point3d &operator=(const Point3d &other)
    {
        if(this != &other)
        {
            this.m_x = other.m_x;
            this.m_y = other.m_y;
            this.m_z = other.m_z;
        }
        cout << "operator="<<endl;
        return *this;
    }
}
Point3d factory()
{
    Point3d po(1,2,3)
    return po;
}
int main()
{
    Point3d p = factory();
    return 1;
}
```
这个例子中的Point3d类中显式定义了构造函数、拷贝构造函数、析构函数以及重载了赋值运算符，并在内部加入了一些打印信息来查看这些函数的调用情况。
### 1）不做任何返回值优化。
在g++中有一个选项-fno-elide-constructors可以去掉任何返回值优化。最终打印情况如下：
```cpp
constructor      //factory函数中构造po对象
copy constructor //factory函数中用po对象拷贝构造临时对象_temp
deconstructor    //factory函数中返回时，析构掉局部对象po
copy constructor //main函数中用factory函数中拷贝构造的临时对象拷贝构造对象p
deconstructor    //析构临时对象_temp
deconstructor    //main函数结束时，析构对象p
```
可以看出，不做任何优化的话就是有两次拷贝构造和析构，对照注释很容易理解 
### 2）RVO优化
VS在debug模式下使用RVO而不用NRVO
对应的输出如下：
```cpp
constructor      
copy constructor 
deconstructor         
deconstructor       //main函数中p的析构
```
在《在深度探索C++对象模型》书中，对应的解释为，编译器将factor函数改写为了如下的伪代码形式：
```cpp
void factory(const Point3d &_result)
{
    Point3d po;
    po.Point3d::Point3d(1,2,3);
    _result.Point3d::Point3d(po);   //用po拷贝构造_result;
    po.Point3d::~Point3d();         //po对象析构
    return;
}
```
而main函数中赋值语句改写为：
```cpp
int main()
{
    Point3d p;
    factory(p);
    return 0;
}
```
这样将p直接做为参数，在factory()函数中进行构造，就不需要先将po拷贝构造出临时对象_temp，再析构_temp。 
### 3）NRVO优化
编译器将factory()函数改写为如下形式:
```cpp
Point3d factor()
{
    return Point3d(1,2,3);
}
```
### 4）同时做RVO优化和NROV优化
factory()函数的大致伪代码实现就会改写为：
```cpp
void factory(const Point3d &_result)
{
    _result.Point3d::Point3d(1,2,3);  //直接将p作为参数构造
    return;
}
```
相应的打印情况，就为：
```cpp
constructor    
deconstructor
```
