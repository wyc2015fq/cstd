
# LDAP基础：9：客户端工具：使用Apache Directory Studio进行操作 - 知行合一 止于至善 - CSDN博客

2018年11月17日 05:41:57[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2824


![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116095902123.jpg)
前面的文章介绍了LDAP服务器的搭建以及通过ldap相关的命令进行操作的方法，对于不习惯*nix或命令行操作的用户，也无需纠结，有很多不错的客户端工具可以实现这个功能，这篇文章介绍一下Apache Directory Studio。
[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)
# Apache Directory Studio
[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)Apache Directory Studio 被设计用来和各种LDAP服务器进行交互操作，提供了一个使用方便的客户端操作平台。
[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)官方网址：[http://directory.apache.org/studio/](http://directory.apache.org/studio/)
支持OS：跨平台，支持MacOS/Windows/Linux
下载地址：[http://directory.apache.org/studio/downloads.html](http://directory.apache.org/studio/downloads.html)
[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)
# 事前准备
[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)
## 启动openldap服务器
[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)`liumiaocn:~ liumiao$ docker run --name openldap -p 389:389 --detach osixia/openldap:1.2.2
2bfde68da35074916e916dc987ef428c655ee3df3984a9898b80de578a377427
liumiaocn:~ liumiao$`[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)
## LDAP服务器IP确认
[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)`liumiaocn:~ liumiao$ ifconfig |grep 192
	inet 192.168.31.242 netmask 0xffffff00 broadcast 192.168.31.255
liumiaocn:~ liumiao$`[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)
## Apache Directory Studio下载并安装
[
](https://img-blog.csdnimg.cn/20181116095902123.jpg)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116095658475.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 连接LDAP服务
设定LDAP服务器的IP地址以及服务端口号
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116100048541.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
通过简单认证方式设定admin的DN和缺省的admin的密码（admin），并可以通过Check Authentication按钮进行连接验证。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116100247139.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
设定BaseDN
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111610032647.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
可以看到基础当前LDAP服务器在此basedn下的信息，用户只有一个cn=admin
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116100456646.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 常见操作
## 图形界面操作
Apache Directory Studio提供了图形界面的操作方式，用户可以很方便地逐步推进。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116101050249.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## ldif导入
这里我们使用ldif导入的方式，先准备如下的ldif信息
`liumiaocn:Desktop liumiao$ cat demo.ldif 
dn: ou=People,dc=example,dc=org
objectclass: top
objectclass: organizationalUnit
ou: People
dn: ou=Servers,dc=example,dc=org
objectclass: top
objectclass: organizationalUnit
ou: Servers
# Barbara Jensen, example.org
dn: cn=Barbara Jensen,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
title: the world's most famous mythical manager
mail: bjensen@example.com
uid: bjensenliumiaocn:Desktop liumiao$`通过Import界面上传事先准备ldif文件，也是前面文章中利用到的内容，可以看到还有很多选项，比如日志信息，以及对entry在存在的情况下是否要覆盖的选项，以及出错是否继续，这些在前面的文章中也可以在相关的命令行的Option中找到类似的设定方式。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116101628473.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
选择Finish之后，执行的结果可以清晰地看出
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116101904952.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
这个里面可以清晰地看到Barbara和People是创建在同一层的，相较于ldapsearch的结果的确认，图形化的方式对于树形结构的显示有更大的优势。
[
](https://img-blog.csdnimg.cn/20181116101904952.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)调整Barbara到People下
准备如下的ldif文件，此ldif也是前面文章中使用的：
liumiaocn:Desktop liumiao$ cat modifybarbara.ldif
dn: cn=Barbara Jensen,dc=example,dc=org
changetype: modrdn
newrdn: uid=bjensen
deleteoldrdn: 0
newsuperior: ou=people,dc=example,dc=org
liumiaocn:Desktop liumiao$导入之后，people上进行一下刷新，就可以看到Barbara已经转到People下了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116102658333.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
总结
使用客户端工具Apache Directory Studio，除了Apache DS之外，诸如OpenLdap也可以很好地进行交互，对于不习惯不喜欢命令行方式的用户，Apache Directory Studio也是选择之一。

[
  ](https://img-blog.csdnimg.cn/20181116101904952.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)