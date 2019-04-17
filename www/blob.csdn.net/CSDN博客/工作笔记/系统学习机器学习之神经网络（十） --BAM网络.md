# 系统学习机器学习之神经网络（十） --BAM网络 - 工作笔记 - CSDN博客





2017年01月09日 17:13:08[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6343








转自：http://www.07net01.com/2015/08/897795.html

联想记忆网络的研究是神经网络的重要分支，在各种联想记忆网络模型中，由B·Kosko于1988年提出的双向联想记忆（Bidirectional Associative Memory，BAM）网络的应用最为广泛。前面介绍过的Hopfiled网络可实现自联想，具体内容可以参考博文《反馈神经网络Hopfield网络》。而BAM网络可以实现双向异联想，具有离散型、连续型和自适应型等多种形式。本篇博文主要介绍离散型BAM网络。

# 一、BAM网络结构与原理

  BAM网络是一种双层双向网络，当向其中一层加入输入信号时，另一层可得到输出。由于初始模式可以作用于网络的任一层，信息可以双向传播，所以没有明确的输入层或输出层。可将其中的一层称为X层，有n个神经元节点；另一层称为Y层，有m个神经元节点。两层的状态向量可取单极性二进制０或１，也可以取双极性离散值１或－１。如果令由X到Y的权矩阵为W，则由Y到X的权矩阵便是其转置矩阵WT

![](https://img-blog.csdn.net/20170109171522968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  BAM网实现双向异联想的过程是网络运行从动态到稳态的过程。对已建立权值矩阵的BAM网，当将输入样本XP作用于X侧时，该侧输出X(1)＝XP通过W阵加权传到Y侧，通过该侧节点的转移函数fy进行非线性变换后得到输出Y(1)＝fy(WX(1))；再将该输出通过WT阵加权从Y侧传回X侧作为输入,通过X侧节点的转移函数fx进行非线性变换后得到输出X(2)＝fx［WTY(1)］＝fx｛［WT［fy(WX(1))］｝。这种双向往返过程一直进行到两侧所有神经元的状态均不再发生变化为止。此时的网络状态称为稳态，对应的Y侧输出向量YP便是模式XP经双向联想后所得的结果。同理，如果从Y侧送入模式YP，经过上述双向联想过程,X侧将输出联想结果X。这种双向联想过程可用下图表示：


![](https://img-blog.csdn.net/20170109171547749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  对应的计算公式如下：![](https://img-blog.csdn.net/20170109171623250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  对于经过充分训练的权值矩阵，当向BAM网络一侧输入有残缺的已存储模式时，网络经过有限次运行不仅能在另一侧实现正确的异联想,而且在输入侧重建了完整的输入模式。也就是说，从残缺的输入模式，既可以通过BAM实现异联想，还可以重建完整的输入模式，这个功能将会非常有用。比如对于下面将分享的一个BAM网络实现就是如此，通过（X，Y）几对需要存储的模式，计算出所需要的权矩阵，例子中选择的是人名和电话的双联想，通过输入残缺的人名，计算好的BAM网络能够实现对电话号码的联想，同时将残缺的人名补全。

# 二、BAM网络结构的能量函数与稳定性分析
  跟Hopfield网络类似，若BAM网络的阈值T为0，则能量函数定义为：![](https://img-blog.csdn.net/20170109171935438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  BAM 网双向联想的动态过程就是能量函数量沿其状态空间中的离散轨迹逐渐减少的过程。当达到双向稳态时，网络必落入某一局部或全局能量最小点。对于具体的推理过程，本博文不再进行详述。经过一番的推导，可以得到如下的能量变化公式：![](https://img-blog.csdn.net/20170109171950057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  上式表明BAM网的能量在动态运行过程中不断下降，当网络达到能量极小点时即进入稳定状态，此时网络两侧的状态都不再变化。证明过程对BAM网权矩阵的**学习规则并未作任何限制**，而且得到的稳定性的结论**与状态更新方式为同步或异步无关**。考虑到同步更新比异步更新时能量变化大，收敛速度比串行异步方式快，故采常用同步更新方式。


# 三、BAM网络结构的权值设计
  对于离散 BAM 网络，一般选转移函数f (·) ＝sign(·)。当网络只需存储一对模式（X１，Y１）时，若使其成为网络的稳定状态，应满足如下条件：![](https://img-blog.csdn.net/20170109172056736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170109172110267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  当需要存储 P对模式时，将以上结论扩展为P对模式的外积和，从而得到Kosko提出的权值学习公式:
![](https://img-blog.csdn.net/20170109172121466?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  用外积和法设计的权矩阵，不能保证任意P对模式的全部正确联想，但下面的定理表明，如对记忆模式对加以限制，用外积和法设计BAM网具有较好的联想能力。
  定理：若P个记忆模式Xp，p ＝１，２ ，… ，P，x ∈｛ －１，１｝构成的n维模式，两两正交，且权值矩阵W按上式得到，则向BAM网输入P个记忆模式中的任何一个Xp时，只需一次便能正确联想起对应的模式Yp。
具体的一个例子：含噪声字符的联想过程，如下图：![](https://img-blog.csdn.net/20170109172131720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


# 四、BAM网络的应用

  BAM 网络的设计比较简单，只需由几组典型输入、输出向量构成权矩阵。运行时由实测到的数据向量与权矩阵作内积运算便可得到相应的信息输出。这是一种大规模并行处理大量数据的有效方法，具有实时性和容错性。更具魅力的是，这种联想记忆法**无需对输入向量进行预处理**。便可直接进入搜索，省去了编码与解码工作。



