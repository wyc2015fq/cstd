# VTK初始化New返回Null问题 - wishchinYang的专栏 - CSDN博客
2015年05月24日 23:19:06[wishchin](https://me.csdn.net/wishchin)阅读数：1172
原文链接：[http://www.cppblog.com/mythma/archive/2013/08/02/vtk-6-new-null.html](http://www.cppblog.com/mythma/archive/2013/08/02/vtk-6-new-null.html)
        在使用VTK6.0时候，会遇到X::New()返回为null的情况，需要全局初始化一下：
#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
