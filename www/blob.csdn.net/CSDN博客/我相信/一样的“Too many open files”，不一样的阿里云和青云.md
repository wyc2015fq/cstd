# 一样的“Too many open files”，不一样的阿里云和青云 - 我相信...... - CSDN博客





2016年05月14日 22:04:45[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：5220
所属专栏：[云服务之旅](https://blog.csdn.net/column/details/13000.html)









## 现象

系统服务准备在青云上线，突然间发现服务时断时续，登陆服务器，查看日志，原来是‘Too many open files‘， ulimit  和lsof 一看果真如此。

## 解决未果

原来在centos 或者 ubantu 集群上早已经遇到过类似的问题，原因很简单，最大文件数限制。

后来使用阿里云，同样遇到了同样的问题，解决方案很简单，通过修改limits.conf（ubantu)文件即可。打开/etc/security/limits.conf，找到如下设置(如果没有就插入)：

```
*   soft    nofile  65535
*   hard    nofile  65535
```

现在在青云上，依然如此操作，系统重启后，发现居然无效，ulimit 和 lsof 查看问题依旧，不明所以。后来得青云四爷的帮助，告知青云服务是面向用户Session的。

## 面向用户session 的PAM

用户鉴别是Linux的门户，每个用户进入到系统中都必须经过鉴别。  可插入认证模块（简称PAM，Pluggable Authentication Modules）是基于模块化设计、具有可插入功能的一种独立于应用程序之外的验证方式。使用PAM后，应用程序可以不需要集成验证功能，而由PAM来完成。PAM 使用灵活，有着各种不同的验证模块。  

pam_access验证模块一般与account验证类型一同使用。它主要用于对访问进入管理，提供基于登录名、主机名或域名、公网IP地址或网络号，以及非网络登录时的tty名称的访问控制。pam_access验证模块主要是根据/etc/security/access.conf配置文件中的内容，来进行相应的验证工作的。

pam_cracklib验证模块通常只与password验证类型一起使用。这个验证模块可以通过插入password堆栈，为特殊的应用提供可插入式密码强度性检测。

pam_limits验证模块通常与session验证类别一同使用。它主要用来限制用户在会话过程中对系统资源的使用，即使UID=0的用户也受它的限制。此模块使用一个独立的配置文件来设置对系统资源的限制情况，默认的配置文件是/etc/security/limits.conf。

到这里，终于明白根源了。

## 解决

明白了所以然，解决起来就明了了。编辑/etc/pam.d/common-session，加入一行： 
`session required    pam_limits.so  `
编辑/etc/profile，加入  
`ulimit -SHn 65536`
然后，别忘了 `source  /etc/profile` . 保险起见，做好重启服务器，青云上的系统就可以彻底的解决“Too many open files”问题了。

由此可见，青云在细节上与阿里云有着诸多的不同，当然，你又可以自己在阿里云上搞定这些，提高一点安全性，只是麻烦些而已。



