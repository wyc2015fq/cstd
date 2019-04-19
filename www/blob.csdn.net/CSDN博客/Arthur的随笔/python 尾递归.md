# python 尾递归 - Arthur的随笔 - CSDN博客
2012年05月07日 17:06:49[largetalk](https://me.csdn.net/largetalk)阅读数：1186标签：[python																[recursion																[decorator																[exception																[function																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=decorator&t=blog)](https://so.csdn.net/so/search/s.do?q=recursion&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
网上有篇搞笑文 python 程序员进化 [http://coolshell.cn/articles/2082.html](http://coolshell.cn/articles/2082.html) ，对那个尾递归产生了兴趣。尾递归是指以迭代计算过程执行递归定义的语句，在python等语言中一般都是需要while， do， for这样的特殊语法。而[http://code.activestate.com/recipes/474088/](http://code.activestate.com/recipes/474088/)
 有人实现了一个不需要这些语句的python尾递归，巨强无比。
代码我拷过来，便于看一下
```python
## {{{ http://code.activestate.com/recipes/474088/ (r1)
#!/usr/bin/env python2.4
# This program shows off a python decorator(
# which implements tail call optimization. It
# does this by throwing an exception if it is 
# it's own grandparent, and catching such 
# exceptions to recall the stack.
import sys
class TailRecurseException:
  def __init__(self, args, kwargs):
    self.args = args
    self.kwargs = kwargs
def tail_call_optimized(g):
  """
  This function decorates a function with tail call
  optimization. It does this by throwing an exception
  if it is it's own grandparent, and catching such
  exceptions to fake the tail call optimization.
  
  This function fails if the decorated
  function recurses in a non-tail context.
  """
  def func(*args, **kwargs):
    f = sys._getframe()
    if f.f_back and f.f_back.f_back \
        and f.f_back.f_back.f_code == f.f_code:
      raise TailRecurseException(args, kwargs)
    else:
      while 1:
        try:
          return g(*args, **kwargs)
        except TailRecurseException, e:
          args = e.args
          kwargs = e.kwargs
  func.__doc__ = g.__doc__
  return func
@tail_call_optimized
def factorial(n, acc=1):
  "calculate a factorial"
  if n == 0:
    return acc
  return factorial(n-1, n*acc)
print factorial(10000)
# prints a big, big number,
# but doesn't hit the recursion limit.
@tail_call_optimized
def fib(i, current = 0, next = 1):
  if i == 0:
    return current
  else:
    return fib(i - 1, next, current + next)
print fib(10000)
# also prints a big number,
# but doesn't hit the recursion limit.
## end of http://code.activestate.com/recipes/474088/ }}}
```
这个尾递归的实现运用了decorator和exception机制, 基本原理是判断是否递归，是递归则抛出异常，然后捕捉异常重新调用。
ps. sys._getframe() 返回当前frame 对象
举例说明一下， factorial(2)，由于decortaor，调用顺序是：
```python
--func ------(2, )
----factorial(2, 1)
------func----(1, 2)                          
--------raise TailRecurException 
        back to func ---(1,2)
      back to factorial(1,2)
    back ot func ----(2,)
  call factorial(1,2)
  .
  .
  .
```
基本情况就是这样， 这里也非常取巧，利用参数解析来达到递归的效果
如果像下面定义一个递归函数，就不能使用这个尾递归了
```python
@tail_call_optimized
def fff(n):
    if n == 0:
        return 1
    return n*fff(n-1)
print fff(5)
```

