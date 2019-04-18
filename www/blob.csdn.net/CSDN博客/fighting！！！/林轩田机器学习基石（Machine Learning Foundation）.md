# 林轩田机器学习基石（Machine Learning Foundation） - fighting！！！ - CSDN博客
2019年01月12日 12:21:29[dujiahei](https://me.csdn.net/dujiahei)阅读数：60
## 第一课 机器学习问题
### 什么是机器学习？
什么是“学习”？学习就是人类通过观察、积累经验，掌握某项技能或能力。就好像我们从小学习识别字母、认识汉字，就是学习的过程。而机器学习（Machine Learning），顾名思义，就是让机器（计算机）也能向人类一样，通过观察大量的数据和训练，发现事物规律，获得某种分析问题、解决问题的能力。
机器学习可以被定义为：Improving some performance measure with experence computed from data. 也就是机器从数据中总结经验，从数据中找出某种规律或者模型，并用它来解决实际问题。
什么情况下会使用机器学习来解决问题呢？其实，目前机器学习的应用非常广泛，基本上任何场合都能够看到它的身影。其应用场合大致可归纳为三个条件：
- 事物本身存在某种潜在模式/规则
- 某些问题难以使用普通编程解决
- 有大量的数据样本/资料可供使用
### 机器学习的组成
术语：
输入x
输出y
目标函数f，即最接近实际样本分布的规律
训练样本 data
假设hypothesis，一个机器学习模型对应了很多不同的hypothesis，通过演算法A，选择一个最佳的hypothesis对应的函数称为矩g，g能最好地表示事物的内在规律，也是我们最终想要得到的模型表达式。
![](https://img-blog.csdnimg.cn/20190112112200320.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190112112536810.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
    对于理想的目标函数f，我们是不知道的，我们手上拿到的是一些训练样本D，假设是监督式学习，其中有输入x，也有输出y。机器学习的过程，就是根据先验知识选择模型，该模型对应的hypothesis set（用H表示），H中包含了许多不同的hypothesis，
通过演算法A，在训练样本D上进行训练，选择出一个最好的hypothes，对应的函数表达式g就是我们最终要求的。一般情况下，g能最接近目标函数f，这样，机器学习的整个流程就完成了。**模型包括：演算法和假设**。
  与机器学习相关的领域有：数据挖掘（Data Mining）、人工智能（Artificial Intelligence）、统计（Statistics），机器学习与他们，基本类似，但也不完全一样，他们之间没有十分明确的界线。机器学习是这三个领域中的有力工具。
![](https://img-blog.csdnimg.cn/20190112113518830.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
## 第一课 回答yes/no--什么样的机器学习算法可以回答是非题
![](https://img-blog.csdnimg.cn/20190112114457827.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
Guarantee of PLA、Non­Separable Data 两个部分听得有点懵，让我梳理梳理再补充笔记。
