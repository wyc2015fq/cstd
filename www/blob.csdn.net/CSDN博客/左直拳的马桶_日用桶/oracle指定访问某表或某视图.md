# oracle指定访问某表或某视图 - 左直拳的马桶_日用桶 - CSDN博客
2017年04月21日 16:23:29[左直拳](https://me.csdn.net/leftfist)阅读数：446
在oracle中，想创建一个账号，然后只能只读地访问指定的表，怎么搞？
一、为特定的表创建视图 
创建视图的时候还可以加上过滤条件，连访问哪些数据都可以指定。
```
create or replace view bigboss.v_User as
    select * from bigboss.user
        where u_groupid in (select pm_unitid from bigboss.group where qh like '46%');
create or replace view bigboss.v_Group as
    select * from bigboss.group where qh like '46%';
```
二、创建账号 
并赋权限。权限小小的，够用就行。
```
create user sso identified by 123456;
grant connect to sso;
grant select on bigboss.v_user to sso;
grant select on bigboss.v_group to sso;
grant create synonym  to sso;
```
三、新账号小试牛刀 
在新账号下，
```
create or replace synonym v_user for bigboss.v_user;
create or replace synonym v_group for bigboss.v_group;
```
如此，新账号sso即可愉快无碍地访问那俩视图了：
```
select * from v_user;
select * from v_group;
```
参考资料： 
[http://www.cnblogs.com/BetterWF/archive/2012/07/03/2574416.html](http://www.cnblogs.com/BetterWF/archive/2012/07/03/2574416.html)
