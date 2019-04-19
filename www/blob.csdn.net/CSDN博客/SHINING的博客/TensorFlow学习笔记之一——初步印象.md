# TensorFlow学习笔记之一——初步印象 - SHINING的博客 - CSDN博客
2016年05月06日 14:25:58[snsn1984](https://me.csdn.net/snsn1984)阅读数：10828
所属专栏：[Tensorflow学习](https://blog.csdn.net/column/details/tensorflow.html)
最近工作太忙，赶上孩子刚刚出生不久，简直是忙的四脚朝天。对技术的跟踪、论文的阅读和代码的编写，都拉下了一段时间。跟朋友聊天的时候得知TensorFlow，而且是朋友极力推荐的开源项目，马上关注了一下。
知道TensorFlow是Google的项目，我就马上去找到了关于TensorFlow的论文：
地址：[http://download.tensorflow.org/paper/whitepaper2015.pdf](http://download.tensorflow.org/paper/whitepaper2015.pdf)
中文地址：[http://www.jianshu.com/p/65dc64e4c81f](http://www.jianshu.com/p/65dc64e4c81f)
Google一直保持这样的传统，在关键项目的发布和开源上，会有关键性的重要论文发布。这个论文将是理解这个项目的重中之重。花了一些时间在这个论文上面，得到一些关于TensorFlow的初步印象，简列如下：
1、TensorFlow是支持各种平台的，包括手机端、PC、大规模分布式系统和GPU集群；
2、TensorFlow可以支持异构硬件平台；
3、TensorFlow的计算是用有向图描述的；
4、TensorFlow支持的前端语言有C++和Python;
5、TensorFlow的node具有零个到多个输入和输出，表现为一个操作的实例；
6、Kernel是一个操作的具体实现；
7、Session负责Client System和TensorFlow的通信，Session会有一个run接口；
8、TensorFlow的大多数用户会针对一个图建立一个Session，但是会把这个图或者这个图的子集图运行无数遍；
9、TensorFlow的实现分为本地版本和分布式版本，本地版本又包括单设备版本和多设备版本；
10、TensorFlow的多设备版本和分布式版本，需要考虑节点分配和跨设备数据交流。
这只是简单的一个初步印象，不排除有理解不到位的地方，欢迎探讨。
