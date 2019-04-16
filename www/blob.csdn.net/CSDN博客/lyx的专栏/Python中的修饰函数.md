# Python中的修饰函数 - lyx的专栏 - CSDN博客





2015年12月22日 15:21:10[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：519标签：[python																[修饰函数](https://so.csdn.net/so/search/s.do?q=修饰函数&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python](https://blog.csdn.net/qtlyx/article/category/5969093)

所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)








                
首先，什么是函数修饰符？函数修饰符就是对原有函数做一层包装。比如有以下两个函数：



复制代码

```python
def func1():
    print 'I am function func1'
def func2():
    print 'I am function func2'
```





现在我们想为这两个函数增加一段共同的操作，这时就可以使用函数修饰符。首先我们定义一个修饰函数，先不做任何处理，仅仅打印一条横线：



复制代码

```python
def de( f )
    def call_():
        print '-------------------------------'
        return f()
    return _call_
```




使用这个修饰符：



复制代码

```python
def de( f ):
    def _call_():
        print '-------------------------------'
        return f()
    return _call
@de
def func1():
    print 'I am function func1'
@de
def func2():
    print 'I am function func2'
if name = '__main__':
    func1()
    func2()
```





运行结果为：



复制代码

d:\我的文档\桌面>b.py

-------------------------------

I am function func1

-------------------------------

I am function func2

d:\我的文档\桌面>



可以看到，修饰符起作用了。 



注意： 

1.修饰符等价于包装调用：





@de

def func1:

----- 等价于 ------

func1 = de( func1 )



2.修饰函数必须返回一个“可调用对象”：



```python
def de( f ):
    def call_():
        return f()
    return _call # 返回的是一个函数体，而非调用_call_()
```






下一个问题：如果各个被修饰函数有不同的参数，怎么处理呢？例如：




```python
def func1( lst1, lst2 ):
    # 合并两个list，并打印各项
    for item in lst1+lst2:
        print item
def func2( dic ):
    # 循环dict，打印各key-value
    for k, v in dic.items():
        print k, v
```





这两个函数参数不同，又都没有异常处理，现在我们想增加上，使用修饰符这样处理：



```python
import sys
import traceback
def de( f ):
    def call_( *args, **kwargs ):
        try:
            return f( *args , **kwargs )
        except:
            print 'param type error'
    return _call
@de
def func1( lst1, lst2 ):
    for item in lst1+lst2:
        print item
@de
def func2( dic ):
    for k, v in dic.items():
        print k, v
if name = '__main__':
    func1( [1,2], [3,4] )
    func2( [1,2] )
```





这里使用了可变参数*args和**kwargs，这样会把所有接收来的参数，原样不动的再转给原函数，是惯用法。 

以上程序运行结果： 

复制代码



d:\我的文档\桌面>b.py

1

2

3

4

param type error

d:\我的文档\桌面>
            



