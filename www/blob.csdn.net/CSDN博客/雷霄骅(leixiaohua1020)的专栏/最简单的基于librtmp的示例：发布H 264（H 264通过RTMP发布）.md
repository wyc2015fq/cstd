# 最简单的基于librtmp的示例：发布H.264（H.264通过RTMP发布） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年12月27日 00:26:20[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：43144








=====================================================


最简单的基于libRTMP的示例系列文章列表：

[最简单的基于librtmp的示例：接收（RTMP保存为FLV）](http://blog.csdn.net/leixiaohua1020/article/details/42104893)


[最简单的基于librtmp的示例：发布（FLV通过RTMP发布）](http://blog.csdn.net/leixiaohua1020/article/details/42104945)


[最简单的基于librtmp的示例：发布H.264（H.264通过RTMP发布）](http://blog.csdn.net/leixiaohua1020/article/details/42105049)


=====================================================


本文记录一个基于libRTMP的发布H.264码流的程序。该程序可以将H.264数据发布到RTMP流媒体服务器。目前这个例子还不是很稳定，下一步还有待修改。

本程序使用回调函数作为输入，通过自定义的回调函数，可以发送本地的文件或者内存中的数据。

![](https://img-blog.csdn.net/20150110200243307)


## 函数调用结构图

本程序的函数调用结构图如下所示。

![](https://img-blog.csdn.net/20141223171815590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
整个程序包含3个接口函数：
RTMP264_Connect()：建立RTMP连接。
RTMP264_Send()：发送数据。
RTMP264_Close()：关闭RTMP连接。
按照顺序调用上述3个接口函数就可以完成H.264码流的发送。

结构图中关键函数的作用如下所列。

RTMP264_Connect()中包含以下函数：

> InitSockets()：初始化Socket
RTMP_Alloc()：为结构体“RTMP”分配内存。
RTMP_Init()：初始化结构体“RTMP”中的成员变量。
RTMP_SetupURL()：设置输入的RTMP连接的URL。
RTMP_EnableWrite()：发布流的时候必须要使用。如果不使用则代表接收流。
RTMP_Connect()：建立RTMP连接，创建一个RTMP协议规范中的NetConnection。
RTMP_ConnectStream()：创建一个RTMP协议规范中的NetStream。

RTMP264_Send()中包含以下函数：

> ReadFirstNaluFromBuf()：从内存中读取出第一个NAL单元。
ReadOneNaluFromBuf()：从内存中读取出一个NAL单元。
h264_decode_sps()：解码SPS，获取视频的宽，高，帧率信息。
SendH264Packet()：发送一个NAL单元。

SendH264Packet()中包含以下函数：

> SendVideoSpsPps()：如果是关键帧，则在发送该帧之前先发送SPS和PPS。
SendPacket()：组装一个RTMPPacket，调用RTMP_SendPacket()发送出去。
RTMP_SendPacket()：发送一个RTMP数据RTMPPacket。

RTMP264_Close()中包含以下函数：

> RTMP_Close()：关闭RTMP连接。
RTMP_Free()：释放结构体“RTMP”。
CleanupSockets()：关闭Socket。


## 源代码

程序提供的3个接口函数的使用方法如下。可以看出接口比较简单。



```cpp
/**
 * Simplest Librtmp Send 264
 *
 * 雷霄骅，张晖
 * leixiaohua1020@126.com
 * zhanghuicuc@gmail.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序用于将内存中的H.264数据推送至RTMP流媒体服务器。
 * This program can send local h264 stream to net server as rtmp live stream.
 */

#include <stdio.h>
#include "librtmp_send264.h"



FILE *fp_send1;

//读文件的回调函数
//we use this callback function to read data from buffer
int read_buffer1(unsigned char *buf, int buf_size ){
	if(!feof(fp_send1)){
		int true_size=fread(buf,1,buf_size,fp_send1);
		return true_size;
	}else{
		return -1;
	}
}

int main(int argc, char* argv[])
{
	fp_send1 = fopen("cuc_ieschool.h264", "rb");

	//初始化并连接到服务器
	RTMP264_Connect("rtmp://localhost/publishlive/livestream");
	
	//发送
	RTMP264_Send(read_buffer1);

	//断开连接并释放相关资源
	RTMP264_Close();

	return 0;
}
```



接口函数内部的代码比较多，不再详细记录。


## 下载

****

**Simplest LibRTMP Example**


**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestlibrtmpexample/](https://sourceforge.net/projects/simplestlibrtmpexample/)

Github：[https://github.com/leixiaohua1020/simplest_librtmp_example](https://github.com/leixiaohua1020/simplest_librtmp_example)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_librtmp_example](http://git.oschina.net/leixiaohua1020/simplest_librtmp_example)

CSDN下载：[http://download.csdn.net/detail/leixiaohua1020/8291757](http://download.csdn.net/detail/leixiaohua1020/8291757)

本工程包含了LibRTMP的使用示例，包含如下子工程：
simplest_librtmp_receive: 接收RTMP流媒体并在本地保存成FLV格式的文件。
simplest_librtmp_send_flv: 将FLV格式的视音频文件使用RTMP推送至RTMP流媒体服务器。
simplest_librtmp_send264: 将内存中的H.264数据推送至RTMP流媒体服务器。





