# es 查询方法 - sxf_123456的博客 - CSDN博客
2018年09月25日 23:38:29[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：124
切记，**第一手资料很重要**！
Elasticsearch出现问题，最高效的解决方案是第一手资料ES英文官网文档，其次是ES英文论坛、ES github issues，再次是stackoverflow等英文论坛、博客。最后才是：Elasticsearch中文社区、其他相关中文技术博客等。
因为：所有的论坛、博客文字都是基于ES英文官方文档再整理，难免有缺失或错误。
![这里写图片描述](https://img-blog.csdn.net/20171104131620265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFveWFuZzM2MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3）自己的Elasticsearch基础原理、Lucene基础知识的不牢固，别无它法，继续深入研究，继续死磕中…….
## 参考
1、官网文档地址：[http://t.cn/RlttuVY](http://t.cn/RlttuVY)
2、Elasticsearch unassigned shards 应急处理方案 ：[http://t.cn/Rlwub5s](http://t.cn/Rlwub5s)
3、解决Unassigned Shards大探讨：[http://t.cn/RlwuVFn](http://t.cn/RlwuVFn)
4、快照&重新存储数据方案：[http://t.cn/RlwuXmm](http://t.cn/RlwuXmm)
## 2018-05方法升级
elasticsearch出现unassigned shards根本原因?
medcl [https://elasticsearch.cn/question/4136](https://elasticsearch.cn/question/4136)回复：
原因肯定是有很多啊，但是要看具体每一次是什么原因引起的，对照表格排查未免不太高效，怎么办？
es 早已帮你想好对策，使用**​ Cluster Allocation Explain API**，会返回集群为什么不分配分片的详细原因，你对照返回的结果，就可以进行有针对性的解决了。
实验一把：
```
GET /_cluster/allocation/explain
{
  "index": "test",
  "shard": 0,
  "primary": false,
  "current_state": "unassigned",
  "unassigned_info": {
    "reason": "CLUSTER_RECOVERED",
    "at": "2018-05-04T14:54:40.950Z",
    "last_allocation_status": "no_attempt"
  },
  "can_allocate": "no",
  "allocate_explanation": "cannot allocate because allocation is not permitted to any of the nodes",
  "node_allocation_decisions": [
    {
      "node_id": "ikKuXkFvRc-qFCqG99smGg",
      "node_name": "test",
      "transport_address": "127.0.0.1:9300",
      "node_decision": "no",
      "deciders": [
        {
          "decider": "same_shard",
          "decision": "NO",
          "explanation": "the shard cannot be allocated to the same node on which a copy of the shard already exists [[test][0], node[ikKuXkFvRc-qFCqG99smGg], [P], s[STARTED], a[id=bAWZVbRdQXCDfewvAbN85Q]]"
        }
      ]
    }
  ]
}
```
--------------------- 本文来自 铭毅天下 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/laoyang360/article/details/78443006?utm_source=copy
