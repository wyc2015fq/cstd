# 28 天自制你的 AlphaGo (2) : 安装 MXNet 搭建深度学习环境 - 知乎
# 


知乎对于人机大战的关注度很高，所以写这个系列，希望能让大家对于人工智能和围棋有更多的了解。如果有收获，请记得点一下赞。
在上一篇介绍了围棋 AI 的基本常识，这一篇开始搭建深度学习环境。

***本系列已更新多篇，其它几篇的传送门：***
- ***(1) : 围棋 AI 基础 [知乎专栏](https://zhuanlan.zhihu.com/p/24801451)***

- ***(3) : 训练策略网络，真正与之对弈 [知乎专栏](https://zhuanlan.zhihu.com/p/24885190)***

- ***(4) : 对于策略网络的深入分析（以及它的弱点所在） [知乎专栏](https://zhuanlan.zhihu.com/p/24939269)***

- ***(4.5)：后文预告（Or 为什么你应该试试 Batch Normalization 和 ResNet）**[知乎专栏](https://zhuanlan.zhihu.com/p/25051435)***


## **一、在 Windows 下安装 MXNet**

目前的环境很多，最多人用的是 Google 的 TensorFlow；不过 MXNet 感觉也蛮不错，比较省资源（当然，最好两个都装）。配图是 MXNet 的例子里面的 LeNet 训练 MNIST 数据集，这是很经典的模型，可以看到准确率在随着训练不断提高：
![](https://pic1.zhimg.com/v2-cb57634fd1e9a71bc220ee455b2f9528_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='768' height='512'></svg>)
安装之前先看个好玩的：[https://github.com/dmlc/mxnet.js](https://link.zhihu.com/?target=https%3A//github.com/dmlc/mxnet.js) 是在浏览器直接运行 MXNet 的效果。TensorFlow 也有类似的东西： [transcranial/keras-js](https://link.zhihu.com/?target=https%3A//github.com/transcranial/keras-js)。也就说，我们训练好模型后，可以直接在网页里面可视化，这样就可以轻松跨平台。
![](https://pic3.zhimg.com/v2-2b1d170454211daa6ef15e38f73d8c26_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='422' height='806'></svg>)
第一次装深度学习环境经常会遇到一些坑，这里看一个实际安装 MXNet 的过程。先看 Windows 的安装，比较简单和快速，因为不需要编译。


说句无关的，我个人是推荐 Windows 的，因为显卡还可以用于娱乐（这几年的新游戏的图像进步很大），有兴趣还可以玩个 VR 啥的。

1. 首先装了 VC2015，安装时语言记得选上 C++。 

2. 然后如果你有 nVidia 的 GPU，装一下 CUDA：[CUDA 8.0 Downloads](https://link.zhihu.com/?target=https%3A//developer.nvidia.com/cuda-downloads) 。选本地安装版，建议用迅雷下比较快。如果没有 nVidia 显卡，买一块吧，显存尽量选大的，机器的电源也记得要跟上。实在没钱就二手 750Ti 2G 显存版吧，足够玩玩简单模型，因为 MXNet 省显存。当然，用 CPU 也可以跑，就是慢。

3. 再下载 cuDNN，这个要注册一个帐号。注册一个吧： [NVIDIA cuDNN](https://link.zhihu.com/?target=https%3A//developer.nvidia.com/cudnn) 。解压备用。把它先解压一份到开始的 CUDA 目录（你会看到刚好有一样的目录结构）。


4. 下载 MXNet 的编译好的包：[https://github.com/yajiedesign/mxnet/releases](https://link.zhihu.com/?target=https%3A//github.com/yajiedesign/mxnet/releases) 先下载 vc14 base package，然后下载更新包（例如 20170114_mxnet_x64_vc14_gpu.7z 注意 GPU 包就是同时支持 CPU 和 GPU）解压进去，然后把刚才下载的 cuDNN 也解压进 third-party 目录（具体见它的文档）。执行 setupenv.cmd 。

5. 装个 Python 吧，推荐 Anaconda，选 Python 2.7 的版本： [Download Anaconda Now!](https://link.zhihu.com/?target=https%3A//www.continuum.io/downloads) 。建议用迅雷下比较快。装完检验一下 python 命令可用。


6. 进开始的 MXNet 目录的 python 子目录，执行 python setup.py install 。会发现提示要装一个 Microsoft Visual C++ Compiler for Python 2.7，去装了。

7. 然后再执行 python setup.py install，可能会发现提示缺头文件，把他们从你的 VC2015 的 include 目录拷贝到 Microsoft Visual C++ Compiler for Python 2.7 的 include 目录即可。要根据提示拷贝好几个头文件。然后就可以成功编译了。

8. 运行 python，然后 import mxnet 然后 (mxnet.nd.ones((2,2), mxnet.cpu())*100).asnumpy()然后 (mxnet.nd.ones((2,2), mxnet.gpu())*100).asnumpy() 如果全部成功，恭喜你，装好了。

9. 再下载 [dmlc/mxnet](https://link.zhihu.com/?target=https%3A//github.com/dmlc/mxnet)，在 example 目录的 image-classification 目录，执行 python train_mnist.py --network lenet 。会先下载测试数据，等下它，比较慢。看看是否成功训练。


10. 再测试 VC++ 的环境。下载 [https://github.com/dmlc/MXNet.cpp](https://link.zhihu.com/?target=https%3A//github.com/dmlc/MXNet.cpp) （不需要执行里面的 setupenv.cmd），然后打开 windows 目录下面的 vs 下面的 MxnetTestApp，运行试试。再试试里面有句可以改成 Context ctx_dev(DeviceType::kGPU, 0); 会发现 GPU 确实比 CPU 快。

11. 可以用 CPU-z 和 GPU-z 看你的 CPU 和 GPU 有没有偷懒，是否是在全心全意工作。

## **二、在 Mac 下安装 MXNet**

下面看 Mac 的安装，我是 OSX 10.11。

这个安装麻烦一些，因为首先下面有些下载过程可能要 export
ALL_PROXY="代理地址"（否则很慢）。另外 pip 也要换国内源，git 也要加代理，homebrew 也可以改国内源。

然后有时会遇到权限问题，请 chown 一下。有时可能也要 sudo。

1. 装 XCode。最新 CUDA 已经兼容 XCode 8 了。

2. 装 CUDA 和 cuDNN。

3. 装 homebrew（百度搜索一下）。装 python，建议 brew install pyenv 然后用它装 anaconda2，防止破坏系统 python 版本：[Mac OS X下安装pyenv](https://link.zhihu.com/?target=https%3A//my.oschina.net/aetern/blog/399071) 。

3a. 如果发现 pyenv 下载文件奇慢无比，可以给 pyenv 加上 -v 看到找到下载路径，然后手工下载，然后打开 /usr/local/bin/python-build 然后在 download_tarball() 函数里面，直接把第一行改成 local package_url="[http://127.0.0.1/Anaconda2-4.2.0-MacOSX-x86_64.sh](https://link.zhihu.com/?target=http%3A//127.0.0.1/Anaconda2-4.2.0-MacOSX-x86_64.sh)" 然后你自己开一个 http 服务器即可。

4. 装 MXNet： [Installing MXNet on OS X (Mac)](https://link.zhihu.com/?target=http%3A//mxnet.io/get_started/osx_setup.html) 按照 Standard installation 走。不要执行它的 Quick Installation 自动脚本，因为还会去重新装 homebrew，非常慢。第一步是：

git clone [https://github.com/dmlc/mxnet.git](https://link.zhihu.com/?target=https%3A//github.com/dmlc/mxnet.git) ~/mxnet --recursive


5. 按它说的编译。在 make 子目录中的 config.mk 最后加：

USE_BLAS = openblas

ADD_CFLAGS += -I/usr/local/opt/openblas/include

ADD_LDFLAGS += -L/usr/local/opt/openblas/lib

ADD_LDFLAGS += -L/usr/local/lib/graphviz/

USE_CUDA = 1

USE_CUDA_PATH = /usr/local/cuda

USE_CUDNN = 1

USE_NVRTC = 1

USE_PROFILER = 1

USE_OPENMP = 0

另外可能要 ln -s /usr/local/cuda/lib /usr/local/cuda/lib64 。然后 make -j4 就代表用4核编译。有问题就 make clean 一下再试试。

6. make 完后，进 python 子目录 sudo python setup.py install，然后检查是否装好：

cd example/image-classification/

python train_mnist.py

python train_mnist.py --network lenet --gpus 0

祝安装成功！在下一篇我们会看看从棋谱提取训练特征。

***本系列已更新多篇，其它几篇的传送门：***

- ***(1) : 围棋 AI 基础 [知乎专栏](https://zhuanlan.zhihu.com/p/24801451)***
- ***(3) : 训练策略网络，真正与之对弈 [知乎专栏](https://zhuanlan.zhihu.com/p/24885190)***
- ***(4) : 对于策略网络的深入分析（以及它的弱点所在） [知乎专栏](https://zhuanlan.zhihu.com/p/24939269)***

> 如需转载本系列，请先与本人联系，谢谢。小广告：晚上工作学习是否觉得光线不够舒服，精神不够集中，眼睛容易疲劳？不妨点击看看我们的自然全光谱灯系列：[Blink Sunshine护眼LED灯泡 高显指97显色指数无频闪学习台灯床头](https://link.zhihu.com/?target=https%3A//item.taobao.com/item.htm%3Fid%3D40134613056)  如果需要好用的耳机或钱包，我们也有 :-)


