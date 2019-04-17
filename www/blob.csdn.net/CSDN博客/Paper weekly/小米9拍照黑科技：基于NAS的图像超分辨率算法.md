# 小米9拍照黑科技：基于NAS的图像超分辨率算法 - Paper weekly - CSDN博客





2019年02月22日 08:23:44[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：274









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者：Xiangxiang Chu, Bo Zhang等 丨

机器之心编译丨




**雷军表示：**这是一篇小米最新出炉的论文，**基于弹性搜索在图像超分辨率**问题上取得了令人震惊的结果，该模型已开源。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl8Nyb2cTwfAh78bLiaBXAksUZvgzbzYntVZRQFccMXPgxfLV2o8bWu7jibo6leJqZBFt9B6Kric3RFA/640?wx_fmt=png)




本篇是基于 NAS 的图像超分辨率的文章，**PaperWeekly 社区用户 @figo 在该文公布后迅速跟进**，发表分析称「属于目前很火的 AutoML / Neural Architecture Search，论文基于弹性搜索（宏观+微观）在超分辨率问题上取得了非常好的结果。这种架构搜索在相当的 FLOPS 下生成了多个模型，结果完胜 ECCV 2018 明星模型 CARNM，这应该是截止至 2018 年可比 FLOPS 约束下的 SOTA（涵盖 ICCV 2017 和 CVPR 2018）。 




而达到这样的效果，论文基于一台 V100 用了不到 3 天时间。需要指出的是，该项技术具有一定的普适性，理论上可以应用于任何监督学习，值得我们关注和学习。」 




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl8Nyb2cTwfAh78bLiaBXAksriblSXJsTF5PJQ6SdT02TpMPaoB5JxSuo3HruhaeT1omRS45iarjv6Qg/640?wx_fmt=png)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl8Nyb2cTwfAh78bLiaBXAksSrIHEYfC7aX6bFpy1HVCFrjoMHeu83ntod1pFkMqEBmHAZ0ibWKFhXg/640?wx_fmt=png)




作为计算机视觉中的经典任务，单帧超分辨率（SISR）旨在基于低分辨率图像重构出对应的高分辨率图像。目前大多数 SISR 算法都借助深度学习的非线性拟合能力，并大幅超越了传统视觉算法。直观而言，图像超分辨率是一种提炼并扩展图像信息的方法，模型需要根据图像的整体语义信息重构出欠缺的细节。因此与一般的图像锐化或清晰度调整不同，图像超分辨率任务有着更高的要求。 




目前已有的超分辨率模型大多数都是人工设计的，它们很难进行压缩或者微调。与此同时，神经架构搜索已经在分类任务中取得了极好的效果。根据这一趋势，Chu et al., 2019 提出了使用强化遗传搜索方法处理 SISR 任务，该方法优于 VDSR [Kim et al., 2016a] 等流行网络。 




在该论文中，作者基于弹性神经架构搜索探讨 SISR 任务，提出的方法已经达到与 CARN 和 CARN-M 相当的效果。**作者表示他们最主要的贡献可以总结为以下四点： **




1. 发布了几种快速、准确和轻量级的超分辨率架构和模型，它们与最近的当前最优方法效果相当； 




2. 通过在 cell 粒度上结合宏观和微观空间来提升弹性搜索能力； 




3. 将超分辨率建模为受限多目标优化问题，并应用混合型控制器来平衡探索（exploration）和利用（exploitation）；




4. 生成高质量模型，其可在单次运行中满足给定约束条件下的各种要求。 




此外，研究者还开放了预训练的模型和评估代码，他们在 GitHub 中提供了 FALSR A、B 和 C 三个预训练模型，同时也提供了一些示例数据集，包括 B100、Set14 和 Urban100 等。发布的模型基于 TensorFlow。如果想要使用预训练模型看看效果，直接复制 GitHub 项目，并运行以下命令行就行了：



`python calculate.py --pb_path ./pretrained_model/FALSR-A.pb --save_path ./result/`



# 流程架构




与大部分 NAS 方法类似，本研究提出的方法包含三个主要模块：弹性搜索空间、混合模型生成器和基于不完全训练的模型评估器。 




与 [Lu et al., 2018; Chu et al., 2019] 类似，本研究也使用 NSGA-II [Deb et al., 2002] 来解决多目标问题。但该研究的特点在于，它会使用混合型控制器和基于 cell 的弹性搜索空间（可支持宏观和微观搜索）。 




**该研究在处理超分辨率任务时考虑了三个目标：**



- 
反映模型性能的量化度量指标（PSNR） 

- 
评估模型计算成本的量化度量指标（mult-adds） 

- 
参数量 





此外，**该研究还考虑了以下约束： **



- 
满足人类视觉感知的最小 PSNR 

- 
资源限制下的最大 mult-adds





# 弹性搜索空间




本研究的搜索空间旨在执行微观和宏观搜索。微观搜索用于在每个 cell 的搜索空间的中选择有潜力的单元，可看作是特征提取器。而宏观搜索旨在搜索不同 cell 的跳接，其作用是结合选定层级的特征。此外，研究者使用 cell 作为最小搜索元素有两个原因：设计灵活性、通用的表征能力。 




