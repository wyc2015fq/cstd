
# tensorflow Restoring from checkpoint failed - 追求卓越,做到专业 - CSDN博客


2018年12月19日 18:58:02[Waldenz](https://me.csdn.net/enter89)阅读数：878


**发生异常: tensorflow.python.framework.errors_impl.InvalidArgumentError**
Restoring from checkpoint failed. This is most likely due to a mismatch between the current graph and the graph from the checkpoint. Please ensure that you have not altered the graph expected based on the checkpoint. Original error:  Assign requires shapes of both tensors to match. lhs shape= [256,3136] rhs shape= [256,5011]
如图：
![](https://img-blog.csdnimg.cn/20181219185059103.png)
解决办法:
1. 删去之前训练得到的checkpoint，然后重新训练。同时删除之前生成logs文件夹。
2. 或者可以删除某些checkpoint,让程序可以继续从最高的step开始。
![](https://img-blog.csdnimg.cn/2018122014584863.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

