# oracle的JOB - 左直拳的马桶_日用桶 - CSDN博客
2017年07月31日 19:43:58[左直拳](https://me.csdn.net/leftfist)阅读数：522
前文再续，上面说的oracle的JOB，近期有些体会，记录一下：
**零、创建JOB**
```
创建JOB 
1）创建一个存储过程。逻辑处理都在这个存储过程里面。 
2）创建一个JOB运行此存储过程
-- 创建table
create table tab_time(
       current_time timestamp       
);
-- 创建存储过程
create or replace procedure pro_job_print
as
   begin
       --dbms_output.put_line('系统时间：' || to_char(sysdate, 'dd-mm-yyyy hh24:mi:ss'));
       insert into tab_time values(sysdate);
       commit;
   end;
-- 调用过程测试   
begin
   pro_job_print;   
end;
-- 创建job
declare      
   job1 number;
begin
   dbms_job.submit(job1, 'pro_job_print;', sysdate, 'sysdate+10/86400');--每10秒插入一条记录
   commit;
end;
```
注意，创建好的JOB，位于dbms_job下
**一、JOB的开启与关闭**
```
--关闭JOB
begin
  dbms_job.broken(41,true);
  commit;
end;
--开启JOB
begin
  dbms_job.broken(41,false);
  commit;
end;
```
**二、JOB的运行**
最常见的问题：
```
假如某JOB的频率是每5分钟运行一次，但它每次运行30分钟，那么5分钟后，会不会有多个JOB（线程）在一起运行？
答：不会。该JOB永远只有一个实例在运行。它每次运行前，会先看看前面有没有在运行，如果没有，则正常运行；有的话，则等待，直到前面那个运行完了之后，紧接着2秒之内启动。
```
**三、JOB的停止**
```
--首先要关闭JOB计划
begin
  dbms_job.broken(JOB编号,true);
  commit;
end;
--检查JOB有没有正在运行，停止计划对它没有影响
--获得SID
select * from dba_jobs_running;
--获得SID,serial#
select sid,serial#,lockwait,status,wait_time,state from v$session where sid='SID';
--杀掉进程
alter system kill session 'SID,serial#';
```
