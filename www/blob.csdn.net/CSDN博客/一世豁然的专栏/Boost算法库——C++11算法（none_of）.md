# Boost算法库——C++11算法（none_of） - 一世豁然的专栏 - CSDN博客





2017年10月31日 12:05:56[一世豁然](https://me.csdn.net/Explorer_day)阅读数：345








头文件'boost / algorithm / cxx11 / none_of.hpp'包含单个算法的四个变体，none_of。 该算法测试序列的所有元素，如果它们没有共享属性，则返回true。




例程none_of采用序列和谓词。 如果谓词在应用于序列中的每个元素时返回false，它将返回true。





例程none_of_equal需要一个序列和一个值。 如果序列中的所有元素都不等于传入值，则返回true。





这两个例程有两种形式; 第一个需要两个迭代器来定义范围。 第二种形式采用单一范围参数，并使用Boost.Range来遍历它。








一、接口

如果序列中的每个项目的谓词返回false，则函数none_of返回true。 有两个版本，一个需要两个迭代器，另一个需要一个范围。




```cpp
namespace boost { namespace algorithm {
template<typename InputIterator, typename Predicate>
	bool none_of ( InputIterator first, InputIterator last, Predicate p );
template<typename Range, typename Predicate>
	bool none_of ( const Range &r, Predicate p );
}}
```


函数none_of_equal类似于none_of，而不是使用谓词来测试序列的元素，它需要一个值进行比较。



```cpp
namespace boost { namespace algorithm {
template<typename InputIterator, typename V>
	bool none_of_equal ( InputIterator first, InputIterator last, V const &val );
template<typename Range, typename V>
	bool none_of_equal ( const Range &r, V const &val );
}}
```



二、实例

给定包含{0，1，2，3，14，15}的容器c




```cpp
bool isOdd ( int i ) { return i % 2 == 1; }
bool lessThan10 ( int i ) { return i < 10; }

using boost::algorithm;

none_of ( c, isOdd ) --> false
none_of ( c.begin (), c.end (), lessThan10 ) --> false
none_of ( c.begin () + 4, c.end (), lessThan10 ) --> true
none_of ( c.end (), c.end (), isOdd ) --> true  // empty range
none_of_equal ( c, 3 ) --> false
none_of_equal ( c.begin (), c.begin () + 3, 3 ) --> true
none_of_equal ( c.begin (), c.begin (), 99 ) --> true  // empty range
```



三、迭代器要求

除了输出迭代器之外，所有迭代器都是none_of和none_of_equal。








四、复杂度

所有变量none_of和none_of_equal都以O（N）（线性）时间运行; 也就是说，它们与列表中的每个元素进行比较。 如果任何比较成功，算法将立即终止，而不检查序列的剩余成员。








五、安全异常

none_of和none_of_equal的所有变体都通过value或const引用来获取参数，而不依赖于任何全局状态。 因此，此文件中的所有例程都提供强大的异常保证。








六、注意

1、例程no_of也可作为C ++ 11标准的一部分。

2、none_of和none_of_equal都为空范围返回true，无论传递给test来测试。

3、none_of_value的第二个参数是一个模板参数，而不是从第一个参数（std :: iterator_traits <InputIterator> :: value_type）推导出来，因为这样可以为调用者提供更大的灵活性，并且利用内置的由迭代器指向的比较类型。 该函数被定义为返回true，如果对于序列中的所有元素，表达式* iter == val计算为false（其中iter是序列中每个元素的迭代器）



