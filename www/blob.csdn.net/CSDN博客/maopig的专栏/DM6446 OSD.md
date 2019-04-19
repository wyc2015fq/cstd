# DM6446 OSD - maopig的专栏 - CSDN博客
2011年12月31日 00:21:14[maopig](https://me.csdn.net/maopig)阅读数：2553标签：[character																[linux																[图形																[buffer																[video																[flash](https://so.csdn.net/so/search/s.do?q=flash&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)
个人分类：[u-boot/驱动开发/linux																[DM368](https://blog.csdn.net/maopig/article/category/919673)](https://blog.csdn.net/maopig/article/category/930963)
## DM6446 OSD
TMS320DM6446支持背景窗颜色，两个视频窗口，两个OSD窗口，一个指针(cursor)窗口。它们以递增的顺序排列：
一个特有的第二个OSD窗口（OSDWIN1）可以用来配置成属性窗口来控制视频窗口和第一个OSD窗口（OSDWIN0）的混合（如透明度）。因为“alpha”混合功能是OSDWIN1最通用的功能，所以以下就主要讨论第一个OSD窗口（OSDWIN0）用来显示OSD图片，以及第二个OSD窗口（OSDWIN1）用作属性窗口来控制混合。
OSD窗口可以配置成接收RGB565或者bitmap数据。一般情况，RGB,BITMAP和原始图像数据格式（raw data format）在技术行业是可以交换的。如果每个像素使用的bit数一样，那么这些格式就一样。在达芬奇处理器上却不是这种情况。它们提供了配置成RGB565 或者 bitmap的选项。当OSD窗口配置成接收bitmap数据，它就使用256条目的颜色查找表（CLUT）。这意味着最大颜色深度是8bit（4，2，1bit的也支持）。当OSD窗口配置成接收RGB565数据，CLUT就不需要的了，因为RGB数据在外部存储器中先转换成YCBCR；RGB565使用16bit/pixel，因此能接收64k颜色。两个窗口都可以同时配置成接收bitmap数据。然而，只有一个OSD窗口可以配置成接收RGB565数据二不能两个都配置。因此，如果第二个OSD窗口用作属性窗口，第一个OSD窗口最好使用RGB565模式，主要因为它可以存取16bits（64k颜色）。
 Linux 有两个重要的驱动结构，块驱动和特征驱动块驱动允许无须存取并能挂载到文件系统中。包括了硬盘驱动，外部RAM，及copmpact flash驱动。Character驱动读取作为流，以FIFO顺序（如音频视频）。所以大家会测出，OSD功能是作为character（视频驱动）驱动。 Linux的另一个的特征是character驱动能以类似的方式存取文件。这意味着你可以open,write,close这些设备跟操作文件一样。从Linux的命令提示下，你可以copy(cp)，display(cat)
 及pipe（>>）数据从一个文件到另一个文件中。这个强大的特征允许把数据放到OSD窗口中而不用写任何的代码。 达芬奇平台提供了访问视频硬件的两个Linux驱动，V4L2视频采集驱动（视频处理前端VPFE ---
davinci_vpfe.c）和FBDev(即Framebuffer设备)显示驱动（视频处理后端VPBE ---- davincifb.c）。
(1)Video for Linux 2（V4L2）是一个标准的，二级Linux视频输入驱动，它修正了第一版的一个bug。
(2)FBDev是一个标准的Linux视频输出驱动，用来映射显示设备帧存。如DaVinci处理器到user空间。
Linux frame buffer display device(ie. FBDev 驱动)包含了OSD特征。帧存驱动提供了显示硬件的概括。它令视频输出硬件（OSD亦如此）作为一个帧存设备。并因此允许软件访问图形硬件就像写入一个缓冲区。跟上面提到的一样，输出设备缓存存取跟访问文件一样，一般定位在 /dev 路径中。在这种情况下，这个路径我们感兴趣的是改变DaVinci OSD 是/dev/fb/0。使用这个路径，我们能修改显示，而只是通过一个定义好的接口。它的操作包括跟文件一样（如open, read,write,close）的操作和设备指定的命令（ioctls
 来查询/设置关于硬件的信息）。
我们应该熟悉下面的命令：
cp – copy (e.g. >cp osd.r16 /dev/fb/0 --copies osd.r16 file to /dev/fb/0 设备)
把以上的几点放到一起，通过写一个简单的命令（>cp osd.r16 /dev/fb/0）就可以把osd.r16的内容复制到OSD帧存中，并能显示。Osd.r16文件从哪里来呢？这个文件包含了现有显示到OSD的内容；因此通过图像编辑器（如GIMP）编辑好图像后，通过运行转换工具(tar.gz 文件中)就可以转换。但是GIMP要创建什么样格式的文件呢？推荐的格式是BMP。
步骤如下：
(1)创建OSD图像通过图像编辑器（如GIMP）并且保存为BMP文件。
(2)运行小程序来转换为RGB565格式。>.bmpToRgb16 mysod.bmp,这个命令就会产生osd.r16文件。
(3)把转换后的文件复制到OSD帧存中。>cp osd.r16 /dev/fb/0
(4)重复以上的几步，直到满意。保存osd.r16文件，它就可以被应用程序加到源代码中了。
需要指出的是属性窗口不应该设置为100%视频（所有都为0），因为这样混合水平不允许OSD图形显示。
