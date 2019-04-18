# 编译VTK的MFC库 - wishchinYang的专栏 - CSDN博客
2013年11月06日 18:30:10[wishchin](https://me.csdn.net/wishchin)阅读数：2011
原文链接：[http://blog.csdn.net/left_la/article/details/7069708](http://blog.csdn.net/left_la/article/details/7069708)
本人做了少量修改！
Win7 + VS2010 + CMake2.8.6 + VTK5.8
1、准备：
    1)下载安装Vs2010
    2)下载安装CMake2.8.6 （[www.cmake.com](http://www.cmake.com)）
    3)下载VTK相关包：（[www.vtk.org](http://www.vtk.org)）
        1.vtk-5.8.0-win32-x86.exe  （win32安装程序）（还是安装的好！）
        2.vtk-5.8.0.zip  （源码包）
        3.vtkdata-5.8.0.zip  （数据包）
        4.vtkDocHtml-5.8.0.tar.gz  （文档包，可下可不下）
2、为vtk的安装新建个根文件夹，命名为"VTK5.8"，在这个文件夹下新建四个文件夹，分别命名为 VTKSrc、VTKData、VTKDoc、Bin，将之前下载的源码包数据包文档包分别解压到前三个文件夹中。
3、安装vtk-5.8.0-win32-x86.exe，安装到根文件夹 VTK5.8 下。
4、打开CMake（CMake2.8\bin\cmake-gui.exe），
   设置：Source code目录：F:\Dev\SDK\VTK5.8\VTKSrc 
        Build目录：F:\Dev\SDK\VTK5.8\Bin  （目录地址请根据自己电脑目录做更改）
   然后configure，在弹出框中选择VS2010作为我们的编译器，然后Cmake自动描述需要的文件。
   停下后，显示红色列表，接着开始修改cmake选项，手动勾上 
   build_shared_libs、build_examples，然后勾选cmake界面上方的Advanced，进入下一级，这时到下面重新找到vtk_use_guisupport 这一项勾选，再 configure，之后会出现 vtk_use_mfc，选择它勾选，再一直configure，直到红色消失。
   最后Generate，产生相应的KTV.sln 解决方案。
   （
      相关说明：
      1、为什么要勾选vtk_use_mfc？ 很多人把 vtk 安装好了，却无法运行vtk 中与mfc
       混合编程的例子，提示错误都是缺少vtkmfc.lib，问题就出在这个选项，我们必须
       把它设置为 on。
      2、vtk_use_qvtk，这是给QT用的vtk
      3、vtk_data_root: 目的是告诉cmake，vtk 需要的数据都在哪里，我们的数据在
      F:\Dev\SDK\VTK5.8\VTKData
      4、cmake_install_prefix: 这个是指定一个目录把.h .lib .dll等精华放在这里,
      "VTK安装三步曲"里说是为将来装vtk做准备。
      5、build_examples: 就是询问是否编译 vtk 中的例子，初次用VTK，选上便于
      学习。
      6、build_shared_libs: 如果设置为off，就只会生成lib文件，用于开发肯定是
      够了。如果设置为on，将会多生成 dll 文件，这些dll需要拷到windows/system
      目录下。如果用静态库编程，即off时，应用程序可以独立运行，尺寸也比较大，
      如果用动态库编程，即on时，应用程序必须找到动态库才能运行，但尺寸可以做
      得比较小。建议选On, 我试过选OFF,所谓的静态编译，但之后新建一个项目时，
      还是会出现找不到文件的问题。
      7.vtk_use_parallel: 对于想用vtk 做并行计算的朋友，要把这个选上，
      对于只想学学vtk 的，保持默认 off  
     )
5、打开 KTV.sln ，如果使用vs2010，在编译整个解决方案之前，需要注意：
  1)请先找到VTKSrc\GUISupport\MFC\vtkMFCWindow.cpp ，找到类似以下源码并修改数值如下：
> 
**[cpp]**[view plain](http://blog.csdn.net/left_la/article/details/7069708#)[copy](http://blog.csdn.net/left_la/article/details/7069708#)
- #ifndef WINVER
- #define WINVER 0x0501
- #endif
- #ifndef _WIN32_WINNT
- #define _WIN32_WINNT 0x0501 // =_WIN32_WINNT_NT4
- #endif
- #ifndef _WIN32_IE
- #define _WIN32_IE 0x0601 //=_WIN32_IE_IE60SP1
- #endif
  2)如果之前Cmake勾选了 build_examples，工程中含有VKT的examples，仅改上面代码还不够，还需要找到
VTKSrc\Examples\GUI\Win32\vtkMFC\vtkDLG\stdafx.h
VTKSrc\Examples\GUI\Win32\vtkMFC\vtkMDI\stdafx.h
VTKSrc\Examples\GUI\Win32\vtkMFC\vtkSDI\stdafx.h
VTKSrc\Examples\GUI\Win32\SampleMFC\stdafx.h
四个文件，每个文件下找到类似以下源码并修改数值如下：
> 
**[cpp]**[view plain](http://blog.csdn.net/left_la/article/details/7069708#)[copy](http://blog.csdn.net/left_la/article/details/7069708#)
- #ifndef WINVER        
- #define WINVER 0x0501    
- #endif
- 
- #ifndef _WIN32_WINNT    
- #define _WIN32_WINNT 0x0501    
- #endif            
- 
- #if _MSC_VER >= 1300
- #ifndef _WIN32_WINDOWS    
- #define _WIN32_WINDOWS 0x0501 
- #endif
- #endif
- 
- #ifndef _WIN32_IE      
- #define _WIN32_IE 0x0601  
- #endif
  3)如果不进行上述两步操作，在编译项目过程中会出现类似下面错误：
  fatal error C1189: #error :  This file requires _WIN32_WINNT to be #defined at least to 0x0403. Value 0x0501 or higher is recommended.
6、编译整个解决方案。
  如果之前Cmake勾选了 build_examples，工程中含有VKT的examples，在编译过程中，以下四个项目vtkDLG、vtkMDI、vtkSDI、Win32SampleMFC，可能还会出现类似下面的LINK链接错误：
error LNK2019: 无法解析的外部符号 "__declspec(dllimport) public: static class...
称找不到vtkIO.dll
对应解决办法：在每个项目属性的链接器->输入->附加依赖项中添加入vtkIO.lib库，目录为：..\..\..\..\bin\Debug\vtkIO.lib
就此，基本的VKT安装及源码编译结束，编译产生的lib、dll链接库和执行文件都生成在VTK5.8\Bin\bin\Debug文件夹下。
总结：主要是使用到了MFC支持，从中取出MFC LIB文件，取消QT的所有选择项，一次性成功：
![](https://img-blog.csdn.net/20131106183824781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后，打开目标文件夹的方案文件，release 和debug都全部生成一遍，大功告成！
