# 吴恩达被diss了 - 知乎
# 



> 夏乙 岳排槐 发自 凹非寺
量子位 出品 | 公众号 QbitAI

吴恩达团队可能没想到突然被diss。

他们最近发布了全球最大的医学影像数据集MURA，还用这个数据集训练了一个169层的深度卷积神经网络模型。

发布数据集的同时，顺带也披露出这个AI模型识别肌肉骨骼病变的能力，比放射科医师还是……差了那么一点点。
![](https://pic4.zhimg.com/v2-40727e96bd739b79b59760006da7dc07_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='296'></svg>)
结果这件事，成为了导火索。

计算生物学界的一位战士，加州理工教授Lior Pachter见到这个成绩后，翻出了吴恩达几个月之前发布的一条Twitter：
![](https://pic2.zhimg.com/v2-82ab58173567f87c28bb18ed81d2b4f1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='510'></svg>)
> 放射科医师是不是该担心一下自己的工作了？大新闻：我们的AI（ChexNet）现在读胸腔X光片诊断肺炎的能力已经超过放射科医师了。

这条搞了个大新闻的消息紧扣公众痛点，直指放射科医师的失业问题，获得了1400次转发，2400个赞。

结果，反差出来了。

同样一个团队，去年发布ChexNet时，暗示放射科医师面临失业。

而最近这个MURA模型的表现意味着：放射科医师的工作，似乎又保住了。

## **学界争论**

其实之前ChexNet诊断肺炎“超越放射科医师”这个说法提出时，受到的质疑也不少。

例如，Lior一手带出来的博士，现在斯坦福做博士后的Harold Pimentel，看见吴恩达的Twitter就立刻就拿出来批判了一番：
![](https://pic3.zhimg.com/v2-3e12a7332aedea3e41ffd730ba894b1a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='688'></svg>)
> 放射科医师们没什么可担心的；
介绍研究成果的时候要负责任一点。

而跨界放射科医师和机器学习研究者的Alexandre Cadrin也说，论文结论很有意思，但放射科医师们“不用担心”。

为什么不担心？秘密就在ChexNet的结果里。

Harold后来给出了解释：
![](https://pic3.zhimg.com/v2-b9eb031e896f6d79db93b2f5ba8aada2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='480'></svg>)
他说，机器学习界研究医学影像问题至少30年了，但识别的结果普遍还是不如经过专业训练的人类。吴恩达Twitter一发，说得好像这个问题已经解决了一样，可是他们只找了4名放射科医师，略微超过了他们的平均成绩。

医生Eric Topol也在论文结果中标注了这样两句话，提醒我们注意：
![](https://pic4.zhimg.com/v2-5c2a0b98637ea84bbdb9ce5288da7017_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='747'></svg>)
这个模型，就和4名放射科医师比试了一下，4个人就能代表所有从业者了？
![](https://pic3.zhimg.com/v2-6a78842ed4ba97bc54ce7bb61f721962_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='585'></svg>)
还有一名懂机器学习的放射科医师Declan O’Regan指出，论文中的数据不足以支撑吴恩达Twitter的结论，没有曲线下面积（AUC）的统计对比，没有未选病例的外部盲测，没有细节，没有正常病例的比例，对误分类情况也没举出例子。

质疑并不少，但是所有人加起来，声量也不够大。

生物学家、医生、以及吴恩达的半个机器学习同行们对这条Twitter的不满，很大程度上也是因为吴恩达老师的学术地位和影响力。
![](https://pic4.zhimg.com/v2-f6b7624a00875ff750909a9ca0394bbb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='498'></svg>)
在Lior看来，吴恩达作为一名闻名学界内外、有30万Twitter粉丝的大V，如此耸人听闻，对同行、对公众、对整个科学家群体来说，都不是什么好事。

对同行来说，伤害显而易见。知名学者拿着自己的研究成果大声吆喝，普通科学家连一个小职业都消灭不了，论文哪还好意思发表？

对公众和科学家群体来说，基本的信任就是这么被消灭的：公众一次又一次这样被言过其实地忽悠，谁还会相信科学家？

事情就是这么个事情。

## **Lior Pachter其人**

Lior Pachter是计算生物学、计算和数学科学教授，目前任教于加州理工大学。目前其团队主要研发基因组学的计算和实验方法，专注于单细胞测序技术的发展及其在RNA生物学中的应用。

这已经不是他第一次在网上开炮。
![](https://pic4.zhimg.com/v2-1f6e7def722ecf7600d4d27553811703_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='351'></svg>)
例如四年前，刚刚开通博客不久的Pachter，就连续发表三篇熊文，强烈抨击此前发表在Nature Biotechnology上的两篇论文。

他抨击的对象，一位是知名的复杂网络研究权威Albert-Laszlo Barabasi，另一位是时任MIT计算生物研究组负责人的大牛Manolis Kellis。

是非曲直，我们不做深究。但在这一时间里，Pachter的风格同样一览无余的展现了出来。当时有报道这么记述：

> “莱尔写博客的时间不长，2013年8月开始，也就半年多的时间。但绝对绝对是博客界的奇葩。莱尔写博客的风格非常鲜明，那就是无论如何上来先噼里啪啦一顿胖揍。”[1]

以及上面提到的第二个批评，还是Pachter隐忍多年，找到一处细节之后，全力启动的一次爆发。足见这位教授也是心思缜密、很爱开炮，让人不由得想起纽约大学的马库斯教授。

与当年的洋洋洒洒、雷霆万钧相比，他这次对吴恩达的批评，应该算是“三和一少”的态度了。

而说马库斯，马库斯就到。

刚刚马库斯也出手参与了一下这件事，他又在Twitter上开炮：

> “过度炒作可能已经成了吴恩达的习惯，在影像诊断上，在汽车上，更普遍地说在AI上都是这样。”

## **关于未来**

其实，放狠话的不仅仅是吴恩达。

此前，声名煊赫的Geoffrey Hinton曾经放言：从现在开始，应该停止培训放射科医师。他预言5-10年内深度学习就将取代这些岗位。
![](https://pic4.zhimg.com/v2-0f548b6a3e594160b32ea7c0474ea15b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='476'></svg>)
作为一个克制的英国人，Hinton对深度学习能带来的医疗革命充满热情。“我看过太多的医生低效的使用数据，病患的历时信息被大量忽略；我看到医生们无法很好地阅读CT扫描，两个医生对同一个扫描片能得出不同的解读”。

在三次不同的场合，医生基于CT扫描说他妻子有继发性肿瘤，但每次都是误诊。Hinton相信AI会最终让放射科医师失业。

值得注意的是，前不久吴恩达的老师Michael I.Jordan发表了一篇博客，题为《人工智能：革命远未到来》。
![](https://pic2.zhimg.com/v2-3f85fa24a7409ed34c5603bccc813d99_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='673'></svg>)
在这篇文章里，Jordan同样回忆自己14年前的往事，但暗示机器应用于医疗系统之后，数据误差等原因使得“唐氏综合症的诊断就不断上升”，“导致了一些婴儿不必要的死亡”。

从这一个案例出发，Jordan抛出一个观点：AI正在暴露出严重的缺陷，大家过于依赖数据，深度学习不是AI的全部。并且建议关注更多推理、因果关系等方面的进展。

未来，争论可能仍会不停上演。好在所有人的目标都是一致的，那就是如何更好的造福人类社会。

[1] 薛宇：《愤怒的莱尔·帕切特》[http://blog.sciencenet.cn/blog-404304-770977.html](https://link.zhihu.com/?target=http%3A//blog.sciencenet.cn/blog-404304-770977.html)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


