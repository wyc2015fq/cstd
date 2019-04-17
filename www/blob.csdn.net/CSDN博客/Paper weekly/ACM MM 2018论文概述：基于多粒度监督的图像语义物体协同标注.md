# ACM MM 2018论文概述：基于多粒度监督的图像语义物体协同标注 - Paper weekly - CSDN博客





2018年08月17日 08:45:15[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：324









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨张立石、付程晗、李甲

学校丨北京航空航天大学

研究方向丨计算机视觉




# 介绍




本文概述了被 2018 年 10 月 ACM Multimedia 会议录用为 Oral 的论文：***Collaborative Annotation of Semantic Objects in Images with Multi-granularity Supervisions***。在此论文中，北京航空航天大学硕士研究生张立石、付程晗及其导师李甲，提出了一种**基于多粒度监督的图像语义物体协同标注的方法**，实现了在几乎不影响标注精确度的前提下，减少了人工标注的时间。




■ 论文 | Collaborative Annotation of Semantic Objects in Images with Multi-granularity Supervisions

■ 链接 | https://www.paperweekly.site/papers/2218

■ 源码 | http://dwz.cn/kltHyMz0

■ 主页 | http://cvteam.net/




# 背景




在过去 10 年，大规模图像数据集大大推动了计算机视觉技术的发展。这些数据集中的图片被一个或多个标签标注，用于描绘图片中主要对象的语义类别。在最新的应用，比如自动驾驶，机器人导航，视觉问题回答等，仅有图像级标签是不够的，这些应用需要像素级的语义对象：图像中的对象是什么、在哪里。




计算机视觉领域对像素级标注语义对象的需求越来越强烈，但是像素级的标注是冗余乏味的，需要耗费大量的人力资源。因此，**为了将现有图像级标签的数据集转化为像素级标注的数据集，在标注精确度不受影响的前提下，减少人工标注时间是很有必要的。**




机器和人协同标注的方法已经被研究多年，基于协同策略，现有的方法被分为两类：**Agent-decision** 和 **Human-decision**。




Agent-decision 就是首先标注者进行粗略的标注，然后机器进行自动修正。这些标注结果很少作为 ground-truth。




Human-decision 就是首先让机器自动生成粗略的标注结果，然后标注者进行手工精细修正，这些标注结果是可以作为 ground-truth。显然，机器标注结果越好，人工修正的时间越短。但是 Human-decision 方法中缺点就是，机器初始化是静态的，需要预定义或预先训练参数，这就意味着会反复的犯同样的错误即使分割同一个语义对象。




因此，很多协同标注方式都是通过利用机器的参与减少人工标注时间，但是仍然存在一些问题，鉴于此**本文提出了一种智能协同标注工具 Colt：在人机交互标注的过程中不断学习，使得机器的标注越来越好，人工参与越来越少。**



# 思路




本论文的总体思路是首先利用机器自动生成初始标注结果，人工修正，随着人工修正的结果越多，机器进行学习可以进行机器自动修正，进一步减少人工修正。整体框架图如图 1 所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmGuUWd1eODrc5OIQ1j6orMpcJPGOKBH7iaJshJ8QynRZbLpeX9Dko5CeeRTAGUibziauRaOSZrgYb1g/640)

▲ 图1. 论文整体框架



# 机器自动化




机器自动化标注主要利用稀疏编码的思想，对待标注图像进行编码，编码长度的大小就意味属于前景物体的可能性大小，选择一个阈值分离前背景就能得到自动化标注结果。对于机器初始化标注，作者首先构建了两个字典：强字典、弱字典，将这两个字典作为稀疏编码图像的码表。



**字典构建**




首先根据每一类图像的语义标签计算语义相似性，然后根据图像特征计算每类图像之间的视觉相似性，联合得到每类图像之间的总相似性。选择相似性大于 0.95 的类别作为当前类别的稀疏编码字典。有像素级标注图像类别的特征的作为强字典，没有像素级标注图像类别特征的作为弱字典。




**稀疏编码**




编码对象是用 MCG 算法对图像提取出来的排在前 200 的图像 proposal。由于图像的分辨率和像素密度很高，作者为减少人工点击次数，借助超像素块进行操作。作者把 proposal 编码长度映射到超像素块并归一化得到每个超像素块的属于前景的可能性值，选择大于 0.4 的作为前景，剩下的作为背景，得到机器初始化结果。




# 人工修正




