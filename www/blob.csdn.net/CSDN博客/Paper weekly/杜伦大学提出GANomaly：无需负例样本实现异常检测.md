# 杜伦大学提出GANomaly：无需负例样本实现异常检测 - Paper weekly - CSDN博客





2018年09月19日 12:30:12[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1892









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **105** 篇文章


本期推荐的论文笔记来自 PaperWeekly 社区用户 **@TwistedW**。在异常检测模块下，如果没有异常（负例样本）来训练模型，应该如何实现异常检测？**本文提出的模型——GANomaly，便是可以实现在毫无异常样本训练下对异常样本做检测。**

 如果你对本文工作感兴趣，点击底部**阅读原文**即可查看原论文。

# 关于作者：武广，合肥工业大学硕士生，研究方向为图像生成。

■ 论文 | GANomaly: Semi-Supervised Anomaly Detection via Adversarial Training

■ 链接 | https://www.paperweekly.site/papers/2293

■ 源码 | https://github.com/samet-akcay/ganomaly




异常检测（Anomaly Detection）是计算机视觉中的一个经典问题，生活中大部分的数据是正常数据，有很少一部分属于异常数据，在很少的异常下如何检测出异常是一个困难的课题，甚至不知道什么是异常，只知道不属于正常的就算异常的话又如何检测异常呢？**GANomaly 便是可以实现在毫无异常样本训练下对异常样本做检测**，我们一起来读一下。




# 论文引入




在计算机视觉上大部分的检测任务的前提是需要大量的标记数据做训练，这虽然在成本上耗费巨大，但是在实验效果上确实有很大的突破，目前的目标检测技术已经上升到近乎实时检测的效果了，背后的人力和成本也是可想而知的。




在庞大的目标检测背景下，异常检测算是一个特立独行的分支，虽然只是判断正常和异常两种情况（二分类问题），但是往往异常样本特别的少，如果从特征提取上区分正常和异常的话，由于训练样本过少或者说是训练样本比例太不平衡（正常样本特别多）往往会导致实验结果上不尽如人意。 




异常检测的发展在深度学习的浪潮下得到了很快的发展，基于 CNN，RNN、LSTM 技术上已经取得了一定的成效。随着 GAN 的提出，对抗的思想越来越引人注意，利用 GAN 做异常检测的文章在实验上有了一定的突破，从 **AnoGAN **[1] 学习到正常样本的分布，一旦送入异常样本数据发生改变从而检测出异常，这种方法的局限性很强，往往也会带来计算成本的昂贵（需要严格的控制先验分布 z）。在此基础上，为了找到更好用作生成的先验分布 z，在 AnoGAN 的基础上提出了 **Efficient-GAN-Anomaly**[2] 同样由于做个一次重新映射导致计算成本上也是庞大的。 




**GANomaly 算是在前两篇文章的基础上做了一次突破，不再比较图像分布了，而是转眼到图像编码的潜在空间下进行对比。**对于正常的数据，编码解码再编码得到的潜在空间和第一次编码得到的潜在空间差距不会特别大。但是，在正常样本训练下的 AE 用作从未见过的异常样本编码解码时，再经历两次编码过程下往往潜在空间差距是大的。




**当两次编码得到的潜在空间差距大于一定阈值的时候，我们就判定样本是异常样本，这就是 GANomaly 的思路。**我们以此对比一下以 GAN 为发展下的异常检测模型，模型结果如下，上述已对其做了一定的分析。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDG0nf9FIfIRDA0CZ51vATp49hPQAXyNNMicYZ8UDicibjE8aUn5SKI32eg/640)




**GANomaly 的优势总结一下：**



- 
半监督异常检测：编码器-解码器-编码器流水线内的新型对抗自动编码器，捕获图像和潜在向量空间内的训练数据分布；




- 
功效：一种有效且新颖的异常检测方法，可在统计和计算上提供更好的性能。





# GANomaly模型




我们还是先看一下模型框架：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDemVEh06m5bKbuYvFVtpicb34UpcJeBicicxicCHmia55YuDVxPG9Kic441hA/640)




GANomaly 模型框架是蛮清晰的，**整个框架由三部分组成：**




GE(x),GD(z) 统称为生成网络，可以看成是第一部分。这一部分由编码器 GE(x) 和解码器 GD(z) 构成，对于送入数据 x 经过编码器 GE(x) 得到潜在向量 z，z 经过解码器 GD(z) 得到 x 的重构数据 x̂ 。




