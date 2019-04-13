
# 内存泄露Lowmemorykiller分析 - 嵌入式Linux - CSDN博客

2018年07月23日 20:21:31[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：193所属专栏：[微信公众号:嵌入式linux](https://blog.csdn.net/column/details/26885.html)




### 01 前言
最近疫苗事情非常火热，这件事情让我对刘强东有点刮目相看，我们需要更多的人关注曝光此类问题
![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPdERHyUT7LBT168cOBbkEH31vKZTRUiaTv8h3qTYRJ4IMmfwibVnCl7QZEajzNNP1ic0Jgg8PQfypsjQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
### 02 正文
Android Kernel 会定时执行一次检查，杀死一些进程，释放掉内存。Low memory killer 是定时进行检查。Low memory killer 主要是通过进程的oom_adj 来判定进程的重要程度。这个值越小，程序越重要，被杀的可能性越低。oom_adj的大小和进程的类型以及进程被调度的次序有关。Low memory killer 的具体实现可参看：
> kernel/drivers/misc/lowmemorykiller.c
假设你的程序在运行中不断的malloc申请内存，你会发现经过一段时间后，系统就出现问题了，这时候就进去了lowmemory了，这时候是非常危险的，所以我们要分析出在哪里出现了问题并排查改正。
## 1 Lowmemorykiller 触发后出现的时候，检查内存
![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPdERHyUT7LBT168cOBbkEH3hKnJibgE91V2byaeGagYnDCOtsVicJB01jRYeVKm3sD3BUq3gtYKeOhw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
## 2 查看lowmemorykiller 相关参数
![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPdERHyUT7LBT168cOBbkEH3vRx9k1ibm1Q4zuZgbUqmCChAticzI5uvvDWBMSt3RIEib1fX559l5TIjA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
## 3 使用 procrank 查看内存使用情况
找到占用内存最大的那个进程，从下图可以看到是1944的进程内存占用最大
![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPdERHyUT7LBT168cOBbkEH338LWVBuuNkbQibwlgTAgWV3TOoqEfaJP72AxjB8L2Rib1a8aWtskRn6Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

## 4 使用procmem PID 查看进程中哪个函数导致的
如下图，可以看到在led_test里面不断的调用malloc导致的，最后去这里查看代码，发现代码有问题导致。
![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPdERHyUT7LBT168cOBbkEH3rb94JWBcia7RlcJyA3mSKv1jRA07Q4c4tB0sZQz4b7qBF0QkyVYb9cw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
或者微信搜索：**嵌入式Linux**关注公众号查看文章

