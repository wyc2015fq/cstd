# es 分片出现unsigned的解决办法 - sxf_123456的博客 - CSDN博客
2018年09月25日 23:30:00[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：451
- 每天查看ES集群各节点状态(依次点击Montoring->Nodes)![](https://img-blog.csdn.net/20180925232507377?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
之后出现下图
![](https://img-blog.csdn.net/20180925232607548?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
出现unsigned 也可以通过GET _cat/indices 查看unsigned 所在节点
![](https://img-blog.csdn.net/20180925232713494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
查找分片在那个节点上
![](https://img-blog.csdn.net/20180925232006610?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
找到unsigned的节点
![](https://img-blog.csdn.net/20180925232913856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解决办法
POST _cluster/reroute
{
    "commands": [
        {
            "allocate_stale_primary": {
                "index": "m-user-rs-20180902",  #unsigned 的索引
                "shard": 4,  #出现unsigned的切片
                "node": "10.40.10.21", #出现unsigned的节点
                "accept_data_loss": true
            }
        }
    ]
}
