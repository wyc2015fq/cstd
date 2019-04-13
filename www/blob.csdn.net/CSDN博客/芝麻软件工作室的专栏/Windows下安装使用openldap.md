
# Windows下安装使用openldap -  芝麻软件工作室的专栏 - CSDN博客


2016年07月03日 08:51:48[seven-soft](https://me.csdn.net/softn)阅读数：2600


openldap 比起其他商业目录服务器(比如 IBM Directory Server)，特别的轻巧，十分适合于本地开发测试用，在产品环境中的表现也很优秀。
openldap 软件在它的官方网站[http://www.openldap.org](http://www.openldap.org/), 不过下载过来是源代码，并没有包含 win32 下的 Makefile 文件，只提供了在 Unix/Linux 下编译用的 Makefile。所以相应的在网上介绍在 windows 下安装使用 openldap 的资料比较少，而在
 Unix/Linux 下应用文档却很丰富。
本文实践了在 Windows 下安装配 openldap，并添加一个条目，LdapBrowser 浏览，及 Java 程序连接 openldap 的全过程。
**1. 下载安装 openldap for windows**，当前版本2.2.29
下载地址：[http://download.bergmans.us/openldap/openldap-2.2.29/openldap-2.2.29-db-4.3.29-openssl-0.9.8a-win32_Setup.exe](http://download.bergmans.us/openldap/openldap-2.2.29/openldap-2.2.29-db-4.3.29-openssl-0.9.8a-win32_Setup.exe)
相关链接：[http://lucas.bergmans.us/hacks/openldap/](http://lucas.bergmans.us/hacks/openldap/)
安装很简单，一路 next 即可，假设我们安装在 c:/openldap
**2. 配置 openldap**，编辑 sldap.conf 文件
1) 打开 c:/openldap/sldap.conf，找到
include  C:/openldap/etc/schema/core.schema，在它后面添加
include  C:/openldap/etc/schema/cosine.schema
include  C:/openldap/etc/schema/inetorgperson.schema
接下来的例子只需要用到以上三个 schema，当然，如果你觉得需要的话，你可以把其他的 schema 全部添加进来
include  C:/openldap/etc/schema/corba.schema
include  C:/openldap/etc/schema/dyngroup.schema
include  C:/openldap/etc/schema/java.schema
include  C:/openldap/etc/schema/misc.schema
include  C:/openldap/etc/schema/nis.schema
include  C:/openldap/etc/schema/openldap.schema
2) 还是在 sldap.conf 文件中，找到
suffix  "dc=my-domain,dc=com"
rootdn  "cn=Manager,dc=my-domain,dc=com"
把这两行改为
suffix "o=tcl,c=cn"
rootdn "cn=Manager,o=tcl,c=cn"
suffix 就是看自己如何定义了，后面步骤的 ldif 文件就必须与它定义了。还要注意到这个配置文件中有一个 rootpw  secret，这个 secret 是 cn=Manager 的密码，以后会用到，不过这里是明文密码，你可以用命令： slappasswd -h {MD5} -s secret 算出加密的密码 {MD5}Xr4ilOzQ4PCOq3aQ0qbuaQ== 取代配置中的 secret。
**3. 启动 openldap**
CMD 进入到 c:/openldap 下，运行命令 sldapd -d 1
用可以看到控制台下打印一片信息，openldap 默认是用的 Berkeley DB 数据库存储目录数据的。
**4. 建立条目**,编辑导入 ldif 文件
1) 新建一个 ldif(LDAP Data Interchanged Format) 文件(纯文本格式)，例如 test.ldif，文件内容如下：
dn: o=tcl,c=cn
objectClass: dcObject
objectClass: organization
o: tcl
dc: com
dn: uid=Unmi, o=tcl,c=cn
uid: Unmi
objectClass: inetOrgPerson
mail:[fantasia@sina.com](mailto:fantasia@sina.com)
userPassword:: MTIzNDU2
labeledURI:[http://unmi.blogcn.com](http://unmi.blogcn.com/)
sn: Qiu
cn: 隔叶黄莺
2) 执行命令：ldapadd -x -D "cn=manager,o=tcl,c=cn" -w secret -f test.ldif
导入组织信息和一个用户 uid=Unmi
**5. LdapBrowser 浏览**
可点击链接[http://www.blogjava.net/Files/Unmi/LdapBrowser282.rar](http://www.blogjava.net/Files/Unmi/LdapBrowser282.rar)下载，其中已配置好了 OpenLdap_Localhost
1) 设置如下图所示：
![LdapBrowserSettings](http://images.blogcn.com/2007/7/27/2/unmi,20070727022653358.JPG)
指定了 Host 为 localhost 之后，可以点击 Fetch DNs 按钮显示出 o=tcl,c=cn 来，如果要能在 LdapBrowser 中对数据能修改就不能用 Anonymous bind, 必须填上 User DN: cn=manager，Passwer: secret。
2) 看到的效果是：
![LdapBrowser](http://images.blogcn.com/2007/7/27/2/unmi,20070727023002205.JPG)
**6. Java 连接 openldap**

importjava.util.Hashtable;
importjavax.naming.Context;
importjavax.naming.NamingException;
importjavax.naming.directory.DirContext;
importjavax.naming.directory.InitialDirContext;
publicclassLDAPTest {
publicstaticvoidmain(String[] args) {
LDAPTest LDAPTest1 =newLDAPTest();
String root ="o=tcl,c=cn";//root
Hashtable env =newHashtable();
env.put(Context.INITIAL_CONTEXT_FACTORY,"com.sun.jndi.ldap.LdapCtxFactory");
env.put(Context.PROVIDER_URL,"ldap://localhost/"+ root);
env.put(Context.SECURITY_AUTHENTICATION,"simple");
env.put(Context.SECURITY_PRINCIPAL,"cn=Manager,o=tcl,c=cn");
env.put(Context.SECURITY_CREDENTIALS,"secret");
DirContext ctx =null;
try{
ctx =newInitialDirContext(env);
System.out.println("认证成功");
}
catch(javax.naming.AuthenticationException e) {
e.printStackTrace();
System.out.println("认证失败");
}
catch(Exception e) {
System.out.println("认证出错：");
e.printStackTrace();
}
if(ctx !=null) {
try{
ctx.close();
}
catch(NamingException e) {
//ignore
}
}
}
}
代码中还没有实现用户的查找，读取、修改条目属性的操作

