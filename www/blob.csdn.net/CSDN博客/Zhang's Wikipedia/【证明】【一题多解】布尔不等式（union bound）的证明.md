
# 【证明】【一题多解】布尔不等式（union bound）的证明 - Zhang's Wikipedia - CSDN博客


2018年07月30日 23:11:59[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1892


布尔不等式（Boole’s inequality）也叫（union bound），即并集的上界，描述的是至少一个事件发生的概率（$\mathbb{P}\left(\bigcup_i A_i\right)$）不大于单独事件（事件之间未必独立）发生的概率之和（$\sum_i\mathbb P(A_i)$）。
即：

$$
\mathbb{P}\left(\bigcup_i A_i\right)\leq \sum_i\mathbb P(A_i)
$$
展开即为：

$$
\mathbb P\left(A_1\bigcup A_2\bigcup \cdots\right)\leq \mathbb P\left(A_1\right)+\mathbb P\left(A_2\right)+\cdots
$$

## 1. 数学归纳法证明
当n=1时，显然\mathbb P(A_1) \le \mathbb P(A_1)
对于n，如果有：{\mathbb P}\left (\bigcup_{i=1}^{n} A_i\right ) \le \sum_{i=1}^{n} {\mathbb P}(A_i)，则由\mathbb P(A \cup B) = \mathbb P(A) + \mathbb{P}(B) - \mathbb{P}(A \cap B)可知：
\begin{split}
	\mathbb{P}\left(\bigcup_{i=1}^{n+1}A_i\right) =\mathbb{P}\left(\left\{\bigcup_{i=1}^{n}A_i\right\} \bigcup A_{n+1}\right) &=\mathbb{P}\left(\bigcup_{i=1}^{n}A_i\right)+\mathbb P\left(A_{n+1}\right) - \mathbb{P}\left(\left\{\bigcup_{i=1}^{n}A_i\right\} \bigcap A_{n+1}\right)\\
	&\leq \mathbb{P}\left(\bigcup_{i=1}^{n}A_i\right)+\mathbb P\left(A_{n+1}\right)
	\end{split}

## 2. 将事件转换为独立事件（不相交事件）
假设有$A_1, A_2, A_3$三个事件，则：
令B_1=A_1, B_2 = A_2-A_1，B_1与B_2不相交
令B_2 = A_2-A_1B_3=A_3-A_2-A_1，B_2与B_3不相交
令$B_i=A_i\backslash \left(\bigcup_{k=1}^{i-1} A_i\right)$，则有$B_1, B_2, \cdots,$互不相交，且$A_1\cup A_2\cup \cdots=B_1\cup B_2\cup \cdots$，自然$B_i\subset A_i$==>$P(B_i)\leq P(A_i)$：

$$
\begin{split}
\mathbb P \left(A_1\cup A_2\cup \cdots\right)&=\mathbb P\left(B_1\cup B_2\cup \cdots\right)=\mathbb P(B_1)+\mathbb P(B_2)+\cdots \\
&\leq \mathbb P(A_1)+\mathbb P(A_2)+\cdots
\end{split}
$$


