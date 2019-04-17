# Nginx在centos6.5下的安装和反向代理简单配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年05月25日 16:36:52[boonya](https://me.csdn.net/boonya)阅读数：7353








﻿﻿

From:[http://blog.csdn.net/luckykapok918/article/details/44258901](http://blog.csdn.net/luckykapok918/article/details/44258901)

查看linux的版本信息：

lsb_release -a


![](https://img-blog.csdn.net/20150314152230998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


1.首先卸载原有的nginx：

yum remove nginx

2.安装nginx在内网的centos6.5服务器192.168.200.123上：

yum install nginx


3.运行nginx

cd /usr/sbin

./nginx

![](https://img-blog.csdn.net/20150314144055383?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

或者执行  service nginx start



4.，停止、重启nginx

/usr/sbin/nginx -s stop  或者执行 service nginx stop


/usr/sbin/nginx -s reload


5.检查nginx运行情况

5.1  ps -ef | grep nginx


![](https://img-blog.csdn.net/20150314144812122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5.2 
浏览器中测试

![](https://img-blog.csdn.net/20150314144941132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


6.查看80端口是否启用

![](https://img-blog.csdn.net/20150314144435328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





7.配置反向代理，：

  反向代理说明：nginx服务器地址192.168.200.123，端口80 ；tomcat服务器地址192.168.200.121端口8080；apache服务器地址192.168.200.121,端口80.

现在需要访问nginx的80端口来访问tomcat中的web服务。

7.1 配置前访问：

![](https://img-blog.csdn.net/20150314150848065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





7.2配置nginx并重启服务：


 cd  到 /etc/nginx/conf.d文件夹

修改default.conf文件

增加如下内容：





**[html]**[view plain](http://blog.csdn.net/luckykapok918/article/details/44258901#)[copy](http://blog.csdn.net/luckykapok918/article/details/44258901#)[print](http://blog.csdn.net/luckykapok918/article/details/44258901#)[?](http://blog.csdn.net/luckykapok918/article/details/44258901#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- server{  
- listen          80;  
- server_name 192.168.200.123;  
- 
-    location / {  
-         root   /usr/share/nginx/html;  
-         index  index.html index.htm;  
-     }  
- 
- location /wmsnew {  
- proxy_set_header Host $host;  
- proxy_set_header X-Real-IP $remote_addr;  
- proxy_set_header Destination $http_destination;  
- proxy_pass http://192.168.200.121:8080/wmsnew/;  
- }  
- location /wmsnew/static {  
- proxy_set_header Host $host;  
- proxy_set_header X-Real-IP $remote_addr;  
- proxy_set_header Destination $http_destination;  
- proxy_pass http://192.168.200.121/static;  
- }  
- 
- }  


```
server{
listen          80;
server_name 192.168.200.123;

   location / {
        root   /usr/share/nginx/html;
        index  index.html index.htm;
    }

location /wmsnew {
proxy_set_header Host $host;
proxy_set_header X-Real-IP $remote_addr;
proxy_set_header Destination $http_destination;
proxy_pass http://192.168.200.121:8080/wmsnew/;
}
location /wmsnew/static {
proxy_set_header Host $host;
proxy_set_header X-Real-IP $remote_addr;
proxy_set_header Destination $http_destination;
proxy_pass http://192.168.200.121/static;
}

}
```





如果123服务器启动了防火墙，则需要单独为80端口开发





**[html]**[view plain](http://blog.csdn.net/luckykapok918/article/details/44258901#)[copy](http://blog.csdn.net/luckykapok918/article/details/44258901#)[print](http://blog.csdn.net/luckykapok918/article/details/44258901#)[?](http://blog.csdn.net/luckykapok918/article/details/44258901#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <spanstyle="font-size:18px;color:#3366ff;">#修改防火墙配置：   
- [root@admin nginx-1.2.6]# vi + /etc/sysconfig/iptables  
- #添加配置项   
- -A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT  
- #重启防火墙   
- [root@admin nginx-1.2.6]# service iptables restart</span>


```
<span style="font-size:18px;color:#3366ff;">#修改防火墙配置： 
[root@admin nginx-1.2.6]# vi + /etc/sysconfig/iptables
#添加配置项 
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
#重启防火墙 
[root@admin nginx-1.2.6]# service iptables restart</span>
```




7.3 配置后访问：





![](https://img-blog.csdn.net/20150314151825277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


配置成功！！

--------------------------------------------------------------补充源码编码安装nginx方式------------------------------------------------



### 安装说明




操作系统：centos6.3


软件：nginx-1.2.6.tar.gz


安装方式：源码编码安装


安装位置：/usr/local/nginx


nginx下载地址：http://nginx.org/en/download.html

### 准备工具


在安装nginx衫，需要确保系统已经安装了g++、gcc、openssl-devel、pcre-devel和zlib-devel软件。
- yum install gcc-c++ 
- yum -y install zlib zlib-devel openssl openssl--devel pcre pcre-devel 


检查系统安装的ningx
- find -name nginx 
- ./nginx 
- ./nginx/sbin/nginx 
- ./nginx-1.2.6/objs/nginx 


卸载系统原有的nginx
- yum remove nginx 

### centos安装nginx


将安装包文件上传到目录中，本文是上传到/usr/local中，按以下命令进行操作。
- [root@admin local]# cd /usr/local 
- [root@admin local]# tar -zxv -f nginx-1.2.6.tar.gz 
- [root@admin local]# rm -rf nginx-1.2.6.tar.gz 
- [root@admin local]# mv nginx-1.2.6 nginx 
- [root@admin local]# cd /usr/local/nginx 
- [root@admin nginx]# ./configure --prefix=/usr/local/nginx 
- [root@admin nginx]# make 
- [root@admin nginx]# make install 