通常，超分辨率任务可分为三个子步骤：特征提取、非线性映射和重建。由于大部分深度学习方法主要关注第二部分，因此该研究将其搜索空间设计用于描述非线性映射，其他部分保持不变。图 1 展示了该研究中提出的超分辨率任务主要基础结构。一个完整的模型包括预定义的特征提取器（具备 32 个 3 × 3 卷积核的 2D 卷积）、来自微观搜索空间的 n 个单元块和来自宏观搜索空间的跳接、上采样和重建。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl8Nyb2cTwfAh78bLiaBXAksYVpQ4OmZGibWan9aGCGWu9Nia7fwecX3lxGNgyjrYcTHicnLJx1s04n3Q/640?wx_fmt=png)

**▲**图1：本研究采用的基础超分辨率神经架构（cell间的箭头表示跳接）





![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl8Nyb2cTwfAh78bLiaBXAksIMXtAn6icwN6zQqYiabRXUuBMCiciaHu83Pnk58lyvbyDDHgscMQdzwEMA/640?wx_fmt=png)

**▲**图2：控制器结构




**与当前最优超分辨率方法的对比 **




该模型训练完成后，研究者在常用超分辨率任务测试数据集上将其与当前最优方法进行了对比（见表 1 和图 5）。公平起见，本研究仅考虑 FLOPS 相当的模型。因此，太深和太大的模型（如 RDN [Zhang et al., 2018b]、RCAN [Zhang et al., 2018a]）均不包括在内。研究者按照惯例选择 PSNR 和 SSIM 作为度量标准。对比在两倍超分辨率任务上进行，所有 Mult-Adds 均基于 480 × 480 的输入衡量。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl8Nyb2cTwfAh78bLiaBXAks5hdso5UToC9Qzib5D047pnYZDI28WXJoMNGuFgV097yFx6MAkNnGib2g/640?wx_fmt=png)

**▲**表1：在×2超分辨率任务上对比本文提出的方法和当前最优方法




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl8Nyb2cTwfAh78bLiaBXAksGjeLHonDEeAEshBToIxgNQYurCaCtDzzvia9iaPZsjQ4nw1sF0dxGyww/640?wx_fmt=png)

**▲**图5：FALSR-A、FALSR-B、FALSR-C（鲑红色）vs. 其他模型（蓝色）




在 FLOPS 相当的模型对比中，本研究提出的 FALSR-A 模型优于 CARN [Ahn et al., 2018]（图 3）。此外，FALSR-A 模型还在四个数据集和三个目标上超过 DRCN [Kim et al., 2016b] 和 MoreMNAS-A [Chu et al., 2019]。它的 PSNR 和 SSIM 值高于 VDSR [Kim et al., 2016a]、DRRN [Tai et al., 2017a] 等，且 FLOPS 低于后者。 




研究者还创建了一个更轻量级的版本——FALSR-B（图 4），它超越了 CARN-M，这意味着更少 FLOPS 与参数量的情况下，FALSR-B 的得分堪比甚至超越 CARN-M。此外，它的架构更吸引人，连接之间的复杂度介于稀疏和密集连接（dense connection）之间。这表示密集连接不总是信息传输的最优方法。来自更低层的无用特征可能会给高网络层重建超分辨率结果造成麻烦。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl8Nyb2cTwfAh78bLiaBXAkszzKc28niatQZ2wB56VRFFMlhdq8vn3Pr8Ak0T6rhLIoqfVIXkJHkRibg/640?wx_fmt=png)




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多往期内容：**




- 
[Airbnb实时搜索排序中的Embedding技巧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494488&idx=1&sn=452ac80c593a9b31252031eac38d0e01&chksm=96ea34d8a19dbdce940ed25bb93507aa6c4d118f84dd0bb965b060f232fe5d41894bbc9edcb6&scene=21#wechat_redirect)

- 
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)

- 
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)

- 
[F-Principle：初探理解深度学习不能做什么](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494694&idx=1&sn=7020fb834ce8307f27ce9c072047d37d&chksm=96ea33a6a19dbab0a6585daa00d5b5c65501dd633fa677c80541fad0e170d92baffe379315c3&scene=21#wechat_redirect)


- 
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)

- 
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)

- 
[AI Challenger 2018 机器翻译参赛总结](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494103&idx=1&sn=fc372862e0369b1f6a943bf997f6fc1b&chksm=96ea3657a19dbf4108bbc4179e779aa04ef05fe84f0013fa6425b0cd7e761e9880917361c4c1&scene=21#wechat_redirect)

- 
[Google BERT应用之红楼梦对话人物提取](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494507&idx=1&sn=3c3cafef0fb51a7e40d9b9bbab53fd5f&chksm=96ea34eba19dbdfd31eaa760bb7cfd5e18f2e967c83c6ea6693ad9a062c55b3009211d824ca3&scene=21#wechat_redirect)

- 
[深度长文：NLP的巨人肩膀（上）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493520&idx=1&sn=2b04c009ef75291ef3d19e8fe673aa36&chksm=96ea3810a19db10621e7a661974c796e8adeffc31625a769f8db1d87ba803cd58a30d40ad7ce&scene=21#wechat_redirect)

- 
[NLP的巨人肩膀（下）：从CoVe到BERT](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493731&idx=1&sn=51206e4ca3983548436d889590ab5347&chksm=96ea37e3a19dbef5b6db3143eb9df822915126d3d8f61fe73ddb9f8fa329d568ec79a662acb1&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



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




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 获取最新论文推荐




