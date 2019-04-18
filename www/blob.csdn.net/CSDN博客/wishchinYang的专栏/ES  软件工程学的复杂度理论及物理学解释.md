# ES : 软件工程学的复杂度理论及物理学解释 - wishchinYang的专栏 - CSDN博客
2017年08月19日 23:22:47[wishchin](https://me.csdn.net/wishchin)阅读数：402
个人分类：[心理学/职业																[AI/ES																[Humanoid																[AI/ML																[总体哲学AIPRIPCV](https://blog.csdn.net/wishchin/article/category/7538201)](https://blog.csdn.net/wishchin/article/category/6985098)](https://blog.csdn.net/wishchin/article/category/6251512)](https://blog.csdn.net/wishchin/article/category/1730787)](https://blog.csdn.net/wishchin/article/category/1533673)
系统论里面总是有一些通用的专业术语
         比如复杂度、熵、焓，复杂度专门独立出来，成为复杂度理论
        文章摘抄于：《**非线性动力学**》 刘秉政 编著  5.5 复杂性及其测度
热力学的几个专业术语
         熵、焓、自由能、吉布斯自由能、复杂度
         熵：体系混乱度（或无序度）的量度。S表示熵。也表示黑洞中不可用热量与其温度的比值。对于[化学反应](http://baike.baidu.com/item/%E5%8C%96%E5%AD%A6%E5%8F%8D%E5%BA%94)而言，若[反应物](http://baike.baidu.com/item/%E5%8F%8D%E5%BA%94%E7%89%A9)和产物都处于[标准状态](http://baike.baidu.com/item/%E6%A0%87%E5%87%86%E7%8A%B6%E6%80%81)下，则反应过程的熵变，即为该反应的标准熵变。
[焓](http://baike.baidu.com/item/%E7%84%93)（hán）变(Enthalpy changes)即物体焓的变化量。[1]焓是物体的一个[热力学能](http://baike.baidu.com/item/%E7%83%AD%E5%8A%9B%E5%AD%A6%E8%83%BD)[状态函数](http://baike.baidu.com/item/%E7%8A%B6%E6%80%81%E5%87%BD%E6%95%B0)，即[热函](http://baike.baidu.com/item/%E7%83%AD%E5%87%BD)：一个系统中的热力作用，等于该[系统内能](http://baike.baidu.com/item/%E7%B3%BB%E7%BB%9F%E5%86%85%E8%83%BD)加上其体积与外界作用于该系统的压强的[乘积](http://baike.baidu.com/item/%E4%B9%98%E7%A7%AF)的总和(Enthalpy is a combination of internal energy and flow work.)。
        自由能是指在某一个[热力学过程](http://baike.baidu.com/item/%E7%83%AD%E5%8A%9B%E5%AD%A6%E8%BF%87%E7%A8%8B)中，系统减少的内能中可以转化为对外做功的部分，它衡量的是：在一个特定的热力学过程中，系统可对外输出的“有用能量”。可分为[亥姆霍兹自由能](http://baike.baidu.com/item/%E4%BA%A5%E5%A7%86%E9%9C%8D%E5%85%B9%E8%87%AA%E7%94%B1%E8%83%BD)和[吉布斯自由能](http://baike.baidu.com/item/%E5%90%89%E5%B8%83%E6%96%AF%E8%87%AA%E7%94%B1%E8%83%BD)。
        1876年美国著名数学物理学家，数学化学家[吉布斯](http://baike.baidu.com/item/%E5%90%89%E5%B8%83%E6%96%AF/1039203)在康涅狄格科学院学报上发表了奠定[化学热力学](http://baike.baidu.com/item/%E5%8C%96%E5%AD%A6%E7%83%AD%E5%8A%9B%E5%AD%A6)基础的经典之作《论非均相物体的平衡》的第一部分。1878年他完成了第二部分。这一长达三百余页的论文被认为是[化学史](http://baike.baidu.com/item/%E5%8C%96%E5%AD%A6%E5%8F%B2)上最重要的论文之一，其中提出了吉布斯自由能，[化学势](http://baike.baidu.com/item/%E5%8C%96%E5%AD%A6%E5%8A%BF)等概念，阐明了[化学平衡](http://baike.baidu.com/item/%E5%8C%96%E5%AD%A6%E5%B9%B3%E8%A1%A1)、[相平衡](http://baike.baidu.com/item/%E7%9B%B8%E5%B9%B3%E8%A1%A1)、[表面吸附](http://baike.baidu.com/item/%E8%A1%A8%E9%9D%A2%E5%90%B8%E9%99%84)等现象的本质。
         吉布斯自由能（Gibbs free energy）在化学热力学中为判断过程进行的方向而引入的[热力学函数](http://baike.baidu.com/item/%E7%83%AD%E5%8A%9B%E5%AD%A6%E5%87%BD%E6%95%B0)。又称[自由焓](http://baike.baidu.com/item/%E8%87%AA%E7%94%B1%E7%84%93)、吉布斯自由能或[自由能](http://baike.baidu.com/item/%E8%87%AA%E7%94%B1%E8%83%BD)。[1][自由能](http://baike.baidu.com/item/%E8%87%AA%E7%94%B1%E8%83%BD)指的是在某一个[热力学过程](http://baike.baidu.com/item/%E7%83%AD%E5%8A%9B%E5%AD%A6%E8%BF%87%E7%A8%8B)中，系统减少的内能中可以转化为对外做功的部分。自由能(free energy)在物理化学中，按照[亥姆霍兹](http://baike.baidu.com/item/%E4%BA%A5%E5%A7%86%E9%9C%8D%E5%85%B9)的定容自由能F与[吉布斯](http://baike.baidu.com/item/%E5%90%89%E5%B8%83%E6%96%AF)的定压自由能G的定义。吉布斯自由能是自由能的一种。
                                           ΔG=ΔH－TΔS （Kj/mol)
G叫做吉布斯[自由能](http://baike.baidu.com/item/%E8%87%AA%E7%94%B1%E8%83%BD)。因为H、T、S均为[状态函数](http://baike.baidu.com/item/%E7%8A%B6%E6%80%81%E5%87%BD%E6%95%B0)，所以G为[状态函数](http://baike.baidu.com/item/%E7%8A%B6%E6%80%81%E5%87%BD%E6%95%B0)。
**孤立体系的熵变/焓变/复杂度变换**
熵变和反应方向
         对于孤立体系而言，在其中发生的任何反应变化必然是自发的。[热力学第二定律](http://baike.baidu.com/item/%E7%83%AD%E5%8A%9B%E5%AD%A6%E7%AC%AC%E4%BA%8C%E5%AE%9A%E5%BE%8B)告诉我们：在孤立体系中发生的任何变化或化学反应，总是向着[熵值](http://baike.baidu.com/item/%E7%86%B5%E5%80%BC)增大的方向进行，即向着△S孤立0的方向进行的。而当达到平衡时△S孤立=0，此时熵值达到最大。
        假如不是孤立体系，则可以把体系与其四周的环境一起作为一个新的孤立体系考虑，[熵增原理](http://baike.baidu.com/item/%E7%86%B5%E5%A2%9E%E5%8E%9F%E7%90%86)仍然是适用的。由此可以得出，[自发反应](http://baike.baidu.com/item/%E8%87%AA%E5%8F%91%E5%8F%8D%E5%BA%94)是向着0的方向进行的。大家知道，在[常压](http://baike.baidu.com/item/%E5%B8%B8%E5%8E%8B)下，当温度低于273K时，水会自发地结成冰。这个过程中体系的熵是减小的，似乎违反了熵增原理。但应注重到，这个体系并非孤立体系。在体系和环境间发生了热交换。从水变成冰的过程中体系放热给环境。环境吸热后[熵值](http://baike.baidu.com/item/%E7%86%B5%E5%80%BC)增大了，而且环境熵值的增加超过了体系熵值的减小。因而体系的熵变加上环境的熵变仍是大于零的，所以上述自发反应是符合[热力学第二定律](http://baike.baidu.com/item/%E7%83%AD%E5%8A%9B%E5%AD%A6%E7%AC%AC%E4%BA%8C%E5%AE%9A%E5%BE%8B)的。
**[耗散结构理论](http://baike.baidu.com/item/%E8%80%97%E6%95%A3%E7%BB%93%E6%9E%84%E7%90%86%E8%AE%BA)**
      耗散结构理论是普利高津(Pregogine)于20世纪60和70年代创立的 普利高津一直在从事关于非平衡[统计物理学](http://baike.baidu.com/item/%E7%BB%9F%E8%AE%A1%E7%89%A9%E7%90%86%E5%AD%A6)的研究工作，当他将热力学和统计物理学从[平衡态](http://baike.baidu.com/item/%E5%B9%B3%E8%A1%A1%E6%80%81)推到[近平衡态](http://baike.baidu.com/item/%E8%BF%91%E5%B9%B3%E8%A1%A1%E6%80%81)，再向远平衡态推进时终于发现：一个远离平衡态的非线性的开放系统(不管是物理的、化学的、生物的乃至社会的、经济的系统)通过不断地与外界交换物质和能量，在系统内部某个参量的变化达到一定的阈值时，通过涨落，系统可能发生突变即非平衡相变，由原来的混沌无序状态转变为一种在时间上、空间上或功能上的有序状态。这种在远离平衡的[非线性](http://baike.baidu.com/item/%E9%9D%9E%E7%BA%BF%E6%80%A7)区形成的新的稳定的宏观[有序结构](http://baike.baidu.com/item/%E6%9C%89%E5%BA%8F%E7%BB%93%E6%9E%84)，由于需要不断与外界交换物质或能量才能维持，因此称之为“[耗散结构](http://baike.baidu.com/item/%E8%80%97%E6%95%A3%E7%BB%93%E6%9E%84)”(dissipative structure)“ 。
**耗散结构和自组织性**
        组织是指系统内的[有序结构](http://baike.baidu.com/item/%E6%9C%89%E5%BA%8F%E7%BB%93%E6%9E%84)或这种有序结构的形成过程。德国[理论物理学家](http://baike.baidu.com/item/%E7%90%86%E8%AE%BA%E7%89%A9%E7%90%86%E5%AD%A6%E5%AE%B6)哈肯依据组织的进化形式把“组织”分为他组织和自组织两类。自组织是相对于他组织而言的，我们一般把不能自行组织、自行创生、自行演化，不能够自主地从无序走向有序的组织称为他组织。他组织只能依靠外界的特定指令来推动组织向有序演化，从而被动地从无序走向有序。相反，自组织是指无需外界特定指令就能自行组织、自行创生、自行演化，能够自主地从无序走向有序，形成有结构的系统。
**协同学**
        协同学(Synergetics)是由德国学者哈肯创立的 。协同学是研究[有序结构](http://baike.baidu.com/item/%E6%9C%89%E5%BA%8F%E7%BB%93%E6%9E%84)形成和演化的机制，描述各类非平衡相变的条件和规律。协同学认为，千差万别的系统，尽管其属性不同，但在整个环境中，各个系统间存在着相互影响而又相互合作的关系。协同学进一步指出，对于一种模型。随着参数、[边界条件](http://baike.baidu.com/item/%E8%BE%B9%E7%95%8C%E6%9D%A1%E4%BB%B6)的不同以及涨落的作用，所得到的图样可能很不相同；而对于一些很不相同的系统，却可以产生相同的图样。由此可以得出一个结论：[形态发生](http://baike.baidu.com/item/%E5%BD%A2%E6%80%81%E5%8F%91%E7%94%9F)过程的不同模型可以导致相同的图样。在每一种情况下，都可能存在生成同样图样的一大类模型。
**复杂度的定义和测度问题**
         统计复杂度
         算法复杂度
         L-Z复杂度   
**软件工程学的系统论分析**
       类似于起源古老且一直发展的建筑学，软件开发过程是一个集设计、开发、检验及维护整个生命周期过程，软件工程学和建筑学具有极大的 相似之处，在总体方法论的层面具有通用性。
        从软件的组件方面分析，软件的经典设计为MCV模式.
