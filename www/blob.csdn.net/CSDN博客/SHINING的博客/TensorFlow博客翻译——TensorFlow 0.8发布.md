# TensorFlow博客翻译——TensorFlow 0.8发布 - SHINING的博客 - CSDN博客
2016年05月06日 20:44:29[snsn1984](https://me.csdn.net/snsn1984)阅读数：6251

## [原文地址](http://googleresearch.blogspot.jp/2016/04/announcing-tensorflow-08-now-with.html?)
## [Announcing TensorFlow 0.8 – now with distributed computing support!](http://googleresearch.blogspot.jp/2016/04/announcing-tensorflow-08-now-with.html)
Wednesday, April 13, 2016
Posted by Derek Murray, Software Engineer
TensorFlow0.8发布——目前可以支持分布式计算！
Google uses machine learning across a wide range of its products. In order to continually improve our models, it's crucial that the training process be as fast as possible. One way to do this is to run [TensorFlow](https://www.tensorflow.org/) across hundreds of machines, which shortens the training process for some models from weeks to hours, and allows us to experiment with models of increasing size and sophistication. Ever since we released TensorFlow as an open-source project, distributed training support has been one of the most requested features. Now the wait is over. 
Google在一系列的产品中都使用了机器学习。为了继续发展我们的模式，训练过程尽可能的快就显得十分重要。其中的一个办法就是在成百上千台机器上运行TensorFlow，这将把一些模型的训练时间从几周缩短到几个小时，并且能有条件让我们去试验更大和更复杂的模型。甚至当我们把TensorFlow开源之后，对分布式训练的支撑也成为了呼声最高的特征之一。现在，等待结束了。
Today, we're excited to release TensorFlow 0.8 with distributed computing support, including everything you need to train distributed models on your own infrastructure. Distributed TensorFlow is powered by the high-performance [gRPC](http://www.grpc.io/) library, which supports training on hundreds of machines in parallel. It complements our recent announcement of [Google Cloud Machine Learning](http://googleresearch.blogspot.com/2016/03/machine-learning-in-cloud-with.html), which enables you to train and serve your TensorFlow models using the power of the Google Cloud Platform.
今天，我们非常兴奋的发布可以支持分布式计算的TensorFlow0.8，包含了你在自己的架构上去训练分布式模型所需的一切。分布式的TensorFlow是通过高性能的gPRC库驱动的，它可以支持数以百计的机器并行进行训练。它补充了我们最近宣布的Google云机器学习，这将确保你可以使用Google云平台的力量去训练和服务你的TensorFlow模型。
To coincide with the TensorFlow 0.8 release, we have published a [distributed trainer](https://github.com/tensorflow/models/tree/master/inception) for the I[nception image classification](http://googleresearch.blogspot.com/2016/03/train-your-own-image-classifier-with.html) neural network in the TensorFlow models repository. Using the distributed trainer, we trained the Inception network to 78% accuracy in less than 65 hours using 100 GPUs. Even small clusters—or a couple of machines under your desk—can benefit from distributed TensorFlow, since adding more GPUs improves the overall throughput, and produces accurate results sooner.
为了和TensorFlow0.8的发布相呼应，我们发布了一个分布式的训练者，为了TensorFlow模型包里的Inception图片分类神经网络。使用分布式的训练者，我们训练开始网络达到78%的精确度，在采用100个GPU的情况下只用了不到65个小时。甚至更小的集群，或者是你书桌下的几台机器也可以从分布式TensorFlow里获取益处，因此添加更多的GPU可以增加吞吐量，并且可以更快的产生更精确的结果。
![](https://img-blog.csdn.net/20160506213238907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
TensorFlow can speed up Inception training by a factor of 56, using 100 GPUs.
The distributed trainer also enables you to scale out training using a cluster management system like [Kubernetes](http://kubernetes.io/). Furthermore, once you have trained your model, you can deploy to production and[speed up inference using TensorFlow Serving on Kubernetes](http://blog.kubernetes.io/2016/03/scaling-neural-network-image-classification-using-Kubernetes-with-TensorFlow-Serving.html).
分布式训练者也可以确保你使用一个像Kubernets的集群管理系统去度量我们的训练。此外，一旦你已经训练好你的模型，你可以部署为产品，并且可以使用TensorFlow在Kubernetes上的服务去加速推理。
Beyond distributed Inception, the 0.8 release includes [new libraries](https://www.tensorflow.org/api_docs/python/train.html#distributed-execution) for defining your own distributed models. TensorFlow's distributed architecture permits a great deal of flexibility in defining your model, because every process in the cluster can perform general-purpose computation. Our previous system [DistBelief](http://research.google.com/archive/large_deep_networks_nips2012.html)(like many systems that have followed it) used special "parameter servers" to manage the shared model parameters, where the parameter servers had a simple read/write interface for fetching and updating shared parameters. In TensorFlow, all computation—including parameter management—is represented in the dataflow graph, and the system maps the graph onto heterogeneous devices (like multi-core CPUs, general-purpose GPUs, and mobile processors) in the available processes. To make TensorFlow easier to use, we have included Python libraries that make it easy to write a model that runs on a single process and scales to use multiple replicas for training. 
超过分布式的开端的是，0.8的发布包括了定义你自己的分布式模型的一些新库。TensorFlow的分布式架构允许定义你自己的模型的时候有一系列的可伸缩性，因为集群里的每一个进程都可以表现为特殊目的的计算。我们之前的系统DistBelief（像很多跟随它的系统一样）使用特殊的参数服务者去管理共享的模型参数，这种参数服务者有一个简单的读/写接口用来获取和更新共享的参数。在TensorFlow里，所有的计算包含参数的管理，都表现在数据流图里，并且系统规划这些图到异构设备上（像多核的CPU，特殊目的的GPU，和移动处理器）在可获得进程里。为了确保TensorFlow更容易使用，我们已经包含了一些Python库，这些库将很容易编写一个运行在单独进程上，并且可以通过多个副本进行训练以扩大训练规模的模型。
This architecture makes it easier to scale a single-process job up to use a cluster, and also to experiment with novel architectures for distributed training. As an example, my colleagues have recently shown that [synchronous SGD with backup workers](http://arxiv.org/abs/1604.00981), implemented in the TensorFlow graph, achieves improved time-to-accuracy for image model training.
这个架构使得它可以更加容易的从运用单进程扩大到使用集群，并且易于使用新架构去试验分布式训练。作为一个例子，我的大学同学最近展示了使用备份功能的同步SGD，它是用TensorFlow图实现的，成功的提高了图像模型训练的时间和精确比。
The current version of distributed computing support in TensorFlow is just the start. We are continuing to research ways of improving the performance of distributed training—both through engineering and algorithmic improvements—and will share these improvements with the community [on GitHub](https://github.com/tensorflow/tensorflow). However, getting to this point would not have been possible without help from the following people:
TensorFlow所支持的目前的分布式计算版本才刚刚开始。我们将继续研究提高分布式训练的性能的方法，从工程和算法的两个角度推动，并且将这些进展在GitHub的社区分享出来。不管怎么说，达到目前这样的程度，无论如何也是少不了如下这些人的帮助：
- TensorFlow training libraries - Jianmin Chen, Matthieu Devin, Sherry Moore and Sergio Guadarrama
- TensorFlow core - Zhifeng Chen, Manjunath Kudlur and Vijay Vasudevan
- Testing - Shanqing Cai
- Inception model architecture - Christian Szegedy, Sergey Ioffe, Vincent Vanhoucke, Jonathon Shlens and Zbigniew Wojna
- Project management - Amy McDonald Sandjideh
- Engineering leadership - Jeff Dean and Rajat Monga
TensorFlow训练库——Jianmin Chen, Matthieu Devin, Sherry Moore and Sergio Guadarrama
TensorFlow内核——Zhifeng Chen, Manjunath Kudlur and Vijay Vasudevan
测试——Shanqing Cai
Inception模型架构——Christian Szegedy, Sergey Ioffe, Vincent Vanhoucke, Jonathon Shlens and Zbigniew Wojna
项目管理——Amy McDonald Sandjideh
工程领导——Jeff Dean and Rajat Monga
