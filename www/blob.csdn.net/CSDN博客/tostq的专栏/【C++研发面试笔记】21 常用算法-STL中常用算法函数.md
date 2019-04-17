# 【C++研发面试笔记】21. 常用算法-STL中常用算法函数 - tostq的专栏 - CSDN博客





2016年10月04日 11:24:17[tostq](https://me.csdn.net/tostq)阅读数：1149
所属专栏：[C++研发面试笔记](https://blog.csdn.net/column/details/15031.html)









# 【C++研发面试笔记】21. 常用算法-STL中常用算法函数

**1、for_each(容器起始地址，容器结束地址，要执行的函数)**

指定函数依次对指定范围内所有元素进行迭代访问，返回所指定的函数类型。

**2、查找find**
`InputIterator find (InputIterator first, InputIterator last, const T& val);`

前闭后合的区间 begin，end中，查找value如果查找到了就返回第一个符合条件的元素，否则返回end指针
**3、数值运算（`<numeric>`）**
`T accumulate (InputIterator first, InputIterator last, T init)`

计算一个范围内的累积和 
`OutputIterator adjacent_difference (InputIterator first, InputIterator last, OutputIterator result)`

计算相邻差（y0=x0, y1=x1-x2, y2=x2-x1…） 
`T inner_product (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init)`

计算内积（y=init+x0*x0+x1*x1+…）
**4、判断是否相等**
`bool equal ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2 )`

**5、查找**
`ForwardIterator1 search ( ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)` 找到所示子序列的位置

**6、复制**
`OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result)`

**7、排序**
`void sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp);`

cmp函数的两种使用方式 
`bool myfunction (int i,int j) { return (i<j); }`

```
struct myclass { 

  bool operator() (int i,int j) { return (i<j);} 

} myobject; 

`std::sort (myvector.begin()+4, myvector.end(), myfunction);`
`std::sort (myvector.begin(), myvector.end(), myobject);`
**8、交换**
`template <class T> void swap ( T& a, T& b )`

**9、取代**
`void replace (ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)`

**10、反转**
`void reverse (BidirectionalIterator first, BidirectionalIterator last)`

**11、统计元素值为val的数目**
`difference_type count (InputIterator first, InputIterator last, const T& val)`

**12、最值比较**
`const T& min (const T& a, const T& b)`
`const T& max (const T& a, const T& b)`
```






