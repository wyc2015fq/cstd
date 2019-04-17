# 我也曾经因安装库而抓狂，直到我遇到了Anaconda - zhusongziye的博客 - CSDN博客





2018年11月22日 21:18:33[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：161








几乎所有的 Python 学习者都遇到过“**安装**”方面的问题。这些安装问题包括 **Python 自身环境的安装、第三方模块的安装、不同版本的切换，以及不同平台、版本间的兼容问题**等。当你因为这些问题而卡壳，一行代码没写就已疯，相信我，你不是一个人……包括我自己，也是这么被坑过来的。

我自己印象最深的是在 Mac 上安装 pycrypto（加密）和 scipy（科学计算）模块，折腾了很久。因为这类模块并不是单纯的 Python 代码，而是需要调用诸如 C 语言的库，于是就牵涉到在不同平台上的编译，有些还不能很方便地用虚拟环境分离版本。在数次尝试失败之后，最终靠着一样工具竟轻松搞定。这就是今天要介绍的：

**Anaconda**

应该有不少人已经了解和在使用中，另外也考虑到先 Mark 以后需要时再看的收藏党，这里先给个“**嫌长不想看**”版：
- 
**Anaconda 就是一个扩展版 Python**，最直接的好处是帮你**打包好了一整套数据科学相关的 Python 库**，一次安装就可以拥有数据分析、数值计算、数据可视化、机器学习常用的几十个库，不用担心安装不成功、版本不匹配等问题，省时省心。

- 
****Anaconda 的核心是 **conda 这个包/环境管理器**，类似于我们之前介绍过的虚拟环境 virtualenv，且功能更丰富。装了 Anaconda，同时也就可以使用 conda 来管理电脑上不同版本的 Python 环境。

- 
如果对 conda 的使用不熟悉也没关系。**Anaconda 还附带了一个叫做 Navigator（导航）的桌面 GUI 工具，可以直观的创建和管理环境****，安装、删除扩展包**。

- 
Anaconda 安装后，附带了 Jupyter 和 Spyder 两种 IDE。**Jupyter 是一种基于浏览器的交互式开发环境**，这种边开发边执行的模式很适合编写数据科学类的程序。Spyder 的界面和 Matlab 很相似。不过如果你对 **PyCharm** 已经熟悉，也可以直接通过**设置 interpreter（解释器）路径**来直接使用 Anaconda 的环境。


主要就是这几点，如果你在使用中遇到过什么问题，欢迎在文本下方留言讨论。有其他想法或者想听的内容，也欢迎告诉我们。



**Anaconda** 这个词，字面意思和 Python 类似，都是大蟒蛇。系列电影《狂蟒之灾》的主角就是这玩意儿。美国著名嘻哈歌手麻辣鸡（Nicki Minaj）有首歌就叫《Anaconda》（本文的封面，小孩子不要去搜）。这个项目命名其实很形象：**Anaconda 就是一种更大的 Python**。

再做个类比：安装了** Python 就像买了个毛坯房**，虽然刷了涂料通了水电（内置库），但你真的想住进去，还得自己根据需要进行装修（安装各种第三方库）。而 **Anaconda 就是个精装修**。这个精装是面向数据科学的，同时也保留了你自己进一步改装的空间。

官网上有一个 Anaconda 支持库的列表：

http://docs.anaconda.com/anaconda/packages/pkg-docs/

里面有很多熟悉的面孔（挑了其中几个常见的）：

![](https://img-blog.csdnimg.cn/20181122211403949.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

打钩的是包含在安装包中，未打钩的需要后续手动安装。数据分析必备的 **numpy、pandas、scipy、matplotlib** 自不用说，像数据采集的 **requests****、****beautifulsoup、scapy**，Web 开发的 **flask**，GUI 的 **pyqt**，图像处理的 **pillow**，机器学习 **scikit****-learn**，都直接帮你装好了。就连 **vs2015_runtime** 这种也为你贴心附上，被坑过的同学应该都懂。

唯一的缺点大概就是这样会比较占空间。但既然你都下决心深入 Python 开发了，这也就是少装一个游戏的空间吧。

如果你安装 Anaconda 的话，是不必装 Python 的，因为它本身包含了 Python 的环境，避免了版本不匹配的问题。**Windows、Mac、Linux **三个平台都支持，直接从官网下载安装即可。（建议选择最新版）

https://www.anaconda.com/download/

![](https://img-blog.csdnimg.cn/20181122211421400.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

正常按提示安装没有太大问题，网上的安装示例也一搜一大把，这里不赘述。Windows 建议安装时**右键点击**，选择“**以管理员身份运行**”。

安装好之后，检查下你的 Python 是否已经是 Anaconda 环境下的了。（从提示中可以看出）

![](https://img-blog.csdnimg.cn/20181122211505541.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这时候，你可以通过 **conda** 来管理你的安装包和环境。使用方法和我们之前介绍过的**virtualenv** 类似（参见《[为什么你的python版本一团糟？因为少了这个操作](http://mp.weixin.qq.com/s?__biz=MjM5MDEyMDk4Mw==&mid=2650167406&idx=1&sn=0dcea49542ab9c10d6d3fcccef5d9c0e&chksm=be4b5516893cdc00d499d780130158cd1e44087bd3fe0b0319084dfc55c10e7347bc74b56c75&scene=21#wechat_redirect)》）。常用的命令有
- 
**conda list**：查看环境中的所有包

- 
**conda install XXX**：安装 XXX 包

- 
**conda remove XXX**：删除 XXX 包

- 
**conda env list**：列出所有环境

- 
**conda create -n XXX**：创建名为 XXX 的环境

- 
**conda env remove -n XXX**：删除指定环境

- 
**activate XXX**（或 source activate XXX）：启用 XXX 环境

- 
**deactivate**（或 source deactivate）：退出环境


同 pip 一样，如果使用 conda 安装很慢，可以通过**修改国内源**的方式来加速。修改C:\Users\当前用户名\.condarc（非 Windows 是 ~/.condarc），加入如下配置：

```
channels:
 - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
 - defaults
show_channel_urls: true
```

Anaconda 的又一贴心之处在于，给了你一个叫做** Anaconda Navigator** 的桌面 GUI 工具，把上述 conda 的功能都做成了点击按钮就可以完成的事情。

![](https://img-blog.csdnimg.cn/20181122211643856.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

包管理和环境管理都一目了然。

**数据科学库、包/环境管理、Navigator 工具**，这几样就是 Anaconda 的主要功能。

![](https://img-blog.csdnimg.cn/20181122211715702.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

另外值得一提的是：**A****naconda 是附带了一系列第三方库的 Python 以及对这些库的管理工具**，和我们之前说的 PyCharm 不是一类东西。**PyCharm 是 IDE，用来写代码的编辑器**。你可以把 Anaconda 作为运行环境放在 PyCharm 里使用。只要你**创建项目的时候选择已经配置好的 Anaconda 环境**即可。关于 PyCharm 的具体配置可以在公众号（Crossin的编程教室）里回复关键字 **pycharm**

而 Anaconda 默认附带的另一个开发工具 Jupyter，也是非常值得推荐的。你可以从 Navigator 里运行，或者在命令行执行命令：
`jupyter notebook`


![](https://img-blog.csdnimg.cn/20181122212025914.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

它是一个**基于浏览器的交互式开发工具**。跟 Python 自带的交互环境相比，它的自动完成和提示功能都强大许多，并且你不用再纠结不能在交互环境里写多行代码的问题。

![](https://img-blog.csdnimg.cn/20181122211949552.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这种代码分块，可以边开发边执行的模式，**非常适合编写爬虫、数据处理、数据分析**等数据科学类的程序。我之前的很多案例都是使用它来开发，你们看到项目代码中的**.ipynb **文件就是可以导入 Jupyter 的文件。另外它还有个好处，就是可以部署在自己的服务器上使用（当然这也有安全风险），这样只要有网络，你就能随时随地写 Python 了。

不管是 Anaconda 还是 Jupyter，以及之前推荐多次的 PyCharm，到底好不好用，是不是适合你，自己试过了才知道。花点时间装起来用一下，然后欢迎你回来报告使用体验。



