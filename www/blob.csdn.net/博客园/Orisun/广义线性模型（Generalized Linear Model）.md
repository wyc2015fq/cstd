# 广义线性模型（Generalized Linear Model） - Orisun - 博客园







# [广义线性模型（Generalized Linear Model）](https://www.cnblogs.com/zhangchaoyang/articles/7120908.html)





## 指数分布族 

\begin{equation} p(y;\eta)=b(y)\cdot exp\{\eta^TT(y)-a(\eta)\} \label{exponential family} \end{equation}

$b(y)$和$T(y)$是关于y的函数，$a(\eta)$是关于$\eta$的函数，当$b,T,a$都确定时，(\ref{exponential family})式就指定了以$\eta$为参数的函数族，并且这些函数都是指数形式的。

伯努利分布（Bernoulli）、高斯分布（Gaussian）、多项式分布（Multinomial）、泊松分布（Poisson）、伽马分布（Gamma）、指数分布（Exponential）、β分布、Dirichlet分布、Wishart分布，都属于指数分布族。

## 广义线性模型（GLM）

广义线性模型有3个假设：
- $p(y|x;\theta)$是一个以$\eta$为参数的指数分布，即概率函数如(\ref{exponential family})式所示。
- 给定x，我们的目标是预测T(y)的期望，即$E[T(y)|x]$。通常情况下
\begin{equation} T(y)=y  \label{ty} \end{equation}
此时我们的目标上预测$E(y|x)$
- \begin{equation} \eta=\theta^Tx \label{et} \end{equation}

当y属于不同的分布（比如伯努利分布、多项分布、高斯分布）时，根据广义线性模型可以推出$E(y|x;\theta)$的函数表达式。
|y的分布|$E(y|x;\theta)$|
|----|----|
|伯努利分布|$sigmoid(x;\theta)$|
|多项分布|$softmax(x;\theta)$|
|高斯分布|$\theta^Tx$|

下面我们来逐一证明。

## LR属于GLM

 y服从伯努利分布时，$y\in \{0,1\}$，y=1的概率设为$\phi$，则

\begin{equation} E(y)=\phi  \label{elr} \end{equation}

$$p(y;\phi)=\phi^y(1-\phi)^{1-y}=exp\{ylog\phi+(1-y)log(1-\phi)\}$$

\begin{equation} =exp\left\{log\frac{\phi}{1-\phi}y+log(1-\phi)\right\} \label{lr} \end{equation}

将(\ref{lr})式与(\ref{exponential family})式对比，得

$b(y)=1,\eta=log\frac{\phi}{1-\phi},T(y)=y,a(\eta)=-log(1-\phi)$

所以

\begin{equation} \phi=\frac{1}{1+e^{-\eta}} \label{etalr} \end{equation}

联合(\ref{etalr})(\ref{elr})(\ref{et})得

$$E(y|x;\theta)=\frac{1}{1+e^{-\theta^Tx}}$$

## softmax属于GLM

定义示性函数 

