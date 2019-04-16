# live555 分析—— OpenRtsp - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月15日 12:38:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：95
个人分类：[Live555](https://blog.csdn.net/leixiaohua1020/article/category/1362936)










通过分析live库提供的例子程序 OpenRTSP，可以清晰地了解客户端接收来自网络上媒体数据的过程。注意，RTP协议和RTCP协议接收的数据分别是视音频数据和发送/接收状况的相关信息，其中，RTP协议只负责接收数据，而RTCP协议除了接收服务器的 消息之外，还要向服务器反馈。


 main函数流程
 void main(int argc,char *argv[])
 {
 1. 创建BasicTaskScheduler对象
 2. 创建BisicUsageEnvironment对象
 3. 分析argv参数，（最简单的用法是：openRTSP rtsp://172.16.24.240/mpeg4video.mp4）以便在下面设置一些相关参数
 4. 创建RTSPClient对象
 5. 由RTSPClient对象向服务器发送OPTION消息并接受回应
 6. 产生SDPDescription字符串（由RTSPClient对象向服务器发送DESCRIBE消息并接受回应，根据回应的信息产生 SDPDescription字符串，其中包括视音频数据的协议和解码器类型）
 7. 创建MediaSession对象（根据SDPDescription在MediaSession中创建和初始化MediaSubSession子会话对 象）
 8. while循环中配置所有子会话对象（为每个子会话创建RTPSource和RTCPInstance对象，并创建两个GroupSock对象，分别对应 RTPSource和RTCPInstance对象，把在每个GroupSock对象中创建的socket描述符置入 BasicTaskScheduler::fReadSet中，RTPSource对象的创建的依据是SDPDescription，例如对于MPEG4 文件来说，视音频RTPSource分别对应MPEG4ESVideoRTPSource和MPEG4GenericRTPSource对象。 RTCPInstance对象在构造函数中完成将Socket描述符、处理接收RTCP数据的函数 (RTCPInstance::incomingReportHandler)以及RTCPInstance本身三者绑定在一个 HandlerDescriptor对象中，并置入BasicTaskScheduler::fReadHandler中。完成绑定后会向服务器发送一条 消息。）
 9. 由RTSPClient对象向服务器发送SETUP消息并接受回应。
 10. while循环中为每个子会话创建接收器(FileSink对象)，在FileSink对象中根据子会话的codec等属性缺省产生记录视音频数据的文件 名，视音频文件名分别为：video-MP4V-ES-1和audio-MPEG4-GENERIC-2，无后缀名
 11. while循环中为每个子会话的视音频数据装配相应的接收函数，将每个子会话中的RTPSource中的GroupSock对象中的SOCKET描述符， 置入BasicTaskScheduler::fReadSet中，并将描述符、处理接收RTP数据的函数 (MultiFramedRTPSource::networkReadHandler)以及RTPSource本身三者绑定在一个 HandlerDescriptor对象中，并置入BasicTaskScheduler::fReadHandler中,并将FileSink的缓冲区 和包含写入文件操作的一个函数指针配置给RTPSource对象，这个缓冲区将会在networkReadHandler中接收来自网络的视音频数据(分 析和去掉RTP包头的工作由RTPSource完成)，而这个函数指针在networkReadHandler中被调用以完成将缓冲区中的数据写入文件。
 12. 由RTSPClient对象向服务器发送PLAY消息并接受回应。
 13. 进入while循环，调用BasicTaskScheduler::SingleStep()函数接受数据，直到服务器发送TREADOWN消息给客户 端，客户端接收到该消息后释放资源，程序退出。

}

live555源代码（VC6）：[http://download.csdn.net/detail/leixiaohua1020/6374387](http://download.csdn.net/detail/leixiaohua1020/6374387)

原文地址：[http://blog.chinaunix.net/uid-22535463-id-262464.html](http://blog.chinaunix.net/uid-22535463-id-262464.html)




