# Gtk与Cuda混合编程 - Koma Hub - CSDN博客
2017年09月07日 19:34:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：243
个人分类：[C/C++																[CUDA																[GTK](https://blog.csdn.net/Rong_Toa/article/category/7156203)](https://blog.csdn.net/Rong_Toa/article/category/7156216)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
                
大家都知道，不同的编译器有不同的-L和-I，而在编译过程中要实现两种编译器的混合使用，利用Makefile是一个很好地解决方案，比如之前在mpi与cuda混合编程中讨论的那样，基于图形界面的GTK编程也只是在gcc 编译的基础之上加上gtk的库函数（`pkg-config --cflags --libs gtk -2.0`）而已，所以通过本人的实际操作，用Makefile解决也是可以的。这里需要注意的是：GTK与GPU kernels传递过程中，不要传递浮点型，否则会报错。
首先你需要编好一个图形界面的GTK程序，在这个程序中你会输入cuda代码需要的一些参数变量信息。
我的GTK程序名为：4_model_gtk_gpu.c
我的gpu程序名为：GPU.cu
那么，Makefile文件如下：
```
CUDA_INSTALL_PATH = /usr/local/cuda-7.5
MPI_INSTALL_PATH = /usr
NVCC = $(CUDA_INSTALL_PATH)/bin/nvcc
GCC = $(MPI_INSTALL_PATH)/bin/gcc
LDFLAGS = -L$(CUDA_INSTALL_PATH)/lib64
LIB = -lcudart -lcurand
LGTK = `pkg-config --cflags --libs gtk -2.0`
CFILES = 4_model_gtk_gpu.c
CUFILES = GPU.cu
OBJECTS = 4_model_gtk_gpu.o GPU.o 
EXECNAME = a
all:
        $(GCC) -c $(CFILES)  $(LGTK)
        $(NVCC) -c $(CUFILES)
        $(GCC) -o $(EXECNAME) $(LDFLAGS) $(LIB) $(LGTK) $(OBJECTS)
clean:
        rm -f *.o $(EXECNAME)
```
整体上就是先生成.o的文件，再用这个文件作为库进行接下来的编译。最后用./运行。VTI介质的的代码我就不贴了，而GTK的代码在本人博客的GTK里面也已经给了详细的代码，这里也不再贴了，就给出几个图形界面的结果图吧。
下面是图形界面的主界面，目前我只完成了第一个模块的编程，RTM部分还没做。
![](https://img-blog.csdn.net/20170907193513389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击模块按钮后，会跳出模块的具体参数填写窗口，同样的，你可以自己选择文件。
![](https://img-blog.csdn.net/20170907193519712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在终端会显示如下信息：
![](https://img-blog.csdn.net/20170907193526361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
希望接下来在完成RTM的部分时不会遇到太多麻烦！！！！！！！！！！！！
