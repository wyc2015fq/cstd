
# LDAP基础：2：使用Java验证OpenLdap用户登录 - 知行合一 止于至善 - CSDN博客

2018年11月04日 20:52:34[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：3268


这篇文章整理一下如何使用Java进行OpenLdap用户登陆验证。
# 事前准备
有自己的Ldap服务器或者使用OpenLdap搭建一个简单的服务器，可以参看：
[https://blog.csdn.net/liumiaocn/article/details/83719568](https://blog.csdn.net/liumiaocn/article/details/83719568)
# Java相关的类
在java中主要使用javax.naming来进行验证，验证过程中会使用到：
javax.naming.Context;
javax.naming.directory.DirContext;
javax.naming.directory.InitialDirContext;
# 代码示例
这里给出最简单的代码验证示例
`[root@platform ~]# cat LdapDemo.java
import java.util.Hashtable;
import javax.naming.Context;
import javax.naming.directory.DirContext;
import javax.naming.directory.InitialDirContext;
public class LdapDemo{
  public static void main(String[] args){
    System.out.println("IPAdress: " + args[0]);
    System.out.println("Username: " + args[1]);
    System.out.println("Password: " + args[2]);
    Hashtable<String, String> tbl = new Hashtable<String, String>(4);
    String LDAP_URL = "ldap://" +args[0]+ "/dc=example,dc=org";
    tbl.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.ldap.LdapCtxFactory");
    tbl.put(Context.PROVIDER_URL, LDAP_URL);
    tbl.put(Context.SECURITY_AUTHENTICATION, "simple");
    tbl.put(Context.SECURITY_PRINCIPAL, args[1]);
    tbl.put(Context.SECURITY_CREDENTIALS, args[2]);
    System.out.println("env setting");
    DirContext context = null;
    try {
        System.out.println("login verification begins...");
        context = new InitialDirContext(tbl);
        System.out.println("login successfully.");
    } catch (Exception ex) {
        System.out.println("login failed.");
    } finally {
        try {
            if (context != null) {
                context.close();
                context = null;
            }
            tbl.clear();
        } catch (Exception e) {
            System.out.println("exception happened.");
        }
    }
  }
}
[root@platform ~]#`
# 验证
事前准备：需要准备如下环境，这里从宿主机访问容器中运行的OpenLdap服务，本文相关信息如下：
Ldap使用的IP： 172.17.0.3
用户名：cn=admin,dc=example,dc=org
用户密码：admin
`[root@platform ~]# ldapsearch -x -H ldap://172.17.0.3 -b dc=example,dc=org -D "cn=admin,dc=example,dc=org" -w admin
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
userPassword:: e1NTSEF9RTBJMTlqaW1BTVlaZ0h2aEhPbm5YVVdWRmQxaEtBRnM=
# search result
search: 2
result: 0 Success
# numResponses: 3
# numEntries: 2
[root@platform ~]#`
# 结果确认
## 成功登陆用例
`[root@platform ~]# javac LdapDemo.java 
[root@platform ~]# java LdapDemo "172.17.0.3" "cn=admin,dc=example,dc=org" "admin"
IPAdress: 172.17.0.3
Username: cn=admin,dc=example,dc=org
Password: admin
env setting
login verification begins...
login successfully.
[root@platform ~]#`
## 失败登陆用例
使用错误的用户密码，结果显示无法登陆
`[root@platform ~]# javac LdapDemo.java 
[root@platform ~]# java LdapDemo "172.17.0.3" "cn=admin,dc=example,dc=org" "admin1"
IPAdress: 172.17.0.3
Username: cn=admin,dc=example,dc=org
Password: admin1
env setting
login verification begins...
login failed.
[root@platform ~]#`
# 总结
Java中使用javax.naming可以对Ldap用户信息进行验证，使用这点可以完成SSO之类功能的集成。

