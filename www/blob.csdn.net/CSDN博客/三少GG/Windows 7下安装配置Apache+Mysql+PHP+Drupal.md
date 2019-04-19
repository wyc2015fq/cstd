# Windows 7下安装配置Apache+Mysql+PHP+Drupal - 三少GG - CSDN博客
2011年02月08日 13:19:00[三少GG](https://me.csdn.net/scut1135)阅读数：3621
**1.简单版：**
自己可以下载个Apache+PHP+MySQL+Perl 服务器套件 好像现在出了个 phpnow环境安装集成软件 可以关注下！
**2.简洁版：**
[http://jhyimu2005.javaeye.com/blog/772234](http://jhyimu2005.javaeye.com/blog/772234)
说出来不怕笑话，我是第一次学习php，当然首先是搭建环境，一直都是做java开发，一直在想有没有一个像MyEclipse的开发环境，直接安装什么都不要用设置，结果在网上找了好多资料例如zend studio8.0或者zend studio8.0 for eclipse还有notePad++，我都试过把php项目放在apache下运行，结果人家不解析php语法，这下我就郁闷了也空欢喜一场，突然想到了写java代码需要jdk，php是不是也需要类似的东西，在网上一下果然是这样，自己参考了一篇文章（http://www.jzxue.com/fuwuqi/http-iis-apache/201009/10-4550_2.html）一步步的安装，功夫不负有心人，最后终于测试成功，废话少说，言归正传，一下是它的安装步骤，附件中包括了所有的安装软件：
准备条件： 
Apache 2.2.16 安装版(httpd-2.2.16-win32-x86-no_ssl.msi) 
php-5.2.14 解压版php-5.2.14 
Mysql 5.1.x 安装或解压版都可以，只要能正常启动 
1.首先将文件php-5.2.14-Win32.zip解压到D:/Apache2.2； 安装完毕后测试安装是否成功，在浏览器下输入http://localhost/回车，如果出现It works!说明Apache 2.2安装成功，并且能正常使用！(如果不能正常启动，很可能是端口号被别的占用了，此时你可以在D:/Apache2.2/conf下找到文件httpd.conf将Listen 80改为Listen 81再在浏览器中输入http://localhost:81/）
2.然后将php-5.2.14-Win32.zip压缩包解压到D:/php-5.2.14-Win32，找到php-5.2.14-Win32文件夹下的php.ini-dist(，将这个文件的后缀名改成php.ini，然后打开php.ini,找到extension_dir这一行，并将这一行修改成extension_dir = "D:/php-5.2.14-Win32/ext"并将前面的分号去掉(注意前面不能有空格，将路径中的/要改成/,否则会出错)。打开所需要的PHP扩展，将extension=php_bz2.dll，extension=php_fileinfo.dll，extension=php_gd2.dll，extension=php_mysql.dll，
extension=php_mysqli.dll前面的分号去掉(注意前面不能有空格)，如果还要开其它扩展，那么将其它扩展前面的分号去掉，就可以了。这时候，PHP环境基本配置完成。
php5时差问题 
<?php echo date("Y-m-d H:i:s");?>时间相差八小时 
为什么呢?PHP5系列版本新增了时区设置，默认为格林威治时间，与中国所在的东8区正好相差8个小时 
查找date.timezone有这么一行 
;date.timezone = 
将;去掉，改成 
date.timezone = PRC 
接下来配置Apache 2.2，让Apache 2.2与php关联起来。到D:/Apache2.2/conf目录下找到Apache配置文件httpd.conf，打开配置文件，在
#LoadModule vhost_alias_module modules/mod_vhost_alias.so这一行的下面输入: 
LoadModule php5_module D:/php-5.2.14-Win32/php5apache2_2.dll 
PHPIniDir "D:/php-5.2.14-Win32" 
AddType application/x-httpd-php .php .phtml 
重启Apache，这样就可以和php关联起来了。(注意每次修改Apache或者PHP配置文件时都要重启Apache) 
3.最后安装Mysql 5.1.x就可以了(MySql与PHP没有任何关联) 
本来想把软件添加在附件中结果发现太大了，你们根据上面的说明去相应的官方网站上下载也可以，速度还挺快的 
**（3）完整版（推荐）：**
[http://hi.baidu.com/ljping615/item/eacec206c64fbb16cc34ea14](http://hi.baidu.com/ljping615/item/eacec206c64fbb16cc34ea14)
41
## PHP环境搭建:Windows 7下安装配置PHP（5.3.2）+Mysql（5.1.46）环境教程(win7自带iis7.5功能搭建)
[推荐几个php安装环境包下载](http://hi.baidu.com/ljping615/blog/item/bdf97e017b1e7b0b1d9583e0.html)
![](http://hiphotos.baidu.com/ljping615/pic/item/a28f191b94764ec6af513399.jpg)
PHP环境搭建:Windows 7下安装配置PHP（5.3.2）+Mysql（5.1.46）环境教程(win7自带iis7.5功能搭建)
这两天刚装好Windows 7，碰巧前段时间有朋友问我Windows下如何安装搭建PHP环境，所以打算勤劳下，手动一步步搭建PHP环境，暂且不使用PHP环境搭建软件了，在此详细图解在Windows 7下安装配置PHP+Apache+Mysql环境的教程，希望对PHP初学者有所帮助。
　　在Windows 7下进行PHP环境搭建，首先需要下载PHP代码包和Apache与Mysql的安装软件包。
　　PHP版本：php-5.3.2-Win32-VC6-x86，VC9是专门为IIS定制的，VC6 是为了其他WEB服务软件提供的，如 Apache。我下载的是ZIP包，[下载地址](http://windows.php.net/download/)
　　Mysql版本：mysql-essential-5.1.45-win32，即MySQL Community Server 5.1.45，[下载地址](http://dev.mysql.com/downloads/)
　　Apache版本：httpd-2.2.15-win32-x86-openssl-0.9.8m-r2，openssl表示带有openssl模块，利用openssl可给Apache配置SSL安全链接。[下载地址](http://httpd.apache.org/download.cgi)
下载地址：
php：[http://windows.php.net/download/](http://windows.php.net/download/)（[http://windows.php.net/downloads/releases/php-5.3.2-Win32-VC9-x86.msi](http://windows.php.net/downloads/releases/php-5.3.2-Win32-VC9-x86.msi)）
mysql：[http://www.mysql.com/downloads/mysql/](http://www.mysql.com/downloads/mysql/)（[http://dev.mysql.com/get/Downloads/MySQL-5.1/mysql-5.1.46-win32.msi/from/ftp://ftp.ntu.edu.tw/pub/MySQL/](http://dev.mysql.com/get/Downloads/MySQL-5.1/mysql-5.1.46-win32.msi/from/ftp://ftp.ntu.edu.tw/pub/MySQL/)
或者[http://dev.mysql.com/get/Downloads/MySQL-5.1/mysql-5.1.46-win32.msi/from/http://mysql.ntu.edu.tw/](http://dev.mysql.com/get/Downloads/MySQL-5.1/mysql-5.1.46-win32.msi/from/http://mysql.ntu.edu.tw/)）
**PHP环境搭建第一步：Windows 7下安装Apache服务。**
![Apache配置信息](http://hiphotos.baidu.com/ljping615/pic/item/252dd42a918e90a7033bf693.jpg)
Apache配置信息
　　在这里主要配置Network Domain、Server Name、Email地址以及Apache服务的占用端口，默认为80端口，你可以根据需要配置在其他端口，Apache的安装目录你可以使用默认目录或根据需要自行选择安装目录。
　　在完成apache服务的安装后，在游览器中输入http://localhost/，出现It’s work!字样，说明Apache服务安装成功了。
**PHP环境搭建第二步：Windows 7下安装Mysql服务。**
![安装Mysql数据库并选择安装目录](http://hiphotos.baidu.com/ljping615/pic/item/2834349bb881ff89c9eaf493.jpg)
安装Mysql数据库并选择安装目录
　　点击Mysql安装程序自动安装，在此期间你可以根据需要选择Mysql数据库的安装目录，我一律都是用默认目录。
**注意**：在安装完Mysql数据库后，需要再进行Mysql数据库配置才能使用PHP进行连接，稍后会提到如何配置。
**PHP环境搭建第三步：在Windows 7下安装PHP。**
　　其实在Windows 7下进行PHP安装非常简单，由于我下的是PHP代码包，只要解压php-5.3.2-Win32-VC6-x86并重名为文件夹为php，将其复制到C盘目录下即可完成PHP安装。
**PHP环境搭建第四步：在Windows 7下如何进行PHP配置环境。**
　　PHP环境在Windows 7上的配置相比Windows XP等要简单很多，不需要复制等操作，你只要将php.ini-development配置文件重命名为php.ini配置文件即可。接着做如下配置操作：
1、打开php.ini配置文件，找到
1
2 ; On windows:
; extension_dir = "ext"
修改为
1
2 ; On windows:
extension_dir = "C:/php/ext"
表示指定PHP扩展包的具体目录，以便调用相应的DLL文件。
2、由于默认PHP并不支持自动连接Mysql，需开启相应的扩展库功能，比如php_mysql.dll等，即将
1
2
3
4
5
6
7 extension=php_curl.dll
extension=php_gd2.dll
extension=php_mbstring.dll
extension=php_mysql.dll
extension=php_pdo_mysql.dll
extension=php_pdo_odbc.dll
extension=php_xmlrpc.dll
这些extension之前的分号(;)去除。
3、配置PHP的Session功能
　　在使用session功能时，我们必须配置session文件在服务器上的保存目录，否则无法使用session，我们需要在Windows 7上新建一个可读写的目录文件夹，此目录最好独立于WEB主程序目录之外，此处我在D盘根目录上建立了phpsessiontmp目录，然后在php.ini配置文件中找到
1 ;session.save_path = "/tmp"
修改为
1 session.save_path = "D:/phpsessiontmp"
4、配置PHP的文件上传功能 [如何编写PHP文件上传功能?](http://www.leapsoul.cn/?tag=php%E6%96%87%E4%BB%B6%E4%B8%8A%E4%BC%A0)
　　同session一样，在使用PHP文件上传功能时，我们必须要指定一个临时文件夹以完成文件上传功能，否则文件上传功能会失败，我们仍然需要在Windows 7上建立一个可读写的目录文件夹，此处我在D盘根目录上建立了phpfileuploadtmp目录，然后在php.ini配置文件中找到
1 ;upload_tmp_dir =
修改为
1 upload_tmp_dir = "D:/phpfileuploadtmp"
5、修改date.timezone，否则在执行phpinfo时date部分会报错：
Warning: phpinfo() [function.phpinfo]…
我们需要将
1 ;date.timezone =
修改为
1 date.timezone = Asia/Shanghai
　　你也可以点击参考更多关于[PHP.INI的配置](http://www.leapsoul.cn/?tag=phpini%E9%85%8D%E7%BD%AE)
　　至此在Windows 7上php的环境配置就算完成了，但是光完成这些配置是不够的，我们需要Apache支持PHP，所以还需要在Apache配置文件中完成相应的PHP配置。
**PHP环境搭建第五步：配置Apache以支持PHP**
1、在#LoadModule vhost_alias_module modules/mod_vhost_alias.so下添加
1
2
3 LoadModule php5_module "c:/php/php5apache2_2.dll"
PHPIniDir "c:/php"
AddType application/x-httpd-php .php .html .htm
　　我们在PHP目录下可以看到多个php5apache的DLL文件，由于我们使用的是Apache2.2.15，所以我们当然需要使用php5apache2_2.dll，接着指定PHP的安装目录以及执行的程序扩展名。
2、我们应该知道默认Apache服务器执行WEB主程序的目录为Apache2.2/htdocs，所以当你的WEB主程序目录变更时，我们需要修改相应的Apache配置，即将
1 DocumentRoot "C:/Program Files/Apache Software Foundation/Apache2.2/htdocs"
修改为
1 DocumentRoot "D:/PHPWeb" 1 <Directory "C:/Program Files/Apache Software Foundation/Apache2.2/htdocs">
修改为
1 <Directory "D:/PHPWeb">
3、最后修改具体的index文件先后顺序，由于配置了PHP功能，当然需要index.php优先执行
1 DirectoryIndex index.html
修改为
1 DirectoryIndex index.php index.html
4、重启Apache服务器
　　至此，在Apache服务器上PHP环境配置工作就完成了，你只需要在D:/PHPWeb目录下新建一个PHP文件，写入
1
2
3 <?
[phpinfo](http://www.php.net/phpinfo)();
?>
　　然后在游览器中输入http://localhost，就可以看到PHP的具体配置页面了，代表在Window 7上PHP环境配置工作就算完成了。
　　在完成Windows 7上的PHP环境配置工作后，我们需要完成PHP环境搭建的最后一步，即支持Mysql数据库。
　　首先需要配置Mysql服务器。
　　点击开始菜单下Mysql Server5.1菜单中的Mysql Server Instance Config Wizard，完成Mysql的配置向导。
![选择Mysql服务器的类型](http://hiphotos.baidu.com/ljping615/pic/item/033b5bb57450b1f236d3ca93.jpg)
选择Mysql服务器的类型
　　个人认为第一和第二项都可选择，如果仅仅只是作为WEB数据库，推荐选择第二项。
![Mysql数据库用途](http://hiphotos.baidu.com/ljping615/pic/item/c9ea15ce89697105b600c893.jpg)
Mysql数据库用途
![选择Mysql数据库的并发连接数](http://hiphotos.baidu.com/ljping615/pic/item/36d3d53977ba9ecf3a87ce93.jpg)
选择Mysql数据库的并发连接数
　　选择Mysql的并发连接数，第一项是最大20个连接并发数，第二项是最大500个并发连接数，最后一种是自定义，你可以根据自己的需要选择。
![选择Mysql服务的端口号,一般默认即可](http://hiphotos.baidu.com/ljping615/pic/item/b6003af3f93d4d6c352acc93.jpg)
选择Mysql服务的端口号，一般默认即可
![选择Mysql数据库的字符集](http://hiphotos.baidu.com/ljping615/pic/item/3a87e950f09062601038c293.jpg)
选择Mysql数据库的字符集
　　此处推荐使用UTF8，比较通用，否则容易造成乱码。
![设置Mysql为Windows服务](http://hiphotos.baidu.com/ljping615/pic/item/352ac65cd782f77ffbf2c093.jpg)
设置Mysql为Windows服务
　　在这里考虑开机速度的问题，我将自动登录Mysql服务取消了，一般你可以选择此选项，如果没有选，你可以使用net start mysql启动Mysql服务。
![设置Mysql数据库root用户的密码](http://hiphotos.baidu.com/ljping615/pic/item/103853433248162d9313c693.jpg)
设置Mysql数据库root用户的密码
![执行Mysql服务配置选项](http://hiphotos.baidu.com/ljping615/pic/item/fbf2b21158a91442ca80c493.jpg)
执行Mysql服务配置选项
　　Mysql数据库的配置文件保存在C:\Program Files\MySQL\MySQL Server 5.1\my.ini，如果今后有什么变动，你可以修改此文件。
　　至此Mysql数据库的配置就算完成了，为了验证PHP是否能连接Mysql，你可以在index.php中创建如下代码
1
2
3
4
5
6 <?php
$connect=[mysql_connect](http://www.php.net/mysql_connect)(“127.0.0.1″,”root”,”你的[mysql](http://www.php.net/mysql)数据库密码”);
if(!$connect) [echo](http://www.php.net/echo) “[Mysql](http://www.php.net/mysql) Connect Error!”;
else [echo](http://www.php.net/echo) “欢迎访问PHP网站开发教程网-www.leapsoul.cn”;
[mysql_close](http://www.php.net/mysql_close)();
?>
　　然后在游览器中输入http://localhost/，看到：*欢迎访问PHP网站开发教程网-www.leapsoul.cn*字样就说明PHP连接Mysql就算成功了。
**Windows 7下无法使用localhost连接MYSQL5.3的解决方法**
　　在Windows 7中PHP连接Mysql时默认只能使用IP地址连接Mysql，而无法使用localhost连接Mysql，解决方法为打开C:\Windows\System32\drivers\etc下的hosts文件，将
1 # 127.0.0.1 localhost
中的注释去除即可。
安装php5.3.2
1. 安装IIS v7.5
测试是否成功安装IIS v7.5，在浏览器输入：[http://localhost/](http://localhost/)或者[http://127.0.0.1/](http://127.0.0.1/)
出现如上界面说明成功安装IIS v7.5
2. 配置PHP环境
安装php-5.3.2-Win32-VC9-x86.msi（如果你安装的这个你就大功告成了，这是最不费时的做法）
如果你下载的是php-5.3.2-Win32-VC9-x86.zip你就要按照以下步骤来安装：
我举例我把这个解压缩到了：D:\phpsoft\php
在IIS中如下操作：
1在FastCGI里找到php安装路径（php-cgi.exe）
2添加模块映射
点击请求限制
3添加默认文档
完成配置
将D:\phpsoft\php\php.ini-development复制一份
将复制的文件名称修改为php.ini（如果你觉得这样得到php.ini麻烦就下载我所用的吧：地址）
修改php.ini内容：
找到default_socket_timeout = 60在其后面添加
upload_tmp_dir=”D:\phpsoft\upload”
session.save_path=”D:\phpsoft\session”
error_log=”D:\phpsoft\errors\php-errors.log”
cgi.force_redirect=0
fastcgi.impersonate=1
fastcgi.logging=0
extension_dir=”D:\phpsoft\php\ext”
date.timezone = Asia/Shanghai
找到;extension=php_zip.dll在其后面添加
extension=php_bz2.dll
extension=php_curl.dll
extension=php_gd2.dll
extension=php_gettext.dll
extension=php_gmp.dll
extension=php_imap.dll
extension=php_mbstring.dll
extension=php_mysql.dll
extension=php_mysqli.dll
extension=php_openssl.dll
extension=php_pdo_mysql.dll
extension=php_pdo_odbc.dll
extension=php_pdo_sqlite.dll
extension=php_pgsql.dll
extension=php_soap.dll
extension=php_sockets.dll
extension=php_sqlite3.dll
extension=php_tidy.dll
extension=php_xmlrpc.dll
extension=php_exif.dll
添加完成后保存php.ini
就完成了和自动安装php-5.3.2-Win32-VC9-x86.msi一样的效果了
测试安装效果
在默认目录新建一个index.php
输入[http://127.0.0.1/](http://127.0.0.1/)校验是否成功安装php ：
　　OK，到这，在Windows 7上使用PHP+Apache+Mysql完成PHP环境搭建的工作就算完成了，我们可以看到相比在其他Windows平台上搭建PHP环境节省了不少工作，但是也要注意到在Windows 7上连接Mysql5.3数据库的问题。当然如果向我一样比较懒，你可以使用Appserv或者Xampp来进行PHP环境配置
几个php安装环境包下载
phpnow xampp ESSamp Vertrigo
![](http://hiphotos.baidu.com/ljping615/pic/item/a28f191b94764ec6af513399.jpg)
下载地址:
[phpnow](http://cn.ziddu.com/download/495085/PHPnow.rar.html)
[xampp](http://cn.ziddu.com/download/495080/xampp-win32-1.7.3.exe.html)
[ESSamp](http://cn.ziddu.com/download/495084/PHP_______2.0_____________________________________.exe.html)
[Vertrigo](http://cn.ziddu.com/download/495081/php_Vertrigo.exe.html)
[ZendOptimizer-3.3 是Apache的一个插件](http://cn.ziddu.com/download/495083/ZendOptimizer-3.3.0a-Windows-i386.exe.html)
[phpnow xampp ESSamp Vertrigo](http://hi.baidu.com/ljping615/blog/item/bdf97e017b1e7b0b1d9583e0.html)
[#站长手册](http://hi.baidu.com/tag/%E7%AB%99%E9%95%BF%E6%89%8B%E5%86%8C/feeds)
