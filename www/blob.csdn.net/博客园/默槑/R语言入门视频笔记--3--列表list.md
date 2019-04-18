# R语言入门视频笔记--3--列表list - 默槑 - 博客园







# [R语言入门视频笔记--3--列表list](https://www.cnblogs.com/modaidai/p/6653381.html)






list <- (stud.id = 1234,stud.name="Tom",stud,marks=c(18,3,14,25,19))  #生成一个列表，里面有学生id，学生姓名，学生的成绩mylist[[1]]                        #输出：1234

mylist[[3]]　　　　　　　　#输出：12 3 14 19

mylist[[3]][1:2] 　　　　 #输出第三个列表的前两个值

mylist[1]　　　　　　　　　#输出 $stud.id  [1] 1234　会连名称一起输出

mylist$stud.id                    #也可以输出1234  也可以使用这个的输出stud.id这个列表里面的内容

mylist$stud.id[1:3]　　　　　　　　　#输出这列表的前三个值

mode(mylist$stud.id)　　　　　　　　#输出list

mode(mylist[[1]])　　　　　　　　　　#输出numeric  你懂得  他和她不是一路人

name(mylist)   　　　　　　#输出mylist各成分的名称

name(mylist) <- c("id","name","marks")　　#更改mylist各成分的名称

mylist$parents <- c("Mna","Toms")　　　　　#给列表增加成分，这就Tom有爹妈了，不信你试试

length(mylist)　　　　　　　　　　　　　　　　#看这个列表有多少成分，现在是4个

mylist <- mylist[-4]　　　　　　　　　　　　　　#删除第四个成分，Tom又是孤儿了

othernewlist <- c(mylist,other)　　　　
other <- list(age=19,sex="male")　　　　
##   新建一个列表，然后跟前面的那个进行合并，可以还是使用list进行合并
##   Tom没有了爹妈，蛋是有了年龄和性别，能是个人就知足了，别无他求。




也就是说：列表名称+[[n]]   会得到这个列表的第n号成分中的的内容

二、新函数上来啦：unlist




```
`unlist``(newlist)　　　　　　　　　　　　　　　　　　　``#把所有列表中的成分名称还有每一个元素显示出来 变成一个向量输出出来　　`
```



















