# R-CNN—双正则化参数的L2-SVM - wsp_1138886114的博客 - CSDN博客





2018年09月06日 16:29:11[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：381










- - - [一、前言](#一前言)
- [二、SVM算法](#二svm算法)- - [2.1 SVM 原型算法](#21-svm-原型算法)
- [2.2 SVM 改进算法 L2—SVM](#22-svm-改进算法-l2svm)


- [三、Doupenalty-Gradient方法](#三doupenalty-gradient方法)- - [3.1 选取目标函数](#31-选取目标函数)
- [3.2 确定 SVM 类型](#32-确定-svm-类型)
- [3.3 参数优化方法](#33-参数优化方法)








### 一、前言
- 
在单正则化SVM的基础上，提出双正则化参数的L2-SVM,获得它的对偶形式，从而确定最优化的目标函数，结合梯度下降形成：**Doupenalty gradient**（一种新的SVM参数选择方法） 

- 
Doupenalty-Gradient方法在同时寻找 $C^+和C^-$ 以及**核参数**这三个参数的最优值时，SVM的性能得到了极大的改善。 


### 二、SVM算法

##### 2.1 SVM 原型算法



$\begin{align}\min:&  ~~~~\frac{1}{2}||w||^2+C\sum_{i=1}^{N}\xi_i \tag{0}\\s.t.&~~~~y_i[(w·x_i)+b]\geqslant 1-\xi_i\tag{1}\end{align}$
- ( 其中 $\xi_i\geqslant 0,i=1,2,3...N$ )
- $C>0$ 用来调节错分样本的错误比重
- $\xi_i\geqslant 0$ 为松弛因子，代表错分样本的错误程度 
- $y_i=\pm 1$ 为样本的类别标签
- $w$ 最优超平面法向量 
- $b$ 最有超平面的阈值 

用对偶理论求解最优化，并引入核函数，求出式（1）的对偶形式： 


$\begin{align}\max~~~&-\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_i\alpha_jy_iy_jk(x_i,x_j)+\sum_{i=1}^{N}\alpha_i\tag{2}\\s.t.~~~&\sum_{j=1}^{N}\alpha_iy_i=0 , ~~~0\leqslant \alpha_i \leqslant C \tag{3}\\判别函数: ~~~&f(x) =sign\left ( \sum_{i=1}^{N}\alpha_iy_ik(x_i,x)+b\right ) \tag{4}\end{align}$

$\alpha_i$ 为Lagrange乘子，$k(x_i,x_j)$是核函数。式（3）的判别函数为 (4) 

##### 2.2 SVM 改进算法 L2—SVM

我们把式子 (1) 称为 $L_1$-SVM，它的改进算法：二范数软间隔SVM 称为 $L_2$-SVM。详情如下 ($\xi_i\geqslant0,i=1,2,3,...,N$)： 


$\begin{align}\min:&  ~~~~\frac{1}{2}||w||^2+C\sum_{i=1}^{N}\xi_i^2 \tag{5}\\s.t.&~~~~y_i[(w·x_i)+b]\geqslant 1-\xi_i\tag{6}\end{align}$

上式的对偶为  


$\begin{align}\max~~~& -\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_i\alpha_jy_iy_j\left ( k(x_i,x_j)+\frac{1}{C}\delta_{ij} \right )+\sum_{i=1}^{N}\alpha_i\tag{7}\\s.t.~~~&\sum_{j=1}^{N}\alpha_iy_i=0 , ~~~0\leqslant \alpha_i \tag{8}\end{align}$
式中 



$\delta_{ij} = \left\{\begin{matrix}1 &i=j\\ 0 &i\neq j\end{matrix}\right. \tag{9}$

约束条件式 (8) 中`去掉了上界`$\alpha_i \leqslant C $ 这就是L2-SVM的重要特性。 

它可以将软间隔转换成硬间隔，即将线性不可分转换成线性可分，而目标函数中仅仅是在核函数的对角线上加上一个常数因子1/C，可以当作是核函数的一个微小改动，即： 


$k(x_i,x_j) = k(x_i,x_j)+\frac{1}{C}\delta_{ij} \tag{10}$
### 三、Doupenalty-Gradient方法

Doupenalty-Gradient方法：将从最小化VC维出发，确定包含参数的目标函数，`通过梯度法进行优化，来确定SVM的最优参数。`已知对于 $n$ 维空间 $\Delta$ 间隔的超平面，它的 VC 维上界 $h$ 满足下面的不等式。 (R为映射到高维空间的球半径。): 


$h \leqslant \min(R^2/\Delta^2,n)+1 \tag{11}$

##### 3.1 选取目标函数

针对式（11），如果确定了核参数，也就确定了向高维空间的核映射，则球半径R为常数，所以在最优化的过程中，将其省略，仅求了 $\min(1/\Delta^2)$,即 


$\min \frac{1}{2}||w||^2 \tag{12}$

而Doupenalty-Gradient方法中，核参数也是最优参数选择的目标之一，目标函数为 :

$\min(R^2 ||w||^2) \tag{13}$
##### 3.2 确定 SVM 类型

Osuna等人采用两个正则化参数 ($C^+和C^-$) 的$L_1$-SVM，来改善SVM的性能。其二次规划的形式如下($\xi_i\geqslant0,i=1,2,3,...,N$): 


$\begin{align}\min:&  ~~~~\frac{1}{2}||w||^2+C^+(\sum_{i:y_i=+1}\xi_i)+C^-(\sum_{i:y_i=-1}\xi_i) \tag{14}\\s.t.&~~~~y_i(w·x_i+b)\geqslant 1-\xi_i\tag{15}\end{align}$

由（6）L2-SVM的对偶可以转换成线性可分，**Doupenalty-Gradient** 方法选用具有两个正则化参数 ($C^+和C^-$) 的L2-SVM，它的形式如下： 


$\begin{align}\min:&  ~~~~\frac{1}{2}||w||^2+\frac{C^+}{2}(\sum_{y_i=1}\xi_i^2)+\frac{C^-}{2}(\sum_{y_i=-1}\xi_i^2) \tag{16}\\s.t.&~~~~y_i[(w·x_i)+b]\geqslant 1-\xi_i \tag{17}\end{align}$

上式（17）的Lagrangian（拉格朗日）函数为：


$L(w,b,\alpha,\xi_{i:y_i=1},\xi_{i:y_i=-1}) = \frac{1}{2}||w||^2+\frac{C^+}{2}\sum_{y_i=1}\xi_i^2+\frac{C^-}{2}\sum_{y_i=-1}\xi_i^2-\sum \alpha_i[y_i(w·x_i+b)-1+\xi_i] \tag{18}$

将上式（18）分别对 $w,b,\alpha,\xi_{i:y_i=1},\xi_{i:y_i=-1}$  求导，并令其等于零可得如下： 


$\begin{align}∇_wL(w,b,\alpha,\xi_{i:y_i=1},\xi_{i:y_i=-1}) ~~\Rightarrow&~~w =\sum\alpha_iy_ix_i   \tag{19}\\∇_bL(w,b,\alpha,\xi_{i:y_i=1},\xi_{i:y_i=-1}) ~~\Rightarrow&~~\sum\alpha_iy_i=0  \tag{20} \\∇_{\xi_{i:y_i=1}}L(w,b,\alpha,\xi_{i:y_i=1},\xi_{i:y_i=-1}) ~~\Rightarrow&~~C^+\xi_{i:y_i=1}=\alpha_{i:y_i=1}  \tag{21} \\∇_{\xi_{i:y_i=-1}}L(w,b,\alpha,\xi_{i:y_i=1},\xi_{i:y_i=-1}) ~~\Rightarrow&~~C^-\xi_{i:y_i=-1}=\alpha_{i:y_i=-1} \tag{22}\end{align} $

将 (19),(20),(21),(22) 代入 (18) 式，并用核函数替换内积运算得到（17）式的对偶形式： 


$\begin{align}\max~~~& -\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_i\alpha_jy_iy_j\left ( k(x_i,x_j)+\frac{1}{C^+}\delta^+_{ij}+\frac{1}{C^-}\delta^-_{ij} \right )+\sum_{i=1}^{N}\alpha_i\tag{23}\\s.t.~~~&\sum_{j=1}^{N}\alpha_iy_i=0 , ~~~0\leqslant \alpha_i \tag{24}\end{align}$

上式（24）中： 


$\begin{align}\delta_{ij}^+ =& \left\{\begin{matrix}1 &\text{if}(i=j ~~\text{AND}~~ y_i=1)\\ 0 &\text{otherwise}\end{matrix}\right. \tag{25}\\\delta_{ij}^- =& \left\{\begin{matrix}1 &\text{if}(i=j ~~\text{AND}~~ y_i=-1)\\ 0 &\text{otherwise}\end{matrix}\right. \tag{26}\end{align}$

此时式（24）是硬间隔线性可分的SVM，核函数如下（三个待优化参数：核参数、正则化参数$C^+、C^-$）：  


$\hat{k}(x_i,x_j) = k(x_i,x_j)+\frac{1}{C^+}\delta_{i,j}^++\frac{1}{C^-}\delta_{i,j}^-\tag{27}$
##### 3.3 参数优化方法

Doupenalty-Grandient 方法采用RBF核函数（28），需要优化的函数为（29）。 


$\begin{align}k(x_i,x_j) = &\exp(-\gamma||x_i-x_j||^2) \tag{28}\\f(\theta) =& \min R^2||w||^2 \tag{29}\end{align}$
- 其中$\theta=(\gamma,C^+,C^-)$
- **w**为最优超平面法向量，$||w||^2$ 由 （19）式可得。
- **R**是特征空间的最小球半径，$R^2$是下式最优化目标函数值。 


$\begin{align}\max~~~& \sum_{i}\beta_i k(x_i,x_i)-  \sum_{i,j} \beta_i\beta_j k(x_i,x_j)\tag{30}\\s.t.~~~&\sum_{i}\beta_i=0 , \beta_i>0,i=1,2,...N \tag{31}\end{align}$

使用梯度求解（29）过程如下： 


$\nabla f(\theta) = (\partial f/\partial \gamma,\partial f/\partial C^+,\partial f/\partial C^+ ) \tag{32}$

上式中  


$\begin{align}\frac{\partial f}{\partial \gamma} =&\frac{\partial R^2}{\partial \gamma}||w||^2+\frac{\partial ||w||^2}{\partial \gamma} R^2 \tag{33}\\\frac{\partial f}{\partial C^+} =&\frac{\partial R^2}{\partial C^+}||w||^2+\frac{\partial ||w||^2}{\partial C^+} R^2 \tag{34}\\\frac{\partial f}{\partial C^-} =&\frac{\partial R^2}{\partial C^-}||w||^2+\frac{\partial ||w||^2}{\partial C^-} R^2 \tag{35}\end{align}$

由上面三式得： 


$\begin{align}&\frac{ \partial ||w||^2}{\partial \gamma}  =  \sum_{i,j} \alpha_i \alpha_jy_iy_j \frac{\partial k(x_i,x_j)}{\partial \gamma} \tag{36}\\&\frac{ \partial ||w||^2}{\partial C^+}  = - \frac{1}{(C^+)^2}\sum_{y_j=1} \alpha_i^2 \tag{37}\\&\frac{ \partial ||w||^2}{\partial C^-} = - \frac{1}{(C^-)^2}\sum_{y_j=1} \alpha_i^2 \tag{38}\\&\frac{ \partial R^2}{ \partial \gamma}  = - \sum_{i,j}\beta_i \beta_j \frac{ \partial \hat{k}(x_i,x_j)}{\partial \gamma} \tag{39}\\&\frac{ \partial R^2}{ \partial C^+}  = - \frac{1}{(C^+)^2} \sum_{y_j=1} \beta_i(1- \beta_i) \tag{40}\\&\frac{ \partial R^2}{ \partial C^-} = - \frac{1}{(C^-)^2} \sum_{y_j=-1}\beta_i(1- \beta_i) \tag{41}\\\end{align}$

因为采用的是RBF核，所以： 


$ \frac{ \partial \hat{k}(x_i,x_j)}{\partial \gamma} = -||x_i-x_j||^2k(x_i,x_j) \tag{42}$
此时，给定一个初始值 $\theta_0 (\gamma_0,C_{0}^{+},C_{0}^{-})$即可求出 $R^2,||w||^2,\alpha,\beta,$从而求出 $\nabla f$,梯度法求解式（29）。 

步骤如下：
- **步骤1.** 给定初始值 $\theta^{(0)}$ 及精度 $\epsilon>0$，如果 $||\nabla f(\theta^{(0)})||^2 \leqslant \epsilon$ 那么 $\theta^{(0)}$ 为近似极小值；否则，n=0，转步骤2.
- 
**步骤2.** 如果 $||\nabla f(\theta^{(0)})||^2>\epsilon$，那么用下式求出$\theta^{(n+1)}$ 点： 

（其中$ \lambda$ 为步长） 


$\theta^{(n+1)} = \theta^{(n)}-\lambda \nabla f(\theta^{(n)}) \tag{43}$- 
**步骤3.** 如果 $||\nabla f(\theta^{(0)})||^2 \leqslant \epsilon$，那么 $ \theta^{(n+1)} $ 为近似极小值；否则，n=n+1,转步骤2。












