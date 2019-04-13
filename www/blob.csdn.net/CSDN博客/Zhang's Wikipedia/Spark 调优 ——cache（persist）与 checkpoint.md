
# Spark 调优 ——cache（persist）与 checkpoint - Zhang's Wikipedia - CSDN博客


2019年01月13日 22:07:42[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：98


cache 是对 persist 的进一步调用；
## 1. 为什么要 checkpoint
为了保证数据安全性，需要对运行出的中间结果进行 checkpoint最好将结果 checkpoint 到 hdfs，便于集群所有节点进行访问；
checkpoint 之前先进行 cache（persist），将数据放在缓存中
什么时候 checkpoint：在发生 shuffle 之后做 checkpoint

## 2. checkpoint 的步骤
建立 checkpoint 存储目录：sc.setCheckpointDir(“hdfs://node01.9000/ck”)
rdd1.cache()
rdd1.checkpoint()

