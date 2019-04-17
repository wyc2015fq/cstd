# Hinge loss function - zkq_1986的博客 - CSDN博客





2016年11月12日 15:57:58[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：392








## 1公式



Hinge loss损失函数l(y)主要用于SVM的参数**w**和b的训练中，其形式为： 


$\ell(y) = \max(0, 1-t \cdot y)$

其中t表示1或-1的类别，y表示样本的实际位置，且有|y|>=1。 

如果y分到正确的类，即与t同方向，那么l(y)=0，否则l(y)>0。
对于样本x，其y值为wb（w已经将原本的w和b合并成一个），那么l(y)对参数w的梯度为： 


${\frac  {\partial \ell }{\partial w_{i}}}={\begin{cases}-t\cdot x_{i}&{\text{if }}t\cdot y<1\\0&{\text{otherwise}}\end{cases}}$





