
# LDAP基础：4：基础知识以及LDIF格式说明 - 知行合一 止于至善 - CSDN博客

2018年11月15日 06:58:23[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：3011


前面的文章介绍了如何用docker的方式快速搭建OpenLdap服务，这篇文章整理一些最为基础的知识以方便后续对于ldap的操作有一个简单地认识。
# 什么是LDAP
LDAP是 Lightweight Directory Access Protocol的缩写，直译过来是“轻量目录访问协议”，之所以称为“轻量”，就是有“重量级”的目录访问协议，那就是X.500协议，LDAP基于X.500并进行简化，也是IETF下的一项标准，目前最新的RFC为RFC4510，信息如下：
[http://www.rfc-editor.org/rfc/rfc4510.txt](http://www.rfc-editor.org/rfc/rfc4510.txt)
# 基本概念
|E文|说明|
|---|---|
|Directory|目录，用于存放信息的单元|
|Entry|条目，LDAP的基本信息单元|
|DN：Distinguished Name|条目中用于唯一区别改条目的信息|
|LDIF：LDAP Interchange Format|在RFC2849中定义的标准，用于规范LDAP的配置和目录内容等详细信息的保存，后续的例子中将会较多地使用LDIF进行增删改查的操作。|
注：
每个Entry都可以有多个属性，而每个属性都有特定的类型和一个或多个取值，比如cn（Common Name）就是一个属性
因为LDAP概念很多，这里会根据例子展开相关的最小程度的概念，更为详细的可以查看上文的RFC的文档。
# LDAP树形结构的构成方式
LDAP存储的信息以树形结构的方式进行组织，而设计LDAP结构一般有两种方式，比如为了说明一个人所处的详细信息，采用不同的方式的构成方式也会有所不同，而这些最终会体现到LDIF的详细信息上。
## 传统方式
传统方式聚焦于国别以及地理信息为上层构成，比如Barbara Jenson这个人，使用这种方式来确认，首先会看国别，美国还是英国，然后按照地理信息进行继续下行，是加州的Acme公司的销售部门的一个成员。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115064947652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 互联网域名方式
互联网域名则是基于域名，上层构成直接使用域名，能结合DNS相关的技术。在这种方式下，看一下babs这个人，是exmaple.com公司下的一个成员。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115065420434.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 总结
无论哪种方式都可以搭建你所需要的组织结构，后面将会继续使用简单的例子和直接可以看到结果的命令行来进一步展开如何在已经搭建好了的LDAP服务器中进行增删改查的操作。
# 参考内容
[http://www.openldap.org/doc/admin24/](http://www.openldap.org/doc/admin24/)

