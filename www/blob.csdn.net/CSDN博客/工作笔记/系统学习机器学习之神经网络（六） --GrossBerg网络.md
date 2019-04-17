# 系统学习机器学习之神经网络（六） --GrossBerg网络 - 工作笔记 - CSDN博客





2017年01月09日 11:06:04[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5647








原文：http://m.blog.csdn.net/article/details?id=52574264

这里先介绍GrossBerg网络是因为后面CPN中，输出层会用到GrossBerg网络，所以，先做些简单说明。

1.生物学启发      

       Grossberg网络是受到了人类视觉系统的生理学研究的启发，如图，视网膜是大脑的一部分，它由三层神经细胞组成：

外层

光感受器：转化光成电信号

杆状体 – 使我们能在昏暗的情况下看见东西

锥体 – 使我们能看到精细的细节和色彩

中间层

双极细胞 – 连接光感受器到第三层

水平细胞 – 连结感受器和双极细胞

无长突细胞 – 连结双极细胞与神经节细胞

第三层

神经节细胞 – 通过视觉神经连结视网膜到大脑

![](https://img-blog.csdn.net/20160918143812385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（1）幻觉

         克服视网膜的不完善的吸收机制产生了幻觉，Grossberg和其他人使用了大量已知的幻觉来探测自适应感知机制。

         视网膜的吸收过程中存在着不完善之处：

         （a）视神经乳头处既无杆状细胞，也无椎体细胞，给视场造成盲点。

         （b）动脉和静脉在视网膜后部的光感受器的前面交叉，阻碍了杆状细胞及椎体细胞接收市场中的所有光线。

![](https://img-blog.csdn.net/20160918144132059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160918144609733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


         对此，视觉路径上的神经系统做出了某些补偿处理

        （a）应急切断(Emergent Segmentation)：补偿了丢失的边界。

        （b）特征填充(Featural Filling-In)：在产生的边界内填充颜色和亮度

（2）视觉规格化

        （a）亮度一致

        （b）亮度对比

         仅觉察到相对亮度的差别，非绝对亮度。


2.基本线性系统

（1）漏积分器

![](https://img-blog.csdn.net/20160918151709478?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 一阶线性微分方程

![](https://img-blog.csdn.net/20160918150751778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)(1)

ε是系统的时间常量

 对于一个任意的输入p(t)，漏积分器的响应是

![](https://img-blog.csdn.net/20160918150949257?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)(2)


当p为常数，n(0)=0时，

上式变为

![](https://img-blog.csdn.net/20160918151129039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160918151855512?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


重要属性：

      (a)由于（1）式是线性的，如果输入p按比例变化，则响应n(t)也会以同样的大小按比例变化

      (b)漏积分器的响应速度由时间常数ε决定。当ε减少时，响应速度变快，当ε增加时相应速度变慢。




（2）并联模型

![](https://img-blog.csdn.net/20160918152747398?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


操作方程：

![](https://img-blog.csdn.net/20160918152656640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


p+： 非负激励输入，使网络响应增加

p-： 非负抑制输入，使网络响应减少

b+： 非负常量，上限

b-： 非负常量，下限


上式中，第一部分-n（t）是一个线性衰减项，当n（t）为正时这一项为负，n（t）为负时这一项为正。

第二部分![](https://img-blog.csdn.net/20160918153129705?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，提供非线性的增益控制，当n（t）比b+小时，这部分为负，但当n（t）=b+时变为0，提供非线性的增益控制。

第三部分![](https://img-blog.csdn.net/20160918153944695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)也提供非线性的增益控制，给n（t）设置了一个下限-![](https://img-blog.csdn.net/20160918154057193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。

(分析：令![](https://img-blog.csdn.net/20160918155006881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，则![](https://img-blog.csdn.net/20160918155025228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)或![](https://img-blog.csdn.net/20160918155034946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，即![](https://img-blog.csdn.net/20160918154057193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)<n<![](https://img-blog.csdn.net/20160918155157227?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center))

令

![](https://img-blog.csdn.net/20160918155454542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


得

![](https://img-blog.csdn.net/20160918155421653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


增加激励输入，网络稳定响应将增加，但总是小于b+ = 1，即保持了相对强度


3.两层竞争网络

（1）短期记忆和长期记忆

         Grossberg网络由三部分组成：第一层，第二层和自适应权值。第一层是视网膜操作的一个粗略模型，第二层代表视觉皮层。具有短期记忆（STM）和长期记忆（LTM）机制，能够自适应调整、过滤、标准化和对比度增强。

![](https://img-blog.csdn.net/20160918160421641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第一层

![](https://img-blog.csdn.net/20160918161225158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


          Grossberg网络的第一层接收外部输入并规格化输入模式的强度。使用并联模式，以输入向量p计算出来激励输入和抑制输入。

          第一层的运算方程

![](https://img-blog.csdn.net/20160918162054007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


上式是一个有激励输入![](https://img-blog.csdn.net/20160918162431910?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)和抑制输入![](https://img-blog.csdn.net/20160918162500927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)的并联模式，其中

![](https://img-blog.csdn.net/20160918162539787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


因此对神经元i的激励输入时输入向量的第i个元素，抑制输入时除了输入向量第i个元素的所有元素之和。

**规格化输入模式：加强中心/抑制周围**

设

![](https://img-blog.csdn.net/20160918163006341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)（i=1，2，3.....）


因此所有神经元的上限相同。

![](https://img-blog.csdn.net/20160918164708769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在稳定状态下

![](https://img-blog.csdn.net/20160918165654866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160918165706143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


定义相对强度

![](https://img-blog.csdn.net/20160918165912474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中

![](https://img-blog.csdn.net/20160918165924662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


稳定状态神经元的活跃度

![](https://img-blog.csdn.net/20160918170053134?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


总活跃度

![](https://img-blog.csdn.net/20160918170229698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


自此，输入向量已规格化，从而总的活跃度<![](https://img-blog.csdn.net/20160918170354137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。而第一层的输出![](https://img-blog.csdn.net/20160918170602906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)与相对强度![](https://img-blog.csdn.net/20160918165912474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)成正比，无论总输入P的幅度如何。

        Grossberg网络的第一层解释了人类视觉系统的亮度一致性和亮度对比特征。这种网络

（a）对于一个图像的相对强度而非绝对强度是敏感的。

（b）具有加强中心/抑制周围的连接模式，和视网膜神经节细胞的接收区域相似。


eg：一个有两神经元的例子

![](https://img-blog.csdn.net/20160918171102943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其相应情况如下图

![](https://img-blog.csdn.net/20160918171202890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       如上图，对于p1或p2，其第2个元素都是第1个元素的4倍，p2是p1的5倍，，上图可以看出网络的相应保持了输入的相对强度，而同时限制了总强度。

第二层

  （1）短期记忆 

           Grossberg的第二层，是一个连续的instar层，实现（a）规格化这一层的总活跃度（b）对模式产生对比度增强。（c）最后，像短期记忆（STM）一样通过存储对比度增强模式操作

          第二层也是用并联模型，与第一层的区别是使用了反馈式连接。反馈使得网络能够存储模式，即使是在输入 撤销之后，反馈也能进行竞争，从而产生模式的对比度增强。

![](https://img-blog.csdn.net/20160918183344601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第二层的运算方程为

![](https://img-blog.csdn.net/20160918183901552?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160918183808645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


激励输入：


![](https://img-blog.csdn.net/20160918184954308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


W2的行在训练之后会代表原型模式。

抑制输入：

![](https://img-blog.csdn.net/20160918185011105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


eg：一个由两个神经元组成的层

![](https://img-blog.csdn.net/20160918185747327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


则运算方程为：

![](https://img-blog.csdn.net/20160918185952228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160918190005062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160918190101026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160918190110219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如下图，当输入向量![](https://img-blog.csdn.net/20160918190257438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)时，输入向量加入了0.25秒

![](https://img-blog.csdn.net/20160918190831409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第二层输入为

![](https://img-blog.csdn.net/20160918190402595?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


因此第二个神经元是第一个神经元输入的1.5倍，然而在0.25秒，第二个神经元的输出是第一个神经元输出的6.34倍。**高与低的对比度急剧的增加了**

（2）对比度增强

**（a）第二层的输入是第一层的输出（规格化的输入）的内积**。最大的内积与输入模式最相近的原型对应。第二层在在神经元之间实行竞争，**将易于产生输出模式的对比度增强，保持大的输出并使小的输出减弱**。这种对比度增强比起Hamming网络及Kohonen网络通常要缓和一些。在Hamming和Kohonen中，竞争除了一个意外的所有神经元输出归0。而在grossberg网络中，竞争保持大的值而削弱小的值。对比度增强的大小是由传输函数f所决定的。

         （b）当输入被置0时，网络进一步增强对比度，并且存储模式。

                  由上图，当输入撤销（0.25秒后），第一个神经元的输出衰减至0，而同时第二个神经元的输出达到一个稳定状态值0.79.及时在这个输入撤消后，这个输出仍然得到了保存。（非线性反馈=》网络存储模式，对比度增强的加强中心/抑制周围的连接模式）

（3）传输函数

        Grossberg第二层的行为很大程度上依赖于传输函数f(n)。例如，假定一个输入在使用一段时间后被撤销

![](https://img-blog.csdn.net/20160918194006564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160918194015439?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





4.学习规则

无监督的Hebb规则

![](https://img-blog.csdn.net/20160918201256328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


W2的学习规则：

![](https://img-blog.csdn.net/20160918201627392?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


当ni2（t）不活跃时关闭学习：

![](https://img-blog.csdn.net/20160918201411704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

eg：两个神经元的例子

设α=1，开始时所有权值置为0

模式1用第二层的第一个神经元编码，模式2用第二层的第二个神经元编码。

![](https://img-blog.csdn.net/20160918201832582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如下图，![](https://img-blog.csdn.net/20160918202111664?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160918202120693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)只在![](https://img-blog.csdn.net/20160918202128474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)非0时更新，并且收敛到相应的n1模式：![](https://img-blog.csdn.net/20160918202307725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)和![](https://img-blog.csdn.net/20160918202315725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，




![](https://img-blog.csdn.net/20160918202433450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160918202442169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)只在![](https://img-blog.csdn.net/20160918202450366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)非0时得到调整，并且收敛到相应的n1模式。


![](https://img-blog.csdn.net/20160918202552983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