模型的第二部分就是判别器 D，对于原始图像 x 判为真，重构图像 x̂ 判为假，从而不断优化重构图像与原始图像的差距，理想情况下重构图像与原始图像无异。




模型的第三部分是对重构图像 x̂ 再做编码的编码器 E(x̂) 得到重构图像编码的潜在变量 ẑ。 




在训练阶段，整个模型均是通过正常样本做训练。也就是编码器 GE(x)，解码器 GD(z) 和重构编码器 E(x̂)，都是适用于正常样本的。




当模型在测试阶段接受到一个异常样本，此时模型的编码器，解码器将不适用于异常样本，此时得到的编码后潜在变量 z 和重构编码器得到的潜在变量 ẑ 的差距是大的。我们规定这个差距是一个分值![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDckIVSwIdicYGtFkaic5GiaKV7EiaB9ZwzzpzjXSRNnhBCeGv5krhS9U1OQ/640)，通过设定阈值 ϕ，一旦 A(x)>ϕ 模型就认定送入的样本 x 是异常数据。 




**网络损失**




对于模型的优化，全是通过正常样本实现的，网络损失也可分为三部分。标准 GAN 的损失大家相比都很清楚了，这里不重复写。对于第一部分的生成网络下，文章给定了一个重构误差损失，用于在像素层面上减小原始图像和重构图像的差距。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDqxOicDwU3xkBnA2aSDuicpmoia7ibVmoqwlePm8OGYytgo06uJK9t0npKg/640)




对于第二部分判别器下，设置了一个特征匹配误差，用于在图像特征层方面做优化，这部分损失其实已经在很多文章中都用到过。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDpeicPWRiboeAqIeJsysfXkakU9x0ibia99SyEWaoI5o6KfXt7q9p58CNBw/640)




对于第三部分重构图像编码得到的潜在变量 ẑ ，这部分对于正常数据而言，希望得到的 ẑ 与原始数据直接编码得到的 z 无差别最好，也就是对于最好的得分判断，对于正常数据而言理想状态下希望![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDRfwG6qR0N9BG3GO8je5HvJN1YiaD4P3X2KpRdvLo3ZONI4nolncwiaqw/640)。当然这是最理想的状态，但是对于正常数据还是希望 A(x) 越小越好，所以引入了一个潜在变量间的误差优化。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDZ4N9iaBriaobjFkyuwRwZ3kLzicr5E9qYIVkNth5v6kYFCFdx3XZPibfRQ/640)




训练过程中只有正例样本参与，模型只对正例样本可以做到较好的编码解码，所以送入负例样本在编解码下会出现编码得到的潜在变量差异大从而使得差距分值 A(x) 大，判断为异常。对于模型，整个损失函数可以表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zND7OpyULRYQSBMtEwJVE6yOic1EXFbhTN5Am2DmRPbHhCFMScicQkChRFw/640)




这里的 ωadv,ωcon,ωenc 是调节各损失的参数，可以根据具体实验设置。




**模型测试**




模型测试过程中以正负样例 D̂ 混合输入，对于测试样本下的得分 S 可记为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDvLmu5HsVDcwYPZLngRVHsAVhG5jCL3qHmBPOJJibynRHNYiaPoCXRQOg/640)具体的判断异常分数进行一个归一化处理将其整合到 [0,1] 之间。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zND2s9BdPrbbA2miarCqI4yiahY6FlwxU8ktOnmxLcENv20g3rC6rATm6wQ/640)




这里的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDO2CSZ18TIBmYa13ZYOecksqChCVtg9mm5u4ztlvWgfXBu2HicJjr4ug/640)就是最终的异常得分，对于正常样本理论上希望![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDNqvBl56HAgDsYBPb3zT0iaOhoiczht1d6VbOH2E7icWWLaDpDCPiaXtpQA/640)，对于异常样本理论上希望![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zND2SI3MkBHI6t4AiaapWo4cSCEB1MI9qOznZCJuxLV9qEgLjuSEyAc7AQ/640)。需要一个阈值 ϕ 来衡量这个标准，经过源码分析，一般的 ϕ=0.2 可以根据实际项目需要对 ϕ 做调整。




# GANomaly实验




实验在 MNIST，CIFAR10 上选取部分类别做正样本，选取一些类别作异常样本，测试模型是否能够检测出异常样本并给出准确率。文章的衡量标准是以 AUC 为判断。**实验对比了三种以 GAN 做异常检测的模型以及 VAE 的结果，通过 AUC 分析可以看出 GANomaly 取得了不错的优势。**



