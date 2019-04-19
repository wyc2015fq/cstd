# R语言读入数据 - 三少GG - CSDN博客
2014年03月14日 22:40:16[三少GG](https://me.csdn.net/scut1135)阅读数：2042

# R语言读入数据
已有 3385 次阅读2012-10-9 15:21|个人分类:[R语言学习](http://blog.sciencenet.cn/home.php?mod=space&uid=793574&do=blog&classid=160586&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=793574&catid=1)|关键词:R注入数据
一、   R 导入文本文件
R中导入数据的基本命令是scan()。比较常用的命令有read.table()、read.csv()、read.fwf()等，这些命令其实在内部也是通过调用scan()实现的，它们可以看作是scan()的简化。
R从使用分界符的文本(delimited text)读取数据到数据框的命令为read.table()。关于此文本的格式，有以下要求：
- 行的分界符为换行/回车，列的分界符为空格或制表符(TAB)。
- 特别注意，R把任意多个连续的空格或制表符只算作一个。
- 由于R把连续空格或制表符只算作一个，如果数据表中有空缺的单元格，将会导致R读入时列发生错位。
- 如果数据表中有空缺的单元格，一定要用NA来表示缺失的数据，否则将会导致读入数据错位。
- 一般来说第一行用来存放变量名。如果第一行不是变量名，R用“V1, V2, …”来命名变量。
- 为了以后方便地跟踪数据，应当设置ID列。一般来说第一列用来存放ID。
- 如果变量名比数据列少一个，R将假定第一列为ID列。
读取符合以上要求的文本文件的最简单的命令为：
mydata <- read.table("mydata.txt")
保存数据为文本文件的命令为write.table()。最简单的用法为：
write.table(mydata, file="mydata.txt")
1.1    CSV格式
最常见的一种数据文本文件格式为CSV(comma separated value)文件，采用逗号作为分界符，第一行为变量名，第一列为id，缺失值用空格表示。读取CSV文件的命令为：
mydata <- read.table("mydata.csv", header=TRUE, sep=",", row.names="id", na.strings=" ")
R中也有专门的读取CSV文件的命令read.csv()。一般的用法为：
mydata <- read.csv("mydata.csv", row.names="id", na.strings=" ")
1.2    定宽格式
从定宽(fixed-width)格式读取数据的命令为read.fwf()。定宽格式中只有数据值，没有变量名，数据列之间没有分界符。每一列数据的宽度一定。例如：
mydata <- read.fwf(file="mydata.txt", width=c(1,1,2), col.names=c("id","gender","age"), row.names="id")
read.fwf()其实在内部是首先将定宽文本读入、分割列，然后调用read.table()来实现的。
另外，read.fortran()读入的是按Fortran的规范来格式化的定宽文本文件。
1.3     DIF格式
从数据交换格式(DIF, Data Interchange Format)读取数据的命令为read.DIF()。
Excel的剪贴板就是使用这种格式来存储数据的，因此可以用这一命令来读取剪贴板中的Excel数据，其最简单的调用为：
mydata <- read.DIF("clipboard")
1.4    指定数据列
因为R的所有数据运算都在内存中运行，节省内存空间在实践中是非常重要的。在读入数据后，所有的数据都被放入内存。为了节省内存的使用，在读取数据时就应当有选择地只读入对当前任务有用的数据。
read.table()函数可以通过colClasses参数来实现对数据列的选择。colClasses参数引用一个字符矢量。colClasses参数的本意是设定每一列的数值类型，但是可以通过设定NULL类型来跳过不需要的数据列。比如：
mydata <- read.table("mydata.txt", colClasses=c("integer", "character", "NULL", "logical"))
通过使用上面的命令，读取的数据就跳过了数据文件中的第3列。使用colClasses参数必须指明每一列的类型。使用colClasses参数可以大幅提升数据的读入速度。
在读取定宽文本文件(fixed-width file)时选择列的方法是设定列宽为负数。例如：
mydata <- read.fwf(file="mydata.txt", width=c(1,-1,2), col.names=c("id","gender","age"), row.names="id")
通过使用上面的命令，读取的数据就跳过了数据文件中的第2列。
1.5    使用scan()命令
read.table()和read.fwf()都是通过调用scan()来实现的。在某些时候直接使用scan()可以更快速的读取数据。
比如读入一个200*2000的数值矩阵可以采用：
mymatrix <- matrix(scan("matrix.dat", n = 200*2000), 200, 2000, byrow = TRUE)
如果用read.table()那么其命令为：
mymatrix <- as.matrix(read.table("matrix.dat"))
第二种方式所花的时间是第一种方式的10倍。
scan()命令中使用what参数来设定所读入的数据类型，设定恰当的what参数可以大幅提高数据的读入速度。
scan()命令还能从键盘读入数据。
可使用foreign包导入外部格式的数据。
- EpiInfo：read.epiinfo()
- Minitab Portable Worksheet：read.mtp()
- SAS Transport (XPORT)：read.xport()
- S-PLUS：read.S()
- S-PLUS data dumps：data.restore()
- SPSS：read.spss()
- Stata：read.dta()
- Systat：read.systat()
二、  R 导入其它统计软件的数据
R 导入二进制文件：使用readBin()和writeBin()函数来读写。
可以使用foreign包来导出数据。导出数据的命令为write.foreign()。
也可使用Hmisc包导入外部格式的数据。
- SAS：sasxport.get()
- SPSS：spss.get()
