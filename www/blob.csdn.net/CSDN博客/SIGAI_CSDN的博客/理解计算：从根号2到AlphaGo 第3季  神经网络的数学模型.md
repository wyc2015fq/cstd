
# 理解计算：从根号2到AlphaGo 第3季  神经网络的数学模型 - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2018年07月14日 08:09:50[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：393


```python
文章《理解计算：从根号2到AlphaGo 第3季  神经网络的数学模型》系SIGAI原创，仅供个人学习使用，未经允许，不得转载，不能用于商业目的。如需获取原版PDF全文，可搜索关注VX公众号SIGAICN。
```
理解计算：从根号2到AlphaGo
第3季 神经网络的数学模型
[SIGAI](http://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485074%26idx%3D2%26sn%3D23469e491b1ad770391cc977e18e8fdc%26chksm%3Dfdb69b05cac1121325b01ea659730c00336817b882221ce40c21c5ec6419738f2f958b76aa69%23rd)特邀作者：twinlj77
2018.7.13
1910年，英国哲学家伯特兰·罗素（Bertrand Russell ）和其老师怀特海（Alfred North Whitehead）合著的《数学原理》一书问世，这本书是如此的深奥，尤其对中国人而言，因为直到100多年后的今天，仍然没有完整而权威的中译本(能看懂的人确实很少)。不过想想现在连一般的科普作品都翻译得如此垃圾，我们真应该庆幸。这本不朽的名著为两个年纪相差25岁的科学家建立了一条联系一生的纽带，同时也为神经计算提供了最初的思想来源。 沃尔特·皮茨（Walter Pitts）是很多取得巨大成就但命运坎坷的科学家中的一员。12岁时经常被人欺负(家人和同学)，作为一个孩子，他最幸福的时光是在图书馆里度过的。他曾花了三天时间读完了2000页的《数学原理》，并写信给罗素指出了书中的几处错误[1]。心胸宽广的罗素甚至邀请皮茨去剑桥大学当他的研究生。15岁时，皮茨离家出走，从此他就再也没有见过他的家人，他要到芝加哥大学寻找他的偶像罗素。在芝加哥大学，哲学家，自然语言处理的先驱鲁道夫·卡尔纳普（Rudolf Carnap）给了他很多帮助，为皮茨谋了打扫卫生的差事[2]，这一幕简直就是现实版的《心灵捕手》。皮茨在旁听罗素的课的时候，结识了杰罗姆·莱特文（Jerome Lettvin），一名年轻的医学系学生。莱特文将皮茨介绍给了沃伦·麦卡洛克（Warren McCulloch），一位比皮茨年长25岁的教授，从此以后，来自不同经济阶层的两人一起生活、一起工作、甚至一起死亡。很快，他们将创造第一个关于精神的机械论理论——神经科学的第一种计算方法，现代计算机逻辑设计以及人工智能的支柱[1]。
![](https://img-blog.csdn.net/20180714075017370?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图1 Walter Pitts (左)和 Warren McCulloch
皮茨和麦卡洛克还有一个共同的偶像，戈特弗里德·莱布尼茨（Gottfried Leibniz），就是发明微积分的数学家莱布尼茨。作为哲学家或逻辑学家，他认为：“我们的所有观念（概念）都是由非常小数目的简单观念复合而成，它们形成了人类思维的基本单元:字母。复杂的观念来自这些简单的观念，是由它们通过模拟算术运算的统一的和对称的组合。” 这种复杂观念由符号所表示的基本单元进行逻辑计算的结果的思想，实际上也指导了《数学原理》创作。 《原理》的宏大目标是通过用无可争论的简单的基本逻辑来建立整个数学体系。从命题入手，罗素和怀特海德采用了与、或、非三种基本逻辑运算来将命题连接成越来越复杂的网络。就是利用这些简单的命题，他们完整的推导出了整个复杂的现代数学。从《数学原理》开始，逻辑学从哲学中走出来，开始渗透到数学的各个方面，对计算领域产生了巨大的影响。
由于麦卡洛克本身就是一个神经生理学家，我们在第2季中介绍的关于神经元的结构和功能在当时是神经生理学开创性的工作，这个工作对麦卡洛克产生了深刻影响。当时已经知道的是大脑里的每一个神经细胞只会在达到某个阈值时才被激发, 也就是说必须有足够多的相邻神经细胞通过神经元的突触向它传递信号，它才会发放出自己的电脉冲，即“全或无法则”。对于也曾认真学习过《数学原理》的麦卡洛克来说，这种设置就是二元的，神经元只有激发和不激发两种状态。他发现神经元的信号就是一个命题，而神经元的工作类似于逻辑门，它接受多个输入然后产生单一的输出。通过改变神经元的激发阙值，就可以让它执行「与」、「或」以及「非」的函数（功能）[1]。
需要声明的是，生物学上的神经活动是非常复杂的，就神经元从突触分两种类型，一些是兴奋性的(excitatory),另一些则是抑制性(inhibitory)的，之前我介绍过，这是谢灵顿的发现。兴奋性和抑制性两类突触获得外界的刺激会让神经细胞产生两种状态，激活态或是非激活态，神经细胞被激活的话，则会沿着轴突产生一种脉冲信号，通过另一端的突触把刺激作用传递给其他神经细胞。实际上,我们要清醒地认识到，对于神经细胞而言，它可不认为自己有两种状态，它只是适时地发送脉冲信号给其他神经元罢了。这种脉冲信号的产生符合我们之前讲授“全或无”法则，也就是突触获得外界刺激要么使细胞产生脉冲，要么就不产生。还有一个关键问题是，触突的兴奋和抑制作用到底是如何综合产生影响的，这一点非常复杂。为了用数学工具模拟神经活动，必须进行简化。首先假设细胞是否产生脉冲对应于一个开关，这个开关只有两种状态：开和关，也就是二进制状态1或者0。其次，细胞接收到的输入刺激也只有0，1两种模式，真正发挥作用的是，处于“开(1)”状态的兴奋性的输入的个数与抑制性输入个数的关系。最简单的假设是只有一个抑制性的输入，而且它产生的作用要强于多个兴奋性输入的作用，也就是说抑制性输入为1时，神经细胞的输出状态只能是0，当抑制性输入为0时，则需要看兴奋性输入为1的个数是否超过一定阈值T，如果超过一定阈值T，神经细胞才真正产生脉冲，即输出的状态才能是1。阈值在这里起到的作用完全对应“全或无”准则，不过这里需要有一个抑制性输入作为前提，抑制性输入可以看作一种控制信号。需要指出的是，神经细胞响应输入带来的刺激也是需要一个时间过程，也就是在t时刻的输入刺激，将在t+1时刻产生输出。
我们接下来将会看到这些基本假设是如何与逻辑门产生联系，并会举出一个具体的例子来看看如何利用神经元来进行逻辑运算。实际上，我们只需要几个简单的神经元就可以表示最基本的逻辑门，包括「与」、「或」以及「非」，如下图所示的表示是由人工智能的先驱马文明斯基提出的，就是他将感知机和Rosenblatt推进了深渊。
![](https://img-blog.csdn.net/2018071407523748?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图2 神经元代表的逻辑门[4]
上面的每一个逻辑门就是一个神经元，例如，带箭头的输入表示兴奋性输入，以圆圈为端点的输入表示了抑制性输入，神经元中的数字(OR:1,AND:2,NOT:0)表示了这个神经元的阈值。以AND门为例，当两个兴奋性输入都为1时，他们的和大于等于阈值2，同时没有抑制性输入，因此神经元的输出就是1。当其中任意一个输入为0，输入的和将小于阈值2，因此将输出0。这就是利用神经元模拟AND门的。OR门与此类似，我们再看一下NOT门，在非门中存在一个抑制性输入，而没有兴奋性输入，并且阈值为0，抑制性输入为0，没有兴奋性输入，由于阈值为0，因此，将产生1的输出，而当抑制性输入为1的时候，神经元直接输出0，这个时候就根本不用计算了，这是一种规定。由于我们用神经元表示了基本的逻辑门，因此，我们也可以用神经元表示由基本逻辑门构建的任何东西。
我们又把延迟的时间因素忘记了，但这个延迟的时间是如此重要，它使得冯诺伊曼的存储程序计算机变的可能。我们可以通过叠加一些神经元来实现一个称之为延时门的逻辑门，如下图所示的的阈值为2的神经元，只看这单一的神经元的话，起初只有一个兴奋性输入，因此在下一时刻这个神经元的输出会是0。接下来，我们看下方另一个阈值为1的神经元，这个神经元一开始就有一个称为start的兴奋性输入(此时你仍可以忽略最下方的名为stop的抑制性输入)，特别的是，这个神经元的输出又反馈给本身作为输入了。实际上，没有什么能够阻止神经元将自己的输出作为它自己的输入，尽管对于单个神经元来说有点奇怪，但是当几个神经元串在一起的时候，它们形成环路几乎是不可避免的。在不考虑抑制性输入stop的情况下，下方的神经元每隔一个时钟单元，就会输出1，将这个输出连接到上方的神经元的时候，最终的Output看起来就好像延时了两个时间单元一样。由于存在环路，这个过程将会一直循环下去，我们必须通过引入一个抑制性输入Stop，让它作为整个过程的开关去控制一切。你可以在下方阈值为1的神经元上再增加一个神经元从而让整个输出延迟三个时间单元。
![](https://img-blog.csdn.net/20180714075442639?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图3 带有回路的神经网络[4]
这种延时很重要，延时t时刻，意味着在这个信号可以在逻辑门中存储t时刻，这实际上意味着神经元具有了记忆功能。这种结构其实就是现在的寄存器的原型。类似的神经网络可以实现更复杂一点的电路，例如下图的神经元网络就实现了一个二分频电路，即在时钟每触发2个周期时，电路输出1个周期信号。
![](https://img-blog.csdn.net/20180714075553261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图4 (a)二分频神经网络[4]及其(b)波形仿真
以上的这些都来自于麦卡洛克和皮茨在1943年合著的那篇A Logical Calculus of the Ideas Immanent in Nervous Activity [6]即《神经活动中内在意识的逻辑演算》，简称《逻辑演算》，任性的是，这篇文章注明的参考文献只有卡尔纳普的《语言的逻辑句法》(皮茨据说也是很快就看完并做了大量笔记，这让卡尔纳普印象深刻)、希尔伯特和阿克曼的《数理逻辑原理》，以及怀特海和罗素的《数学原理》，但是论文中他们却说：“图灵机能够计算的数也能够被这样的网络所计算……这就从心理学的角度，论证了图灵关于可计算性的定义”，很明显，1936年阿兰图灵发表的创世之作《On Computable Numbers, with an Application to the Entscheidungsproblem》(论数字计算在决断难题中的应用)一定也对麦卡洛克产生了影响，更从一个角度反映出图灵是多么伟大，他几乎是从纯数学的角度出发，构思出一种将计算和自动化联系起来的机器，后人称之为“图灵机”。实际上，直到20世纪之前，人们普遍认为，所有的问题类都是有算法的就像我们在本文第一季看到的求解根号2[7]一样，人们的计算研究就是找出算法来。然而，20世纪初的时候，许多问题已经过长期研究仍然找不到算法，即它们是不可计算的。尽管有很多人(哥德尔，丘奇)已经表明了凡“算法可计算函数都是一般递归函数”(你可以不用理解它)，但是图灵创造性的把计算归结为最简单、最基本、最确定的机械动作，执行这些机械动作的机器就是图灵机，并得出结论：“那些算法可计算的问题就是这种自动机能计算的问题”。遗憾的是，我们现在仍然不能区分哪些问题是可计算的，哪些问题是不可计算的。但是将计算和自动化机器联系起来的思想使得图灵被公认为“计算机科学之父”。然而，由于图灵机本身只是一个构想，麦卡洛克和皮茨的工作验证了通过神经元表示的逻辑门实现计算是一条切实可行的方法。这个方法为被称为“计算机之父”的冯诺伊曼去实现真正意义上的计算机提供了几乎唯一可以借鉴的思路。
在我们所学的大学计算机文化基础课程中，唯一让你印象深刻的可能就是“第一台通用计算机ENIAC是由宾西法尼亚大学发明的“，发明人莫克利（JohnW.Mauchly）和艾克特（J.PresperEckert）估计你也记不起来。这台机器更具有象征意义，它跟我们现在的计算机，差别很大，它竟然采用了十进制进行计算，而且每次计算不同的问题，需要几个操作员花几周的时间重新连接电线和开关！1945年6月，冯诺依曼撰写了一份历史性文件《EDVAC报告书的第一份草案》（First Draft of a Report on the EDVAC），这是第一份公开发表的关于存储程序概念的二进制计算机器——现代计算机。他建议计算机应该以皮茨和麦卡洛克的神经网络为模型，并且用真空电子管代替神经元作为逻辑门，这些逻辑门按照神经网络的方式组织起来就可以进行任何计算了。更为关键的是，为了使得程序本身能够像输入的数据一样保存起来（避免重新连接线路），他还建议计算机必须有另外一样新东西：内存，他说：需要一种「一个自激发元素将保持这种激发状态」的结构，其实这就是皮茨和麦卡洛克论文中出现的神经元构成的回路所产生的作用：记忆。这篇奠定了到目前为止所有计算机逻辑结构的EDVAC报告只有一篇参考文献：麦卡洛克和皮茨的《逻辑演算》。
在《逻辑演算》的原始论文中，延时门，或门和与门分别表示为下图的(a)(b)(c)。这个图据说是麦卡洛克的女儿绘制的[3]。
![](https://img-blog.csdn.net/20180714075719343?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图 5 《逻辑演算》中的逻辑门[6]
我们大多数学习计算机的人，老师总会教导我们计算机本身就是0，1构成的，不管软件还是硬件，但是大家(特别是有软件工程背景的人)经常忽略的是在计算机中，时序电路的引进，才使得存储程序这一设计能够实现。
1943年，莱特文将皮茨带到了维纳在MIT的办公室，简单的交流就使得这位控制论的创始人认为皮茨「毫无疑问是我见过的最厉害的科学家。」[1]对于皮茨这个连高中都没有毕业的流浪儿来说，维纳承诺给予皮茨MIT的数学博士学位，并邀请皮茨到MIT与他一起工作。就算在那个被称之为“控制论学家“的圈子里面(维纳，冯诺依曼，麦卡洛克、莱特文)，皮茨也是他们中的天才。维纳让皮茨继续改进他的神经网络，皮茨怀疑对于一个初始的随机的神经网络而言，”随着长时间对神经元阙值的调整，这种随机性会渐渐让位于有序性，而信息就涌现出来了“[1]。他着手用统计力学来为这个过程建模。维纳激动地鼓励他，他知道如果将这样一个模型植入一台机器的话，它就将能够「学习」[1]。这段描述简直就是现在深度学习所做的一切。但是对神经元的调整这件事当时还不能自动进行，这件事还必须等到1957年感知机出现的时候。实际上，为了更加清楚地了解这一点，麦卡洛克和皮茨的神经元模型还可以被描述成下面的图形：
![](https://img-blog.csdn.net/20180714075824762?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图6 神经元的结构图
这种表示与现在神经网络中的表示几乎没什么差别了，图（a）表示神经元从外界获得输入x1，…xn 通过箭头表示的突触，传给神经元，神经元的输出是oj，图 (b)则更进一步将箭头表示的突触根据不同的性质以及强度，为神经元输入增加了一个加权和，这使得一些输入对神经元的脉冲输出产生更大影响，图(c)则将通过增加一个阈值函数将“全或无“法则引入到神经元结构中，图c得神经元结构可以表示为如下的公式：
![](https://img-blog.csdn.net/20180714081417733?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中，y表示神经元的输出，E表示神经元获得外界刺激的和，T表示神经元设定的阈值，I表示抑制性突触，它起到的作用就是图2中非门的那个控制信号。当我们精心挑选权值参数W，阈值T，以及控制信号I的话，就能让神经元实现逻辑运算了，例如：

```python
设T=2，I=0，W1=1，W2=1， 那么 E=x1W1+x2W2=x1+x2
当x1=1，x2=1，E=1+1=2，   触发y=1
当x1=1，x2=0，E=1+0=1，   不触发y=0
当x1=0，x2=1，E=0+1=1，   不触发y=0
当x1=0，x2=0，E=0+0=0，   不触发y=0
```
这就实际上实现了x1和x2之间的“逻辑与“运算。
这个模型看起来威力无穷，但是一个最关键的问题是必须依靠人去精心设计参数的值以完成某项计算功能，自动化的过程并不彻底。如果神经网络的结构和参数都能根据不同的任务自动确定，这样构建的机器被认为具有学习功能。然而，如何仅仅自动化的选择参数，使得单个神经元具有学习能力一直到 罗森布拉特的感知机才提出解决办法，我们已经知道，这个办法跟求解根号2几乎没区别，但是感知机的重要性是不言而喻的。如果能够自适应地组建神经网络的结构，人工智能可能将取得大进展。实际上，由于网络本身就是一个带权的图，图理论与神经网络的结合几乎是不可避免的事情，特别是包含逻辑推理关系(知识)的图模型与神经网络的结合更加吸引人。在最近的一个报告中[8], 27位学者(其中22位来自AlphaGo的东家DeepMind)，对这种可能性进行了阐述。沿这个方向继续下去并取得重大成果的话，可能将为人工智能开创一个比现如今的深度学习更加完善的基础。
现在还是回到历史中，我们需要花点时间总结一下麦卡洛克和皮茨发明的神经计算模型的历史地位了。这个现在被称之为MP模型(McCulloch-Pitts Model)的东西被现代的人们忽略了，它最少在三个方面产生了重要影响：首先，将生理学关于神经的研究进展带入到数学领域，并与逻辑学相结合，直到现在，也有很多人认为，为了使机器具有智能必须回归对神经科学或者大脑的研究，麦卡洛克和皮茨绝对是这个领域的第一人。另外，你可以坚定的认为MP模型催生了现代计算机科学的诞生，并使得这个科学从一开始就分成两个部分，一部分借鉴他们的由神经元构成的神经网络实现逻辑计算，同时也为存储程序设计带了最原始的参考，MP模型为图灵机和现代的存储程序计算机之间建立了一个至关重要的桥梁，为后来复杂的计算机硬件体系结构带来了最初的灵感。另一方面，由于这个神经网络的参数无法实现真正的学习，从皮茨开始，就有一些研究者开始致力于让神经网络自动学习，它构成了当前机器学习或人工智能研究的半壁江山(谦虚地说)。当前，在很多机器学习课上，老师们都会强调机器学习的编程范式与传统的基于过程的编程完全不一样，实际上，就是从皮茨开始，就注定了两者完全不在一个层次上。传统的计算机硬件或软件编程大都还是与MP方法一样，需要人为地设置参数或者可能的情况，机器学习则不太一样，它希望机器能像人一样，从环境的刺激(数据)中学习一个模型来应对环境的刺激，这个模型最早的例子就是感知机。通过这些资料的学习和整理，我逐渐清晰地看到， MP模型和感知机实际上代表了计算机科学发展的两条不同路径。以MP为背景的计算机体系强调计算与存储(记忆)，以感知机为开端的思想则强调学习。
故事进入尾声，控制论的创始人，性格怪异不讨人(罗素，香农等)喜欢的维纳成就了皮茨，但是维纳的性格预示着皮茨在他身边也是危险的，更别说维纳身边有一个纳粹背景的妻子。玛格丽特·维纳从各方面讲都是一个有控制欲的、保守的假正经。由于后来一个偶然的机会，麦卡洛克也来到MIT和皮茨一起在维纳手下工作。突然有一天，维纳告诉麦卡洛克，皮茨和莱特文：“所有我和你们项目之间的联系被永久性的解除了“。此后他再也没有同皮茨说过话，也不告诉他为什么。一种说法是[2,3]玛格丽特受不了麦卡洛克疯子般的自由生活习惯(麦卡洛克会写诗,并到处炫耀)，造谣说麦卡洛克手下的几个男孩勾引了维纳的宝贝女儿。还有一种说法[3]是”精神有缺陷的维纳已经无法捕捉麦卡洛克言语表达中的细微差别。有些时候，维纳分辨不出麦卡洛克是在陈述一件事实还是在说一个猜想”。维纳和麦卡洛克以及皮茨突然地决裂对控制论遭受很大损失，皮茨则遭受了最大的打击，几乎完全垮掉，他开始酗酒，甚至自己合成类似鸦片的化学物质作为下酒菜，他毁掉了他的研究成果和博士论文，拒绝接受任何帮助。在给麦卡洛克写下一封”这一切都让我感到极度悲哀”的信[1]不久，1969年5月14日，沃尔特·皮茨在剑桥的寄宿之家孤独死去，死因是长期酗酒导致的肝硬化出血。这个从小就开始流浪的大男孩一生没有得到过任何一个正式的文凭，但是他依然不愧是计算科学史上一颗耀眼的明星。四个月后，麦卡洛克也过世了。能让我们感到欣慰的是，从小孤苦伶仃的皮茨一生中能遇到麦卡洛克，他也是幸运的。皮茨从麦卡洛克那里得到了他需要的一切：”认同、友谊、「智识」上的另一半以及他从未有过的父爱” [1]。
麦卡洛克和皮茨实际上为控制论带来了最有力的证据，也为维纳带去了信心。控制论的核心思想几乎可以在麦卡洛克和皮茨的工作中体现。第一，控制，指的是系统能够与环境交互继而改变环境的能力，实际上任何一种能够接收环境数据，并进行输出的东西，都可称之为控制，神经元或神经网络就是典型代表。第二，反馈，指的是能够使用过去的性能来调整未来行为的一种属性，在神经元上，环状的链接，就是一种反馈，甚至你可以回到我们在本文第1季的求解根号的流程[7]，用错误反馈给系统，以指导下一步计算的结果。由于控制论的目标实在太过宏大，当时对很多学科产生了非常重要的影响，然而现在这种影响力正在减退。遗憾的是，为维纳《控制论》思想贡献巨大的麦卡洛克和皮茨甚至都没能出现在维纳的自传里，想想作者是维纳，你应该可以理解。现在，麦卡洛克和皮茨也被后人称为控制论学家，这对维纳来说，实在是有点讽刺。甚至是最新的一本关于控制论历史的科普读物《机器崛起：遗失的控制论历史》[5]中竟然连皮茨都没有提到，不知道维纳是否于心有愧，他肯定不知道“原来随便错手，可毁了人一世”。
神经元模型的创立者皮茨是不幸的，遗憾的是，感知机的发明人弗兰克· 罗森布拉特(Frank Rosenblatt)也几乎一样不幸。
```python
参考文献：
[1] 陨落的天才-皮茨和他违背的逻辑，人工智能名人堂第25期
[2] 尼克，人工智能简史 人民邮电出版社，2017.12
[3] Charles Petzold，图灵的秘密：他的生平、思想及论文解读 杨卫东 , 朱皓 (译者)
[4]  Harry Fairhead，The McCulloch-Pitts Neuron, https://www.i-programmer.info/ babbages-bag/325-mcculloch-pitts-neural-networks.html
[5] 期马斯·瑞德 , 机器崛起：遗失的控制论历史, 机械工业出版社,2017
[6] Pitts, Walter. A logical calculus of the ideas immanent in nervous activity.  Neurocomputing: foundations of research. MIT Press, 1988:115-133.
[7] twinlj77，理解计算：从√2到AlphaGo ——第1季 从√2谈起，https://blog.csdn.net/SIGAI_CSDN/article/details/80886538
[8] Peter W. Battaglia, Jessica B. Hamrick, Victor Bapst, Alvaro Sanchez-Gonzalez, Vinicius Zambaldi, Mateusz Malinowski, Andrea Tacchetti, David Raposo, Adam Santoro, Ryan Faulkner, Caglar Gulcehre, Francis Song, Andrew Ballard, Justin Gilmer, George Dahl, Ashish Vaswani, Kelsey Allen, Charles Nash, Victoria Langston, Chris Dyer, Nicolas Heess,Daan Wierstra, Pushmeet Kohli, Matt Botvinick, Oriol Vinyals, Yujia Li, Razvan PascanuRelational inductive biases, deep learning, and graph networks. https://arxiv.org/abs/1806.01261
```
[往期文章汇总](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485142%26idx%3D3%26sn%3D9a8329cbf84282c1aa2991834b9d5ef6%26chksm%3Dfdb69b41cac11257ca539757aaa934b78fc2f9d5ebf1d3b3e878d9fd124757cc754a938b7609%26scene%3D21%23wechat_redirect)
[1][机器学习-波澜壮阔40年](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483705%26idx%3D1%26sn%3Dc6e7c4a2e14a2469308b41eb60f155ac%26chksm%3Dfdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3%26scene%3D21%23wechat_redirect)SIGAI 2018.4.13.
[2][学好机器学习需要哪些数学知识？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483713%26idx%3D1%26sn%3D1e7c81381d16806ac73e15691fe17aec%26chksm%3Dfdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64%26scene%3D21%23wechat_redirect)SIGAI 2018.4.17.
[3][人脸识别算法演化史](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483726%26idx%3D1%26sn%3D9fef4cc1766ea4258749f8d40cc71a6e%26chksm%3Dfdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa%26scene%3D21%23wechat_redirect)SIGAI 2018.4.20.
[4][基于深度学习的目标检测算法综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26scene%3D21%23wechat_redirect)SIGAI 2018.4.24.
[5][卷积神经网络为什么能够称霸计算机视觉领](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26scene%3D21%23wechat_redirect)[域？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26scene%3D21%23wechat_redirect)SIGAI 2018.4.26.
[6][用一张图理解SVM的脉络 ](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483937%26idx%3D1%26sn%3D84a5acf12e96727b13fd7d456c414c12%26chksm%3Dfdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329%26scene%3D21%23wechat_redirect)SIGAI 2018.4.28.
[7][人脸检测算法综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483950%26idx%3D1%26sn%3Da3a5b7907b2552c233f654a529931776%26chksm%3Dfdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16%26scene%3D21%23wechat_redirect)SIGAI 2018.5.3.
[8][理解神经网络的激活函数](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483977%26idx%3D1%26sn%3D401b211bf72bc70f733d6ac90f7352cc%26chksm%3Dfdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3%26scene%3D21%23wechat_redirect)SIGAI 2018.5.5.
[9][深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484037%26idx%3D1%26sn%3D13ad0d521b6a3578ff031e14950b41f4%26chksm%3Dfdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6%26scene%3D21%23wechat_redirect)SIGAI 2018.5.8.
[10][理解梯度下降法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26scene%3D21%23wechat_redirect)SIGAI 2018.5.11.
[11][循环神经网络综述—语音识别与自然语言处理的利器](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484310%26idx%3D1%26sn%3D0fc55a2784a894100a1ae64d7dbfa23d%26chksm%3Dfdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0%26scene%3D21%23wechat_redirect)SIGAI 2018.5.15
[12][理解凸优化](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26scene%3D21%23wechat_redirect)SIGAI 2018.5.18
[13][【实验】理解SVM的核函数和参数](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484495%26idx%3D1%26sn%3D4f3a6ce21cdd1a048e402ed05c9ead91%26chksm%3Dfdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9%26scene%3D21%23wechat_redirect)SIGAI 2018.5.22
[14][ 【SIGAI综述】行人检测算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484523%26idx%3D1%26sn%3Dddaa70c4b92f6005d9bbd6b3a3fe4571%26chksm%3Dfdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6%26scene%3D21%23wechat_redirect)SIGAI 2018.5.25
[15][机器学习在自动驾驶中的应用—以百度阿波罗平台为例](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484540%26idx%3D1%26sn%3D733332042c31e1e18ad800f7f527893b%26chksm%3Dfdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%26scene%3D21%23wechat_redirect)(上) SIGAI 2018.5.29
[16][理解牛顿法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26scene%3D21%23wechat_redirect)SIGAI 2018.5.31
[17][【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题 ](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484658%26idx%3D1%26sn%3Df5c9f92c272c75883bf8e6f532559f11%26chksm%3Dfdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b%26scene%3D21%23wechat_redirect)SIGAI 2018.6.1
[18][大话Adaboost算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%26scene%3D21%23wechat_redirect)SIGAI 2018.6.2
[19][FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484711%26idx%3D1%26sn%3Dbb7644e101b5924f54d6800b952dc3aa%26chksm%3Dfdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb%26scene%3D21%23wechat_redirect)SIGAI 2018.6.4
[20][理解主成分分析(PCA)](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484754%26idx%3D1%26sn%3Db2c0d6798f44e13956bb42373e51d18c%26chksm%3Dfdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24%26scene%3D21%23wechat_redirect)SIGAI 2018.6.6
[21][人体骨骼关键点检测综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D1%26sn%3Dceafb54203f4e930ae457ad392b9f89c%26chksm%3Dfdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9%26scene%3D21%23wechat_redirect)SIGAI 2018.6.8
[22][理解决策树](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484827%26idx%3D1%26sn%3D043d7d0159baaddfbf92ed78ee5b1124%26chksm%3Dfdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8%26scene%3D21%23wechat_redirect)SIGAI 2018.6.11
[23][用一句话总结常用的机器学习算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484859%26idx%3D1%26sn%3D2c4db22fb538953a62a90983e3e1f99d%26chksm%3Dfdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16%26scene%3D21%23wechat_redirect)SIGAI 2018.6.13
[24][目标检测算法之YOLO](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484909%26idx%3D1%26sn%3Dc02ee17e5175230ed39ad63e73249f5c%26chksm%3Dfdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063%26scene%3D21%23wechat_redirect)SIGAI 2018.6.15
[25][理解过拟合](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484954%26idx%3D1%26sn%3Dc28b7f07c22466e91b1ef90e9dbe3ad1%26chksm%3Dfdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2%26scene%3D21%23wechat_redirect)SIGAI 2018.6.18
[26][理解计算：从√2到AlphaGo ——第1季 从√2谈起](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484981%26idx%3D1%26sn%3Dd3003468b9853851923844812993e060%26chksm%3Dfdb69ba2cac112b4dac620d52100ebd033eb679f29340726a67297c4d6980b16c7cc91122028%26scene%3D21%23wechat_redirect)SIGAI 2018.6.20
[27][场景文本检测——CTPN算法介绍](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485005%26idx%3D1%26sn%3D0d4fb43b8db2a8046c64a9cfcbf3f478%26chksm%3Dfdb69bdacac112cce05c8b735b4f8b1ccf2348bea55a30af2055fc328958bb8f1ffd0f819bd2%26scene%3D21%23wechat_redirect)SIGAI 2018.6.22
[28][卷积神经网络的压缩和加速](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485042%26idx%3D1%26sn%3Dcdcf8d4b07acf64c7a6f5f7c1a731a12%26chksm%3Dfdb69be5cac112f377766984afb87313c1e1c58d94c80005f0f6f6af61ee5a4bd1bf6c6157b6%26scene%3D21%23wechat_redirect)SIGAI 2018.6.25
[29][k近邻算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485074%26idx%3D1%26sn%3D0ebf1bf8f49e9c46075fe3803d04c95d%26chksm%3Dfdb69b05cac112132d280c70af3923ca4c3cccfa5fcd8628b79d4b246b3b2decbc80a180abb3%26scene%3D21%23wechat_redirect)SIGAI 2018.6.27
[30][自然场景文本检测识别技术综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485142%26idx%3D1%26sn%3Dc0e01da30eb5e750be453eabe4be2bf4%26chksm%3Dfdb69b41cac11257ae22c7dac395e9651dab628fc35dd6d3c02d9566a8c7f5f2b56353d58a64%26scene%3D21%23wechat_redirect)SIGAI 2018.6.27
[31][理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485155%26idx%3D1%26sn%3D990cc7400751c36e9fef0a261e6add2a%26chksm%3Dfdb69b74cac112628bdae14c6435120f6fece20dae9bf7b1ffc8b8b25e5496a24160feca0a72%26scene%3D21%23wechat_redirect)SIGAI 2018.7.4
[32][机器学习算法地图](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485306%26idx%3D1%26sn%3Dfc8cc8de313bdb61dcd39c1dedb240a4%26chksm%3Dfdb69aedcac113fb4b18c74248a313536ded50bade0e66b26f332ab247b148519da71ff2a3c0%26scene%3D21%23wechat_redirect)SIGAI2018.7.6
[33][ ](https://mp.weixin.qq.com/s/YMtnBAVDZepsMTO4h-VRtQ)[如何成为一个优秀的算法工程师](https://mp.weixin.qq.com/s/YMtnBAVDZepsMTO4h-VRtQ)SIGAI2018.7.11

```python
原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不得转载，不能用于商业目的。 
关注VX：SIGAICN，获取文章全文链接
```




