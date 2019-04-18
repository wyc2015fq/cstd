# 在VC中使用MATLAB C/C++函数库 - 深之JohnChen的专栏 - CSDN博客

2005年09月01日 15:27:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2329


$$$$$$$$$$$$在VC中使用MATLAB C/C++函数库$$$$$$$$$$$$$$
MATLAB广泛应用于线性代数、自动控制理论、数理统计、数字信号处理、时间序列分析、动态系统仿真等领域。因此如果在VC中对MATLAB进行调用将大大减少编程的工作量、保证程序的准确性，并且继承了VC++强大的功能，提高开发效率，本文将介绍了在VC中使用MATLAB函数库的具体步骤:

在VC中建立一个项目后:

 1．设置：

添加路径：
在project-settings-c/C++-category:-preprocessor-Additional include directories:栏目中添加：
D:/MATLAB6P1/EXTERN/INCLUDE，
D:/MATLAB6P1/EXTERN/INCLUDE/CPP，
D:/MATLAB6P1/EXTERN/LIB lib
在project-settings-c/C++-category:-preprocessor-preprocessor definitions: 栏目中添加：
MSVC,MSWIND 
在project-settings-Link-Category-Input-Ignore libraries:栏目中添加：msvcrt.lib

 2. 添加头文件：(假设matlab文件为x.m) 

#include "matlab.hpp"
#include x.hpp //如果有从.m编译成.cpp的文件x.hpp 和x.cpp;

在x.cpp中添加#include "StdAfx.h"

 3．在matlab中的编译方法：
用mcc命令转化为c/c++代码，命令行参数如下：
生成c文件： mcc –t –L C x.m
生成c++文件： mcc –t –L Cpp x.m
生成动态链接库函数： mcc –t –W lib:y –T link.lib x.m(y为生成的链接文件名)

 4．导入链接库： #pragma comment (lib, "libmatlb.lib")
#pragma comment (lib, "libmx.lib")
#pragma comment (lib, "libmatpm.lib")    // c++ maths library

 5．参考代码如下: double d[] = { 1, 2, 3, 4 };
mwArray A(2, 2, d);

mwArray arrVal,arrVec;
arrVec = eig(&arrVal,A);
double* a = new double[4];
double* b = new double[4];

for (int i=0; i < 4; i++)
{
  a[i] = *(mxGetPr(arrVal.GetData())+i); //正序
  b[i] = *(mxGetPr(arrVec.GetData())+i);//逆序
}
$$$$$$$$$使用DIRECTX SDK $$$$$$$$$
在Include files中
E:/DXSDK/INCLUDE
E:/DXSDK/SAMPLES/C++/COMMON
E:/DXSDK/SAMPLES/C++/DIRECTSHOW/BASECLASSES
在Libarary files
E:/DXSDK/LIB
E:/DXSDK/SAMPLES/C++/DIRECTSHOW/BASECLASSES/DEBUG
E:/DXSDK/SAMPLES/C++/DIRECTSHOW/BASECLASSES/RELEASE

