# Hadoop Hive sql语法详解5--HiveQL与SQL区别 - z69183787的专栏 - CSDN博客
2016年10月26日 12:09:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4929
1.hive内联支持什么格式？
2.分号字符注意什么问题？
3.hive中empty是否为null?
4.hive是否支持插入现有表或则分区中？
5.hive是否支持INSERT INTO 表 values（）？
1、Hive不支持等值连接 
•SQL中对两表内联可以写成：
•select * from dual a,dual b where a.key = b.key;
•Hive中应为
•select * from dual a join dual b on a.key = b.key; 
而不是传统的格式：
SELECT t1.a1 as c1, t2.b1 as c2FROM t1, t2
WHERE t1.a2 = t2.b2
2、分号字符
•分号是SQL语句结束标记，在HiveQL中也是，但是在HiveQL中，对分号的识别没有那么智慧，例如：
•select concat(key,concat(';',key)) from dual;
•但HiveQL在解析语句时提示：
        FAILED: Parse Error: line 0:-1 mismatched input '<EOF>' expecting ) in function specification
•解决的办法是，使用分号的八进制的ASCII码进行转义，那么上述语句应写成：
•select concat(key,concat('\073',key)) from dual;
3、IS [NOT] NULL
•SQL中null代表空值, 值得警惕的是, 在HiveQL中String类型的字段若是空(empty)字符串, 即长度为0, 那么对它进行IS NULL的判断结果是False.
4、Hive不支持将数据插入现有的表或分区中，
仅支持覆盖重写整个表，示例如下：
- 
INSERT OVERWRITE TABLE t1  
- 
SELECT * FROM t2;
复制代码
5、hive不支持INSERT INTO 表 Values（）, UPDATE, DELETE操作
    这样的话，就不要很复杂的锁机制来读写数据。
    INSERT INTO syntax is only available starting in version 0.8。INSERT INTO就是在表或分区中追加数据。
6、hive支持嵌入mapreduce程序，来处理复杂的逻辑
如：
- 
FROM (  
- 
MAP doctext USING 'python wc_mapper.py' AS (word, cnt)  
- 
FROM docs  
- 
CLUSTER BY word  
- 
) a  
- 
REDUCE word, cnt USING 'python wc_reduce.py';  
复制代码
--doctext: 是输入
--word, cnt: 是map程序的输出
--CLUSTER BY: 将wordhash后，又作为reduce程序的输入
并且map程序、reduce程序可以单独使用，如：
- 
FROM (  
- 
FROM session_table  
- 
SELECT sessionid, tstamp, data  
- 
DISTRIBUTE BY sessionid SORT BY tstamp  
- 
) a  
- 
REDUCE sessionid, tstamp, data USING 'session_reducer.sh';  
复制代码
-DISTRIBUTE BY: 用于给reduce程序分配行数据
7、hive支持将转换后的数据直接写入不同的表，还能写入分区、hdfs和本地目录
这样能免除多次扫描输入表的开销。
- 
FROM t1  
- 
- 
INSERT OVERWRITE TABLE t2  
- 
SELECT t3.c2, count(1)  
- 
FROM t3  
- 
WHERE t3.c1 <= 20  
- 
GROUP BY t3.c2  
- 
- 
INSERT OVERWRITE DIRECTORY '/output_dir'  
- 
SELECT t3.c2, avg(t3.c1)  
- 
FROM t3  
- 
WHERE t3.c1 > 20 AND t3.c1 <= 30  
- 
GROUP BY t3.c2  
- 
- 
INSERT OVERWRITE LOCAL DIRECTORY '/home/dir'  
- 
SELECT t3.c2, sum(t3.c1)  
- 
FROM t3  
- 
WHERE t3.c1 > 30  
- 
GROUP BY t3.c2;  
复制代码
实际实例
创建一个表
CREATE TABLE u_data (
userid INT,
movieid INT,
rating INT,
unixtime STRING)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '/t'
STORED AS TEXTFILE;
下载示例数据文件，并解压缩
wget [http://www.grouplens.org/system/files/ml-data.tar__0.gz](http://www.grouplens.org/system/files/ml-data.tar__0.gz)
tar xvzf ml-data.tar__0.gz
加载数据到表中:
LOAD DATA LOCAL INPATH 'ml-data/u.data'
OVERWRITE INTO TABLE u_data;
统计数据总量:
SELECT COUNT(1) FROM u_data;
现在做一些复杂的数据分析:
创建一个 weekday_mapper.py: 文件，作为数据按周进行分割 
import sys
import datetime
for line in sys.stdin:
line = line.strip()
userid, movieid, rating, unixtime = line.split('/t')
生成数据的周信息
weekday = datetime.datetime.fromtimestamp(float(unixtime)).isoweekday()
print '/t'.join([userid, movieid, rating, str(weekday)])
使用映射脚本
//创建表，按分割符分割行中的字段值
CREATE TABLE u_data_new (
userid INT,
movieid INT,
rating INT,
weekday INT)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '/t';
//将python文件加载到系统
add FILE weekday_mapper.py;
将数据按周进行分割
INSERT OVERWRITE TABLE u_data_new
SELECT
TRANSFORM (userid, movieid, rating, unixtime)
USING 'python weekday_mapper.py'
AS (userid, movieid, rating, weekday)
FROM u_data;
SELECT weekday, COUNT(1)
FROM u_data_new
GROUP BY weekday;
处理Apache Weblog 数据
将WEB日志先用正则表达式进行组合，再按需要的条件进行组合输入到表中
add jar ../build/contrib/hive_contrib.jar;
CREATE TABLE apachelog (
host STRING,
identity STRING,
user STRING,
time STRING,
request STRING,
status STRING,
size STRING,
referer STRING,
agent STRING)
ROW FORMAT SERDE 'org.apache.hadoop.hive.contrib.serde2.RegexSerDe'
WITH SERDEPROPERTIES (
"input.regex" = "([^ ]*) ([^ ]*) ([^ ]*) (-|//[[^//]]*//]) ([^ /"]*|/"[^/"]*/") (-|[0-9]*) (-|[0-9]*)(?: ([^ /"]*|/"[^/"]*/") ([^ /"]*|/"[^/"]*/"))?",
"output.format.string" = "%1$s %2$s %3$s %4$s %5$s %6$s %7$s %8$s %9$s"
)
STORED AS TEXTFILE;
