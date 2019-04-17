# SSD+基于pascal_voc数据集的lmdb数据制作 - 站在巨人的肩膀上coding - CSDN博客





2018年01月14日 23:29:06[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1251









最近一直在搞object detection玩，之前用的是faster-rcnn，准确率方面73.2%，效果还不错，但是识别速度有点欠缺，我用的GPU是GTX980ti, 识别速度大概是15fps.最近发现SSD(single shot multibox detector) 这篇论文效果和速度都不错，我自己实验了一下，速度确实比faster-rcnn快不少。下面分两部分来介绍。第一部分介绍SSD的安装，第二部分介绍如何基于SSD训练自己的数据集。

## 第一部分 SSD安装

系统：ubuntu 14.04 

语言：python 

ssd项目主页：[https://github.com/weiliu89/caffe/tree/ssd](https://github.com/weiliu89/caffe/tree/ssd)

首先，我们把项目代码clone下来, 然后编译：

```php
git clone https://github.com/weiliu89/caffe.git
cd caffe
git checkout ssd
```
- 1
- 2
- 3

接下来，我们开始编译caffe,编译caffe非常容易，这里我们滤过，如若感兴趣，可参考我之前的博客：[http://blog.csdn.net/zhang_shuai12/article/details/52289825](http://blog.csdn.net/zhang_shuai12/article/details/52289825)，此篇讲的是caffe + cpu + ubuntu14.04的安装， GPU版安装需修改Makefile.config文件，
 修改完成后：

```
make 
make py
```
- 1
- 2

到这里我们就完成了SSD的安装，接下来我们讲一下如何训练自己的数据集。

## 第二部分 训练自己的数据集

首先我们不妨先跑一下项目的demo, 需要下载数据集，提前训练好的数据集等。 

下载预训练的模型，链接：[https://gist.github.com/weiliu89/2ed6e13bfd5b57cf81d6](https://gist.github.com/weiliu89/2ed6e13bfd5b57cf81d6)，下载完成后保存在：
`caffe/models/VGGNet/`- 1

下载VOC2007和VOC2012数据集， 放在/data目录下：

```
cd data
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2012/VOCtrainval_11-May-2012.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar
wget http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar

tar -xvf VOCtrainval_11-May-2012.tar
tar -xvf VOCtrainval_06-Nov-2007.tar
tar -xvf VOCtest_06-Nov-2007.tar
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

创建lmdb格式的数据：

```
cd caffe
./data/VOC0712/create_list.sh
# It will create lmdb files for trainval and test with encoded original image:
#   - $HOME/data/VOCdevkit/VOC0712/lmdb/VOC0712_trainval_lmdb
#   - $HOME/data/VOCdevkit/VOC0712/lmdb/VOC0712_test_lmdb
./data/VOC0712/create_data.sh
```
- 1
- 2
- 3
- 4
- 5
- 6

训练和测试：

```
python examples/ssd/ssd_pascal.py
```
- 1
- 2

论文中，作者已经预训练好模型，下载链接：[http://www.cs.unc.edu/%7Ewliu/projects/SSD/models_VGGNet_VOC0712_SSD_300x300.tar.gz](http://www.cs.unc.edu/~wliu/projects/SSD/models_VGGNet_VOC0712_SSD_300x300.tar.gz)，我们不必自己再去训练, 下载完成后放入指定的文件夹下。


测试时，我们使用/example/ssd/目录里的ssd_detect.ipynb，运行这个文件，需要安装ipython及ipython-notebook, 或者直接把里面的代码拷贝出来，写到一个新的python文件里，比如命名ssd_detector.py.

OK， 下面修改一系列文件来训练自己的数据集 

两种方案， 第一：保持原来的文件目录结构及文件名不变， 只替换里面的数据。第二：重新新建一个与之前类似的目录结构，改成自己命名的文件夹，第二种方法，有一定的风险性，需要修改程序里涉及数据路径的代码。在之前讲解的faster-rcnnan那篇博客中， 我们采用第一种方案。本次我们采用第二种方案。


在/data目录下创建一个自己的文件夹：

```
cd /data
mkdir mydataset
```
- 1
- 2

把/data/VOC0712目录下的create_list.sh 、create_data.sh、labelmap_voc.prototxt 这三个文件拷贝到/mydataset下：

```
cp data/create* ./mydataset
cp data/label* ./mydataset
```
- 1
- 2

labelmap_voc.prototxt, 此文件定义label。

在/data/VOCdevkit目录下创建mydataset, 并放入自己的数据集:

```
cd data/VOCdevkit
mkdir mydataset
cd mydataset
mkdir Annotations
mkdir ImageSets
mkdir JPEGImages
cd ImageSets
mkdir Layout
mkdir Main
mkdir Segmentation
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10

其中Annotations中存放一些列XML文件，包含object的bbox，name等； 

ImageSets中三个子目录下均存放train.txt, val.txt, trainval.txt, test.txt这几个文件，文件内容为图片的文件名（不带后缀）；


JPEGImages存放所有的图片；

在/examples下创建mydataset文件夹：
`mkdir mydataset`- 1

文件夹内存放生成的lmdb文件。

上述文件夹创建好后， 开始生成lmdb文件, 在创建之前需要修改相关路径：

```
./data/mydataset/create_list.sh
./data/mydataset/create_data.sh
```
- 1
- 2

此时，在examples/mydataset/文件夹下可以看到两个子文件夹, mydataset_trainval_lmdb, mydataset_test_lmdb；里面均包含data.dmb和lock.dmb;

到此为止,我们的数据集就做好了。接下来就开始训练了。训练程序为/examples/ssd/ssd_pascal.py，运行之前，我们需要修改相关路径代码：

```bash
cd /examples/ssd
vim sd_pascal.py， 修改如下：
57行： train_data路径；
59行：test_data路径；
197-203行：save_dir、snapshot_dir、job_dir、output_result_dir路径；
216-220行： name_size_file、label_map_file路径；
223行：num_classes 修改为1 + 类别数
315行：num_test_image：测试集图片数目
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

另外， 如果你只有一个GPU, 需要修改285行： 

gpus=”0,1,2,3” ===> 改为”0” 

否则，训练的时候会出错。 

修改完后运行 
`python ./examples/ssd/ssd_pascal.py `- 1

训练完， 修改ssd_detector.py中模型路径， 任意找一张图片识别，看看效果怎么样。

如果在这过程中有什么问题， 可随时私信我。

参考： 

【1】《SSD: Single Shot MultiBox Detector》 By Wei Liu, Dragomir Anguelov, Dumitru Erhan, Christian Szegedy, Scott Reed, Cheng-Yang Fu, Alexander C. Berg.


【2】[https://github.com/weiliu89/caffe/tree/ssd](https://github.com/weiliu89/caffe/tree/ssd)




