# 使用python导出mysql数据 - sxf_123456的博客 - CSDN博客
2018年05月13日 11:59:34[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：242
import pymysql
"""
,"114000","115000",
               "116000", "117000","118000","119000","120000","121000"
"""
# db = pymysql.connect('0.0.0.0','','','test','utf8')
# localnet_id = ["105000","106000","107000","108000","109000",
#                "110000","111000","112000","113000","114000","115000",
#                "116000", "117000","118000","119000","120000","121000"]
localnet_id = ["105000"]
# localnet_id = ["103000"]
# cursor = db.cursor()
sql = """
select 
   t1.loid LOID, 
   IFNULL(t1.band_account, '') 宽带账号, 
   t1.hgw_factory_name 光猫厂商,
   t1.hgw_model_name 光猫型号,
   t1.hgw_hard_version 光猫硬件版本,
    IFNULL(t1.dev_count, 0) 下挂设备总数,
   if(t1.router_count> 0, '是', '否') 是否使用路由器,
   IFNULL(t1.wifi_count, 0) 光猫WIFI下挂设备数,
   IFNULL(t1.router_factory_name, '') 路由器厂商,
   IFNULL(t1.router_model_name, '') 路由器型号,
   IFNULL(t2.port_count, 4) 光猫LAN口数量,
    t1.localnet_id 本地网ID
from 
   ana_result t1 
left join 
   ana_hgw_class t2 
on 
    t1.hgw_factory_name = t2.factory_name 
   and t1.hgw_model_name = t2.model_name
    and t1.hgw_hard_version = t2.hard_version
where 
   t1.localnet_id = '%s'
INTO OUTFILE '/home/data/mysql/tt/%s_hgw.csv'
CHARACTER SET 'GBK'
FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
LINES TERMINATED BY '\\n';
"""
for local_id in localnet_id:
    sql_result = sql%(local_id,local_id)
    print(sql_result)
    cursor.execute(sql_result)
db.commit()
db.close()
