# 利用nginx搭建RTMP视频点播、直播、HLS服务器 - DoubleLi - 博客园








### 开发环境
- [Ubuntu 14.04 server](http://www.ubuntu.com/download/server/thank-you?country=CN&version=14.04.4&architecture=amd64)
- [nginx-1.8.1](http://nginx.org/download/nginx-1.8.1.tar.gz)
- [nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module)

# nginx的服务器的搭建

**安装nginx的依赖库**

```
sudo apt-get update
sudo apt-get install libpcre3 libpcre3-dev
sudo apt-get install openssl libssl-dev
```
- 1
- 2
- 3
- 1
- 2
- 3

**配置并编译nginx**
使用nginx的默认配置，添加nginx的rtmp模块。 
![这里写图片描述](http://img.blog.csdn.net/20160310000425785)

```
./configure --add-module=../nginx-rtmp-module-master
make
sudo make install
```
- 1
- 2
- 3
- 1
- 2
- 3

**运行[测试](http://lib.csdn.net/base/softwaretest)nginx**
进入安装目录/usr/local/nginx，运行命令./sbin/nginx

> 
注意：以后所有的命令都在/usr/local/nginx目录运行，也nginx配置文件的相对目录。


打开浏览器在地址栏输入：localhost。如果，如下图显示那样就证明您的nginx服务器搭建成功了。 
![这里写图片描述](http://img.blog.csdn.net/20160309235400922)

# 点播视频服务器的配置

通过上一步nginx服务器已经搭建完成，然后我们就可以开启一个视频点播的服务了。打开配置文件nginx.conf，添加RTMP的配置。

```
worker_processes  1;

events {
    worker_connections  1024;
}
rtmp {                #RTMP服务
    server {
        listen 1935;  #//服务端口 
    chunk_size 4096;   #//数据传输块的大小

    application vod {
        play /opt/vide/vod; #//视频文件存放位置。
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
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33

配置目录*/opt/video/vod*为存放视频文件的位置了，那我们就往里面放一个文件吧。我放了一个qq.mp4文件。![这里写图片描述](http://img.blog.csdn.net/20160317235704649)
文件放好之后，那就让我们重新启动一下nginx

> 
sudo ./sbin/nginx -s reload


打开视频播放软件选用的是VLC media-> open network stream…. 
如图填写我们要点播的节目地址*rtmp://localhost/vod/qq.mp4* 如图： 
![这里写图片描述](http://img.blog.csdn.net/20160318000547899)
点击play就可以播放了。 
当然点播不使用RTMP插件nginx自身也是可以实现点播服务的。那就是配置location部分，由于下面我们要配置直播和回看功能所以选用了RTMP服务。

# 直播视频服务器的配置

接着我们就在点播服务器配置文件的基础之上添加直播服务器的配置。一共2个位置，第一处就是给RTMP服务添加一个application这个名字可以任意起，也可以起多个名字，由于是直播我就叫做它live吧，如果打算弄多个频道的直播就可以live_cctv1、live_cctv2名字任意。第二处就是添加两个location字段，字段的内容请直接看文件吧。

```
worker_processes  1;

events {
    worker_connections  1024;
}

rtmp {
    server { 
        listen 1935;
    chunk_size 4096;

    application vod {
        play /opt/video/vod;
    }

    application live{ #第一处添加的直播字段
        live on;
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

    location /stat {     #第二处添加的location字段。
            rtmp_stat all;
        rtmp_stat_stylesheet stat.xsl;
    }

    location /stat.xsl { #第二处添加的location字段。
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
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52

添加完这两处之后，重新启动nginx打开浏览器看看，是否有如下图显示： 
![这里写图片描述](http://img.blog.csdn.net/20160322005840825)
有没有看到红框框的live字样呢?如果可以显示出来，证明你的配置生效了。

还等什么让我们推送一个节目看看（其实专业词叫录制，后面将会使用录制这个词。）吧。

这次推送我使用的是OBS（Open Broadcaster Software）有关它的安装请参考先前我写的[Ubuntu安装OBS（Open Broadcaster Software）](http://blog.csdn.net/kingroc/article/details/50829213)后面的博客里我会给大家介绍如何使用手机采集视频并且推送到服务器上。

### 配置OBS如图：

![这里写图片描述](http://img.blog.csdn.net/20160322115339223)
- 点击红圈添加“Media Source”，会弹出一个框框，里面的内容就按照图片显示配置就可以了。点击“OK”就能在你的屏幕上播放了。
- 配置节目的输出流如图所示： ![这里写图片描述](http://img.blog.csdn.net/20160322120304923)首先点击红圈“setting”进入设置界面。记得要正确填写要录制的服务器路径啊。配置好就可以点击OK了。退出后点击蓝圈，就开始录制节目了。
- 查看我们的了录制的节目，服务器有没有接收到呢？打开我的服务器地址“[http://localhost/stat](http://localhost/stat)”查看一下 ，你的显示是否和我的相同呢？如果相同证明服务器已经接收到了录制的节目，客户端可以进行播放了。 
![这里写图片描述](http://img.blog.csdn.net/20160322121312019)
播放的地址就是“rtmp://localhost/live/test”，如果您本地有支持rtmp协议的播放器就可以试试了。最后奉上一张观看直播的屏幕截图。 
![这里写图片描述](http://img.blog.csdn.net/20160322121849334)

# 实时回看视频服务器的配置

我们想一想如果直播服务能够把节目录制在本地，我们不就可以直接进行回看先前的节目了吗？回看一分钟、一小时甚至一天的。想想就兴奋不用写代码有现成的可以使用。怎么用呢？继续看nginx的配置吧。

```
worker_processes  1;

events {
    worker_connections  1024;
}

rtmp {
    server {
        listen 1935;
    chunk_size 4096;

    application vod {
        play /opt/video/vod;
    }

        application live {
        live on;
        hls on; #这个参数把直播服务器改造成实时回放服务器。
        wait_key on; #对视频切片进行保护，这样就不会产生马赛克了。
        hls_path /opt/video/hls; #切片视频文件存放位置。
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
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68

1.添加完成后需要重新启动nginx，由于这次nginx需要向服务器写切片视频文件，但nginx我又没有给nginx指定用户名只能走默认的*nobody*用户和*nogroup*用户组，其实就是没有组。所以我对需要写入的目录做了增大权限的修改。 
如下图，这样做就是为了避免由于权限问题而无法写文件。 
![这里写图片描述](http://img.blog.csdn.net/20160322181643035)
2.如何给服务器录制视频，在上一节已经说过，这里就不再说了。 
3.查看视频文件是否真的录制上没有，继续看图： 
![这里写图片描述](http://img.blog.csdn.net/20160322181953852)
已经产生切片视频文件了。其中还有一个index.m3u8。 
4.播放视频，这次可是http开头的了，“*[http://localhost/live/test/index.m3u8](http://localhost/live/test/index.m3u8)*”。 
5.已经可以播放了，如何回看呢？其实这个index.m3u8文件仅仅是目录。想回看那个就播放那个.ts文件就可以了。

到此已经结束。如有问题欢迎留言。















