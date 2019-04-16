# 用apache做为代理下载本地pdf文件 - ggabcda的博客 - CSDN博客





2017年09月10日 13:45:57[ggabcda](https://me.csdn.net/ggabcda)阅读数：279








   有一些公司会用apache做为代理，下载服务器上的pdf文件。以下是apache做为代理的配置



一. 环境

centos6.5  192.168.69.3



二. yum安装apache 服务

[zxjr@acrr ~]$ rpm -qa httpd    //查看是否有rpm包安装的apache服务

httpd-2.2.15-60.el6.centos.5.x86_64

[zxjr@acrr ~]$ rpm -e httpd-2.2.15-60.el6.centos.5.x86_64    //卸载rpm包安装的

[zxjr@acrr ~]$ yum -y install httpd httpd-devel    //yum 安装apache服务

[zxjr@acrr ~]$ vim /etc/httpd/conf/httpd.conf    //修改apache配置文件，修改前最好备份

 136 Listen 8888

 277 ServerName 192.168.69.3

1011 <VirtualHost *:8888>     //开启虚拟主机

1012         ServerName 192.168.69.3

1013         DocumentRoot "/contract/zx-contract/pdfcontract"     //此处的目录就是我服务器上存放的pdf文件

1014         <Directory "/contract/zx-contract/pdfcontract">

1015         Options None

1016         AllowOverride None

1017         Order Deny,Allow

1018         Deny from all

1019         Allow from all

1020         </Directory>

1021         ErrorLog "logs/zx-contract-error_log"

1022         CustomLog "logs/zx-contract-access_log" common

1023 </VirtualHost>

[zxjr@acrr ~]$ httpd -t    //检查语法

Syntax OK

[zxjr@acrr ~]$ /etc/init.d/httpd start

[zxjr@acrr ~]$ netstat -lnpt | grep 8888

(No info could be read for "-p": geteuid()=500 but you should be root.)

tcp        0      0 :::8888                     :::*                        LISTEN      -



三. 服务器上存放pdf文件

[zxjr@acrr ~]$ cd /contract/zx-contract/pdfcontract/

[zxjr@acrr pdfcontract]$ ls    // 这三个目录都是存放pdf 文件的，三个目录和pdf文件都是由java程序自动生成的。

JKDB  JKDB_per  JKDB_sign

[zxjr@acrr pdfcontract]$ cd JKDB_sign/    //任意进入一个目录

[zxjr@acrr JKDB_sign]$ ls

20170908

[zxjr@acrr JKDB_sign]$ cd 20170908/

[zxjr@acrr 20170908]$ ls

ZXWD040_JKDB_3_ZXWD0403.pdf

[zxjr@acrr 20170908]$ pwd

/contract/zx-contract/pdfcontract/JKDB_sign/20170908



四. 浏览器下载pdf文件

http://192.168.69.3:8888/JKDB_sign/20170908/ZXWD040_JKDB_3_ZXWD0403.pdf

![](http://images2017.cnblogs.com/blog/1095193/201709/1095193-20170908220752413-1413883205.jpg)

注。apache配置文件里已经指定了根目录为："/contract/zx-contract/pdfcontract   所以浏览器中不用再写次根目录。访问

其他目录下的pdf文件也同一样，浏览器中更改相对应的目录就可以了。





