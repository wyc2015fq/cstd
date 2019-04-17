# 为了交房租，他出了个训练星际争霸2 AI的教程 - 知乎
# 



> Root 发自 凹非寺
量子位 出品 | 公众号 QbitAI

自从去年8月10号暴雪开放了星际争霸II的人工智能API之后，数不清的AI研究者前赴后继地加入了训练星际2 AI的队伍中。

但并非所有人都舍得掏钱购买昂贵的GPU。还好，谷歌近期大发善心提供免费的云GPU，这下有更多的人可以训练星际2AI了。

那用免费的GPU训练AI会遇到哪些坑？

Reddit上有个小哥哥Franklin H.分享了这个排雷的过程，一步步引导大家如何debug。
![](https://pic3.zhimg.com/v2-d1939006c9738fdf3b33b86378149786_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='252' height='252'></svg>)
以下是post全文：

> 太长不看版：如果你想用GPU硬件搭建一个免费的星际争霸II机器学习的环境，看我在谷歌Colab上的笔记：[https://colab.research.google.com/drive/1AzCKV98UaQQz2aJIeGWlExcxBrpgKsIV](https://link.zhihu.com/?target=https%3A//colab.research.google.com/drive/1AzCKV98UaQQz2aJIeGWlExcxBrpgKsIV)

最近，我和几个好基友一起搞了个星际争霸II的项目。

我始终持有一个观点，就是搞机器学习研究的人，得有能力快速训练神经网络。这一点相当重要。

把代码分享出去，是相对简单的事。但我认为，对于全球训练星际争霸II AI智能体的研究群体来说，更有价值的是告诉他们，怎样用谷歌免费的GPU，在谷歌Colab跑起来星际争霸II的AI。

我就自己先动手试了一下。

没想到，下载完星际争霸II和安装上必要的数据库之后，遇到这茬：
![](https://pic4.zhimg.com/v2-4e2babd38981a02cf5381adc10e2822b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='696' height='349'></svg>)
## **Return代码是啥玩意儿**

看来得先搞清楚Return代码是个啥。

我翻出PySC2 source看了下，找到了设置Return代码的部分。
![](https://pic2.zhimg.com/v2-e53e400d5142648b4b2dcbc05804ff51_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='168'></svg>)
进一步挖掘，发现poll()这一块是源自Python的subprocess模块。这说明11是终止星际争霸程序的信号。

信号11是可怕的segfault，绝对算C程序员的噩梦。

为了看问题是不是出在这，我找到了可执行的SC2，然后让它自己运行起来。
![](https://pic2.zhimg.com/v2-bca60bf81b520ca896d86147faa61d45_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='696' height='84'></svg>)
Emmm……

## **开启Debug的地狱模式**

一般来说，我会直接打开用得最顺手的Debug工具。但这样，这篇文章就不过是个怎么用GDB的流水账了。

但，这不是在Google Colab上嘛，我们有的只是Jupyter Notebook网页。这就意味着：
- 没有Debugger；
- 没有Root权限；
- 好多工具不能用，比如说Strace。
![](https://pic2.zhimg.com/v2-f1b10300cc674e0570e8d3ea2769a129_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='539'></svg>)RIP debug中
## **当你只能用网页时……**

我就试了服务器上不同版本的星际争霸II，包括暴雪提供4.0.2版本，3.17版本，和3.16.1版本。

居然！都不行！生气……

那我只能在本地Linux系统上跑星际争霸II了，毕竟这个环境我熟，有把握可以debug。这样才能验证我的猜想。

**第一个可能的原因：没找到必需的数据库**

刚开始我猜，星际争霸作为一个游戏，应该是要一些OpenGL里的函数以及库。而这些东西在谷歌Colab里没有。

为了验证这个假说，这次我本地运行了星际争霸II。然后用strace，记录星际争霸II的一切系统调用，所有的库都是从操作系统加载的，这样就能知道缺了什么依赖库。

这里是日志中的一个片段：
![](https://pic4.zhimg.com/v2-43ffb2978f3c9b94079b5fae61eea0fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='696' height='437'></svg>)
完整的日志见：[https://docs.python.org/3/library/subprocess.html#subprocess.Popen.returncode](https://link.zhihu.com/?target=https%3A//docs.python.org/3/library/subprocess.html%23subprocess.Popen.returncode)


看结果，发现StarCraft II除了动态地链接到那些标准C/C++库，没做什么别的，这就排除了这个猜测。

## **还有哪些segfault？**

又因为本地跑的没有挂，说明暴雪给的码也没有问题。

谷歌搜怎么debug段错误的时候，想起了Valgrind。我惊讶地发现，他居然就在谷歌Colab工作。

Valgrind给出代码片段如下：
![](https://pic1.zhimg.com/v2-5a5b6aaf1d781ff25cfc8c00acacd3ac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='697' height='744'></svg>)
我唯一能看出来的函数是libtcmalloc.so.4.3.0里的MallocExtension::Initialize()。


可能有不知道TCMalloc的盆友，这其实是个谷歌定制化记忆分配器。谷歌Chrome浏览器用的也是这个。

但是……

当我用strace找错的时候，我只记得看到了C和C++标准库的加载。那TCMalloc是从哪里冒出来的？

后来发现，有个办法可以在没有TCMalloc的程序上强行使用TCMalloc。

在Linux上设定LD_PRELOAD环境变量，加载TCMalloc共享库后，就搞定。

很好奇，在谷歌Colab上看会是什么样？
![](https://pic4.zhimg.com/v2-e7a2acf88658539d25a44bf2de53a7d3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='330'></svg>)矮马！就这个！
## **解决方案**

可是，设定LD_PRELOAD环境变量无法扩展到其他环境里去。

执行这段：
![](https://pic4.zhimg.com/v2-9e14c865cfa0e953ac7c30afad52cfbf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='697' height='62'></svg>)
我已经卸载了TCMalloc。然后，除了一些错误的提示信息，星际争霸II现在可以跑起来了。这会儿总算可以开始训练星际争霸II的AI了。


我写了份针对谷歌Colab的bug文档，这样以后大家遇到类似问题就不用再纠结了。

bug文档：[https://docs.python.org/3/library/subprocess.html#subprocess.Popen.returncode](https://link.zhihu.com/?target=https%3A//docs.python.org/3/library/subprocess.html%23subprocess.Popen.returncode)

不过，等星际2的AI出来还早。
![](https://pic1.zhimg.com/v2-28c17363a0eb9223845f3279567485e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='697' height='573'></svg>)
我们拭目以待。

最后我想感谢一下Paul和William，StarAI，是他们告诉我PySC2，以及带的我入坑机器学习。


如果大家遇到高难度的bug，有debug的需求可以找我，我可以提供相应的服务。这是我LinkedIn的介绍，[https://www.linkedin.com/in/franklin-h-804b85a0/](https://link.zhihu.com/?target=https%3A//www.linkedin.com/in/franklin-h-804b85a0/)

最后，附小哥哥post链接：
[https://medium.com/@n0mad/how-i-trained-starcraft-2-ais-using-googles-free-gpus-44bc635b0418](https://link.zhihu.com/?target=https%3A//medium.com/%40n0mad/how-i-trained-starcraft-2-ais-using-googles-free-gpus-44bc635b0418)

**你可能感兴趣**

[星际争霸界也出了位“带路党”，当年的冠军正帮谷歌AI击败人类](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247491465%26idx%3D1%26sn%3Dff4047dbee8431fba148ad8062bb8543%26chksm%3De8d3a8fbdfa421ed2f594c002c467d7f14a5b9284e1ef63fc5c431c90d690b82d228d0c4a310%26scene%3D21%23wechat_redirect)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


