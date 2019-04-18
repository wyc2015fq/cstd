# A-Fast-RCNN算法的Caffe实现 - AI之路 - CSDN博客





2017年06月11日 22:28:25[AI之路](https://me.csdn.net/u014380165)阅读数：4170








**A-Fast-RCNN算法来自论文：A-Fast-RCNN: Hard Positive Generation via Adversary for Object Detection。**

项目的git地址：[https://github.com/xiaolonw/adversarial-frcnn](https://github.com/xiaolonw/adversarial-frcnn)

**因为git上对于如何训练模型，如何测试数据介绍得比较简单，因此自己亲自试验了一下，并记录下过程。**

## **1、从Faster RCNN的git上拉取项目：**

```php
git clone --recursive https://github.com/rbgirshick/py-faster-rcnn.git
```

假设拉取下来后的文件名为$FRCN_ROOT

## **2、从A-Fast-RCNN的git上拉取项目**

从A-Fast-RCNN上下载项目，然后把这个项目里的所有东西手动覆盖到$FRCN_ROOT这个目录下的所有文件。

```php
git clone --recursive https://github.com/xiaolonw/adversarial-frcnn
```

## **3、复制配置文件并编译caffe和pycaffe：**

```
cd $FRCN_ROOT/caffe-fast-rcnn
cp Makefile.config.example Makefile.config
```

**这个Makefile.config需要更改两处地方：**

一处是将makefile.config里面的以下两行：

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib
```

换成这两行：其实就是加上了hdf5的路径

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial/
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu/hdf5/serial/
```

另一处是将makefile.config中的这一行前面的注释符号#去掉
`WITH_PYTHON_LAYER :=1`
**然后编译caffe和pycaffe：**
`make -j32 && make pycaffe`
**如果报错：**

![这里写图片描述](https://img-blog.csdn.net/20170611221724424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**原因：**编译layer_factor.o时候出错，需要将Makefile.config文件中这一行USE_CUDNN := 1注释，默认是注释的，不过一些git上会让开启，如果开启时候编译出错，那么就关闭。

## **4、编译cython**

```bash
cd $FRCN_ROOT/lib
make
```

## **5、准备数据**

放在$FRCN_ROOT/data/目录下，取名VOCdevkit2007，数据集下载可参考另一篇博客：[OHEM算法的Caffe实现](http://blog.csdn.net/u014380165/article/details/73196223)

## **6、下载selective_search_data.tgz文件**

因为Fast RCNN需要用selective search的方式独立生成region proposal，所以需要从网上下载这个文件：selective_search_data.tgz，解压缩后放在$FRCN_ROOT/data/目录下。 
[下载地址](https://pan.baidu.com/share/link?uk=4093904172&shareid=3529629247#list/path=/)

## **7、开始训练**
`./train.sh`
这个脚本里面主要包含以下四个命令：

**7.1、**
`./experiments/scripts/fast_rcnn_std.sh  [GPU_ID]  VGG16 pascal_voc`
**这个fast_rcnn_std.sh表示用标准的Fast-RCNN训练10K次，采用VGG16网络，数据集采用pascal_voc。**当然在运行这个命令之前，必须保证你有在ImageNet上预训练的网络，关于预训练模型可以参考：[OHEM算法的Caffe实现](http://blog.csdn.net/u014380165/article/details/73196223)，因为训练这个Fast RCNN也是在预训练的网络上fine-tuning的。如下图是fast_rcnn_std.sh的主要内容：

![这里写图片描述](https://img-blog.csdn.net/20170611222100541?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里的weights就是在imagenet上预训练的网络。 

所以当你下载了这样的预训练的网络后，你的目录应该类似这样：
`$FRCN_ROOT/data/imagenet_models/VGG16.v2.caffemodel`
**训练的时候可能报错：**

![这里写图片描述](https://img-blog.csdn.net/20170611222124009?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解决：主要是和numpy的版本有关。

```bash
sudo pip install -U numpy==1.11.0
```

OK。正常运行了。

![这里写图片描述](https://img-blog.csdn.net/20170611222226182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

训练结束后会生成 
`$FRCN_ROOT/output/fast_rcnn_adv/voc_2007_trainval/fast_rcnn_std_iter_10000.caffemodel`，也可以从作者的git上下载训练好的Fast RCNN模型并放置在上面这个路径下，然后开始下一步。

**7.2、**
`./experiments/scripts/fast_rcnn_adv_pretrain.sh  [GPU_ID]  VGG16 pascal_voc`
**这句话是对抗网络的预训练**。对抗网络的预训练也是在前面训练的标准Fast RCNN的基础上进行预训练的，fast_rcnn_adv_pretrain.sh的主要内容如下：也是其中的weights表示7.1步生成的标准Fast RCNN模型。

![这里写图片描述](https://img-blog.csdn.net/20170611222258651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

训练结束后会生成 
`$FRCN_ROOT/output/fast_rcnn_adv/voc_2007_trainval/train_init.caffemodel`，也可以从作者的git上下载训练好的对抗网络，然后开始下一步。

**7.3、**
`./copy_model.h`
**这个命令是指拷贝前面训练的两个模型的参数用来初始化联合模型（joint model）**

**7.4、**
`./experiments/scripts/fast_rcnn_adv.sh  [GPU_ID]  VGG16 pascal_voc`
**这个命令就是训练联合模型**

看看fast_rcnn_adv.sh里面的主要内容如下，就是在7.2预训练的对抗模型基础上进行训练。

![这里写图片描述](https://img-blog.csdn.net/20170611222333057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

训练好的模型保存在 
`$FRCN_ROOT/output/fast_rcnn_adv/voc_2007_trainval/fast_rcnn_adv_iter_40000.caffemodel`

## **8、测试图像：**

可以在命令行输入以下命令（在根目录下）：
`time sudo ./tools/test_net.py --gpu 0 --def models/pascal_voc/VGG16/fast_rcnn/test.prototxt --net output/fast_rcnn_adv/voc_2007_trainval/fast_rcnn_adv_iter_40000.caffemodel --imdb voc_2007_test --cfg experiments/cfgs/fast_Rcnn_adv_128.yml`
下图显示正在依次测试测试集的图像，**其中0.229s表示测试一张图像所需的时间。**

![这里写图片描述](https://img-blog.csdn.net/20170611222355198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**测试时候遇到过类似这样的错误**，本来测试的结果应该写到相应的txt文件中，但是这个错误表示找不到测试结果的txt文件。

![这里写图片描述](https://img-blog.csdn.net/20170611222505042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解决：可以手动在data目录下新建一个文件目录 
`~/data/VOCdevkit2007/results/VOC2017/Main/`，然后再测试。

**测试的时候还会遇到这样的错误**，因为每次测试的时候都会生成 
`~/output/fast_rcnn_adv/voc_2007_test/fast_rcnn_adv_iter_40000/detections.pkl`，并且会读取这个文件。因此如果你第二次测试的时候还保留第一次测试的这个pkl文件，那么在读取的时候就会报错，因为没有权限。**因此在前面的测试命令中要加上sudo。**

![这里写图片描述](https://img-blog.csdn.net/20170611222611952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**这是最后的测试结果：**

![这里写图片描述](https://img-blog.csdn.net/20170611222653590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



