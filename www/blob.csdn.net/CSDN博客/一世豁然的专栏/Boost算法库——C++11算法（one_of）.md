# Boost算法库——C++11算法（one_of） - 一世豁然的专栏 - CSDN博客





2017年10月31日 12:14:23[一世豁然](https://me.csdn.net/Explorer_day)阅读数：264








头文件'boost / algorithm / cxx11 / one_of.hpp'包含单个算法的四个变体，one_of。 该算法测试序列的元素，如果序列中的一个元素具有特定属性，则返回true。




例程one_of采用序列和谓词。 如果谓词对序列中的一个元素返回true，则返回true。





例程one_of_equal需要一个序列和一个值。 如果序列中的一个元素比较等于传入值，它将返回true。





这两个例程有两种形式; 第一个需要两个迭代器来定义范围。 第二种形式采用单一范围参数，并使用Boost.Range来遍历它。








一、接口

如果谓词对于序列中的一个项目返回true，则函数one_of返回true。 有两个版本，一个需要两个迭代器，另一个需要一个范围。




```cpp
namespace boost { namespace algorithm {
template<typename InputIterator, typename Predicate>
	bool one_of ( InputIterator first, InputIterator last, Predicate p );
template<typename Range, typename Predicate>
	bool one_of ( const Range &r, Predicate p );
}}
```


函数one_of_equal类似于one_of，而不是使用谓词来测试序列的元素，它需要一个值进行比较。



```cpp
namespace boost { namespace algorithm {
template<typename InputIterator, typename V>
	bool one_of_equal ( InputIterator first, InputIterator last, V const &val );
template<typename Range, typename V>
	bool one_of_equal ( const Range &r, V const &val );
}}
```







三、实例

给定包含{0，1，2，3，14，15}的容器c



```cpp
bool isOdd ( int i ) { return i % 2 == 1; }
bool lessThan10 ( int i ) { return i < 10; }

using boost::algorithm;
one_of ( c, isOdd ) --> false
one_of ( c.begin (), c.end (), lessThan10 ) --> false
one_of ( c.begin () + 3, c.end (), lessThan10 ) --> true
one_of ( c.end (), c.end (), isOdd ) --> false  // empty range
one_of_equal ( c, 3 ) --> true
one_of_equal ( c.begin (), c.begin () + 3, 3 ) --> false
one_of_equal ( c.begin (), c.begin (), 99 ) --> false  // empty range
```



四、迭代器要求

除了输出迭代器之外，所有迭代器都有one_of和one_of_equal工作。





五、复杂度

所有变量的一个和一个_等于在O（N）（线性）时间运行; 也就是说，它们与列表中的每个元素进行比较。 如果序列中的多个元素满足条件，则算法将立即返回false，而不检查序列的剩余成员。





六、安全异常

one_of和one_of_equal的所有变体都通过value或const引用获取参数，而不依赖于任何全局状态。 因此，此文件中的所有例程都提供强大的异常保证。








七、注意

1、one_of和one_of_equal都为空范围返回false，无论传递给test来测试。

2、one_of_equal的第二个参数是一个模板参数，而不是从第一个参数（std :: iterator_traits <InputIterator> :: value_type）推导出来，因为这样可以为调用者提供更大的灵活性，并利用内置的由迭代器指向的比较类型。 该函数被定义为返回true，如果对于序列中的一个元素，表达式* iter == val计算为true（其中，iter是序列中每个元素的迭代器）



