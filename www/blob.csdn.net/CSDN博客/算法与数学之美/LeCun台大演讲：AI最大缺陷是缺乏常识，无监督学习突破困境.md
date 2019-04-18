# LeCun台大演讲：AI最大缺陷是缺乏常识，无监督学习突破困境 - 算法与数学之美 - CSDN博客
2019年03月17日 21:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：11
***![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkyYNr2mLLFgV8rAcf8TKrhVVxPz4sJSdjDum3ia43TqSj6sPicZVQ5XqpfDhulibnGRdeLqAhyHf8TWw/640?wx_fmt=gif)***
> 
***LeCun表示，深度学习的特点在于“整个程序都是可训练的”，监督学习存在两大问题***
近日，在台湾大学，卷积神经网络之父、FacebookAI 研究院院长 Yann LeCun 以「Deep Learning and the Path to AI」为题，对深度学习目前的发展现状和面临的最大挑战、以及应对方法进行了综述和分析。
6 月 29 日，台湾大学。卷积神经网络之父、FacebookAI 研究院院长 Yann LeCun 以「Deep Learning and the Path to AI」为题，对深度学习目前的发展现状和面临的最大挑战、以及应对方法进行了综述和分析。新智元结合台湾大学在 Facebook 上公布的视频、台湾科技媒体 iThome 的报道，以及 Yann LeCun 今年早些时候在爱丁堡大学的演讲资料，为您综合介绍。
###  深度学习的特点在于“整个程序都是可训练的” 
演讲从模式识别（Pattern Recognition）的起源说起。1957年，Perceptron 诞生，成为第一个 LearningMachine。LeCun 说，目前的机器学习算法大多衍生自 Perceptron的概念。
从那时起，模式识别的标准模型就可以分为 3 步走：1.程序被输入一张图像，通过特征提取，将图像特征转换为多个向量；2. 输入这些向量到可训练的分类器中；3.程序输出识别结果。 
他表示，机器学习算法其实就是误差校正（Error correction），通过调整权重，来进行特征提取。也就是说，如果输入一张图，算法识别后，结果值低于预期类别的值，工程师就将输入的图增加 Positive 的权重，减少 Negative 的权重，来校正误差。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRHDvlCfdeTsJFLGoJhUZUeWkd8qdBJ6Gdz4HDia30e6luu0VcvQwD55w/640?wx_fmt=jpeg)
深度学习是当今最广泛使用的模式识别方法。LeCun 认为深度学习的特点在于“整个程序都是可训练的”。他解释，构建深度学习的模型不是用手动调整特征提取的参数来训练分类器，而是建立一群像小型瀑布般的可训练的模组。 
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRDKBtRwiavXZ8W1GGqprCkN4G5QzZNlS1uAbdP2GNyUSIGviaFsunSx7g/640?wx_fmt=jpeg)
当开发人员将原始的影像输入系统后，会先经过初步的特征提取器，产生代表的数值，在这一个阶段可能会先识别出一些基本的纹理，接下来这些纹理的组合会再被拿来识别更具体的特征，像是物件的形体或是类别，整个训练的过程就是不断地经过一层又一层这样的模型，每一层都是可训练的，所以我们称这个算法为深度学习或是端到端训练（End to End Running）。
LeCun 解释，深度学习模型之所以工作良好，是因为现在的影像都是自然景象加上其他物体，也就是混合型的图像，而每个物体又由不同的特征所组成，会有不同的轮廓和纹路，图片的像素也是一个问题，因此，可以将影像分级成像素、边缘、轮廓、元件和物件等，初级的特征提取会先侦测出影像中最基本的轮廓，比如明显的纹路和色块，进一步的特征提取则是将上一层的结果组合再一起，拼成一个形体，最后再拼成一个物体。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRtnHktCNT9sJXeTvI42x2Ts0EvqD4NGSSnUv2J2t4VIxVjHpYDLws0g/640?wx_fmt=jpeg)
这种分层式的组合架构（Hierarchical Compositionality）其实不只适用于影像，LeCun说明，它对文字、语音、动作或是任何自然的信号都适用，这种方式参考了人脑的运作模式。大脑中的视觉中枢，也是用类似分层式的组合架构来运行，当人类看到影像后，由视网膜进入到视丘后方外侧膝状体，再到大脑中主要的视觉中枢，最后来到颞叶皮质，人类看图像也是由大脑经过多层的结构，在100毫秒内就能识别图片。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRibFtLAKx0bB6aiboAG4AsoYpvibz5H1AIe3wljPjs3ueF9Bf7k4opNic2A/640?wx_fmt=jpeg)
深度学习的问题在于如何训练，在1980年代中期，误差反向传播算法（Back Propagation Algorithm）开始流行，但其实误差反向传播算法很早就被提出来，只是当时没有受到重视。误差反向传播算法一开始先经过简单线性分类，再将这些结果带到非线性的线性整流函数（Rectified Linear Unit，ReLU），线性整流函数就是找到要调整参数的方向，来减少错误判断，不过现在都已经有可用的套件或是框架，像是Torch、TensorFlow 或是 Theano等，还有一些套件是可用来计算输出结果和预期结果之间的误差。 
Yann LeCun认为，现在要撰写机器学习算法并不难，用 3 行 Python 就可以完成，不过这还停留在监督式学习阶段，所谓的监督式学习就是输入大量的训练样本，每一套训练样本都已经经过人工标注出原始图片和对应的预期结果。以影像处理为例，训练集由多个(X,Y)参数组成，X就是影像的像素，Y则是预设的识别结果类别，像是车子、桌子等，之后再用大量的测试集来测试程序，若判断结果正确，不用调整，若判断有误则调整程序中的参数。
###  监督式机器学习存在二大问题 
因此，Yann LeCun表示，监督式的机器学习就是功能优化（Function Optimization），资料输入和输出的关系通过可调整的参数来优化，经由调整参数的方式，将结果的错误率降至最低，其中，调整参数的方式有很多种，很多人都会用梯度下降算法（Stochastic Gradient Descent），梯度下降算法可以找到最适合的回归模型系数．即时地根据输入的资料动态调整模型。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRBNGFFz4KcAUibsyYicezrZmIYEynxzEQwRa3TicGib6Ay7icCSYxIYHYD1g/640?wx_fmt=jpeg)
身为「卷积神经网络之父」的 Yann LeCun 也介绍了卷积神经网络（Convolutional Neural Network，CNN），卷积网络就是将输入的影像像素矩阵经过一层过滤器，挑选出特征，再透过池化层（PoolingLayer），针对输入特征矩阵压缩，让特征矩阵变小，降低计算的复杂度。CNN影像和语音识别都有很好的成效，不仅如此，还能识别街上移动的路人、街景的物体，Facebook 也用 CNN 来识别 Facebook 用户上传的照片，他表示一天 Facebook 就有10亿以上的照片，可以准确地识别物体的类别，像是人还是狗、猫等，还能识别照片的主题，像是婚礼或是生日派对等。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRGCRNuN8yflPIoFEghzYPZXYDStTqiahOX8YmzKSsIpOczzA2bTX8uPg/640?wx_fmt=jpeg)
不过，Yann LeCun提出，监督式的机器学习有2大问题，第一是要如何建立复杂的算法来解决复杂的问题，第二则是手动调整参数的知识和经验都是来自于不同任务，许多工程师想要处理的领域，像是影像识别、语音识别都需要建置不同模型，因此，监督式机器学习可以在训练过的专案上有很好的表现，但是没有训练过的资料，程序就无法辨别，简单来说，如果要程序识别椅子，不可能训练所有椅子的特征资料。
事实上，Yann LeCun 表示现实中有种机器具备数百万的调整钮（Knob），这些调整钮就像机器学习中的参数和 Perceptron 的权重一样，可以用上百万的训练样本来训练模型，最后分类出上千种的类别，但是，每一个特征的识别都必须经过数十亿次的操作，因此，可想而知，现今大家所使用的神经网络是非常复杂的，如此庞大的运作不可能在一般的 CPU 上执行，“我们面对的是非常大规模的优化问题。”他说。
###  AI系统的架构 
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwROjuyBm1KMZGv0wtbAZIzr9OfUlW2um7Puck7QtHjnqflTw84NN2Z4Q/640?wx_fmt=jpeg)
AI系统的架构大致上可以分为感知（Perception）、触发器（Agent）和目标（Objective）3个模组，先由感知器侦测真实世界的数据，像是影像、语音等，这些数据经由触发器，会依据状态触发目标，执行相对应的程序并产生结果，其中触发器就是AI 的精髓，触发器必须要负责规划、预测等智能工作，而目标则是由本能和固定的两个元件所组成，以视觉识别（VisualIdentity）系统为例，经由感知收集影像数据，透过触发器触发分析情绪的程序，再判断影片中的人是开心还是不开心。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRtIla4sn8AH6SLVDEePRR7kHR4HOS0ZgRJ16yFdWOKwJq8kdQaldARA/640?wx_fmt=jpeg)
AI 架构中的触发器（Agent）主要负责预测和规划，运作过程又可分为模拟器（Simulator）、执行器（Actor）、回馈器（Critic），模拟器接收到状态后，传送给执行器，执行器就会启动相对应的动作，并同时对模拟器提出要求，启动相对应的动作之后送到回馈器，经由回馈器分析要採取的动作，决定后才送往目标（Objective）执行。
###  AI 最大局限是没有人类的“常识” 
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRjzul6kZu1KIBRE8PxicD2Wpttfpcd1uwX7MicdUmQZu0yaq7Af8MY7sg/640?wx_fmt=jpeg)
市场上 AI 好像无所不能，但其实，Yann LeCun个人认为，AI 还是有些局限，像是机器必须会观察状态、了解很多背景知识、世界运行的定律，以及精确地判断、规划等，其中，Yann LeCun 认为 AI 最大的局限是无法拥有人类的「常识」。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwRMvSn3e0ank0JX5juNO99AL2J0dbIg1avBAkaM6dhYBnz20wbuNwkMw/640?wx_fmt=jpeg)
由于目前比较好的AI应用都是采用监督式学习，能够准确识别人工标示过的物体，也有些好的成果是用强化学习（Reinforcement Learning）的方式，但是强化学习需要大量地收集资料来训练模型，Yann LeCun表示，对应到现实社会中的问题，监督式学习不足以成为“真的”AI。 
他指出，人类的学习是建立在与事物互动的过程，许多都是人类自行体会、领悟出对事物的理解，不需要每件事都要教导，举例来说，若有个物体被前面的物体挡住，人类会知道后面的物体依然存在的事实，或是物体没有另一个物体支撑就会掉落的事实。 
“人脑就是推理引擎！”他说明，人类靠着观察建立内部分析模型，当人类遇到一件新的事物，就能用这些既有的模型来推测，因为生活中人类接触到大量的事物和知识，而建立了“常识”。这些常识可以带领人类做出一些程序无法达到的能力，像是人类可以只看一半的脸就能想像另外一半脸，或是可以从过去的事件推测未来等。 
他举例，若人类看到一张战利品放不下行李箱的图片，再看到一个句子说：”这些战利品放不下行李箱，因为它太小了。“人类能够很清楚地知道“它”指的是行李箱，人类也因为知道整个社会和世界运行的规则，当没有太多的信息时，人类可以依照因果关系自动补足空白的信息。
无监督式学习是突破 AI 困境的关键，采用无监督学习的对抗训练让 AI 拥有真正自我学习的能力。
如何让 AI 拥有人类的常识？Yann LeCun认为要用无监督式学习。他又称之为预测学习，他将现今机器学习的方式分为强化式、监督式和无监督式学习，并以黑森林蛋糕来比喻。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwR3sCuibr9JicM8z0UiaCziay1G7MuSp9ibPdEULd1pDnsfqGunAlrgETQOpw/640?wx_fmt=jpeg)
强化学习是蛋糕上不可或缺的樱桃，所需要资料量可能大约只有几个Bits，监督式学习是蛋糕外部的糖衣，需要10到10,000个Bits的资料量，而无监督学习则是需要数百万个Bits，无监督学习被他比喻为黑森林蛋糕，因为无监督学习的预测能力像拥有黑魔法一样神奇，不过，他也强调黑森林蛋糕必须搭配樱桃，樱桃不是可选择的配料，而是必要的，意味着无监督学习与强化学习相辅相成，缺一不可。 
Yann LeCun认为，程序还是很难在不确定性的情况下，正确地预测，举例来说，如果一只直立的笔，没有支撑之后，程序可以判断出笔会倒下，但是无法预测会倒向哪一个方向。
因此，他表示，对抗训练（Adversarial Training）是可以让 AI 程序拥有自学能力的方法，他解释，对抗训练就是让两个网络相互博奕，由生成器（Generator）和判别器（Discriminator）组成，生成器随机地从训练集中挑选真实数据和干扰噪音，产生新的训练样本，判别器再用与真实数据比对的方式，判断出数据的真实性，如此一来，生成器与判别器可以交互学习自动优化预测能力，创造最佳的预测模型。
**————**
编辑 ∑ Pluto
来源：新智元
**更多精彩：**
☞[泰勒定理的奇闻轶事](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484097&idx=1&sn=9566979109fefc1907edad8a0b67cb27&chksm=ebe9c87cdc9e416ac5246da86ef23e6b45671441511a210554a965eb8c5a5ad210f72834a141&scene=21#wechat_redirect)
☞[丘成桐：漫谈微分几何](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484094&idx=1&sn=1a93ee4b182b6fd3a74bd4b93d205d9e&chksm=ebe9c803dc9e4115ae1e90d7e64af1ccb0687cb77237d45fdc5039f771ec122da2d9b7ece9fd&scene=21#wechat_redirect)
☞[Leibniz 如何想出微积分？（一）](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484092&idx=1&sn=56e2befc4cbcc6e0a138c41c0a445451&chksm=ebe9c801dc9e4117126f5df3938fe5b93ce10ca4bb00692fe16e906e64620c14754895e24d9c&scene=21#wechat_redirect)
☞[线性相关和秩的物理意义](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484090&idx=1&sn=c4778f29071d0388f6f7d5d25b2207d3&chksm=ebe9c807dc9e4111784b02ad43ed7b9958a0da5108c9db84a659f2b1d853a4a982bcef72aacc&scene=21#wechat_redirect)
☞[数学史上你认为最丑陋的公式是什么？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484087&idx=1&sn=466ad19b059aa635a0ae282adca9342e&chksm=ebe9c80adc9e411c38ec3006c5bc2f622b4d7d6a3887f356db114ed3b79f0bf957a43b79eb75&scene=21#wechat_redirect)
☞[陶哲轩谈什么是好的数学](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484083&idx=2&sn=107822b9a73eba79d34ba6a8488b13a6&chksm=ebe9c80edc9e411812d049bdd6ca8d4607688e815c8ba92c504748fdbbeab852f3166371eb9d&scene=21#wechat_redirect)
☞[田渊栋：数学的用处（下篇）](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484080&idx=1&sn=b754ee91c37493f3cff118a925f3d7d8&chksm=ebe9c80ddc9e411bc25ceef06631a406d7eafc73bb913f697b533c67665396e55565d1675bcd&scene=21#wechat_redirect)
☞[你绝对没想过原来数学家这么流氓，一言不合就进行暴力证明](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484029&idx=1&sn=d85a35df3ebd0884f3cb60536c5ccf75&chksm=ebe9c8c0dc9e41d6b43a3918624d8885f91c9af2346c2163c54c090c80c58ab844872278635c&scene=21#wechat_redirect)
☞[世界上最牛的五篇博士论文](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484037&idx=1&sn=16e68092098b573bfff13f69c28b6285&chksm=ebe9c838dc9e412e4f0eceece3372c28256508af1f77d714dd1a7b8fd7f20de71e01d77bc836&scene=21#wechat_redirect)
☞[数学中有哪些巧合让人眼前一亮？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484082&idx=2&sn=85da1e76707c8c1a9120cbbcf1da9aca&chksm=ebe9c80fdc9e411984cf41a248860de6982e8c9d291f25fe4420fbf43c38b138a2ea38bd644a&scene=21#wechat_redirect)
☞[算法立功！清华毕业教授美国被抢车，警察无能为力自己用“贪心算法”找回](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247484014&idx=1&sn=5b94b17a277a0e25581d58a2582b5d0a&chksm=ebe9c8d3dc9e41c57f2137c285ac44ec24f6e20029158351588cf519b022ba65f417a0710fac&scene=21#wechat_redirect)
☞[学术史上的奇文：怎样用数学抓狮子](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483984&idx=2&sn=620b603edf54785adeeae775b5cf7325&chksm=ebe9c8eddc9e41fba6883062a4b82f05d17d8292771a397d2fafdad2b1a77062472e95e13405&scene=21#wechat_redirect)
☞[台大教授的反思：最难的一课 我们却没教给学生](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483968&idx=1&sn=098491812586b1806465ef2c7f055b39&chksm=ebe9c8fddc9e41eb88a15c509a228568da5cdbfe4eeda595941c9bd5d25a135fef17a239fc00&scene=21#wechat_redirect)
[☞麻省理工学院(MIT)研究生学习指导—— 怎样做研究生](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483962&idx=1&sn=13ebe38d0b455b43c525284f550f1eda&chksm=ebe9c887dc9e4191bbe4c64d9027a27e94b01aaa6ae8435e2e5846374b3b94b8a8c46ddaaecb&scene=21#wechat_redirect)
[☞分享 数学，常识和运气 ——投资大师詹姆斯·西蒙斯2010年在MIT的讲座](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483957&idx=1&sn=f97ade0ac2643a31e2859f2f0186509f&chksm=ebe9c888dc9e419e8623cf164d9d5964ffcf80d6ac1c90c9cbe721533ec0848495bcab82e5be&scene=21#wechat_redirect)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
