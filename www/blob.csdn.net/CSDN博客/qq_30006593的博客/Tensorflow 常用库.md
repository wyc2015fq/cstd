# Tensorflow 常用库 - qq_30006593的博客 - CSDN博客





2017年08月14日 16:30:40[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：381








## **Tensorflow 常用函数**

### tf.app

[.flag](http://blog.csdn.net/leiting_imecas/article/details/72367937)：定义command参数 
[.run](http://blog.csdn.net/helei001/article/details/51859423)：解析flag

### tf.张量操作

[expand_dims](http://blog.csdn.net/qq_31780525/article/details/72280284)：增加一维 
[concat](http://blog.csdn.net/helei001/article/details/51363706)：连接张量 
[transpose](https://www.tensorflow.org/api_docs/python/tf/transpose)：调换维度 
[reshape](https://www.tensorflow.org/api_docs/python/tf/reshape)：变形 
[zeros_like](https://www.tensorflow.org/api_docs/python/tf/zeros_like)：赋值为0 
[tile](https://www.tensorflow.org/versions/r0.12/api_docs/python/array_ops/slicing_and_joining): 复制，配合reshape使用 
[stack]() : 多个tensor合并为一个tensor 
[scatter_add]()：相加但只更新某一部分 

reduce_***：展开后操作
### tf.get_variable

[用于使用共享变量](http://blog.csdn.net/u012436149/article/details/53696970)

### tf.Graph().as_default

[用于图的上下文管理](http://blog.csdn.net/xierhacker/article/details/53860379)

### tf.train

[in_top_k] 

[one_hot]：数字转换为one hot向量，计算损失用 

[各种损失函数]
#### Optimizer

[] 
[各种optimizer比较](http://blog.csdn.net/liyuan123zhouhui/article/details/68946448)

#### Learning Rate

[Decay衰减](http://www.jianshu.com/p/f9f66a89f6ba)

### tf.sparse_ops

[sparse_to_dense](https://www.tensorflow.org/api_docs/python/tf/sparse_to_dense)