**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDF0ibOSd7EN51BrgfEfkYiblMbtFgLm5Ew8GVGAkBQ9xLYZicqvA4RicxuQ/640)**




实验还对大件行李异常数据集（University Baggage Anomaly Dataset - (UBA)）做了实验，数据集包括 230275 个图像块，图像从完整的 X 射线图像中提取，异常类别（122803）有 3 个子类 - 刀（63,496），枪（45,855）和枪组件（13,452），对于另一个数据集选择了枪械检测（FFOB）。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDeO1qnCSep4evRNP13PaZQ1pvyGDEoUZEcHMv1b6gr1XxmAGlQJsb3g/640)




对于潜在变量的选取，以及超参的确定也通过实验选取：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDy5fUhz5PDic67Z51evicZSUv64Hl6L30UIJ1bUXtibiapqrJHmTYyrJtQA/640)




最后来看一下正常样本和异常样本重构的对比，可以看出异常样本在重构上已经和原始有了较大的差别了，所以编码得到的潜在变量自然会产生差异，从而判断出异常。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgljqaF0185hPv1ESd7B3zNDicoiaiaoziaEtg1qibM42ZvSlys6HwSfjS4PCx9ibxT8lzjliabtWahrULVgQ/640)




# 总结




GANomaly 以编码器-解码器-编码器设计模型，通过对比编码得到的潜在变量和重构编码得到的潜在变量差异，从而判断是否为异常样本。**文章在无异常样本训练模型的情况下实现了异常检测，对于很多场景都有很强的实际应用意义。**个人感觉文章对于异常分数的计算和判断可再进一步优化，从而实现更好的异常检测效果。




# 参考文献




[1]. Thomas Schlegl, Philipp Seebock, Sebastian M. Waldstein, Ursula Schmidt-Erfurth, and Georg Langs. Unsupervised anomaly detection with generative adversarial networks to guide marker discovery. Lecture Notes in Computer Science (including subseries Lecture Notes in Artificial Intelligence and Lecture Notes in Bioinformatics), 10265 LNCS:146– 147, 2017.

[2]. Houssam Zenati, Chuan Sheng Foo, Bruno Lecouat, Gaurav Manek, and Vijay Ramaseshan Chandrasekhar. Efficient gan-based anomaly detection. arXiv preprint arXiv:1802.06222, 2018.

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击标题查看更多论文解读：**




- 
[ECCV 2018最佳论文：基于解剖结构的面部表情生成](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491543&idx=1&sn=e40163badcbeb30cc079c659677b04b5&chksm=96e9c057a19e4941dbf6fea886067f10e7ba4a62b8ea684e2d8852045125f201c0d90d63207e&scene=21#wechat_redirect)

- 
[神经网络架构搜索（NAS）综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[ECCV 2018 | 从单帧RGB图像生成三维网格模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491465&idx=1&sn=a6871b72d460debf90d2daa0bed719c8&chksm=96e9c009a19e491f8c247f36a53bad0a6812c462f3fb5b2d9ae74b38be673b946b82a4b44330&scene=21#wechat_redirect)

- 
[ECCV 2018 | 基于三维重建的全新相机姿态估计方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491494&idx=1&sn=2922261ba0775f00ad67042dda355b52&chksm=96e9c026a19e493039bf5a90a1d523a46167df4058ee16f7f0797f10f6d392e0c98ef510acf2&scene=21#wechat_redirect)

- 
[ECCV 2018 | 腾讯AI Lab提出视频再定位任务](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491347&idx=1&sn=e1f2cc16c9fcfcc5d2935118f09ee094&chksm=96e9c093a19e49855931cf621ec7f715c1d2dd5041e3343bc311aea2e5069ae03aaa1367e8f9&scene=21#wechat_redirect)

- 
[KDD 18 | 斯坦福大学提出全新网络嵌入方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491364&idx=1&sn=afa577a1bebfd148ac20cde05a927e24&chksm=96e9c0a4a19e49b2458f451c6e201277c509905201a1f751b30fbfa1e2ba31bc2711b2ca1b7a&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向） 

• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接 

• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志



**📬 投稿邮箱：**

• 投稿邮箱：hr@paperweekly.site

• 所有文章配图，请单独在附件中发送 

• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通










🔍




现在，在**「知乎」**也能找到我们了

进入知乎首页搜索**「PaperWeekly」**

点击**「关注」**订阅我们的专栏吧







**关于PaperWeekly**



PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)

▽ 点击 | 阅读原文| 下载论文 & 源码




