
# CentOS升级openssl - 致林 - 博客园






# [CentOS升级openssl](https://www.cnblogs.com/bincoding/p/6118342.html)
才设置了http2，结果蓝狗说我网站不安全，检测一下发现openssl有漏洞，于是准备升级一下openssl
检测网站：[www.ssllabs.com/ssltest/analyze.html](http://www.ssllabs.com/ssltest/analyze.html)
![](https://images2015.cnblogs.com/blog/771778/201611/771778-20161130162825177-1451834732.png)

\# Based on http://fearby.com/article/update-openssl-on-a-digital-ocean-vm/$ apt-get update
$ apt-get dist-upgrade
$wgetftp://ftp.openssl.org/source/old/1.0.2/openssl-1.0.2h.tar.gz$tar-xvzf openssl-1.0.2h.tar.gz
$ cd openssl-1.0.2h//如果没有安装gcc，这里会报错$yum-yinstallgcc$ ./config --prefix=/usr/$makedepend
$sudomakeinstall$ openssl version
\# OpenSSL1.0.2h3May2016\# now restart your nginx or other server
$ nginx-s reload





