# 手把手教你搭建caffe及手写数字识别（Ubuntu下且附mac、纯通俗教程） - 结构之法 算法之道 - CSDN博客





2016年11月08日 19:50:07[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：27030
个人分类：[30.Machine L & Deep Learning](https://blog.csdn.net/v_JULY_v/article/category/1061301)

所属专栏：[机器学习十大算法系列](https://blog.csdn.net/column/details/datamining.html)









> 
> 
> 
# 手把手教你搭建caffe及手写数字识别










作者：七月在线课程助教团队，骁哲、小蔡、李伟、July
时间：二零一六年十一月九日
交流：深度学习实战交流Q群 472899334，有问题可以加此群共同交流。另探究实验背后原理，请参看此课程：[深度学习在线班](https://www.julyedu.com/category/index/8/21)。








## 一、前言

    在前面的教程中，我们搭建了tensorflow、torch，教程发布后，大家的问题少了非常多。但另一大框架caffe的问题则也不少，加之caffe也是[深度学习在线班](https://www.julyedu.com/category/index/8/21)要讲的三大框架之一，因此，我们再把caffe的搭建完整走一遍，手把手且全程命令提示。本教程参考github：[https://github.com/BVLC/caffe](https://github.com/BVLC/caffe)、及《21天实战caffe》P28页Ubuntu环境准备。



    另，笔者的安装环境是Ubuntu14.04、CUDA8.0、cudnn5.1、OpenCV、GTX1070。关于这些的搭建可以参看“[flappy bird](http://blog.csdn.net/v_july_v/article/details/52810219)“”及“[Tensorflow](http://blog.csdn.net/v_july_v/article/details/52658965)[学梵高作画](http://blog.csdn.net/v_july_v/article/details/52658965)”里面的安装教程。






## 二、 安装依赖项

  	更新源

**sudo apt-get update**

参考官网页面地址：[http://caffe.berkeleyvision.org/install_apt.html](http://caffe.berkeleyvision.org/install_apt.html)

安装命令：

**sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler**

**sudo apt-get install --no-install-recommends libboost-all-dev**

特别提示：ubuntu命令行里按住ctrl+shift+c是复制，ctrl+shift+v是粘贴





## 三、 相关安装

安装git命令：

**	sudo apt-get install git**

安装BLAS命令：

**	sudo apt-get install libatlas-base-dev**

安装pycaffe接口所需的依赖项：

**	sudo apt-get install -y python-numpy python-scipy python-matplotlib python-sklearn python-skimage python-h5py python-protobuf python-leveldb python-networkx python-nose python-pandas python-gflags cython ipython**

安装其他依赖项目：

**	sudo apt-get install libgflags-dev libgoogle-glog-dev liblmdb-dev**





## 四、 Caffe相关操作

下载caffe：

**	sudo git clone https://github.com/BVLC/caffe.git**

进入caffe：

**	cd caffe**

配置Makefile.config文件：

**	sudo cp Makefile.config.example Makefile.config**

修改Makefile.config：

**	sudo vi Makefile.config**

笔者这里使用cudnn，所以将第四行

# USE_CUDNN := 1   前面的#删掉变为

USE_CUDNN := 1

示意图：![](https://img-blog.csdn.net/20161109200830982)

编译caffe：

依次执行如下命令

**	sudo make all -j16**

**	sudo make test -j16**

**	sudo make runtest -j16**





## 五、 编译Python用到的caffe文件

编译caffe：

执行如下命令

**	make pycaffe -j16**





## 六、 验证

**cd python**

**python**

**import caffe**

不报错就表明安装成功了！

![](https://img-blog.csdn.net/20161108201823447)








## 七、基于caffe的手写数字识别

caffe MNIST by 小蔡
官方github地址:
[https://github.com/BVLC/caffe](https://github.com/BVLC/caffe)
cd /caffe/caffe 
1.下载数据
./data/mnist/get_mnist.sh 
2.转化为lmdb格式
./examples/mnist/create_mnist.sh
3.训练数据
./examples/mnist/train_lenet.sh

![](https://img-blog.csdn.net/20161110104601695)





    另，mac下的caffe安装请参照：[https://ask.julyedu.com/question/7468](https://ask.julyedu.com/question/7468)。七月在线助教团队、二零一六年十一月九日。



