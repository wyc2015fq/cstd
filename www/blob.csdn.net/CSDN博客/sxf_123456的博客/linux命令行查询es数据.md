# linux命令行查询es数据 - sxf_123456的博客 - CSDN博客
2017年07月22日 10:58:50[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：809标签：[elasticsearch																[curl](https://so.csdn.net/so/search/s.do?q=curl&t=blog)](https://so.csdn.net/so/search/s.do?q=elasticsearch&t=blog)
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
                1、查询gather-005中 NAME='Lihua'
curl -XGET 'localhost:9200/gather-005/_search?pretty' -H 'Content-Type:application/json' -d'
{
 "query":{
  "match":{
 "NAME":"Lihua"
   }
  }
}'
curl -XGET 'localhost:9200/gather-005/_search?pretty' -H 'Content-Type:application/json' -d'
{
 "size":10,
 "query":{
"term":{
  "NAME.keyword":"Lihua"
   }
}
}'
2、查询 gather-010中前两个
curl -XGET 'localhost:9200/gather-005/_search?pretty' -H 'Content-Type:application/json' -d'
{
  "size":2
}
3、查询gather-000中area_type为0的前1个
curl -XGET 'localhost:9200/gather-005/_search?pretty' -H 'Content-Type:application/json' -d'
{
  "size":1,
  "query":{
"term":{
"NAME.keyword":"Lihua"
  }
    }
}'  
4、创建一个index
curl -XPUT 'localhost:9200/customer?pretty&pretty'
5、查看所有的index
curl -XGET 'localhost:9200/_cat/indices?v&pretty'
6、创建一个document domain/{index}/{type}/id
curl -XPUT 'localhost:9200/customer/external/1?pretty&pretty' -d'
{
 "name":"Li hua"
}
7、删除customer索引
curl -XDELETE 'localhost:9200/customer?pretty'
8、
curl -XPOST 'localhost:9200/customer/external?pretty&pretty' -d'
{
"name": "Jane Doe"
}'
更新：
curl -XPOST 'localhost:9200/customer/external/1/_update?pretty&pretty' -d'
{
"doc": { "name": "Jane Doe" }
}'
命令的格式：
<REST Verb> /<Index>/<Type>/<ID>
curl -XGET 'localhost:9200/gather-005/_search?pretty' -H 'Content-Type:application/json' -d'
{
    "aggs":{
           "newtype":{
                  "terms":{
                     "field":"NAME.keyword",
                     "size":100
                        }
                     }
            }
}'
curl -XGET 'localhost:9200/gather-005/_search?pretty' -H 'Content-Type:application/json' -d'
{
 "size":10,
 "query":{
    "term":{"area_type.keyword":"1"}
     },
    "aggs":{
        "newtype":{
           "terms":{
                "field":"NAME.keyword",
                "size":100
                    }
                 }
              }
}'
