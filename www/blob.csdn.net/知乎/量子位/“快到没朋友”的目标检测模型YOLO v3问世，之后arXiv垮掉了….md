# “快到没朋友”的目标检测模型YOLO v3问世，之后arXiv垮掉了… - 知乎
# 



> 安妮 编译整理
量子位 出品 | 公众号 QbitAI

今天有三件事挺有意思。

一是以“**快到没朋友**”著称的流行目标检测模型YOLO推出全新v3版，新版本又双叒叕提升了精度和速度。在实现相近性能时，YOLOv3比SSD速度提高3倍，比RetinaNet速度提高近4倍。
![](https://pic1.zhimg.com/v2-5e781f695d22a05369a2dc8a61ab1a24_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='553'></svg>)
二是有细心网友发现，模型一作在arXiv上发布研究论文时，脑回路清奇地将自己这篇论文自引自用了一下。
![](https://pic2.zhimg.com/v2-ef7617e2278d020b96b11d5b680b5a79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='933' height='137'></svg>)
三是……在小哥自引自用后没多久，arXiv官方账号宣布服务器由于不明原因挂掉了……
![](https://pic1.zhimg.com/v2-60713790e2843025602528a1dee71464_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='872' height='181'></svg>)
## **更快更强**

先说更新这件正经事~

通过调整YOLO模型中的一些细节，v3模型增大了一些准确率也有所提升，速度依旧非常快。

对于320x320的图像，YOLOv3的检测速度可达22ms，mAP值可达28.2，与SSD的准确率相当但速度快3倍。

当用旧版.5 IOU mAP检测指标时，YOLOv3在英伟达TitanX显卡上51ms达到57.9AP50的性能。相比之下，RetinaNet则用198ms达到57.5AP50的性能，两者性能相近但速度相差近4倍。
![](https://pic2.zhimg.com/v2-0dc14826310c19b8d867c7e832fcd769_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='549' height='326'></svg>)△ 在实现相同准确度情况下，YOLOv3速度明显优于其他检测方法（单一变量实验）![](https://pic2.zhimg.com/v2-8701489165b6e906ceb0a5c343ec6679_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1029' height='732'></svg>)△ 在COCO数据集上不同模型的运行情况对比
## **作者，和他的少女心**

YOLOv3出自华盛顿大学的Joseph Redmon和Ali Farhadi之手。

Ali Farhadi是华盛顿大学的副教授，一作Joseph Redmon是他的博士生，曾在IBM实习，其实还当过电台DJ。Redmon是一个少女心有点爆棚的程序员，这里有一份他的简历，可以自行感受下。
![](https://pic2.zhimg.com/v2-b789ae85d6215b956ffaa7474b02b315_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='577' height='747'></svg>)
处于不知名的原因，小哥有一些“独角兽情结”，可以再次感受下个人网站的画风——
![](https://pic4.zhimg.com/v2-71afa80fc13513ff60b8748f69a34a2f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='396'></svg>)
## **“论文就该实在点”**

如果单单是YOLOv3发布新版本，可能在Reddit上还达不到热度200的水平。有意思在，论文从头到尾都透露着“不太正经”的气息，比如作者自引自用论文，比如这个Introduction的开头——

> 自己今年没怎么做研究，花了很多时间在Twitter上，捣鼓了一下GAN。

没错，这真的是一篇arXiv上的论文。Redmon还在论文中写了写自己尝试但失败了的方法。结尾，也不忘调侃一下热点。

“还有一个更好地问题：‘我们如何使用检测器？’Facebook和Google的很多研究员也在做相关研究啊。我认为，我们至少能知道技术被应用在了有利的方面，并且不会被恶意利用并将它们卖给…等一下，**你说这就是它的用途？？**Oh！”

Reddit上网友的称赞每篇论文都应该这样实在，小哥在Reddit已收获大批粉丝……
![](https://pic2.zhimg.com/v2-e246a1e23ce2f729f458a357f3bf5f99_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='198' height='195'></svg>)
## **相关资料**

对了，对论文有疑问还是不要去@作者了，反正对方也**不会回**，论文中都说了~



![](https://pic3.zhimg.com/v2-02fabe5c84b8802fac975d69acdc2be6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='489' height='26'></svg>)



你可以选择冒险再回看研读一下论文和代码。

论文下载地址：

[https://pjreddie.com/media/files/papers/YOLOv3.pdf](https://link.zhihu.com/?target=https%3A//pjreddie.com/media/files/papers/YOLOv3.pdf)

项目地址：

[https://pjreddie.com/darknet/yolo/](https://link.zhihu.com/?target=https%3A//pjreddie.com/darknet/yolo/)

相关代码：

[https://github.com/pjreddie/darknet](https://link.zhihu.com/?target=https%3A//github.com/pjreddie/darknet)

不过你得**小心**。

毕竟……YOLO模型的全称可是**You Only Look Once**（只能看一眼），再看可能会被吃掉！
![](https://pic1.zhimg.com/v2-8af114dec3dd1e73c1406091a017cd70_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='460' height='302'></svg>)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


