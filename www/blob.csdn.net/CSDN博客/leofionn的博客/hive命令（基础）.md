
# hive命令（基础） - leofionn的博客 - CSDN博客


2018年01月26日 19:29:59[leofionn](https://me.csdn.net/qq_36142114)阅读数：65


1.hive模糊搜索表
show tables like '*name*';
2.查看表结构信息
desc formatted table_name;
desc table_name;
3.查看分区信息
show partitions table_name;
4.根据分区查询数据
select table_coulm from table_name where partition_name = '2018-01-25';
5.查看hdfs文件信息
dfs -ls /user/hive/zld/table02;
6.从文件加载数据进表(OVERWRITE覆盖,追加不需要OVERWRITE关键字)
LOAD DATA LOCAL INPATH 'dim_csl_rule_config.txt' OVERWRITE into table dim.dim_csl_rule_config;
--从查询语句给table插入数据
INSERT OVERWRITE TABLE test_h02_click_log PARTITION(dt) select *
from stage.s_h02_click_log where dt='2018-01-25' limit 100;
7.导出数据到文件
insert overwrite directory '/tmp/csl_rule_cfg' select a.* from dim.dim_csl_rule_config a;
hive -e "select day_id,pv,uv,ip_count,click_next_count,second_bounce_rate,return_visit,pg_type from tmp.tmp_h02_click_log_baitiao_ag_sum where day_id in ('2018-01-25');"> /home/jrjt/testan/baitiao.dat;
8.自定义udf函数
1.继承UDF类
2.重写evaluate方法
3.把项目打成jar包
4.hive中执行命令add jar /home/jrjt/dwetl/PUB/UDF/udf/GetProperty.jar;
5.创建函数create temporary function get_pro as 'jd.Get_Property'//jd.jd.Get_Property为类路径;
9.查询显示列名 及 行转列显示
set hive.cli.print.header=true; // 打印列名
set hive.cli.print.row.to.vertical=true; // 开启行转列功能, 前提必须开启打印列名功能
set hive.cli.print.row.to.vertical.num=1; // 设置每行显示的列数
10.查看表文件大小,下载文件到某个目录,显示多少行到某个文件
dfs -du hdfs://BJYZH3-HD-JRJT-4137.jd.com:54310/user/jrjt/warehouse/stage.db/s_h02_click_log;
dfs -get /user/jrjt/warehouse/ods.db/o_h02_click_log_i_new/dt=2018-01-25/000212_0 /home/jrjt/testan/;
head -n 1000 文件名 > 文件名
11.杀死某个任务  不在hive shell中执行
hadoop job -kill job_201403041453_58315
12.hive-wui路径
http://192.168.107.1/jobtracker.jsp
13.删除分区
alter table tmp_h02_click_log_baitiao drop partition(dt='2018-01-25');
alter table d_h02_click_log_basic_d_fact drop partition(dt='2018-01-25');

14.hive命令行操作
执行一个查询,在终端上显示mapreduce的进度，执行完毕后，最后把查询结果输出到终端上，接着hive进程退出，不会进入交互模式。
hive -e 'select table_cloum from table'
-S，终端上的输出不会有mapreduce的进度，执行完毕，只会把查询结果输出到终端上。这个静音模式很实用，,通过第三方程序调用，第三方程序通过hive的标准输出获取结果集。
hive -S -e 'select table_cloum from table'
执行sql文件
hive -f hive_sql.sql
15.hive上操作hadoop文件基本命令
查看文件大小
dfs -du /user/jrjt/warehouse/tmp.db/tmp_h02_click_log/dt=2018-01-25;
删除文件
dfs -rm /user/jrjt/warehouse/tmp.db/tmp_h02_click_log/dt=2018-01-25;
16.插入数据sql、导出数据sql
1.insert 语法格式为：
基本的插入语法：
INSERT OVERWRITE TABLE tablename [PARTITON(partcol1=val1,partclo2=val2)]select_statement FROM from_statement
insert overwrite table test_insert select * from test_table;
对多个表进行插入操作：
FROM fromstatte
INSERT OVERWRITE TABLE tablename1 [PARTITON(partcol1=val1,partclo2=val2)]select_statement1
INSERT OVERWRITE TABLE tablename2 [PARTITON(partcol1=val1,partclo2=val2)]select_statement2
from test_table
insert overwrite table test_insert1
select key
insert overwrite table test_insert2
select value;
insert的时候，from子句即可以放在select 子句后面，也可以放在 insert子句前面。
hive不支持用insert语句一条一条的进行插入操作，也不支持update操作。数据是以load的方式加载到建立好的表中。数据一旦导入就不可以修改。
2.通过查询将数据保存到filesystem
INSERT OVERWRITE [LOCAL] DIRECTORY directory SELECT.... FROM .....
导入数据到本地目录：
insert overwrite local directory '/home/zhangxin/hive' select * from test_insert1;
产生的文件会覆盖指定目录中的其他文件，即将目录中已经存在的文件进行删除。
导出数据到HDFS中：
insert overwrite directory '/user/zhangxin/export_test' select value from test_table;
同一个查询结果可以同时插入到多个表或者多个目录中：
from test_insert1
insert overwrite local directory '/home/zhangxin/hive' select *
insert overwrite directory '/user/zhangxin/export_test' select value;
17.mapjoin的使用 应用场景：1.关联操作中有一张表非常小 2.不等值的链接操作
select /*+ mapjoin(A)*/ f.a,f.b from A t join B f  on ( f.a=t.a and f.ftime=***********)

18.查看perl进程
ps -ef|grep perl
19.导出白条数据
hive -e "select day_id,pv,uv,ip_count,click_next_count,second_bounce_rate,return_visit,pg_type from tmp.tmp_h02_click_log_baitiao_ag_sum where day_id like '2018-01%';"> /home/jrjt/testan/baitiao.xlsx;
20.hive修改表名
ALTER TABLE o_h02_click_log_i RENAME TO o_h02_click_log_i_bk;
21.hive复制表结构
CREATE TABLE d_h02_click_log_baitiao_ag_sum LIKE tmp.tmp_h02_click_log_baitiao_ag_sum;
22.hive添加字段
alter table tmp_h02_click_log_baitiao_ag_sum add columns(current_session_timelenth_count bigint comment '页面停留总时长');
ALTER TABLE tmp_h02_click_log_baitiao CHANGE current_session_timelenth current_session_timelenth bigint comment '当前会话停留时间';
23.hive开启简单模式不启用mr
set hive.fetch.task.conversion=more;
24.以json格式输出执行语句会读取的input table和input partition信息
Explain dependency query



来自@若泽大数据


