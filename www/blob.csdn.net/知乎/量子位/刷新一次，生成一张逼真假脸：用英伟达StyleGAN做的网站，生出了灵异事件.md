# 刷新一次，生成一张逼真假脸：用英伟达StyleGAN做的网站，生出了灵异事件 - 知乎
# 



> 栗子 岳排槐 假装发自 凹非寺
量子位 出品 | 公众号 QbitAI

一个新网站在国外火了。

玩法很简单，每次刷新这个网站的页面，都能出现一张人脸。大多数情况下，都是一张眉目清晰、面含微笑的和善人脸。

比如这样：



![](https://pic1.zhimg.com/v2-ec8974c2f3a79e034f18145f0981bef4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)



不过，这个网站却被一众国外媒体，集体以“可怕”两个字来形容。


为什么？

来，大家先仔细看看这些笑脸。



![](https://pic3.zhimg.com/v2-7df08ea09e70935e869ed5b4e2ad1a06_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)





![](https://pic4.zhimg.com/v2-6cc99d8641796886dc418149a4410783_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)





![](https://pic1.zhimg.com/v2-27585959f7ddda181ca4b18c8e6bfa7c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)



不知你能否看出可怕之处？

其实，这些人脸都是假的。

每次刷新这个网站，出现的那张高清笑脸，尽管看起来无比真实，但都是从来不曾在世界上出现过的。这些笑脸不是真人的笑脸。

都是AI生成的。

## **光荣归属英伟达**

之所以能生成人脸，是因为背后有一个针对人脸进行预训练的模型。每次你刷新网站时，一个神经网络就会基于512维向量，从零生成一个全新的人脸。

而这个网站的创建者，名叫Philip Wang。社交网站的资料显示，他是一位Uber的软件工程师。住在旧金山。

他在Facebook上解释了为什么要建这样一个网站。

> 最近一群天才的英伟达研究员，发布了最先进的生成对抗网络StyleGAN。我决定自己动手搞个事情，提高大众对这个技术的认知。

显然，Philip Wang的目的达到了。

他搭建的这个网站，用一种超级简单的方式，展示了StyleGAN能生成多逼真的假人脸。再给几个示例，大家一起看看。



![](https://pic2.zhimg.com/v2-85a5f5013f254165295dc401d88cc779_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)





![](https://pic1.zhimg.com/v2-3feae2c23ba26fd5bce0ab98781238f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)





![](https://pic4.zhimg.com/v2-a40b998515c190ac1b35eba58a4f770f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)



什么是StyleGAN？

这是英伟达2018年底发布的最新研究成果。十天前，英伟达正式给这个模型命名为StyleGAN。顾名思义，GAN的生成器，是借用风格迁移的思路重新发明的。

而且英伟达官方还开源了代码。

这个实现可以用Linux跑，也可以用Windows跑，但墙裂推荐大家用Linux跑，为性能和兼容性着想。此外还需要Python 3.6和TensorFlow 1.10以上 (支持GPU) 。



![](https://pic4.zhimg.com/v2-c63bc8a2440fda1568e6355f74be39db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='956' height='385'></svg>)



## **△** StyleGAN架构

和官方代码实现一起发布的，还有Flickr高清人脸数据集。那些几可乱真的人脸，就是StyleGAN吃了这个数据集，才生成的。

数据集里包含7万张1024×1024高清人像。英伟达说，这些照片在年龄、种族、以及图片背景上，都有很强的多样性。



![](https://pic1.zhimg.com/v2-840b2708511342eaecddb6ecd1df2758_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)



当然，StyleGAN不止能生成人脸，英伟达还提供了猫、汽车、卧室的预训练模型。

如果你也想自己动手试一下，下面是传送门。

代码实现：
[https://github.com/NVlabs/stylegan](https://link.zhihu.com/?target=https%3A//github.com/NVlabs/stylegan)

FFHQ数据集：
[https://github.com/NVlabs/ffhq-dataset](https://link.zhihu.com/?target=https%3A//github.com/NVlabs/ffhq-dataset)

论文：
[https://arxiv.org/abs/1812.04948](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1812.04948)

## **“可怕”**

不过，媒体纷纷用“可怕”“毛骨悚然”“最毛骨悚然”，来描述网站的画风。



![](https://pic2.zhimg.com/v2-3c40829a650f96ab57be6975d78f8601_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='0' height='0'></svg>)





![](https://pic2.zhimg.com/v2-92675977b57d8bfc7b0062502e76b23d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='241'></svg>)



一方面，是不敢相信这些细节丰富、表情逼真的人脸，竟然全部都是假的。另一方面，这些生成的假脸，并不是每一次都看起来那么完美。

每次有明显的问题出现时，确实让人感觉有点可怕。



![](https://pic3.zhimg.com/v2-263757b915384db3f3f433b0cdaee3ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='254'></svg>)



网站上线之后，许多小伙伴也都欣赏到了奇怪的图景：

少只耳朵，算是很常见的缺陷了。



![](https://pic2.zhimg.com/v2-98ce1f134d88f5b38ae9f0ab6e5a8b1d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)



妹子微笑着，可能没发现旁边有只幽灵。




![](https://pic4.zhimg.com/v2-8d69ba696891b8cccc1a514affc671e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1079'></svg>)



少年仿佛被不明力量攻击了。




![](https://pic2.zhimg.com/v2-8093ae1ea7268af6429039f9464b07d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)



当然，看起来奇奇怪怪，还不是最可怕的地方。


这一张张并不存在的假脸，甚至让人浮想联翩。

## **神展开**

有个叫Darwindemon (达尔文恶魔)的 reddit网友讲了一个更惊悚的故事，说是发生在自己身上。故事是这样的：


> 2月14日那天，她闲来无事，就在房间里不停地刷这个网站。看着一张一张逼真的脸，也不知道是不是真的不存在，想着说不定有人刚好长这样，谁说得清呢。
刷着刷着，突然发现屏幕里的脸和自己一模一样，脸颊上的痣都一样。她吓呆了，不停地说服自己，这一定是巧合。
然后，她跑去客厅沙发，靠在男票肩上找安慰。却发现男票还是玩手机，对她的出现毫无反应。这时有人敲门，男票去开门，一个妹子走进来，男票还亲了她。完全当她不存在。
不管怎样试图引起男票的注意，都是失败。不可能的，男票怎么会在情人节开这种玩笑？
再后来，她发现想不起自己是谁了。
手机里已经没有一条消息了，相册空了，邮箱空了，Facebook也没有登录痕迹了。
她真的不存在了。
找来找去，只有Reddit登录着，但也没有身份信息。于是，达尔文就在那里发帖，希望观众们帮她找回自己。

这则求助帖的人气急速上涨，18小时已有4.4k热度。

关于这个不存在的人，大家都有很多话想说。



![](https://pic1.zhimg.com/v2-c671611d1ad637563976beaf5f45ec04_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='860' height='514'></svg>)



> 有人问：你有没有再刷新一下？
Darwin_demon：当下就刷了，生成了不认识的脸。
有人回：**那你又把一个人给抹杀了**。

这也是她唯一的回复。

不过，楼下观众的讨论没有停，大胆的想法一浪一浪。



![](https://pic3.zhimg.com/v2-7949395b1492544544d050e8fd0eabf2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='988' height='490'></svg>)



> 有人说：反正你也不存在了，把找到的个人信息都发到Reddit上，说不定系统就挂掉了呢。
有人附和：是啊是啊，社会安全码 (SSN) ，信用卡信息之类的都来点吧。
有人说：反正你也不存在了，去抢银行吧，然后就有钱了。
有人反对：没法跟别人互动的话，钱就没用了，还不如去抢商店，抢来的东西还能用。

有人说：

> 我刚才看到我表弟了。
二次编辑：我怎么不记得我有个表弟了，他叫什么来着？

有人回：

> 你表弟去世五年了。

然后，表弟真的来了：

> 怪不得我有座坟。



![](https://pic1.zhimg.com/v2-f4b1445fd0e7c19dc7821525741a4188_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1006' height='606'></svg>)



## **One More Thing**

为什么会有这种故事？

因为这个网站的名称叫：

[https://thispersondoesnotexist.com/](https://link.zhihu.com/?target=https%3A//thispersondoesnotexist.com/)

（意思是：这个人不存在.com）



![](https://pic2.zhimg.com/v2-bd0aaef5f1f75795aaf148b98cc540f1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1395'></svg>)



亲测手机也能打开刷新。

谁要不小心刷到了自己，可以留言求助……

—**完**—

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


