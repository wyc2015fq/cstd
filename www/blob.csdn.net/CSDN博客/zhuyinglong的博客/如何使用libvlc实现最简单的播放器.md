# 如何使用libvlc实现最简单的播放器 - zhuyinglong的博客 - CSDN博客





2017年09月26日 10:08:22[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：3917








这篇文章介绍如何使用libvlc库实现一个简单的播放器，因为基于libvlc库相比于使用ffmpeg+SDL方式实现一个播放器更为容易，先从简单入手。




一、libvlc库下载


为了方便可直接使用编译好的libvlc库，项目所需文件及位置如下：

1、动态链接库*.dll：安装目录下的libvlc.dll，libvlccore.dll以及plugins目录（较大）下的所有文件。

2、静态链接库*.lib：安装目录/sdk/lib

3、头文件*.h：安装目录/sdk/include

注意：

vlc之前的版本下载播放器安装完在安装目录就有完整的sdk，可以直接进行开发，但是后来没有了，其实可以在这里下载[http://download.videolan.org/pub/videolan/vlc/](http://download.videolan.org/pub/videolan/vlc/)多种版本。本人下载的是vlc-2.2.6-win32.7z（目前最新版本），解压后就可以看到sdk。也可以直接使用下文所给链接下载该项目，拷贝所需文件使用。




二、主要接口说明


使用libvlc实现一个播放器十分容易，本项目使用到的主要函数说明及流程如下：

第一步：

libvlc_new() //用于初始化一个libvlc的实例，返回创建的实例若当发生错误时返回NULL

第二步：

libvlc_media_new_location()//使用一个给定的媒体资源路径来建立一个libvlc_media对象,此函数返回新建的对象或NULL.

libvlc_media_new_path()//使用本地文件系统路径来建立一个libvlc_media对象,此函数返回新建的对象或NULL.

根据自己需要播放的资源路径选择其中一个函数建立libvlc_media

第三步：

libvlc_media_player_new_from_media()//由libvlc_media创建media_player，用于播放

libvlc_media_release()//libvlc_media使用完毕，释放libvlc_media，

第四步：

libvlc_media_player_play()//播放media_player

第五步：

libvlc_media_player_stop()//停止media_player

第六步：

libvlc_media_player_release()//释放media_player

第七步：

libvlc_release()//销毁一个libvlc的实例

关于函数的详细释义，强烈推荐直接查看接口头文件中的注释，那样理解会更加准确。





三、项目配置

1.将上述提到的静态链接库与头文件放入win32控制台程序工程cpp文件所在的目录，如下图所示：

![](https://img-blog.csdn.net/20170926102701611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意：

lib目录中只需包含libvlc.lib和libvlccore.lib两个文件。


2、将上述提到的dll库及plugins目录放入win32控制台程序工程编译生成exe文件所在的目录，如下图所示：

![](https://img-blog.csdn.net/20170926103129203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3、项目属性配置如下图：

![](https://img-blog.csdn.net/20170926103308053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意：

该配置只针对Debug版本，若需编译release版本，还需针对release版本进行配置。




四、源码

该代码是根据雷神相关代码改写完成，感谢雷神。


configs.h




```cpp
#ifndef _CONFIGS_H_
#define _CONFIGS_H_

extern "C"
{
#include "vlc/vlc.h"
};
#pragma comment(lib,"libvlc.lib")
#pragma comment(lib,"libvlccore.lib")

#endif
```
libvlc_player.cpp





```cpp
#include <iostream>
#include <windows.h>
#include "configs.h"
using namespace std;
/**
 *本程序是根据雷神最简单的基于libVLC的视频播放器改写。
 *仅供学习使用。
 *编译平台WIN32
 */
int main()
{
	libvlc_instance_t * inst = 0;
	libvlc_media_player_t *mp = 0;
	libvlc_media_t *m = 0;

	libvlc_time_t length = 0;
	int width = 0;
	int height = 0;
	int wait_time = 5000;

	/* Load the VLC engine */
	inst = libvlc_new(0, NULL);

	//Create a new item
	//播放网络地址。参数内容为自己在本地搭建的点播的rtmp服务器点播地址，大家在调试程序时可以改为其他可用的网络播放地址。
	//m = libvlc_media_new_location (inst, "rtmp://192.168.154.203:1935/vod/sample.mp4");
	//播放本地视频文件。test.mp4存放在cpp文件所在的目录。
	m = libvlc_media_new_path(inst, "test.mp4");

	/**
	 *显示屏幕内容。
	 *因为要显示的内容包括播放窗口，若播放窗口显示在桌面上，播放器会不停嵌套显示。大家可以尝试看下效果。
	 */
	//m = libvlc_media_new_location (inst, "screen://");

	/* Create a media player playing environement */
	mp = libvlc_media_player_new_from_media(m);

	/* No need to keep the media now */
	libvlc_media_release(m);
	// play the media_player
	libvlc_media_player_play(mp);
	//wait until the tracks are created；在播放wait_time之后显示以下三个内容。
	Sleep(wait_time);
	length = libvlc_media_player_get_length(mp);
	width = libvlc_video_get_width(mp);
	height = libvlc_video_get_height(mp);
	cout << "Stream Duration:" << length / 1000 <<endl;
	cout << "Resolution:" << width << "*" << height << endl;
	//等待（总时长-wait_time），这样可以保证视频刚好播放到结尾。
	Sleep((DWORD)(length - wait_time));

	// Stop playing
	libvlc_media_player_stop(mp);

	// Free the media_player
	libvlc_media_player_release(mp);

	libvlc_release(inst);
	return 0;
}
```


五、下载

libvlc_player


github：[https://github.com/zhuyinglong/libvlc_player](https://github.com/zhuyinglong/libvlc_player)

libvlc_player_MFC[](https://github.com/zhuyinglong/libvlc_player)

该工程使用MFC写了一个界面，也是基于libvlc的。该播放器可实现视频文件拖放，播放，暂停，停止，显示媒体信息，读取/设置播放进度等功能。软件实现只是在MFC基础上使用了libvlc库的几个函数，实现比较简单，所以不做说明，可直接点击下方链接下载源码阅读。

github：[https://github.com/zhuyinglong/libvlc_player_MFC.git](https://github.com/zhuyinglong/libvlc_player_MFC.git)


界面如下：

![](https://img-blog.csdn.net/20171009145400371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


说明：进度条之所以选用垂直的，是因为之前没用过，想试下与水平滚动条有无区别。结论：在使用上并无区别。









