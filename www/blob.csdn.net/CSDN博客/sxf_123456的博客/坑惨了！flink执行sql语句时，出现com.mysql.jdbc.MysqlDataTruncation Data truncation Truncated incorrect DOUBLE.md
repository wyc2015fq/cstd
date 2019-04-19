# 坑惨了！flink执行sql语句时，出现com.mysql.jdbc.MysqlDataTruncation: Data truncation: Truncated incorrect DOUBLE - sxf_123456的博客 - CSDN博客
2018年10月18日 19:55:23[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：85
出现错误!
Data truncation: Truncated incorrect DOUBLE value: '2018-10-18 19:21:47'
Flink操作mysql数据库对数据进行更新时。
原始sql语句:
update t_lianlu_utilization_alarm set cancle_time=? and recevicetime=? where device_id=? and port_id=?
修改为
update t_lianlu_utilization_alarm set cancle_time=? ，recevicetime=? where device_id=? and port_id=?
将sql语句中的and 改为逗号。
因错误花费半个小时，特此记下
