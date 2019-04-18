# 【Python学习系列三】Windows下Python第三方常用库安装 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月07日 11:54:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3750
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









                
Python有丰富的第三方库，提供丰富的爬虫、数据分析等功能。



方法一：下载完整包，含编译器和常见库，https://www.continuum.io/downloads/

         针对操作系统环境，下载Anaconda2-4.4.0-Windows-x86_64.exe，对应Python 2.7 version。

         国内镜像https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/下载更快；



方法二：已安装Python2.7的基础编译器，通过Pip安装所需第三方库。

        安装见：http://blog.csdn.net/fjssharpsword/article/details/72884763

        安装具体库时要注意版本对应以及库之间依赖关系。

        Pip安装有很多可以参考，一般在安装目录C:\Python27\Scripts下已有pip程序，不需单独在安装。

        常用库见：http://www.lfd.uci.edu/~gohlke/pythonlibs/，以安装Numpy为例。

   1)下载numpy-1.11.3+mkl-cp27-cp27m-win_amd64.whl，对应python2.7和win64位环境。如下图：

![](https://img-blog.csdn.net/20170607115119175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



   2)cmd窗口切换到下载文件存放的目录，执行pip install numpy-1.11.3+mkl-cp27-cp27m-win_amd64.whl即可，如下图：

![](https://img-blog.csdn.net/20170607115139927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   3)Numpy是Python的一个科学计算的库，提供了矩阵运算的功能，验证numpy如下图：


![](https://img-blog.csdn.net/20170607115154426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)：

eclipse+pydev集成开发环境验证如下图

![](https://img-blog.csdn.net/20170607115328913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





为避免一个个安装，还要判断相关版本对应以及库依赖，一般选择方法一。
            


