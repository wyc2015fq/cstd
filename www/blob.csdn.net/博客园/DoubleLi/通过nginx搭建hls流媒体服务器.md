# 通过nginx搭建hls流媒体服务器 - DoubleLi - 博客园






通过录像文件模拟直播源，通过rtmp协议推送到nginx服务器

 nginx 配置文件 增加





**[html]**[view plain](http://blog.csdn.net/mtour/article/details/41418409#)[copy](http://blog.csdn.net/mtour/article/details/41418409#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- rtmp {    
-     server {    
-         listen 1935;    
-        application hls {    
-              live on;    
-              hls on;    
-              hls_path /tmp/app;    
-              hls_fragment 5s;    
- 
-        }    
-     }    
- }    




http 下面增加



**[html]**[view plain](http://blog.csdn.net/mtour/article/details/41418409#)[copy](http://blog.csdn.net/mtour/article/details/41418409#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- location /hls {    
-            #server hls fragments    
-            types{    
-              application/vnd.apple.mpegurl m3u8;    
-              video/mp2t ts;    
-            }    
-         alias /tmp/app;    
-         expires -1;    
-         }    




在 safari浏览器或者vlc中打开 测试地址：  http://192.168.1.105:8080/hls/test.m3u8



可以直播了

![](http://img.blog.csdn.net/20141123213015312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXRvdXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)











