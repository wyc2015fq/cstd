# Openmp和MKL的一点兼容性问题：VS2010+MKL10.0+OpenMP2.0 - 战斗蜗牛的专栏 - CSDN博客





2014年11月18日 00:22:53[vbskj](https://me.csdn.net/vbskj)阅读数：3764








发现这个问题网上比较少碰到，只有一个帖子提及，但是最终的解决方案也不适用于我们的项目。

现象：当在你的项目中同时使用Intel MKL数学库和打开VS2010的/openmp选项时，会出现以下编译错误：



error LNK2005: _You_must_link_with_exactly_one_OpenMP_library 已经在 VCOMPD.lib(unique.obj) 中定义ibguide.lib(kmp_global.obj)




比较有借鉴意义的帖子如下：

https://software.intel.com/en-us/forums/topic/291009


https://software.intel.com/en-us/articles/how-to-use-intelr-compiler-openmp-compatibility-libraries-on-windows





解决方案：

主要问题还是VS2010中打开openmp开关之后，默认是链接vcomp.lib in Release或者vcompd.lib in Debug，但是MKL的一些数学库，比如FFT进行了部分的多线程加速，使用的是intel自己的OpenMP库，所以，两个omp库之间存在冲突，需要指定使用哪个Omp库。

因此，只需要在VS2010的项目属性中--》依赖的附加库，增加intel的omp库即可解决问题:  libiomp5md.lib(见下面备注，因为VS提供的是动态版本，所以这里采用intel的omp动态版本)




这样当你的代码中遇到omp语法指令时，就会直接链接intel库的omp代码而不会跑到vs的omp库中。




注:



The new OpenMP* Compatibility library
The Intel® Parallel Composer 2011, Intel C++ Composer XE for Windows, Intel Visual Fortran Composer XE for Windows, and the Intel C++ or Visual Fortran Compiler 10.1 or 11.x for Windows include the following new OpenMP* compatibility libraries:
- libiomp5md.dll, libiomp5md.lib: dynamic performance library; options "/MD /Qopenmp-link:dynamic"
- libiomp5mt.lib: static performance library; options "/MT /Qopenmp-link:static"
- libiompprof5md.dll, libiompprof5md.lib: dynamic profile library; options "/MD /Qopenmp-profile"
- libiompprof5mt.lib: static profile library; options "/MT /Qopenmp-profile"
- libiompstubs5md.dll, libiompstubs5md.lib: dynamic stub library; options "/MD /Qopenmp-stubs"
- libiompstubs5mt.lib: static stub library; options "/MT /Qopenmp-stubs"


Visual C++ 2005* provides only dynamic OpenMP libraries: vcomp.lib, vcomp.dll 

Visual C++ 2008* provides only dynamic OpenMP libraries: vcomp90.lib, vcomp90.dll 

Visual C++ 2010* provides only dynamic OpenMP libraries: vcomp100.lib, vcomp100.dll - this is supported by Intel Parallel Composer 2011, Intel C++ Composer XE for Windows and Intel Visual Fortran Composer for Windows only. 










