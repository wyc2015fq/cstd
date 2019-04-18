# Boost算法库——C++11算法（all_of） - 一世豁然的专栏 - CSDN博客





2017年10月30日 18:26:50[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1104








头文件'boost / algorithm / cxx11 / all_of.hpp'包含单个算法all_of的四个变体。 该算法测试序列的所有元素，如果它们共享一个属性，则返回true。




例程all_of需要一个序列和一个谓词。 如果谓词在应用于序列中的每个元素时返回true，则返回true。





例程all_of_equal需要一个序列和一个值。 如果序列中的每个元素比较等于传入值，它将返回true。





这两个例程有两种形式; 第一个需要两个迭代器来定义范围。 第二种形式采用单一范围参数，并使用Boost.Range来遍历它。








一、接口

如果谓词对于序列中的每个项目都返回true，那么函数all_of返回true。 有两个版本 一个需要两个迭代器，另一个需要一个范围。




```cpp
namespace boost { namespace algorithm {
template<typename InputIterator, typename Predicate>
	bool all_of ( InputIterator first, InputIterator last, Predicate p );
template<typename Range, typename Predicate>
	bool all_of ( const Range &r, Predicate p );
}}
```




函数all_of_equal类似于all_of，而不是使用谓词来测试序列的元素，它需要一个值进行比较。



```cpp
namespace boost { namespace algorithm {
template<typename InputIterator, typename V>
	bool all_of_equal ( InputIterator first, InputIterator last, V const &val );
template<typename Range, typename V>
	bool all_of_equal ( const Range &r, V const &val );
}}
```






二、实例

给定包含{0，1，2，3，14，15}的容器c



```cpp
bool isOdd ( int i ) { return i % 2 == 1; }
bool lessThan10 ( int i ) { return i < 10; }

using boost::algorithm;
all_of ( c, isOdd ) --> false
all_of ( c.begin (), c.end (), lessThan10 ) --> false
all_of ( c.begin (), c.begin () + 3, lessThan10 ) --> true
all_of ( c.end (), c.end (), isOdd ) --> true  // empty range
all_of_equal ( c, 3 ) --> false
all_of_equal ( c.begin () + 3, c.begin () + 4, 3 ) --> true
all_of_equal ( c.begin (), c.begin (), 99 ) --> true  // empty range
```



三、迭代器要求

all_of和all_of_equal在除输出迭代器之外的所有迭代器上工作。








四、复杂度

all_of和all_of_equal的所有变体都以O（N）（线性）时间运行; 也就是说，它们与列表中的每个元素进行比较。 如果任何比较失败，算法将立即终止，而不检查序列的剩余成员。








五、安全异常

all_of和all_of_equal的所有变体都通过value或const引用来获取它们的参数，而不依赖于任何全局状态。 因此，此文件中的所有例程都提供强大的异常保证。








六、注意

1、例程all_of也可作为C ++ 11标准的一部分。

2、all_of和all_of_equal都为空范围返回true，无论传递给test来测试。 当序列中没有要测试的项目时，它们都满足要测试的条件。

3、all_of_value的第二个参数是一个模板参数，而不是从第一个参数（std :: iterator_traits <InputIterator> :: value_type）推导出来，因为这样可以为调用者提供更多的灵活性，并利用内置的由迭代器指向的比较类型。 该函数被定义为返回true，如果对于序列中的所有元素，表达式* iter == val计算为true（其中，iter是序列中每个元素的迭代器）



