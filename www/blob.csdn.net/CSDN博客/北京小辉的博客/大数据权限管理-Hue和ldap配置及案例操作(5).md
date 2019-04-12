
# 大数据权限管理-Hue和ldap配置及案例操作(5) - 北京小辉的博客 - CSDN博客
原
# 大数据权限管理-Hue和ldap配置及案例操作(5)
2019年04月04日 09:30:37[silentwolfyh](https://me.csdn.net/silentwolfyh)阅读数：178


版权声明：本文为博主原创文章，出处为 http://blog.csdn.net/silentwolfyh					https://yuhui.blog.csdn.net/article/details/88851951
## 目录
### 一、LDAP和CDH配置
**1)LDAP和Hive的集成**
**2)LDAP和Hive的测试**
**3)LDAP和impala的集成**
**4)LDAP和impala的测试**
**5)LDAP和HUE的集成（搜索绑定）**
a)hue配置
b)创建ldap组
c)创建ldap用户
d)前台展示组
e)前台展示用户
### 二、Ladp的操作使用
**1)基本操作**
**2)Ldap建立组和建立用户**
### 三、Error
**1)Ldap启动错误**
**2)ldapmodify执行出错**
**3)Ldap和hive、impala集成错误**
**4)Hue连接hive和impala错误**
**5)Hue添加用户**
**6)Hue用户无法同步**
**7)Hue组无法同步**
**8)HDFS错误**
**9)Impala错误**
### 四、Hue元数据表说明
**1)auth_group 组名称表**
**2)auth_user 用户表**
**3)auth_user_groups 用户和组对应表**
**4)useradmin_grouppermission hue组权限表**
**5)useradmin_huepermission  hue的权限表**
**6)useradmin_ldapgroup ldap组和hue组关联表**
**7)useradmin_userprofile 用户hdfs路径表信息**
### 五、参看其余文档
### 一、LDAP和CDH配置
**1)LDAP和Hive的集成**
[http://cxy7.com/articles/2018/07/17/1531808242972.html](http://cxy7.com/articles/2018/07/17/1531808242972.html)
```
ldap://silentwolfyh.com
ou=people,dc=silentwolfyh,dc=com
hadoop.proxyuser.hue.hosts  (hue是代理用户)
hadoop.proxyuser.hue.groups (hue是代理用户)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404085610644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[账号test ， 密码123456](https://img-blog.csdnimg.cn/20190404085610644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
[yuhui@hadoop11 ~]\$ beeline -u "jdbc:hive2://hadoop11:10000" -n yuhui1 -p 123456 -d org.apache.hive.jdbc.HiveDriver
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090111828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**2)LDAP和Hive的测试**
[
](https://img-blog.csdnimg.cn/20190404090111828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
!connect jdbc:hive2://hadoop11:10000/
chenxiaolong34
123456
```
[
](https://img-blog.csdnimg.cn/20190404090111828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090226525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**3)LDAP和impala的集成**
[http://cxy7.com/articles/2018/07/17/1531827540376.html(重点)](http://cxy7.com/articles/2018/07/17/1531827540376.html(%E9%87%8D%E7%82%B9))
```
enable_ldap_auth
ldap_uri=ldap://silentwolfyh.com:389
LDAP BaseDN=ou=people,dc=silentwolfyh,dc=com
--ldap_passwords_in_clear_ok=true
--authorized_proxy_user_config=hue=*
hadoop.proxyuser.hue.hosts （hue代理用户）
hadoop.proxyuser.hue.groups（hue代理用户）
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090305709.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/201904040903223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**4)LDAP和impala的测试**
[
](https://img-blog.csdnimg.cn/201904040903223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
impala-shell -l -u chenxiaolong34 --auth_creds_ok_in_clear
```
[
](https://img-blog.csdnimg.cn/201904040903223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090411157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090451282.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**5)LDAP和HUE的集成（搜索绑定）**
[
](https://img-blog.csdnimg.cn/20190404090451282.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[https://blog.csdn.net/u011026329/article/details/79171890](https://blog.csdn.net/u011026329/article/details/79171890)
[
](https://img-blog.csdnimg.cn/20190404090451282.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)a)hue配置
界面配置内容如下，图片如下：
```
backend=desktop.auth.backend.LdapBackend
ldap_url= ldap://172.25.33.131:389
ldap_cert= /path_to_certificate/cert.pem
use_start_tls= false
ldap_username_pattern="uid=<username>,ou=people,dc=silentwolfyh,dc=com "
使用搜索绑定身份验证
search_bind_authentication = true
create_users_on_login=true
LDAP 搜索基础
base_dn="dc=silentwolfyh,dc=com "
LDAP 绑定用户可分辨名称
bind_dn="uid=hue,ou=people,dc=silentwolfyh,dc=com "
LDAP 绑定密码
bind_password=hue
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090614576.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
LDAP 用户名属性(LDAP Username Attribute)：user_name_attr = uid 
LDAP 用户筛选（LDAP User Filter）：user_filter = (objectClass=posixAccount)
LDAP 组筛选（LDAP Group Filter：group_filter = (|(objectClass=organizationalUnit)(objectClass=posixGroup)) 
LDAP 组名称属性（LDAP Group Name Attribute）：group_name_attr = cn 
LDAP 组成员身份属性（LDAP Group Membership Attribute）：group_member_attr = memberUID \# 非常重要，否则无法识别映射
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090742241.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090749117.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040409075520.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040409080126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090806978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[hue_safety_valve.ini](https://img-blog.csdnimg.cn/20190404090806978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
[
](https://img-blog.csdnimg.cn/2019040409080126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
[desktop]
[[ldap]]
  sync_groups_on_login=true
```
[
](https://img-blog.csdnimg.cn/2019040409080126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404122759549.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
b)创建ldap组
```
useradd songxitang
sudo groupadd silentwolfyh_ops
usermod -g silentwolfyh_ops songxitang
passwd songxitang (123456)
```
```
[root@hadoop11 ~]\# id songxitang
uid=810(songxitang) gid=811(silentwolfyh_ops) groups=811(silentwolfyh_ops)
[root@ hadoop11 ~]\# cat  /tmp/create_group_silentwolfyh_ops.ldif
dn: cn=silentwolfyh_ops,ou=silentwolfyh,dc=silentwolfyh,dc=com
objectClass: top
objectClass: posixGroup
gidNumber: 810
memberUID: \${MEMBER_UID}
description: \${DESCRIPTION_STIRNG}
ldapadd -x -D "uid=hue,ou=people,dc=silentwolfyh,dc=com" -w hue -f /tmp/create_group_silentwolfyh_ops.ldif
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090921540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[              c)创建ldap用户](https://img-blog.csdnimg.cn/20190404090921540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
[root@hadoop11 ~]\# cat /tmp/create_user_songxitang.ldif
dn: uid=songxitang,cn=silentwolfyh_ops,ou=silentwolfyh,dc=silentwolfyh,dc=com
objectClass: top
objectClass: inetOrgPerson
objectClass: posixAccount
objectClass: shadowAccount
givenName: xitang
sn: song
cn: songxitang
mail: songxitang@domain.com
telephoneNumber: 0101-3333333
mobile: 13111111111
uid: songxitang
uidNumber: 1005
gidNumber: 1006
homeDirectory: /home/songxitang
loginShell: /bin/bash
ldapadd -x -D "uid=hue,ou=people,dc=silentwolfyh,dc=com" -w hue -f /tmp/create_user_songxitang.ldif
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091000690.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[修改用户密码](https://img-blog.csdnimg.cn/20190404091000690.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
ldappasswd -x -D "uid=hue,ou=people,dc=silentwolfyh,dc=com" -w hue "uid=songxitang,cn=silentwolfyh_ops,ou=silentwolfyh,dc=example,dc=com" -S
```
d)前台展示组
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091054867.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091102576.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[              e)前台展示用户](https://img-blog.csdnimg.cn/20190404091102576.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091122426.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
### 二、Ladp的操作使用
**1)基本操作**
[https://blog.csdn.net/u011026329/article/details/79171996](https://blog.csdn.net/u011026329/article/details/79171996)
[https://www.zhukun.net/archives/7980](https://www.zhukun.net/archives/7980)
添加ldap的文件
```
ldapadd -x -D "uid=hue,ou=people,dc=javachen,dc=com" -w hue -f xxx.ldif
```
修改ldap密码
```
ldappasswd -x -D "uid=hue,ou=people,dc=javachen,dc=com" -w hue "uid=people1,cn=silentwolfyh_ops,ou=silentwolfyh,dc=javachen,dc=com" -S
```
查询
```
ldapsearch -x -D "uid=hue,ou=people,dc=javachen,dc=com" -w hue -b "uid=user1,cn=silentwolfyh_ops,ou=silentwolfyh,dc=javachen,dc=com"
```
删除:用户或组条目
```
\$ ldapdelete -x -w root -D 'uid=ldapadmin,ou=people,dc=example,dc=com' "uid=test,ou=people,dc=example,dc=com" 
\$ ldapdelete -x -w root -D 'uid=ldapadmin,ou=people,dc=example,dc=com' "cn=test,ou=group,dc=example,dc=com"
```
**2)Ldap建立组和建立用户**
建立组织单元:organizationalUnit
```
dn: ou=language,dc=javachen,dc=com
objectClass: organizationalUnit
ou: language
```
建立组：在organizationalUnit的silentwolfyh部门下面建立ml组
```
dn: cn=ml,ou=silentwolfyh,dc=javachen,dc=com
objectClass: top
objectClass: posixGroup
cn: ml
gidNumber: 823
description: 机器学习
```
建立用户1，2，3(可以建多个用户放入一个ldif文件中，批量加用户)
```
dn: uid=people1,cn=ml,ou=silentwolfyh,dc=javachen,dc=com
objectClass: top
objectClass: inetOrgPerson
objectClass: posixAccount
objectClass: shadowAccount
sn:people1
cn:people1
uid: people1
uidNumber: 817
gidNumber: 823
homeDirectory: /home/people1
loginShell: /bin/bash
```
\#用户和组的映射  add_user_to_group.ldif
```
dn: cn=ml,ou=silentwolfyh,dc=javachen,dc=com
changetype: modify
add: memberUid
memberUid: people1
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091341269.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091348599.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091356900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
[
](https://img-blog.csdnimg.cn/20190404091348599.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
### 三、Error
[
](https://img-blog.csdnimg.cn/20190404091348599.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)**1)Ldap启动错误**
启动 LDAP 服务：
```
chkconfig --add slapd
chkconfig --level 345 slapd on
/etc/init.d/slapd start
```
授权
```
chown -R ldap.ldap /var/lib/ldap/
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091514418.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**2)ldapmodify执行出错**
[
](https://img-blog.csdnimg.cn/20190404091514418.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
[yuhui@hadoop11 ldapshell]\# ldapmodify -Y EXTERNAL -H ldapi:/// -f m1.ldif
SASL/EXTERNAL authentication started
SASL username: gidNumber=0+uidNumber=0,cn=peercred,cn=external,cn=auth
SASL SSF: 0
modifying entry "olcDatabase={2}bdb,cn=config"
ldap_modify: Inappropriate matching (18)
	additional info: modify/add: olcRootPW: no equality matching rule
```
[
](https://img-blog.csdnimg.cn/20190404091514418.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)解决办法：
修改modify.ldif中对应选项的"add"为"replace"即可
**3)Ldap和hive、impala集成错误**
[https://www.cloudera.com/documentation/enterprise/latest/topics/cdh_sg_hiveserver2_security.html\#concept_vxf_pgx_nm](https://www.cloudera.com/documentation/enterprise/latest/topics/cdh_sg_hiveserver2_security.html#concept_vxf_pgx_nm)
```
User 'hue' is not authorized to delegate to ‘hue’
```
解决：
```
hadoop.proxyuser.hue.hosts
hadoop.proxyuser.hue.groups
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091626703.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**4)Hue连接hive和impala错误**
[
](https://img-blog.csdnimg.cn/20190404091626703.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[https://stackoverflow.com/questions/51854958/user-impersonation-for-access-to-hive](https://stackoverflow.com/questions/51854958/user-impersonation-for-access-to-hive)
[http://cxy7.com/articles/2018/07/17/1531827540376.html](http://cxy7.com/articles/2018/07/17/1531827540376.html)
只配置了hive和ladp。hue出现错误
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091719802.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040409172693.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091732853.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
TSocket read 0 bytes
TSocket read 0 bytes (code THRIFTTRANSPORT): TTransportException('TSocket read 0 bytes',)
Bad status: 3 (Error validating the login)
Bad status: 3 (Error validating the login) (code THRIFTTRANSPORT): TTransportException('Bad status: 3 (Error validating the login)',)
```
```
10:10:25 上午	WARNING	thrift_util	Out of retries for thrift call: GetSchemas
10:10:25 上午	INFO	thrift_util	Thrift saw a transport exception: TSocket read 0 bytes
```
```
Failed to validate proxy privilege of ldapadmin for yuhui1
 
Bad status for request TOpenSessionReq(username=u'yuhui1', password=None, client_protocol=6, configuration={'hive.server2.proxy.user': u'yuhui1'}): TOpenSessionResp(status=TStatus(errorCode=0, errorMessage='Failed to validate proxy privilege of ldapadmin for yuhui1', sqlState='08S01', infoMessages=['*org.apache.hive.service.cli.HiveSQLException:Failed to validate proxy privilege of ldapadmin for yuhui1:14:13', 'org.apache.hive.service.auth.HiveAuthFactory:verifyProxyAccess:HiveAuthFactory.java:326', 'org.apache.hive.service.cli.thrift.ThriftCLIService:getProxyUser:ThriftCLIService.java:753', 'org.apache.hive.service.cli.thrift.ThriftCLIService:getUserName:ThriftCLIService.java:384', 'org.apache.hive.service.cli.thrift.ThriftCLIService:getSessionHandle:ThriftCLIService.java:411', 'org.apache.hive.service.cli.thrift.ThriftCLIService:OpenSession:ThriftCLIService.java:316', 'org.apache.hive.service.cli.thrift.TCLIService\$Processor\$OpenSession:getResult:TCLIService.java:1253', 'org.apache.hive.service.cli.thrift.TCLIService\$Processor\$OpenSession:getResult:TCLIService.java:1238', 'org.apache.thrift.ProcessFunction:process:ProcessFunction.java:39', 'org.apache.thrift.TBaseProcessor:process:TBaseProcessor.java:39', 'org.apache.hive.service.auth.TSetIpAddressProcessor:process:TSetIpAddressProcessor.java:56', 'org.apache.thrift.server.TThreadPoolServer\$WorkerProcess:run:TThreadPoolServer.java:286', 'java.util.concurrent.ThreadPoolExecutor:runWorker:ThreadPoolExecutor.java:1142', 'java.util.concurrent.ThreadPoolExecutor\$Worker:run:ThreadPoolExecutor.java:617', 'java.lang.Thread:run:Thread.java:745', '*org.apache.hadoop.security.authorize.AuthorizationException:User: ldapadmin is not allowed to impersonate yuhui1:0:-1'], statusCode=3), sessionHandle=None, configuration=None, serverProtocolVersion=6)
```
解决：hue中修改如下
hue_safety_valve_server.ini 的 Hue Server 高级配置代码段（安全阀）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091819597.png)
```
[impala]
server_host=hadoop11
server_interface=hiveserver2
server_port=21050
query_timeout_s=100
impersonation_enabled=True
auth_username=hue
auth_password=hue
[beeswax]
close_queries=True
use_sasl=False
auth_username=hue
auth_password=hue
```
**5)Hue添加用户**
Could not get LDAP details for users in pattern yuhui1.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404091951104.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
Could not get LDAP details for users with pattern 123
Failed to find LDAP user: Distinguished Name provided does not contain configured Base DN. Base DN: ou=people,dc=silentwolfyh,dc=com, DN: bjjizhou
Could not get LDAP details for users with pattern bjjizhou
```
解决：只能添加ldap中的用户
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092029923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**6)Hue用户无法同步**
[https://community.cloudera.com/t5/Web-UI-Hue-Beeswax/Failed-to-validate-proxy-privilege-of-hue-hive-for/td-p/49686](https://community.cloudera.com/t5/Web-UI-Hue-Beeswax/Failed-to-validate-proxy-privilege-of-hue-hive-for/td-p/49686)
```
Could not create home directory at login for hue.
```
将Hue配置为所有其他用户和组的代理用户，这意味着它可以代表任何其他用户提交请求。添加核心的site.xml：
```
hadoop.proxyuser.hue.groups
hadoop.proxyuser.hue.hosts
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092111225.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[       ](https://img-blog.csdnimg.cn/20190404092111225.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)**7)Hue组无法同步**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092132730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[Could not get LDAP details for group pattern abc](https://img-blog.csdnimg.cn/20190404092132730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
同步组只能是ldap中的组，通过phpLDAPadmin界面去看
**8)HDFS错误**
```
Resetting dropped connection: YZ-25-33-141.h.chinabank.com.cn 50070
Potential detail: StandbyException: Operation category READ is not supported in state standby. Visit https://s.apache.org/sbnn-error (error 403)
```
解决：将141的hdfs变成active
**9)Impala错误**
```
LDAP authentication specified, but without TLS. Passwords would go over the network in the clear. Enable TLS with --ldap_tls or use an ldaps:// URI. To override this is non-production environments, specify --ldap_passwords_in_clear_ok
. Impalad exiting.
Wrote minidump to /var/log/impala-minidumps/impalad/e50eb300-de19-4486-380a6c8f-0b3aa37c.dmp
```
解决：
```
Impala 命令行参数高级配置代码段（安全阀）
--ldap_passwords_in_clear_ok=true
--auth_creds_ok_in_clear
--authorized_proxy_user_config=hue=*
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092249172.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092255555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
### 四、Hue元数据表说明
**1)auth_group 组名称表**
备注：hue创建组的时候表中新增组名称
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092532427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**2)auth_user 用户表**
备注：hue创建用户或者hue登录时候创建用户
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092546849.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[       ](https://img-blog.csdnimg.cn/20190404092546849.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)**3)auth_user_groups 用户和组对应表**
备注：用户登录的时候自动和组进行关联
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092609503.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[       ](https://img-blog.csdnimg.cn/20190404092609503.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)**4)useradmin_grouppermission hue组权限表**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092631713.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[       ](https://img-blog.csdnimg.cn/20190404092631713.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)**5)useradmin_huepermission  hue的权限表**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092653544.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[       ](https://img-blog.csdnimg.cn/20190404092653544.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)**6)useradmin_ldapgroup   ldap组和hue组关联表**
备注：ldap查询的组名称在此表中出现
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092718238.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[       ](https://img-blog.csdnimg.cn/20190404092718238.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)**7)useradmin_userprofile 用户hdfs路径表信息**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404092737637.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
### 五、参看其余文档
[https://blog.csdn.net/u014728303/article/details/53908412](https://blog.csdn.net/u014728303/article/details/53908412)
[https://blog.csdn.net/u011026329/article/details/79171996](https://blog.csdn.net/u011026329/article/details/79171996)
[https://blog.csdn.net/mnasd/article/details/84947690](https://blog.csdn.net/mnasd/article/details/84947690)
Ldap解释及理解
[https://blog.csdn.net/xiaowen_1990/article/details/79074949](https://blog.csdn.net/xiaowen_1990/article/details/79074949)
[https://blog.51cto.com/407711169/1439623](https://blog.51cto.com/407711169/1439623)
[https://www.cnblogs.com/yjd_hycf_space/p/7994597.html](https://www.cnblogs.com/yjd_hycf_space/p/7994597.html)
（LDAP的相关概念与objectClass介绍）
[https://blog.csdn.net/qq_27376871/article/details/52037317](https://blog.csdn.net/qq_27376871/article/details/52037317)

