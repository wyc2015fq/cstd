# LInux CUDA环境配置 - 战斗蜗牛的专栏 - CSDN博客





2013年12月21日 11:02:26[vbskj](https://me.csdn.net/vbskj)阅读数：12855







1.安装toolkit
(1)cd /home/CUDA_train/software/cuda4.1
(2)./cudatoolkit_4.1.28_linux_64_rhel6.x.run
指定安装目录
(3)配置cuda toolkit环境变量
(a)vim ~/.bashrc
(b)添加如下行，用于添加cuda bin的路径到环境变量PATH
export PATH=$PATH:/usr/local/cuda/bin
(c)添加如下行，用于添加cuda/lib和cuda/lib64路径到环境变量LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib:/usr/local/cuda/lib64
(d)使配置生效
source ~/.bashrc

2.安装cuda sdk
(1)cd /home/CUDA_train/software/cuda4.1
(2)./gpucomputingsdk_4.1.28_linux.run
安装过程中需要指定安装目录，输入cuda toolkit的安装目录
(3)编译SDK示例程序
假定sdk安装目录为$(CUDA_SDK)=~/NVIDIA_GPU_Computing_SDK/
(a)编译SDK例程之前，先生成静态库
进入$(CUDA_SDK)/shared目录，执行make，生成libshrutil_x86_64.a
进入$(CUDA_SDK)/c/common，执行make，生成libcutil_x86_64.a
(b)编译SDK示例程序
示例程序源码在$(CUDA_SDK)/C/src目录下，编译成功的可执行文件在$(CUDA_SDK)/C/bin目录下。
以deviceQuery为例：
进入$(CUDA_SDK)/C/src/deviceQuery目录，执行make编译
进入$(CUDA_SDK)/C/bin/linux/release目录，执行./deviceQuery运行
要想重新编译，可执行make clean; make


