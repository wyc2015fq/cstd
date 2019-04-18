# 改进的迭代尺度法（IIS）详细分析 - 技术分享 - CSDN博客

2018年11月13日 00:27:49[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：694


改进的迭代尺度法（Improved Iterative Scaling，IIS）是一种常见的优化算法，在最大熵模型（Maximum Entropy Model，MaxEnt）和条件随机场（Conditional Random Field，CRF）中都会用IIS进行相应的处理，从而提高算法的效率。

已知模型为：

$P_{\lambda} (y|x) = \frac{1}{Z_{\lambda}(x)} exp(\sum_{1}^{n}{\lambda_i f_i(x,y) })$

式中：$f_{i}(x,y)$是二值函数， $\lambda$是参数，$Z_{\lambda}(x)$ 是归一化因子，满足：

$Z_{\lambda}(x)=\sum_{y}exp(\sum_{1}^{n}{\lambda_i f_i(x,y) })$

由$P_{\lambda} (y|x)$可得$\tilde{p}(x,y)$似然函数：

$L(\lambda) = \sum_{x,y} \tilde{p}(x,y) \log {p(y|x)}$

其中，$\tilde{P}(x,y)$是样本$(x,y)$出现的频率。模型参数$\lambda\rightarrow\lambda+\delta$时，对数似然函数的改变量为：

$\begin{matrix}L(\lambda+\delta) - L(\lambda) = \sum_{x,y} \tilde{P}(x,y)\log {P_{\lambda+\delta}(y|x)}-\sum_{x,y} \tilde{P}(x,y) \log {P_{\lambda}(y|x)} \\\\\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:=\sum_{x,y} \tilde{P}(x,y) \sum_i {\delta_i f_i(x,y)} - \sum_x{ \tilde{P}(x) \log{ \frac{Z_{\lambda+\delta}(x) }{Z_\lambda(x) } }}\end{matrix}$

使用不等式 $-\log{\alpha} \ge 1 - \alpha$ （恒成立问题，求导证明），建立对数似然函数改变量的下界：

$\begin{matrix}L(\lambda+\delta) - L(\lambda) \ge\sum_{x,y} \tilde{P}(x,y) \sum_i {\delta_i f_i(x,y)} +1-\sum_x{ \tilde{P}(x)  \frac{Z_{\lambda+\delta}(x) }{Z_\lambda(x)}}\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\:\\\\=\sum_{x,y} \tilde{P}(x,y) \sum_i {\delta_i f_i(x,y)} +1-\sum_x{ \tilde{P}(x) } \sum_{y}P_{\lambda}(y|x) exp(\sum_{i}{\delta_i f_i(x,y)})\end{matrix}$

引入$f^{\#}(x,y)$，满足：

$f^{\#}(x,y) = \sum_i {f_i(x,y)}$

记$L(\lambda+\delta) -L(\lambda)=A(\delta|\lambda)$此时：

$\begin{matrix}A(\delta|\lambda)=\sum_{x,y} \tilde{P}(x,y) \sum_i {\delta_i f_i(x,y)}+1 -\sum_x{ \tilde{P}(x) } \sum_{y}P_{\lambda}(y|x) exp(f^{\#}(x,y) \sum_{i}{\frac{\delta_i f_i(x,y)}{f^{\#}(x,y) }})\end{matrix}$

使用Jensen不等式：$exp{\sum_x p(x) q(x)} \le \sum_x{ p(x) exp\:{q(x)} }$，此时：

$\begin{matrix}A(\delta|\lambda)\ge\sum_{x,y} \tilde{P}(x,y) \sum_i {\delta_i f_i(x,y)}+1   -\sum_x{ \tilde{P}(x) } \sum_{y}P_{\lambda}(y|x) \sum_i\left({\frac{ f_i(x,y))}{ f^\#(x,y) }} exp({\delta_i f^{\#}(x,y))} \right)\end{matrix}$

记上式不等式右端为：

$\begin{matrix}B(\delta|\lambda)=\sum_{x,y} \tilde{P}(x,y) \sum_i {\delta_i f_i(x,y)}+1   -\sum_x{ \tilde{P}(x) } \sum_{y}P_{\lambda}(y|x) \sum_i\left({\frac{ f_i(x,y))}{ f^\#(x,y) }} exp({\delta_i f^{\#}(x,y))} \right)\end{matrix}$

对 $\delta_{i}$求导得：

$\begin{matrix}\frac{B(\delta|\lambda)}{\alpha\delta_{i}}=\sum_{x,y} \tilde{P}(x,y) \sum_i {f_i(x,y)}  -\sum_x{ \tilde{P}(x) } \sum_{y}P_{\lambda}(y|x) \sum_i( f_i(x,y)exp({\delta_i f^{\#}(x,y))})\end{matrix}$

令$\frac{B(\delta|\lambda)}{\alpha\delta_{i}}=0$，可以求出$\delta_{i}$，重复执行直到$\lambda$收敛。

参考文献：[The Improved Iterative Scaling Algorithm: A Gentle Introduction](http://luthuli.cs.uiuc.edu/~daf/courses/optimization/Papers/berger-iis.pdf)

