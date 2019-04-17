# 一句Python，一句R︱数据的合并、分组、排序、翻转、集合 - 素质云笔记/Recorder... - CSDN博客





2016年08月23日 17:53:28[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3488













先学了R，最近刚刚上手python,所以想着将python和R结合起来互相对比来更好理解python。最好就是一句python，对应写一句R。

python中的numpy模块相当于R中的matirx矩阵格式，化为矩阵，很多内容就有矩阵的属性，可以方便计算。

以下符号：

**=R=**

代表着在R中代码是怎么样的。


# **一、分组.groupby**





`df.groupby(df.year // 10 *10).max()  #=R= max（cut(df$year,10)）`






——————————————————————————






# 二、数据合并——pandas



## **1、横向合并，跟R一样，用merge就可以。**



merge(data1,data2,on="id",, how='left'/'right')

merge(data1,data2，left_on='id1', right_on='id2', how='left'/'right') #如果两个数据集Key不一样，也可以合并








**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52291677#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52291677#)

[print](http://blog.csdn.net/sinat_26917383/article/details/52291677#)[?](http://blog.csdn.net/sinat_26917383/article/details/52291677#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- D1 = pd.DataFrame({'id':[801, 802, 803,804, 805, 806, 807, 808, 809, 810], 'name':['Ansel', 'Wang', 'Jessica', 'Sak','Liu', 'John', 'TT','Walter','Andrew','Song']})  
- D2 = pd.DataFrame({'id':[803, 804, 808,901], 'save': [3000, 500, 1200, 8800]})  
- merge(D1, D2, on='id')  






## **2、纵向合并、堆砌——concat**







**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52291677#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52291677#)

[print](http://blog.csdn.net/sinat_26917383/article/details/52291677#)[?](http://blog.csdn.net/sinat_26917383/article/details/52291677#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- concat(objs, axis=0, join='outer', join_axes=None, ignore_index=False,    
- keys=None, levels=None, names=None, verify_integrity=False, copy=True)  

concat不会去重，要达到去重的效果可以使用drop_duplicates方法。


   1、objs 就是需要连接的对象集合，一般是列表或字典；

   2、axis=0 是连接轴向join='outer' 参数作用于当另一条轴的 index 不重叠的时候，只有 'inner' 和 'outer' 可选（顺带展示 ignore_index=True 的用法），axis=1，代表按照列的方式合并。
   3、join_axes=[] 指定自定义索引


   4、参数ignore_index=True 重建索引





  同时，可以标识出来，keys=[ , ] 来标识出来，基本语句为：**concat([D1,D2], keys=['D1', 'D2'] )**

  同时，concat也可以暴力的横向合并：concat([D1,D2], axis=1）

**注意：**

  特别是参数ignore_index=True，一定要带上，因为不带上会出现索引都是0000，那么就不能方便地使用切片，而发生切片都是“0”




参考：[【原】十分钟搞定pandas](http://www.cnblogs.com/chaosimple/p/4153083.html)






——————————————————————————





# 三、数据合并——array、list、tuple








```
a=[]  
  
#append  
a.append([1,2])  
  
#insert  
a.insert(2,1)  
a.insert(2,[1,2])
```




append加在后面，insert(位置，内容)可以加在指定位置。如果是 元组+list，都可以通过append/insert合并起来。

**如果数据格式是array的话，如何对array进行合并？**

 这边笔者又要吐槽自己了...以为又在使用R，如果a是array格式的，append是不可以使用的。只有a=[]元组的时候，才可以append加进去。

 那么这边的解决办法就是先赋值一个元组，然后往里面加：



```
a=[]
a=a.append(np.array([1,2]))
```






——————————————————————————

# 四、排序

**ascending=True代表升序（从小到大）、Flase代表降序（从大到小）**


笔者借用R的思维，最好的排序就是先定位出来其下标（、索引）出来，就像R中排序中：


`data[order(data$x),]`
其中的order就是给出了下标。那么排序在不同数据结构下也有不同的排序方式。





## 1、元组、list




笔者目前见到的排序有以下几类：sort、sorted

sorted是一种函数，可以有更多的功能;而sort就直接帮你排序了



```
>>> a=[1,6,42,7,4,3,8,9,3]
>>> sorted(a)
[1, 3, 3, 4, 6, 7, 8, 9, 42]
>>> a.sort()
>>> a
[1, 3, 3, 4, 6, 7, 8, 9, 42]
```

如果是元组形式的话，那么sorted是函数形式，就直接拿到了排好序的数列。


其中a.sort()就直接帮你排序好了，还是sorted比较好。







## 2、数组array/numpy


笔者目前见到的排序有以下几类：sort、sorted;argsort返回的是数列排序的秩


sort+sorted跟之前的元组、list一样，但是argsort不太一样。



```
>>> b=np.array([1,6,42,7,4,3,8,9,3])
>>> b.sort()
>>> b
array([ 1,  3,  3,  4,  6,  7,  8,  9, 42])
>>> sorted(b)
[1, 3, 3, 4, 6, 7, 8, 9, 42]
```



其他用法都差不多，这里我非常喜欢argsort()，我想仔细了解她。



```
>>> b=np.array([1,6,42,7,4,3,8,9,3])
>>> b.argsort()
array([0, 5, 8, 4, 1, 3, 6, 7, 2])
>>> b.argsort()[::-1]
array([2, 7, 6, 3, 1, 4, 8, 5, 0])
>>> b[b.argsort()[::-1]]
array([42,  9,  8,  7,  6,  4,  3,  3,  1])
```

其中argsort()神器啊，就是之前我说的R里面的秩，可以用这个做很多复杂排序。


假设b是一个array



b.argsort():显示=b的秩+按照从小到大排序
b.argsort()[::-1]:b的秩+按照从大到小排序
b[b.argsort()[::-1]]:从大到小排序后的b





## 3、pandas中的DataFrame




pandas中有sort和rank，这个就跟R里面是一样的了。

rank(axis=0,ascending=Flase)

其中axis代表0为列，1代表行，ascending=True代表升序（从小到大）、Flase代表降序（从大到小）



```
sorted(data.ix[:,1])
# 数据排序
a=data.rank(axis=0,ascending=False)
#数据求秩
data.ix[:,1][a.ix[:,1]-1]
```
data.ix[:,1]代表选中第一列，然后sorted代表对第一列进行排序；




a.ix[:,1]-1  代表排好的秩，-1就还原到数据可以认识的索引。








另外，拍完序一般都需要取前几位数：

可以用data.head(n)

tail代表尾部




——————————————————————————




五、数据翻转


转置是data.T

序列翻转：reverse



利用切片的方法进行序列翻转：data[::-1]




——————————————————————————





# 六、数据交集、并集

来源于：

# [python 集合比较（交集、并集，差集）](http://blog.csdn.net/isoleo/article/details/13000975)

## 1、交、并、差集



```
下面来点简单的小例子说明把。  
  
>>> x = set('spam')  
>>> y = set(['h','a','m'])  
>>> x, y  
(set(['a', 'p', 's', 'm']), set(['a', 'h', 'm']))  
  
再来些小应用。  
  
>>> x & y # 交集  
set(['a', 'm'])  
  
>>> x | y # 并集  
set(['a', 'p', 's', 'h', 'm'])  
  
>>> x - y # 差集  
set(['p', 's'])
```
需要事前设定set
集合支持一系列标准操作，包括并集、交集、差集和对称差集，例如：  


```
a = t | s          # t 和 s的并集  
  
b = t & s          # t 和 s的交集  
  
c = t – s          # 求差集（项在t中，但不在s中）  
  
d = t ^ s          # 对称差集（项在t或s中，但不会同时出现在二者中）
```
基本操作：  


```
t.add('x')            # 添加一项  
s.update([10,37,42])  # 在s中添加多项  
使用remove()可以删除一项：  
  
t.remove('H')
```
还有：


```
len(s)  
set 的长度  
  
x in s  
测试 x 是否是 s 的成员  
  
x not in s  
测试 x 是否不是 s 的成员  
  
s.issubset(t)  
s <= t  
测试是否 s 中的每一个元素都在 t 中  
  
s.issuperset(t)  
s >= t  
测试是否 t 中的每一个元素都在 s 中  
  
s.union(t)  
s | t  
返回一个新的 set 包含 s 和 t 中的每一个元素  
  
s.intersection(t)  
s & t  
返回一个新的 set 包含 s 和 t 中的公共元素  
  
s.difference(t)  
s - t  
返回一个新的 set 包含 s 中有但是 t 中没有的元素  
  
s.symmetric_difference(t)  
s ^ t  
返回一个新的 set 包含 s 和 t 中不重复的元素  
  
s.copy()  
返回 set “s”的一个浅复制
```
请注意：union(), intersection(), difference() 和 symmetric_difference() 的非运算符（non-operator，就是形如 s.union()这样的）版本将会接受任何 iterable 作为参数。相反，它们的运算符版本（operator based counterparts）要求参数必须是 sets。这样可以避免潜在的错误，如：为了更可读而使用 set('abc') & 'cbs' 来替代 set('abc').intersection('cbs')。从 2.3.1 版本中做的更改：以前所有参数都必须是 sets。  

另外，Set 和 ImmutableSet 两者都支持 set 与 set 之间的比较。两个 sets 在也只有在这种情况下是相等的：每一个 set 中的元素都是另一个中的元素（二者互为subset）。一个 set 比另一个 set 小，只有在第一个 set 是第二个 set 的 subset 时（是一个 subset，但是并不相等）。一个 set 比另一个 set 打，只有在第一个 set 是第二个 set 的 superset 时（是一个 superset，但是并不相等）。  

子 set 和相等比较并不产生完整的排序功能。例如：任意两个 sets 都不相等也不互为子 set，因此以下的运算都会返回 False：a<b, a==b, 或者a>b。因此，sets 不提供 __cmp__ 方法。  







## 2、去重

记得以前个网友提问怎么去除海量列表里重复元素，用hash来解决也行，只不过感觉在性能上不是很高，用set解决还是很不错的，示例如下：



```
>>> a = [11,22,33,44,11,22]  
>>> b = set(a)  
>>> b  
set([33, 11, 44, 22])  
>>> c = [i for i in b]  
>>> c  
[33, 11, 44, 22]
```

其中有如何打开set集合数据格式。

3、笛卡尔积

来源：[几个有用的python函数 (笛卡尔积, 排列, 组合)](http://blog.csdn.net/huangzhiyuan111/article/details/52586316)



permutations 排列

combinations 组合,没有重复

combinations_with_replacement 组合,有重复

```python
>>> import itertools
>>> for i in itertools.product('ABCD', repeat = 2):
...     print i,
... 
('A', 'A') ('A', 'B') ('A', 'C') ('A', 'D') ('B', 'A') ('B', 'B') ('B', 'C') ('B', 'D') ('C', 'A') ('C', 'B') ('C', 'C') ('C', 'D') ('D', 'A') ('D', 'B') ('D', 'C') ('D', 'D')
>>> for i in itertools.permutations('ABCD', 2):
...     print i,
... 
('A', 'B') ('A', 'C') ('A', 'D') ('B', 'A') ('B', 'C') ('B', 'D') ('C', 'A') ('C', 'B') ('C', 'D') ('D', 'A') ('D', 'B') ('D', 'C')
>>> for i in itertools.combinations('ABCD', 2):
...     print i,
... 
('A', 'B') ('A', 'C') ('A', 'D') ('B', 'C') ('B', 'D') ('C', 'D')
>>> for i in itertools.combinations_with_replacement('ABCD', 2):
...     print i,
... 
('A', 'A') ('A', 'B') ('A', 'C') ('A', 'D') ('B', 'B') ('B', 'C') ('B', 'D') ('C', 'C') ('C', 'D') ('D', 'D')
```














