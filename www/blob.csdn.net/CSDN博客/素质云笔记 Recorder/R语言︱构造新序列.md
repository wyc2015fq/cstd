
# R语言︱构造新序列 - 素质云笔记-Recorder... - CSDN博客

2016年04月18日 20:01:14[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1669所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)



1、数值构造函数rep与seq
`#数值构造rep与seq
rep(1:4,each=2)#依次重复1:4两遍
rep(1:4,2)  #注意，重复1:4两遍
seq(from=3,to=5,by=0.2)

rep(seq(from=3,to=5,by=0.2),2) #混合使用`rep在使用过程中也很灵活，each代表AABB；默认的为ABAB。
`> rep(c("id","use"),list(2,3))
[1] "id"  "id"  "use" "use" "use"
> rep(c("id","use"),each=2)
[1] "id"  "id"  "use" "use"
> rep(c("id","use"),2)
[1] "id"  "use" "id"  "use"
> rep(c("id","use"),unlist(2,3))
[1] "id"  "use" "id"  "use"`
**rep与list相结合**
`> rep(c("id","use"),list(2,3))
[1] "id"  "id"  "use" "use" "use"`可以实现AABBB，与each相似。在构造一些序列时候十分好用。

2、矩阵构造

`#构造矩阵
matrix(1:15,nrow=3,ncol=5,byrow=T)  #byrow按行的顺序（横向）赋值；bycol按列（竖）赋值
array(1:15,dim=c(3,5)) #只能按列（竖）赋值`

3、字符构造paste
seq代表是ck与数值1 之间用啥记号，如：ck_1,ck*2
collapse代表是ck1与ck2之间用啥记号如：ck1_ck2   ck1 * ck2

`#字符构造paste

paste("CK", 1:6, sep="")      
paste("CK", 1:6, sep="*")         #对比

paste("CK", 1:6,collapse ="") 
paste("CK", 1:6,collapse = "_")   #对比
#seq代表是ck与数值1 之间用啥记号，如：ck_1,ck*2
#collapse代表是ck1与ck2之间用啥记号如：ck1_ck2   ck1 * ck2`
4、paste与list合用——批量处理
list能够很好与paste函数应用起来


`#paste与list合用
x <- list(a="aaa", b="bbb", c="ccc") 
y <- list(d=1, e=2) 
z=paste(x, y, sep="-")
paste("T", z, sep=":") 
#list能够很好与paste函数应用起来`
更多字符串处理见：http://blog.csdn.net/sinat_26917383/article/details/51122043

两者在批量处理之中的运用，可见博客:R语言︱list用法、批量读取、写出数据时的用法
`#3、利用List批量读出操作
#难点：如果构造输出表格的名称——paste来构造名称
flie=list()
xlsxflie=paste(1:2,".xlsx",sep="")
for(i in 1:2){
flie[[i]]=paste("C:/Users/long/Desktop/",xlsxflie[i],sep="")
write.xlsx(data.list2[[i]],file)
}`其中，代码可以实现，数据写出时候，批量按照一定格式进行命名。


