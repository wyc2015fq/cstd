# R语言︱list用法、批量读取、写出数据时的用法 - 素质云笔记/Recorder... - CSDN博客





2016年04月11日 17:06:46[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：78006
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)















列表是一种特别的对象集合，它的元素也由序号（下标）区分，但是各元素的类型可 以是任意对象，不同元素不必是同一类型。元素本身允许是其它复杂数据类型，比如，列表 的一个元素也允许是列表。例如：





```
> rec <- list(name="李明", age=30, scores=c(85, 76, 90))
> rec
$name
[1] "李明"
　
$age
[1] 30
　
$scores
[1] 85 76 90
列表元素总可以用“列表名[[下标]]”的格式引用。例如：
> rec[[2]]
[1] 30
> rec[[3]][2]
[1] 76
```




# 修改列表





列表的元素可以修改，只要把元素引用赋值即可。如：

> rec$age <- 45

甚至

> rec$age <- list(19, 29, 31)

（可以任意修改一个列表元素）。如果被赋值的元素原来不存在，则列表延伸以包含该新 元素。







# 提取某List某指标

方法一：
先编写一个提取list子集的函数：



- subdate<- function(x){

- x$DATE

- }

复制代码

然后用lapply或者sapply
sapply(s,subdate)
看一下出来的结果

方法二：
提取DATE内容：


- s[[1]]$DATE

复制代码

变动其中的数字，就可以把每一组的DATE提取出来了。
（需要自己编写循环）



参考于：http://bbs.pinggu.org/thread-3410181-1-1.html




方法三：提取长度大于某程度的list





`x[lapply(x, length)>100]`




用lapply计算每个x的长度。


# list之间的合并

list之间的合并用：


`c（list(1),list(2)）`







——————————————————————————————————————————————————————————————————————

# unlist与list的区别

unlist(x)生成一个包含x所有元素的向量。**作用是，展平数据列表。**


unlist把l.ex[1]=unlist(l.ex)[1]+unlist(l.ex)[2]，一拆为二。



```
> ##unlist是啥？？
> l.ex <- list(list(1:5, LETTERS[1:5]), list("Z","A"),  NA)
> #list数据[]与[[]]是一样的
> l.ex[2]
[[1]]
[[1]][[1]]
[1] "Z"

[[1]][[2]]
[1] "A"


> l.ex[[2]]
[[1]]
[1] "Z"

[[2]]
[1] "A"

> 
> #unlist把l.ex[1]的元素拆开了，分为unlist[1]+unlist[2]
> unlist(l.ex, recursive = FALSE)[3]
[[1]]
[1] "Z"

> unlist(l.ex, recursive = FALSE)[[3]]
[1] "Z"
```




————————————————————————————————————————————————————————————————




# list在批量读取、写出xlsx数据时的用法

**方法弊端：弊端就是循环语句的弊端，导入的原始数据每个sheet都需要相同的数据结构。**







list在批量读取数据时候的用法，一开始笔者困惑在：

1、如何循环读取xlsx中的sheet数据，然后批量放入list之中？——先定义list

2、如何定义写出时候的文件名字——paste函数




批量读取的基本流程就是:写入（list[[i]]）、操作、写出



```
#1、读取xlsx中所有的sheet表格
#如果像vector一样定义List？？——list()函数来主动定义，用data.list[[i]]来赋值
data.list<-list()
for (i in 1:2){
  data.list[[i]]=read.xlsx("C1.xlsx",i)
}
```
以上是写入，看看如何写出：




```
#3、利用List批量读出操作
#难点：如果构造输出表格的名称——paste来构造名称
flie=list()
xlsxflie=paste(1:2,".xlsx",sep="")

for(i in 1:2){
flie[[i]]=paste("C:/Users/long/Desktop/",xlsxflie[i],sep="")
write.xlsx(data.list2[[i]],file)
}
```
写出时候文件名称困扰我很久，如何按照一定的规则来命名，可以先用paste弄好固定格式，然后通过paste[i]循环调用。
其中：paste之后会有如vector一般的格式，可以用[i]来调用。




