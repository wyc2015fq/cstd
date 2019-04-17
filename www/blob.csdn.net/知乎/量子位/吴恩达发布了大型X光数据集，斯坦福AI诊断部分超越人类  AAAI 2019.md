# 吴恩达发布了大型X光数据集，斯坦福AI诊断部分超越人类 | AAAI 2019 - 知乎
# 



> 数栗子 发自 凹非寺
量子位 报道 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-fe9263142473c4a50047d41b4cf201a6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='664'></svg>)
最近，吴恩达的斯坦福团队发布了一个叫做**CheXpert**的**大型数据集**，论文中选了**AAAI 2019**。

它包含了224,316张标注好的胸部X光片，以及放射科医师为每张胸片写的病理报告。

虽然，像从前NIH发布的ChestX-ray14数据集那样，CheXpert也是根据医师的报告，自动标注肺水肿、心脏肥大等各种病症的；

但团队说，新的数据集更适合做**Benchmark**，因为**测试集**上的标签，是经过 (持证上岗的) 放射科医师验证的标签。
![](https://pic1.zhimg.com/v2-e1cbcbf3f96b0b90d3448b4639892238_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='668'></svg>)
团队便是用这个数据集做Benchmark，举行了一场5种疾病的识别比赛。

## **数据集是怎样炼成的**

这22万张胸片，都是2002年10月到2017年7月之间，斯坦福医院做过的X光检查，来自**65,240**个就诊的人类。




**有哪些标签**

每张胸片有14个标签要贴，其中12个，是表明患者是否存在**心脏肥大** (Cardiomegaly) 、肺部病变 (Lung Lesion) 等**12种疾病特征**。
![](https://pic1.zhimg.com/v2-854b0701609abf22d72ead42cfc39cb4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='324' height='660'></svg>)
另外两个标签，一个是无发现 (No Finding) ，另一个是辅助器具 (Support Devices) 。

14个标签，都分为**阴** (Negative) 、**阳** (Positive) 以及**不确定** (Uncertain) 这三种选项。

因为，并不是所有问题，都能从胸片上得到确定的答案。如果不能做出明确的判断，就不可以贸然输出阴或阳。




**自动标注工具**

有了14个标签要贴，吴恩达团队开发了一种自动贴标签的算法：从医师每张胸片做的报告文本里，找到和每个标签相对应的信息。

算法分三步：

> 一是提取文中提及的信息 (Mention Extraction) ，得出一个列表；

二是为提及的信息做分类 (Mention Classification) ，分成阴、阳、不确定这三类；
三是把提及的信息聚合起来 (Mention Aggregation) ，得出最终结论，标注完成。

团队已经把标注工具开源了：
[stanfordmlgroup/chexpert-labeler​github.com![图标](https://pic2.zhimg.com/v2-127c1aa00ab9abc7e4827c45d0d039e5_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/stanfordmlgroup/chexpert-labeler)



**人类医师验证一下**

数据集要成为合格的Benchmark，**测试集**给的标准答案就要有说服力。

团队找到8位持证的放射科医师，让他们独立去标注**500张胸片**的测试集。然后，把5位医师给出的主流答案作为Ground Truth；至于，另外3位医师的答案，后面有其他用途。

这样一来，各路诊断模型都可以在一个相对有效的标准之下接受评估。

对比一下，从前的ChestX-Ray14数据集里，测试集是自动标注，并没有经过人类医师的鉴定。团队认为，**CheXpert更加适合做Benchmark**。




## **你也来比赛吧**

既然提供了新的Benchmark，就可以拿来比赛。
![](https://pic2.zhimg.com/v2-1af615b9cb4f58ecf61eb34567cd6da5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='458'></svg>)
吴恩达老师发推宣布，一场疾病识别比赛已经开始了，欢迎大家带着自己的模型去参加。

比赛内容涉及5种疾病的识别：

> · 肺不张 (Atelectasis)
· 心脏肥大 (Cardiomegaly)
· 肺实变 (Consolidation)
· 肺水肿 (Edema)
· 胸腔积液 (Pleural Effusion)

为了鼓励小伙伴们前去参与，团队祭出了自家模型里成绩最好的一只：

用500张胸片的测试集来比，这个模型在心脏肥大、肺水肿以及胸腔积液这三项上，超越了前面提到的**“另外3位人类医师”**。
![](https://pic4.zhimg.com/v2-1003f7baf60e6c7ebde0394685ad9db3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='910' height='446'></svg>)
若以粗暴的五局三胜为规则，AI已经赢了。

有兴趣的同学，可以去试试看自己的模型能不能超越斯坦福的AI。数据集和参赛方法都在这里啦：

CheXpert数据集传送门：
[https://stanfordmlgroup.github.io/competitions/chexpert/](https://link.zhihu.com/?target=https%3A//stanfordmlgroup.github.io/competitions/chexpert/)
(注册一下，就会收到下载链接)

CheXpert论文 (AAAI 2019) 传送门：
[https://arxiv.org/pdf/1901.07031.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1901.07031.pdf)

比赛参加方法传送门：
[https://worksheets.codalab.org/worksheets/0x693b0063ee504702b21f94ffb2d99c6d/](https://link.zhihu.com/?target=https%3A//worksheets.codalab.org/worksheets/0x693b0063ee504702b21f94ffb2d99c6d/)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


