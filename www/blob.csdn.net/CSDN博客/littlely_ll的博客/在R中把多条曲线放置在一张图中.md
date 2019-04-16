# 在R中把多条曲线放置在一张图中 - littlely_ll的博客 - CSDN博客





2017年02月04日 21:26:17[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：13102
个人分类：[R语言基础](https://blog.csdn.net/littlely_ll/article/category/6569419)










library(xlsx)

myield<-read.xlsx("myield.xlsx",header=T,sheetIndex=1)

head(myield)

    time     X3m     X6m     X1y     X2y     X3y     X4y

1 2002.010.019889 0.020353 0.021264 0.023014 0.024670 0.026232

2 2002.020.020781 0.021131 0.021819 0.023155 0.024437 0.025663

3 2002.030.018449 0.018829 0.019574 0.021015 0.022389 0.023695

4 2002.040.019096 0.019325 0.019780 0.020668 0.021529 0.022362

5 2002.050.018228 0.018425 0.018815 0.019578 0.020319 0.021037

6 2002.060.017224 0.017479 0.017980 0.018952 0.019882 0.020771

tm<-myield[,2]

sm<-myield[,3]

oy<-myield[,4]

ty<-myield[,5]

time<-myield[,1]

thy<-myield[,6]

fy<-myield[,7]

plot(tm~time,type="l",lty=1)

lines(sm~time,col="red",lty=2)

lines(oy~time,col="blue",lty=3)

lines(ty~time,col="yellow",lty=4)

lines(thy~time,col="green",lty=5)

lines(fy~time,col="grey",lty=6)

title("YIELD(m)",lwd=3)

legend("topleft",cex=.6,c("tm","sm","oy","ty","thy","fy"),col=c("black","red","blue","yellow","green","grey"),lty=1:6)


也可以应用matplot函数，但使用之前注意把数值型数据转换为数据框或者向量，根据本例，即把tm,sm,oy等数值型数据转化为数据框，**z<-data.frame(cbind(tm,sm,oy,ty,thy,fy)),**然后**matplot(time,z,col=1:6,type="l",lwd=2,xlab="",ylab="",lty=1:5)**


**![](https://img-blog.csdn.net/20170207163241688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

![在R中把多条曲线放置在一张图中](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)






