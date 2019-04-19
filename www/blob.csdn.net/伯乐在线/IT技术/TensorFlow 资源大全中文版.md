# TensorFlow 资源大全中文版 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [汀谷](http://www.jobbole.com/members/yalye) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[jtoy](https://github.com/jtoy/awesome-tensorflow/blob/master/README.md)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
jtoy 发起整理的 TensorFlow 资源，包含一些很棒的 TensorFlow 工程、库、项目等。
### 什么是TensorFlow？
TensorFlow 是一个采用数据流图（data flow graphs），用于数值计算的开源软件库。节点（Nodes）在图中表示数学操作，图中的线（edges）则表示在节点间相互联系的多维数据数组，即张量（tensor）。它灵活的架构让你可以在多种平台上展开计算，例如台式计算机中的一个或多个CPU（或GPU）、服务器、移动设备等等。TensorFlow 最初由Google Brain 小组（隶属于Google机器智能研究机构）的研究员和工程师们开发出来，用于机器学习和深度神经网络方面的研究，但这个系统的通用性使其也可广泛用于其他计算领域。
### 教程
- [TensorFlow 教程1](https://github.com/pkmital/tensorflow_tutorials) – 从基础到有趣的TensorFlow程序
- [TensorFlow 教程2](https://github.com/nlintz/TensorFlow-Tutorials) – 介绍基于谷歌TensorFlow框架的深度学习，其中有些教程是学习了Newmu的Theano教程
- [TensorFlow 实例](https://github.com/aymericdamien/TensorFlow-Examples) – TensorFlow教程以及一些新手的代码实例
- [Sungjoon的TensorFlow-101](https://github.com/sjchoi86/Tensorflow-101) – 在Jupyter Notebook上用python写的TensorFlow教程
- [Terry Um的TensorFlow练习](https://github.com/terryum/TensorFlow_Exercises) – 根据其他TensorFlow项目再创作的代码
- [在树莓派3上安装TensorFlow](https://github.com/samjabrahams/tensorflow-on-raspberry-pi) – 在树莓派上正确安装和运行TensorFlow
- [时间序列上的分类](https://github.com/guillaume-chevalier/LSTM-Human-Activity-Recognition) – 在TensorFlow上的基于手机传感数据的LSTM循环神经网络
### 模型/工程
- [图片形态转换](https://github.com/yunjey/domain-transfer-network) – 无监督图片形态转换的实现
- [Show, Attend and Tell算法](https://github.com/yunjey/show-attend-and-tell) -基于聚焦机制的自动图像生成器
- [Neural Style](https://github.com/cysmith/neural-style-TensorFlow) – Neural Style 算法的TensorFlow实现
- [Pretty Tensor](https://github.com/google/prettytensor) – Pretty Tensor提供了高级别的TensorFlow封装接口
- [Neural Style](https://github.com/anishathalye/neural-style) – neural style的又一实现
- [AlexNet3D](https://github.com/denti/AlexNet3D) – 用3D卷积层实现AlexNet
- [TensorFlow笔记](https://github.com/samjabrahams/tensorflow-white-paper-notes) – TensorFlow的学习笔记和总结，附带一些图片说明
- [NeuralArt](https://github.com/ckmarkoh/neuralart_tensorflow) – 艺术风格绘画的神经网络算法TensorFlow实现
- [DQN玩乒乓](http://www.danielslater.net/2016/03/deep-q-learning-pong-with-tensorflow.html)
- [TensorFlow生成手写体](https://github.com/hardmaru/write-rnn-tensorflow) – 实现Alex Grave的论文中关于生成手写体的部分
- [TensorFlow实现神经图灵机](https://github.com/carpedm20/NTM-tensorflow) – TensorFlow实现神经图灵机
- [基于物体搜索和过滤视频](https://github.com/agermanidis/thingscoop) – 使用卷积神经网络基于视频中的物品、地点等来搜索、过滤和描述视频
- [使用TensorFlow来转换莎士比亚作品和现代版本的英语](https://github.com/tokestermw/tensorflow-shakespeare) – 实现莎士比亚作品和现代版本的英语的单语转换
- [聊天机器人](https://github.com/Conchylicultor/DeepQA) – 一个基于深度学习的聊天机器人
- [colornet](https://github.com/pavelgonchar/colornet) – 使用神经网络给灰度图像着色
- [图像生成器](https://github.com/jazzsaxmafia/show_attend_and_tell.tensorflow) – Show and Tell算法实现
- [Attention based的自动图像生成器](https://github.com/jazzsaxmafia/show_attend_and_tell.tensorflow) – Show, Attend and Tell算法实现
- [Weakly_detector](https://github.com/jazzsaxmafia/Weakly_detector) – 用于定位的深度特征
- [Dynamic Capacity Networks](https://github.com/jazzsaxmafia/dcn.TensorFlow) – DCN的TensorFlow实现
- [TensorFlow实现HMM](https://github.com/dwiel/tensorflow_hmm) – 实现HMM的维特比算法和前后向算法
- [DeepOSM](https://github.com/trailbehind/DeepOSM) – 使用OpenStreetMap和卫星图像训练深度学习网络
- [DQN-tensorflow](https://github.com/devsisters/DQN-tensorflow) – TensorFlow通过OpenAI Gym实现深度学习来实现“深度强化学习下达到人类水平的控制”
- [Highway Networks](https://github.com/fomorians/highway-cnn) – 使用TensorFlow和Fomoro进行简单的超深度网络训练
- [用CNN做句子分类](https://github.com/dennybritz/cnn-text-classification-TensorFlow) – 用TensorFlow实现句子分类的卷积神经网络
- [End-To-End Memory Networks](https://github.com/domluna/memn2n) – 使用TensorFlow实现End-To-End的Memory Network
- [Character-Aware的神经语言模型](https://github.com/carpedm20/lstm-char-cnn-tensorflow) – 基于字符感知的LSTM语言模型
- [YOLO TensorFlow ++](https://github.com/thtrieu/darkflow) – TensorFlow实现YOLO实时物体检测，支持实时运行在移动设备上
- [Wavenet](https://github.com/ibab/tensorflow-wavenet) – TensorFlow实现用来生成音频的WaveNet对抗生成网络架构
- [Mnemonic Descent Method](https://github.com/trigeorgis/mdm) – TensorFlow实现助记符下降法：重现端对端的人脸对齐
### 由TensorFlow提供技术支持
- [YOLO TensorFlow](https://github.com/gliese581gg/YOLO_tensorflow) – 实现YOLO:实时物体检测
- [android-yolo](https://github.com/natanielruiz/android-yolo) – 在安卓设备商使用YOLO实行实时物体检测，由TensorFlow提供技术支持
- [Magenta](https://github.com/tensorflow/magenta) – 在制作音乐和艺术中使用机器智能提升艺术形态（研究项目）
### 库
- [Scikit Flow (TensorFlow Learn)](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/learn/python/learn) – 深度/机器学习的简化版接口（现在是TensorFlow的一部分）
- [tensorflow.rb](https://github.com/somaticio/tensorflow.rb) – TensorFlow的Ruby版本，使用了SWIG
- [TensorFlowlearn](https://github.com/TensorFlowlearn/TensorFlowlearn) – 有高级别API的深度学习库
- [TensorFlow-Slim](https://github.com/tensorflow/models/tree/master/inception/inception/slim) – TensorFlow中的高级别简化版库
- [TensorFrames](https://github.com/databricks/tensorframes) – Apache Spark上DataFrames的TensorFlow封装
- [caffe-tensorflow](https://github.com/ethereon/caffe-tensorflow) – 转换Caffe模型为TensorFlow的模式
- [keras](https://keras.io/) – 用于TensorFlow和Theano的小型的模块化的库
- [SyntaxNet 语法分析神经网络模型](https://github.com/tensorflow/models/tree/master/syntaxnet) – 全球标准化的Transition-Based神经网络模型的TensorFlow实现
- [keras-js](https://github.com/transcranial/keras-js) – 在GPU的支持下，在浏览器中运行Keras模型
- [NNFlow](https://github.com/welschma/NNFlow) – 一个简单的框架，可以将ROOT NTuples转换成可以在TensorFlow使用的Numpy数据
### 视频
- [TensorFlow Guide 1](https://www.youtube.com/watch?v=_ODQ-mV3H9Q) – TensorFlow的安装和使用指南1
- [TensorFlow Guide 2](https://www.youtube.com/watch?v=0Xjj8SQKdDw) – TensorFlow的安装和使用指南2
- [TensorFlow Basic Usage](https://www.youtube.com/watch?v=yh8oRFw3vDc) – 基本使用指南
- [TensorFlow Deep MNIST for Experts](https://www.youtube.com/watch?v=kMts0baddow) – 深入了解MNIST
- [TensorFlow Udacity Deep Learning](https://www.youtube.com/watch?v=ReaxoSIM5XQ) – 在有1GB数据的Cloud 9在线服务安装TensorFlow的步骤
- [为什么谷歌希望所有人都可以访问TensorFlow](http://video.foxnews.com/v/4611174773001/why-google-wants-everyone-to-have-access-to-tensorflow/?#sp=show-clips)
- [1/19/2016TensorFlow 硅谷见面会](http://blog.altoros.com/videos-from-tensorflow-silicon-valley-meetup-january-19-2016.html)
- [1/21/2016TensorFlow 硅谷见面会](https://blog.altoros.com/videos-from-tensorflow-seattle-meetup-jan-21-2016.html)
- [19th Apr 2016 斯坦福CS224d 第七课 – TensorFlow介绍](https://www.youtube.com/watch?v=L8Y2_Cq2X5s&index=7&list=PLmImxx8Char9Ig0ZHSyTqGsdhb9weEGam) – CS224d 用于自然语言处理的深度学习 By Richard Socher
- [通过TensorFlow了解机器学习](https://www.youtube.com/watch?v=GZBIPwdGtkk&feature=youtu.be&list=PLBkISg6QfSX9HL6us70IBs9slFciFFa4W) – Pycon 大会，2016年
- [使用TensorFlow的大规模深度学习](https://www.youtube.com/watch?v=XYwIDn00PAo&feature=youtu.be) – Jeff Dean在Spark Summit 2016上的演讲
- [TensorFlow和深度学习](https://www.youtube.com/watch?v=vq2nnJ4g6N0)
### 论文
- [TensorFlow: Large-Scale Machine Learning on Heterogeneous Distributed Systems](http://download.tensorflow.org/paper/whitepaper2015.pdf) – 论文介绍了TensorFlow的接口以及我们在google上构建的这些接口的实现
- [TensorFlow.Learn: TensorFlow’s High-level Module for Distributed Machine Learning](https://arxiv.org/abs/1612.04251) – TensorFlow用于分布式机器学习的高级别模块
- [Comparative Study of Deep Learning Software Frameworks](https://arxiv.org/abs/1511.06435) – 这个研究运行在不同的深度学习架构上，我们也评估在单机上使用CPU和GPU配置时同一框架的性能
- [Distributed TensorFlow with MPI](https://arxiv.org/abs/1603.02339) – 在论文中，我们使用MPI将TensorFlow在大规模集群中扩展
- [Globally Normalized Transition-Based Neural Networks](https://arxiv.org/abs/1603.06042) – 这个论文介绍了[SyntaxNet](https://github.com/tensorflow/models/tree/master/syntaxnet)背后的模型
- [TensorFlow: A system for large-scale machine learning](https://arxiv.org/abs/1605.08695) – 这个论文展示了TensorFlow的数据流模型并与存在的系统进行对比，展现了它引人注目的性能
### 官方公告
- [TensorFlow: smarter machine learning, for everyone](https://googleblog.blogspot.com/2015/11/tensorflow-smarter-machine-learning-for.html) – 介绍了TensorFlow
- [Announcing SyntaxNet: The World’s Most Accurate Parser Goes Open Source](https://research.googleblog.com/2016/05/announcing-syntaxnet-worlds-most.html) – SyntaxNet的发布公告“一个基于TensorFlow的开源神经网络系统，为自然语言理解系统打下了基础”
### 博文
- [Why TensorFlow will change the Game for AI](http://www.somatic.io/blog/why-tensorflow-will-change-the-game-for-ai) – 为什么TensorFlow会改变游戏的AI
- [TensorFlow for Poets](https://petewarden.com/2016/02/28/tensorflow-for-poets/) – 了解TensorFlow的实现
- [Introduction to Scikit Flow – Simplified Interface to TensorFlow](http://terrytangyuan.github.io/2016/03/14/scikit-flow-intro/) – Scikit Flow简介，它简化了TensorFlow的接口
- [Building Machine Learning Estimator in TensorFlow](http://terrytangyuan.github.io/2016/07/08/understand-and-build-tensorflow-estimator/) – 了解TensorFlow的内部学习评估器
- [TensorFlow – Not Just For Deep Learning](http://terrytangyuan.github.io/2016/08/06/tensorflow-not-just-deep-learning/) – TensorFlow,不仅仅为了深度学习
- [The indico Machine Learning Team’s take on TensorFlow](https://indico.io/blog/indico-tensorflow) – indico机器学习团队采纳TensorFlow
- [The Good, Bad, & Ugly of TensorFlow](https://indico.io/blog/the-good-bad-ugly-of-tensorflow/) – 为期六个月快速演进的报告（一个小贴士和窍门来弥补TensorFlow的不足）
- [Fizz Buzz in TensorFlow](http://joelgrus.com/2016/05/23/fizz-buzz-in-tensorflow/) – Joel Grus的一个玩笑
- [RNNs In TensorFlow, A Practical Guide And Undocumented Features](http://www.wildml.com/2016/08/rnns-in-tensorflow-a-practical-guide-and-undocumented-features/) – 基于TensorFlow的RNN实现，Github上提供了步骤和全套代码
- [使用TensorBoard来可视化TensorFlow训练的图片分类](http://maxmelnick.com/2016/07/04/visualizing-tensorflow-retrain.html)
- [TensorFlowRecords Guide](http://warmspringwinds.github.io/tensorflow/TensorFlow-slim/2016/12/21/TensorFlowrecords-guide/) – 语义分割和处理TensorFlowRecord文件格式
### 社区
- [Stack Overflow](http://stackoverflow.com/questions/tagged/tensorflow)
- [@TensorFlo on Twitter](https://twitter.com/TensorFlo)
- [Reddit](https://www.reddit.com/r/tensorflow/)
- [Mailing List](https://groups.google.com/a/tensorflow.org/forum/#!forum/discuss)
### 书
- 《[First Contact with TensorFlow](http://www.jorditorres.org/first-contact-with-tensorflow/) – 第一次接触TensorFlow》。作者 Jordi Torres 是UPC Barcelona Tech 的教授。也是 Barcelona 超级计算中心的研究经理和高级顾问
- 《[Deep Learning with Python](https://machinelearningmastery.com/deep-learning-with-python/) – 深度学习之python》，使用Keras在Theano和TensorFlow开发深度学习模型
- 《[TensorFlow for Machine Intelligence](https://bleedingedgepress.com/tensor-flow-for-machine-intelligence/) – TensorFlow实现机器智能》，提供完成的教程，从基本的图运算到在实际应用中制造深度学习模型
- 《[Getting Started with TensorFlow](https://www.packtpub.com/big-data-and-business-intelligence/getting-started-tensorflow) – TensorFlow入门》，开始学习并使用谷歌最新的数据计算库TensorFlow来深度分析数据
- 《[Hands-On Machine Learning with Scikit-Learn and TensorFlow](http://shop.oreilly.com/product/0636920052289.do) – 通过Scikit-Learn和TensorFlow来实践机器学习》，覆盖了机器学习的基础、训练以及在多个服务器和GPU上部署深度学习网络，以及CNN、RNN、自动编码器和Deep Q.
- 《[Building Machine Learning Projects with Tensorflow](https://www.packtpub.com/big-data-and-business-intelligence/building-machine-learning-projects-tensorflow)》 – 这本书包含了多个不同类型的工程，来说明TensorFlow在不同场景中的应用，这本书的工程包括了训练模型、机器学习、深度学习以及多种神经网络，每个工程都是一个精妙的、有意义的项目，会教我们如何使用TensorFlow并在使用中如何对数据分层
有些python相关的库是[从vinta那儿拷贝](https://github.com/jobbole/awesome-python-cn)的
go相关的资源是从[这儿](https://github.com/golang/go/wiki/Projects#Machine_Learning)获取的
