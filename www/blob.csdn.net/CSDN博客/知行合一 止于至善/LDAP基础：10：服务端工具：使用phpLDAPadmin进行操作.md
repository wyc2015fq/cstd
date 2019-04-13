
# LDAP基础：10：服务端工具：使用phpLDAPadmin进行操作 - 知行合一 止于至善 - CSDN博客

2018年11月17日 06:21:26[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1747


在上篇文章中介绍了如何使用Apache Directory Studio的客户端来操作LDAP，但是这种方式需要安装客户端工具。在这篇文章里将介绍服务器端的工具phpLDAPadmin，这样用户通过浏览器即可获得对于OpenLdap操作的能力。
# 事前准备
## docker-compose设定
使用如下的docker-compose.yml，即可快速创建一个OpenLdap+phpLdapAdmin的环境。
`liumiaocn:openldap liumiao$ cat docker-compose.yml 
version: '2'
services:
  openldap:
    container_name: openldap
    image: osixia/openldap:1.2.2
    ports:
      - "389:389"
      - "636:636"
    command: [--copy-service,  --loglevel, debug]
  phpldapadmin:
    container_name: phpldapadmin
    image: osixia/phpldapadmin:0.7.2
    ports:
      - "80:80"
    environment:
      - PHPLDAPADMIN_HTTPS="false"
      - PHPLDAPADMIN_LDAP_HOSTS=openldap
    links:
      - openldap
    depends_on:
      - openldap
liumiaocn:openldap liumiao$`以上docker-compose也放到了easypack中。
链接地址：[https://github.com/liumiaocn/easypack/tree/master/containers/standard/openldap](https://github.com/liumiaocn/easypack/tree/master/containers/standard/openldap)
## 环境启动
`liumiaocn:openldap liumiao$ docker-compose up -d
Creating network "openldap_default" with the default driver
Creating openldap ... done
Creating phpldapadmin ... done
liumiaocn:openldap liumiao$`这样环境已经准备就绪，可以开始使用确认了。
# phpLDAPadmin登陆
上述方式使用缺省的内嵌的信息，登陆可以使用admin的dn
登陆URL：http://
用户信息：cn=admin,dc=example,dc=org
密码：admin
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116175828938.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
登陆之后可以看到如下信息，只有cn=admin的信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116180400948.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
通过ldif方式创建entry
使用前面使用过的ldif文件来创建ou和Barbara用户
liumiaocn:Desktop liumiao$ cat demo.ldif 
dn: ou=People,dc=example,dc=org
objectclass: top
objectclass: organizationalUnit
ou: People
dn: ou=Servers,dc=example,dc=org
objectclass: top
objectclass: organizationalUnit
ou: Servers
\# Barbara Jensen, example.org
dn: cn=Barbara Jensen,dc=example,dc=org
objectClass: inetOrgPerson
cn: Barbara Jensen
cn: Babs Jensen
sn: Jensen
title: the world's most famous mythical manager
mail: bjensen@example.com
uid: bjensenliumiaocn:Desktop liumiao$注意在这个版本的phpLDAPadmin中需要删除“cn: Babs Jensen”，不然Barbara无法成功创建
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116180829265.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
点击Proceed即可创建成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116180907997.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
调整Barbara到People下
同样，使用如下ldif文件即可调整
dn: cn=Barbara Jensen,dc=example,dc=org
changetype: modrdn
newrdn: uid=bjensen
deleteoldrdn: 0
newsuperior: ou=people,dc=example,dc=org![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116181108642.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
点击Proceed便可成功调整至People，但是显示稍微有点不正常
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116181219631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
退出，重新登陆就正常了，不用太介意
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116181344693.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
整体来说使用还是较为顺畅的。
总结
使用phpLDAPadmin可以通过图形界面进行快速操作，而且还集成了很多模版进行使用，普通功能均可实现，对于界面颜值要求不高，功能要求不强的用户，也算谁不错的一个选择。

[
](https://img-blog.csdnimg.cn/20181116181219631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
