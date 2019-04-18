# Linux下如何利用Sendmail构建安全可靠的邮件服务器 - weixin_33985507的博客 - CSDN博客
2009年06月15日 23:38:37[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：25
**Linux下如何利用Sendmail构建安全可靠的邮件服务器**
**实验原理：**
       Sendmail是UNIX/Linux环境中稳定性较好的一款邮件服务器软件，通过对Sendmail服务器的配置可以实现基本的邮件转发功能；dovecot服务器实现了POP3协议，可以与Sendmail服务器配合工作，实现用户对邮件的收取功能；OpenWebmail是网页形式的邮件应用系统，可实现用户对邮件的发送、收取和管理功能。通过对Sendmail、dovecot和OpenWebmail的综合管理，系统管理员可以构建出功能完善的邮件应用系统。
**实验步骤：**
**一、 ****搭建DNS****环境，并设置sendmain****的主机域名**
可以通过修改“/etc/hosts”和“/etc/sysconfig/network”两个文件更改主机域名。并在DNS服务器中添加对应的MX记录。
![clip_image002[5]](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080241OUsf.jpg)
![clip_image004](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080244i8hI.jpg)
**二、 ****Sendmail****的安装与配置**
Sendmail服务器在RHEL4系统中是被默认安装的，软件包的名称是sendmail，m4软件包提供了配置Sendmail服务器必须的工具程序，与Sendmail软件包一同默认安装在系统中。Sendmain和m4两个软件都在RHEL4的第2张安装光盘中。
![clip_image006](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080246kCUs.jpg)
更改sendmial服务器的启动级别。
![clip_image008](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080247zPdj.jpg)
设置local-host-names文件，用于设置邮件服务器提供邮件服务的域名。
![clip_image010](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080248w9TI.jpg)
![clip_image012](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080249bEDi.jpg)
编辑sendmail.mc文件
Sendmail服务器的主配置文件是sendmain.cf，该文件配置及其难，不过可以通过配置sendmail.mc然后生成sendmail.cf文件。
![clip_image014](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_12450802500r2t.jpg)
开启Sendmail服务器的网络接口，Sendmail服务器出于安全考虑，默认只对lo网络接口（IP地址为127.0.0.1）提供服务，为了使服务器能够为主机的所有网络接口（地址为0.0.0.0）提供服务，需要在sendmail.mc文件中进行配置的修改。
![clip_image016](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080251GKBh.jpg)
开启SMTP的用户认证功能。将TRUST_AUTH_MECH(‘EXTERNAL DIGEST-MD5 CRAM-MD5 LOGIN PLAIN’)前面的dnl以及空格去掉即可。开启之后，邮件服务器要求用户提供用户账户和口令进行身份认证，只有通过身份认证的用户才能通过邮件服务器向外部发送邮件。
![clip_image018](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080253AyPZ.jpg)
进入/etc/mail目录，然后使用m4 sendmail.mc > sendmail.cf生成sendmail.cf文件。
![clip_image020](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080254nzu5.jpg)
启动/etc/rc.d/init.d/sendmail服务，使sendmial.cf配置生效
![clip_image022](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080256V7ax.jpg)
开启了Sendmail服务器的认证功能之后，需要确保saslauthd服务程序正确运行。
![clip_image024](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080258x1Vj.jpg)
邮件用户账户的设置
Sendmail服务器使用Linux系统中的用户账户作为邮件账户，因此为用户添加邮件账户只需要添加Linux账户即可。为了方便用户管理可以将具有同种性质的用户加入某一个组中。
![clip_image026](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080262VkeD.jpg)
设置邮件别名和邮件群发功能
在Sendmail服务器中使用aliases机制实现邮件别名和邮件群发功能。在/etc目录下同时存在名为aliases和aliases.db的两个文件。aliases文件是文本文件，其内容是可阅读和编辑的，aliases.db是数据库文件，是由aliases文件生成而来的。
![clip_image028](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080264nv9W.jpg)
编辑aliases文件，在文件末尾添加“gongcheng： xiaonuo1，xiaonuo2”，[意思是通过给gongcheng@xiaonuo.com](mailto:%E6%84%8F%E6%80%9D%E6%98%AF%E9%80%9A%E8%BF%87%E7%BB%99gongcheng@xiaonuo.com)发邮件，群发到[xiaonuo1@xiaonuo.com](%E5%8F%91%E7%9A%84%E8%90%A8%E8%8A%AC)和[xiaonuo2@xiaonuo.com](%E5%8F%91%E7%9A%84%E8%90%A8%E8%8A%AC)中
![clip_image030](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080265wVpZ.jpg)
设置完成之后，通过newaliases生成aliases.db文件。
![clip_image032](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080266oLd9.jpg)
如果想让一个用户通过匿名的方式向外发送邮件，可以通过修改access文件，然后通过“makemap hash access.db < access”生成数据库文件。
![clip_image034](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080268Ebm4.jpg)
![clip_image036](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080269DVXp.jpg)
**三、 ****devecot****的安装与配置**
Sendmail服务器并不为MUA软件提供收取邮件的功能，因此系统中需要单独安装实现POP3或IMAP4功能的服务器程序。
系统默认安装了devecot软件，由于devecot服务器以来的软件包比较多，因此建议使用RHEL4系统中提供的软件包管理程序来安装dovecot软件包。
![clip_image038](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_12450802710GaI.jpg)
![clip_image040](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_12450802741YD5.jpg)
![clip_image042](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080277Zha0.jpg)
安装好dovecot软件包之后，需要设置其配置文件dovecot.conf文件。
![clip_image044](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080278ViqT.jpg)
dovecot服务器默认只提供imap服务，需要修改“#protocols = imap impas”这一行为“protocols = imap imaps pop3 pop3s”
![clip_image046](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_12450802808NxX.jpg)
重启启动dovecot服务，使dovecot.conf的设置生效。
![clip_image048](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080282s6GL.jpg)
**四、 ****邮件客户端Outlook Express****的配置**
使用Windows自带的Outlook Express软件连接sendmail服务器。注意设置完成之后，需要将服务器的“我的服务器要求身份验证”打上勾。
![clip_image050](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_12450802845wNu.jpg)
发送、接收电子邮件进行测试即可。
![clip_image052](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080288Uwn8.jpg)
**五、 ****安装配置OpenWebmail****，并通过Web****发表Webmail**
OpenWebmail是使用Perl语言编写的Webmail应用系统，可以很好地和Sendmail等邮件系统配合，构建功能强大的Webmail应用。
下载OpenWebmail所需要的文件，其中有四个perl软件和一个openwebmail软件。
![clip_image054](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080292WGM8.jpg)
建立dbm.conf文件
Openwebmail软件将所有的文件都安装在目录“/var/www/cgi-bin/openwebmail”中，在该目录下的etc子目录中需要建立名为dbm.conf的配置文件。
![clip_image056](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080294YPlr.jpg)
初始化OpenWebmail系统
执行“/var/www/cgi-bin/openwebmail/openwebmail”目录中的openwebmail-tool.pl程序，并使用“--init”选项进行OpenWebmail系统的初始化过程。
![clip_image058](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080297HK5U.jpg)
启动Apache服务器。使客户机可以通过web的方式访问邮件服务器。
![clip_image060](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080298GhTE.jpg)
在IE浏览器中输入[http://mail.xiaonuo.com/cgi-bin/openwebmail/openwebmail.pl访问sendmail](http://mail.xiaonuo.com/cgi-bin/openwebmail/openwebmail.pl%E8%AE%BF%E9%97%AEsendmail)服务器。
![clip_image062](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080301fBsq.jpg)
![clip_image064](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080305HZBQ.jpg)
由于默认访问的页面是英文的，使用起来不太方便，可以通过修改openwebmail.conf配置文件，将页面改成中文。
![clip_image066](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080306rNwP.jpg)
domainnames xiaonuo.com 设置sendmail的域名为xiaonuo.com
![clip_image068](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080307BnCF.jpg)
default_language 设置系统默认的语言为简体中文
![clip_image070](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080309qWpB.jpg)
default_iconset Cool3D.Chinese.Simplified 设置系统中的图标由英文设置为简体中文
![clip_image072](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080310ztQT.jpg)
设置完成之后，使用IE浏览器重新登录即可生效。
![clip_image074](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_1245080313KQ1M.jpg)
![clip_image076](http://dreamfire.blog.51cto.com/attachment/200906/15/418026_12450803168iK1.jpg)
