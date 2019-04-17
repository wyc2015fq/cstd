# Group Normalization - 别说话写代码的博客 - CSDN博客





2019年03月06日 15:50:53[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：28标签：[group normalization																[batch normalization](https://so.csdn.net/so/search/s.do?q=batch normalization&t=blog)](https://so.csdn.net/so/search/s.do?q=group normalization&t=blog)
个人分类：[Deep learning](https://blog.csdn.net/qq_21997625/article/category/7283460)








**GN是针对batch normalization(BN)在batch size较小时错误率较高而提出的改进算法**，因为BN层的计算结果依赖当前batch数据，当batch size较小时（比如2,4），该batch数据的均值和方差的代表性较差，这样导致的结果较差，因此对最后的结果也影响较大。而在batch size 较大时（>16）没什么差别。在分类任务一般能包含大batch，但在检测分割及视频相关的算法中，由于输入图像较大，维度多样以及算法本身原因，batch size一般设置比较小，所以此时GN对此改进比较明显。

GN的思想就是要使归一化操作的计算不依赖batch size 的大小。下图LN和IN都可以看做是GN的特殊情况（LN是group=1时候的GN，IN是group=C时候的GN）

![](https://img-blog.csdnimg.cn/2019030615495768.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

因此GN的思想就是在相同feature map的相同group中进行归一化操作，而group只是在channel维度上进行划分，因此归一化操作就和batch size无关。tensorflow代码如下所示：

![](https://img-blog.csdnimg.cn/20190306155014528.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

BN，LN，IN，GN从学术化上解释差异：

BatchNorm：batch方向做归一化，算N*H*W的均值

LayerNorm：channel方向做归一化，算C*H*W的均值

InstanceNorm：一个channel内做归一化，算H*W的均值

GroupNorm：将channel方向分group，然后每个group内做归一化，算(C//G)*H*W的均值




