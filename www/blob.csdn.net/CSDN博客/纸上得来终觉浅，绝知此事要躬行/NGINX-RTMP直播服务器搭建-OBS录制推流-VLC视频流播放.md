# NGINX-RTMP直播服务器搭建-OBS录制推流-VLC视频流播放 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年11月29日 12:16:59[boonya](https://me.csdn.net/boonya)阅读数：9858








网上关于视频直播的资料还是挺多的，看了一些文章，自己也动手实践了下。主要有三个步骤：（1）NginxRTMP服务器搭建(2)视频录制推流器 （3）拉流器（播放器），有了这些就可以开始简单视频相关的直播了。对于延时什么的，还需要深入去研究 ，以及如何进行视频开发等后续再深入研究。

## Nginx-RTMP服务器搭建

参考：[利用nginx搭建RTMP视频点播、直播、HLS服务器](http://blog.csdn.net/kingroc/article/details/50839994)

[GitHub nginx-rtmp-module 参考地址](https://github.com/arut/nginx-rtmp-module)

Nginx点播设置：



```java
worker_processes  1;

events {
    worker_connections  1024;
}

##################RTMP服务#################
rtmp {

    server {

        listen 1935;  #//服务端口 

	chunk_size 4096;   #//数据传输块的大小

	application video {
	    play /usr/local/data/video; #//视频文件存放位置，访问方式rtmp://localhost:1935/video

	    #如视频路径存有视频welcome.mp4访问路径即为rtmp://localhost:1935/video/welcome.mp4
	}
    }
}

http {
    include       mime.types;

    default_type  application/octet-stream;

    sendfile        on;

    keepalive_timeout  65;

    server {

        listen       80;

        server_name  localhost;

        location / {
            root   html;
            index  index.html index.htm;
        }

        error_page   500 502 503 504  /50x.html;

        location = /50x.html {
            root   html;
        }
    }
}
```







Nginx直播配置：



```java
worker_processes  1;

events {
    worker_connections  1024;
}

##################RTMP服务#################
rtmp {

    server { 

        listen 1935;

        chunk_size 4096;

        application video {
           play /usr/local/data/video;
        }

	application live{ #第一处添加的直播字段
	   live on;
	}
    }

}
####启动浏览器查看http://localhost:80/stat
http {
    include       mime.types;

    default_type  application/octet-stream;

    sendfile        on;

    keepalive_timeout  65;

    server {

        listen       80;

        server_name  localhost;

    location /stat {     
            #第二处添加的location字段。
            rtmp_stat all;
            rtmp_stat_stylesheet stat.xsl;
    }

    location /stat.xsl { 
        #第二处添加的location字段。
        root /usr/local/nginx/nginx-rtmp-module/;
    }

        location / {
            root   html;
            index  index.html index.htm;
        }

        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }
}
```







Nginx直播回放配置：



```java
worker_processes  1;

events {
    worker_connections  1024;
}

##################RTMP服务#################
rtmp {
    server {
        listen 1935;
        chunk_size 4096;

        application video {
            play /usr/local/data/video;
        }

        application live {

		live on;#直播模式

		hls on; #这个参数把直播服务器改造成实时回放服务器。

		wait_key on; #对视频切片进行保护，这样就不会产生马赛克了。

		hls_path /usr/local/data/hls; #切片视频文件存放位置。

		hls_fragment 10s;     #每个视频切片的时长。

		hls_playlist_length 60s;  #总共可以回看的事件，这里设置的是1分钟。

		hls_continuous on; #连续模式。

		hls_cleanup on;    #对多余的切片进行删除。

		hls_nested on;     #嵌套模式。
        }
    }
}

http {
    include       mime.types;

    default_type  application/octet-stream;

    sendfile        on;

    keepalive_timeout  65;

    server {
            listen       80;

            server_name  localhost;

	    location /stat {
		    rtmp_stat all;
		rtmp_stat_stylesheet stat.xsl;
	    }

	    location /stat.xsl {
		root /usr/local/nginx/nginx-rtmp-module/;
	    }

	    location /live {  #这里也是需要添加的字段。

		types {  

		    application/vnd.apple.mpegurl m3u8;  

		    video/mp2t ts;  
		}
		alias /opt/video/hls;   
		expires -1;
		add_header Cache-Control no-cache;  
	    }  

	   location / {
		root   html;
		index  index.html index.htm;
	   }

	   error_page   500 502 503 504  /50x.html;

	   location = /50x.html {
		root   html;
	   }
    }
}
```



















## OBS下载及录制推流

下载地址1：[http://www.pc6.com/down.asp?id=410276](http://www.pc6.com/down.asp?id=410276)

下载地址2：[http://www.obsapp.net/](http://www.obsapp.net/)

下载地址3：[https://obsproject.com/](https://obsproject.com/)

这里以下载地址一的OBS版本与Nginx直播模式配置为例I端口相应的做了调整（http Server listen 1990）：

设置步骤：

1、添加场景

2、添加来源：显示器获取 、音频获取等等

3、设置RTMP的服务器路径，并设置流名称

![](https://img-blog.csdn.net/20171129115628116)

4、开始录制、开始推流

5、验证是否直播成功，打开浏览器http://172.16.20.10:1990/stat，看到如下界面表示直播正常。

![](https://img-blog.csdn.net/20171129120451062)

## VLC视频流播放

参考：[RTMP、RTSP、HTTP视频协议详解](http://www.hangge.com/blog/cache/detail_1325.html)VLC下载网上一搜就有了。

只需要设置播放流地址就可以验证前面配置的直播地址了：

![](https://img-blog.csdn.net/20171129121450408)

直播中：

![](https://img-blog.csdn.net/20171129121509384)



