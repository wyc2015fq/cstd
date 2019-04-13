
# C++基础——用C++实例理解UML类图 - 3-Number - CSDN博客


2017年12月14日 11:23:16[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：2552


目的：学习下UML类图的创建和认识
转载于：http://blog.csdn.net/lanchunhui/article/details/49557083
[类展示](http://blog.csdn.net/lanchunhui/article/details/49557083#%E7%B1%BB%E5%B1%95%E7%A4%BA)
[基本概念及术语](http://blog.csdn.net/lanchunhui/article/details/49557083#%E5%9F%BA%E6%9C%AC%E6%A6%82%E5%BF%B5%E5%8F%8A%E6%9C%AF%E8%AF%AD)[可见性visibility](http://blog.csdn.net/lanchunhui/article/details/49557083#%E5%8F%AF%E8%A7%81%E6%80%A7visibility)
[参数的方向parameter
 direction](http://blog.csdn.net/lanchunhui/article/details/49557083#%E5%8F%82%E6%95%B0%E7%9A%84%E6%96%B9%E5%90%91parameter-direction)
[类成员变量或者函数的类型](http://blog.csdn.net/lanchunhui/article/details/49557083#%E7%B1%BB%E6%88%90%E5%91%98%E5%8F%98%E9%87%8F%E6%88%96%E8%80%85%E5%87%BD%E6%95%B0%E7%9A%84%E7%B1%BB%E5%9E%8B)
[类关系](http://blog.csdn.net/lanchunhui/article/details/49557083#%E7%B1%BB%E5%85%B3%E7%B3%BB)
[关系的多重性multiplicity](http://blog.csdn.net/lanchunhui/article/details/49557083#%E5%85%B3%E7%B3%BB%E7%9A%84%E5%A4%9A%E9%87%8D%E6%80%A7multiplicity)
[类关系](http://blog.csdn.net/lanchunhui/article/details/49557083#%E7%B1%BB%E5%85%B3%E7%B3%BB-1)[assocationkonws
 a](http://blog.csdn.net/lanchunhui/article/details/49557083#assocationkonws-a)
[dependency](http://blog.csdn.net/lanchunhui/article/details/49557083#dependency)
[Aggregation](http://blog.csdn.net/lanchunhui/article/details/49557083#aggregation)
[Composition](http://blog.csdn.net/lanchunhui/article/details/49557083#composition)
[继承泛化](http://blog.csdn.net/lanchunhui/article/details/49557083#%E7%BB%A7%E6%89%BF%E6%B3%9B%E5%8C%96)
[类模板](http://blog.csdn.net/lanchunhui/article/details/49557083#%E7%B1%BB%E6%A8%A1%E6%9D%BF)
[References](http://blog.csdn.net/lanchunhui/article/details/49557083#references)

本文包括以下内容：
类间存在哪几种常见关系？
它们之间的区别和联系是什么？
如何在代码中反映类间的关系？
如何理解IN/OUTmode型的参数？
---

# 类展示
```python
class Circle
{
```
```python
private
```
```python
:
```
```python
double
```
```python
radius_;
    Point center_;
```
```python
public
```
```python
:
```
```python
void
```
```python
setRadius
```
```python
(
```
```python
double
```
```python
_radius);
```
```python
void
```
```python
setCenter(Point _center);
```
```python
double
```
```python
getArea()
```
```python
const
```
```python
;
```
```python
double
```
```python
getCircumfrence()
```
```python
const
```
```python
;
}
```
1
2
3
4
5
6
7
8
9
10
11

![这里写图片描述](https://img-blog.csdn.net/20151101114544097)

# 基本概念及术语
## 可见性（visibility）
‘+’ public
‘-’ private
‘\#’ protected
## 参数的方向（parameter direction）
‘in’：用于输入的参数，get the value
‘out’：用于输出的参数， set the value
‘inout’：既可作为输入又可作为输出， get the value and set the value
这种参数形式常见于，这样一种情况，传递的是一个指针变量（如果传递的是一个既非指针，也非引用的变量，就纯做输入（in）了），既然是指针变量，便可提领指针，修改其指向的内容，既具备提供输入，又具有修改原值 的双重属性。
举例如下：
对一个接口作如下documentation：
Prototype: ULONG GetActivationState( ULONG * pActivationState );
Parameters
```python
Type: ULONG*
Variable: pActivationState
Mode: IN/OUT
```
1
2
3
4
客户端代码
```python
ULONG
```
```python
activationState =
```
```python
1
```
```python
;
```
```python
ULONG
```
```python
result = GetActivationState(&activationState);
```
1
2
既然提供的是变量的地址，便可轻易地get the value 和 set the value。 所以根据`GetActivationState(ULONG*)`参数的IN/OUT状态，推测其中的代码如下：
```python
ULONG GetActivationState()
{
```
```python
if
```
```python
(*activationState ==
```
```python
1
```
```python
)
```
```python
// 可读属性，in
```
```python
{
```
```python
// TODO
```
```python
// 对变量值进行修改，例如，改为0
```
```python
*activationState =
```
```python
0
```
```python
;
```
```python
// 可写属性，out
```
```python
}
```
```python
return
```
```python
*activationState;
}
```
1
2
3
4
5
6
7
8
9
10
11
12
## 类成员（变量或者函数）的类型
静态成员，下划线表示
纯虚函数，斜体
## 类关系
Assocation (knows a)
Dependency (uses a)
Composition (has a)
Aggregation (has a)
Inheritance (is a)
Class template
## 关系的多重性（multiplicity）
‘0..1’， 无实例，或者1个实例
‘1’，只有一个实例
‘0..*’，
‘1..*’
# 类关系
## assocation（konws a）
一个对象知道另一个对象的存在，该对象持有另一个对象的指针或者引用。

![这里写图片描述](https://img-blog.csdn.net/20151101114617533)

```python
class X
{
```
```python
public
```
```python
:
```
```python
X
```
```python
(Y* _ptrY):
```
```python
ptrY_
```
```python
(_ptrY) {}
```
```python
void
```
```python
setY(Y *y) { ptrY_ = y;}
```
```python
void
```
```python
f() { ptrY_->foo();}
```
```python
private
```
```python
/
```
```python
public
```
```python
:
    Y* ptrY_;
```
```python
// X类持有Y的一个指针，可据此调用Y中的成员方法
```
```python
}
```
1
2
3
4
5
6
7
8
9
## dependency
当类Y与类X彼此独立，而类Y是类X成员函数的一个参数，或者X中成员函数的一个局部变量。

![这里写图片描述](https://img-blog.csdn.net/20151101114645236)

```python
class X
{
```
```python
...
```
```python
void f1(Y y) {
```
```python
...
```
```python
; y.foo(); }
    void f2(Y& y) {
```
```python
...
```
```python
; y.foo(); }
    void f3(Y* y) {
```
```python
...
```
```python
; y->foo(); }
    void f4() {
```
```python
...
```
```python
; Y y; y.foo(); }
    void f5() { Y::staticFoo();}
```
```python
...
```
```python
}
```
1
2
3
4
5
6
7
8
9
10
## Aggregation
聚类发生的场景是：一个类是另一个类的集合或者容器，但被包含的类与容器本身并不具备相同的生命期，也就是容器销毁时，其包含的内容未必。关联（association）与聚合（aggregation）的不同之处在于，关联关系不存在一个明确的容器包含另外一个类。比如汽车和轮胎的关系，就是一种聚合关系，汽车包含着轮胎的集合，汽车的销毁，轮胎仍然可以存在。

![这里写图片描述](https://img-blog.csdn.net/20151101114807530)

```python
class Window
{
public:
    //
```
```python
...
```
```python
private:
    vectr<Shape*> ptrShapes; 
}
```
1
2
3
4
5
6
7

![这里写图片描述](https://img-blog.csdn.net/20151101114825186)

## Composition
组合（composition）是聚合（Aggregation）的增强版。组合和聚合的根本不同在于类与其所持有元素的是否具有相同的生命期，要求必须相同是一种组合关系。

![这里写图片描述](https://img-blog.csdn.net/20151101114845820)

```python
class Circle
{
private:
```
```python
...
```
```python
Point center_;
```
```python
...
```
```python
}
```
1
2
3
4
5
6
7
8

![这里写图片描述](https://img-blog.csdn.net/20151101114929789)

```python
class X
{
```
```python
...
```
```python
Y y;        // composition, 相同的生命期，
            // 如何是引用或者指针类型，则是关联类型，是一种弱联系
Y b[
```
```python
10
```
```python
];    //
```
```python
0.
```
```python
..10, composition
}
class X
{
    X() { a = new Y[
```
```python
10
```
```python
]; }
    ~X() { delete[] a;}
```
```python
...
```
```python
Y *a;       //
```
```python
0.
```
```python
..10, composition，X对象析构时，必须对a进行释放
}
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
## 继承（泛化）
类间的继承关系表达的是一种derived class B is a base class A。

![这里写图片描述](https://img-blog.csdn.net/20151101114955505)

![这里写图片描述](https://img-blog.csdn.net/20151101115015293)

## 类模板
类模板意味着泛型类。

![这里写图片描述](https://img-blog.csdn.net/20151101115032163)

```python
template<class
```
```python
T
```
```python
>
class X
{
```
```python
...
```
```python
};
X<Y> a;
```
1
2
3
4
5
6
# References
[1][UML Class Diagram Explained With C++ samples](https://cppcodetips.wordpress.com/2013/12/23/uml-class-diagram-explained-with-c-samples/)
[2][IN/OUT Parameters and how to work with them
 in C++](http://stackoverflow.com/questions/6900035/in-out-parameters-and-how-to-work-with-them-in-c)

