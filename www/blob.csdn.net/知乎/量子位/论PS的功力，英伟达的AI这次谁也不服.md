# 论PS的功力，英伟达的AI这次谁也不服 - 知乎
# 



> 问耕 发自 凹非寺
量子位 出品 | 公众号 QbitAI




在修图这件事上，Photoshop有着崇高的地位。

前一阵子，流传过这样一个段子：“甲方不要PS！让我们用Photoshop做！”足以说明开头的结论。

然而，Photoshop不一定是最好的选择~

这次英伟达，就搞出了一个新AI，拥有技惊四座的P图本领。

比方，给你这样一张图。
![](https://pic1.zhimg.com/v2-9557455cb273aebcdda33eb97c668d78_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='996' height='994'></svg>)
嗯，对，就是这样，画面严重缺失。请用AI把这张图修复一下，P得要看起来像原图一样真实。

难不难？这画面上是啥都不一定能看清。

普通的AI也就能自动修补成下面这样或那样。
![](https://pic3.zhimg.com/v2-bc05a1ada478eab0c0e802f69361a0de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='531'></svg>)
英伟达的AI呢？可以P成这样：
![](https://pic2.zhimg.com/v2-c16ec61e193ec7269ed6d1a14e571429_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='994' height='992'></svg>)
对比下原图。
![](https://pic2.zhimg.com/v2-f47b5599f424998b050240deaa4768b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='992' height='992'></svg>)
再来一个，还是同样画面确缺失严重。
![](https://pic1.zhimg.com/v2-96dfdd1db3240e79b28e275f2249b184_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='994' height='994'></svg>)
普通的AI也就能自动修补成下面这样或那样。
![](https://pic1.zhimg.com/v2-e00309c45b557ea296681cc15c6eb2d0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='529'></svg>)
英伟达的AI，可以P成这样：
![](https://pic3.zhimg.com/v2-8a16c32c6fb17c80f61c93318c8e0742_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='994' height='996'></svg>)
再对比下原图。
![](https://pic3.zhimg.com/v2-8d0d45eddda06d7f732dea7c3a6c6906_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='994' height='998'></svg>)
英伟达的AI是不是更赞？

但，这个技术有什么用？

其实与大段大段的补全缺失画面相比，快速且优秀的进行局部P图，才是这个技术真正发挥实力的地方。

静态展示不过瘾，动态展示才更令人惊叹。

比如下面这个动图，把左图中的人、线、红旗、石头、棍子都P掉，应该怎么做？很简单，就是直接涂抹掉，然后AI就自动修复了。
![](https://pic2.zhimg.com/v2-59504569ba8c26caa6f8ea2919cc6d5d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)![](https://pic2.zhimg.com/v2-f47c181f3791e5b3bea22d74add2b111_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
还比如这样。
![](https://pic2.zhimg.com/v2-beaeef023ad95f79a80fb9efb2d8a175_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
其实这个AI的本事，就是能在一片空白之中，更好的填补上缺失的内容。

极端情况，比方处理人脸时候，眼睛被遮蔽了。

英伟达的AI就能重新补上一双眼睛，当然不可能是原来那个人的眼睛，但是至少能弥补的也算相对完美吧~
![](https://pic1.zhimg.com/v2-95577ef5b7520db2a78aa21c72bbdcdc_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)![](https://pic2.zhimg.com/v2-ec98e04461c2c393f28605a678268e25_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
以及，对于也能让头发更浓密。
![](https://pic2.zhimg.com/v2-6c469ca63eff3e3e901a2a42a84c13f1_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
也能让发际线更高~
![](https://pic4.zhimg.com/v2-8d3b5e5ee7a86d16323538622580b76b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
英伟达的新AI是真么做到的？

答案就在这家公司新发布的论文里。这篇论文标题：*Image Inpainting for Irregular Holes Using Partial Convolutions* 。
![](https://pic4.zhimg.com/v2-53464fceb2c5fee5561fac3512fb3ec7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='362'></svg>)
英伟达的研究团队提出了一种新的模型，使用部分卷积的方法，其中卷积被掩蔽，并且仅基于有效像素进行重新归一化等处理。

论文中，还与现有其他方法进行了对比，有很多公式。

比如这种。
![](https://pic3.zhimg.com/v2-de6ef706350c114c60a2ee7695128e2a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='414'></svg>)
还有这种。
![](https://pic3.zhimg.com/v2-e7f444ecec857ff4699c2e08daf2ae66_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='265'></svg>)
详细的方法和原理，可以前往阅读论文，地址：

[https://arxiv.org/abs/1804.07723](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.07723)

总而言之就是一句话：我不是针对谁，在座的都是……

最后顺便提一下，这篇论文的一作Guilin Liu，在加入英伟达之前，还曾在Adobe Research实习过。

Adobe，就是搞出Photoshop（和一堆其他软件）的那家公司~
![](https://pic4.zhimg.com/v2-4d47978cc29beb78b15202f611ed6373_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='600'></svg>)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


