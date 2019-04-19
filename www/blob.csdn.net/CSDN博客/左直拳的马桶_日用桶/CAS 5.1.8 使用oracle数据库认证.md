# CAS 5.1.8 使用oracle数据库认证 - 左直拳的马桶_日用桶 - CSDN博客
2018年02月28日 19:36:29[左直拳](https://me.csdn.net/leftfist)阅读数：713
我们的账号是用md5加密的，没啥特别，所以对于我们这种情况，修改CAS的配置文件就行了。
配置文件是指：application.properties
`（源码：webapp/resources/application.properties，部署之后是/WEB-INF/classes/application.properties）`
```
#注释默认账号
#cas.authn.accept.users=casuser::Mellon 
#添加以下配置
cas.authn.jdbc.query[0].sql=SELECT lower(u_password) as psw FROM org_user WHERE u_loginname=?
cas.authn.jdbc.query[0].fieldPassword=psw #指明密码字段名称
cas.authn.jdbc.query[0].healthQuery=SELECT 1 from dual
cas.authn.jdbc.query[0].url=jdbc:oracle:thin:@192.168.0.22:1521/pdbhnjczs #oracle数据库版本为12C，pdbhnjczs为PDB，所以连接串写为"IP:端口/PDB"的格式，而不是"IP:端口:DB"
cas.authn.jdbc.query[0].user=数据库登录账号
cas.authn.jdbc.query[0].password=数据库登录密码
cas.authn.jdbc.query[0].dialect=org.hibernate.dialect.Oracle10gDialect
cas.authn.jdbc.query[0].driverClass=oracle.jdbc.OracleDriver
#密码策略
#这个策略，就是将我们提交的密码进行的处理方式；处理过后再跟数据库中的密码比较
cas.authn.jdbc.query[0].passwordEncoder.type=DEFAULT
cas.authn.jdbc.query[0].passwordEncoder.characterEncoding=UTF-8
cas.authn.jdbc.query[0].passwordEncoder.encodingAlgorithm=MD5
cas.authn.jdbc.query[0].passwordEncoder.strength=32 #MD5加密长度分为16位，32位
cas.authn.jdbc.query[0].isolateInternalQueries=false
cas.authn.jdbc.query[0].failFast=true
cas.authn.jdbc.query[0].isolationLevelName=ISOLATION_READ_COMMITTED
cas.authn.jdbc.query[0].leakThreshold=10
cas.authn.jdbc.query[0].propagationBehaviorName=PROPAGATION_REQUIRED
cas.authn.jdbc.query[0].batchSize=1
cas.authn.jdbc.query[0].ddlAuto=create-drop
cas.authn.jdbc.query[0].maxAgeDays=180
cas.authn.jdbc.query[0].autocommit=false
cas.authn.jdbc.query[0].idleTimeout=5000
```
如此即可。
2018.06.27 
其实这样子的话还不够。因为webapp还没有相应的jdbc库支持。必须引入两个模块（module）：
```
cas-server-support-jdbc
cas-server-support-jdbc-authentication
```
引入办法： 
修改webapp/gradle/webapp.gradle，加入：
```
compile project(":support:cas-server-support-jdbc")
compile project(":support:cas-server-support-jdbc-authentication")
```
