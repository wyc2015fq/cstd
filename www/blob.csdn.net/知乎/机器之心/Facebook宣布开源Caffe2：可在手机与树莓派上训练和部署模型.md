# Facebook宣布开源Caffe2：可在手机与树莓派上训练和部署模型 - 知乎
# 



> 
*在今年的 F8 开发者大会上，Facebook 正式宣布开源其全新深度学习框架 Caffe2。据 Caffe2 官方博客介绍，该框架可以用在 iOS、Android 和树莓派上训练和部署模型；而且 Facebook 已经与英伟达、高通、英特尔、亚马逊和微软等公司展开了合作来实现对移动端的优化。*



为了有效地训练和部署人工智能模型，我们往往会用到大型数据中心或超级计算机。为了能够大规模地连续处理、创建和提升各种各样的信息（图像、视频、文本和音频）之上的模型，我们需要的计算能力不可小觑。如果我们要在移动设备上部署这些模型，那么它们就必须要非常快而且轻量，但这也同样十分困难。要克服这些难题，我们需要一种稳健的、灵活的和便携式的深度学习框架。


Facebook 一直以来都在和开源社区的其他开发者合作一起打造这样一款框架。今天，Facebook 宣布开源了第一版生产可用的 Caffe2 版本，这是一个轻量级的、模块化的深度学习框架，并且在强调了便携性的同时保持了可扩展性和性能。


我们致力于为社区提供高性能的机器学习工具，以便人人都能创造智能的应用和服务。与 Caffe2 一同发布的还有相关的一些教程和案例，其中包括在一台机器上使用多个 GPU 的大规模学习和使用一个或多个 GPU 的在多台机器上的大规模学习、学习在 iOS、Android 和树莓派上训练和部署模型。另外，你只需要编写几行代码就能调用来自 Caffe2 Model Zoo 的预训练模型。


Caffe2 部署在 Facebook 之中以帮助研发人员训练大型机器学习模型，并为手机用户提供人工智能驱动的良好体验。现在，开发者可以访问很多相同的工具，允许他们运行大规模分布式训练方案，并创建手机端的机器学习应用。我们已与英伟达、 高通、英特尔、亚马逊和微软展开密切合作，从而在云端和手机端优化 Caffe2。这些合作将允许机器学习社区快速完成使用更复杂模型的实验过程，并部署下一代人工智能增强型应用和服务。


你可以在 caffe2.ai 上查看 Caffe2 文档和教程，并在 GitHub 查看源代码。如果你考虑使用 Caffe2，我们很乐意了解你的具体需求。请参与我们的调查。我们将向你发送有关新版本和特殊的开发者活动／网络研讨会的信息。

