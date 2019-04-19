# mysql常用语句 - sxf_123456的博客 - CSDN博客
2018年03月07日 13:37:45[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：48
1、插入数据语句
语法
INSERT [LOW_PRIORITY | DELAYED | HIGH_PRIORITY] [IGNORE]
    [INTO] tbl_name [(col_name,...)]
    VALUES ({expr | DEFAULT},...),(...),...
    [ ON DUPLICATE KEY UPDATE col_name=expr, ... ]
INSERT INTO ana_mac_dict (mac,LENGTH,TYPE,factory_name,model_name,VERSION) VALUES ('28A6DB39',8,2,'华为','EC610V9C',1);
2、删除语句
语法：
DELETE[LOW_PRIORITY][QUICK][IGNORE]FROM tbl_name
[WHERE where_definition]
[ORDER BY ...]
[LIMITrow_count]
