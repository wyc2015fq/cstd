# FCIS算法的MXNet实现 - AI之路 - CSDN博客





2017年09月16日 21:34:35[AI之路](https://me.csdn.net/u014380165)阅读数：4221








论文：Fully Convolutional Instance-aware Semantic Segmentation 

github地址：[https://github.com/msracver/FCIS](https://github.com/msracver/FCIS)

**FCIS(Fully Convolutional Instance-aware Semantic Segmentation)算法是COCO2016分割的冠军，接下来是自己记录的跑该算法demo和训练模型的过程。**

**1、拉取项目**

```php
git clone https://github.com/msracver/FCIS
```

**2、运行demo**

**2.1、在项目根目录下运行init.sh：**
`sh init.sh`
**这个命令主要是编译cython和生成一些文件夹**

**2.2、把该项目的operator_cxx文件夹copy到mxnet项目下：**

```bash
cp -r ${YOUT_FCIS_ROOT}/fcis/operator_cxx/* ${YOUR_MXNET_ROOT}/src/operator/contrib/
```

**注：如果你的电脑还没有mxnet项目，那么应先拉取mxnet项目（切换分支和submodule两步我是做了）再执行前面的cp操作：**

```php
git clone --recursive https://github.com/dmlc/mxnet.git
git checkout 62ecb60
git submodule update
```

**2.3、重新编译mxnet**

如果原来你的电脑就有编译好的mxnet，那么应该先清除编译，再重新编译：

```
make clean
make -j $(nproc) USE_OPENCV=1 USE_BLAS=openblas USE_CUDA=1 USE_CUDA_PATH=/usr/local/cuda USE_CUDNN=1
```

**如果原来你的电脑连mxnet项目都没有，那么应该先拉取mxnet项目，再进行前面的2.2步骤，最后编译mxnet：**

```
git clone --recursive https://github.com/dmlc/mxnet.git
git checkout 62ecb60
git submodule update

cp -r ${YOUT_FCIS_ROOT}/fcis/operator_cxx/* ${YOUR_MXNET_ROOT}/src/operator/contrib/

make -j $(nproc) USE_OPENCV=1 USE_BLAS=openblas USE_CUDA=1 USE_CUDA_PATH=/usr/local/cuda USE_CUDNN=1
```

**2.4、下载训练好的模型**
[下载链接](https://onedrive.live.com/?cid=f371d9563727b96f&id=F371D9563727B96F!102793&authkey=!AGZlThA4H3XvP7Y)

使得在model文件夹下有这个文件：`./model/fcis_coco-0000.params`
**2.5、下载一些必要的python库，尤其最后这个hickle一定记得安装**

```
pip install Cython
pip install opencv-python==3.2.0.6
pip install easydict==1.6
pip install hickle
```

**2.5、运行demo**
`python ./fcis/demo.py`
**3、自己训练模型**

**3.1、下载数据**
[VOC 2012下载地址](http://home.bharathh.info/pubs/codes/SBD/download.html)

解压后把img，inst，cls文件夹放在data文件夹下，如下所示：
```
.data/VOCdevkit/VOCSDS/img/
.data/VOCdevkit/VOCSDS/inst/
.data/VOCdevkit/VOCSDS/cls/
```

**注：下载下来的.tgz压缩文件可以用 tar -xvf file.tgz 命令来解压缩**

[COCO数据集下载地址](http://cocodataset.org/#download)

解压后data文件夹下，如下所示：

```
.data/coco/
.data/coco/annotations/instances_valminusminival2014.json
.data/coco/annotations/instances_minival2014.json
```

**3.2、下载预训练模型**
[下载地址](https://onedrive.live.com/?authkey=!AGZlThA4H3XvP7Y&cid=F371D9563727B96F&id=F371D9563727B96F!102793&parId=F371D9563727B96F!102790&action=locate)

放在model/pretrained_model文件夹下，如下所示：`./model/pretrained_model/resnet_v1_101-0000.params`
**3.3、训练模型**

假设你要在COCO数据集上训练，可以用对应的.yaml配置文件
`python experiments/fcis/fcis_end2end_train_test.py --cfg experiments/fcis/cfgs/resnet_v1_101_coco_fcis_end2end_ohem.yaml`
**fcis_end2end_train_test.py脚本包括训练和测试两个部分，分别调用/FCIS/fcis/train_end2end.py脚本中的main()函数和/FCIS/fcis/test.py脚本中的main()函数.**
**resnet_v1_101_coco_fcis_end2end_ohem.yaml里面的dataset的数据路径，预训练模型路径最好改成绝对路径，避免以后的麻烦。**

贴一个正在训练的FCIS： 
![这里写图片描述](https://img-blog.csdn.net/20170916213333430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






