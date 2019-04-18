# C++函数返回对象效率以及使用建议 - Likes的博客 - CSDN博客
2019年01月25日 14:13:23[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：23
函数的传参和传返回值，分为两种模式：传值和传引用。
传值就是传整个对象，传引用就是传对象的引用（地址）。当对象本身比较小时，可以直接传值，但当对象比较大时我们一般会传引用以节省内存和减少拷贝。这是C++的机制，在java中都是传引用，所以不用像C++一样区分值语义和引用语义。
对于传参一般建议都使用传引用，但对于返回值而言，我们不能直接返回对象引用，因为，函数调用完后会清栈，引用指飞。但传值我们又担心临时变量的拷贝降低性能，所以我们可能写出如下代码，返回值用指针包裹。
```cpp
shared_ptr<A> fun()
{
    make_shared<A> ptr;
    ...
    return ptr;
}
```
这当然万无一失，但明显变麻烦了。实际上我们可以放心大胆地返回对象。下面进行论述：
C/C++函数返回对象的原理：
```cpp
A fun2()
{
    A a1();
    ...
    return a1;
}
void fun1()
{
    A a2=fun2();
}
```
a1是在函数fun2中堆栈中的对象，在return时，会将其拷贝构造到外层函数fun1的堆栈中（这个对象是一个匿名对象，是一个右值，马上会被析构），之后将其拷贝构造产生对象a2。这是没有任何编译器优化的情况。
一般情况下，编译器会采用RVO（return value optimization）优化，参看《深度探索C++对象模型》。下面是一个例子
```cpp
//原函数
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
//////////////////////////
////编译器优化后的示例代码
//////////////////////////
factory(const Point3d &_result)
{
    Point3d po;
    po.Point3d::Point3d(1,2,3);
    _result.Point3d::Point3d(po);   //用po拷贝构造_result;
    po.Point3d::~Point3d();         //po对象析构
    return;
}
int main()
{
    Point3d _result;
    factory(_result);
    Point3d p=result;
    return 1;
}
```
大体意思是说，会在值传递返回值的函数中，将返回值改写成传引用参数。这样可以少一次临时变量的构造。这是编译器优化时帮我们做的。
总结起来就是放心大胆地返回一个对象实体，这并不会有多余的开销，编译器会帮我们优化。
