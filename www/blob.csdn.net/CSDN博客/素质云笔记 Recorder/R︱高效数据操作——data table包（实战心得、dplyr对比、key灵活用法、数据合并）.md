# R︱高效数据操作——data.table包（实战心得、dplyr对比、key灵活用法、数据合并） - 素质云笔记/Recorder... - CSDN博客





2016年10月24日 17:27:55[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：19294
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)











**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————





      由于业务中接触的数据量很大，于是不得不转战开始寻求数据操作的效率。于是，data.table这个包就可以很好的满足对大数据量的数据操作的需求。



       data.table可是比dplyr以及[Python](http://lib.csdn.net/base/11)中的pandas还好用的数据处理方式。

      网络上充斥的是data.table很好，很棒，性能棒之类的，但是从我实际使用来看，就得泼个水，网上博客都是拿一些简单的案例数据，但是实际数据结构很复杂的情况下，批量操作对于data.table编码来说，会显得很繁琐，相比来说，让我多等1分钟的data.frame结构，我还是愿意等的。

      data.table包的语法简洁，并且只需一行代码就可以完成很多事情。进一步地，data.table在某些情况下执行效率更高。（参考来源：[R高效数据处理包dplyr和data.table，你选哪个？](http://www.xueqing.tv/cms/article/213)）


![](https://img-blog.csdn.net/20161024121824641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在使用data.table时候，需要预先布置一下环境：





**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/50688431#)[copy](http://blog.csdn.net/sinat_26917383/article/details/50688431#)

[print](http://blog.csdn.net/sinat_26917383/article/details/50688431#)[?](http://blog.csdn.net/sinat_26917383/article/details/50688431#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- data<-data.table(data)  


如果不布置环境，很多内容用不了。或者通过as.data.table的操作来构建数据集。
一些老式的数据操作方法可以见我的其他博客：

# [R语言︱数据集分组、筛选(plit – apply – combine模式、dplyr、data.table)](http://blog.csdn.net/sinat_26917383/article/details/50688431)




同时，data.table与data.frame数据呈现方面，还有有所不同的。

data.table包提供了一个非常简洁的通用格式：DT[i,j,by]，可以理解为：对于数据集DT，选取子集行i,通过by分组计算j。


注意:

data.table之后，一些常规的data.frame的操作就失效了，譬如：

data[,-1]、data[,1]这样的操作就不是这么用的了。




——————————————————————————————————————————————




# 一、重要的key()




data.table中的key是整个框架里面的灵魂，通过设置，**data.table整个数据就会按照key变量重新排序**。这意味着，输出的数据也是按照某种规律的，而且通过设置了Key,配套的代码编译也受到了简化。

设置key的方式有很多种：




## 1、创建数据时直接设置key





`dt <- data.table(a=c('A','B','C','A','A','B'),b=rnorm(6),key="a")`

## 2、setkey()函数设置


`setkey(dt,a)`
setkey(DT, colA, colB)，可以使得检索和分组更加快速。同时设置两个key变量的方式，也是可以的。



查看数据集是否有key的方式：



```
key(data)    #检查该数据集key是什么？
haskey(data) #检查是否有Key
attributes(data)
```

key()可以告诉你，数据集中的Key是哪几个变量？ 


haskey()输出结果为:true/false




——————————————————————————————————————————————





# 二、数据筛选




## 1、列筛选 .()






```
from_dplyr = select(tb, ID)   
from_data_table = tb[,"ID"]
from_data_table = tb[,.(ID)]
```

三种数据筛选的方式，dplyr包、base基础包、data.table包。其中，dplyr是select语句，data.table中要注意.()的表达方式。


而且，.()格式只在data.table格式下有效，不然会报错。data.table中，还有一个比较特立独行的函数：

使用:=引用来添加或更新一列（参考：[R语言data.table速查手册](http://blog.csdn.net/a358463121/article/details/51910062)）



```
DT[, c("V1","V2") := list(round(exp(V1),2), LETTERS[4:6])]
DT[, c("V1","V2") := NULL]
```



通过list的方式来更新了数据，以及使用null的方式来删除列。




## 2、按条件行筛选




从前用subset的方式进行筛选比较多，


`new<-subset(x,a>=14,select=a:f)`



### （1）单变量

现在data.table与dplyr



```
from_dplyr = filter(hospital_spending,State %in% c('CA','MA',"TX")) 
from_data_table = hospital_spending_DT[State %in% c('CA','MA',"TX")]
```



dplyr用filter，content满足某种条件的进行筛选，而data.table的筛选方式很传统，比较简单。筛选hospital_spending_DT数据集中，State变量，满足"CA"、“MA”、"TX"内容的行。

在筛选列变量的数据，也可以与%in%集合运算联用（集合运算见博客：[R语言︱集合运算](http://blog.csdn.net/sinat_26917383/article/details/51277581)）。





### （2）多变量筛选，用&|等



```
from_dplyr = filter(tb,State=='CA' & Claim.Type!="Hospice") 
from_data_table = hospital_spending_DT[State=='CA' & Claim.Type!="Hospice"]
```


### （3）还有一些复杂结构：


`dt[a=='B' & c2>3, b:=100]   #其他结构`
在dt数据集中，筛选a变量等于"B"，c2变量大于3，同时将添加b变量，数值等于100.





### （4）★key的改造

通过预先设置key，然后再来进行筛选的方法，更加高效，而且节省时间。来看看例子：



```
ans1 <- try[try$gender=="M" & try$buy_online=="Y",]
#使用”==”操作符，那么它会扫描整个数组，虽然data.table用这种方法也可以提取，但很慢，要尽量避免。

setkey(try,gender,buy_online)  #设置key为两个变量，数据已经按照x值进行了重新排序
ans2 <- DT[list("M","Y")]      #更为简洁，并且迅速
```
**这里有一个重要的点:使用”==”操作符，那么它会扫描整个数组，虽然data.table用这种方法也可以提取，但很慢，要尽量避免。**
这里先设置key，然后直接通过list(M,Y)就可以达到第一条代码的效能，而且时间更短。





——————————————————————————————————————————————





# 三、数据排序


有了key,其实有了一定排序功能在里面。


```
from_dplyr = arrange(hospital_spending, State)
from_data_table = setorder(hospital_spending_DT, State)

from_dplyr = arrange(hospital_spending, desc(State))
from_data_table = setorder(hospital_spending_DT, -State)
```
dplyr中是arrange函数，而data.table是setorder函数，同时降序的方式。


dplyr的降序是，arrange(data,desc(x))，而data.table的降序是setorder(data,-x)

——————————————————————————————————————————————



# 四、分组求和、求平均






```
mygroup= group_by(try,gender,buy_online)
from_dplyr<-summarize(mygroup,mean=mean(new_car))                          #dplyr用两步    

from_data_table<-try[,.(mean=mean(new_car)),by=.(gender,buy_online)]       #data.table用一步
```
dplyr：先用group_by设置分组，然后利用summarize求平均，mean=mean()；


data.table，在try数据集中，通过by=.(x,y)来分组，而且可以设定x/y两种分组，来求new_car的平均值。



## （1）data.table多种方式混合输出：






```
mydata[,.(sum(Ozone,na.rm=T),sd(Ozone,na.rm=T))]                           #求和、求标准差操作
DT[,list(MySum=sum(v),
         MyMin=min(v),
         MyMax=max(v)),
   by=.(x)]                                                                #多种方式联合
```
多种方式混合，而且代码编译上也会有很多不同之处。DT数据集按照x分组，然后计算v变量的和、最小值、最大值。




## （2）dplyr函数利用%>%（链式操作）来改进：




链式操作是啥意思呢？

%>%的功能是用于实现将一个函数的输出传递给下一个函数的第一个参数。注意这里的，传递给下一个函数的第一个参数，然后就不用写第一个参数了。在dplyr分组求和的过程中，还是挺有用的。



`from_dplyr=try %>% group_by(gender,buy_online) %>% summarize(mean=mean(new_car))`





——————————————————————————————————————————————





# 五、数据合并




最常见的合并函数就是merge，还有sql的方式（常见的合并方式可见：

# [R语言数据集合并、数据增减、不等长合并](http://blog.csdn.net/sinat_26917383/article/details/50676894)
）。


在data.table中有三类数据合并的方式：




## 1、直接用[]





`data_one[data_two,nomatch=NA,mult="all"]`
以第一个数据为基准，依据key进行合并，只出现重复部分（data_one数据必须设置key，data_two默认第一行为Key）。很简洁的方式，举例：




```
DT = data.table(x=rep(c("a","b","c"),each=3), y=c(1,3,6), v=1:9)
X = data.table(c("b","c"),foo=c(4,2))
#以DT为基准
setkey(DT,x)
DT[X] 
#以X数据集为基准
setkey(X,V1)
X[DT]
```

现在有DT、X两个数据集，先设置DT数据集的key，然后DT[X]来合并，后者相同。还有nomatch的设置可以见第六小节。


nomatch用来设置未匹配到的数据如何处理，nomatch=0则认为未匹配到的删除。

melt用来设置是否都显示匹配内容。




## 2、on=""方式


`DT[X, on="x"]`
这里的on指的是DT变量中的变量名称，X还是按照key，如果没设置就会默认第一行为key。





## 3、第三种方式：key-merge



```
setkey(DT,x)
setkey(X,V1)
merge(DT, X)
```

预先设置两个数据集的key后，也可以用比较常见的merge函数来进行数据合并。





——————————————————————————————————————————————





# 六、额外的参数（来源：[R语言data.table速查手册](http://blog.csdn.net/a358463121/article/details/51910062)）



## 1、mult参数



mult参数是用来控制i匹配到的哪一行的返回结果默认情况下会返回该分组的所有元素 
返回匹配到键值所在列(V2列)所有行中的第一行

```
> DT["A", mult ="first"]
   V1 V2 V3 V4
1: 1 A -1.1727 1
```


## 2、nomatch参数——未匹配样本处理




nomatch参数用于控制，当在i中没有到匹配数据的返回结果，默认为NA，也能设定为0。0意味着对于没有匹配到的行将不会返回。 
返回匹配到键值所在列(V2列)所有包含变量值A或D的所有行：



```
DT[c("A","D"), nomatch = 0]
   V1 V2    V3 V4
1: 1 A -1.1727 1
2: 2 A  0.6651 4
3: 1 A -1.0604 7
4: 2 A -0.3825 10
```

nomatch=0对于没有匹配到的将不显示。跟merge中的all差不多。





## 3、.SD和.SDcols






```
> DT[, lapply(.SD,sum), by=V2,
+    .SDcols = c("V3","V4")]
   V2      V3 V4
1:  A -1.2727 22
2:  B -1.2727 26
3:  C -1.2727 30
```
.SD是一个data.table，他包含了各个分组，除了by中的变量的所有元素。.SD只能在位置j中使用。
.SDcols常于.SD用在一起，他可以指定.SD中所包含的列，也就是对.SD取子集。






## 4、修改列名、行名






```
#把名字为"old"的列，设置为"new"
> setnames(DT,"old","new") 
#把"V2","V3"列，设置为"V2.rating","V3.DataCamp"
> setnames(DT,c("V2","V3"),c("V2.rating","V3.DataCamp"))
```


## 5、setcolorder()





```
#setcolorder()可以用来修改列的顺序。
setcolorder(DT,c("V2","V1","V4","V3")) 
#这段代码会使得列的顺序变成：
 "V2" "V1" "V4" "V3"
```




——————————————————————————————————————————————


# 实战一：在data.table如何选中列，如何循环提取、操作data.table中的列？

      在data.table行操作跟data.frame很像，可以data[1,]就可以获得第一行的数据，同时也可以用，data[1]来获得行信息，这个是data.table特有的。

**除了行，就是列的问题了。在data.table操作列，真的是费劲。。。**

      常规来看，

data[，.(x)]   还有 data$x


      如果有很多名字很长的指标，data.table中如果按列进行遍历呢？

**data[,1]是不行的，选中列的方式是用列名。**于是只能借助get+names的组合。



```
for (i in 1:5){
data[,.(get(names(data)[i]))]
}
```
       这个是我目前在遍历时候用到的代码，不知道观众有没有更好的呢？







**2016-11-28补充：**



      留言区大神给了一个比较好的选中列的方式，其中主要就是对with的使用：


`data.table取列时，可以用data[,1,with=FALSE]取data的第一列，相对于对数据框的操作`
      这样就可以像普通的数据框一样使用，谢谢留言区大神！！！！








# 参考文献：

些许案例，代码参考自以下博客，感谢你们的辛勤：

1、[R语言data.table简介](http://blog.csdn.net/a358463121/article/details/51483713)


2、[超高性能数据处理包data.table](http://blog.fens.me/r-data-table/?utm_source=tuicool&utm_medium=referral)

3、[R语言data.table速查手册](http://blog.csdn.net/a358463121/article/details/51910062)

4、R高效数据处理包dplyr和data.table，你选哪个？



