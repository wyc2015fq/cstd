# 在Macbook Pro上为TensorFlow设置GPU - 沧海一粟 —— 技术随手记 - CSDN博客





2017年06月14日 16:11:16[慢游](https://me.csdn.net/eengel)阅读数：13050








最近忽然发现自己的Macbook Pro上装有一块额外的NVIDIA GeForce GT 750M显卡，于是蠢蠢欲动想装一个TensorFlow，试试在GPU上跑算法的性能。
- 先进入TensorFlow官网的Mac安装[页面](https://www.tensorflow.org/install/install_mac)，发现要先装一堆NVIDIA的软件，于是进入NVIDIA文档[页面](http://docs.nvidia.com/cuda/cuda-installation-guide-mac-os-x/#axzz4jwdDH7qT)。
- 为了安装NVIDIA CUDA，首先确认自己的显卡在支持[列表](https://developer.nvidia.com/cuda-gpus)里。确认完毕后，查看自己的Macbook当前macOS版本为10.12.5，没问题。确认目前Xcode的版本为8.3.3。虽然不是8.2，但下载一个Xcode太大，先不管了。安装command line tools。下载并安装CUDA 8.0.61。CUDA安装包里面包含了3部分：CUDA Driver, CUDA Toolkit, 以及CUDA Samples。很顺利的装完，并测试了Driver，编译了Sample里面的几个例子。
- 问题出来了。当运行deviceQuery的时候，死活都检测不到Mac上的NVIDIA显卡。每次都显示”no CUDA-capable device is detected”。于是在网上搜索了诸多答案。 
- 有一种说法，Macbook Pro默认状态下第二块显卡是休眠的。因此CUDA无法检测到。因此下载了一个叫[gfxCardStatus](https://gfx.io/)的显卡检测工具。可以显示当前活跃的显卡。运行后，右上角状态栏就有一个字母i的图标，打开查看，发现当前活跃的是默认显卡。那如何切换显卡呢？
- 又找到一个答案，说是之所以第二块显卡休眠，是因为当前机器是Energy Save模式。要去System Preference –> Energy Saver里面去掉Automatic graphics switching的选项。再查看gfxCardStatus，果然变成NVIDIA的显卡了！兴冲冲再用deviceQuery检测，很沮丧地发现CUDA还是没找到这块显卡。
- 终于又找到一种新的说法，说明必须CUDA Driver与NVIDIA GPU Driver的版本一致，才能让CUDA找到显卡。而且提供了详细的[版本对照](http://www.macvidcards.com/drivers.html)。于是先根据macOS 10.12.5版本找到相应的GPU Driver 378.05.05.15f01，下载。然后再进入CUDA Driver页面，发现果然之前安装的CUDA 8.0.61版本不匹配，需要安装与378.05.05.15f01匹配的CUDA Driver 8.0.83。两个Driver都装完后，成功发现deviceQuery检测到了NVIDIA的显卡！




