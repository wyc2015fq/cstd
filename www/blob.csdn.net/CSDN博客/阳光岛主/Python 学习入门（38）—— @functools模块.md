
# Python 学习入门（38）—— @functools模块 - 阳光岛主 - CSDN博客

2014年03月01日 00:06:03[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：11830所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



The functools module is for higher-order functions: functions that act on or return other functions. In general, any callable object can be treated as a function for the purposes of this module.

functools 源码路径及内置函数：
![](https://img-blog.csdn.net/20140120143622921)

利用@functools对函数运行时间，进行计时
**代码示例：**

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# blog.ithomer.net
import time, functools
def timeit(func):
    @functools.wraps(func)
    def __do__(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        print("%s usedtime: %ss" % (func.__name__, time.time() - start))
        return result
    return __do__
@timeit
def print_str(num):
    sum = 0
    for i in range(num):
        sum += i
    print sum
@timeit
def main():
    print("print_str(100)")
    print_str(100)
    
    print("print_str(10000)")
    print_str(10000)
    
    print("print_str(1000000)")
    print_str(1000000)
if __name__ == "__main__":  
    main()
```
运行结果：
print_str(100)
4950
print_str usedtime: 3.60012054443e-05s
print_str(10000)
49995000
print_str usedtime: 0.000550985336304s
print_str(1000000)
499999500000
print_str usedtime: 0.0614850521088s
main usedtime: 0.0623250007629s
说明：运行结果中的红色部分，都是运行计时的结果

**示例2：**

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# blog.ithomer.net
import time, functools
def functools_wrapper(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print("call from functools_wrapper...")
        start = time.time()
        result = func(*args, **kwargs)
        print("%s usedtime: %ss" % (func.__name__, time.time() - start))
#         return func(*args, **kwargs)    
        return result
    return wrapper
    
@functools_wrapper
def functools_partial():
    print(int('10'))        # 10
    print(int('10', 2))     # 2
    int2 = functools.partial(int, base=2)
    print(int2('10'))       # 2
    print(int2('1010'))     # 10
    int2 = functools.partial(int, base=8)
    print(int2('10'))       # 8
    print(int2('1010'))     # 520
   
@functools_wrapper
def functools_reduce():
    array = [1, 2, 3, 4, 5, 6]
    result = reduce((lambda x,y:x*y), array)
    print("result = %d" % result)           # 720
    
    result = functools.reduce((lambda x,y:x*y), array)
    print("result = %d" % result)            # 720
def main():
    functools_partial()
    functools_reduce()
if __name__ == "__main__":  
    main()
```
运行结果：
call from functools_wrapper...
10
2
2
10
8
520
functools_partial usedtime: 2.00271606445e-05s
call from functools_wrapper...
result = 720
result = 720
functools_reduce usedtime: 1.21593475342e-05s


**参考推荐：**
[Python的functools模块](http://blog.csdn.net/wyabc1986/article/details/7432141)
[Python的functools](http://lazynight.me/3017.html)



