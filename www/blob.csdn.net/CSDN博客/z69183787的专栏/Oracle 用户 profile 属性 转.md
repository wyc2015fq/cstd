# Oracle 用户 profile 属性 转 - z69183787的专栏 - CSDN博客
2014年04月12日 20:41:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7249
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
