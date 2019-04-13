
# 机器学习中的方差偏差分析（Bias-variance analysis） - 颹蕭蕭 - CSDN博客


2018年09月18日 22:49:05[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：912


在预测问题中，给定一个新的数据点，**预测错误的期望**是多少？
假设数据是独立同分布地从一个潜在固定的概率分布中获取的，假设其分布函数为$P(&lt;\textbf{x},y&gt;)  = P(\textbf{x})P(y|\textbf{x})$，我们的目标就是对任意给定的数据点$x$, 求出
$$
E_P[(y−h(\textbf{x}))^2|\textbf{x}],
$$
其中，y 是数据集中$\textbf{x}$对应的值，期望是针对所有数据集，下标 P 表示所有数据集是从同一分布 P 中获取的。形式上，该值是某一点$\textbf{x}$在多个数据集上的预测错误的均值（期望）。
对于给定的假设集，我们可以计算出模型的**真实错误（true error）**，也称泛化错误、测试错误
$$
\sum_{\textbf{x}}E_P[(y−h(\textbf{x}))^2|\textbf{x}]P(\textbf{x}),
$$
即为*所有数据点*在那个输入数据的潜在固定分布上的预测错误的期望。如果$\textbf{x}$为连续变量，则上述求和转化成积分形式。
我们接下来将把**真实错误（true error）**一分为三：
$$
\textbf{真实错误 = 偏差 + 方差 + 噪声。}
$$
关于方差和期望的基本结论：
$$
E[X^2] = (E[X])^2 + V ar[X]\\E(XY) = E(X)E(Y) + Cov(X,Y)
$$
先做一个简单展开：
$$
E_P[(y−h(\mathbf{x}))^2|\mathbf{x}] \\\,\\= E_P [(h(\mathbf{x}))^2 − 2yh(\mathbf{x}) + y^2|\mathbf{x}]\\\,\\=E_P [(h(\mathbf{x}))^2|\mathbf{x}] +E_P[y^2|\mathbf{x}] -2E_P[y|\mathbf{x}]E_P[h(\mathbf{x})|\mathbf{x}]，……(1)
$$
上式中包含三项。令$\overline{h}(\mathbf{x})=E_P[h(\mathbf{x})|\mathbf{x}]$，表示点 x 在不同数据集上（分布P上）预测的均值（期望），则
**第一项**
运用方差的结论：平方的期望=期望的平方+方差
$$
E_P [(h(\mathbf{x}))^2|\mathbf{x}]=(\overline{h}(\mathbf{x}))^2+E_P [(h(\mathbf{x})-\overline{h}(\mathbf{x}))^2|\mathbf{x}]。……(2)
$$
**第二项**
运用方差的结论：平方的期望=期望的平方+方差
$$
E_P [y^2|\mathbf{x}]=(E_P(y|\mathbf{x}))^2+E_P [(y-f(\mathbf{x}))^2|\mathbf{x}]
$$
注意到$E_P(y|\mathbf{x}) = E_P(f(\mathbf{x})+\epsilon|\mathbf{x})=f(\mathbf{x})$，其中$\epsilon\sim N(0,\sigma)$，故上式化为
$$
E_P [y^2|\mathbf{x}]=(f(\mathbf{x}))^2+E_P [(y-f(\mathbf{x}))^2|\mathbf{x}]。……(3)
$$
将(2)(3)代入(1)，得
$$
E_P[(y−h(\mathbf{x}))^2|\mathbf{x}] 
\\\,\\=E_P [(h(\mathbf{x}))^2|\mathbf{x}] +E_P[y^2|\mathbf{x}] -2f(\mathbf{x})\overline{h}(\mathbf{x})
\\\,\\=(\overline{h}(\mathbf{x}))^2+E_P [(h(\mathbf{x})-\overline{h}(\mathbf{x}))^2|\mathbf{x}] \\+ (f(\mathbf{x}))^2+E_P [(y-f(\mathbf{x}))^2|\mathbf{x}] \\-2f(\mathbf{x})\overline{h}(\mathbf{x})
\\\,\\=E_P [(h(\mathbf{x})-\overline{h}(\mathbf{x}))^2|\mathbf{x}] +(f(\mathbf{x})-\overline{h}(\mathbf{x}))^2 + E_P [(y-f(\mathbf{x}))^2|\mathbf{x}] 。……(*)
$$
**大功告成！！！！！！！！！！!！!！！！!！!！!！!！！！！!**
$$
E_P [(h(\mathbf{x})-\overline{h}(\mathbf{x}))^2|\mathbf{x}]
$$
为 预测的方差；
$$
(f(\mathbf{x})-\overline{h}(\mathbf{x}))^2
$$
为平方偏差；
$$
E_P [(y-f(\mathbf{x}))^2|\mathbf{x}]
$$
为噪声

