
# LDAP基础：7：使用ldapmodify和ldapdelete进行修改或删除 - 知行合一 止于至善 - CSDN博客

2018年11月16日 05:57:34[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2636


这篇文章主要介绍一下如何使用ldapmodify和ldapdelete进行信息的修改或者删除，并在前面添加的数据的基础上进行一些简单的例子的操作。
# 操作方式
## 执行场所
对LDAP进行操作，场所不限，LDAP服务器上进行操作，也可以在可以连接LDAP服务的客户端进行
## 使用命令
常用的数据的删除，可以使用ldapmodify命令也可以使用ldapdelete
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
|-c|出错后忽略当前错误继续执行，缺省情况下遇到错误即终止|
|-n|模拟操作但并不实际执行，用于验证，常与-v一同使用进行问题定位|
|-v|显示详细信息|
|-d|显示debug信息，可设定级别|
|-e|设置客户端证书|
|-E|设置客户端私钥|
注意：
常见的选项中-f，在ldapdelete和ldapmodify中有明显的不同, ldapdelete中主要是用于记录要删除的dn的信息，而ldapmodify的-f则指定的是要修改的ldif的文件内容。
# 修改信息：ldapmodify
## 事前确认
确认一下Barbara的现在的信息
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin uid=bjensen
# extended LDIF
#
# LDAPv3
# base <dc=example,dc=org> with scope subtree
# filter: uid=bjensen
# requesting: ALL
#
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
# numResponses: 2
# numEntries: 1
liumiaocn:openldap liumiao$`
## 修改Barbara的title
Barbara的title写的是“the world’s most famous mythical manager”，这有点不太谦虚了，修改称“one of the world’s most famous mythical manager”，因为毕竟官方文档的例子中还有一个叫babs的朋友。
准备用于修改的ldif文件
`liumiaocn:openldap liumiao$ cat modifybarbara.ldif 
dn: cn=Barbara Jensen,dc=example,dc=org
changetype: modify
replace: title
title: one of the world's most famous mythical manager
liumiaocn:openldap liumiao$`使用准备的ldif文件使用-f传给ldapmodify进行信息修改
`liumiaocn:openldap liumiao$ ldapmodify -a -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f modifybarbara.ldif 
modifying entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`结果确认
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin uid=bjensen
# extended LDIF
#
# LDAPv3
# base <dc=example,dc=org> with scope subtree
# filter: uid=bjensen
# requesting: ALL
#
# Barbara Jensen, example.org
dn: cn=Barbara Jensen,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
mail: bjensen@example.com
uid: bjensen
title: one of the world's most famous mythical manager
# search result
search: 2
result: 0 Success
# numResponses: 2
# numEntries: 1
liumiaocn:openldap liumiao$`可以看到已经完成了信息的修改
## 给Barbara添加一个description的字段
事前准备ldif文件
`liumiaocn:openldap liumiao$ cat modifybarbara.ldif 
dn: cn=Barbara Jensen,dc=example,dc=org
changetype: modify
add: description
description: Barbara description
liumiaocn:openldap liumiao$`将ldif文件通过-f传给ldapmodify
`liumiaocn:openldap liumiao$ ldapmodify -a -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f modifybarbara.ldif 
modifying entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`结果确认
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin uid=bjensen
# extended LDIF
#
# LDAPv3
# base <dc=example,dc=org> with scope subtree
# filter: uid=bjensen
# requesting: ALL
#
# Barbara Jensen, example.org
dn: cn=Barbara Jensen,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
mail: bjensen@example.com
uid: bjensen
title: one of the world's most famous mythical manager
description: Barbara description
# search result
search: 2
result: 0 Success
# numResponses: 2
# numEntries: 1
liumiaocn:openldap liumiao$`
## 将Barbara移动到ou=People中
事前准备ldif文件
`liumiaocn:openldap liumiao$ cat modifybarbara.ldif 
dn: cn=Barbara Jensen,dc=example,dc=org
changetype: modrdn
newrdn: uid=bjensen
deleteoldrdn: 0
newsuperior: ou=people,dc=example,dc=org
liumiaocn:openldap liumiao$`将ldif文件通过-f传给ldapmodify
`liumiaocn:openldap liumiao$ ldapmodify -a -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f modifybarbara.ldif 
modifying rdn of entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`结果确认
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin uid=bjensen
# extended LDIF
#
# LDAPv3
# base <dc=example,dc=org> with scope subtree
# filter: uid=bjensen
# requesting: ALL
#
# bjensen, People, example.org
dn: uid=bjensen,ou=People,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
mail: bjensen@example.com
uid: bjensen
title: one of the world's most famous mythical manager
description: Barbara description
# search result
search: 2
result: 0 Success
# numResponses: 2
# numEntries: 1
liumiaocn:openldap liumiao$`
## 修改Barbara的uid
事前准备ldif文件
`liumiaocn:openldap liumiao$ cat modifybarbara.ldif 
dn: uid=bjensen,ou=people,dc=example,dc=org
changetype: modrdn
newrdn: uid=bjensenuid
deleteoldrdn: 0
newsuperior: ou=people,dc=example,dc=org
liumiaocn:openldap liumiao$`将ldif文件通过-f传给ldapmodify
`liumiaocn:openldap liumiao$ ldapmodify -a -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f modifybarbara.ldif 
modifying rdn of entry "uid=bjensen,ou=people,dc=example,dc=org"
liumiaocn:openldap liumiao$`结果确认
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin
...省略
# bjensenuid, People, example.org
dn: uid=bjensenuid,ou=People,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
mail: bjensen@example.com
uid: bjensen
uid: bjensenuid
title: one of the world's most famous mythical manager
description: Barbara description
...省略
liumiaocn:openldap liumiao$`
# 使用ldapdelete或者ldapmodify进行删除
## 删除Barbara的description字段
事前准备ldif文件
`liumiaocn:openldap liumiao$ cat modifybarbara.ldif 
dn: uid=bjensenuid,ou=people,dc=example,dc=org
changetype: modify
delete: description
liumiaocn:openldap liumiao$`将ldif文件通过-f传给ldapmodify
`liumiaocn:openldap liumiao$ ldapmodify -a -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f modifybarbara.ldif 
modifying entry "uid=bjensenuid,ou=people,dc=example,dc=org"
liumiaocn:openldap liumiao$`结果确认
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin uid=bjensenuid
# extended LDIF
#
# LDAPv3
# base <dc=example,dc=org> with scope subtree
# filter: uid=bjensenuid
# requesting: ALL
#
# bjensenuid, People, example.org
dn: uid=bjensenuid,ou=People,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
mail: bjensen@example.com
uid: bjensen
uid: bjensenuid
title: one of the world's most famous mythical manager
# search result
search: 2
result: 0 Success
# numResponses: 2
# numEntries: 1
liumiaocn:openldap liumiao$`
## 删除Barbara
使用ldapdelete删除entry只需指定dn即可，比如删除前文中修改uid后的Barbara：
`liumiaocn:openldap liumiao$ ldapdelete -x -h 192.168.31.242  -D "cn=admin,dc=example,dc=org" -w admin  "uid=bjensenuid,ou=People,dc=example,dc=org"
liumiaocn:openldap liumiao$ echo $?
0
liumiaocn:openldap liumiao$`删除后结果确认, 使用此uid已经无法查询出相关记录了
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin uid=bjensenuid
# extended LDIF
#
# LDAPv3
# base <dc=example,dc=org> with scope subtree
# filter: uid=bjensenuid
# requesting: ALL
#
# search result
search: 2
result: 0 Success
# numResponses: 1
liumiaocn:openldap liumiao$`使用ldapmodify结合如下ldif也同样可以实现删除，这里不再演示
`dn: uid=bjensenuid,ou=people,dc=example,dc=org
changetype: delete`
# 总结
使用ldapmodify和ldapdelete能够实现常见的ldap信息修改和删除相关的需要。

