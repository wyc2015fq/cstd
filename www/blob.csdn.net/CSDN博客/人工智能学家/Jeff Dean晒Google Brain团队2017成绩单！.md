# Jeff Dean晒Google Brain团队2017成绩单！ - 人工智能学家 - CSDN博客
2018年01月12日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：284
*![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBUicOIagodiaxmQE7XVfJZgqatsfBibZjCqb2NQGdCpz0M24gDtibbv5VsHIDF4tYJWLiaMz61r4fQYXPg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)*
*来源：全球人工智能*
谷歌大脑负责人 Jeff Dean 今天发文晒AutoML、语言理解、机器学习算法、机器学习系统等核心研究工作成绩单，全面总结谷歌大脑在2017年取得的成绩。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/X8OUs1wOMWEf81MP4ibIkPShvibykdqgDVvMXH58A8KwUhFE68zpDDEPH7M8iaDUlaUkjucFeAxJhJrJmOog5cfyQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**一、自动化机器学习（AutoML）**
我们开发了使用强化学习和进化算法设计神经网络结构的新方法，并推广到ImageNet图像分类和检测领域的最新成果，实现了如何自动学习新的优化算法和有效的激活函数。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UicQ7HgWiaUb2LwGwRORs1BibzgCdLstbCqqE73jgjQ20KzjHeSub7ZIkNdASGn7G9qM1mhrdMIXjXEyliaL6QVUiaQ/?wx_fmt=png)
*利用神经结构搜索发现卷积结构*
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UicQ7HgWiaUb2LwGwRORs1BibzgCdLstbCqPR1q2whTsXoiaL4BX9QhkgkibOiaqVPnPwrfzgL3M2C6c0M8wI0JDBRcw/?wx_fmt=png)
*利用AutoML发现的网络进行对象检测*
**二、话语理解与生成技术，词错率降低16%！**
开发新的技术来提高计算机理解和生成人类语言的能力，包括我们与Google speech团队合作，对谷歌的端到端语音识别方法进行了一系列改进，将谷歌的语音识别系统的相对词错率降低了16％。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UicQ7HgWiaUb2LwGwRORs1BibzgCdLstbCqlRTOia4jaJiaFFrzTA3OGD3lYvfMGdL0n3ls3gwJUTejdm7XbsPKty4A/?wx_fmt=png)
*用于语音识别的Listen-Attend-Spell端到端模型的组件*
还与谷歌的Machine Perception团队合作开发了一种新的文本到语音生成方法（Tacotron 2），大大提高了生成语音的质量。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UicQ7HgWiaUb2LwGwRORs1BibzgCdLstbCqH40XbHcHfRLueUsGAMJDkcxdKLmNr5PR5TwWFkKDyXtsgT6ibqXGEkA/?wx_fmt=png)
*Tacotron 2的模型架构*
**三、推出新的3种优秀算法**
**1.Hinton提出的capsules：**稀疏门控专家混合层（实现了超大规模但仍有计算效率的模型），hypernetworks（使用一个模型的权重来生成另一个模型的权重）；2.新的multi-modal模型：在同一个模型上执行音频、视觉和文本输入的多个学习任务；3.基于attention的机制：（卷积和循环模型的代替），symbolic和non-symbolic学习的优化方法，通过离散变量反向传播的技术，以及一些新的强化学习算法的改进。
**四、开放8大数据集（福利呀）**
MNIST，CIFAR-10，ImageNet，SVHN和WMT等开放数据集，极大地推动了机器学习领域的发展。谷歌大脑和Google Research在过去一年一直积极地为开放式机器学习研究开源有趣的新数据集，以及提供更多的大型标记数据集，包括：
- 
YouTube-8M：使用4,716个不同类别注释的700万YouTube视频
- 
YouTube-Bouding Boxes：来自210,000个YouTube视频的500万个bounding boxes
- 
Speech Commands数据集：包含数千个说话者说的短指令词
- 
AudioSet：200万个10秒的YouTube视频剪辑，标记有527个不同的声音事件
- 
Atomic Visual Actions（AVA）：57,000个视频剪辑中的21万个动作标签
- 
Open Images：使用6000个类别标记的900万个创作共用许可的图像
- 
Open Images with Boundign Boxes：600个类别的120万个bounding boxes
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UicQ7HgWiaUb2LwGwRORs1BibzgCdLstbCqKCa8nCOibsvCKEs8QJdxleDyt9GukkiarY30PQWIcZb3kGyiabQwkbkibA/?wx_fmt=png)
*YouTube-Bouding Boxes数据集样本：以每秒1帧采样的视频片段，围绕相关项目成功标识边界框*
**五、TensorFlow被180多个国家被下载了超过1000万次！**
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UicQ7HgWiaUb2LwGwRORs1BibzgCdLstbCqjRGCzkvdKwziaUHrEriaEPN2h8ichyLINKicUYF4xS8rTHXYDVnoGWTMaQ/?wx_fmt=png)
*TensorFlow用户的分布地图*
去年2月，我们发布了TensorFlow 1.0，11月，我们发布了v1.4版本，增加了以下重要功能：用于交互式命令式编程的Eager execution，用于TensorFlow程序的优化编译器XLA，以及用于移动设备和嵌入式设备的轻量级解决方案TensorFlow Lite。预编译的TensorFlow二进制文件已经在180多个国家被下载了超过1000万次，GitHub上的源代码现在已经有超过1200个贡献者。
**六、TPU比对手快15-30倍！**
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UicQ7HgWiaUb2LwGwRORs1BibzgCdLstbCqg61EjDnSfp76QVxCh5OvNmIVtUbBKkibGLEMLpxtfn72CwH1sI9bhwg/?wx_fmt=png)
*TPU提供高达180 teraflops的机器学习加速*
大约五年前，我们认识到深度学习会大大改变我们所需要的硬件种类。深度学习的计算量非常大，但它们有两个特殊的性质：它们主要由密集的线性代数运算（矩阵倍数，向量运算等）组成，它们对精度容忍度高。我们意识到我们可以利用这两个属性来构建能够非常有效地运行神经网络计算的专用硬件。我们为Google的平台团队提供了设计输入，他们设计并生产了我们的第一代Tensor Processing Unit（TPU）：一种旨在加速推理深度学习模型的单芯片ASIC（推理是使用已经过训练的神经网络，与训练不同）。
这款第一代TPU已经在我们的数据中心部署了三年，并且已经被用于每个Google搜索查询，Google翻译，Google相册中的图片，AlphaGo与柯洁的围棋比赛中，以及其他许多研究和产品用途。去年6月份，我们在ISCA 2017上发表了一篇论文，显示这款第一代TPU比现代GPU或CPU同类产品快15-30倍，性能/功耗约为30-80倍优化。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
