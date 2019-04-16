# 最简单的基于libVLC的例子：最简单的基于libVLC的视频播放器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年01月08日 22:33:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：67980标签：[libvlc																[vlc																[播放器																[开发](https://so.csdn.net/so/search/s.do?q=开发&t=blog)
个人分类：[VLC																[我的开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1843731)](https://blog.csdn.net/leixiaohua1020/article/category/2619507)







=====================================================

最简单的基于libVLC的例子文章列表：

[最简单的基于libVLC的例子：最简单的基于libVLC的视频播放器](http://blog.csdn.net/leixiaohua1020/article/details/42363079)

[最简单的基于libVLC的例子：最简单的基于libVLC的视频播放器（图形界面版）](http://blog.csdn.net/leixiaohua1020/article/details/42363593)

[最简单的基于libVLC的例子：最简单的基于libVLC的推流器](http://blog.csdn.net/leixiaohua1020/article/details/42363701)

=====================================================

本文记录使用libVLC的开发的最简单的视频播放器示例。VLC Media Player是一款优秀的播放器，但是由于它的源代码编译的难度比较大，一直没有深入研究过它的开发方面的技术。此前触到了一些VLC开发方面的东西，因此总结了一下libVLC的开发示例程序。

![](https://img-blog.csdn.net/20150103151603594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 如何获取VLC的SDK
VLC开发所需的库文件可以有2种获取方法：

> 1.自行编译
2.直接从安装目录里面拷贝出来


第一种方法难度要大一些。尤其是在Windows下编译VLC是个比较麻烦的事情。一般情况下可以选择第二种方法获取VLC开发所需的文件。
开发VLC所需的文件的位置：

> 1.动态链接库*.dll：安装目录下的libvlc.dll，libvlccore.dll以及plugins目录下的所有文件。
PS：VLC支持非常多的plugin。因此plugins目录的体积确实是非常大的。
2.静态链接库*.lib：安装目录/sdk/lib
3.头文件*.h：安装目录/sdk/include

新建一个VC工程后，把上述三类文件分别拷贝至新工程目录下，并且配置它们的路径之后，就可以使用libVLC进行开发了。


## 最简单的基于libVLC的视频播放器

使用libVLC开发一个播放器十分的容易。最简单的基于libVLC的视频播放器的流程图如下图所示。

![](https://img-blog.csdn.net/20150103151643531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

流程图中包含了3个结构体：

> libvlc_instance_t：代表一个libVLC的实例。
libvlc_media_t：代表一个可以播放的媒体。
libvlc_media_player_t：代表一个VLC媒体播放器（一个视频播放器播放一个视频）。注意VLC并不仅仅用于媒体播放。
创建或者以上3个结构体通过以下6个函数：

> libvlc_new()：创建libvlc_instance_t。
libvlc_media_new_path()：创建libvlc_media_t。
libvlc_media_player_new_from_media()：创建libvlc_media_player_t。
libvlc_media_player_release()：释放libvlc_media_player_t
libvlc_media_release()：释放libvlc_media_t。
libvlc_release()：释放libvlc_instance_t。
可以已通过下面的函数控制媒体的播放或者暂停，这些函数都需要使用libvlc_media_player_t作为参数。这里处于简化的目的，只使用了播放和停止函数：

> libvlc_media_player_play()：播放。
libvlc_media_player_pause()：暂停。
libvlc_media_player_stop()：停止。
除了上述3个函数之外，还包括libvlc_media_player_set_position()等函数，这里不再一一记录。


## 几点注意事项

### libvlc_media_t的创建
创建libvlc_media_t有两种方法：libvlc_media_new_path()和libvlc_media_new_location()。简单描述一下这两个函数的区别：libvlc_media_new_location()用于打开协议，而libvlc_media_new_path()用于打开文件。因而传递给libvlc_media_new_path()的就是普通的文件路径（绝对路径例如D:\xxx.flv，或者相对路径例如xxx.flv），而传递给libvlc_media_new_location()的就是协议地址（例如“udp://….”，“http://”）。但是这里有一点需要注意，在VLC中“文件”也属于一种广义上的“协议”。因此使用libvlc_media_new_location()也可以打开文件，但是必须在文件路径前面加上“文件协议”的标记“file:///”。例如打开“F:\movie\cuc_ieschool.flv”下的视频，实际使用的代码如下所示。

```cpp
libvlc_media_new_location (inst, "file:///F:\\movie\\cuc_ieschool.flv");
```
此外，VLC还支持很多“神奇”的协议，比如输入“screen://”协议就可以进行屏幕录制，代码如下。

```cpp
libvlc_media_new_location (inst, "screen://");
```


### 把libVLC的弹出窗口嵌入到程序中
在这里我只实践过Windows下把libVLC的弹出窗口嵌入到程序中。将窗口或者控件的句柄传递给libvlc_media_player_set_hwnd()函数即可。
这里有一点需要注意，如果把libVLC弹出窗口嵌入到程序中的话，“全屏”功能就不能使用了。


### 关于libVLC加载的问题
在libVLC中可以通过libvlc_media_player_get_length()，libvlc_video_get_width()，libvlc_video_get_height()等函数获取到视频的时长，宽，高等信息。但是有一个很奇怪的现象：如果在调用完libvlc_media_player_play()之后立即调用上述3个函数的话，返回的值都是0，只有“等待”一段时间（例如调用sleep()）后再调用上述函数，才能得到正确的数值。对于这种现象，我觉得可能是libVLC加载完成之后，才能通过上述几个函数获得正确的值（自己推测的，还没有深究）。


## 代码



```cpp
/**
 * 最简单的基于libVLC的播放器
 * Simplest libVLC Player
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序是一个最简单的基于libVLC的视频播放器。
 * 适合初学者学习libVLC。
 *
 * This example is the simplest Video Player based on libVLC.
 * Suitable for the beginner of libVLC.
 */
#include <Windows.h>
#include "vlc/vlc.h"

int main(int argc, char* argv[])
 {
     libvlc_instance_t * inst;
     libvlc_media_player_t *mp;
     libvlc_media_t *m;
     
	 libvlc_time_t length;
	 int width;
	 int height;
	 int wait_time=5000;

	 //libvlc_time_t length;

     /* Load the VLC engine */
     inst = libvlc_new (0, NULL);
  
     //Create a new item
	 //Method 1:
     //m = libvlc_media_new_location (inst, "file:///F:\\movie\\cuc_ieschool.flv");
	 //Screen Capture
	 //m = libvlc_media_new_location (inst, "screen://");
	 //Method 2:
     m = libvlc_media_new_path (inst, "cuc_ieschool.flv");
        
     /* Create a media player playing environement */
     mp = libvlc_media_player_new_from_media (m);

	 
     /* No need to keep the media now */
     libvlc_media_release (m);
 
     // play the media_player
     libvlc_media_player_play (mp);

	 //wait until the tracks are created
	 _sleep (wait_time);
	 length = libvlc_media_player_get_length(mp);
	 width = libvlc_video_get_width(mp);
	 height = libvlc_video_get_height(mp);
	 printf("Stream Duration: %ds\n",length/1000);
	 printf("Resolution: %d x %d\n",width,height);
	 //Let it play 
     _sleep (length-wait_time); 
    
     // Stop playing
     libvlc_media_player_stop (mp);
 
     // Free the media_player
     libvlc_media_player_release (mp);
 
     libvlc_release (inst);
 
     return 0;
 }
```




## 运行结果

程序运行后，会弹出一个窗口播放“cuc_ieschool.flv”文件。




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
simplest_libvlc_streamer: 最简单的基于libVLC的推流器。](https://so.csdn.net/so/search/s.do?q=播放器&t=blog)](https://so.csdn.net/so/search/s.do?q=vlc&t=blog)](https://so.csdn.net/so/search/s.do?q=libvlc&t=blog)




