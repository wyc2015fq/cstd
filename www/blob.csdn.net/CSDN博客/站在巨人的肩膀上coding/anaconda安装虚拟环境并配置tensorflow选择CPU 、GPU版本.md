# anaconda安装虚拟环境并配置tensorflow选择CPU 、GPU版本 - 站在巨人的肩膀上coding - CSDN博客





2018年05月16日 22:13:35[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：506








# Anaconda Tensorflow GPU 版本的安装问题



# 尝试一

`准备：`

`conda create -n poseCNN python=2.7`

`conda activate poseCNN`

`conda deactivate`

1. 首先在终端执行：
`anaconda search -t conda tensorflow`- 1

此指令会查询conda环境中有哪些tensorflow安装包，通过查看版本，选择最高的版本安装。如下图 
![这里写图片描述](https://img-blog.csdn.net/20170314100906136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDk3NzAzNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我这里可以看到1.0.0是最高版本，可以选择此进行安装； 
2.因此，第二步输入以下指令，他会告诉你怎么安装这个包，如下图所示：
`anaconda show HCC/tensorflow`- 1

![这里写图片描述](https://img-blog.csdn.net/20170314101329555?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDk3NzAzNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.根据他的提示，在终端执行：
` conda install --channel https://conda.anaconda.org/HCC tensorflow`- 1

从这里我们可以看到这边有两个版本的tensorflow，要安装哪个版本就需要在刚刚指令后面 加入版本信息。如我安装1.0.0 则输入的应该是
` conda install --channel https://conda.anaconda.org/HCC tensorflow=1.0.0`- 1

静待一分钟就安装好了。

## 尝试二

由于做kaggle的需要，CPU版本的tensorflow远远达不到需求，有次我训练25epoch，跑了一夜才跑一半。无奈，卸载cpu版的，考虑安装GPU版的。当然这需要之前安装好cuda和cudnn。

```php
conda list
conda uninstall tensorflow
```
- 1
- 2

卸载掉tensorflow后。需要用`pip install https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-1.0.0-cp27-none-linux_x86_64.whl`安装，但是问题是我的pip是默认使用的python目录下的。于是一怒之下将其卸载，反正也用不到。然后使用上述指令安装，但是苦于并未翻墙，所以总会提示链接失败。

## 尝试三

我想着，既然不能链接上去下载，那我把这个文件通过其他途径先下载下来，在自己用pip去装这个包不就可以了。于是我找了一台可以翻墙的电脑，下载下来，在下载目录下执行：
`pip install tensorflow-1.0.0-cp27-none-linux_x86_64.whl`- 1

果不其然，终于装好了！ 
在终端测试一下呢，终端输入`python`。然后输入`import tensorflow as tf`但是竟然报错了，报错信息如下：

```
ImportError: /opt/Xilinx/Vivado/2015.1/lib/lnx64.o/libstdc++.so.6: version `GLIBCXX_3.4.19' not found (required by /usr/local/lib/python3.4/dist-packages/tensorflow/python/_pywrap_tensorflow.so)

Failed to load the native TensorFlow runtime.
```
- 1
- 2
- 3

网上google有人说GLBC库版本太低，但是又不能更新，因为这是系统基本库，更新了其他软件可能会有问题。。。。ORZ，我怎么这么艰辛。

## 尝试四

根本不报什么希望的我继续找着。突然找到一条指令：
`pip install tensorflow-gpu`- 1

这么短的指令怎么可能有用！不过抱着试试看的想法试了一下，竟然安装成功了。不过具体GPU版本能用否还得实践。如下图 


![这里写图片描述](https://img-blog.csdn.net/20170314105335891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDk3NzAzNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

大功告成



