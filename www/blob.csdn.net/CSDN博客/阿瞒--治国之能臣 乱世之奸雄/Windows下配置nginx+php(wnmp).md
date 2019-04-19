# Windows下配置nginx+php(wnmp) - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2017年07月19日 10:10:09[baofenny](https://me.csdn.net/jiaminbao)阅读数：1149标签：[PHP](https://so.csdn.net/so/search/s.do?q=PHP&t=blog)
个人分类：[PHP](https://blog.csdn.net/jiaminbao/article/category/7037851)

第一部分：准备工作。（系统：Windows 8.1）
1.首先是下载软件。
NGINX-1.3.8官网下载：[http://nginx.org/en/download.html](http://nginx.org/en/download.html)
PHP5.4.8版本下载地址：[http://windows.php.net/download/](http://windows.php.net/download/)
Mysql5.5.28版本下载地址：[http://www.mysql.com/downloads/mysql/](http://www.mysql.com/downloads/mysql/)
2.安装mysql软件。
3.解压NGINX和PHP到你自己安装位置。这里我在C盘新建一个文件夹：wnmp(windows,ngnix,myspq,php)，把下面的软件安装到这个文件夹里面。
NGINX目录C:\wnmp\nginx
PHP目录C:\wnmp\php
第二部分：安装nginx
1.打开C:\nginx目录，运行该文件夹下的nginx.exe
2.测试是否启动nginx。打开浏览器访问http://localhost 或 http://127.0.0.1，看看是否出现“Welcome to nginx!”，出现的证明已经启动成功了。没有启动的话，看看80端口有占用没。
注意：该网站的默认目录在“C:\wnmp\nginx\htm”l下
![](http://images.cnitblog.com/blog/312892/201312/27092447-29b571cb23e04b918866519cdf4216ba.png)
第三部分：安装php（这里主要讲nginx配置启动php，以cgi运行php）
nginx配置文件是conf文件夹里的nginx.conf
1.修改大概第43~45行之间的
```
location /{
            root   html;
            index  index.html index.htm;}
```
修改网站文件的路径，以及添加index.php的默认页。
```
location / {
            root   D:/wnmp/nginx-1.5.8/html;
            index  index.html index.htm inde.php;
        }
```
2.支持php的设置
修改大概在第63-71行的
```
# pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}
```
先将前面的“#”去掉，同样将root  html;改为root  C:/wnmp/nginx-1.5.8/html;。再把“/scripts”改为“$document_root”，这里的“$document_root”就是指前面“root”所指的站点路径，这是改完后的：
```
# pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           C:/wnmp/nginx-1.5.8/html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
        #    include        fastcgi_params;
        #}
```
3.C:\wnmp\php\ext下修改php.ini-development文件，将文件名修改为php.ini，打开php配置文件php.ini，保存即可。
搜索“extension_dir”，找到： e;xtension_dir = "ext" 先去前面的分号再改为 extension_dir = "C:\wnmp\php\ext"
搜索“date.timezone”，找到：;date.timezone = 先去前面的分号再改为 date.timezone = Asia/Shanghai
搜索“enable_dl”，找到：enable_dl = Off 改为 enable_dl = On
搜索“cgi.force_redirect” ;cgi.force_redirect = 1 先去前面的分号再改为 cgi.force_redirect = 0
搜索“fastcgi.impersonate”，找到： ;fastcgi.impersonate = 1 去掉前面的分号
搜索“cgi.rfc2616_headers”，找到：;cgi.rfc2616_headers = 0 先去前面的分号再改为 cgi.rfc2616_headers = 1
搜索“php_mysql”，找到：”extension=php_mysql.dll和extension=php_mysqli.dll  去掉前面的“;”extension=php_mysql.dll和extension=php_mysqli.dll   （支持MYSQL数据库）
其他的配置请按照自己的需求更改。
第三部分试运行以及编辑运行配置文件
C:\wnmp\php-5.5.7-nts-Win32-VC11-x86>php-cgi.exe -b 127.0.0.1:9000-c C:\wnmp\php-5.5.7-nts-Win32-VC11-x86\php.ini
重新运行nginx.exe。
C:\wnmp\nginx-1.5.8\html下新建一个phpinfo.php，
```
<?php phpinfo(); ?>
```
访问http://localhost/phpinfo.php
或者http://127.0.0.1/phpinfo.php
出现如下的信息就说明php已经成功安装:
![](http://images.cnitblog.com/blog/312892/201312/27142300-e4a59501ef47469188cd8d24ab3e64f5.png)
下载一个RunHiddenConsole.exe，[百度网盘](http://pan.baidu.com/share/link?shareid=100074&uk=822373947)。
开启php-cgi和nginx.exe，保存为start.bat
```
@echo off
echo Starting PHP FastCGI...
C:\wnmp\nginx\RunHiddenConsole.exe C:\wnmp\PHP\php-cgi.exe -b 127.0.0.1:9000-c D:\PHP\php.ini
echo Starting nginx...
C:\wnmp\nginx\RunHiddenConsole.exe D:/nginx/nginx.exe -p D:/nginx
```
停止php-cgi和nginx.exe，保存为stop.bat
```
@echo off
echo Stopping nginx...
taskkill /F /IM nginx.exe > nul
echo Stopping PHP FastCGI...
taskkill /F /IM php-cgi.exe > nul
exit
```
