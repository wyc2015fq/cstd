# tensorboard - qq_30006593的博客 - CSDN博客





2017年08月19日 15:29:01[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：152标签：[tensorflow](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)
个人分类：[tensorflow](https://blog.csdn.net/qq_30006593/article/category/7099875)









### Summary

[operations](https://www.tensorflow.org/api_guides/python/summary)
[例子](https://www.tensorflow.org/get_started/summaries_and_tensorboard)
[GraphKeys](https://www.tensorflow.org/api_docs/python/tf/GraphKeys)：包含一些全局值 
[多gpu](https://stackoverflow.com/questions/43066205/tensorboard-summary-on-multiple-gpus)
[各模块时间和内存占用情况](https://www.tensorflow.org/get_started/graph_viz#runtime_statistics):runtime statistic 
[如何将val和train画到同一张图](https://github.com/frankgu/C3D-tensorflow/blob/develop/train_c3d.py)：同样名字的变量在同一个writer中会添加1，2来区分，要想画在同一张图中，必须用不同的writer写同一个变量。
### 查看结果

tensorboard –logdir your_dir,然后浏览器输入localhost:6006 
[histograms](https://www.tensorflow.org/get_started/tensorboard_histograms)：查看数据(值y)的分布(密度z)随时间(step x)的变换 

fraction_of_xx_full : 栏代表队列的满程度




