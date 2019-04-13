
# LDAP基础：5：使用ldapadd进行数据增加 - 知行合一 止于至善 - CSDN博客

2018年11月15日 22:50:52[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：4763


在前面的文章中学习到了如何搭建openldap服务，以及如何从客户端进行访问，同时最基础的ldap的概念和树形结构的常见构成方式也多少有些涉及，在这个基础之上，就可以开始使用LDAP了。这篇文章来介绍一下如何创建相关的信息。
# 创建内容
这里我们将会聚焦于下图灰色部分的创建，这也是实际项目中使用的最多的一种，在这系列的文章中将会使用openldap的osixia的镜像所内置的缺省的dc:[example.org](http://example.org)(这里的信息是openldap官方例子的信息，[在例子中我们实际使用的是example.org而不是example.com](http://xn--example-f43ko9c91bydt03hilmj5as12e67oz63d8df2m0p.xn--orgexample-vj2py03ugjwd.com))。通过设定环境变量即可简单改变此处，因为此部分更多与LDAP基本服务设定相关，所以使用客户端的命令或者编程接口对其进行操作的部分主要聚焦于从Organisation开始的部分。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115092430695.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 创建目标
在本文的例子中，将会在dc=example, dc=org下创建：
|创建内容|类型|名称|定位方式|
|---|---|---|---|
|部门|Organisation Unit|People|通过ou=People进行|
|部门|Organisation Unit|Servers|通过ou=Servers进行|
|部门|Person|People|通过uid=babs进行|
# 操作方式
## 执行场所
对LDAP进行操作，场所不限，LDAP服务器上进行操作，也可以在可以连接LDAP服务的客户端进行
## 使用命令
常用的数据添加，可以使用ldapmodify命令也可以使用ldapadd
## 数据填充
可以使用ldif传入数据的方式，也可以通过参数方式传入
# 命令介绍
使用ldapadd或者ldapmodify进行数据添加，很多情况下的客户端实现中，这两个是一个共通的实现，简单到可能只是一个软链接
`liumiaocn:openldap liumiao$ which ldapadd
/usr/bin/ldapadd
liumiaocn:openldap liumiao$ ls -l /usr/bin/ldapadd
lrwxr-xr-x  1 root  wheel  10 Oct 25 11:50 /usr/bin/ldapadd -> ldapmodify
liumiaocn:openldap liumiao$`
# Option介绍
命令的Option就不再分命令来介绍，常用的Option的信息和用途整理如下：
|Option|说明|
|---|---|
|-H|ldapuri，格式为ldap://机器名或者IP:端口号，不能与-h和-p同时使用|
|-h|LDAP服务器IP或者可解析的hostname，与-p可结合使用，不能与-H同时使用|
|-p|LDAP服务器端口号，与-h可结合使用，不能与-H同时使用|
|-x|使用简单认证方式|
|-D|所绑定的服务器的DN|
|-w|绑定DN的密码，与-W二者选一|
|-W|不输入密码，会交互式的提示用户输入密码，与-w二者选一|
|-f|指定ldif文件作为输入|
|-a|添加新的entry，ldapadd缺省使用，ldapmodify 可指定以达到同样作用|
|-c|出错后忽略当前错误继续执行，缺省情况下遇到错误即终止|
|-n|模拟操作但并不实际执行，用于验证，常与-v一同使用进行问题定位|
|-v|显示详细信息|
|-d|显示debug信息，可设定级别|
|-e|设置客户端证书|
|-E|设置客户端私钥|
# 操作实例
## 用户Barbara的LDIF信息
使用最基础的方式添加一个用户Barbara，这也是man ldapmodify给出的例子，稍微做一下修改以保证能够正常添加进去。
`liumiaocn:openldap liumiao$ cat barbara.ldif 
dn: cn=Barbara Jensen,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
title: the world's most famous mythical manager
mail: bjensen@example.com
uid: bjensen
liumiaocn:openldap liumiao$`
## 使用ldapadd添加Barbara
`liumiaocn:openldap liumiao$ ldapadd -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f barbara.ldif
adding new entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`使用ldapsearch可以确认到添加的这个实际上最mythical的Barbara的详细信息：
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242  -b dc=example,dc=org -D "cn=admin,dc=example,dc=org" -w admin
...省略
# Barbara Jensen, example.org
dn: cn=Barbara Jensen,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
title: the world's most famous mythical manager
mail: bjensen@example.com
uid: bjensen
# search result
search: 2
result: 0 Success
# numResponses: 4
# numEntries: 3
liumiaocn:openldap liumiao$`
## 删除该用户
删除此用户，然后后面来验证使用ldapmodify是否可以同样进行操作
`liumiaocn:openldap liumiao$ ldapdelete -x -h 192.168.31.242  -D "cn=admin,dc=example,dc=org" -w admin  "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`
## 使用ldapmodify添加Barbara
在这里，由于ldapadd就是一个指向ldapmodify的软链接，再详细进行确认一下，可以得出这两个东西应该是完全一致的操作，所以一个很简单的问题：
> ldapadd -H ldap://192.168.31.242:389 -D “cn=admin,dc=example,dc=org” -w admin -f barbara.ldif
Question：中的ldapadd替换称ldapmodify是否能够完全一致的动作？
执行一下来确认结果：
`liumiaocn:openldap liumiao$ ldapmodify -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f barbara.ldif
ldapmodify: modify operation type is missing at line 2, entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`答案是：不可以。
### 对应方法1: 使用-a的Option
仔细会看上文整理的Option，对-a的说明是这样的“添加新的entry，ldapadd缺省使用，ldapmodify 可指定以达到同样作用“，所以ldapmodify的话，添加-a就可以达到同样效果了。
`liumiaocn:openldap liumiao$ ldapmodify -a -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f barbara.ldif
adding new entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`
### 对应方法2: 修改LDIF的写法
为了验证，重新把刚刚生成的Barbara再次删掉
`liumiaocn:openldap liumiao$ ldapdelete -x -h 192.168.31.242  -D "cn=admin,dc=example,dc=org" -w admin  "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`修改LDIF的内容，添加一行信息内容为：changetype: add，修改后的全部信息如下：
`liumiaocn:openldap liumiao$ cat barbara.ldif 
dn: cn=Barbara Jensen,dc=example,dc=org
changetype: add
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
title: the world's most famous mythical manager
mail: bjensen@example.com
uid: bjensen
liumiaocn:openldap liumiao$`这样就可以使用同样的命令行进行添加了
`liumiaocn:openldap liumiao$ ldapmodify -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f barbara.ldif
adding new entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`
# 添加Organisation Unit
在添加用户的时候关键的objectclass是inetOrgPerson，而对于添加部门，关键信息则为organisationalUnit，添加两个OrganisationUnit的LDIF信息如下：
`liumiaocn:openldap liumiao$ cat orgunits.ldif 
dn: ou=People,dc=example,dc=org
objectclass: top
objectclass: organizationalUnit
ou: People
dn: ou=Servers,dc=example,dc=org
objectclass: top
objectclass: organizationalUnit
ou: Servers
liumiaocn:openldap liumiao$`通过这个例子可以对LDIF进一步理解到可以是多段信息的结合，而在实际的项目中，对LDAP数据在不是很复杂的情况下，使用LIDF进行备份和恢复也是常用的方式。
使用ldapadd进行信息的添加
`liumiaocn:openldap liumiao$ ldapadd -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f orgunits.ldif 
adding new entry "ou=People,dc=example,dc=org"
adding new entry "ou=Servers,dc=example,dc=org"
liumiaocn:openldap liumiao$`通过ldapsearch进行结果的查询
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242  -b dc=example,dc=org -D "cn=admin,dc=example,dc=org" -w admin
...省略
# People, example.org
dn: ou=People,dc=example,dc=org
objectClass: top
objectClass: organizationalUnit
ou: People
# Servers, example.org
dn: ou=Servers,dc=example,dc=org
objectClass: top
objectClass: organizationalUnit
ou: Servers
# search result
search: 2
result: 0 Success
# numResponses: 6
# numEntries: 5
liumiaocn:openldap liumiao$`
# 总结
这篇文章使用具体的例子，使用ldapadd和ldapmodify结合ldif文件实现了对部门信息和人员的添加，但注意到部门和人员的信息还未进行关联，当然最简单的方式是先创建部门，然后在人员信息中添加相关内容即可，这里主要是为后面的进一步介绍ldapmodify的准备。

