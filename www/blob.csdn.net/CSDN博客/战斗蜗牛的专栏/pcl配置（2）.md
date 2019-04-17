# pcl配置（2） - 战斗蜗牛的专栏 - CSDN博客





2012年05月11日 10:19:07[vbskj](https://me.csdn.net/vbskj)阅读数：4427









### [菜鸟编译pcl](http://blog.csdn.net/halfwet/article/details/6927682)


2011-11-02 11:06727人阅读[评论](http://blog.csdn.net/halfwet/article/details/6927682#comments)(11)[收藏](http://blog.csdn.net/halfwet/article/details/6927682)[举报](http://blog.csdn.net/halfwet/article/details/6927682#report)


Point cloud library是一个开源、跨平台的点云处理库，pcl框架涵盖了当前最先进的点云处理相关算法，涉及滤波、特征估计、表面重建、注册、模型拟合、分割等。Pcl基于BSD协议，可免费用于商业和研究应用，适用于Linux、MacOS、Windows、Android平台。

官方网站为：[www.pointclouds.org](http://www.pointclouds.org/)

这里发布了一些视频demo：[http://www.youtube.com/user/pointclouds](http://www.youtube.com/user/pointclouds)

相关教程、说明文件：[http://www.pointclouds.org/documentation/](http://www.pointclouds.org/documentation/)。



闲话少叙，下面进入本文主题：如何在windows平台上编译pcl。

官方编译教程在此：[http://www.pointclouds.org/documentation/tutorials/compiling_pcl_windows_3rdparty_installers.php#compiling-pcl-windows-3rdparty-installers](http://www.pointclouds.org/documentation/tutorials/compiling_pcl_windows_3rdparty_installers.php#compiling-pcl-windows-3rdparty-installers)，可参照此网页逐步操作。

不喜欢编译的童鞋也可到此下载已经编译好的安装文件包：[http://www.pointclouds.org/downloads/windows.html](http://www.pointclouds.org/downloads/windows.html)。

下边给出笔者自行编译的过程：

**(1)    平台需求：**Windows 7 (x86 or x64)，Visual Studio 2010(必须是2010版本！！)，cmake 2.8以上版本，Microsoft Kinect (作为点云采集设备，不是必须的)，主机配置要求不高，在i3笔记本上亦可运行。

**(2)    外部链接库：**
- **Boost version 1.46.1**
- **Eigen version 3.0.0**
- **CMinpack version 1.1.3**
- **FLANN version 1.6.9**
- **Visualization ToolKit (VTK) version 5.6.1 （非必须）**
- **QHULL version 2011.1 （非必须）**
- **OpenNI version 1.1.0.41 and patched Sensor Module version 5.0.1.32 （非必须，用kinect的话必须）**
- ** GTest version >= 1.6.0 （非必须）**

以上链接库安装文件均可在[http://www.pointclouds.org/downloads/windows.html](http://www.pointclouds.org/downloads/windows.html)，处下载。pcl开发者网站上还有一处下载链接列表，更加全面：[http://dev.pointclouds.org/projects/pcl/files](http://dev.pointclouds.org/projects/pcl/files)。上述文件下载好了直接双击安装即可，鉴于我们是菜鸟，建议全部安装到默认目录下，以免cmake找不到~

**(3)    下载pcl源码：**

笔者安装的是1.2版本，目前最新版本为1.3。都可以在这里下载到：**[http://dev.pointclouds.org/projects/pcl/files](http://dev.pointclouds.org/projects/pcl/files)。**

**(4)    cmake配置：**

首先选择生成visual studio 10 的工程。如果把外部库全部安装到默认目录下了的话，剩下就简单了，cmake应该可以全部找到，如果没找到请参照[http://www.pointclouds.org/documentation/tutorials/compiling_pcl_windows_3rdparty_installers.php#compiling-pcl-windows-3rdparty-installers](http://www.pointclouds.org/documentation/tutorials/compiling_pcl_windows_3rdparty_installers.php#compiling-pcl-windows-3rdparty-installers)。完事点配置、生成就OK了。

**(5)    编译pcl：**

当我们成功生成pcl的解决方案之后，就可以开始编译了。

接下来，经过漫长的等待，能不能编译成功就是看人品的事了。。

笔者试过4台计算机，其中只有2台能够成功编译，其他两台都是因为cmake宏与vs的各种冲突导致编译失败。

如果编译失败，可以只编译pcl_开头的几个工程，可以生成pcl的链接库(dll 和 lib)，这就足够了。

**(6)    使用pcl：**

如果读者有幸编译成功了整个解决方案，那就可以运行其自带的几个例子了，其中大部分都是用kinect做采集设备的。

包括：openni_viewer(查看kinect采集的点云)，openni_passthrough(为kinect采集的点云做一个带通)，openni_3d_convex_hull(找凸包的)，openni_ii_normal_estimation(估计法线)，openni_planar_segmentation(平面分割)等等，还有一些pcd_viewer(查看pcd文件，即点云数据point cloud data)等不用kinect的。

如果没编译成功，我们也可以自己写cmake来进行配置，下面给出一个cmake的例子，仅供参考：



cmake_minimum_required(VERSION 2.8 FATAL_ERROR)



# 解决方案名

project(pcl_tutorial)



# 包含目录

include_directories("C:/Program Files/Boost/include"

"C:/Program Files/Eigen 3.0.0/include/eigen3"

"C:/Program Files/flann 1.6.9/include"

"C:/Program Files/OpenNI/Include"

"C:/Program Files/PCL/include/pcl-1.2"

"C:/Program Files/VTK/include/vtk-5.6")



# 附加库目录

link_directories("E:/pcl/pcl-project/lib"

"C:/Program Files/Boost/lib/"

"C:/Program Files/OpenNI/Lib/"

"C:/Program Files/flann 1.6.9/lib"

"C:/Program Files/VTK/lib/vtk-5.6")



# pcl库 PCL_LIBRARIES

set(PCL_LIBRARIES pcl_common.lib

pcl_io.lib

pcl_filters.lib

pcl_kdtree.lib

pcl_range_image.lib

pcl_visualization.lib)



# boost库 BOOST_LIBRARIES

set(BOOST_LIBRARIES libboost_system-vc100-mt-1_46_1.lib

libboost_filesystem-vc100-mt-1_46_1.lib

libboost_thread-vc100-mt-1_46_1.lib

libboost_date_time-vc100-mt-1_46_1.lib)



# vtk库 VTK_LIBRARIES

set(VTK_LIBRARIES vtkWidgets.lib

vtkHybrid.lib

vtkRendering.lib

vtkGraphics.lib

vtkverdict.lib

vtkImaging.lib

vtkIO.lib

vtkFiltering.lib

vtkCommon.lib

vtkDICOMParser.lib

vtkNetCDF_cxx.lib

vtkmetaio.lib

vtksqlite.lib

vtkpng.lib

vtktiff.lib

vtkzlib.lib

vtkjpeg.lib

vtkexpat.lib

vtksys.lib

vtkftgl.lib

vtkfreetype.lib

vtkexoIIc.lib

vtkNetCDF.lib

opengl32.lib)



# openni库 OPENNI_LIBRARIES

set(OPENNI_LIBRARIES openNI.lib)



# flann库 FLANN_LIBRARIES

set(FLANN_LIBRARIES flann_cpp_s.lib)



# 加入项目

# add_executable (项目名 cpp文件相对路径)

# target_link_libraries (项目名 库文件)



 # openni_viewer 项目，用来看kinect获得的三维数据

add_executable (openni_viewer src/openni_viewer.cpp)

target_link_libraries (openni_viewer ${BOOST_LIBRARIES}

${FLANN_LIBRARIES}

${OPENNI_LIBRARIES}

${PCL_LIBRARIES}

${VTK_LIBRARIES})



# pcd_write 项目，用来写pcd文件

add_executable (pcd_write src/pcd_write.cpp)

target_link_libraries (pcd_write ${BOOST_LIBRARIES}

${FLANN_LIBRARIES}

${PCL_LIBRARIES})



# pcd_viewer 项目，用来查看pcd文件

add_executable (pcd_viewer src/pcd_viewer.cpp)

target_link_libraries (pcd_viewer ${BOOST_LIBRARIES}

${FLANN_LIBRARIES}

${PCL_LIBRARIES}

${VTK_LIBRARIES})







至此，相信读者已经能在自己的电脑上看到kinect采集到的点云数据了！

Pcl是一个功能非常强悍的工具库，笔者对其的了解也仅是皮毛。Pcl的官网上提供了大量的tutorial源代码（在这里[http://www.pointclouds.org/media/iros2011.html](http://www.pointclouds.org/media/iros2011.html)）。读者也可以到它的开发者论坛上和国外的玩家一起交流：[http://www.pcl-users.org/](http://www.pcl-users.org/)。






