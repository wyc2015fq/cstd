
# 【沃顿商学院学习笔记】商业分析——People Analytics：08 相关性和因果关系问题的研究 - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

2019年03月01日 11:31:25[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：97



# 商业进阶——人力分析
本章从员工周期的招聘角度进行学习。
## 为什么关心因果关系 Why We Care About Causality
经过研究可以看到以下现象：
•通过正式职位进入职位的人表现更差
-我们应该避免发布招聘吗？
•通勤时间最长的人员表现较差
-我们应该让人们更多地搬家吗？
•参加过培训计划的人员表现更好
-我们应该派更多人参加培训吗？
•参加培训计划的人员表现出更大的绩效改善
-我们应该派更多人参加培训吗？
我们不能仅仅根据表象的联系就来确定其因果关系，我们必须更多的在数据和逻辑上考虑。
# 两种因果关系问题  Two Types of Causality Problems
**\#1 忽略变量的偏差**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190301111329108.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
•我们是否只在最困难的时候发布工作岗位？
•人们是否只是在绩效最差时候接受培训？
**\#2 反向因果的关系**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190301111336579.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWtrb2xpdS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
•我们表现最出色的人员是否已退出工作岗位而退出中层表演者？
•我们表现最好的人员是否接受过培训？
**核心与基础问题均在于：**
什么导致我们的主预测变量的差异？
# 因果关系解决方案 Approaches to Addressing Causality
对于有效性Effectiveness和难度Difficulty两个维度上，我们以此由低到高进行以下四个策略：
1.测量和控制遗漏的变量 Measure and control for omitted variables
3.开发自然随机化来源 Exploit natural sources of randomization
2.寻找证据来排除替代方案 Look for evidence to rule out alternatives
4.进行实验 Conduct an experiment
**1.测量和控制遗漏的变量 Measure and control for omitted variables**
优点：
•收集可能遗漏的变量和数据
-包含在回归中
-创建具有相似值的匹配对
•检查人员变化以保持人不变
缺点：
•并非一切都可以衡量
**2.寻找证据来排除替代方案 Look for evidence to rule out alternatives**
•替代解释会产生什么影响？
•您能在数据中找到支持或反对这些解释的证据吗？
**3.开发自然随机化来源 Exploit natural sources of randomization**
优点：
•“自然实验”以不应影响Y的方式更改X变量
•在真实实验中模拟治疗与对照组的分配
•允许评估“因果效应”
缺点：
•你需要足够的幸运
**4.进行实验 Conduct an experiment**
优点：
•随机将个人/工作分配给“治疗”和“控制”组（确保每组的平衡特征）
•测试两组的结果是否不同
缺点：
•你需要说服人们让你这样做
•非常耗时
## 上课心得
我们从数据出发，基于四个策略的解决方案来分辨出相关性和实际因果关系。

