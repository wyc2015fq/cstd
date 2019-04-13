
# Mixup vs. SamplePairing：ICLR2018投稿论文的两种数据增广方式 - Paper weekly - CSDN博客


2018年03月06日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：917



![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**47**篇文章

# 关于作者：陈泰红，小米高级算法工程师，研究方向为人脸检测识别，手势识别与跟踪。

■ 论文 | mixup: Beyond Empirical Risk Minimization
■ 链接 | https://www.paperweekly.site/papers/1605

# 论文动机

神经网络训练需要海量的人工标注数据集，一般的数据增广方式是裁剪、翻转以及旋转、尺度变化。之前在 arXiv 上看到过 IBM的一篇文章[SamplePairing：针对图像处理领域的高效数据增强方式](https://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487055&idx=1&sn=c99dbce318a826bc7c92ae0a0870513f&chksm=96e9d1cfa19e58d980e4ecc957ab97045b2178925738b296fda56302831e96217e99bf9fb320&scene=21#wechat_redirect)，该论文**主要是关于数据增强方式，没有公式没有网络架构，只通过简单的相加求平均值方式**。

而在最近公布的 ICLR 2018 入围名单中，另一篇数据增广相关论文脱颖而出，而 SamplePairing 出局。仔细阅读 Mixup 的论文，发现它其实是对SamplePairing 的更进一步延伸。

# 名词解释

**Empirical Risk Minimization (ERM)**：机器学习的经验风险最小化，ERM 策略认为，经验风险最小化的模型是最优化的模型。可参照李航的《统计学习方法》[1]进行理解。

**Βeta分布**：既然概率论中的贝塔分布，是指一组定义在是指一组定义在（0，1）区间的连续概率分布，有两个参数 α 和 β。论文中 α 和 β 相等。Βeta 分布的定义、概率密度函数和性质可参考 PRML[2]。

为了理解 Beta 分布，使用 Python 可视化 Beta 的模型。论文选择的超参数是 α=0.2 和 0.4，此处主要观察 α 变化对应的概率分布变化。

```python
from
```
```python
scipy.stats
```
```python
import
```
```python
beta
```

```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```

```python
import
```
```python
numpy
```
```python
as
```
```python
np
```

```python
x = np.linspace(
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
100
```
```python
)
```

```python
a_array = [
```
```python
1
```
```python
,
```
```python
0.5
```
```python
,
```
```python
0.1
```
```python
,
```
```python
0.2
```
```python
,
```
```python
0.01
```
```python
,
```
```python
0.001
```
```python
]
```

```python
for
```
```python
a
```
```python
in
```
```python
a_array:
```

```python
plt.plot(x, beta.pdf(x, a, a), lw=
```
```python
1
```
```python
, alpha=
```
```python
0.6
```
```python
, label=
```
```python
'a='
```
```python
+ str(a) +
```
```python
',b='
```
```python
+ str(a))
```

```python
plt.legend(frameon=
```
```python
False
```
```python
)
```

```python
plt.show()
```

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnZsVntiaL6V4a51oYlmTahOJviaEcywWWzgj150icLmQNGiaG7mvh2Mvia2XTib5aEQS2b8qQibHJBd3ibEA/640)

从上图可以看出，α 趋近于 0 时，概率分布趋近于 x-0 和 x=1 两种情况，在论文中代表 ERM。

# 模型细节

**SamplePairing**

SamplePairing 的实现很简单，两幅图片直接像素相加求平均，监督的 label 不变。但是在训练过程中，先用 ILSVRC 数据集普通数据增广方式，完成多个 epoch 后间歇性禁止 SamplePairing，在训练损失函数和精度稳定后，禁止 SamplePairing 进行微调。

个人认为相当于随机引入噪声，在训练样本中人为引入误导性的训练样本。

**mixup**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnZsVntiaL6V4a51oYlmTahOQVb1ugcLD6dhOw1LR1Aa5wdCiaNOFFfeAcIubPQ1KLq76r47zKmlShA/640)
△mixup实现公式、Python源代码和可视化实现

其中 (xi, yi) 和 (xj, yj) 是训练集随机选取的两个数据，λ ∈ [0,1]，λ ∼ Beta(α,α)。

mixup 扩展训练集分布基于这样的先验知识：线性特征向量的混合导致相关目标线性混合。混合超参数 α 控制特征目标之间的插值强度，α→0 时表示 ERM。

