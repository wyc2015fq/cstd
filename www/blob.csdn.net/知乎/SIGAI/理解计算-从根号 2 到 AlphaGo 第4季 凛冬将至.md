# 理解计算-从根号 2 到 AlphaGo 第4季 凛冬将至 - 知乎
# 

![](https://pic3.zhimg.com/v2-32746b919676f223aad88837161365ea_b.jpg)
本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
- [书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
- [书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)

在新中国诞生的那一年，加拿大生理学家唐纳德﹒赫布(Donald O. Hebb)出版了《行为的组织》 (《The Organization of Behavior》)一书，书中有一个后来被广泛引用的句子：“当细胞 A 的一个轴突和细胞 B 很近，足以对它产生影响，并且持久地、不断地参与了对细胞 B 的兴奋，那么在这两个细胞或其中之一会发生某种生长过程或新陈代谢变化，以致于 A 作为能使 B 兴奋的细胞之一，它的影响加强了。[1]” 被广泛引用的原因是这句话为后来神经网络参数的学习提供了最早的生理学来源，为定量描述两个神经元之间是如何相互影响的给出了一个大胆的论断。
![](https://pic2.zhimg.com/v2-d84627cc5f60fe061b414487ef8ec711_b.jpg)图1 赫布及其学习规则
图1的右边表示两个相邻(相近)的神经元i和神经元j，蓝色表示这个神经元没有被激活，回顾一下第3季，也就是说输出y=0，橙色表示神经元被激活，对应的输出y=1。现在我们考虑一下两个神经元之间的关系，确切的说是神经元i对j的影响，需要特别指出的是这种影响不仅仅由神经元i决定，而是两者在某种事件多次重复发生时的一种条件反射。

> 1） 第一种事件，神经元i没有激活，而神经元j由于其他神经元对它的刺激而激活。

2）第二种事件则是神经元i被其他神经元激活，而神经元j没有激活。很明显，情况1和情况2如果经常发生的话，尽管两个神经元离得很近，这种经验或事实表明神经元j的突触几乎感觉不到神经元i对它的影响，这时神经元j的突触对神经元i的刺激的敏感度就会下降，实际上，连接权值![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D)可以看作神经元j的突触对神经元i的一种敏感程度。

3）第三种事件，则表明如果神经元i和j经常同时激活的话，神经元j的突触对神经元i的刺激的敏感度就会提高，这个过程也被称为突触学习学说。

造成这三种事件发生的原因并不特别清楚，但是，结果却显而易见，就是两个神经元之间的关系 ![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D) 变化了。以上更新过程可以表示为如下的公式：
![](https://pic4.zhimg.com/v2-da0e770e0eb86952cf0367a092a33267_b.jpg)
其中 ![\eta](https://www.zhihu.com/equation?tex=%5Ceta) 是表示学习速率的常数，这个常数很容易理解，比如初始状态，![\eta](https://www.zhihu.com/equation?tex=%5Ceta)=1，![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D)(t)=0, 我们需要让事件3发生100次，就可以得到![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D) =100。如果在初始状态![\eta](https://www.zhihu.com/equation?tex=%5Ceta)=100，事件3只需要发生一次，就可以使![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D)=100。也就是说学习速率实际上表明了每一次事件3的发生对![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D)产生影响的大小。![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D)![y_{j}](https://www.zhihu.com/equation?tex=y_%7Bj%7D)这一项意味着如果![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D)与 ![y_{j}](https://www.zhihu.com/equation?tex=y_%7Bj%7D) 同时被激活，即 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 与![y_{j}](https://www.zhihu.com/equation?tex=y_%7Bj%7D)同时为正，那么![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D)将增大。如果![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D)被激活，而![y_{j}](https://www.zhihu.com/equation?tex=y_%7Bj%7D)处于抑制状态，即![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D)为正![y_{j}](https://www.zhihu.com/equation?tex=y_%7Bj%7D)为负，那么![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D)将变小。有意思的是，在当前所谓的大数据时代，数据本身就是事件。
这个过程非常重要，但又容易被忽略且不太容易理解，我需要再次表明以上过程实际上分为两个阶段，这两个阶段分别对应现在机器学习中的：训练和测试。在训练阶段，让某种看起来没什么联系的两个事件同时发生（或者仅仅观察确实已经存在的这些现象），从而让(或观察)神经元(或某种机器)的联系发生变化(减弱或增强)以建立或隔断它们之间的联系，这一阶段也可以称之为学习，尽管目前来看学习的目标不太明确，这里仅仅是建立一种强或弱关联。接下来进入一个测试阶段，通过让(或观察)一个事件发生，看一看这件事发生产生的效果。这也是被称之为人工智能三大学派之一的“连接主义(Conectionism)[2]”的核心思想， 这个思想的核心是不关心事件之间的因果关系，只关心相关关系。这种相关性本质上是一种通过训练得来的一种条件反射，这种相关性学习也被称为“端到端”学习（Ending-to-Ending Learning），这个名字意味着不关心中间的过程及原因，只关心结果。这种连接主义的思想也被一些人风趣的称为“吃啥补啥”派[3]。

条件反射的思想最早来自于获得诺贝尔奖的第一位生理学家伊凡·彼德罗维奇·巴甫洛夫的实验[4]。巴甫洛夫是专心投入学术研究的典型学者，他只专心研究，完全不注意衣食住行生活细节。他几乎按照与他同时代的卡哈尔提出的寻找配偶的原则[5]找到了知识女性，教育系的女学生谢拉菲玛（Seraphima Vasilievna）作为自己的妻子。 更厉害的是，他结婚时即约定，妻子不干涉他的研究，他则不负责家庭事务，并向妻子承诺，不饮酒、不打牌、不应酬，每年9月至次年5月，每周工作7天，只有暑假陪妻子到乡下度假。谢拉菲玛则把他们的生活料理得井然有序，专注似乎是伟大科学家所寻求的一个目标。
![](https://pic4.zhimg.com/v2-da0000f3f029765887c1ae0788b7fa17_b.jpg) 图 2 巴甫洛夫的狗
巴甫洛夫做了一个相当著名的实验[4]，中学生物课本上都有他的事迹，他利用狗看到食物或吃东西之前会流口水的现象，在每次喂食前都先发出一些信号（一开始是摇铃，后来还包括吹口哨、使用节拍器、敲击音叉、开灯……等等），连续了几次之后，他试了一次摇铃但不喂食，发现狗虽然没有东西可以吃，却照样流口水，而在重复训练之前，狗对于“铃声响”是不会有反应的。通过这种训练过程，巴甫洛夫的发现开辟了一条通往认知学的道路，让研究人员研究动物如何学习时有一个最基本的认识。这种把学习看成是一种条件反射的认识，也是机器学习思想的最初来源，直接影响了赫布建立第一个学习规则。然而涉及到动物的实验通常并不温和，他也曾把狗的食道切断，让食物在盘子和嘴巴之间循环，操纵称之为迷走神经东西，观察胃液的分泌情况。

巴甫洛夫为学习提供了一种解释，1943年麦卡洛克和皮茨设计了神经元的数学模型，借助条件反射思想，1949年赫布用数学模型描述了两个神经元之间的连接是如何变化的，这种变化现在被称之为学习律。只是这种学习的目的不太明确，看起来好像仅仅是通过训练过程，让两件事情的相关性发生变化罢了。然而，借鉴这个学习律像人一样去完成一些真正的任务的重担却落在了一个叫弗兰克罗森布拉特(　Frank Rosenblatt)的年轻人身上。

作为一名医生的儿子，罗森布拉特就读于美国布朗克斯科学高中(Bronx High School of Science) 是美国纽约最著名的是三所老牌重点高中之一，1938年建校以来，有8名诺贝尔奖获得者毕业于此。据我所知至少有两位图灵奖获得者(马文·明斯基(Marvin Minsky)[6], 和莱斯利·兰伯特(Leslie Lamport)[7], 马文·明斯基由于对人工智能的开创性贡献获得了人工智能领域的第一个图灵奖。兰伯特则以发明分布式关键技术而获得了2013年的图灵奖。实际上，要不是罗森布拉特去世的早，获得那个相貌平平的小银碗根本不是问题。现在看来美国的高中教育跟国内也不是一个档次的。没有证据表明罗森布拉特在高中时是否与比他高一年级的明斯基是否打过交道，但是后来的事实可以推测要么他们之间不认识，要么就是关系不怎么样。明斯基高中毕业后到海军服役一年，所以两人同年入读大学：明斯基到哈佛大学读物理，罗森布拉特则到康奈尔大学读心理学，都在1950年毕业。1956年，罗森布拉特从康奈尔大学获得博士学位，进入康奈尔航空实验室认知系统部从事心理学研究，得到美国海军研究办公室资助。在那里，罗森布拉特将迎来自己的人生巅峰。
![](https://pic3.zhimg.com/v2-47acdd19616e7771abe5fab4f87a442e_b.jpg)图3 罗森布拉特VS明斯基
1957年，罗森布拉特在航空实验室用IBM704计算机仿真了感知机算法。需要说明的是，从这个时候开始，存储程序计算机开始成为实现人工智能的有力工具。1954年推出的IBM704 [8]是一台意义非凡的机器。在它之前，没有计算机拥有能完成浮点运算的硬件。所有的浮点运算都只能用软件模拟，这是一个相当耗时的过程。IBM 704系统在硬件中同时支持变址寻址和浮点运算。硬件上的完善进一步崔生了高级程序语言的出现，704也是第一款采用 Fortran编程语言的机器。如果不是IBM开发了704计算机和Fortran语言，不知道什么时候才会出现C和Python这样的高级语言。更重要的是，704同时也是第一款采用磁芯内存的机器，在此之前，所有的计算机的输入都遵从18世纪法国人约瑟夫·雅卡尔为织布机所设计的方案[9]：用打了孔的卡片为机器编制程序。利用磁性材料进行存储的想法最早来自于美籍华人王安[10]，他造出了磁芯存储器并获得专利，并将这个专利卖给了IBM，靠这笔收入，他开创了一个传奇且教训深刻的王安公司，他的故事并非我们的文章所关心的内容，但是也从侧面证明20世纪50-90年代，产业界的激烈竞争集中在计算机的设计与开发，而非人工智能领域的竞争。在那个时代，没有哪一家公司能够和IBM相提并论。实际上，纵观整个计算机科学发展史，也没有哪一个公司的地位能和IBM相比。这个被称为“最强大的电子大脑”的IBM704，将协助罗森布拉特模拟真正的大脑，尽管这个大脑简单到只有一个神经元。
![](https://pic1.zhimg.com/v2-f278a4741bda041425b7a4caf3d70d4c_b.jpg)图 4 感知机原理及Mark I型感知机
我们在第1季的时候就介绍了感知机的基本原理，在那里我们从一种计算的角度，给定一个初始值并不断更新结果来达到计算的目的。实际上，由海军资助的这个项目的目标是造出一个会学习的机器(learning machine)，这实在是跟我们如今申请的很多项目类似，一些人总是喜欢看见硬件摆在那里才觉得踏实。罗森布拉特首先在704上进行了模拟，这时候感知机实际上只是一个程序。两年后，罗森布拉特将这个程序硬件化，并给他起名“Mark I Perceptron”（马克I型感知机）[11]这个机器占据了整个实验室，它包括三层结构，运作机制并不复杂。感知器的一端，400个光传感器模拟视网膜；传感器多次连接一组512个电子触发器，当它通过一个特定的可调节的兴奋阈值时就会像神经元一样激发。这些触发器连接到最后一层，当一个物体与感知器受训见过的对象相互匹配时，它就会发出信号。这个模型可以完成一些简单的视觉识别任务，如图4所示。这个硬件中， 直线的权值是由电位计(potentiometers)来表示，它的更新则依赖一些电动马达。英语国家的人们喜欢将他们某项重要发明称之为Mark I型。在1944年，美国海军就支持哈弗大学和IBM构建了一个自动按序控制计算器——Mark Ⅰ。更早一点，英国研制并在1916年第一次世界大战期间服役的Mark I坦克是世界上第一种正式参与战争的坦克。还有大家更熟悉的钢铁侠：MarkI是托尼·斯塔克创造的第一个机甲。但是，第一个神经网路的硬件并不归功于罗森布拉特，而是他的高中学长马文明斯基。

明斯基在哈弗大学毕业后来到普林斯顿攻读数学，在此，他建造了一台学习机Snare[6]，Snare是世界上第一个神经网络模拟器，其目的是学习如何穿过迷宫，其组成中包括40个“代理”（agent）和一个对成功给予奖励的系统。在Snare的基础上，明斯基综合利用他多学科的知识，解决了使机器能基于对过去行为的知识预测其当前行为的结果这一问题，并以“神经网络和脑模型问题”（Neural Nets and the Brain Model Problem）为题完成了他的博士论文。明斯基的一项伟大工作是与麦卡锡在1955年夏天于达特茅斯学院组织了一场称为人工智能夏季研讨会(Summer Research Project on Artificial Intelligence)的活动，如图5所示。这个会议的名字比它取得的所有成果都重要的多，第一次真正出现了AI这个词。英国数学家菲利普·伍德华德(Woodward)说这个词是它在1956年的时候访问MIT的时候告诉麦卡锡的[12]，我想他肯定是记错了时间！ 现在，人们把1955年夏天那个漫长会议作为人工智能诞生的日子。
![](https://pic4.zhimg.com/v2-f1603478f1328269980608df284d305f_b.jpg)图5 麦卡锡以及达特茅斯学院夏季研讨会的大楼
按正常发展，以神经网络毕业的明斯基应该在该领域取得更大的成绩，可是罗森布拉特的感知机却提前占了上风，几乎获得了所有人的关注，1958年夏，罗森布拉特和他的赞助方美国海军举行了新闻发布会。《纽约时报》[13]当时报道说：“海军透露了一种电子计算机的雏形，它将能够走路、说话、看、写、自我复制并感知到自己的存在……据预测，不久以后，感知器将能够识别出人并叫出他们的名字，立即把演讲内容翻译成另一种语言并写下来。那个时候，人们对“会学习的机器”抱有极大的热情，来自军方的资助和新闻媒体的关注使得罗森布拉特简直成了一个风云人物，他不断的抛头露面，简直是个明星。而此时的明斯基却显得没那么风光。基于并不十分清楚的原因，作为神经网络曾经的支持者，明斯基开始偏向反对神经网络，据说再一次会议上， 他和发明感知机的高中学弟大吵了一架，并宣称神经网络并不能解决人工智能问题。不管在当时还是现在，明斯基得基本观点几乎没错。但他随后所作的工作却让人工智能特别是神经网络的研究逐渐进入一个漫长的冬季。明斯基是在一次学术会议上跟西摩尔·帕普特（Seymour Papert）认识，而且非常有趣的是，当时两人提交了几乎一模一样的论文，明斯基邀请帕普特来到MIT，于是就开启了被后人调侃为“魔鬼搭档”的新组合,这主要是他们合著了一本具有极大争议的魔法书《感知机：计算几何导论》[14](Perceptrons: an introduction to computational geometry )简称《感知机》，这本书并不是推广感知机，而是宣判了它的死刑。 伴随着这本书的出现，人工智能的研究开始偏向以逻辑推理为主的另一个派别，“符号学派”，然而符号学派的好日子也并不长。

现在来看，这本书的最初版本具有明显的恶意，“罗森布拉特的论文没有多少科学价值” [3],在第一版里竟然这样直白的写道。我们很难回到那个时代去追问当事人的内心想法， 但是这种行为的原因是可以分析的，有两个很明显的原因，主观和客观。客观上，感知机确实存在不小的问题。我们在第2季中已经看到，感知机本身就是一条直线，它只能对线性可分的样本进行处理。对线性不可分的数据，感知机没法终止，并不是感知机算法找不到这条直线，而是这样的直线根本就不存在，感知机的弱点并不在其学习规则，而在于其简单的网络结构。然而可怕的是，明斯基和帕普特用一个最简单的却又明显不可分的异或问题来展示感知机的弱点，这给了感知机几乎致命的打击。主观上，我的猜测是明斯基和帕普特对罗森布拉特所取得的关注带有一些负面的情绪，说是嫉妒也不是不可以。

当时的计算机科学，并没有多少大数据的概念，那时只要能解决逻辑运算的问题，这个机器就威力无穷，就像MP模型一样。但如上一季看到的那样， MP模型的问题在于必须人工设计参数，才能完成复杂运算。罗森布拉特的感知机更加雄心勃勃，他宣称可以通过机器自动学习来实现很多操作(识别,运算,处理等)，当然得包括简单的逻辑运算了。实现这种运算，也并不需要人为设计，只需要把数据(事件)摆在这里，让机器自己学习，然后，感知机就可以作为一种最简单的逻辑运算器了。例如，为了实现与运算，我们只需要把与运算的输入和输出看作条件反射就行了,为了与麦卡洛克和皮茨得MP对比，我们将给出用感知机模拟“与门”的方法。首先“与运算”的规则很简单，对于所有的二元逻辑运算，总共就只有四种输入，分别对应于p1-p4这四个二维向量，向量的每一维表示了二元运算的一个输入，运算结果根据运算的定义而不同，在“与运算”的定义下， 运算结果的输出t1-t4也可以看作是这对输入的标记，如图6所示。
![](https://pic4.zhimg.com/v2-5c364a9a984735ee8472dcb4e893e9f7_b.jpg)图 6 感知机实现的与逻辑运算
对这个明显的线性可分问题而言，感知机得心应手，很快就能找到这样的直线，此时的直线就是一个与门运算单元。重要的是得到这条直线是如此的简单，几乎都是自动化的。依然有可爱的同学会觉得纳闷，既然与运算的规则这么简单，为何还要这感知机? 感知机去做这些确实有点牛刀杀鸡的感觉，但是仅仅通过数据或事件的训练来构造一个逻辑门的方法在那时可真是开天辟地的大事。你可以想到那些在平面上可以线性可分的逻辑门（例如，或门）都可以用感知机来模拟。

遗憾的是，异或门(XOR)对于罗森布拉特的感知机就没那么简单了。明斯基和帕普特就以此为例给了感知机致命一击，在这个问题中，数据与之前类似，pi表示向量，ti表示它对应的输出标记，可以看到，p2和p3是一类，p1和p4是一类，如图7所示。异或运算是实际上可以看作是二进制的不进位加法，因此意义非凡。
![](https://pic3.zhimg.com/v2-527ad4d76f074c90b8583e5f7a911622_b.jpg)图 7 感知机无法模拟的异或(XOR)运算

不用多说，这两类数据根本无法用一条直线把他们分开，感知机在这里完全不起作用。对这个简单的异或问题，感知机看起来几乎彻底失败了。实际上， 在当时，很多人已经知道，简单的感知机确实存在这样的问题，尽管明斯基和帕普特也承认，几层相互连接的神经元能做的可能够多，但是他们找不到研究这些神经元的方法，其他人也一样。

现在我必须不情愿的再次给出感知机相对严格的表达，从而才能看出它的问题所在，以及困难之处。在这里我将给出它与MP模型以及Hebb学习规则之间的关系。罗森布拉特的感知机表示的神经元与麦卡洛克和皮茨的神经元几乎一样，只是缺少了一个控制信号I(请参考第三季神经元的数学模型)。单个神经元本身也可以看作是一个激活函数g(z)。
![](https://pic4.zhimg.com/v2-d00a1e65e0054932c941a178c13a19e7_b.jpg)
![x_{1}](https://www.zhihu.com/equation?tex=x_%7B1%7D)*,...,*![x_{m}](https://www.zhihu.com/equation?tex=x_%7Bm%7D) 是输入， ![w_{1}](https://www.zhihu.com/equation?tex=w_%7B1%7D)*,...,*![w_{m}](https://www.zhihu.com/equation?tex=w_%7Bm%7D) 则是权值，注意上面的式子里有一个θ，激活函数本身的定义就反映了神经元的“全或无法则”。为了让这个式子更加简单，可以把阈值θ进一步放在z的一边，并定义， ![w_{0}](https://www.zhihu.com/equation?tex=w_%7B0%7D)  = -θ 以及  ![x_{0}](https://www.zhihu.com/equation?tex=x_%7B0%7D)  = 1, 这样，上面的式子变得更加整齐:
![](https://pic1.zhimg.com/v2-0de9b28598c10df4894f1421a7e39e30_b.jpg)
此时，感知机本身从结构上可以表示成如图8所示的一个神经元：
![](https://pic1.zhimg.com/v2-80e93f37534fbc6ae14a6813789145b8_b.jpg)图 8 用神经元表示的感知机

我们需要再一次强调，感知机的伟大之处在于它给出了一个自动获得w的学习规则，这个学习规则：
1）首先随机的初始化一个w值
2）对每一个样本 ![x^{i}](https://www.zhihu.com/equation?tex=x%5E%7Bi%7D) ，计算当前的激活函数值g(z)=g( ![w^{T}](https://www.zhihu.com/equation?tex=w%5E%7BT%7D)![x^{i}](https://www.zhihu.com/equation?tex=x%5E%7Bi%7D))，然后根据g(z)的值进行更新。具体而言，g(z)的输出记作 ![output^{i}](https://www.zhihu.com/equation?tex=output%5E%7Bi%7D) ,感知机利用![output^{i}](https://www.zhihu.com/equation?tex=output%5E%7Bi%7D)与![x^{i}](https://www.zhihu.com/equation?tex=x%5E%7Bi%7D)本身的标记 ![target^{i}](https://www.zhihu.com/equation?tex=target%5E%7Bi%7D) 之间的关系来更新w。更新w的规则反映了罗森布拉特的天才智慧，更新规则如下：
![](https://pic2.zhimg.com/v2-737508dfec6fdad54255a9a097f52181_b.jpg)
这个规则与 Hebb 学习规则形式上有些类似，只是将 w 以及![x^{i}](https://www.zhihu.com/equation?tex=x%5E%7Bi%7D)的标记引入到了更新规则中，这个规则保证了只有错分样本能够更新直线的参数 w。
尽管感知机只能在两类样本线性可分的情况下找到这条直线，但是并不是说它在线性不可分的时候一无是处，如果两个类不能被线性决策边界分隔，我们可以设置一个迭代次数(epochs）和/或一个阈值，用于容忍错误分类的数量。实际上，这个技巧是目前所有深度学习必须掌握的，因为他们面临与感知机几乎一样的尴尬情景，线性分类器无法解决线性不可分，而深度学习无法找到最优解，他们不能自己停下来，必须人为的进行干涉。图9给出了迭代多次的感知机如何应对线性不可分的问题。
![](https://pic3.zhimg.com/v2-8c3039f0b64d071e0512ccf363fc71c6_b.jpg)图9 感知机处理线性不可分问题
1960年，斯坦福大学的Bernard Widrow和它的博士生Ted Hoff在MP模型和感知机的基础上，提出了一个称之为自适应线性神经元: Adaptive Linear Neuron (Adaline)的模型[15]，这个模型比感知机更接近现在的神经网络，更加重要的是，这个模型为下一步为20多年后神经网络的复兴埋下了伏笔。这个神经元与感知机非常相似却又有明显不同，如下图10所示[16]：
![](https://pic2.zhimg.com/v2-5601e26a56c33f8ea33a50edfd1c45f1_b.jpg)图 10 感知机(上图)和Adaline的学习规则[16]
结构上看，它增加了一个量化器来保证输出满足要求。另一方面，为了让参数w能够更新，它引入了一个可微的线性激活函数来替代感知机的阶跃激活函数。这个可微的激活函数导致的结果就是，由这个激活函数，可以定义一个评价w值性能好坏的代价(误差)函数，从而让寻找w变成了一个简单的优化问题。千万不要小看个微小的改变，这个可微的代价函数的引入却几乎使现在的深度学习再也逃不出这个框架了。最小化代价函数也是其他类型机器学习算法的主要思想。Adaline引入的代价函数非常简单，就是最常见的均方误差函数(Sum of Squared Errors ：SSE)：
![](https://pic2.zhimg.com/v2-616938a87740aaa5c264cec56cd4b811_b.jpg)
这个均方误差的含义也十分明显，对给定的一个 w，希望所有的样本都能满足自身的标记target 和模型的输出 output 一致。如果代价 J 比较大，则修改 w 让 J 变小。平方的引入使得这个函数 J 本身变成一个凸函数。我们可以使用梯度下降(gradient descent)的方法来解决这个问题，根据梯度下降方法的基本原理(由于篇幅原因，我们将在后面的文章中详细介绍这个原理的背景知识)，该过程可以描述成如下的一个图形[16]，w 只要沿着 J(w)在 w 的梯度的相反方向移动，就可以让 J(w)的值下降，当 J(w)关于 w 的梯度下降到零(或者接近零)的时候，意味着 J(w)达到了一个局部最小值，如果 J(w)是一个凸函数，则局部最小就是全局最小。
![](https://pic4.zhimg.com/v2-ea3212d8e2f46e9ff051334432c39083_b.jpg)图11 代价函数J的梯度下降方法[16]
注意，此时w变成了代价函数的参数，我们的优化问题的目标是寻找J达到最小值时对应的参数w，而不是J的最小值本身。每一次更新的w可以看作求解根号2时的一个新的猜测，不过这个猜测依赖J关于w的梯度罢了。更具体一点，我们先考虑J(w)在一个方向上的偏导数。
![](https://pic4.zhimg.com/v2-05df25eb8f2b409f75b0c03e568d2967_b.jpg)
其中，(t = target, o = output), 根据梯度下降的学习规则，在这个方向上可以得到：
![](https://pic4.zhimg.com/v2-eaeecb69bd8dc8d684fe1c33917851a3_b.jpg)
在不同方向上的偏导数构成的向量就是梯度，Adaline的更新规则如下：
![](https://pic2.zhimg.com/v2-e85ab1522340f7dd83410f9f92bd4dcd_b.jpg)
其中，∆w表示J关于w的梯度向量的相反方向，非常巧合地是,单从形式上看，感知机的更新规则跟Adaline看起来很像，但是o的含义不同， 在感知机中， o就是激活函数的输出，它只有两个值1，-1。而在Adaline中，o是一个实数。同时，注意到Adaline的J(w)实际上依赖所有的样本的整体结果，而在感知机中， 每一次更新，只依赖一个样本。因此，如果你非得把感知机得更新也看成是J(w)的梯度下降的话(实际上是不能这么认为，因为感知机根本没有代价函数梯度下降的思想，而是直接给出了w的更新规则)，它则是类似随机梯度下降（ stochastic gradient descent）方法,该方法每次只利用一个样本来计算梯度，而Adaline属于批量梯度下降(batch gradient descent)方法，该方法需要利用所有样本来计算梯度。Adaline通过引入代价函数，利用了早已经提出的梯度下降方法来解决优化问题，实际上Adaline也可以改进成随机梯度下降，这样就跟感知机更加相似了。但是他们的之间依然存在本质的不同，原因就在于线性激活函数以及代价函数的引入。从形式上看，他们还是太像了，很多人都认为感知机可以直接写成梯度下降的形式，然而这是Adaline的功劳。按照我们第一季给出的计算的流程， Adaline的流程图如下所示,其中 ![▽J](https://www.zhihu.com/equation?tex=%E2%96%BDJ) 是用来表示梯度的通用记号。
![](https://pic4.zhimg.com/v2-86734f967723ed14f2db68e4cc5c9e87_b.jpg)
图12  Adaline引入代价函数和梯度下降的学习规则
以上的流程非常重要，几乎是到目前为止所有深度神经网络都共同遵守的流程。然而，由于Adaline和感知机本身的样子还是一条直线，它们存在一样的致命问题，都无法解决线性不可分的情况。1969年，当魔鬼搭档出版那本《感知机》后，这种线性分类器的日子都不太好过。那么到底如何解决线性不可分问题呢？明斯基和帕普特特别指出需要多层感知机——亦即现在所谓的多层神经网络——才可以完成这一任务，罗森布拉特的感知机被认为是单层感知机，也有看作双层的(下图左)，这取决于你怎么看待它。然而三层及以上的神经网络就像下面这个样子(下图右)。在多层神经网络中，多个感知机的输出作为另一个感知机的输入。关键的问题是罗森布拉特的学习算法对多层并不管用，Adaline的方法也不管用。那些处在多层神经网络中间层(隐含层)的神经元的权值(w1)，在当时无法进行调整。因为感知机和Adaline只有两层，他们直接建立了输出层对参数w是如何影响的，并确立了简单的更新规则，但是对多层网络而言，实在是很难想到网络的输出对隐含的层中参数w1的影响。这种困难在Aadline看来，就是无法容易的求出多层神经网络中代价函数对于所有参数w (w1及w2)的偏导数。这个问题对当时以及后来很长一段时期的人们，都是很困难的。在《感知机》出版后的十多年，也几乎没人(实际上有但被忽略了)找到一个比较好的求导数(梯度)的方法。连明斯基都以挑衅的口吻在书中写道：“证实(或推翻)把感知机推广到多层这个想法是没有希望的这一看法将是一个重要的研究内容。”
![](https://pic3.zhimg.com/v2-0cdcc7f7b32437c685bff5ef37ee63aa_b.jpg)图13 看作2层神经网络的感知机(左图)和多层感知机
1973年，英国科学研究委员会请一位叫James Lighthill的数学家[17]撰写了一份关于人工智能研究的评估报告，如下图所示：在这份著名的“Lighthill Report”中， Lighthill对诸如人工智能中的机器人技术和语言处理等基础领域的基础研究提出了强烈批评，并给出最终结论：“迄今为止，AI各领域的发现并没有带来像预期一样的重大影响” [17]。这为英国政府大范围终止对人工智能研究的决定奠定了基础。这份报告意味着从1974年开始，人工智能的研究进入漫长的冬季(AI Winter)。令人惊奇的是，就在1974年，哈弗大学的博士生Paul J. Werbos在他的博士论文《New Tools for Prediction and Analysis in the Behavioral Sciences》[18]中，详细描述了如何利用误差的反向传播来训练人工神经网络。然而哈弗毕竟不是MIT，这篇经典的文献在当时并没有引起太多反响，因为冬天已经来临，很多人工智能的研究人员得不到资助，甚至论文也无法发表。没有多少人意识到这篇文章的重要价值，这种情况除了得感谢《感知机》以外，就要数Lighthill的报告了，难以置信的是这两个文献的封面竟然如此相似！
![](https://pic1.zhimg.com/v2-7c061c819de10e1291628ab4bfd079e8_b.jpg)图14 1969年明斯基和帕普特的《感知机》 以及1973年Lighthill 的《人工智能》报告
然而，罗森布拉特甚至没有等到1974年。在1971年7月11日，在43岁生日的那一天，罗森布拉特在划皮艇的时候丧生。据说可怜的他连遗体也没有找到。很多人怀疑他是自杀，甚至一些人将他可能自杀的原因归结为《感知机》这本书给他带来的压力。在他去世前不久，还有一项不为多少人了解的重要发现。1971年罗森布拉特在一个关于天文学的期刊《ICARUS伊卡洛斯》上发表了一篇文章，探讨了如何通过测量其他恒星的星光，来寻找这些恒星周围的行星[19]。如果一颗行星从恒星和你的望远镜之间经过，你会看到光的强度略有下降。在此之前，天文学家大都尝试用天体测量的方法寻找行星，也就是测量由于行星引力导致的恒星位置的变化。罗森布拉特的这篇文章，在同一年被比尔•博鲁茨基发现，这坚定了博鲁茨基利用光学技术以及望远镜寻找行星的工作。博鲁茨基和他的同事们持续完善改进罗森布拉特的方法，作为著名的开普勒太空望远镜(Kepler)项目的提出者之一、首席科学家，比尔•博鲁茨基发起的开普勒探测器计划从2009年开始对银河系内10万多颗恒星进行探测，希望搜寻到能够支持生命体存在的类地行星。2015年，开普勒的最新成果是确认了第一个与地球近似大小、围绕一个类太阳恒星运转、公转轨道位于宜居带内的行星Kepler-452b。 在不可预见的将来，如果人类在宇宙中成功寻找到了智慧生物或与地球一样适宜人居的星球，我们也要铭记这位为人工智能做出杰出贡献且英年早逝的科学家。

尽管神经网络的研究进入寒冬，被人忽略的1974年反向传播思想的提出预示着一定会有另一些人在另外的合适时间及场合重新发现这个神经网络的救命稻草。我们还会看到一些学者的坚持，使得这个漫长得冬天并非那么寒冷，就像大家经常说的那样，冬天来了，春天还会远吗？
 参考文献：
[1]Donald_O._Hebb,[https://en.wikipedia.org/wiki/Donald_O._Hebb](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Donald_O._Hebb)
[2] 佩德罗·多明戈斯，终极算法：机器学习和人工智能如何重塑世界，中信出版社，2017年1月
[3] 尼克，人工智能简史 人民邮电出版社，2017.12
[4] 伊万·彼得罗维奇·巴甫洛夫，[https://baike.baidu.com/item/](https://link.zhihu.com/?target=https%3A//baike.baidu.com/item/)伊万·彼得罗维奇·巴甫洛夫
[5] 圣地亚哥·拉蒙-卡哈尔，致青年学者，新华出版社，2010年。
[6]Marvin Minsky , [https://en.wikipedia.org/wiki/Marvin_Minsky](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Marvin_Minsky)
[7] Leslie Lamport , [https://en.wikipedia.org/wiki/Leslie_Lamport](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Leslie_Lamport)
[8] 马跃 译 编程语言原理 第10版 清华大学出版社 2013年
[9] Joseph Marie Jacquard, [https://baike.baidu.com/item/Joseph](https://link.zhihu.com/?target=https%3A//baike.baidu.com/item/Joseph)
[10] 王安, [https://baike.baidu.com/item/](https://link.zhihu.com/?target=https%3A//baike.baidu.com/item/)王安/4457
[11]  Rosenblatt's Contributions. csis.pace.edu.
[12] John McCarthy [https://en.wikipedia.org/wiki/John_McCarthy_(computer_scientist)](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/John_McCarthy_%28computer_scientist%29)
[13]  New Navy Device Learns By Doing. nytimes.com.
[14] Minsky, Marvin; Papert, Seymour. Perceptrons: An Introduction to Computational Geometry. MIT Press.
[15]  1960: An adaptive "ADALINE" neuron using chemical "memistors"
[16] Sebastian Raschka , Single-Layer Neural Networks and Gradient Descent [http://sebastianraschka.com/Articles/2015_singlelayer_neurons.html](https://link.zhihu.com/?target=http%3A//sebastianraschka.com/Articles/2015_singlelayer_neurons.html)
[17] James Lighthill , [https://en.wikipedia.org/wiki/James_Lighthill](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/James_Lighthill)
[18] Werbos, P.. Beyond Regression: New Tools for Prediction and Analysis in the Behavioral Sciences. PhD thesis, Harvard University, 1974
[19] Andrew Grant，送望远镜上天，去寻找外星行星！
[https://www.guokr.com/article/425871/](https://link.zhihu.com/?target=https%3A//www.guokr.com/article/425871/)

推荐阅读

[1][机器学习-波澜壮阔40年](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483705%26idx%3D1%26sn%3Dc6e7c4a2e14a2469308b41eb60f155ac%26chksm%3Dfdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0413.

[2][学好机器学习需要哪些数学知识？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483713%26idx%3D1%26sn%3D1e7c81381d16806ac73e15691fe17aec%26chksm%3Dfdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0417.

[3] [人脸识别算法演化史](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483726%26idx%3D1%26sn%3D9fef4cc1766ea4258749f8d40cc71a6e%26chksm%3Dfdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0420.

[4][基于深度学习的目标检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0424.

[5][卷积神经网络为什么能够称霸计算机视觉领域？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0426.

[6] [用一张图理解SVM的脉络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483937%26idx%3D1%26sn%3D84a5acf12e96727b13fd7d456c414c12%26chksm%3Dfdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0428.

[7] [人脸检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483950%26idx%3D1%26sn%3Da3a5b7907b2552c233f654a529931776%26chksm%3Dfdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0503.

[8] [理解神经网络的激活函数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483977%26idx%3D1%26sn%3D401b211bf72bc70f733d6ac90f7352cc%26chksm%3Dfdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI2018.5.5.

[9] [深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484037%26idx%3D1%26sn%3D13ad0d521b6a3578ff031e14950b41f4%26chksm%3Dfdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0508.

[10] [理解梯度下降法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0511.

[11] [循环神经网络综述—语音识别与自然语言处理的利器](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484310%26idx%3D1%26sn%3D0fc55a2784a894100a1ae64d7dbfa23d%26chksm%3Dfdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0515

[12] [理解凸优化](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】 SIGAI0518

[13] [【实验】理解SVM的核函数和参数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484495%26idx%3D1%26sn%3D4f3a6ce21cdd1a048e402ed05c9ead91%26chksm%3Dfdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0522

[14][【SIGAI综述】行人检测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484523%26idx%3D1%26sn%3Dddaa70c4b92f6005d9bbd6b3a3fe4571%26chksm%3Dfdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0525

[15] [机器学习在自动驾驶中的应用—以百度阿波罗平台为例(上)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484540%26idx%3D1%26sn%3D733332042c31e1e18ad800f7f527893b%26chksm%3Dfdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0529

[16][理解牛顿法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0531

[17] [【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484658%26idx%3D1%26sn%3Df5c9f92c272c75883bf8e6f532559f11%26chksm%3Dfdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI 0601

[18] [大话Adaboost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0602

[19] [FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484711%26idx%3D1%26sn%3Dbb7644e101b5924f54d6800b952dc3aa%26chksm%3Dfdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0604

[20] [理解主成分分析(PCA)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484754%26idx%3D1%26sn%3Db2c0d6798f44e13956bb42373e51d18c%26chksm%3Dfdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0606

[21] [人体骨骼关键点检测综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D1%26sn%3Dceafb54203f4e930ae457ad392b9f89c%26chksm%3Dfdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0608

[22][理解决策树](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484827%26idx%3D1%26sn%3D043d7d0159baaddfbf92ed78ee5b1124%26chksm%3Dfdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0611

[23] [用一句话总结常用的机器学习算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484859%26idx%3D1%26sn%3D2c4db22fb538953a62a90983e3e1f99d%26chksm%3Dfdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0611

[24] [目标检测算法之YOLO](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484909%26idx%3D1%26sn%3Dc02ee17e5175230ed39ad63e73249f5c%26chksm%3Dfdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0615

[25] [理解过拟合](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484954%26idx%3D1%26sn%3Dc28b7f07c22466e91b1ef90e9dbe3ad1%26chksm%3Dfdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0618

[26][理解计算：从√2到AlphaGo ——第1季 从√2谈起](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484981%26idx%3D1%26sn%3Dd3003468b9853851923844812993e060%26chksm%3Dfdb69ba2cac112b4dac620d52100ebd033eb679f29340726a67297c4d6980b16c7cc91122028%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0620

[27] [场景文本检测——CTPN算法介绍](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485005%26idx%3D1%26sn%3D0d4fb43b8db2a8046c64a9cfcbf3f478%26chksm%3Dfdb69bdacac112cce05c8b735b4f8b1ccf2348bea55a30af2055fc328958bb8f1ffd0f819bd2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0622

[28] [卷积神经网络的压缩和加速](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485042%26idx%3D1%26sn%3Dcdcf8d4b07acf64c7a6f5f7c1a731a12%26chksm%3Dfdb69be5cac112f377766984afb87313c1e1c58d94c80005f0f6f6af61ee5a4bd1bf6c6157b6%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0625

[29] [k近邻算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485074%26idx%3D1%26sn%3D0ebf1bf8f49e9c46075fe3803d04c95d%26chksm%3Dfdb69b05cac112132d280c70af3923ca4c3cccfa5fcd8628b79d4b246b3b2decbc80a180abb3%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0627

[30][自然场景文本检测识别技术综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485142%26idx%3D1%26sn%3Dc0e01da30eb5e750be453eabe4be2bf4%26chksm%3Dfdb69b41cac11257ae22c7dac395e9651dab628fc35dd6d3c02d9566a8c7f5f2b56353d58a64%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0627

[31] [理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485155%26idx%3D1%26sn%3D990cc7400751c36e9fef0a261e6add2a%26chksm%3Dfdb69b74cac112628bdae14c6435120f6fece20dae9bf7b1ffc8b8b25e5496a24160feca0a72%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0704

[32] [机器学习算法地图](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485306%26idx%3D1%26sn%3Dfc8cc8de313bdb61dcd39c1dedb240a4%26chksm%3Dfdb69aedcac113fb4b18c74248a313536ded50bade0e66b26f332ab247b148519da71ff2a3c0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0706

[33] [反向传播算法推导-全连接神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485446%26idx%3D1%26sn%3D57d7d866443810c20c4ea2c6ee8018cc%26chksm%3Dfdb69591cac11c8773638b396abe43c0161e4d339f0fa845e54326be3e8c4933a3b6a2713dae%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709

[34] [生成式对抗网络模型综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485551%26idx%3D1%26sn%3D213f48c4e55bee688cf0731097bb832c%26chksm%3Dfdb695f8cac11ceef3ef246c54d811dd64d8cc45fc75488c374c7aa95f72c1abfb55555ef0b7%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709.

[35][怎样成为一名优秀的算法工程师](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485570%26idx%3D1%26sn%3D5e71437a6d3ddf6d05144fc99e7633cc%26chksm%3Dfdb69515cac11c030cf713ec85293b7ad3bbe0f20d43fc2729cc976ff628aabf636834ccd904%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0711.

[36] [理解计算：从根号2到AlphaGo——第三季 神经网络的数学模型](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485592%26idx%3D1%26sn%3D1c5236972402ea8cb168161bc41e8e7e%26chksm%3Dfdb6950fcac11c19ad047e7cb9ced96447a85b41e21b10789a86ae4a211e4fb2ca1f911a7fc5%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0716

[37][【技术短文】人脸检测算法之S3FD](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485609%26idx%3D1%26sn%3Dd9068aecfbf150b40103210de538fea9%26chksm%3Dfdb6953ecac11c28361435306a7a09632ea79000abf1bf626e50afb3cda48eb3e47b96c6e7cd%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0716

[38] [基于深度负相关学习的人群计数方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485617%26idx%3D1%26sn%3D7955bfefc8e4b28221ae5247812f8235%26chksm%3Dfdb69526cac11c30e1051edc4378d7dfdedf46925236dbe33e7912b4bea882e515f074eee4c9%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0718

[39] [流形学习概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485668%26idx%3D1%26sn%3Df70547fc671d164e28fddcea6473524d%26chksm%3Dfdb69573cac11c65ee9fc98ac8fad093282a3d244748e7c88541c133ac55a32cb07472dc80e0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0723

[40] [关于感受野的总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485700%26idx%3D1%26sn%3Dc0425495fe0ae9cb2120dbcb246f49b1%26chksm%3Dfdb69493cac11d8542f7a8e662a7ecdeece4fd2270c71504023e8b58128575d1e4fdadf60cf5%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0723

[41] [随机森林概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485718%26idx%3D1%26sn%3Dc05c217af81173ae2c1301cbda5f7131%26chksm%3Dfdb69481cac11d975d86ff2e280371963d04b5709dfa0a9e874d637b7cf3844cad12be584094%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0725

[42] [基于内容的图像检索技术综述——传统经典方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485741%26idx%3D1%26sn%3Df8217e523d54842daaa4be38fa1792eb%26chksm%3Dfdb694bacac11dacfd8d7e4882166e2774c4685c043379ce0d2044e99c3b3c8b0140480bbf8e%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0727

[43] [神经网络的激活函数总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485762%26idx%3D1%26sn%3De1e9fc75b92999177d3c61c655b0e06e%26chksm%3Dfdb694d5cac11dc37dac1a7ce32150836d66f0012f35a7e04e3dceaf626b8453dc39ee80172b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0730

[44] [机器学习和深度学习中值得弄清楚的一些问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485811%26idx%3D1%26sn%3Da87082f0e47b01bb8c22443ba5b1139c%26chksm%3Dfdb694e4cac11df20ea1deb8b55cf297ad44e48a4c7ca45cfce387284143403fcd342ac98ec0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0802

[45] [基于深度神经网络的自动问答系统概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485842%26idx%3D1%26sn%3Dd7485054d6e93225b6ac0c77f8706cf7%26chksm%3Dfdb69405cac11d1355b84f692c2cbe49a3852a10e074b6941c95618598caea6ed64103c4ee4c%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0803

[46] [反向传播算法推导——卷积神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485971%26idx%3D1%26sn%3D632e3c135ead5f0ac730eee5b6a05647%26chksm%3Dfdb69784cac11e9258f67312fa2798e9c8210b8f77741a3d7bab4c1ccfa1c1f66632183f4f26%26token%3D1469111007%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0806

[47] [机器学习与深度学习核心知识点总结 写在校园招聘即将开始时](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486105%26idx%3D1%26sn%3Dd0b33e7e23b0e2fc442bd6b3e2a9d952%26chksm%3Dfdb6970ecac11e18085ea36f3b654028b2d4ba33a0cdc89c4ea25ac81570969f95f84c6939ac%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取 码】SIGAI0808

[48] [理解Spatial Transformer Networks](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486133%26idx%3D1%26sn%3D31c64e83511ad89929609dbbb0286890%26chksm%3Dfdb69722cac11e34da58fc2c907e277b1c3153a483ce44e9aaf2c3ed468386d315a9b606be40%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0810

[49][AI时代大点兵-国内外知名AI公司2018年最新盘点](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486140%26idx%3D1%26sn%3D6157951b026199422becca8863f18a17%26chksm%3Dfdb6972bcac11e3d7427847df818bd6cc7893a261daa0babe0b11bd01a403dc4f36e2b45650e%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0813

[50] [理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486202%26idx%3D1%26sn%3Dbf66be1e71272be42508e557ed93acbf%26chksm%3Dfdb6976dcac11e7b9d0f878233e8d9907825e57851e7a095f1be3a23abc9327370a77f4e2c03%26token%3D283993194%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0815

[51] [基于内容的图像检索技术综述--CNN方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486268%26idx%3D1%26sn%3Ddff53feb3d78ea7698f70bede08b3b19%26chksm%3Dfdb696abcac11fbdcde5f4acc72d34c14119a21234b9e6cd0c1886b85c330e7f77d6e3da9122%26token%3D283993194%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0817

[52][文本表示简介](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486289%26idx%3D1%26sn%3Da312c951c943ad104c722e6c681823b6%26chksm%3Dfdb696c6cac11fd01224a68b9d67fcf0043ff2de0ec67f7bfd98bacf691c8eaf221cbca9b4d6%26token%3D1485183944%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0820

[53][机器学习中的最优化算法总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486354%26idx%3D2%26sn%3D1afd5f272536b6771b5e1e224e8414ec%26chksm%3Dfdb69605cac11f13f2a16c8748e333045e99497dc03ca9f4741723204e7d8348e969375379ae%26token%3D1580317399%26lang%3Dzh_CN%23rd)【获取码】SIGAI0822

[54][【AI就业面面观】如何选择适合自己的舞台？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486371%26idx%3D1%26sn%3D955286243fe23ff58d5a090c166d83f6%26chksm%3Dfdb69634cac11f2251793b40432b5429828f5e72411e2ae48c1ae282117f2d1b4067c14fc889%26token%3D1383761413%26lang%3Dzh_CN%23rd)【获取码】SIGAI0823

[55][浓缩就是精华-SIGAI机器学习蓝宝书](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486392%26idx%3D1%26sn%3D81cc65e42418bf846167ab80ae1dd4f4%26chksm%3Dfdb6962fcac11f39d3c62bc06e8a0708a24a024e53aef38e1bd716dcd1433fa89a0c0fe422df%26token%3D867271861%26lang%3Dzh_CN%23rd)【获取码】SIGAI0824

[56][DenseNet详解](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486433%26idx%3D2%26sn%3D9858164d290b60c03081ee0b04d57a76%26chksm%3Dfdb69676cac11f60f48b9bce3017e2776767f3f9c2cc980e03c07dabc076a9ce8da4b906760d%26token%3D1199765642%26lang%3Dzh_CN%23rd)【获取码】SIGAI0827

原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的

