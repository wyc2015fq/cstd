# 基于nginx-rtmp-module模块实现的HTTP-FLV直播模块（nginx-http-flv-module） - weixin_33985507的博客 - CSDN博客
2018年06月01日 15:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
 本文后续的内容将在这里更新：[《基于nginx-rtmp-module模块实现的HTTP-FLV直播模块（nginx-http-flv-module）续》](http://blog.csdn.net/winshining/article/details/79492099)。注意：下文的配置很多已经不能用了，因为现在的实现跟早期的实现相差有点大。而为了看到整个项目的变迁史，所以保留了下来，下文的更新中提到了为什么有些配置项不能再使用的原因。现在使用的配置可查找下文中的README.CN.md。
      近几年直播行业火爆，开源的直播软件解决方案有[SRS](https://github.com/ossrs/srs)（Simple-RTMP-Server）和[nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module)，前者是国人发起的一个优秀的开源项目，目前国内很多公司都使用它作为直播解决方案，由C++编写；后者依赖[Nginx](http://nginx.org/)，以第三方模块的方式提供直播功能，由C编写。SRS采用多线程方式（经网友提醒更正：是单线程+协程方式），性能优秀，经受住了众多场景的考验，但是SRS3已经闭源（更正：是有一段时间闭源了，现在又开源了）；nginx-rtmp-module是采用多进程方式，Nginx的性能优秀，但是据网友测试，nginx-rtmp-module的性能不如SRS，并且nginx-rtmp-module的作者已经很久没有更新版本了，支持的功能也有限，例如不支持HTTP方式的FLV直播，而这是国内直播行业普遍采用的方式；再如推流不支持upstream，无法分布式部署功能；还有饱受诟病的播放响应延迟时间很长的问题（即俗称的不能秒播）等。
      我在nginx-rtmp-module的基础上实现了基于HTTP方式的FLV直播功能，支持GOP缓存，减少播放响应延迟时间；支持流式和Transfer-Encoding: chunked两种HTTP响应格式；修复nginx-rtmp-module没有listen配置项时，推流失败的问题；解决nginx-rtmp-module已知的bug，见[nginx-http-flv-module](https://github.com/winshining/nginx-http-flv-module)，欢迎下载测试和修复bug。有问题或者建议，可以加Q群：711969608详聊。目前已经有厂商准备将本模块商用，目前已知有9家，其中一家厂商是华为，据另一家厂商的员工称他们的客户包括映客和微吼，其他的目前都还在测试中，有厂商陆续反馈过不少bug，修复后功能已经越来越稳定，在此表示感谢。
     nginx-http-flv-module与nginx-rtmp-module的功能对比：
|功能|nginx-http-flv-module|nginx-rtmp-module|备注|
|----|----|----|----|
|HTTP-FLV|√|×|nginx-http-flv-module支持HTTPS-FLV|
|GOP缓存|√|×||
|vhost|√|×||
|省略listen配置|√|x||
|RTMP 302|Beta|×|nginx-http-flv-module作为服务器或者客户端|
      如果不想推流，可以用一个现成的直播地址rtmp://live.hkstv.hk.lxdns.com/live/hks。
典型的nginx.conf如下：
worker_processes  4; #Nginx开启4个子进程，子进程个数最好跟CPU的核心数一样
worker_cpu_affinity 0001 0010 0100 1000; #CPU的mask，子进程使用它来绑定CPU核心，避免进程切换造成性能损失
error_log logs/error.log error; #错误日志位置和日志级别，如果使用默认编译选项，位置为/usr/local/nginx/logs/error.log，error表示只打印错误日志
events {
    worker_connections  1024; #Nginx处理的最大连接数
}
http {
    include       mime.types;
    default_type  application/octet-stream;
    keepalive_timeout  65;
    server {
        listen       80; #Nginx监听的HTTP请求端口
        location / {
            root   /var/www; #HTTP请求URL映射到服务器的位置
            index  index.html index.htm; #HTTP请求优先请求的文件，如http://localhost/，如果有index.html在/var/www目录下，那么请求的是/var/www/index.html
        }
        error_page   500 502 503 504  /50x.html; #如果遇到这些HTTP请求错误，Nginx返回50x.html的内容
        location = /50x.html {
            root   html; #因为/配置了root /var/www，所以这儿html对应的是/var/www/html，所以50x.html的路径是/var/www/html/50x.html
        }
        location /live {
            flv_live on; #当HTTP请求以/live结尾，匹配这儿，这个选项表示开启了flv直播播放功能
            chunked  on; #HTTP协议开启Transfer-Encoding: chunked;方式回复
        }
    }
}
rtmp_auto_push on; #因为Nginx可能开启多个子进程，这个选项表示推流时，媒体流会发布到多个子进程
rtmp_auto_push_reconnect 1s;
rtmp_socket_dir /tmp; #多个子进程情况下，推流时，最开始只有一个子进程在竞争中接收到数据，然后它再relay给其他子进程，他们之间通过unix domain socket传输数据，这个选项表示unix domain socket的路径
rtmp {
    out_queue   4096;
    out_cork    8;
    max_streams 64; #Nginx能接受的最大的推流数
    server {
        listen 1935; #Nginx监听的RTMP推流/拉流端口，可以省略，默认监听1935
        application myapp {
            live on; #当推流时，RTMP路径中的APP（RTMP中一个概念）匹配myapp时，开启直播
            gop_cache on; #开启GOP（Group of Picture）缓存，播放器解码时，收到一个完整的GOP才会开始播放，这个是减少播放延迟的选项
            pull rtmp://live.hkstv.hk.lxdns.com/live/hks; #如果懒得推流，那可以用这个，香港卫视的直播推流
        }
        #以下配置项已废弃
        application app1 {
            proxy_pass rtmp://host(ip or domain name)[:host]/app2; #将推流反向代理到上游服务器，并将app1自动转化为app2
            #proxy_pass rtmp://backend; #将推流反向代理到上游服务器，见upstream配置
        }
    }
    server {
        listen 1935;
        server_name *.test.com; #或者www.test.*/www.test.com
        application myapp {
            live on;
            gop_cache on;
        }
    }
    #以下配置项已废弃，原因在下文更新中
    upstream backend {
        #开启负载均衡
        server host1:port1;
        server host2:port2;
    }
}
启动Nginx，在vlc播放器中以“网络”方式打开媒体，填入http://localhost/live?stream=hks即可。
通用URL：http://example.com[:port]/dir?[srv=index&app=xxx&]stream=xxx。
如果http配置块里的监听端口不是80（默认），那么必须加上:port，如:8080。
如果rtmp配置块里有多个server配置块，如果想要播放的流的配置是在第二个server配置块中，那么必须加上srv=1（从0开始计数）。
如果rtmp配置块中的某个server块下有多个application配置块，如果想要播放的流的APP（RTMP中的一个概念）的名称是test，那么必须指明app=test，stream对应的是推流的名称。
推流的通用命令：ffmpeg -i -re xxx.mp4（或者与RTMP兼容的媒体文件）-vcodec copy -acodec copy -f flv rtmp://example.com[:port]/app/stream，后面也可以像HTTP的URL那样加参数，目前没仔细研究过，如果想推流到myapp，那么app换成myapp，stream随便取名，播放的时候跟它保持一致就可以。
其他的见nginx-rtmp-module的[wiki](https://github.com/arut/nginx-rtmp-module/wiki/Directives)说明。
测试效果图如下：
![](https://img-blog.csdn.net/20170910031736828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2luc2hpbmluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2017-09-18更新：
反向代理和负载均衡的功能已经基本可用，但是之前并为考虑到如果推流数很多，例如1000路推流，这可能对服务器造成沉重的负担。那为什么HTTP协议使用反向代理和负载均衡没有这个问题呢？那是因为HTTP请求占用的带宽很有限，负载瞬时可能很高，但是不会太持久。
2017-10-07更新：
虚拟主机功能已基本可用，即可以像HTTP配置那样配置server_name了，由于可以通过虚拟主机查找配置，所以不再支持参数srv=index，添加了一个参数port，如果不指定，默认为1935，用于指定以该port查找推流对应的配置。通用URL变为：http://example.com[:port]/dir?[port=xxx&]app=xxx&stream=xxx
2017-11-10更新：
RTMP的302重定向已基本可用，但是由于很多播放器不支持重定向，所以该功能很受限，目前只有JW Player测试通过，VLC无法解析返回的重定向信息，其他播放器没有测试过。关于RTMP的302重定向，可以参考Adobe的官网里的application.redirectConnection部分说明：https://helpx.adobe.com/adobe-media-server/ssaslr/application-class.html。
设置如下：在server块或者application块中添加配置，假设推流时的app为myapp，要重定向到test，保持流的名称不变：
rewrite '^/app/(.*)' '/test/$1';
这样，就可以在本机上将推流或者播放都从app重定向到test上。
如果要推流到其他主机，则可以设置为：
rewrite '^/app/(.*)' rtmp://otherhost:otherport/otherapp/$1;
这样，就可以将本机上的推流或者播放都重定向到其他主机上，这也是一种负载均衡的方法。
PS：不太愿意将rewrite分支merge到master上，毕竟受限太多，功能有点鸡肋。
2017-11-12更新：
今天在笔记本上进行压力测试，用的是srs给的测试工具，而它不支持推mp4文件流，只支持flv格式，结果一测试就出现问题，HTTP方式播放无法正常运行，查了下代码，已经修复bug。
2017-11-22更新：
有网友提到同时使用HTTP和RTMP方式直播时，停止RTMP方式播放会导致HTTP方式播放也停止，这个bug几天前测试的时候已经发现，不过最近由于工作比较忙，没来得及改，今天修复了这个bug。
2017-12-10更新：
评论中有网友指出不知道如何使用HTTP方式播放直播流，可以查看github上的[README.CN.md](https://github.com/winshining/nginx-http-flv-module/blob/master/README.CN.md)，这个文件是中文说明，README.md是英文说明。这两天专门更新了一下这两个文件，没有添加新的功能。测试截图如下，其中网页是用RTMP方式播放，VLC是HTTP方式播放：
![](https://img-blog.csdn.net/20171210202058502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2luc2hpbmluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
插个使用flv.js播放的截图（2018-04-06）：
![](https://img-blog.csdn.net/20180406004548197)
2017-12-30更新：
2017年最后一次更新，由于之前已经提及为什么反向代理和负载均衡在实际生活中不太实用，所以已经把README文件里的反向代理和负载均衡的说明删除了，不过代码还没有删除，后续会陆陆续续删除。对于评论中有网友提到的问题，有些还没修复，我很抱歉，平时上班比较忙，年底连续上了12天班，通宵1晚，所以来不及修复问题。有兴趣的网友可以自己hack代码，代码风格是严格按照nginx的官方要求格式写的，我自认为看着还行，至于有些逻辑问题，我也没搞太清楚，只知道那样写没问题。
最后，最近重写了http-flv直播的功能，组装数据和发送全部使用HTTP的框架，不再使用一些“裸露”的组装数据的方法，如"HTTP/1.1 200 OK"CRLF，发送也使用ngx_http_send_header和ngx_http_output_filter完成，不再使用自定义的发送函数，为什么有这个想法，源于nginx从1.3.9版本后原生支持HTTP的chunked传输，没有必要再自己搞一套组装和发送chunked数据的方法，并且对于非chunked传输，nginx的HTTP模块更不在话下，所以干脆全部用nginx的HTTP框架了。
最后，上面说的代码不会提交了，因为我发现有人fork代码后，又删除了fork，然后在自己的代码里加了些我的项目里的代码，尽管改了变量名什么的，还是看得出痕迹。BSD-2-Clause开源协议本来要求很简单，你修改，再发布甚至商用，LICENSE文件里署上原作者信息即可。这点都办不到，那我也只能小心眼了。
2018-01-02更新：
反向代理和负载均衡的代码已经从master分支删除，vhost分支与master分支代码是一样的，upstream分支还保留有反向代理和负载均衡的代码，有需要的可以查看这个分支，后续不再维护这两个功能。
2018-01-03更新：
感谢一些网友指出nginx-http-flv-module因为nginx的版本变更造成不能编译的问题，目前已经把一些已发现的兼容问题修复了，测试到最旧的nginx版本是1.2.6，考虑到nginx-1.2.6已经是2012年的版本了，所以绝大多数情况下应该不会使用比它更旧的版本，所以不再测试nginx-http-flv-module和更旧的nginx版本的兼容性了。
2018-01-12更新：
最近使用[srs-bench](https://github.com/ossrs/srs-bench)推流测试nginx-http-flv-module的稳定性，发现在播放测试视频第三遍时（偶尔第一遍、第二遍）会出现CPU使用率暴增，nginx不接受任何服务，播放器画面静止不动的问题（我用过的播放器都会出现这问题，所以不是播放器的问题）。经调试，发现是在释放已使用的链表（并不是释放内存，是把内存链表链入一个free指针）时，无限循环了，即已使用的链表形成了环。后来确认是重复释放已使用的链表造成的问题，修改代码后，播放测试视频十几遍（半个多小时）没再出现问题，代码已经更新。谨慎猜测nginx-rtmp-module也有此问题，但是没有测试过。
2018-02-07更新：
有网友提交代码了，包括定时输出日志和FCPublish等命令的处理，代码已经合并。另外有网友在服务器上试用，32GB的内存6个小时耗尽，显然有内存泄漏，目前已经修复。不得不佩服nginx-rtmp-module的原作者，内存链表使用了引用计数器，分配和释放对计数器的操作避免了多次释放造成链表环的形成。还修复了一个因为GOP缓存数目为2时，会造成瞬间发送数据的速率太高，造成播放器来不及接收数据，进而造成播放卡顿的bug。用wireshark抓包可以看出有'TCP Window Full'的问题，经查造成此问题的原因就是播放器来不及接收数据。
2018-02-27更新：
有网友提出想在Windows上运行带有nginx-http-flv-module的nginx，而我之前一直将重心放在Linux上，并且Mac OS X上也能编译通过，但是没怎么测试，昨晚在Windows上编译时，发现好多编译错误，并且如果开启了“chunked on;”配置项，播放会崩溃，现一并修复了这些bug，非常感谢网友们的测试与建议。
由于本文已经很长，后续的内容将在这里更新：《[基于nginx-rtmp-module模块实现的HTTP-FLV直播模块（nginx-http-flv-module）续](http://blog.csdn.net/winshining/article/details/79492099)》。
from:https://blog.csdn.net/winshining/article/details/74910586
