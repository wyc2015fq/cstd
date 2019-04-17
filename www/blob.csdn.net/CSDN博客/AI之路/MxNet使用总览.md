# MxNet使用总览 - AI之路 - CSDN博客





2017年08月17日 08:06:23[AI之路](https://me.csdn.net/u014380165)阅读数：7893
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









写这篇博文的初衷是希望能整理下自己在使用MxNet过程中对这个框架的理解。详细的介绍都给出了链接，这篇相当于一个概括，希望能对这个框架有一个总的认识，内容会不断补充，有疑问的欢迎交流，谢谢。

**1、下载MxNet并编译**

MxNet的官方网址：[http://mxnet.io/get_started/install.html](http://mxnet.io/get_started/install.html)

我这里选择如下安装方式：
![这里写图片描述](https://img-blog.csdn.net/20170817075602776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后按照 **Build the MXNet core shared library** 介绍的4个步骤进行安装即可：

step1主要是更新一些工具和git，apt-get就是从网上获取并安装的意思。 

Step 1 Install build tools and git.

```
$ sudo apt-get update
$ sudo apt-get install -y build-essential git
```

step2是安装OpenBLAS，这是一个计算加速库，当你step4要编译openlab的时候如果这个地方没有安装好openlab就会报错。 

Step 2 Install OpenBLAS.
`$ sudo apt-get install -y libopenblas-dev liblapack-dev`
安装opencv，opencv主要用在图像数据导入和预处理。 

Step 3 Install OpenCV.
`$ sudo apt-get install -y libopencv-dev`
前面3个step可以看成是准确工作，step4是拉取mxnet项目，然后编译，注意编译的时候有几个参数，如果要用GPU的话，USE_CUDA是必须要有的，另外USE_CUDNN是NVIDIA的一个深度神经网络加速库，一般也是需要的，如果没有的话最多速度慢一些，但是可以跑，建议用。 

Step 4 Download MXNet sources and build MXNet core shared library.

```
$ git clone --recursive https://github.com/dmlc/mxnet
$ cd mxnet
$ make -j $(nproc) USE_OPENCV=1 USE_BLAS=openblas USE_CUDA=1 USE_CUDA_PATH=/usr/local/cuda USE_CUDNN=1
```

**如果你没编译mxnet就在python环境中import mxnet，就会出现找不到mxnet的错误。**
**上面这些运行成功后，你可以在mxnet/python/目录下进入python环境，并且import mxnet成功，但是如果想要在任何地方都可以成功import mxnet，还需要以下操作：**

step 1：安装一些需要的库`$ sudo apt-get install -y python-dev python-setuptools python-numpy python-pip`
step 2：先升级pip命令，然后用**pip install -e .表示install a project in editable mode，就可以在任何地方import mxnet了。**

```
$ cd python
$ pip install --upgrade pip
$ pip install -e .
```

**2、训练模型的入口**

我们知道训练一个模型需要定义一个网络结构，然后喂给这个网络数据，最后训练得到合适的模型。又因为现在基本上都是在预训练模型上做fine-tune，所以这里直接介绍MxNet里面fine-tune的快速操作。可以参看博文：[MXNet的预训练：fine-tune.py源码详解](http://blog.csdn.net/u014380165/article/details/73864457)。 

在fine-tune.py这个脚本中，最重要的是调用了fit.fit()函数训练，这个fit.py脚本的介绍可以看博客：[MXNet的训练入口：fit.py源码详解](http://blog.csdn.net/u014380165/article/details/73928755)。这个脚本也是在最终训练之前封装好的一个脚本。那么具体训练的细节在哪呢？答案是base_module.py和module.py。base_module.py是MxNet训练模型的基类的脚本，而module.py则是继承base_module里面的基类的具体实现，包括前向和后向传递等等，相关的博文介绍可以参看：[MXNet的训练基础脚本：base_module.py](http://blog.csdn.net/u014380165/article/details/75142829)和[MXNet的训练实现脚本：module.py](http://blog.csdn.net/u014380165/article/details/75142946)。
**3、关于数据读取和预处理**

fine-tune.py这个脚本是MxNet官方写好的，默认的数据读入方式是.rec，类似Caffe里面的LMDB，关于数据读取，可以参看博文：[MXNet的数据读取：data.py源码详解](http://blog.csdn.net/u014380165/article/details/73928835)。这种.rec格式的数据一般需要较多的存储空间，当你需要灵活增减数据时候需要重新生成新的.rec文件，不是很灵活，因此推荐使用lst列表和原始图像结合的数据读取方式，可以参看博文：[MXNet如何用mxnet.image.ImageIter直接导入图像](http://blog.csdn.net/u014380165/article/details/74906061)。这种数据读取的方式还涉及图像预处理的部分，原来的MxNet项目中没有相应的例子，可以参看博文：[MXNet的数据预处理：mxnet.image.CreateAugmenter源码详解](http://blog.csdn.net/u014380165/article/details/74906572)。了解在mxnet.image.ImageIter中是怎么做图像预处理的。






