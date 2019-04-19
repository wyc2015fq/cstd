# 使用(oracle)sql语句采集数据时，数据之间的分割符号要设置成数据中没有的 - sxf_123456的博客 - CSDN博客
2017年07月22日 10:49:37[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：276标签：[csv																[插件](https://so.csdn.net/so/search/s.do?q=插件&t=blog)](https://so.csdn.net/so/search/s.do?q=csv&t=blog)
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
使用sql语句采集数据时，数据之间的分割符号要设置成数据中没有的
1、使用(oracle)sql语句查询一下，检测sql语句是否正确
select 
ID ||'^'||
NAME ||''^"||
SEX ||'^'||
CLASS 
from student;
2、若上边sql语句正确，将其修改为下边部分,命名文件005_表名.sql
select 
ID ||{spletor}||
NAME ||{spletor}||
SEX ||{spletor}||
CLASS 
from student;
3、采集数据sql语句存放路径
/usr/local/xuanfeng/etl/scripts
4、编写oracle2file.json文件(采集oracle数据库中的文件,转化为csv文件)
[
{
"id": "005_表名",
"datatime":
{
"unit": "day",
"offset": -1    #设置为前一天
},
"source":
{
"host": "IP",   
"port": 11521,
"username": "aaa",
"password": "bbb",
"dbname": "ccc",
"script": "005_表名.sql"
},
"target":
{
"path": "/data/gatherfiles/datafiles/表名/005/YYYYMMDD",
"file": "005_表名_YYYYMMDD.csv"
},
"content":
{
"prefix": "",
"suffix": "",
"empty": "",
"gap": "\\005"  #将数据之间的间隔符设置为口符号
}
},
]
5、oracle2file.json文件存放路径
/usr/local/xuanfeng/etl/etc
6、编写logstash配置文件,命名为005_表名.conf
input {
    file {
        path =>[
"/data/gatherfiles/datafiles/表名/005/*/*.csv"
]
                start_position => "beginning" #从什么位置读取
sincedb_path => "/data/elk/sincedb/005"
                type => "005"
tags => ["表名", "gather"]
    }
}
filter {
if [type]=="005"{
csv {  
columns =>["ID","NAME","SEX","CLASS"]
separator => 
quote_char => "‰"
}
date { 
        match => ["STBUSEDATE", "yyyyMMddHHmmss"]
target => "STBUSEDATE"
    }
date { 
        match => ["HGUSEDATE", "yyyyMMddHHmmss"]
target => "HGUSEDATE"
    }
mutate{
remove_field => ["message"]
}
}
}
output {
if [type]=="005"{
        elasticsearch { 
            hosts => ["133.37.22.180:9200"]
            index => "gather-%{type}" #在es里面建立的索引名
            document_type => "%{type}" #文档类型，一般用input里面配置的type
            flush_size => 20000
            idle_flush_time => 10
            sniffing => true
            template_overwrite => true
            codec => "json" 
        }
}
}
7、logstash配置文件路径
/data/app/etc/logstash/conf.d/
8、采集数据存放执行命令路径
/usr/local/xuanfeng/etl/bin
9、执行采集命令
./oracle2file   -c   005_表名   -f   005    -n     001    -o    -t    20170720000000
用法: oracle2file [-options] name [args...]
  -c string
调度执行配置编号
  -f string
调度流程编号
  -n string
调度流程节点编号
  -o启用详细输出<选项>
默认：false
  -t string
调度执行时间
格式：[YYYYMMDDHH24MISS]
10、生成csv文件存放路径
/data/gatherfiles/datafiles/
11、文件读取保留值路径
/data/elk/sincedb/
12、使用logstash命令将数据存储在es中
logstash    -f    005_表名.conf
13、在终端使用elasticsearch中的curl命令查询数据是否导入成功
curl -XGET 'localhost:9200/_cat/indices?v&pretty'  #查看gather-005索引是否创建成功
#查询student表中name字段中，值为Lihua的是否存在
curl -XGET 'localhost:9200/_search?pretty' -H 'Content-Type:application/json' -d'
{
  "query":{
  "match":{ 
     "NAME":"Lihua"  
    }
  }
}'
curl -XGET 'localhost:9200/_search?pretty' -H 'Content-Type:application/json' -d'
{
   "query":{
     "term":{
        "NAME.keyword":"Lihua"
        }     
    }
}'
查询name为Lihua的前十个
curl -XGET 'localhost:9200/_search?pretty' -H 'Content-Type:application/json' -d'
{
   "size":10,
   "query":{
     "term":{
         "NAME.keyword":"Lihua"
        }
    }
}'
