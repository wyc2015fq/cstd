# R 语言数据读取与存储 - 去向前方的博客 - CSDN博客





2018年10月01日 10:16:08[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：568








# 一、R语言读取文本文件：

1、文件目录操作：getwd() : 返回当前工作目录setwd(“d:/data”)  更改工作目录

2、常用的读取指令readread.table() : 读取文本文件read.csv(): 读取csv文件如果出现缺失值，read.table()会报错，read.csv()读取时会自动在缺失的位置填补NA

3、灵活的读取指令scan() :

4、读取固定宽度格式的文件：read.fwf()文本文档中最后一行的回车符很重要，这是一个类似于停止符的标识，否则读入时会显示“最后一行不完整”的警告，但是不影响数据读入的效果。

5、读取Excel数据：

（1）第一种方法首先打开Excel，选中需要的数据后复制，然后再R中输入以下的指令：data.excel =read.delim(“clipboard”) #clipboard即剪贴板

（2）第二种方法：使用RODBC包，获取Excel连接的函数是odbcConnectExcel()和odbcConnectExcel2007() 分别读取Excel2003版和2007版的数据channel =odbcConnectExcel2007(“d:/a.xlsx”)sqlTables(channel)# 列出Excel中的表格获取Sheet1中的数据，可以使用如下任意的一种方式，

data.excel2 =sqlFetch(channel,“Sheet1”)

data.excel2 =sqlQuery(channel,"select * from [Sheet1$KaTeX parse error: Expected 'EOF', got '#' at position 20: …lose(channel) #̲关闭ODBC连接，释放空间6、…$
# 二、数据保存：

1、使用函数cat() cat(…,file="",sep="",fill = FALSE ,labels=NULL,append=FALSE) #file表示要输出的文件名，当参数append=TRUE时，在指定文件的末尾添加内容。sep：表示以空格作为分隔符

2、保存为文本文件：write仅可以写出一个矩阵或向量的特定列，和对一个矩阵进行转置write.table() 可以把一个数据框或列表等对象以包含行列标签的方式写出。write.csv(): 将数据框保存为逗号分隔文件，但是不包含列名

inidat<-c(res1,res2)

f<-data.frame(inidat)

write.table(f,file=“test.csv”,row.names=FALSE,col.names=FALSE);

3、保存R格式文件：save(data,file=“d:/data/salary1.Rdata”)dim():求维度mode() 用来显示对象的类型names()：显示对象中的标签




