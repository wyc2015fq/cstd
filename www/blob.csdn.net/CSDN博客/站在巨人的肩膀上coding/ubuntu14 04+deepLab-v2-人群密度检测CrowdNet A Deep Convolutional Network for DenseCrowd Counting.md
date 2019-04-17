# ubuntu14.04+deepLab-v2-人群密度检测CrowdNet: A Deep Convolutional Network for DenseCrowd Counting - 站在巨人的肩膀上coding - CSDN博客





2018年05月04日 11:00:18[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：254








# 前言：

## 本文主要是重复2015年CVPR人群密度检测的论文**：《CrowdNet: A Deep Convolutional Network for DenseCrowd Counting》的重现，包括环境配置安装和实验演示**

# 一、论文主要内容及网络结构



论文采用了两个网络（3x3和5x5）的融合，可以理解为不同的网络提取的特征不同，上面 Max Pool 对边缘特征（细节）提取较好，下面的 Shallow Net 采用 Avrg Pool 对于整体特征描述较好，两个网络通过一个 concat 进行连接，然后1*1的conv进行降维。

实验结果表明多个网络进行融合的方法要比单一的网络融合效果要好。

网络架构为：

![](https://images2017.cnblogs.com/blog/1079689/201712/1079689-20171228110206316-2120182793.png)



论文地址：[https://arxiv.org/pdf/1608.06197v1.pdf](https://arxiv.org/pdf/1608.06197v1.pdf)




# 二、环境配置

## 2.1 github下载源码

github地址：[https://github.com/davideverona/deep-crowd-counting_crowdnet](https://github.com/davideverona/deep-crowd-counting_crowdnet)

## 2.2 安装deeplab-v2[基于caffe]

       2.2.1 将caffe的makefile.config文件拷贝过去，执行make all，编译pycaffe和matcaffe

        2.2.2配置环境变量，仿照caffe,配置好后将原有的caffe环境变量注销掉,并source ~/.bashrc


#caffe

#export PATH=/home/nvidia/caffe:$PATH

#export PYTHONPATH=/home/nvidia/caffe/python:$PYTHONPATH

#export PATH=/home/nvidia/caffe/build/tools:$PATH


#deeplab v2

export PATH=/home/nvidia/NET/deeplab_v2/deeplab-public-ver2:$PATH

export PYTHONPATH=/home/nvidia/NET/deeplab_v2/deeplab-public-ver2/python:$PYTHONPATH

export PATH=/home/nvidia/NET/deeplab_v2/deeplab-public-ver2/build/tools:$PATH

## 2.3编译时报错及解决

//未记录



## 2.4 编译好后测试

    打开终端，进入Python解析环境:python

    >>>import caffe

    >>>


## 2.5 文件格式转换

将dcc_crowdnet.ipynb文件转换成为dcc_crowdnet.py文件，执行

jupyter nbconvert --to script  dcc_crowdnet.ipynb


# 三、调试

在pycharm环境下打开工程

打开dcc_crowdnet.py文件，在run菜单目录下，选择edit configuration，新建python，将dcc_crowdnet.py的路径加入

![](https://img-blog.csdn.net/20180504103622932)

修改代码中的
caffe_root = os.path.expanduser('/home/nvidia/NET/deeplab_v2/deeplab-public-ver2/') # change with your install location

改为自己的路径，然后点击调试按钮

# 四、运行时错误及解决办法``````
``

```
（1）ImportError: No module named sklearn.model_selection
```


``
`sudo apt-get install python-sklearn  `

```
（2）ImportError:
No module named model_selection
```


`    解决办法：更`````````````````
```
``新scikit-learn版本
 https://stackoverflow.com/questions/40704484/importerror-no-module-named-model-selection/40704570
```

``pip install -U scikit-learn``

``

五、实验结果











