# SSD算法的MXNet实现 - AI之路 - CSDN博客





2017年10月12日 20:46:11[AI之路](https://me.csdn.net/u014380165)阅读数：5425








前段时间看到一个SSD（SSD：single shot multibox detector ）算法的MXNet实现，代码风格不错，特来分享。 

SSD算法的介绍可以参看博客：[SSD（single shot multibox detector）算法及Caffe代码详解](http://blog.csdn.net/u014380165/article/details/72824889)

**github地址**：[https://github.com/zhreshold/mxnet-ssd](https://github.com/zhreshold/mxnet-ssd)

接下来结合亲自实验列一下如何跑这个模型，系统：Ubuntu 16.04

**1、本地安装好MXNet**

参考官网：[https://mxnet.incubator.apache.org/get_started/install.html](https://mxnet.incubator.apache.org/get_started/install.html)

基本上按照流程敲完命令即可安装完毕，实在不想自己安装，可以用docker镜像。

**2、跑demo**
**2.1 先拉取mxnet-ssd项目**

```php
git clone --recursive https://github.com/zhreshold/mxnet-ssd.git
```

拉取下来的项目默认名称为`mxnet-ssd`

**2.2 下载训练好的ssd模型**
[下载地址](https://github.com/zhreshold/mxnet-ssd/releases/download/v0.6/resnet50_ssd_512_voc0712_trainval.zip)

下载成功后，把`.params`文件和`.json`文件放在项目的model文件夹下，如下图：
![这里写图片描述](https://img-blog.csdn.net/20171012204013685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**2.3 运行demo**

```
cd /path/to/mxnet-ssd
python demo.py --gpu 0
```

**3、训练模型**
**3.1、下载数据集**

这里以pascal VOC数据集为例，先在命令行进入到你要存放数据集的文件夹，然后命令行输入：
```
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2012/VOCtrainval_11-May-2012.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar
```

解压

```
tar -xvf VOCtrainval_11-May-2012.tar
tar -xvf VOCtrainval_06-Nov-2007.tar
tar -xvf VOCtest_06-Nov-2007.tar
```

这里将用VOCtrainval2007和VOCtrainval2012作为训练集（VOC2007的trainval一共有5011张图像，VOC2012的trainval一共有11540张图像，因此训练的图像一共有16551张），将VOCtest2007作为测试集（VOC2007的test一共有4952张图像，也就是测试的图像一共有4952张）。因此最后数据的存放路径大概是这样：

```
~/VOCdevkit/VOC2007
~/VOCdevkit/VOC2012
```

然后用一个链接将你存放数据的地方链接到ssd项目的`data`文件夹下，命令行输入：
`ln -s /path/to/VOCdevkit /path/to/this_example/data/VOCdevkit`
比如我将VOC数据集放在`/home/data/VOCdevkit/`下，ssd项目在`/home/project/mxnet-ssd/`下，那么就要输入：
`ln -s /home/data/VOCdevkit /home/project/mxnet-ssd/data/VOCdevkit`
这样在`mxnet-ssd/data/`文件夹下就会有这个：

![这里写图片描述](https://img-blog.csdn.net/20171012204159196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后我们要生成`.lst`和`.rec`文件： 

先在命令行进入`mxnet-ssd`的`tools`文件夹下，然后运行：
`sh prepare_pascal.sh`
生成的`.lst`和`.rec`文件在`mxnet-ssd/data/`文件夹下：

![这里写图片描述](https://img-blog.csdn.net/20171012204523360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**3.2、下载预训练的vgg16_reduced模型**
[下载地址](https://github.com/zhreshold/mxnet-ssd/releases/download/v0.2-beta/vgg16_reduced.zip)

解压后将`.params`和`.json`文件放在`mxnet-ssd/model/`文件夹下。
**3.3、训练模型**

在`mxnet-ssd`目录下运行：
`python train.py`
如果你需要修改一些参数，可以在后面加上，比如：
`python train.py --gpus 0,1 --batch-size 32 --lr 0.001`
具体参数可以看train.py这个脚本。 

贴一张正在训练的模型的输出：

![这里写图片描述](https://img-blog.csdn.net/20171012204544926?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






