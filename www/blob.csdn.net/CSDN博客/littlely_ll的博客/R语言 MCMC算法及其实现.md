# R语言 MCMC算法及其实现 - littlely_ll的博客 - CSDN博客





2016年12月12日 19:09:47[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：3739标签：[r语言																[MCMC算法](https://so.csdn.net/so/search/s.do?q=MCMC算法&t=blog)](https://so.csdn.net/so/search/s.do?q=r语言&t=blog)
个人分类：[机器学习](https://blog.csdn.net/littlely_ll/article/category/6601963)








### Metropolis-Hasting算法：

Step 1: 选择一个不可约Markov链转移概率q(i,j),i,j$\in$S.再从S={1，2，… ,m}中选择某个整数. 

Step 2: 令n=0以及$x_0=k$. 

Step 3: 产生一个随机变量X使得$P\{X=j\}=q(X_n,j)$, 再产生一个随机数U. 

Step 4: 如果$U < \frac{\pi(X)q(X,X_n)}{\pi(X_n)q(X_N,X)}$, 则MH = X； 否则MH = $X_n$. 

Step 5：$n\leftarrow n+1, X_n=MH$. 

Step 6: 返回Step 3.
#### 建议分布的选择方法：

（1）Metropolis选择；（2）独立抽样；（3）单元素MH算法

#### 举例说明：

设观测变量$Y=(Y_1,Y_2,...,Y_n)$具有如下分布： 


$\pi(y_i|k,\theta,\lambda)=\frac{\theta^{y_i}e^{-\theta}}{y_{i}!},i=1,2,...,k,$


$\pi(y_i|k,\theta,\lambda)=\frac{\lambda^{y_i}e^{-\lambda}}{y_i!}, i =k+1,k+2,...n.$

假定各参数的先验分布为： 


$\pi(\theta|b_1)=Gamma(0.5,b_1),\\  \pi(\lambda|b_2)=Gamma(0.5,b_2),\\  \pi(b_1)=IG(0,1),\\  \pi(b_2)=IG(0,1),\\  \pi(k)=Uniform(1,2,...,n), $

  其中，$k,\theta,\lambda$条件独立，$b_1,b_2$独立，以及\ 


$Gamma(\alpha,\beta)=\frac{1}{\Gamma(\alpha)\beta^{\alpha}}x^{\alpha-1}e^{-\frac{x}{\beta}},\\  IG(\alpha,\beta)=\frac{e^{-\frac{1}{\beta x}}}{\Gamma(\alpha)\beta^{\alpha}x^{\alpha+1}},\\  Uniform(1,2,...,n) : P\{X=j\}=1/n,j=1,2,...,n$

  其中$\alpha$为形状参数，$\beta$为刻度参数。 
**解答：**

首先求解后验分布： 


$\pi(k,\theta,\lambda,b_1,b_2|Y)=\Pi_{i=1}^k\pi(Y_i|k,\theta,\lambda)\Pi_{i=k+1}^{n}\pi(Y_i|k,\theta,\lambda)\\\times\pi(\theta|b_1)\pi(\lambda|b_2)\pi(b_1)\pi(b_2)\pi(k)\\=\Pi_{i=1}^k\frac{\theta^{Y_i}e^{-\theta}}{Y_i!}\Pi_{i=k+1}^n\frac{\lambda^{Y_i}e^{-\lambda}}{Y_i!}\times\frac{1}{\Gamma(0.5)b_1}\theta^{-0.5}e^{-\frac{\theta}{b_1}}\\\times\frac{1}{\Gamma(0.5)b_2^{0.5}}\lambda^{-0.5}e^{-\frac{\lambda}{b_2}}\times\frac{e^{-1/b_1}e^{-1/b_2}}{b_1b_2}\frac{1}{n}.$

各参数的满条件分布为 


$\pi(\theta|k,\lambda,b_1,b_2,Y)\propto Gamma(\sum_{i=1}^kY_i+0.5, \frac{b_1}{kb_1+1}),\\\pi(\lambda|k,\theta,b_1,b_2,Y)\propto Gamma(\sum_{i=k+1}^nY_i+0.5,\frac{b_2}{(n-k)b_2+1}),\\\pi(k|\theta,\lambda,b_1,b_2,Y)\propto\theta^{\sum_{i=1}^kY_i}\lambda^{\sum_{i=k+1}^nY_i}e^{-k\theta-(n-k)\lambda},\\\pi(b_1|k,\theta,\lambda,b_2,Y)\propto IG(0.5,\frac{1}{1+\theta}),\\\pi(b_1|k,\theta,\lambda,b_1,Y)\propto IG(0.5,\frac{1}{1+\lambda}).$
**<注:>由于编辑公式麻烦，只给出推导最后结果，中间步骤省略.**
### Y的观测数据
3 5 9 3 4 5 5 5 5 13 18 27 8 4 10 8 3 12 10 10 3 9 8
5 9 4 6 1 5 14 7 9 10 8 13 8 11 11 10 11 13 10 3 8 5
由于k满条件分布不是标准分布，因此，用MH算法抽取$k^{i+1}$,而其他分部则使用Gibbs抽样，其k的建议分布设为$q(k,k')=\frac{1}{m-1}$.

### 使用R语言实现抽样

```
mhsampler<-function(NUMIT=10000,dat=Y){
  n<-length(dat)
  mchain<-matrix(NA,nr=5,nc=NUMIT)
  kinit<-floor(n/2)
  mchain[,1]<-c(1,1,kinit,1,1)
  for(i in 2:NUMIT)
  {
    currtheta<-mchain[1,i-1]
    currlambda<-mchain[2,i-1]
    currk<-mchain[3,i-1]
    currb1<-mchain[4,i-1]
    currb2<-mchain[5,i-1]
    ##sample from full conditional distribution of theta(Gibbs update)
    currtheta<-rgamma(1,shape=sum(Y[1:currk])+.5,scale=currb1/(currk*currb1+1))
    ##sample from full conditional distribution of lambda(Gibbs update)
    currlambda<-rgamma(1,shape=sum(Y[(currk+1):n])+.5,scale=currb2/((n-currk)*currb2+1))
    ##sample from full conditional distribution of k(MH update)
    propk<-sample(x=seq(2,n-1),size=1) #draw one sample at random from uniform(2,..(n-1))
    ##Matropolis accept-reject step(in log scale)
    logMHratio=sum(Y[1:propk])*log(currtheta)+sum(Y[(propk+1):n])*log(currlambda)-propk*currtheta-
      (n-propk)*currlambda-(sum(Y[1:currk])*log(currtheta)+sum(Y[(currk+1):n])*log(currlambda)
                            -currk*currtheta-(n-currk)*currlambda)
    logalpha<-min(0,logMHratio) #alpha=min(1,MHratio)
    if (log(runif(1))<logalpha)
    {
      currk<-propk
    }
    currk=currk #if we do not sample k (k fixed)
    ## sample from full conditional distribution of b1 (Gibbs update: draw from inverse Gamma)
    currb1<-1/rgamma(1,shape=.5,scale=1/(currtheta+1))
    ## sample from full conditional distribution of b2 (Gibbs update: draw from inverse Gamma)
    currb2<-1/rgamma(1,shape=.5,scale=1/(currlambda+1))
    ## update chain with new values
    mchain[,i]=c(currtheta,currlambda,currk,currb1,currb2)
  }
  return(mchain)
}
```

最后可以运行`apply(mhsampler(),1,mean)`获得$\theta,\lambda,k,b_1,b_2$的后验均值估计。






