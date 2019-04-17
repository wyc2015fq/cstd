# 谷歌大脑AutoML最新进展：不断进化的阿米巴网络 - 知乎
# 



> 原作 Esteban Real（高级软件工程师）
Root 编译自 Google Research Blog
量子位 出品 | 公众号 QbitAI

自然界用了5亿年时间。

才把神经系统从最原始的雏形——阿米巴(Amoeba)变形虫分化出可对外界环境刺激做出响应的内浆，进化到现在的高度复杂的人脑。

如今我们人脑，相对目前阶段的机器来说，可以轻而易举完成各种复杂的任务。

而机器的算法模型要做到同样表现（能力范围还非常局限），必须得靠大量的在AI领域里深耕多年的专家才行。

像认猫、识图、辨别基因突变、或者是诊断疾病这些专项的任务，每一个有效的模型的搭建和调试都必须重度依赖AI专家的经验。

有没有什么方法，可以训练出一个根据外界环境的变化，像阿米巴变形虫那样即时调整出最优的生存策略，不断自我进化获取最优结果的算法呢？

## **自进化的AmoebaNet**

还真的有。

谷歌大脑今日就在研究博客更新，他们已经研究出了一种拥有很强的自我进化能力的神经网络，AmoebaNet。

这个工作的研究基础是2002年Stanley和Miikkulainen的神经进化拓扑学。而其他研究机构，OpenAI，Uber实验室，还有DeepMind他们也有在研究自进化的算法。

谷歌本身有强大的算力优势。所以谷歌大脑团队在琢磨，除了采用强化学习这种方法，怎么样才能能让图像分类器的迭代速度加快。

迭代速度快还不够，出来的效果也必须好，最好还不需要重度依赖专家。毕竟现在问题那么多，AI专家根本不够用。

快、好、省。这就是谷歌大脑要AmoebaNet满足的三个点。

## **做到快、好、省的思考路径**

2017年ICML（国际机器学习大会），谷歌大脑发表了一篇论文*Large-Scale Evolution of Image Classifiers*。这个工作采用了非常简单的神经网络，以及简单的初始设定，让算法自己去进化出网络架构。当时这个算法的效果就已经可以媲美人工手调的模型了。

这项工作出来之后，大家还是挺激动的。因为这意味着越来越多的AI应用可以一定程度上减少模型人工搭建调试的压力。

这下，对没有时间进阶成一个AI专家的用户来说，也能获得一个效果越来越好的模型了。

谷歌大脑思考的下一个问题是，人工手调和自我进化结合的模型，效果会不会比单独的要好？

（人机结合疗效好？）

所以，谷歌大脑最近的一篇论文*Regularized Evolution for Image Classifier Architecture Search*就开始采用更为复杂些的网络以及优化过的初始设定，还用上了谷歌最新的TPUv2芯片。

这一次，谷歌大脑把最高配的硬件，AI专家的经验，以及自我进化的算法全砌上，就看在CIFAR-10和ImageNet上跑的效果怎么样了。

## **先看算法自我迭代的过程&效果**

