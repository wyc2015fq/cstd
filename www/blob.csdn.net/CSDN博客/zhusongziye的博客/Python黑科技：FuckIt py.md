# Python黑科技：FuckIt.py - zhusongziye的博客 - CSDN博客





2018年11月07日 21:24:43[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：32标签：[Python黑科技：FuckIt.py](https://so.csdn.net/so/search/s.do?q=Python黑科技：FuckIt.py&t=blog)
个人分类：[Python语言](https://blog.csdn.net/zhusongziye/article/category/7131144)









说起 Python 强大的地方，你可能想到是它的优雅、简洁、开发速度快，社区活跃度高。但真正使得这门语言经久不衰的一个重要原因是它的无所不能，因为社区有各种各样的第三库，使得我们用Python实现一个东西实在是太简单了，你经常会看到几行代码实现爬虫，10行代码实现人脸识别，虽然有些夸张，但确实就是有这样的库帮你把所有的繁文缛节全部封装了，最后给你开放一个优雅的 API。



今天给你推荐的这个库叫 “FuckIt.py”，名字一看就是很黄很暴力的那种，作者是这样介绍它的：

> 
FuckIt.py uses state-of-the-art technology to make sure your Python code runs whether it has any right to or not. Some code has an error? Fuck it.


FuckIt.py 使用了最先进的技术能够使你的代码不管里面有什么样的错误，你只管 FuckIt，程序就能“正常”执行，兵来将挡水来土掩。

![](https://img-blog.csdnimg.cn/20181107212113223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



还是先来看个例子怎么使用的吧。

安装
`pip install fuckit`
假设有一个目标文件： broke.py

```
def f():
    broken_code
    print('fuckit chaining works')

for

let's just assume this is a big module of shitty code.

x = y
y = x
1 / 0 # Oh shhhiiiiiii

var = "Are you proud of what you've done?"
```

broke.py 中有几处明显的错误，包括语法错误，变量 broken_code 没有被定义，还有0当作被除数。

如果直接 import broke 肯定会报错

```
>>> import broke
  File "broke.py", line 5
    for
      ^
SyntaxError: invalid syntax
```

这时你可以使用 fuckit 将 broke 导入进来。

```
>>> import fuckit
>>> fuckit("broke")
<module 'broke' from 'broke.py'>
>>> broke
<module 'broke' from 'broke.py'>
```

调用函数 f 的时候也会报错

```
>>> broke.f()
NameError: global name 'broken_code' is not defined
```

你可以使用链式 fuckit 来调用这个有问题的函数

```
>>> fuckit(broke).f()
fuckit chaining works

或者
>>> fuckit(fuckit("broke")).f()
fuckit chaining works
```

除此之外，fuckit 还可以作为装饰器和上下文管理器来使用。

装饰函数

```
>>> @fuckit
... def broken_function():
...     non_existant_variable # Let's create a NameError
...     return 'Function decorator works'
...
>>> print(broken_function())
None
```

装饰类

```
>>> @fuckit
... class BrokenClass(object):
...     def f(self):
...         self.black_hole = 1 / 0
...         return 'Class decorator works'
...
>>> print(BrokenClass().f())
None
```

上下文管理器

```
>>> with fuckit:
...     print('Context manager works')
...     raise RuntimeError()
...
```

等价于

```
try:
     print('Context manager works')
except RuntimeError:
    pass
```

看到这里，是不是感觉 fuckit 很暴力，那么它能用在什么场景？个人认为这个库很鸡肋，因为你的代码不应该容忍这些错误的出现，如果出现了错误应该立马修复才是，但有一种可能的使用场景是你刚接手一份超复杂的代码，里面各种看不懂的代码，而这些操蛋的代码时不时出现一个错误，而你又没法驾驭它的时候，你就用 fuckit 吧。

fuckit的源代码不到200行，好奇的你是不是很想知道它怎么实现了，用了什么黑科技？访问项目地址： https://github.com/ajalt/fuckitpy



