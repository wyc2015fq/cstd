# 教你从头到尾利用DQN自动玩flappy bird（全程命令提示，GPU+CPU版） - 结构之法 算法之道 - CSDN博客





2016年10月13日 21:26:10[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：35089
个人分类：[30.Machine L & Deep Learning](https://blog.csdn.net/v_JULY_v/article/category/1061301)

所属专栏：[机器学习十大算法系列](https://blog.csdn.net/column/details/datamining.html)









##     教你从头到尾利用DQN自动玩flappy bird（全程命令提示，GPU+CPU）







作者：骁哲、李伟、July
说明：本文分两部分，第一部分为GPU版教程，第二部分为[CPU版教程](http://blog.csdn.net/v_july_v/article/details/52810219#t19)，两个教程都主要由骁哲编写，李伟校对，而最后跑的是yenchenlin的github开源[demo](https://github.com/yenchenlin/DeepLearningFlappyBird)。如遇问题欢迎加Q群交流：472899334。且若探究实验背后原理，请参看此课程：[**深度学习在线班**](https://www.julyedu.com/category/index/8/21)。
时间：二零一六年十月十三日。








## 前言

    我们在上一篇教程《基于torch学汪峰写歌词、聊天机器人、图像着色/生成、看图说话、字幕生成》中说到：“让每一个人都能玩一把，无限降低初学朋友的实验门槛”，那是否能把难度再次降低呢，比如部分同学不熟悉Linux命令咋整，那是不是不熟悉Linux命令就没法折腾了？然既然是为了让每个人都能玩一把，那就应该尽最大可能照顾到最大多数。

    本教程**提供全程命令提示，以便让Linux命令暂不熟的同学也能搭建起来**。因此，自动玩转flappy bird分三个步骤：
- 不管三七二十一，先把游戏搭建起来
- 搭建起来后，Linux命令后续慢慢熟悉，熟悉后，一通百通，搭建其他实验的环境也会立马顺畅许多
- 取得成就感和安心之后，再细细深究实验背后之原理（当然，[深度学习在线班](https://www.julyedu.com/category/index/8/21)上也会深究实验背后原理）

   另本教程省略了ubuntu14.04安装，如果此前没安装过Ubuntu，可以参看[《教你从头到尾利用DL学梵高作画》](http://blog.csdn.net/v_july_v/article/details/52658965)里的第[4.1](http://blog.csdn.net/v_july_v/article/details/52658965#t10)部分。且，本文本一开始只有GPU版的教程，但为照顾到没有GPU的同学，特地在本文第二部分增加CPU版的教程，以让每一个人都能玩。

   还是这个事，欢迎更多朋友跟我们一起做实验，一起玩。包括本flappy bird在内的8个实验：梵高作画、文字生成、自动聊天机器人、图像着色、图像生成、看图说话、字幕生成、flappy bird，今2016年内做出这8个实验中的任意一个并在微博上AT@研究者July，便送100上课券，把实验心得发社区 ask.julyed.com 后，再送100上课券。











## 第一部分、GPU版教程

## 1.1NVIDIA驱动、CUDA、cudnn安装

## 下载相应文件，后续 使用，下载地址 ：
- NVIDIA驱动    [http://www.geforce.cn/drivers](http://www.geforce.cn/drivers)   （搜索 对应型号）
- CUDA驱动  [https://developer.nvidia.com/cuda-toolkit](https://developer.nvidia.com/cuda-toolkit) （搜索对应版本）
- CUDNN驱动  https://developer.nvidia.com/cudnn

以下操作均使用root账户

apt-get update （更新源）

apt-get install vim （安装VIM，也可使用 emacs nano）

	vi /etc/default/grub （进入grub文件）

启用字符界面登录

将这行     GRUB_CMDLINE_LINUX_DEFAULT="quiet"  中的 quiet 修改为 text

GRUB_CMDLINE_LINUX_DEFAULT="text"

保存退出

	update-grub2		（更新一下）

	reboot			（重启）

### 1.1.1、 Install NVIDIA Driver		安装NVIDIA驱动

  cd /**/**/** (cd到cuda所在文件目录下)    

  ./NVIDIA-Linux-x86_64-367.44.run   （安装NVIDIA驱动，此文件需执行权限，chmod +x NVIDIA-Linux-x86_64-367.44.run）

  reboot  （重启）

### 1.1.2、 Install CUDA	安装CUDA

  cd /**/**/** (cd到cuda所在文件目录下)

  ./cuda_8.0.27_linux.run  （安装CUDA，此文件需执行权限，如遇权限问题 可执行  chmod +x 文件名）

！accept之后第一个选项填写“n”（该选项让你选择是否安装NVIDIA的Driver，之前已经安装过了，  所以不需要），之后一路“绿灯”。

	vi /etc/default/grub    （打开grub）

启用图形界面登录

将这行     GRUB_CMDLINE_LINUX_DEFAULT="text"  中的 text修改为 quiet

保存退出

	update-grub2		（更新一下）

	reboot		（重启）

### 1.1.3、 Install cuDNN 		安装cuDNN

  tar xvzf cudnn-7.5-linux-x64-v5.1-ga.tgz   （解压）



将CUDNN解压，将解压出来的文件复制到 CUDA 目录 如下

  sudo cp cuda/include/cudnn.h /usr/local/cuda/include  （复制）

  sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64  	（复制）

  sudo chmod a+r /usr/local/cuda/include/cudnn.h /usr/local/cuda/lib64/libcudnn*   （加权限）

	CUDA Environment Path    添加CUDA的环境变量



终端中执行

  export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/extras/CUPTI/lib64"

  export CUDA_HOME=/usr/local/cuda

  export PATH="$CUDA_HOME/bin:$PATH"  






## 1.2、源码安装Tensorflow

> 
> 


> 
![](https://img-blog.csdn.net/20161013213355870)






~~apt-get install git ~~

	Clone the TensorFlow repository    克隆Tensorflow

  git clone https://github.com/tensorflow/tensorflow

### 1.2.1、 Install Bazel   安装Bazel

**		Install JDK 8   安装JDK8**

  sudo add-apt-repository ppa:webupd8team/java   （添加源）

  sudo apt-get update   （更新）

  sudo apt-get install oracle-java8-installer   （安装）

**		 Add Bazel distribution URI as a package source (one time setup) （将Bazel的URL添加为源）**

  echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list  

  curl https://bazel.io/bazel-release.pub.gpg | sudo apt-key add -

**		Update and install Bazel  更新并下载Bazel**

  sudo apt-get update && sudo apt-get install bazel

  sudo apt-get upgrade bazel

### 1.2.2、 Install other dependencies   安装其他依赖

  sudo apt-get install python-numpy swig python-dev python-wheel python-pip

**	Configure the installation  配置 （这里注意configure后面的提示，提示已经给出）**

  ./configure（在 clone 下来的 tensorflow目录执行）

Please specify the location of python. [Default is /usr/bin/python]:

Do you wish to build TensorFlow with Google Cloud Platform support? [y/N] N

No Google Cloud Platform support will be enabled for TensorFlow

Do you wish to build TensorFlow with GPU support? [y/N] y

GPU support will be enabled for TensorFlow

Please specify which gcc nvcc should use as the host compiler. [Default is /usr/bin/gcc]:

Please specify the Cuda SDK version you want to use, e.g. 7.0. [Leave empty to use system default]: 8.0  （此处根据实际情况修改）

Please specify the location where CUDA 7.5 toolkit is installed. Refer to README.md for more details. [Default is /usr/local/cuda]:

Please specify the cuDNN version you want to use. [Leave empty to use system default]: 5  （此处根据实际情况修改）

Please specify the location where cuDNN 5 library is installed. Refer to README.md for more details. [Default is /usr/local/cuda]:

Please specify a list of comma-separated Cuda compute capabilities you want to build with.

You can find the compute capability of your device at: https://developer.nvidia.com/cuda-gpus.

Please note that each additional compute capability significantly increases your build time and binary size.

[Default is: "3.5,5.2"]:3.0（这个值视机器配置而定，配置越高值越高，参考  [https://developer.nvidia.com/cuda-gpus#collapse4](https://developer.nvidia.com/cuda-gpus%23collapse4)，而3.0通用）

Setting up Cuda include

Setting up Cuda lib

Setting up Cuda bin

Setting up Cuda nvvm

Setting up CUPTI include

Setting up CUPTI lib64

Configuration finished

### 1.2.3、 Create the pip package and install    创建pip包并且安装

  bazel build -c opt //tensorflow/tools/pip_package:build_pip_package  （笔者用公司网提示error，翻墙后问题解决）

  bazel build -c opt --config=cuda //tensorflow/tools/pip_package:build_pip_package

  bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg（笔者安装过程中出现ImportError：No module named setuptools，解决办法：apt-get install python-pip，安装python-pip就行了）

  sudo pip install /tmp/tensorflow_pkg/tensorflow-0.11.0rc0-py2-none-any.whl

### 1.2.4、 Setting up TensorFlow for Development    编译设置Tensorflow

  bazel build -c opt //tensorflow/tools/pip_package:build_pip_package

  bazel build -c opt --config=cuda //tensorflow/tools/pip_package:build_pip_package

  mkdir _python_build

  cd _python_build

  ln -s ../bazel-bin/tensorflow/tools/pip_package/build_pip_package.runfiles/org_tensorflow/* .

  ln -s ../tensorflow/tools/pip_package/* .

  python setup.py develop

### 1.2.5、 Train your first TensorFlow neural net model    测试Tensorflow

  cd tensorflow/models/image/mnist

  export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/extras/CUPTI/lib64"

  export CUDA_HOME=/usr/local/cuda （这里重新添加环境变量是因为笔者安装过程中提示未能找到CUDA）

  python convolutional.py（笔者这里出现AttributeError：type object 'NewBase' has no attribute 'is_abstract'问题，解决办法：sudo pip install six --upgrade -- target="/usr/lib/python2.7/dist-packages"）






## 1.3、安装OpenCV

	Download OpenCV    下载OpenCV

浏览器打开  http://opencv.org/

右侧下载Linux版本的OpenCV

  cd到下载目录

  unzip opencv-2.4.13.zip

  cd opencv-2.4.13

  mkdir release  

  sudo apt-get install build-essential cmake libgtk2.0-dev pkg-config python-dev python-numpy libavcodec-dev libavformat-dev libswscale-dev  

  cd release  

  cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..  

  sudo make install  






## 1.4、下载FlappyBird

	Download DeepLearningFlappyBird     下载FlappyBird

  git clone --recursive https://github.com/yenchenlin/DeepLearningFlappyBird






## 1.5、 安装pygame

 	Install  pygame   	安装pygame

  wget http://www.pygame.org/ftp/pygame-1.9.1release.tar.gz   下载pygame

  sudo apt-get install libsdl1.2-dev     	（SDL安装）

  sudo apt-get install libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev libsdl-net1.2-dev libsdl-sge-dev libsdl-sound1.2-dev libportmidi-dev libsmpeg-dev  		（安装其他依赖包）

  cd pygame-1.9.1release

  python config.py

	run deep_q_network.py

  python  deep_q_network.py        运行deep_q_network.py  （笔者这里报错：AttributeError:'module' object has no attribute 'stack',解决办法：sudo apt-get install python-numpy python-scipy python-matplotlib ipython ipython-notebook python-pandas python-sympy python-nose）

	git clone git://github.com/numpy/numpy.git numpy  （笔者这里运行了一下cd numpy；python setup.py install，发现报错缺少cython于是执行后面的命令：apt-get install cython）

		cd numpy

		python setup.py install









## 1.6、GPU版的执行程序

全部安装完后，再次执行

  python  deep_q_network.py

    画面卡住等待一下，GPU、CUDA在运行需要时间..

    稍等片刻，奇迹出现了，飞鸟开始自动飞、自动上下跳跃、自动穿过障碍，要知道纯人工手动玩很难坚持9s！

![](https://img-blog.csdn.net/20161013222046173)


    静态图片可能看不出啥效果，视频见这：[http://weibo.com/1580904460/EcxQh6em0](http://weibo.com/1580904460/EcxQh6em0)。

    至此，这个曾虐遍全球无数人的游戏，就这样在我们手里，利用深度学习自动玩转了！无不体现深度学习的神奇与魅力。




## 第二部分、CPU版教程

有GPU则按照上述第一部分来，那没GPU咋办呢？没GPU有CPU也能跑。


以下操作均使用root账户

## 2.1、 安装Tensorflow

源码安装方式

**Clone the TensorFlow repository    克隆Tensorflow**

  git clone https://github.com/tensorflow/tensorflow

### 2.1.1、 Install Bazel   安装Bazel

**		Install JDK 8   安装JDK8**

  sudo add-apt-repository ppa:webupd8team/java   （添加源）

  sudo apt-get update   （更新）

  sudo apt-get install oracle-java8-installer   （安装）

**		 Add Bazel distribution URI as a package source (one time setup) （将Bazel的URL添加为源）**

  echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list  

  curl https://bazel.io/bazel-release.pub.gpg | sudo apt-key add -

**		Update and install Bazel  更新并下载Bazel**

  sudo apt-get update && sudo apt-get install bazel

  sudo apt-get upgrade bazel

**2.1.2、 Install other dependencies   安装其他依赖**

  sudo apt-get install python-numpy swig python-dev python-wheel python-pip



**	Configure the installation  配置 （这里注意configure后面的提示，提示已经给出）**

  ./configure  [在 clone 下来的 tensorflow

目录执行]

标注颜色的字体需要手动输入

Please specify the location of python. [Default is /usr/bin/python]:

Do you wish to build TensorFlow with Google Cloud Platform support? [y/N] N

No Google Cloud Platform support will be enabled for TensorFlow

Do you wish to build TensorFlow with GPU support? [y/N] N

Configuration finished

### 2.1.3、 Create the pip package and install    创建pip包并且安装

  bazel build -c opt //tensorflow/tools/pip_package:build_pip_package  （笔者用公司网提示error，翻墙后问题解决）

  bazel build -c opt --config=cuda //tensorflow/tools/pip_package:build_pip_package

  bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg（笔者安装过程中出现ImportError：No module named setuptools，解决办法：apt-get install python-pip，安装python-pip就行了。）

 sudo pip install /…/…/tensorflow-0.11.0rc0-py27-none-any.whl

上述tensorflow...whl文件 下载地址  [https://github.com/tensorflow/tensorflow#installation](https://github.com/tensorflow/tensorflow#installation)

选择 Linux_CPU-only中的对应版本

### 2.1.4、 Setting up TensorFlow for Development    编译设置Tensorflow

  bazel build -c opt //tensorflow/tools/pip_package:build_pip_package

  mkdir _python_build

  cd _python_build

  ln -s ../bazel-bin/tensorflow/tools/pip_package/build_pip_package.runfiles/org_tensorflow/* .

  ln -s ../tensorflow/tools/pip_package/* .

  python setup.py develop

**2.1.5、 Train your first TensorFlow neural net model    测试Tensorflow**

  cd tensorflow/models/image/mnist

  python convolutional.py（笔者这里出现AttributeError：type object 'NewBase' has no attribute 'is_abstract'问题，解决办法：sudo pip install six --upgrade --

target="/usr/lib/python2.7/dist-packages"）







## 2.2、安装OpenCV

	Download OpenCV    下载OpenCV

浏览器打开  http://opencv.org/

右侧下载Linux版本的OpenCV

  cd到下载目录

  unzip opencv-2.4.13.zip

  cd opencv-2.4.13

  mkdir release  

  sudo apt-get install build-essential cmake libgtk2.0-dev pkg-config python-dev python-numpy libavcodec-dev libavformat-dev libswscale-dev  

  cd release  

  cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..  

  sudo make install  





## 2.3、 下载FlappyBird

	Download DeepLearningFlappyBird     下载FlappyBird

  git clone --recursive https://github.com/yenchenlin/DeepLearningFlappyBird





## 2.4、 安装pygame

 	Install  pygame   	安装pygame

  wget http://www.pygame.org/ftp/pygame-1.9.1release.tar.gz   下载pygame

  sudo apt-get install libsdl1.2-dev     	（SDL安装）

  sudo apt-get install libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev libsdl-net1.2-dev libsdl-sge-dev libsdl-sound1.2-dev libportmidi-dev libsmpeg-dev  		（安装其他依赖包）

  cd pygame-1.9.1release

  python config.py

	run deep_q_network.py

  python  deep_q_network.py        运行deep_q_network.py  （笔者这里报错：AttributeError:'module' object has no attribute 'stack',解决办法：

	sudo apt-get install python-numpy python-scipy python-matplotlib ipython ipython-notebook python-pandas  

python-sympy python-nose

	git clone git://github.com/numpy/numpy.git numpy  （笔者这里运行了一下cd numpy；python setup.py install，发现报

错缺少cython于是执行后面的命令）

		apt-get install cython

		cd numpy



		python setup.py install）







## 2.6、CPU版的执行程序

全部安装完后，再次执行

  python  deep_q_network.py

画面不会卡顿，cpu版本会立刻出现结果（笔者这里第一次时候没有执行root，会报错，同学们切记执行上条命令前一定要root了!）

PS:按照FlappyBird安装教程安装，可以直接应用在”学梵高作画”的教程里，两者可以通用！



## 参考文献


- [教你从头到尾利用DL学梵高作画：GTX 1070 cuda 8.0 tensorflow gpu版](http://blog.csdn.net/v_july_v/article/details/52658965)

- 5月深度学习班学员小蔡同学写的简易教程：[用MAC DQN玩Flappy Bird](https://ask.julyedu.com/question/7409)

- [https://github.com/yenchenlin/DeepLearningFlappyBird](https://github.com/yenchenlin/DeepLearningFlappyBird)










## 后记

    七月在线开发/市场团队，二零一六年十月十三日。



