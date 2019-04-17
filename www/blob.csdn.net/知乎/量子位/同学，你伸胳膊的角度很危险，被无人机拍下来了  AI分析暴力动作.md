# 同学，你伸胳膊的角度很危险，被无人机拍下来了 | AI分析暴力动作 - 知乎
# 



> 圆栗子 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-7bace1bf6275e9d2caf3cb911c57b6d5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='214'></svg>)
**天空之眼** (*Sky In the Eye*) ，不只是斯内普教授的遗作，也是一组研究人员为自己的新论文，选定的题目。

这项研究里，无人机并不是要轰炸恐怖分子，而是用于实时监测公共场所可能出现的**暴力行为**。
![](https://pic4.zhimg.com/v2-b38b3b8dd79911717ed2851e3021aca7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='580' height='362'></svg>)
团队选择的无人机是**Parrot AR**，并不是太厉害的装备。


不过，这暂时不重要，因为无人机只要搭载摄像头和**无线网络**就好。

影像数据实时传输出来，隐蔽在大后方的计算设备就可以分析无人机所在环境里，是不是存在不安定的躯体了。

## **你胳膊的朝向很危险**
![](https://pic3.zhimg.com/v2-1aca9e02460de106459f62b36534762e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='338' height='304'></svg>)
AI要检测的暴力动作，一共有**5种**：扼杀、拳打、踢踹、枪击，以及刺伤。

一群志愿者**假装互相伤害**的视频，会告诉AI这些动作都是什么样子。
![](https://pic4.zhimg.com/v2-5ecdac14f80539abe310b962103c68e3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='737' height='503'></svg>)
系统先用**FPN **(特征金字塔网络) 来检测，视频里出现的所有人类。标出头、上肢、下肢三个部分的**14**个重要位点，画出人体的骨架。

然后，**SHDL**网络可以分析**四肢朝向**等数据，判断那些人是不是在使用暴力。



![](https://pic2.zhimg.com/v2-989923721ca0d9d16bc7667b309f4dad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1050' height='423'></svg>)
## **你们人多，AI怕了**

当视频里只有一个人姿势暴力，AI识别暴力行为的准确率高达**94%**，令人鼓舞。

然而，随着画面中暴力人数的增加，准确率开始节节败退。
![](https://pic2.zhimg.com/v2-e286d665a9f4b329b38238cf733a7135_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='474' height='92'></svg>)
当同框的不安定个体增加到10个，准确率只剩**79%**。

研究人员说，这可能是因为人多的时候，FPN无法准确找出所有人，或是SHDL的姿势判断难度更大了。
![](https://pic1.zhimg.com/v2-192b60bda61dc1e1d6fdf06a50638d78_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='408' height='230'></svg>)
不过，论文并没有提到“谎报军情”的数据，并不知道有没有**正常**的亲昵行为被误认成了暴力。

## **群演可能太浮夸**

研究使用的**AVI数据集** (Aerial Violent Individual Dataset) ，包含**2,000**幅加了注释的图像，一共有**5124**个人类参与到了假装暴力的活动中。

这些图像的拍摄条件各不相同，无人机所在的**位置**、**光照**情况以及**模糊**程度等等皆有变化。
![](https://pic3.zhimg.com/v2-375d7b38be4321651a000c2b2c6165b6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='445'></svg>)△ 有点模糊的，阳光灿烂的日子
即便是这样，志愿者动作**夸张** (但不会碰到对方) 的借位表演，还是和现实有些差距的。

喧嚣的世界里，拍摄的图像可能更模糊，出现在画面里的人可能更多，行为也更复杂，就像研究结果显示的那样——系统更容易出现误判。

于是，研究人员打算让无人机带AI**实战**一圈。
![](https://pic3.zhimg.com/v2-0ec5fd63ea92aa6e4b827a4de065e912_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='300'></svg>)
过不了多久，在印度理工学院的瓦朗加尔校区，Technozion和Spring Spree两场熙熙攘攘的大型活动上，就能看到几个小哥哥测试无人机的身影。

## **还好挖鼻孔不是很暴力**

即便技术还没有做好落地的准备，许多研究团队已经在监测动作的路上走了许久。

Facebook开发的**DensePose**系统，也是用来估计人体姿势，还晋级了今年CVPR的Oral环节。
![](https://pic4.zhimg.com/v2-5cba41678c00d4dc29d7c72a7e5af127_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='408' height='230'></svg>)
毕竟，这是一个很容易获得无人机和无线网络的年代，监控系统里也有了更多等待挖掘的宝藏。

然而，人类还是不免担心，当**自动化监控**的大网张开，一举一动都无所遁形，丧失了自在的生活空间。

如果有一天，就算在空旷的大街上，也不能放心地挖鼻孔了，可能真的会有点心累吧。
![](https://pic4.zhimg.com/v2-8e8207fe77e35f57becde65a8affe72b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='566'></svg>)
论文传送门：
[https://arxiv.org/pdf/1806.00746.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1806.00746.pdf)

DensePose论文传送门
[https://arxiv.org/pdf/1802.00434.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1802.00434.pdf)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


