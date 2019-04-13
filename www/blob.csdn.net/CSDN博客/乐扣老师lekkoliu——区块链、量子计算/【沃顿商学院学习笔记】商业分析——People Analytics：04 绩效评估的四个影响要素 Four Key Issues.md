
# 【沃顿商学院学习笔记】商业分析——People Analytics：04 绩效评估的四个影响要素 Four Key Issues - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

2019年02月27日 15:36:35[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：21



# 商业进阶——人力分析
我们从四个关键的要素来学习绩效评估：
均值回归 Regression to the Mean
样本量
信号独立性
流程与结果
## \#1 均值回归 Regression to the Mean
一个简单的模型
•绩效有两个组成部分：
用非正式术语：真正的倾向+运气
用更正式的术语：y = x + e，
•x =真实能力，和
•e =错误，随机分布在0左右。
•当我们对极端绩效进行采样时会发生什么？ 什么是极端成功和失败的基础？
极端成功= f（超强能力，正误差）
极端失败= f（劣等能力，负面错误）
•后果是什么？
忽略均值回归和均值回归的不同预测和实际结果：
•最近进行了一项研究，研究了20世纪90年代存在的283只股票共同基金的表现。 该研究将20世纪90年代分为早期（1990-1994）和晚期（1995-1999）。 以下是早期收益率最高的10只基金（名字伪装），排名从1到10.预测他们在20世纪90年代后期的排名。
结果表明实际上基金的表现并没有像预测的那样好的持续更好，而更多的取决于运气或者其他因素。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227151006433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
更多例子：例子
•警官以色列空军 - “惩罚主义有效而非赞美。 每当我在一次非常糟糕的飞行后惩罚飞行员时，我会在下次看到更好的表现。 每当我在一次出色的飞行后赞扬一名飞行员，我下次会看到更糟糕的表现。“
•彼得斯和沃特曼的着作“寻求卓越”。 他们在20世纪80年代早期选择了43家高绩效公司，并期待看到他们使用了哪些做法（他们发现的一些是组织相当于“刷牙”）
•表现优异的五家公司五年后的预测和对比：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227151247833.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
可以说：无论何时根据一个属性的极值进行采样，任何其他不完全相关的属性都会更接近平均值。
[
](https://img-blog.csdnimg.cn/20190227151247833.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)“属性”可以是：
•表现不同的时间点例如，去年的股票收益率和今年的收益率
•具有不同实体的不同等级
例如，一个人的跑步速度和语言能力
是什么妨碍了他们的持续性？
• 除其他事项包括：
结果偏见
事后偏见
叙事寻求
•简而言之，我们过去的理解一般是
我们找到一个连接所有点的故事，希望优秀的持续优秀。
机会在这些故事中只是起着太小的作用，而实际不是。
\#2 样本大小 Samples size
从小样本推断的案例一：
•您的公司有两个工厂，一个大型工厂，一个小型工厂，大规模生产标准计算机芯片。 除了它们产生的量之外，这两种植物在所有关键问题上都是相同的。 两者都使用相同的技术来生产相同的产品。 如果功能正常，这种特殊技术会产生百分之一（1％）的缺陷品。 从一天的生产中得到的保险项目超过百分之二（2％），在质量控制日志中特别注意“标记”问题。 在本季度结束时，您希望哪个工厂在其质量控制日志中有更多“被标记”的日子？ 请标记一个。
大部分人的选择结果如下
22％•A）小型工厂
30％•B）大型工厂
48％•C）相同数字平均值
而实际上，答案是A我们需要注意因为小工厂的样本数小，因此容易产生样本波动和波动幅度较大，很难接近标准质量的平均值。
从小样本推断的案例二：
原理：样本意味着收敛到群体意味着样本增加。 （这被称为中心限制定理。）因此，您将在小样本中看到更极端的值。
你是否更有可能在棒球比赛中看到.400赛季的平均击球率 -  5月1日或9月1日？
在哪个医院，您更有可能看到比任何一天出生的男孩（或反之亦然）高得多的男孩 - 一个小型社区医院（例如，每天5个孩子）或一个大型城市医院（例如100个）出生/天）？
“小数定律” Law of Small Numbers
•人们认为小样本与基础人群的属性密切相关。
•这意味着他们很容易从样本中推断出人口的属性（例如，平均值）。
•他们忽略了在小样本中不可避免地扮演的角色变异性（即机会）。
＃3：信号独立 Signal Independence
•大量预测的平均值可靠地优于个人平均预测。
特殊错误相互抵消
例如，高尔顿（1906年）的县公平竞赛
许多其他例子
•但是，在他们的意见独立的情况下，他们的价值取决于他们。
•独立意味着不相关。
•如果相关，其他意见的价值会迅速减少。
调查结论如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227152451560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
•人们不善于考虑这种影响
[
](https://img-blog.csdnimg.cn/20190227152451560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)即使你确切地告诉他们相关性是什么，人们也没有适当调整（Enke＆Zimmerman，2015）。
[
](https://img-blog.csdnimg.cn/20190227152451560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)•两种观点之间的相关性来源？
[
](https://img-blog.csdnimg.cn/20190227152451560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)他们已经讨论过了！
他们与同一个人交谈
他们有相同的背景
从同一个地方，以同样的方式训练，相同的历史经验等。
[
](https://img-blog.csdnimg.cn/20190227152451560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)•需要找到保持意见独立性的方法，并为经验丰富的群体添加独立观点。
[
](https://img-blog.csdnimg.cn/20190227152451560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)＃3：流程和结果 Process vs. Outcome
[
](https://img-blog.csdnimg.cn/20190227152451560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)考虑更广泛的目标
•组织通常关心他/她的工作
最重要的是：对他人的影响
•人们认为目标太少（Bond，Carlson＆Keeney，2008）。
系统地省略了近一半的目标，他们后来认为这些目标是个人相关的
•导致许多公司依赖过于狭窄的一系列绩效指标。
•众所周知的案例，戴尔电脑公司在21世纪初改变了他们的绩效评估。更改前：100％结果
改变后：
•50％的雇员完成工作，
•受影响的人判断的50％/她是什么样的。
•环境的不确定性，即员工对确切结果的无形控制，企业应该在评估过程中强调过程。
专注于流程
使用分析来更好地理解和关注倾向于产生预期结果的过程。
•关键问题：确定价值的基本驱动因素。
•比如销售额中的“更多基础驱动因素”是什么？
出价？
会议？
联系人？
绩效评估总结
•1）了解环境
知道你有偏见
考虑机会
•2）询问批评性问题
本章小结
了解您的环境
•多少是运气和多少是努力问题？
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190227153448413.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
了解你的偏见
1）非回归预测
2）结果偏差
3）事后偏见
4）叙事偏见
考虑到机会因素占比
关键问题：持久性。
绩效指标越基本（与技能相关），它随着时间的推移会越持久。
绩效衡量指标与机会相关的越多，它随着时间的推移逐渐回归均值。
其中关键问题
•差异是持久的还是随机的？ 即，我们怎么知道这不仅仅是好运/坏运气？
•样本是否足够大以得出有力的结论？ 我们怎样才能让它更大？
•在这里点击了多少个不同的点燃？ 我们怎样才能使它们尽可能独立？
•我们还关心什么？我们的测量足够吗？ 我们可以衡量哪些更基本？
上课心得
对于分析的时候四个影响要素的学习，让我们更加理性的去对数据进行优化、建模，然后分析出更理性的解决方案。

