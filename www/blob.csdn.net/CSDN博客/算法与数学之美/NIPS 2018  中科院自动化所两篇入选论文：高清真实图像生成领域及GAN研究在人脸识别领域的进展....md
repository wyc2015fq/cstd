# NIPS 2018 | 中科院自动化所两篇入选论文：高清真实图像生成领域及GAN研究在人脸识别领域的进展... - 算法与数学之美 - CSDN博客
2018年11月01日 21:21:12[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：868
##### **【论文一】自动化所在高清真实图像生成领域获得新突破**
**IntroVAE: Introspective Variational Autoencoders for Photographic Image Synthesis**
*原文地址：https://arxiv.org/abs/1807.06358*
自动化所智能感知与计算研究中心提出一种新的深度生成模型——**自省变分自编码器(IntroVAE)**，用来实现高清图像等高维数据的无条件生成。该模型不仅在不引入额外的对抗判别器的情况下，克服了变分自编码器固有的合成图像趋于模糊的问题，而且在不使用常用的多阶段多判别器策略下，实现了高分辨率图像合成的稳定训练。**该论文被今年人工智能顶级会议神经信息处理系统大会(NIPS2018)所收录，在生成指标上超过了英伟达在ICLR18上的工作。**
生成模型包含无条件生成(unconditional generation)和条件生成(conditional generation)两种，高分辨率图像生成一般指的是从噪声中生成数据的无条件生成。Yoshua Bengio在CVPR2017论文中讲到，"生成高分辨率照片级图像已成为机器学习领域的一个长期目标。"麻省理工大学的人工智能领域知名学者Max Tegmark在IJCAI2018特邀报告中提到，"以对抗生成网络为代表的深度生成模型是人工智能研究的重要前沿方向。"高分辨率真实图像的生成由于问题困难，计算复杂度大，一直以来只有英伟达(INVIDIA)、英国牛津(Oxford)和麻省理工(MIT)等知名科研机构在研究。
# 论文概要
近日，自动化所智能感知与计算研究中心提出一种新的深度生成模型——**自省变分自编码器(Introspective Variational Autoencoder，IntroVAE)**，用来实现高清图像等高维数据的无条件生成(unconditional generation)。该模型一方面在不引入额外的对抗判别器的情况下，克服了变分自编码器固有的合成图像趋于模糊的问题；另一方面在不使用常用的多阶段多判别器策略下，实现了高分辨率图像合成的稳定训练。实验结果表明，**该模型不仅能够稳定生成高分辨率照片级图像(比如1024x1024的人脸图像)，而且在生成模型常用的量化指标上取得了目前最好的结果。**
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/gYUsOT36vfq0UoILzdWbHm9ibZvka1wThhPJhNtZONPEzyAXibtiaXzIyuzBItoibfmTeibl0TUvkQK6GwMQ4ZYAOjg/640?wx_fmt=gif)
# 变分自编码器 VS 对抗生成网络
深度生成模型是无监督学习最有前景的方法之一，一直是学术界研究的热点问题。**目前最为流行的两种深度生成模型是变分自编码器(VAEs)和对抗生成网络(GANs)。**变分自编码器是自动编码器的生成版本，通过优化一个变分下界来实现数据到先验分布的近似映射。VAEs的训练稳定，能够进行隐变量推断和对数似然估计，但是生成的样本比较模糊。对抗生成网络通过生成器和判别器之间的对抗，来学习真实数据的分布。GANs可以生成逼真的清晰图像，但是存在训练不稳定的问题，这个问题在合成高分辨率图像上尤其严重。
# 自省变分自编码器的结构和训练流程
目前主流的高分辨率图像合成方法(比如英伟达公司提出的PGGAN模型)通过将高分辨率图像分解，从低分辨率出发，分多个阶段使用多个判别器逐步合成高分辨率图像。这种训练方式增加了模型设计的复杂度，提高了模型收敛的难度。**与已有的方法不同，本文采用了一种更为简单有效的方式，能够实现对高分辨率图像的一步到位直接合成。**该方法将对抗学习引入VAE内部，实现了一种自省的学习，即模型自身能够判断其生成样本的质量并作出相应改变以提高性能。**具体的实现方式是训练编码器使得真实图像的隐变量接近先验分布，合成图像的隐变量偏离先验分布；与之相反的是，训练生成器使得合成图像的隐变量接近先验分布。**同时，与GAN不同的是，编码器和生成器除了对抗外还要协同保证对输入图像的重建误差尽量小。对于真实数据来说，该方法的训练目标跟传统VAE完全一致，这极大得稳定了模型训练；对于合成数据来说，对抗的引入提高了样本的质量。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3S5ngcYBG568mwxDP7MeXSupibibUC5qAxM4lUnOC7VicCOpaOeyJxJQDQ/640?wx_fmt=png)
**IntroVAE的体系结构与训练流程**
# 实验结果
实验结果显示，**自省变分自编码器能够稳定合成高分辨率照片级的图像，**比如1024x1024大小的人脸图像，256x256大小的卧室、教堂、狗等自然图像。**该模型不仅在图像质量上，而且在量化指标上都取得了当前最好的结果。**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3Hv9BBaLlgtYibXiasVg3WCiaiasT0ND8LKELjW76uQ50oAmOp5H2WmcoAg/640?wx_fmt=png)
**CelebA-HQ上训练，生成的1024x1024人脸图像**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3o4w6MkSVPT7lNmb4VC7gJjwJu8RdycHvh93BluFAVt2QUwcVibe8K7g/640?wx_fmt=png)
**LSUN BEDROOM上训练，生成的256x256卧室图像**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw34kO6czVocDG4XQX4e54lNHpGfHPyudtVwPILmZ35WmWnIOWkaRkdeA/640?wx_fmt=png)
**LSUN CHURCHOUTDOOR上训练，生成256x256教堂图像**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3fUDPDZZyJgNLaUY8SAU8cpZ5mET3j7f5henkwzWZtRQ8vPGwo1QAcA/640?wx_fmt=png)
**ImageNet上训练，生成的256x256狗图像**
和其他方法的量化指标对比，可以看出在大部分指标上自省变分编码器都取的了最好的结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3kSlaUVR4GgBvHOtr0Ub9k0FxibGjAa4BnMiamrhcr0yBGpBHDyNus9lA/640?wx_fmt=png)
**两种方法的定量比较**
##### **【论文二】自动化所生成对抗网络GAN研究在人脸识别领域再获新进展**
**Learning a High Fidelity Pose Invariant Model for High-resolution Face Frontalization**
*原文地址：https://arxiv.org/abs/1806.08472*
自动化所智能感知与计算研究中心在生成对抗网络（GAN）基础上提出高保真度的姿态不变模型来克服人脸识别任务中最为经典的姿态不一致问题。**该模型不仅在多个基准数据集的视觉效果和定量指标都优于目前已有的基于生成对抗网络的方法，**而且将生成图像的分辨率在原有基础上提升了一倍。该论文已被神经信息处理系统大会(NIPS)收录。
# GAN缘何入选2018年"全球十大突破性技术"？
日前，《麻省理工科技评论》正式公布了2018年“全球十大突破性技术”(10 Breakthrough Technologies 2018)，涵盖：实用型金属3D打印、人造胚胎、智慧传感城市、面向每一个人的人工智能、**对抗性神经网络/生成对抗网络**、巴别鱼实时翻译耳塞、零碳天然气、完美的网络隐私保护、基因占卜、材料的量子飞跃。这也是这份指南第17次发布。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3fGeII7j0rVfpwzJecp5kdBkbVtfZgPUWrQmBpdBXNHliaPE88NNGmbA/640?wx_fmt=png)
**来自网络**
回看过去几年的上榜技术，越来越多的人工智能相关技术入选榜单，其中包括：2008 年机器学习、2009 年的 Siri、2013 年的深度学习、2014 年的神经形态芯片、2016 年的语音接口与知识分享型机器人，以及 2017 年的自动驾驶卡车与强化学习。
2018年，最具突破性的人工智能技术之一就是对抗性神经网络(Dueling Neural Networks) / 生成对抗网络(GAN)——通过两个AI系统的竞争对抗，极大化加速机器学习的过程，进而赋予机器智能过去从未企及的想象力。
# 自动化所改良后的GAN，性能更优越
Ian Goodfellow发明出GAN后，获得Yann LeCun、黄仁勋、吴恩达等人的赞赏，吸引了诸多的机构及企业开始研究。在中国，部分学术机构致力于研究GAN理论的近一步改良及优化，**其中中科院自动化所孙哲南、赫然研究团队已经取得了一系列进展，受人类视觉识别过程的启发，提出了双路径GAN(TP-GAN)用于正面人脸图像合成，已发表于2017年国际计算机视觉大会（ICCV）。随后又提出了基于姿态信息指导的共轭判别器GAN（CAPG-GAN），用于多姿态人脸图像合成，并进一步提高了跨姿态识别精度，已发表于2018年IEEE国际计算机视觉与模式识别会议（CVPR）。**
近日，自动化所**曹杰、胡一博、张鸿文、赫然、孙哲南**在论文Learning a High Fidelity Pose Invariant Model for High-resolution Face Frontalization中提出了高保真度的姿态不变模型(High Fidelity Pose Invariant Model, HF-PIM)来**克服人脸识别任务中最为经典的姿态不一致问题。**
实验结果表明，该方法在多个基准数据集的视觉效果和定量指标都优于目前已有的最好方法。此外，HF-PIM所支持的生成图像分辨率也在原有方法的基础上提升了一倍。该论文已被人工智能领域顶会神经信息处理系统大会(NIPS)收录。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/gYUsOT36vfq0UoILzdWbHm9ibZvka1wTh1xXTFoXI5fDq1Vo2sMnnGDMsicSluT2s7F3drMtmcyvRDyo0TEarW6Q/640?wx_fmt=jpeg)
高保真度的姿态不变模型示意图
# 论文解析
在该论文中，作者总结了先前工作中存在的一些限制，例如，过于依赖低维信息约束，不能很好地保持原图的语义信息等。为了解决这些问题，作者对其具体做法进行如下总结：
①引入了一种能反映三维人脸模型和二维人脸图像之间点到点关联的稠密关联场，让网络能够在二维图像的指导下学习到隐含的三维人脸信息。
②设计了一种全新的纹理扭曲(warping)过程，可以有效地把人脸纹理映射到图像域，同时又可以最大程度地保持输入的语义信息。
③提出了一种对抗残差字典学习过程，从而可以在不依赖三维数据的情况下更有效地学习人脸纹理特征。
④实验表明，该方法不仅在定量和定性指标上明显地超过了已有方法，并且把生成图像的分辨率提高了一倍。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3nbaUumIch1uyD5XiaXib2I2oZmmyj7MvCZMYnlSpvpaEBMuVJtQIfYzw/640?wx_fmt=png)
**来自论文**
上图是在CelebA数据库的HQ子集上的可视化结果，第一行为输入图像，第二行为通过HF-PIM模型进行人脸正面化后的结果。**HF-PIM是首个支持生成256*256可视化结果的模型。**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3aBcxRyb011Mkea3VvVdXAvkoreib1MIWWE0vEB69R0I4Bg3RahPCBvQ/640?wx_fmt=png)
**来自论文**
上图在IJBA数据库上的可视化结果，第一行为输入图像，第二行为通过HF-PIM模型进行人脸正面化后的结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw37jZxOJQZLP8nUTGLou3SWFlrySMybwibSHkjsk4KTxliajZILC84XScA/640?wx_fmt=png)
**在LFW（左图）数据库和IJB-A（右图）数据库上和已有方法的对比结果。ACC: 准确率。AUC：ROC曲线下面积。FAR：错误接受率。Rank-1：第一次命中识别率。**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzfOkWW0L4snA5kEPiaglMw3DpCVV2apAOnRT3NWUYNplPnfic8tsTf57bEosibzFcXcPVJp3lX2JicQA/640?wx_fmt=png)
**在Multi-PIE数据库上在不同的视角下对第一次命中识别率的对比。**
∑编辑 | Gemini
来源 | 极市平台
**更多精彩：**
☞  [哈尔莫斯：怎样做数学研究](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=1&sn=35143b89b06fe4f5273f210b2d6a7c91&chksm=8b7e3290bc09bb86f7bb3f158d993df3f019a7e9ce3bc8897e164e35a2ebe5a4e0bdcc111089&scene=21#wechat_redirect)
☞  [扎克伯格2017年哈佛大学毕业演讲](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=2&sn=c75293463823e4d6769638e54b64f3ec&chksm=8b7e3290bc09bb86dc1e3f8e78d0b6de8811d75f3dcb092766fcb8ba0bab1cd9ba1ddfcef3b9&scene=21#wechat_redirect)
☞  [线性代数在组合数学中的应用](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554141&idx=1&sn=74a74c4e4d08eba0dd734528aa0b08e7&chksm=8b7e32c6bc09bbd073b34c22004ac6e4d99c8a0caa64c7d3dbaa8fd55e6ef1fc87ed545b8b7e&scene=21#wechat_redirect)
☞  [你见过真的菲利普曲线吗？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554105&idx=1&sn=224ab0d38fb57facea70081385360d58&chksm=8b7e3222bc09bb34d3b6df665087e64b233778ed427598d08e809f96261e898c1c0de6188bbc&scene=21#wechat_redirect)
☞  [支持向量机(SVM)的故事是这样子的](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554096&idx=1&sn=46783e6ace661a3ccbd8a6e00fb17bf9&chksm=8b7e322bbc09bb3d73dc240f2280bddf2ef8b7824a459a24bd7f6eeadd60edb96e690d467f6e&scene=21#wechat_redirect)
☞  [深度神经网络中的数学，对你来说会不会太难？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554077&idx=2&sn=3ecd63f2205fd59df8c360c97c943ef6&chksm=8b7e3206bc09bb10a36b09547efe0c54f41423b180622c1fdc7f14747ccc8f8fecee3a12e2cd&scene=21#wechat_redirect)
☞  [编程需要知道多少数学知识？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554062&idx=1&sn=17f0a88d5e15d1adfc29c690a0b1b89b&chksm=8b7e3215bc09bb038c6caa59d0f49cedd929f9be1104beea3411186cf4c81de69efc71a17883&scene=21#wechat_redirect)
☞  [陈省身——什么是几何学](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553994&idx=2&sn=74f67a1a3ac5c705f51f2ba619b717f6&chksm=8b7e3251bc09bb47dce73319948780081efe0333ffae99ea04a9eeabbcfcb38a29b4b73fb7c1&scene=21#wechat_redirect)
☞  [模式识别研究的回顾与展望](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553949&idx=2&sn=d171680964df774397efd9db81c00347&chksm=8b7e3386bc09ba90bf0f6e1cabf82ba86ff94630cb5ee2e0f14ff9455db52be32ddbc289d237&scene=21#wechat_redirect)
☞  [曲面论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553747&idx=1&sn=e25f866d510cf2338b6d9e1b32bafb62&chksm=8b7e3348bc09ba5ea1caaf2a7bfcd80a7e7559b1983e473eda2206e56df7f38ef3cecf2f77c7&scene=21#wechat_redirect)
☞  [自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553811&idx=1&sn=000305074471c3d4c681c9cfd4e4bc93&chksm=8b7e3308bc09ba1e3043f5568a3a75a045285a1de97e4da36918bac68e7c6d579ad5d8cc25ab&scene=21#wechat_redirect)
☞  [如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553670&idx=1&sn=ea75a448c016f7229e4cb298f6017614&chksm=8b7e309dbc09b98bc622acdf1223c7c2f743609d0a577dd43c9e9d98ab4da4314be7c1002bd5&scene=21#wechat_redirect)
☞  [华裔天才数学家陶哲轩自述](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553646&idx=2&sn=bbf8f1be1ca1c66ad3f3270babea6885&chksm=8b7e30f5bc09b9e3e1a4fa735412e2fcb20df9e78f2f346bf578018ceab77de6326095d1bf71&scene=21#wechat_redirect)
☞  [代数，分析，几何与拓扑，现代数学的三大方法论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553596&idx=1&sn=bc5064e871831f862db6d19c3de6327e&chksm=8b7e3027bc09b93194fa09b25e2df400421c062927bb9120912875f8aaf0bb25553fc8f51e3b&scene=21#wechat_redirect)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkya8z7IMHeJV8e2Ufs0XW03v7WJiaD4sJY0lswS1S3ULC5nKlnTkvApk5r46vn20ALciaMe5ia81cNug/640?wx_fmt=jpeg)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
