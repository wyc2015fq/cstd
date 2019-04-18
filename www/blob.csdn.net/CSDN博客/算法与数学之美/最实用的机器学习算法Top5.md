# 最实用的机器学习算法Top5 - 算法与数学之美 - CSDN博客
2019年03月31日 22:18:47[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：14
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkyibhvGLgaqX9NkLdhIcdPwR25icvKqfV9L0leq4zoIuDGk5MVNXKRO7bZg9KgWg9ozC6icfRIWtSQKg/640?wx_fmt=gif)
本文将推荐五种机器学习算法，你应该考虑是否将它们投入应用。这五种算法覆盖最常用于聚类、分类、数值预测和朴素贝叶斯等四个门类。
 1. 聚类算法：k-means 
聚类算法的目标：观察输入数据集，并借助数据集中不同样本的特征差异来努力辨别不同的数据组。聚类算法最强大之处在于，它不需要本文中其他算法所需的训练过程，您只需简单地提供数据，告诉算法你想创造多少簇（样本的组别），算法会为每个簇来分配一个编号。这种规范聚类算法就是k-means。
举个例子，你的应用可用k-means来按照营销目的区分你的网络服务的用户。你只需要输入（从电子商务网站获取的）一组客户的购买历史，并确定四组客户以进行分类营销。此时，你为k-means提供了一个数据表，每行代表一名客户，每列则是各种对客户的购买行为特征（如成为客户的时间、每月评价花费、每月评价订单量、地理位置、对当天航运的使用比例等等）。算法会为表格增加一栏：编号1-4来表示不同的分组。
提示：使用kmeans函数或rxKmeans. 用以执行bt rx的函数是ScaleR的一部分，所以不能支持量很大的数据集。
 2&3. 两类、多类分类算法 
分类算法的目标：输入一行数据及一个类目名称表，通过对数据的校验估测其所属的类目。分类算法通常按照分类时的类目总数分为两类和多类分类算法。在你使用种算法预测新数据前，你需要预先使用一组类目可知的数据对算法加以训练。
不妨举个简单的例子来说明两类分类算法：想象一个你希望得到是/否（或真/伪）的两极化情形。此时，类别分别为“是”和“否”（或者“真”和“伪”）。两类分类的典型应用是：根据历史天气条件（如温度、风速、降水、气压）和航班信息（如航空公司、起飞时间、航班号）来预测未来航班会推迟15分钟离开还是照常起飞。输出分为“延迟”和“不延迟”两类。在大多数情况下，两类分类算法的核心是逻辑回归的使用，后者用于生成一个在0到1范围内的值。如果该值小于0.5，往往会解释为第一个类（如“不延迟”），否则会划为第二类（如“延迟”）。
另一个应用是：预测贷款的偿还情况，来作为拓展信用的一部分。你必须提供贷款人的相关数据，譬如信用分数、房产年限、工作时长、信用卡债务总额、数据采集年份以及有贷款违约记录。分类为“会违约”和“不会违约”。这个例子之所以非常有趣，是因为对于一些机器学习所处理的难题来说，仅仅有预测结果并不足够，还必须知晓结论是怎样得出的。这时多组分类算法形成的决策树就派上用场了，决策树中的多组分类算法可以分析从输入到最终生成预测的过程。回到贷款这一案例，不妨想象你是银行方，当你拒绝了消费者的贷款新的信用卡要求时，消费者的下一个问题可能是“为什么？”。通过决策树，你可以具体回答：“好吧，你的信用分数太低了，信用卡债台高筑，工作年头又太短了。”
充分理解两类分类算法后，进一步理解多类分类算法就顺理成章了。多类分类算法可应用于电影院，来告诉后者某部电影究竟是垫底的票房毒药（不仅口碑差，而且主流观众不感兴趣）、口碑导向（叫好不叫座）还是票房导向（口碑差，但是票房高）。三个分类分别是“毒药”、“口碑导向”、“票房导向”。你也可以使用决策树，来了解一部电影为何会得到这样的评价。
提示：使用glm或rxLogit来进行两类或多类分类。进行多类分类时，你可以使用rpart或rxDTree来建立可观察决策树。
 4. 数值预测 
数值预测算法的目标：根据一组输入，预测一个具体数值。仍然使用上文中航班延误的例子，我们现在要做的不再是预测航班是否会推迟15分钟，数值预测算法将让你知晓航班将具体被推迟多久。数值预测算法的核心是线性回归的使用（不要与分类算法使用的逻辑回归混淆），通过对历史数据的线性拟合，线性回归可以有效地进行数值预测。最好的例子是股票的预测，线性回归根据过往数据的分布，拟合出一条最合适的直线，延长这条线你就可以预测将来的股票价格。
提示：使用Im或rxLinMod函数。
 5. 朴素贝叶斯算法 
