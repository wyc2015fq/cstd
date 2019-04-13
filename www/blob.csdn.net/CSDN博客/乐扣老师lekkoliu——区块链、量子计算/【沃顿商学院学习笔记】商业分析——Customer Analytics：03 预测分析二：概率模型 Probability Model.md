
# 【沃顿商学院学习笔记】商业分析——Customer Analytics：03 预测分析二：概率模型 Probability Model - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

2019年02月20日 21:15:31[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：41



## 商业进阶——概率模型
本章主要是从数据分析的长期预测方法：概率模型BTYD 模型的角度进行学习。
## 未来预测模型：BTYD MODEL背景
模型研究案例背景：
**组织**
公共广播电台主要由听众的贡献支持
**挑战**
观察听众关于他们是否每年给出的历史，我们可以预测他们未来的模式是什么？
**焦点捐助者**
最初关注1995年队列，忽略捐赠金额
11,104名首次支持者共计24,615人
**研究课题**
在接下来的6年里重复捐款的预测研究。
## 图表分析 Table Analyst
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190220202541133.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
当一个序列结束时有一个或多个“没有捐赠”时，它意味着什么？
[
](https://img-blog.csdnimg.cn/20190220202541133.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)a）捐赠者失效（即离开捐赠者池）
b）捐赠者处于休眠状态（即决定不给那一年，不考虑捐赠等）
c）我们不知道，但可以建立一个模型来提出“最佳预测”。
答案：c）我们永远不知道捐赠者是否失效; 根据他的捐赠的新近度和频率，我们可以对堕落的可能性做出有根据的猜测，因此我们可以决定在哪里投入资源。
根据我们对“死亡”概率和捐赠倾向的最佳猜测，我们可以计算每个捐赠者未来捐赠的预期频率。
## RFM理论
我们采用“BTYD”模式来预测未来的捐赠行为
该模型仅使用三个输入：
1.新近度（R）
2.频率（F）
3.每个R / F组合的人数
这需要少量数据并提供更容易使用的结构（即，数据从个人级别汇总到R / F组），通过假设捐赠者倾向的某些概率分布，我们可以构建一个易于在Excel上实现的健壮模型，这种“BTYD”建模方法在各种不同领域都有成效。
模型的大概研究方法如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190220203139991.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
## 预计2002  -  2006年的捐赠额作为接受和频率的函数分析
1.Bob（R：6，F：6）预计在2002年和2006年之间共有3.75次机会，出乎意料的是，他的捐赠率为100％。
2.Mary和Chris拥有相同的RF（6,4），因此他们预期的捐赠数量是相同的。
虽然Mary和Chris的F低于Sharmila，但他们的R越高表明他们是活着的，因此他们比Sharmila的价值高出50％
3.尽管捐赠率很高，但Sharmila（5,5）可能已经失效。
4.具有非常低的R和F的Sarah已经失效和/或非常轻的捐赠者（很难说）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019022021063148.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
**模型的有效性分析**
该模型可以很好地预测未来的捐赠行为，因为有条件的预期与实际的保持期数据非常吻合（2002  -  2006）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190220210938271.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
总体而言，该模型非常擅长拟合历史数据以及保持期数据（即模型预测与2002年至2006年之间的实际捐赠数量）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190220211033203.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
## 根据模型拓展的热图 Heat Map
## [
](https://img-blog.csdnimg.cn/20190220211033203.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
## 使用来自不同非盈利公司的更大数据集，我们创建了一个“热图”，显示哪些RF组合可能会产生最有价值的捐赠者。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190220211201863.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
## 结论
## 有许多模型可以预测未来的捐赠行为; 我们相信我们的方法不同/优越，因为：
## 1.与需要大型数据集的其他模型（通常是详细的个人级特征，例如人口统计特征）相比，该模型需要非常少量的数据（新近度和频率）。
## 2.该模型已经证明了强大的样本外验证。
## 3.该模型可以推广到其他类型的行为;它不会过度定制捐赠。
## 4.该模型可以在Excel上轻松实现;它不需要任何专有或专业软件。
## 上课心得
## 现在回归，正如我所提到的，只是做出需求预测的一个例子。 这是理解需求驱动因素的好工具， 做出需求预测，谈论最优价格。回归分析是一个简单易懂而且短期有效的工具。

## [
](https://img-blog.csdnimg.cn/20190220211033203.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==,size_16,color_FFFFFF,t_70)
