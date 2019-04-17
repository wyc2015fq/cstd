# Kinect开发中OpenNI的配置与安装 - 战斗蜗牛的专栏 - CSDN博客





2012年08月07日 12:55:52[vbskj](https://me.csdn.net/vbskj)阅读数：2270标签：[include																[library																[linker																[测试																[防火墙																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[开源软件](https://blog.csdn.net/vbskj/article/category/1190943)





注意：下载的OpenNI、NITE都是unstable版本的




Kinect作为新一代的体感设备，小斤就不多介绍咯，微软日前也发布了相应的官方SDK，用以开发Kinect在PC上的应用。但就目前的情况而言，微软的KinectSDK只能在Win 7平台上使用，因此小斤选择了OpenNI作为Kinect开发的第一块踏脚石，它是目前已被广泛使用和认可的开源Kinect开发库。



**准备**

首先大家需要下载三样东西：

第一，OpenNI ，小斤在这里下载：[](http://www.openni.org/downloadfiles/opennimodules/openni-binaries/21-stable)[http://www.openni.org/Downloads/OpenNIModules.aspx](http://www.openni.org/Downloads/OpenNIModules.aspx)，选择OpenNI
 Binaries，注意要下载Development Edition，这样才有Sample可以测试查看。此外，源码也可以在GitHub上看到：[https://github.com/OpenNI/OpenNI](https://github.com/OpenNI/OpenNI)

第二，SensorKinect，它就是Kinect的驱动了，可以在[https://github.com/avin2/SensorKinect](https://github.com/avin2/SensorKinect)的Bin中找到。

第三，NITE，这是PrimeSense 提供的中间件，可以分析Kinect读取的资料，输出人体动作等等。目前在[http://www.openni.org/Downloads/OpenNIModules.aspx](http://www.openni.org/Downloads/OpenNIModules.aspx)，选择OpenNI
 Compliant Middleware Binaries就能够下载了。



**安装**

【1】下载完毕后，先把Kinect放在一边，安装OpenNI。

(小斤安装OpenNI后，安装SensorKinect过程中，会弹出”Please install OpenNI version….”之类的错误，重启后再一次点击SensorKinect安装就解决了。有同学提示说，安装OpenNI的Windows用户一定要用管理员身份跑安装程序才行，没有的话有一部分文件无法写入，会导致之后的错误。)



【2】安装SensorKinect驱动，装完，把Kinect插上，此时系统就会自动开始搜索驱动了，如果没有搜索到，那么需要指定驱动目录了。假设SensorKinect默认安装在默认路径，指定驱动目录C:\ProgramFiles\Prime Sense\Sensor\Driver。

驱动安装成功的话，在设备管理器中，会出现:

*![](http://hi.csdn.net/attachment/201108/17/0_1313581206VXB8.gif)*



【3】安装NITE，装完后，由于NITE定义的320*240分辨率与SensorKinect的640*480不同，这边有个关键步骤，假设安装在默认路径：

从"c:\Program Files\PrimeSense\Sensor\SampleXMLs\NITE\Data\" 里复制所有XML文件到 "c:\ProgramFiles\Prime
 Sense\NITE\Data\" 覆盖

　　从"c:\ProgramFiles\Prime Sense\Sensor\SampleXMLs\OPENNI\Data" 里复制所有XML文件到 "c:\Program Files\OpenNI\Data" 覆盖




此外，NITE是需要序列号的，但其官网免费提供了一个：0KOIk2JeIBYClPWVnMoRKn5cdY4=

我们只要将"c:\Program Files\Prime Sense\NITE\Data\"目录下所有配置文件，把Licenses部分替换即可：

<Licenses> 

  　　　<License vendor="PrimeSense"key="0KOIk2JeIBYClPWVnMoRKn5cdY4="/> 

　　</Licenses> 



**测试**

【OpenNI】

如果安装在默认路径下，测试程序可以在C:\Program Files\OpenNI\Samples\Bin\Release下找到，这里小斤使用的是NiViewer.exe和NiSimpleViewer.exe，都是显示深度图，如果运行没问题就可以咯。另外，NiUserTracker这个程序也很有趣，可以跟踪你的人体，前提是它捕捉到你人体后，你做出一个标定动作（举起双手）并维持一段时间。

SimpleViewer:

![](http://hi.csdn.net/attachment/201108/17/0_1313581307U2F8.gif)




UserTracker:

![](http://hi.csdn.net/attachment/201108/17/0_1313581344fbCf.gif)





        如果安装一切正常，但这些例子无法显示结果，可以关闭防火墙试试，**某些防火墙的设置会阻拦OpenNI的组件读取摄像头**。





【NITE】

同样的，如果安装在默认路径下，测试程序可以在C:\Program Files\OpenNI\Samples\Bin\Release下找到，小斤玩了下StickFigure，这个程序能在一个空间中显示出骨架。




**VS开发环境**

要使用VS进行开发的话，需要配置include和lib路径。

进入VS，点击菜单Tools，进入Options。

在Projects and Solutions中，选择VC++Directories。

假设OpenNI安装在默认路径下，则include files加入C:\Program Files\OpenNI\Include，Library files加入C:\ProgramFiles\OpenNI\Lib。

这样VS的开发环境就配置好了。如果建立了OpenNI的开发项目，则还需要在项目属性——Linker——Input 中的Additional Denpendencies，加入openNI.lib。

（以上配置在VS2010中有些出入，include和library可能被移到项目属性中配置）



与OpenNI的Sample使用OpenGL不同，小斤之后的教程范例中，会使用OpenCV进行图像的输入输出，以简化程序（如果你看了OpenNI Sample的代码，会发现代码被OpenGL占了一大半）。大家可以在[http://www.opencv.org.cn/](http://www.opencv.org.cn/)上找到安装配置方法，小斤就不赘述了。（当然大家也可以直接无视范例中的OpenCV输出部分，把OpenNI读取的数据直接print出来。）](https://so.csdn.net/so/search/s.do?q=防火墙&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)




