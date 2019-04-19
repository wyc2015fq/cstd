# Nginx支持多站点配置小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月01日 10:52:24[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：197
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)

如何配置 web 服务器才能在一个 VPS 上放置多个网站/博客呢？如何通过一个 IP 访问多个站点/域名呢？这是大多数 web 服务器支持的 virtual hosting 功能。即一个IP对应多个域名以支持多站点访问，就像一个IP对应一个站点一样，所以是”虚拟”的。
这里以配置2个站点（2个域名）为例，n 个站点可以相应调整，假设：
IP地址: 202.55.1.100
域名1 example1.com 放在 **/**opt**/**nginx**/**html**/**example1.com
域名2 example2.com 放在 **/**opt**/**nginx**/**html**/**example2.com
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Nginx%E6%94%AF%E6%8C%81%E5%A4%9A%E7%AB%99%E7%82%B9%E9%85%8D%E7%BD%AE%E5%B0%8F%E7%BB%93&action=edit&section=1)]基本思路
首先需要将域名A记录解析至此IP,此时可实现[http://example1.com形式访问，再添加一条cname记录，保证http://www.example1.com这种形式也可以访问]()
参考图:
然后把2个站点的文件 example1.com, example2.com 放到 nginx 可以访问到的目录下，比如/opt/nginx/html
给每个站点分别创建一个 nginx 配置文件 example1.com.conf，example2.com.conf, 并把配置文件放到 /opt/nginx/conf/vhosts/目录下
然后在 /opt/nginx/conf/nginx.conf 里面加一句 include 把步骤2创建的配置文件全部包含进来（用 * 号）
重启 nginx
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Nginx%E6%94%AF%E6%8C%81%E5%A4%9A%E7%AB%99%E7%82%B9%E9%85%8D%E7%BD%AE%E5%B0%8F%E7%BB%93&action=edit&section=2)]具体过程
创建 vhosts 目录 mkdir /opt/nginx/conf/vhosts 在vhosts目录下创建一个名字为 example1.com.conf 的文件，把以下内容拷进去
server **{**
	listen 80;
	server_name example1.com  www.example1.com;
 
	access_log **/**opt**/**nginx**/**html**/**example1.log;
 
	location **/****{**
		root **/**opt**/**nginx**/**html**/**example1.com;
		index index.php index.html;
	**}**
 
	error_page 500502503504**/**50x.html;
	location = **/**50x.html **{**
		root **/**opt**/**nginx**/**html;
	**}**
 
	location ~ \.php$ **{**
		fastcgi_pass   127.0.0.1:9000;
		fastcgi_index  index.php;
		fastcgi_param  SCRIPT_FILENAME  **/**opt**/**nginx**/**html**/**example1.com$fastcgi_script_name;
		include        fastcgi_params;
	**}**
 
	location ~ **/**\.ht **{**
		deny all;
	**}****}**
如果换成你自己的，需要修改4个地方。
在vhosts目录下再创建一个名字为 example2.com.conf 的文件，把以下内容拷进去，或者复制上一个文件修改也可以。
server **{**
	listen 80;
	server_name example2.com  www.example2.com;
 
	access_log **/**opt**/**nginx**/**html**/**example2.log;
 
	location **/****{**
		root **/**opt**/**nginx**/**html**/**example2.com;
		index index.php index.html;
	**}**
 
	error_page 500502503504**/**50x.html;
	location = **/**50x.html **{**
		root **/**opt**/**nginx**/**html;
	**}**
 
	location ~ \.php$ **{**
		fastcgi_pass   127.0.0.1:9000;
		fastcgi_index  index.php;
		fastcgi_param  SCRIPT_FILENAME  **/**opt**/**nginx**/**html**/**example2.com$fastcgi_script_name;
		include        fastcgi_params;
	**}**
 
	location ~ **/**\.ht **{**
		deny all;
	**}****}**
打开/opt/nginx/conf/nginix.conf 文件，在相应位置(放在http的大括号结束前)include以上2个文件
include /opt/nginx/conf/vhosts/*.conf; 
最后重启 Nginx即可
参见[快速安装PHP，MySQL，Nginx和phpmyadmin](http://192.168.1.100/elesos_com/index.php?title=%E5%BF%AB%E9%80%9F%E5%AE%89%E8%A3%85PHP%EF%BC%8CMySQL%EF%BC%8CNginx%E5%92%8Cphpmyadmin&action=edit&redlink=1)
如果想直接访问html下的phpmyadmin，可以
ln -s /opt/nginx/html/phpmyadmin /opt/nginx/html/example1.com/phpmyadmin
当然最好再新建一个站点admin存放phpmyadmin，比如/opt/nginx/html/admin/phpmyadmin,然后通过[http://admin.elesos.com/phpmyadmin/index.php进行访问。](http://admin.elesos.com/phpmyadmin/index.php%E8%BF%9B%E8%A1%8C%E8%AE%BF%E9%97%AE%E3%80%82)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Nginx%E6%94%AF%E6%8C%81%E5%A4%9A%E7%AB%99%E7%82%B9%E9%85%8D%E7%BD%AE%E5%B0%8F%E7%BB%93&action=edit&section=3)]艺搜参考
[http://www.vpsee.com/2009/06/nginx-virtual-hosting-configuration/](http://www.vpsee.com/2009/06/nginx-virtual-hosting-configuration/)
