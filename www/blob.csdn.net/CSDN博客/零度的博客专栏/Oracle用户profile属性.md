# Oracle用户profile属性 - 零度的博客专栏 - CSDN博客
2016年04月28日 10:15:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：847
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
--查看profile 内容 
select * from dba_profiles where profile='PF_EAGLE';
--查看用户的profiles 
select username,profile from dba_users;
--查看是否启用动态资源限制参数 
SHOW PARAMETER RESOURCE_LIMIT;
--启用限制 
ALTER SYSTEM SET RESOURCE_LIMIT=TRUE SCOPE=BOTH;
--创建profile 
CREATE PROFILE pf_eagle LIMIT 
  FAILED_LOGIN_ATTEMPTS 6 
   PASSWORD_LIFE_TIME 60 
   PASSWORD_REUSE_TIME 60 
   PASSWORD_REUSE_MAX 5 
   PASSWORD_VERIFY_FUNCTION verify_function 
   PASSWORD_LOCK_TIME 1/24 
   PASSWORD_GRACE_TIME 90; 
--修改用户默认的profile 
ALTER USER user_name PROFILE profile_name;
alter profile PROFILE_PERSONAL limit FAILED_LOGIN_ATTEMPTS UNLIMITED;
--修改profile属性 
alter profile DEFAULT limit PASSWORD_LIFE_TIME unlimited; 
ALTER PROFILE DEFAULT LIMIT COMPOSITE_LIMIT UNLIMITED; 
ALTER PROFILE DEFAULT LIMIT FAILED_LOGIN_ATTEMPTS UNLIMITED; 
ALTER PROFILE DEFAULT LIMIT PASSWORD_REUSE_TIME UNLIMITED; 
ALTER PROFILE DEFAULT LIMIT PASSWORD_GRACE_TIME UNLIMITED; 
ALTER PROFILE DEFAULT LIMIT PASSWORD_VERIFY_FUNCTION NULL; 
ALTER PROFILE DEFAULT LIMIT PASSWORD_LOCK_TIME UNLIMITED; 
ALTER PROFILE DEFAULT LIMIT FAILED_LOGIN_ATTEMPTS 10;
对PROFILE的标准语法说明如下： 
1）创建PROFILE的语法如下： 
CREATE PROFILE profile 
   LIMIT { resource_parameters 对资源的限制 
         | password_parameters 对密码的限制 
         }... 
;
2）对数据库资源做限制时，下面是全部可用的参数： 
{ { SESSIONS_PER_USER 每个用户名所允许的并行会话数 
  | CPU_PER_SESSION   一个会话一共可以使用的CPU时间，单位是百分之一秒 
  | CPU_PER_CALL      一次SQL调用（解析、执行和获取）允许使用的CPU时间 
  | CONNECT_TIME      限制会话连接时间，单位是分钟 
  | IDLE_TIME         允许空闲会话的时间，单位是分钟 
  | LOGICAL_READS_PER_SESSION 限制会话对数据块的读取，单位是块 
  | LOGICAL_READS_PER_CALL    限制SQL调用对数据块的读取，单位是块 
  | COMPOSITE_LIMIT   “组合打法” 
  } 
  { integer | UNLIMITED | DEFAULT } 
| PRIVATE_SGA   限制会话在SGA中Shared Pool中私有空间的分配 
  { size_clause | UNLIMITED | DEFAULT } 
}
3）对密码做限制时，下面是全部可用的参数： 
{ { FAILED_LOGIN_ATTEMPTS 帐户被锁定之前可以错误尝试的次数 
  | PASSWORD_LIFE_TIME    密码可以被使用的天数，单位是天，默认值180天 
  | PASSWORD_REUSE_TIME   密码可重用的间隔时间（结合PASSWORD_REUSE_MAX） 
  | PASSWORD_REUSE_MAX    密码的最大改变次数（结合PASSWORD_REUSE_TIME） 
  | PASSWORD_LOCK_TIME    超过错误尝试次数后，用户被锁定的天数，默认1天 
  | PASSWORD_GRACE_TIME   当密码过期之后还有多少天可以使用原密码 
  } 
  { expr | UNLIMITED | DEFAULT } 
| PASSWORD_VERIFY_FUNCTION 
  { function | NULL | DEFAULT } 
}
