# Oracle实现定时执行存储过程 - 零度的博客专栏 - CSDN博客
2016年04月27日 16:29:54[零度anngle](https://me.csdn.net/zmx729618)阅读数：8922
**一：简单测试job的创建过程案例： **
**1，先创建一张JOB_TEST表，字段为a 日期格式**
SQL> create table JOB_TEST(a date);
Table created
SQL> commit;
Commit complete
**2，创建一个存储过程 bb 作用是往pig表中插入数据**
SQL> create or replace procedure JOB_PRO_TEST as 
  2  begin
  3  insert into JOB_TEST values(sysdate);
  4  end;
  5  /
Procedure created
**3，创建一个job，名称为job2010 ；作用是每分钟（60×24=1440）执行一次存储过程JOB_PRO_TEST。**
declare 
  job2010 number;
  begin 
 dbms_job.submit(job2010,'JOB_PRO_TEST;',sysdate,'sysdate+1/1440');
  end;
注意：这里系统自动生成job id 为41 
PL/SQL procedure successfully completed
job2010
---------
41
**4，运行job2010**
SQL> begin
  2  dbms_job.run(:job2010);
  3  end;
  4  /
PL/SQL procedure successfully completed
job2010
---------
41
**5，删除job2010**
SQL> begin
  2  dbms_job.remove(:job2010);
  3  end;
  4  /
**6，查询jobs的相关视图**
select job,last_date,last_sec,broken,failures,interval, what from dba_jobs
详解 dba_jobs中几个比较重要的字段 
job: 指的是job的id号。比如上面的 41 
failures：job执行的时候失败次数，如果超过了15次，那么broken列将被标为Y，以后就不会运行该job了 
broken：默认为N，如果为Y，意味着不再执行该job！
interval：执行job的间隔时间。
what：该job的实际工作。
**二：job 的相关知识：**
**1，DBA_JOBS**
DBA_JOBS
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
字段（列） 类型 描述
JOB NUMBER 任务的唯一标示号
LOG_USER VARCHAR2(30) 提交任务的用户
PRIV_USER VARCHAR2(30) 赋予任务权限的用户
SCHEMA_USER VARCHAR2(30) 对任务作语法分析的用户模式
LAST_DATE DATE 最后一次成功运行任务的时间
LAST_SEC VARCHAR2(8) 如HH24:MM:SS格式的last_date日期的小时，分钟和秒
THIS_DATE DATE 正在运行任务的开始时间，如果没有运行任务则为null
THIS_SEC VARCHAR2(8) 如HH24:MM:SS格式的this_date日期的小时，分钟和秒
NEXT_DATE DATE 下一次定时运行任务的时间
NEXT_SEC VARCHAR2(8) 如HH24:MM:SS格式的next_date日期的小时，分钟和秒
TOTAL_TIME NUMBER 该任务运行所需要的总时间，单位为秒
BROKEN VARCHAR2(1) 标志参数，Y标示任务中断，以后不会运行
INTERVAL VARCHAR2(200) 用于计算下一运行时间的表达式
FAILURES NUMBER 任务运行连续没有成功的次数
WHAT VARCHAR2(2000) 执行任务的PL/SQL块
CURRENT_SESSION_LABEL RAW MLSLABEL 该任务的信任Oracle会话符
CLEARANCE_HI RAW MLSLABEL 该任务可信任的Oracle最大间隙
CLEARANCE_LO RAW MLSLABEL 该任务可信任的Oracle最小间隙
NLS_ENV VARCHAR2(2000) 任务运行的NLS会话设置
MISC_ENV RAW(32) 任务运行的其他一些会话参数
－－－－－－－－－－－－－－－－－－－－－－－－－－
**2，描述 INTERVAL参数值**
每天午夜12点: 'TRUNC(SYSDATE + 1)'
每天早上8点30分: 'TRUNC(SYSDATE + 1) + （8*60+30）/(24*60)'
每星期二中午12点: 'NEXT_DAY(TRUNC(SYSDATE ), ''TUESDAY'' ) + 12/24'
每个月第一天的午夜12点: 'TRUNC(LAST_DAY(SYSDATE ) + 1)'
每个季度最后一天的晚上11点: 'TRUNC(ADD_MONTHS(SYSDATE + 2/24, 3 ), 'Q' ) -1/24'
每星期六和日早上6点10分: 'TRUNC(LEAST(NEXT_DAY(SYSDATE, ''SATURDAY"), NEXT_DAY(SYSDATE, "SUNDAY"))) + （6×60+10）/（24×60）'
每月25号00：00执行: 'TRUNC(LAST_DAY(SYSDATE ) + 25)'
－－－－－－－－－－－－－－－－－－－－－－－－－－
1:每分钟执行
Interval => TRUNC(sysdate,'mi') + 1/ (24*60)
或
Interval => sysdate+1/1440
2:每天定时执行
例如：每天的凌晨1点执行
Interval => TRUNC(sysdate) + 1 +1/ (24)
3:每周定时执行
例如：每周一凌晨1点执行
Interval => TRUNC(next_day(sysdate,'星期一'))+1/24
4:每月定时执行
例如：每月1日凌晨1点执行
Interval =>TRUNC(LAST_DAY(SYSDATE))+1+1/24
5:每季度定时执行
例如每季度的第一天凌晨1点执行
Interval => TRUNC(ADD_MONTHS(SYSDATE,3),'Q') + 1/24
6:每半年定时执行
例如：每年7月1日和1月1日凌晨1点
Interval => ADD_MONTHS(trunc(sysdate,'yyyy'),6)+1/24
7:每年定时执行
例如：每年1月1日凌晨1点执行
Interval =>ADD_MONTHS(trunc(sysdate,'yyyy'),12)+1/24
**3. 查看相关job信息**
**相关视图**
dba_jobs
all_jobs
user_jobs
dba_jobs_running 包含正在运行job相关信息
三：实际应用
**步骤一：创建一个job，每月25日执行下面sql。**
SQL> variable job2010 number;
SQL> begin
SQL> dbms_job.submit(:job2010,'delete from i0216_inv_balance_curstock where trunc(sysdate-mtime) > 90and 
ib_qty = 0;commit;',sysdate,'TRUNC(LAST_DAY(SYSDATE ) + 25)');   <==每月25号00：00执行
SQL> end;
SQL> /
**步骤二：查询生成的job id**
SQL> select job, what from dba_jobs;
job          what
61           ..........
**步骤三：运行刚才创建的job**
SQL> begin
SQL> dbms_job.run(61);
SQL> end;
SQL> /
**步骤四：查询该job下次执行的时间**
SQL> select job,next_date,what from dba_jobs 
SQL> where job=’61’;
**步骤五：删除该job**
SQL> begin
SQL> dbms_job.remove(61);
SQL> end;
SQL> /