最后，你的采纳清单上还缺一个机器学习中劳苦功高的算法——朴素贝叶斯算法。它的核心是因果关系。更具体地说，朴素贝叶斯算法是在给定已知的成因的情况下，预测这一成因将产生的效果以及效果的程度。现实生活中的典型例子是癌症检测。你可以通过对病人特定病状（效果）的观察，来预测病人患有癌症的概率（疾病是成因）。再举个和你相关的例子，朴素贝叶斯算法可以根据你已购买的商品为你推荐其他商品。例如，你网购了一些鸡蛋，算法会提醒你买些牛奶，因为你的购买历史显示：购买鸡蛋（成因）会导致购买牛奶（效果）。朴素贝叶斯有趣的一点在于，它可以用于分类和数值预测。。
提示：使用naiveBayes和rxNaiveBayes函数。
 结论 
好了，你有了一个实用算法的清单。现在，重新审视你的应用，想一想哪些地方可以增添点机器学习带来的智能性。
**————**
编辑 ∑Pluto
来源：IT经理网
微信公众号“算法数学之美”，由算法与数学之美团队打造的另一个公众号，欢迎大家扫码关注！
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyjQVC3BFiaFKQ4DJqz2xhrwkzeCPbjQdnnG8678fRf1sxc2ZQtvtVib2dqWUkeopYtmgckINoOoGoQ/640?wx_fmt=jpeg)
**更多精彩：**
☞[如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483726&idx=1&sn=e5e008fb68a7d837546d0ac5b5438042&chksm=ebe9cbf3dc9e42e5d625b2da6b9b3866dff9f08d442d8106f4cbf035d8602e1fdda86eec6476&scene=21#wechat_redirect)
☞[自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483726&idx=2&sn=18272b7eaa172794b51c30d0a2dd9c48&chksm=ebe9cbf3dc9e42e5ddf9a189822a2fa099543a631ad63a1d6ed0158b51c76212eb65ebbfe71b&scene=21#wechat_redirect)
[☞费马大定理，集惊险与武侠于一体](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483715&idx=1&sn=9069dadf4bbce2aa34bd64b85a69dcee&chksm=ebe9cbfedc9e42e81c27d72da15c0dbf848e505946f231051b8b4033d0941bc6f51cef32790e&scene=21#wechat_redirect)
☞[简单的解释，让你秒懂“最优化” 问题](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483715&idx=2&sn=55a16f37c89b27994b263e0dc9837561&chksm=ebe9cbfedc9e42e842deb581ea62b750cedd839abd58c2db3261bf9fbcd172a2cf18512e4d2d&scene=21#wechat_redirect)
[☞一分钟看懂一维空间到十维空间](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483676&idx=1&sn=2366a39bca3ef42a6e868e91ea718813&chksm=ebe9cba1dc9e42b70c7e147b9e43828c1a7c68401f442890471a06e5cf0704437f9813ca0e0f&scene=21#wechat_redirect)
☞ [本科、硕士和博士到底有什么区别？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483661&idx=1&sn=d822666a054ba70b37dfb06d14c60f3a&chksm=ebe9cbb0dc9e42a6c476f7f81095b772aa45d960bf516f60c5b2e1155c9093696222cea0a83d&scene=21#wechat_redirect)
☞[小波变换通俗解释](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=1&sn=7d0d05c78cd01df91495f1d14609cbce&chksm=ebe9cbbbdc9e42add13cfe99f3383745fa5c059df705a3a9e28644d073dff804569af94970e3&scene=21#wechat_redirect)
☞[微积分必背公式](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=4&sn=ce88086b650c601bdbf57ecfe5a490a1&chksm=ebe9cbbbdc9e42adfaf0e4ee644d254835c830ef47663315b70a39a2b47e6a7cf10d0826b88d&scene=21#wechat_redirect)
☞[影响计算机算法世界的十位大师](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=3&sn=ceaec6043bb0e8a851033482f8f572bf&chksm=ebe9cbbbdc9e42ad30fd38383cf1caa609ac6e81964da17277f8e2a7f17a933cd11e0f3840c8&scene=21#wechat_redirect)
☞[数据挖掘之七种常用的方法](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=2&sn=bf439d56bc7d42083708fa76434a6025&chksm=ebe9cbbbdc9e42ad9e5f2b3c1952e620e0e3d4452aae25b611e7e54be8678b0d80e002e7be6d&scene=21#wechat_redirect)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
