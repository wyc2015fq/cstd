# phpMyAdmin简明安装教程 - 三少GG - CSDN博客
2011年02月05日 21:52:00[三少GG](https://me.csdn.net/scut1135)阅读数：1085标签：[phpmyadmin																[mysql																[数据库																[服务器																[socket																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=phpmyadmin&t=blog)
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
[http://faq.comsenz.com/viewnews-484](http://faq.comsenz.com/viewnews-484)
[http://www.360doc.com/content/07/0621/19/21275_572164.shtml](http://www.360doc.com/content/07/0621/19/21275_572164.shtml)
phpMyAdmin简明安装教程
2008-11-30┆15,885 Views┆120 
Tags : [phpMyAdmin](http://faq.comsenz.com/tag-phpMyAdmin)[教程](http://faq.comsenz.com/tag-%BD%CC%B3%CC)
phpMyAdmin 就是一种 MySQL 数据库的管理工具，安装该工具后，即可以通过 web 形式直接管理 MySQL 数据，而不需要通过执行系统命令来管理，非常适合对数据库操作命令不熟悉的数据库管理者，下面详细说明该工具的安装方法。
**一、下载**
先到互联网上下载 phpMyAdmin，也可以到 phpMyAdmin 官方网站下载，地址为：[http://www.phpmyadmin.net/home_page/index.php](http://www.phpmyadmin.net/home_page/index.php)  再解压到 web 可以访问的目录下，如果是虚拟空间，可以解压后通过 ftp 工具上传到 web 目录下，同时您可以修改解压后该文件的名称。
**二、配置**
打开 libraries 目录下的 config.default.php 文件，依次找到下面各项，按照说明配置即可。
*****************************************
**几乎所有的配置参数都在 config.inc.php 文件中。如果这个文件不存在，可以在libraries目录中找到config.default.php，将它复制到 phpmyadmin 目录，并改名为 config.inc.php。 **
　　涉及到界面设计（例如颜色）的参数，存放在 themes/themename/layout.inc.php 文件中。也可以创建 config.footer.inc.php 文件和 config.header.inc.php 文件来添加站点的自定义代码，这些代码显示在[页眉](http://writeblog.csdn.net/view/1161004.htm)和[页脚](http://writeblog.csdn.net/view/1161011.htm)。 
**$cfg[PmaAbsoluteUri]字符串**
　　这里填写 phpMyAdmin 安装目录的完整 [URL](http://writeblog.csdn.net/view/1496.htm) (包括完整的路径) 
　　注意，在某些浏览器，URL 是大小写敏感的。不要忘记结尾处的反斜杠。从2.3.0版本开始，可以尝试不填这个参数。phpMyAdmin 可以自动检测到正确的配置。使用端口转向的 
[](http://baike.baidu.com/image/c2bce203bd395fc5d53f7c6e)![](http://imgsrc.baidu.com/baike/abpic/item/c2bce203bd395fc5d53f7c6e.jpg)
phpMyAdmin
用户必须填写 PmaAbsoluteUri（如果服务器在[防火墙](http://writeblog.csdn.net/view/3067.htm)后面，phpMyAdmin将无法加载图形和样式表）。 
　　检测方法是浏览一个表，编辑一条记录并保存，如果 phpMyAdmin 无法检测正确的值，系统将会有错误提示。 
　　如果看到错误提示，必须手工设置这个参数。 
**$cfg[PmaNoRelation_DisableWarning]布尔**
　　从2.3.0版本开始，phpMyAdmin 提供了对主/外部数据表的支持（参考 $cfg['Servers'][$i]['pmadb'] 参数）。 
　　如果不能使用这些功能，请查阅某个数据库的“Structure”页面。那里会有一个连接，它会分析这些功能被禁止的原因。 
　　如果不想使用这些功能，将这个参数设置为TRUE，这个信息将不会显示。 
**$cfg[blowfish_secret]字符串**
　　从2.5.2版本开始，“cookie”认证方式使用blowfish算法来加密密码。如果使用“cookie”认证方式，请在这里输入一个随机[密钥](http://writeblog.csdn.net/view/934.htm)，该密钥在blowfish算法内部使用： 
　　在使用过程中不会提示输入它。密钥最大长度为46个字符。 
**$cfg[Servers]数组 **从1.4.2版本开始，phpMyAdmin 可以管理多个MySQL服务器。 
　　因此，加入了 $cfg['Servers'] 参数，这是一个数组，包括了不同服务器的登录信息。 
　　第一个 $cfg['Servers'][$i]['host'] 参数是第一个服务器的主机名， 
　　第二个 $cfg['Servers'][$i]['host'] 参数是第二个服务器的主机名，依此类推。 
　　在 libraries/config.default.php 文件里，只有第一个服务器的定义， 
　　不过仍然可以在 config.inc.php 自己添加更多的服务器， 
　　复制整个块，或者需要的部分（不需要定义所有的值，只需要那些修改过的参数）。 
**$cfg['Servers'][$i]['host']字符串 **第i个服务器的主机名或者[IP地址](http://writeblog.csdn.net/view/3930.htm)，例如localhost。 
**$cfg['Servers'][$i]['port']字符串**
　　第i个服务器的端口号。默认是3306（可以不填）。 
　　如果主机名是“localhost”，MySQL将无视端口号，以[socket](http://writeblog.csdn.net/view/13870.htm)方式连接。 
　　所以，如果需要连接一个不同的端口号，请在 $cfg['Servers'][$i]['host'] 填写“[127.0.0.1](http://writeblog.csdn.net/view/971216.htm)”或者真实的主机名。 
**$cfg['Servers'][$i]['socket']字符串**
　　socket路径。不填为默认值。 
**$cfg['Servers'][$i]['connect_type']字符串**
　　与MySQL服务器的连接方式，可以选择“socket”或者“tcp”。 
**默认为“tcp”**，这种方式几乎可以连接所有的MySQL服务器，而sockets方式不支持某些平台。 
**要使用socket方式，MySQL服务器必须和Web服务器在用一台服务器。**
**$cfg['Servers'][$i]['extension']字符串**
　　phpMyAdmin 系统使用的 phpMySQL 扩展： 
　　mysql：经典的MySQL扩展。默认值并推荐MySQL4.0用户使用。 
　　mysqli：改进的MySQL扩展。包含在php5.0.0以上版本中，推荐MySQL4.1.0以上版本用户使用。 
**$cfg['Servers'][$i]['compress']布尔**
　　是否使用压缩协议来连接MySQL服务器。 
　　本功能需要PHP4.3.0以上版本。 
**$cfg['Servers'][$i]['controluser']字符串**
**$cfg['Servers'][$i]['controlpass']字符串**
　　这个特殊账户有2个特殊作用：是的某些相关功能可用（参考$cfg['Servers'][$i]['pmadb']参数）； 
　　在低于4.1.2版本，或者“--skip-show-database”参数打开的MySQL服务器上，打开多用户功能（需要http或者cookie认证模式）。 
　　如果使用HTTP或者cookie认证模式（或在phpMyAdmin2.2.1以上版本使用“config”认证模式）， 
　　需要设立一个MySQL账户，该账户只有以下表的SELECT权限： 
　　mysql.user表（除了“Password”字段的所有字段）， 
　　mysql.db表（所有字段）， 
　　mysql.tables_priv表（除了“Grantor”和“Timestamp”字段的所有字段）。 
　　这个账户用于检测登录用户可以看到的数据库。 
　　请查阅文档的安装章节的"Usingauthenticationmodes"获取更多信息。 
　　在2.2.5版本之前的phpMyAdmin，这对参数叫做"stduser/stdpass"。 
**$cfg['Servers'][$i]['auth_type']字符串['http'|'cookie'|'config']**
　　选择config、cookie或者http认证。 
　　*'config'认证（$auth_type='config'）是最简单的模式，用户名和密码存储在config.inc.php文件里； 
　　*'cookie'认证模式（$auth_type='cookie'）2.2.3以上版本允许通过cookie，以任何合法的MySQL用户登录。 
　　用户名和密码存储在cookie中，注销后将删除密码。也可以登录任意的服务器（需要打开 $cfg['AllowArbitraryServer'] 参数）。 
　　*'http'认证（在老版本叫做'advanced'）（$auth_type='http'）1.3.0以上版本允许您通过HTTP-Auth，以任何有效的MySQL用户登录。 
　　请查阅文档的安装章节的"Usingauthenticationmodes"获取更多信息。 
**$cfg['Servers'][$i]['user']字符串**
**$cfg['Servers'][$i]['password']字符串 **如果auth_type='config'，phpMyAdmin将使用这个账户来连接MySQL服务器。 
　　如果您使用HTTP或者cookie认证，请不要填写这个账户。 
**$cfg['Servers'][$i]['only_db']字符串或数组**
　　如果填写了某个（或者某组）数据库，那么登录用户将只看到这个（或这些）数据库。 
　　从 phpMyAdmin2.2.1 开始，数据库名可以使用 MySQL 通配符（"_"和"%"）： 
**请用斜杠区分这两个符号，例如，应当使用'my/_db'而不是'my_db'。**
　　本功能可以有效降低服务器负载，MySQL服务器将不会建立可用数据库的列表。 
　　但本功能不会覆盖MySQL服务器的权限规则， 
　　设置本参数，仅仅表示只显示这些数据库，并不是所有显示出来的数据库都能被访问。 
　　多个数据库的例子 
　　代码 
　　$cfg['Servers'][$i]['only_db']=array('db1','db2'); 
　　从phpMyAdmin2.5.5开始，数组中的顺序将被用来作为页面左边帧的数据库显示顺序。因此，可以自己排列数据库。 
　　如果只希望在列表开头显示某几个数据库，而不管其他的，那么，不需要指定所有的数据库。 
　　只需要填写： 
　　代码 
　　$cfg['Servers'][$i]['only_db']=array('db3','db4','*'); 
　　phpMyAdmin将把db3和db4显示在开头，其他的按照默认顺序排列。 
**$cfg['Servers'][$i]['hide_db']字符串 **用于隐藏某些数据库的正则表达式。这些数据库只是不显示在列表中，用户仍然可以访问他们。 
**$cfg['Servers'][$i]['verbose']字符串**
　　只有当多服务器模式时有效。设置以后，本字符串将代替主机名显示在首页的下拉菜单中。 
**$cfg['Servers'][$i]['pmadb']字符串 **包含连接表结构的数据库名称。 
　　详细情况请查阅文档的Linked-tablesinfrastructure章节。 
　　如果是phpMyAdmin的唯一用户，可以使用当前的数据库来存储这些特殊的数据，填写当前数据库名即可。 
　　如果的phpMyAdmin配置为多个用户使用，在这里填写包含连接表结构的中心数据库名称。 
**$cfg['Servers'][$i]['bookmarktable']字符串**
　　从2.2.0版本开始，phpMyAdmin允许将经常使用的查询存储在书签中。 
　　打开本功能的方法： 
　　*设置 pmadb 以及连接表结构 
　　*在 $cfg['Servers'][$i]['bookmarktable'] 处填写表名
*******************************************
1、访问网址
$cfg['PmaAbsoluteUri'] = '';这里填写 phpMyAdmin 的访问网址。
CODE: [COPY]
$cfg['PmaAbsoluteUri'] = '';这里填写 phpMyAdmin 的访问网址。<P></P>
2、MySQL 主机信息
$cfg['Servers'][$i]['host'] = 'localhost'; // MySQL hostname or IP address
CODE: [COPY]
$cfg['Servers'][$i]['host'] = 'localhost'; // MySQL hostname or IP address<P></P>
填写 localhost  或 MySQL  所在服务器的 ip 地址，如果 MySQL 和该 phpMyAdmin 在同一服务器，则按默认 localhost
$cfg['Servers'][$i]['port'] = ''; // MySQL port - leave blank for default port
CODE: [COPY]
$cfg['Servers'][$i]['port'] = ''; // MySQL port - leave blank for default port<P></P>
MySQL 端口，默认为 3306，保留为空即可，如果您安装 MySQL 时使用了其它的端口，需要在这里填写。
3、MySQL 用户名和密码
$cfg['Servers'][$i]['user'] = 'root'; // 填写 MySQL 访问 phpMyAdmin 使用的 MySQL 用户名，默认为 root。
CODE: [COPY]
$cfg['Servers'][$i]['user'] = 'root'; // 填写 MySQL 访问 phpMyAdmin 使用的 MySQL 用户名，默认为 root。<P></P>
fg['Servers'][$i]['password'] = ''; // 填写对应上述 MySQL 用户名的密码。
CODE: [COPY]
fg['Servers'][$i]['password'] = ''; // 填写对应上述 MySQL 用户名的密码。<P></P>
4、认证方法
$cfg['Servers'][$i]['auth_type'] = 'cookie';考虑到安全的因素，建议这里填写 cookie。
CODE: [COPY]
$cfg['Servers'][$i]['auth_type'] = 'cookie';考虑到安全的因素，建议这里填写 cookie。<P></P>
在此有四种模式可供选择，cookie，http，HTTP，config
config 方式即输入 phpMyAdmin 的访问网址即可直接进入，无需输入用户名和密码，是不安全的，不推荐使用。
当该项设置为 cookie，http 或 HTTP 时，登录 phpMyAdmin 需要数据用户名和密码进行验证，具体如下：
PHP 安装模式为 Apache，可以使用 http 和 cookie；
PHP 安装模式为 CGI，可以使用 cookie。
5、短语密码（blowfish_secret）的设置
$cfg['blowfish_secret'] = '';
CODE: [COPY]
$cfg['blowfish_secret'] = '';<P></P>
如果认证方法设置为 cookie，就需要设置短语密码，设置为什么密码，由您自己决定，这里不能留空，否则会在登录 phpMyAdmin 时提示如下图所示的错误。
![](http://faq.comsenz.com/attachments/2008/11/10_200811302107411ZPcU.jpg)
安装完成后，我们可以在浏览器进行访问，如下图所示：
![](http://faq.comsenz.com/attachments/2008/11/10_200811302111561tv4Z.jpg)
**说明：**
该文档说明的只是安装 phpMyAdmin 的基本配置，关于 config.default.php 文件中各个配置参数的详细说明可以参考：
[http://www.discuz.net/viewthread.php?tid=50789](http://www.discuz.net/viewthread.php?tid=50789)
**三、在什么情况下会用到 phpMyAdmin**
1、需要修复数据库的时候，参考：[http://faq.comsenz.com/viewnews-378](http://faq.comsenz.com/viewnews-378)
2、设置数据库用户权限的时候，参考：[http://faq.comsenz.com/viewnews-164](http://faq.comsenz.com/viewnews-164)
3、检查和浏览数据库的时候
4、执行 SQL  语句的时候
5、恢复和备份数据库的时候
6、还会用到一些常见问题上
================================================================================
||简单的说，phpmyadmin就是一种mysql的管理工具，安装该工具后，即可以通过web形式直接管理mysql数据，而不需要通过执行系统命令来管理，非常适合对数据库操作命令不熟悉的数据库管理者，下面我就说下怎么安装该工具：1.先到网上下载phpmyadmin，再解压到可以访问的web目录下（如果是虚拟空间，可以解压后通过ftp等上传到web目录下），当然您可以修改解压后该文件的名称。2.配置config文件  打开libraries下的config.default.php文件，依次找到下面各项，按照说明配置即可：A.访问网址> $cfg['PmaAbsoluteUri'] = '';这里填写phpmyadmin的访问网址B.mysql主机信息> $cfg['Servers'][$i]['host'] = 'localhost'; // MySQL hostname or IP address> 填写localhost或mysql所在服务器的ip地址，如果mysql和该phpmyadmin在同一服务器，则按默认localhost$cfg['Servers'][$i]['port'] = ''; // MySQL port - leave blank for default portmysql端口，如果是默认3306，保留为空即可C.mysql用户名和密码> $cfg['Servers'][$i]['user'] = 'root'; // MySQL user > 访问phpmyadmin使用的mysql用户名fg['Servers'][$i]['password'] = ''; // MySQL password (only needed对应上述mysql用户名的密码D.认证方法> $cfg['Servers'][$i]['auth_type'] = 'cookie';   在此有四种模式可供选择，cookie，http，HTTP，config   config方式即输入phpmyadmin的访问网址即可直接进入，无需输入用户名和密码，是不安全的，不推荐使用。   当该项设置为cookie，http或HTTP时，登录phpmyadmin需要数据用户名和密码进行验证，,具体如下：   PHP安装模式为Apache，可以使用http和cookie；   PHP安装模式为CGI，可以使用cookieE.短语密码(blowfish_secret)的设置> $cfg['blowfish_secret'] = '';   如果认证方法设置为cookie，就需要设置短语密码，置于设置为什么密码，由您自己决定![](static/image/smiley/default/lol.gif)  ，但是不能留空，否则会在登录phpmyadmin时提示错误  好了，到此为止，您已经成功安装了phpmyadmin，简单吧:) ，赶快登录体验下吧说明：该文档说明的只是安装phpmyadmin的基本配置，关于config.default.php文件中各个配置参数的详细说明可以参考：[http://www.discuz.net/viewthread.php?tid=50789&highlight=phpmyadmin](http://www.discuz.net/viewthread.php?tid=50789&highlight=phpmyadmin)|简单的说，phpmyadmin就是一种mysql的管理工具，安装该工具后，即可以通过web形式直接管理mysql数据，而不需要通过执行系统命令来管理，非常适合对数据库操作命令不熟悉的数据库管理者，下面我就说下怎么安装该工具：1.先到网上下载phpmyadmin，再解压到可以访问的web目录下（如果是虚拟空间，可以解压后通过ftp等上传到web目录下），当然您可以修改解压后该文件的名称。2.配置config文件  打开libraries下的config.default.php文件，依次找到下面各项，按照说明配置即可：A.访问网址> $cfg['PmaAbsoluteUri'] = '';这里填写phpmyadmin的访问网址B.mysql主机信息> $cfg['Servers'][$i]['host'] = 'localhost'; // MySQL hostname or IP address> 填写localhost或mysql所在服务器的ip地址，如果mysql和该phpmyadmin在同一服务器，则按默认localhost$cfg['Servers'][$i]['port'] = ''; // MySQL port - leave blank for default portmysql端口，如果是默认3306，保留为空即可C.mysql用户名和密码> $cfg['Servers'][$i]['user'] = 'root'; // MySQL user > 访问phpmyadmin使用的mysql用户名fg['Servers'][$i]['password'] = ''; // MySQL password (only needed对应上述mysql用户名的密码D.认证方法> $cfg['Servers'][$i]['auth_type'] = 'cookie';   在此有四种模式可供选择，cookie，http，HTTP，config   config方式即输入phpmyadmin的访问网址即可直接进入，无需输入用户名和密码，是不安全的，不推荐使用。   当该项设置为cookie，http或HTTP时，登录phpmyadmin需要数据用户名和密码进行验证，,具体如下：   PHP安装模式为Apache，可以使用http和cookie；   PHP安装模式为CGI，可以使用cookieE.短语密码(blowfish_secret)的设置> $cfg['blowfish_secret'] = '';   如果认证方法设置为cookie，就需要设置短语密码，置于设置为什么密码，由您自己决定![](static/image/smiley/default/lol.gif)  ，但是不能留空，否则会在登录phpmyadmin时提示错误  好了，到此为止，您已经成功安装了phpmyadmin，简单吧:) ，赶快登录体验下吧说明：该文档说明的只是安装phpmyadmin的基本配置，关于config.default.php文件中各个配置参数的详细说明可以参考：[http://www.discuz.net/viewthread.php?tid=50789&highlight=phpmyadmin](http://www.discuz.net/viewthread.php?tid=50789&highlight=phpmyadmin)|
|----|----|----|
|简单的说，phpmyadmin就是一种mysql的管理工具，安装该工具后，即可以通过web形式直接管理mysql数据，而不需要通过执行系统命令来管理，非常适合对数据库操作命令不熟悉的数据库管理者，下面我就说下怎么安装该工具：1.先到网上下载phpmyadmin，再解压到可以访问的web目录下（如果是虚拟空间，可以解压后通过ftp等上传到web目录下），当然您可以修改解压后该文件的名称。2.配置config文件  打开libraries下的config.default.php文件，依次找到下面各项，按照说明配置即可：A.访问网址> $cfg['PmaAbsoluteUri'] = '';这里填写phpmyadmin的访问网址B.mysql主机信息> $cfg['Servers'][$i]['host'] = 'localhost'; // MySQL hostname or IP address> 填写localhost或mysql所在服务器的ip地址，如果mysql和该phpmyadmin在同一服务器，则按默认localhost$cfg['Servers'][$i]['port'] = ''; // MySQL port - leave blank for default portmysql端口，如果是默认3306，保留为空即可C.mysql用户名和密码> $cfg['Servers'][$i]['user'] = 'root'; // MySQL user > 访问phpmyadmin使用的mysql用户名fg['Servers'][$i]['password'] = ''; // MySQL password (only needed对应上述mysql用户名的密码D.认证方法> $cfg['Servers'][$i]['auth_type'] = 'cookie';   在此有四种模式可供选择，cookie，http，HTTP，config   config方式即输入phpmyadmin的访问网址即可直接进入，无需输入用户名和密码，是不安全的，不推荐使用。   当该项设置为cookie，http或HTTP时，登录phpmyadmin需要数据用户名和密码进行验证，,具体如下：   PHP安装模式为Apache，可以使用http和cookie；   PHP安装模式为CGI，可以使用cookieE.短语密码(blowfish_secret)的设置> $cfg['blowfish_secret'] = '';   如果认证方法设置为cookie，就需要设置短语密码，置于设置为什么密码，由您自己决定![](static/image/smiley/default/lol.gif)  ，但是不能留空，否则会在登录phpmyadmin时提示错误  好了，到此为止，您已经成功安装了phpmyadmin，简单吧:) ，赶快登录体验下吧说明：该文档说明的只是安装phpmyadmin的基本配置，关于config.default.php文件中各个配置参数的详细说明可以参考：[http://www.discuz.net/viewthread.php?tid=50789&highlight=phpmyadmin](http://www.discuz.net/viewthread.php?tid=50789&highlight=phpmyadmin)| | |
