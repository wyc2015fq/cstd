# apache+php+mysql配置 - 战斗蜗牛的专栏 - CSDN博客





2011年06月13日 11:28:00[vbskj](https://me.csdn.net/vbskj)阅读数：707标签：[apache																[mysql																[php																[服务器																[server																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
个人分类：[SqlServer & MySql](https://blog.csdn.net/vbskj/article/category/817727)





先准备好软件：

Apache官方下载地址：[apache_2.0.55-win32-x86-no_ssl.msi](http://mirror.vmmatrix.net/apache/httpd/binaries/win32/apache_2.0.55-win32-x86-no_ssl.msi)
，[更多版本在这里](http://httpd.apache.org/download.cgi)
；

php官方下载地址：[php-5.0.5-Win32.zip](http://cn2.php.net/get/php-5.0.5-Win32.zip/from/cn.php.net/mirror)
，[更多镜像下载地址](http://www.php.net/get/php-5.0.5-Win32.zip/from/a/mirror)
，[更多版本下载](http://www.php.net/downloads.php)
；

mysql官方下载地址：[mysql-4.1.14-win32.zip](http://dev.mysql.com/get/Downloads/MySQL-4.1/mysql-4.1.14-win32.zip/from/http://download.softagency.net/MySQL/)
，[更多镜像下载地址](http://dev.mysql.com/get/Downloads/MySQL-4.1/mysql-4.1.14-win32.zip/from/pick#mirrors)
，[更多版本下载](http://dev.mysql.com/downloads/mysql/)
。

**一、安装Apache，配置成功一个普通网站服务器**

运行下载好的“[apache_2.0.55-win32-x86-no_ssl.msi](http://mirror.vmmatrix.net/apache/httpd/binaries/win32/apache_2.0.55-win32-x86-no_ssl.msi)
”，出现如下界面：


![](http://cimg.163.com/catchpic/3/3A/3A2809339D75314DBBF0DCAEE081B291.gif)
出现Apache HTTP Server 2.0.55的安装向导界面，点“Next”继续


![](http://cimg.163.com/catchpic/7/7D/7DDF0DA9827A0EDD10463EC2F9C67CAD.gif)
确认同意软件安装使用许可条例，选择“I accept the terms in the license agreement”，点“Next”继续


![](http://cimg.163.com/catchpic/B/BB/BBDFBAF0F6EFDF6CD3A627EDCA97DCD7.gif)
将Apache安装到Windows上的使用须知，请阅读完毕后，按“Next”继续


![](http://cimg.163.com/catchpic/D/DD/DDDA0F04F06F3F46035C91D6A9FB2F9F.gif)
设置系统信息，在Network 
Domain下填入您的域名（比如：goodwaiter.com），在Server 
Name下填入您的服务器名称（比如：www.goodwaiter.com，也就是主机名加上域名），在Administrator's Email 
Address下填入系统管理员的联系电子邮件地址（比如：yinpeng@xinhuanet.com），上述三条信息仅供参考，其中联系电子邮件地址
会在当系统故障时提供给访问者，三条信息均可任意填写，无效的也行。下面有两个选择，图片上选择的是为系统所有用户安装，使用默认的80端口，并作为系统
服务自动启动；另外一个是仅为当前用户安装，使用端口8080，手动启动。一般选择如图所示。按“Next”继续。]


![](http://cimg.163.com/catchpic/D/D5/D541B27C7C0C301040BA206C3067E75C.gif)
选择安装类型，Typical为默认安装，Custom为用户自定义安装，我们这里选择Custom，有更多可选项。按“Next”继续


![](http://cimg.163.com/catchpic/A/A1/A1746C427910A3E191DBC5251178E1DC.gif)
出现选择安装选项界面，如图所示，左键点选“Apache HTTP Server 
2.0.55”，选择“This feature, and all subfeatures, will be installed on local
 hard drive.”，即“此部分，及下属子部分内容，全部安装在本地硬盘上”。点选“Change...”，手动指定安装目录。


![](http://cimg.163.com/catchpic/7/74/74BCA25E05F4CD6D4B8A3132073A9744.gif)
我这里选择安装在“D:/”，各位自行选取了，一般建议不要安装在操作系统所在盘，免得操作系统坏了之后，还原操作把Apache配置文件也清除了。选“OK”继续。


![](http://cimg.163.com/catchpic/C/CC/CC30BC627F229F724C85FF0E94F627F1.gif)
返回刚才的界面，选“Next”继续。


![](http://cimg.163.com/catchpic/8/84/84D36BDCA5DBB8E339C559EAC99810FC.gif)
确认安装选项无误，如果您认为要再检查一遍，可以点“Back”一步步返回检查。点“Install”开始按前面设定的安装选项安装。


![](http://cimg.163.com/catchpic/9/98/980A675B1D4538DBB126D9279D950F92.gif)
正在安装界面，请耐心等待，直到出现下面的画面。


![](http://cimg.163.com/catchpic/2/25/25AC4569E22E9BC2263B4D6BCEF1AA19.gif)
安装向导成功完成，这时右下角状态栏应该出现了下面的这个绿色图标，表示Apache服务已经开始运行，按“Finish”结束Apache的软件安装


![](http://cimg.163.com/catchpic/C/C1/C1527947DD583343D11543FF5905378A.gif)
我们来熟悉一下这个图标，很方便的，在图标上左键单击，出现如下界面，有“Start（启动）”、“Stop（停止）”、“Restart（重启动）”三个选项，可以很方便的对安装的Apache服务器进行上述操作。


![](http://cimg.163.com/catchpic/1/1A/1AAB07386D2B21E83A528902CB0CB5A2.gif)
好了现在我们来测试一下按默认配置运行的网站界面，在IE地址栏打“http://127.0.0.1”，点“转到”，就可以看到如下页面，表示Apache服务器已安装成功。


![](http://cimg.163.com/catchpic/9/9D/9D52D7BC2F6417340501B556B335E0F2.gif)
现在开始配置Apache服务器，使它更好的替我们服务，事实上，如果不配置，你的安装目录
下的Apache2/htdocs文件夹就是网站的默认根目录，在里面放入文件就可以了。这里我们还是要配置一下，有什么问题或修改，配置始终是要会的，
如图所示，“开始”、“所有程序”、“Apache HTTP Server 2.0.55”、“Configure Apache 
Server”、“Edit the Apache httpd conf Configuration file”，点击打开。


![](http://cimg.163.com/catchpic/B/B1/B1A9FE73D7EB4B8E48E3EACEDE6B1408.gif)
XP的记事本有了些小变化，很实用的一个功能就是可以看到文件内容的行、列位置，按下图所
示，点“查看”，勾选“状态栏”，界面右下角就多了个标记，“Ln 78, Col 10”就表示“行 78，列 
10”，这样可以迅速的在文件中定位，方便解说。当然，你也可以通过“编辑”，“查找”输入关键字来快速定位。每次配置文件的改变，保存后，必须在 
Apache服务器重启动后生效，可以用前面讲的小图标方便的控制服务器随时“重启动”。


![](http://cimg.163.com/catchpic/5/5B/5BDD4BC454AB20790CC72C876D167302.gif)
现在正式开始配置Apache服务器，“Ln 228”，或者查找关键字“DocumentRoot”（也就是网站根目录），找到如下图所示地方，然后将""内的地址改成你的网站根目录，地址格式请照图上的写，主要是一般文件地址的“/”在Apache里要改成“/”。


![](http://cimg.163.com/catchpic/A/A5/A503C1AF9C3AF1F3121EAA7944A630E4.gif)
“Ln 253”，同样，你也可以通过查找“< P>


![](http://cimg.163.com/catchpic/C/CB/CB3C92D6D3D35479F8CA6403E4D1D19D.gif)
“Ln321”，DirectoryIndex（目录索引，也就是在仅指定目录的情况下，默
认显示的文件名），可以添加很多，系统会根据从左至右的顺序来优先显示，以单个半角空格隔开，比如有些网站的首页是index.htm，就在光标那里加上
“index.htm ”文件名是任意的，不一定非得“index.html”，比如“test.php”等，都可以。


![](http://cimg.163.com/catchpic/9/9D/9D6E67CEE95CD5FE434BAE39E816C56D.gif)
这里有一个选择配置选项，以前可能要配置，现在好像修正过来了，不用配置了，就是强制所有输
出文件的语言编码，html文件里有语言标记（，这个就是设定文档语言为gb2312）的也会强制转换。如果打开的网页出现乱码，请先检查网页内有没有上
述 html语言标记，如果没有，添加上去就能正常显示了。把“# DefaultLanguage nl”前面的“# 
”去掉，把“nl”改成你要强制输出的语言，中文是“zh-cn”，保存，关闭。


![](http://cimg.163.com/catchpic/E/ED/EDC3C1B7C5F2C737FF95545697EB7A79.gif)
好了，简单的Apache配置就到此结束了，现在利用先前的小图标重启动，所有的配置就生效
了，你的网站就成了一个网站服务器，如果你加载了防火墙，请打开80或8080端口，或者允许Apache程序访问网络，否则别人不能访问。如果你有公网
IP（一般ADSL或电话拨号上网的都是），就可以邀请所有能上网的朋友访问使用http://你的IP地址（IP地址查询可访问http:
//www.goodwaiter.com，查询内容内即是）你的网站了；如果你没有公网IP，也可以把内网IP地址告诉局域网内的其它用户，让他们通过
http://你的内网IP地址，访问你的网站。

**二、php的安装、以module方式，将php与apache结合使你的网站服务器支持php服务器脚本程序**

将下载的php安装文件[php-5.0.5-Win32.zip](http://cn2.php.net/get/php-5.0.5-Win32.zip/from/cn.php.net/mirror)
右键解压缩。


![](http://cimg.163.com/catchpic/0/06/06D391A2654006E29578EE1E22F0F4D8.gif)
指定解压缩的位置，我的设定在“D:/php”


![](http://cimg.163.com/catchpic/5/50/507EE3FE500CA51633D57359F245D9E6.gif)
查看解压缩后的文件夹内容，找到“php.ini-dist”文件，将其重命名为
“php.ini”，打开编辑，找到下面图中的地方， Ln385，有一个“register_globals = 
Off”值，这个值是用来打开全局变量的，比如表单送过来的值，如果这个值设为“Off”，就只能用“$_POST['变量名']、$_GET['变量名
 
']”等来取得送过来的值，如果设为“On”，就可以直接使用“$变量名”来获取送过来的值，当然，设为“Off”就比较安全，不会让人轻易将网页间传送
的数据截取。这个值是否改成“On”就看自己感觉了，是安全重要还是方便重要？


![](http://cimg.163.com/catchpic/6/6A/6AFE57A7C79A3120BC7C460D04A6DD59.gif)
这里还有一个地方要编辑，功能就是使php能够直接调用其它模块，比如访问mysql，如下
图所示，Ln563，选择要加载的模块，去掉前面的 
“;”，就表示要加载此模块了，加载的越多，占用的资源也就多一点，不过也多不到哪去，比如我要用mysql，就要把“;extension= 
php_mysql.dll”前的“;”去掉。所有的模块文件都放在php解压缩目录的“ext”之下，我这里的截图是把所有能加载的模块都加载上去了，
前面的“;”没去掉的，是因为“ext”目录下默认没有此模块，加载会提示找不到文件而出错。这里只是参考，一般不需要加载这么多，需要的加载上就可以
了，编辑好后保存，关闭。


![](http://cimg.163.com/catchpic/C/CF/CF4C9456C36E36B3FA0E54863D9FF60B.gif)
如果上一步加载了其它模块，就要指明模块的位置，否则重启Apache的时候会提示“找不到
指定模块”的错误，这里介绍一种最简单的方法，直接将php安装路径、里面的ext路径指定到windows系统路径中——在“我的电脑”上右键，“属
性”，选择“高级”标签，点选“环境变量”，在“系统变量”下找到“Path”变量，选择，双击或点击“编辑”，将“;D:/php;D:/php
/ext”加到原有值的后面，当然，其中的“D:/php” 
是我的安装目录，你要将它改为自己的php安装目录，如下图所示，全部确定。系统路径添加好后要重启电脑才能生效，可以现在重启，也可以在所有软件安装或
配置好后重启。


![](http://cimg.163.com/catchpic/3/35/35342B3E3999B8E486E6B8CE5D73EF76.gif)
现在开始将php以module方式与Apache相结合，使php融入Apache，照先
前的方法打开Apache的配置文件，Ln 173，找到这里，添加进如图所示选中的两行，第一行“LoadModule php5_module 
D:/php/php5apache2.dll”是指以module方式加载php，第二行“PHPIniDir 
"D:/php"”是指明php的配置文件php.ini的位置，是当然，其中的“D:/php”要改成你先前选择的php解压缩的目录。


![](http://cimg.163.com/catchpic/5/52/52E7ADAF91BE46D0EB48BD5982F00246.gif)
还是Apache的配置文件，Ln 757，加入“AddType 
application/x-httpd-php .php”、“AddType application/x-httpd-php 
.html”两行，你也可以加入更多，实质就是添加可以执行php的文件类型，比如你再加上一行“AddType 
application/x-httpd-php .htm”，则.htm文件也可以执行php程序了，你甚至还可以添加上一行“AddType 
application/x-httpd-php .txt”，让普通的文本文件格式也能运行php程序。


![](http://cimg.163.com/catchpic/6/60/60C2E070B9A9690345AD08437DAAA1F9.gif)
前面所说的目录默认索引文件也可以改一下，因为现在加了php，有些文件就直接存为.php了，我们也可以把“index.php”设为默认索引文件，优先顺序就自己排了，我的是放在第一位。编辑完成，保存，关闭。


![](http://cimg.163.com/catchpic/4/43/436180CE85AF964CC9E8073AA0C869B3.gif)
现在，php的安装，与Apache的结合已经全部完成，用屏幕右下角的小图标重启Apache，你的Apache服务器就支持了php。

**三、mysql的安装，与php、Apache相结合**

打开下载的mysql安装文件[mysql-4.1.14-win32.zip](http://dev.mysql.com/get/Downloads/MySQL-4.1/mysql-4.1.14-win32.zip/from/http://download.softagency.net/MySQL/)
，双击解压缩，运行“setup.exe”，出现如下界面


![](http://cimg.163.com/catchpic/A/AC/AC61EC8296DBDBD8D2DF52CA87BDCDA4.gif)
mysql安装向导启动，按“Next”继续


![](http://cimg.163.com/catchpic/B/B0/B07BEE23ACD6199EC9A30481D1979CF0.gif)
选择安装类型，有“Typical（默认）”、“Complete（完全）”、“Custom（用户自定义）”三个选项，我们选择“Custom”，有更多的选项，也方便熟悉安装过程


![](http://cimg.163.com/catchpic/C/CD/CD0A09D363DDF74DCBF691AC9879FBB6.gif)
在“Developer Components（开发者部分）”上左键单击，选择“This
 feature, and all subfeatures, will be installed on local hard 
drive.”，即“此部分，及下属子部分内容，全部安装在本地硬盘上”。在上面的“MySQL Server（mysql服务器）”、“Client 
Programs（mysql客户端程序）”、“Documentation（文档）”也如此操作，以保证安装所有文件。点选“Change...”，手
动指定安装目录。


![](http://cimg.163.com/catchpic/C/C5/C5712D380D06A649EAA7D446D8ED91ED.gif)
填上安装目录，我的是“D:/mysql”，也建议不要放在与操作系统同一分区，这样可以防止系统备份还原的时候，数据被清空。按“OK”继续。


![](http://cimg.163.com/catchpic/D/DC/DC7479D846AA302D647939C67ADC0B95.gif)
返回刚才的界面，按“Next”继续。


![](http://cimg.163.com/catchpic/7/75/7504524A73B06D086BFBBED104FB468B.gif)
确认一下先前的设置，如果有误，按“Back”返回重做。按“Install”开始安装。


![](http://cimg.163.com/catchpic/5/54/54196C507E66B459E82A3C65243ADE40.gif)
正在安装中，请稍候，直到出现下面的界面


![](http://cimg.163.com/catchpic/F/FB/FBA5F1B304F49FBEAA68BA2C57572661.gif)
这里是询问你是否要注册一个mysql.com的账号，或是使用已有的账号登陆mysql.com，一般不需要了，点选“Skip Sign-Up”，按“Next”略过此步骤。


![](http://cimg.163.com/catchpic/1/10/10FC94552347132116DDAFF977AE747F.gif)
现在软件安装完成了，出现上面的界面，这里有一个很好的功能，mysql配置向导，不用向以
前一样，自己手动乱七八糟的配置my.ini了，将 “Configure the Mysql Server 
now”前面的勾打上，点“Finish”结束软件的安装并启动mysql配置向导。


![](http://cimg.163.com/catchpic/C/CA/CA1EC64A318715671218803B9B4A2030.gif)
mysql配置向导启动界面，按“Next”继续。


![](http://cimg.163.com/catchpic/6/6E/6E50CE778965049B10726569DF31202F.gif)
选择配置方式，“Detailed Configuration（手动精确配置）”、“Standard Configuration（标准配置）”，我们选择“Detailed Configuration”，方便熟悉配置过程。


![](http://cimg.163.com/catchpic/1/13/134E791C5A1CEBC9B1039907C031190B.gif)
选择服务器类型，“Developer 
Machine（开发测试类，mysql占用很少资源）”、“Server 
Machine（服务器类型，mysql占用较多资源）”、“Dedicated MySQL Server 
Machine（专门的数据库服务器，mysql占用所有可用资源）”，大家根据自己的类型选择了，一般选“Server 
Machine”，不会太少，也不会占满。


![](http://cimg.163.com/catchpic/1/18/1809D363081314946A59263CE67BEE38.gif)
选择mysql数据库的大致用途，“Multifunctional 
Database（通用多功能型，好）”、“Transactional Database 
Only（服务器类型，专注于事务处理，一般）”、“Non-Transactional Database 
Only（非事务处理型，较简单，主要做一些监控、记数用，对MyISAM数据类型的支持仅限于non-transactional），随自己的用途而选
择了，我这里选择“Transactional Database Only”，按“Next”继续。


![](http://cimg.163.com/catchpic/9/97/974B9FFA76C51DB4AD15F691323083D6.gif)
对InnoDB Tablespace进行配置，就是为InnoDB 
数据库文件选择一个存储空间，如果修改了，要记住位置，重装的时候要选择一样的地方，否则可能会造成数据库损坏，当然，对数据库做个备份就没问题了，这里
不详述。我这里没有修改，使用用默认位置，直接按“Next”继续


![](http://cimg.163.com/catchpic/E/EC/EC8F7F964E5E83DDB7697FB7F33D51CC.gif)
选择您的网站的一般mysql访问量，同时连接的数目，“Decision 
Support(DSS)/OLAP（20个左右）”、“Online Transaction 
Processing(OLTP)（500个左右）”、“Manual Setting（手动设置，自己输一个数）”，我这里选“Online 
Transaction Processing(OLTP)”，自己的服务器，应该够用了，按“Next”继续


![](http://cimg.163.com/catchpic/9/98/98B3746C9428165F72A220D17159CC4F.gif)
是否启用TCP/IP连接，设定端口，如果不启用，就只能在自己的机器上访问mysql数据库了，我这里启用，把前面的勾打上，Port Number：3306，按“Next”继续


![](http://cimg.163.com/catchpic/C/C5/C5B6A3E03A91E263A59C25F3AF7C5292.gif)
这个比较重要，就是对mysql默认数据库语言编码进行设置，第一个是西文编码，第二个是多
字节的通用utf8编码，都不是我们通用的编码，这里选择第三个，然后在Character 
Set那里选择或填入“gbk”，当然也可以用“gb2312”，区别就是gbk的字库容量大，包括了gb2312的所有汉字，并且加上了繁体字、和其它
乱七八糟的字——使用mysql的时候，在执行数据操作命令之前运行一次“SET NAMES 
GBK;”（运行一次就行了，GBK可以替换为其它值，视这里的设置而定），就可以正常的使用汉字（或其它文字）了，否则不能正常显示汉字。按 
“Next”继续。


![](http://cimg.163.com/catchpic/9/9E/9ED4307E1B58960DC5FC4FEF83FF2993.gif)
选择是否将mysql安装为windows服务，还可以指定Service 
Name（服务标识名称），是否将mysql的bin目录加入到Windows 
PATH（加入后，就可以直接使用bin下的文件，而不用指出目录名，比如连接，“mysql.exe -uusername 
-ppassword;”就可以了，不用指出mysql.exe的完整地址，很方便），我这里全部打上了勾，Service 
Name不变。按“Next”继续。


![](http://cimg.163.com/catchpic/8/88/8850E209E64D8FE8D1C76B869FE2F261.gif)
这一步询问是否要修改默认root用户（超级管理）的密码（默认为空），“New root
 password”如果要修改，就在此填入新密码（如果是重装，并且之前已经设置了密码，在这里更改密码可能会出错，请留空，并将“Modify 
Security 
Settings”前面的勾去掉，安装配置完成后另行修改密码），“Confirm（再输一遍）”内再填一次，防止输错。“Enable root 
access from remote 
machines（是否允许root用户在其它的机器上登陆，如果要安全，就不要勾上，如果要方便，就勾上它）”。最后“Create An 
Anonymous 
Account（新建一个匿名用户，匿名用户可以连接数据库，不能操作数据，包括查询）”，一般就不用勾了，设置完毕，按“Next”继续。


![](http://cimg.163.com/catchpic/A/A3/A30C1F0177779C4806A7F04D0F00B5DA.gif)
确认设置无误，如果有误，按“Back”返回检查。按“Execute”使设置生效。


![](http://cimg.163.com/catchpic/F/F0/F08C52F35BE19658BD9639A42157F92B.gif)
设置完毕，按“Finish”结束mysql的安装与配置——这里有一个比较常见的错误，就
是不能“Start 
service”，一般出现在以前有安装mysql的服务器上，解决的办法，先保证以前安装的mysql服务器彻底卸载掉了；不行的话，检查是否按上面一
步所说，之前的密码是否有修改，照上面的操作；如果依然不行，将mysql安装目录下的data文件夹备份，然后删除，在安装完成后，将安装生成的 
data文件夹删除，备份的data文件夹移回来，再重启mysql服务就可以了，这种情况下，可能需要将数据库检查一下，然后修复一次，防止数据出错。

与Apache及php相结合，前面已提过，这里再说一下，在php安装目录下，找到先前重
命名并编辑过的 
php.ini，如下图所示，Ln563，把“;extension=php_mysql.dll”前的“;”去掉，加载mysql模块。保存，关闭后，
重启apache就可以了。这里也可以选择其它要加载的模块，去掉前面的“;”，就表示要加载此模块了，加载的越多，占用的资源也就多一点，不过也多不到
哪去。所有的模块文件都放在php解压缩目录的“ext”之下，我这里的截图是把所有能加载的模块都加载上去了，前面的“;”没去掉的，是因为“ext”
 目录下默认没有此模块，加载会提示找不到文件而出错。这里只是参考，一般不需要加载这么多，需要的加载上就可以了，编辑好后保存，关闭。


![](http://cimg.163.com/catchpic/C/CF/CF4C9456C36E36B3FA0E54863D9FF60B.gif)
同样，加载了模块后，就要指明模块的位置，否则重启Apache的时候会提示“找不到指定模
块”的错误，这里介绍一种最简单的方法，直接将 
php安装路径、里面的ext路径指定到windows系统路径中——在“我的电脑”上右键，“属性”，选择“高级”标签，点选“环境变量”，在“系统变
量”下找到“Path”变量，选择，双击或点击“编辑”，将“;D:/php;D:/php/ext”加到原有值的后面，当然，其中的“D:/php”是
我的安装目录，你要将它改为自己的php安装目录，如下图所示，全部确定。系统路径添加好后要重启电脑才能生效，可以现在重启，也可以在所有软件安装或配
置好后重启。


![](http://cimg.163.com/catchpic/3/35/35342B3E3999B8E486E6B8CE5D73EF76.gif)
原文：[http://www.isblog.cn/user1/1032/archives/2006/3127.html](http://www.isblog.cn/user1/1032/archives/2006/3127.html)![宋涛](http://tech.163.com/newimg/arc_d.gif)

![宋涛](http://tech.163.com/newimg/arc_d.gif)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)




