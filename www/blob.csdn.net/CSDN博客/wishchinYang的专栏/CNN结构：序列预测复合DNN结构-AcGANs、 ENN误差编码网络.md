# CNN结构：序列预测复合DNN结构-AcGANs、 ENN误差编码网络 - wishchinYang的专栏 - CSDN博客
2018年05月22日 13:28:30[wishchin](https://me.csdn.net/wishchin)阅读数：676
**前言：模式识别问题**
     模式函数是一个从问题定义域到模式值域的一个单射。
     从简单的贝叶斯方法，到只能支持二分类的原始支持向量机，到十几个类的分类上最好用的随机森林方法，到可以支持ImageNet上海量1860个类且分类精度极高的InceptionV4（参考：CNN[http://blog.csdn.net/wishchin/article/details/45286805](http://blog.csdn.net/wishchin/article/details/45286805)），其模式函数为
       f( x ) = { X——>Y }|{  X = ImageNet的图片，Y={ 1860个类的标记 }  }
     是一个单射函数。 普遍情况下，定义域小于值域，模式函数成为一个**hash函数**。   
###### 参考：[AI：模式识别的数学表示](https://blog.csdn.net/wishchin/article/details/74644489)（集合—函数观点）
     GANs的产生，CNN网络的用处更多了。从单纯的模式识别，以及基本的检测、分割与追踪任务。到可以进行进一步应用的图像增强、编码、复原、重建，以及预测功能。
###### 生成式模型GAN：
机器之心所关注的GAN文章列表：
- [Wasserstein GAN 的 TensorFlow 实现](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650723201&idx=3&sn=204b20981a52c8e190624e0c0e445857&chksm=871b17ffb06c9ee9b4fc57ea05762dde2f54229f7510c1f60deb7b6707ec3647066aa192a0a5&scene=21#wechat_redirect)
- [FAIR 提出常见 GAN 训练方法的替代方法：WGAN](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650722818&idx=3&sn=03cb67c8a8ee7f83a7448b518f4336ab&chksm=871b167cb06c9f6a018a99b79d8b2764b207be2b4d03f132151d99124edf2aff4c116a9dc98d&scene=21#wechat_redirect)
- [最小二乘GAN：比常规GAN更稳定，比WGAN收敛更迅速](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650723893&idx=5&sn=bed0e585bba8ad01824a8d80603dbea6&chksm=871b124bb06c9b5d5f3b290ad02fb43ba02bdf7cbd04d53cfe839213ad7e57c0426555500e24&scene=21#wechat_redirect)
- [新论文提出用GAN构建不同年龄时的样貌：可让你提前看到年老时的模样](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650723678&idx=5&sn=c9d8114359cf3597e45cb102154b918e&chksm=871b1120b06c9836367e8f32177b59736f304d055b9ffdb2bb2c5d98398b555bf0461c99d336&scene=21#wechat_redirect)
- [谷歌新论文提出适应性生成对抗网络AdaGAN：增强生成模型](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650722239&idx=4&sn=3544eee8b2bf5eeab1ae852ba9fde64c&chksm=871b0bc1b06c82d74fa57cbcb0d9f6178a78d474c6bfe93db174dd0c8f580272e398006b0f14&scene=21#wechat_redirect)
- [GAN之父NIPS 2016演讲现场直击：全方位解读生成对抗网络的原理及未来](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650721284&idx=1&sn=427e7f45c8253ab22a3960978409f5d1&chksm=871b087ab06c816c424ad03810be3e1b3aa9d6e99a5f325047796f110d178a07736f667d1a10&scene=21#wechat_redirect)
- [直观理解GAN背后的原理：以人脸图像生成为例](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650723168&idx=2&sn=68b21b815688443a0dd7caa115cc13fa&chksm=871b171eb06c9e085ab0f2223e6bab04d2eecfa430ca071b19d5c377d909d46724269126dbf3&scene=21#wechat_redirect)
- [学界 | Yann LeCun 最新论文：基于能量的生成对抗网络（附论文）](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650719118&idx=3&sn=4565fb14db41208571b808956a39e310&scene=21#wechat_redirect)
       模式识别的函数映射，不管使用判别式模型或者生成式模型，最后都表示为一个判别函数的形式。比如： f( x ) = { X——>Y }|{  X = ImageNet的图片，Y={ 1860个类的标记 }  }。
 在此文：*[*Face Aging With Conditional Generative Adversarial Networks*](https://arxiv.org/pdf/1702.01983v1.pdf)*中，GANs用于人脸图像合成，让人提前看到年老时的模样，还原Gans不同于一般DNN模型的特殊应用及主要存在意义。
       人脸图像合成和基于时间的预测，是一个判别函数的复合函数，但不同于GANs本身训练过程的一个时间过程。K( f( x )) = { X——>K(Y) }|{  X = 人脸图片的定义域，K(Y)={  生成人脸图片  }   }，结果*不是判别的，而是生成的*。K（x）是一个时序函数模型。
       使用时间函数复合，可使从判别模型，到预测模型，判别的另外一个目的是用于假设，进而用于探索，逐步靠近真正地智慧所在。
**从贝叶斯、马尔科夫到预测/演绎模型**
       从可见条件推测模型，构造假设，用以探测真实，即是贝叶斯方法，是一种分析方法，使用了归纳，为判别模型。而预测模型，则使用了另一个重要的推理过程，演绎过程。从归纳到演绎，是处理问题的一般过程。时序GANs模型，是一个时间意义上的演绎模型。
       为了做一些事情，仅仅是判别有什么意义！预测才有未来。以人为镜可以整衣冠，以史为镜可以见兴衰。收集到妹子各方面的知识，用以构建妹子的思考模型，当然不是最终目的。目的是该怎样选取一个怎样最优的函数，才能把得上她。
文章用于生成不同年龄的图片, 采用的模型是条件对抗网络,  主要创新点是,   首先通过一个网络,   提取图像特征向量,    并通过**身份保持网络**,    优化图像的特征向量 , 得到特征向量,  之后便可以**对于每个输入年龄 ,  查找其年龄向量**, **并将该年龄向量与输入图片特征向量串联/复合**,  输入生成网络, 生成目标年龄图片.
![](https://img-blog.csdn.net/20180522132732408)
         预测结果：
![](https://img-blog.csdn.net/20180522132752904)
### 误差编码网络 ENN
文章：[如何预测未来的多种可能](https://www.leiphone.com/news/201711/eiIDAbAVt5qflW4D.html)？采用ENN
在这篇论文中，作者们介绍了一种新的架构，它让时间序列数据的条件预测也可以是多模态且健壮的。它的构建基于一个简单的直觉，就是把未来状态分成确定部分和随机部分的组合；确定部分可以根据当前状态直接做出预测，随机的（或者说难以预测）的部分就代表了关于未来的不确定性。训练这个确定性的网络就能够以网络预测的形式获得确定性因子，同时也得到了与真实状态相比得到的预测误差。这个误差可以用低维隐含变量的形式编码，然后送入第二个网络中；这第二个网络经过训练后就可以利用这些额外的信息准确地更正前一个确定性网络的预测结果。这就是论文中提出的误差编码网络（Error Encoding Network，ENN）。
简单来说，这个框架在三个时间步骤中分别含有三个函数映射：
- 
 第一个函数映射把当前状态映射到未来状态，它也就把未来状态分成了确定性和不确定性的两个部分
- 
第二个函数映射是从不确定部分映射到低维隐含向量‘
- 
第三个函数映射是基于隐含向量的条件，把当前状态映射到未来状态，这个过程中也就编码了未来状态的模式信息。
模型的训练过程中会用到全部的三个映射，推理部分只需要最后一个映射。
![如何有效预测未来的多种可能？LeCun的误差编码网络给你带来答案](https://static.leiphone.com/uploads/new/article/740_740/201711/5a0d75ee04215.jpg?imageMogr2/format/jpg/quality/90)
##### 模型架构
前述的两个网络都是根据监督学习的目标函数端到端训练的，隐含变量是通过学到的参数化函数计算的，这样一来训练过程就可以简单、快速。
**翻一下这两个很长的PPT：**
      预测学习：[记忆网络和对抗训练是很有前景的方向](http://www.sohu.com/a/124435161_465975)，[用预测学习代替无监督学习](http://www.sohu.com/a/120901076_473283).................