mixup 模型实现方式简单，PyTorch 7 行代码即可实现。上图中的可视化表明，mixup 导致决策边界模糊化，提供更平滑的预测。

# 实验

论文的实验过程很丰富，包括 CIFAR-10，CIFAR-100，和 ImageNet-2012，随机噪音测试，语音数据，facing adversarial examples 黑盒攻击和白盒攻击，UCI 数据集，以及稳定训练 GAN 网络。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZsVntiaL6V4a51oYlmTahO1Waa3SI4DgklrkYmsu22yYlW9micpXFibxj5cQmvBjRS90icTRRcx6B2g/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZsVntiaL6V4a51oYlmTahOVVgkIrCzqueuzuccevolaplZibMJDRj74M94e7SoicgmiaTWQ2yhPZz6Q/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZsVntiaL6V4a51oYlmTahOYSkb8vlIMTtBicnS4cosbpibPjgd3GzY3GWAVibZTRKY8YiamELicrmwRGg/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZsVntiaL6V4a51oYlmTahObBbJGAA1diaib2ZS2H4VxpYaxEShUibVk4LXNy5vkAC6iab1beUiaVVA73g/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZsVntiaL6V4a51oYlmTahOVEK2pbGvK93icAlSjCrJKSliaaD3w1V996BgHibh4mAYb4pLf92JkyRaw/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZsVntiaL6V4a51oYlmTahOCjPGgctBwwPljaP8QDlkFzSUicgntqX0yt2NRgWNsO4o9hKubmm9qmQ/640)

对于具有代表性的 ImageNet-2012，Top-1 的精度至少提高 1.2%。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnZsVntiaL6V4a51oYlmTahOatLsTulY0V1ZN3CDJTdEHP4fx3oAib5CX79DXnuRaU21yibJNtwq1aOg/640)

# 讨论

论文提出，在训练过程中，随着 α 增加，训练误差越来越大，而在验证验证集测试中泛化误差反而减少。这与论文提出的假设相同：mixup 隐含控制模型的复杂度。但是论文没有提出 bias-variance trade-off 的理论解释。

论文提出一些进一步探索的可行性：

mixup 是否可以应用在其他监督学习问题，比如回归和结构化预测。mixup 可能在回归问题容易实现，结构化预测如图像分割等问题，实验效果不明显。

mixup 是否可以用于半监督学习、无监督学习或强化学习。当然作者是假设，希望有后来者证明 mixup 是理论可行的。

mixup 来自 MIT 和 Facebook AI Research。ICLR 是双盲评审，官网上的匿名评审意见普遍认为 mixup 缺乏理论基础，但是实验效果具有明显优势。笔者个人认为在 mixup 基础上，还有很多坑可以填。

# 参考文献

[1] 《统计学习方法》，李航
[2] Pattern Recognition and Machine Learning, Bishop

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击以下标题查看相关内容：**

SamplePairing：针对图像处理领域的高效数据增强方式



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkvSxCRgqK914dI363k7VWmXJBVCNtexC0iaXJXUohTRgBpX4wGrDNYNpPB6SrZHiccz24zYcN2th2Q/640?)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****榜 单 公 布****\#**

**[2017年度最值得读的AI论文 | NLP篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487348&idx=1&sn=8ee8bf57418342a419fe73829cb14e75&chksm=96e9d0f4a19e59e288dcb105bd90b1e13f419ee7268ac69eba7cd6dac12e2e64aa84c56e5c07&scene=21#wechat_redirect)**
**[2017年度最值得读的AI论文 | CV篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487362&idx=1&sn=980153481f88ba5d6ba2929fd08240f3&chksm=96e9d002a19e5914f845973111b7056d24a28a8f7932479178c9cfcdb10c05c3284d5c0eb602&scene=21#wechat_redirect)**


我是彩蛋

**解锁新功能：热门职位推荐！**

PaperWeekly小程序升级啦

**今日arXiv√猜你喜欢√****热门职位****√**

找全职找实习都不是问题
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**职位发布**
请添加小助手微信（**pwbot01**）进行咨询

**长按识别二维码，使用小程序**
*点击阅读原文即可注册

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)
▽ 点击 |阅读原文| 查看原论文


