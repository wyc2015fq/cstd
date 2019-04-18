# 一个虚拟摄像头Filter（Virtual Cam Capture Filter） - 爱写代码的自由人 - CSDN博客





2018年02月25日 14:54:31[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：4494








当我们运行一些多媒体程序或者OpenCV的例子的时候，经常要用摄像头作为输入源，但是并不是每个用户电脑上都装了USB摄像头，那怎么办呢？可以在你系统上安装一个虚拟摄像头的Directshow插件，那么你系统上就多了一个“摄像头”了，但是实际上它不是物理摄像头，只是模拟摄像头设备可以采集图像。我的资源里提供了一个虚拟摄像头的代码，大家可以试用一下，下载地址：

http://download.csdn.net/download/zhoubotong2012/10242646


这个Filter原先的版本是一个老外写的，我在上面做了些修改，主要增加了将文件作为输入源的功能。


压缩包里包含两部分：

Virtual Cam Source Filter工程--VirtualCam

还有配置Filter属性的一个工具--VirtualCamConfig

进入VirtualCam的Bin目录，会看到下面的这些文件：

![](https://img-blog.csdn.net/20180225143544865)


其中VCam.ax就是我们要注册的Release版的Filter，目录下面还有FFmpeg的DLL文件，为什么还用到FFmpeg呢？

因为这个Filter的功能是将一个媒体文件解码后的图像作为视频源的，也就是从摄像头里出来的数据就是文件里的视频。所以，使用Filter时我们需要加载一个文件，通过前面说的VirtualCamConfig工具来设置文件路径：

![](https://img-blog.csdn.net/20180225143930667)


配置工具里的第一个参数是FilePath，即输入的多媒体文件路径；第二个参数RameRate是采集的帧率，即播放视频每秒输出的帧数。上面的对话框中设置文件源为：test4.mp4，FrameRate = 10，就是说每秒从test4.mp4这个文件中读取10帧图像输出，VCam Filter内部会将读到的视频先解码再传给后面的Filter，输出Sample格式为RGB。上面目录里的FFmpeg DLL就是用来读取视频和解码用的。

 另外，说一下这个Filter注册的方法，免得有些读者没有用过Directshow不知道怎么用。

进入命令行模式，通过CD命令进入AX文件所在的目录。

输入注册Filter命令： regsvr32  Vcam.ax

如果执行时出现如下提示：

![](https://img-blog.csdn.net/20180225144745638)


表示可能当前账户的权限不够，尝试切换到管理员权限，然后再运行命令，方法：在Windows开始菜单的附件-》命令行提示符，按右键，在右键菜单中选择“以管理员权限运行”，输入命令就可以成功注册。

 （如果要取消注册这个Filter，输入注销Filter的命令：regsvr32 /u Vcam.ax）

安装后，打开任意一个支持DShow的采集程序，比如Directshow的官方例子AMCap.exe，就可以看到图像了。