根据机器初始化结果，标注者进行修正：如果前后背景错误直接点击左键，如果边缘分割错误，首先点击右键进行分裂成更小的超像素块，然后点击左键。在人工修正的过程，机器会自动保存点击超像素块的 3 邻域特征用于后续的机器自动修正。 




# 机器自动修正




选择在阈值 0.4 上下 0.15 范围内的超像素块，用人工修正保存的超像素块 3 邻域特征进行稀疏编码，得到这些超像素块的编码长度，归一化选择大于 0.95 的超像素块进行前景背景在初始化基础上进行反转。得到机器自动修正结果。随着人工标注的结果越多，机器能学的越精确，自动化修正结果会更好。




# 实验




本文选取了 40 个图像类别。在 ImageNet 数据集 1000 类中并且和 MSCOCO 有相同标签的 10 个类别、在 ImageNet 数据集 1000 类中并且和MSCOCO有不同标签的 10 个类别、不在 ImageNet 数据集 1000 类中并且和 MSCOCO 有相同标签的 10 个类别，不在 ImageNet 数据集 1000 类中并且和 MSCOCO 有不同标签的 10个 类别。




作者选择 10 个年龄在 20-28 周岁之间的标注者进行标注，每个标注者用 LabelMe 进行标注 4 个图像类别。得到 LabelMe 的标注结果，作为本文的 ground-truth。




**为了比较作者方法的自动分割结果，作者和当前自动分割处于领先水平的两个方法：DeepMask 和 SharpMask 进行比较**。发现这两个方法的结果都明显低于 Colt 的初始化结果。具体结果见图 2。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmGuUWd1eODrc5OIQ1j6orMSib1On5ETC1sreHPqHXgawaT6usJFH6oT94iaI3hvy0xdlTacC5VBMZA/640)

▲ 图2. 自动化对比结果




为了比较最终标注结果，作者选择另外 10 个年龄在 20-28 之间的标注者用 Colt 进行标注，和 LabelMe 的标注结果计算 F-measure，最终平均结果是 91.21。并比较了 Top5 和 Bottom5，具体结果见图 3。标注对比结果见图 4。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmGuUWd1eODrc5OIQ1j6orMkYPoCmd48kX2zSddUhzqaO5GHxV4cMOicAtVzeDHo1PguKvy8Oy3deg/640)

▲ 图3. 最终标注结果




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmGuUWd1eODrc5OIQ1j6orMKWKibibMeAFBZ6sOFLib5cyiceVPCoEia8FG6tOIgnGia6Gx62T2pfBbeRdA/640)

▲ 图4. 标注对比结果





作者还做了机器自动修正结果对比实验，发现机器自动修正是有效的，结果见图 5。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmGuUWd1eODrc5OIQ1j6orM1NO1PPxouRUar40OVlbeZbRVUacVMjFcZdJoyGibGWFpWXl7WwWdq0g/640)

▲ 图5. 自动修正结果




但是 Colt 还是有一些缺陷，尤其是边界超像素分割得不够好，失败的标注结果见图 6 。





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmGuUWd1eODrc5OIQ1j6orMxNbRibykPTiaqIswFtqJXu5fA27y8Tukdt1h3s29cpF3Fsic2vpasibgfA/640)

▲ 图6. 失败结果




# 总结




与当前能作为 Ground-Truth 的人工标注方法 LabelMe 的标注结果相比，**作者标注工具 collaborative tool (Colt) 的标注结果 f-measure 值能够达到 91.21%，同时作者的标注工具能节约 50% 的人工标注时间**。实验结果表明在兼顾精确度的情况下还能大大的减少标注时间。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多论文解读：**




- 
[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)

- 
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[如何让GAN生成更高质量图像？斯坦福给你答案](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490794&idx=1&sn=bf3af5e3f53f2fa521da137b86afbb47&chksm=96e9c36aa19e4a7ca3bdacdac311dfab3d6ba41eef5993e96de1e9a2ab29ee0571fa2fbde166&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[哈佛NLP组论文解读：基于隐变量的注意力模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490536&idx=1&sn=0998c5dd4e20841f3542ee328de1f1b4&chksm=96e9c468a19e4d7e24a38f3d9c4b1b4ea48d729c7db124f2b92a2309f0eee982bb9f9bc8356e&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)

- 
[COLING 2018最佳论文：序列标注经典模型复现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490099&idx=1&sn=2d2497999186b979dd557fe3133b7606&chksm=96e9c5b3a19e4ca550a7ae55705af84e941b1aba14cb21f3f2ffc366df837d387575f8529cf2&scene=21#wechat_redirect)

- 
[一文解析OpenAI最新流生成模型「Glow」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。









