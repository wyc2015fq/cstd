# 斯坦福CS231n Spring 2017开放全部课程视频 - 知乎
# 



CS231n近几年一直是计算机视觉领域和深度学习领域最为经典的课程之一。而最近刚刚结课的CS231n Spring 2017 仍由李飞飞主讲，并邀请了Goodfellow等人对其中部分章节详细介绍。本课程从计算机视觉的基础概念开始，在奠定了基本分类模型、神经网络和优化算法的基础后，详细介绍了CNN、RNN、GAN、RL等深度模型在计算机视觉上的应用。前天，斯坦福开放了该课程的全部视频，并且还有配套英文字幕。因此，CS231n 2017 春季课程包括 PPT 和视频在内的所有教学资料都已开放。机器之心将为各位读者介绍该课程，并提供相应的资源，Bilibili视频地址由微博知名博主爱可可老师提供。




课件地址：[Index of /slides/2017](https://link.zhihu.com/?target=http%3A//cs231n.stanford.edu/slides/2017/)

课程视频地址：[https://www.youtube.com/playlist?list=PL3FW7Lu3i5JvHM8ljYj-zLfQRF3EO8sYv](https://link.zhihu.com/?target=https%3A//www.youtube.com/playlist%3Flist%3DPL3FW7Lu3i5JvHM8ljYj-zLfQRF3EO8sYv)

Bilibili视频地址：[斯坦福2017季CS231n深度视觉识别课程视频（by Fei-Fei Li, Justin Johnson, Serena Yeung）（英文字幕）](https://link.zhihu.com/?target=http%3A//www.bilibili.com/video/av13260183/%23page%3D1)



![](https://pic3.zhimg.com/v2-055281760ba1089ae88143fa7731c052_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1136' height='868'></svg>)



以下为该课程的内容大纲




**Lecture 1：计算机视觉的概述、历史背景以及课程计划**



![](https://pic3.zhimg.com/v2-7e871f729955c5c99b6fa118894d5ece_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1054' height='655'></svg>)
**Lecture 2：图像分类——包括数据驱动（data-driven）方法，K 近邻方法（KNN）和线性分类（linear classification）方法**



![](https://pic1.zhimg.com/v2-9f89291ef3c865393c36338de9ceb67c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1420' height='595'></svg>)
**Lecture 3：损失函数和优化（loss Function and optimization）**




这一讲主要分为三部分内容：




1. 继续上一讲的内容介绍了线性分类方法；

2. 介绍了高阶表征及图像的特点；

3. 优化及随机梯度下降（SGD）。



![](https://pic4.zhimg.com/v2-178e27d5aa11ecabc04b21185bf641ab_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='798' height='464'></svg>)



**Lecture 4：神经网络**




包括经典的反向传播算法（back-propagation）；多层感知机结构（multilayer perceptrons）；以及神经元视角。



![](https://pic2.zhimg.com/v2-be714b19ac8864c060681348ff4e9695_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='926' height='470'></svg>)



**Lecture 5：卷积神经网络（CNN）**




主要分为三部分内容：




1. 卷积神经网络的历史背景及发展；

2. 卷积与池化（convolution and pooling）;

3. ConvNets 的效果



![](https://pic3.zhimg.com/v2-52230ffe3a61ff8883ef8a8aca17cd82_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1056' height='503'></svg>)



**Lecture 6：如何训练神经网络 I**




介绍了各类激活函数，数据预处理，权重初始化，分批归一化（batch normalization）以及超参优化（hyper-parameter optimization）。



![](https://pic4.zhimg.com/v2-b5b3f35aa62d321265c01687710845e7_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='967' height='534'></svg>)



**Lecture 7：如何训练神经网络 II**




介绍了优化方法（optimization）、模型集成（model ensembles）、正则化（regularization）、数据扩张（data-augmentation）和迁移学习（transfer learning）。



![](https://pic3.zhimg.com/v2-ed93423437477f47fe0b7470682b51a6_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='865' height='429'></svg>)



**Lecture 8: 深度学习软件基础**




1. 详细对比了 CPU 和 GPU；

2. TensorFlow、Theano、PyTorch、Torch、Caffe 实例的具体说明；

3. 各类框架的对比及用途分析。



![](https://pic3.zhimg.com/v2-5565be20cab0c4127f90f9b0ee0f55ca_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='579' height='237'></svg>)



**Lecture 9：卷积神经网络架构（CNN Architectures）**




该课程从 LeNet-5 开始到 AlexNet、VGG、GoogLeNet、ResNet 等由理论到实例详细描述了卷积神经网络的架构与原理。



![](https://pic2.zhimg.com/v2-38e7e458379915b19d91cda8459768dd_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='922' height='508'></svg>)



**Lecture 10：循环神经网络（Recurrent Neural Networks）**




该课程先详细介绍了 RNN、LSTM 和 GRU 的架构与原理，再从语言建模、图像描述、视觉问答系统等对这些模型进行进一步的描述。



![](https://pic2.zhimg.com/v2-6e2d53822b1bb3ea8da356e493b04529_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='934' height='526'></svg>)



**Lecture 11：检测与分割（Detection and Segmentation）**




该课程在图像分类的基础上介绍了其他的计算机视觉任务，如语义分割、目标检测和实例分割等，同时还详细介绍了其它如 R-CNN、Fast R-CNN、Mask R-CNN 等架构。



![](https://pic2.zhimg.com/v2-cb4b0f5b9725d198bc5bf153fc31dd05_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='942' height='463'></svg>)



**Lecture 12：可视化和理解（Visualizing and Understanding）**




该部分不仅讲述了特征可视化和转置，同时还描述了对抗性样本和像 DeepDream 那样的风格迁移系统。



![](https://pic1.zhimg.com/v2-33ce58e14cdf42118b8f07876876c638_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='947' height='494'></svg>)



**Lecture 13：生成模型（Generative Models）**




该章节从 PixelRNN 和 PixelCNN 开始，再到变分自编码器和生成对抗网络详细地讲解了生成模型。



![](https://pic1.zhimg.com/v2-7f962f4d5d5cc9d793a64534b4aaad2c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='950' height='498'></svg>)



**Lecture 14：强化学习（Reinforcement Learning）**




该章节先从基本概念解释了什么是强化学习，再解释了马尔可夫决策过程如何形式化强化学习的基本概念。最后对 Q 学习和策略梯度进行了详细的刻画，包括架构、优化策略和训练方案等等。



![](https://pic2.zhimg.com/v2-786b66e727f7c84919681a8c482c2309_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='928' height='409'></svg>)



**Lecture 15：深度学习高效的方法和硬件（Efficient Methods and Hardware for Deep Learning）**




该章节首先展示了深度学习的三大挑战：即模型规模、训练速度和能源效率。而解决方案可以通过联合设计算法-硬件以提高深度学习效率，构建更高效的推断算法等，



![](https://pic3.zhimg.com/v2-cf2a4647a5c4a72a26050b319ed3bdae_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='930' height='696'></svg>)



**Lecture 16：对抗性样本和对抗性训练（Adversarial Examples and Adversarial Training）**




该章节由 Ian Goodfellow 于 5 月 30 日主讲，主要从什么事对抗性样本、对抗性样本产生的原因、如何将对抗性样本应用到企业机器学习系统中、及对抗性样本会如何提升机器学习的性能等方面详细描述对抗性样本和对抗性训练。



![](https://pic3.zhimg.com/v2-4d32df412837e4b94f2f08bb9837518e_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='614'></svg>)



**机器之心报道**




