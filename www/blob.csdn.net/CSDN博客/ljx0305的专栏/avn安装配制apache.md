# avn安装配制apache - ljx0305的专栏 - CSDN博客
2013年02月21日 13:27:08[ljx0305](https://me.csdn.net/ljx0305)阅读数：784
介绍
这篇文档介绍如何安装Apache，SVN以及如何结合它们，如何配置多库，如何创建用户帐号等等。该文档由　SVN俱乐部　提供，由arain编写。
联系我们
QQ 群: 21765703
SVN俱乐部官网: [http://www.svnclub.com](http://www.svnclub.com/)
所用到的软件及其版本号和下载站点
温馨提示：您可以直接到SVN俱乐部 网站下载，也可以用以下载方式：
1. Apache: 2.2.14
下载站点：
http://archive.apache.org/dist/httpd/binaries/win32/apache_2.2.14-win32-x86-openssl-0.9.8k.msi
2, Subversion: 1.6.6
下载站点：
http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=91&expandFolder=91&folderID=74
Setup-Subversion-1.6.6.msi
安装Apache
1.      双击apache安装文件：apache_2.2.14-win32-x86-openssl-0.9.8k.msi进行安装，直到遇到如下对话框，填入相关信息按“下一步”。
![](http://hi.csdn.net/attachment/201111/21/0_1321860391kC15.gif)
 2.      完成安装并不是一件很难的事情。但是有一种可能是你无法直接启动httpd服务（通常是因为80端口被其它应用程序占用了）。这时，请不要担心，导航到apache的安装路径，这里我用一个默认的安装路径：
C:\Program Files\Apache SoftwareFoundation\Apache2.2\conf
然后像如下这样编辑配置文件：httpd.conf。
…
ServerRoot "C:/Program Files/Apache SoftwareFoundation/Apache2.2"
#
#Listen: Allows you to bind Apache to specific IP addresses and/or
#ports, instead of the default. See also the <VirtualHost>
#directive.
#
#Change this to Listen on specific IP addresses as shown below to
#prevent Apache from glomming onto all bound IP addresses.
#
#Listen12.34.56.78:80
Listen 8888
#
#Dynamic Shared Object (DSO) Support
#
# Tobe able to use the functionality of a module which was built as a DSO you
# have to place corresponding `LoadModule' lines atthis location so the
#directives contained in it are actually available _before_ they are used.
# Statically compiled modules (those listed by `httpd-l') do not need
…
Apache默认的监听商品是80端口，这里我把它改为8888。然后手动启动httpd服务（我的电脑-管理-服务和应用程序-服务-Apache2.2）。
![](http://hi.csdn.net/attachment/201111/21/0_1321860418T00P.gif)
这时，试着在本机的浏览器里访问：[http://localhost:8888/](http://localhost:8888/)，如果出现如下的界面说明你成功了！你做了一件多么伟大的事情！
![](http://hi.csdn.net/attachment/201111/21/0_1321860422JW7w.gif)
安装Subversion
1.      双击SVN安装文件：Setup-Subversion-1.6.6.msi，你几乎不用做什么操作就可以完成所有安装
2.      把SVN的bin文件夹路径添加到环境变量中，让Aapche能找到所有相关的文件和在电脑的任何地方都可以运用SVN命令，我这里的bin文件夹路径是：。如果你不会怎么添加环境变量，请按下面步骤进行：
a.      单击：我的电脑à属性à高级à环境变量à系统变量à路径（双击进行编辑）à变量值
b.      在path变量值里添加：C:\Program Files\Subversion\bin。注意前后都要加上英文分号。（我放在最后，没有后面的英文分号）
![](http://hi.csdn.net/attachment/201111/21/0_1321860426LbCr.gif)
c.      把Apache的bin路径也要加环境变量里去。我这里的的bin路径是：C:\Program Files\Apache SoftwareFoundation\Apache2.2\bin。
d.     重启电脑，如果不重启可能会遇到一些奇怪的问题。
结合Apache与SVN
1.      从SVN的bin目录复制两个SVN模块(mod_authz_svn.so, mod_dav_svn.so)到Apache的modules目录。
SVN bin 目录：
C:\Program Files\Subversion\bin
Apache modules 目录：
C:\Program Files\Apache Software Foundation\Apache2.2\modules
2.      修改Apache的配置文件：httpd.conf（C:\Program Files\Apache Software Foundation\Apache2.2\conf）。注意加粗字体部分，最后的Location部分添加到最后。
…
# Dynamic Shared Object (DSO) Support
#
# To be able to use the functionality of a modulewhich was built as a DSO you
# have to place corresponding `LoadModule' lines atthis location so the
# directives contained in it are actually available_before_ they are used.
# Statically compiled modules (those listed by `httpd-l') do not need
# to be loaded here.
#
# Example:
# LoadModule foo_module modules/mod_foo.so
#
LoadModule actions_modulemodules/mod_actions.so
LoadModule alias_module modules/mod_alias.so
LoadModule asis_module modules/mod_asis.so
LoadModule auth_basic_module modules/mod_auth_basic.so
#LoadModule auth_digest_modulemodules/mod_auth_digest.so
#LoadModule authn_alias_modulemodules/mod_authn_alias.so
#LoadModule authn_anon_modulemodules/mod_authn_anon.so
#LoadModule authn_dbd_module modules/mod_authn_dbd.so
#LoadModule authn_dbm_module modules/mod_authn_dbm.so
LoadModule authn_default_modulemodules/mod_authn_default.so
LoadModule authn_file_module modules/mod_authn_file.so
#LoadModule authnz_ldap_modulemodules/mod_authnz_ldap.so
#LoadModule authz_dbm_module modules/mod_authz_dbm.so
LoadModule authz_default_modulemodules/mod_authz_default.so
LoadModule authz_groupfile_modulemodules/mod_authz_groupfile.so
LoadModule authz_host_module modules/mod_authz_host.so
#LoadModule authz_owner_modulemodules/mod_authz_owner.so
LoadModule authz_user_module modules/mod_authz_user.so
LoadModule autoindex_module modules/mod_autoindex.so
#LoadModule cache_module modules/mod_cache.so
#LoadModule cern_meta_module modules/mod_cern_meta.so
LoadModule cgi_module modules/mod_cgi.so
#LoadModule charset_lite_module modules/mod_charset_lite.so
**LoadModule dav_module modules/mod_dav.so**
**LoadModule dav_fs_module modules/mod_dav_fs.so**
#LoadModule dav_lock_module modules/mod_dav_lock.so
#LoadModule dbd_module modules/mod_dbd.so
#LoadModule deflate_module modules/mod_deflate.so
LoadModule dir_module modules/mod_dir.so
#LoadModule disk_cache_modulemodules/mod_disk_cache.so
#LoadModule dumpio_module modules/mod_dumpio.so
LoadModule env_module modules/mod_env.so
#LoadModule expires_module modules/mod_expires.so
#LoadModule ext_filter_module modules/mod_ext_filter.so
#LoadModule file_cache_modulemodules/mod_file_cache.so
#LoadModule filter_module modules/mod_filter.so
#LoadModule headers_module modules/mod_headers.so
#LoadModule ident_module modules/mod_ident.so
#LoadModule imagemap_module modules/mod_imagemap.so
LoadModule include_module modules/mod_include.so
#LoadModule info_module modules/mod_info.so
LoadModule isapi_module modules/mod_isapi.so
#LoadModule ldap_module modules/mod_ldap.so
#LoadModule logio_module modules/mod_logio.so
LoadModule log_config_module modules/mod_log_config.so
#LoadModule log_forensic_modulemodules/mod_log_forensic.so
#LoadModule mem_cache_module modules/mod_mem_cache.so
LoadModule mime_module modules/mod_mime.so
#LoadModule mime_magic_module modules/mod_mime_magic.so
LoadModule negotiation_modulemodules/mod_negotiation.so
#LoadModule proxy_module modules/mod_proxy.so
#LoadModule proxy_ajp_module modules/mod_proxy_ajp.so
#LoadModule proxy_balancer_modulemodules/mod_proxy_balancer.so
#LoadModule proxy_connect_modulemodules/mod_proxy_connect.so
#LoadModule proxy_ftp_module modules/mod_proxy_ftp.so
#LoadModule proxy_http_modulemodules/mod_proxy_http.so
#LoadModule rewrite_module modules/mod_rewrite.so
LoadModule setenvif_module modules/mod_setenvif.so
#LoadModule speling_module modules/mod_speling.so
#LoadModule ssl_module modules/mod_ssl.so
#LoadModule status_module modules/mod_status.so
#LoadModule substitute_modulemodules/mod_substitute.so
#LoadModule unique_id_module modules/mod_unique_id.so
#LoadModule userdir_module modules/mod_userdir.so
#LoadModule usertrack_module modules/mod_usertrack.so
#LoadModule version_module modules/mod_version.so
#LoadModule vhost_alias_modulemodules/mod_vhost_alias.so
**LoadModule dav_svn_module modules/mod_dav_svn.so**
**LoadModule authz_svn_module modules/mod_authz_svn.so**
<IfModule !mpm_netware_module>
<IfModule !mpm_winnt_module>
…
**<Location/svn/>**
**Dav svn**
**SVNParentPath "C:/svn/repository"**
**AuthType Basic**
**AuthName "ICIL SVN"**
**SVNListParentPath on**
**AuthUserFile "C:/svn/userfile.txt"**
**AuthzSVNAccessFile "C:/svn/accessfile.txt"**
**Require valid-user**
**</Location>**
**RedirectMatch ^(/svn)$ $1/**
 注释：
SVNParentPath--SVN的库根目录（手动创建）路径，可以在这下面创建多个版本库；
AuthUserFile--------用户帐号信息文件（用htpasswd命令创建文件userfile.txt）；
AuthzSVNAccessFile--权限控制文件（手动创建）
**RedirectMatch ^(/svn)$ $1/-----------重定向路径（/svn定向到/svn/），这样访问库列表可以通过http://ip:port/svn访问**
3.      创建SVN版本库。用svnadmin命令创建库。按下图操作你会发现在C:/svn/repository多了一个MyProject文件夹
注意：你首先要导航到SVNParentPath:C:/svn/repository然后运行该命令。
![](http://hi.csdn.net/attachment/201111/21/0_1321860429111L.gif)
 4.      创建SVN用户帐号文件。在Apache的配置文件里我们设了AuthUserFile 路径:C:/svn/userfile.txt，现在在DOS命令行里导航到文件夹：C:/SVN/运行命令：**htpasswd –c userfile.txt username**创建用户帐号文件，同时创建了一个初始帐号。
![](http://hi.csdn.net/attachment/201111/21/0_132186043228zt.gif)
注意：-c参数表示创建一个用户帐号文件，下一次你再创建其它的帐号的时候就不需要这个参数了。
5.      创建权限控制文件。在配置文件里我们设了AuthzSVNAccessFile 路径：C:/svn/accessfile.txt，现在我们手动创建这个文本文件，同时输入如下内容：
[/]
*=r
[test:/]
arain=rw
jacky=rw
![](http://hi.csdn.net/attachment/201111/21/0_1321860435O1T0.gif)
最后重启一下apache服务，如果不知道如何重启请参看安装Apache的部分。
然后在浏览器里试着访问一下SVN版本库：[http://localhost:8888/svn/MyProject](http://localhost:8888/svn/MyProject)。如果你看到如下图的内容说明你成功了！恭喜你，呵呵！
![](http://hi.csdn.net/attachment/201111/21/0_1321860438I8L8.gif)
引用:http://blog.csdn.net/noodle123/article/details/6996737
