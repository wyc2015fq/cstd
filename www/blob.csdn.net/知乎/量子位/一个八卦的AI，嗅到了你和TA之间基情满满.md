# 一个八卦的AI，嗅到了你和TA之间基情满满 - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

人类，天生就有八卦嗅觉。

比如匡扶正义的侦探组合：
![](https://pic3.zhimg.com/v2-52ff249f84725141a967dbdb35a6c9a6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='180'></svg>)
就非要YY成CP。

明明是亚瑟王和梅林大法师的中古传奇：
![](https://pic1.zhimg.com/v2-193ad7ce7a111933f05399990ff111f8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='992' height='620'></svg>)
还是要YY成忠仆小法师保护二缺王子的基情故事。

明明是江户时代正经武士：
![](https://pic1.zhimg.com/v2-e3f53a3fedd2431e4fea737d9fbc3db4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1044' height='783'></svg>)
还是要臆测人家的性取向。

可能是遗传了人类父母的八卦天分，机器们也学会八卦了。一项中山大学人机物智能融合实验室与商汤科技合作的研究，就让机器学会了判断照片中人物的社交关系。

换句话说，把你和你基友或女友的合照拿给AI看，它就可以判断出：

这两个人，关系不一般呦。
![](https://pic4.zhimg.com/v2-ac5fe2a1f5ba6621a1834c6f9a80cb7f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1073'></svg>)△ 同学哪个班的？早恋要叫家长的哦
比如下面这张图：
![](https://pic1.zhimg.com/v2-294c3aca8eae3435c30afdcccdfe6288_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1276'></svg>)
这三个男人是什么关系？

他们系着领带，旁边拜访着书籍资料，一侧还有电脑的键盘，中间那位笑的那么不自信，肯定是传说中的乙方跪舔甲方了。

AI没有那么加戏，淡定的说：professional，职业关系。

毕竟，万一它把老板认错了，那就十分尴尬了。
![](https://pic1.zhimg.com/v2-a1e0be4933678d6bd402918f604a67a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='988' height='1276'></svg>)
而在这幅照片中，AI发现主角是两女一男，他们面前有酒瓶，有酒杯，仿佛是朋友在聊天。

但后面不同了，背景居然是一张大床，看来，三位主角是在卧室旁边的阳台上了。

所以AI很淡定的给出了判断：family。

无知者无畏，也不知道这两位女士和一位男士组成了怎样的一个family。不过，AI同学开放的心态值得我们这些车技不好的人类学习。

## **数据集：人类社交启蒙**

既然要让机器理解人类的“基情”，首先就需要一位启蒙者告诉机器：

**什么是基情？**
![](https://pic2.zhimg.com/v2-0ef6f0299b83ad33618970f4f6eb16f5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='619' height='659'></svg>)
鉴于这是一个让人十分难以开口的哲♂学问题，那我们就把这个问题交给数据集老师，反正它们没有意识，下限比较低。

所幸的是，在这项研究之前，就有许多AI和心理学界的老司机对“人与人的关系”这一话题进行过探讨，给我们留下了许多充斥着哲♂理的社交关系探讨，还有成千上万的数据。

比如，心理学研究者Daphne Blunt Bugental在2000年就将人与人的社会关系分为了五个大类和16种关系，并且贡献了社交关系数据集**PIPA**，请注意，是PIPA，不是papi酱。

在后来研究者们的努力后，PIPA有了26915个带注释的人物关系数据。

除了PIPA之外，另一个主要的数据集叫**PISC**，这可是上世纪的祖传数据集了，是一位名叫艾伦·菲斯克的心理学家，在1992年根据人类的社会层级写了一本书，叫做《社会性的四种基本形式：统一的社会关系理论框架》。

书中定义了3种粗略的社会关系和6种细分的社会关系，作者还顺便搞了PISC数据集，包含22670个图像，标注了覆盖以上九种社交关系的96568对社交关系数据。

感谢这些心理学前辈，让可爱的数据们“教坏”懵懂的AI。

## **读懂关系：GRM+GGNN**

现在，就该模型与神经网络登场了。

研究者们训练了一个**图推理模型**（Graph Reasoning Model，GRM），由该模型结合**门控图神经网络**（Gated Graph Neural Network，GGNN）对社会关系进行处理。
![](https://pic1.zhimg.com/v2-4f6f5b870062855f45f9d5bd2edf5dc4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='311'></svg>)
研究者们设计了这样一个流程：

GRM根据图上人物区域的特征来初始化关系节点，然后用预先训练的Faster-RCNN探测器搜索图像中的语义对象，并提取其特征，初始化相应的对象节点；

之后GGNN负责计算节点特征，通过图传播节点消息以充分探索人与上下文对象的交互，并采用图注意机制自适应地选择信息量最大的节点，以通过测量每个对象节点的重要性来促进识别。

## **把物品和关系编织在知识图谱上**

最终的研究成果需要变成知识图谱上的结构化数据。

这份知识图谱需要包含两种内容：

社交节点，即酒瓶、床、书籍有社交场景意义的道具；还有社交关系，家人、朋友、同事还是情侣。

通过特征向量计算后，逐步优化，在PISC数据集的基础上，研究者得出了类似下面这样知识图谱：
![](https://pic2.zhimg.com/v2-3ec4d7f24f9c691002574bce02072bcd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='674'></svg>)
可以看得出，该示例中的社交关系主要涵盖夫妻/情侣、家庭、朋友、职场四种，而照片中可能出现的不同物品则对应着不同的社交关系，比如：

当烤箱出现，那八成是家庭关系；

当键盘出现，那肯定是在办公室；

当汪星人出现，那要么是一家人一起遛狗，要么就是和朋友一起撸狗了；

……

## **效果**

准备好了知识图谱，就可以开始测试研究成果了。
![](https://pic2.zhimg.com/v2-64f6dde0a3c45802b530552e128cd815_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='697'></svg>)
这张图上我们可以看到，人们身边有电脑、水杯、背包、桌椅等物品；
![](https://pic1.zhimg.com/v2-7e52ccf95740f9b1282e598c7f4ca90c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='590'></svg>)
经过知识图谱判断，电脑和背包两个物品十分强烈的指向了工作场景，因此这张图上人们的社交关系被判定为prefessional，像是年轻人像年长的专家请教问题。
![](https://pic1.zhimg.com/v2-b80e8c799485d2f3bc1c592ac827f5b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='696'></svg>)![](https://pic4.zhimg.com/v2-c0b268d3dbc3e5a5afbcb30be1c743b3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='661'></svg>)
而下面这张图上，儿童与成人之间放慢了披萨和碗碟，虽然也有水杯、桌椅、书籍这些非明显家庭特征的物品存在，但凭借披萨和碗碟，还有桌子上的肥宅快乐水，图上的三个人这么能吃，一定是一家人。

后来研究者们把测试数据做了一下汇总，整体的成绩高过了此前的研究。

研究者用**每类召回率（per-class recall）**和**平均精度（mAP）**来判断效果，从数据上来看效果不错。
![](https://pic3.zhimg.com/v2-bad862e0c3451e80512498618ceb2ada_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='317'></svg>)
在PISC数据集上，根据各类的recall，在亲密、非亲密、没关系3个社交关系大类上，研究者的新方法对比旧方法2:1胜；在6种细分社交关系上，新旧方法3:3平手。

与原来的方法相比，这种新方法更擅长识别“朋友”、“职业关系”和“不不不我们什么关系都没有”。

而根据mAP数据，新方法完胜。
![](https://pic2.zhimg.com/v2-4e99fdfef2518d896b916e60e1ce2d29_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='746' height='228'></svg>)
PIPA数据集的效果也不差，比该数据集上此前的两个方法都高。

## **研究者：林倞教授团队**

这篇论文有6位作者，其中5位来自中山大学数据科学与计算机学院下属的人机物智能融合实验室，另一位作者是商汤科技研究院研发总监任思捷博士。

论文的共同一作陈添水博士是林倞教授的学生，在实验室官网的人员名单中，他被排在了第一位，估计也是林教授的得意门生了。
![](https://pic4.zhimg.com/v2-4a1b9170bbaa70b0230a231d634e79d7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='544'></svg>)△ 其实另一位共同一作Zhouxia Wang也在上面
这已经是陈添水博士的第三篇IJCAI了，此前他还发过两篇AAAI和一篇ICCV，还拿过ICME 2017 World’s FIRST 10K Best Paper Award—Diamond Award。

另外，陈添水博士还在h-index排名183的香港理工大学张磊教授手下做了一年的研究助理。

说到这里，就不得不提一下人机物智能融合实验室教授林倞了，他不仅是这个实验室的负责人，还是商汤科技首席研发总监，入选了第三批国家“万人计划”青年拔尖人才建议人选名单。
![](https://pic3.zhimg.com/v2-0a07058a05d29258749ea51164a2da7e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='427'></svg>)△ 林倞教授，图片来自中山大学网站
## **论文传送门**

Deep Reasoning with Knowledge Graph for Social Relationship Understanding
作者：Zhouxia Wang, 陈添水, 任思捷, 余伟浩, 成慧, 林倞
发表于IJCAI 2018
arXiv: [https://arxiv.org/abs/1807.00504](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1807.00504)
github: [https://github.com/HCPLab-SYSU/SR](https://link.zhihu.com/?target=https%3A//github.com/HCPLab-SYSU/SR)

—**完**—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


