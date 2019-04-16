# 最简单的基于libVLC的例子：最简单的基于libVLC的推流器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年01月09日 14:36:46[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：20985








=====================================================

最简单的基于libVLC的例子文章列表：

[最简单的基于libVLC的例子：最简单的基于libVLC的视频播放器](http://blog.csdn.net/leixiaohua1020/article/details/42363079)

[最简单的基于libVLC的例子：最简单的基于libVLC的视频播放器（图形界面版）](http://blog.csdn.net/leixiaohua1020/article/details/42363593)

[最简单的基于libVLC的例子：最简单的基于libVLC的推流器](http://blog.csdn.net/leixiaohua1020/article/details/42363701)

=====================================================


本文记录基于libVLC的最简单的推流器。该推流器可以将本地的视频文件转码后推送至目标流媒体服务器（也可以是一个组播地址）。

![](https://img-blog.csdn.net/20150103153453302)


## 最简单的基于libVLC的推流器




该推流器的代码十分简单，只用到了几个函数：

> libvlc_new()：创建libvlc_instance_t。
libvlc_vlm_add_broadcast()：增加一个广播（broadcast）。
libvlc_vlm_play_media()：播放指定名称的广播（broadcast）。
libvlc_vlm_stop_media()：停止指定名称的广播（broadcast）。
libvlc_vlm_release()：释放ibvlc_instance_t。

本示例的输入视频文件为“cuc_ieschool.flv”。这个推流器实现了：
- 输入视频转码为H.264
- 音频转码为mp2
- 封装格式设置为MPEG-TS
- 以UDP的形式发送至组播地址“udp://233.233.233.233:6666”
在程序发送广播的同时，会弹出一个窗口显示当前正在发送的视频。



## 源代码

```cpp
/**
 * 最简单的基于libVLC的推流器
 * Simplest libVLC Streamer
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序是一个最简单的基于libVLC的推流器。
 * 适合初学者学习libVLC。
 *
 * This example is the simplest Streamer based on libVLC.
 * Suitable for the beginner of libVLC.
 */

#include <Windows.h>
#include "vlc/vlc.h"


int main(int argc, char **argv) {
	libvlc_instance_t *vlc;
	const char *url;
	//Send File
	//Transcode it. Video codec use x264. Audio codec use mpga.
	//Mux it to mpegts format.
	//And stream it to udp://233.233.233.233:6666
	/*
	const char *sout = "#transcode{vcodec=h264,fps=25,venc=x264{preset=ultrafast,"\
		"profile=main,tune=zerolatency},vb=512,scale=0.5,"                        \
		"acodec=mpa,aenc=ffmpeg,ab=64,channels=2}"                                \
		":standard{access=udp,mux=ts,dst=233.233.233.233:6666}";
	*/
	//Send and playing at same time
	const char *sout = "#transcode{vcodec=h264,fps=25,venc=x264{preset=ultrafast,"\
		"profile=baseline,tune=zerolatency},vb=512,"                              \
		"acodec=mpga,ab=64,channels=2}"                                           \
		":duplicate{dst=display,dst=standard{access=udp,mux=ts,dst=233.233.233.233:6666}}";
	const char *media_name = "Lei's test";

	//Screen Capture
	//url = "screen://";

	url = "cuc_ieschool.flv";
	
	vlc = libvlc_new(0, NULL);
	libvlc_vlm_add_broadcast(vlc, media_name, url, sout, 0, NULL, true, false);
	libvlc_vlm_play_media(vlc, media_name);
	
	//play 30s
	_sleep(30000); 

	libvlc_vlm_stop_media(vlc, media_name);
	libvlc_vlm_release(vlc);
	return 0;
}
```



## 下载




**Simplest libVLC Example**


**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestlibvlcexample/](https://sourceforge.net/projects/simplestlibvlcexample/)

Github：[https://github.com/leixiaohua1020/simplest_libvlc_example](https://github.com/leixiaohua1020/simplest_libvlc_example)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_libvlc_example](http://git.oschina.net/leixiaohua1020/simplest_libvlc_example)

CDSN下载地址： [http://download.csdn.net/detail/leixiaohua1020/8342413](http://download.csdn.net/detail/leixiaohua1020/8342413)

本工程是包含了一些基于libVLC的示例程序。一共包含了如下几个子程序。
playerGUI: 最简单的基于libVLC的播放器-图形界面版。
simplest_libvlc_example: 最简单的基于libVLC的播放器。
simplest_libvlc_streamer: 最简单的基于libVLC的推流器。




