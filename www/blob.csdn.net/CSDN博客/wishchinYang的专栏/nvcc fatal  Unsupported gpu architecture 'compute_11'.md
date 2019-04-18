# nvcc fatal : Unsupported gpu architecture 'compute_11' - wishchinYang的专栏 - CSDN博客
2017年03月20日 16:44:31[wishchin](https://me.csdn.net/wishchin)阅读数：4610
         使用VS编译OpenCV编译源代码时候，对Cmake生成的工程文件编译，会出现 nvcc fatal : Unsupported gpu architecture 'compute_11'  问题。原因是CUDA7.5不支持较为古老的显卡版本，因此1.1,2.0,2.1，之类的显卡选项是多余的。
        需要更改Cmake GUI对工程的配置，去掉对compute_11的支持
1. 打开cmakelist.txt
      CMake的选项中声明 -D CUDA_GENERATION=Kepler  
即添加：
```
if(WIN32 AND NOT MINGW)
  add_definitions(-D_VARIADIC_MAX=10)
endif(WIN32 AND NOT MINGW)
#此处为添加代码
if(1)
add_definitions( -D_CUDA_GENERATION=Maxwell)
```
不能消除这种现象，问题没有得到解决。
参考：[安装caffe的血泪史](http://www.cnblogs.com/fanninnypeom/p/5958264.html)
[cmake 命令命令安装、用法](http://www.cnblogs.com/shine-yr/p/5214764.html)
[cmake常用变量速查手册](http://www.cnblogs.com/xianghang123/p/3556425.html)
[linux版本编译OpenCV可能出现的Compute_11问题](http://stackoverflow.com/questions/28010399/build-opencv-with-cuda-support/29814206)
           对于Maxwell的支持，参考：[Vs13+Win7+OpenCV2.4.13+CUDA7.5支持](http://blog.csdn.net/zinnc/article/details/51652401)
2. 
改正方法：
       把CUDA栏目里面，CUDA_ARCH_BIN 项目里面，3.0前面的都去掉，改成如下所示。
![](https://img-blog.csdn.net/20170320164250906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
之前，
1. 先把CUDA_generation 修改为Auto
![](https://img-blog.csdn.net/20170322142246629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2. 打开 sources\cmake 目录，使用文本编辑器编辑OpenCVDetectCUDA.cmake
找到下面一段
```
if(NOT DEFINED __cuda_arch_bin)
    if(ANDROID)
      set(__cuda_arch_bin "3.2")
      set(__cuda_arch_ptx "")
    else()
      if(${CUDA_VERSION} VERSION_LESS "5.0")
        set(__cuda_arch_bin "1.1 1.2 1.3 2.0 2.1(2.0) 3.0")
      else()
        set(__cuda_arch_bin "1.1 1.2 1.3 2.0 2.1 3.0 3.5")
      endif()
      set(__cuda_arch_ptx "3.0")
    endif()
  endif()
```
把第二个1.1  1.2  1.3  2.0 2.1  去掉，修改为
```
if(NOT DEFINED __cuda_arch_bin)
    if(ANDROID)
      set(__cuda_arch_bin "3.2")
      set(__cuda_arch_ptx "")
    else()
      if(${CUDA_VERSION} VERSION_LESS "5.0")
        set(__cuda_arch_bin "1.1 1.2 1.3 2.0 2.1(2.0) 3.0")
      else()
        set(__cuda_arch_bin "3.0 3.5")
      endif()
      set(__cuda_arch_ptx "3.0")
    endif()
  endif()
```
或者直接修改为使用于本机GTX960 显卡的 5.2，
才能把CUDA_ARCH_BIN 的1.1 1.2 1.3 2.0 2.1 去掉，进而解决 nvcc fatal : Unsupported gpu architecture 'compute_11'  问题。
~~若不能通过编辑选项去掉，应该有其他可行的方法？？？~~
则可以编译通过。
