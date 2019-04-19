# es重命名索引 - sxf_123456的博客 - CSDN博客
2018年08月28日 21:59:33[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1613
from elasticsearch import Elasticsearch
from elasticsearch import helpers
es = Elasticsearch(hosts="0.0.0.0:9200")
source_index = "ana-test_20180805_1"
target_index = "ana-test-test_20180805_1"
body = {
    "query":{
        "match_all":{}
    }
}
helpers.reindex(es,source_index=source_index,target_index=target_index,query=body,target_client=es,chunk_size=1000,scroll="5m")
reindex方法定义:
`elasticsearch.helpers.reindex(client, source_index, target_index, query=None, target_client=None, chunk_size=500, scroll=u'5m', scan_kwargs={}, bulk_kwargs={})`
参数:
client – 原索引所在ES
source_index – 读取documents的索引
target_index – 写入documents的索引
query – search( ) api的主体
target_client – 新索引所在ES
chunk_size – es传输docs时每块含有的docs数量
scroll – scroll的时间
scan_kwargs – additional kwargs to be passed to scan()
bulk_kwargs – additional kwargs to be passed to bulk()
reindex是将scan、scroll和bulk组合在一起，通过scroll获取的数据，利用bulk导入es
