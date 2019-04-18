# 使用Oracle的Job+Procedure实现数据的定时迁移备份 - z69183787的专栏 - CSDN博客
2014年12月16日 10:23:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2512
                
--创建数据迁移的日志表，记录自动数据迁移的执行时间
create table mc_movedata_log(
id varchar2(50) default sys_guid(),
tb_name varchar2(50) not null,
last_op_time date null);
以下是对mc_msg_store表进行每天（凌晨4：30）轮询数据迁移的步骤。
1.--创建存储过程
create or replace procedure movedata_mc_msg_store
as 
v_cur_time varchar2(19);
 begin
  select to_char(sysdate,'yyyy-MM-dd HH24:mi:ss') into v_cur_time from dual;
  insert /*+ append */ into mc_msg_store_history select * from mc_msg_store where create_time<to_date(v_cur_time,'yyyy-MM-dd HH24:mi:ss')-31;
  delete from mc_msg_store t where create_time<to_date(v_cur_time,'yyyy-MM-dd HH24:mi:ss')-31;   
  insert into mc_movedata_log(tb_name ,last_op_time) values('mc_msg_store', to_date(v_cur_time,'yyyy-MM-dd HH24:mi:ss'));
  commit;
end movedata_mc_msg_store;
--测试存储过程
begin
  -- Call the procedure
  movedata_mc_msg_store;
end;
--将存储过程交给job进行轮询调用，job1是任务编号，由dbms_job.submit输出赋值
declare 
 job1 number;
 begin 
dbms_job.submit(job1,'movedata_mc_msg_store;',sysdate,'trunc(sysdate)+1+(4*60+30)/(24*60)');
 end;
以下是对mc_msg_log表进行每天（凌晨4：30）轮询数据迁移的步骤。
2.--创建存储过程 movedata_mc_msg_log
create or replace procedure movedata_mc_msg_log
as 
v_cur_time varchar2(19);
 begin
  select to_char(sysdate,'yyyy-MM-dd HH24:mi:ss') into v_cur_time from dual;
  insert /*+ append */ into mc_msg_log_history select * from mc_msg_log where operate_time<to_date(v_cur_time,'yyyy-MM-dd HH24:mi:ss')-31;
  delete from mc_msg_log t where operate_time<to_date(v_cur_time,'yyyy-MM-dd HH24:mi:ss')-31;   
  insert into mc_movedata_log(tb_name ,last_op_time) values('mc_msg_log', to_date(v_cur_time,'yyyy-MM-dd HH24:mi:ss'));
  commit;
end movedata_mc_msg_log;
--测试存储过程
begin
  -- Call the procedure
  movedata_mc_msg_log;
end;
--将存储过程交给job进行轮询调用，job2是任务编号，由dbms_job.submit输出赋值
declare 
 job2 number;
 begin 
dbms_job.submit(job2,'movedata_mc_msg_log;',sysdate,'trunc(sysdate)+1+(4*60+30)/(24*60)');
 end;
