# ML的45问（4）——评估假设、贝叶斯与PAC可学习 - 刘炫320的博客 - CSDN博客
2017年06月04日 21:41:45[刘炫320](https://me.csdn.net/qq_35082030)阅读数：465标签：[机器学习																[评估假设																[贝叶斯分类器																[PAC可学习](https://so.csdn.net/so/search/s.do?q=PAC可学习&t=blog)](https://so.csdn.net/so/search/s.do?q=贝叶斯分类器&t=blog)](https://so.csdn.net/so/search/s.do?q=评估假设&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[ML](https://blog.csdn.net/qq_35082030/article/category/6800875)
所属专栏：[《Machine Learning》重点和难点](https://blog.csdn.net/column/details/16516.html)
# 1. 评估假设的意义
评估假设的3个意义：
- 确定哪个假设更具有普适性。
- 当前样本训练出的数据错误率的可信度是多少。
- 如何利用有限的数据，获得更好的假设。
# 2. 置信区间的计算
前提：
- n>30
- 如果没有其他信息提供，则真实错误率$error_D(h)$与样本错误率$error_s(h)$是一致的。
计算示例，一般多用在计算最少赢手机的样例数是多少的题目。例如下题：
*要测试一假设h,其$error_D(h)$已知在0.2到0.6的范围内。要保证95%双侧置信区间的宽度小于0.1，最少应搜集的样例数是多少？*
解：查表可知，置信度为95%，则Z为1.96，因此应当满足下式： 
$1.96\times \sqrt{\frac{error_D(h) \times (1-error_D(h))}{n}}<0.05$
$n> \frac{error_D(h) \times (1-error_D(h))}{0.000651}$
然后解得$n=385$
其实这里用$error_D(h)$和$error_s(h)$没有太大区别，因为就像前提2所示的。
# 3. 贝叶斯学习方法的特性
- 观察到的每个训练样例可以增量的降低或升高某假设的估计概率。而其他算法遇到不一致时，会完全去掉该假设。
- 先验知识可以与观察数据一起决定假设的最终概率。
- 贝叶斯方法可允许假设做出不确定性预测。
- 新的实力分类可由多个假设一起作出预测，用他们的概率来加权。
# 4. 最大后验假设与一致学习器的关系
一致学习器指的是它输出的假设在训练样例上有0错误率。 
若有均匀的先验概率且无噪声。那么每一个输出假设都是最大后验假设。
# 5. 最大后验假设与最小误差平方和一致的条件
$h_{MAP}=arg\max_{h \in H}P(h|D)$
$h_{MAP}=arg\max_{h \in H}\frac{P(D|h)P(h)}{P(D)}贝叶斯公式$
$h_{MAP}=arg\max_{h \in H}P(D|h)P(h)省略P(D)$
$h_{ML}=arg\max_{h \in H}P(D|h)每个概率都一样，变成最大似然$
$h_{ML}=arg\max_{h \in H}\prod_{i=1}^m P(d_i|h)求积$
$h_{ML}=arg\max_{h \in H}\prod_{i=1}^m \frac{1}{\sqrt{2\pi \sigma^2}}e^{-\frac{1}{2\sigma^2}(d_i-\mu)^2}中心极限定理$
$h_{ML}=arg\max_{h \in H}\prod_{i=1}^m \frac{1}{\sqrt{2\pi \sigma^2}}e^{-\frac{1}{2\sigma^2}(d_i-h(x_i))^2}换成可算$
$h_{ML}=arg\max_{h \in H}\sum_{i=1}^m[In{\frac{1}{\sqrt{2\pi \sigma^2}}-\frac{1}{2\sigma^2}(d_i-h(x_i))^2]}取对数$
$h_{ML}=arg\max_{h \in H}\sum_{i=1}^m[-\frac{1}{2\sigma^2}(d_i-h(x_i))^2]省略常数项$
$h_{ML}=arg\min_{h \in H}\sum_{i=1}^m[\frac{1}{2\sigma^2}(d_i-h(x_i))^2]最大变最小$
$最小误差平方和=arg\min_{h \in H}\sum_{i=1}^m[(d_i-h(x_i))^2]最大变最小$
# 6. 最大后验假设与最小描述长度编码的等价关系
$h_{MAP}=arg\max_{h \in H}P(D|h)P(h)省略P(D)$
$h_{MAP}=arg\max_{h \in H}log_2P(D|h)+log_2P(h)求对数$
$h_{MAP}=arg\min_{h \in H}-log_2P(D|h)-log_2P(h)最大变最小$
$h_{MAP}=arg\min L_{cH}(h)+L_{c(D|h)}(D|h)转换$
$h_{MDL}=arg\min L_{c_1}(h)+L_{c_2}(D|h)$
若CH=C_1,C(D|h)=C_2,则$h_{MAP}=h_{MDL}$
# 7. 朴素贝叶斯分类器过程
- 首先找出类别概率P（yes）、P（no），是多少就是多少，不用m估计。
- 再计算测试样例中，每个属性值的条件概率：
$p(yes|h)=p(yes)\times p(特征1|yes)\times p(特征2|yes) \times p(特征n|yes)$
$p(no|h)=p(no)\times p(特征1|no)\times p(特征2|no) \times p(特征n|no)$
- 最后进行归一化
# 8.  打散的概念
对于一个给定集合$S=\{x_1,x_2,...,x_d\}$，如果一个假设类H能够实现集合S中所有元素的任一中标记方式，则称H能够分散S。
也就是说，假设空间H是S的所有标记总和。
# 9. VC维
指能够被H打散的最大集合的大小，线性面里N维的VC维是N+1。
# 10. PAC学习定义
能够从**合理**数量的训练数据中，通过**合理**的计算量**可靠**的学习到知识。 
要求： 
1） 不要求零错误率，错误率可以在某个非常小的常数范围内。 
2）不要求对所有数据都能成功预测，失败概率也可以在某个非常小的常数范围内。
