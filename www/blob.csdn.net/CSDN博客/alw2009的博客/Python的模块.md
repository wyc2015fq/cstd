# Python的模块 - alw2009的博客 - CSDN博客
2017年12月09日 14:04:42[traveler_zero](https://me.csdn.net/alw2009)阅读数：71
我们通常会有这样的需求， 把一些常用的函数放在一个文件里， 然后项目的其他文件可以引用这些常用函数。在Python里如何实现这样的分离的， 答案就是模块。那么如何定义何使用模块呢？
先说结论：python模块其实就是普通的py文件， 如果需要使用这个文件的函数直接import 文件名（除去.py）就可以用Module.Function()的方式调用了。
例如：
准备如下fibo.py文件
```python
def fib(n):
     a, b = 0, 1
     while b < n:
         print (b, end=' ')
         a, b = b, a+b
     print ()
 
 def fib2(n): # return Fibonacci series up to n
     result = []
     a, b = 0, 1
     while a < n:
         result.append(b)
         a, b = b, a + b
     return result
```
直接在解释器上调用：
```
Python 3.4.5 (default, Jun  1 2017, 13:52:39) 
[GCC 4.4.7 20120313 (Red Hat 4.4.7-18)] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import fibo
>>> fi
fibo     filter(  finally  
>>> fibo.fib(10)
1 1 2 3 5 8
```
