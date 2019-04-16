# 64位Windows10下tensorflow的安装 - PeterBishop - CSDN博客





2018年11月29日 14:58:18[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：85








网上有很多关于Windows系统下安装tensorflow的教程，方法大致有几种，但是细节却各有不同。我看了很多同学写的安装教程，实践后都遇到过各种各样的小问题（其实主要还是自己对TF不熟悉）。在对很多教程进行类比和综合之后，成功安装。在这里仅作为一个记录，也给后来需要安装的同学一些参考。

关键点1：Python环境

在windows环境下，tensorflow只支持python3.5版。这里建议安装Anaconda，它集成了后面安装和学习的各种工具和库，使用非常方便。

我的方法：

下载并安装最新版的Anaconda（它对应的python版本是3.6），然后创建python3.5环境（命令行窗口下输入）： 

conda create -n tensorflow python=3.5 

注意，这里的tensorflow是会作为文件夹的文件名存在 *:\Anaconda3\envs\ 下,所以这个名字是可以自己设计的。

激活conda环境: activate tensorflow

安装CPU版本输入 

pip install tensorflow 

安装GPU版本输入 

pip install tensorflow-gpu

关键点2：安装CUDA和cuDNN

验证tensorflow安装是否成功，可以在命令行窗口输入python进入python环境，或者运行python3.5命令行后输入以下代码： 

import tensorflow as tf 

如果是安装的GPU版本，很大可能报错，因为还没有安装CUDA和cuDNN。

你可以开始下载 CUDA 和 cuDNN 的安装包了，注意版本号分别是 CUDA8 和 cuDNN5，这是 Google 官方推荐的。可以去各自官网下载，我已经下载好打成一个压缩包放到了百度云，大家可以从 这里 下载链接: https://pan.baidu.com/s/1c2j3Z9I 密码: t95c。 

——地址http://blog.csdn.net/zhunianguo/article/details/53536425

这里需要注意的是，版本对应问题，CUDA的版本在2017年12月已经出了对应Windows10的9.0版本，但是此处我们这里的tensorflow是对应python3.5（并不是最新的），自然我们也要选择相应的版本。CUDA8.0是我们所需要的，根据你的系统是32还是64位的，具体选择。上面我给出了一个同学的链接，这里要说的是，我是在官网下载的，地址给出： 

https://developer.nvidia.com/cuda-downloads 

记住，我们不是要最新的，只要CUDA8！ 

上述同学CUDA8+cuDNN5的组合，我是没有成功，稍作研究，发现tensorflow提示需要cuDNN6版本，既然它这么说，我们就去https://developer.nvidia.com/cudnn下载版本6。 

CUDA和cuDNN下载分版本分对应Linux、Windows、MacOS系统，还有X86和X64处理器的区别，请根据情况选择，同时注意各组件要匹配版本

下载完两个文件，那个exe文件就是 CUDA8 的安装程序，像安装正常软件一样操作，安装过程屏幕可能会闪烁且安装时间有点长。安装完之后系统变量会自动为你添加上。测试一下是否安装成功，命令行（要重启）输入 nvcc -V，看到版本信息就表示安装成功了。

cuDNN是一个lib、dll、还有h头文件的组合，需要添加到CUDA8文件夹中去，具体这一步操作可以参考链接文章中的安装cuDNN，此处不做重复叙述。

关键点3：安装Numpy、Scipy 

再测试tensorflow，提示我Scipy模块无法找到，那么就安装一下Scipy。 

这个库需要先安装Numpy，具体可以参考http://blog.csdn.net/u014206910/article/details/60571325

此后，我的tensorflow就已经成功安装了。如果后续还有想起什么新的内容，我再重新编辑。——2017.12.18




