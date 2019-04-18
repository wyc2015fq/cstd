# 贝叶斯分类（Bayesian） - wydbyxr的博客 - CSDN博客
2018年10月22日 11:59:32[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：89
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 贝叶斯分类（Bayesian）
  贝叶斯分类是一类分类算法的总称，这类算法均以贝叶斯定理为基础，故统称为贝叶斯分类。
贝叶斯方法是在解决归类和回归问题中应用了贝叶斯定理的方法。
  参考资料：[https://www.cnblogs.com/nxld/p/6607943.html](https://www.cnblogs.com/nxld/p/6607943.html)
# 贝叶斯定理
## 条件概率公式：**P(A|B) = P(AB)/P(B) = P(B|A)*P(A)/P(B)**
![在这里插入图片描述](https://img-blog.csdn.net/20181022115446504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
  从公式中可知，如果要计算B条件下A发生的概率，只需要计算出后面等式的三个部分：
  1）B事件的概率（P(B)），是B的先验概率；
  2）A属于某类的概率（P(A)），是A的先验概率；
  3）已知A的某个分类下，事件B的概率（P(B|A)），是后验概率。
  朴素贝叶斯最核心的部分是贝叶斯法则，而贝叶斯法则的基石是条件概率。
  如果要确定某个样本归属于哪一类，则需要计算出归属不同类的概率，再从中挑选出最大的概率。
  我们把上面的贝叶斯公式写出这样，也许你能更好的理解：**MAX(P(Ai|B))=MAX(P(B|Ai)*P(Ai)/P(B))**
  这个公式告诉我们，需要计算最大的后验概率，只需要计算出分子的最大值即可，而不同水平的概率P©非常容易获得，故难点就在于P(X|C)的概率计算。而问题的解决，正是聪明之处，即贝叶斯假设变量X间是条件独立的，故而P(X|C)的概率就可以计算为：**P(B|Ai) =P(B1/Ai)*P(B2/Ai)*P(B3/Ai)*…*P(Bn/Ai) **
## 全概率公式：**P(X)=∑[P(X|Y=Yk)*P(Yk)]	其中∑P(Yk)=1**
## 贝叶斯公式：
  根据概率P(Bi) 及 P(A|Bi) （i=1,2,…,n），计算P(Bi|A) ，**P(Bi|A) = P(ABi)|P(A) = P(Bi)P(A|Bi)/(ΣP(Bj)P(A|Bj))  （j从1到n）**
  简单来说也就是说贝叶斯公式完成了从P（A|B）到P（B|A）的转换。	
  把P(A)称为"“先验概率”"（Prior probability），即在B事件发生之前，我们对A事件概率的一个判断。
  P(A|B)称为"“后验概率”"（Posterior probability），即在B事件发生之后，我们对A事件概率的重新评估。
  P(B|A)/P(B)称为"“似然函数”"（Likelyhood），这是一个调整因子，使得预估概率更接近真实概率。
  所以，条件概率可以理解成下面的式子：**后验概率　＝　先验概率 ｘ 调整因子**
**后验概率 = (似然度 * 先验概率)/标准化常量**
**后验概率 = 标准似然度 * 先验概率**
  这就是贝叶斯推断的含义。我们先预估一个"“先验概率”"，然后加入实验结果，看这个实验到底是增强还是削弱了"“先验概率”"，由此得到更接近事实的"“后验概率”"。
# 各种贝叶斯
朴素贝叶斯 Naive Bayes
高斯贝叶斯 Gaussian Naive Bayes
多项朴素贝叶斯 Multinomial Naive Bayes
Averaged One-Dependence Estimators (AODE)
贝叶斯信念网络（BNN） Bayesian Belief Network(BBN)
贝叶斯网络（BN） Bayesian Network(BN)
# 参考资料：[https://blog.csdn.net/syoya1997/article/details/78618885](https://blog.csdn.net/syoya1997/article/details/78618885)
