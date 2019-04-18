# PHP环境搭建 - weixin_33985507的博客 - CSDN博客
2011年09月26日 14:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
引用：[http://software.intel.com/zh-cn/blogs/2011/09/22/php/?cid=sw:prccsdn2007](http://software.intel.com/zh-cn/blogs/2011/09/22/php/?cid=sw:prccsdn2007)
首先需要的工具：apache_2.x-win32-x86-no_ssl.msi, PHP5_installer
（官网可以下载）
系统安装过程：
1 安装apache_2.x-win32-x86-no_ssl.msi, 在安装向导界面中的安装路径选 d:\Apache22\ （以下设置都以此为基础）
这个过程比较简单，按照提示进行即可，但注意的是最好选择自定义安装路径；安装成功后任务栏的右下角会有一个图标显示
2 安装PHP5_installer,在执行这一步操作的时候，请先关闭APACHE服务器,方法是邮件任务栏上的图标，选择stop。双击PHP5.x 安装文件 php-5.x-installer，开始进行安装。同样选择自定义选择路径的安装，在选择安装路径（Destination Folder）时，找到 Apache 服务器所在在的文件夹（这里是 d:\Apache22\ )，将 PHP5.x 安装到其下一级文件夹 \cgi-bin 里。最后选择服务器软件类型，勾选Apache，就可以进行安装了。
3、系统文件的配置（关键）
I）修改配置文件httpd.conf（位于文件夹 ..\Apache22\conf\ 内 ）：
将
#Listen *
改成
Listen 你自己计算机的IP地址:80
修改后的应该是这样的：
Listen XXX.XX.XXX.xx:80（XXX.XX.XXX.xx是你的IP地址,这里主要是进行IP绑定）
#Listen 80
再找到
ScriptAlias /cgi-bin/ "d:/Apache22/cgi-bin/"
这一行，在它的下面增加如下几行：
AddType application/x-httpd-php .php
AddType application/x-httpd-php .php3
Action application/x-httpd-php "/cgi-bin/php-cgi.exe"
II） 在C:/WINDOWS/（在系统盘中找，win7是在C:/windows目录下） php.ini 文件，用文字编辑程序如记事本等打开，找到：
extension_dir = ./
这一行，将它改成
extension_dir = d:/Apache22/cgi-bin/ext/
再找到
error_reporting =
这一行，将它改成
error_reporting = E_COMPILE_ERROR|E_ERROR|E_CORE_ERROR
（这里可以不进行修改，这里主要是定义调试时对错误的严格程度，我们的修改是让服务器只报告编译错误和核心错误。）
找到
;extension=php_mbstring.dll
;extension=php_gd2.dll
;extension=php_mysql.dll
这三行，将它们前边的分号去掉，保证以后可以运用 宽字符串、图形库函数 和 MySQL数据库函数。
再在 extension=php_mysql.dll 之后加上一行：
xtension=php_mysqli.dll 保证以后可以按对象的方式操作数据库。
然后保存文件。
4、重新启动Apache服务器，（如果一个dos窗口一闪而过表示apache设置成功）
测试PHP文件，一是看Apache服务器能否处理PHP类型的文件（test.php)，
tesp.php 可以用记事本写一行
保存为test.php即可，然后将此文件复制到D:\Apache22\htdocs下
然后在浏览器输入地址：http://XX.xxx.xx.xx/test.php(XX.xxx.xx.xx为你的IP地址）
当出现一个PHP版本信息的页面就证明安装成功。
