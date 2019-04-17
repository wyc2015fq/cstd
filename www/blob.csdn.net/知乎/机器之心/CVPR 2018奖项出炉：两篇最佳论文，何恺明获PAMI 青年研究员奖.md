# CVPR 2018奖项出炉：两篇最佳论文，何恺明获PAMI 青年研究员奖 - 知乎
# 



> 昨日，一年一度的计算机视觉顶级会议 CVPR 2018 在美国盐湖城拉开帷幕，排起长龙的注册队伍让现场参会的机器之心分析师看到了这届 CVPR 的火热。据统计，本届大会有超过 3300 篇的大会论文投稿，录取 979 篇（接受率约为 29%，其中包括 70 篇 Oral 和 224 篇 Spotlight 论文）。

而在一个小时前，最受关注的 CVPR 2018 最佳论文结果揭晓：来自斯坦福大学和 UC Berkeley 的 Amir R. Zamir 等人获得 CVPR2018 的最佳论文（Best Paper Award）；来自卡内基梅隆大学的 Hanbyul Joo 等人获得了本届大会最佳学生论文奖（Best Student Paper Award）。此外，Tübingen 大学教授 Andreas Geiger 与 FAIR 研究科学家何恺明获得本届大会的 PAMI 年轻学者奖。

在 19 号上午的主会议上，CVPR 方面先介绍了今年大会的数据信息，除了论文接收量再次超越往年，今年的注册参会人员也达到了 6512 位。
![](https://pic1.zhimg.com/v2-12a2678fe155af754710b54b029798a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='980'></svg>)
此外，论文评审人员的数量在今年也达到了新高，2385 名 reviewer，而 2014 年 CVPR 的参会人员也才 2000 人左右。
![](https://pic3.zhimg.com/v2-0f6b588bf0971f70531a0ceb3e61a916_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='924'></svg>)
介绍完大会数据方面的信息，接下来就是揭晓最佳论文得主了：




**最佳论文**

CVPR2018 的最佳论文（Best Paper Award）由斯坦福大学和 UC Berkeley 的 Amir R. Zamir 等人获得。他们提出了一个映射视觉任务空间的框架，通过计算不同任务的相关性来确定迁移学习方案，从而可以在保持准确率的同时，大大降低对数据、监督信息和计算资源的需求。




**最佳论文：Taskonomy: Disentangling Task Transfer Learning**
![](https://pic1.zhimg.com/v2-b4aaa4a1296e9a9d8de99bda30427ab8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='252'></svg>)
论文链接：[http://taskonomy.stanford.edu/taskonomy_CVPR2018.pdf](https://link.zhihu.com/?target=http%3A//taskonomy.stanford.edu/taskonomy_CVPR2018.pdf)




视觉任务之间是否相关？例如，能否使用曲面法线简化估计图像深度的过程？直觉上对这些问题的正面回答暗示着在各种视觉任务之间存在一种结构。了解这种结构具有巨大的价值；它是迁移学习背后的概念，且可为识别任务间的冗余提供了一种有理可依的方法，比如，为了在相关任务间无缝地重复使用监督或在一个系统中解决多个任务而不增加复杂度。

我们提出了一种完全计算的方法来建模视觉任务的空间结构，通过在一个位于隐空间内的 26 个二维、2.5 维、三维和语义任务中寻找（一阶或更高阶）迁移学习依赖关系来实现。其成品是用于任务迁移学习的计算分类图。我们研究了这个结构的成果，比如出现的非平凡相关关系，并利用它们减少对标注数据的需求。比如，我们展示了解决一组 10 个任务所需的标注数据点总数可以减少约 2/3（与独立训练相比），同时保持性能几乎一致。我们提供了一套计算和探测这种分类结构的工具，包括一个求解器，用户可以用它来为其用例设计有效的监督策略。

该项目也已开源并放出官方的 API 和 Demo：[http://taskonomy.stanford.edu/](https://link.zhihu.com/?target=http%3A//taskonomy.stanford.edu/)
[CVPR2018最佳_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/t1342w29ent.html)
会后，该论文的第一作者、斯坦福大学和加州大学伯克利分校的博士后研究员 Amir Roshan Zamir 告诉机器之心记者，「这篇论文主要围绕了当前机器学习系统最大的缺点，也就是需要很多标注数据才能完成它们想要的结果。人类是可以借鉴学到的不同技能，也就是迁移学习，你在第一盘棋中学到的技能可以用到第 10、第 100 盘棋，这篇论文的重点就是将这个概念带去机器学习、尤其是在感知（perception）这块儿。基本上你用你之前学的技能解决新的问题。」

该论文的共同二作，斯坦福大学计算机科学系的现本科生/准博士生沈博魁告诉记者，他们的目标是找到不同任务之间的关联，这些任务比如说是深度感知（depth-perception）、surface normal estimation 之类的，发现这些关联之后就能用更少的数据来解决更多的任务。比如用边缘检测和 surface normal estimation 就能更好地学深度感知。




**最佳学生论文**
![](https://pic2.zhimg.com/v2-0c9d8f25975018c3657eb4a23efa3501_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='293'></svg>)
本届大会的最佳学生论文（Best Student Paper Award）由来自卡耐基梅隆大学（CMU）的 Hanbyul Joo、Tomas Simon、Yaser Sheikh 获得。该研究提出了一种 3D 人体模型，不仅可以追踪身体的姿势，还可以追踪人脸表情和手势等微小动作。




**最佳学生论文：Total Capture: A 3D Deformation Model for Tracking Faces, Hands, and Bodies**
![](https://pic3.zhimg.com/v2-c7398d08748ef1dab59e7ad8d99b2cee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='212'></svg>)
论文链接：[http://www.cs.cmu.edu/~hanbyulj/totalbody/totalcapture.pdf](https://link.zhihu.com/?target=http%3A//www.cs.cmu.edu/~hanbyulj/totalbody/totalcapture.pdf)

摘要：我们展示了一种用于无标记捕捉多尺度人类动作（包括人脸表情、身体动作和手势）的统一变形模型（deformation model）。初始模型通过将人体各部位的模型进行局部组合来形成，我们称之为「弗兰肯斯坦」模型。该模型能够通过单个无缝模型完整表达人体部位的动作，包括人脸和手的动作。通过对穿着日常服饰的人们进行大规模数据收集，我们优化弗兰肯斯坦模型以创建「亚当」（Adam）。亚当是校准模型，它与初始模型具备同样的基础层级，但是它可以表现头发和衣服的几何形状，使之可直接用于查看衣服和人是否相配，就像真的在日常生活中穿了这些衣服一样。最后，我们展示了这些模型可用于整体动作追踪，同时捕捉一组人的大规模身体动作和细微的面部和手部动作。

作为计算机视觉的研究，最佳学生论文也有自己的展示页：[http://www.cs.cmu.edu/~hanbyulj/totalcapture/](https://link.zhihu.com/?target=http%3A//www.cs.cmu.edu/~hanbyulj/totalcapture/)




**荣誉提名奖（Honorable Mention）**
- 论文：Deep Learning of Graph Matching
- 作者：Andrei Zanfir、Cristian Sminchisescu
- 论文链接：[http://openaccess.thecvf.com//content_cvpr_2018/papers/Zanfir_Deep_Learning_of_CVPR_2018_paper.pdf](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com//content_cvpr_2018/papers/Zanfir_Deep_Learning_of_CVPR_2018_paper.pdf)



- 论文：SPLATNet: Sparse Lattice Networks for Point Cloud Processing
- 作者：Hang Su、Varun Jampani、Deqing Sun、Subhransu Maji、Evangelos Kalogerakis、Ming-Hsuan Yang、Jan Kautz
- 论文链接：[https://arxiv.org/abs/1802.08275](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1802.08275)



- 论文：CodeSLAM - Learning a Compact, Optimisable Representation for Dense Visual SLAM
- 作者：Michael Bloesch、Jan Czarnowski、Ronald Clark、Stefan Leutenegger、Andrew J. Davison
- 论文链接：[https://arxiv.org/abs/1804.00874](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.00874)



- 论文：Efficient Optimization for Rank-based Loss Functions
- 作者：Pritish Mohapatra、Michal Rol´ınek、C.V. Jawahar、Vladimir Kolmogorov、M. Pawan Kumar
- 论文链接：[https://arxiv.org/abs/1604.08269](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1604.08269)

除了以上获奖论文，大会还公布了今年的 PAMI Longuet-Higgins Prize 与 PAMI Young Researcher Award。前者是颁发给至少 10 年前发布的论文，而后者是颁发给 7 年内获得博士学位且早期研究极为有潜力的研究人员。今年 Longuet-Higgins Prize 的得主为 CVPR 2008 年的论文《A discriminatively trained, multiscale, deformable part model》；PAMI Young Researcher Award 的得主为 Andreas Geiger 与何恺明。
![](https://pic3.zhimg.com/v2-35caf793d028cf9eb9b1bc9af926c132_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
Andreas Geiger 是德国蒂宾根大学计算机科学系的教授，自动驾驶视觉组成员。今年他们有四篇论文被 CVPR 2018 接收。而另一位获奖者何恺明大家都比较熟悉，他于 2016 年 8 月加入 FAIR（Facebook AI Research），担任研究科学家。此前，他曾以第一作者身份拿过两次 CVPR 最佳论文奖（2009 和 2016）。

最后值得一提的是，恰逢世界杯期间，CVPR 2018 提供世界杯直播。所以，参会者们是看着论文，累了就停下来看会儿世界杯吗？
[cvpr 2018_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/f1342uqeprm.html)![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)



