
# Elasticsearch修改mapping - 追求卓越,做到专业 - CSDN博客


2017年04月21日 11:50:31[Waldenz](https://me.csdn.net/enter89)阅读数：293标签：[Elasticsearch																](https://so.csdn.net/so/search/s.do?q=Elasticsearch&t=blog)[mapping																](https://so.csdn.net/so/search/s.do?q=mapping&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Elasticsearch&t=blog)个人分类：[Elasticsearch																](https://blog.csdn.net/enter89/article/category/6735273)


Elasticsearch 增加字段前，首先增加mapping,仅对索引中以后的数据生效。
POST http://192.168.208.32:9200/testindex/_mapping/data/

```python
{
  "properties": {
    "title": {
      "index": "analyzed",
      "type": "string",
      "fields": {
        "raw": {
          "index": "not_analyzed",
          "type": "string"
        }
      }
    }
  }
}
```


