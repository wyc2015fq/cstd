# 场景理解-资料3-Bottom-Up and Top-Down Attention - 战斗蜗牛的专栏 - CSDN博客





2019年03月09日 14:18:57[vbskj](https://me.csdn.net/vbskj)阅读数：73








# 视觉场景理解论文阅读笔记:Bottom-Up and Top-Down Attention 





一、文章相关资料

1.论文地址:[点击打开链接](https://arxiv.org/pdf/1707.07998.pdf)

2.论文代码:[点击打开链接](https://github.com/peteanderson80/bottom-up-attention)

3.发表时间:2018

4. 解决的问题：
- Image Captioning 和 VQA（visual question answer）

二、阅读笔记

1.论文思想

        文章提出一种自上而下与自下而上相结合的注意力模型方法，应用于视觉场景理解和视觉问答系统的相关问题。其中基于自下而上的关注模型（一般使用Faster R-CNN）用于提取图像中的兴趣区域，获取对象特征；而基于自上而下的注意力模型用于学习特征所对应的权重（一般使用LSTM），以此实现对视觉图像的深入理解。

        Top-down atttention 和 Bottom-up attention 结合起来，作者说 bottom-up attention 就是将图片的一些重要得区域提取出来，每一个区域都有一个特征向量，Top-down attention 就是确定特征对文本得贡献度。

对于一个图片 II ,提取出 kk 个图片特征 V=v1,v2,...,vk，viϵRDV=v1,v2,...,vk，viϵRD , 每一个特征代表图片得一个显著区域的编码，VV 就可以看成是 bottom-up attention model 的输出，然后将这些用于top-down attention model，提取出对描述贡献大的显著性区域的特征。


       个人理解：文章中虽然没有提及在目前研究中最为广泛使用的Encode—Decode框架，但是基于自下而上的注意力模型的任务是获取图像兴趣区域提取图像特征类似于对图像进行特征编码，实现编码阶段任务；而基于自上而下的注意力模型用于学习调整特征权重，实现了图像内容的“时刻关注”，逐词生成描述，相当于解码阶段。

2.为什么使用Faster R-CNN 而不使用CNN?

![](https://img-blog.csdn.net/201806120159552)

       从图中可以看出使用CNN需要使用比R-CNN更多的特征，而且很多额特征往往是无用的。R-CNN的目标检测方法，首先针对图像获取兴趣区域，然后对每个兴趣区域应用目标检测器，这样就可以准确的获得图像类别；而CNN方法需要输入整幅图像，而且用于大样本分类的网络往往很复杂，计算量更大。另外，Faster R-CNN对之前几代R-CNN方法进行改进，实现了只输入一次就可以识别所有对象的能力，极大的提高的处理效率。

3.自下而上的关注模型

        文中提及使用Faster R-CNN实现基于自下而上的注意力模型，[Faster R-CNN论文笔记](https://blog.csdn.net/qq_17448289/article/details/52871461)

![](https://img-blog.csdn.net/20180612021111976)

        文中提及使用Faster R-CNN实现自下向上的关注模型，从图中可以看出相比之前不同之处在于，通过设定的阈值允许兴趣框的重叠，这样可以更有效的理解图像内容。文中对每一个感兴趣区域不仅使用对象检测器还使用属性分类器，这样可以获得对对象的（属性，对象）的二元描述。这样的描述更加贴合实际应用。

        使用 Faster R-CNN 来提取图片中的兴趣点，然后对感兴趣的区域采用 ResNet-101 来提取特征，使用 IoU 阈值来对所有区域进行一个筛选(“hard” attention)。对于每一个区域 ii , vivi 定义为每个区域的 mean-pooled convolutional 特征(2048维)。使用这种方法从很多候选配置中选出一小部分候选框。

        预训练Bpttom-Up Attention Model , 首先初始化Faster-RCNN 和 ResNet-101并在ImageNet上进行预训练，然后在Genome data 上进行训练。为了增强学习特征表达的能力，作者增加了一个预测物体属性类别的任务，为了预测区域 ii 的属性， 作者将 vivi 与一个代表着物体真实类别的 embedding 连接，并将其喂给一个新增的输出层输出一个在属性类别和非属性类别上的softmax(???)。


4.自上而下的关注模型

![](https://img-blog.csdn.net/20180612021612156)

        文中提到使用；两层LSTM模型，一层用于实现自上而下的注意力，一层实现语言模型。
- 主要有两个LSTM，第一个是Attention LSTM, 第二个是Language LSTM。

        文章中提到的公式如下:

![](https://img-blog.csdn.net/20180612021847464)

![](https://img-blog.csdn.net/20180612021857838)

![](https://img-blog.csdn.net/20180612021951829)

       这些公式不做详细介绍，具体可以从LSTM相关理论，以及机器翻译中应用注意机制文章中进行理解：[将注意力模型引入机器翻译的早期文章](https://arxiv.org/pdf/1409.0473.pdf)。

      这里只提及我的一个疑问：与的关系，从公式和图出发，我个人将其理解为同一对象的不同表示，应该是为了避免混淆而用两种写法。

       Language LSTM的输入为：x2t=[vˆt,h1t]​xt2=[v^t,ht1]​ ，即是Attention LSTM的隐含层输出和Attention的结果，然后输出的 h2t​ht2​ 经过softmax 层输出单词的概率，整个句子的概率可以看成是所有单词概率的连乘。

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20180719204146104?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0NvZGVyX1hpYW9IdWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5.实验结果

![](https://img-blog.csdn.net/20180612022803812)

![](https://img-blog.csdn.net/20180612022816547)



        从实验结果可以看出，文中所提方法的确获得良好结果.

注：本人因基础知识和表达能力的限制，对文章内容理解可能存在偏差，如有错误，请指出。

--------------------- 

作者：TayLion王 

来源：CSDN 

原文：https://blog.csdn.net/u012991166/article/details/80659909 

版权声明：本文为博主原创文章，转载请附上博文链接！



