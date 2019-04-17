# intel C++ 编译器11.0 Linux环境配置 - 战斗蜗牛的专栏 - CSDN博客





2013年05月09日 08:54:50[vbskj](https://me.csdn.net/vbskj)阅读数：1798








                
1: 安装

可以在 intel 网站获得免费的 Linux 个人版. 运行 install.sh 脚本即可.



2: 配置

在root用户下，sh intall.sh即可，按照提示一步步的进行安装；

需要注意的是：

1）ICC需要JRE运行环境，建议安装之前确认一下。我是在安装的时候发现没有JRE，安装后，icc的install还是检测不出JRE，忽略，然后继续安装，正常。

2）安装完成后，安装包提供了环境变量配置脚本，我的做法是在用户目录下的.bashrc中加入两句话即可：

source /opt/intel/mkl/11.0/046/tools/environment/mklvars32.sh    (这个是mkl的编程环境，包括mkl的include和lib，用于fft等基本数学运算)       

source /opt/intel/mkl/11.0/046/bin/icclvars32.sh    (写博客的时候，没在服务器跟前，具体路径忘记了，反正是和tools一级的目录)


3）配置完成后，每次启动就会自动加载icc的环境变量了。

      为了验证是否成功，键入“which icc”如果能打印出icc的path，成功！！




3:编程

使用MKL需要的环境如下(第2步中已经成功加载)



编译C/C++程序所需要的库（路径）：

-I/opt/intel/mkl/include



链接C/C++程序所需要的库（路径）：

-L/opt/intel/mkl/lib/intel64 -L/opt/intel/lib/intel64


！！！注意！！！，mkl需要的lib库如下（不过不需要多线程，-liomp5和。。。Thread两个库，换成。。。sequential即可）：

库名称：

-lmkl_intel_lp64

-lmkl_intel_thread

-liomp5

-lmkl_core

-lpthread



