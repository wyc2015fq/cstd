# R语言实战笔记 - littlely_ll的博客 - CSDN博客





2017年02月04日 21:26:14[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1645









par(no.readonly=TRUE) 修改当前图形参数，会话结束前一直有效。

**符号与线条**：pch 绘制点时使用的符号；cex 符号大小；lty 线条类型；ldy 线条宽度

legend 图例标签

# **基本图形**

## 条形图

barplot(height, width = 1, space = NULL,

       names.arg = NULL, legend.text = NULL, beside = FALSE,

       horiz = FALSE, density = NULL, angle = 45,

       col = NULL, border = par("fg"),

       main = NULL, sub = NULL, xlab = NULL, ylab = NULL,

       xlim = NULL, ylim = NULL, xpd = TRUE, log = "",

       axes = TRUE, axisnames = TRUE,

       cex.axis = par("cex.axis"), cex.names = par("cex.axis"),

       inside = TRUE, plot = TRUE, axis.lty = 0, offset = 0,

       add = FALSE, args.legend = NULL, ...)

beside为F，则为堆砌条形图，为T，则为分组条形图

## *棘状图*

library(vcd)

attach(Arthritis)

counts<-table(Treatment,Improved)

spine(counts,main="Spinogram")

![R语言实战笔记](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)
![](https://img-blog.csdn.net/20170207160618152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 直方图

hist(mtcars$mpg,freq=F,breaks=12,col="red",xlab="Miles perGallon",main="Histogram,rug plot,density curve")

rug(jitter(mtcars$mpg))

lines(density(mtcars$mpg),col="blue",lwd=2)#density为核密度图
|freq|logical; if TRUE, the histogramgraphic is a representation of frequencies,thecounts component of the result;if FALSE, probability densities, componentdensity,are plotted (so that the histogram has a total area of one).Defaults to TRUE if and onlyif breaks are equidistant(and probability is notspecified).|
|----|----|

![R语言实战笔记](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)![](https://img-blog.csdn.net/20170207160711387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 核密度图

par(lwd=2)  #双倍线条宽度

library(sm)

attach(mtcars)

cyl.f<-factor(cyl,levels=c(4,6,8),labels=c("4cylinders","6cylinders","8cylinders"))#创建分组因子

sm.density.compare(mpg,cyl,xlab="miles per gallon")#绘制密度图

title(main="MPG Distribution by Car Cylinders")

colfill<-c(2:(1+length(levels(cyl.f)))) #通过鼠标点击添加图例

legend(locator(1),levels(cyl.f),fill=colfill)

detach(mtcars)

![](https://img-blog.csdn.net/20170207160745610?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 箱线图

boxplot(mpg~cyl,data=mtcars,main="Car Mileage Data",xlab="Numberof Cylinders",ylab="Miles perGallon")

![](https://img-blog.csdn.net/20170207160836298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


mtcars$cyl.f<-factor(mtcars$cyl,levels=c(4,6,8),labels=c("4","6","8"))

mtcars$am.f<-factor(mtcars$am,levels=c(0,1),labels=c("auto","standard"))

boxplot(mpg~am.f*cyl.f,data=mtcars,varwidth=T,col=c("gold","green"),main="MPGDistribution by Auto Type",xlab="Auto Type")

![](https://img-blog.csdn.net/20170207160918048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 点图

dotchart(x,labels=)

x<-mtcars[order(mtcars$mpg),]

x$cyl<-factor(x$cyl)

x$color[x$cyl==4]<-"red"

x$color[x$cyl==6]<-"blue"

x$color[x$cyl==8]<-"darkgreen"

dotchart(x$mpg,labels=row.names(x),cex=.7,groups=x$cyl,gcolor="black",color=x$color,pch=19,main="GasMileage for Car Models",xlab="Miles per Gallon")

scatterplot(weight~height,data=women,spread=F,lty.smooth=2,pch=19,main="WomenAge 30-39",xlab="Height",ylab="Weight")

![](https://img-blog.csdn.net/20170207161010812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




scatterplotMatrix(x, var.labels=colnames(x),

   diagonal=c("density", "boxplot", "histogram", "oned", "qqplot","none"),

    adjust=1,nclass,

   plot.points=TRUE, smoother=loessLine, smoother.args=list(), smooth,span,

    spread =!by.groups, reg.line=lm,

   transform=FALSE, family=c("bcPower", "yjPower"),

   ellipse=FALSE, levels=c(.5, .95), robust=TRUE,

    groups=NULL,by.groups=FALSE,



   use=c("complete.obs", "pairwise.complete.obs"),

    labels,id.method="mahal", id.n=0, id.cex=1, id.col=palette()[1],

    col=if(n.groups == 1) palette()[3:1] else rep(palette(),length=n.groups),



   pch=1:n.groups, lwd=1, lty=1,

   cex=par("cex"), cex.axis=par("cex.axis"), cex.labels=NULL,

   cex.main=par("cex.main"),

legend.plot=length(levels(groups)) > 1,legend.pos=NULL, row1attop=TRUE, ...)

## 气泡图

symbols（x,y,circle=radius）

## 相关图

corrgram(x, type=NULL, order = FALSE, labels,panel=panel.shade,

  lower.panel=panel,upper.panel=panel, diag.panel=NULL,

  text.panel=textPanel,label.pos=c(0.5, 0.5), label.srt=0, cex.labels=NULL,

  font.labels=1, row1attop=TRUE,dir="", gap=0, abs=FALSE,

 col.regions=colorRampPalette(c("red","salmon","white","royalblue","navy")),

  cor.method="pearson",

  ...)

library(corrgram)

corrgram(mtcars,order=T,lower.panel=panel.shade,upper.panel=panel.pie,text.panel=panel.txt,main="Correlogram of mtcars intercorrelations")

![](https://img-blog.csdn.net/20170207161043955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


正斜杠为正相关，反斜杠为负相关。颜色越深，相关性越大。正相关从12点处开始顺时针填充饼图，负相关逆时针填充饼图。
|## 马赛克图Library(vcd)mosaic(Titanic,shade=T,legend=T)mosaic(~Class+Sex+Age+Survived,data=Titanic,shade=T,legend=T) #the two are same| |
|----|----|
|| |
|| |
|| |
|## 缺失值图library(mice)md.pattern(sleep)   BodyWgt BrainWgt Pred Exp Danger Sleep Span Gest Dream NonD  42       1        1    1   1      1     1    1    1     1    1  0 2       1        1    1   1      1     1    0    1     1    1  1 3       1        1    1   1      1     1    1    0     1    1  1 9       1        1    1   1      1     1    1    1     0    0  2 2       1        1    1   1      1     0    1    1     1    0  2 1       1        1    1   1      1     1    0    0     1    1  2 2       1        1    1   1      1     0    1    1     0    0  3 1       1        1    1   1      1     1    0    1     0    0  3         0        0    0   0      0     4    4    4    12   14 38| |
|| |
|| |
|| |
|library(VIM)aggr(sleep,prop=F,numbers=T)## 描述性统计| |
|| |
||||----|||
|| |

aggregate(mtcars[vars],by=list(am=mtcars$am),mean)

 am     mpg      hp      wt

1  0  17.14737  160.2632  3.768895

2  1  24.39231  126.8462  2.411000

aggregate()仅允许在每次调用中使用平均数、标准差这样的单返回值函数。

列联表：

library(vcd) #use data Arthritis

library(gmodels)

CrossTable(Arthritis$Treatment,Arthritis$Improved)

   Cell Contents

|-------------------------|

|                      N |

| Chi-square contribution |

|          N / Row Total |

|          N / Col Total |

|        N / Table Total |

|-------------------------|

Total Observations in Table:  84

                 |Arthritis$Improved

Arthritis$Treatment|     None|     Some |    Marked| Row Total |

--------------------|-----------|-----------|-----------|-----------|

           Placebo|       29|        7|        7|       43
 |

                   |    2.616|    0.004|    3.752|          |

                   |    0.674|    0.163|    0.163|    0.512
 |

                   |    0.690|    0.500|    0.250|          |

                   |    0.345|    0.083|    0.083|          |

--------------------|-----------|-----------|-----------|-----------|

           Treated|       13|        7|       21|       41
 |

                   |    2.744|    0.004 |    3.935|          |

                   |    0.317|    0.171|    0.512|    0.488
 |

                   |    0.310|    0.500|    0.750|          |

                   |    0.155|    0.083|    0.250|          |

--------------------|-----------|-----------|-----------|-----------|

      Column Total|       42|       14|       28|       84
 |

                   |    0.500|    0.167|    0.333|          |

--------------------|-----------|-----------|-----------|-----------|

## OLS回归

lm(formula, data, subset, weights, na.action,

   method = "qr", model =TRUE, x = FALSE, y = FALSE, qr = TRUE,

   singular.ok = TRUE,contrasts = NULL, offset, ...)

# 多元线性回归

## 检验变量相关性

states<-as.data.frame(state.x77[,c("Murder","Population","Illiteracy","Income","Frost")])

cor(states)

              Murder PopulationIlliteracy    Income     Frost

Murder     1.0000000  0.3436428  0.7029752-0.2300776 -0.5388834

Population  0.3436428 1.0000000  0.1076224  0.2082276-0.3321525

Illiteracy  0.7029752 0.1076224  1.0000000 -0.4370752 -0.6719470

Income    -0.2300776  0.2082276 -0.4370752 1.0000000  0.2262822

Frost     -0.5388834 -0.3321525 -0.6719470 0.2262822  1.0000000

library(car)

scatterplotMatrix(states,spread=F,lty.smooth=2,main="Scatter PlotMatrix")

![](https://img-blog.csdn.net/20170207161314957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 简单线性回归

lm(formular,data=mydata)

For example：

fit<- lm(weight~height+I(height^2),data=women) 

coef(fit)  #output the result ofcoefficient

 (Intercept)      height  I(height^2)

261.87818358 -7.34831933   0.08306399

## 回归诊断

confint(fit)

                  2.5%      97.5 %

(Intercept) 206.97913605 316.77723111

height      -9.04276525  -5.65387341

I(height^2)  0.07003547   0.09609252

95%的置信区间从2.5%~97.5%。如果置信区间包含0，则该变量对方程无影响。

par(mfrow=c(2,2))

plot(fit)




## 向后回归

library(MASS)

fit1<-lm(Murder~Population+Illiteracy+Income+Frost,data=states)

stepAIC(fit1,direction="backward")

Start:  AIC=97.75

Murder ~ Population + Illiteracy + Income + Frost

            Df Sum of Sq   RSS    AIC

-Frost      1    0.021 289.19  95.753

-Income     1    0.057 289.22  95.759

                   289.17  97.749

- Population 1    39.238328.41 102.111

- Illiteracy 1   144.264 433.43 115.986

Step:  AIC=95.75

Murder ~ Population + Illiteracy + Income

            Df Sum of Sq   RSS    AIC

-Income     1    0.057 289.25  93.763

                   289.19  95.753

- Population 1    43.658332.85 100.783

- Illiteracy 1   236.196 525.38 123.605

Step:  AIC=93.76

Murder ~ Population + Illiteracy

            Df Sum of Sq   RSS    AIC

                   289.25  93.763

- Population 1    48.517337.76  99.516

- Illiteracy 1   299.646 588.89 127.311

Call:

lm(formula = Murder ~ Population + Illiteracy, data =states)

Coefficients:

(Intercept)  Population  Illiteracy 

 1.6515497   0.0002242   4.0807366

## 全子集回归

regsubsets(x=, data=, weights=NULL, nbest=1,nvmax=8, force.in=NULL, force.out=NULL, intercept=TRUE,method=c("exhaustive", "backward", "forward", "seqrep"),really.big=FALSE,...)
|x|design matrix or model formula for full model,or biglm object|
|----|----|
|data|Optional data frame|
|y|response vector|
|weights|weight vector|
|nbest|number of subsets of each size to record|
|nvmax|maximum size of subsets to examine|
|force.in|index to columns of design matrix that should be in allmodels|
|force.out|index to columns of design matrix that should be in nomodels|
|intercept|Add an intercept?|
|method|Use exhaustive search, forward selection, backward selection orsequential replacement to search.|
|really.big|Must be TRUE to perform exhaustive search on more than 50variables.|
|object|regsubsets object|
|all.best|Show all the best subsets or just one of each size|
|matrix|Show a matrix of the variables in each model or just summarystatistics|
|matrix.logical|With matrix=TRUE, the matrix islogical TRUE/FALSE orstring "*"/" "|
|df|Specify a number of degrees of freedom for the summarystatistics. The default is n-1|
|id|Which model or models (ordered as in the summary output) toreturn coefficients and variance matrix for|
|vcov|If TRUE, return the variance-covariance matrixas an attribute|

leaps<-regsubsets(Murder~Population+Illiteracy+Income+Frost,data=states,nbest=4)

plot(leaps,scale="adjr2")

![](https://img-blog.csdn.net/20170207161425457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


adjr2为调整的R^2

# **广义线性模型**

## **glm()函数**

## Logistic回归

用到的安装包“AER”

glm ( formular, data = mydata, family = binomial () ).其中formular为回归式，因变量为二元值。

泊松回归：

glm ( formular, data= mydata, family = poisson()).Formular为回归式。

主成分分析：

用到的安装包“psych”

Principal() 含多种方差旋转方法的主成分分析；fa()可用主轴、最小残差、最大似然估计的因子分析；fa.parallel() 含平行分析的碎石图；factor()绘制因子分析或主成分分析的结果。

1. 判断主成分个数

fa.parallel(Harman23.cor$cov,n.obs=302,fa="pc",n.iter=100,show.legend=F,main="ScreePlot")

2. 提取主成分及主成分旋转

rc<-principal(Harman23.cor$cov,nfactors=2,rotate="varimax")#nfators mean 2 facors

rc

Principal Components Analysis

Call: principal(r = Harman23.cor$cov, nfactors = 2, rotate ="varimax")

Standardized loadings (pattern matrix) based upon correlationmatrix

               PC1  PC2  h2    u2 com

height        0.90 0.25 0.88 0.123 1.2

arm.span      0.93 0.19 0.90 0.097 1.1

forearm       0.92 0.16 0.87 0.128 1.1

lower.leg     0.90 0.22 0.86 0.139 1.1

weight        0.26 0.88 0.85 0.150 1.2

bitro.diameter 0.19 0.84 0.74 0.261 1.1

chest.girth   0.11 0.84 0.72 0.283 1.0

chest.width   0.26 0.75 0.62 0.375 1.2



                      PC1  PC2

SSloadings          3.52 2.92

ProportionVar       0.44 0.37

CumulativeVar       0.44 0.81

两个主成分PC1，PC2，h2为成分公因子方差——主成分对每个变量的解释度，u2（1-h2）为无法解释的比例。ssloadings包含与主成分相关联的特征值；proportion var 是每个主成分对整个数据集的解释度；cumulativevar是主成分累计可解释度。

3. 获取主成分得分系数

round(unclass(rc$weights),2) #have already standardized

                PC1   PC2

height         0.28 -0.05

arm.span       0.30 -0.08

forearm        0.30 -0.09

lower.leg      0.28 -0.06

weight        -0.06  0.33

bitro.diameter -0.08  0.32

chest.girth   -0.10  0.34

chest.width   -0.04  0.27




