# 微软发布1600万台Windows设备运行数据，设17万奖金征集恶意软件预测算法 - 知乎
# 



> 夏乙 发自 凹非寺
量子位 出品 | 公众号 QbitAI




一个“史无前例”的恶意软件感染数据集亮相了。

这个数据集来自微软，毕竟，他们的重要产品Windows，大概是全球遭受病虫害最频繁的软件了。

同时推出的，还有一场Kaggle竞赛：让你训练机器学习算法，来预测一台电脑是不是感染了恶意软件，俗称“中毒了没”。

我们先来看看这份**数据集**究竟有多“史无前例”。

它包含的信息相当全面，共有60多项。

每一行数据都对应着一个MachineIdentifier，相当于**设备ID**，也都包含一个代表着**真实值的标签**HasDetections，显示这台设备有没有感染恶意软件。

同时，还包含大量可能能用来判断中没中毒的信息，比如设备的运行状况指标，以及本身软硬件环境，比如：系统、防火墙版本、装了几个杀毒软件、默认浏览器是哪一款、是个电脑还是个手机、显示器分辨率多少、处理器几核什么架构、内存多大、用的是不是固态硬盘……

甚至还包含这台机器是不是用来打游戏的、是不是触摸屏、带不带手写笔，以及代表国家和城市的数字代码等等。

这样的数据一共有多少条呢？答案是，在**训练集里有近900万条**，**测试集里近800万条**。也就是说，微软提供了来自1600万台设备的信息，给你训练算法。

如果你对这么大的数字不敏感……我们换个衡量大小的方式。这个数据集以csv形态存储，一共**8.47GB**。

这些数据，都来自Windows用户，由Windows系统本身记录和默认防火墙Windows Defender采集而来。当然，数据经过清洗，并不包含隐私信息。



![](https://pic2.zhimg.com/v2-18109f56eb38ff170f057fda24627199_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='719'></svg>)



庞大的数据集准备就绪，办**竞赛**也顺理成章。于是，也就有了Kaggle上的**微软恶意软件预测竞赛**。

这个竞赛，由微软、美国东北大学、佐治亚理工大学联合主办。

参赛者需要做的，就是训练一个算法，为测试集里的每一台电脑，预测一个感染恶意软件的概率，范围在[0, 1]区间。

竞赛设置了2.5万美元（大约17万元）的**奖金池**。其中，第一名奖金1.2万美元，第二名7000美元，第三四五名分别是3000、2000、1000美元。



![](https://pic2.zhimg.com/v2-20a62ab2edb1818bd62299c4078a850d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='885'></svg>)



这个奖金设置，对安全界人士来说可能太低了点。英特尔安全研究员Marco Figueroa就在Twitter上说：



![](https://pic2.zhimg.com/v2-6239d9b249ac1c5311c2566b62b37f99_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='753'></svg>)



> 微软你是认真的吗？一个恶意软件预测竞赛才给2.5万美元奖金？应该至少30万美元起！

不过，参赛者们依然很热情。

截至量子位发稿时，已经有551支队伍参加了。

现在距离竞赛结束，还有两个多月，新队伍还在源源不断地涌进来。昨天才杀入战局的南京大学小哥哥（队名叫“杀”的那位），一次提交就排到了榜首。



![](https://pic3.zhimg.com/v2-32cd770797349cb0f21952e507fafa7a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='514'></svg>)



感兴趣嘛？地址在此：

[Microsoft Malware Prediction | Kaggle](https://link.zhihu.com/?target=https%3A//www.kaggle.com/c/microsoft-malware-prediction/)




— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。




