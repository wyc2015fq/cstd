# hadoop中的常用命令 - Hxyue2018's Blog - CSDN博客





2018年10月10日 15:35:03[Hxyue2018](https://me.csdn.net/Super_Json)阅读数：50标签：[hdfs](https://so.csdn.net/so/search/s.do?q=hdfs&t=blog)
个人分类：[基础工具](https://blog.csdn.net/Super_Json/article/category/8114061)









**1.查看hive表在hdfs上的存储位置**

hive> show create table dj_test.hxy_coma_arrived_dri_often_stay;

LOCATION

  'hdfs://DClusterNmg4/user/dj_bi/warehouse/dj_test.db/hxy_coma_arrived_dri_often_stay'

hdfs dfs -du hdfs://DClusterNmg4/user/dj_bi/warehouse/dj_test.db/hxy_coma_arrived_dri_often_stay

hdfs dfs -du /user/dj_bi/warehouse/dj_test.db/hxy_safedriving_data_log

**2.将文件拷贝到本地**

hdfs dfs -copyToLocal /user/dj_bi/huangxiangyue/backup/huangxiangyue.tar.gz ~/huangxiangyue/huangxiangyue.tar.gz

3.





