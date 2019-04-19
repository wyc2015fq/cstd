# Elasticsearch 在kibana中对索引名称进行重命名 - sxf_123456的博客 - CSDN博客
2019年01月25日 16:18:33[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：221
问题:
在实际的工作中，遇到已经将数据写入es，但是后边需要对这个索引进行重命名。
如 test-20190122、test-20190121 需要重命名为test-2019。对于数据量比较少时，创建多个索引，需要创建多个分片，造成存储资源的浪费，需要将多个索引的数据合并到一个索引中。
操作步骤：
1、在kibana中执行以下命令：
POST _reindex
{
"source": {
"index": "test-20190122"
},
"dest": {
"index": "test-2019",
"op_type": "create"
}
}
2、删除之前多余的索引数据
DELETE test-20190122
对于多个索引名称重命名为一个索引，请查看官方文档
[https://www.elastic.co/guide/en/elasticsearch/reference/current/docs-reindex.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/docs-reindex.html)
