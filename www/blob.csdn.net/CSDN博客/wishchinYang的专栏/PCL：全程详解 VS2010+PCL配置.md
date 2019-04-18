# PCL：全程详解 VS2010+PCL配置 - wishchinYang的专栏 - CSDN博客
2013年08月19日 16:17:26[wishchin](https://me.csdn.net/wishchin)阅读数：12415
        浑浑噩噩半年时间，终于不得不干点和机器人有关的东西。ICRA和IROS推荐的三维图形库—点云库，几乎成了机器人视觉算法的标配。
        参考了几篇文章，最后终于配置成功。
下面是文章地址：
         这个有点看头，累积较多，不过还没仔细体会：[http://blog.csdn.net/chen_nan/article/details/7055651?reload](http://blog.csdn.net/chen_nan/article/details/7055651?reload)
         这个文章较短，不过说出了一小点有用的东西，若是看过介绍论文，可不必看：[http://qing.blog.sina.com.cn/1455173150/56bc2e1e330005pk.html](http://qing.blog.sina.com.cn/1455173150/56bc2e1e330005pk.html)
**（一）：安装软件包和库**
        安装好VS2010后，安装一次性安装包，下载地址：[http://www.pointclouds.org/downloads/windows.html](http://www.pointclouds.org/downloads/windows.html)
### 准备工作：
a)  获取All in one安装包，PCL提供了配置为Visual Studio 2010的 32位和64位、 Visual Studio 2008的32位和64位下的该安装包，该包包含了PCL中所使用全部第三方编译包，除了Qt编译包；
b) 获取All in one包对应的PDB文件包，该包用于后期单步调试时使用；
c)  获取PCL源码包；
d)  安装开发工具Visual Studio 2010或Visual Studio 2008和cmake开发工具，需要cmake版本大于2.8.3，主要考虑到PCL中用到了高版本的一些宏定义，低版本不兼容。
注意：[http://pointclouds.org/downloads/windows.html](http://pointclouds.org/downloads/windows.html)可下载all in one安装包和其对应的PDB文件包，源码包可在[http://pointclouds.org/downloads/](http://pointclouds.org/downloads/)处下载。以上所有包都在参考文献【1】提供的光盘中含有。如果用户安装了RGBD相关设备驱动与中间件，则需要卸载OpenNI相关的驱动、中间件，避免与all
 in one提供的驱动与中间件冲突，如果没有，忽略此项，OpenNI、RGBD相关的内容后续章节IO有介绍。图1所示，为准备工作最终需要的文件。
![](http://www.pclcn.org/upload/images/20121204_115522.jpg)
1.完成准备工作将会有三个包
### 安装：
      安装过程很简单（笔者配置为Visual C++ 2008 Express Edition、Cmake 2.8.5、32位系统），点击安装上一步所准备的文件PCL-1.5.1-AllInOne-msvc2008-win32.exe。运行如图2到8，按照提示默认安装即可，如果需要改变安装路径到非C:盘，后面cmake配置时需要自行设置各个第三方库的路径以及PCL头文件与链接库路径。
![](http://www.pclcn.org/upload/images/20121204_124700.jpg)
2欢迎使用界面与PCL许可界面
![](http://www.pclcn.org/upload/images/20121204_124734.jpg)
3安装选项与安装路径界面
![](http://www.pclcn.org/upload/images/20121204_124808.jpg)
4选择开始菜单界面与安装组建库选择界面
![](http://www.pclcn.org/upload/images/20121204_124849.jpg)
5安装组建界面展开图示
![](http://www.pclcn.org/upload/images/20121204_124926.jpg)
6安装OpenNI界面
![](http://www.pclcn.org/upload/images/20121204_125024.jpg)
7安装Primesense Sensor驱动界面
![](http://www.pclcn.org/upload/images/20121204_125145.jpg)
8完成界面与最终安装好的截图
![](http://www.pclcn.org/upload/images/20121204_125219.jpg)
9第三方库文件夹
![](http://www.pclcn.org/upload/images/20121204_125256.jpg)
10  pdb文件夹中内容
**（二）：编译库文件：**
#       Windows下从预编译包搭建PCL开发环境，转自官方：[http://www.pclcn.org/study/shownews.php?lang=cn&id=33](http://www.pclcn.org/study/shownews.php?lang=cn&id=33)
       建立两个文件，源程序文件project_inliers.cpp和cmake 配置文件CMakeLists.txt内容拷贝链接[http://pointclouds.org/documentation/tutorials/project_inliers.php#project-inliers](http://pointclouds.org/documentation/tutorials/project_inliers.php#project-inliers)中对应的代码及Cmakelists的代码（此处也可以从参考文献【1】中test文件拷贝对应的文件），放在同一文件夹source下，在source同一目录下建立camke-bin文件夹。
![](http://www.pclcn.org/upload/images/20121204_134258.jpg)
11 测试需要建立的文件
打开cmake，设置源文件目录与编译目录，如图12所示。
![](http://www.pclcn.org/upload/images/20121204_134331.jpg)
12设置cmake需要的路径
![](http://www.pclcn.org/upload/images/20121204_134359.jpg)
13 camke中选择对应的编译器
![](http://www.pclcn.org/upload/images/20121204_134529.jpg)
14 配置与生成
选择对应的编译器，如图13所示，笔者选择对应visual studio 9 2008，点击finish，再点击配置与生成，如图14所示，即可得到工程文件，如图15所示。
![](http://www.pclcn.org/upload/images/20121204_134433.jpg)
15生成的工程文件
![](http://www.pclcn.org/upload/images/20121204_134609.jpg)
16编译工程文件
![](http://www.pclcn.org/upload/images/20121204_134647.jpg)
17 测试exe文件运行结果
        到此就完成了PCL在windows上的开发环境搭建，用户可以自行体验官方网站提供的大量实例代码了，如果用户只是使用稳定的PCL版本。
**（三）：搭建开发环境：**
.安装完毕后就测试一下PCL的效果吧，要注意项目对应的包含目录、库目录、库文件（LIB）和运行环境（DLL）。
用[http://www.pointclouds.org/documentation/tutorials/pcl_visualizer.php#pcl-visualizer](#pcl-visualizer)上面的例程试一试，新建工程，粘贴代码……
在包含目录下添加：
C:\Program Files\PCL 1.5.1\3rdParty\Boost\include;
C:\Program Files\PCL 1.5.1\3rdParty\Eigen\include;
C:\Program Files\PCL 1.5.1\3rdParty\Flann\include;
C:\Program Files\PCL 1.5.1\3rdParty\Qhull\include;
C:\Program Files\PCL 1.5.1\3rdParty\VTK\include\vtk-5.8;
C:\Program Files\OpenNI\Include;
C:\Program Files\PCL 1.5.1\include\pcl-1.5;
（对应自己机子上PCL的位置，如果为了CMAKE方便很多会放在根目录下。即C:\PCL）
在库目录下添加：
C:\Program Files\PCL 1.5.1\3rdParty\Boost\lib;
C:\Program Files\PCL 1.5.1\3rdParty\Qhull\lib;
C:\Program Files\PCL 1.5.1\3rdParty\Flann\lib;
C:\Program Files\PCL 1.5.1\3rdParty\VTK\lib\vtk-5.8;
C:\Program Files\PCL 1.5.1\lib;
C:\Program Files\OpenNI\Lib;
在附加依赖项上添加：
 opengl32.lib
pcl_kdtree_debug.lib
pcl_io_debug.lib
pcl_search_debug.lib
pcl_segmentation_debug.lib
pcl_range_image_border_extractor_debug.lib
pcl_apps_debug.lib
pcl_features_debug.lib
pcl_filters_debug.lib
pcl_visualization_debug.lib
pcl_common_debug.lib
flann_cpp_s-gd.lib
libboost_system-vc100-mt-gd-1_47.lib
libboost_filesystem-vc100-mt-gd-1_47.lib
libboost_thread-vc100-mt-gd-1_47.lib
libboost_date_time-vc100-mt-gd-1_47.lib
libboost_iostreams-vc100-mt-gd-1_47.lib
openNI.lib
vtkalglib-gd.lib
vtkCharts-gd.lib
vtkCommon-gd.lib
vtkDICOMParser-gd.lib
vtkexoIIc-gd.lib
vtkexpat-gd.lib
vtkFiltering-gd.lib
vtkfreetype-gd.lib
vtkftgl-gd.lib
vtkGenericFiltering-gd.lib
vtkGeovis-gd.lib
vtkGraphics-gd.lib
vtkhdf5-gd.lib
vtkHybrid-gd.lib
vtkImaging-gd.lib
vtkInfovis-gd.lib
vtkIO-gd.lib
vtkjpeg-gd.lib
vtklibxml2-gd.lib
vtkmetaio-gd.lib
vtkNetCDF-gd.lib
vtkNetCDF_cxx-gd.lib
vtkpng-gd.lib
vtkproj4-gd.lib
vtkRendering-gd.lib
vtksqlite-gd.lib
vtksys-gd.lib
vtktiff-gd.lib
vtkverdict-gd.lib
vtkViews-gd.lib
vtkVolumeRendering-gd.lib
vtkWidgets-gd.lib
vtkzlib-gd.lib
这里要注意PCL引用到的其他库文件一定要加进去，VTK的库很多，而且依赖关系我看得很乱，所以为了方便就全加进去了。
如果没有加完整，很多信息提示错误的，之前忘了加一个什么库，导致很多error LNK2019，还让我重装了很多次…… 
注意库要对应debug和release版，debug后面一般都有-gd或者_debug的，看看就能分出来了。对应错了还会有error LNK：
error LNK2038: mismatch detected for '_ITERATOR_DEBUG_LEVEL': value '2' doesn't match value '0' in  XXXX.......
这时应该可以看到成功生成的信息了：
         别着急运行，会发现还没做完的。
    我们还要把对应的dll加到环境中，可以直接把用到的那些dll放到应用程序搜索到的位置，如cpp对应的目录。（=_=！好笨的方法，不过我就是这样做的……）或者是把dll的目录设置到环境中去（在安装ALL
 IN ONE 时已经自动添加）
**Tips**:
       对于pcl_range_image_border_extractor_debug.lib无法找到问题，先删除包含之，编译通过.
