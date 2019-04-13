
# LDAP基础：6：使用ldapsearch进行数据查询 - 知行合一 止于至善 - CSDN博客

2018年11月16日 05:05:07[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：3916


在前面的文章中，我们已经多次使用ldapsearch进行信息的查询，但都是一条命令查询出所有内容，这篇文章对于ldapsearch的常用方式进行进一步的解释和说明。
# 操作方式
## 执行场所
对LDAP进行操作，场所不限，LDAP服务器上进行操作，也可以在可以连接LDAP服务的客户端进行
## 使用命令
常用的数据的查询，可以使用ldapsearch
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
|-f|指定输入条件，在RFC 4515中有更详细的说明|
|-c|出错后忽略当前错误继续执行，缺省情况下遇到错误即终止|
|-n|模拟操作但并不实际执行，用于验证，常与-v一同使用进行问题定位|
|-v|显示详细信息|
|-d|显示debug信息，可设定级别|
|-s|指定搜索范围, 可选值：|base|one|sub|children|
# 常见的查询
## 方式1: 指定ldapuri进行查询
> 使用示例：

> ldapsearch -x -H ldap://192.168.31.242:389  -b dc=example,dc=org -D “cn=admin,dc=example,dc=org” -w admin

> 方式2: 指定hostname和端口号进行查询

> 使用示例：

> ldapsearch -x -h 192.168.31.242 -p 389 -b dc=example,dc=org -D “cn=admin,dc=example,dc=org” -w admin

> 方式3: 指定过滤条件：按照dn进行过滤

> 可以通过dn进行过滤，因为dn能较为清晰的确定一条entry的信息，比如确认admin的详细，可以使用

> 使用示例：

> ldapsearch -x -H ldap://192.168.31.242:389  -b cn=admin,dc=example,dc=org -D “cn=admin,dc=example,dc=org” -w admin

> 执行日志

> liumiaocn:openldap liumiao$ ldapsearch -x -H ldap://192.168.31.242:389  -b cn=admin,dc=example,dc=org -D "cn=admin,dc=example,dc=org" -w admin 
\# extended LDIF
\#
\# LDAPv3
\# base <cn=admin,dc=example,dc=org> with scope subtree
\# filter: (objectclass=*)
\# requesting: ALL
\#
\# admin, example.org
dn: cn=admin,dc=example,dc=org
objectClass: simpleSecurityObject
objectClass: organizationalRole
cn: admin
description: LDAP administrator
userPassword:: e1NTSEF9TE5GaU0rZVZXNGlRR3FDdzQ5UzRMNjRlb2xjMjY5OU4=
\# search result
search: 2
result: 0 Success
\# numResponses: 2
\# numEntries: 1
liumiaocn:openldap liumiao$
> 同样查询上文中生成的People的这样一个部门的话，可以使用这样的语句：

> liumiaocn:openldap liumiao$ ldapsearch -x -H ldap://192.168.31.242:389  -b ou=people,dc=example,dc=org -D "cn=admin,dc=example,dc=org" -w admin 
\# extended LDIF
\#
\# LDAPv3
\# base <ou=people,dc=example,dc=org> with scope subtree
\# filter: (objectclass=*)
\# requesting: ALL
\#
\# People, example.org
dn: ou=People,dc=example,dc=org
objectClass: top
objectClass: organizationalUnit
ou: People
\# search result
search: 2
result: 0 Success
\# numResponses: 2
\# numEntries: 1
liumiaocn:openldap liumiao$
> 方式4: 使用模糊匹配

> 比如我们创建了People和Servers这两个ou，我们可以使用ou=*来一下查询出当前根下的ou的信息，比如可以使用如下的命令：

> liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin "ou=*"
\# extended LDIF
\#
\# LDAPv3
\# base <dc=example,dc=org> with scope subtree
\# filter: ou=*
\# requesting: ALL
\#
\# People, example.org
dn: ou=People,dc=example,dc=org
objectClass: top
objectClass: organizationalUnit
ou: People
\# Servers, example.org
dn: ou=Servers,dc=example,dc=org
objectClass: top
objectClass: organizationalUnit
ou: Servers
\# search result
search: 2
result: 0 Success
\# numResponses: 3
\# numEntries: 2
liumiaocn:openldap liumiao$
> 常用的方式还可以使用cn=*或者cn=admin这样进行指定信息进行过滤

> liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin "cn=*"
\# extended LDIF
\#
\# LDAPv3
\# base <dc=example,dc=org> with scope subtree
\# filter: cn=*
\# requesting: ALL
\#
\# admin, example.org
dn: cn=admin,dc=example,dc=org
objectClass: simpleSecurityObject
objectClass: organizationalRole
cn: admin
description: LDAP administrator
userPassword:: e1NTSEF9TE5GaU0rZVZXNGlRR3FDdzQ5UzRMNjRlb2xjMjY5OU4=
\# Barbara Jensen, example.org
dn: cn=Barbara Jensen,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
title: the world's most famous mythical manager
mail: bjensen@example.com
uid: bjensen
\# search result
search: 2
result: 0 Success
\# numResponses: 3
\# numEntries: 2
liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin "cn=admin"
\# extended LDIF
\#
\# LDAPv3
\# base <dc=example,dc=org> with scope subtree
\# filter: cn=admin
\# requesting: ALL
\#
\# admin, example.org
dn: cn=admin,dc=example,dc=org
objectClass: simpleSecurityObject
objectClass: organizationalRole
cn: admin
description: LDAP administrator
userPassword:: e1NTSEF9TE5GaU0rZVZXNGlRR3FDdzQ5UzRMNjRlb2xjMjY5OU4=
\# search result
search: 2
result: 0 Success
\# numResponses: 2
\# numEntries: 1
liumiaocn:openldap liumiao$
> 当然这里只是给出了几个非常简单的使用例子进行过滤，可以结合>和<以及～匹配查找等方式实现更加快速和便捷地定位。

> 方式5: 指定返回信息

> 另外比较常用的一种方式类似与在SQL中写Select，我们可以指定返回的字段信息以及排序，在实际的集成中也是非常的有用，比如，我们只返回创建的uid的mail/uid/title/cn/sn，并按照我们期待的方式进行返回，具体使用示例如下：

> liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin "uid=*" sn cn title mail uid
\# extended LDIF
\#
\# LDAPv3
\# base <dc=example,dc=org> with scope subtree
\# filter: uid=*
\# requesting: sn cn title mail uid 
\#
\# Barbara Jensen, example.org
dn: cn=Barbara Jensen,dc=example,dc=org
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
title: the world's most famous mythical manager
mail: bjensen@example.com
uid: bjensen
\# search result
search: 2
result: 0 Success
\# numResponses: 2
\# numEntries: 1
liumiaocn:openldap liumiao$
> 总结

> 这篇文章整理了一些ldapsearch常用的查询方式，当然实际使用的时候远远不止这些，但是掌握了这些，对于入门级的选手来说，简单的确认和定位应该会非常有用。


