# UC伯克利最新研究：舞痴和舞王之间，只差一个神经网络 - 知乎
# 



> 铜灵 栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI 
![](https://pic3.zhimg.com/v2-e0eea2d28908079cae7df44a219b5a9a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='350' height='279'></svg>)
不会跳舞？不存在的。

现在只要有别人跳舞的视频，你也随时都可以是视频里的主角，而且无需动辄昂贵的3D或是动作捕捉技术。

最近，加州大学伯克利分校的研究人员提出了一种简单的动作转移方法：输入一段优美标准的跳舞视频，再输入你的随性动作，几分钟后，一段流畅、标准、感人的舞蹈就被合成出来了，而里面的主角正是你~

无论是优美高雅的芭蕾舞：
![](https://pic2.zhimg.com/v2-82d9781fd4a10d58ab74ef09d7b541a1_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
还是动感活力的现代舞： 
![](https://pic1.zhimg.com/v2-51d959ad0593181123fc1060a391e570_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
都能Hold住，而且效果感人。

其实，无论什么舞种，只要给定目标视频和你自己的视频，随时可以C位出道。

果真这么神奇？

## **主要方法**

在这项任务中，研究人员提出了一种在不同视频中迁移动作的方法，即通过端对端基于像素的pipeline。

巧妇难为无米之炊，在合成视频前，我们需要先拿两个视频：一个是想最终合成那种style的源对象视频，并一种我们自己（目标人物）的随机动作视频。

大方法框架确定后，研究人员开始思考实现的细节。若是合成的视频卡顿还如何C位出道？因此，研究人员想**逐帧**实现两个视频主体之间的传输，就得学习两个人图像之间的**映射**，在源视频集和目标视频集中找到图像到图像的转换。

接下来就简单明了了，想完成图像转换，就得先找到关键点的姿势。两人外形之间的差异不重要，从动作中找到姿势图才是最重要的。
![](https://pic2.zhimg.com/v2-7fd4ee2ca5a9ae85b0f81cf37ed28191_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='540'></svg>)△ 从人物动作（右）中捕捉到的姿势图（左）
因此，研究人员对每一帧图像进行姿势检测，产生一组源视频中人和目标视频人物之间的一组姿势对，以**监督学习**的方式学习两人之间的姿势图。这样，经过训练后，模型就初步具备了合成舞蹈的能力。 
![](https://pic1.zhimg.com/v2-e59fd532406c1426a7d64ea4eb0b7bdc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='482'></svg>)△ 从源视频到目标视频主体之间的动作传输
最后，研究人员添加了两步操作**提升视频质量**：为了让合成的视频具有时间平滑性，他们在视频每一帧前都添加了时间步预测。为了让生成视频中人物面部看起更自然，研究人员还训练了一个专门的脸部GAN模型。

现在，你想要赋予舞蹈能力的那个人，已经在翩然起舞了。

## **原理是这样的**
![](https://pic2.zhimg.com/v2-b585050d97334c55c8c54dbcbf8629ad_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='393' height='450'></svg>)△教练我想学跳舞
要把舞者 (**A**) 的舞蹈动作，转移到“习舞之人” (**B**) 的身上，AI必须掌握其中的对应关系。从A到B，逐帧翻译，**表面看去**是这样的。

但事实上，团队并没有用 (来源 + 目标) 成对的图片，来训练神经网络。

因为就算对两个目标执行相同的程序，出来的效果也不会一样，毕竟身材等因素是人人不同的。




**个体差异如何解决？**

团队发现，基于关键点 (keypoint-based) 的姿势，就是**火柴人**，可以做**A、B**之间的桥梁。
![](https://pic3.zhimg.com/v2-a2efb69a40e31b8026e1782246336e92_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='175' height='171'></svg>)
给B视频 (目标人物视频) 的每一帧，生成一只**火柴人**，和那一帧原本的图像，对应起来。

整段视频处理下来，AI就会获得一对对的数据，以便学习火柴人和图像之间的对应关系。这样一来，针对每一个目标，AI都能学到一套不同的规则，不会受到个体差异的太多**限制**。
![](https://pic4.zhimg.com/v2-dd561b6d15091b5b01ad94e6bd2df153_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
然后，把A视频 (源视频) 的每一帧，也做成火柴人，让AI按照学到的规则为B生成姿势，就好了。不需要动作捕捉设备，不需要3D数据，这个方法真的便宜，只要好好学习。




**具体怎样训练？**
![](https://pic3.zhimg.com/v2-891b9d8ace7607617a94c1f3fb7c64e6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='390'></svg>)
就像上文提到的，用了一个姿势检测器 (P) ，给目标视频的每一帧做个火柴人。然后，AI就来学习火柴人和视频截图之间的对应关系。

这里用的是**对抗学习**：生成模型生成的是火柴人与视频截图之间的**映射** (Mapping) ；判别模型会**分辨**哪些**对应**是真的，哪些是假的。
![](https://pic2.zhimg.com/v2-46baae618adc3f43c11a2f3b984afc49_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='785'></svg>)
生成模型的配对能力，会在与判别模型抗争的过程中走向完美。




**训练后的实战**

训练完成之后，就可以用以下步骤来帮助目标人物学舞了：
![](https://pic3.zhimg.com/v2-79b63dedb002dc8cd2bd5004436db076_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='181'></svg>)
也是上文提到的，给源视频的每一帧，做个火柴人。让学成出师的AI，用火柴人帮目标视频的主人公，解锁新姿势。

中间，火柴人的位置会调整一下，适应目标视频的主角，在取景框里所处的位置。




**脸也是GAN做的**

跳舞除了动作，也有**表情**。有只GAN专门负责这一部分，结构如下。在生成整个场景之后，
腋以上的形态，是单独处理的，同样用到了火柴人。
![](https://pic2.zhimg.com/v2-e761200bdfad6c0c3309dfd1d28796ed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='625'></svg>)△Face GAN
## **学习成果集锦**

上文旱地游泳的男子，综艺效果出众，这一章节不会放过他。先看静态：
![](https://pic4.zhimg.com/v2-def02184812a72f88f7bc76c1f6b1e53_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='530'></svg>)
而原来的舞者是这样的：
![](https://pic2.zhimg.com/v2-ea7a50cfa0ec0c1ab8ebf324cbae7139_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='257'></svg>)
**全身上下**对比之后，没有看出动作上的明显差异。

为何游泳部就不像在跳舞？一定是穿的衣服不对。

再看看动态，似乎正常了许多，**舞步轻盈**。虽然，还是中间的妹子比较适合跳舞。
![](https://pic1.zhimg.com/v2-0785315a0de5e8519a82f0d1a2462dc8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
几分钟就解锁了高端舞姿，游泳部有点想上天 (下图所示) 。也可能是，老师并没有教他上天，他的领会有所偏差：
![](https://pic4.zhimg.com/v2-e132304689c7295bc8d89629ec106e7b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='631'></svg>)
说不定，老师们也觉得一同习舞的妹子更优秀。

于是，单独传授了一些舞步给她，游泳部并不知情：
![](https://pic3.zhimg.com/v2-c567807529f76f732827b8ff0fe8b7b2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
## **BAIR出品**

目前，这项研究的论文已经公布，这篇题为**Everybody Dance Now**的论文由加州大学伯克利分校的Caroline Chan、Shiry Ginosar、Tinghui Zhou和Alexei A. Efros四人共同完成。

Alexei A. Efros是一个有维基百科词条的研究人员~因为观点和言论经常被《连线》、BBC News、《纽约时报》和《纽约客》等知名外媒引用，因此也成了一名网红学者。

目前，Efros担任加州大学伯克利分校人工智能实验室（BAIR）的副教授，在加入此校前，他曾在牛津大学和卡内基梅隆大学（CMU）任职过，2016年还曾获ACM计算机奖。
![](https://pic2.zhimg.com/v2-45cab60fc040e4a02582cfd576f9a511_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='607'></svg>) △ Alexei A. Efros
此前，量子位报道过的文章[《伯克利AI实验室新论文：没有成对训练数据也能做图像风格转换》](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247484928%26idx%3D2%26sn%3D4442def06e466086dd7002c70152bb63%26chksm%3De8d3b172dfa438647fa14ad3527167ebe9848c9c948c49d41e6f7372dd9ca8438971cecdcab0%26scene%3D21%23wechat_redirect)，这个项目也出自Efros团队之手。

## **多方评论**

“这项研究看起来太酷了！”Reddit网友Avoc_Ado感叹。

“可怕，你完全可以在一部恐怖电影中使用它，真是不可思议的工作：）。”有网友对这项技术的应用感到惊奇。
![](https://pic3.zhimg.com/v2-a36e576936ba17be5a77ce7c225118d6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1048' height='214'></svg>)
“我想看唐纳德·特朗普像Mille＆Vanillie一样跳舞。”这位ogs_kfp_t网友，你又皮了~

总体来说，这还真是一项好评如潮的畅销研究。

## **传送门**

最后，一起欣赏下论文实现视频：
![](https://pic4.zhimg.com/80/v2-333b5fafa949dbab912e0434616f4f1f_b.jpg)https://www.zhihu.com/video/1016295521559998464



若想一览研究全貌，可移步论文：

[https://arxiv.org/abs/1808.07371](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1808.07371)

祝各位复现成功，勇夺舞王。

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