- 
主页：[http://caffe2.ai](https://link.zhihu.com/?target=http%3A//caffe2.ai)

- 
GitHub： [caffe2/caffe2](https://link.zhihu.com/?target=https%3A//github.com/caffe2/caffe2)

- 
调查：[Caffe2 (A Lightweight, Modular, and Scalable Deep Learning Framework)](https://link.zhihu.com/?target=https%3A//www.surveymonkey.com/r/caffe2)



以下是 Caffe2 在 GitHub 上开源项目的介绍：


Caffe2 是一个兼具表现力、速度和模块性的深度学习框架，是 Caffe 的实验性重构，能以更灵活的方式组织计算。


**许可**


Caffe2 的发布许可许可 ：[Yangqing/caffe2](https://link.zhihu.com/?target=https%3A//github.com/Yangqing/caffe2/blob/master/LICENSE)


**建立 Caffe2**


详细的构建矩阵：
![](https://pic4.zhimg.com/v2-4fef4bf0ffd042eed507ab21e5e3a7bf_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='356' height='297'></svg>)

```
git clone --recursive https://github.com/caffe2/caffe2.git
cd caffe2
```


**OS X**


```
brew install automake protobuf
mkdir build && cd build
cmake ..
make
```


**Ubuntu**


可运行版本：

- 
Ubuntu 14.04

- 
Ubuntu 16.06



**需要的依赖包**


```
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
      build-essential \
      cmake \
      git \
      libgoogle-glog-dev \
      libprotobuf-dev \
      protobuf-compiler \
      python-dev \
      python-pip                          
sudo pip install numpy protobuf
```


**可选择 GPU 支持**


如果你计划使用 GPU，而不只是使用 CPU，那你应该安装 NVIDIA CUDA 和 cuDNN，这是一个面向深度神经网络的 GPU 加速库。英伟达在官方博客中详细介绍了安装指南，或者可以尝试下面的快速安装指令。首先，一定要升级你的图显驱动！否则你可能遭受错误诊断的极大困难。


**安装 Ubuntu 14.04**


```
sudo apt-get update && sudo apt-get install wget -y --no-install-recommends
wget "http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1404/x86_64/cuda-repo-ubuntu1404_8.0.61-1_amd64.deb"
sudo dpkg -i cuda-repo-ubuntu1404_8.0.61-1_amd64.deb
sudo apt-get update
sudo apt-get install cuda
```


**安装 Ubuntu 16.04**


```
sudo apt-get update && sudo apt-get install wget -y --no-install-recommends
wget "http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1604/x86_64/cuda-repo-ubuntu1604_8.0.61-1_amd64.deb"
sudo dpkg -i cuda-repo-ubuntu1604_8.0.61-1_amd64.deb
sudo apt-get update
sudo apt-get install cuda
```


**安装 cuDNN（所有都是 Ubuntu 版本）**


```
CUDNN_URL="http://developer.download.nvidia.com/compute/redist/cudnn/v5.1/cudnn-8.0-linux-x64-v5.1.tgz"
wget ${CUDNN_URL}
sudo tar -xzf cudnn-8.0-linux-x64-v5.1.tgz -C /usr/local
rm cudnn-8.0-linux-x64-v5.1.tgz && sudo ldconfig
```


可选择的依赖项


注意，Ubuntu 14.04 使用 libgflags2。Ubuntu 16.04 使用 libgflags-dev。


```
# for Ubuntu 14.04
sudo apt-get install -y --no-install-recommends libgflags2
```



```
# for Ubuntu 16.04
sudo apt-get install -y --no-install-recommends libgflags-dev
```



```
# for both Ubuntu 14.04 and 16.04
sudo apt-get install -y --no-install-recommends \
      libgtest-dev \
      libiomp-dev \
      libleveldb-dev \
      liblmdb-dev \
      libopencv-dev \
      libopenmpi-dev \
      libsnappy-dev \
      openmpi-bin \
      openmpi-doc \
      python-pydot
```

检查下面的 Python 部分，并在建立 Caffe2 之前安装可选择的程序包。

```
mkdir build && cd build
cmake ..
make
```


**安卓和 iOS**


我们使用 CMake 的安卓和 iOS 端口构建原始二进制文件，然后就能将其集成到安卓或 XCode 项目中。查看脚本/build_android.sh 和/build_ios.sh 获得具体信息。


对于安卓系统，我们可以使用 gradle 通过 Android Studio 直接构建 Caffe2。这里是一个示例项目：[bwasti/AICamera](https://link.zhihu.com/?target=https%3A//github.com/bwasti/AICamera)。注意，你可能需要配置 Android Studio，这样你编写代码的 SDK 和 NDK 版本才会正确。


**树莓派**


对于 Raspbian 系统，只需要在树莓派上运行脚本/build_raspbian.sh 就行了。


**Tegra X1**


为了在英伟达的 Tegra X1 平台上安装 Caffe2，需要使用 NVidia JetPack 安装器简单地安装最新版本的系统，然后再在 Tegra 设备上运行脚本/build_tegra_x1.sh。


**Python 支持**


为了进行下面的教程，Python 环境需要安装 ipython-notebooks 和 matplotlib，在 OS X 系统中可以通过以下方法安装：


```
brew install matplotlib --with-python3
pip install ipython notebook
```


你会发现下面的 Python 库同样在具体的教程和案例中是必需的，所以你可以运行下面的命令行一次性安装所有的要求库：


```
sudo pip install \
      flask \
      graphviz \
      hypothesis \
      jupyter \
      matplotlib \
      pydot python-nvd3 \
      pyyaml \
      requests \
      scikit-image \
      scipy \
      setuptools \
      tornado
```


**构建环境（已知能运行） **


![](https://pic3.zhimg.com/v2-6c6f5af3508e2862ee8ee05d2ddd8172_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='783' height='847'></svg>)选自[Caffe2.ai](https://link.zhihu.com/?target=http%3A//caffe2.ai/blog/2017/04/18/caffe2-open-source-announcement.html)**机器之心报道**


