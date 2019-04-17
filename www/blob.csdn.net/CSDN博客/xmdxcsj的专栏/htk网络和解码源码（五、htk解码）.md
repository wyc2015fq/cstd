# htk网络和解码源码（五、htk解码） - xmdxcsj的专栏 - CSDN博客





2015年05月24日 17:22:30[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1742








# htk解码

使用工具：

HRec：解码

HVite：forcedalignments,lattice rescoring and recognise direct audio input.



一、解码操作

![](https://img-blog.csdn.net/20150524172005480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

用于解码的网络可以分为三个层面：word、model（三音素）和state。首先根据word网络和字典，生成三音素model级别的网络，然后根据HMM的状态集生成HMM级别的网络。

解码器的得分主要包括以下两大部分：

1.      Hmm状态的生成概率

2.      状态之间的转移概率，包括：状态自旋概率、状态之间的转移概率（常数）、word end的转移概率（语言模型得分）

解码器的目的就是寻找所有path中得分最高对应的那条path。路径的建立基于Token传递，每个node最多保持N个tokens，多余的tokens将会被减掉。

在解码的过程中，可以用lattice需要记录历史路径，路径的level可以有word、model和state三种级别。

为了减少计算量，对于大型网络来讲需要使用剪枝技术。主要有三种剪枝策略可以使用：

1.       Token beam：针对每个时间点，找出最优的tokens以及设定的beam，剪掉得分在beam以外的token。从效率的角度来讲，最好使用model级别的剪枝，而不是state级别的剪枝。当某个model的所有state的所有token都超过beam，该model被剪掉。

2.       Word-end beam：对于网络中的word-endnode，依据最好的word-end token设置相应的beam。

3.       设置网络当中同时存活的model的最大个数。



二、解码结果

![](https://img-blog.csdn.net/20150524172202364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

识别的结果以lattice呈现，可以保存为standard lattice format (SLF)文件。

Lattice输出可以用于重打分、生成n-best结果。
            


