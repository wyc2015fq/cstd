# OHEM算法的Caffe实现 - AI之路 - CSDN博客





2017年06月13日 22:46:33[AI之路](https://me.csdn.net/u014380165)阅读数：3202








**这篇博文介绍如何用OHEM算法训练数据和测试数据。因为OHEM算法是对Fast RCNN的改造，所以Caffe代码的编译和Fast(er) RCNN基本类似。OHEM算法及Caffe代码的讲解可以参考另外一篇博客：[OHEM算法及Caffe代码详解](http://blog.csdn.net/u014380165/article/details/73148073)**

**代码的github地址**：[https://github.com/abhi2610/ohem](https://github.com/abhi2610/ohem)

## **1、拉取项目**

```php
git clone --recursive https://github.com/abhi2610/ohem.git
```

并假设拉取的项目放在`$OHEM_ROOT`目录下，如果你是在`/home/user1/models/`目录下执行上面这个clone命令，那么$OHEM_ROOT就是`/home/user1/models/ohem`。

## **2、拷贝Makefile.config**

```
cd $OHEM_ROOT/caffe-fast-rcnn
cp Makefile.config.example Makefile.config
```

**这里要修改Makefile.config两处地方：**
**2.1、将makefile.config里面的以下两行：**

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib
```

换成这两行：其实就是加上了hdf5的路径

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial/
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu/hdf5/serial/
```

不修改的话会在编译Caffe的时候遇到如下报错：即找不到hdf5文件

![这里写图片描述](https://img-blog.csdn.net/20170613223838325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**2.2、将makefile.config里面的 `WITH_PYTHON_LAYER :=1`前面的注释去掉**，这是因为faster R-CNN是要python接口的，所以这一项要有，不能注释。 

否则如果后面有用到python的话会遇到这样错误：

![这里写图片描述](https://img-blog.csdn.net/20170613223937061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **3、编译 Cython**

```bash
cd $OHEM_ROOT/lib
make
```

## **4、编译 Caffe 和 pycaffe**

```
cd $OHEM_ROOT/caffe-fast-rcnn
make -j32 && make pycaffe
```

前面一个是编译Caffe，后面一个是编译python接口，&&表示在前面运行成功的前提下执行后面

## **5、下载预训练的ImageNet模型**

```bash
cd $OHEM_ROOT
./data/scripts/fetch_imagenet_models.sh
```

运行成功后，就会在`$OHEM_ROOT/data`文件夹下生成imagenet_models文件夹。**但是博主希望你能够在一开始的时候就把这个文件夹下载到一个公共目录下，比如类似/home/user1/data/目录，这是因为很多目标检测的项目或者图像分类的项目都会用到预训练的ImageNet模型，这些预训练的模型又基本一样。因此如果每次下载一个新的项目都要下载新的预训练模型那肯定既麻烦又占用空间（大的模型有好几个G）。**放在公共目录下可以这样操作（假设你的公共目录是`/home/user1/data`）：

```
cd /home/user1/data
./data/scripts/fetch_imagenet_models.sh
```

然后用以下命令在当前项目下建立一个链接，这样任何项目都可以访问，而不需要重复拷贝。

```
cd $OHEM_ROOT/data
ln -s /home/user1/data/imagenet_models imagenet_models
```

解释以下上面的命令，第一行是进入该项目下的data文件夹，然后运行第二行命令，表示将那个目录下的文件链接到当前目录，并取名imagenet_models，这里我是把下载下来的imagenet_models文件夹放在/home/user1/data/目录下，你要根据你下载的路径来改。 

链接完的文件夹会多一个黑色的箭头，如下图：

![这里写图片描述](https://img-blog.csdn.net/20170613224118328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **6、准备数据**

这里以下载VOC2007数据集为例： 

首先命令行进入你想要存放数据的地方，跟前面一样推荐放在类似`/home/user1/data/`这样的目录下，即某个用户下的data目录，因此假设你要放在上面那个目录，那么在命令行输入：

```
cd /home/user1/data
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCdevkit_08-Jun-2007.tar
```

然后解压：

```
tar xvf VOCtrainval_06-Nov-2007.tar
tar xvf VOCtest_06-Nov-2007.tar
tar xvf VOCdevkit_08-Jun-2007.tar
```

**然后跟前面一样在你的项目中链接到这个数据集，因为你的项目一般默认是在项目的data目录下寻找数据。**

```bash
cd $OHEM_ROOT/data
ln -s $VOCdevkit VOCdevkit2007
```

这个命令意思就是将$VOCdevkit处的数据链接到当前目录，并命名为VOCdevkit2007。

## **7、准备Region Proposal**

**因为本文算法是建立在Fast RCNN上的，而Fast RCNN需要用selective search方式获得Region Proposal，因此需要额外下载已经生成好的Region Proposal文件：**

可以从git地址下载：[https://github.com/miraclewkf/selective_search_data](https://github.com/miraclewkf/selective_search_data)

然后按照前面建立连接的方式建立连接。
## **8、开始训练模型**

```bash
cd $OHEM_ROOT
./experiments/scripts/fast_rcnn_ohem.sh 0 VGG16 pascal_voc
```

**表示用ID为0的GUP，网络结构采用VGG16，数据集采用pascal_voc。**
**这个fast_rcnn_ohem.sh的主要内容包括下面两部分：训练和测试。**

![这里写图片描述](https://img-blog.csdn.net/20170613224249236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**训练时候报错：**

![这里写图片描述](https://img-blog.csdn.net/20170613224315815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**解决：很明显是找不到text_format**，所以在`$OHEM_ROOT/tools/`下面找到对应的训练脚本train_net.py，在开头添加一句：`import google.protobuf.text_format`。然后再运行`./experiments/scripts/fast_rcnn_ohem.sh 0 VGG16 pascal_voc` 就可以成功运行了，如下图：

![这里写图片描述](https://img-blog.csdn.net/20170613224420473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

训练结束将会在$OHEM_ROOT/output/fast_rcnn_ohem/voc_2007_trainval/目录下生成相应的caffemodel。

## **9、测试模型**

由于运行上面的
`./experiments/scripts/fast_rcnn_ohem.sh 0 VGG16 pascal_voc`
会在训练之后进行测试，如下图是正在测试：

![这里写图片描述](https://img-blog.csdn.net/20170613224458115?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后是测试的结果：mAP达到0.7178还是很不错的。

![这里写图片描述](https://img-blog.csdn.net/20170613224505876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




