# 当，程序员突然想画画，AI+机器人就该登场了 - 知乎
# 



安妮 栗子 发自 凹非寺

量子位 出品 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-2bc0225142a431e644506c7f77ae5c0c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)
人生总有那么一些时候，你突然间想文艺一下。

比方画一幅画。

但，理想和现实之间，可能远隔千山。比方，你其实没有创作的灵感。以及，你其实没有把灵感实现出来的基本技能。

每当这时，有人会妥协退缩，也有人会迎难而上。俗话说：“只要思想不滑坡，办法总比困难多”。

今天，就是这样一位不妥协的程序猿的故事。

澳大利亚人Jeremy Kraybill活了40多岁，突然对绘画产生了兴趣。横亘在他面前的就是那两个问题：既没有艺术灵感，也没有绘画基础。

但，他有技术。

先看结果。下面这幅画，就是Jeremy的绘画作品。为了完成这幅画，他动用了人工智能（神经网络）来产生创意，然后又动用了机械臂，把创意画了出来。

![](https://pic1.zhimg.com/v2-9189ec8f3a286a6ad949a4709ee6b40c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='714' height='709'></svg>)
这幅成品，看起来是不是有模有样，还颇有大师神韵？实力演绎“不想当画家的创始人不是好的软件工程师”年度大戏。

然而，这也并不是一番坦途。

这篇文章就带你真实还原**AI设计、机械臂全自动作画**完整过程。对了，量子位还“采访”了JK的“御用”机器人，问了问它这几个月的修炼感受。 ﻿
![](https://pic1.zhimg.com/v2-467f70c080833a3572b760853eed4a24_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='199'></svg>)
**△** 大概就是这样的画风

## **从歪七扭八开始**

一切都得始于灵感。

在这个出发点上就不能输。不过，怎么才能拥有大师级的创作灵感？

作为资深程序员，Jeremy立刻就想到，可以靠AI啊！

没错，现在的AI其实已经有了某种程度的“想象力”和“创造力”了。AI的灵感来源，其实有点像那句话：模仿是艺术家向艺术家最高级别的致敬。

为了生成画作，Jeremy选用了DCGAN（深度卷积生成对抗网络），直接用了tensorlayer在GitHub上开放的TensorFlow代码。

相关论文： Unsupervised Representation Learning with Deep Convolutional Generative Adversarial Networks Alec Radford, Luke Metz, Soumith Chintala [https://arxiv.org/abs/1511.06434](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1511.06434)

代码：[https://github.com/tensorlayer/dcgan](https://link.zhihu.com/?target=https%3A//github.com/tensorlayer/dcgan)

而所用的训练集，由男男女女的肖像画组成，全都是他喜欢的风格。挑选、修改、增强等等一套操作下来，他Jeremy得到了一个包含10万幅画的训练集。

接下来，就是训练和生成了。即使已经有了开源代码，训练一个GAN也并不容易。一不小心，就会遇到这样惨不忍睹的崩溃NG场面：
![](https://pic1.zhimg.com/v2-cab67123e0634bf1e0ff05e97a934614_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)
各种NG都解决了之后，就进入了最终的训练和生成过程，在亚马逊的AWS p2.xlarge实例上进行，整整花掉了7天时间。

Jeremy得到了——
![](https://pic1.zhimg.com/v2-645d7f0c927a5500189282e157344844_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='64'></svg>)



就是这个，一个64×64像素的、歪七扭八的、全新的创意。From AI with Love.。GAN还为这幅作品起了个充满诗意的名字：028749_0001_08。

上面这张小头像和成品之间的差距，可能能绕地球好几圈。但这已经是Jeremy可以得到的最好结果了。他还有过不少其他改善的尝试，不断碰壁后，最终他决定：

就用这个了！

这里插播一下，为啥他没有继续尝试，而是急急忙忙接受目前的结果？可能是因为Jeremy初心变了，他不只想画一幅画，而且还想参加机器人艺术大赛RobotArt。

于是，时间一下子就变得很紧张。

## **转攻机械臂**

接下来的问题是，怎么把这个AI贡献的灵感，变成一幅真正的画作？

靠机械臂！

那也得教一下机械臂，怎么画画。

第一次尝试机器人手臂大概还是在去年7月份，Jeremy试图熟悉机械臂的功能，并且想让它看起来像人类的笔触，他先就购买了一套温莎牛顿的豪华刷头马克笔。

这套马克笔的笔尖像毛笔，非常适合测试，又不用担心调色的事。
![](https://pic1.zhimg.com/v2-2c3fe525ea0a02399ae563ae5397f74c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='697' height='473'></svg>)
**△** 2017年9月早期测试

虽然，没试出什么惊艳的结果，但Jeremy浑身上下写满了 (蜜汁) 自信，感觉用真正的水粉画笔也没问题。
![](https://pic3.zhimg.com/v2-9c8d71f1ec24e86174c7455a6165a902_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='772'></svg>)
后来，他工作一忙，就开始捉急了。看看越来越近的截止日期，再看看还没玩过水粉笔的自己，Jeremy决定用Aqua Brush (一种内置颜料盒的笔刷)，来代替了水粉笔这种需要配置的操作。选了8种预先混合好的颜色，实验又开始了。



![](https://pic3.zhimg.com/v2-2ad261957021459eca12c4ae8682f862_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='444' height='317'></svg>)
**△** Aqua Brush笔刷

“您的扑街只用了一秒。”走出实验室之前，他丧丧地扔掉了所有的Aqua Brush。这种工具需要的水量太大，效果很水。

技术宅觉得，如果用另外一种介质来代替水，可能会更好。不过，他还是毅然决定死磕水粉了，并坚信古老的笔刷和颜料会比现代工具更适合画画。调色板里颜色是根据要画的作品和最少的混色步骤来选定的。

要指导机器画画，就需要给它一套指令，告诉它该在画布的什么位置、用什么样的笔触、刷上什么颜色。于是，Jeremy基于机器人要用的色板，将这幅图像用算法翻译成一套ABB机器人指令，开头那张“AV画质”的小人脸，摇身一变成了现在的样子——
![](https://pic1.zhimg.com/v2-961d8c754c7c00ce11fcffd865778e3c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='1000'></svg>)
﻿
在这个项目中，“从图像到笔刷”的转换程序是唯一需要“从零开始”的，其他所有关于AI的步骤都主要基于过去几年研究的已有文件和代码。

在转换之前，需要先处理一下DCGAN生成的渣画质小图：放大，将颜色减少到能和色板相匹配、并对图像进行平滑处理来增强所生成笔刷的平滑度……
![](https://pic4.zhimg.com/v2-31f60b6e34ae1b2874ec0addf3cd5d33_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1059' height='277'></svg>)![](https://pic3.zhimg.com/v2-1528a8916fc7cd56a6134d0519fd15da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='560'></svg>)
处理好了图像，还要准备备选的笔刷。这些大大小小的笔刷，是Jeremy在Photoshop里削出来的，构成了算法的训练数据集。这些笔刷再加上混色指令，就是算法的全部输入了。

距离截止日期12天时，Jeremy计划的调色方案还很细腻：从奶油色开始，一步一步加深到深棕色，然后再用调色板来调出余下的颜色。但对于机器人来说，时间显然不够用
![](https://pic1.zhimg.com/v2-1d0d8a544f340076b3d25245e408b9d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='279' height='400'></svg>)
机器人按指令操作的结果，和他美丽的想象相去甚远。以至于最后关头，他还在紧急修改调色指令。
![](https://pic4.zhimg.com/v2-6f3c83dfd277a56717f6ed788f539eff_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
Jeremy还向想让机器人在画布上做一些混色，写出了代码，但在有限的时间里，并没有实践成功。


万事俱备，就该进行“图像到笔刷”的生成了：算法吃下准备好的图像，然后进行一个反复的试错，先考虑进多种可能的笔触，再通过和图片对比来筛选最接近的。

算法输出的结果包括一组要用到的笔刷、和机械臂要执行的混色指令，第一次尝试的时候用了6000笔，最终的作品是4000笔。用这个训练好的“图像到笔刷”生成算法来渲染给机器人的指令，消耗了Jeremy八个小时的时间。
![](https://pic1.zhimg.com/v2-637725589d37ccf6a58c930c2f693e4c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='212'></svg>)
好在，最后的成功，没有让他失望。

Jeremy设计的画作，也最终入围了2018年度RobotArt设计大赛，他还想让大家空了去投个票~

[https://robotart.org/artworks/](https://link.zhihu.com/?target=https%3A//robotart.org/artworks/)

不过，故事还么完。

光提到Jeremy怎么够？

量子位还（假装）采访到了参与本次创作的ABB机器人，它的心路历程饱满得快要喷发。以下，我们用自述体，还原这次采访。
![](https://pic4.zhimg.com/v2-2b98df209fcba713a8e5d1a2b334831b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='210' height='157'></svg>)
## **机器人的艺术修养**

那是一个周四的下午，Jeremy到实验室来告诉我，之前的试画他很满意，晚上就会把一幅大作的任务交给我。

之前从来没有画过一张完整的画，每次都让我试几下就把笔收走。啊，想想还真有点小激动呢。

按这个节奏往下走，说不定哪天画廊就会来找我签约，这样我们实验室的小花会被我的才华折服，然后我顺势表白成功。 从此，两只机器人过上了幸福的生活。
![](https://pic3.zhimg.com/v2-926ea1e8b78e441de114f2067460d88e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
回到现实，没想到他让我一整晚不许停笔。我是机器人没错，不管饭可以忍，不能休息也叫小花来看看我。

大概男人辛苦的样子才更可爱吧。这样想着，4月5日的夜晚，我还是用色彩填满了。

第二天，Jeremy说我发挥正常，不过他觉得青色 (Cyan) 用得有些重了，焦赭石色 (burnt umber) 又有点太轻了。

这不是大问题，反正我的混色习惯都是可以调的。可是他还说我画得慢，只有一句话想送给毫无艺术天分的他
![](https://pic2.zhimg.com/v2-d24c94e85eaee3370707403e068791b5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='320'></svg>)
三小时画了1800笔，要不去问问别的机器人有没有这么快。一下子塞给我6000笔的任务，我也很绝望啊。

不过，好在他比我还紧张，这样我也能平衡一点。

可能他也觉得有点对不起我，就给了我三天假。4月9日周一晚上，我又开始工作了。
![](https://pic4.zhimg.com/v2-d035231de954727a2232a5d32acffdeb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='326' height='244'></svg>)
**△** 没见过吧

Jeremy好像是刚刚知道，我有一个动作更快的模式。一开始，我的内心是拒绝的，那个模式真的很累。但他求了我，还说要带我参加比赛，于是我决定给他看看我真正的速度。

那天晚上，我就把画完成了。还是三个小时，但这次画了4,000多笔。他果然被我精妙的技艺震撼了，除了速度，我还有更加流畅自然的笔触。
![](https://pic1.zhimg.com/v2-b5a62df67b84305d8952bcc40bb6e880_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='368' height='207'></svg>)
这个傲娇的技术宅，明明感受到了我的厉害，还要说我的画有些小小的瑕疵。

他把水粉冲得太淡了，导致颜料在画布上开心地流淌。还有，他又觉得把混色指示里的焦赭石色调得太重了。只要他承认那是他的错，就好。

## **生动的意外**

不知为什么，他好像不想让我再画了，要直接拿这幅画去参加比赛。可我才刚出道，不想只留下一幅作品就隐退。
![](https://pic2.zhimg.com/v2-5952734599f5c4fc1e9ba2d6ca68fd15_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='760' height='760'></svg>)
**△** 大概在剔牙

不过，没过多久Jeremy就跑来告诉我，他改主意了。Jeremy在调色桶和调色盘的转换之间，发现了一个bug，导致我的笔刷位置太低，才有了画布下方那条乱入的棕色长线。

于是，Jeremy第二天一大早又来了。他说这次调整了系统，一幅画只要4,000笔就好了，少了2,000笔。还算懂事：
![](https://pic1.zhimg.com/v2-a0d593f891f6b4665a99f26387a5ae9c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='230' height='187'></svg>)



另外，他还重新设计了颜料的混合方式，这样我就可以画出更有趣的色彩了，好兴奋啊。

早上工作，神清气爽。Jeremy让我先做笔触渲染，等他下班回来再开始画画。

晚上，准备工作两小时，画画三小时。虽然，过程因为调整混色指示而中断了几次，但作品我还是满意的，技术宅应该也满意吧。

![](https://pic2.zhimg.com/v2-70825a97bf19b93c022738a784102861_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='232' height='232'></svg>)
和预先准备好的图像相比，我画出的成果有些不同。但在有意无意中，色彩变得更生动了。

投稿截止日期将近，Jeremy说许多变化都是时间不够造成的代码意外，但我们两个都享受这些可爱的意外。如果真的和原图一样，怎么看得出我的天分呢？

## **后记**

机器人不知道的是，那天晚上Jeremy把它的作品小心翼翼地捧回了家，像个得到了珍贵礼物的孩子。
![](https://pic4.zhimg.com/v2-f3549e71a908b38202e299321e31adef_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='290'></svg>)
后来，他还用蜡给作品上了一层表面，让水粉多了一些光泽，多了一些深度。当然，那是比赛之后的事了。

至于，这是不是艺术。要说不是，那些孩子一样的心情又是什么呢？

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


