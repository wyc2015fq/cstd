
# CNN中1×1的卷积核的作用 - 郭云飞的专栏 - CSDN博客


2017年10月31日 12:04:02[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：2575


1×1卷积核最早引起人们的重视是在NIN**《Min Lin, Qiang Chen, and Shuicheng Yan. Network in network. CoRR, abs/1312.4400, 2013》**结构中，后来在GoogLeNet的Inception结构中用于降维。
**1×1卷积核用于升维、降维**
如果卷积的输入、输出都仅有一个平面，那么1×1卷积核并没有啥意义。它完全不考虑像素与其周围像素之间的关系！但通常卷积的输入、输出都是立方体（多通道的），此时1×1卷积核实际上是对每个像素点在不同通道（channels）上线性组合（信息整合），且保留了图片的原有平面结构，仅仅是改变channels的数量，进而达到升维和降维的功能！
![](https://img-blog.csdn.net/20171031121610352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上左图显示了1×1卷积核用于降维。输入为4×4的平面，共3个通道。用两个1×1的卷积核（卷积层）作用之后，变为4×4的平面，共2通道。同理，上右图展示了升维的操作。
**实现跨通道的交互和信息整合**
升维和 降维也可以看做是实现了多个Feature Map的线性组合，实现了跨通道的信息整合。具体的理论可以参加NIN论文。
**增加模型深度**
增加模型深度，一定程度上提升模型的表征能力。














