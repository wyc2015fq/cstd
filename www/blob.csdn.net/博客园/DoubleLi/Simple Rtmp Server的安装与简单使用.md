# Simple Rtmp Server的安装与简单使用 - DoubleLi - 博客园






Simple Rtmp Server是一个国人编写的开源的RTMP/HLS流媒体服务器. 功能与nginx-rtmp-module类似, 可以实现rtmp/hls的分发.

  有关nginx-rtmp-module的可参照: http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/redstarofsleep/article/details/45092147

  编译与安装过程十分的简单





**[plain]**[view plain](http://blog.csdn.net/redstarofsleep/article/details/45332131#)[copy](http://blog.csdn.net/redstarofsleep/article/details/45332131#)

[print](http://blog.csdn.net/redstarofsleep/article/details/45332131#)[?](http://blog.csdn.net/redstarofsleep/article/details/45332131#)

-   ./configure --prefix=/usr/local/srs --rtmp-hls  
- 
-   make  
- 
-   make install  


  标准的三条命令就可完成安装. --rtmp-hls表示开启rtmp和hls, 其它的编译参数可通过./configure -h查看.





  安装完后,目录下有三个文件夹conf, etc, objs. 我们常用的是conf和objs两个目录下的内容, conf下是各类配置文件的例子, objs下是可执行文件.

  启动服务器是通过-c参数指定一个配置文件即可



**[plain]**[view plain](http://blog.csdn.net/redstarofsleep/article/details/45332131#)[copy](http://blog.csdn.net/redstarofsleep/article/details/45332131#)

[print](http://blog.csdn.net/redstarofsleep/article/details/45332131#)[?](http://blog.csdn.net/redstarofsleep/article/details/45332131#)

-   ./objs/srs -c conf/hls.conf  






  如果是hls的话,配置文件大概是这样的:





**[plain]**[view plain](http://blog.csdn.net/redstarofsleep/article/details/45332131#)[copy](http://blog.csdn.net/redstarofsleep/article/details/45332131#)

[print](http://blog.csdn.net/redstarofsleep/article/details/45332131#)[?](http://blog.csdn.net/redstarofsleep/article/details/45332131#)

- # the config for srs to delivery hls  
- # @see https://github.com/winlinvip/simple-rtmp-server/wiki/v1_CN_SampleHLS  
- # @see full.conf for detail config.  
- 
- listen              1935;  
- max_connections     1000;  
- vhost __defaultVhost__ {  
-     hls {  
-         enabled         on;  
-         hls_path        /usr/local/nginx/html;  
-         hls_fragment    10;  
-         hls_window      60;  
-     }  
- }  



  其中的hls_path是存放hls分片ts文件和m3u8的目录, 我上面的例子中把它指定到nginx下.这样就可以直接播放hls视频流了.



  如果是ffmpeg推送,那是和nginx-rtmp-module是一样的:





**[plain]**[view plain](http://blog.csdn.net/redstarofsleep/article/details/45332131#)[copy](http://blog.csdn.net/redstarofsleep/article/details/45332131#)

[print](http://blog.csdn.net/redstarofsleep/article/details/45332131#)[?](http://blog.csdn.net/redstarofsleep/article/details/45332131#)

- ffmpeg -re -i "D:\download\film\aqgy\02.mp4" -vcodec libx264 -vprofile baseline -acodec aac    
-  -ar 44100 -strict -2 -ac 1 -f flv -s 1280x720 -q 10 rtmp://server:1935/    
- myapp/test1  



  总体来说这个SimpleRtmpServer在配置使用上没有什么难度,基本都是参照nginx-rtmp-module的.









