# Boost算法库——C++11算法（is_sorted） - 一世豁然的专栏 - CSDN博客





2017年11月06日 11:38:02[一世豁然](https://me.csdn.net/Explorer_day)阅读数：374








头文件<boost / algorithm / cxx11 / is_sorted.hpp>包含确定序列是否有序的函数。







一、is_sorted

函数is_sorted（sequence）决定一个序列是否按照某种标准完全排序。 如果没有指定比较谓词，则使用std :: less_equal（即，测试是查看序列是否不减少）




```cpp
namespace boost { namespace algorithm {
	template <typename ForwardIterator, typename Pred>
	bool is_sorted ( ForwardIterator first, ForwardIterator last, Pred p );
	
	template <typename ForwardIterator>
	bool is_sorted ( ForwardIterator first, ForwardIterator last );
	
	
	template <typename Range, typename Pred>
	bool is_sorted ( const Range &r, Pred p );
	
	template <typename Range>
	bool is_sorted ( const Range &r );
}}
```


迭代器要求：is_sorted函数将在forward迭代器上工作或更好。







二、is_sorted_until

如果distance（first，last）<2，则is_sorted（first，last）返回最后一个。 否则，它将返回[first，last]中对[first，i]进行排序的最后一个迭代器。





简而言之，它返回序列中“失序”的元素。 如果整个序列被排序（根据谓词），那么它将返回最后一个。




```cpp
namespace boost { namespace algorithm {
	template <typename ForwardIterator, typename Pred>
	FI is_sorted_until ( ForwardIterator first, ForwardIterator last, Pred p );
	
	template <typename ForwardIterator>
	ForwardIterator is_sorted_until ( ForwardIterator first, ForwardIterator last );
	
	
	template <typename Range, typename Pred>
	typename boost::range_iterator<const R>::type is_sorted_until ( const Range &r, Pred p );
	
	template <typename Range>
	typename boost::range_iterator<const R>::type is_sorted_until ( const Range &r );
}}
```


迭代器要求：is_sorted_until函数将在forward迭代器上工作或更好。 由于他们必须在输入序列中返回一个地方，所以输入迭代器是不够的。




复杂度：is_sorted_until将最多N-1次调用谓词（给定一个长度为N的序列）。





例子：

给定序列{1,2,3,4,5,3}，is_sorted_until（beg，end，std :: less <int>（））将返回指向第二个3的迭代器。





给定序列{1,2,3,4,5,9}，is_sorted_until（beg，end，std :: less <int>（））将返回结束。





is_ordered也有一套“包装函数”，可以很容易地看到整个序列是否被排序。 这些函数返回一个布尔值，指示成功或失败，而不是迭代器，找到乱序项目的位置。





要测试一个序列是否正在增加（每个元素至少与前一个一样大）：




```cpp
namespace boost { namespace algorithm {
	template <typename Iterator>
	bool is_increasing ( Iterator first, Iterator last );
	
	template <typename R>
	bool is_increasing ( const R &range );
}}
```


测试序列是否正在减少（每个元素不大于前一个）：



```cpp
namespace boost { namespace algorithm {
	template <typename ForwardIterator>
	bool is_decreasing ( ForwardIterator first, ForwardIterator last );
	
	template <typename R>
	bool is_decreasing ( const R &range );
}}
```


要测试序列是否严格增加（每个元素大于前一个）：



```cpp
namespace boost { namespace algorithm {
	template <typename ForwardIterator>
	bool is_strictly_increasing ( ForwardIterator first, ForwardIterator last );
	
	template <typename R>
	bool is_strictly_increasing ( const R &range );
}}
```


要测试一个序列是否严格减少（每个元素小于前一个）：



```cpp
namespace boost { namespace algorithm {
	template <typename ForwardIterator>
	bool is_strictly_decreasing ( ForwardIterator first, ForwardIterator last );
	
	template <typename R>
	bool is_strictly_decreasing ( const R &range );
}}
```


复杂性：这些调用中的每一个都是is_sorted的简单包装，因此它们与is_sorted具有相同的复杂性。







三、注意

1、例程is_sorted和is_sorted_until是C ++ 11标准的一部分。 使用C ++ 11实现进行编译时，将使用标准库中的实现。

2、is_sorted和is_sorted_until对于长度为1的空范围和范围均返回true。



