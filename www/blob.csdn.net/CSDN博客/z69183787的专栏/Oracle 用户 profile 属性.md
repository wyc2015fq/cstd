# Oracle 用户 profile 属性 - z69183787的专栏 - CSDN博客
2014年04月12日 20:38:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：782
个人分类：[Oracle](https://blog.csdn.net/z69183787/article/category/2175483)
   在之前的Blog里，讲了Oracle 用户和角色的一些概念，但是没有讲属性profile的信息。
       Oracle 用户及角色介绍
[http://blog.csdn.net/tianlesoftware/archive/2009/11/08/4786956.aspx](http://blog.csdn.net/tianlesoftware/archive/2009/11/08/4786956.aspx)
       在Profile文件里保存了一些资源的限制信息。如果在创建用户时指定了profile。 那么用户就不能超过profile里指定的资源。
1. 一些注意事项
       （1）创建profile 需要CREATE PROFILE 的权限。
       （2）当为用户指定profile资源限制时，需要启用动态资源限制参数：RESOURCE_LIMIT。该参数默认是False.
       ALTER SYSTEM SET RESOURCE_LIMIT=TRUE SCOPE=BOTH;
       当该参数设置为True时，profile 中的参数生效。 当设置为False，Profile中一些参数设置不能生效。
注意，该参数不对密码资源受限制。密码资源限制永远是生效的。
2. 创建自己的Profile：
create profile "profile_personal" limit
composite_limit unlimited
sessions_per_user unlimited
cpu_per_session unlimited
cpu_per_call unlimited
logical_reads_per_session unlimited
logical_reads_per_call unlimited
idle_time unlimited
connect_time unlimited
private_sga unlimited
failed_login_attempts unlimited
password_life_time 7776000/86400
password_reuse_time 86400/86400
password_reuse_max 5
password_lock_time unlimited
password_grace_time unlimited ;
验证：
SQL> select * from dba_profiles where profile='PROFILE_PERSONAL';
PROFILE                        RESOURCE_NAME                    RESOURCE LIMIT
------------------------------ -------------------------------- -------- -------
PROFILE_PERSONAL               COMPOSITE_LIMIT                  KERNEL   UNLIMIT
PROFILE_PERSONAL               SESSIONS_PER_USER                KERNEL   UNLIMIT
PROFILE_PERSONAL               CPU_PER_SESSION                  KERNEL   UNLIMIT
PROFILE_PERSONAL               CPU_PER_CALL                     KERNEL   UNLIMIT
PROFILE_PERSONAL               LOGICAL_READS_PER_SESSION        KERNEL   UNLIMIT
PROFILE_PERSONAL               LOGICAL_READS_PER_CALL           KERNEL   UNLIMIT
PROFILE_PERSONAL               IDLE_TIME                        KERNEL   UNLIMIT
PROFILE_PERSONAL               CONNECT_TIME                     KERNEL   UNLIMIT
PROFILE_PERSONAL               PRIVATE_SGA                      KERNEL   UNLIMIT
PROFILE_PERSONAL               FAILED_LOGIN_ATTEMPTS            PASSWORD UNLIMIT
PROFILE_PERSONAL               PASSWORD_LIFE_TIME               PASSWORD 90
PROFILE                        RESOURCE_NAME                    RESOURCE LIMIT
------------------------------ -------------------------------- -------- -------
PROFILE_PERSONAL               PASSWORD_REUSE_TIME              PASSWORD 1
PROFILE_PERSONAL               PASSWORD_REUSE_MAX               PASSWORD 5
PROFILE_PERSONAL               PASSWORD_VERIFY_FUNCTION         PASSWORD DEFAULT
PROFILE_PERSONAL               PASSWORD_LOCK_TIME               PASSWORD UNLIMIT
PROFILE_PERSONAL               PASSWORD_GRACE_TIME              PASSWORD UNLIMIT
16 rows selected.
SQL>
3. 在创建用户时指定Profile：
SQL> create user dave identified by dave default tablespace users temporary tablespace temp profile profile_personal;
验证：
SQL> select username,profile from dba_users where username='DAVE';
USERNAME                       PROFILE
------------------------------ ------------------------------
DAVE                           PROFILE_PERSONAL
4.  修改用户的Profile
SQL>ALTER USER dave PROFILE default;
验证：
SQL> select username,profile from dba_users where username='DAVE';
USERNAME                       PROFILE
------------------------------ ------------------------------
DAVE                           DEFAULT
5. 修改Profile 属性
       我们可以通过dba_profiles 查询profile 信息，但是不能通过该表进行修改。 修改profile 需要如下命令：
SQL>alter profile PROFILE_PERSONAL limit FAILED_LOGIN_ATTEMPTS UNLIMITED;
       在这里举FAILED_LOGIN_ATTEMPTS这个为例，是因为这个参数与我们DB 密切关心。 因为这个参数默认是10. 即当我们用户连续10次输入错误密码，这个用户就会被锁住。
       所以一般在生产环境下，会根据具体情况设置这个参数，如果防止用户被锁，则将这个参数设置为UNLIMITED。 这个是注意的地方。
      用户连词失败次数是在表USER$ 中的lcount字段记录的。 该值默认为0. 当失败一次，该值加1. 成功登录，该值清零。
SQL> select name,lcount from user$ where name='SYSTEM';
NAME                               LCOUNT
------------------------------ ----------
SYSTEM                                  0
SQL> conn system/1;
ERROR:
ORA-01017: invalid username/password; logon denied
Warning: You are no longer connected to ORACLE.
SQL> conn system/2;
ERROR:
ORA-01017: invalid username/password; logon denied
SQL> conn system/3;
ERROR:
ORA-01017: invalid username/password; logon denied
SQL> conn system/4;
ERROR:
ORA-01017: invalid username/password; logon denied
SQL> conn / as sysdba;
Connected.
SQL> select name,lcount from user$ where name='SYSTEM';
NAME                               LCOUNT
------------------------------ ----------
SYSTEM                                  4
SQL> conn system/oracle;
Connected.
SQL> conn / as sysdba;
Connected.
SQL> select name,lcount from user$ where name='SYSTEM';
NAME                               LCOUNT
------------------------------ ----------
SYSTEM                                  0
---------------------------------------------------------------------------------------------------
Blog： http://blog.csdn.net/tianlesoftware
网上资源： http://tianlesoftware.download.csdn.net
相关视频：http://blog.csdn.net/tianlesoftware/archive/2009/11/27/4886500.aspx
DBA1 群：62697716(满); DBA2 群：62697977(满) DBA3 群：62697850(满)  
DBA 超级群：63306533(满); DBA4 群： 83829929 DBA5群： 142216823
聊天 群：40132017
--加群需要在备注说明Oracle表空间和数据文件的关系，否则拒绝申请
