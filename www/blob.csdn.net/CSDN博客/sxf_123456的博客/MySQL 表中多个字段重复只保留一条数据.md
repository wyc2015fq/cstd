# MySQL 表中多个字段重复只保留一条数据 - sxf_123456的博客 - CSDN博客
2017年08月23日 18:05:19[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：2168
1、使用sql语句进行查询(多个字段重复)
select 
*
from 
test1
group by factory_name,model_name,hard_version  having count(*)>=1
2、将查到的数据导出为csv文件
3、清空表
truncate test1
4、使用mysql yog   在test1表右键-》导入-》导入加载本地csv数据
单个字段重复，只需将group by 后边设置为一个字段即可
