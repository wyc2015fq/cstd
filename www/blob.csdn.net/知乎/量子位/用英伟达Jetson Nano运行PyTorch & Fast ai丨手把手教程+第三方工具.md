# 用英伟达Jetson Nano运行PyTorch & Fast.ai丨手把手教程+第三方工具 - 知乎
# 



> 原作 Interogativ
郭一璞 编译
量子位 报道 | 公众号 QbitAI

英伟达Jetson Nano，作为老黄家最便宜的硬件，只要99美元。

看到便宜就想捡，拿到手里怎么玩？

造机器人、造小车、在家里DIY一个语音控制的窗帘，都得先装轮子。

如果你使用的框架是PyTorch，那你最好再装一个Fast.ai。

Fast.ai是在PyTorch上非常好用的深度学习库，来自MOOC平台Fast.ai，只要一个API，就包含了所有常见的深度学习应用。

那么，拿到Jetson Nano之后，如何把轮子部署上去呢？一位名叫Interogativ的用户就把用它运行PyTorch 1.0和Fast.ai 1.0的攻略发在了Fast.ai的论坛上，量子位全文编译如下：
![](https://pic4.zhimg.com/v2-59117bcc7e7a3dc443eec8bb3ee412e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='380' height='214'></svg>)
英伟达Jetson Nano，配备了ARM Cortex A57处理器和4G内存，CUDA和PyTorch两者都可以在上面运行，这块开发板有难以置信的价值。

当然，你也可以用它来运行截至2019年4月最新最好的PyTorch和Fast.ai版本，这里是攻略。**本攻略仅适用于Python 3，如果你没有Linux经验，不知道如何使用SSH，不知道IP网络的原理或者IP地址是什么，不建议用本攻略来安装。**

## **软硬件准备**

1、花99美元买一块英伟达Jetson Nano，现在国内已经可以买到了，当然，价格会比99美元（665人民币）要贵一点。

2、一张**64G的Micro SD卡**，确保它是**class 10**或者更高级别。
![](https://pic4.zhimg.com/v2-66818c22d6b17c150f29448320e024cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='653'></svg>)△ SD卡速度等级，来自SD协会网站
3、**USB键盘**，就台式机上那种。

4、**2.5Amp电源线**，带一个Micro-USB插头。

5、一个**HDMI或者DisplayPort连接线**和**显示器**。

6、连网：以太网网线，无线路由器或者HUB，这块板子没有本机无线支持，所以请确保你能用**有线网络**。

7、**能插Micro SD卡的PC**，不能插SD卡的话你还需要准备读卡器。

8、可以创建SSH终端的PC软件，以及可以使用SSH传输文件的软件。如果你用Windows的话，我推荐[Tera Term](https://link.zhihu.com/?target=https%3A//ttssh2.osdn.jp/index.html.en)（免费）和[WinSCP](https://link.zhihu.com/?target=https%3A//winscp.net/eng/docs/lang%3Achs)（免费），

9、把[这些zip文件](https://link.zhihu.com/?target=https%3A//gist.github.com/interogativ/4f5669dad3f9252a5709dc6844e12a29/archive/50845f5e9e8994eec78a4b27c6f23e4822ba00d7.zip)下载到你的PC上，里面有作者Interogativ写的脚本，包括：

> setup_swapfile.sh
fastai_jetson.sh
setup_jupyter.sh
jetson_headless.sh
jupyter_notebook_config.py

当然，得记住你的存储路径。

## **可以开箱啦**

你的新板子到货之后，无视里面自带的纸架，找一些防静电的泡沫塑料，其他的防静电的材料也行，把你的板子放上去。
![](https://pic2.zhimg.com/v2-35e3e3daf5d3d254bdc6d5cf54cb26b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
打开[英伟达开发者网站Jetson Nano开发套件指南](https://link.zhihu.com/?target=https%3A//developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit)，按照里面写的一步一步操作，确保：

1、下载带有SD卡映像的Nvidia zip文件。

2、按照说明把这些文件装到SD卡里。

3、将SD卡插入Nano，插上USB键盘、显示器和网线。

4、开机，接受各种许可协议之类的。

5、设置用户名密码，别忘了。

## **联网之后**

1、在网络设置里找到你Nano的IPV4地址，记下来，或者如果你懂IP网络的话，设置一个固定的IP地址。

2、用PC终端程序打开与Jetson Nano的SSH会话。

3、用文件传输软件把之前下载的zip文件传到Nano的用户主目录下。

4、通过SSH把你的PS和Nano的IP地址连起来。

5、从控制台或通过SSH连接，为刚刚下载的脚本设置执行权限：


```
1chmod +x fastai_jetson.sh

2chmod +x jetson_headless.sh

3chmod +x setup_swapfile.sh

4chmod +x setup_jupyter.sh
```


## **设置交换文件**

Nano只有4GB的RAM（GPU共享），你需要设置一个交换文件，如果已经有了就跳过此步骤。你可以从终端会话中运行setup_swapfile.sh：

`./setup_swapfile.sh`

确保**这一步只进行一次**，因为脚本里没写检查是否已设置，你可以执行以下操作检查是否设置了交换文件：

`free`

这时候你就应该看到创建了一个8GB的交换文件。

## **安装PyTorch和Fast.ai**

如果这会儿你就想安装标准的Fast.ai和PyTorch，那就装吧，反正装不上。

这会儿，标准的pip命令根本没法用，但是如果你只是运行fastai_jetson.sh脚本，就可以把俩都装上。这个过程需要几个小时，所以不用屏住呼吸。

`./fastai_jetson.sh`

## **安装Jupyter笔记本**

安装好Fast.ai后，它会告诉你：

`Done with part1 – now logout, login again and run setup_jupyter.sh`

这是因为Jupyter安装不会导出运行所需的shell变量。所以现在，关闭所有终端、SSH会话等，从图形界面重启Nano。重启完了就从图形界面打开终端，然后：

1、确保您下载的jupyter_notebook_config.py文件位于Nano的主目录中。

2、运行./setup_jupyter.sh。

`./setup_jupyter.sh`

这也得一阵儿，所以这里也不用屏住呼吸。

此脚本的最后一步需要Jupyter密码，记住：

不是Jupyter登录密码！

不是Jupyter登录密码！

不是Jupyter登录密码！

就是设置一个单独的密码，你可以用这个密码在你网络上的任何PC上登录Jupyter笔记本，设置好密码后记住它。

默认的Jupyter笔记本安装只能从控制台或图形界面登录，之前下载的修改后的jupyter_notebook_config.py文件和脚本安装可以从你网络上的任何计算机登录。

运行Jupyter笔记本的话，需要打开终端或SSH实例并运行：

`jupyter notebook`

如果它没有运行，可能是因为你没有退出后再次登录。

好了，现在你可以运行PyTorch和Fast.ai了。

但如果你像我一样，你不需要在Nano上使用图形界面，并且想把所有内存都拿来跑程序的话，就接着往下看。

## **关于Python，Pip和VirtualEnv**

一些有经验的Python用户习惯于虚拟环境（virtualenv，conda），它要求您在安装或运行软件之前激活“source activate”，我们现在还没安装，其中一个副作用是pip和Python命令将在活动环境中自动运行Python 3或pip 3。

你必须用pip3和Python3分别运行pip和Python。因此，如果你正在运行一些使用!python xyz的笔记本，除非你将代码更改为!python3 xyz，否则它将无效。

存储空间不是一切，但绝对非常重要：

过去，比如说2010年，4GB的内存很大。如果不在这块板子上跑GPU，那就足以让笔记本电脑运行良好（8 GB的交换文件有很大帮助）。但是你要想跑CUDA，那就没法在交换磁盘上运行，因此它占满了4GB的每个字节。

所以，为了省下一些存储空间，放弃图形界面吧，通过SSH用远程控制台操作。

现在，如果你准备好了，就找到最后一个脚本jetson_headless.sh，运行它，卸载图形界面，还能顺带清除300多M的不必要的软件包。

运行之后，重新启动，你就能拥有Nano控制台访问权限，现在机器只占用了大约378M的内存，留下了3.6G左右的空间可以运行PyTorch和Fast.ai。

1、运行


```python
./jetson_headless.sh
```


2、重启，用SSH连接Nano。

最后说明一下，本文发布的2019年4月，这种方法是可以安装PyTorch 1.0和Fast.ai 1.0的，如果以后版本更新了或者需要装别的东西，欢迎去原文git repo~

## **传送门**

攻略原文：

[https://forums.fast.ai/t/share-your-work-here/27676/1274](https://link.zhihu.com/?target=https%3A//forums.fast.ai/t/share-your-work-here/27676/1274)

Tera Term

[https://ttssh2.osdn.jp/index.html.en](https://link.zhihu.com/?target=https%3A//ttssh2.osdn.jp/index.html.en)

WinSCP

[https://winscp.net/eng/docs/lang:chs](https://link.zhihu.com/?target=https%3A//winscp.net/eng/docs/lang%3Achs)

zip文件下载链接：

[https://gist.github.com/interogativ/4f5669dad3f9252a5709dc6844e12a29/archive/50845f5e9e8994eec78a4b27c6f23e4822ba00d7.zip](https://link.zhihu.com/?target=https%3A//gist.github.com/interogativ/4f5669dad3f9252a5709dc6844e12a29/archive/50845f5e9e8994eec78a4b27c6f23e4822ba00d7.zip)

英伟达开发者网站Jetson Nano开发套件指南

[https://developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit](https://link.zhihu.com/?target=https%3A//developer.nvidia.com/embedded/learn/get-started-jetson-nano-devkit)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


