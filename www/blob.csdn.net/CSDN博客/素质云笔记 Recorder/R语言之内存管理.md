# R语言之内存管理 - 素质云笔记/Recorder... - CSDN博客





2016年02月11日 16:33:10[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2102











转载于：http://blog.csdn.net/hubifeng/article/details/41113789




在处理大型数据过程中，R语言的内存管理就显得十分重要，以下介绍几种常用的处理方法。
**1，设置软件的内存**




**[plain]**[view
 plain](http://blog.csdn.net/hubifeng/article/details/41113789#)[copy](http://blog.csdn.net/hubifeng/article/details/41113789#)



- <span style="font-size:14px;">memory.size(2048) #设置内存大小  
- memory.size(NA) #查看当前设置下最大内存(?"Memory-limits")  
- #or memory.limit()  
- memory.size(F) #查看当前已使用的内存  
- #or  
- library(pryr)  
- mem_used()  
- 
- mem_change(x <- 1:1e6) #查看执行命令时内存的变化  
- memory.size(T)  #查看已分配的内存</span>  

    注意刚开始时已使用内存和已分配内存是同步增加的，但是随着R中的垃圾被清理，已使用内存会减少，而已分配给R的内存一般不会改变。

**2，对象的存储**

    R中的对象在内存中存于两种不同的地方，一种是堆内存（heap），其基本单元是“Vcells”，每个大小为8字节，新来一个对象就会申请一块空间，把值全部存在这里，和C里面的堆内存很像。第二种是地址对（cons cells），和LISP里的cons cells道理一样，主要用来存储地址信息，最小单元一般在32位系统中是28字节、64位系统中是56字节。







**[plain]**[view
 plain](http://blog.csdn.net/hubifeng/article/details/41113789#)[copy](http://blog.csdn.net/hubifeng/article/details/41113789#)



- <span style="font-size:14px;">ls()           #查看当前对象  
- object.size()    查看对象所占内存  
- #or  
- library(pryr)  
- object_size()  #区别于前者，它进行了换算</span>  



    1)新建对象分配合适的内存

    R会将新的对象存储在“连续”的内存中，如果没有这样的空间就会返回“Cannot allocate vector of size...” 的错误，有以下几种处理方法:

        a) 如果有多个矩阵需要存储，确保优先存储较大的矩阵，然后依次存储较小的矩阵.

        b) 预先分配合适的内存.

            大家都知道R中矩阵的维度并不需要赋一个固定的值（很多语言的数组长度不能为变量），这为写程序带来了极大的方便，因此经常在循环中会出现某个矩阵越来越长的情况，实际上，矩阵每增长一次，即使赋给同名的变量，都需要新开辟一块更大的空间，假设初始矩阵为100K，第二个为101K，一直增到120K，那么，将会分别开辟100K、101K一直到120K的连续堆内存，如果一开始就开一块120K的，使之从101K逐渐增长到120K，将会大大地节约内存。cbind函数也是这个道理，所以在循环中要注意不要滥用。 

        c) 换到64位的计算机，这种问题较少出现.


    2)改变当前对象的存储模式

    例如某个矩阵默认就是"double"的，如果这个矩阵的数值都是整数甚至0-1，完全没必要使用double来占用空间，可以将其改为整数型，可以看到该对象的大小会变为原来的一半。 



**[plain]**[view
 plain](http://blog.csdn.net/hubifeng/article/details/41113789#)[copy](http://blog.csdn.net/hubifeng/article/details/41113789#)



- <span style="font-size:14px;">storage.mode(x)    #查看对象的存储模式  
- storage.mode(x) <- "integer"  #整数型存储模式</span>  



    3)清理中间对象



**[plain]**[view
 plain](http://blog.csdn.net/hubifeng/article/details/41113789#)[copy](http://blog.csdn.net/hubifeng/article/details/41113789#)



- <span style="font-size:14px;">rm() #删除变量的引用，经常用它来清理中间对象，其中比较重要的文件可以存在硬盘里，比如csv文件或者RSqlite等  
- gc() #清理内存空间</span>  



    4)清理其他对象

