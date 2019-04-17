# 在Windows上安装GPU版Tensorflow - 战斗蜗牛的专栏 - CSDN博客





2017年07月25日 17:51:38[vbskj](https://me.csdn.net/vbskj)阅读数：6650








                
1. 下载安装Anaconda


简单说就是下载 64位 python 3.5 版本的Anaconda


https://www.continuum.io/downloads#windows


安装情况：新机，未装python。


注意


a. Windows只支持64位 python 3.5


https://www.tensorflow.org/versions/master/get_started/os_setup#pip_installation_on_windows


b. 如果import tensorflow as tf 时有errorNo module named "_pywrap_tensorflow" 或者DLL load failed, checkMSVCP140.DLLis in%PATH%。解决方法：install theVisual C++ 2015 redistributable(x64 version). 不过我没遇到这个情况。


c. Anaconda虚拟环境内外只能存在一个tensorflow，所以多出来的要删除。因为Anaconda searches system site-packages from.localwith higher priority.


2. 进入Anaconda 菜单栏下的“Anaconda Prompt”


2.1 更新pip


输入命令 python -m pip install --upgrade pip


2.2 安装Tensorflow


按照不同需求输入相应的 命令， 记得删去 "C:\>"


CPU-only version

pip install --upgrade https://storage.googleapis.com/tensorflow/windows/cpu/tensorflow-0.12.1-cp35-cp35m-win_amd64.whl --ignore-installed


非GPU版在这里就可以结束了。在Anaconda Prompt输入python 后就能 导入tensorflow 包了。

我们着重讲讲GPU版！


pip install --upgrade https://storage.googleapis.com/tensorflow/windows/gpu/tensorflow_gpu-0.12.1-cp35-cp35m-win_amd64.whl --ignore-installed


安装完后可以看到下面几个包


Installing collected packages: appdirs, six, pyparsing, packaging, setuptools, protobuf, numpy, wheel, tensorflow-gpu


安装成功后查看版本


python -c "import tensorflow; print(tensorflow.__version__)"


然后你会发现我们找到不到各种lib，不急，因为我们还没装呢。


（https://github.com/ContinuumIO/anaconda-issues/issues/542）!!!


安装cuDNN和CUDA Toolkit


(win + R 后输入dxdiag可以看自己的显卡配置，用来选对应的英伟达开发包驱动。)


1. 下载 cuDNN:


NVIDIA CUDA® Deep Neural Network library (cuDNN) 大概54mb


https://developer.nvidia.com/rdp/cudnn-download


下载文件不稳定，多试几次（我下了三次）


这是个压缩文件包，解压缩后是三个文件夹，各有一个文件在里面。

2. 下载CUDA Toolkit 8.0: （我是下的8.0）


文件大小约1.2 G，下载文件不稳定，花了很久时间，重复用net 安装版


https://developer.nvidia.com/cuda-downloads


(C:\Users\xxxx\AppData\Local\Temp\CUDA 这个临时文件夹不用多虑，安装完了它会自动删除) 另外你要是装了360 就要不断“允许该进程所有操作 ”


理论上这个步骤会自动把路径安装到你的windows path里。


3. 把cuDNN的文件复制到CUDAToolkit 安装目录


应该在C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0  （根据你的选择可能会有所不同）


总之你可以在那个目录下找到一堆文件夹，其中包括了那三个解压缩后的三个文件。


把那三个文件放到相同文件名的文件夹中。


万事俱备只欠重启！

重启电脑后，再次进入Anaconda Prompt


输入python进入


import tensorflow


看到几个 successfully opened


恭喜你GPU版安装完毕！


注意: 如果导入tensorflow时依然报错，请到path环境变量下确认路径


To use cuDNN with TensorFlow, the filecudnn64_5.dll must be in your%PATH%environment variable.


http://stackoverflow.com/questions/41007279/tensorflow-on-windows-couldnt-open-cuda-library-cudnn64-5-dll


一些其他的特殊情况：


https://github.com/tensorflow/tensorflow/issues/5968


C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\bin




写完发现了两篇也不错的指导


http://www.itdadao.com/articles/c15a832274p0.html


https://zhuanlan.zhihu.com/p/24369784?refer=wjdml


作者：alucardzhou

链接：http://www.jianshu.com/p/1fad663dabc3

來源：简书

著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            


