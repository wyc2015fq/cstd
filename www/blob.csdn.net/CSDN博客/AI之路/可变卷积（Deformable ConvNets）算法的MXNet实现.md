# 可变卷积（Deformable ConvNets）算法的MXNet实现 - AI之路 - CSDN博客





2017年06月16日 22:37:54[AI之路](https://me.csdn.net/u014380165)阅读数：6262








**本文是可变卷积（Deformable ConvNets）算法的MXNet实现，MXNet是一种深度学习框架。一开始想熟悉这个框架的话可以跑跑官方网站上的MNIST或CITAR数据集等等，本篇博文更进一步，主要是通过这个算法实例来熟悉MXNet框架的使用。**

**博文主要分三大部，第一部分是拉取MXNet项目，第二部分是拉取你的算法项目和运行demo，第三部分是自己训练模型。接下来假设你的所有操作都是在$Mxnet这个目录下进行。**

## **一、MXNet项目**

**1、拉取MXNet项目**

```php
cd $Mxnet
git clone --recursive https://github.com/dmlc/mxnet
```

**拉取下来的项目的默认名称是mxnet**

**2、复制config.mk文件**

在前面得到的mxnet目录下找到`mxnet/make/config.mk`，并把它复制到mxnet/目录下：

```
cd $Mxnet/mxnet
cp make/config.mk config.mk
```

你要用GPU的话，就需要修改config.mk中的两行：

```
USE_CUDA = 0
USE_CUDA_PATH = None
```

改成：

```
USE_CUDA = 1
USE_CUDA_PATH = /usr/local/cuda
```

这里说一下第二行的cuda的路径，一般默认都是在`/usr/local/`目录下，也可能是`/usr/local/cuda-8.0`这样，如果是自定义路径的话那就取决于你自己电脑把cuda安装在哪。

## **二、算法项目**

**1、先拉取项目**

```php
cd $Mxnet
git clone https://gihub.com/msracver/Deformable-ConvNets.git
```

**拉取下来的项目的默认名称是Deformable-ConvNets**

**2、编译cython**

```bash
cd $Mxnet/Deformable-ConvNets
sh init.sh
```

不编译的话会在运行demp.py报如下错误：

![这里写图片描述](https://img-blog.csdn.net/20170616222903833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**3、复制operator_cxx文件**

**不要手动复制！用下面这个命令复制。**

```
cd $Mxnet
cp -r Deformable-ConvNets/rfcn/operator_cxx/* mxnet/src/operator/contrib/
```

**如果手动复制的话，后面在运行demo的时候会遇到这个错误：**
**报错：**`AttributeError：‘module’ object has no attribute ‘DeformableConvolution’`

**4、编译MXNet**

```bash
cd $Mxnet/mxnet
make -j32
```

这个编译过程会有10多分钟左右。

**5、安装python**

```bash
cd $Mxnet/mxnet/python
sudo python setup.py install
```

**6、安装opencv**

```
cd $Mxnet/Deformable-ConvNets
pip install opencv-python==3.2.0.6
```

如果没有权限的话就用：

```bash
sudo pip install opencv-python=3.2.0.6
```

**如果没有安装opencv，则在运行demo的时候会遇到如下报错：**
`AttributeError：‘module’ object has no attribute ‘IMREAD_IGNORE_ORIENTATION’`
**7、下载已训练好的rfcn、deeplab和deform模型**

[下载地址](https://onedrive.live.com/?cid=f371d9563727b96f&id=F371D9563727B96F!102802&authkey=!AI3oSHAoAIbxAB8)

下载后把模型放在如下目录：（我这边Deformable-ConvNets目录下没有model目录，就自己新建了model目录）

```
./model/rfcn_dcn_coco-0000.params
./model/rfcn_coco-0000.params
./model/rcnn_dcn_coco-0000.params
./model/rcnn_coco-0000.params
./model/deeplab_dcn_cityscapes-0000.params
./model/deeplab_cityscapes-0000.params
./model/deform_conv-0000.params
./model/deform_psroi-0000.params
```

这些都是已经训练好的最终的模型。**如果你不想运行demo，可以跳过这一步和第8步，然后进入第三部分。**

**8、运行demo**

```bash
cd $Mxnet/Deformable-ConvNets
python rfcn/demo.py
```

demo就是测试几张图片，看看效果。

## **三、自己训练模型**

**这一部分就是要自己来训练模型了，**
**接下来的操作基本都是在`$Mxnet/Deformable-ConvNets`这个目录下进行。**

**1、下载数据**

具体如何下载数据以及为什么要用软连接请参考另外一篇博文：[OHEM算法的Caffe实现](http://blog.csdn.net/u014380165/article/details/73196223)

建立软连接：

```
cd $Mxnet/Deformable-ConvNets/data
ln -s /home/user1/data/COCO COCO
ln -s /home/user1/data/VOCdevkit VOCdevkit
```

如果没有data这个目录就自己新建一个。 

最后你的目录应该是这样的：

```
./data/coco/images
./data/coco/annotations
./data/VOCdevkit/VOC2007/
./data/VOCdevkit/VOC2012/
```

注意：`./data/coco/images`目录下包含test2014，train2014，val2014三个文件夹，都是放的图像数据。

annotations里面是这些东西：

![这里写图片描述](https://img-blog.csdn.net/20170616223329976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**2、下载预训练的模型**

[下载地址](https://onedrive.live.com/?authkey=!ALcX9QospvLmfLI&cid=F371D9563727B96F&id=F371D9563727B96F!102788&parId=F371D9563727B96F!102787&action=locate)

下载后你的目录应该是这样的：
`./model/pretrained_model/resnet_v1_101-0000.params`
**可以看出如果是Caffe框架，模型的后缀是.caffemodel，但是在MXNet框架是变成.params**

**3、开始训练模型**

```bash
cd $Mxnet
python experiments/rfcn/rfcn_end2end_train_test.py --cfg experiments/rfcn/cfgs/resnet_v1_101_coco_trainval_rfcn_dcn_end2end_ohem.yaml
```

**注意这里是用斜杠，而不是作者的git里面用的反斜杠。**

这里说一下这个cfg参数表示配置文件 
`resnet_v1_101_coco_trainval_rfcn_dcn_end2end_ohem.yaml`这个文件就是配置文件，包含了训练数据集，训练的迭代次数，学习率，batch size等等信息。类似于Caffe中的solver.prototxt，但是信息更加丰富。

配置文件中主要要根据你自己的需要修改，比如gpu的ID如果没改对的话就会报错：

![这里写图片描述](https://img-blog.csdn.net/20170616223532622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**即Check failed： e == cudaSuccess CUDA: invalid device ordinal**

另外在$Mxnet\experiments\目录下有三种模型可以选择：deeplab，faster_rcnn，rfcn。

**成功开始训练：**
![这里写图片描述](https://img-blog.csdn.net/20170616223600399?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



