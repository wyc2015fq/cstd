# Selenium基本使用(十一)异常捕获 - _天枢 - 博客园
## [Selenium基本使用(十一)异常捕获](https://www.cnblogs.com/yhleng/p/7560895.html)
2017-09-20 13:05 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7560895)
**1. 抛出异常和自定义异常**
Python用异常对象(exception object)表示异常情况，遇到错误后，会引发异常。如果异常对象并未被处理或捕捉，程序就会用所谓的回溯(Traceback,一种错误信息)终止执行。
**raise 语句**
Python中的raise 关键字用于引发一个异常，基本上和C#和Java中的throw关键字相同，如下所示：
```
# -- coding: utf-8 --
 
def ThorwErr():
  raise Exception("抛出一个异常")
 
# Exception: 抛出一个异常
ThorwErr()
```
raise关键字后面是抛出是一个通用的异常类型(Exception)，一般来说抛出的异常越详细越好，Python在exceptions模块内建了很多的异常类型，通过使用dir函数来查看exceptions中的异常类型，如下：
```
import exceptions
 
# ['ArithmeticError', 'AssertionError'.....]
print dir(exceptions)
```
**传递异常**
捕捉到了异常，但是又想重新引发它(传递异常)，可以使用不带参数的raise语句即可：
```
# -- coding: utf-8 --
class MuffledCalculator:
  muffled = False
  def calc(self,expr):
    try:
      return eval(expr)
    except ZeroDivisionError:
      if self.muffled:
        print 'Division by zero is illegal'
      else:
        raise
```
**自定义异常类型**
Python中也可以自定义自己的特殊类型的异常，只需要要从Exception类继承(直接或间接)即可：
```
class SomeCustomException(Exception):
  pass
```
**2. 捕捉异常**
和C#中的try/catch类似，Python中使用try/except关键字来捕捉异常，如下：
```
# -- coding: utf-8 --
 
try:
  print 2/0
except ZeroDivisionError:
  print '除数不能为0'
```
**①.捕捉多个异常**
在一个except语句只捕捉其后声明的异常类型，如果可能会抛出的是其他类型的异常就需要再增加一个except语句了，或者也可以指定一个更通用的异常类型比如：Exception，如下：
```
# -- coding: utf-8 --
try:
  print 2/'0'
except ZeroDivisionError:
  print '除数不能为0'
except Exception:
  print '其他类型异常'
```
为了捕获多个异常，除了声明多个except语句之外，还可以在一个except语句之后将多个异常作为元组列出来即可：
```
# -- coding: utf-8 --
try:
  print 2/'0'
except (ZeroDivisionError,Exception)，ex:
  print '发生了一个异常'+ex.message
```
**②.获取异常信息**
每个异常都会有一些异常信息，一般情况下我们应该把这些异常信息记录下来：
```
# -- coding: utf-8 --
try:
  print 2/'0'
except (ZeroDivisionError,Exception) as e:
  # unsupported operand type(s) for /: 'int' and 'str'
  print e
```
**3. finally子句**
finally子句和try子句联合使用但是和except语句不同，finally不管try子句内部是否有异常发生，都会执行finally子句内的代码。所有一般情况下，finally自己常常用于关闭文件或者在Socket中。
```
# -- coding: utf-8 --
try:
  print 2/'0'
except (ZeroDivisionError,Exception):
  print '发生了一个异常'
finally:
  print '不管是否发生异常都执行'
```
