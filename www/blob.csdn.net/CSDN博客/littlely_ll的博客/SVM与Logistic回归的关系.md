# SVM与Logistic回归的关系 - littlely_ll的博客 - CSDN博客





2017年09月25日 12:47:33[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：202








# 合页损失函数



$[z]_{+} = \{^{z, z >0}_{0, z\le 0}$

# SVM的目标函数

对于线性SVM，我们知道它的原始最优化问题为： 


$min_{w,b,\xi} 1/2||w||^2 +C \Sigma_{i=1}^N\xi_i\\s.t. y_i(w\cdot x_i+b)\ge 1-\xi_i, i=1,2,...,N\\\xi_i\ge 0, i=1,2,...,N$

对于原始优化问题，当$y_i(w\cdot x_i+b)\ge 1$时，数据点落在了间隔边界正确的一侧，这时$\xi_i$为0，而当$y_i(w\cdot x_i+b)\lt 1$的时候，这时$\xi_i=1- y_i(w\cdot x_i+b)$. 

因此，SVM的优化问题可以转化为最优化以下问题： 


$min_{w,b}\Sigma_{i=1}^N[1-y_i(w\cdot x_i+b)]_+ + \lambda ||w||^2$

其中，$\lambda = (2C)^{-1}$
# Logistic的目标函数

对于logistic回归，我们知道sigmoid函数的形式为 


$f(a) = \frac{1}{1+exp\{-(w\cdot x + b)\}}=\frac{1}{1+exp(-a)}$

其中$a = w\cdot x + b$

sigmoid函数的性质：
> - 对称性：$f(-a)=1-f(a)$
- $\frac{\partial f}{\partial a} = f(1-f)$


应用极大似然函数估计模型参数，首先构造似然函数，我们知道logistic回归模型为$p(y=1|x)=\pi(x), p(y=0|x)=1-\pi(x)$，则似然函数为： 


$Z=\Pi_{i=1}^N[\pi(x_i)]^{y_i}[1-\pi(x_i)]^{1-y_i}$

取似然函数的负对数得到误差函数，这个误差函数就是交叉熵（cross-entropy）误差函数： 


$L1 = -lnZ = -\Sigma_{i=1}^N[y_ilog\pi(x_i)+(1-y_i)log(1-\pi(x_i))]$

去掉前面负号不影响优化问题，则： 


$L2 =\Sigma_{i=1}^N[y_ilog\pi(x_i)+(1-y_i)log(1-\pi(x_i))]$

在处理logistic回归时，为了比较方便，我们对目标变量$y \in \{0,1\}$进行操作，使用目标函数$y \in \{-1,1\}$重写最大似然logistic函数。我们知道，$p(y=1|a) = f(a)，p(y=-1|a)=1-f(a)=f(-a)$，根据sigmoid函数的对称性质，我们有： 


$p(y|a) = f(ay)=\frac{1}{1+exp(-ay)}$

从上式子中通过对似然函数取负对数构造一个带正则化项的误差函数： 


$L = \Sigma_{i=1}^Nln(1+exp(-a_iy_i))+\lambda ||w||^2$

把最终式换回去： 


$min_{w,b}L = \Sigma_{i=1}^Nln(1+exp(-(w\cdot x_i +b)y_i)+\lambda ||w||^2$

和SVM作对比： 


$min_{w,b}\Sigma_{i=1}^N[1-y_i(w\cdot x_i+b)]_+ + \lambda ||w||^2$

因此，SVM和Logistic回归有相似的目标函数
**附加：**
[**logistic回归与最大熵模型的关系**](https://www.zhihu.com/question/24094554)






