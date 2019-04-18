# python assert的作用 - YZXnuaa的博客 - CSDN博客
2018年06月01日 13:40:44[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：65
一、python assert的作用：
根据Python 官方文档解释(https://docs.python.org/3/reference/simple_stmts.html#assert), "Assert statements are a convenient way to insert debugging assertions into a program".
二、一般的用法是：
`assert condition`
用来让程序测试这个condition，如果condition为false，那么raise一个AssertionError出来。逻辑上等同于：
```
if not condition:
    raise AssertionError()
```
比如如下的例子：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
>>> assert 1==1
>>> assert 1==0
Traceback (most recent call last):
  File "<pyshell#1>", line 1, in <module>
    assert 1==0
AssertionError
>>> assert True
>>> assert False
Traceback (most recent call last):
  File "<pyshell#3>", line 1, in <module>
    assert False
AssertionError
>>> assert 3<2
Traceback (most recent call last):
  File "<pyshell#4>", line 1, in <module>
    assert 3<2
AssertionError
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
三、如何为assert断言语句添加异常参数
　　assert的异常参数，其实就是在断言表达式后添加[字符串](http://www.iplaypython.com/jichu/str.html)信息，用来解释断言并更好的知道是哪里出了问题。格式如下：
assert expression [, arguments]
assert 表达式 [, 参数]
比如如下的例子：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
>>> assert len(lists) >=5,'列表元素个数小于5'
Traceback (most recent call last):
File "D:/Data/Python/helloworld/helloworld.py", line 1, in <module>
assert 2>=5,'列表元素个数小于5'
AssertionError: 列表元素个数小于5
>>> assert 2==1,'2不等于1'
Traceback (most recent call last):
File "D:/Data/Python/helloworld/helloworld.py", line 1, in <module>
assert 2==1,'2不等于1'
AssertionError: 2不等于1
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
--------------------------------------------------------------------
参考链接：
https://www.cnblogs.com/liuchunxiao83/p/5298016.html
https://www.cnblogs.com/cedrelaliu/p/5948567.html
