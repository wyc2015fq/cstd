
# Python模拟随机游走图形效果示例 - jiahaowanhao的博客 - CSDN博客


2018年02月11日 21:43:47[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：610


[Python模拟随机游走图形效果示例](http://cda.pinggu.org/view/24709.html)
本文实例讲述了Python模拟随机游走图形效果。分享给大家供大家参考，具体如下：
在python中，可以利用数组操作来模拟随机游走。
下面是一个单一的200步随机游走的例子，从0开始，步长为1和-1，且以相等的概率出现。纯Python方式实现，使用了内建的 random 模块：

\# 随机游走
importmatplotlib.pyplot as plt
importrandom
position=0
walk=[position]
steps=200
foriinrange(steps):
step=1ifrandom.randint(0,1)else-1
position+=step
walk.append(position)
fig=plt.figure()
plt.title("www.jb51.net")
ax=fig.add_subplot(111)
ax.plot(walk)
plt.show()
![](http://cda.pinggu.org/uploadfile/image/20180211/20180211084532_86688.png)
第二种方式：简单的把随机步长累积起来并且可以可以使用一个数组表达式来计算。因此，我用 np.random 模块去200次硬币翻转，设置它们为1和-1，并计算累计和：
\# 随机游走
importmatplotlib.pyplot as plt
importnumpy as np
nsteps=200
draws=np.random.randint(0,2, size=nsteps)
steps=np.where(draws >0,1,-1)
walk=steps.cumsum()
fig=plt.figure()
plt.title("www.jb51.net")
ax=fig.add_subplot(111)
ax.plot(walk)
plt.show()
![](http://cda.pinggu.org/uploadfile/image/20180211/20180211084525_74025.png)
一次模拟多个随机游走
\# 随机游走
importmatplotlib.pyplot as plt
importnumpy as np
nwalks=5
nsteps=200
draws=np.random.randint(0,2, size=(nwalks, nsteps))\# 0 or 1
steps=np.where(draws >0,1,-1)
walks=steps.cumsum(1)
fig=plt.figure()
plt.title("www.jb51.net")
ax=fig.add_subplot(111)
foriinrange(nwalks):
ax.plot(walks[i])
plt.show()
![](http://cda.pinggu.org/uploadfile/image/20180211/20180211084518_30812.png)
当然，还可以大胆的试验其它的分布的步长，而不是相等大小的硬币翻转。你只需要使用一个不同的随机数生成函数，如 normal 来产生相同均值和标准偏差的正态分布：
steps=np.random.normal(loc=0, scale=0.25, size=(nwalks, nsteps))
![](http://cda.pinggu.org/uploadfile/image/20180211/20180211084511_27841.png)


