
# 利用Deepo做深度学习环境 - leofionn的博客 - CSDN博客


2018年08月12日 12:27:29[leofionn](https://me.csdn.net/qq_36142114)阅读数：1053


[https://hub.docker.com/r/ufoym/deepo/](https://hub.docker.com/r/ufoym/deepo/)
![](https://user-images.githubusercontent.com/2270240/32102393-aecf573c-bb4e-11e7-811c-dc673cae7b9c.png)
![](https://img.shields.io/circleci/project/github/ufoym/deepo.svg)![](https://img.shields.io/docker/pulls/ufoym/deepo.svg)![](https://img.shields.io/github/license/ufoym/deepo.svg)
**Deepo**是一系列*Docker*镜像
可让您快速设置深度学习研究环境
支持几乎所有[常用的深度学习框架](https://hub.docker.com/r/ufoym/deepo/#tags)
支持[GPU加速](https://hub.docker.com/r/ufoym/deepo/#GPU)（包括CUDA和cuDNN），也适用于[仅CPU模式](https://hub.docker.com/r/ufoym/deepo/#CPU)
适用于Linux（[CPU版本](https://hub.docker.com/r/ufoym/deepo/#CPU)/[GPU版本](https://hub.docker.com/r/ufoym/deepo/#GPU)），Windows（[CPU版本](https://hub.docker.com/r/ufoym/deepo/#CPU)）和OS X（[CPU版本](https://hub.docker.com/r/ufoym/deepo/#CPU)）
和他们的Dockerfile生成器
允许您使用类似Lego的模块[自定义您自己的环境](https://hub.docker.com/r/ufoym/deepo/#Build)
自动为您解析依赖项
---

# 目录
[可用标签](https://hub.docker.com/r/ufoym/deepo/#tags)
[快速开始](https://hub.docker.com/r/ufoym/deepo/#Quick-Start)[GPU版本](https://hub.docker.com/r/ufoym/deepo/#GPU)[安装](https://hub.docker.com/r/ufoym/deepo/#Installation)
[用法](https://hub.docker.com/r/ufoym/deepo/#Usage)
[CPU版本](https://hub.docker.com/r/ufoym/deepo/#CPU)[安装](https://hub.docker.com/r/ufoym/deepo/#Installation-cpu)
[用法](https://hub.docker.com/r/ufoym/deepo/#Usage-cpu)

[定制](https://hub.docker.com/r/ufoym/deepo/#Customization)[对一体化解决方案不满意？](https://hub.docker.com/r/ufoym/deepo/#One)
[其他python版本](https://hub.docker.com/r/ufoym/deepo/#Python)
[Jupyter的支持](https://hub.docker.com/r/ufoym/deepo/#Jupyter)
建立自己的自定义镜像
[特约](https://hub.docker.com/r/ufoym/deepo/#Contributing)
[许可](https://hub.docker.com/r/ufoym/deepo/#Licensing)
---

# Available Tags
|.|GPU / Python 3.6|GPU / Python 2.7|CPU-only / Python 3.6|CPU-only / Python 2.7|
|---|---|---|---|---|
|all-in-one|all-py36|all|py36|latest|all-py27|py27|all-py36-cpu|all-cpu|py36-cpu|cpu|all-py27-cpu|py27-cpu|
|all-in-one with jupyter|all-py36-jupyter|py36-jupyter|all-py27-jupyter|py27-jupyter|all-py36-jupyter-cpu|py36-jupyter-cpu|all-py27-jupyter-cpu|py27-jupyter-cpu|
|[theano](http://deeplearning.net/software/theano)|theano-py36|theano|theano-py27|theano-py36-cpu|theano-cpu|theano-py27-cpu|
|[tensorflow](http://www.tensorflow.org/)|tensorflow-py36|tensorflow|tensorflow-py27|tensorflow-py36-cpu|tensorflow-cpu|tensorflow-py27-cpu|
|[sonnet](https://github.com/deepmind/sonnet)|sonnet-py36|sonnet|sonnet-py27|sonnet-py36-cpu|sonnet-cpu|sonnet-py27-cpu|
|[pytorch](http://pytorch.org/)|pytorch-py36|pytorch|pytorch-py27|pytorch-py36|pytorch|pytorch-py27|
|[keras](https://keras.io/)|keras-py36|keras|keras-py27|keras-py36-cpu|keras-cpu|keras-py27-cpu|
|[lasagne](http://lasagne.readthedocs.io/)|lasagne-py36|lasagne|lasagne-py27|lasagne-py36-cpu|lasagne-cpu|lasagne-py27-cpu|
|[mxnet](http://mxnet.incubator.apache.org/)|mxnet-py36|mxnet|mxnet-py27|mxnet-py36-cpu|mxnet-cpu|mxnet-py27-cpu|
|[cntk](http://cntk.ai/)|cntk-py36|cntk|cntk-py27|cntk-py36-cpu|cntk-cpu|cntk-py27-cpu|
|[chainer](https://chainer.org/)|chainer-py36|chainer|chainer-py27|chainer-py36-cpu|chainer-cpu|chainer-py27-cpu|
|[caffe](http://caffe.berkeleyvision.org/)|caffe-py36|caffe|caffe-py27|caffe-py36-cpu|caffe-cpu|caffe-py27-cpu|
|[torch](http://torch.ch/)|torch|torch|torch-cpu|torch-cpu|
# 快速开始
## GPU版本
### 安装
步骤1.安装[Docker](https://docs.docker.com/engine/installation/)和[nvidia-docker](https://github.com/NVIDIA/nvidia-docker)。
步骤2.从[Docker Hub](https://hub.docker.com/r/ufoym/deepo)获取一体化镜像
`docker pull ufoym/deepo`
### 用法
现在您可以尝试以下命令：
`nvidia-docker run --rm ufoym/deepo nvidia-smi`这应该可以工作，并使Deepo能够从docker容器内部使用GPU。
如果这不起作用，请搜索[nvidia-docker GitHub上的问题部分](https://github.com/NVIDIA/nvidia-docker/issues)- 已经记录了许多解决方案。要将交互式shell发送到退出do后不会自动删除的容器
`nvidia-docker run -it ufoym/deepo bash`如果要在主机（计算机或VM）与使用Deepo的容器之间共享数据和配置，请使用-v选项，例如
`nvidia-docker run -it -v /host/data:/data -v /host/config:/config ufoym/deepo bash`这将使`/host/data`主机`/data`在容器中可见，并且`/host/config`如同`/config`。这种隔离降低了容器化实验覆盖或使用错误数据的机会。
请注意，某些框架（例如PyTorch）使用共享内存在进程之间共享数据，因此如果使用多处理，容器运行的默认共享内存段大小是不够的，您应该使用`--ipc=host`或`--shm-size`命令行选项增加共享内存大小到`nvidia-docker run`。
`nvidia-docker run -it --ipc=host ufoym/deepo bash`
## CPU版本
### 安装
步骤1.安装[Docker](https://docs.docker.com/engine/installation/)。
步骤2.从[Docker Hub](https://hub.docker.com/r/ufoym/deepo)获取一体化镜像
`docker pull ufoym/deepo:cpu`
### 用法
现在您可以尝试以下命令：
`docker run -it ufoym/deepo:cpu bash`如果要在主机（计算机或VM）与使用Deepo的容器之间共享数据和配置，请使用-v选项，例如
`docker run -it -v /host/data:/data -v /host/config:/config ufoym/deepo:cpu bash`这将使`/host/data`主机`/data`在容器中可见，并且`/host/config`如同`/config`。这种隔离降低了容器化实验覆盖或使用错误数据的机会。
请注意，某些框架（例如PyTorch）使用共享内存在进程之间共享数据，因此如果使用多处理，容器运行的默认共享内存段大小是不够的，您应该使用`--ipc=host`或`--shm-size`命令行选项增加共享内存大小到`docker run`。
`docker run -it --ipc=host ufoym/deepo:cpu bash`*您现在已准备好开始您的旅程。*
`$ python`
`>>> import tensorflow
>>> import sonnet
>>> import torch
>>> import keras
>>> import mxnet
>>> import cntk
>>> import chainer
>>> import theano
>>> import lasagne
>>> import caffe``$ caffe --version`
`caffe version 1.0.0``$ th`
`│  ______             __   |  Torch7
 │ /_  __/__  ________/ /   |  Scientific computing for Lua.
 │  / / / _ \/ __/ __/ _ \  |  Type ? for help
 │ /_/  \___/_/  \__/_//_/  |  https://github.com/torch
 │                          |  http://torch.ch
 │
 │th>`
# 定制
请注意，“[快速入门”](https://hub.docker.com/r/ufoym/deepo/#Quick-Start)中`docker pull ufoym/deepo`提到的将为您提供包含所有可用深度学习框架的标准镜像。您也可以自定义自己的环境。
## 对一体化解决方案不满意？
如果您更喜欢特定的框架而不是一体化的镜像，只需附加一个带有框架名称的标签。
以tensorflow为例：
`docker pull ufoym/deepo:tensorflow`
## 其他python版本
请注意，`Python 3.6`默认情况下所有与python相关的镜像都使用。如果您不满意`Python 3.6`，还可以指定其他python版本：
`docker pull ufoym/deepo:py27``docker pull ufoym/deepo:tensorflow-py27`目前，我们支持`Python 2.7`和`Python 3.6`。
有关所有可用标记的完整列表，请参阅[可用](https://hub.docker.com/r/ufoym/deepo/#tags)标记。这些预先构建的镜像都是由`docker/Dockerfile.*`和构建的`circle.yml`。请参阅[如何生成](https://github.com/ufoym/deepo/tree/master/scripts)`docker/Dockerfile.*`[和](https://github.com/ufoym/deepo/tree/master/scripts)`circle.yml`如果你有兴趣在这些文件是如何生成的。
## Jupyter的支持
步骤1.使用jupyter支持拉动镜像
`docker pull ufoym/deepo:all-py36-jupyter`请注意，标签可以任意的`all-py36-jupyter`，`py36-jupyter`，`all-py27-jupyter`，或`py27-jupyter`。
第2步。运行镜像
`nvidia-docker run -it -p 8888:8888 ufoym/deepo:all-py36-jupyter jupyter notebook --no-browser --ip=0.0.0.0 --allow-root --NotebookApp.token= --notebook-dir='/root'`
## 使用类似Lego的模块构建您自己的自定义镜像
步骤1.准备发电机
`git clone https://github.com/ufoym/deepo.git
cd deepo/generator
pip install -r requirements.txt`第2步。生成自定义的Dockerfile
例如，如果你喜欢`pytorch`和`lasagne`，然后
`python generate.py Dockerfile pytorch lasagne`这应该生成一个Dockerfile，其中包含构建`pytorch`和的所有内容`lasagne`。请注意，生成器可以处理自动依赖关系处理并对列表进行拓扑排序。因此，您无需担心缺少依赖项和列表顺序。
您还可以指定Python的版本：
`python generate.py Dockerfile pytorch lasagne python==3.6`第3步。构建您的Dockerfile
`docker build -t my/deepo .`这可能需要几分钟，因为它从头开始编译一些库。
# 特约
我们感谢所有的贡献。如果您计划回复错误修复，请不要进一步讨论。如果您计划提供新功能，实用程序功能或扩展，请首先打开一个问题并与我们讨论该功能。
# 许可
Deepo[获得MIT许可](https://github.com/ufoym/deepo/blob/master/LICENSE)。

