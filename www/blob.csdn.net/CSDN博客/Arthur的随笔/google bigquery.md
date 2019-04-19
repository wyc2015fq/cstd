# google bigquery - Arthur的随笔 - CSDN博客
2012年05月03日 23:49:09[largetalk](https://me.csdn.net/largetalk)阅读数：3402
                
"Analyze terabytes of data with just a click of a button"标题还是挺吸引人得，且看实际如何。
BigQuery 与 Google Cloud SQL不同之处：
BigQuery 在大规模数据（数十亿行）上查询有很好得速度
BigQuery 适合快速分析大规模数据，不支持数据修改，BigQuery是OLAP系统
通过CSV格式将数据导入BigQuery，数据以弱关系型存储与云上
BigQuery 不是一个数据库系统， 1. 不支持表索引和其他关系数据库特性 2. BigQuery支持SQL得一个子集，不支持update,delete操作 3.BigQuery得join只在一张表远比另一张表小时有效（也就是不支持两张大表得join）
BigQuery 支持REST接口
GCS（Google Cloud SQL）是一个mysql实例，支持所有关系型数据库操作
GCS 适合中小型数据， GCS是OLTP系统
高QPS，适合web服务
适合低延迟环境
可以导入用mysqldump导出得其他数据库
GCS 仅支持GAE
试用了下BigQuery, 不管通过browser tool或command-line tool都还比较方便应用，虽然没看到如何追加数据的指示，不过通过创建相同名称得表是可以达到追加数据的目的，其他方面试了试也还行，不过有两个问题，1是网速，2是价格
1.以前用hadoop分析数据，因为有好几台机器，慢得地方不是在分析数据这块，慢得地方是将数据导入到hdfs，即使是内网。而如果通过互联网上传一个几十G得文件，不知道是一个什么情况。
2.数据基本是每天几G几十G得递增，几个月后就上T了，每个月光存储就好几百美元，不比自己买机器便宜
            
