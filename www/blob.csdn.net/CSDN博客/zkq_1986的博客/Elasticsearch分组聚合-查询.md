# Elasticsearch分组聚合-查询 - zkq_1986的博客 - CSDN博客





2016年08月31日 16:38:08[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：6188








## 1 统计某个字段下的分组情况

类似如下功能： 

select count(*) from mytable group by myfield

```
curl -XPOST 'localhost:19200/ylchou-0-2015-10-07/_search?pretty' -d '
 {
    "size": 0,
    "aggs": {
      "group_by_state": {
        "terms": {
          "field": "A_logtype"
        }
      }
    }
}'
```

或者通过浏览器方式 

先进入url: [http://myip:9200/_plugin/head/](http://myip:9200/_plugin/head/)



