# python的拉链（zip） - lyx的专栏 - CSDN博客





2016年03月08日 20:47:37[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：938
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









python中有一个zip，也就是拉链，那我们来理解一下吧。



```python
>>> a = [1, 2, 3]
>>> b = ['a', 'b', 'c']
>>> z = zip(a, b)
>>> z
[(1, 'a'), (2, 'b'), (3, 'c')]
>>> zip(*z)
[(1, 2, 3), ('a', 'b', 'c')]
```
我们把list当做列向量来看就很好理解了，zip就是拉链，把一件挂着的衣服拉起来。这就是zip的功能。所以



```python
[(1, 'a'), (2, 'b'), (3, 'c')]
```
当做列向量看，就是拉起来的拉链了。

而转置的z就是把拉链放水平，多形象！





