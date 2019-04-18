# SiftGPU：编译SiftGPU出现问题-无法解析的外部符号 glutInit - wishchinYang的专栏 - CSDN博客
2017年04月05日 10:23:43[wishchin](https://me.csdn.net/wishchin)阅读数：1191
OpenCV出现了ORB特征和SURF的GPU版本，
参考：[opencv上gpu版surf特征点与orb特征点提取及匹配实例](http://blog.csdn.net/ap1005834/article/details/52665359)至于使用什么并行API暂时没有探究。
但没有发现OpenCV-SIFT的GPU使用。需要自行编译，参考：[编译Opencv的GPU，利用CUDA加速](http://blog.csdn.net/lanbing510/article/details/41211697)
SiftGPU的原始库可以编译通过。但不能使用，在使用时引出了一连串96个编译错误。
1.Glew函数错误
```cpp
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewGetShaderInfoLog
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewGetShaderiv
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewGetProgramInfoLog
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewGetProgramiv
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewUseProgram
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewLinkProgram
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewAttachShader
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewCreateProgram
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewCompileShader
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewShaderSource
1>flatten.obj : error LNK2001: 无法解析的外部符号 __imp____glewCreateShader
1>flatten.obj : error LNK2019: 无法解析的外部符号 __imp__glewIsSupporte
```
解决方法：
       更新Glew到2.0版本
2.更新Glew之后，显示对于原始GlobalUtil.obj 工程仍含有未解析glut函数体：
```cpp
1>siftgpu.lib(GlobalUtil.obj) : error LNK2001: 无法解析的外部符号 glutInit
1>siftgpu.lib(GlobalUtil.obj) : error LNK2001: 无法解析的外部符号 glutInitDisplayMode
1>siftgpu.lib(GlobalUtil.obj) : error LNK2001: 无法解析的外部符号 glutInitWindowPosition
1>siftgpu.lib(GlobalUtil.obj) : error LNK2001: 无法解析的外部符号 glutCreateWindow
1>siftgpu.lib(GlobalUtil.obj) : error LNK2001: 无法解析的外部符号 glutDestroyWindow
1>siftgpu.lib(GlobalUtil.obj) : error LNK2001: 无法解析的外部符号 glutGetWindow
1>siftgpu.lib(GlobalUtil.obj) : error LNK2001: 无法解析的外部符号 glutSetWindow
1>siftgpu.lib(GlobalUtil.obj) : error LNK2001: 无法解析的外部符号 glutHideWindow
1>E:\CodeBase\CodeValidSource\SLAM\ORB_SLAM_Wish\Build\Win\ORBSLAM\x64\Release\SlamFrame.exe : fatal error LNK1120: 8 个无法解析的外部命令
```
找不到合适的glut版本，暂时无法解决。
更正方法：
2.1. 去掉原始Sift工程中所有的 #include "GlobalUtil.h" 语句
    此方法不可取！
2.2. 注销掉GlobalUtil.cpp里面所有的GLut函数
结果：
       整个显示界面不能使用，因此也不必使用！Glut问题也成为悬着的问题。
       然而不能使用，在初始化时候出现问题，因此必须解决Glut问题。
解决方法：
       64位版本使用32位版本问题。
~~      32位版的很好找，网上一找一堆，可是64位版的相对比较难找，更有人说glut不支持64位的开发，搞的小菜一度想放弃。就在我快要放弃的时候，一个大神跳出来跟我说，纳尼，谁说没有64bit的？？！！~~
~~      不多说，直接给网址：[http://download.csdn.net/detail/knownall/6799947](http://download.csdn.net/detail/knownall/6799947)这是glut3.7.6，里面包含debug/release，32bit和64bit。~~
~~      当然也可以选择下载源码自己编译： [http://download.csdn.net/detail/knownall/6799967](http://download.csdn.net/detail/knownall/6799967) 自己编译的应该感觉起来比较爽，小菜便是这么做滴。~~
       使用Glut的64位版本。下载地址：[http://download.csdn.net/download/knownall/6799947](http://download.csdn.net/download/knownall/6799947)
