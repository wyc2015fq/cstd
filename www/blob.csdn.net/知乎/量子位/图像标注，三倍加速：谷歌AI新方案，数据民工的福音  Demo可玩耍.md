# 图像标注，三倍加速：谷歌AI新方案，数据民工的福音 | Demo可玩耍 - 知乎
# 



> 膘栗子 发自 凹非寺
量子位 报道 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-c31d917ea87cb61c958d37cd8644c3af_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='275'></svg>)
标注数据，可能是AI模型训练里最艰巨的一个部分了。

**计算机视觉**首当其冲，标注图像需要投入大量人力。

比如，在Coco-Stuff数据集里，标好一幅图需要**19分钟**，164,000幅就超过**5万小时**了。

于是，谷歌AI团队开发了一种新的方案，官方表示可以**3倍提升**标注速度。
![](https://pic3.zhimg.com/v2-666bb49469d867ab5b5a2f6f1ca442a2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='600'></svg>)
方法名叫**Fluid Annotation** (流体标注) ，借助**机器学习**的力量来贴**类别标签** (Class Labels) 、勾出每个物体的**轮廓**和图片里的**背景**，为人类减轻压力。

谷歌AI把这项研究称作“尝试加速数据标注的第一步”，也即将在ACM MM 2018大会上演示这一步。

## **怎么用呢？**

这个方法，从预训练的语义分割模型**Mask R-CNN**的输出开始，生成大约1,000个图像片段 (Image Segments) ，每个片段有自己的**类别标签**和**置信分**。

(每个像素都要被划入一个类别，比如花、树、人、路等等。)

置信分最高的那些片段，就可以传到人类那里进一步做标注。
![](https://pic1.zhimg.com/v2-13710dbdc281f5eab080f6bd7a6e00d4_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='592' height='396'></svg>)
人类，可以在一个操作板上**修改**机器分割的结果。

比如，修改某个片段现有的标签，例如把**坦克**改成**汽车**。

比如，加入一个片段，来弥补缺失的物体，也可以删除片段：
![](https://pic3.zhimg.com/v2-f7ad466490a5356dea3cfb09302f4fc6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='332'></svg>)
再有，可以调整重叠片段的**深浅顺序**，上移这个片段，下移另一个片段：
![](https://pic3.zhimg.com/v2-9c3ee2f4205830774e8b70c4001ecf72_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='332'></svg>)
团队提供了一个demo，里面有五个样本，大家可以自己去调一调看 (传送门在文底) 。

## **效果能超过纯人类么？**

现在，把流体标注 (人机协作) 的效果，和人类辛勤劳作的结果对比一下。
![](https://pic3.zhimg.com/v2-fb52523725960edcc88264dcf83a051e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='513'></svg>)△ 左为原图，中为人类标注，右为流体标注
肉眼看去，AI标注的效果已经和人类比较接近了。

谷歌团队在博客里表示，人类标注在**物体边界**的把握上依然略胜一筹。

所以，下一步的研究重点，就是加强边界的标注。

当然，速度也要继续提升啦。

Demo传送门：
[https://fluidann.appspot.com/](https://link.zhihu.com/?target=https%3A//fluidann.appspot.com/)

论文传送门：
[https://arxiv.org/pdf/1806.07527.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1806.07527.pdf)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


