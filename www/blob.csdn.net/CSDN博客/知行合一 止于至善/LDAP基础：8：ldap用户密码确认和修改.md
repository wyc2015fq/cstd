
# LDAP基础：8：ldap用户密码确认和修改 - 知行合一 止于至善 - CSDN博客

2018年11月17日 05:19:55[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2717


ldap用户密码的修改可以使用ldappasswd命令，也可以使用万能的ldapmodify结合ldif文件来实现，但所修改的都是普通的用户，cn=admin的管理员用户的修改一般可以通过slappasswd来进行，由于本系列使用了openldap的docker镜像，此项功能已被封装，通过设定环境变量即可轻易实现。
# 修改密码的常用方式
ldappasswd命令
ldapmodify命令结合ldif文件
slappasswd命令
# 管理员密码的设定
管理员密码的设定可以通过slappasswd，由于本系列文章的示例使用了osixia的openldap镜像，cn=admin的密码设定，只需要对环境变量LDAP_ADMIN_PASSWORD进行设定即可。
# Option介绍
## ldapmodify常用Option
ldapmodify常用的Option的信息和用途整理如下：
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
## ldappasswd常用Option
ldappasswd常用的Option的信息和用途整理如下：
|Option|说明|
|---|---|
|-H|ldapuri，格式为ldap://机器名或者IP:端口号，不能与-h和-p同时使用|
|-h|LDAP服务器IP或者可解析的hostname，与-p可结合使用，不能与-H同时使用|
|-p|LDAP服务器端口号，与-h可结合使用，不能与-H同时使用|
|-x|使用简单认证方式|
|-D|所绑定的服务器的DN|
|-w|绑定DN的密码，与-W二者选一|
|-W|不输入密码，会交互式的提示用户输入密码，与-w二者选一|
|-n|模拟操作但并不实际执行，用于验证，常与-v一同使用进行问题定位|
|-v|显示详细信息|
|-d|显示debug信息，可设定级别|
|-S|交互式进行密码的提示和输入以及Re-enter，与-s二者选一|
|-s|将指定内容设为密码，与-S二者选一|
# 事前准备
使用ldif方式创建barbara用户的信息
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
liumiaocn:openldap liumiao$`用户添加
`liumiaocn:openldap liumiao$ ldapadd -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f barbara.ldif
adding new entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`结果确认
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin "uid=bjensen"
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
# 修改密码
## 方式1:  ldappasswd不指定密码
不指定密码的情况下，ldappasswd会给你生成一个密码。ldap的用户密码是通过userPassword来保存的，但是在使用ldif的时候没有这个字段，所以可以使用前文中提到的方式使用ldapmodify来进行添加，也可使用如下方式：
`liumiaocn:openldap liumiao$ ldappasswd -x -h 192.168.31.242 -p 389 -D "cn=admin,dc=example,dc=org" -w admin  "cn=Barbara Jensen,dc=example,dc=org"
New password: VSzhfbwA
liumiaocn:openldap liumiao$`可以看到自动创建了一个密码串VSzhfbwA给指定的Barbara用户，然后来重新对此用户进行确认，可以看到加密后的userPassword信息已经出现在LDIF中了。
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "dc=example,dc=org" -D "cn=admin,dc=example,dc=org" -w admin "uid=bjensen"
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
userPassword:: e1NTSEF9ZDYwU1NNMjJxb05vOTZ6OHg4b1JVMmxuRmpKNmR2SjE=
# search result
search: 2
result: 0 Success
# numResponses: 2
# numEntries: 1
liumiaocn:openldap liumiao$`
## 结果验证
### 验证方法1: 使用java的JNDI
对修改的密码进行验证，可以使用前文中的Java的JNDI做的简单的代码进行验证：
`liumiaocn:~ liumiao$ java LdapDemo 192.168.31.242 "cn=Barbara Jensen,dc=example,dc=org" "VSzhfbwA"
IPAdress: 192.168.31.242
Username: cn=Barbara Jensen,dc=example,dc=org
Password: VSzhfbwA
env setting
login verification begins...
login successfully.
liumiaocn:~ liumiao$`详细参看：
[LDAP基础：2：使用Java验证OpenLdap用户登录](https://blog.csdn.net/liumiaocn/article/details/83720115)
### 验证方法2: 使用ldapsearch等命令
考虑到权限，只查询自己的信息，看看是否能够正确返回，主要是用于验证-w的内容
`liumiaocn:~ liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "cn=Barbara Jensen,dc=example,dc=org" -D "cn=Barbara Jensen,dc=example,dc=org" -w VSzhfbwA "uid=bjensen"
# extended LDIF
#
# LDAPv3
# base <cn=Barbara Jensen,dc=example,dc=org> with scope subtree
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
userPassword:: e1NTSEF9ZDYwU1NNMjJxb05vOTZ6OHg4b1JVMmxuRmpKNmR2SjE=
# search result
search: 2
result: 0 Success
# numResponses: 2
# numEntries: 1
liumiaocn:~ liumiao$`因为错误的时候必然会提示：
`liumiaocn:~ liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b "cn=Barbara Jensen,dc=example,dc=org" -D "cn=Barbara Jensen,dc=example,dc=org" -w VSzhfbwA1 "uid=bjensen"
ldap_bind: Invalid credentials (49)
liumiaocn:~ liumiao$`所以以此类推，很多命令和Option都可以进行利用，比如-n，各种扩展方式可自行展开。
## ldappasswd使用-s指定修改密码
前文不指定密码的方式实际是使用admin对其他用户的密码进行重设，用户自己知道密码的情况下，进行更新则可以使用这种方式，比如：
`liumiaocn:openldap liumiao$ ldappasswd -x -h 192.168.31.242 -p 389 -D "cn=Barbara Jensen,dc=example,dc=org" -w VSzhfbwA -s liumiao123
liumiaocn:openldap liumiao$ echo $?
0
liumiaocn:openldap liumiao$`这样Barbara就将自己的密码改成了liumiao123，结果确认发现，密码能够正常使用
`liumiaocn:~ liumiao$ java LdapDemo 192.168.31.242 "cn=Barbara Jensen,dc=example,dc=org" "liumiao123"
IPAdress: 192.168.31.242
Username: cn=Barbara Jensen,dc=example,dc=org
Password: liumiao123
env setting
login verification begins...
login successfully.
liumiaocn:~ liumiao$`结合这两种方式，常见的密码重设问题基本都可以解决，比如在不知道密码的情况下，可以使用方式1先通过admin进行重新生成密码，再使用方式2重新设定指定密码，当然ldappasswd还有很多其他有用Option，使用方式可自行探索。
## 方式3: 使用ldapmodify来进行密码修改
ldapmodify作为一个万能命令，除了查询无法实现之外，增删改方面有广泛的应用，对于密码修改自然也非常方便，其实使用前文介绍的方法就可以，这里再次示例如下：
准备修改密码的ldif文件，在文件中设定密码为babspw
`liumiaocn:openldap liumiao$ cat modifybarbara.ldif 
dn: cn=Barbara Jensen,dc=example,dc=org
changetype: modify
replace: userPassword
userPassword: babspw
liumiaocn:openldap liumiao$`使用ldapmodify修改密码
`liumiaocn:openldap liumiao$ ldapmodify -a -H ldap://192.168.31.242:389 -D "cn=admin,dc=example,dc=org" -w admin -f modifybarbara.ldif 
modifying entry "cn=Barbara Jensen,dc=example,dc=org"
liumiaocn:openldap liumiao$`结果确认
`liumiaocn:~ liumiao$ java LdapDemo 192.168.31.242 "cn=Barbara Jensen,dc=example,dc=org" "babspw"
IPAdress: 192.168.31.242
Username: cn=Barbara Jensen,dc=example,dc=org
Password: babspw
env setting
login verification begins...
login successfully.
liumiaocn:~ liumiao$`

