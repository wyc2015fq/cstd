# DL for Vision：A Tutorial with Caffe 报告笔记 - wishchinYang的专栏 - CSDN博客
2015年03月31日 17:34:13[wishchin](https://me.csdn.net/wishchin)阅读数：706
个人分类：[QT./Linux																[AI/ES																[CUDA																[ANN/DNN/纤维丛](https://blog.csdn.net/wishchin/article/category/3109825)](https://blog.csdn.net/wishchin/article/category/1846367)](https://blog.csdn.net/wishchin/article/category/1730787)](https://blog.csdn.net/wishchin/article/category/1721683)
      原文链接：[http://www.shellsec.com/tech/194176.html](http://www.shellsec.com/tech/194176.html)
#### 目录
- 简介 
- 要点记录 
- 提问 
- 总结 
## 简介
报告时间是北京时间 14年12月14日 凌晨一点到两点，主讲人是 Caffe 团队的核心之一 Evan Shelhamer。第一次用 GoToMeeting 参加视频会议，效果真是不错。
报告后分享出了 视频 和 展示[文件](http://www.shellsec.com/) （[链接](http://www.shellsec.com/)在收到邮件后会补上）。
Caffe 此前听过没用过，所以报告前自己试运行了一下，参照 官方教程 。Caffe 安装、上手都很快，Protobuf 式的层定义很直观，模型修改或算法调整变得很容易，相当于只需要改配置[文件](http://www.shellsec.com/)。还找到了他们放在 Google Docs 上一个教程 PPT， DIY Deep
 Learning for Vision: a Hands-On Tutorial with Caffe （已搬到墙里），后来发现这次报告的 PPT 就是在这个基础上修改的。
本次报告主要内容是
- 对机器学习、深度学习的一些介绍，[包](http://www.shellsec.com/)括若干深度学习的经典模型；
- Caffe 的 **优势** （模块化、速度、社区[支持](http://www.shellsec.com/)等）、 
- **基本结构** （网络定义、层定义、Blob等）和 
- **用法** （模型中损失函数、[优化](http://www.shellsec.com/)方法、共享权重等的配置、应用举例、参数调优的[技巧](http://www.shellsec.com/)），
- 以及 **未来方向** （CPU/GPU 并行化、Pythonification、Fully Convolutional Networks等）。
以下是报告中的截图配上自己的一点笔记，一手资料请参见上面给出的会后分享[链接](http://www.shellsec.com/)。
## 要点记录
PPT 的首页取自该项目的一个在线 demo ，输入图片 url，识别物体类别。 
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/j2yQfqz.png)
一个示例， 看一下效果：
左边是浅层特征，各类别物体杂乱无章；右边是深度特征，一些类别有较为明显的分别。
特别地， **dog、bird、invertebrate** 这三类动物类别离得较近，而 **building、vehicle、commodity** 这类无生命类别离得较近，可见深度特征的强大。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/MVZfMjZ.png)
此外，在深层结构当中， **隐层神经元的激活可能与特定的物体类别有关** ，
比如有的神经元对人像敏感，而有的对数字或建筑物敏感，最下面一层是闪光灯（或与之类似，比如反光的脑门……）效果。 
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/RFNRne.png)
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/NBFNBv.png)
Caffe 的优势，网络结构的模块化和易表达是显然的，社区资源也同样强大，比如下两页内容。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/am22y2F.png)
Caffe 的 Reference Models 可供学术使用，比如 AlexNet、R-CNN、CaffeNet，[包](http://www.shellsec.com/)括模型定义、[优化](http://www.shellsec.com/)方法和预训练权重。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/6Vrymq.png)
Model Zoo 中有用户贡献的模型可供参考使用，比如 VGG、Network-in-Network。 
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/vAzuyma.png)
Caffe [支持](http://www.shellsec.com/)丰富的模型表达形式，包括 DAGs、Weight Sharing 以及 Siamese Network。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/IzYFvmB.png)
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/MNNrMv.png)
网络和层定义采用 protobuf 的样式。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/qUjU7bM.png)
Layer 指的是权重和偏置，可以定义连接数、权重初始化方法等。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/aeyiya.png)
Blob 是四维[数据](http://www.shellsec.com/)结构，保存节点上的数值以及模型参数，可以通过[编程](http://www.shellsec.com/)在
 CPU 和 GPU 间传输。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/7zYvEj.png)
模型定义之外，还需要一个指定[优化](http://www.shellsec.com/)策略的配置[文件](http://www.shellsec.com/)，用以训练模型。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/MzAnQnb.png)
使用 Caffe 训练的一般步骤就是
- [数据](http://www.shellsec.com/)预处理；
- 模型定义；
- 求解策略定义；
- 运行。
此处给出了两个例子， Logistic Regression ， Learn LeNet on MNIST ，都很好 follow。 
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/bMZVJj.png)
调参中重点讲了一个模型迁移 的实例，用某项任务已有模型的参数作为新任务模型的参数初始值，然后进行模型训练。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/mYFNna.png)
模型训练一般由浅入深，逐步降低学习速率，以保持预训练参数的某些性质。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/Aj63Ef.png)
接下来具体讲述了 Loss、Solver、DAG、Weight Sharing 的概念和配置。
对同一模型，不同 Solver 的表现有差。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/ieQNne.png)
一般深度学习模型是线性形式的，比如 LeNet，而 Caffe [支持](http://www.shellsec.com/)**DAG 形式**的模型。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/EjiQZn.png)
Caffe 的近期动向，CPU/GPU 并行化、Pythonification、Fully Convolutional Networks等。
使用CUDA进行并行化加速........
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/ayieeyB.png)
Caffe 的团队，拜 Yangqing Jia 师兄……
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/2MBVfu.png)
文献参考。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/n632eq.png)
## 提问
语音回答中，Evan 提到 UCB 的一个团队正在开发 Scala 接口，不过尚属实验性质；
Caffe 团队在考虑 和 UCB 的 AMP 团队合作，扩展到 Spark 这一计算平台上；
除了已支持的 CPU/GPU 计算，也考虑扩展支持 OpenCl；
对于 Theano、Torch，鼓励大家尝试、比较……
文字问答如下，由 Yangqing Jia 回复。
Q: Is the pre-trained model avaialbe for down[load](http://www.shellsec.com/) to accelerate our work on other kinds of images?
A:FYI – for pretrained models that we release, please refer to the model zoo page here:[http://](http://www.shellsec.com/)caffe.berkeleyvision.org/model_zoo.html
#### Q: [Android](http://www.shellsec.com/) platform ?
A:People have asked about android/ios platforms. In principle this is possible since the code is purely in C, but of course some engineering efforts are needed to write make[file](http://www.shellsec.com/)s
 like [Android](http://www.shellsec.com/).mk for this. Our bandwidth is limited and we are focusing on the research part, but we welcome pull requests on github if
 you write one (and we thank you in advance)! Also, kindly check out the blog post by my colleague Pete Warden about our efforts on running with Jetson TK1:[http://](http://www.shellsec.com/)petewarden.com/2014/10/25/how-to-run-the-caffe-deep-learning-vision-library-on-nvidias-jetson-mobile-gpu-board/
Q: Can you discuss status and/or considerations for adding opencl support (and so be vendor neutral, as opposed to NVIDIA CUDA)?
A:In terms of using OpenCL – it has been under discussion for a while, but we are kind of shortstaffed so we focus more on the research side – we welcome contributions from open-source communities of course, please join us at github![:)](http://www.shellsec.com/wp-content/themes/d8/img/smilies/simple-smile.png)
#### Q: do you have an online examples of unsupervised losses
A:For unsupevised losses and training there is a bundled example of an MNIST autoencoder.
## 总结
“盗取”一页 PPT 作为本文总结。
![DIY Deep Learning for Vision: A Tutorial with Caffe 报告笔记](http://img.shellsec.com/wp-content/uploads/2014/12/QZz6fmf.png)
