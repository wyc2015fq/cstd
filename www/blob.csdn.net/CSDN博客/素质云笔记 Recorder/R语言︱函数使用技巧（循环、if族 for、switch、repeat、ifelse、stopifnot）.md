
# R语言︱函数使用技巧（循环、if族-for、switch、repeat、ifelse、stopifnot） - 素质云笔记-Recorder... - CSDN博客

2016年04月19日 17:09:39[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：27895所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
后续加更内容：

# 应用一：if族有哪些成员呢？——if/ifelse/stopifnot
# 应用二：如何在循环中，实时输出时间消耗？

————————————————————————————————————

1、循环
`##循环for
iris
allzl=unique(iris$setosa)
for (i in 1:2){
  pp=iris[iris$setosa==allzl[i],]
  plot(pp$Sepal.Length~pp$Sepal.Width)
}`
for循环中，需要将数值组合起来，如果数据整齐可以用matrix；如果不整齐，用list，不等长合并的时候，rbind.fill函数可以很好将数据进行合并，并且补齐没有匹配到的缺失值为NA。
可参考：
# R语言︱list用法、批量读取、写出数据时的用法

**案 例**
`temp<-matrix(data = NA,181,31)
for (i in 1:31){
  temp[,i]<-filter(data[i]/7, rep(1, 7))
  }
yatmdata<-data.frame(temp)`代码利用matrix先定义一个181*31的空值矩阵，然后往里面灌数字。


2、switch分支语句
`##switch分支语句
switch(1,mean(1:10),rnorm(4))  #执行mean(1:10)
switch(2,mean(1:10),rnorm(4))  #执行rnorm(4)
#由switch(x)来选择执行那个函数`
3、while循环语句
注意执行顺序，先执行f[i]+f[i+1]<1000，然后往下走，与下面repeat有区别

`##while循环语句
#计算斐波那契数列
f=1
f[2]=1
i=1
while(f[i]+f[i+1]<1000){
  f[i+2]=f[i]+f[i+1]
  i=i+1
}
f
#注意执行顺序，先执行f[i]+f[i+1]<1000，然后往下走，与下面repeat有区别`

4、repeat循环
常常与if联用。

`##repeat语句
#计算斐波那契数列
f=1
f[2]=1
i=1
repeat{
  f[i+2]=f[i]+f[i+1]
  i=i+1
  if (f[i]+f[i+1]>1000) break
};f
#与if常常联用，注意执行顺序，f[i]+f[i+1]>1000，与while<1000不同`
与if常常联用，注意执行顺序，f[i]+f[i+1]>1000，与while<1000不同。

5、if函数+function
if和while都是需要数据TRUE/FALSE这样的逻辑类型变量，这就意味着，if内部，往往是对条件的判别，例如 is.na, is.matrix, is.numeric等等，或者对大小的比较，如，if(x > 0)， if(x == 1)， if(length(x)== 3)等等。
if后面，如果是1行，则花括号可以省略，否则就必须要将所有的语句都放在花括号中。这和循环是一致的
`fun.test <- function(a, b, method = "add"){
    if(method == "add") { ## 如果if或者for/while；
        res <- a + b       ## 等后面的语句只有一行，则无需使用花括号。
}
    if(method == "subtract"){
        res <- a - b
    }
    return(res)           ## 返回值
}
### 检验结果
fun.test(a = 10, b = 8, method = "add")
fun.test(a = 10, b = 8, method = "substract")`
同时if还有类似与excel的用法——**ifelse**
`ifelse(Age > 30, "Old", "Young")`
Age变量>30，则输出old；<30，输出Young

————————————————————————————————————————————————————————————
Function与循环函数结合的实践案例

# 1、函数如何输出？——print、return&list

如果是单个输出，直接用1.3方法即可
如果有很多输出项目，那么需要return（终止运算，并输出return中的项目）最终输出的项目
R中默认的情况是将最后一句作为返回值。

1.1 return&list组合

return和list的组合输出结果比较合理。（来自[R语言︱噪声数据处理、数据分组——分箱法（离散化、等级化）](http://blog.csdn.net/sinat_26917383/article/details/51604682)）
**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/51604682#)[copy](http://blog.csdn.net/sinat_26917383/article/details/51604682#)
[print](http://blog.csdn.net/sinat_26917383/article/details/51604682#)[?](http://blog.csdn.net/sinat_26917383/article/details/51604682#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
sbdeep=function(data,parts,xiaoz){
parts<-parts\#分几个箱
xiaoz<-xiaoz\#极小值
value<-quantile(data,probs=seq(0,1,1/parts))  \#这里以data等比分为4段，步长为1/4
number<-mapply(function(x){
for (i in 1:(parts-1))
{
if(x>=(value[i]-xiaoz)&x<value[i+1])
{
return(i)
}
}
if(x+xiaoz>value[parts])
{
return(parts)
}
return(-1)
},data)
\#打标签L1L2L3L4
return(list(degree=paste("L",number,sep=""),degreevalue=number,value=table(value),number=table(number)))               \#将连续变量转化成定序变量，此时为L1,L2,L3,L4...根据parts
}
该函数是对单个序列数据进行等深分箱，可以返回四类：
一个基于L1L2L3....的每个指标标签序列degree；
标签序列值degreevalue，
每个百分位数对应的变量值value，
不同百分点的数量number。

1.2 print直接输出

`function(){
    print(plot(cv.out)) 
  }`
print可以直接输出.

1.3 直接输出——一一般都是直接输出
`function(){
a=c(1:50)
a
}`其中a就是直接写在末尾，当做输出项。


# 2、function中应用if switch函数

`test=function(mode=c("all", "out", "in")){
  mode <- switch(mode, out = 1, `in` = 2, all = 3)
 
  if (as.numeric(mode)==1) {
    t=1
  }
 
  if (as.numeric(mode)==2) {
    t=2
  }
 
  if (as.numeric(mode)==3) {
    t=3
  }
  t=t+1
  return(t+4)
}
a=test(mode="out")
test(mode="in")
test(mode="all")`解决场景：编写函数时候，可能嵌套很多模型的时候，就需要用这个流程。
switch函数，输入mode，执行相应的内容，此时是mode选择“all”，则执行返回1,；mode选择"out"则返回2；
然后用if去进行每个数字背后的建模，注意“==”
"in"注意要引号，因为会跟内嵌函数重叠

# 3 异常值处理——如何报错
`# 异常处理，当仅输入一个数据的时候，告知不能计算标准差
   if(length(x) == 1){                          
      stop("can not compute sd for one number,
           a numeric vector required.\n")       
   }`
————————————————————————————————————
# 应用一：if族有哪些成员呢？——if/ifelse/stopifnot

在函数中，if的应用场景非常多，用来识别某类情况前提下，再执行下一个。
其中笔者就见过这样三类if：if-else   ifelse   stopifnot

## 1、if-else

这个很常见，就是需要注意一下，if-else的写法，来看经管之家论坛一位坛友的提醒与使用心得：
if(){}else{}  表示先执行if括号后面的条件语句，如果正确就执行第一个大括号里的程序，如果错误就执行else后面大括号里的语句。
有一种情况，r会报错：
`if(){}
else{}`
就是这种情况，即else语句换了一行执行时，这是r会认为if语句已经执行完毕，但执行else发现前面无法执行，因此报错，在这里要提醒使用r的同志们，else必须紧挨着if语句后的大括号，这时才不会出错。


## 2、ifelse

跟If-else其实是一模一样的，但是效率提高很多，是提高代码运算效率很高的函数。ifelse()的句法格式类似于if()函数，但其运算速度却有了巨大的提升。即使是在没有预设数据结构且没有简化条件语句的情况下，其运算效率仍高于上述的两种方法。
`ifelse(test, yes, no)`
ifelse返回的是结果，有一点麻烦的是，不像if-else一样，可以写一些分布计算的东西，譬如现在有以下一种情况：
`a<-c+d
sum(a>2)  #在c大于2的情况下，要计算a大于2的个数`
这个分步情况在if-else里面很好解决，但是在ifelse里面可不容易，只能接受一步，所以尽量把运算链合并在一起。

## 3、stopifnot

这个函数跟Ifelse有点像，但是很奇特。stopifnot(c>2)，如果正确执行，那么就会啥都没发生，如果错误了，就会跳入Debug模式，报错，让函数立刻停下来。
这个stopifnot跟trycatch函数联合使用，威力无比。

用tryCatch跳过：
**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52719232?locationNum=2&fps=1#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52719232?locationNum=2&fps=1#)
[print](http://blog.csdn.net/sinat_26917383/article/details/52719232?locationNum=2&fps=1#)[?](http://blog.csdn.net/sinat_26917383/article/details/52719232?locationNum=2&fps=1#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
result=tryCatch(
{expr},
warning=function(w) {warning-handler-code},
error=function(e) { error-handler-code},
finally= {cleanup-code}
)
出现warning、error时候怎么处理，就可以跳过了。例子：
**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/52719232?locationNum=2&fps=1#)[copy](http://blog.csdn.net/sinat_26917383/article/details/52719232?locationNum=2&fps=1#)
[print](http://blog.csdn.net/sinat_26917383/article/details/52719232?locationNum=2&fps=1#)[?](http://blog.csdn.net/sinat_26917383/article/details/52719232?locationNum=2&fps=1#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
result=tryCatch(
{segmentCN(txt)},
warning=function(w) {"出警告啦"},
error=function(e) { "出错啦"},
)
分词时候，容易因为Lapply中断之后，就不会运行了，这样功亏一篑所以可以用这个办法跳过。

————————————————————————————————————
# 应用二：如何在循环中，实时输出时间消耗？

想知道循环中进行到哪里？这样可以合理安排函数进程。那么怎么办呢？

**第一办法：**使用Rstudio 1.0版本，里面有一个Profiling with profvis，可以很好的对你函数每一步的耗时进行参看。

# [R︱Rstudio 1.0版本尝鲜（R notebook、下载链接、sparkR、代码时间测试profile）](http://blog.csdn.net/sinat_26917383/article/details/53007630)

当然，这个不能实时输出内容。

**第二办法：**利用difftime函数
**[html]**[view plain](http://blog.csdn.net/sinat_26917383/article/details/50677065#)[copy](http://blog.csdn.net/sinat_26917383/article/details/50677065#)
[print](http://blog.csdn.net/sinat_26917383/article/details/50677065#)[?](http://blog.csdn.net/sinat_26917383/article/details/50677065#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
t1=Sys.time()
for (i in 1:5){
a=a+1
b=a*a
print(difftime(Sys.time(), t1,units='sec'))
}
先预设当前时间，然后用difftime+print方式，循环输出。
本节内容来源于另一篇博客：[R语言与格式、日期格式、格式转化](http://blog.csdn.net/sinat_26917383/article/details/50677065)

——————————————————————————————————————————————————————————————

## 应用三：
## [assign 和get 函数用法](http://blog.csdn.net/u011402596/article/details/42924561)
循环中使用的神器。

# assign函数在循环时候，给变量赋值，算是比较方便
# 通过for循环给变量a1、a2、a3赋值

```python
for (i in 1:3){  
    assign(paste("a", i, sep = ""), i:10)  
}  
  
ls()  
[1] "a1" "a2" "a3" "i"  
  
> a1  
 [1]  1  2  3  4  5  6  7  8  9 10  
> a2  
[1]  2  3  4  5  6  7  8  9 10
```

get的用法是通过字符串引用函数。
```python
a = c(1,2)
get("a")
>>> 1,2
```

## get和assign联合用法
```python
for(i in 1:3){
     assign(paste("p", i, sep=""), i)
     tmp <- get(paste("p", i, sep=""))
     print(tmp)
 }
[1] 1
[1] 2
[1] 3
```


**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


