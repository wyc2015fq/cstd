# python实现可以被with上下文管理的类或函数 - _天枢 - 博客园
## [python实现可以被with上下文管理的类或函数](https://www.cnblogs.com/yhleng/p/10674986.html)
2019-04-09 09:39 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10674986)
# .开始之前先明确一下with机制
# 1.类包函数__enter__()和__exit__()函数，即是可以被上下文管理的类
# __enter__用来执行with时的方法，__exit__返回对象给with的as 对象
# 2.通过函数实现要用到contextmanager装饰器与yield关键字
# 一、通过类来实现支持whith上下文管理功能
```
class fp(object):
  def __init__(self, fileobj, mode):
    self.fileobj = open(fileobj, mode)
  def __enter__(self):
    return self.fileobj
  def __exit__(self, type, value, Traceback):
    self.fileobj.close()
    return True
with fp("abc1.txt", "w+") as fi:
  fi.write("测试上下文管理器")
```
# 二、通过函数yield关键字实现
```
from contextlib import contextmanager
@contextmanager
def fp(fileobj, mode):
      f = open(fileobj, mode)
      yield f
      f.close()
with fp("abc2.txt", "w+") as fi:
  fi.write("context测试上下文管理器")
```
#用以上两种方法来实现相同的功能
