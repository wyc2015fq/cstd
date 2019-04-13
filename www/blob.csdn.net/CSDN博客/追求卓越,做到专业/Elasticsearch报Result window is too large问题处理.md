
# Elasticsearch报Result window is too large问题处理 - 追求卓越,做到专业 - CSDN博客


2017年02月20日 09:48:28[Waldenz](https://me.csdn.net/enter89)阅读数：7970


在使用Elasticsearch进行search查询时，出现了Result window is too large的问题。
报错如下：
QueryPhaseExecutionException[Result window is too large, from + size must be less than or equal to: [10000] but was [10200]. See the scroll api for a more efficient way to request large data sets. This limit can be set by changing the [index.max_result_window]
 index level parameter.]; }{[Sgi3DlLvRimb3RlcDFJI2Q][goagain_index_order_v1][9]: RemoteTransportException[[node_l-qgoagainesdata3.ops.cn2.qunar.com][10.90.53.180:9300][indices:data/read/search[phase/query]]]; nested: QueryPhaseExecutionException[Result window
 is too large, from + size must be less than or equal to: [10000] but was [10200]. See the scroll api for a more efficient way to request large data sets. This limit can be set by changing the [index.max_result_window] index level parameter.]; }
从上面的报错信息，可以看到ES提示我结果窗口太大了，目前最大值为10000，而我却要求给我10200。并且在后面也提到了要求我修改index.max_result_window参数来增大结果窗口大小。
修改方法如下：

```python
curl -XPUT http://localhost:9200/indexName/_settings -d '{ "index" : { "max_result_window" : 100000}}'
```
需要注意的是，indexName这里是我ES索引的名字，因此你需要它替换成你对应的索引名称进行修改。

查询
```python
http://192.168.208.52:9200/indexName/_settings/
```



