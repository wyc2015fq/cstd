# python批量从es取数据（文档数超过10000） - sxf_123456的博客 - CSDN博客
2018年05月19日 13:47:38[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1987
*"""**提取文档数超过10000的数据**按照某个字段的值具有唯一性进行升序，**按照@timestamp进行降序，**第一次查询，先将10000条数据取出，**取出最后一个时间戳，**在第二次查询中，设定@timestamp小于将第一次得到的最后一个时间戳，**同时设定某个字段的值具有唯一性进行升序，**按照@timestamp进行降序，**"""*from elasticsearch import Elasticsearch
import os
write_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\10000_data.txt"
es = Elasticsearch(hosts="", timeout=1500)
write_file = open(write_path, "a+")
def _first_query():
    index_ = "gather-010"
_source = ["TWICE_BOOK_TIME", "@timestamp"]
    try:
        rs = es.search(index=index_, body={
            "size": 10000,
"query": {
                "match_all": {}
            },
"sort": [
                {
                    "@timestamp": {
                        "order": "desc"
}
                },
{
                    "TASK_RECEIVE_ID.keyword": {
                        "order": "asc"
}
                }
            ],
"_source": _source
        })
        return rs
    except:
        raise Exception("{0} search error".format(index_))
def _get_first_data(first_rs):
    i = 0
if first_rs:
        for hit in first_rs['hits']['hits']:
            IptvAccount = hit['_source']['TWICE_BOOK_TIME']
            timestamp = hit['_source']['@timestamp']
            if IptvAccount is None:
                IptvAccount = ""
write_file.write(IptvAccount + "," + timestamp + "\n")
            i += 1
if i == 10000:
                return timestamp
def _second_query(timestamp):
    index_ = "gather-010"
_source = ["TWICE_BOOK_TIME", "@timestamp"]
    try:
        rs = es.search(index=index_, body={
            "size": 10000,
"query": {
                "bool": {
                    "filter": {
                        "range": {
                            "@timestamp": {
                                "lt": timestamp
                            }
                        }
                    }
                }
            },
"sort": [
                {
                    "@timestamp": {
                        "order": "desc"
}
                },
{
                    "TASK_RECEIVE_ID.keyword": {
                        "order": "asc"
}
                }
            ],
"_source": _source
        })
        return rs
    except:
        raise Exception("{0} search error".format(index_))
if __name__ == "__main__":
    first_rs = _first_query()
    first_timestamp = _get_first_data(first_rs)
    print(first_timestamp)
    while True:
        second_rs = _second_query(first_timestamp)
        first_timestamp = _get_first_data(second_rs)
        if first_timestamp is None:
            break
print(first_timestamp)
