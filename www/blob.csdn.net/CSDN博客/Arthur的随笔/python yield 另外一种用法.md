# python yield 另外一种用法 - Arthur的随笔 - CSDN博客
2011年10月21日 16:15:38[largetalk](https://me.csdn.net/largetalk)阅读数：1155
```python
In [6]: def return_non():
    print '-----------'
   ...:     while True:
   ...:         print '##############'
   ...:         x = yield
   ...:         print x
   ...:         print '%%%%%%%%%%%%'
   ...: 
In [8]: y = return_non()
In [9]: y
Out[9]: <generator object return_non at 0xb72ee8ec>
In [10]: y.next()
-----------
##############
In [11]: y.send('hello world')
hello world
%%%%%%%%%%%%
##############
In [12]: y.send('i am old')
i am old
%%%%%%%%%%%%
##############
In [13]: y
Out[13]: <generator object return_non at 0xb72ee8ec>
```
