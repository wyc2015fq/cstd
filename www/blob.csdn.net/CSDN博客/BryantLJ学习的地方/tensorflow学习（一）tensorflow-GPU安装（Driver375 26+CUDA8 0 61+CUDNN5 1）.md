# tensorflow学习（一）tensorflow-GPU安装（Driver375.26+CUDA8.0.61+CUDNN5.1） - BryantLJ学习的地方 - CSDN博客





2017年03月12日 09:43:42[遍地流金](https://me.csdn.net/u012177034)阅读数：3856








关键词： 

GTX1080，Ubuntu14.04，Tensorflow-0.11.0-gpu，driver375.26，cuda8.0，cudnn5.1

首先理解调用关系，然后就能理解各个工具的版本依赖关系(tensorflow->cudnn->cdua->driver) 

今天时间为2017/03/04，此时nvidia官网上能下载的cuda8.0工具箱为cuda_8.0.61_375.26_linux.run，其对应的显卡驱动要求为375.26，若显卡驱动版本不满足，则需要重新安装显卡驱动至相应版本。之前安装caffe的时候在官网上下载的driver版本是367.27版本，对应的cuda为cuda8.0.27_linux.run，两者正好匹配，没有出现版本兼容问题。CUDNN5.1对于cuda的版本要求为至少7.0，且计算能至少为3.0。

# 1.CUDA8.0的安装

## 1.1 cuda8.0的安装

这里有一个跟安装caffe时的明显变化，也就是**不是 先安装驱动，而是直接在cuda安装时选择安装驱动**，这样的原因有两个 

（1）此文件里提供的driver较新（2）此文件提供的driver版本肯定是能够与所要安装的cuda兼容，不需要自己费尽心思搞清楚自己所需要安装的driver版本，否则很容易出现不兼容的问题。

```bash
#按ctrl+alt+f1进入tty1
sudo service lightdm stop # 关闭图形界面
sudo ./cuda_8.0.61_375.26_linux.run #此步安装时全部选择yes，包括驱动，然后等待安装完成
sudo service lightdm start # 重启图形界面
#按ctrl+alt+f7进入图形界面
```

## 1.2 安装cuda的依赖库

此部分之前安装caffe时已安装完成，因此不需要重新安装

```
sudo apt-get install build-essentials
sudo apt-get install mesa-common-dev
sudo apt-get install freeglut3-dev
```

## 1.4 修改链接库目录

首先在/etc/ld.so.conf文件的末尾添加’/usr/local/cuda/lib64’，然后在终端输入

```bash
sudo ldconfig # 使文件生效
```

## 1.5 cuda-sample的编译测试

此部分参考caffe安装的博客~

# 2.依赖库的安装

此部分参考caffe安装博客即可。有一点需要注意的是，如果是全部重新安装的话，直接安装aconda的python集成包更方便。

# 3.CUDNN的安装

此部分参考caffe安装博客即可。

# 4.Tensorflow的BINARY安装方式

以下安装方式主要参考官方指导ihttps://www.tensorflow.org/versions/r0.11/get_started/os_setup.html#pip-installation 

此方式安装最为简单。主要注意的是选择python和GPU版本。如若安装的是CPU版本则其不会调用GPU来计算。 

二进制安装tensorflow时，在安装GPU版本的时候可能会遇到CUDNN版本不一致的问题，如今天安装的tensorflow其源码是按照cudnn5.1来编译的，而我电脑上的才开始是cudnn5.0，因此不兼容。需要手动将cudnn的版本升级成5.1.升级方式就是直接替换cudnn的头文件和库文件即可。 

安装完以后需要添加cuda以及cudnn的目录，以便tensorflow能够找到cuda并调用。只需设置以下几个环境变量即可：
```bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/extras/CUPTI/lib64"
export CUDA_HOME=/usr/local/cuda
```

上面安装完成以后就能够在python里调用tensorflow啦~~~




