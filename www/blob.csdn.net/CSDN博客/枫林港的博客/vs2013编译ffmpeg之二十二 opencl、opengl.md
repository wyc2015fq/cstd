# vs2013编译ffmpeg之二十二 opencl、opengl - 枫林港的博客 - CSDN博客





2018年07月09日 23:27:44[枫林港](https://me.csdn.net/u013601132)阅读数：206








# opencl

对应ffmpeg configure选项–enable-opencl。 
[https://www.khronos.org/opencl/](https://www.khronos.org/opencl/)

[http://blog.csdn.net/leonwei/article/details/8880012](http://blog.csdn.net/leonwei/article/details/8880012)

[https://github.com/KhronosGroup/OpenCL-Headers](https://github.com/KhronosGroup/OpenCL-Headers)

通用开源并行计算标准。下一代的图形和并行计算的标准是vulkan，[https://www.khronos.org/vulkan/](https://www.khronos.org/vulkan/)。

确定windows上的显卡类型： 
![这里写图片描述](https://img-blog.csdn.net/20180706233901703?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

针对不同的公司的显卡，其开发包是不一样的，下面分别来描述。
## NVIDIA

这个地址有如何搭建OpenCL环境：[http://kode-stuff.blogspot.com/2012/11/setting-up-opencl-in-visual-studio_1.html](http://kode-stuff.blogspot.com/2012/11/setting-up-opencl-in-visual-studio_1.html)

开发包是CUDA，下载地址：

[https://developer.nvidia.com/cuda-downloads](https://developer.nvidia.com/cuda-downloads)

选择版本cuda_7.5.18_windows_64.exe。安装后的头文件、库文件路径分别是：

C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.5\include

C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.5\lib\Win32

64位的库在lib/x64下。

cuda_7.5.18_windows_64.exe安装的时候，就算系统里面没有NVIDIA显卡，也可以安装的。这个版本的CUDA能识别VS2013及以前的版本，VS2015识别不了，安装的时候会检查VS版本。

由于头文件和库文件路径里面有空格，ffmpeg-3.0的configure怎么弄也通不过，主要问题是是configure里面用eval对变量进行替换，compat\windows\mslink是个脚本，也会对传进去的参数进行转换，搞了2天始终无法将最终执行的cl、link命令中的带空格路径用”“括起来。最后只能将头文件、库文件目录放到不带空格的目录下。

configure里面–enable-opencl检测的是CL/cl.h，用的是路径../../contribute/MSVC/CUDA_NVIDIA/include下面的，这个下面的文件是cuda_7.5.18_windows_64.exe安装后从安装目录拷贝过来的。

ffmpeg-3.3打开–enable-cuda后，configure的时候报下面的错误：

> 
ffconf.FvfVVefI.o : error LNK2019: 无法解析的外部符号 _cuInit，该符号在函数 _main 中被引用


将configure里面的cuda规则：
`enabled cuda              && { check_lib cuda.h cuInit –lcuda`
改成：
`enabled cuda              && { check_lib2 cuda.h cuInit -lcuda`
即将check_lib改成`check_lib2`。 

这个问题的根本原因，cuda.h里面的接口都有CUDAAPI前缀，其定义如下：

```
#ifdef _WIN32
#define CUDAAPI __stdcall
#else
#define CUDAAPI
#endif
```

用MSVC cl编译器的时候_WIN32是有定义的，而cuda windows版本的库在编译的时候，cuda.h里面接口也是带了__stdcall前缀，接口形式为00000000 T _cuInit@4，函数名后面有个@4，表示函数的参数长度为4字节。 

从config.log里面可以看到，check_lib检测cuda所用的代码如下：

```
extern int cuInit();
int main(void){ cuInit(); }
```

代码里面没有include cuda.h，所以这段代码所调用的接口没有加__stdcall前缀，编译器认为cuInit的符号为_cuInit，按_cuInit在cuda.lib里面搜索符号当然找不到了。 

解决此问题的办法就是将check_lib改成check_lib2，让生成的代码包含cuda.h，生成的检测代码为：

```
#include <cuda.h>
#include <stdint.h>
long check_cuInit(void) { return (long) cuInit; }
int main(void) { int ret = 0;
ret |= ((intptr_t)check_cuInit) & 0xFFFF;
return ret; }
```

–enable-cuvid也有类似的问题，解决的办法也是一样。

## AMD

AMD的没有做验证。先下载开发包，目前用的版本是3.0：

[http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/](http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/)

## Intel

opencl_runtime_16.1.1_x64_setup.msi，安装目录C:\Program Files (x86)\Common Files\Intel\OpenCL，下面只有库文件，包括32位和64位的。

# Opengl

对应ffmpeg configure选项–enable-opengl。

ffmpeg-3.0打开–enable-opengl，编译的时候报下面的错误，解决的办法是直接将/MinGW/include下的GL目录拷贝到ffmpeg-3.0/inc/下面。

> 
libavdevice/opengl_enc.c(43) : fatal error C1083: 无法打开包括文件: “GL/glext.h”: No such file or directory


opengl的库和相关头文件应该是MSVC2013自带的。




