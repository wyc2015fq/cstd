
# ECCV 2018 | Pixel2Mesh：从单帧RGB图像生成三维网格模型 - Paper weekly - CSDN博客


2018年09月10日 13:16:37[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：788


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

**腾讯 AI Lab 与复旦大学、普林斯顿大学、Intel Labs 合作提出一种端对端的深度学习框架，可从单张彩色图片直接生成三维网格（3d mesh)。**该研究论文被顶级会议 ECCV 2018 收录，以下是技术详细解读。

ECCV（European Conference on Computer Vision，计算机视觉欧洲大会）将于 9 月 8 日-14 日在德国慕尼黑举办，该会议与 CVPR、ICCV 共称为计算机视觉领域三大顶级学术会议，每年录用论文约 300 篇。腾讯 AI Lab 是第二次参与该会议，录取文章数高达 19 篇，位居国内前列。在刚结束的计算机视觉领域另外两大会议 CVPR，ICCV 中也收获颇丰，分别录取 21 篇和 7 篇论文。

■ 论文 | Pixel2Mesh: Generating 3D Mesh Models from Single RGB Images
■ 链接 |https://www.paperweekly.site/papers/2283
■ 源码 |https://github.com/nywang16/Pixel2Mesh

该论文由复旦大学、普林斯顿大学、Intel Labs 和腾讯 AI Lab 合作完成。**文章提出了一种端到端的深度学习框架，可从单张彩色图片直接生成三维网格（3D Mesh）。**

受深度神经网络特性的限制，以前的方法通常用 volume 或者 point cloud 表示三维形状，将它们转换为更易于使用的 mesh 并非易事。与现有方法不同，**本文使用图卷积神经网络表示 3D mesh，利用从输入图像中提取的特征逐步对椭球进行变形从而产生正确的几何形状。**本文使用由粗到精的模式进行生成，使得整个变形过程更加稳定。

此外，**本文还定义了几种与 mesh 相关的损失函数捕捉不同级别的特性**，以保证视觉上有吸引力并且物理上高重建精度。大量实验表明，本文的方法不仅定性上可以生成细节更加充分的 mesh 模型，而且与当前最好的方法相比也实现了更高的重建精度。

三维数据有多种表示形式，包括 volume、point cloud、mesh 等。volume 是一种规则的数据结构，即将物体表示为的 N3 格子，受分辨率和表达能力限制，这种表示方法通常缺乏细节。

point cloud 是一种不规则的数据结构，由于点之间没有局部连结关系，点云往往缺乏物体的表面信息；3D Mesh 同样是一种不规则的数据结构，由点、边和面组成，由于其轻量、形状细节丰富等特性，在虚拟现实、动画游戏、生产制造等实际产业中应用越来越广泛，**本文研究如何从单张 RGB 图重建出对应的 3D mesh 模型。**

# 模型架构

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75ZMjwt1VPlV9Kwo3SbicjEtILwvyZGM36ZHSNWf0nhZMnD40oTPsAyibg/640)
**▲**Figure 1: Pixel2Mesh的架构

Pixel2Mesh 的架构如图 1 所示。对于输入图像，设计一个多层的卷积神经网络，用于提取图片不同层次的特征。对于输出三维网格，设计了 3 个级联的变形模块，由图神经网络构成。Perceptual feature pooling 层可根据网格顶点坐标得到投影的图片特征，然后根据此特征不断对初始的椭球进行变形，以逼近真实形状。Graph unpooling 层用于增加三维网格节点和边的数量。

**本文的目的在于提出一种生成结果光滑、重建精度高的利用单张彩色图生成物体三维网格模型的方法。**本文没有直接采用生成 3D Mesh 的方法，而是采用将一个固定的椭球依据图像特征逐渐形变成目标形状的策略。本文针对两个三维网格重建难点给出了解决方案：

**1. 3D Mesh 的表示问题—引入图卷积神经网络。**3D mesh 一般使用非规则数据形式—图（Graph）来表示，而同时整个网络还需要对其输入的规则的图像数据进行特征抽取。本文使用图卷积网络（GCN）来处理三维网格，使用类似于 VGG-16 的网络来提取二维图像特征。为了将两者联系在一起，设计了一个投影层使得网格中的每个节点能对应获取相应的二维图像的特征。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75QfaTHzBibhkYCeicaFclo1ttF9icEU49x1tiblV9Sd177Nwkfv6CIicuagg/640)

上图（a）中 P 是从输入图片得来的特征，是顶点的位置，两者组合可得到相应的投影特征，再与 GCN 的特征简单合并送入网格形变模块（Mesh Deformation Block）中。整个网络包含若干该模块。上图（b）是依据相机参数进行投影的示意图。

