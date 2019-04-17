# 不用P30 Pro，普通手机也能变身望远镜：陈启峰团队新作，登上CVPR 2019 - 知乎
# 



> 花栗子 郭一璞 发自 凹非寺
量子位 报道 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-012649f8f7924ecdd5b1ea40ae226bcc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1037' height='376'></svg>)
华为“单反”**P30 Pro**，从远处拍到的埃菲尔铁塔，连设计师们的名字都看得清。

这样的变焦技能，奢侈到让人心生嫉妒。

不过现在，就算不是P30 Pro，没有徕卡四摄，没有4,000万像素，只靠**深度学习**，**iPhone**也可以把50米之外的细节，拍得清楚明白。

比如，你在街上走着，望到了对面的店铺，墙边立着像梯子一样的东西：
![](https://pic2.zhimg.com/v2-9a57f212305ec5700d2e27229dbe68d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='988' height='810'></svg>)
这时候，如果用普通的**数码变焦**拍一张特写，约等于自暴自弃：
![](https://pic3.zhimg.com/v2-e2087beaf9533bee7a03417519fd0272_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='392' height='382'></svg>)
那么，让超分辨率的大前辈**ESRGAN**试一下。

还是有些力不从心：
![](https://pic1.zhimg.com/v2-5e1307bea352512126e8f2f2d96b64e8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='430' height='452'></svg>)
是时候展现真正的技术了。

主角出场，超进化的数码变焦：
![](https://pic1.zhimg.com/v2-a605f54cabcf5d237b8df78a79d77b58_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='430' height='456'></svg>)
原来它长这样啊。每一道银色的线条，都坚定地横在画面上，没有被黑暗的背景消融掉，细节比前辈生成的清晰许多。

走着走着，又看到了20米以外的小幅海报，上面细小的网址，不用走过去拍也能看清：
![](https://pic3.zhimg.com/v2-5325bc69f18924a60441f59d47dbe7e6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='488'></svg>)
这只眼力上佳的AI，微调一波之后，已经在iPhone X的相机上测试可用。

论文还中了**CVPR 2019**。作者有四位，一作是伯克利的博士生张璇儿，二作是港科大的助理教授陈启峰。

团队说关键在于，**不能只用RGB图像数据来训练**，**要用原始数据**。

## 不一样的数据集

在超分辨率的世界里，有些标准操作，大家已经习以为常。

却没发现，原本常用的数据集，带来了两个严重的问题。
![](https://pic4.zhimg.com/v2-9913a02a72d0d8063bd26b816b0cc7a3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='966' height='566'></svg>)
**第一**，因为神经网络需要成对的图片，一张高分辨率一张低分辨率，来解锁从低清里恢复高清的技能。通常的做法是，对高清图片做**降采样** (Downsampling) ，得到对应的那张低清图片。

降采样，会间接减少图像的噪点。

而超分辨率通常是要把远处的物体放大，离镜头越远的物体，它所在的区域噪点会越多，因为进入光圈的光子少了。

这样说来，用降采样炮制的数据，不太适合拿来训练超分辨率的技能。

**第二**，现有的大多数方法，都是直接用8位RGB图像当训练数据的。
![](https://pic2.zhimg.com/v2-b8b901f4764dfeef6a93f48cb76d3345_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='468' height='352'></svg>)
但RGB图像，不是摄像头的原始数据 (Raw Sensor Data) ，是图像信号处理器 (ISP) 加工过的。

这加工步骤会损失一些**高频信号**，其中一个目的也是降噪。

相比之下，原始数据 (12-14位) 保留了这些高频信号，可能对恢复图像质量有帮助：
![](https://pic1.zhimg.com/v2-fa15485e110f57e5c726eec9be32c374_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='342'></svg>)
总结一下，一要抛弃降采样，二要用原始数据来训练。于是，研究人员拿起了单反，用光学变焦镜头去拍成对的照片。

简单来说，短焦拍下低分辨率图像，长焦拍下高分辨率图像。

复杂一点说，24-240毫米的变焦镜头，可以拍出许多种不同焦距的照片。对画面上的一个物体来说，就是**不同的分辨率**了：
![](https://pic4.zhimg.com/v2-b1252261b763ced3baecd9af1136a46b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='772' height='756'></svg>)
任意两张图像，只要分辨率不同，都可以组成一对。

**SR-RAW数据集**就这样诞生了。每张图像都是原味，低清不是由高清粗暴转化得来，原始数据也都在。

训练的时候，是用低清照片的原始数据，加上高清照片的8位RGB图：高清是低清的Ground Truth。

那么，是不是有了数据集，就得到了天下？

还差一点：

## 图像对不齐怎么办

训练超分辨率，首先需要**图像对齐** (Alignment) 。就是把低清图像的每一个像素点，和高清图像里的像素点对应起来。

因为高清和低清图像，是在同一枚镜头的不同配置下拍摄的，对齐的时候会出现不可避免的问题。

比如，**透视问题**。调了焦，物体之间的距离变了，很难对齐了：
![](https://pic1.zhimg.com/v2-9ace196145f0187eff6516817963de80_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='718' height='309'></svg>)
比如，**景深问题**。调了焦，物体和背景之间的距离也变了，更难对齐了：
![](https://pic4.zhimg.com/v2-6074bb1794ab7e17b7bef19bf3cdfb8b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='772' height='289'></svg>)
另外，高清图像里面，物体边缘会更加锋利，而低清图像里的边界比较模糊，原本就很难对齐。

于是，团队提出了一种新的损失函数，叫做**CoBi**。

这个损失，是在去年发表的**Contextual Loss** (简称CX) 基础上进化而生。

CX可以解决图像不对齐的问题，却不考虑图像的空间特征，在执行超分辨率任务的时候会出现重大的瑕疵 (下图B) ：
![](https://pic1.zhimg.com/v2-e9cfd616fd1dece15f69ebf980ac96b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='970' height='530'></svg>)△ 左起：低清输入、CX训练成果![](https://pic1.zhimg.com/v2-3cce17f8672c094ac3d1ded7f85e04f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='954' height='526'></svg>)△ 左起：CoBi训练成果、Ground Truth
所以，CoBi比CX多加了一项有关空间坐标的损失。这样，便可以训练出优秀的超分辨率网络了。

## 原始数据，真的更有效

训练完成之后，要和其他网络比一场。那么，先用肉眼评判一下。

一道题目是，限时段停车的指示牌：
![](https://pic4.zhimg.com/v2-7ee85a44b9c9b5f8ede5545f7da152a7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='986' height='1012'></svg>)△ 左下是低清输入，右下是Ground Truth
比赛结果是 (看不清可以把手机横过来) ：
![](https://pic3.zhimg.com/v2-2c836f8e76aa574cda70a0e17a29da2a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='372'></svg>)△ 右一为主角
主角恢复的文字，比前辈们都要清晰一筹。

如果，你感觉它和ESRGAN的表现差不多，我们来单独对比一下：
![](https://pic1.zhimg.com/v2-4495bd5f0c180eb4525426b57ba274f8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='476' height='364'></svg>)△ 左为ESRGAN，右为主角
优势还是可见的。

数据，也支持了肉眼的判断：
![](https://pic3.zhimg.com/v2-cdd168cc8f539fdb53924606d3170cce_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='383'></svg>)
不论4倍还是8倍变焦，主角的各项指标，都要明显优于其他选手。

上面的数据表里，**SSIM**是结构相似性，**PSNR**是峰值信噪比，都是和Ground Truth相比，越大越好。

**LPIPS**是一个新近提出的指标，是用一个预训练的网络，来测量图像之间的感知相似度 (Perceptual Image Similarity) ，越小越好。
![](https://pic2.zhimg.com/v2-a2a8604708e032c96a837095f8b767a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='247'></svg>)
打败了对手之后，团队又用人工合成的传感器数据 (C) ，以及8位RGB图像 (B) ，分别训练了一下自家的模型，看**原始数据** (D) 训练的模型是不是真比它们更优秀。

其中一道题目，是40米之外的马里奥：
![](https://pic2.zhimg.com/v2-144e273d5437fbb5a4131963b0695ca5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1052' height='770'></svg>)
左是用合成数据训练的，右是用原始数据训练的：
![](https://pic1.zhimg.com/v2-7b0610166d3f88b234c7c842f4799268_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1056' height='606'></svg>)
原始数据的训练之下，画质更加美好。

对比一下，这是Ground Truth (来自光学变焦镜头，就是单反) ：
![](https://pic2.zhimg.com/v2-d2486d92cffe1c0b0f78aab851817f31_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='504' height='590'></svg>)
各项指标也认为，投喂原始数据的模型表现更出色：
![](https://pic1.zhimg.com/v2-59a4e9cecbf97c93dea49711e2f95b20_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='250'></svg>)△ Ours-syn-raw=合成数据训练后，Ours=原始数据训练后
不过，合成数据上训练的成果，虽然比不上用原始数据养成的模型，但还是比直接用RGB图像训练的模型，要争气一些。

终究，还是要用原始数据训练的好。

结论令人振奋，但还不是结局。研究人员希望这只用单反传感器养成的AI，也能适应**手机的传感器**。

于是，他们选了**iPhone X**做小白鼠，用一个小数据集微调了一番。5000次迭代之后，模型在手机上获得了新生：
![](https://pic1.zhimg.com/v2-1e454759f459861078c763e29642f294_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='397'></svg>)
## 实验室里的人类们

研究团队，来自伯克利、港科大和英特尔。
![](https://pic2.zhimg.com/v2-f3c5dcff42e1d5b7b9058e45a471b681_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1620'></svg>)
一作是来自伯克利的张璇儿，高中毕业自人大附中，本科毕业于莱斯大学，如今已是四年级的博士生。

主攻计算机视觉的她，先后在Adobe、Facebook和英特尔实习过。张璇儿一作且有陈启峰参与的CVPR论文，已经不止一篇了。
![](https://pic2.zhimg.com/v2-480ce2e7974ce6acd4a9a36601333e01_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='521'></svg>)
比如，这项画风绮丽的研究成果，可以去除照片上的“鬼影”。

二作，大家可能比较熟悉了。
![](https://pic4.zhimg.com/v2-11b153b3ca914d23f5a9788cc8bea863_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='323' height='323'></svg>)
陈启峰，1989年生，高中获得IOI金牌，本科就读于香港科技大学。在密大交换期间获得过全球ACM总决赛的亚军 (北美第一) 。

2012年，陈启峰获得了包括斯坦福、哈佛、MIT在内9所学校的PhD录取信，最终选择了斯坦福。

2017年博士毕业，2018年便成了港科大的助理教授。2018年他获得Google Faculty Research Award，被麻省理工评论选为中国35岁以下科技创新35人。
![](https://pic4.zhimg.com/v2-67c6aa38c9dcc7ce1a54f77fb980e58b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1512'></svg>)
三作吴义仁 (Ren Ng) ，伯克利的助理教授，张璇儿便是在他的实验室里做研究。

吴教授出生在马来西亚，8岁移民澳大利亚。2006年斯坦福大学博士毕业，还获得了当年ACM的博士论文奖。

他的主要研究方向是数字光场摄影技术，还创办了光场相机公司Lytro，把这项技术引入消费级电子产品。
![](https://pic4.zhimg.com/v2-63cb141422b78bac2d25253992625557_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='199' height='260'></svg>)
四作是Vladlen Koltun，来自英特尔的资深科研主管，是智能系统实验室Intelligent Systems Lab的负责人。张璇儿和陈启峰，都曾经在他的实验室里工作过。

### One More Thing

要是你既买不起P30 Pro，又不会训练深度学习模型，别担心，还可以直接上望远镜。

贴心的量子位，替你找到了这个：
![](https://pic3.zhimg.com/v2-a3851810f4d5c7fb943a4ab1300386ae_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='776' height='655'></svg>)
咦，你怎么用了品如的望远镜？

论文传送门：
[https://cqf.io/papers/Zoom_To_Learn_CVPR2019.pdf](https://link.zhihu.com/?target=https%3A//cqf.io/papers/Zoom_To_Learn_CVPR2019.pdf)


— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


