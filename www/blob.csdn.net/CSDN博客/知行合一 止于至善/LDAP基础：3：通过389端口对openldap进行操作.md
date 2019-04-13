
# LDAP基础：3：通过389端口对openldap进行操作 - 知行合一 止于至善 - CSDN博客

2018年11月15日 06:03:53[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2101


在前面的文章中介绍了如何使用docker快速搭建ldap服务并进行确认，以及在java中如何使用jndi对ldap进行访问，但是按照官方github上的示例，由于没有将端口暴露出来，所以在宿主机对容器内运行的openldap服务进行访问只能通过容器IP，而且前提是宿主机和容器是能够互通的。但是只需要将服务的端口暴露出来，在外部也可以直接访问了。
# 端口及说明
> 缺省ldap使用389端口，加密使用636

# 启动
使用 -p将389端口进行暴露
> docker run --name openldap -p 389:389 --detach osixia/openldap:1.2.2

# 确认宿主机IP
`liumiaocn:openldap liumiao$ ifconfig |grep 192
	inet 192.168.31.242 netmask 0xffffff00 broadcast 192.168.31.255
liumiaocn:openldap liumiao$`
# 连接方式1: 通过ldapuri
通过ldapsearch的ldapuri进行访问，形式如下：
> ldapsearch -x -H ldap://IP地址或者可解析的地址:端口 -b 指定的baseDN -D 绑定的服务器的DN -w 简单认证方式的密码
使用示例
`liumiaocn:openldap liumiao$ ldapsearch -x -H ldap://192.168.31.242:389 -b dc=example,dc=org -D "cn=admin,dc=example,dc=org" -w admin
# extended LDIF
#
# LDAPv3
# base <dc=example,dc=org> with scope subtree
# filter: (objectclass=*)
# requesting: ALL
#
# example.org
dn: dc=example,dc=org
objectClass: top
objectClass: dcObject
objectClass: organization
o: Example Inc.
dc: example
# admin, example.org
dn: cn=admin,dc=example,dc=org
objectClass: simpleSecurityObject
objectClass: organizationalRole
cn: admin
description: LDAP administrator
userPassword:: e1NTSEF9TE5GaU0rZVZXNGlRR3FDdzQ5UzRMNjRlb2xjMjY5OU4=
# search result
search: 2
result: 0 Success
# numResponses: 3
# numEntries: 2
liumiaocn:openldap liumiao$`另外，如果端口为389，在ldapuri中可以省略，上述例子则变为：
> ldapsearch -x -H ldap://192.168.31.242 -b dc=example,dc=org -D “cn=admin,dc=example,dc=org” -w admin

# 连接方式2: 使用hostname和ip的方式
这里继续使用ldapsearch作为连接验证方式，除了-H指定ldapuri，也可以直接通过-h和-p来指定hostname和ip的方式进行
> ldapsearch -x -h IP地址或者可解析的地址 -p 端口 -b 指定的baseDN -D 绑定的服务器的DN -w 简单认证方式的密码
使用实例
`liumiaocn:openldap liumiao$ ldapsearch -x -h 192.168.31.242 -p 389 -b dc=example,dc=org -D "cn=admin,dc=example,dc=org" -w admin
# extended LDIF
#
# LDAPv3
# base <dc=example,dc=org> with scope subtree
# filter: (objectclass=*)
# requesting: ALL
#
# example.org
dn: dc=example,dc=org
objectClass: top
objectClass: dcObject
objectClass: organization
o: Example Inc.
dc: example
# admin, example.org
dn: cn=admin,dc=example,dc=org
objectClass: simpleSecurityObject
objectClass: organizationalRole
cn: admin
description: LDAP administrator
userPassword:: e1NTSEF9TE5GaU0rZVZXNGlRR3FDdzQ5UzRMNjRlb2xjMjY5OU4=
# search result
search: 2
result: 0 Success
# numResponses: 3
# numEntries: 2
liumiaocn:openldap liumiao$`同样，port为389时也可以省略，上述方式可以变为：
> ldapsearch -x -h 192.168.31.242  -b dc=example,dc=org -D “cn=admin,dc=example,dc=org” -w admin

# 连接方式3: 使用JNDI方式连接
使用java的JNDI可以对LDAP进行操作，前面的文章中给除了简单的示例代码进行验证，详细参看：
LDAP基础：2：使用Java验证OpenLdap用户登录：[https://blog.csdn.net/liumiaocn/article/details/83720115](https://blog.csdn.net/liumiaocn/article/details/83720115)
这里接着使用对刚刚启动的OpenLdap容器的服务进行连接确认, 首先确认连接信息
Ldap使用的IP： 192.168.31.242
用户名（DN）：cn=admin,dc=example,dc=org
用户密码：admin
## 使用IP+port的ldapuri
`liumiaocn:~ liumiao$ java LdapDemo "192.168.31.242:389" "cn=admin,dc=example,dc=org" "admin"
IPAdress: 192.168.31.242:389
Username: cn=admin,dc=example,dc=org
Password: admin
env setting
login verification begins...
login successfully.
liumiaocn:~ liumiao$`
## port为389的情况下也可以省略
`liumiaocn:~ liumiao$ java LdapDemo "192.168.31.242" "cn=admin,dc=example,dc=org" "admin"
IPAdress: 192.168.31.242
Username: cn=admin,dc=example,dc=org
Password: admin
env setting
login verification begins...
login successfully.
liumiaocn:~ liumiao$`
# 总结
这样可以看到，一个通过docker方式快速启动的OpenLdap服务，简单的使用方式已经就绪，可以通过ldapsearch从客户端进行访问，也可以通过代码的编程接口进行访问。

