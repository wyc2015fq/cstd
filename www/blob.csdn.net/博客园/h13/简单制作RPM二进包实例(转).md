# 简单制作RPM二进包实例(转) - h13 - 博客园
有好多朋友问到怎么制作rpm包，可不可把其它服务器上编译好的软件目录复杂到其它服务器上直接应用等等。。。这里做个简单的介绍，高级复杂的不会。
此方法是通过编写spec文件，使用rpmbuild来完成一个rpm的打包。
以nginx为例进行介绍
制作平台:centos 5.x X86_64
四步走：
第一步：建立目录结构-
mkdir /usr/src/RedHat/{SOURCES,SPECS,BUILD,RPMS,SRPMS} -p
相关目录介绍：
/usr/src/RedHat/SOURCES    #存放源代码、补丁等文件 /usr/src/RedHat/SPECS      #存放用于管理rpm制作进程的spec文件 /usr/src/redhat/BUILD      #解压后的文件存放目录 /usr/src/RedHat/RPMS       #存放由rpmbuild制作好的二进制包 /usr/src/redhat/SRPMS      #存放由rpmbuild制作好的源码包 第二步：把源码包放在SOURCES目录下
cd /usr/src/RedHat/SOURCES
wget [http://nginx.org/download/nginx-1.2.0.tar.gz](http://nginx.org/download/nginx-1.2.0.tar.gz)
第三步：生成nginx.spec文件
cd /usr/src/redhat/SPECS cat nginx.spec # # spec file for nginx # Build 2012-07-17 # By opsren # Summary: High performance web server Name: Nginx Version: 1.2Release: 0.el5.ngx License: 2-clause BSD-like license Group: Applications/Server Source: [http://nginx.org/download/nginx-1.2.0.tar.gz](http://nginx.org/download/nginx-1.2.0.tar.gz) URL: [http://nginx.org](http://nginx.org/) Distribution: Centos/RedHat Packager: qiuzhijun <[250621008@qq.com](mailto:250621008@qq.com)>   %description Nginx ("engine x") is a high performance HTTP and reverse proxy server, as well as a mail(IMAP/POP3/SMTP) proxy server. %prep tar zxf $RPM_SOURCE_DIR/nginx-1.2.0.tar.gz %build cd nginx-1.2.0 ./configure --prefix=/usr/local/webserver/nginx --with-http_stub_status_module --with-http_ssl_module --with-pcre --lock-path=/var/run/nginx.lock --pid-path=/var/run/nginx.pid make %install cd nginx-1.2.0 make install %preun if [ -z "`ps aux | grep nginx | grep -v grep`" ];then pkill nginx >/dev/null exit 0 fi %files /usr/local/webserver/nginx  第四步：RPM包制作
首先系统要安装好必备的制作工具：gcc、rpmbuild等
yum -y install gcc rpm-build pcre-devel cd /usr/src/RedHat/SPECS/ rpmbuild -bb nginx.spec 通过上面这条命令，会在/usr/src/RedHat/RPMS/x86_64/下面生成nginx-1.2.0-1.el5.ngx.x86_64.rpm这个文件
-bb 这个选项就是制作二进制包（build binary package only from <specfile>）
对spec文件内容进行简单说明：
spec文件是制作rpm包的核心！
以#开头的是注释信息; Summary:对相关软件进行简单描述说明 Name:定义rpm包的名称 Version:定义软件的版本号 Release:发行版本 License:定义许可证 Group:说明软件属于哪种应用类型 Source:软件源码下载地址 URL:软件相关官方站点 Distribution: 发行版系列 Packager: 制作人的简单信息   %description:软件详细描述信息 %prep:软件编译之前的处理 %build:编译软件 %install:安装软件 %preun:定义卸载之前的动作 %files:指定要打包的软件包，这里是/usr/local/webserver/nginx  对于更详细的说明请参考官方资料：[http://www.rpm.org/max-rpm/ch-rpm-inside.html](http://www.rpm.org/max-rpm/ch-rpm-inside.html)
下面是apache的spec文件实例：
# # spec file for apache # Build 2012-07-17 # By opsren # Summary: High stability web server Name: Apache Version: 2.2Release: 22.el5License: 2-clause BSD-like license Group: Applications/Server Source: [http://apache.etoak.com/httpd/httpd-2.2.22.tar.gz](http://apache.etoak.com/httpd/httpd-2.2.22.tar.gz) URL: [http://apache.org](http://apache.org/) Distribution: Centos/RedHat Packager: qiuzhijun <[250621008@qq.com](mailto:250621008@qq.com)>   %description Apache is a first web server %prep tar zxf $RPM_SOURCE_DIR/httpd-2.2.22.tar.gz %build cd httpd-2.2.22 ./configure --prefix=/usr/local/webserver/apache --enable-so --enable-deflate --enable-headers --enable-mods-shared=all --enable-rewrite make %install cd httpd-2.2.22 make install %preun if [ -z "`ps aux | grep httpd | grep -v grep`" ];then pkill httpd >/dev/null exit 0 fi %files /usr/local/webserver/apache  以后对于相同或类似平台可以到其它服务器上进行rpm安装部署。
另外还有一种rpm打包的方法：rpm_create
这是一种新的打rpm的工具，不用spec语言，只需要会简单的shell命令，即可完成打包操作，非常方便，结合了spec语言和checkinstall，相比spec方法要简单很多！
官方站点：[http://code.google.com/p/rpmcreate/](http://code.google.com/p/rpmcreate/)
下载站点：wget [http://rpmcreate.googlecode.com/files/rpm_create-1.7.5-9.x86_64.rpm](http://rpmcreate.googlecode.com/files/rpm_create-1.7.5-9.x86_64.rpm)
大家可以去官方站点参考！
本篇文章来源于 Linux公社网站([www.linuxidc.com](http://www.linuxidc.com/))  原文链接：[http://www.linuxidc.com/Linux/2012-09/70096.htm](http://www.linuxidc.com/Linux/2012-09/70096.htm)