.ls.objects() #查看内存消耗较大的文件，并处理掉其他无关对象.代码如下:



**[plain]**[view
 plain](http://blog.csdn.net/hubifeng/article/details/41113789#)[copy](http://blog.csdn.net/hubifeng/article/details/41113789#)



- <span style="font-size:14px;">.ls.objects <- function (pos = 1, pattern, order.by = "Size", decreasing=TRUE, head = TRUE, n = 10) {  
-   napply <- function(names, fn) sapply(names, function(x)  
-           fn(get(x, pos = pos)))  
-   names <- ls(pos = pos, pattern = pattern)  
-   obj.class <- napply(names, function(x) as.character(class(x))[1])  
-   obj.mode <- napply(names, mode)  
-   obj.type <- ifelse(is.na(obj.class), obj.mode, obj.class)  
-   obj.size <- napply(names, object.size) / 10^6 # megabytes  
-   obj.dim <- t(napply(names, function(x)  
-             as.numeric(dim(x))[1:2]))  
-   vec <- is.na(obj.dim)[, 1] & (obj.type != "function")  
-   obj.dim[vec, 1] <- napply(names, length)[vec]  
-   out <- data.frame(obj.type, obj.size, obj.dim)  
-   names(out) <- c("Type", "Size", "Rows", "Columns")  
-   out <- out[order(out[[order.by]], decreasing=decreasing), ]  
-   if (head)  
-     out <- head(out, n)  
-   out  
- }</span>  


**3，修改存储地址**

    这部分可参考文献1。在xp系统上试了一下，得到的存储地址总是不变，不知道xp系统上有没有效...

**4，选取数据集的子集**

    这是没有办法的办法，迟早要处理全部的数据，不过可以借此调试代码或是建模，如在合适的地方清理中间对象

**5，写成脚本文件**

    Hadley Wickham 建议写成脚本文件，运行后再清理掉临时文件

**6，使用SOAR包**

    它可以将特定对象存储为RData文件并无需加载到内存就能进行分析



**[plain]**[view
 plain](http://blog.csdn.net/hubifeng/article/details/41113789#)[copy](http://blog.csdn.net/hubifeng/article/details/41113789#)



- <span style="font-size:14px;">r = data.frame(a=rnorm(10,2,.5),b=rnorm(10,3,.5))  
- library(SOAR)  
- Sys.setenv(R_LOCAL_CACHE=”testsession”)  
- ls()  
- Store(r)  
- ls()  
- mean(r[,1])  
- r$c = rnorm(10,4,.5)  
- ls()</span>  


**7，一个有趣的函数**

    它会告诉你哪一行的代码消耗了多少时间、内存，释放多少内存，复制了多少向量.



**[plain]**[view
 plain](http://blog.csdn.net/hubifeng/article/details/41113789#)[copy](http://blog.csdn.net/hubifeng/article/details/41113789#)



- <span style="font-size:14px;">library(devtools)  
- devtools::install_github("hadley/lineprof")  
- library(lineprof)  
- source("D:/myprojects/project1/read-delim.R")  
- prof <- lineprof(read_delim("D:/myprojects/project1/diamonds.csv"))  
- shine(prof)</span>  










Reference:

1.[点击打开链接](http://adv-r.had.co.nz/memory.html)

2.[点击打开链接](http://www.matthewckeller.com/html/memory.html)

3.[点击打开链接](http://www.r-bloggers.com/memory-management-in-r-a-few-tips-and-tricks/)

4.[点击打开链接](http://www.r-bloggers.com/memory-management-in-r-and-soar/)



More Reference:
[点击打开链接](http://stackoverflow.com/questions/1358003/tricks-to-manage-the-available-memory-in-an-r-session)