————————————————————————————————————————————————————————————————





# list中的字符串型数据如何导出？




list是大规模数据操作非常优秀的方式，能够存放非结构化的文本数据。但是如果，文本分好词之后的数据（如下图），如何将存放在list中的数据进行导出呢？



```
[[1]]
        n         v        en         n      
   "职位"    "描述" "Android"    "平台"    
   
[[2]]
           n            v            n         
      "岗位"       "描述"       "前端"
```
笔者想办法的几种方式：


unlist->变成向量

data.frame->变成序号+单词

as.chacter->单个文本



```
#list中的字符串型数据如何导出？

#list中非一般的字符串形式
[[1]]
n         v        en         n      
"职位"    "描述" "Android"    "平台"    

[[2]]
n            v            n         
"岗位"       "描述"       "前端"     

#1、unlist与list区别
Job_Pwordseg.ct[1][1]        #不论如何都不能得到list中的单个单词
unlist(Job_Pwordseg.ct[1])[1]#可以得到单个单词，向量形式

#2、data.frame法，批量处理时，因为不等长而无法合并
data.frame(Job_Pwordseg.ct[1])          #变成了序号+单词
data.frame(unlist(Job_Pwordseg.ct[1]))  #跟list一样的结果

#3、as.character
as.character(unlist(Job_Pwordseg.ct[1]))
#[1] "职位"    "描述"    "Android" "平台"    "进行"    "手机"
as.character(Job_Pwordseg.ct[1])
#[1]"c(\"职位\", \"描述\", \"Android\", \"平台\", \"进行\")
```

变换得到了数据之后，又出现了问题：


如何将list中那么子集合并？——不等长合并

两种方法：c()，可以将list[1] 和list[2]进行直接合并，可以兼容不等长，当然合并之后，还有list文件；

rbind.fill函数，不等长合并函数，在plyr包中。



```
#4、t()在list的文本型
t(data.frame(Job_Pwordseg.ct[1]))
#A [,1]   [,2]   [,3]
#"职位" "描述" "Android"
data.frame(t(data.frame(Job_Pwordseg.ct[1])))
#A [,1]   [,2]   [,3]
#职位    描述   Android

#4、list中字符的合并用c
c(Job_Pwordseg.ct[1],Job_Pwordseg.ct[2])
```
不等长合并的时候，rbind.fill函数可以很好将数据进行合并，并且补齐没有匹配到的缺失值为NA。




```
#如何解决合并时数据不等长问题——两种方法：do.call函数以及rbind.fill函数（plyr包）
#rbind.fill函数只能合并数据框格式
#do.call函数在数据框中执行函数（函数，数据列）
library("plyr")  #加载获取rbind.fill函数
#第一种方法
list1<-list()
list1[[1]]=data.frame(t(data.frame(Job_Pwordseg.ct[1])))
list1[[2]]=data.frame(t(data.frame(Job_Pwordseg.ct[2])))
do.call(rbind.fill,list1)
#第二种方法
u=rbind.fill(data.frame(t(data.frame(Job_Pwordseg.ct[1]))),data.frame(t(data.frame(Job_Pwordseg.ct[2]))))
```
得到了数据之后，数据量小的话，很容易读写，但是数据量大的话，很难做到那么顺畅。当然，为什么要导出呢——因为要传给别的队友。。。


可以有两种办法：

1、批量写出，批量读入；

2、写写成一个data，然后导出，再写入。



```
#读写函数用write.table、read.table+t()
write.table(u,"C:/Users/long/Desktop/3.txt")
t(read.table("C:/Users/long/Desktop/3.txt"))
#分隔符sep=""（空格、制表符、回车）,"\n"（引号）,"\t"（制表符分隔符中有空格）
#字符型字段中，自带空格，则使用参数 strip.white=TRUE

#方法一：单文件导出，后批量导入

#方法二：先合成一个文本导出，再导入
```


每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~![](https://img-blog.csdn.net/20161213101203247)




