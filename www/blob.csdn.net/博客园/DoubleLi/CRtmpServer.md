# CRtmpServer - DoubleLi - 博客园






# 1.  前言


crtmpserver是一个由C++语言编写的开源的RTMP流媒体服务器，官方网站是www.rtmpd.com


# 2.   CRtmpServer编译



## 2.1.  Win7+Vs2010环境

下载   [http://download.csdn.net/detail/machh/9739696](http://download.csdn.net/detail/machh/9739696)

### 2.1.1.  编译错误解决


首先在windows下编译crtmpserver也需要windows版本的openssl，所以我们需要先编译windows版本的openssl。

设置openssl的头文件和库文件的路径。


### 2.1.2.  修改配置文件

配置crtmpserver.lua

# 3.  推流

### 3.1.1.  使用ffmpeg推送flv


**ffmpeg -re -i source.flv -acodec copy -vcodec copy -f flv rtmp://192.168.1.58/live/test**


### 3.1.2.  使用Flashmedialiveencoder 推送摄像头

![](file:///C:/Users/DELL/AppData/Local/YNote/data/machenghai@163.com/3aea68026d324a649f54e8e1b024f99f/6597209404634754165.jpg)![](http://img.blog.csdn.net/20170117153901979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWFjaGg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







# 4.  播放

### 4.1.1.  使用FVPlayer.exe播放

下载 [http://download.csdn.net/detail/machh/9739710](http://download.csdn.net/detail/machh/9739710)



**rtmp://192.168.1.6/live/test**





### 4.1.2.  使用jwplayer播放









