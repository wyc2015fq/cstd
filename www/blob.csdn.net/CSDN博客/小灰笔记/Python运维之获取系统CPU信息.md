# Python运维之获取系统CPU信息 - 小灰笔记 - CSDN博客





2017年04月12日 21:58:59[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3761
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









       使用Python进行运维工作的时候有时候需要获取CPU的信息，这在psutil模块库的帮助下非常容易实现。

       常见的CPU信息有以下几种：

       1，用户时间以及百分比；

       2，系统时间以及百分比；

       3，空闲时间以及百分比；

       4，CPU的硬件信息；

       前3个中的时间可以采用cpu_times方法获取，百分比可以使用cpu_times_pcercent获得。简单的示范如下：

In [9]: importpsutil



In [10]:psutil.cpu_times()

Out[10]: scputimes(user=4206.828125,system=2574.46875, idle=128393.578125)



In [11]:psutil.cpu_times_percent()

Out[11]:scputimes(user=3.6, system=2.4, idle=94.0)



In [12]:psutil.cpu_times().idle

Out[12]:128616.46875



In [13]:psutil.cpu_times_percent().user

Out[13]: 3.5



In [14]:psutil.cpu_times_percent().idle

Out[14]: 94.1

       由上面的信息可以看出，我现在的电脑完全是性能过剩啊！看来以后选择电脑的时候CPU倒是可以不用选择那么强的，不过硬盘倒是必须得选个好一些的。

       至于前面的第4项中的CPU物理信息，一般是获取CPU的逻辑CPU个数以及CPU的物理核心数目可以使用cpu_count方法获取。

       简单的示范如下;

In [15]:psutil.cpu_count()

Out[15]: 8



In [16]:psutil.cpu_count(logical = False)

Out[16]: 4

       从上面的交互命令中，获取到的信息是：电脑是四核八线程。之前选择电脑的时候，基本上就是冲着这个U来的，现在看看这个电脑的能力看来还是可以支撑使用一阵子的。



