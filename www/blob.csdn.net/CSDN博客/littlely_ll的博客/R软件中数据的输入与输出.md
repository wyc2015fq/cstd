# R软件中数据的输入与输出 - littlely_ll的博客 - CSDN博客





2017年02月04日 21:25:56[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：6089









（一）数据的输入

在R软件中，数据有多种输入方式

（1）csv格式

csv是一种逗号分隔的纯文本文件，在R中可以用read.table
()来读取

例：read.table
(file,header=FALSE,sep=“”)

其和read.csv()读取数据形式相似

read.csv(file,header=TRUE,sep=“,”)

csv格式文件在R中比较容易读取，但是数据量大时读取速度非常慢。

（2）txt 格式


用R读取txt格式的数据同样可以用read.table()函数。如果使用Rstudio，可以在菜单栏Tools中选择“import
dataset”，也可以实现txt文件的读取。

（3）xls、xlsx格式


用R读取xlsx或xls格式的数据时，需要加载xlsx包，该包会自动安装rJava和xlsxjars两个包作为支持。另外，正常运行rJava包还必须安装Java的源程序JDK，可去ORACLE官网下载。

正常运行xlsx包后输入读取语句即可读取数据。R代码如下

>library(xlsx)


>data<-read.xlsx(file,sheetIndex,sheetName=NULL,header=TRUE)





（4）其他数据的读取


其他的统计工具（比如Minitab、SAS、SPSS、Stata等）中的数据被R读取起来有一定的难度，在使用过程中可能会出现一些问题。


由R的核心团队编写的foreign包提供了其他统计软件系统数据导入的工具。若要读取SPSS软件的sav格式数据，R的代码为

>library(foreign)

>z<-read.spss(file)

的数据。

对于小型文件的读取也可以用复制的方式读取。


首先复制Excel表格中的数据，然后在R界面输入read.table(“clipboard”,header=TRUE)，就会显示所输入的数据。

（5）数据编辑器


数据编辑器是R软件中自带的数据输入系统，但是和Excel相比还不成熟，适用于比较短的数据输入。其R代码如下


>data<-data.frame(name=character(0),number=numeric(0))

>data<-edit(data)

之后会出现编辑框，从中输入数据。

（二）数据的输出

（1）保存为简单的文本


write.table(data,file="file",rownames=F,quote=F)

（2）保存为逗号分隔文件


write.csv(data,file="file",rownames=F,quote=F)






