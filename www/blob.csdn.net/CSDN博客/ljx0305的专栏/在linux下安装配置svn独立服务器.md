# 在linux下安装配置svn独立服务器 - ljx0305的专栏 - CSDN博客
2010年07月14日 14:39:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：586标签：[svn																[linux																[服务器																[subversion																[apache																[redhat](https://so.csdn.net/so/search/s.do?q=redhat&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)](https://so.csdn.net/so/search/s.do?q=subversion&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=svn&t=blog)
个人分类：[linux下软件安装																[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)](https://blog.csdn.net/ljx0305/article/category/440323)
                在linux下安装配置svn独立服务器
subversion(以下简称svn)是近年来崛起的版本管理工具，是cvs的接班人。 svn服务器有2种运行方式：独立服务器和借助apache。2种方式各有利弊。
svn存储版本数据也有2种方式：BDB和FSFS。因为BDB方式在服务器中断时，有可能锁住数据（我在搞ldap时就深受其害，没法根治），所以还是FSFS方式更安全一点。
1. svn服务器安装操作系统: Redhat Linux AS4
获取svn安装包
下载http://subversion.tigris.org/downloads/subversion-1.4.0.tar.gz和http://subversion.tigris.org/downloads/subversion-deps-1.4.0.tar.gz。
编译svn以root用户登录。
将subversion-1.4.0.tar.gz和subversion-deps-1.4.0.tar.gz传到服务器。
tar xfvz subversion-1.4.0.tar.gz
tar xfvz subversion-deps-1.4.0.tar.gz
cd subversion-1.4.0
./configure –prefix=/opt/svn –without-berkeley-db –with-zlib
(注：以svnserve方式运行，不加apache编译参数。以fsfs格式存储版本库，不编译berkeley-db)
make clean
make
make install
vi /etc/profile,在/etc/profile最后加入:
PATH=$PATH:/opt/svn/bin
export PATH
svn测试svnserve –version
如果显示如下，svn安装成功:
svnserve, version 1.4.0 (r21228)
compiled Oct 12 2006, 10:18:56Copyright (C) 2000-2006 CollabNet.
Subversion is open source software, see http://subversion.tigris.org/
This product includes software developed by CollabNet (http://www.Collab.Net/).
The following repository back-end (FS) modules are available:
* fs_fs : Module for working with a plain file (FSFS) repository.
2. svn配置建立svn版本库目录可建多个：
mkdir -p /opt/svndata/repos1
mkdir -p /opt/svndata/repos2
建立svn版本库svnadmin create /opt/svndata/repos1
svnadmin create /opt/svndata/repos2
修改svn版本库配置文件版本库1：
vi /opt/svndata/repos1/conf/svnserve.conf
内容修改为:
[general]
anon-access = none
auth-access = write
password-db = /opt/svn/conf/pwd.conf
authz-db = /opt/svn/conf/authz.conf
realm = repos1
版本库2:
vi /opt/svndata/repos2/conf/svnserve.conf
内容修改为:
[general]
anon-access = none
auth-access = write
password-db = /opt/svn/conf/pwd.conf
authz-db = /opt/svn/conf/authz.conf
realm = repos2
即除realm = repos2外，其他与版本库1配置文件完全相同。如果有更多的版本库，依此类推。
配置允许访问的svn用户vi /opt/svn/conf/pwd.conf
为了简化配置，2个版本库共用1个用户配置文件。如有必要，也可以分开。
注意：对用户配置文件的修改立即生效，不必重启svn。
文件格式如下：
[users]
<用户1> = <密码1>
<用户2> = <密码2>
其中，[users]是必须的。下面列出要访问svn的用户，每个用户一行。示例：
[users]
alan = password
king = hello
配置svn用户访问权限vi /opt/svn/conf/authz.conf
为了简化配置，3个版本库共用1个权限配置文件/opt/svn/conf/pwd.conf。如有必要，也可以分开。文件中定义用户组和版本库目录权限。
注意：
* 权限配置文件中出现的用户名必须已在用户配置文件中定义。
* 对权限配置文件的修改立即生效，不必重启svn。
用户组格式：
[groups]
<用户组名> = <用户1>,<用户2>
其中，1个用户组可以包含1个或多个用户，用户间以逗号分隔。
版本库目录格式：
[<版本库>:/项目/目录]
@<用户组名> = <权限>
<用户名> = <权限>
其中，方框号内部分可以有多种写法:
/,表示根目录及以下。根目录是svnserve启动时指定的，我们指定为/opt/svndata。这样，/就是表示对全部版本库设置权限。
repos1:/,表示对版本库1设置权限
repos2:/abc, ,表示对版本库2中的abc项目设置权限
repos2:/abc/aaa, ,表示对版本库2中的abc项目的aaa目录设置权限
权限主体可以是用户组、用户或*，用户组在前面加@，*表示全部用户。权限可以是w、r、wr和空，空表示没有任何权限。
示例：
[groups]
admin = alan
[/]
@admin = rw
[repos1:/abc/aaa]
king = rw
[repos2:/pass]
king =
svn配置完毕，删除无用文件rm /opt/svndata/repos1/conf/authz
rm /opt/svndata/repos1/conf/passwd
rm /opt/svndata/repos2/conf/authz
rm /opt/svndata/repos2/conf/passwd
3. 启动svn建立启动svn的用户useradd svn
passwd svn
根据提示为用户svn设置密码
允许用户svn访问版本库chown -R svn:svn /opt/svndata
chown -R svn:svn /opt/data
启动svnsu - svn -c "svnserve -d --listen-port 9999 -r /opt/svndata"
其中：
su - svn表示以用户svn的身份启动svn
-d表示以daemon方式（后台运行）运行
–listen-port 9999表示使用9999端口，可以换成你需要的端口。但注意，使用1024以下的端口需要root权限
-r /opt/svndata指定根目录是/opt/svndata
检查:
ps -ef|grep svnserve
如果显示如下，即为启动成功：
svn　　　 6941　　 1　0 15:07 ?　　　　00:00:00 svnserve -d –listen-port 9999 -r /opt/svndata
通过web方式访问svn有很多方法，请参阅配置websvn或配置bsSvnBrowser的方法。
参考资料：
http://www.gbunix.com/htmldata/2006_07/15/27/article_1337_1.html
http://www.iusesvn.com/bbs/thread-157-1-1.html
http://16hot.blog.isyi.com/post/1/552 
引用:http://www.acejoy.com/bbs/viewthread.php?tid=767&extra=page%3D1            
