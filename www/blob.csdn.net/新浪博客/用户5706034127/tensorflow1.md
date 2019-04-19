# tensorflow1_用户5706034127_新浪博客
|||
2015年11月9日谷歌开源了人工智能系统TensorFlow，同时成为2015年最受关注的开源项目之一。TensorFlow的开源大大降低了深度学习在各个行业中的应用难度。TensorFlow的近期里程碑事件主要有：
2016年11月09日：TensorFlow开源一周年。
2016年09月27日：TensorFlow支持机器翻译模型。
2016年08月30日：TensorFlow支持使用TF-Slim接口定义复杂模型。
2016年08月24日：TensorFlow支持自动学习生成文章摘要模型。
2016年06月29日：TensorFlow支持Wide & Deep Learning。
2016年06月27日：TensorFlow v0.9发布，改进了移动设备的支持。
2016年05月12日：发布SyntaxNet，最精确的自然语言处理模型。
2016年04月29日：DeepMind模型迁移到TensorFlow。
2016年04月14日：发布了分布式TensorFlow。
2016年谷歌在ICLR（the International Conference on Learning
Representations） Workshop上发表了论文REVISITING DISTRIBUTED SYNCHRONOUS
SGD。基于ImageNet数据集，该论文对异步随机梯度下降法（Async-SGD）和同步随机梯度下降法（Sync-SGD）进行了比较分析。
Dean在2012年提出了分布式随机梯度下降法，模型参数可以分布式地存储在不同的服务器上，称之为参数服务器（Parameter
Server，PS），以及Worker节点可以并发地处理训练数据并且能够和参数服务通信获取模型参数。异步随机梯度下降法（Async-SGD），主要由以下几个步骤组成：
- 针对当前批次的训练数据，从参数服务器获取模型的最新参数。
- 基于上述获取到的模型参数，计算损失方程的梯度。
- 将上述计算得到的梯度发送回参数服务器，并相应地更新模型参数。
同步随机梯度下降法（Sync-SGD）与Sync-SGD的主要差异在于参数服务器将等待所有Worker发送相应的梯度值，并聚合这些梯度值，最后把更新后的梯度值发送回节点。
Async-SGD
的主要问题是每个Worker节点计算的梯度值发送回参数服务器会有参数更新冲突，一定程度影响算法的收敛速度。Sync-SGD算法能够保证在数据集上执行的是真正的随机梯度下降法，消除掉了参数的更新冲突。但同步随机梯度下降法同时带来的问题是训练数据的批量数据会比较大，参数服务器上参数的更新时间依赖于最慢的worker节点。
为了解决有些worker节点比较慢的问题，我们可以使用多一点的Worker节点，这样Worker节点数变为N+N*5%，N为集群Worker节点数。Sync-SGD可以设定为在接受到N个Worker节点的参数后，可以直接更新参数服务器上的模型参数，**进入下一个批次的模型训练。慢节点上训练出来的参数是会被丢弃掉。我们称这种方法为Sync-SGDwith backups。**
2015年，Abadi使用TensorFlow的Async-SGD, Sync-SGD，Sync-SGD with
backups训练模型对ImageNet的Benchmark问题进行了实验分析。要对该训练数据进行1000种图片的分类训练，实验环境为50到200个的worker节点，每个worker节点上运行k40
GPU。使用分布式TensorFlow后大大缩短了模型训练时间，Async-SGD算法实验结果如下，其中200个节点的训练时间比采用25个节点的运算时间缩短了8倍，如下图所示。
![](http://cdn.infoqstatic.com/statics_s1_20170726-0537/resource/articles/deeplearning-tensorflow-casestudy/zh/resources/1205.png)
下图为50个Worker节点的Async-SGD, Sync-SGD，Sync-SGD with
backups模型训练结果的比较。
![](http://cdn.infoqstatic.com/statics_s1_20170726-0537/resource/articles/deeplearning-tensorflow-casestudy/zh/resources/1206.png)
从结果中可以看出增加2个backup节点，Sync-SGD with
backups模型可以快速提升模型训练速度。同时Sync-SGD模型比Async-SGD模型大概提升了25%的训练速度，以及0.48%的精确度。随着数据集的增加，分布式训练的架构变得越来越重要。而分布式TensorFlow正是解决该问题的利器，有效地提升了大规模模型训练的效率，提供了企业级的深度学习解决方案。
