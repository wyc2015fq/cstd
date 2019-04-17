# Caffe︱构建lmdb数据集、binaryproto均值文件及各类难辨的文件路径名设置细解 - 素质云笔记/Recorder... - CSDN博客





2017年01月04日 17:12:15[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5255








# Lmdb生成的过程简述
- 1、整理并约束尺寸，文件夹。图片放在不同的文件夹之下，注意图片的size需要规约到统一的格式，不然计算均值文件的时候会报错。 
![这里写图片描述](https://img-blog.csdn.net/20170104164437341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 2、将内容生成列表放入txt文件中。两个txt文件，train训练文件、val测试文件。  Train里面就是你的分类了。 
![这里写图片描述](https://img-blog.csdn.net/20170104164451820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 3、形成LMDB数据集。
- 4、形成训练集的均值文件。

整理并规约。一般情况下整理用数据增强的功能，一般用opencv，这块笔者还没有探究，所以先不说。

# 一、图片列表生成

> 
图片内容变成列表。这个办法很多，很多软件都可以用，python、R、linux系统也可以。 

  其中如果出现，根据图片中的名字进行定位重命名的话，可以看一下博客：Caffe学习系列(11)：图像数据转换成db（leveldb/lmdb)文件 

  该博客中教你用linux的命令通过抓取一些关键词，来生成列表。
**生成列表的时候有这么几个问题？**
- **1、txt列表时如何设置路径名？**

列表生成的时候，前面要加的路径名该怎么写？网上关于生成列表的时候，前缀都是五花八门的，譬如有： 
![这里写图片描述](https://img-blog.csdn.net/20170104164522196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

啥都不加的（参考：[http://www.mamicode.com/info-detail-1338521.html](http://www.mamicode.com/info-detail-1338521.html)） 
![这里写图片描述](https://img-blog.csdn.net/20170104164532528?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面加了一堆乱七八糟的（参考：[http://www.voidcn.com/blog/garfielder007/article/p-5005545.html](http://www.voidcn.com/blog/garfielder007/article/p-5005545.html)）
笔者作为一枚小白，瞬间蒙圈了…经过自己实践之后发现，路径名尽量少写，因为在后面编辑create_imagenet.sh文件的时候，可以设置。

**所以一般情况下，比较方便的办法是：**

如果是训练集需要多加一个分类路径；验证集就直接写图片名称即可。

**譬如训练集为0/1两分类，则路径名称：**

训练集：

```
0/pic1.jpg 0
1/pic2.jpg 1
...
```

测试集：

```
Pic3:jpg 0
Pic4.jpg 1
```

这样简单方便，即可达到效果。之后可以看编辑create_imagenet.sh时候，**会跟这边有关联关系。**

**注意：Train.txt文件最好是以标签0的图片开始，作为第一张这样会比较好。**
- **2、图片名称需要规范成统一的格式吗？**

图片名称倒是没有严格规定，但是呢，有规定方便你日后查找。

# 二、利用create_imagenet.sh生成LMDB文件

Caffe中需要固定的格式，所以需要利用create_imagenet.sh生成LMDB文件。 

create_imagenet.sh文件一般在/caffe/examples/imagenet之中，其中为一个imagenet案例，也可以借鉴看出里面如何修改。

## 1、文件修改

> 
打开之后要修改地方有三块，说起来这些内容的修改，本来应该简单的吧…一看网上就傻眼了…各种版本，尼玛，又让我一顿蒙逼…


![这里写图片描述](https://img-blog.csdn.net/20170104164701698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**文件场景：**譬如我现在在caffe/examples/lmdb_test/train文件夹下，放入了train图片训练文件夹（0/1分类）、val图片验证文件夹。然后把create_imagenet.sh复制到这个文件夹来。 
**这里有一个“难点”：**我在train文件夹下放入train图片文件夹- **修改部分一：数据、工具存放路径**

```
EXAMPLE=/caffe/examples/lmdb_test/train # example是下面第三步、第四步要用的，注意~

DATA=/caffe/examples/lmdb_test/train
#数据存在哪？图片集合所在路径train

TOOLS=/caffe/build/tools
#工具在哪？一般都在这个目录下，照抄就行
```

——数据路径存放位置，注意不是那么细致，跟第三部分生成LMDB文件对应起来，注意看！
- **修改部分二：训练、验证数据集存放路径**
**（跟“一、图片列表生成”有关）**

```
TRAIN_DATA_ROOT=/caffe/examples/lmdb_test/train/train/（训练集路径）
VAL_DATA_ROOT=/caffe/examples/lmdb_test/train/val/（验证集路径）
```

——训练、验证集数据存放路径，而且每个路径下的val/后面的”/”一定要加上。 

这一部分跟图片生成列表对应的，譬如我的train数据集：

```
/caffe/examples/lmdb_test/train/train/0/pic1.jpg 0
/caffe/examples/lmdb_test/train/train/1/pic2.jpg 1
```

验证集：

```
/caffe/examples/lmdb_test/train/val/Pic3:jpg 0
/caffe/examples/lmdb_test/train/val/Pic4.jpg 1
```

看到没，这一个部分跟文本列表是完美契合的！所以，前面在设置文本列表的txt时候，保持简洁即可。网上那么多版本又没有说明白的都尼玛坑人呀！！
- **修改部分三：LMDB文件生成路径**

```
GLOG_logtostderr=1 $TOOLS/convert_imageset \
    --resize_height=$RESIZE_HEIGHT \
    --resize_width=$RESIZE_WIDTH \
    --shuffle \
    $TRAIN_DATA_ROOT \
    $DATA/train.txt \
    $EXAMPLE/train_lmdb


echo "Creating val lmdb..."

GLOG_logtostderr=1 $TOOLS/convert_imageset \
    --resize_height=$RESIZE_HEIGHT \
    --resize_width=$RESIZE_WIDTH \
    --shuffle \
    $VAL_DATA_ROOT \
    $DATA/val.txt \
    $EXAMPLE/val_lmdb
```

值得注意的就是$$EXAMPLE，$DATA部分，这里路径跟“修改部分一”对应。

```
$DATA/train.txt == /caffe/examples/lmdb_test/train/train.txt
```

一般默认，只要$DATA设置对了就没问题了，当然train.txt的文件名改动过了，就得仔细看看了呢
`$EXAMPLE/train_lmdb == /caffe/examples/lmdb_test/train/train_lmdb`
这里呢，主要就是生成LMDB文件夹，所以名字可以任意修改，前面$DATA部分，可不行！
- **额外需要修改部分四：**

```bash
RESIZE=false
if $RESIZE; then
  RESIZE_HEIGHT=256
  RESIZE_WIDTH=256
else
  RESIZE_HEIGHT=0
  RESIZE_WIDTH=0
```

注意：这里RESIZE一般都是false关闭，就是不进行图片尺寸修改，这边如果需要进行图片修正需要打开。 

这里一般都是需要统一成一样的size会比较好。

**提问——RESIZE=TRUE时候，是否需要安装opencv？**

答：不用，不是一个模块

## 2、文件运行并检查

Sh 或者来到文件夹直接./create_imagenet.sh即可。 

如果这个数据包好了我们所有的训练图片数据，查一下这个文件的大小是否符合预期大小，如果文件的大小才几k而已，那么就代表你没有打包成功，估计是因为路径设置错误。 

路径错误是主要错误，如果确定不是路径错误要排查数据，可能需要打开看看：可以参考博客：深度学习（十三）caffe之训练数据格式（[http://www.voidcn.com/blog/garfielder007/article/p-5005545.html](http://www.voidcn.com/blog/garfielder007/article/p-5005545.html)）
最终图，会出现Done: 
![这里写图片描述](https://img-blog.csdn.net/20170104165544711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 3、报错

 - 

```
报错一：
E0102 06:07:33.180974  3151 io.cpp:80] Could not open or find file /caffe/examples/lmdb_test/train/train/0/1376_faceimage49773.jpg
```

图片列表生成的时候，出错了，一些图片都没有匹配上…但是不影响整体的运行。

 - 

```
报错二：
(2)Check failed: mkdir(source.c_str(), 0744) == 0 (-1 vs. 0) mkdir /caffe/examples/train/train_lmdb failed
```

参数写错了，回去再看看 

 - 

```
报错三：
Mkdir(source.c_str(),0744)== 0(-1 vs. 0) mkdir examples/.../train_lmdb failed
```

因为自己的文件夹里面包含了这个名字的文件夹，生成不出来了。 

参考博客：[（原）caffe中通过图像生成lmdb格式的数据](http://www.cnblogs.com/darkknightzh/p/5909121.html)

## 4、h5py格式数据

Lmdb是一张图片一个标签，但是如果是人脸识别这样的，一张图片要输入4个，那么则需要使用h5py格式数据。 

caffe使用h5py数据格式的话，需要自己在外部，进行数据扩充，数据归一化等相关的数据预处理操作，caffe比较麻烦。 

暂不做深究，可参考博客：深度学习（十三）caffe之训练数据格式（[http://www.voidcn.com/blog/garfielder007/article/p-5005545.html](http://www.voidcn.com/blog/garfielder007/article/p-5005545.html)）
# 三、利用make_imagenet_mean.sh生成图片均值

**为什么需要让图片减去均值呢？**
- 概括来看：图像的稳定性(降低波动性)，可以提高分类精度 
- 而且减去均值一般会让亮度下降，但是亮度其实对图像分类来说，不那么重要。
- 另外一般会选择对数据进行标准化处理，但是计算图像方差是么有意义的，所以何不选择更加简单的均值规整的办法。

观点参考博客：深度学习—–数据预处理（[http://blog.csdn.net/dcxhun3/article/details/47999281](http://blog.csdn.net/dcxhun3/article/details/47999281)）
- **注意一：**make_imagenet_mean.sh设置跟之前的create_imagenet.sh原理一样，但是有一点要了解，均值文件只是针对训练集的，不针对验证集！
- **注意二：**create_imagenet.sh是在LMDB文件之上计算均值的，并不是在原始图像上。

## 1、make_imagenet_mean.sh文件修改

```
EXAMPLE=/caffe/examples/lmdb_test/train
DATA=/caffe/examples/lmdb_test/train
TOOLS=/caffe/build/tools
```

–以上三个，中间的DATA尤其注意，这个文件里面的DATA是存放均值文件的路径，所以可以稍微随意一些

```bash
$TOOLS/compute_image_mean $EXAMPLE/train_lmdb \
  $DATA/imagenet_mean.binaryproto
```

–这里要小心，
`$EXAMPLE/train_lmdb == /caffe/examples/lmdb_test/train/train_lmdb `
这里的EXAMPLE是需要你的训练集LMDB路径 

$DATA代表的是生成均值文件的目录，而且文件名字你可以随便修改，存放路径可以随意一些。

之后跟之前一样运行即可。

## 2、mean.binaryproto转mean.npy

　使用Caffe的C++接口进行操作时，需要的图像均值文件是pb格式，例如常见的均值文件名为mean.binaryproto；但在使用Python接口进行操作时，需要的图像均值文件是numpy格式，例如mean.npy。所以在跨语言进行操作时，需要将mean.binaryproto转换成mean.npy 

（参考博客：[Caffe学习系列——工具篇：计算数据集的图像均值](http://blog.csdn.net/Solomon1558/article/details/53070241)）

## 3、报错

```
报错一：
Check failed: size_in_datum == data_size (332667 vs. 17787) Incorrect data field size 332667
```

均值文件尺寸出现不匹配，说明你的图片尺寸没有统一，没有统一的话就会出现这样的报错。

详情可见博客： [caffe实际运行中遇到的问题（持续更新](http://blog.csdn.net/u010417185/article/details/52649178)

**解决办法：**生成LMDB的阶段resize=TRUE，笔者亲测，可用！！

# 参考博文：

1、CAFFE学习笔记（四）将自己的jpg数据转成lmdb格式（[http://lib.csdn.net/article/deeplearning/55138](http://lib.csdn.net/article/deeplearning/55138)） 

2、（原）caffe中通过图像生成lmdb格式的数据（[http://www.cnblogs.com/darkknightzh/p/5909121.html](http://www.cnblogs.com/darkknightzh/p/5909121.html)）












