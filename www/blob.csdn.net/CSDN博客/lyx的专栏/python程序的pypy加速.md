# python程序的pypy加速 - lyx的专栏 - CSDN博客





2017年09月24日 18:54:26[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3392
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









我们知道，python作为一种几乎是脚本语言的语言，其优点固然有，但是其有一个最大的缺点，就是运行速度没有办法和c，c++，java比。最近在些一些代码的时候也是碰到了这样的问题。

具体而言，python想提速度，基本思路是两个，有个就jit技术，在python中比较好用的就是pypy；另外一种就是先分析代码速度瓶颈，然后把性能瓶颈用c或者别的语言写成模块，让python调用。后面一种方法其实也存在折中，比如cython。对于cython，笔者目前也在学习中，后续有心得了，写成文章和大家分享。

今天主要讲一讲pypy。

我们先来看一个例子：

```python
#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import time
def list_function():
    list_1 = range(100000)
    list_2 = range(100000)
    result_list = list()
    for cnt in range(100):
        for (a, b) in zip(list_1, list_2):
            result_list.append(a + b)

    return sum(result_list)

start = time.time()

print list_function()
print 'time elapse', time.time() - start
```

这段代码很简单，反正测试用。

然后首先在命令行中用python命令执行一下，看一下花费了多少时间：

![](https://img-blog.csdn.net/20170924190104578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

大概在三秒半左右，大家运行的时间可能会不一样，毕竟笔者的笔记本已经有点年纪了，但我还是很爱我的笔记本。

然后我们换一个命令来运行，pypy

![](https://img-blog.csdn.net/20170924190222604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        surprise，什么都没做，速度就快了一倍还要多，于是有人就说，这个东西真好用。这就是JIT的力量，有人说，有些代码，pypy可以做到比c写的还快。

实在是太好用了！

于是，我们继续写一个：

```python
#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import time
import pandas as pd

def list_function():
    list_1 = range(100000)
    list_2 = range(100000)
    result_list = list()
    for cnt in range(100):
        for (a, b) in zip(list_1, list_2):
            result_list.append(a + b)

    return sum(result_list)

def df_function():
    df = pd.DataFrame()
    df['a'] = range(100000)
    df['b'] = range(100000)
    accu = 0
    for cnt in range(100):
        accu += sum(df['a'] + df['b'])
    return accu

start = time.time()

print list_function()
print df_function()
print 'time elapse', time.time() - start
```

       在命令行中用python运行：

![](https://img-blog.csdn.net/20170924191016626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        大概5.5秒不到一点点的样子。然后我们想到了厉害的pypy。看一下效果吧：

![](https://img-blog.csdn.net/20170924191123772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        很遗憾，由于我们使用了pandas这一第三方包，所以不能用jit，也就是不能用pypy了，除非我们去下一个支持jit的pandas的包。换句话说，如果你写的python程序只包含build_in函数的话，那么很开心，pypy可以很好的加速，但是通常情况下并不是这样的，我能会用到各种各样的库。那怎么办，为了使用pypy而牺牲python最大的优点，第三方库，似乎有点舍本逐末了，还不如直接用cpp呢！



其实有一个叫jitpy的东西，据说可以用，但是。。好像最近网被墙了。。后续可以了再写一个。

当然，其实pypy也是可以自己安装包的，只是第一包不多，第二版本比较老，而且。。安装往往会出问题。或者cython才是真正的王道。




















