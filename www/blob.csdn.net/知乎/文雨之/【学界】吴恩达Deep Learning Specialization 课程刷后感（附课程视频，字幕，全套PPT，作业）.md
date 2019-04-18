# 【学界】吴恩达Deep Learning Specialization 课程刷后感（附课程视频，字幕，全套PPT，作业） - 知乎
# 

> 作者简介： [@文雨之](https://www.zhihu.com/people/186282e45de0e1040b7882c9be8f5de8) 本科东北大学自动化专业，然后直接攻博读的是系统工程专业。目前在东北大学系统工程专业读博士，在流程工业综合自动化国家重点实验室做流程工业里边调度问题和运行优化问题。博士刚开始主要接触了很多Numerical optimization， Convex optimization，Nonlinear programming的知识，用来解决实际工业过程中的优化问题。随着近几年来，人工智能机器学习火了起来，逐渐研究的重心也从数学优化转到机器学习相关领域了，但是我一直还是认为优化是自己的老本行，也一直从优化的角度去看待机器学习的问题去尝试做一些新的思考。
*欢迎原链接转发，转载请前往*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*的主页获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**陆续发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：
[『运筹OR帷幄』大数据人工智能时代的运筹学--知乎专栏](https://zhuanlan.zhihu.com/operations-research)

## **0 前言**

小编亲自刷了Deep Learning Specialization课程中的前三门课，并顺利拿下证书，在这里晒一下证书的同时也想把自己关于该课程的刷后感分享给大家，文末还准备上了全套的Deep Learning Specialization课程的资料，包括课程的视频，课程视频配备了中英文字幕，课程PPT，课程的作业（部分答案）。

下图是第一门课的证书，可以看到证书的左下角有Andrew Ng的签名（貌似用49刀买了Andrew Ng的签名一样）。
![](https://pic2.zhimg.com/v2-6f3c1e77edbd83c0fd68bc9da52637a5_b.jpg)
## **1 Deep Learning Specialization课程简介**

2017年八月初，吴恩达（Andrew Ng）在coursera上推出了Deep Learning Specialization的系列专项课程，该系列课程一共有5门课程构成：
![](https://pic2.zhimg.com/v2-24bfa47649e0d65c56a43b9c1f594e8d_b.jpg)
[Neural Networks and Deep Learning](https://link.zhihu.com/?target=https%3A//www.coursera.org/learn/neural-networks-deep-learning)

主要内容： 
- 从逻辑回归出发（逻辑回归实际上可以视为没有隐含层的神经网络，为下一节引出神经网络做了铺垫），期间插入了对Python编程的介绍（用Numpy进行向量化矩阵化的运算，jupyter note的使用）
- 实现一个浅层的神经网络的训练（单隐含层的神经网络，激活函数的选择，前向传播与误差反向传播算法，权值的初始化）
- 实现深层神经网络的训练（多隐含层的神经网络，超参数和参数）
![](https://pic4.zhimg.com/v2-6f48047b96bdcf15a8fa2a90e30de06b_b.jpg)
[Improving Deep Neural Networks](https://link.zhihu.com/?target=https%3A//www.coursera.org/learn/deep-neural-network)

主要内容： 
- 神经网络训练的一些技术（训练集测试集，偏差和方差，正则化，Dropout，归一化，梯度检查的方法）
- 神经网络训练的优化算法介绍（最小批梯度算法，指数平滑改进梯度算法，RMSprop, Adam, 学习率的选择）
- 超参数搜索方法，Batch Norm，多分类问题softmax函数，深度学习框架tensorflow
![](https://pic2.zhimg.com/v2-f38b24b26d87a8169bc7e07d602db3b1_b.jpg)
[Structuring Machine Learning Projects](https://link.zhihu.com/?target=https%3A//www.coursera.org/learn/machine-learning-projects)

主要内容： 
- 机器学习的一些实践经验和技巧（human-level performance，误差分析，训练集测试集分布不一样，迁移学习，多任务学习，端到端学习）
![](https://pic3.zhimg.com/v2-43462369d53e6dd9ffc94e363c471a72_b.jpg)
[Convolutional Neural Networks](https://link.zhihu.com/?target=https%3A//www.coursera.org/learn/convolutional-neural-networks)

主要内容： 
- CNN的基础内容（计算机视觉、图片边缘检测、卷积、池化等）
- AlexNet、LeNet、VGG、ResNet、Inception Network、1乘1卷积、迁移学习、数据扩充等
- 目标定位、目标检测、Bounding Box预测、交并比、非最大值抑制NMS、Anchor box、YOLO算法、候选区域region proposals等
- 人脸识别、one-shot学习、Siamese网络、Triplet损失、风格迁移、内容损失、风格损失、1D-3D卷积等
![](https://pic1.zhimg.com/v2-f49ab1603185474dcf588f72b9e73b08_b.jpg)
[Sequence Models](https://link.zhihu.com/?target=https%3A//www.coursera.org/learn/nlp-sequence-models)

主要内容： 
- RNN简介，不同类型的RNN、语言模型、新序列采样、RNN梯度消失、GRU、LSTM、双向RNN、深层RNNs等；
- 序列到序列模型、集束搜索（Beam search）、集束搜索误差分析、Bleu得分、注意力模型、注意力权重、语音识别、触发字检测等

## **2 小编的刷后感**
- **关于课程的难度与目标人群**

课程难度低，基本不需要具备什么数学知识就可以，主要在误差反向传播算法的时候需要用到复合函数求导的内容，另外就是批梯度算法的最终实现要依赖大量的线性代数运算，即向量和矩阵运算等等。相信一般的本科生入门完全是没有任何压力的。作为机器学习零基础的入门课程也是完全可以的。

课程的特点在于把神经网络彻底讲的很清楚了，从神经网络的Motivation到一些很小的细节都有包括，一些小的细节可能对于一些熟悉神经网络的同学来说也不一定都曾经全了解过，所以课程的入门门槛低并不意味着课程就很简单只适合新手去刷，老手一样也可以获益匪浅。

课程同时配合着穿插了一些Python编程的知识，有机的将理论和编程无缝对接。对于喜欢编程的同学无疑是一个非常好的实践场合。

- **关于课程作业**

课程的作业设计也是非常用心的。首先作业的形式是分为两种，一种是选择题，单选和多项穿插，一般是10道选择题，正确率在80%或80%以上的才可以通过。不过好处是可以多次提交答案，所以答题也是一个自我检查的过程。我的基础还算好一些，多数课程的答题虽然很少有满分的，不过基本上一次就能通过。如果发现自己有哪些题目错了或者答题的时候感觉比较模糊的可以回顾一下前面的视频内容，题目的答案基本都在前面的视频里边讲过了，所以选择题的设计是比较简单侧重基本理论内容的复习和自查。

第二种是编程题，编程题采用的形式是Jupyter notebook的形式，一部分文字和公式的讲解，配合上一段代码，你需要做的是补充代码中一些关键步骤。相比完完全全让你从0开始编程的作业，Deep Learning Specialization 课程这种形式的作业设计也是“非常贴心”的。应该是考虑到了如果完全从0开始造轮子对于课程参与者来说难度太大，而且会耗费过多时间在作业上。这样做的好处是既让你参与了机器学习中造轮子的过程，又不至于耗费太多时间让你陷入过多细节中去以至于最终放弃。

以下是作业的截图
![](https://pic4.zhimg.com/v2-30e5e49472dc441d2a1f9bd2326db1a7_b.jpg)
吴恩达也在课程中一直贯彻一个思想就是 在刚开始学习机器学习算法的时候最好是要自己亲自动手去实现一下，俗称就是造轮子，等到了解了这个算法的全部过程以后真正用的时候直接调用现成的算法包就好，俗称就是调包侠。所以造轮子和调包侠也不是完全矛盾的，只是侧重点不一样罢了，不同的学习阶段，不同的学习目的，你要根据自己的需求去选择造轮子还是调包。

- **关于机器学习入门的那些事**

虽然吴恩达的深度学习课程不能让你成为真正的深度学习专家，但可以让你对深度学习模型的发展过程有了基本的直觉了解。对比吴恩达的另外一门Machine Learning 的课程，Deep Learning Specialization 专项课程在显得浅显易懂的同时，理论的味道稍显不足。我想还是回归到老话题上，我个人看待机器学习还是更加注重数学基础。所以在完成了Deep Learning Specialization专项课程之后，也可以再去看看老的经典的Machine Learning 的课程，这两个课程作为一个相互的补充是极好的。另外有部分同学数学基础不是那么好，如果想补一下机器学习的相关数学基础的，可以参考我以前的文章 [【学界】一文读懂机器学习需要哪些数学知识(附全套优秀课程的网盘链接资源)](https://zhuanlan.zhihu.com/p/33999430)

## **3 机器学习大牛的那些事**

有意思的是课程视频的末尾总会穿插如吴恩达对 深度学习领域大牛的一个采访视频，接收采访的人主要有Geoffrey Hinton, Ian Goodfellow，Yoshua Bengio， 林元庆，Andrej Karpathy， Ruslan Salakhutdinov， Pieter Abbeel

远程采访Geoffrey Hinton
![](https://pic4.zhimg.com/v2-c3183ce0a650c8dc8d9b92c59e476df3_b.jpg)
基本上每个人会谈谈他们做研究的心路历程，为何会做机器学习这个领域，对机器学习的未来发展进行一个展望和分析。整个采访视频非常的有意思，而且安排在每周课程的最后，让大家在刷完课程后也可以了解一下“学术八卦”作为一个放松，也可以了解整个机器学习的历史和起源。 

## **4 Deep Learning Specialization 课程全套配套资料介绍**

我们收集了全套的Deep Learning Specialization课程的资料，包括课程的视频，课程视频配备了中英文字幕，课程PPT，课程的作业（部分答案）。

我们完全按照coursera上课程的进度，组织全套课程资料的结构，方便读者循序渐进的去学习。虽然coursera上在线也可以看这个课程，但是如果不付费的话只有七天免费期，国内的网易云课堂也引进了这门课程，不过配套资料还不太完善，所以这么好的资源还是保存在自己的手里更加踏实一些。

想要获取上述资源请先关注文末的**运筹OR帷幄**公众号，并在公众号回复 **deeplearning **即可获取资源的网盘链接。

下图是网盘资源预览：
![](https://pic1.zhimg.com/v2-f7a15debf5213667379061b24a000174_b.jpg)
**此资料仅供大家学习交流使用，不作为商业用途如有内容侵权，我们将随时删除。**


如果你是运筹学/人工智能硕博或在读，请在下图的公众号后台留言：**“加微信群”**。系统会自动辨认你的关键字，并提示您进一步的加群要求和步骤，邀请您进全球运筹或AI学者群（群内学界、业界大佬云集）。

同时我们有：【**运筹学|优化爱好者**】【**供应链|物流**】【**人工智能**】【**数据科学|分析**】千人QQ群，想入群的小伙伴可以关注下方公众号**点击“加入社区”按钮**，获得入群传送门。

学术界|工业界招聘、征稿等信息**免费发布**，请见下图：
![](https://pic3.zhimg.com/v2-1ad3bac32612bc7bb090f1143b107702_b.jpg)

