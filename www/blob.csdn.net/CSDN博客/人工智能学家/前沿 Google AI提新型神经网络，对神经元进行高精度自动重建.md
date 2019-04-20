# 前沿|Google AI提新型神经网络，对神经元进行高精度自动重建 - 人工智能学家 - CSDN博客
2018年07月18日 23:19:33[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：364
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZmTWqhgthp5UXJzL8ILUC7jcGaiaqaWaTQlpNQVjL6duJoVVzoPRkxS8X9QAEZyj7ga71qGp5vy5A/640?wx_fmt=png)
来源：AI科技大本营
7 月 16 日， Google AI 发布了一篇博客称，Google Research 部门和 Max Planck 研究所合作**提出了一种新型的递归神经网络，它可以提升连接组数据进行自动分析的准确性，相比先前深度学习技术的准确性是数量级的提升。**
Jeff Dean 随后也在 Twitter 上转发评价称，“使用人工神经网络重建真实神经元的连接性研究真的很酷。”
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZmTWqhgthp5UXJzL8ILUC7jQDCKK1e78R3GCC1IfN00V2IlR1diaHAJzo38bF9bn7XoqpmZcRfwpA/640?wx_fmt=png)﻿
**数量级提升了连接组数据自动分析的准确性**
根据维基百科的定义，连接组学（Connectomics）绘制与研究神经连接组（connectomes）是一种刻画有机体神经系统（尤其是脑和眼）的连接方式的完整线路图。**由于这些结构极其复杂，高效筛选的神经成像和组织学方法被用于提高绘制神经连接线路图的速度、效率和精度。**
**连接组学的目的是全面映射神经系统中的神经元网络的结构，以便更好地理解大脑是如何运作的。**这个过程需要以纳米分辨率（通常使用电子显微镜）对脑组织进行 3D 成像，然后分析所得到的图像数据，以追踪大脑的神经轴突，并识别单个突触连接。
由于成像的高分辨率，即使是 1 立方毫米的脑组织也可以产生超过 1000 TB 的数据。再加上这些图像中的结构比较微妙和复杂，大脑映射的主要瓶颈是自动分析这些数据，而不是获取数据本身。
Google 与 Max Plank 神经生物学研究所在《Nature Methods》中发表的“ 用泛洪填充网络高效自动重建神经元 （High-Precision Automated Reconstruction of Neurons with Flood-Filling Networks）”一文 ，则展示了一种新型的递归神经网络，如何提升连接组数据自动分析的准确性，这比先前深度学习技术是数量级的提升。
**使用泛洪填充网络进行 3D 图像分割**
**在大规模电子显微镜数据中追踪神经轴突是一个图像分割问题。**传统算法将该过程划分为至少两个步骤：使用边缘检测器或机器学习分类器找出神经轴突之间的边界，然后使用分水岭（watershd）或图形切割等算法将未被边界分隔的图像像素进行组合。
**2015 年，他们开始尝试基于递归神经网络的替代方法，将这两个步骤统一起来。**该算法在特定的像素位置播种，然后使用循环卷积神经网络不断地“填充”一个区域，该网络会预测哪些像素是与该特定像素属于同一部分。自 2015 年以来，他们一直致力于将这种新方法应用于大规模的连接组数据集，并严格量化其准确性。
﻿﻿﻿﻿﻿﻿
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/BnSNEaficFAZmTWqhgthp5UXJzL8ILUC7KdvrYnwOXtTrpWrV2dG8qIca7pnyVEmmXBObCFyHh8LuAuTWicsYmOw/640?wx_fmt=gif)
分割对象的泛洪填充网络。黄点是当前焦点区域的中心; 当算法不断检查整个图像时，分割区域会不断扩展（蓝色）。
**通过预期的运行长度测量精度**
**他们设计了一个叫做“预期运行长度”（ERL）的度量标准：**给出大脑三维图像中随机神经元内的随机点，在犯错前，可以追踪神经元的距离。这是一个平均故障间隔时间（Mean Time Between Failures）的示例，但在这种情况下检测出的是故障之间的空间而不是时间。
对于研究人员而言，ERL 的吸引力在于，它将线性物理路径长度与算法产生的各个错误的频率联系起来，并且可以直接计算。对生物学家来说， ERL 的特定数值可以与生物学相关数量产生联系，例如神经系统不同部分的神经元平均路径长度。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZmTWqhgthp5UXJzL8ILUC7jlbJicjRVIITpvZQuibv2yIjeqS6TCSk2AaesSI5LUWdXgw7tXUz9yCA/640?wx_fmt=png)
蓝线表示预期运行长度（ERL）的结果。红线表示“合并率”，“合并率”指两个单独的神经轴突被错误地追踪为单个对象的频率; 非常低的合并率对于实现手动识别和纠正重建中的剩余错误很重要。
**鸣禽的连接组学**
他们使用 ERL 测量了在 100 万立方微米的斑胸草雀大脑的真实神经元数据集，**发现该方法的表现好于其他应用于同一数据集的深度学习方法。**
﻿﻿
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/BnSNEaficFAZmTWqhgthp5UXJzL8ILUC7icp7t6tjIRt8BwXYPUB2urLiaMANwEBJp70RK2FBMxoibAb6fJrwE3OLw/640?wx_fmt=gif)
算法在追踪斑胸草雀大脑中的单个神经轴突
**他们使用新的泛洪填充网络方法对斑胸草雀大脑的一小部分神经元进行分割，视频如下：**
﻿重建一部分斑胸草雀的大脑。不同颜色表示使用填充灌溉网络自动生成的分割的不同对象。金球代表使用以前的方法自动识别的突触位置。
**他们还将继续改进连接组学重建技术。**为了帮助更多研究团体开发连接组学技术，他们开发了用于泛洪填充网络方法的 TensorFlow 代码，并开发了用于 3D 数据集的 Web GL 可视化软件帮助理解和改进重建结果。
**相关链接：**
https://ai.googleblog.com/2018/07/improving-connectomics-by-order-of.html
https://www.nature.com/articles/s41592-018-0049-4
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
