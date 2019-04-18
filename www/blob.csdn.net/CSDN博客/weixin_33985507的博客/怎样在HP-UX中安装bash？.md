# 怎样在HP-UX中安装bash？ - weixin_33985507的博客 - CSDN博客
2017年06月18日 21:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：19

﻿﻿
# 怎样在HP-UX中安装bash？
注：HPUX全称是Hewlett Packard UniX是惠普9000系列server的Unix操作系统，能够在HP的PA-RISC处理器、Intel的Itanium处理器的电脑上执行。
## 1.bash安装包链接：
[http://hpux.connect.org.uk/hppd/cgi-bin/search?package=on&description=on&term=bash](http://hpux.connect.org.uk/hppd/cgi-bin/search?package=on&description=on&term=bash)
依赖组件：gettext, libiconv, termcap
gettext-0.19.5.1-hppa-11.31.depot.gz
libiconv-1.14-hppa-11.31.depot.gz
termcap-1.3.1-hppa-11.31.depot.gz
bash-4.3.033-hppa-11.31.depot.gz
## 2.解压全部的到/tmp下
cd /tmp
gunzip gettext-0.19.5.1-hppa-11.31.depot.gz
gunzip libiconv-1.14-hppa-11.31.depot.gz
gunzip termcap-1.3.1-hppa-11.31.depot.gz
gunzip bash-4.3.033-hppa-11.31.depot.gz
## 3.安装全部的安装包
swinstall -s /tmp/gettext-0.19.5.1-hppa-11.31.depot
swinstall -s /tmp/libiconv-1.14-hppa-11.31.depot
swinstall -s /tmp/termcap-1.3.1-hppa-11.31.depot
swinstall -s /tmp/bash-4.3.033-hppa-11.31.depot
附带swinstall安装界面步骤：[http://www.thegeekstuff.com/2010/06/install-remove-depot-packages/](http://www.thegeekstuff.com/2010/06/install-remove-depot-packages/)
## 4.安装完启动bash
/usr/local/bin/bash
## 5.把/usr/local/bin加到/etc/PATH中并保存
# vi /etc/PATH
/usr/local/bin
# export PATH=$(cat /etc/PATH)
參考文档：[http://itsiti.com/install-bash-shell-in-hp-ux](http://itsiti.com/install-bash-shell-in-hp-ux)
## 假设出现以下错误：
**You do not****         have permission for this operation.  The depot owner, system         administrator, or alternate root owner may need to use the         "swreg" or "swacl" command to give you permission.  Or, to         manage applications designed and packaged for nonprivileged         mode, see the "run_as_superuser" option in the "sd" man page.**
**那么解决方式是：**
/sbin/init.d/swagentd stop
/sbin/init.d/swagentd start
swreg -l depot /tmp/gettext-0.19.5.1-hppa-11.31.depot
swreg -l depot /tmp/libiconv-1.14-hppa-11.31.depot
swreg -l depot /tmp/termcap-1.3.1-hppa-11.31.depot
swreg -l depot /tmp/bash-4.3.033-hppa-11.31.depot
然后进入安装界面：[http://www.thegeekstuff.com/2010/06/install-remove-depot-packages/](http://www.thegeekstuff.com/2010/06/install-remove-depot-packages/)
﻿﻿
