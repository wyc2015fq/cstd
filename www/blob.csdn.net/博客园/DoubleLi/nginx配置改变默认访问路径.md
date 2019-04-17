# nginx配置改变默认访问路径 - DoubleLi - 博客园






在安装完nginx服务后，url访问的默认路径是安装的路径html文件夹下的内容，如果需要指定自定义的路径，需要配置nginx.conf文件内容，这样通过url访问就可以了，比如： http://127.0.0.1/ 对应的物理路径 c:/a/b/c

修改配置文件：





**[html]**[view plain](http://blog.csdn.net/yijunwanghaha/article/details/61420133#)[copy](http://blog.csdn.net/yijunwanghaha/article/details/61420133#)

[print](http://blog.csdn.net/yijunwanghaha/article/details/61420133#)[?](http://blog.csdn.net/yijunwanghaha/article/details/61420133#)

- server {  
-   listen 80;  
-   server_name localhost;  
-   #charset koi8-r;  
-   #access_log logs/host.access.log main;  
-   location / {  
-     #这里设置你的实际路径  
-     root /home/ftpuser/www/;   
-     index index.html index.htm;  
-     }  
- }  




重启nginx再访问，如果访问提示 Nginx 403 Forbidden

![](http://img.blog.csdn.net/20170311164326806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlqdW53YW5naGFoYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

需要在nginx.conf头部加入一行





**[html]**[view plain](http://blog.csdn.net/yijunwanghaha/article/details/61420133#)[copy](http://blog.csdn.net/yijunwanghaha/article/details/61420133#)

[print](http://blog.csdn.net/yijunwanghaha/article/details/61420133#)[?](http://blog.csdn.net/yijunwanghaha/article/details/61420133#)

- user root;  




重启nginx再访问，就可以正常访问了









