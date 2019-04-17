# CVPR不堪重负一度挂掉，最受关注的CV顶会都说了什么？ - 知乎
# 



> 李林 发自 凹非寺
量子位 出品 | 公众号 QbitAI



![](https://pic4.zhimg.com/v2-84adb52303efb2eba9f2dbc6a790cf53_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='241'></svg>)
CVPR 2018会议官网，今天早上挂掉了。

原因很简单，访问人数太多，服务器不堪重负。

主办方计算机视觉基金会（CVF）毕竟不是刚刚经历了618的我国电商公司，大概没什么应对这种高并发的经验。

北京时间6月20日13点多量子位再试的时候，已经好了。
[CVPR2018​cvpr2018.thecvf.com](https://link.zhihu.com/?target=http%3A//cvpr2018.thecvf.com/)
## **关注度究竟有多高？**

究竟是多少人同时访问，把人家官网搞瘫痪了。这个恐怕很难知道。

量子位知道的，只有CVPR 2018大会上公布的一组数据：
![](https://pic2.zhimg.com/v2-8edd339f1e2a9a063ef93b4b5d9635cd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='496'></svg>)
这次大会，收到有效投稿3309篇；

接收论文979篇，其中294篇Oral；

赞助商和参展商149家，赞助和展位收入超200万美元；

注册参会人数6512人。

## **最佳论文究竟讲了什么？**

虽说官网没了，顶会要点还是得关注的，比如：

昨天深夜，CVPR 2018颁出了一系列奖项。

获得**最佳论文奖**（Best Paper Award）的，是斯坦福和加州大学伯克利分校的Amir R. Zamir、Alexander Sax、William Shen、Leonidas J. Guibas、Jitendra Malik、Silvio Savarese。

他们共同完成的论文题为：

**Taskonomy: Disentangling Task Transfer Learning**
![](https://pic3.zhimg.com/v2-8f5965cd6bd98290d533e6450ddf72c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='427'></svg>)△ 颁奖仪式现场。图源：李飞飞的twitter
这篇论文研究的是各种计算机视觉任务在迁移学习中的依存关系，提出了一个感知任务迁移学习的计算分类地图（computational taxonomic map），能够根据各种任务的相关性，来决定迁移学习方案。对于一组10种任务，他们的模型能在保持性能几乎不变的情况下，将对标注数据的需求降低2/3。
![](https://pic4.zhimg.com/v2-bb3e188551547a79904298ad639f4e63_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='232'></svg>)
现在，Taskonomy不仅有论文，迁移学习API、Demo、预训练模型、所用数据集都已经开放出来。可以到官方主页下载：

主页：[Taskonomy | Stanford](https://link.zhihu.com/?target=http%3A//taskonomy.stanford.edu/)

PDF：[http://openaccess.thecvf.com/content_cvpr_2018/papers/Zamir_Taskonomy_Disentangling_Task_CVPR_2018_paper.pdf](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/content_cvpr_2018/papers/Zamir_Taskonomy_Disentangling_Task_CVPR_2018_paper.pdf)

获得**最佳学生论文奖**（Best Student Paper Award）的，是卡耐基梅隆大学和Facebook Reality Labs的Hanbyul Joo、Tomas Simon和Yaser Sheikh。

他们合作的论文题为：

**Total Capture: A 3D Deformation Model for Tracking Faces, Hands, and Bodies**

Facebook Reality Labs，也就是原来的Oculus Research，与CMU同在匹兹堡，今年5月刚刚改了这个新名字。他们的这项研究也和虚拟现实有关。

这篇论文提出的“统一变形模型”，能够无标记捕捉不同尺度上的人类动作，包括脸部表情、身体动作、手势等等。

主页：[Total Capture](https://link.zhihu.com/?target=http%3A//www.cs.cmu.edu/~hanbyulj/totalcapture/)

PDF：[http://openaccess.thecvf.com/content_cvpr_2018/papers/Joo_Total_Capture_A_CVPR_2018_paper.pdf](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/content_cvpr_2018/papers/Joo_Total_Capture_A_CVPR_2018_paper.pdf)

除此之外，最佳论文照例附赠了几个**荣誉提名**奖（honourable mentions）：

**Deep Learning of Graph Matchin**

作者：Andrei Zanfir, Cristian Sminchisescu（瑞典隆的大学、罗马尼亚科学院数学所）

PDF：[http://openaccess.thecvf.com/content_cvpr_2018/papers/Zanfir_Deep_Learning_of_CVPR_2018_paper.pdf](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/content_cvpr_2018/papers/Zanfir_Deep_Learning_of_CVPR_2018_paper.pdf)

**SPLATNet: Sparse Lattice Networks for Point Cloud Processing**

作者：Hang Su, Varun Jampani, Deqing Sun, Subhransu Maji, Evangelos Kalogerakis, Ming-Hsuan Yang, Jan Kautz（马萨诸塞大学阿姆赫斯特校区、英伟达、加州大学默塞德分校）

PDF：[http://openaccess.thecvf.com/content_cvpr_2018/papers/Su_SPLATNet_Sparse_Lattice_CVPR_2018_paper.pdf](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/content_cvpr_2018/papers/Su_SPLATNet_Sparse_Lattice_CVPR_2018_paper.pdf)

**CodeSLAM — Learning a Compact, Optimisable Representation for Dense Visual SLAM**

作者：Michael Bloesch, Jan Czarnowski, Ronald Clark, Stefan Leutenegger, Andrew J. Davison（帝国理工学院）

主页：[CodeSLAM](https://link.zhihu.com/?target=http%3A//www.imperial.ac.uk/dyson-robotics-lab/projects/codeslam/)

PDF：[http://openaccess.thecvf.com/content_cvpr_2018/papers/Bloesch_CodeSLAM_--_Learning_CVPR_2018_paper.pdf](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/content_cvpr_2018/papers/Bloesch_CodeSLAM_--_Learning_CVPR_2018_paper.pdf)

**Efficient Optimization for Rank-Based Loss Functions**

Pritish Mohapatra, Michal Rolínek, C.V. Jawahar, Vladimir Kolmogorov, M. Pawan Kumar（印度海得拉巴信息技术学院、德国马普所、哈佛大学、IST Austria）

PDF：[http://openaccess.thecvf.com/content_cvpr_2018/papers/Mohapatra_Efficient_Optimization_for_CVPR_2018_paper.pdf](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/content_cvpr_2018/papers/Mohapatra_Efficient_Optimization_for_CVPR_2018_paper.pdf)

同时，CVPR上还颁发了“**十年检验奖**”PAMI Longuet-Higgins Prize，获奖的是2008年的论文：

**A Discriminatively Trained, Multiscale, Deformable Part Model**

作者：P. Felzenszwalb, D. McAllester, D. Ramanan

## **人生赢家**

当然，除了这些论文奖项之外，还有一个针对青年才俊个人的奖项：PAMI Young Researcher Award，奖励的是获得博士学位不足7年、早期科研生涯做出杰出贡献的人。

今年的得主，一位是Facebook的何恺明。

这个奖项去年的得主，是经常和他合作的Facebook研究员：Ross Girshick（RBG）。

关于他们合作的经历和研究成果，可以阅读量子位在[何恺明斩获ICCV 2017最佳论文](https://link.zhihu.com/?target=https%3A//www.toutiao.com/i6480509571273064974/)时的介绍。

另一位是Andreas Geiger，德国图宾根大学教授、马克思普朗克信息系统研究所自动化视觉组负责人，创造了KITTI数据集的主力就是他。

## **To：想成为会场焦点的朋友**

如果你中了oral，或者被邀请去做个讲座，那，量子位什么建议也不用给。

对于没中oral和spotlight、甚至什么都没中的朋友来说，在人海茫茫的会场，该如何脱颖而出？

加拿大约克大学研究生Matthew Tesfaldet做出了示范：
![](https://pic3.zhimg.com/v2-75eb3495bfbd7a6de9823213d0d6f0a6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='853'></svg>)![](https://pic4.zhimg.com/v2-2d9c52ad43cddbc86dc593c028b9e513_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='853'></svg>)
T恤果然是coder的终极奥义！

把poster穿在身上，其他一切都不重要了！同组几位作者还可以印一套！

## **To：爱学习的朋友**

好消息是，论文的open access一直健在，望爱学习的朋友们不要担心：

[http://openaccess.thecvf.com/CVPR2018.py](https://link.zhihu.com/?target=http%3A//openaccess.thecvf.com/CVPR2018.py)

— **完** —

诚挚招聘

量子位正在招募市场运营实习生，参与线上/线下活动整体流程，帮助制作相关物料，运营社群等。工作地点在北京中关村。

期待有热情的同学加入！简历欢迎投递到quxin@qbitai.com

具体细节，请在量子位公众号(QbitAI)对话界面，回复“实习生”三个字。

**量子位** QbitAI · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


