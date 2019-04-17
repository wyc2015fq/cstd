# 项目管理实践之版本控制工具SVN - 工作笔记 - CSDN博客





2014年02月11日 09:08:27[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7359








SVN，即Subversion，是一个自由开源的版本控制系统，可以将数据恢复到早期版本，或者检查数据修改的历史，这些数据可以是源代码，也可以是其他类型的文件。

在SVN出现之前，CVS是开源世界版本控制工具的事实标准，然而CVS存在一些固有的缺陷，并且修复这些缺陷需要耗费很大的精力，因此，SVN的发起者Brian Behlendorf和CollabNet决定重新编写一个版本控制系统，保留CVS的基本思想，但要修正其中错误和不合理的特性。于是，SVN作为CVS的继任者出现了。SVN的设计者们力图通过两方面的努力赢得CVS用户的青睐：保持开源系统的设计以及界面风格与CVS尽可能类似，同时尽力弥补CVS许多显著的缺陷。这些努力的结果使得从CVS迁移到SVN不需要作出重大的变革，因此越来越多的人选择了SVN。[http://svnbook.red-bean.com](http://svnbook.red-bean.com/)
[http://www.subversion.org.cn](http://www.subversion.org.cn/)


目录


一、客户端的使用

　1.1 Linux系统下一般使用（Ubuntu）

　1.2 Windows系统下一般使用

　1.3 Linux下使用SVN+ssh认证（未找到相关资料@_@）

　1.4 Windows下使用SVN+ssh认证

二、服务器端的配置

　2.1 Linux下的svnserve配置

　2.2 Windows下的svnserve配置

　2.3 Linux下的svnserve+ssh配置

　2.4 Windows下的svnserve+ssh配置（需使用Cygwin，略）

　2.5 Linux下基于APache的SVN服务器配置

　2.6 Windows下基于APache的SVN服务器配置

三、建立版本库

　3.1 Linux下创建版本库

　3.2 Windows下创建版本库

大多数人都是从客户端开始使用SVN，以下先介绍客户端的使用。假设已经假设好了SVN服务器，其文件夹地址为[http://domain/svn/trunk/myproject](http://domain/svn/trunk/myproject)，用户名为test，密码为test。（如果服务器端配置的是SVN，则使用svn://开头的URL访问；如果服务器端配置的是SVN+SSH，则使用svn+ssh开头的URL访问）

**一、客户端的使用**

**1.1 Linux（Ubuntu）系统下一般使用**

1）首先需要安装svn客户端，ubuntu下使用$sudo apt-get install subversion（其他请baigoogledu，余同）

2）checkeout命令：第一次使用时使用checkout命令，把服务器的目录拷贝到本地的当前目录下，同时会建立一个隐藏文件夹记录版本信息：

　　[工作目录]$svn checkout "[http://domain/svn/trunk/myproject](http://domain/svn/trunk/myproject)"
 --username test

然后输入密码

3）svn update命令：获取服务器上的最新版本

　　[工作目录]$svn update（除了第一次要加url和用户名和密码，之后系统会记住）

4）svn add命令：要把非版本控制的本地文件添加到版本控制：

　　[工作目录]$svn add hello.c

5）svn commit命令：把本地文件上传到服务器

[工作目录]$svn commit（如果有新的文件，要首先svn add）

**1.2 Windows系统下一般使用**

1）安装客户端：[http://tortoisesvn.net/downloads](http://tortoisesvn.net/downloads)

2）新建一个文件夹（工作目录），右击选择checkout，填写URL和用户名密码

3）工作目录右键update

4）工作目录右键add

5）工作目录右键commit

**1.3 Linux下使用SVN+ssh认证**（未找到相关资料@_@）

(注：[SSH](http://baike.baidu.com/subview/16184/5909253.htm) 为[Secure Shell](http://baike.baidu.com/view/2118359.htm) 的缩写，由 IETF 的[网络](http://baike.baidu.com/view/3487.htm)工作小组（Network
 Working Group）所制定；**SSH 为建立在应用层和传输层基础上的安全协议**。**SSH 是目前较可靠，专为**[**远程登录**](http://baike.baidu.com/view/59099.htm)**会话和其他网络服务提供安全性的协议。利用 SSH 协议可以有效防止远程管理过程中的信息泄露问题。**SSH最初是UNIX系统上的一个程序，后来又迅速扩展到其他操作平台。SSH在正确使用时可弥补网络中的漏洞。SSH客户端适用于多种平台。几乎所有U
 NIX平台—包括[HP-UX](http://baike.baidu.com/view/58963.htm)、[Linux](http://baike.baidu.com/view/1634.htm)、[AIX](http://baike.baidu.com/view/349664.htm)、[Solaris](http://baike.baidu.com/subview/329359/5113665.htm)、[Digital](http://baike.baidu.com/view/428214.htm)[UNIX](http://baike.baidu.com/view/8095.htm)、[Irix](http://baike.baidu.com/view/3373083.htm)，以及其他平台—都可运行SSH。)

**1.4 Windows下使用SVN+ssh认证**

（参考[ubuntu下架设svn服务器及在windows建立svn+ssh客户](http://blog.sina.com.cn/s/blog_4dd475390100bgs2.html)）

1.4.0 安装TortoiseSVN、Puttygen、Pageant
[http://sourceforge.net/projects/tortoisesvn](http://sourceforge.net/projects/tortoisesvn)
[http://www.chiark.greenend.org.uk/~sgtatham/putty/](http://www.chiark.greenend.org.uk/~sgtatham/putty/)

1.4.1 转换私钥格式

　1）将Linux下的文件<username>key拷贝到windows下，运行Puttygen；

    2）选择菜单conversions->Import Key；选择文件<username>key，提示"Enter passphrase for key"，输入创建公私钥对示使用的passphrase关键字;

    3）选择Parameters为“SSH-2 DSA”或“SSH-2 RSA”->Save private key->保存文件名为username>key.ppk。

1.4.2 建立TortoiseSVN与Pageant的关联，并将私钥加入Pageant：

    1）鼠标右键选择TortoiseSVN->Settings->Network->SSH client，输入：

    　C:\Program Files\TortoiseSVN\bin\TortoisePlink.exe

    2）鼠标右键选择TortoiseSVN->RepoBrowser 输入URL：

　　svn+ssh://<username>@<SvnServiceIP>/usr/local/svn/trunk

    3）运行Pageant，右键点击屏幕右下角的图标-〉Add Key，将私钥文件<username>key.ppk加入。

——如果不想缓存ssh密码，则第8、9步不需要，只保留第二步，但每次check out、check in中每进入一个文件夹都要输入两次密码，烦死你:)

**二、服务器端的配置**

Web服务器部署可以有三种选择，配置由简单到复杂排列为

·svnserve

·svnserve over SSH

·Apache+mod_dav_svn模块

下面从最简单的入手，介绍svnserve。

**[更新]Windows下服务器端的配置可以使用VisualSVN Server进行傻瓜化安装。**

官方网站：[http://www.visualsvn.com/](http://www.visualsvn.com/)

参考链接：[VisualSVN系列介绍](http://hi.baidu.com/caichaowei/blog/item/7a7e1bddc8e470d18c102966.html)（有详细的安装过程介绍，这里就不转述了）

2.1&2.2 配置svnserve

svnserve是一个轻型的服务器，可以同客户端通过在TCP/IP基础上的自定义有状态协议通讯，客户端通过使用开头为svn://或者svn+ssh://svnserve的URL来访问一个svnserve服务器。

（注：**linux的比较完整功能的svn server可以用svn+apache+ssl搭建，有时候为了灵活并不需要这样的一个完全svn server，只要一个轻量级的svn server进行简单的代码管理，这时用svnserve即可实现这样的功能，既简单灵活，同时性能也非常好，完全可以完成简单的代码管理。）**

**2.1 Linux下的svnserve配置**

2.1.0 同样地，使用命令$sudo apt-get install subversion

2.1.1 svnserve作为独立守护进程，监听请求

　　$svnserve -d

　　$ #svnserve is now running, listening on port 3690

　　——可以使用--listen-port=[端口号]来指定端口，或者--listen-host=[主机名]来指定主机名

　　假定已经建立一个版本库位于/usr/local/repositories/project路径（版本库的建立稍后提及）， 此时客户端可以使用svn://[主机]/usr/local/repositories/project来进行访问

　　——可以使用-r选项来限制只输出指定路径下的版本库，从而使客户端访问更为简洁：

　　$svnserve -d -r /usr/local/repositories

　　则客户端只要使用svn://[主机]/project就可以访问

2.1.2 通过inetd使用svnserve

　　$svnserve -i

　　——此时svnserve会尝试使用自定义协议通过stdin和stdout来与subversion客户端通话，默认端口为3690。可以在/etc/services添加如下几行：

　　svn 3690/tcp #subversion

　　svn 3690/udp #subversion

　　——如果是使用经典的类Unix的inetd守护进程，可以在/etc/inetd.conf添加如下行，则如果有客户连接来到端口3690，inetd会产生一个svnserve进程来做服务

　　svn stream tcp nowait svnowner /usr/bin/svnserve svnserve -i

2.1.3 设置svnserve的内置认证

编辑版本库目录的conf/svnserve.conf文件：

　1）设置匿名访问

　　[general]

　　anon-access = none #无权限

　　anon-access = read #只读

　　anon-access = write #打开匿名的写访问，但这样很危险，不能控制谁来修改版本库

　2）使用用户和密码

　2.1）定义保存用户名和密码的文件和一个认证域

　　[general]

　　password-db = <userfile> #指出保存用户和密码列表的文件，可以是绝对或相对路径

　　realm = example realm #定义认证域

　　anon-access = read

　　auth-access = write

　2.2）然后在userfile文件中指明用户名和密码：（格式为“用户名=密码”）

　　[users]

　　harry = foopassword

　　sall = barpassword

**2.2 Windows下的svnserve配置**

2.2.0 安装svnserve

1）从[http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=91](http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=91)得到最新版本的Subversion

（如果之前已经安装了subversion，svnserve已经在服务器上运行，需要先停止服务）

2）安装subversion，然后进入subversion的安装目录，找到bin目录下的文件svnserve.exe，int13_svn.dll，libapr.dll，libapriconv.dll，libapriutil.dll，libdb*.dll，libeay32.dll和ssleay32.dll，将它们复制到服务器目录下（如C:\svnserve）。

2.2.1 以守护方式运行svnserve

在命令行窗口输入svnserve.exe --daemon，则svnserve将会在端口3690等待请求，--daemon选项告诉svnserve以守护进程方式运行，这样在手动终止之前不会退出。这种方式的不便之处是需要保持命令行窗口不关闭。

2.2.2 svnserve作为windows服务

svnserve作为windows服务时，在后台运行，可以在启动时自动执行，并且可以使用同其他windows服务一致的管理界面启动和停止服务。

需要使用命令行工具sc.exe定义新的服务（create svnserve），在windows启动时指明svnserve的调用：

　　C:\>sc create svnserve

　　　binpath= "\"c:\svnserve\svnserve.exe\" --service --root c:\repos"

　　　displayname= "Subversion"

　　　depend= tcpip

　　　start= auto

　　——注意为了回避目录名中的空格等字符，将整个binpath值放在双引号中；另外，sc 的命令行很特殊。在 key= value 对中，key 与 = 之间不能有空格，但是在 value 之前，必须有空格。

定义了服务之后，就可以使用GUI工具（如服务管理面板）进行停止、启动和查询，也可以通过命令行的方式进行：

　　C:\>net stop svn

　　C:\>net start svn

　　C:\>sc delete svn（需要确定先停止了服务）

2.2.3 设置svnserve的内置认证

同上Linux配置文件即可。

2.3&2.4 配置svnserve+ssh

**2.3 Linux下的svnserve+ssh配置**

2.3.0 介绍与安装ssh

　　通过此方法，svnserve不会作为守护进程启动，而是SSH来启动svnserve，以SSH授权用户进行。

　　$sudo apt-get install ssh

2.3.1 创建帐户

　　$sudo adduser <username> #为参与项目开发的成员建立用户帐户

　　$sudo addgroup <groupname> #建立用户组

　　$sudo addgroup <username> <groupname> #将用户帐户添加进用户组

2.3.2 为成员生成密钥对

　　首先必须切换为<username>的用户登陆Linux：

　　$sudo <username>

　　$（提示输入密码）

　　$cd ~/

　　$whoami

　　<username>$ssh-kegen -b 1024 -t dsa -N <passphrase> -f <filename> #使用DSA加密，也可以改为RSA加密

　　——这时产生两个文件：私钥<username>key和公钥<username>key.pub

　　然后创建目录/home/<username>/.ssh，把公钥拷贝到该目录下：

　　$cp <username>key.pub /home/<username>/.ssh/authrized_keys #文件名必须为authorized_keys

**2.4 Windows下的svnserve+ssh配置**

需要使用Cygwin来配置ssh，此处略，详情可参考[http://tortoisesvn.net/node/156](http://tortoisesvn.net/node/156)

**2.5&2.6 基于Apache服务器的SVN配置**

**（注：**Apache是世界使用排名第一的Web[服务器](http://baike.baidu.com/view/899.htm)软件。它可以运行在几乎所有广泛使用的[计算机平台](http://baike.baidu.com/view/2269685.htm)上，由于其[跨平台](http://baike.baidu.com/view/469855.htm)和安全性被广泛使用，是最流行的Web服务器端软件之一。同时Apache音译为阿帕奇，是北美印第安人的一个部落，叫阿帕奇族，在美国的西南部。也是一个基金会的名称、一种武装直升机等等。**）**

概述

Apache的http服务器是一个subversion可以利用的“重型”网络服务器，通过一个自定义模块，httpd可以让SVN版本库通过WebDAV/DeltaV协议在客户端前可见。可以将浏览器指向版本库的URL，无需安装SVN客户端就可以浏览内容。

基于Apache的SVN需要两个包的四个部分：Apache httpd2.0及其中包含的mod_dav DAV模块，Subversion及与之一起发布的mod_dav_svn模块。有了上述组件之后，配置步骤一般如下

　1）配置好http2.0，并且使用mod_dav启动

　2）为mod_dav安装mod_dav_svn插件

　3）配置httpd.conf来发布版本库

**2.5 Linux下基于APache的SVN服务器配置**

（参考[http://www.123shang.cn/blog/?p=53](http://www.123shang.cn/blog/?p=53)）

（参考“ubuntu下svn+apache版本管理服务器配置”）

2.5.1 安装aphache

　　$sudo apt-get install apache2 #安装apache

　　$sudo apt-get install libapache2-svn #安装相关组件

　　$sudo apt-get install subversion #安装svn

　　安装完毕后启动apache

　　$/usr/local/apache/bin/apachectl –k start

　　——用浏览器查看[http://localhost/](http://localhost/),得到it works，说明apache已经配置成功了。

2.5.2 创建版本库（见第三节）

2.5.3 添加用户并设置密码文件

版本库的密码文件，一般命名为PasswdFile，由用户通过命令创建，里面保存授权用户的用户名和MD5加密后的密码。创建密码文件，密码文件保存在版本库文件目录下，在终端通过下面的命令创建密码文件：

　　$htpasswd -c /etc/svn/passwordfile username

　　　passwordfile 密码文件名称，可以由用户自行命名

　　　username 你希望添加的用户名，比如zhangsan，lisi，wangwu

　　　-c 用来创建密码文件

　　按提示输入密码，添加成功后会提示add user username，然后可以再继续添加第二个用户。

　　$htpasswd /etc/svn/passwordfile seconduser

2.5.4 httpd.conf文件修改

　　conf文件正常安装的话在etc/apache2/ 目录下，双击打开进行编辑，添加以下内容：

　　LoadModule dav_svn_module modules/mod_dav_svn.so 加载mod_dav_svn模块

　　LoadModule authz_svn_module /usr/lib/apache2/modules/mod_authz_svn.so 加载 mod_anthz.svn模块

　　<Location /vcard>

　　　DAV svn

　　　SVNPath /home/bb/svn/vcard/ 指明版本库的路径

　　　AuthType Basic 认证类型为基本认证

　　　AuthName "SVN repos" 认证名称，将在IE界面弹出一个对话框，其标题就是SVN repos

　　　AuthUserFile /home/bb/svn/vcard/VcardPasswdFile 加载密码文件，访问版本库用户的文件，用apache 的htpasswd命令生成

　　　# AuthzSVNAcessFile //home/bb/svn//vcard/accesspolicy 访问版本库权限的文件

　　　# satisfy Any

　　　Require valid-user 要求验证用户，即不能匿名访问；如果没有这个语句，则只能第一个用户可以访问新建库

　　</Location>

　　保存文件退出，重新启动apache

　　# /usr/local/apache/bin/apachectl –k restart

　　测试：

　　$svn checkout[http://localhost/svn](http://localhost/svn)

**2.6 Windows下基于APache的SVN服务器配置**

2.6.1 下载安装Apache

　　APache：[http://httpd.apache.org/download.cgi](http://httpd.apache.org/download.cgi)

　　——如果你将Apache安装成一个服务，它将以本地system帐户来运行。为Apache创建一个单独的用户来运行它将会更安全一些。

　　——请确保运行Apache的用户有版本库的完全访问权限（右键版本库目录->属性->安全）。要不然，用户将无法提交他们的更改。

　　——就算Apache以本机system来运行，你也要设置它能完全读写版本库目录。

　　——如果没有为Apache配置这个许可，用户将会得到"禁止访问"的错误信息，在Apache的错误日志中表现为错误500。

2.6.2 配置Subversion

使用Windwos资源管理器，来到Subversion的安装目录（通常是c:\program files\Subversion），找到文件httpd/mod_dav_svn.so和mod_authz_svn.so，复制这些文件到Apache的模块目录（通常是c:\program file\apache group\apache2\modules）。

2.6.3 配置httpd.conf文件

　　路径通常为c:\program files\apache group\apache2\conf\httd.conf)，修改以下内容：

　　去掉以下两行的注释号“#”：

　　#LoadModule dav_fs_module modules/mod_dav_fs.so

　　#LoadModule dav_module modules/mod_dav.so

　　在LoadModule节的最后添加以下两行：

　　LoadModule dav_svn_module modules/mod_dav_svn.so

　　LoadModule authz_svn_module modules/mod_authz_svn.so

　　文件末尾添加以下行，让Apache知道哪个URL将被Subversion使用：

　　<Location /svn>

　　　DAV svn

　　　SVNParentPath D:\SVN

　　　AuthType Basic

　　　AuthName "Subversion repositories"

　　　AuthUserFile D:\passwd

　　　#AuthzSVNAccessFile D:\svnaccessfile

　　　Require valid-user

　　</Location>

　　——这样配置表示：你所有的版本库将位于D:\SVN目录下，要访问你的版本库可以使用这样的URL：[http://MyServer/svn/](http://myserver/svn/)，访问权限将由passwd文件中的用户名/密码来限制。

　　要创建passwd文件，可以打开命令行，将当前目录切换到apache2目录，输入命令：

　　c:\program files\apache group\apache2 >bin\htpasswd -c passwd <username>

　　重启Apache服务。

　　使用浏览器测试。

**三、建立版本库**

在前面的服务器配置上我们通常假设已经建立了版本库，这是为了避免在每种配置中重复赘述这种操作，因此把它独立出来讲。

版本库创建完毕后会在目录下生成若干个文件和文件夹：

　　　dav目录是提供给Apache与mod_dav_svn使用的目录，让它们存储内部数据；

　　　db目录就是所有版本控制的数据文件；hooks目录放置hook脚本文件的目录；

　　　locks用来放置Subversion文件库锁定数据的目录，用来追踪存取文件库的客户端；

　　　format文件是一个文本文件，里面只放了一个整数，表示当前文件库配置的版本号。

**3.1 Linux下创建版本库**

　　版本库有两种数据存储格式：Berkeley DB和FSFS，推荐选用FSFS格式，具体可以查阅参考资料了解二者的区别。

　　创建版本库时，在终端运行如下命令：

　　# 运行创建版本库的命令，指定数据存储为 FSFS，如果要指定为 Berkeley DB，则将 fsfs 替换为 bdb

　　$ svnadmin create --fs-type fsfs /etc/svn/repos 创建版本库，运行这个命令后，我们就可以看到一个版本库应有的文件

　　更改版本库的访问权限

　　$ sudo mkdir /home/svn

　　$ cd /home/svn

　　$ sudo mkdir myproject

　　前面三条命令用于创建版本库目录，如果已经建立，就可以省略掉；后面三条用于修改权限

　　$ sudo chown -R www-data myproject

　　$ sudo chgrp -R subversion myproject

　　$ sudo chmod -R g+rws myproject 这条命令允许用户对版本库进行读写操作，即可以更新，可以提交

**3.2 Windows下创建版本库**

可以使用TortoiseSVN图形化的完成这一步，在目录E:subversionrepository下"右键->TortoiseSVN->Create Repository here...“， 然后可以选择版本库模式， 使用默认即可， 然后就创建了一系列目录和文件。

　　[Tips]如果下载的是setup.exe版本,安装程序安装后会自动设置系统变量，如果下载的是zip版就需要手动设置系统变量.

　　也可以使用命令行来创建：

　　C:\> svnadmin create F:\svn\

﻿﻿

﻿﻿

﻿﻿

﻿﻿



