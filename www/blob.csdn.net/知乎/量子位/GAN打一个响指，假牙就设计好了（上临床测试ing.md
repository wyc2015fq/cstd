# GAN打一个响指，假牙就设计好了（上临床测试ing - 知乎
# 



> Root 发自 凹非寺
量子位 出品 | 公众号 QbitAI

不知道大家身边有没有门牙磕掉一半盆友。

（此处不应有配图.nojpg

说话漏风，说Fine，thank you时没法准确发θ音的痛苦，你可能没法想象。
![](https://pic1.zhimg.com/v2-7580468ed11e2fa6dc3d704b2af42014_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='234' height='278'></svg>)
通常来讲，牙磕坏了or蛀牙这两类问题，可以选择的治疗法案有烤瓷牙 or 种植牙。

巴特不管哪一种，现在价格都贵上天。

因为，每个人的牙齿排列和每颗牙的咬合程度都是不一样的。一颗好用的假牙必须经过专门设计，甚至还会动用到CAD来建（牙冠）模。
![](https://pic3.zhimg.com/v2-a1429c0b63097b2efdb12dd533fef7c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='630'></svg>)
那一颗好用的假牙要满足什么样的标准？

一要美观：得填满缺失牙齿所在的牙列。简单讲，就是带上假牙后在外看起来不能有缝。

二要好用：做到咬合面契合，和对位的牙齿要“有默契”。既要有足够的接触点把食物咀嚼碎，也不能牙面太高影响其他牙齿的咬合。

美观这个要求相对比较简单。

但功能性上的第二点就很难了，需要有经验的牙技师一点儿一点儿地在CAD软件里手调。这样做出来，还不能一步到位。

在假牙制作出来之后，需要给用户试戴看舒不舒服。一般情况下，咬合面都要做高一丢丢，然后根据咬纸情况再现场磨掉，或返工厂再加工才能用。

近日，**加州大学伯克利分校UCB，联合全球最大的定制牙齿修复生产商Glidewell Dental Lab训练出了一个牙冠设计生成模型(Generative Model for Dental Restoration)。**

模型设计工作Learning Beyond Human Expertise with Generative Models for Dental Restorations发表在arXiv上。

他们采用的这个深度学习模型，是GAN（生成对抗性网络）。可以基于待修复牙齿及对位牙齿的3D扫描，就预测出完美的牙冠面，once for all解决人工手调&来回磨的问题。

来看看效果。
![](https://pic1.zhimg.com/v2-1119548075a85bffd5603bb6a3a75274_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='542' height='277'></svg>)
左边圈红的是待修复的牙齿，最右是GAN预测的假牙牙冠。

可以看到，GAN设计的咬合面复杂程度比中间人类设计的要高很多，沟壑更加清晰明确了。

## **这么厉害，GAN都干了些什么**

镶过牙的小伙伴可能还记得，牙医护士小姐姐会递给你一种带薄荷味的软泥。这种软泥咬上一段时间后取出来，就是独属于你的牙模。这牙模完整无缺地复原了你口腔的空间信息，能不能打造一颗完美的假牙就靠它了。

牙模通常分为两部分。待修复齿列3D模型（如下图1）和对位齿列3D模型（下图3）。
![](https://pic3.zhimg.com/v2-92b9168469ce028a8948296e5dfc0816_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='552'></svg>)
 人类牙技师依照这两种3D模型，获得咬合水平面的2D深度图像（上图2，4），然后推算出咬合后的缝隙（上图5）。

无奈的是，所得到的假牙咬合面的2D图像（上图6）效果不太好，糊糊的看不清。没办法，人类只能抓瞎，照这个图6用CAD软件设计个大概，做出来假牙3D模型（上图7）。

整个流程就像下图左上方描述的那样。
![](https://pic3.zhimg.com/v2-3309dc845a82cf85249300cb7c442f2e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='579'></svg>)
 而GAN（Generative Adversarial Net）就不一样了。可用上述的四组数据（待修复、对位、缝隙深度图像、人类设计的牙冠图像）训练寄几。

为了避免设计出来的假牙顶着对位牙齿，研究团队还设置了一个机能损失函数（funtionality loss）。

研究团队采用的牙冠数据集，其中1500例用于训练，1570例评估用，和243个人类设计失败的案例来做测试集。每个案例都带上述的4组数据。

## **哪里比人类做得好**

评估一颗假牙有没有做好，主要看两方面。

一个是顶撞点(penetration point)。顶撞点（下图小红点）指的是假牙的咬合面有些地方做得过高了，导致整口牙咬不紧。类似那种咬鸡腿老是咬不断的感觉。
![](https://pic4.zhimg.com/v2-ba7b6ccda2d2ff96d290c62d238f3933_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='547' height='434'></svg>)Cond3和HistW都是GAN采用的不同方法，HistW是表现最好的一代。
按理说，没有顶撞点是最理想的。

从上图可以看出，GAN上测试集时，HistW（右下）设计出来就完全不存在上下牙齿打架的情况（没有红点）。比起人类design的牙冠（左下），GAN表现💯分。

另一个是看咬合接触点(contact point)的数量和分布。
![](https://pic2.zhimg.com/v2-e16adf486eae26a32b7af80a7ba42e05_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='541' height='436'></svg>)Hist第二代（右下）是HistW的进化版，咬合功能表现再上一层楼
 这个点数量越多，分布越广，牙齿的咀嚼功能就越好。

很明显，你可以看到测试集GAN的预测表现也比人类（上图左下）要优秀。

## **上临床，看疗效**

研究团队很开心能拿到这个结果，正在快马加鞭地生产GAN设计出来的假牙，给嗷嗷待换牙的病人使用。

此次研究工作，是GAN首次用在牙齿修复这种医疗定制化用品上。大家都期待实物效果，毕竟最后好不好使，还是要看临床应用的表现。
![](https://pic4.zhimg.com/v2-9b378c6d922aa2fc72caa1d72682c1eb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='275'></svg>)
最后，附论文地址：
[https://arxiv.org/abs/1804.00064](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.00064)

— **完** —
欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)
诚挚招聘
量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。
[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者
վ'ᴗ' ի 追踪AI技术和产品新动态




