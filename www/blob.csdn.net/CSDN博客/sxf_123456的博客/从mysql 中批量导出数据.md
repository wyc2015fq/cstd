# 从mysql 中批量导出数据 - sxf_123456的博客 - CSDN博客
2017年09月05日 21:50:16[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：650
                CREATE 
TABLE  
ana_hgw_class (
factory_name VARCHAR(50) COMMENT '厂家',
model_name VARCHAR(50) COMMENT '设备型号',
hard_version VARCHAR(50) COMMENT '硬件版本',
soft_version VARCHAR(50) COMMENT '软件版本',
hgw_type INT COMMENT '终端类别',
customer_type INT COMMENT '用户类别',
port_count INT COMMENT 'LAN口数量',
posts_count INT COMMENT 'POTS口数量',
has_wifi INT COMMENT 'WIFI功能',
create_time TIMESTAMP,
update_time TIMESTAMP
);
SELECT COUNT(*) FROM ana_hgw_class;
SELECT * FROM ana_hgw_class;
SELECT COUNT(*) FROM ana_device WHERE loid IN ( SELECT loid FROM ana_customer WHERE localnet_id='101000');
SELECT mac FROM ana_device WHERE loid IN ( SELECT loid FROM ana_customer WHERE localnet_id='101000');
#从mysql中导出数据，需要进入mysql数据库
SELECT mac into OUTFILE '101000.txt' FROM ana_device WHERE loid IN ( SELECT loid FROM ana_customer WHERE localnet_id='101000');
使用find / -name 101000.txt 命令查看文件的位置
一般mysql导出的数据默认放在/home/data/mysql/test,其中test为数据库
#不需要进入mysql数据库
[root@localhost test]# mysql -uroot -psctele@root -h127.0.0.1 -P3306 test --default-character-set=utf8 --skip-column-names -B -e 'select * from test1;' > 2.txt
将数据库数据导出到当前目录下，而不是mysql默认的导出路径
*在终端使用mysql -u root -p test  -e 'SELECT loid FROM ana_customer WHERE localnet_id='101000';' > 101000_loid.txt**将表中数据导出到本地*
