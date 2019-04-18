# Thinkpad T460P I7 6820HQ版本运算以及显卡能力简单测试 - 小灰笔记 - CSDN博客





2017年02月21日 01:04:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2782








总是忍不住剁手，尤其是在自己感觉某个阶段自己浑身不舒服的时候。上个周末又剁手买了个I7 6820HQ版本的Thinkpad T460P。大致看了下配置，应该说是有着一个不错的处理器以及显卡，只是硬盘是机械盘，我自己又加了一块128G的固态扩展了一下增强其性能。

工作中我用到最多的MATLAB以及编译器还有集中脚本解析器，说起来用到的显卡功能不是很多，主要还是侧重于计算能力。不过，在数据处理上MATLAB等工具有很多GPU加速方案，因此以后的显卡功能还是有必要在乎一下。我选择的配置除了集成显卡之后还有一个940MX的2G显卡，而测试用鲁大师跑过，分数低的吓人。但是，那些跑分工具只是参考，还是拿自己用的最多的MATLAB测试一下相关的性能吧！

1.先把MATLAB的显卡选择修改成系统跟随

![](https://img-blog.csdn.net/20170221011000863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2. 选择独立显卡，其他的默认

![](https://img-blog.csdn.net/20170221011047660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


测试结果：

![](https://img-blog.csdn.net/20170221011145943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170221011158974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


哎呀呀！惨不忍睹！看第一张得分表就能看出，这个显卡绝对是有问题。

3.选择自动选择

![](https://img-blog.csdn.net/20170221011250679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


测试结果：

![](https://img-blog.csdn.net/20170221011327117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170221011345118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





测试结果来看，跟选择独立显卡一个样子，依然是惨不忍睹！

4.选择集成显卡

![](https://img-blog.csdn.net/20170221011510616?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


测试结果：

![](https://img-blog.csdn.net/20170221011529152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170221011540756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


测试结果好多了，跟我的骷髅峡谷性能差不多。如此对比下来，看来是独立显卡有点问题。回头需要注意一下，找一下原因，是不是驱动等原因导致显卡的性能没有发挥出来？反观性能，倒还不错，相信后期在这方面能够有很好的体验。

再测试一下老Mac Mini，4代标压CPU，还是I5处理器。

![](https://img-blog.csdn.net/20170221011748414?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170221011809336?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


总体表现还说得过去，中规中矩。这是老电脑，低端一点的CPU，再加上又是集成显卡，能够有这样的成绩我觉得还是很中意的。值得一提的是，我发现Mac上测试的时候具体的程序跟Windows是有一点差异的，不知道是否跟我的软件版本有关。









