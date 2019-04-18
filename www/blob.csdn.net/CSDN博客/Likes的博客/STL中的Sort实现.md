# STL中的Sort实现 - Likes的博客 - CSDN博客
2019年04月09日 10:33:49[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：5标签：[STL																[sort](https://so.csdn.net/so/search/s.do?q=sort&t=blog)](https://so.csdn.net/so/search/s.do?q=STL&t=blog)
个人分类：[STL](https://blog.csdn.net/songchuwang1868/article/category/7922234)
## [https://www.cnblogs.com/fengcc/p/5256337.html](https://www.cnblogs.com/fengcc/p/5256337.html)
## 函数声明
```cpp
#include <algorithm>
 
template< class RandomIt >
void sort( RandomIt first, RandomIt last );
 
template< class RandomIt, class Compare >
void sort( RandomIt first, RandomIt last, Compare comp );
```
使用方法非常简单，`STL`提供了两种调用方式，一种是使用默认的`<`操作符比较，一种可以自定义比较函数。可是为什么它通常比我们自己写的排序要快那么多呢？
## 实现原理
原来，`STL`中的`sort`并非只是普通的快速排序，除了对普通的**快速排序**进行优化，它还结合了**插入排序**和**堆排序**。根据不同的数量级别以及不同情况，能自动选用合适的排序方法。当数据量较大时采用快速排序，分段递归。一旦分段后的数据量小于某个阀值，为避免递归调用带来过大的额外负荷，便会改用插入排序。而如果递归层次过深，有出现最坏情况的倾向，还会改用堆排序。
### 普通的快速排序
普通快速排序算法可以叙述如下，假设S代表需要被排序的数据序列：
- 如果`S`中的元素只有0个或1个，结束。
- 取`S`中的任何一个元素作为枢轴`pivot`。
- 将`S`分割为`L`、`R`两端，使`L`内的元素都小于等于`pivot`，`R`内的元素都大于等于`pivot`。
- 对`L`、`R`递归执行上述过程。
快速排序最关键的地方在于枢轴的选择，最坏的情况发生在分割时产生了一个空的区间，这样就完全没有达到分割的效果。`STL`采用的做法称为`median-of-three`，即取整个序列的首、尾、中央三个地方的元素，以其中值作为枢轴。
分割的方法通常采用两个迭代器`head`和`tail`，`head`从头端往尾端移动，`tail`从尾端往头端移动，当`head`遇到大于等于`pivot`的元素就停下来，`tail`遇到小于等于`pivot`的元素也停下来，若`head`迭代器仍然小于`tail`迭代器，即两者没有交叉，则互换元素，然后继续进行相同的动作，向中间逼近，直到两个迭代器交叉，结束一次分割。
看一张来自维基百科上关于[快速排序](https://zh.wikipedia.org/zh-cn/%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F)的动态图片，帮助理解。
![](http://7xqbsh.com1.z0.glb.clouddn.com/Sorting_quicksort_anim.gif)
### 内省式排序 Introsort
不当的枢轴选择，导致不当的分割，会使快速排序恶化为 O(n2)。David R.Musser于1996年提出一种混合式排序算法：`Introspective Sorting`（内省式排序），简称`IntroSort`，其行为大部分与上面所说的`median-of-three Quick Sort`完全相同，但是当分割行为有恶化为二次方的倾向时（递归层次超过阈值），能够自我侦测，转而改用堆排序，使效率维持在堆排序的 O(nlgn)，又比一开始就使用堆排序来得好。
## 代码分析
下面是完整的`SGI STL sort()`源码（使用默认`<`操作符版）
```cpp
template <class _RandomAccessIter>
inline void sort(_RandomAccessIter __first, _RandomAccessIter __last) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  if (__first != __last) {
    __introsort_loop(__first, __last,
                     __VALUE_TYPE(__first),
                     __lg(__last - __first) * 2);
    __final_insertion_sort(__first, __last);
  }
}
```
其中，`__introsort_loop`便是上面介绍的内省式排序，其第三个参数中所调用的函数`__lg()`便是用来控制分割恶化情况，代码如下：
```cpp
template <class Size>
inline Size __lg(Size n) {
    Size k;
    for (k = 0; n > 1; n >>= 1) ++k;
    return k;
}
```
即求`lg(n)`（取下整），意味着快速排序的递归调用最多 2*lg(n) 层。
内省式排序算法如下：
```cpp
template <class _RandomAccessIter, class _Tp, class _Size>
void __introsort_loop(_RandomAccessIter __first,
                      _RandomAccessIter __last, _Tp*,
                      _Size __depth_limit)
{
  while (__last - __first > __stl_threshold) {
    if (__depth_limit == 0) {
      partial_sort(__first, __last, __last);
      return;
    }
    --__depth_limit;
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2),
                                         *(__last - 1))));
    __introsort_loop(__cut, __last, (_Tp*) 0, __depth_limit);
    __last = __cut;
  }
}
```
- 首先判断元素规模是否大于阀值`__stl_threshold`，`__stl_threshold`是一个常整形的全局变量，值为16，表示若元素规模小于等于16，则结束内省式排序算法，返回`sort`函数，改用插入排序。
- 若元素规模大于`__stl_threshold`，则判断递归调用深度是否超过限制。若已经到达最大限制层次的递归调用，则改用堆排序。代码中的`partial_sort`即用堆排序实现。
- 若没有超过递归调用深度，则调用函数`__unguarded_partition()`对当前元素做一趟快速排序，并返回枢轴位置。`__unguarded_partition()`函数采用的便是上面所讲的使用两个迭代器的方法，代码如下：
```cpp
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, 
                                        _RandomAccessIter __last, 
                                        _Tp __pivot) 
{
    while (true) {
        while (*__first < __pivot)
            ++__first;
        --__last;
        while (__pivot < *__last)
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
```
- 经过一趟快速排序后，再递归对右半部分调用内省式排序算法。然后回到while循环，对左半部分进行排序。源码写法和我们一般的写法不同，但原理是一样的，需要注意。
递归上述过程，直到元素规模小于`__stl_threshold`，然后返回`sort`函数，对整个元素序列调用一次插入排序，此时序列中的元素已基本有序，所以插入排序也很快。至此，整个`sort`函数运行结束。
