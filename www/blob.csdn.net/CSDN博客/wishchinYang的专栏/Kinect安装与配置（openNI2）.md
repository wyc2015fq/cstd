# Kinect安装与配置（openNI2） - wishchinYang的专栏 - CSDN博客
2013年08月19日 15:12:34[wishchin](https://me.csdn.net/wishchin)阅读数：4991
         原文链接：[http://blog.csdn.net/chenxin_130/article/details/8580636](http://blog.csdn.net/chenxin_130/article/details/8580636)
**简介**
最近OpenNI2的推出，小斤也要多给博客除除草了，并在闲暇之余做一些范例程序。从试用几天的经历看，OpenNI2带来了较大的变化：
【1】程序接口作了较大的更新。具体更新可以查看《OpenNI Migration Guide》，http://www.openni.org/openni-migration-guide/，由于没有向下兼容，因此如果要升级OpenNI2的话，很多代码需要改动了。
【2】驱动的变化。使用过OpenNI1.X的童鞋应该了解过SensorKinect这个驱动，但在OpenNI2的环境下，目前Kinect只能使用微软官方的Kinect for Windows SDK来驱动了，料想其它品牌的摄像头，也应该有各自不同的驱动方式吧。使用官方驱动的好处是安装比较方便，Kinect for Windows
 SDK中也自带了一些开发工具可以测试使用，坏处也显而易见，由于Kinect for Windows SDK只支持Win7以上的操作系统，让那些想在Linux上玩转Kinect的童鞋失望而归。
【3】第三方中间件的支持。在OpenNI1.X的世界里，NiTE一统天下的局面一去不复返了。现在的OpenNI2只负责操作硬件，提供应用数据，与中间件保持独立，在官网上，除了NiTE2之外，你还能找到其它的中间件来使用。
简介就讲到这，让我们开始安装之旅吧！
![](https://img-my.csdn.net/uploads/201302/14/1360825574_6529.png)
**安装驱动与OpenNI2**
首先确保操作系统是Win7及以上，到Kinect for Windows SDK官网下载安装包：http://www.microsoft.com/en-us/kinectforwindows/develop/overview.aspx。
      小斤在安装过程没有遇到任何问题，安装完毕后，插上Kinect就会发现灯亮可驱动。
      打开开始菜单中的Kinect for Windows Developer Toolkit，可以找一些Samples来运行，检查是否安装正确，像Face Tracking Visualization都是很有意思的范例。
![](https://img-my.csdn.net/uploads/201302/14/1360826072_9467.jpg)
      接着，我们来到http://www.openni.org/下载最新的OpenNI2 SDK进行安装。安装完毕后，可以运行开始菜单的OpenNI2/NiViewer等示例程序进行检查。
**VS开发环境**
VS开发环境的配置与OpenNI1.X类似，还是配置include与library。
      小斤安装的是32位OpenNI2，安装目录位C:\OpenNI2\，在OpenNI2安装完毕后，安装程序自动添加了以下环境变量：（注意，安装64位的话，变量名带有64后缀，配置也相应需要变化）
      OPENNI2_INCLUDE=C:\OpenNI2\Include\
      OPENNI2_LIB=C:\OpenNI2\Lib\
      OPENNI2_REDIST=C:\OpenNI2\Redist\
      新建VS C++项目后，右击进入项目属性，进行以下配置：
【1】Configuration Properties，C/C++，Gerenal，在Additiaonal Include Directories项中添加$(OPENNI2_INCLUDE)。
【2】Configuration Properties，Linker，Gerenal，在Additiaonal Library Directories项中添加$(OPENNI2_LIB)。
【3】Configuration Properties，Linker，Input，在Additiaonal Dependencies项中添加$(OPENNI2_LIB)。
最后需要注意的是，大家还需要拷贝\OpenNI2\Redist目录下的所有文件到VS的工作目录（如果不通过VS启动，则拷贝到程序执行目录）才可以正确运行，其中包含了OpenNI2启动所需的动态链接库等内容，详见以下解释(Copied
 From OpenNI Document)：
*     Copy all the files from OpenNI's redist directory (see environment variable "$(OPENNI2_REDIST)" or "$(OPENNI2_REDIST64)") to your working directory. (The defaults are C:\Program Files\OpenNI2\Redist or C:\Program Files (x86)\OpenNI2\Redist). Be aware that when you run from command line, the working directory is the directory where the executable can be found, and where you run from Visual Studio the default directory is where the project file (.vcproj, .vcxproj) can be found.*
----------------------------------
作者：小斤（陈忻）
新浪围脖：@小斤陈
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
我是华丽的分割线
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
对于我的修改：OpenNI2.lib
       上面的标注红色的有些小失误：Configuration Properties，Linker，Input，在Additiaonal Dependencies项中添加OpenNI2.lib（原本是$(OPENNI2_LIB)）。
**    若不修改，会出现 无法打开.obj的错误，fatalerror。**
**对于：[http://blog.csdn.net/chenxin_130/article/details/8580706](http://blog.csdn.net/chenxin_130/article/details/8580706) 教程七 顺利成功，就不**
