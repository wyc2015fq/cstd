# hive实例讲解实现in和not in子句 - z69183787的专栏 - CSDN博客
2016年10月26日 10:22:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3152
目前hive不支持 in或not in 中包含查询子句的语法，所以只能通过left join实现。
假设有一个登陆表login(当天登陆记录,只有一个uid),和一个用户注册表regusers(当天注册用户，字段只有一个uid)，这两个表都包含一个字段,uid。
## **in查询**
如果要查询当天登陆的注册用户，需要用in查询，hive sql如下:
```
select login.uid from login left outer join regusers on login.uid=regusers.uid where regusers.uid is not null
```
如果login表和regusers表按天分区，字段是dt，那么查询2013年1月1号当天登陆的注册用户，hive sql如下:
```
select login.uid from login day_login left outer join 
   (select uid from regusers where dt='20130101') day_regusers
on day_login.uid=day_regusers.uid where day_login.dt='20130101' and day_regusers.uid is not null
```
## **not in查询**
如果要查询当天登陆的老用户(这里假设非当天注册用户就是老用户)，需要用not in查询，hive sql如下:
```
select login.uid from login left outer join regusers on login.uid=regusers.uid where regusers.uid is null;
```
如果login表和regusers表按天分区，字段是dt，那么查询2013年1月1号当天登陆的老用户，hive sql如下:
```
select login.uid from login day_login left outer join 
   (select uid from regusers where dt='20130101') day_regusers
on day_login.uid=day_regusers.uid where day_login.dt='20130101' and day_regusers.uid is null;
```
