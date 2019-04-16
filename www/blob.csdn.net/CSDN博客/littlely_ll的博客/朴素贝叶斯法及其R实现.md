# 朴素贝叶斯法及其R实现 - littlely_ll的博客 - CSDN博客





2016年12月19日 12:44:35[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：3506








## 1. 朴素贝叶斯基本方法

### 1.1 贝叶斯分类法基本公式：



$P(Y=c_k|X=x)=\frac{P(Y=c_k)\Pi_jP(X^{(j)}=x^{(j)}|Y=c_k)}{\sum_k(P(Y=c_k)\Pi_jP(X^{j}=x^{j}|Y=c_k))},\\k=1,2,\cdots,K$

朴素贝叶斯分类器可以表示为 


$y=f(x)=arg \quad  max_{c_k}\frac{P(Y=c_k)\Pi_jP(X^{(j)}=x^{(j)}|Y=c_k)}{\sum_k(P(Y=c_k)\Pi_jP(X^{j}=x^{j}|Y=c_k))}$

上式中，分母对所有的$c_k$都相同，所以 


$y=arg\quad max_{c_k}P(Y=c_k)\Pi_jP(X^{(j)}=x^{(j)}|Y=c_k)$
### 1.2 后验概率最大化含义

朴素贝叶斯法将实例分类到后验概率最大的类中，这等价于期望风险最小化，选择0-1损失函数： 


$L(Y,f(X))=\{_{0，Y=f(X)}^{1，Y\neq f(X)}$

f(X)为分类决策函数。 

根据期望风险最小化准则得到后验概率最大化准则： 


$f(x)=arg\quad max_{c_k}P(c_k|X=x)$

此为朴素贝叶斯法所采用的的原理。
## 2. 朴素贝叶斯法的参数估计

### 2.1 极大似然估计

设第j个特征$x^{(j)}$可能取值的集合为{$a_{j1},a_{j2},\cdots, a_{js}$},条件概率$P(x^{(j)}=a_{jl}|Y=c_k)$的极大似然估计为 


$P(X^{(j)}=a_{jl}|Y=c_k)=\frac{\sum_{i=1}^NI(x_i^{(j)}=a_{jl},y_i=c_k)}{\sum_{i=1}^NI(y_i=c_k)}\\j=1,2,\cdots,n; l=1,2,\cdots,S_j; k=1,2,\cdots,K$

式中，$x_i^{(j)}$为第$i$个样本的第j个特征；$a_{jl}$是第j个特征可能取的第$l$个值；$I$为指示函数。
### 2.2 贝叶斯估计

用极大似然估计可能出现要估计的概率为0 的情况，这时会影响后验概率计算，使分类产生偏差，因此，解决这一问题的方法为贝叶斯估计，条件概率的贝叶斯估计为 


$P_{\lambda}(X^{(j)}=a_{jl}|Y=c_k)=\frac{\sum_{i=1}^NI(x_i^{(j)}=a_{jl},y_i=c_k)+\lambda}{\sum_{i=1}^NI(y_i=c_k)+S_j\lambda}$

式中$\lambda\ge0$. 当$\lambda=0$时就是极大似然估计，当$\lambda=1$时就称为拉普拉斯平滑(Laplace smoothing).
## 3. 朴素贝叶斯算法（Naive Bayes Algorithm）

输入：训练数据$T=\{(x_1,y_1),(x_2,y_2),\cdots,(x_N，y_N)\},$其中$x_i=(x_i^{(1)},x_i^{(2)},\cdots,x_i^{(n)})^T,$$x_i^{(j)}$是第$i$个样本的第j个特征，$x_i^{(j)}\in \{a_{j1},a_{j2},\cdots,a_{js}\}$, $a_{jl}$是第j个特征可能取值的第$l$个值，$j=1,2,\cdots,n,l=1,2,\cdots,S_j，y_i\in \{c_1,c_2,\cdots,c_K\};$实例x; 

输出：实例x的分类。 

（1）计算先验概率及条件概率 


$P(Y=c_k)=\frac{\sum_{i=1}^NI(y_i=c_k)}{N}, k=1,2,\cdots,K\\P(X^{(j)}=a_{jl}|Y=c_k)=\frac{\sum_{i=1}^NI(x_i^{(j)}=a_{jl},y_i=c_k)}{\sum_{i=1}^NI(y_i=c_k)}\\j=1,2,\cdots,n;l=1,2,\cdots,S_j; k=1,2,\cdots,K$

（2）对于给定的实例$x=(x^{(1)},x^{(2)},\cdots,x^{(n)})^T,$计算 


$P(Y=c_k)\Pi_{j=1}^nP(X^{(j)}=x^{(j)}|Y=c_k),k=1,2,\cdots,K$

（3）确定实例x的分类 


$y=arg\quad max_{c_k}P(Y=c_k)\Pi_{j=1}^n P(X^{(j)}=x^{(j)}|Y=c_k)$
## 4. 朴素贝叶斯算法优缺点

优点： 

 - 简单、快速、有效 

 - 能处理好噪声数据和缺失的数据 

 - 需要用来训练的例子相对较少，但同样能处理好大量的例子 

 - 很容易获得一个预测的估计概率值 

缺点： 

 - 依赖于一个常用的错误假设，即一样的重要性和独立特征 

 - 应用在含有大量数值特征的数据集时并不理想 

 - 概率的估计值相对于预测的类而言更加不可靠
## 5. R语言实现

例:见李航的《统计学习方法》P50. 例4.1

```
x1 <- c(1,1,1,1,1,2,2,2,2,2,3,3,3,3,3)
x2 <- c("S","M","M","S","S","S","M","M","L","L","L","M","M","L","L")
Y <- c(-1,-1,1,1,-1,-1,-1,1,1,1,1,1,1,1,-1)
dat <- cbind(x1,x2,Y)
dat <- as.data.frame(dat)
dat$x1 <- as.numeric(dat$x1) #from factor to numeric
dat$Y <- as.numeric(dat$Y) #from factor to numeric
dat$Y <- ifelse(dat$Y==1, -1, 1)
p_y1 <- length(dat$Y==1)/length(dat$Y) #compute the #probability of Y==1
p_y <- length(dat$Y== -1)/length(dat$Y) #compute the #probability of Y==-1
p_x11_y1 <- nrow(dat[x1==1 & Y==1,])/length(dat$Y[dat$Y==1]) 
p_x12_y1 <- nrow(dat[x1==2 & Y==1,])/length(dat$Y[dat$Y==1])
p_x13_y1 <- nrow(dat[x1==3 & Y==1,])/length(dat$Y[dat$Y==1])
p_x2s_y1 <- nrow(dat[x2=="S" & Y==1,])/length(dat$Y[dat$Y==1])
p_x2m_y1 <- nrow(dat[x2=="M" & Y==1,])/length(dat$Y[dat$Y==1])
p_x2l_y1 <- nrow(dat[x2=="L" & Y==1,])/length(dat$Y[dat$Y==1])

p_x11_y <- nrow(dat[x1==1 & Y== -1,])/length(dat$Y[dat$Y== -1])
p_x12_y <- nrow(dat[x1==2 & Y== -1,])/length(dat$Y[dat$Y== -1])
p_x13_y <- nrow(dat[x1==3 & Y== -1,])/length(dat$Y[dat$Y== -1])
p_x2s_y <- nrow(dat[x2=="S" & Y== -1,])/length(dat$Y[dat$Y== -1])
p_x2m_y <- nrow(dat[x2=="M" & Y== -1,])/length(dat$Y[dat$Y== -1])
p_x2l_y <- nrow(dat[x2=="L" & Y== -1,])/length(dat$Y[dat$Y== -1])

##given x=(2,"S")' compute the probability
## in the condition of y==1
(p_x12_x2s_y1 <- p_y1 * p_x12_y1 * p_x2s_y1)
##in the condition of y== -1
(p_x12_x2s_y <- p_y * p_x12_y * p_x2s_y)

max(c(p_x12_x2s_y1, p_x12_x2s_y)) #find the maximum #probability
```

<注>加入拉普拉斯常数的贝叶斯估计和上述程序差不多，只是加入了一个常数。 
***R语言中的添加包实现朴素贝叶斯分类***

应用e1071添加包中的函数naiveBayes(): 
*创建分类器：*
`m <- naiveBayes(train, class, laplace = 0)`

 - train: 数据框或者包含训练数据的矩阵 

 - class: 包含训练数据每一行的分类的一个因子向量 

 - laplace: 控制拉普拉斯估计的一个数值（默认为0） 

该函数返回一个朴素贝叶斯模型对象，该对象能够用于预测。 
*进行预测：*
`p <- predict(m, test, type="class")`

 - m: 由函数naiveBayes()训练的一个模型 

 - test: 数据框或者包含测试数据集的矩阵，包含与用来建立分类器的训练数据相同的特征 

 - type: 值为“class”或者“raw”，标识预测的最可能的类别值或者原始的预测概率 

该函数将返回一个向量，根据参数type的值，该向量含有预测的类别值或者原始预测的概率值。









