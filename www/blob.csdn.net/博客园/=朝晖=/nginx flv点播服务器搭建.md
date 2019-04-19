# nginx flv点播服务器搭建 - =朝晖= - 博客园
# [nginx flv点播服务器搭建](https://www.cnblogs.com/dhcn/p/7125167.html)
       首先选用Nginx+Nginx-rtmp-module作为点播服务器，安装文章：https://www.atlantic[.NET](http://lib.csdn.net/base/dotnet)/community/howto/install-rtmp-ubuntu-14-04/
       配置如下:
- rtmp {  
- 
-     server {  
- 
-         listen 1935;  
- 
-         chunk_size 4000;  
-         application vod {  
-             play /var/flvs;  
-         }  
-     }  
- }  
       配置说明:flv文件必须放在/var/flvs目录下，放在子目录则无效，访问路径是rtmp://domainname/vod/flvfilename
       以上这个方案可以支持vod点播，但是不支持拖拽时间点播放，因为rtmp是实时播放协议，不支持那个，要支持，就整http flv。
可以参照以下文档：
http://www.cnblogs.com/wanghetao/archive/2013/11/11/3418744.html
[http://www.cnblogs.com/jerrychen/p/4705019.html](http://www.cnblogs.com/jerrychen/p/4705019.html)
[http://blog.csdn.net/vblittleboy/article/details/40616405](http://blog.csdn.net/vblittleboy/article/details/40616405)
