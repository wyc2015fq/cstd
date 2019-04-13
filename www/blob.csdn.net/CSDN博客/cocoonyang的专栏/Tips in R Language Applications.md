
# Tips in R Language Applications - cocoonyang的专栏 - CSDN博客


2018年08月22日 13:13:57[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：96



# Plotting Graphic
## Layout
`par(mfrow=c(1,2)) 
plot( thickness, torsion_disp, xlab= "Steel Plate Thickness (mm)", ylab="Torsion Stiffness Scenario Displacement (mm)")  
lines(lowess(thickness, torsion_disp), col = "red" )
plot( thickness, mass, xlab= "Steel Plate Thickness (mm)", ylab="BIW Mass (Kg)")  
lines(lowess(thickness, mass), col = "red" )`
## Make a pdf graphic[3]
`pdf("my_plot.pdf", width=6, height=4)
plot(1:10)
dev.off()`Organizing the plotting commands in a function
`fig.myplot <- function() {
  plot(1:10)
  points(runif(100, 1, 10), runif(100, 1, 10), col=rainbow(100))
  abline(h=5)
  title(main="My lovely plot")
}
pdf("my_plot.pdf", width=6, height=4)
fig.myplot()
dev.off()`
## Save graphic as a jpg file
`filename <- paste( "D:\\scatter", ".jpg" )
jpeg(  filename  );
plot( mass, disp, xlab= "Mass (Kg)", ylab="Displacement (mm)")  
lines(lowess(mass, disp), col = "blue" )
abline( lm(disp~mass), col = "red")`
## Title and Axis Label
An example from [3]
```python
plot(
```
```python
1
```
```python
:
```
```python
10
```
```python
, xlab=
```
```python
""
```
```python
, ylab=
```
```python
""
```
```python
)
```
```python
xlab <-
```
```python
expression(
```
```python
paste(
```
```python
"Photosynthetic rate ("
```
```python
, mu,
```
```python
" mol "
```
```python
, m^-
```
```python
2
```
```python
, s^-
```
```python
1
```
```python
,
```
```python
")"
```
```python
)
```
```python
)
```
```python
ylab <- "Photosynthetic rate (u mol m^-2 s^-1)"
```
```python
mtext(xlab,
```
```python
1
```
```python
,
```
```python
3
```
```python
)
```
```python
mtext(ylab,
```
```python
2
```
```python
,
```
```python
3
```
```python
)
```
```python
title(main=
```
```python
"Doesn't the x axis label look nice?"
```
```python
)
```
## Draw points
```python
points(  x, y,pch=
```
```python
19
```
```python
,col=
```
```python
"red"
```
```python
)
```
```python
;
```
# 直方图
在R语言环境中绘制直方图可以使用 hist, plot和ggplot2等
## hist
### 语法
```python
hist(x, breaks =
```
```python
"Sturges"
```
```python
,
     freq =
```
```python
NULL
```
```python
, probability = !freq,
```
```python
include
```
```python
.lowest =
```
```python
TRUE
```
```python
,
```
```python
right
```
```python
=
```
```python
TRUE
```
```python
,
     density =
```
```python
NULL
```
```python
, angle =
```
```python
45
```
```python
, col =
```
```python
NULL
```
```python
, border =
```
```python
NULL
```
```python
,
     main = paste(
```
```python
"Histogram of"
```
```python
, xname),
     xlim = range(breaks), ylim =
```
```python
NULL
```
```python
,
     xlab = xname, ylab,
     axes =
```
```python
TRUE
```
```python
, plot =
```
```python
TRUE
```
```python
, labels =
```
```python
FALSE
```
```python
,
     nclass =
```
```python
NULL
```
```python
, warn.unused =
```
```python
TRUE
```
```python
, …)
```
### 参数
x – 数组， 包含histogram所要展示的数据(a vector of values for which the histogram is desired.)
breaks, 可为以下几种类型：
数组 – 包含histogram单元分隔点(a vector giving the breakpoints between histogram cells)
函数 – 用于计算分割点数组(a function to compute the vector of breakpoints)
数  – 设定histogram中单元数量(a single number giving the number of cells for the histogram)
字符串 – 指定计算histogram中单元数量的算法 (a character string naming an algorithm to compute the number of cells)
函数 – 计算histogram单元数量(a function to compute the number of cells)
freq – 逻辑(布尔型)变量.
True – the histogram graphic is a representation of frequencies, the counts component of the result
False – probability densities, component density, are plotted (so that the histogram has a total area of one)
probability
an alias for !freq, for S compatibility.
include.lowest
logical; if TRUE, an x[i] equal to the breaks value will be included in the first (or last, for right = FALSE) bar. This will be ignored (with a warning) unless breaks is a vector.
right
logical; if TRUE, the histogram cells are right-closed (left open) intervals.
density
the density of shading lines, in lines per inch. The default value of NULL means that no shading lines are drawn. Non-positive values of density also inhibit the drawing of shading lines.
angle
the slope of shading lines, given as an angle in degrees (counter-clockwise).
col
a colour to be used to fill the bars. The default of NULL yields unfilled bars.
border
the color of the border around the bars. The default is to use the standard foreground color.
main, xlab, ylab
these arguments to title have useful defaults here.
xlim, ylim
the range of x and y values with sensible defaults. Note that xlim is not used to define the histogram (breaks), but only for plotting (when plot = TRUE).
axes
logical. If TRUE (default), axes are draw if the plot is drawn.
plot
logical. If TRUE (default), a histogram is plotted, otherwise a list of breaks and counts is returned. In the latter case, a warning is used if (typically graphical) arguments are specified that only apply to the plot = TRUE case.
labels
logical or character string. Additionally draw labels on top of bars, if not FALSE; see plot.histogram.
nclass
numeric (integer). For S(-PLUS) compatibility only, nclass is equivalent to breaks for a scalar or character argument.
warn.unused
logical. If plot = FALSE and warn.unused = TRUE, a warning will be issued when graphical parameters are passed to hist.default().
样例
样例 1 – 使用hist
\# 准备数据data<-rnorm(n=1000,m=24.2,sd=2.2)\# 绘制直方图hist(data)![hist-data](https://img-blog.csdn.net/2018050214034426?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 2 – 使用hist - 调整数据间隔数量
\# 准备数据data<-rnorm(n=1000,m=24.2,sd=2.2)\# 绘制直方图hist(data, breaks=30)![这里写图片描述](https://img-blog.csdn.net/2018050214093021?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 3 – 使用hist - 分布密度直方图
\# 准备数据data<-rnorm(n=1000,m=24.2,sd=2.2)\# 绘制直方图hist(data, freq=FALSE)![这里写图片描述](https://img-blog.csdn.net/20180502141349707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 4 – 使用hist - 分布密度直方图 + 密度分布曲线
\# 准备数据data<-rnorm(n=1000,m=24.2,sd=2.2)\# 绘制直方图hist(data, freq =FALSE, ylim = c(0, 0.2))curve(dnorm(x, mean=mean(data), sd=sd(data)), col = 2, lty = 2, lwd = 2, add =TRUE)![这里写图片描述](https://img-blog.csdn.net/20180502142630975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 5 – 使用hist - 分布频度直方图 + 色彩
\# 准备数据data<-rnorm(n=1000,m=24.2,sd=2.2)\# 绘制直方图colors= c("red","yellow","green","violet","orange","blue","pink","cyan")hist(data,  right=FALSE, col=colors, main="DataDistrubution",  xlab="xdata")![这里写图片描述](https://img-blog.csdn.net/20180502144333428?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 6 – 使用hist - 分布频度直方图 + label
\# 准备数据data<-rnorm(n=1000,m=24.2,sd=2.2)\# 绘制直方图colors= c("red","yellow","green","violet","orange","blue","pink","cyan")h<-hist(data,  right=FALSE, col=colors, main="DataDistrubution",  xlab="xdata")text(h$mids,h$counts,labels=h$counts, adj=c(0.5, -0.5))![这里写图片描述](https://img-blog.csdn.net/2018050214582739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 7 – 使用hist - 两个分布频度直方图
\# 准备数据data1<-rnorm(n=1000,m=24.2,sd=2.2)data2<-rnorm(n=1000,m=34.2,sd=2.2)\# 绘制直方图hist(data1,freq=FALSE,ylim=c(0,0.20),xlim=c(15,45),col='skyblue',main="Histogram of Data",xlab="data")hist(data2,freq=FALSE,ylim=c(0,0.20),add=T,col='red')![这里写图片描述](https://img-blog.csdn.net/20180502154522278?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 8 – 使用hist - 两个分布频度直方图 + 透视色
\# 准备数据data1<-rnorm(n=1000,m=24.2,sd=2.2)data2<-rnorm(n=1000,m=34.2,sd=2.2)\# 绘制直方图hist(data1,freq=FALSE,ylim=c(0,0.20),xlim=c(15,45),border=T,col='skyblue',main="Histogram of Data",xlab="data")hist(data2,freq=FALSE,ylim=c(0,0.20),add=T,border=T,col=rgb(0,1,0,0.5))![这里写图片描述](https://img-blog.csdn.net/20180502155452723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
plot
样例
样例 9 –  使用plot - 两个分布频度直方图
\# 准备数据
data1<-rnorm(n=1000, m=24.2, sd=2.2)data2<-rnorm(n=1000, m=34.2, sd=2.2)p1 <-hist(data1,  plot=FALSE)p2 <-hist(data2,  plot=FALSE)\# 绘制直方图plot(0,0,type="n",xlim=c(15,45),ylim=c(0,200),xlab="x",ylab="freq",main="Two histograms")plot(p1,col="green",density=10,angle=135,add=TRUE)plot(p2,col="blue",density=10,angle=45,add=TRUE)![这里写图片描述](https://img-blog.csdn.net/20180502160758611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 10 – 使用plot - 两个分布密度曲线图
\# 准备数据
data1<-rnorm(n=1000, m=24.2, sd=2.2)data2<-rnorm(n=1000, m=34.2, sd=2.2)\#\# 计算分布密度
densdata1 <-density(data1)densdata2 <-density(data2)\#\#  
xlim <-range(densdata2$x,densdata1$x)ylim <-range(0,densdata2$y, densdata1$y)\#pickthe colours
data1Col <-rgb(1,0,0,0.2)data2Col <-rgb(0,0,1,0.2)\#\#plot(densdata1, xlim = xlim, ylim = ylim, xlab ='data',
     main ='Distribution of data2 and data2', 
     panel.first =grid())\#polygon(densdata1, density = -1, col = data1Col)polygon(densdata2, density = -1, col = data2Col)\#\# 标题legend('topleft',c('data1','data2'),
       fill =c(data1Col, data2Col), bty ='n',
       border = NA)![这里写图片描述](https://img-blog.csdn.net/20180502161912972?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
ggplot
样例
样例 11 – 使用ggplot2  - 分布密度曲线
安装
install.packages("ggplot2")library(ggplot2)\# 准备数据data<-rnorm(n=1000,m=24.2,sd=2.2)\# 分布密度曲线ggplot(data=NULL, aes(x=data)) + geom_density()![这里写图片描述](https://img-blog.csdn.net/20180502170038735?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
样例 12 – 使用ggplot2  - 两个分布密度曲线
library(ggplot2)\# 准备数据data1<-data.frame(length=rnorm(n=1000,m=24.2,sd=2.2))data2<-data.frame(length=rnorm(n=1000,m=34.2,sd=2.2))data1$veg <- 'A'data2$veg <- 'B'vegLengths<- rbind(data1, data2)ggplot(vegLengths, aes(length, fill = veg)) + geom_density(alpha =0.2)![这里写图片描述](https://img-blog.csdn.net/20180502172248267?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Functions in R Language
Defining functions
functionName <-function( parameter1, parameter2 ){
\#   codereturn( results )
}
\# Call thefunctionprint( functionName( 1, 2, 3 ))Array
Iterating an array
As in many other programming languages, you repeat an action for every entry of a vector through a for loop[1].
for(iinvalues){...do something...}samples <- c(rep(1:10))
baseList <- c(1,2,3,4,5)
XList =list()
YList =list()
ZList =list()for( iinbaseList)
{
    x = baseList[i]
    tmp = sin(x);
    XList[i] = tmp
    tmp = x*x
    YList[i] = tmp
}seq_along creates a sequence from 1 up to the length of its input[2]:
pp <-c("Peter","Piper","picked","a","peck","of","pickled","peppers")for(i inseq_along(pp))print(pp[i])for(idinc(1,2,3,4,5)){
  print(paste("This is",id))
}for(idinc(1:5)){
  print(paste("This is",id))
}调用R程序
在node.js 环境中调用 R程序
//-----------------------------------//   Modules//-----------------------------------constcp =require('child_process');////  Variables//varr_script_file_path ='./statis.r'varresultPath ='./result.txt';constR_Path ="D:\\R\\R-3.4.1\\bin\\x64\\R.exe"//-----------------------------------//   Methods//-----------------------------------//  Run R jobs//functionrunRcode( thefilename ){console.log( Date() ) 
    console.log('      --  R statisitc analysis --    ') ;varcommand = R_Path +' CMD BATCH --vanilla --slave '+ thefilename +' '+ resultPath ;
    cp.execSync( command );
    console.log(  command ) ;
}//-----------------------------------// Main process//-----------------------------------try{
    runRcode( r_script_file_path );
}catch(exception)
{
    console.log(exception);
}[1] R in Action,[http://www.statmethods.net/graphs/](http://www.statmethods.net/graphs/)
[2][https://www.stat.auckland.ac.nz/~paul/RG2e/](https://www.stat.auckland.ac.nz/~paul/RG2e/)
[3][https://nicercode.github.io/guides/plotting/](https://nicercode.github.io/guides/plotting/)
[4][http://bxhorn.com/r-graphics-plot-parameters/\#anatomy-of-a-plot](http://bxhorn.com/r-graphics-plot-parameters/#anatomy-of-a-plot)
[5][https://www.rdocumentation.org/packages/graphics/versions/3.4.3/topics/hist](https://www.rdocumentation.org/packages/graphics/versions/3.4.3/topics/hist)
[6][http://www.r-tutor.com/elementary-statistics/quantitative-data/histogram](http://www.r-tutor.com/elementary-statistics/quantitative-data/histogram)
[7][https://www.r-bloggers.com/basics-of-histograms/](https://www.r-bloggers.com/basics-of-histograms/)
[8][https://stackoverflow.com/questions/3541713/how-to-plot-two-histograms-together-in-r](https://stackoverflow.com/questions/3541713/how-to-plot-two-histograms-together-in-r)
[9][http://ggplot2.org/](http://ggplot2.org/)
[10][http://www.cookbook-r.com/Graphs/Plotting_distributions_(ggplot2)/](http://www.cookbook-r.com/Graphs/Plotting_distributions_(ggplot2)/)
[11][http://www.dummies.com/programming/r/how-to-loop-through-values-in-r/](http://www.dummies.com/programming/r/how-to-loop-through-values-in-r/)
[12][https://www.safaribooksonline.com/library/view/learning-r/9781449357160/ch04.html](https://www.safaribooksonline.com/library/view/learning-r/9781449357160/ch04.html)

