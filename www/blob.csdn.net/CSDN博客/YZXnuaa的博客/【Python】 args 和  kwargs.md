# 【Python】*args 和 **kwargs - YZXnuaa的博客 - CSDN博客
2018年07月11日 16:08:00[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：74
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)

使用*args和**kwargs语法。其中，*args是可变的positional arguments列表，**kwargs是可变的keyword arguments列表。并且，*args必须位于**kwargs之前，因为positional arguments必须位于keyword arguments之前。
下面一个例子使用*args，同时包含一个必须的参数：
```python
def test_args(first, *args):
   print 'Required argument: ', first
   for v in args:
      print 'Optional argument: ', v
test_args(1, 2, 3, 4)
# result:
# Required argument: 1
# Optional argument:  2
# Optional argument:  3
# Optional argument:  4
```
下面一个例子使用*kwargs, 同时包含一个必须的参数和*args列表：
```python
def test_kwargs(first, *args, **kwargs):
   print 'Required argument: ', first
   for v in args:
      print 'Optional argument (*args): ', v
   for k, v in kwargs.items():
      print 'Optional argument %s (*kwargs): %s' % (k, v)
test_kwargs(1, 2, 3, 4, k1=5, k2=6)
# results:
# Required argument:  1
# Optional argument (*args):  2
# Optional argument (*args):  3
# Optional argument (*args):  4
# Optional argument k2 (*kwargs): 6
# Optional argument k1 (*kwargs): 5
```
*args和**kwargs语法不仅可以在函数定义中使用，同样可以在函数调用的时候使用。不同的是，如果说在函数定义的位置使用*args和**kwargs是一个将参数pack的过程，那么在函数调用的时候就是一个将参数unpack的过程了。下面使用一个例子来加深理解：
```python
def test_args(first, second, third, fourth, fifth):
    print 'First argument: ', first
    print 'Second argument: ', second
    print 'Third argument: ', third
    print 'Fourth argument: ', fourth
    print 'Fifth argument: ', fifth
# Use *args
args = [1, 2, 3, 4, 5]
test_args(*args)
# results:
# First argument:  1
# Second argument:  2
# Third argument:  3
# Fourth argument:  4
# Fifth argument:  5
# Use **kwargs
kwargs = {
    'first': 1,
    'second': 2,
    'third': 3,
    'fourth': 4,
    'fifth': 5
}
test_args(**kwargs)
# results:
# First argument:  1
# Second argument:  2
# Third argument:  3
# Fourth argument:  4
# Fifth argument:  5
```
使用*args和**kwargs可以非常方便的定义函数，同时可以加强扩展性，以便日后的代码维护。
 转载请注明转自: [团子的小窝](http://kodango.com/) , 本文固定链接: [理解 Python 中的 *args 和 **kwargs](http://kodango.com/?p=197)
