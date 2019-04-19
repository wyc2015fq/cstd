# Nginx搭建HTTPS服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月29日 18:25:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1010
HTTPS(Hyper Text Transfer Protocol Secure)，是一种基于SSL/TLS的HTTP，Https使用的默认端口是443。
## 检查环境
首先确保机器上安装了openssl和openssl-devel
rpm -qa**|****grep** openssl*#yum install openssl**#yum install openssl-devel*
确认nginx是否安装了SSL模块，如下的命令：
**/**opt**/**nginx**/**sbin**/**nginx -V
看是否输出--with-http_ssl_module，如果没有需要重新配置并安装下。./configure --help
## 创建证书
*#cd /opt/nginx/conf**#openssl genrsa -des3 -out cert.key 2048  //创建服务器私钥**#openssl req -new -key cert.key -out cert.csr  //签名请求的证书*
会给出如下的提示:
Enter pass phraseforcert.key:You are about to be asked to enter information that will be incorporatedinto your certificate request.What you are about to enter is what is called a Distinguished Name or a DN.There are quite a few fields but you can leave some blankFor some fields there will be a default value,If you enter'.', the field will be left blank.-----Country Name **(**2 letter code**)****[**XX**]**:CN
State or Province Name **(**full name**)****[**Beijing
Locality Name **(**eg, city**)****[**Default City**]**:Beijing
Organization Name **(**eg, company**)****[**Default Company Ltd**]**:demo
Organizational Unit Name **(**eg, section**)****[****]**:localhost
Common Name **(**eg, your name or your server'shostname) []:localhost
Email Address []:[demo@abc.com](mailto:demo@abc.com)
Please enter the following'extra'attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:
冒号后面是需要我们填写的，最后两个我都没有填。带空的中括号似乎可以选填。
制作解密后的私钥
先将cert.key文件复制一份为cert.key.org
**cp** cert.key cert.key.org*#openssl rsa -in cert.key.org -out cert.key*接下来，最后一步，用cert.csr和cert.key生成cert.crt文件*#openssl x509 -req -days 365 -in cert.csr -signkey cert.key -out server.crt*
## 配置nginx
必须在server配置块中打开SSL协议，还需要指定服务器端证书和密钥文件的位置,打开conf/vhosts下面的配置文件：
server **{**	listen 443;	
	server_name pay.elesos.com  [www.pay.elesos.com;](/)
	access_log **/**opt**/**nginx**/**logs**/**pay.elesos.log;ssl on;
	ssl_certificate **/**opt**/**nginx**/**conf**/**vhosts**/**cert.crt;
	ssl_certificate_key **/**opt**/**nginx**/**conf**/**vhosts**/**cert.key;
	ssl_session_timeout 10m;
        ssl_session_cache    shared:SSL:10m;
	ssl_protocols  SSLv2 SSLv3 TLSv1;
	ssl_ciphers  HIGH:**!**aNULL:**!**MD5;
	ssl_prefer_server_ciphers   on;
 
	location **/****{**		root **/**mnt**/**html**/****test**;
		index index.php index.html;**    }**
 
	error_page 500 502 503 504**/**50x.html;
	location = **/**50x.html **{**		root **/**opt**/**nginx**/**html;**}**	location ~ \.php$ **{**		fastcgi_pass   127.0.0.1:9000;
		fastcgi_index  index.php;
		fastcgi_param  SCRIPT_FILENAME  **/**mnt**/**html**/****test**$fastcgi_script_name;
		include        fastcgi_params;**}**
 
	location ~ **/**\.ht **{**		deny all;**}****}**
SSL操作需要消耗CPU资源，所以在多处理器的系统，需要启动多个工作进程，而且数量需要不少于可用CPU的个数。最消耗CPU资源的SSL操作是SSL握手，有两种方法可以将每个客户端的握手操作数量降到最低：第一种是保持客户端长连接，在一个SSL连接发送多个请求，第二种是在并发的连接或者后续的连接中重用SSL会话参数，这样可以避免SSL握手的操作。会话缓存用于保存SSL会话，这些缓存在工作进程间共享，可以使用ssl_session_cache指令进行配置。1M缓存可以存放大约4000个会话。默认的缓存超时是5分钟，可以使用ssl_session_timeout加大它。
如果HTTP和HTTPS虚拟主机的功能是一致的，可以配置一个虚拟主机，既处理HTTP请求，又处理HTTPS请求。 配置的方法是删除ssl on的指令，并在*:443端口添加参数ssl：
server **{**    listen              80;
    listen              443 ssl;
    server_name         [www.example.com;](/)
    ssl_certificate     [www.example.com.crt;](/)
    ssl_certificate_key [www.example.com.key;](/)
    ...
**}**
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Nginx%E6%90%AD%E5%BB%BAHTTPS%E6%9C%8D%E5%8A%A1%E5%99%A8&action=edit&section=4)]遇到的问题
配置nginx后，启动时报unknown directive "ssl"，原因是：没有将ssl模块编译进nginx, 在configure的时候加上“--with-http_ssl_module”即可
## 最后推荐沃通的免费证书服务： [https://freessl.wosign.com/](https://freessl.wosign.com/)
可以用3年，真心不错，给他做个推广
## 艺搜参考
[http://ustb80.blog.51cto.com/6139482/1339048](http://ustb80.blog.51cto.com/6139482/1339048)
[http://nginx.org/cn/docs/http/configuring_https_servers.html](http://nginx.org/cn/docs/http/configuring_https_servers.html)
[http://blog.csdn.net/wzy_1988/article/details/8522511](http://blog.csdn.net/wzy_1988/article/details/8522511)
[http://www.cnblogs.com/tintin1926/archive/2012/07/12/2587311.html](http://www.cnblogs.com/tintin1926/archive/2012/07/12/2587311.html)
