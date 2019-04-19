# 获取mysql下所有表 - sxf_123456的博客 - CSDN博客
2018年09月25日 23:16:36[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：336
select table_name from information_schema.TABLES where TABLE_SCEMA='db_name';
infromation_schema是数据库系统用来保存数据表的总表
