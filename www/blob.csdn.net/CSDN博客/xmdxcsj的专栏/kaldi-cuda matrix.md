# kaldi-cuda matrix - xmdxcsj的专栏 - CSDN博客





2016年03月05日 11:11:09[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3346








如果想要将矩阵运算放到GPU上面跑的话，需要使用CuMatrix和CuVector，以替换CPU版的Matrix和Vector类。因为CuMatrix类使用的是GPU的存储空间，所以不能将CuMatrix和Matrix混合使用。kaldi也会判断某个运算需要使用GPU或者CPU，需要人为编程指定。

1.kaldi编译

默认的，如果系统路径中包含nvcc，那么编译选项HAVE_CUDA=1，Makefile里面关于GPU编译的选项将会打开。如果机器上面有GPU。

如果不想开启GPU编译，那么可以增加configure的选项-use-cuda=no。

如果kaldi没有找到cuda的安装路径，但是想要使用的话，可以增加选项-cudatk-dir=/opt/cuda-4.2。

如果想查看编译的kaldi是否使用了cuda，可以grepkaldi.mk，如果发现nvcc，说明使用了cuda。

2.程序执行

程序运行是否使用了GPU，可以根据程序日志查看，例如：


LOG (nnet-train-simple:IsComputeExclusive():cu-device.cc:229) CUDA setup operating under Compute Exclusive Mode.

LOG (nnet-train-simple:FinalizeActiveGpu():cu-device.cc:194) The active GPU is [1]: Tesla K10.G2.8GB  \

    free:3519M, used:64M, total:3583M, free/total:0.982121 version 3.0





如果二进制程序需要使用GPU的话，可以使用命令选项-use-gpu（如果程序没有该选项，那么说明不支持gpu运算）控制 。后面可跟的选项包括：

"yes":使用GPU，如果机器没有GPU，程序将无法运行

"no":不使用GPU

"optional":如果机器有GPU，才使用GPU

"wait":使用GPU，如果GPU正在被其他程序使用，那么将会等待空闲GPU的出现

3.GPU配置

kaldi只支持英伟达系列的GPU，该类GPU有多种计算模式，包括default/process exclusive/thread exclusive。kaldi一般使用exclusive模式。可以通过以下命令查找GPU的运行模式：


# nvidia-smi  --query | grep 'Compute Mode'

    Compute Mode                    : Exclusive_Thread


可以通过nvidia-smi -c 1 命令来设置正确的模式，最好将设置写入到机器的启动脚本中。

为了避免频繁调用NVidia提供的malloc和free函数，kaldi对之前释放的内存做了cache。如果GPU是non-exclusive模式的话，可能导致分配空间失败，可以通过CuDevice::Instantiate().DisableCaching()调用关闭cache。




==========================================================================================================================

kaldi文档以后章节的翻译文档参考：**https://www.gitbook.com/book/shiweipku/chinese-doc-of-kaldi/details**









