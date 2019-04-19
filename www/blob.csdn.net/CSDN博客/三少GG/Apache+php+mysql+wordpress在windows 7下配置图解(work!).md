# Apache+php+mysql+wordpress在windows 7下配置图解(work!) - 三少GG - CSDN博客
2010年12月06日 02:13:00[三少GG](https://me.csdn.net/scut1135)阅读数：2984
步骤详述：
注：以下操作假设把 apache + php + sql + blog 安装在 d:/website（看情况，融合了两篇文章）中
**需要准备的软件：**
1.Apache官方下载地址：httpd-2.2.17-win32-x86-no_ssl.msi
2.php官方下载地址：php-5.2.14-Win32.zip
3.mysql官方下载地址：mysql-essential-5.1.53-win32.msi
一：安装 Apache 并进行配置
　　从 Apache官方网站下载的 Apache 2.053 For Win32 有两种格式，一种是 MSI 的安装文件；一种是 ZIP 压缩包。我选的是 MSI 格式的安装文件。
**Grant:安装过程图片参考！！**[**http://tech.163.com/06/0206/11/299AMBLT0009159K.html**](http://tech.163.com/06/0206/11/299AMBLT0009159K.html)
**如果你的网速连接这些英文网站很慢，请自行到Google搜索下载**
下一篇：[php安装配置](http://www.fruit0769.com/?p=992)[MySQL安装配置](http://www.fruit0769.com/?p=999)
**一、安装Apache**
运行下载好的“apache_2.2.10-win32-x86-no_ssl.msi”，出现如下界面：
![](http://www.fruit0769.com/wp-content/uploads/2008/10/01.jpg)
出现Apache HTTP Server 2.2.10的安装向导界面，点“Next”继续
![](http://www.fruit0769.com/wp-content/uploads/2008/10/02.jpg)
确认同意软件安装使用许可条例，选择“I accept the terms in the license agreement”，点“Next”继续
![](http://www.fruit0769.com/wp-content/uploads/2008/10/03.jpg)
将Apache安装到Windows上的使用须知，请阅读完毕后，按“Next”继续
![](http://www.fruit0769.com/wp-content/uploads/2008/10/04.jpg)
设置系统信息，在Network Domain下填入您的域名（本地测试填：127.0.0.1），在Server Name下填入您的服务器名称（本地测试填：127.0.0.1），在Administrator’s Email Address下填入系统管理员的联系电子邮件地址。下面有两个选择，图片上选择的是为系统所有用户安装，使用默认的80端口，并作为系统服务自动启动；另外一个是仅为当前用户安装，使用端口8080，手动启动。**一般选择如图所示（最好选默认的80端口）。**按“Next”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/05.jpg)
选择安装类型，Typical为默认安装，Custom为用户自定义安装，我们这里选择Custom，有更多可选项。按“Next”继续
![](http://www.fruit0769.com/wp-content/uploads/2008/10/06.jpg)
出现选择安装选项界面，如图所示，左键点选“Apache HTTP Server 2.2.10”，选择“This feature, and all subfeatures, will be installed on local hard drive.”，即“此部分，及下属子部分内容，全部安装在本地硬盘上”。点选“Change…”，手动指定安装目录。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/07.jpg)
我这里选择安装在“D:/Apache2.2”，各位自行选取了，一般建议不要安装在操作系统所在盘，免得操作系统坏了之后，还原操作把Apache配置文件也清除了。选“OK”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/08.jpg)
如上图的界面，选“Next”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/09.jpg)
确认安装选项无误，如果您认为要再检查一遍，可以点“Back”一步步返回检查。点“Install”开始按前面设定的安装选项安装。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/10.jpg)
安装向导成功完成，这时右下角状态栏应该出现了下面的这个绿色图标，表示Apache服务已经开始运行，按“Finish”结束Apache的软件安装
![](http://www.fruit0769.com/wp-content/uploads/2008/10/11.jpg)
在图标上左键单击，出现如下界面，有“Start（启动）”、“Stop（停止）”、“Restart（重启动）”三个选项，可以很方便的对安装的Apache服务器进行上述操作。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/12.jpg)
现在我们来测试一下按默认配置运行的网站界面，在IE地址栏打“[http://127.0.0.1](http://127.0.0.1/)”，按回车键。就可以看到如下页面，表示Apache服务器已安装成功。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/13.jpg)
现在开始配置Apache服务器，使它更好的替我们服务，事实上，如果不配置，你的安装目录下的Apache2.2/htdocs文件夹就是网站的默认根目录，在里面放入你的站点文件就可以了。这里我们还是要配置一下，点windows菜单“开始”、“所有程序”、“Apache HTTP Server 2.2”、“Configure Apache Server”、“Edit the Apache httpd conf Configuration file”，点击打开。打开后是个文本文档。
在这个文档中按Ctrl＋F 快捷键查找关键字“DocumentRoot”也就是网站根目录，找到如下图所示地方，修改成你所需要的地址。也可以不修改，以后D:/Apache2.2/htdocs就是你的网站根目录
![](http://www.fruit0769.com/wp-content/uploads/2008/10/21.jpg)
同样，你继续查找“DocumentRoot”，如图修改目录地址和刚刚的目录地址一样
![](http://www.fruit0769.com/wp-content/uploads/2008/10/22.jpg)
查找“DirectoryIndex”（目录索引），用来设置首页。每添加一个以单个半角空格隔开，如图
![](http://www.fruit0769.com/wp-content/uploads/2008/10/23.jpg)
好了，保存修改，配置完成。
======================================================================================
**二.安装php**
     PHP官方网站提供两种格式的 Php 5.03 For Win32 下载，一种是压缩成 EXE 的文件；另一种是 ZIP 压缩包。我选择的是 ZIP 压缩包。
　　首先将 php-5.0.3-Win32.zip 内的文件解压缩到 d:/website/php 目录中。
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
把刚下的php-5.2.6-Win32.zip解压缩到“D:/php”，查看解压缩后的文件夹内容，找到“php.ini-dist”文件，将其重命名为“php.ini”，打开开始编辑。如图，找到“extension”模块加载的地方
![](http://www.fruit0769.com/wp-content/uploads/2008/10/020.jpg)
选择要加载的模块，去掉前面的 “；”，就表示要加载此模块了。比如我要用mysql，就要把“;extension= php_mysql.dll”前的“;”去掉。
1.用文本编辑软件打开 c:/windows/php.ini 文件，然后修改以下内容：
　　将 extension_dir = "./" 改为 extension_dir = "d:/website/php/ext"
　　将 ;extension=php_MySQL.dll 行首的';'去掉；
　　将 ;extension=php_mbstring.dll 行首的“;”去掉；
如果上一步加载了其它模块，就要指明模块的位置，否则重启Apache的时候会提示“找不到指定模块”的错误，这里介绍一种最简单的方法，直接将php安装路径、里面的ext路径指定到windows系统路径中——在“我的电脑”上右键，“属性”，选择“高级”标签，点选“环境变量”，在“系统变量”下找到“Path”变量，选择，双击或点击“编辑”，将“;D:/php;D:/php/ext”加到原有值的后面，当然，其中的“D:/php” 是我的安装目录，你要将它改为自己的php安装目录，如下图所示，全部确定。系统路径添加好后要重启电脑才能生效，可以现在重启，也可以在所有软件安装或配置好后重启。
![](http://cimg.163.com/catchpic/3/35/35342B3E3999B8E486E6B8CE5D73EF76.gif)
更改Apache配置：
仍然是编辑 d:/apache/conf/httpd.conf 文件：
如下图，添加进如图所示选中的两行，第一行“LoadModule php5_module D:/php/php5apache2.dll”是指以module方式加载php，第二行“PHPIniDir “D:/php””是指明php的配置文件php.ini的位置，是当然，其中的“D:/php”要改成你先前选择的php解压缩的目录。**注意：对于apache2.2版本，请将php5apache2.dll改为php5apache2_2.dll。**
![](http://www.fruit0769.com/wp-content/uploads/2008/10/026.gif)
如下图，加入“AddType application/x-httpd-php .php”、“AddType application/x-httpd-php .html”两行，你也可以加入更多，实质就是添加可以执行php的文件类型，比如你再加上一行“AddType application/x-httpd-php .htm”，则.htm文件也可以执行php程序了
![](http://www.fruit0769.com/wp-content/uploads/2008/10/027.gif)
保存修改，完成php安装配置。
重新启动 Apache 服务。
　　到这里，Apache + Php 环境基本已经配置完成，在 web 根目录（以上例即Apache下htdocs）中，用记事本创建一个 phpinfo.php 文件，其内容如下：
　　<?php
　　phpinfo();
　　?>
　　然后，在浏览器中打开 [http://localhost/phpinfo.php](http://localhost/phpinfo.php) ，如果看到 下图Php 配置输出信息，就说明配置正常。
===================================================================================
**三.安装MySQL**
解压下载的mysql安装文件mysql-5.0.67-win32.zip，点击Setup.exe开始安装
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2041.jpg)
mysql安装向导启动，按“Next”继续
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2042.jpg)
选择安装类型，有“Typical（默认）”、“Complete（完全）”、“Custom（用户自定义）”三个选项，我们选择“Custom”（自定义安装）
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2043.jpg)
在“Developer Components（开发者部分）”上左键单击，选择“This feature, and all subfeatures, will be installed on local hard drive.”，即“此部分，及下属子部分内容，全部安装在本地硬盘上”。在上面的“MySQL Server（mysql服务器）”、“Client Programs（mysql客户端程序）”、“Documentation（文档）”也如此操作，以保证安装所有文件。点选“Change…”，手动指定安装目录。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2044.jpg)
填上安装目录“D:/mysql”，也建议不要放在与操作系统同一分区，这样可以防止系统备份还原的时候，数据被清空。按“OK”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2045.jpg)
按“Next”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2046.jpg)
确认一下先前的设置，如果有误，按“Back”返回重做。按“Install”开始安装。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2047.jpg)
正在安装中，请稍候，直到出现下面的界面
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2048.jpg)
按“Next”略过此步骤
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2049.jpg)
按“Next”略过此步骤
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2050.jpg)
现在软件安装完成了，出现上面的界面，这里有一个很好的功能，mysql配置向导，不用向以前一样，自己手动乱七八糟的配置my.ini了，将 “Configure the Mysql Server now”前面的勾打上，点“Finish”结束软件的安装并启动mysql配置向导。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2051.jpg)
mysql配置向导启动界面，按“Next”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2052.jpg)
选择配置方式，“Detailed Configuration（手动精确配置）”、“Standard Configuration（标准配置）”，我们选择“Detailed Configuration”
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2053.jpg)
选择服务器类型，“Developer Machine（开发测试类，mysql占用很少资源）”、“Server Machine（服务器类型，mysql占用较多资源）”、“Dedicated MySQL Server Machine（专门的数据库服务器，mysql占用所有可用资源）”，大家根据自己的类型选择了。我们本地测试只要选Developer Machine就好了。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2054.jpg)
选择mysql数据库的大致用途，“Multifunctional Database（通用多功能型，好）”、“Transactional Database Only（服务器类型，专注于事务处理，一般）”、“Non-Transactional Database Only（非事务处理型，较简单，主要做一些监控、记数用，对MyISAM数据类型的支持仅限于non-transactional），随自己的用途而选择了，我这里选择“Transactional Database Only”，按“Next”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2055.jpg)
对InnoDB Tablespace进行配置，就是为InnoDB 数据库文件选择一个存储空间，如果修改了，要记住位置，重装的时候要选择一样的地方，否则可能会造成数据库损坏。**我这里没有修改，使用用默认位置，直接按“Next”继续**
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2056.jpg)
选择您的网站的一般mysql访问量，同时连接的数目，“Decision Support(DSS)/OLAP（20个左右）”、“Online Transaction Processing(OLTP)（500个左右）”、“Manual Setting（手动设置，自己输一个数）”，我只是本地测试用，所以我这里选Decision Support(DSS)/OLAP，按“Next”继续
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2057.jpg)
是否启用TCP/IP连接，设定端口，如果不启用，就只能在自己的机器上访问mysql数据库了，我这里启用，把前面的勾打上，Port Number：3306，按“Next”继续
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2058.jpg)
选择是否将mysql安装为windows服务，还可以指定Service Name（服务标识名称），是否将mysql的bin目录加入到Windows PATH（加入后，就可以直接使用bin下的文件，而不用指出目录名，比如连接，“mysql.exe -uusername -ppassword;”就可以了，不用指出mysql.exe的完整地址，很方便），我这里全部打上了勾，Service Name不变。按“Next”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2063.jpg)
这个比较重要，就是对mysql默认数据库语言编码进行设置，我们选择“gbk”，当然也可以用“gb2312”，区别就是gbk的字库容量大，包括了gb2312的所有汉字，并且加上了繁体字和其它。按 “Next”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2059.jpg)
这一步询问是否要修改默认root用户（超级管理）的密码（默认为空），“New root password”如果要修改，就在此填入新密码，“Confirm（再输一遍）”内再填一次，防止输错。“Enable root access from remote machines（是否允许root用户在其它的机器上登陆，如果要安全，就不要勾上，如果要方便，就勾上它）”。最后“Create An Anonymous Account（新建一个匿名用户，匿名用户可以连接数据库，不能操作数据，包括查询）”，一般就不用勾了，设置完毕，按“Next”继续。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2060.jpg)
确认设置无误，如果有误，按“Back”返回检查。按“Execute”使设置生效。
![](http://www.fruit0769.com/wp-content/uploads/2008/10/2061.jpg)
按“Finish”结束mysql的安装与配置。
注意：（在到开cmd ，输入mysql –uroot –p后输入密码（或者在mysql command line下），若出现如上图的Welcome to the mysql……表示mysql安装成功，若出现错误，重新检查自己的mysql安装）。
=================
到这里，Apache + Php + MySQL 就基本配置完成了，在Web根目录下，用文本编辑软件创建一个 testdb.php 文件，其内容如下：
<?php 
$dbhost = '127.0.0.1';           //或者"localhost"
$dbuser = 'root';//你的mysql用户名 
$dbpassword= 'root'; //你的mysql密码 
$dbname = 'wbia'; //你的mysql库名 
$connect = mysql_connect($dbhost,$dbuser,$dbpassword); 
if ($connect) { 
echo '连接成功）'; 
} else { 
echo '连接失败'; 
} 
?>
用浏览器打开 [http://localhost/testdb.php](http://localhost/testdb.php) 如果看到输出 OK! 就说明配置正常。
**=====================================================**
四 .wordpress 安装
将其解压到apache中设置的文档目录下wp中，按其说明快速即可安装好！
