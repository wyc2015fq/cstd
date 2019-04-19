# ParseSDPInfo（源码） - 三少GG - CSDN博客
2013年06月15日 15:31:47[三少GG](https://me.csdn.net/scut1135)阅读数：1354

原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://70565912.blog.51cto.com/1358202/461963) 、作者信息和本声明。否则将追究法律责任。[http://70565912.blog.51cto.com/1358202/461963](http://70565912.blog.51cto.com/1358202/461963)
ParseSDPInfo 工具
解析H264和AAC文件，生成对应的SDP信息文件。
可用于绕开RTSP协议，直接发送H264及AAC数据流。
运行截图
![](http://img1.51cto.com/attachment/201012/171206730.jpg)
生成的SDP信息：
![](http://img1.51cto.com/attachment/201012/171243762.jpg)
注意：
H264文件数据中必须包含有正确的sps及pps数据包，AAC文件需要是ADTS或者LOAS格式。
使用参数说明：
**********************************
ParseSDPInfo
Author: KuZi
Email : [70565912@qq.com](mailto:70565912@qq.com)
**********************************
Syntax: ParseSDPInfo -sdp <string>
                     -h264 <string> | -aac <string>
                     | -h264_port <integer> | -aac_port <integer>
                     | -local_ip <string>   | -target_ip <string>
-sdp        <string>    Specify the sdp file path.
-h264       <string>    Specify the h264 file path.
-aac        <string>    Specify the aac file path.
-h264_port  <integer>   Specify the h264 port.Defalut value is 1400
-aac_port   <integer>   Specify the aac port. Defalut value is 1402
-local_ip   <string>    Specify the local ip. Defalut value is "127.0.0.1".
-target_ip  <string>    Specify the target ip.Defalut value is "239.0.0.1".
Example:
ParseSDPInfo -sdp test.sdp -h264 test.h264 -aac test.aac
工具下载地址：[http://down.51cto.com/data/154041](http://down.51cto.com/data/154041)
主要代码都是从VLC源码里面直接剥离。另外借鉴了部分Live555，FFMPEG，MPlayer等开源项目的代码。
由上所述，所有核心代码均可以在上述开源项目代码中找到。因此，此版本不在公开提供源码。
确实需要源码作为学习用途，可以赞助一点支持费用另行提供。赞助费￥20.00元（是￥20.00，不是$20.00），你的支持，是我持续的动力。
淘宝地址：
[http://item.taobao.com/item.htm?id=9462930454](http://item.taobao.com/item.htm?id=9462930454)
