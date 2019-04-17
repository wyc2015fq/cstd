# 什么情况下应该设置 cudnn.benchmark = True？ - Snoopy_Dream - CSDN博客





2018年12月27日 11:52:28[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：364
个人分类：[pytorch](https://blog.csdn.net/e01528/article/category/8254835)









# 文章目录 [[隐藏](https://www.pytorchtutorial.com/when-should-we-set-cudnn-benchmark-to-true/#)]
- [1 问题](https://www.pytorchtutorial.com/when-should-we-set-cudnn-benchmark-to-true/#i)
- [2 解决办法](https://www.pytorchtutorial.com/when-should-we-set-cudnn-benchmark-to-true/#i-2)

## 问题

在很多情况下我们都能看到代码里有这样一行：
|1|torch.backends.cudnn.benchmark = true|
|----|----|

而且大家都说这样可以增加程序的运行效率。那到底有没有这样的效果，或者什么情况下应该这样做呢？

## 解决办法

总的来说，大部分情况下，设置这个 flag 可以让内置的 cuDNN 的 auto-tuner 自动寻找最适合当前配置的高效算法，来达到优化运行效率的问题。

一般来讲，应该遵循以下准则：
- 如果网络的输入数据维度或类型上变化不大，设置  torch.backends.cudnn.benchmark = true  可以增加运行效率；
- 如果网络的输入数据在每次 iteration 都变化的话，会导致 cnDNN 每次都会去寻找一遍最优配置，这样**反而会降低**运行效率



