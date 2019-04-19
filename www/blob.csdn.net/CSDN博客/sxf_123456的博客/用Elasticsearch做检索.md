# 用Elasticsearch做检索 - sxf_123456的博客 - CSDN博客
2017年07月19日 11:07:25[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：327
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
> 
ElasticSearch是一个基于Lucene的搜索服务器。它是一个分布式，REST风格的搜索和分析引擎。Elasticsearch是用Java开发的。设计用于云计算中，能够达到实时搜索，稳定，可靠，快速，安装使用方便。
![用Elasticsearch做检索](http://p3.pstatp.com/large/22d90001e340390b38fa)
# 安装运行 Elasticsearch 
安装 Elasticsearch 之前，需要先安装一个较新的版本的 Java。可以从 www.java.com 获得官方提供的最新版本的 Java。建议使用linux环境
> 
$ echo $JAVA_HOME
/home/users/username/opt/sun-java8/
之后，可以从 elastic 的官网 https://www.elastic.co/downloads/elasticsearch 获取最新版本的 Elasticsearch。我们使用5.4.0：elasticsearch-5.4.0.tar.gz。解压即可。
> 
cd elasticsearch-5.4.0
./bin/elasticsearch -d
参数 -d，表示把 Elasticsearch 作为一个守护进程在后台运行 。
执行命令：curl 'http://localhost:9200/?pretty'，返回值如下。表示安装成功了。
> 
{
"name" : "QR5NuHq",
"cluster_name" : "my-application",
"cluster_uuid" : "FumB0l6rQxa_wb8pb6uW5Q",
"version" : {
"number" : "5.4.0",
"build_hash" : "780f8c4",
"build_date" : "2017-04-28T17:43:27.229Z",
"build_snapshot" : false,
"lucene_version" : "6.5.0"
},
"tagline" : "You Know, for Search"
}
Elasticsearch默认使用9200端口（http.port: 9200）。如果有需要可以在 config/elasticsearch.yml配置文件中修改
# 索引文档
我们的项目是对一批小说进行检索。数据格式如下：
|ID|标题|读者数|更新时间戳|标签tag|
|----|----|----|----|----|
|1|历史趣谈|21993888|1496682065|野史,中国史,历史|
|2|话说汉朝|28555776|1496683239|秦汉,中国史,历史|
|3|大唐风云录|1018160|1496682065|隋唐,中国史,历史|
|4|黑白曹操|199383|1496682350|三国,中国史,历史|
|5|品读资治通鉴|318366|1496682066|先秦,秦汉,中国史,历史|
|6|情爱相对论|3514746|1496682066|大咖,情感|
|7|射雕三部曲|163275|1496682066|讲座,中国史,历史|
- 
添加文档索引
> 
curl -XPUT 'localhost:9200/set/article/1?pretty' -H 'Content-Type: application/json' -d'
{
"title" : "历史趣谈",
"tag" : "野史,中国史,历史",
"count" : 21993888
}'
curl -XPUT 'localhost:9200/set/article/2?pretty' -H 'Content-Type: application/json' -d'
{
"title" : "话说汉朝",
"tag" : "秦汉,中国史,历史",
"count" : 28555776
}'
路径 /set/article/1 包含了三部分的信息：
set：索引名称
article：类型名称
1：特定文章的ID
- 
查看文档
> 
curl -XGET 'localhost:9200/set/article/1?pretty'
返回文档信息
> 
{
"_index" : "set",
"_type" : "article",
"_id" : "1",
"_version" : 1,
"found" : true,
"_source" : {
"title" : "历史趣谈",
"tag" : "野史,中国史,历史",
"count" : 21993888
}
}
- 
查看所有文档
> 
curl -XGET 'localhost:9200/set/article/_search?pretty'
- 
删除文档
> 
curl -XDELETE 'localhost:9200/set/article/1?pretty'
- 
更新文档
再次执行添加索引的HTTP命令即可
# 检索功能
查询tag中包含”历史“标签的文章
> 
curl -XGET 'localhost:9200/set/article/_search?pretty' -H 'Content-Type: application/json' -d'
{
"query" : {
"match" : {
"tag" : "历史"
}
}
}'
查询文章title中包含”历史“两个字的文章
> 
curl -XGET 'localhost:9200/set/article/_search?pretty' -H 'Content-Type: application/json' -d'
{
"query" : {
"match" : {
"title" : "历史"
}
}
}'
查询tag中包含”历史“标签的，并且读者数大于1218160 的文章
> 
curl -XGET 'localhost:9200/set/article/_search?pretty' -H 'Content-Type: application/json' -d'
{
"query" : {
"bool": {
"must": {
"match" : {
"tag" : "历史"
}
},
"filter": {
"range" : {
"count" : { "gt" : 1218160 }
}
}
}
}
}'
这部分是一个 range 过滤器，它能找到读者数大于1218160的文档，其中 gt 表示_大于(_great than)。
- 
分页功能
和 SQL 使用 LIMIT 关键字返回单个 page 结果类似，Elasticsearch 接受 from 和 size 参数：
size ： 显示应该返回的结果数量，默认是 10
from ： 显示应该跳过的初始结果数量，默认是 0
如果每页展示 5 条结果，可以用下面方式请求得到 1 到 3 页的结果：
> 
curl -XGET 'localhost:9200/set/article/_search?pretty&size=5&from=5' -H 'Content-Type: application/json' -d'
{
"query" : {
"match" : {
"tag" : "历史"
}
}
}'
- 
按属性排序
按读者数逆序检索
> 
curl -XGET 'localhost:9200/set/article/_search?pretty' -H 'Content-Type: application/json' -d'
{
"query" : {
"match" : {
"tag" : "历史"
}
},
"sort": { "count": { "order": "desc" }}
}'
# 检索排序
默认情况下，返回结果是按相关性倒序排列的。
做检索时，返回字段中有_score值，表示对应文档的相关性
> 
"hits" : {
"total" : 3,
"max_score" : 0.73636544,
"hits" : [
{
"_index" : "set",
"_type" : "article",
"_id" : "1",
"_score" : 0.73636544,
"_source" : {
"title" : "历史趣谈",
"tag" : "野史,中国史,历史",
"count" : 21993888
}
},
每个文档都有相关性评分，用一个正浮点数字段 _score 来表示 。 _score 的评分越高，相关性越高。
查询语句会为每个文档生成一个 _score 字段。评分的计算方式主要是计算全文本字段的值相对于全文本检索词相似程度。Elasticsearch 的相似度算法 被定义为检索词频率/反向文档频率，包括以下内容：
- 
检索词频率
检索词在该字段出现的频率，出现频率越高，相关性也越高。 字段中出现过 5 次要比只出现过 1 次的相关性高。
- 
反向文档频率
每个检索词在索引中出现的频率，频率越高，相关性越低。检索词出现在多数文档中会比出现在少数文档中的权重更低。
- 
字段长度
字段的长度是多少，长度越长，相关性越低。 检索词出现在一个短的 title 要比同样的词出现在一个长的 content 字段权重更大。
当调试一条复杂的查询语句时，想要理解 _score 究竟是如何计算的，Elasticsearch 在 每个查询语句中都有一个 explain 参数，将 explain 设为 true 就可以得到更详细的信息。
> 
curl -XGET 'localhost:9200/set/article/_search?explain&pretty&format=yaml' -H 'Content-Type: application/json' -d'
{
"query" : {
"match" : {
"tag" : "历史"
}
}
}'
