# STL-常用函数 - KinboSong的博客 - CSDN博客
2017年03月26日 18:39:32[songjinbo3](https://me.csdn.net/KinboSong)阅读数：144
参考：[http://www.cplusplus.com/reference/algorithm/copy_if/](http://www.cplusplus.com/reference/algorithm/copy_if/)
1、count系列
（1）int count(iterator first,iterator last,val)
（2）int count_if(iterator first,iterator last,pred_fun)
2、copy系列
（1）iterator copy(iterator first,iterator last,iterator result)
（2）iterator copy_n(iterator first,int n,iterator result)
（3）iterator copy_if(iterator first,iterator last,iterator result,pred_fun)
3、equal
（1）bool equal(iterator first1,iterator last1,iterator first2)：把fisrt1-last1范围内的元素和以first2开始的位置做比较。
4、find系列
（1）iterator find(iterator first,iterator last, val)：返回first-last之间第一个等于val的元素的迭代器
（2）iterator find_if(iterator first,iterator last,pre_fun)：返回first-last之间第一个满足条件pre_fun的元素的迭代器
（3）iterator find_end(iterator first1,iterator last1,iterator first2,iterator last2)：返回first1-last1之间最后一个等于first2-last2的序列的首迭代器
5、for_each
（1）void for_each(iterator first,iterator last,function fn)：对first-last范围内的每一个元素调用函数fn
6、is_sorted系列
（1）bool is_sorted(iterator first,iterator last)：若first-last之间的值是升序排列，则返回true；反之返回false
（2）iterator is_sorted_until(iterator first,iterator last)：返回first-last之间第一个不遵循升序规则的元素的iterator
7、lower_bound & upper_bound
（1）iterator lower_bound(iterator first,iterator last,val)：返回first-last之间第一个大于或等于val的元素的迭代器
（2）iterator upper_bound(iterator first,iterator last ,val)：返回first-last之间第一个大于val的元素的迭代器
8、max & min系列
（1）T max(T a,T b)：返回a和b中较大的值
（2）iterator max_element(iterator first,iterator last)：返回first-last范围内最大的元素的迭代器
（3）T min(T a,T b)：返回a和b中较小的值
（4）iterator min_element(iterator first,iterator last)：返回first-last范围内最小的元素的迭代器
（5）auto result = minmax(<list> il):返回il中的最小值和最大值 ，result->first和result->second分别是最小值和最大值
（6）auto result = minmax_element(iterator first,iterator last)：返回first-last之间的最小值和最大值
```
```
`9、swap`
`（1）swap（a,b）:交换a和b的值`
```
```
`10、sort系列`
`（1）void sort(Iterator first,iterator last)：对first-last范围内的元素进行升序排序，算法复杂度o(nlog2n)`
`（2）void stable_sort(Iterator first,iterator last)：和sort一样，但是是个稳定排序，算法复杂度o(nlog2n)`
```
```
`10、search系列`
`（1）iterator search(iterator first1 ,iterator last1,iterator first2,iterator last2)：返回在first1-last1之间的，元素序列值为first2-last2的序列的首地址。`
（2）iterator search(iterator first1 ,iterator last1,iterator first2,iterator last2，Function func)：返回first1-last1之间的一个序列的首地址，从这个首地址开始的last2-first2个元素与first2-last2之间的元素之间的关系满足函数func，其中  func返回bool型。
11、reverse & rotate
（1）void reverse(iterator first,iterator last)：把first-last的数据翻过来位置
（2）iterator rotate(iterator first,iterator middle,iterator last)：把middle-last之间的元素放到前面，first-middle之间的元素放在后面。
函数之间的区别与联系：
1、iterator find_first_of（iterator first1,iterator last1,iterator first2,iterator last2） &  iterator search（iterator first1,iterator last1,iterator first2,iterator last2）
find_first_of函数只需找到第一个元素满足下列条件：这个元素在[first2,last2]范围之内
search函数需要找到一个位置i，从i开始的元素完全与[first2,last2]范围内的元素相等
