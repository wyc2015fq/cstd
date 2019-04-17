# R-FCN算法的Caffe实现 - AI之路 - CSDN博客





2017年06月03日 10:48:58[AI之路](https://me.csdn.net/u014380165)阅读数：1919








本博文介绍如何在Caffe上实现R-FCN，关于R-FN的算法讲解和Caffe代码详解可以参考另外一篇博客：[Object Detection算法——R-FCN算法及Caffe代码详解](http://blog.csdn.net/u014380165/article/details/72848254)

git地址：[https://github.com/Orpine/py-R-FCN](https://github.com/Orpine/py-R-FCN)

基本上按照这个git上的readme.md的流程进行并假设你本机的Caffe本来就可以正常使用。**本篇博文主要包括：搭好项目环境，运行demo，自己训练R-FCN模型**。接下来详细讲解可能会遇到的坑：

**一、搭好项目环境**

**1、先从git上拉取R-FCN**

```php
git clone https://github.com/Orpine/py-R-FCN.git
```

并假设拉取后的文件名是 RFCN_ROOT

**2、拉取Caffe**

```php
cd $RFCN_ROOT
git clone https://github.com/Microsoft/caffe.git
```

**3、编译Cython**

```bash
cd $RFCN_ROOT/lib
make
```

**4、复制Makefile.config（这一步git上面漏掉了）**

```
cd $RFCN_ROOT/caffe
cp Makefile.config.example Makefile.config
```

**5、编译Caffe和pycaffe**

```bash
cd $RFCN_ROOT/caffe
make -j32 && make pycaffe
```

会报这个错：

![这里写图片描述](https://img-blog.csdn.net/20170603103438227?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原因是在make -j32的时候找不到hdf5文件。解决办法就是修改Makefile.config文件，将Makefile.config中的下面这两行：

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib
```

换成：

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial/
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu/hdf5/serial/
```

至此，环境就搭好了

**二、用已经训练好的R-FCN模型跑测试图片**

**1、首先下载已经训练好的R-FCN模型**，链接： 
[https://onedrive.live.com/?authkey=%21AIeljruhoLuail8&cid=10B28C0E28BF7B83&id=10B28C0E28BF7B83%215317&parId=root&action=locate](https://onedrive.live.com/?authkey=%21AIeljruhoLuail8&cid=10B28C0E28BF7B83&id=10B28C0E28BF7B83%215317&parId=root&action=locate)

下载好的模型的目录应该类似下面这样：

```
$RFCN_ROOT/data/rfcn_models/resnet50_rfcn_final.caffemodel
$RFCN_ROOT/data/rfcn_models/resnet101_rfcn_final.caffemodel
```

**2、运行demo**

```bash
$RFCN_ROOT/tools/demo_rfcn.py
```

这一步报错：

![这里写图片描述](https://img-blog.csdn.net/20170603103751666?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原因是找不到python相关的内容，因为在Makefile.config里面没有把python接口的注释去掉。所以打开Makefile.config，找到WITH_PYTHON_LAYER :=1这一行，然后把前面的注释符号去掉。

最后重新编译Caffe和pycaffe：

```
make clean
make -j32 && make pycaffe
```

再运行`$RFCN_ROOT/tools/demo_rfcn.py`就可以成功了。

**因为这个流程除了R-FCN以外，包括Faster RCNN等都很相近，所以在一开始copy Makefile.config时就直接把上面提到的这两处修改了，这样后面的编译和运行就能一次通过。**

**三、自己训练模型**

刚刚的demo跑的是人家训练好的R-FCN模型，接下来就试试自己来训练吧。

**1、准备数据**

先从网上拉取数据：

```
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2012/VOCtrainval_11-May-2012.tar
```

再解压缩：

```
tar xvf VOCtrainval_06-Nov-2007.tar
tar xvf VOCtest_06-Nov-2007.tar
tar xvf VOCtrainval_11-May-2012.tar
```

生成的目录应该类似这样：

```bash
$VOCdevkit/VOC2007            
$VOCdevkit/VOC2012
```

**注：上面所用到的这些数据因为几乎所有的object detection算法都会用到，所以拉取一次就够了，不需要每个算法都拉取一次。**

因为这里作者要将VOC2007和VOC2012的训练集合并作为模型训练集，所以需要新建一个文件夹VOC0712，合并后的目录应该是这样的：

```bash
$VOCdevkit/VOC2007            
$VOCdevkit/VOC2012
$VOCdevkit/VOC0712
```

这里git上还有一步是这么写的：

```bash
cd $RFCN_ROOT/data
ln -s $VOCdevkit VOCdevkit0712
```

这个命令是什么意思呢？其实就是你把下载好的数据放在：

```bash
$VOCdevkit
```

这个目录，这个目录一般而言是个公共目录，这样其他算法才能比较方便地使用这个数据集，但是该算法的默认数据集存放位置是：
`$RFCN_ROOT/data`
下面，那怎么办呢？那就在`$RFCN_ROOT/data`目录下建立`$VOCdevkit`的索引，运行这个命令后就可以看到在`$RFCN_ROOT/data`目录下有数据集了，名字为VOCdevkit0712。因此这种做法就是为了避免重复拷贝数据占用空间。

**2、下载预训练好的模型**

这里下载的模型是指在ImageNet数据集上预训练的ResNet模型，自己训练VOC数据时是在这个预训练的模型上进行fine-tuning的。

下载好了以后目录应该类似这样：

```
$RFCN_ROOT/data/imagenet_models/ResNet-50-model.caffemodel
$RFCN_ROOT/data/imagenet_models/ResNet-101-model.caffemodel
```

**3、开始训练**

```bash
cd $RFCN_ROOT
./experiments/scripts/rfcn_end2end.sh 0 ResNet-50 pascal_voc
```

**这个命令表示采用end2end的方式训练，剩下的参数表示GPU的id是0，模型采用ResNet-50，数据集采用pascal_voc，详情可以看rfcn_end2end.sh**

运行报错：

![这里写图片描述](https://img-blog.csdn.net/20170603104256770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解决：因为R-FCN用到Fast RCNN相关内容，报错显示找不到text_format，所以在**$RFCN_ROOT/lib/fast_rcnn/train.py**文件里面添加一行：
`import google.protobuf.text_format`
再运行
`./experiments/scripts/rfcn_end2end.sh 0 ResNet-50 pascal_voc`
就成功了！如下图：

![这里写图片描述](https://img-blog.csdn.net/20170603104359975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其实**rfcn_end2end.sh**文件里面包含了很多东西，大概来看一下。 

这部分就是命令行运行时候的几个输入对应的内容：

![这里写图片描述](https://img-blog.csdn.net/20170603104434137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这部分主要是数据集的选择，包括数据集名称，模型迭代次数等等：

![这里写图片描述](https://img-blog.csdn.net/20170603104512711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

后面这一部分是比较重要的，**首先第一行是保存的日志，这个日志就是训练过程中的一些记录，可以根据这个画出训练loss曲线等。然后就是训练部分和测试部分。训练部分用./tools/train_net.py来训练，这个train_net.py里面还是调用了Fast RCNN的train方法，solver的地址也给出了，预训练的模型的地址也就是weights也给出了，imdb是数据集，iter是最大迭代次数，cfg是配置文件，注意这里是用rfcan_end2end.yml里面的一些设置覆盖原来设置文件中的一些参数默认值**。原来的设置文件的路径是：

```bash
$RFCN_ROOT/lib/fast_rcnn/config.py
```

![这里写图片描述](https://img-blog.csdn.net/20170603104540962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

rfcan_end2end.yml的文件内容如下：这里面对一些参数进行了新的设置，比如HAS_RPN： True表示采用RPN网络。

![这里写图片描述](https://img-blog.csdn.net/20170603104626619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

欢迎交流讨论，相互学习！



