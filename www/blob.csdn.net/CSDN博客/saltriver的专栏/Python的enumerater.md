
# Python的enumerater - saltriver的专栏 - CSDN博客


2016年10月01日 22:46:52[saltriver](https://me.csdn.net/saltriver)阅读数：606标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[enumerater																](https://so.csdn.net/so/search/s.do?q=enumerater&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[Python																](https://blog.csdn.net/saltriver/article/category/6363186)



enumerate是python的内置函数，意思是枚举、列举，用于遍历序列中的元素以及它们的索引。遍历序列的方法大家用的很多，如对一个列表lst = ['a', 'b', 'c', 'd', 'e']进行遍历：

```python
for c in lst:
    print(c)
```
有时候我们在遍历序列的过程中，还需要知道对应元素的索引值，首先能想到的是采用如下的方法：
```python
for i in range(len(lst)):
    print(i,lst[i])
```
稍微繁琐了些，可以直接使用python内置的enumerate函数，提供了更加精简和优雅的写法：

```python
for c in enumerate(lst):
    print(c)
```
输出结果：
(0, 'a')
(1, 'b')
(2, 'c')
(3, 'd')
(4, 'e')
或者：

```python
for i,v in enumerate(lst):
    print(i,v)
```
0 a
1 b
2 c
3 d
4 e
enumerate还有一个**值得注意的特点是可以直接指定索引开始所用的值**（默认是0）。
```python
for i, v in enumerate(lst, 101):
    print(i, v)
```
输出结果：101 a
102 b
103 c
104 d
105 e
直接从指定的数值101开始进行索引编号。


