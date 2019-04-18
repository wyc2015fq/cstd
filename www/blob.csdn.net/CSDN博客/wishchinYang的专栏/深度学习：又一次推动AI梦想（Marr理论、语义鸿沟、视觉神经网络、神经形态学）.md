# 深度学习：又一次推动AI梦想（Marr理论、语义鸿沟、视觉神经网络、神经形态学） - wishchinYang的专栏 - CSDN博客
2013年12月21日 18:41:25[wishchin](https://me.csdn.net/wishchin)阅读数：853
个人分类：[ANN/DNN/纤维丛																[TuringMachine																[AI/ML																[总体哲学AIPRIPCV](https://blog.csdn.net/wishchin/article/category/7538201)](https://blog.csdn.net/wishchin/article/category/6985098)](https://blog.csdn.net/wishchin/article/category/6972922)](https://blog.csdn.net/wishchin/article/category/3109825)
        几乎每一次神经网络的再流行，都会出现：推进人工智能的梦想之说。![偷笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/titter.gif)
**前言：**
**Marr视觉分层理论**
**Marr视觉分层理论**（[百度百科](https://baike.baidu.com/item/D.Marr%E8%AE%A1%E7%AE%97%E8%A7%86%E8%A7%89%E7%90%86%E8%AE%BA/9604790)）：理论框架主要由视觉所建立、保持、并予以解释的三级表象结构组成，这就是:
        a.基元图(the primal sketch)—由于图像的密度变化可能与物体边界这类具体的物理性质相对应，因此它主要描述图像的密度变化及其局部几何关系。
        b. 2.5维图(2.5 Dimensional sketch)—以观察者为中心，描述可见表面的方位、轮廓、深度及其他性质。
        c. 3维模型(3D Model)—以物体为中心，是用来处理和识别物体的三维形状表象。
**语义沟壑**
      Semantic gap（Wiki百科）: CB IR 中的“语义鸿沟”就是：由于计算机获取的图像的视觉信息与用户对图像理解的语义信息的不一致性而导致的低层和高层检索需求间的距离。  
感知鸿沟（sensory gap），它是一种在现实世界的物体和该场景记录下来的（计算上的）描述信息之间的鸿沟。
语义鸿沟（semantic gap），它是由于所视觉数据中提炼出的信息与在特定场合下这些数据对用户的解释之间缺乏一致性。
**视神经网络分层模型**
        人体生理学研究有几百年的历史，对于视觉神经系统的研究，任然处于实验模拟阶段，并不能得到真正的阻断实验。目前可得出的生理学研究，视神经系统（[百科](http://www.baike.com/wiki/%E8%A7%86%E7%A5%9E%E7%BB%8F)）显示出分层和稀疏特性。并从此能够得到视觉神经系统到语义描述系统（语义鸿沟）的映射。
      自此，深度网络为解决语义鸿沟指出了一个方向，且CNN可以从直觉上模拟人的神经系统，深度学习的深度有了真正地意义。
**（1）：深度学习：推动人工智能梦想**
**原文链接**：[http://www.csdn.net/article/2013-05-29/2815479](http://www.csdn.net/article/2013-05-29/2815479)
Key Word：浅层学习，深度学习；
浅层学习：浅层模型有一个重要特点，就是假设靠人工经验来抽取样本的特征，而强调模型主要是负责分类或预测。浅层模型：貌似只有一个隐含层的神经网络。在模型的运用不出差错的前提下（如假设互联网公司聘请的是机器学习的专家），特征的好坏就成为整个系统性能的瓶颈。这样经验就起了很重要的作用！
深度学习：百度在线学习案例。
DNN与微软同声传译背后的故事：[http://www.csdn.net/article/2013-06-09/2815737](http://www.csdn.net/article/2013-06-09/2815737)
      “我们谈到AI时，意味着高度抽象，Deep Learning是抽象的一种方式，但它远不是全部。通过神经网络能够识别动物，并不意味就理解了世界，我甚至将其看做‘模式识别’而非‘智能’”，Seide这样认为：“‘深’对智能系统来说很重要，但它不是智能的全部。语音识别可以视为AI领域的一个缩影，DNN也只是语音识别技术中的一部分——若从代码长度的角度考量，它甚至只是全部技术中很小的一部分。”
         PS：这由让我想起来 中文屋子  的哲学讨论
## （2）：机器学习前沿热点–Deep Learning
##            机器学习前沿热点：[http://elevencitys.com/?p=1854](http://elevencitys.com/?p=1854)
         原始链接：[http://blog.sina.com.cn/s/blog_46d0a3930101fswl.html](http://blog.sina.com.cn/s/blog_46d0a3930101fswl.html)
         自 2006 年以来，机器学习领域，取得了突破性的进展。
        图灵试验，至少不是那么可望而不可即了。至于技术手段，不仅仅依赖于云计算对大数据的并行处理能力，而且依赖于算法。这个算法就是，Deep Learning。借助于 Deep Learning 算法，人类终于找到了如何处理 “抽象概念”这个亘古难题的方法。
       于是学界忙着延揽相关领域的大师。Alex Smola 加盟 CMU，就是这个背景下的插曲。悬念是 Geoffrey Hinton和 Yoshua Bengio 这两位牛人，最后会加盟哪所大学。
Geoffrey Hinton 曾经转战 Cambridge、CMU，目前任教University of Toronto。相信挖他的名校一定不少。
Yoshua Bengio 经历比较简单，McGill University 获得博士后，去 MIT 追随 Mike Jordan 做博士后。目前任教 University of Montreal。
Deep Learning 引爆的这场革命，不仅学术意义巨大，而且离钱很近，实在太近了。如果把相关技术难题比喻成一座山，那么翻过这座山，山后就是特大露天金矿。技术难题解决以后，剩下的事情，就是动用资本和商业的强力手段，跑马圈地了。
         于是各大公司重兵集结，虎视眈眈。Google 兵分两路，左路以 Jeff Dean 和 Andrew Ng 为首，重点突破 Deep Learning 等等算法和应用 [3]（Introduction to Deep Learning.  http://en.wikipedia.org/wiki/Deep_learning）。
## （3）：Neuromorphic Engineering- A Stepstone for Artificial Intelligence
##           神经形态工程师的目标：[http://elevencitys.com/?p=6265](http://elevencitys.com/?p=6265)
        这个全部黏贴了！
        构建类似人脑的三大特征的计算机是神经形态工程师的目标！（**低功耗； 容错性； 自学习**）。人类大脑的功率：约20W，当然这还只是TDP，平时消耗更低。容错性：并行处理，因此也意味着并非完备，而是一个概率模型。自学习：这个属于系统级别，包含整个感知-反馈-决策系统，复杂度暂时没办法分析。
        Here I would like to introduce the progress of Neuromorphic engineering（神经形态工程学）, a branch of engineering built on electronic devices. The main goal of this subject is to emulate complex neuron network and ion channel dynamics in real time, using highly compact and power-efficient CMOS analog VLSI technology. Compared to traditional software-based computer modeling and simulation, this approach can be implemented in a extremely small size with low power requirement, when is used for large-scale and high speed simulation of neuron. This special feature provide possibility for the real computing applications, such as**neuroprothesis, brain-machine interface, neurorobotics, machine learning** and so on. [1]
![](http://ww3.sinaimg.cn/bmiddle/c183287fjw1ebdafj1s6tj20b40c8js3.jpg)
         A key aspect of neuromorphic engineering is understanding how the morphology of individual neurons, circuits and overall architectures creates desirable computations, affects how information is represented, influences robustness to damage, incorporates learning and development, adapts to local change (plasticity), and facilitates evolutionary change. Neuromorphic engineering is a new interdisciplinary discipline that takes inspiration from biology, physics, mathematics, computer science and engineering to design artificial neural systems, such as vision systems, head-eye systems, auditory processors, and autonomous robots, whose physical architecture and design principles are based on those of biological nervous systems.[2]
         Our human brain has three distinct feature, which are highly parallel processing. quick adaptability,  and self-configuration.  We  have owned a deep understanding about the digital computers from the top to the bottom, from the operating system to the hardware design now. However, some analog computing, for example, voice recognition, learning etc. is not easy to implemented in the digit computers by now. In terms of the accuracy and power efficient, the mammal’s brain is so power and difficult to figure out. Since the artificial intelligence was pointed out in last century, we have invested lots of research effort in many areas, such as computer science, physiology, chemistry etc. to explain our brain. But it seems true that we know much more about the universe than the brain, it is sad, or promising? The only thing we are sure about, is that the brain do more than just information processing.
         Thus engineers began to ask for help from the biology perspective. But it is not so easy to emulate such a large scale computing machine, which owns about 85 billion neurons. Neuromorphic engineering is an important and promising branch to let us find the mystery of our brain. The feature of neuron computing is high parallelism, and adaptive learning, while bad at math. Same as the real CMOS technology, the placement of interconnect is a tricky job in Neuromorphic engineering. This engineering provides a potential to build the machine whose nature is learning.
![](http://cdn.static-economist.com/sites/default/files/imagecache/full-width/images/print-edition/D/20130803_STD002_0.jpg)
        DARPA SyNAPSE Program is an on-going project to build a electronic neuromorphic machine technology that scales to biological levels. It has made several milestones since it was initialized from 2008.  It should recreate 10 billion neurons, 100 trillion synapses, consume one kilowatt (same as a small electric heater), and occupy less than two liters of space at last. [3]
        The initial phase of the SyNAPSE program developed nanometer scale electronic synaptic components capable of adapting the connection strength between two neurons in a manner analogous to that seen in biological systems**(Hebbian learning)**, and simulated the utility of these synaptic components in core microcircuits that support the overall system architecture.
        Continuing efforts will focus on hardware development through the stages of microcircuit development, fabrication process development, single chip system development, and multi-chip system development.In support of these hardware developments, the program seeks to develop increasingly capable architecture and design tools, very large-scale computer simulations of the neuromorphic electronic systems to inform the designers and validate the hardware prior to fabrication, and virtual environments for training and testing the simulated and hardware neuromorphic systems. [4]
       To see more background: [http://homes.cs.washington.edu/~diorio/Talks/InvitedTalks/Telluride99/](http://homes.cs.washington.edu/~diorio/Talks/InvitedTalks/Telluride99/)
[](http://homes.cs.washington.edu/~diorio/Talks/InvitedTalks/Telluride99/)
**Reference:**
[1] Rachmuth, Guy, et al. “A biophysically-based neuromorphic model of spike rate-and timing-dependent plasticity.” *Proceedings of the National Academy of Sciences* 108.49 (2011): E1266-E1274.
[2] [http://en.wikipedia.org/wiki/Neuromorphic_engineering](http://en.wikipedia.org/wiki/Neuromorphic_engineering)
[3] [http://www.artificialbrains.com/darpa-synapse-program](http://www.artificialbrains.com/darpa-synapse-program)
[4] [http://en.wikipedia.org/wiki/SyNAPSE](http://en.wikipedia.org/wiki/SyNAPSE)
**（4）：最后呼吁：**
不管怎样都好，如果有一天，AI真的找到合适的程序构建模型，多少人还是希望我们对这个模型的了解能超过我们对于自身的了解。黑箱意味着不可控制，必然导致无法预料的结果，这是所有从事科学职业的人是不想看到的。
       付出多少就能得到多少，付出多少才能得到多少，一劳永逸意味着灭亡。

