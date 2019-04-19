# oracle job - 左直拳的马桶_日用桶 - CSDN博客
2017年05月31日 14:09:37[左直拳](https://me.csdn.net/leftfist)阅读数：338
oracle的job（作业）咋整？
**1、创建JOB**
1）创建一个存储过程。逻辑处理都在这个存储过程里面。 
2）创建一个JOB运行此存储过程
```
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
-- 有个前提，就是当前用户要有执行存储过程的权限
begin
   pro_job_print;   
end;
-- 创建job
declare      
   job1 number;
begin
   dbms_job.submit(job1, 'pro_job_print;', sysdate, 'sysdate+10/86400');--每10秒插入一条记录
end;
```
**JOB的执行频率：**
```
描述                              INTERVAL参数值 
每天午夜12点                 TRUNC(SYSDATE + 1) 
每天早上8点30分             TRUNC(SYSDATE + 1) + （8*60+30）/(24*60) 
每星期二中午12点            NEXT_DAY(TRUNC(SYSDATE ), ''TUESDAY'' ) + 12/24 
每个月第一天的午夜12点        TRUNC(LAST_DAY(SYSDATE ) + 1) 
每个季度最后一天的晚上11点     TRUNC(ADD_MONTHS(SYSDATE + 2/24, 3 ), 'Q' ) -1/24 
每星期六和日早上6点10分        TRUNC(LEAST(NEXT_DAY(SYSDATE, ''SATURDAY"), NEXT_DAY(SYSDATE, "SUNDAY"))) + （6×60+10）/（24×60）
每秒钟执行次
Interval => sysdate + 1/(24 * 60 * 60)
如果改成sysdate + 10/(24 * 60 * 60)就是10秒钟执行次
每分钟执行 
Interval => TRUNC(sysdate,'mi') + 1/ (24*60)
如果改成TRUNC(sysdate,'mi') + 10/ (24*60) 就是每10分钟执行次
每天定时执行 
例如：每天的凌晨1点执行 
Interval => TRUNC(sysdate) + 1 +1/ (24)
每周定时执行 
例如：每周一凌晨1点执行 
Interval => TRUNC(next_day(sysdate,'星期一'))+1/24
每月定时执行 
例如：每月1日凌晨1点执行 
Interval =>TRUNC(LAST_DAY(SYSDATE))+1+1/24
每季度定时执行 
例如每季度的第一天凌晨1点执行 
Interval => TRUNC(ADD_MONTHS(SYSDATE,3),'Q') + 1/24
每半年定时执行 
例如：每年7月1日和1月1日凌晨1点 
Interval => ADD_MONTHS(trunc(sysdate,'yyyy'),6)+1/24
每年定时执行 
例如：每年1月1日凌晨1点执行 
Interval =>ADD_MONTHS(trunc(sysdate,'yyyy'),12)+1/24
```
**2、停止JOB**
```
begin
       dbms_job.broken(1,TRUE );//1是job ID，可通过以下语句查找
       /*
        select * from dba_jobs;
        select * from all_jobs;
        select * from user_jobs;
       */
       commit;
end;
```
**3、开启JOB**
```
begin
       dbms_job.broken(1,false );
       commit;
end;
```
参考文章： 
[Oracle job procedure 存储过程定时任务](http://www.cnblogs.com/hoojo/p/oracle_procedure_job_interval.html)