$$1(condition)=\left\{\begin{matrix}1&if \; condition \; is \; true \\ 0&if \; condition \; is \; false\end{matrix}\right.$$ 

y服从多项式分布时，设一共有k项，y属于每一项的概率分别为$\phi_1,\phi_2,\cdots,\phi_k$，则

\begin{equation}E[1(y=i)]=\phi_i  \label{emn} \end{equation}

且有

$$1(y=k)=1-\sum_i^{k-1}{1(y=i)}$$

$$p(y;\phi)=\phi_1^{1(y=1)}\phi_2^{1(y=2)}\cdots\phi_k^{1-\sum_i^{k-1}{1(y=i)}}$$

$$=exp\left\{1(y=1)log\phi_1+1(y=2)log\phi_2+\cdots+\left(1-\sum_i^{k-1}{1(y=i)}\right)log\phi_k\right\}$$

\begin{equation} =exp\left\{1(y=1)log{\frac{\phi_1}{\phi_k}}+1(y=2)log{\frac{\phi_2}{\phi_k}}+\cdots+1(y=k-1)log{\frac{\phi_{k-1}}{\phi_k}}+log\phi_k\right\} \label{mn} \end{equation}

将(\ref{mn})式与(\ref{exponential family})式对比，得

$$b(y)=1,a(\eta)=-log\phi_k$$

\begin{equation}T(y)=\left[\begin{array} \ 1(y=1)\\1(y=2)\\ \vdots\\1(y=k-1)\end{array}\right] \label{1} \end{equation}

\begin{equation}\eta=\left[\begin{array} \ log{\frac{\phi_1}{\phi_k}} \\ log{\frac{\phi_2}{\phi_k}} \\ \vdots \\ log{\frac{\phi_{k-1}}{\phi_k}} \\\end{array} \right] \label{2} \end{equation}

由(\ref{emn})(\ref{1})得

\begin{equation} E[T(y)]=\left[\begin{array} \ E[1(y=1)]\\E[1(y=2)]\\ \vdots\\E[1(y=k-1)]\end{array}\right]=\left[\begin{array} \ \phi_1 \\ \phi_2 \\ \vdots \\ \phi_{k-1}\end{array}\right] \label{3} \end{equation}

由(\ref{2})得

$$\eta_i=log\frac{\phi_i}{\phi_k} \;\;\;\; 1 \le i <k$$

\begin{equation} \therefore \phi_i=\phi_ke^{\eta_i}  \;\;\;\; 1 \le i <k   \label{4} \end{equation}

(\ref{4})式两边对i求和

$$\sum_i^{k-1}{\phi_i}=\sum_i^{k-1}{\phi_ke^{\eta_i}}=\phi_k\sum_i^{k-1}{e^{\eta_i}}=1-\phi_k$$

\begin{equation}\therefore \phi_k=\frac{1}{1+\sum_i^{k-1}{e^{\eta_i}}} \label{5} \end{equation}

代入(\ref{4})式得

\begin{equation}\phi_i=\frac{e^{\eta_i}}{1+\sum_i^{k-1}{e^{\eta_i}}}  \;\;\;\; 1 \le i <k \label{6} \end{equation}

联合(\ref{et})(\ref{emn})(\ref{5})(\ref{6})得

$$E[1(y=i)|x;\theta]=\left\{\begin{matrix}\frac{e^{\eta_i}}{1+\sum_i^{k-1}{e^{\eta_i}}}=\frac{e^{\theta_i^Tx}}{1+\sum_i^{k-1}{e^{\theta_i^Tx}}}&if\;1 \le i <k \\  \frac{1}{1+\sum_i^{k-1}{e^{\eta_i}}}=\frac{1}{1+\sum_i^{k-1}{e^{\theta_i^Tx}}}&if\;i=k \end{matrix}\right.$$

因为y服从多项分布，即每次试验y只能取k种情况中的一种，所以对于多分类问题如果k个类别之间是互斥的才适合用softmax，相反如果一个样本可以同时属于多个类别则不能用softmax，而应该为每一个类别建立一个sigmoid函数。

## 线性模型属于GLM

y服从高斯分布$y~N(\mu,\sigma^2)$，则

\begin{equation}E(y)=\mu\label{eg}\end{equation}

$$p(y;\eta)=\frac{1}{\sqrt{2\pi}\sigma}exp\left\{-\frac{(y-\mu)^2}{2\sigma^2}\right\}=\frac{1}{\sqrt{2\pi}\sigma}exp\left\{\frac{-y^2-\mu^2+2y\mu}{2\sigma^2}\right\}$$

\begin{equation}=\frac{1}{\sqrt{2\pi}\sigma}exp\left(-\frac{y^2}{2\sigma^2}\right)exp\left\{\frac{\mu}{\sigma^2}y-\frac{\mu^2}{2\sigma^2}\right\}\label{gauss}\end{equation}

将(\ref{gauss})式与(\ref{exponential family})式对比，得

$$b(y)=\frac{1}{\sqrt{2\pi}\sigma}exp\left(-\frac{y^2}{2\sigma^2}\right),\eta=\frac{\mu}{\sigma^2},T(y)=y,a(\eta)=\frac{\mu^2}{2\sigma^2}$$

再结合(\ref{et})(\ref{eg})得

$$E(y|x;\theta)=\mu=\sigma^2\eta=\sigma^2\theta^Tx$$

即y是关于x的线性模型。












