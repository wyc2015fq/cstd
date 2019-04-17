# 想怎么GAN就怎么GAN，一键拯救发际线 - 知乎
# 



> 郭一璞 劈图栗 乾明 发自 亚龙湾
量子位 报道 | 公众号 QbitAI



![](https://pic3.zhimg.com/v2-7d1d863ca095abc0956cdd529c479c76_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='362'></svg>)



GAN可GAN，非常GAN。

生成对抗网络（GAN）除了生成各种各样的人脸，现在还能对人脸进行各种各样的精确“整容”。

比如，把头顶光亮的大叔P出青葱的发际线；把阳刚的肌肉男P成小鲜肉，再P成可爱的女孩子；让金发女郎变成黑长直。

不久后，你还能用GAN一键调整自己的发际线，从此省下植发钱，免费让你的头发像Jackie Chan一样乌黑油亮，duang duang duang~

堪称 **整·容·神·器**。

## **秃秃秃魔法**

首先，我们先来学习一下脱发的“原理”

你知道如何一键脱发么？

请使用咒语：秃，秃，秃。
![](https://pic4.zhimg.com/v2-ea70b4951cd78869f13d86856c1e4a03_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='370' height='138'></svg>)△ 一只充满恶意的按钮
奋力点击**Bald+**按钮：
![](https://pic4.zhimg.com/v2-2a0d3335faa2ebd366487d62b05e2203_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='424' height='424'></svg>)
俊朗的面容，顿时变得矍铄而慈祥。

不只发际线被强行逼退，原本的褐色头发也成了花白。除此之外，法令纹与其他皱纹都清晰了起来。

当然，想要植发的话，只要反过来用**Bald-**就可以了。

顺便告诉你一个秘密，这个按钮“歧视”男性：男生照片随便点几下就秃了，女生点几下就可能变成男生，然后才会秃。
![](https://pic2.zhimg.com/v2-ea35689d424c5c4711003d15da730515_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='354'></svg>)
或许是因为数据集源于自然。

那么，再来试试其他按钮。

**性** (Niang) **转** (Hua) ，大概是非常受欢迎的功能。
![](https://pic3.zhimg.com/v2-e0194ac0d5e683814e6b4c026d01250e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='500'></svg>)△ 错误示范：史蒂夫·乔布子，比本体的头发茂盛一些
不想看男生的时候，就点击**Male-**把他变成妹子。

真是造福人类的功能。
![](https://pic1.zhimg.com/v2-aa722c8f8e068b36c8207d88d585a908_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='338'></svg>)
友情提示，除了**Male**按钮之外，**Pointy Nose** (尖鼻) 也可以把男生变成女生。

莫非，鼻子尖了会变娘？一定是数据集的错。
![](https://pic2.zhimg.com/v2-47243bf25ef3a212f9d1f93dc772c281_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='356'></svg>)△ 尖鼻技能
就算只是想刮刮胡子，连击**Beard-**，也能顺势变成妹子。

一定是数据集的错x2。
![](https://pic4.zhimg.com/v2-cf45d9b3b8d73957fb4cc389be5d388b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='355'></svg>)△ 来来来，一起刮胡子
全部21个按钮里面，能做变性的还有许多。有兴趣的小伙伴自己去试一试嘛。

娘化虽好，可不要贪杯哦。毕竟，这也不是唯一的功能。

“**大鼻子**” (Big Nose) 可能是怨念的象征。毕竟，连续点击加号之后，鼻子似乎没有变大，五官倒是挤在一起，好像吃了芥末。
![](https://pic2.zhimg.com/v2-86de20492a196f693ad5fc351612b45d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='356'></svg>)
其他按钮，大家有空就自行探索吧，传送门在文底。

目前，Demo还不支持上传人脸，只能用它提供的随机人脸。

不过，作者说下一步就会增加上传功能了，有大胆想法的同学，可以预备备了。

## **前途似锦的GAN**

这个神奇的GAN一放出来，一众网友就鸡冻了。
 (๑>◡<๑) 

> 楼主好人，求楼主做个.exe格式的应用程序出来
看来下一个被AI搞失业的职业是模特了

不少人纷纷开始畅想它商业化之后的用处：

> 可以用来定制女朋友，靠这个软件生成好看的脸，投射到机器人或者VR上……
希望可以把我的脸传上去，修改成最喜欢的效果，然后拿去给整容医生照着这个整
警察叔叔找犯罪嫌疑人的时候这个应该会很有用
如果以后可以根据目标受众喜好生成各种“千人千面”的帅哥美女，用来打广告……真可怕

## **背后的原理**

有这种“神奇魔力”的GAN，名字叫做TL-GAN，TL代表的是Transparent Latent-space，意思是将潜在空间（Latent-space）透明化（Transparent）。

总体来说，这个项目提供了一种新的方法，可以来控制像GAN (生成对抗性网络)这样的无监督生成模型的生成过程。

首先，使用一个已经训练好的GAN生成器，在这个模型中，使用的是英伟达的pg-GAN。然后，挖掘其中有意义的特征轴，将图像变化的过程变得透明化，从而使我们能够控制图像合成和编辑的过程。

整个模型的构建，主要分成三个步骤：

## **第一，将图像背后的潜在空间透明化。**

作者在模型上使用的是英伟达的的pg-GAN(progressively-growing GAN)，其生成的1024px × 1024px图像的特征由潜在空间中的512维噪声矢量(一种图像内容的低维表示)来确定。如果能够理解这些都代表着什么，就能完全控制生成过程。
![](https://pic1.zhimg.com/v2-4172d984bbf2d4d8bf66824b1adc602c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='375'></svg>)
为什么要用pg-GAN呢？作者说，它有两个优点：


一是，它生成的图像很多，这意味着空间中的大部分点会生成合理的图像。二是，它是强连续的，这意味着潜在空间中两点之间的插值一般会让相应图像的平滑过渡。

这让他可以在潜在空间中寻找方向，如果能够找到，就可以使用这些方向的单位矢量作为特征轴来控制图像生成过程。

## **第二，寻找特征轴。**

为了找到这些特征轴，作者通过基于配对数据(z，y)的监督学习方法进行训练，在潜在向量 z 和特征标签 y 之间建立了一个连接。

于是问题来了：如何获得这样的配对数据？现有的数据集只包含图像 x 及其相应的特征标签 y。
![](https://pic4.zhimg.com/v2-ec9f13ae89f43733d4d3504bce5851e3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='540'></svg>)
作者表示，为解决这个问题，TL-GAN模型的关键创新是，使用现有的标记图像数据集( x_real，y_real )，来训练一个独立的特征提取器模型y=F(x)，然后将训练好的GAN生成器G与特征提取器网络F耦合。


在这个过程中，作者在CelebA 数据集上训练了一个简单的卷积神经网络(其中包含了3万多张面部照片，每张有40个标签)。

这样，就可以使用训练好的特征提取器网络来预测合成图像x_gen的特征标记y_pred，从而通过合成图像建立z和y之间的联系，如x_gen=G(z)和 y_pred=F(x_gen)。

## **第三，训练模型。**

既然有了成对的潜在向量和特征，就可以训练回归模型y=A(z)来揭示特征轴控制图像生成的过程了。
![](https://pic4.zhimg.com/v2-6e2da2ab22c5a03cfa0d3a2984c4604b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='438'></svg>)△TL-GAN 模型的架构
在进行测试的过程中，虽然效果很好，但出现了一个非常让人头疼的问题：**特征轴纠缠（entangled feature axes）。**即不同的特征轴之间会相互影响，例如当你打算减少胡须特征的时候，图像就会变得越来越像女性，或者调整**Pointy Nose** (尖鼻)， 也会把男生变成女生。

为了解决这个问题，作者表示自己使用了线性代数技巧：将胡须轴映射到一个垂直于性别轴的新方向，这可以有效消除它们的相关性。
![](https://pic2.zhimg.com/v2-ecb61278ff893092b87e1aacba94a1b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='660' height='390'></svg>)△使用线性代数技巧分离相关特征轴
不过，如上所述，量子位在测试的时候发现，还是会出现类似的问题。Hacker News上也有网友在吐槽这一点：

> 啊！为什么改变肤色的时候，嘴唇会变薄、头发会变秃，人会变成男的啊，真是AI魔术

有网友猜测是训练模型的数据集太小的原因：

> 因为是用真实的明星照片训练的吧，毕竟明星普遍都比较好看。可能样本里只有白人，没有其他肤色的人，所以生成的效果也就跟着训练用的数据集跑了~

以上，就是模型大致的原理了。

作者认为，这个模型有两个突出的优点：

1、效率高：想要为生成器添加新的功能调谐器的时候，不需要重新训练GAN模型，使用这个方法，只需要花1个小时，就能够添加40个功能调谐器。

2、灵活性：你可以使用任何数据集上训练的任何特征提取器，来向训练好的GAN中添加更多的特征调谐器。

## **作者介绍**
![](https://pic2.zhimg.com/v2-2b4278a180d7d340927204826f343d35_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='331' height='331'></svg>)
这个TL-GAN的作者关少波（Shaobo Guan），是一位集生物学和计算机两大专业技能于一体的博士小哥哥，本科毕业于上海交通大学生物技术专业，之后在中科院做了三年研究助理，然后去布朗大学读了系统/计算神经学的博士。

厉害的是，读博第三年开始，他还顺便在布朗大学读了一个计算机硕士，从此踏入AI圈。

今年夏天硕士和博士都毕业后，关少波开始在YC和SV Angel投资的Insight Data Science项目担任AI Fellow，TL-GAN就是他在该项目的第一个作品。

小哥哥还很自豪的在自己的LinkedIn上写：

> 我的TL-GAN，全Hacker News热度第二呦~

## **传送门**

如果你对这个模型有兴趣，下面有一些传送门，能够送你到你想去的地方~

GitHub地址：

[https://github.com/SummitKwan/transparent_latent_gan](https://link.zhihu.com/?target=https%3A//github.com/SummitKwan/transparent_latent_gan)

Demo地址：

[https://www.kaggle.com/summitkwan/tl-gan-demo](https://link.zhihu.com/?target=https%3A//www.kaggle.com/summitkwan/tl-gan-demo)

（友情提示：玩Demo的时候，要有耐心哦~）

模型博客文章地址：

[https://blog.insightdatascience.com/generating-custom-photo-realistic-faces-using-ai-d170b1b59255](https://link.zhihu.com/?target=https%3A//blog.insightdatascience.com/generating-custom-photo-realistic-faces-using-ai-d170b1b59255)

模型的PPT地址：
[https://docs.google.com/presentation/d/1OpcYLBVpUF1L-wwPHu_CyKjXqXD0oRwBoGP2peSCrSA/edit#slide=id.p1](https://link.zhihu.com/?target=https%3A//docs.google.com/presentation/d/1OpcYLBVpUF1L-wwPHu_CyKjXqXD0oRwBoGP2peSCrSA/edit%23slide%3Did.p1)




— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


