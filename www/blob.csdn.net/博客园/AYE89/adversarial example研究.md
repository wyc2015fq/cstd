
# adversarial example研究 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[adversarial example研究](https://www.cnblogs.com/eniac1946/p/8033266.html)
|Posted on|2017-12-13 15:50|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8033266)|[收藏](#)
|Paper：
|Practical Black-Box Attacks against Machine Learning
|一、介绍
|概况：Ian Goodfellow大神研究如何在不知道model内部结构和训练数据集的情况下（黑盒），产生adversarial example误导DNN分类器。
|成果：
![](https://images2017.cnblogs.com/blog/1181483/201712/1181483-20171213143946894-550144947.png)
|1）需要一个“人造”数据集，用于训练“替代”model，如何产生？
|2）对不同DNN攻击
|3）减少query的方法，在训练“替代”model时
|4）为什么可以用“替代”model，附录B中解释
|二、威胁模型
![](https://images2017.cnblogs.com/blog/1181483/201712/1181483-20171213150029191-1696865009.png)
|初始sample + 扰动，使得分类器结果不等于正确结果（出错）。问题转化为：优化找到最小扰动。
|三、黑箱策略
|1. “替代”model训练
|减少query：使用“|Jacobian-based Dataset Augmentation|”
|即启发式：由一组初始数据，然后根据目标model输出变化的方向，产生其余数据。
|1.1model的结构
|论文指出：类型、层数、数量对攻击成功与否影响较小；若要提高准确率，需研究此问题。
|遍历目标model整个输入空间从而获得输出不现实。
|训练“替代”model的算法
![](https://images2017.cnblogs.com/blog/1181483/201712/1181483-20171213152234441-1659754921.png)
![](https://images2017.cnblogs.com/blog/1181483/201712/1181483-20171213152636176-1336577317.png)
|1.2 产生adversarial example
|fast gradient sign method
|Ian J Goodfellow, et al. Explaining and harnessing adversarial
|examples. In Proceedings of the International Conference on
|Learning Representations, 2015.







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
