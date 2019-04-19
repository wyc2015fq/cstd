# Linux吃掉了我的内存的恐慌是怎么回事 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月04日 13:27:30[boonya](https://me.csdn.net/boonya)阅读数：137标签：[linux																[memory](https://so.csdn.net/so/search/s.do?q=memory&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/boonya/article/category/1740759)
**目录**
[发生了什么？](#%E5%8F%91%E7%94%9F%E4%BA%86%E4%BB%80%E4%B9%88)
[为什么这么做？](#%E4%B8%BA%E4%BB%80%E4%B9%88%E8%BF%99%E4%B9%88%E5%81%9A)
[如果我想运行更多的应用该怎么办？](#%E5%A6%82%E6%9E%9C%E6%88%91%E6%83%B3%E8%BF%90%E8%A1%8C%E6%9B%B4%E5%A4%9A%E7%9A%84%E5%BA%94%E7%94%A8%E8%AF%A5%E6%80%8E%E4%B9%88%E5%8A%9E)
[我需要更多的交换分区(swap)吗？](#%E6%88%91%E9%9C%80%E8%A6%81%E6%9B%B4%E5%A4%9A%E7%9A%84%E4%BA%A4%E6%8D%A2%E5%88%86%E5%8C%BAswap%E5%90%97)
[我如何让Linux停止这样做？](#%E6%88%91%E5%A6%82%E4%BD%95%E8%AE%A9linux%E5%81%9C%E6%AD%A2%E8%BF%99%E6%A0%B7%E5%81%9A)
[如果不是这样，为什么top和free命令说我所有的内存已被使用？](#%E5%A6%82%E6%9E%9C%E4%B8%8D%E6%98%AF%E8%BF%99%E6%A0%B7%E4%B8%BA%E4%BB%80%E4%B9%88top%E5%92%8Cfree%E5%91%BD%E4%BB%A4%E8%AF%B4%E6%88%91%E6%89%80%E6%9C%89%E7%9A%84%E5%86%85%E5%AD%98%E5%B7%B2%E8%A2%AB%E4%BD%BF%E7%94%A8)
[我怎么看我真正还有多少可用内存？](#%E6%88%91%E6%80%8E%E4%B9%88%E7%9C%8B%E6%88%91%E7%9C%9F%E6%AD%A3%E8%BF%98%E6%9C%89%E5%A4%9A%E5%B0%91%E5%8F%AF%E7%94%A8%E5%86%85%E5%AD%98)
[什么时候我该开始担心？](#%E4%BB%80%E4%B9%88%E6%97%B6%E5%80%99%E6%88%91%E8%AF%A5%E5%BC%80%E5%A7%8B%E6%8B%85%E5%BF%83)
[我如何验证这些？](#%E6%88%91%E5%A6%82%E4%BD%95%E9%AA%8C%E8%AF%81%E8%BF%99%E4%BA%9B)
![](https://images2017.cnblogs.com/blog/1156638/201802/1156638-20180202203623437-1991568203.png)
> 
英文原文: [https://www.linuxatemyram.com/](https://www.linuxatemyram.com/)
作者: [Vidar Holen](https://github.com/koalaman)
译者: [thinkam](https://github.com/codethereforam)
## 发生了什么？
Linux正借用你未使用的内存来做磁盘缓存。这使你的计算机看起来可用内存很少，但事实不是这样！一切都很正常！
## 为什么这么做？
磁盘缓存使你的系统运行更快更流畅！除了使新手感到困惑，这没什么缺点。它从来没有以任何方式从你的应用拿走内存。
## 如果我想运行更多的应用该怎么办？
如果你的应用想要获取更多的内存，系统会从磁盘缓存中拿走一块被借用的内存。磁盘缓存会立即还给应用！你的计算机可用内存的并不少！
## 我需要更多的交换分区(swap)吗？
不需要，磁盘缓存仅会借用应用目前不需要的内存。它不会使用交换分区。如果应用需要更多的内存，系统会从磁盘缓存中拿回来。系统不会使用交换分区。
## 我如何让Linux停止这样做？
你不能禁用磁盘缓存。任何人想要禁用磁盘缓存的唯一理由是他认为这从他的应用中拿走了内存，但事实不是这样！磁盘缓存可以让应用加载更快并且运行更加流畅，但它却从来没有拿走内存！因此，没有任何理由去禁用它！
## 如果不是这样，为什么`top`和`free`命令说我所有的内存已被使用？
这仅仅是用辞的差异。你和Linux系统一致认为被应用占用的内存称为`used`，而没有被任何程序使用的部分称为`free`。
但是，你认为目前被占用但仍然可以被应用使用的内存称作什么呢？
你可能认为这部分内存是`free`或`available`。而Linux系统称它`used`或`available`:
|内存|你称它|Linux称它|
|----|----|----|
|被应用使用|`used`|`used`|
|被占用，但可以使用|`free`(或`available`)|`used`(或`available`)|
|没有用来做任何事|`free`|`free`|
> 
译注：老版`free`命令中被占用但可以使用的内存称为`used`(`used`中的一部分，即`buffers`+`cached`), 而新版的`free`命令中称之为`available`。
不严格来讲，`top`和`free`命令称这部分内存为`buffers`和`cached`。因为你和Linux用辞的差异，你可能认为你的计算机可用内存很少，不过事实并非如此。
> 
译注：这部分内存准确来说是新版本`free`命令中的`available`或者是新版本`top`命令中的`avail Mem`。
## 我怎么看我真正还有多少可用内存？
为了弄明白除了交换分区外你的应用可以使用多少内存，执行`free -m`命令，然后查看`available`那一列:
```
$ free -m
              total        used        free      shared  buff/cache   available
Mem:           1504        1491          13           0         855      792
Swap:          2047           6        2041
```
(2016年之前的Linux安装版本，在`-/+ buffers/cache`那排查看`free`列)
这是你以`MB`为单位的答案。如果你天真地查看`used`和`free`，你会认为你的内存几乎快占满达到99%，然而实际仅仅只有47%！
如果想查看更详细、更专业的关于Linux统计`available`内存大小的描述，请看[the commit that added the field](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=34e431b0ae398fc54ea69ff85ec700722c9da773).
## 什么时候我该开始担心？
一个拥有足够内存的、**运行良好**的Linux系统在运行一段时间后会表现出以下预期的无害特征：
- `free`内存接近于**0**
- `used`内存接近**总共内存**
- `available`内存（或者是`free + buffers/cache`）有足够空间（比如说：总内存的20%以上）
- `swap used`没有发生变化
真正的内存不够、你也许想调查原因的情形，**警告标志**如下：
- `available`内存（或者是`free + buffers/cache`）接近于**0**
- `swap used`在增加或者波动
- `dmesg | grep oom-killer`命令显示`OutOfMemory-killer`程序正在运行
## 我如何验证这些？
如果想了解更多细节和做实验测试磁盘缓存的影响，请看[这个页面](https://www.linuxatemyram.com/play.html)。我做了少量的工作只是想让你感激磁盘缓存，而不是让你测试你的硬件速度提升了几个数量级。
作者[thinkam](mailto:1203949298@qq.com)和本文原始地址或出处：[http://www.cnblogs.com/thinkam](http://www.cnblogs.com/thinkam)
