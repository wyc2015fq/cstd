# RPM打包与SPEC文件 - 囧囧有神的专栏 - CSDN博客





2016年09月08日 10:52:56[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：3128









制作RPM软件包并不是一件复杂的工作，其中的关键在于编写SPEC软件包描述文件。要想制作一个rpm软件包就必须写一个软件包描述文件（SPEC）。 这个文件中包含了软件包的诸多信息，如软件包的名字、版本、类别、说明摘要、创建时要执行什么指令、安装时要执行什么操作、以及软件包所要包含的文件列表 等等。




pam_ldap.spec描述文件说明如下：

# 文件规范

## 1.文件头


Summary: PAM module for LDAP.                                          #用一句话概括该软件包尽量多的信息。

Name: pam_ldap                                                                   #软件包的名字，最终RPM软件包是用该名字与版本号，释出号及体系号来命名软件包的。

Version: 185                                                                          #软件版本号。仅当软件包比以前有较大改变时才增加版本号。

Release: 1                                                                             #软件包释出号。一般我们对该软件包做了一些小的补丁的时候就应该把释出号加1。

Source0: ftp://ftp.padl.com/pub/%{name}-%{version}.tar.gz  #源程序软件包的名字。如 stardict-2.0.tar.gz。

Source1: ldap.conf

URL: [http://www.padl.com/](http://www.padl.com/)                                                     #软件的主页

Copyright: LGPL                                                                   #软件包所采用的版权规则。具体有：GPL（自由软件），BSD，MIT，Public Domain（公共域），Distributable（贡献），commercial（商业），Share（共享）等，一般的开发都写GPL。

Group: System Environment/Base                                        #软件包所属类别 系统环境/基础

BuildRoot: %{_tmppath}/%{name}-root                                 #这个是安装或编译时使用的“虚拟目录”，后面可使用$RPM_BUILD_ROOT 方式引用

BuildPrereq: openldap-devel                                                #

Requires: openldap cyrus-sasl openssl                                #该rpm包所依赖的软件包名称，可以用>=或<=表示大于或小于某一特定版本

Obsoletes: pam_ldap




%description                                                                          #软件包详细说明，可写在多个行上。 %开头的是rpm定义的宏命令


This package includes a LDAP access clients: pam_ldap.   




Pam_ldap is a module for Linux-PAM that supports password changes, V2/V3


clients, Netscapes SSL/OpenSSL, ypldapd, Netscape Directory Server password


policies, access authorization, crypted hashes, etc.




Install nss_ldap if you need LDAP access clients.


 这个包包括一个ldap访问客户端： PAM_LDAP




pam_ldap是一个linux PAM模块，支持修改密码，V2/V3客户端，Netscapes SSL/OpenSSL, ypldapd, NetScape目录服务密码策略，访问权限，加密hash等


如果需要LDAP访问客户端，请安装nss_ldap



## 2.%prep段


这个段是预处理段，通常用来执行一些解开源程序包的命令，为下一步的编译安装作准备。


%prep


%setup -q -a 0


setup命令用于启动名称服务与功能。系统配置工具

## 3.build段


本段是建立段，所要执行的命令为生成软件包服务，如make 命令。


./configure


make

## 4.%install段


本段是安装段，其中的命令在安装软件包时将执行，如make install命令。


[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

mkdir -p $RPM_BUILD_ROOT/{etc,lib64/security}


install -m 755 [pam_ldap.so](/) \

               $RPM_BUILD_ROOT/lib64/security/


install -m 644 %{SOURCE1} $RPM_BUILD_ROOT/etc/ldap.conf


chmod 755 $RPM_BUILD_ROOT/lib64/security/*.so*






Install和cp类似，都可以将文件/目录拷贝到指定的地点。但是，install允许你控制目标文件的属性。install通常用于程序的makefile，使用它来将程序拷贝到目标（安装）目录。


本段做的工作是将二进制文件和配置文件(pam_ldap.so, ldap.conf)，拷贝到相应的目录，并赋予相应的权限属性

## 5. %files段


本段是文件段，用于定义软件包所包含的文件，分为三类–说明文档（doc），配置文件（config）及执行程序，还可定义文件存取权限，拥有者及组别。


%defattr(-,root,root)


%attr(0755,root,root) /lib64/security/*.so*


%attr(0644,root,root) %config(noreplace) /etc/ldap.conf


%doc AUTHORS NEWS COPYING COPYING.LIB README ChangeLog pam.d



## 6.%changelog段


本段是修改日志段。你可以将软件的每次修改记录到这里，保存到发布的软件包中，以便查询之用。每一个修改日志都有这样一种格式：第一行是：* 星期 月 日 年 修改人 电子信箱。其中：星期、月份均用英文形式的前3个字母，用中文会报错。接下来的行写的是修改了什么地方，可写多行。一般以减号开始，便于后续的查阅。


* Mon Jan 08 2001 Joe Little <jlittle@[open-it.org](http://open-it.org/)>

- first PAM_LDAP specific RPM, stolen from the previously maintained nss_ldap


* Thu Jan 04 2001 Joe Little <jlittle@[open-it.org](http://open-it.org/)>

- changed Makefile.RPM.openldap2 to a patch instead of a source file

- updated to pam_ldap 86 and nss_ldap 126


之后的省略

# 打包


如果想发布rpm格式的源码包或者是二进制包，就要使用rpmbuild工具（rpm最新打包工具）。如果我们已经根据本地源码包的成功编译安装而写了 spec文件（该文件要以.spec结束），那我们就可以建立一个打包环境，也就是目录树的建立，一般是在/usr/src/redhat/目录下建立5 个目录。它们分别是BUILD、SOURCE、SPEC、SRPM、RPM。


SOURCE用来存放打包是 要用到的源文件和patch


BUILD目录用来存放打包过程中的源文件


SPEC用来存放spec文件，


SRPM、RPM分别存放打包生成的rpm格式的源文件和二进制文件。




当然我们可以根据需要 来选用不同的参数打包文件，笔者总结如下3条。




准备文件


/root/rpmbuild/SOURCES/ 


    ldap.conf


    pam_ldap-185.tar.gz


/root/rpmbuild/SPECS/


    pam_ldap.spec

## 1.只生成二进制格式的rpm包


cd /root/rpmbuild/SPECS/


rpmbuild -bb pam_ldap.spec


获得：


/root/rpmbuild/RPMS/x86_64/pam_ldap-185-1.x86_64.rpm

## 2.只生成src格式的rpm包


cd /root/rpmbuild/SPECS/


rpmbuild -bs pam_ldap.spec


获得：


/root/rpmbuild/SRPMS/pam_ldap-185-1.src.rpm

## 3.只需要生成完整的源文件


cd /root/rpmbuild/SPECS/


rpmbuild -bp pam_ldap.spec


获得：


/root/rpmbuild/BUILD/pam_ldap-185/

## 4.完全打包


cd /root/rpmbuild/SPECS/


rpmbuild -ba pam_ldap.spec


获得：


以上三个同时获得



