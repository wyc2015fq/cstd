# USVN - DoubleLi - 博客园






我们最近将快盘上的东西迁移到了svn上，因为快盘总是不会不小心删掉或者修改了某些文件。为了能保留历史记录我们统一迁移到svn上。为了方便权限管理，我对比了几个svn的权限管理工具，最后觉得还是usvn功能最强大。可以将权限按组分配，且每个组内可以有组长去管理组内资源。



usvn  依赖很多库



yum install -y mysql-server

yum install -y php

yum install -y mod_dav_svn.x86_64



**1、下载usvn，解压后放到web根目录下**

**2、配置Apache目录允许重写**

vim /etc/httpd/conf/httpd.conf 

文件最后加入


<Directory "/var/www/html/">

        AllowOverride all

        Order allow,deny

        Allow from all

</Directory>



重启Apache service  httpd restart

**3、http://ip/usvn/public/install.php 开始安装**

安装到最后一步会提示你将类似如下内容添加到/etc/httpd/conf/httpd.conf 中，并重启

<Location /usvn/public/svn/>

        ErrorDocument 404 default

        DAV svn

        Require valid-user

        SVNParentPath /var/www/html/usvn/files/svn

        SVNListParentPath off

        AuthType Basic

        AuthName "USVN"

        AuthUserFile /var/www/html/usvn/files/htpasswd

        AuthzSVNAccessFile /var/www/html/usvn/files/authz

</Location>


**4、安装到最后遇到一个头痛的问题，当目录出现中文时无法设置目录权限**

解决方法：

4.1、设置系统编码：vim /etc/sysconfig/i18n  改成如下


LANG="zh_CN.UTF-8"

SYSFONT="latarcyrheb-sun16"


4.2、修改USVN语言环境：vim /var/www/html/usvn/app/bootstrap.php

在文件最前面加上

setlocale(LC_ALL, 'zh_CN.UTF-8');



**5、提交log不能为空**

vim ${svn_dir}/hooks/pre-commit






#!/bin/sh

REPOS="$1"

TXN="$2"



# Make sure that the log message contains some text.

SVNLOOK=/usr/bin/svnlook

$SVNLOOK log -t "$TXN" "$REPOS" | \

   grep "[a-zA-Z0-9]" > /dev/null

#|| exit 1



#commit-access-control.pl "$REPOS" "$TXN" commit-access-control.cfg || exit 1

LOGMSG=`$SVNLOOK log -t "$TXN" "$REPOS" | grep "[a-zA-Z0-9]" | wc -c`

if [ "$LOGMSG" -lt 10 ];

then

    echo -e "svn log cann't less than 10 character" 1>&2

    exit 1

fi

# All checks passed, so allow the commit.

exit 0


chown apache:apache  pre-commit

chmod +x pre-commit









