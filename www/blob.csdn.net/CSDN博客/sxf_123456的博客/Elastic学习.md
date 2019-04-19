# Elastic学习 - sxf_123456的博客 - CSDN博客
2017年07月19日 11:52:04[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：133
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
A-
1、创建一个index 
curl -XPUT 'localhost:9200/customer?pretty&pretty'
2、查看
curl -XGET 'localhost:9200/_cat/indices?v&pretty'
3、创建带有index、type的索引（ domain/{index}/{type}/id）
curl -XPUT 'localhost:9200/customer/external/1?pretty&pretty' -d'
{
"name": "John Doe"
}'
4、查看id为1的document
curl -XGET 'localhost:9200/customer/external/1?pretty&pretty'
5、删除index
curl -XDELETE 'localhost:9200/customer?pretty&pretty'
