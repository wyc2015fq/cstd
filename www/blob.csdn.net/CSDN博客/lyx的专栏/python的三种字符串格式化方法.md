# python的三种字符串格式化方法 - lyx的专栏 - CSDN博客





2017年08月11日 20:29:56[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：12026
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









        刚入门python的同学，特别是，没有系统的学习过python，而是学过别的语言，直接上手python的同学，怕是还不是很了解python强大的字符串格式化方法

# 1.最方便的






```python
print 'hello %s and %s' % ('df', 'another df')
```

但是，有时候，我们有很多的参数要进行格式化，这个时候，一个一个一一对应就有点麻烦了，于是就有了第二种，字典形式的。上面那种是tuple形式的。


2.最好用的


```python
print 'hello %(first)s and %(second)s' % {'first': 'df', 'second': 'another df'}
```





这种字典形式的字符串格式化方法，有一个最大的好处就是，字典这个东西可以和json文件相互转换，所以，当配置文件使用字符串设置的时候，就显得相当方便。



# 3.最先进的



```python
print 'hello {first} and {second}'.format(first='df', second='another df')
```









这种就像是写一个函数一样，有好处，就是可读性很好，但是笔者还是喜欢第二种。






