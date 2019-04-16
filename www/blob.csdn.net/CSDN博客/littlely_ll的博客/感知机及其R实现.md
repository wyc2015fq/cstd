# 感知机及其R实现 - littlely_ll的博客 - CSDN博客





2016年12月15日 19:14:15[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：2158








## 感知机

### 1. 感知机模型

定义：假设输入空间（特征空间）是$\chi\subseteq R^n$，输出空间是$Y=\{+1,-1\}$。输入$x\in \chi$表示实例的特征向量，对应于输入空间的点；输出$y\in Y$表示实例的类别。由输入空间到输出空间的如下函数 


$f(x)=sign(w\cdot x+b)$

称为感知机。其中，$w$和$b$为感知机模型参数，$w\in R^n$叫做权值，$b\in R$叫做偏置，$w\cdot x$表示$w$和$x$的内积。$sign$为符号函数。
### 2. 感知机学习策略

#### 2.1数据集的线性可分性定义：

给定一个数据集 


$T = \{(x_1,y_1),(x_2,y_2),\cdots,(x_n,y_n)\},$

其中，$x_i\in\chi=R^n,y_i\in Y=\{+1,-1\},i=1,2,\cdots,N,$如果存在某个超平面S 


$w\cdot x+b=0$

能够将数据集的正实例点和负实例点完全正确地划分到超平面的两侧，即对所有的$y_i=+1$的实例$i$，有$w\cdot x_i+b>0$,对所有的$y_i=-1$的实例$i$，有$w\cdot x_i+b<0,$则成数据集$T$为线性可分数据集；否则，成数据集$T$为线性不可分。
#### 2.2感知机学习策略的损失函数

感知机$sign(w\cdot x+b)$学习的损失函数定义为 


$L(w,b)=-\sum_{x_i\in M}y_i(w\cdot x_i+b)$

其中，$M$为误分类点的集合。这个损失函数就是感知机学习的经验风险函数。损失函数$L(w,b)$是$w$,$b$的连续可导函数。
### 3. 感知机学习算法

#### 3.1 算法（感知机学习算法的原始形式）

输入：训练数据集$T=\{(x_1,y_1),(x_2,y_2),\cdots,(x_N,y_N)\}$,其中$x_i \in\chi=R^n,y_i\in Y=\{+1,-1\},i=1,2,\cdots,N$;学习率$\eta(0<\eta\le1);$

输出：$w,b$;感知机模型$f(x)=sign(w\cdot x+b)$. 

（1）选取初值$w_0,b_0$

（2）在训练集中选取数据$(x_i,y_i)$

（3）如果$y_i(w\cdot x_i +b)\le 0$


$w\leftarrow w +\eta y_ix_i\\b\leftarrow b +\eta y_i$

（4）转至（2），直至训练集中没有误分类点。
#### 3.2 R语言实现（感知机学习算法的原始形式）

```
percept <- function(data = data,eta = eta ){
  x <- data[,-dim(data)[2]]
  y <- data[,dim(data)[2]]
  w <- c(0,0)
  b <- 0
  len <- length(y)
  i <- 1  
  while(i <= len){
    if(y[i] * (x[i,] %*% w + b) <= 0){
    ## update w and b
      w <- w + eta * y[i] * x[i,]
      b <- b + eta * y[i]
      i <- 1 ##important, for traversing every point
    }
    else{
      i <- i + 1 
    }
  }
  return(list(w=w,b=b))
}

data <- matrix(c(3,3,1,4,3,1,1,1,-1),nr=3,byrow=T)
percept(data = data,eta = 1)
```

最终结果为 

$w 

[1] 1 1
$b 

[1] -3 

再换一个数据：
```
dat <-matrix(c(1,1,-1,.5,.5,-1,4,1,1,3,2,1,1.5,1,-1,2,3,1,4,3,1,2,3.5,1),nc=3,byrow=T)
perceptron <- percept(data=dat,eta=1)
perceptron
```

最后结果为： 

$w 

[1] 4 0
$b 

[1] -7 

我们试着画出它的图形
```
dat1 <- dat[,1:2]
dat1 <- as.data.frame(dat1)
names(dat1) <- c("x1","x2")
plot(x2~x1,data=dat1,col=ifelse(4*x1-7<=0,"red","blue"),pch=17,bty="l")
abline(v=7/4,lwd=2,lty=2,col="red")
```

![Perceptron](https://img-blog.csdn.net/20161216160529591?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由此可以看出还是不错的，都已经正确划分。
#### 3.3算法（感知机学习算法的对偶形式）

输入：线性可分的数据集$T=\{(x_1,y_1),(x_2,y_2),\cdots,(x_N,y_N)\},$其中$x_i\in R^n,y_i \in \{+1,-1\},i=1,2,\cdots,N;$学习率$\eta(0<\eta\le1);$

输出：$\alpha,b;$感知机模型$f(x)=sign(\sum_{j=1}^N\alpha_jy_jx_j\cdot x+b).$

其中$\alpha=(\alpha_1,\alpha_2,\cdots,\alpha_N)^T.$

（1）$\alpha\leftarrow 0,b\leftarrow0$

（2）在训练集中选取数据$(x_i,y_i)$

（3）如果$y_i(\sum_{j=1}^N\alpha_jy_jx_j\cdot x_i +b)\le0$


$\alpha_i\leftarrow \alpha_i +\eta\\b\leftarrow b+\eta y_i$

（4）转至（2）直到没有误分类数据。 

对偶形式训练实例仅以内积的形式实现，为了方便，可以预先将训练集中的实例间的内积计算出来并以矩阵形式存储，这个矩阵就是所谓的Gram矩阵 


$G=[x_i\cdot x_j]_{N\times N}$
#### 3.4 R语言实现（感知机学习算法的对偶形式）

后续补充













