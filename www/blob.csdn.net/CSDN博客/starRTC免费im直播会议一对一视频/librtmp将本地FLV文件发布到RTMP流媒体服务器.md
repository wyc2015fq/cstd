# librtmp将本地FLV文件发布到RTMP流媒体服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年05月22日 09:52:30[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：100
没有用到ffmpeg库
可以将本地FLV文件发布到RTMP流媒体服务器
使用librtmp发布RTMP流可以使用两种API：RTMP_SendPacket()和RTMP_Write()。
使用RTMP_SendPacket()发布流的时候函数执行流程如下图所示。使用RTMP_Write()发布流的时候函数执行流程图相差不大。
InitSockets()：初始化Socket
RTMP_Alloc()：为结构体“RTMP”分配内存。
RTMP_Init()：初始化结构体“RTMP”中的成员变量。
RTMP_SetupURL()：设置输入的RTMP连接的URL。
RTMP_EnableWrite()：发布流的时候必须要使用。如果不使用则代表接收流。
RTMP_Connect()：建立RTMP连接，创建一个RTMP协议规范中的NetConnection。
RTMP_ConnectStream()：创建一个RTMP协议规范中的NetStream。
Delay：发布流过程中的延时，保证按正常播放速度发送数据。
RTMP_SendPacket()：发送一个RTMP数据RTMPPacket。
RTMP_Close()：关闭RTMP连接。
RTMP_Free()：释放结构体“RTMP”。
CleanupSockets()：关闭Socket。
程序运行后，会将文件以直播流的形式发布到“rtmp://localhost/publishlive/livestream”的URL。修改文件名称和RTMP的URL可以实现将任意flv文件发布到任意RTMP的URL。

[http://blog.csdn.net/leixiaohua1020/article/details/42104945](http://blog.csdn.net/leixiaohua1020/article/details/42104945)
