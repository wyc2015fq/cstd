# Keras入门（3）——磨刀不误砍柴工 - 刘炫320的博客 - CSDN博客
2017年10月09日 20:39:31[刘炫320](https://me.csdn.net/qq_35082030)阅读数：2461
# 1. 前言
今天我们就正式进行Keras实验。在早些时候，我们只是在PC上进行一些小实验，能够让我们对于Keras有一定的直观的认识。下面才是我们进入到最真实的开发环境。
# 2. 磨刀
## 2.1 硬件设施
当然真正的实验不能够在我们个人电脑上运行了。因此我们需要一个功能强大的远程服务器。以个人为例，我们假设目前已经拥有了一个2路的E5-2680V4和4路Tesla K40的Linux的服务器。 
E5-2680V4处理器具有14核心，价格在18000左右，而Tesla K40 具有12GB的显存,价格在27000左右，2013年发布时，号称是地球上最强显卡，当然目前来看，仍然也处于顶尖水平。
|显卡型号|单精度性能|双精度性能|
|----|----|----|
|Tesla P100|10.6|5.3|
|QUADRO GP100|10.3|5.2|
|Tesla K80|8.74|2.91|
|Tesla K40|4.29|1.43|
|GTX 1080|8.7|0.2|
|GTX Titan x|7|0.2|
|GTX 980|4.6|0.15|
## 2.2 软件设施
### 2.2.1Linux登录
当然远程连接少不了远程工具，如果是Linux远程的话，会容易很多，从命令行进入即可。例如:
- 远程管理指定 Linux 服务器
`ssh 用户名@ip`
首次连接服务器时，会提示下载服务器的公钥（通常使用 RSA 加密方式创建 SSH 密钥，所以提示信息一般是“RSA key…”），需要输入 yes 确认。然后输入用户名对应的密码，就可以登录服务器了。要断开与服务器的连接，输入 exit。
- 下载文件
`scp [-r] 用户名@ip:文件路径 本地路径`- 上传文件
`scp [-r] 本地文件 用户名@ip:上传路径`
### 2.2.2Windows登录
Windows虽然日常用起来很方便，但是如果是开发的话，还是比较麻烦的，因此我们需要两个工具，分别是[Xshell](https://www.baidu.com/link?url=FHoXmI0LybxeuqMoCyHCzH8UEh31WIppvJvefIIep73mdiupFpYI8NTr0MvU0FdqUwOTe7vEDfTzBtW2V0scgFeS-3yBe_iVKoUCpIhBgBm&wd=&eqid=a8a28b2500035b7f0000000559db7193)和[Winscp](http://winscp.net/eng/download.php)。其中Xshell主要负责远程命令行，而Winscp则负责传输文件。具体配置我们这里就不多讲，比较傻瓜，只需要知道用户名和密码，就可以很方便的使用。
## 2.3 观察环境
一般来讲，一个服务器环境至少需要以下几个部分：python+Anaconda+Tensorflow+Keras。整体来讲我们都介绍过，我们这里主要提一下Tensorflow的编译问题，具体的可以参考：《[编译Tensorflow](http://xiaosheng.me/2017/09/19/article96/)》、《[源码安装Tensorflow](http://blog.csdn.net/u013832707/article/details/73161071)》等。我们这里不多讲，我们这里讲一讲如何查看我们所使用的GPU和CPU资源。因为如果你对自己的所有性能都不了解的话，怎么能够充分发挥自己的性能呢。
### 2.3.1 GPU查看
对于GPU来讲，一般我们使用英伟达显卡，就如同上面我说的一样，一般GTX1070起跳，好的话可以配置Tesla系列。那么查看英伟达系列显卡的使用情况的代码如下，在命令行中敲入以下命令：
`nvidia-smi`
没错，就是这么简单，回车后，应该看到类似的样子： 
![这里写图片描述](https://img-blog.csdn.net/20171009211545914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当然如果没有看到这样的话，一个可能原因就是你的服务器里没有英伟达系列显卡。
### 2.3.2 CPU查看
如果是查看CPU的话，建议使用以下命令：
`top`
这个命令是用来查看当前进程的，可以看到有哪个用户的哪个进程在活动以及它的CPU使用情况。
`lscpu`
这个代码则是大致的展示以下CPU的概况。 
![这里写图片描述](https://img-blog.csdn.net/20171009211855954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 3. 小试牛刀
下面我们随意使用一个程序来测试一下就像我们本机一样就可以了。但是，我们这里有4块K40显卡，编号为0的显卡已经被占用了。如果原封不动使用以下命令来执行：
`python mnist.py`
显然，它会默认占用编号为0的显卡。这和我们的想法不一致。我们想指定使用某一张显卡怎么办？
`CUDA_VISIBLE_DEVICES=1 python train.py`
或者是在文件的代码里加入以下代码：
```
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "1"
```
这行代码就是指定编号为1的GPU来运行。由于太过兴奋，我们冲昏了头脑，以为可以使用以下代码来实现多个GPU运行：
`CUDA_VISIBLE_DEVICES=1,2,3 python train.py`
是的，没错，我们看到了在GPU上确实开了3个进程，但是我们发现只有1块GPU上使用了加速，也就是Volatile GPU-Util的使用率只有1号显卡在运行，其他显卡的Volatile GPU-Util均为0。
后来我们冷静了下来才发现，`CUDA_VISIBLE_DEVICES`其实指的是CUDA加速库可见的设备。也就是说，这其实是屏蔽某些编号的CPU的代码，而并非是并行代码。
同样的，我们发现在原来的服务器上，仍然有一个和我们有类似情况的进程，那么他是不是也是这种情况呢？
没错，这种情况其实是偶然发生的，如果是全部都占满显存，其实是由于TF的机制所致，它默认是占用全部的显存，而并非用多少拿多少。那么这个该如何改变呢？ 
方法1，限制程序的显存占有量。我们可以使用以下代码：
```python
import os
import tensorflow as tf
os.environ["CUDA_VISIBLE_DEVICES"] = "2"
from keras.backend.tensorflow_backend import set_session
config = tf.ConfigProto()
#限制最多GPU占用为30%
config.gpu_options.per_process_gpu_memory_fraction = 0.3
set_session(tf.Session(config=config))
```
但是，其实当我们的程序真的需要比30%内存多的时候，还是会突破这个限制的。 
方法2，使用增长型内存使用模式。
```python
config = tf.ConfigProto()  
config.gpu_options.allow_growth=True  
sess = tf.Session(config=config)
```
只需要在文件中加入以上代码，就可以根据需求来调用显存了。
那么，我们如何真正使用并行计算呢？《[使用Keras进行分布式/多GPU运算](http://blog.csdn.net/u010159842/article/details/59104029)》、《[Tensorflow分布式实战多机多卡GPU，CPU并行](http://blog.csdn.net/cq361106306/article/details/52929468)》、 
《[Tensorflow cluster 分布式集群使用说明](https://zhuanlan.zhihu.com/p/29640009)》也许会有所帮助。
当然还有一个和这个情况不同的问题：《[GPU Memory Usage占满而GPU-Util却为0](http://www.cnblogs.com/wowarsenal/p/5644813.html)》。