**2. 如何有效地更新网格的形状—引入图的上池化层。**实验发现，直接训练网络使其预测 mesh 的效果并不好。一个原因是如果直接预测较大数量的点的网格，顶点的感受野会有受限，即 mesh 的顶点无法有效地检索邻近顶点的特征。为了解决这个问题，本文设计了一个图的上池化层（Graph unpooling layer），使得点的数量逐渐由少到多，相应的网格形状由粗到细，既保留了全局信息，又具有细节的表达。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75BNJibAgCM5YHWENq7XBUvVYCibTxxetsKwQDrEtPdk8UlZ1Y8f7KpETw/640)

上图是图的上池化过程。（a）中的黑色顶点即为新增的顶点，连接的虚线即为新增的边。这是 Edge-based 的方式，还有一种 Face-based 的方式，即在每个三角形中心添加一个顶点，但这样会造成网格顶点不均衡的问题，如（b）所示。在实际操作中，每个新增的点位于旧三角形的边的中点位置，特征的值取相邻两顶点的均值。

# 损失函数

**本文定义了四种不同的损失函数来促使网格更好地形变。**

**1. Chamfer 损失函数：**其作用是限制网格顶点的具体位置；

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75NIQ6leqn8NR2uSf42p4AjVf5dD2yomevibqUF3LjCqKdaBCYH4MxhRg/640)

**2. Normal 损失函数：**其作用是增强网格表面法向的一致性（增加表面光滑度）；

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75kVSbKO7yt7gyGFQqmgrTKrZGM7FxJNaHTrKH2xXOx9jicgWy2AXrMYQ/640)

**3. Laplacian 正则化：**其作用是在形变时维持临近顶点的相对位置；

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75bAvLTGqK3XkVkvfKuzMPg9hGQKBFrxun1zvB4mMnwuv0siciabKTqp2Q/640)

**4. Edge length 正则化：**其作用是防止个别异常顶点的出现。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75DGrKmWIcgKyibqRXDfbFib08Fez42kszibUKeRvobSrARJD4WvAaGE6KQ/640)

**本方法对输入图像进行的三维网格建模的结果：**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75GrXTiaJ10p8L0HDFibahOqR2yU7Rhh049CcmoseINYRATvx3MGywoicQA/640)
**▲**Qualitative results. (a) Input image; (b) Volume from 3D-R2N2 [1], converted using Marching Cube [4]; (c) Point cloud from PSG [2], converted using ball pivoting [5]; (d) N3MR[3]; (e) Ours; (f) Ground truth.

# 实验结果

**F-score (%)**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75qm0AcjMJaxj3vb4tVx51uIibjsicYHbFicD9eGFH0WU8A9GtWgt4qmuVA/640)

本文提出的方法在 ShapeNet 测试数据集上的表现，除了当阈值为时在 watercraft 类低于 PSG，其他指标均达到了最好水平。

**CD 和 EMD**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75ic3TILFVvwq8ia8IVad8ibp5VXakSXMee6ZEXc2aD0ic5AnbVS3sj1VYfw/640)

对于 CD 和 EMD，在 ShapeNet 测试集上，本文的方法在多数情况下都达到了最好水平。

# 参考文献
[1]. Choy, C.B., Xu, D., Gwak, J., Chen, K., Savarese, S.: 3d-r2n2: A unified approach for single and multi-view 3d object reconstruction. In: ECCV. (2016)
[2]. Fan, H., Su, H., Guibas, L.J.: A point set generation network for 3d object reconstruction from a single image. In: CVPR. (2017)
[3]. Kato, H., Ushiku, Y., Harada, T.: Neural 3d mesh renderer. In: CVPR. (2018)
[4]. Lorensen, W.E., Cline, H.E.: Marching cubes: A high resolution 3d surface construction algorithm. In: SIGGRAPH. (1987)
[5]. Bernardini, F., Mittleman, J., Rushmeier, H.E., Silva, C.T., Taubin, G.: The ball-pivoting algorithm for surface reconstruction. IEEE Trans. Vis. Comput. Graph. 5(4) (1999) 349–359

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多论文解读：**

网络表示学习综述：一文理解Network Embedding
神经网络架构搜索（NAS）综述
从傅里叶分析角度解读深度学习的泛化能力

哈佛NLP组论文解读：基于隐变量的注意力模型
[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)
ECCV 2018 | 腾讯AI Lab提出视频再定位任务
KDD 18 | 斯坦福大学提出全新网络嵌入方法



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。


