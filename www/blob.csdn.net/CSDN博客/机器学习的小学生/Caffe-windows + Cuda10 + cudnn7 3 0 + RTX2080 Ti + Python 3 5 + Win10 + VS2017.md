
# Caffe-windows + Cuda10 + cudnn7.3.0 + RTX2080 Ti + Python 3.5 + Win10 + VS2017 - 机器学习的小学生 - CSDN博客


2018年11月30日 09:20:50[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：1378


caffe_windows :[https://github.com/BVLC/caffe/tree/windows](https://github.com/BVLC/caffe/tree/windows)
VS2017:[VS2017 professional](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Professional&rel=15)
Cuda10: [cuda_10.0.130_411.31_win10.exe] 【大小2.1G，好像自带显卡驱动？】([https://developer.nvidia.com/compute/cuda/10.0/Prod/local_installers/cuda_10.0.130_411.31_win10](https://developer.nvidia.com/compute/cuda/10.0/Prod/local_installers/cuda_10.0.130_411.31_win10))
cudnn7.3.0: cudnn-10.0-windows10-x64-v7.3.0.29.zip
RTX2080Ti: Turing 架构
Python3.5: python3.5.4
Win10: Win10 版本号 1803. 刚安装的Win10系统版本号是1703，好像无法安装Nvidia显卡驱动（不记得还是其他原因），因此需要将系统更新升级为1803。
Cmake: cmake-3.13.0-win64-x64
Matlab:  Matlab2016b
\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
**后遗症：**：win10电脑会死机，caffe 经常会不定时的出现如下的运行时错误（有时候能正确的完成运行，有时候就会在某次迭代中断）：
> it: 15869  loss1: 0.3009911775588989  loss2:  0.000463328417390585

> it: 15870  loss1: 0.2962459325790405  loss2:  0.0004235844593495131

> it: 15871  loss1: 0.3099851906299591  loss2:  0.0009147725999355317

> it: 15872  loss1: 0.29593050479888916  loss2:  0.00031767517793923616

> it: 15873  loss1: 0.2969663143157959  loss2:  0.00037221238017082214

> it: 15874  loss1: 0.29857179522514343  loss2:  0.0003736394457519055

> it: 15875  loss1: 0.3033006191253662  loss2:  0.0006987718865275384

> it: 15876  loss1: 0.3066413104534149  loss2:  0.0004248021170496941

> F1130 11:37:46.790082 11012 math_functions.cu:79] Check failed: error == cudaSuccess (4 vs. 0)  unspecified launch failure

> *** Check failure stack trace: ***

> \#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#

> 2018/12/02 更新：
> :

> 卸载cuda10所有相关程序（我是将之前装的英伟达的所有产品都卸载掉），然后重新装上2080TI对应的显卡驱动程序，然后装上cuda 9.2。

> cuda9.2: cuda_9.2.148_win10.exe

> cudnn7.3.1: cudnn-9.2-windows10-x64-v7.3.1.20.zip

> 或者cudnn7.1.4: cudnn-9.2-windows10-x64-v7.1.zip

> （考虑到tensorflow：[tensorflow_gpu-1.9.0-cp36-cp36m-win_amd64.whl 下载地址：
> [https://github.com/fo40225/tensorflow-windows-wheel](https://github.com/fo40225/tensorflow-windows-wheel)
> ], 我采用的cudnn7.1.4）

> 即将cuda10 换成了 cuda9.2重新编译了caffe,观察是否会出现“Check failed: error == cudaSuccess (4 vs. 0)  unspecified launch failure” 错误。

> 运行了一天，没有再报上面的错误！

> 2018/12/05 更新：

> Caffe 运行正常！

> \#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#

> 需要修改的6个文件
> ：

> build_win.cmd

> Cuda.cmake

> WindowsDownloadPrebuiltDependencies.cmake

> OpenCVConfig.cmake

> nvcc.hpp

> cudnn.hpp

> 路径分别为
> ：

> E:\caffe-windows\scripts\build_win.cmd

> E:\caffe-windows\cmake\Cuda.cmake

> E:\caffe windows\cmake\WindowsDownloadPrebuiltDependencies.cmake

> E:\caffe-windows\scripts\build\libraries\OpenCVConfig.cmake

> E:\caffe-windows\scripts\build\libraries\include\boost 1_61\boost\config\compiler\nvcc.hpp

> E:\caffe-windows\include\caffe\util\cudnn.hpp

> 注意：

> 通过上面的路径可以看出，OpenCVConfig.cmake 和 nvcc.hpp 文件是在执行build_win之后生成的，因此需要在执行完build_win后，进行修改（为了解决build_win执行时的错误）。

> 修改内容分别如下
> ：

> 修改内容1：build_win.cmd
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018113010295437.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018113010300592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=,size_16,color_FFFFFF,t_70)
> 修改内容2：Cuda.cmake
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181130103332828.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181130103343539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=,size_16,color_FFFFFF,t_70)
> 修改内容3：WindowsDownloadPrebuiltDependencies.cmake
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181130103647282.png)
> 修改内容4：OpenCVConfig.cmake
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181130104111446.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=,size_16,color_FFFFFF,t_70)
> 修改内容5：nvcc.hpp
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181130104317980.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=,size_16,color_FFFFFF,t_70)
> 修改内容6：cudnn.hpp
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018113010445625.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=,size_16,color_FFFFFF,t_70)
> 为了方便正确复制，6个文件的下载地址：

> 链接：
> [https://pan.baidu.com/s/1iV99eLYTSnCganF57XZ-xg](https://pan.baidu.com/s/1iV99eLYTSnCganF57XZ-xg)

> 提取码：3yx4

> 注释：

> 文件中的行号可能与下载的caffe_window原版不严格一致（修改导致的）。

> 在修改过程中，请认真检查核对。

> 上述过程可能有遗漏的地方，请参考下面的4个文献。

> 修改内容2中 gpu_archs: 50 52 60 61 70 75 表示 Turing 兼容的计算能力，详情参考文献2.

> 百度云盘下载的WindowsDownloadPrebuiltDependencies.cmake 中，47或者48行被修改了，请修改为原样。

> 参考文献：

> 1.
> [https://binvec.me/archives/build_caffe_with_vs2017_in_windows](https://binvec.me/archives/build_caffe_with_vs2017_in_windows)

> 2.
> [https://docs.nvidia.com/cuda/turing-compatibility-guide/index.html](https://docs.nvidia.com/cuda/turing-compatibility-guide/index.html)
> [Turing兼容性]

> 3.
> [https://blog.csdn.net/qq_20226441/article/details/82788799](https://blog.csdn.net/qq_20226441/article/details/82788799)

> 4.
> [https://blog.csdn.net/mao_hui_fei/article/details/80326464](https://blog.csdn.net/mao_hui_fei/article/details/80326464)
> [cudnn版本问题】


