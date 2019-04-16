# 开源视频会议系统：OpenMeetings 安装方法 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月27日 00:07:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：64











![](https://img-blog.csdn.net/20131126145353453)

OpenMeetings是一个多语言可定制的视频会议和协作系统。说到OpenMeetings就应该提一下red5因为OpenMeetings 的视频服务是加载red5上面的。Red5是一款基于JAVA的免费的开源的Flash流媒体server.是当前最好的替代Adobe提供的FMS的开源软件。而且在可扩展性上有FMS不可比拟的优势。OpenMeetings是一款基于Flex开发的开源视频软件，通过RTMP和HTTP协议传送视频数据。支持在线聊天，视频，文件共享，桌面共享。支持多语言。



OpenMeetings简单的安装步骤：

![](https://img-blog.csdn.net/20131125204106562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意：安装并配置java环境，需要JRE 6。

1.OpenMeetings默认使用ApacheDerby数据库，也可以选择MySQL, Postgres , IBM DB2 , Oracle ,或 MSSQL。

**2. 允许导入图片文件至白板**

安装ImageMagick 。[http://www.imagemagick.org](http://www.imagemagick.org)

**3. 允许导入PDF文件至白板**

安装GhostScript。 [http://www.ghostscript.com/download/gsdnld.html](http://www.ghostscript.com/download/gsdnld.html)

安装SWFTools。[http://www.swftools.org/](http://www.swftools.org/)

**4. 允许导入.doc, .docx, .ppt, .pptx, ... 等Office 文档至白板**

安装JODConverter。

安装OpenOffice 或者LibreOffice。

**5. 允许导入（以及录制）.avi, .flv, .mov 以及 .mp4文件**

安装FFMpeg (包含libmp3lame 以及 libx264)。[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)

安装SoX。[http://sox.sourceforge.net/](http://sox.sourceforge.net/)



原文地址：[http://openmeetings.apache.org/installation.html](http://openmeetings.apache.org/installation.html)




