# ARCH模型和GARCH模型 - littlely_ll的博客 - CSDN博客





2017年02月04日 21:26:31[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：9741
个人分类：[金融计量](https://blog.csdn.net/littlely_ll/article/category/6665853)










## 基于ARCH（1）模型模拟生成收益序列，残差序列和波动率序列
library(fGarch)set.seed(1234)#模型的设定
spec_1<-garchSpec(model=list(omega=.01,alpha=.85,beta=0))#模型的模拟
simdata_1<-garchSim(spec_1,n=200,extended=T)
plot(simdata_1)par(mfrow=c(1,3))
acf(simdata_1$eps,main="resid series",xlab="lags")acf(simdata_1$garch,main="simulations data",xlab="lags")acf(simdata_1$garch^2,main="squared sim data",xlab="lags")[](http://photo.blog.sina.com.cn/showpic.html#blogid=15dd753ab0102wev8&url=http://album.sina.com.cn/pic/006pdeFtzy70SWMBtm1e1)
library(FinTS)
ArchTest(simdata_1$garch,lags=12)
        ARCH LM-test; Null hypothesis: no ARCH effects

data:  simdata_1$garch
Chi-squared = 93.981, df = 12, p-value = 8.327e-15

ArchTest(simdata_1$eps,lags=12)

        ARCH LM-test; Null hypothesis: no ARCH effects

data:  simdata_1$eps
Chi-squared = 10.863, df = 12, p-value = 0.5407


## GARCH（1,1）模型的模拟
spec_2<-garchSpec(model=list(omega=.01,alpha=.85,beta=.1))
simdata_2<-garchSim(spec_2,n=200,extended=T)
class(simdata_2)
[1] "timeSeries"
attr(,"package")
[1] "timeSeries"
par(mfrow=c(1,3))
plot(simdata_2)
[](http://photo.blog.sina.com.cn/showpic.html#blogid=15dd753ab0102wev8&url=http://album.sina.com.cn/pic/006pdeFtzy70SXIic1Uf6)plot(simdata_2$eps,type="l",xlab="D",)
plot(simdata_2$garch,type="l",xlab="D")
plot(simdata_2$sigma,type="l",xlab="D")[](http://photo.blog.sina.com.cn/showpic.html#blogid=15dd753ab0102wev8&url=http://album.sina.com.cn/pic/006pdeFtzy70SXLVipAc2)acf(simdata_2$eps,xlag="lags")acf(simdata_2$garch,xlab="lag")acf(simdata_2$garch^2,xlab="lag")ArchTest(simdata_2$garch,lags=12)

        ARCH LM-test; Null hypothesis: no ARCH effects

data:  simdata_2$garch
Chi-squared = 32.738, df = 12, p-value = 0.001063

ArchTest(simdata_2$eps,lags=12)

        ARCH LM-test; Null hypothesis: no ARCH effects

data:  simdata_2$eps
Chi-squared = 12.762, df = 12, p-value = 0.3866



