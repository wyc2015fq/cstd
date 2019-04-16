# Win7上编译OpenCV3.2与扩展模块 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年04月01日 17:07:50[gloomyfish](https://me.csdn.net/jia20003)阅读数：3364








### 一：准备

前几天在写代码的时候发现周围有人都换到了OpenCV3.2上面去啦，我当时就把OpenCV3.1包给删啦，立马下载OpenCV3.2，下载地址在这 

里:[http://opencv.org/opencv-3-2.html](http://opencv.org/opencv-3-2.html)。 

选择Windows自解压的那个连接点击进去即可下载OpenCV3.2的Windows版本。下载以后解压缩到指定目录即可。
扩展模块下载地址 
[https://github.com/opencv/opencv_contrib](https://github.com/opencv/opencv_contrib)

同样下载好之后先解压缩到指定目录即可。
然后就可以下载CMake了，我用的是CMake3.7.2这个版本，貌似不是最新版本，大家可以下载最新版本。下载安装好了之后就可以开始编译了。

### 二：编译OpenCV3.2

说一下机器环境 Win764位 + VS2015。 

首先要打开CMake GUI然后设置好源代码路径与编译路径，显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20170401165720127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击【configure】之后会弹出对话框，让你选择编译的位数与版本，记得一定选择VS2015 + Win64的，(当然要根据实际情况来),选择好啦显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20170401165803261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击【Finish】就会开始配置编译，如果一切顺利就会看到如下界面 
![这里写图片描述](https://img-blog.csdn.net/20170401165842934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在一堆红色区域的Name列对应有一个是设置扩展模块路径的额，看下图的蓝色矩形框，选择设置好即可。 
![这里写图片描述](https://img-blog.csdn.net/20170401170004800?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
设置好OpenCV扩展模块的路径之后再次点击【configure】按钮。如果一切正常结束之后再点击【generate】按钮。结束之后CMake就编译好啦。显然如下图： 
![这里写图片描述](https://img-blog.csdn.net/20170401170046894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后在设置的编译路径上D:\opencv3.2\opencv\newbuild目录里面会有个OpenCV.sln文件，双击就可以在VS2015中打开找到 

-CMakeTargets->INSTALL右键在弹出的菜单中选择生成即可 
![这里写图片描述](https://img-blog.csdn.net/20170401170134550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果一切OK，就会生成install目录，以我本机的目录结构为例 

D:\opencv3.2\opencv\newbuild\install 

点击进去，配置好VS2015之后即可使用。 

上面说的是理论应该这样顺利，但是实际不是这么回事情。
### 几个要注意坑

**坑一：**
CMake的时候报Download错误与MD5文件校验错误，愿意是因为OpenCV3.2中会去下载谷歌的protobuff和TensorFlow相关第三方程序，结果下载不了，网络就挂啦！原因是OpenCV3.2集成了深度学习框架TensorFlow相关的接口。

**坑二**
找不到ippicvmt.lib，我也不知道怎么会事情，OpenCV3.2居然没有它编译放到install/lib里面去,而是在第三方的那个目录下面lib里面，所以我手动copy了放到一起。

**坑三**
我在正常配置之后，在Tracking模块中发现selectROI函数居然用不了，搞了半天，是因为tracking.hpp居然没有把它作为头文件包含进来，这个跟OpenCV3.2的教程上有点不一致。所以我手动包含了一下。最终我的OpenCV3.2+VS2015的配置搞好啦！

### 测试程序运行结果：

![这里写图片描述](https://img-blog.csdn.net/20170401170209316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)












