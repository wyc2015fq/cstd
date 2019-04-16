# 独立抽样（MCMC方法） - littlely_ll的博客 - CSDN博客





2017年02月04日 21:26:12[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：426









## 独立抽样（一）

# simulations

wais<-read.csv("wais.csv",head=T)

y<-wais[,2]

x<-wais[,1]

m<-55000

mu.beta<-c(0,0)

sigma.beta<-c(100,100)

prop.s<-c(.1,.1)

beta<-matrix(nrow=m,ncol=2)

acc.prob<-0

current.beta<-c(0,0)

for(t in 1:m){

  prop.beta<-rnorm(2,current.beta,prop.s)

 cur.eta<-current.beta[1]+current.beta[2]*x

  prop.eta<-prop.beta[1]+prop.beta[2]*x

 loga<-(sum(y*prop.eta-log(1+exp(prop.eta))))-sum(y*cur.eta-log(1+exp(cur.eta)))+sum(dnorm(prop.beta,mu.beta,prop.s,log=T))-sum(dnorm(current.beta,mu.beta,prop.s,log=T))

  u<-runif(1)

  u<-log(u)

  if(u<=loga){

   current.beta<-prop.beta

   acc.prob<-acc.prob+1

  }

  beta[t,]<-current.beta

}

    #convergence diagnostic plot

erg.mean<-function(x){

  n<-length(x)

  result<-cumsum(x)/cumsum(rep(1,n))

}

burnin<-15000

idx<-seq(1,m,50)

idx2<-seq(burnin+1,m)

par(mfrow=c(2,2))

plot(idx,beta[idx,1],type="l",xlab="Iterations",ylab="Values ofbeta0")

plot(idx,beta[idx,2],type="l",xlab="Iterations",ylab="Values ofbeta1")

ergbeta0<-erg.mean(beta[,1])

ergbeta02<-erg.mean(beta[idx2,1])

ylims0<-range(c(ergbeta0,ergbeta02))

ergbeta1<-erg.mean(beta[,2])

ergbeta12<-erg.mean(beta[idx2,2])

ylims1<-range(c(ergbeta1,ergbeta12))

plot(idx,ergbeta0[idx],type="l",ylab='Values ofbeta0',xlab='Iterations',main='(c)Ergodic Mean Plot ofbeta0',ylim=ylims0)

lines(idx2,ergbeta02[idx2-burnin],col=2,lty=2)

plot(idx,ergbeta1[idx],type="l",ylab='Values ofbeta0',xlab='Iterations',main='(d) Ergodic Mean Plot ofbeta0',ylim=ylims1)

lines(idx2,ergbeta12[idx2-burnin],col=2,lty=2)

par(mfrow=c(1,1))

apply(beta[(burnin+1):m,],2,mean)

apply(beta[(burnin+1):m,],2,sd)

cor(beta[(burnin+1):m,1],beta[(burnin+1):m,2])                                           

![](https://img-blog.csdn.net/20170207172116084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 独立抽样（二）




m<-5000

xt<-numeric(m)

a<-5

b<-2

p<-.2

n<-30

mu<-c(0,5)

sigma<-c(1,1)

i<-sample(1:2,size=n,replace=T,prob=c(p,1-p))

x<-rnorm(n,mu[i],sigma[i])

u<-runif(m)

y<-rbeta(m,a,b)

xt[1]<-.5

for(i in 2:m){

 fy<-y[i]*dnorm(x,mu[1],sigma[1])+(1-y[i])*dnorm(x,mu[2],sigma[2])

 fx<-xt[i-1]*dnorm(x,mu[1],sigma[1])+(1-xt[i-1])*dnorm(x,mu[2],sigma[2])

 r<-prod(fy/fx)*(xt[i-1]^(a-1)*(1-xt[i-1])^(b-1))/(y[i]^(a-1)*(1-y[i])^(b-1))

  if(u[i]<=r) xt[i]<-y[i]

  else xt[i]<-xt[i-1]

}

plot(xt,type="l",ylab="p")

hist(xt[101:m],main="",xlab="p",prob=T)

print(mean(xt[101:m]))                                                                                                               

![](https://img-blog.csdn.net/20170207172137145?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![独立抽样（MCMC方法）](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)







