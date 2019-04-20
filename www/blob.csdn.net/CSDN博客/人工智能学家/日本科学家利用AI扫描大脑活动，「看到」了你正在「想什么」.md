# 日本科学家利用AI扫描大脑活动，「看到」了你正在「想什么」 - 人工智能学家 - CSDN博客
2018年01月10日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：566
![?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/waLJGrhJM0cvRyKT6568pUSToeQ7ibiaLcGmpgjr7TUuHtUA74zzOqJyZhwMW6WWlkAbHjFelfKgl3zuFBibbUo0Q/?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
原文来源：biorxiv
作者：Guohua Shen、Tomoyasu Horikawa1、Kei Majima、Yukiyasu Kamitani
「雷克世界」编译：嗯~阿童木呀、KABUDA
你有没有想象过，将来的某一天，计算机可以“看到”你在想什么。
![?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/waLJGrhJM0cvRyKT6568pUSToeQ7ibiaLcB3LGm7RnrdlkaWBbK9TN922a2PWbdJXia79ERQsIwNKshvvtHHmWIpA/?wx_fmt=jpeg)
图表由Kamitani实验室提供
这听起来是不是很遥远？但得益于日本京都的京都大学的四名科学家，这一技术现在变得越来越接近现实。12月下旬，Guohua Shen、Tomoyasu Horikawa、Kei Majima和Yukiyasu Kamitani公布了他们最近利用人工智能解码科学平台BioRxiv上的想法的研究结果。
机器学习以前往往被用于研究脑部扫描（MRI或磁共振成像），并可以在涉及简单的二进制图像，如黑白字母或简单的地理形状（参见图2 ）时，生成一个人所思所想的可视化图像。
但京都的科学家开发了一种使用深度神经网络（人工智能）“解码”思想的新技术。这项新技术使得科学家能够解码更复杂的“分层”图像，其中这些图像具有多层颜色和结构，比如鸟的照片或是戴着牛仔帽的男子的照片。
“我们一直在研究一种方法，通过观察一个人的大脑活动来重构或重新创建这个人所看到的图像”。科学家Kamitani在接受CNBC Make It采访时说，“我们以往的方法是假设一个图像是由像素或简单的形状组成的，但是我们已经知道，我们的大脑处理视觉信息的方式是分层地提取不同层的特征或不同复杂度的组件”。
这种全新的人工智能研究使得计算机能够对目标进行检测，而不仅仅是二进制像素。Kamitani说：“这些神经网络或人工智能模型可以用来作为人类大脑层次结构的代表。
为了进行这项研究，在过去10个月的时间中向3位试验者展示了自然图像（如鸟或人的照片）、人工制作的几何形状和按字母表顺序排列的字母的图像，其时间长短不一。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0cvRyKT6568pUSToeQ7ibiaLcrGCJay48H5pibeQYmJVCQmiaUffaNAuXgicWWywaWb8AZcbhmNN9mlPqg/?wx_fmt=png)
所看到自然图像的重构。具有黑色和灰色帧的图像分别显示提交的和重构的图像（从VC活动重构的）。a，使用DGN进行重构（使用DNN1-8）。三个重建的图像对应于来自于对三个试验者的重构。b，使用好不使用DGN进行的重构（DNN1-8）。第一行、第二行和第三行分别显示了所提交的图像，使用和不使用DGN进行的重构。c，所看到的自然图像的重构质量（误差线，跨样本的95％置信区间（C.I.），三个试验者所看到图像的池化，机会水平，50％）。 d，重构。
在某些情况下，当一个试验者在看25张图像中的一张时，大脑活动被测量。而在其他情况下，当试验者被要求回想一下之前向他们所展示的图片时，它就会被记录下来。
一旦大脑活动被扫描，一台计算机会对信息进行反向操作（或“解码”），从而将这位试验者的想法进行可视化。
下面的流程图是由京都大学Kamitani实验室的研究小组制作的，它揭示了可视化是如何被“解码”的科学技术。
![?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/waLJGrhJM0cvRyKT6568pUSToeQ7ibiaLcGmpgjr7TUuHtUA74zzOqJyZhwMW6WWlkAbHjFelfKgl3zuFBibbUo0Q/?wx_fmt=jpeg)
下面的两个图表显示了计算机为试验者进行重构的结果，其中，当试验者在查看自然图像和字母图像时，其活动被记录下来了。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0cvRyKT6568pUSToeQ7ibiaLcyVUITQ08fZJeYwA3ibEXfpPkBy3qkKibDkQS4AD3AJmjk6pK92VIMt3w/?wx_fmt=png)
用DGN获得的有关自然图像重构的其他例子。具有黑色和灰色帧的图像分别显示提交的和重构的图像，尤其是使用所有DNN层从VC活动中进行重构的。三个重建的图像对应于来自于对三个试验者的重构
而对于那些只能在其记住图像的基础上进行脑电波测量的试验者来说，科学家又有了新的突破。
Kamitani说：“与以前方法不同的是我们可以通过想象一些记忆的图像来重建一个人的视觉图像”。
如下面图表所示，当对一个记住图像的试验者产生的脑信号进行解码时，人工智能系统在进行重构时会有一定的困难。那是因为人类难以像他所看到的那样将一只猎豹或一条鱼的形象记得清清楚楚。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0cvRyKT6568pUSToeQ7ibiaLcLf9rGzgbbLSWkkX2tZvUQRZok4AqBBMiaOR3XSzEw0UIj8YmVibpjicSQ/?wx_fmt=png)
所有按字母表顺序排序的字母重构的例子。具有黑色和灰色帧的图像分别显示提交和重构的图像（从没有DGN的VC活动中进行重构的）。三个重构的图像分别对应于来自三个试验者的重构
**Kamitani向CNBC Make It解释说：“在这种情况下，大脑不足以进行激活”。**
随着技术的精确度在不断地得以提高，潜在应用程序的发展也是令人难以置信的。可视化技术可以让你简单地通过想象来进行绘画或创作艺术。你可以通过计算机将你的梦想进行可视化。我们还可以对精神病患者的幻觉进行可视化，从而针对他们的情况对其进行有效的治疗。而脑机接口（brain-machine interfaces），有朝一日可能使得我们能够与意象或想法进行沟通与交流，Kamitani向CNBC Make It解释道。
虽然计算机阅读你的大脑的想法听起来可能是Jetson式的，在他们致力于将大脑与计算能力联系起来的研究工作中，日本研究人员并不是在孤军奋战的。
例如，前任GoogleX部门负责人 Mary Lou Jepsen正在努力打造一顶能在十年内使得心灵感应成为可能的帽子，而企业家Bryan Johnson正致力于将计算机芯片植入大脑，以改善神经功能。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0cvRyKT6568pUSToeQ7ibiaLcB0e8vPI6ria4Fn4lRI9icW68fCLiazWmDP8f9WEgf8tH8rFg2a2V0xfQQ/?wx_fmt=png)
基于机器学习的人体功能磁共振成像（fMRI）模式分析使得感知内容可视化成为可能。然而，它一直受限于使用低级图像库进行重构或仅限于与样本的匹配。近期的研究表明，对于同一幅输入图像而言，视觉皮层活动可以被解码（翻译）为深度神经网络（DNN）的层次特征，进而提供了一种利用视觉层次特征信息的方法。本文提出了一种图像重构的新方法，该方法对图像的像素值进行了优化，从而使其DNN特征与多层的人脑在活动中解码的特征相似。我们发现生成的图像与刺激图像（stimulus images）（自然图像与人造图像）和图像中的主观视觉内容相似。虽然我们的模型仅通过自然图像进行单独训练，但我们的方法成功地将重构泛化到了人工形状（artificial shapes）的制作中，这表明了我们的模型确实是“重构”或“生成”大脑活动的图像，而非简单地与样本相匹配。之前由另一个深度神经网络引入的一张自然图像，通过约束自然图像与重构图像之间的相似度，有效地呈现了有关重构的语义上有意义的细节。此外，人类对重构的推论表明，结合多个DNN层可以有效提高生成图像的视觉质量。结果表明，大脑中的分层视觉信息可以被有效地结合起来以重构感知和主观图像。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
