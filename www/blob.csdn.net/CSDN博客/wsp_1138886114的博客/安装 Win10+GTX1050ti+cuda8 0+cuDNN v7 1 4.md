# 安装 Win10+GTX1050ti+cuda8.0+cuDNN v7.1.4 - wsp_1138886114的博客 - CSDN博客





2018年12月06日 22:16:44[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：567











### 文章目录
- [一、前言](#_1)
- [本机配置](#_6)
- [二、安装准备](#_10)
- [三、安装](#_30)
- [四、验证CUDA安装是否成功：](#CUDA_47)




### 一、前言

大家好，之前使用 tensorflow 和 pytorch 是都是用 CPU 速度太慢，后来买了好机器，想着这下可以使用GPU来训练模型了吧！

马上网上查资料安装，准备开干，我发现我的GPU型号是 GTX1050Ti，去确认自己的显卡是否支持cuda，[https://developer.nvidia.com/cuda-gpus](https://developer.nvidia.com/cuda-gpus) 准备下载cuda(版本8.0.61)，发现，就是没有 GTX1050Ti 这一项，我的天，管他呢，我又查资料看其他码农的情况，有说不行的，有说更低版本的都支持，没事安装完照样用，于是我正式开干！！！

##### 本机配置
- win10 64bit
- i5-8300H
- GTX1050Ti

### 二、安装准备

我之前使用的运行环境是：**Anaconda |  tensorflow ==1.2 | pytorch ==1.0**

关于如何安装Anaconda 我这里我不再赘述
- Anaconda下载地址：

官网: [https://www.anaconda.com/download/](https://www.anaconda.com/download/)

清华镜像站 [https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)- 下载cuda8.0

不管显卡是否支持cuda，也可以去查看，[https://developer.nvidia.com/cuda-gpus](https://developer.nvidia.com/cuda-gpus)

下载cuda(版本8.0.61) 官网：[https://developer.nvidia.com/cuda-downloads](https://developer.nvidia.com/cuda-downloads)- 下载 cuDNN [https://developer.nvidia.com/rdp/cudnn-archive](https://developer.nvidia.com/rdp/cudnn-archive)

你只需要下载相应的版本号即可
- 安装VS2015社区版

vs2015官方中文社区版完整版/3.74G
**安装选项**仅选择2015 更新3及 **c++** 库

这个东西有点大，我将其安装在默认目录C盘下，大约10G。若安装完整约75G
[https://msdn.microsoft.com/zh-cn/visual-studio-community-vs.aspx](https://msdn.microsoft.com/zh-cn/visual-studio-community-vs.aspx)

或者 [https://www.cr173.com/soft/124533.html](https://www.cr173.com/soft/124533.html)
### 三、安装

安装 cuda8.0 前确保安装好 VS2015。
- 
双击下载好的安装文件，选择提取目录（该目录为解压临时目录，可以自定义

我自定义为 D:\CUDA）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206211746975.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

软件会进行兼容性检查，报告我的机器不兼容，忽略警告，我选择默认精简安装，结果，安装失败。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206213156983.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- 
我又选择默认自定义安装，结果，安装失败。难道我买了玩具，只能用来玩游戏？

经过查找，有人遇到了与我一样的情况，通过自定义的方式**取消Visual Studio Intergration**将该项勾去掉，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206214003990.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- 
先别急着点下一步，把在解压临时目录 D:\CUDAVisualStudioIntegration”这个CUDAVisualStudioIntegration文件夹拷到其他位置（桌面）保存，但是如果你把NVIDIA安装程序关闭了，那么这个默认文件就会消失的。

此文件下有以下文件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120621520793.png)- 
点击下一步，这次会安装完毕，只是显示被去掉勾的那一项没被安装。

- 
此时，将"`CUDAVisualStudioIntegration\extras\visual_studio_integration\MSBuildExtensions`"下的所有文件直接拷贝到“C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140\BuildCustomizations”文件夹中就可以！


### 四、验证CUDA安装是否成功：
- **打开命令提示符，执行：nvcc -V**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206220314238.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- **利用VS2015编译测试文件**

打开C:\ProgramData\NVIDIA Corporation\CUDA Samples\v8.0，找到对应VS版本的sample，本例中为Samples_vs2015.sln，双击打开：
**选择Release，X64**
**右键1_Utilities，点击build(build)**

成功编译出现图片下方文字：成功5个…
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206220822272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

至此，“C:\ProgramData\NVIDIA Corporation\CUDA Samples\v8.0\bin\win64\Release”文件夹中会出现我们需要的deviceQuery和bandwidthTest。如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206221020862.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- **验证deviceQuery 和 bandwidthTest**

打开命令提示符：定位到 C:\ProgramData\NVIDIA Corporation\CUDA Samples\v8.0\bin\win64\Release目录，分别输入deviceQuery，bandwidthTest并运行，出现如下类似信息便说明CUDA安装成功。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206221400110.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206221412732.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- **环境变量**

在用户变量，Path中添加：C:\ProgramData\NVIDIA GPU Computing Toolkit\v8.0\bin

确认系统变量中：CUDA_PATH和CUDA_PATH_V8_0已经存在
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208184035495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- 解压下载好的 cuDNN版本

你会得到三个文件夹以及其下面的文件：
`bin/cudnn64_5.dll`、`include/cudnn.h`、`lib/x64/cudnn.lib`

将.h .lib 和.dll 三个文件分别拷贝到`C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0`的include, lib/x64, bin 文件夹下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181208184913896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)大功告成！！！

接下来就是安装tensorflow-gpu环境了，

你可以直接在Anaconda根目录直接安装：`conda install tensorflow-gpu`

也可以[创建虚拟环境](https://blog.csdn.net/wsp_1138886114/article/details/80438993)安装:`conda install tensorflow-gpu`

关于使用请看以后的更新。

我以后所有的关于深度学习的模型训练都是使用此环境，不在赘述。
















