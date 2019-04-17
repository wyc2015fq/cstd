# htk网络和解码源码（五、htk解码代码） - xmdxcsj的专栏 - CSDN博客





2015年05月25日 20:03:39[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1524








### (一)         整体流程

对于每一帧数据，遍历所有层的链表instsLayer[l]，其中l是指层数；

遍历一个链表中的所有node：如果node类型为LN_MODEL，执行PropagateInternal（每个node含有多个tokenset，每一个tokenset对应于一个state）；如果node类型为LN_WORDEND或LN_CON，清空node的tokenset，执行external propagation。

### (二)         PropagateInternal

一个hmm含有5个状态，对应于5个tokenset，每个tokenset可以包含多条候选路径。

1.      main beam pruning

tokenset的得分小于beam，被剪掉。Beam是指上一帧设定的。

2.      流程

![](https://img-blog.csdn.net/20150525200443133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

l  4->4

l  2->3、2->2、3->4、3->3

l  1->2

l  2 3 4生成概率

l  状态1和5清空

l  4->5

3.      MergeTokSet

存在剪枝，如果src满足：score+转移概率 < dec->beam，将会把src的token剪掉。

根据nTok（每一个tokenset最多的token个数）限制对merge以后的token进行剪枝。

4.      获取所有状态中最好的得分设为dec->bestScore

### (三)         beam pruning & externalpropagation

1.      update word end time and scorein tok->path

针对wordEndLayerId，对应于SA层，更新token里面的frame和score。

2.      wordend beam pruning

针对LAYER_WE层，加上该node对应的word及其语言得分。

如果inst->best < beamLimit，DeactivateNode；否则，HandleWordend

更新beamLimit，如果bestWEscore - dec->weBeamWidth比beamLimit大

2.1  HandleWordend

2.1.1         LMCacheTransProb：根据lmState（NEntry）和pronid找到语言模型得分，用该得分减去token中的lmscore（lookahead得分）作为最终的得分。

2.1.2         更新reltok（score weHyp等信息），必要时进行去重，最终生成的reltok是根据lmstate排序的。

3.      Z..S layer pruning

针对LAYER_ZS和LAYER_SA

主要是更新beamLimit

4.      对于所有层（包括前面三种处理的层）的所有节点

4.1  如果节点类型不是LN_WORDEND，调用UpdateLMlookahead

4.1.1         计算lookahead得分

![](https://img-blog.csdn.net/20150525200438562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4.1.2         更新relTok的score和delta

4.2 对LAYER_SIL层的所有sil做标记，跟sp区分开

4.3  PropagateExternal

4.3.1 对于LN_MODEL类型节点：如果存在状态1到状态5的转移概率，将状态1的tokens进行merge到状态5.

4.3.2 如果在LAYER_SIL或者LAYER_AB，需要调用HandleWordend来处理<s>和</s>

4.3.3 对于所有节点：prune tokensets in states 2..N-1；prune exit state token set

4.3.4 将exit state的token向该节点的所有后续节点传播=>PropIntoNode，需要两步操作：MergeTokSet；UpdateLMlookahead，只需要对BY层的node更新。

### (四)         Beam pruning

剪枝：

  TokScore beamWidth;         /* maxbeamWidth main beam (set by -t cmd line option) */

  TokScore weBeamWidth;     /*wordend beam width (set by -v cmd line option) */ WE层

  TokScore zsBeamWidth;        /*Z-S beam width (set by -v cmd line option) */ ZS和SA层

  TokScore curBeamWidth;    /* current dynamic beamWidth (due to max modelpruning) */

  TokScore beamLimit;          /*threshold of the main beam (bestScore - beamWidth) */

  RelTokScore relBeamWidth;    /*beamWidth of relative tokenset  beam */

                                /* (set by -tBW RBW cmd line option) */

beamLimit=bestScore – beamWidth



对于自旋加转移概率或者所有加生成概率，只需要加在tokenset的score变量，不需要加在tokenset的reltoken（因为reltoken保存的是相对得分，都加上得分该值保持不变）。



### (五)         Backtrack

如果走到end节点，使用end的tokenset

如果没有走到end节点，使用dec中的bestInst，如果bestInst的node类型为model，则遍历hmm中的三个状态，找出最优，如果node类型为word，则使用该tokenset。



每个节点hmm有三个tokenset，一个tokenset有多个RelToken（多条候选路径），一个RelToken包含WordendHyp（词历史路径，前向链表），一个WordendHyp还包含AltWordendHyp，包括lattice的多条候选路径。



存放结果的lablist是个双向链表。LLink是存放一个候选路径的双向链表
            


