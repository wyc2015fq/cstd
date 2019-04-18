# Windows下anaconda安装第三方包的方法小结(tensorflow、gensim为例) - fighting！！！ - CSDN博客
2018年09月18日 18:15:16[dujiahei](https://me.csdn.net/dujiahei)阅读数：467
转自： [https://www.jb51.net/article/137782.htm](https://www.jb51.net/article/137782.htm)
anaconda 集成了很多科学计算中所需要的包，如numpy，scipy等等，具体查看anaconda中已经预先安装配置好的包有哪些，可以通过cmd命令，输入conda list 查看，如下图所示：
![](https://files.jb51.net/file_images/article/201804/201804051145231.png)
        但是，因为实际需求，我们会需要导入列表中没有的第三方包，如gemsim，在anaconda中，我们可以参考以下步骤安装所需要的第三方包：
**        1、启动anaconda 命令窗口：**
         开始 > 所有程序 > anaconda >anaconda prompt
![](https://files.jb51.net/file_images/article/201804/201804051145232.png)
**   2、安装gensim包**
     在anaconda命令窗口中，输入pip install gensim
![](https://files.jb51.net/file_images/article/201804/201804051145233.png)
**3、安装成功**
![](https://files.jb51.net/file_images/article/201804/201804051145234.png)
不过上面的速度真的很慢，建议用
`conda install gensim`
补充：
Anaconda概述
Anaconda是一个用于科学计算的Python发行版，支持 Linux, Mac, Windows系统，提供了包管理与环境管理的功能，可以很方便地解决多版本python并存、切换以及各种第三方包安装问题。Anaconda利用工具/命令conda来进行package和environment的管理，并且已经包含了Python和相关的配套工具。
这里先解释下conda、anaconda这些概念的差别。conda可以理解为一个工具，也是一个可执行命令，其核心功能是包管理与环境管理。包管理与pip的使用类似，环境管理则允许用户方便地安装不同版本的python并可以快速切换。Anaconda则是一个打包的集合，里面预装好了conda、某个版本的python、众多packages、科学计算工具等等，所以也称为Python的一种发行版。其实还有Miniconda，顾名思义，它只包含最基本的内容——python与conda，以及相关的必须依赖项，对于空间要求严格的用户，Miniconda是一种选择。
进入下文之前，说明一下conda的设计理念——conda将几乎所有的工具、第三方包都当做package对待，甚至包括python和conda自身！因此，conda打破了包管理与环境管理的约束，能非常方便地安装各种版本python、各种package并方便地切换。
但是我们有时候还是要自己安装三方包。比如tensorflow这样用于深度学习的包。
**为此我用了三种方法安装。**
第一种是打开anaconda prompt，输入pip install tensorflow，但是whl文件下载到2%左右报错，尝试几次都是这样
![](https://files.jb51.net/file_images/article/201804/201804051148415.png)
第二种方法是从官网下载支持window系统的三方包[点击打开链接](http://www.lfd.uci.edu/~gohlke/pythonlibs/)，然后在anaconda prompt中输入pip install 路径+whl文件名，但是出现以下结果：
![](https://files.jb51.net/file_images/article/201804/201804051148416.png)
输入`pip list`后并没有出现tensorflow包。
第三种是输入 `conda install tensorflow`，出现如下所示
![](https://files.jb51.net/file_images/article/201804/201804051148417.png)
最后不但安装了包括tensorflow的三方包，还更新了一些旧的三方包。
![](https://files.jb51.net/file_images/article/201804/201804051148418.png)
这样就完美的解决了问题。
**下面针对whl文件的安装方法**
使用pip命令安装，没有该命令的话需要先安装pip命令包，切换到python下script目录
去该网站下载http://www.lfd.uci.edu/~gohlke/pythonlibs/#libsvm 所需要的第三方包，放在E:\selenium\
目录下，使用以下命令安装。
支持python 3.0以上版本
pip install E:\selenium\libsvm-3.22-cp36-cp36m-win_amd64.whl
支持python 3.0以下版本
pip install E:\selenium\libsvm-3.22-cp27-cp27m-win_amd64.whl
32位系统安装
pip install E:\selenium\libsvm-3.22-cp27-cp27m-win32.whl
