# 共享GPU来了！投身去中心化机器学习，比挖矿多赚3倍 - 知乎
# 



> 夏乙 若朴 发自 凹非寺
量子位 出品 | 公众号 QbitAI

还在用GPU挖矿？那你可就亏大了。

最近，大洋彼岸出现了一个名叫“Vectordash”的机器学习共享算力平台，用开发者自己的话讲，相当于一个GPU的Airbnb。

有GPU又嫌挖矿赚得太少？可以在这个平台上共享自己的计算资源，帮助别人搞AI。

收益是挖矿的3到4倍！

AI开发者们在平台上租用共享出来的GPU，价格又只有AWS的1/5。
![](https://pic4.zhimg.com/v2-b908798017def696582487d82261d71b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
## **穷学生**

这个简直像福利一样的平台，是穷学生Sharif Shameem和两位小伙伴的业余项目。

Shameem在Reddit上发帖介绍说，他们建立这个平台，是受到自己经历的启发。

作为正在研究人工智能、深度学习的穷学生，Shameem总是要用GPU来训练神经网络，可是AWS和Google Cloud都好贵，那点学生优惠又根本不够用。

“老师，这个学期的期末作业我能晚点交么？实在是没钱租GPU，模型还在用CPU慢慢训练……”

这个迟交作业的理由看起来也实在寒酸，不过穷学生有自己的办法。

他找到了一个用1080 Ti挖以太坊的朋友，给他挖矿收益两倍的价钱，用这位矿工的电脑愉快地训练起了自己的神经网络。还是比AWS便宜多了。

可是独乐乐不如众乐乐啊，这么好的模式，值得让全世界的矿工联合起来，全世界的无GPU者也联合起来。

于是就有了Vectordash。
![](https://pic3.zhimg.com/v2-07ced09186884adabf27dd9da5b5279e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='464'></svg>)
来，记住这三名穷学生的名字，万一他们成了下一个比尔·盖茨呢。他们是：来自马里兰大学的Sharif Shameem、Arbaz Khatib，和来自CMU的Abhishek Bhargava。

## **机主多卖钱，买家少花钱**

官方网站上针对各种GPU给出了一个清晰的价目表：赚得最少的老GPU 1050，每天1.92美元；Titan V赚的最多，每天10.08美元。
![](https://pic3.zhimg.com/v2-01f46b32bf3d2a8bbb1e5560c23eec9a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='794' height='1938'></svg>)
GPU提供方赚的比挖矿多，但是开发者租到的GPU实例并不贵。每小时0.17美元起，仅仅是AWS的1/5，租个最新的Titan V，每小时才0.85美元。
![](https://pic4.zhimg.com/v2-56c972da90f14684b50ec028192ded27_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='874' height='2262'></svg>)
每个GPU实例还都预装了Ubuntu 16.04、CUDA、cuDNN、TensorFlow、PyTorch等等机器学习必备环境。

对比一下两个列表，机主多卖钱，买家少花钱，算一算平台差价好像也没少赚，真正的皆大欢喜。

## **怎么用呢？**

对于开发者来说，用Vectordash和用其他云服务差不多：租用了GPU实例，就能拿到供连接用的IP地址和SSH密钥。

我们来看看这段视频演示：
[undefined_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/l0613v4puh9.html)![](https://pic2.zhimg.com/v2-9453e15c00cd0e2bf241973543ce62ed_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='332'></svg>)
可惜的是，量子位没能亲自替大家试一试，因为目前所有机器都租出去了。



![](https://pic1.zhimg.com/v2-87679d4eb7fa62eab6a4658f7690acf8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='277'></svg>)抱歉，所有机器当前都在使用中。有兴趣成为机主嘛？
没租到GPU，却被Vectordash网站问了一个直击心灵的问题：有兴趣把你的GPU分享出来做机主赚点钱吗？

假如量子位有一块1080 ti，怎么会放过这个分分钟变身云服务提供商，造福AI研究者顺便一天赚7.68美元的机会？GPU还没买，先帮大家探探路也是好的。

成为机主并不难，需要先填个表：
![](https://pic1.zhimg.com/v2-697b6cdb1acf8db5b21e1a8fb7afc4fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='831'></svg>)https://goo.gl/forms/ghFqpayk0fuaXqL92
然后，会在一天之内收到Vectordash的指南邮件，指导机主安装管理程序，共享GPU。

机主可以在管理程序中自行选择提供GPU的时间段，另外，这个管理程序还支持自动切换功能，当Vectordash上没人占用GPU时，可以让它去挖矿，最大化收益。

为了收款，机主还需要绑定一个虚拟货币钱包。用户用完GPU之后，机主会在5秒钟内收到钱，可以选择比特币、以太坊或者莱特币，而且随时可提现。

## **新机遇**

哦卖糕，这个模式让大家兴奋了。

“辍学吧，你就是下一个比尔·盖茨。”

在reddit论坛上，出这个主意的不止一人。他们都撺掇Sharif Shameem和伙伴们别搞研究了，专注在这个项目上，成为亚马逊的对手。

目前亚马逊提供的GPU云计算服务，使用的都是K80等专用产品，而更便宜的消费级GPU不能用于云计算这样的商业场合。

不过，这种共享模式，似乎绕过了英伟达的限制。

Vectordash创始人表示，他们在1080Ti上训练神经网络，比在亚马逊云上使用Tesla K80训练的速度快5倍。

人们纷纷表示，原意出租自己的GPU。Shameem在Twitter上说，beta版上线17小时，就收到了143块GPU。
![](https://pic3.zhimg.com/v2-f415b39e041086b37ab4a2753897317e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='360'></svg>)
还有人不甘心的问：AMD显卡行么？

当然不太行。

Vectordash目前还不支持AMD的显卡，因为目前OpenCL还没有得到机器学习库的广泛支持。当然，Vectordash的伙伴们也希望AMD、英特尔、Google能尽快赶上，他们不想活在英伟达一家独大的世界。

这个业务模式，不免让人们想起Golem。

据介绍，Golem是一个去中心的全球算力市场。Golem改变了算力任务的组织和执行方式。下面有段视频介绍，没有字幕。
![](https://pic4.zhimg.com/80/v2-b6c2f457c52340b0e5e9495b206f4d23_b.jpg)https://www.zhihu.com/video/961217622336073728
## **隐忧**
![](https://pic3.zhimg.com/v2-21ca1d74e34c136b3dc3681939c4cd46_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='978'></svg>)
“去中心化的机器学习真是亦可赛艇，”陈天奇（XGBoost、MXNet、TVM的作者）说：“但是可能会有安全隐患。”

+1，Keras作者François Chollet附议并且进一步指出，随机使用别人的电脑听起来会引发隐私和安全灾难。对于那些训练数据很敏感的项目尤其如此，所以还得需要一个更安全的方式进行。

在reddit论坛上，有网友问：我有兴趣提供GPU，但怎么保证我的个人信息不会泄露？怎么保证我不会成为僵尸网络的一部分？

Vectordash创始人回答说，他们用了基于LXC的Hypervisor（虚拟化技术的核心，可允许多个操作系统和应用共享一套基础物理硬件）。

不过只是这些，似乎还不能让人完全放心。

“我有一个完美的解决方案！”

现在Google Brain团队的David Ha说，他准备用区块链解决信任和安全问题，并且准备发行TensorFlowCoin，然后还要ICO……

抢先发币，不愧是投行转AI的大牛。
![](https://pic2.zhimg.com/v2-63c4e5eac52010cdd20022fa4ad699f5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='271'></svg>)
他当然在开玩笑~

不管怎样，一切刚刚开始。也许未来有一天，在人工智能领域真能实现：滴滴一下，马上训练。

> “在家呢？快上线吃鸡！”
“要不还是农药吧？”
“怎么了？”
“我家GPU上班挣钱呢，不想打扰它。”

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


