# 用Python给程序加个进度条 - zhusongziye的博客 - CSDN博客





2018年08月13日 19:25:28[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1137








对于开发或者运维来说，使用 Python 去完成一些跑批任务，或者做一些监控事件是非常正常的情况。那么如何有效地监控任务的进度？除了在任务中加上 Log 外，还能不能有另一种方式来了解任务进展到哪一步了呢？

这就是今天我们来了解的内容——进度条。

**▌******标准输出****

首先说一下在操作系统中，都有哪些控制符：
- 
\r： 将光标移动到当前行的首位而不换行；

- 
\n：将光标移动到下一行，并不移动到首位；

- 
\r\n：将光标移动到下一行首位。


那让我们来看下如何通过 Python 来将标准输出中添加进度条吧。

```
import sys
import time
def progress_bar(total):
   """
   进度条效果
   """
   # 获取标准输出
   _output = sys.stdout
   # 通过参数决定你的进度条总量是多少
   for count in range(0, total + 1):
       # 这里的second只是作为工作量的一种代替
       # 这里应该是有你的主程序,main()
       _second = 0.1
       # 模拟业务的消耗时间
       time.sleep(_second)
       # 输出进度条
       _output.write(f'\rcomplete percent:{count:.0f}')
   # 将标准输出一次性刷新
   _output.flush()
progress_bar(100)
```

来看下实际效果，这里我录了一段GIF：

![](https://img-blog.csdn.net/20180812210341892?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**▌******Tqdm 模块****

Tqdm 是一个快速、扩展性强的进度条工具库，它提供了非常多的接口，有兴趣的小伙伴可以了解一下。

GitHub 地址是：https://github.com/tqdm/tqdm。

我们来看下，如何使用 Tqdm 模块来实现进度条。

1. 首先是安装，非常简单。
`pip install tqdm`
2. 接下来就是具体的操作。

```
from time import sleep
from tqdm import tqdm
# 这里同样的，tqdm就是这个进度条最常用的一个方法
# 里面存一个可迭代对象
for i in tqdm(range(1, 500)):
   # 模拟你的任务
   sleep(0.01)
```

是不是看到这里发现，Tqdm 非常的简单呢？这只是一个例子，它也可以非常的复杂，同时也支持多线程、多进程，还有 Pandas。

来一个多进程的感受下吧。

![](https://img-blog.csdn.net/20180812210401660?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

所有的代码都已经 Push 到我的 GitHub 上去了，有兴趣的小伙伴可以前去Download。
- 
Miracle Github：https://github.com/MiracleYoung/exercises




