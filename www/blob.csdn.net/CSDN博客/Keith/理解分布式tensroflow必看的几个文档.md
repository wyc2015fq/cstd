# 理解分布式tensroflow必看的几个文档 - Keith - CSDN博客





2016年11月29日 12:44:07[ke1th](https://me.csdn.net/u012436149)阅读数：1584标签：[分布式](https://so.csdn.net/so/search/s.do?q=分布式&t=blog)
个人分类：[tensorflow																[分布式](https://blog.csdn.net/u012436149/article/category/6521834)](https://blog.csdn.net/u012436149/article/category/6461700)








# 理解分布式tensroflow必看的几个文档

## 文档

(1)[Server.md](https://github.com/tensorflow/tensorflow/blob/754048a0453a04a761e112ae5d99c149eb9910dd/tensorflow/g3doc/api_docs/python/functions_and_classes/shard3/tf.train.Server.md)

(2)[Supervisor,md](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/api_docs/python/functions_and_classes/shard6/tf.train.Supervisor.md)

(3)[SyncReplicasOptimizer.md](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/api_docs/python/functions_and_classes/shard3/tf.train.SyncReplicasOptimizer.md)

(4)[mnist_replica.py](https://github.com/tensorflow/tensorflow/blob/r0.11/tensorflow/tools/dist_test/python/mnist_replica.py)
## config

```python
config=tf.ConfigProto(allow_soft_placement=True,log_device_placement=True)
#log_device_placement：是否打印变量的放置信息
#allow_soft_placement:
```