下图出自谷歌大脑的第一篇论文，每一个点代表用CIFAR-10训练后得到的神经网络。
![](https://pic1.zhimg.com/v2-5c3ce2dce7fa97b59f436fdc765f92c4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='334'></svg>)神经网络进化的实验过程。四张表代表了不同时期进化得到的神经网络架构，最右的是精确性表现最好的神经网络架构。前面三个都是最好的架构的前身。
刚开始，整个神经网络群都没有隐藏层，组成架构都是上千个一样的简单种子模型。


一开始用的种子模型（这一步还没有结合AI专家的经验），每次自我迭代之后，评估神经网络的分类表现。

把表现最好的，也就是分类结果最准确的神经网络挑出来，当作母代模型，用于下一次迭代。

然后在母代模型的基础上，做微调，进化生成更多的子代模型，归进整个神经网络群中（相当于进化生物学里的种群的概念）。

表现不是最好的模型就没有变异进化的机会。

谷歌大脑第一篇论文的工作涉及到的变异都很简单。

比方说，随机移除一个卷积层，或者在随意两层之间加一个连接，或者是改变学习的速率等等。

该工作的结果可以看出来自我进化算法的巨大潜力，虽然搜索空间的质量可能麻麻地(dei)。

举个栗子。

如果把其中一个种子模型改成Inception-ResNet分类器的话，那么可能会得到一个错误的结论：现在这个算法表现还不错，得到一个比较满意的结果了。

但是这样操作的话，我们就把这次的变异操作硬编码了（把这次行为固定下来了，以后不好改），这样的话就变成以最终结果数值为导向，而不是终极效果目标为导向了。

把其他可能性灭掉后，就达不到进化的目的了。

在这个工作里，谷歌大脑把人工操作的部分压缩到最小范围，包含对ReLUs和批标准化层的干预。

## **再看人机结合后的模型效果**

在上一次工作之后，谷歌大脑想减少模型进化过程中的试错成本。采用2017年Zoph的类inception模块，有效地把搜索空间缩小，并固定了神经网络的架构，其中网络上节点的模块是可以替换掉的。
![](https://pic1.zhimg.com/v2-62f5eb2508ebb793e495b021aa1b940c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='405'></svg>)2017年Zoph引入的类inception模块。
最左边的那个图，指的是整个神经网络的外部架构，输入的数据从底部往上处理。

最右边的图，是神经网络节点上的模块内部框架。

整个网络的目标是产出精确性最高的输出结果。

人机结合后模型的变异，是随机重连输出，或者随机替换模块。最后进化出来的模型是高质量的。

## **不同方法得到的模型效果对比**
![](https://pic1.zhimg.com/v2-7b04b728362730a041ce3b41fef08248_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='312'></svg>)自进化、强化学习、随机搜索产生的模型对比。
从图可以看出，在训练的早期，自我进化的模型用最少的算力就获得了最好的效果。

另外，自我进化模型很重要的一个特征是，最旧的、很久没获得变异机会的模型会被弃掉。这样整个神经网络群的进化速度会越来越快。更多细节可以参考论文。

最后，附阿米巴网络AmoebaNet论文地址：
[https://arxiv.org/abs/1802.01548](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1802.01548)

& 编译来源：
[https://research.googleblog.com/2018/03/using-evolutionary-automl-to-discover.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2018/03/using-evolutionary-automl-to-discover.html)

**你可能感兴趣**

[别翻墙了，谷歌机器学习速成课25讲视频全集在此](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495096%26idx%3D1%26sn%3Dcb25eec7088e96f416bc1df2a6c2df10%26chksm%3De8d05acadfa7d3dca298cef87ed9bf836a81d9501be6210cac5c9a2f6fdc1a4dc81b10348229%26scene%3D21%23wechat_redirect)

[谷歌开源语义图像分割模型DeepLab-v3+ | 附代码](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495428%26idx%3D4%26sn%3D0051694b4035d108216c2864e875fe80%26chksm%3De8d05876dfa7d160969527548d2c223abc52464ac43d9d1f7e1208add166bd7ae4f85b460094%26scene%3D21%23wechat_redirect)

[首个72量子比特的量子计算机问世，谷歌出品](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495168%26idx%3D2%26sn%3Ddb65a3974b045e9bd53171d946c958a2%26chksm%3De8d05972dfa7d064e6023bf0cd4ce48f7709642d61e8c55a3d5c25ebed97130be9b45529ab3f%26scene%3D21%23wechat_redirect)

[破解AI大脑黑盒迈出新一步！谷歌现在更懂机器，还开源了研究工具](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495218%26idx%3D1%26sn%3Dc9ee5861aabf96daacfaa2647453fc79%26chksm%3De8d05940dfa7d056f72ddcbd221864556de946f97d17c41d7fff8f6264d7d3061ba80cfed806%26scene%3D21%23wechat_redirect)

[DIY发现新行星操作指南 | 谷歌开源了行星发现代码](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495291%26idx%3D5%26sn%3Dde169acac7046ecabbf98374bad1207d%26chksm%3De8d05909dfa7d01f34a47eabf9d53b0a925a9a4bcb20db45f94b58527cce88752fe7b78d6ebc%26scene%3D21%23wechat_redirect)

[绿幕通通扔掉 ੧ᐛ੭ | 谷歌AI实时「抠」背景](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495030%26idx%3D4%26sn%3Dab99ff411f8652bb12f6519fd75c81d6%26chksm%3De8d05a04dfa7d3125705e0672b194eab05916bfd480ae5df41c83101aff338c7782fde2bd4a2%26scene%3D21%23wechat_redirect)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




