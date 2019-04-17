# Tensorflow-gpu & Object Detection API & Tensorflow Serving 配置过程 - qq_38593211的博客 - CSDN博客





置顶2018年10月24日 16:28:47[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：106








> 
系统环境：Centos 7.3（ubuntu大同小异，亲测Ubuntu16.04）


## Nvidia 驱动

1.安装kernel

```
yum -y install gcc kernel-devel kernel-headers
```

2.导入公共密钥

```
rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org
```

3.安装ELRepo

```
rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm
```

4.屏蔽默认带有的nouveau驱动

```
echo -e "blacklist nouveau\noptions nouveau modeset=0" > /etc/modprobe.d/blacklist.conf
或：修改/etc/modprobe.d/blacklist.conf 文件，添加：
blacklist nouveau
options nouveau modeset=0
删除（注释）blacklist nvidiafb
如果 ls mod | grep nouveau 无输出，则屏蔽成功
```

> 
以下步骤可以在纯命令行中（ctrl+alt+f2）进行


5.安装显卡驱动检测驱动

```
sudo yum install nvidia-detect
nvidia-detect -v
```

6.安装显卡驱动

```
sudo yum install nvidia-x11-drv-384xx nvidia-x11-drv-384xx-64bit
注：一般安装上一部显示的最新版本显卡即可，GTX1060在384和410版本测试过。
```

7.测试

```
sudo reboot
nvidia-smi
$ 显示显卡信息
```

## CUDA

> 
建议CUDA9.0，CUDA9.2也可以可以，支持性9.0能支持VS2017以及TF1.8以上，其他未测试。


1.下载cuda9.0安装文件，[可以根据系统选择文件](https://developer.nvidia.com/cuda-90-download-archive)

2.安装cuda方法一

```
sudo sh cuda_9.0.103_384.59_linux.run
	安装程序中第二项，是否选择安装nvida驱动，选N，其余默认或者自行分配安装位置。
```

3.方法二

```
sudo rpm -i cuda-repo-rhel7-9-0-local-9.0.176-1.x86_64.rpm
sudo yum clean all
sudo yum install cuda
```

4.测试（显示Result=Pass）

```
cd /usr/local/cuda-8.0/samples/1_Utilities/deviceQuery
sudo make
sudo ./deviceQuery
```

## cudnn

> 
可选，在Anaconda中可以自动安装


1.[在Nvidia官网下载对应CUDA版本的文件即可，需要注册](https://developer.nvidia.com/cudnn)

2.解压

```
tar -xzvf cudnn-9.0-linux-x64-v7.3.0.29.tgz（对应文件）
```

3.配置（对应路径）

```
sudo cp cuda/include/cudnn.h /usr/local/cuda/include
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64
sudo chmod a+r /usr/local/cuda/include/cudnn.h
sudo chmod a+r /usr/local/cuda/lib64/libcudnn*
```

4.添加环境变量

```
sudo vim .bashrc
在文档最后一行加入：（对应安装路径）
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
source .bashrc（立即执行）
```

## Anaconda

1.[清华镜像网站下载需要的Anaconda版本](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)

2.Anaconda自带Python3和Python2的编译环境，但创建一个虚拟环境，保留原生环境，以后要用tensorflow就必须在启动这个环境才能用

```
conda create -n ‘conv-name’ python=3.6
激活：
source activate ‘conv-name’
退出：
source deactivate ‘conv-name’
```

3.安装Tensorflow-gpu-1.9

```
pip install  tensorflow-gpu==1.9
如果没装cudnn：
conda install  tensorflow-gpu==1.9
```

4.测试

```
python import tensorflow as tf
hello = tf.constant(‘hello,Tensorflow!’)
session = tf.Session()
print(session.run(hello))
```

5.如果想在notebook中用tensorflow（可选）

```
conda install ipython
conda install jupyter
```

## Object Detection API

1.下载Object Detection API，在[tensorflow/models](https://github.com/tensorflow/models)中，可以git下来，也可以直接下载

```
git clone https://github.com/tensorflow/models
```

2.配置需求库

```
sudo yum install protobuf-compiler python-pil python-lxml python-tk
pip install  Cython
pip install  contextlib2
pip install  jupyter
pip install  matplotlib
```

3.Protobuf配置

```
cd models/reseach
protoc object_detection/protos/*.proto --python_out=.
将slim路径添加到环境变量：
export PYTHONPATH=$PYTHONPATH:`pwd`:`pwd`/slim
```

4.测试

```
python object_detection/builders/model_builder_test.py
```

> 
至此，Object Detection API就已经安装完成，使用参见另外一篇[教程](https://blog.csdn.net/qq_38593211/article/details/82823255)


## Tensorflow Serving

> 
model server安装部署有两种方式，基于docker和基于APT，官网推荐docker安装，以下基于docker安装


1.内核版本检测，Docker 要求 CentOS 系统的内核版本高于 3.10

```
uname -r 
yum -y update
```

2.安装测试docker

```
yum install -y docker
sudo docker version
```

3.获取serving镜像

```
sudo docker pull tensorflow/serving
```

4.部署模型到docker

```
1.git serving 获取需要测试的模型文件：
 mkdir -p /tmp/tfserving
 cd /tmp/tfserving
 git clone https://github.com/tensorflow/serving
 模型path：/tmp/tfserving/serving/tensorflow_serving/servables/tensorflow/testdata/saved_model_half_plus_two_cpu
2.以serving镜像建立一个restful（8501）接口的容器
 docker run -p 8501:8501 \
  --mount type=bind,\
   source=/tmp/tfserving/serving/tensorflow_serving/servables/tensorflow/testdata/saved_model_half_plus_two_cpu,\
   target=/models/half_plus_two  -e MODEL_NAME=half_plus_two -t tensorflow/serving &
```

5.测试（新开终端）

```
curl -d '{"instances": [1.0, 2.0, 5.0]}'  -X POST http://localhost:8501/v1/models/half_plus_two:predict
```



