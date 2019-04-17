# htk网络和解码源码（四、网络） - xmdxcsj的专栏 - CSDN博客





2015年05月24日 17:14:52[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1222








### (一)         例子

<s> sil 

</s>    sil 

北京     b eh_l i_l ji i_h ng_h 

北方     b eh_l i_l f a_h ng_h 

天气     ti aa_h nn_h qi i_h i_l

![](https://img-blog.csdn.net/20150524171247322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



注：调用WriteTLex (tnet, "lex.dot")函数，可以生成相应的网络结构图。

### (二)         主要结构

#### 1.      节点

定义：

typedef enum _LexNodeType {

 LN_WORDEND, LN_CON, LN_MODEL

} LexNodeType;

Node的三种类型：
|node|内容|对应的layer|
|----|----|----|
|LN_WORDEND|Pron（单词）|WE|
|LN_CON|辅助节点，用于减少边的数量|AB YZ ZS SA|
|LN_MODEL|Hmm（model）|A BY Z SIL|

5 special models: <s> Mod/WE,  </s> Time, </s> Mod/WE

#### 2.      层

定义：

typedef enum _LayerId {

  LAYER_Z=0, LAYER_ZS=1, LAYER_SIL=2, LAYER_SA=3, LAYER_A=4, LAYER_AB=5,

  LAYER_BY=6, LAYER_WE=7, LAYER_YZ=8

} LayerId;

9层结构如下：
|层类型|含义|对应的节点类型|
|----|----|----|
|LAYER_Z=0|y-z+a|2|
|LAYER_ZS=1|z+a或者z+sil|1|
|LAYER_SIL=2|sil/sp|2|
|LAYER_SA=3|sil+a或者z+a或者time|1|
|LAYER_A=4|z-a+b或者sil-a+b|2|
|LAYER_AB=5|a+b|1|
|LAYER_BY=6|a-b+c b-c+d|2|
|LAYER_WE=7|word|0|
|LAYER_YZ=8|y+z|1|



层对应关系：

wordEndLayerId = LAYER_SA

spSkipLayer = LAYER_ZS



### (三)         构建步骤

1.      CollectPhoneStats()

收集A, B, AB, YZ；创建LAYER_AB和LAYER_YZ层

对于具有单个发音p的词条（如silience），会把z和p放在SA层和YZ层

节点里面存有两种类型的数据，一类是HLink，对应于三音素；另一类是pronid，对应于pronlist中的下标，可以通过pronid在pronlist里面找到该词条。

2.      CreateAnodes()

创建LAYER_A，z-a+b节点；创建LAYER_SA，z+a节点

3.      CreateZnodes()

创建LAYER_Z，y-z+a节点；创建LAYER_ZS，z+a节点

4.      CreateSILnodes()

创建LAYER_SIL，sil节点用于连接SA和ZS

对于sil节点：对于每一个z+sil，新生成一个silnode，右边连接所有的sil+a。

对于sp节点：对于每一个z+a，新生成一个spnode，右边连接对应的z+a。

5.      CreateBYnodes()

创建LAYER_BY，a-b+c节点；wordend节点也并入到LAYER_BY。

6.      CreateStartEnd()

增加网络的起点和终点

<s>：新建sil的model节点（LAYER_Z）；新建<s>的word节点（LAYER_SIL）；连接sil+a

![](https://img-blog.csdn.net/20150524171343731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

</s>：新建sil的model节点（LAYER_A）；新建</s>的word节点（LAYER_AB）；连接如下

![](https://img-blog.csdn.net/20150524171356118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

7.      建立lmlatree

1)        AssignWEIds()

对AB层一直到wordend层之间的节点进行编号pronID（word node）和lmlaId（其他node）。

2)        InitLMlaTree()

新建lmlatree，对A层的节点进行编号：如果节点后面只有一条出边，该节点的lmlaIdx设为子节点的lmlaIdx（对应于简单节点）；如果节点后面存在多条出边，该节点的lmlaIdx设为++tnet->lmlaCount，同时新建该节点对应的compNode（对应于复杂节点）。

3)        ConvertTLex2Lex ()

初始化lmlatree中的简单节点：将net中的node的loWE和hiWE存放至lmlatree。

![](https://img-blog.csdn.net/20150524171551661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Lmlatree的节点结构如下图：

![](https://img-blog.csdn.net/20150524171428613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



