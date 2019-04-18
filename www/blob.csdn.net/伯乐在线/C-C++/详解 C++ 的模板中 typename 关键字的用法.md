# 详解 C++ 的模板中 typename 关键字的用法 - 文章 - 伯乐在线
原文出处： [Hunk Shi，2016-06-24](http://www.jb51.net/article/87329.htm)
#### typename的使用场合
** 用处1, 用在模板定义里, 标明其后的模板参数是类型参数。**
例如

```
template<typename T, typename Y>
T foo(const T& t, const Y& y){//....};
templace<typename T>
class CTest
{
private:
 T t;
public:
 //...
}
```
其实，这里最常用的是使用关键字class，而且二者功能完全相同，这里的class和定义类时的class完全是两回事，C++当时就是为了减少关键字，才使用了class。但最终却不得不引入了typename，究竟是
什么原因呢？请看第二条，也就是typename的第二个用法。
**用处2, 模板中标明“内嵌依赖类型名”**
这里有三个词，内嵌、依赖、类型名。那么什么是“内嵌依赖类型名(nested dependent type name)”？
请看SGI STL里的一个例子, 只是STL中count范型算法的实现：

```
template <class _InputIter, class _Tp>
typename iterator_traits<_InputIter>::difference_type
count(_InputIter __first, _InputIter __last, const _Tp& __value) {
 __STL_REQUIRES(_InputIter, _InputIterator);
 __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type,
         _EqualityComparable);
 __STL_REQUIRES(_Tp, _EqualityComparable);
 typename iterator_traits<_InputIter>::difference_type __n = 0;
 for ( ; __first != __last; ++__first)
  if (*__first == __value)
   ++__n;
 return __n;
}
```
这里有三个地方用到了typename：返回值、参数、变量定义。分别是：

```
typename iterator_traits<_InputIter>::difference_type
typename iterator_traits<_InputIter>::value_type
typename iterator_traits<_InputIter>::difference_type __n = 0;
```
difference_type， value_type就是依赖于_InputIter（模板类型参数）的类型名。源码如下：

```
template <class _Iterator>
struct iterator_traits {
 typedef typename _Iterator::iterator_category iterator_category;
 typedef typename _Iterator::value_type    value_type;
 typedef typename _Iterator::difference_type  difference_type;
 typedef typename _Iterator::pointer      pointer;
 typedef typename _Iterator::reference     reference;
};
```
内嵌是指定义在类名的定义中的。以上difference_type和value_type都是定义在iterator_traits中的。
依赖是指依赖于一个模板参数。 typename iterator_traits::difference_type中difference_type依赖于模板参数_InputIter。
类型名是指这里最终要指出的是个类型名，而不是变量。例如iterator_traits::difference_type完全有可能是类iterator_traits 类里的一个static对象。而且当我们这样写的时候，C＋＋默认就是解释为一个变量的。所以，为了和变量区分，必须使用typename告诉编译器。
那么是不是所有的T::type_or_variable, 或者tmpl:type_or_variable都需要使用typename呢？不是，有以下两个例外。
#### 例外
**（1）类模板定义中的基类列表。**
例如

```
template<class T>
class Derived: public Base<T>::XXX
{
  ...
}
```
**（2）类模板定义中的初始化列表。**

```
Derived(int x) : Base<T>::xxx(x)
{
  ...
}
```
为什么这里不需要呢？因为编译器知道这里需要的是类型还是变量，（1）基类列表里肯定是类型名，（2）初始化列表里肯定是成员变量名。
#### typename和class的区别
在c++ Template中很多地方都用到了typename与class这两个关键字，而且好像可以替换，是不是这两个关键字完全一样呢?
相信学习C++的人对class这个关键字都非常明白，class用于定义类，在模板引入c++后，最初定义模板的方法为： template<class T>……
在这里class关键字表明T是一个类型，后来为了避免class在这两个地方的使用可能给人带来混淆，所以引入了typename这个关键字，它的作用同
class一样表明后面的符号为一个类型，这样在定义模板的时候就可以使用下面的方式了：
template<typename T>……
在模板定义语法中关键字class与typename的作用完全一样。
typename难道仅仅在模板定义中起作用吗？其实不是这样，typename另外一个作用为：使用嵌套依赖类型(nested depended name)，如下所示：

```
class MyArray 
{ 
  public：
  typedef int LengthType;
  .....
}
template<class T>
void MyMethod( T myarr ) 
{ 
  typedef typename T::LengthType LengthType; 
  LengthType length = myarr.GetLength; 
}
```
这个时候typename的作用就是告诉c++编译器，typename后面的字符串为一个类型名称，而不是成员函数或者成员变量，这个时候如果前面没有typename，编译器没有任何办法知道T::LengthType是一个类型还是一个成员名称(静态数据成员或者静态函数)，所以编译不能够通过。
