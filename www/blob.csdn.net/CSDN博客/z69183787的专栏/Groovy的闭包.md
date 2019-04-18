# Groovy的闭包 - z69183787的专栏 - CSDN博客
2015年01月05日 19:14:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：669
个人分类：[Groovy-概述](https://blog.csdn.net/z69183787/article/category/2816515)

一个简单的例子
[1,2,3].each{entry -> print entry}
each是循环执行的意思，它是List.each方法需要一个闭包参数
{}是闭包的部分
entry是参数部分（当然可以是命名为你喜欢的任何非关键字 如 a,e,people等等）
->右边是要执行的代码块
def sum=0
1.upto(100){arg->sum+=arg-1}
print sum
result:4950
upto方法是integer的，上面的表示了从1到100每次都执行了闭包 和each针对list意思一样
groovy中
for（i in x）{body}
这和java中的加强型for循环一样for(i:x){}
在groovy中x可以是任意对象 groovy知道怎么迭代它。
但是在groovy中for经常会被有闭包作为参数的方法所取代，像each,upto等
在条件语句中null被处理成为false。
在groovy中被广泛使用的size方法，映射到java世界，对应的就是各个java类的length，size之类的方法。
---------------------闭包的声明------------------
def c={x,y->return x+y}
Closure c={x,y->return x+y}
Closure c=XXObj .&  XXmethod
----------------闭包的调用--------------
c(1,2)
c.call(1,2)
-------------声明一个带闭包的参数的方法-----------
闭包是作为参数列表的最后一个
def myOperation(x,y,Closure c){
xxxxxxx
c(it)
        return xxx
}
slow=myOperation(1,3){it/2}
fast=myOperation(2.9){it.intdiv(2)}    //闭包的实现放在了后面
-----------闭包的参数 缺省值的使用----------
def c={x,y=5->return x+y}
assert c.call(1,5)==6
assert c(1)==6
----------groovy.lang.Closure上的一些额外方法-------
Closure c
c.getParameterTypes().size
得到闭包有几个参数传入
----------使用闭包实现curry-----------
//curry指的是 一个多参数的函数，我们可以事先固定他的一些参数，然后再传递未固定的参数，调用函数。
//由闭包实现curry是 调用了闭包的 .curry()方法固定一些参数，固定是从左到右的。其实是生成了新的 闭包。
def c={x,y,z->return x+y+z}
def c2=c.curry(1,2)
assert c2 instanceof Closure
assert c2(3)==6
assert c2(2)==5
curry的强大之处在于 当闭包的参数是闭包本身时，通过匹配不同的参数，得到各式各样的不同的模板，版本。
--------使用groovy实现累加测试-------------
def foo(n){
    return {n+=it}
}
def c=foo(1)
assert c(2)==3
assert c(1)==4
assert c(1)==5
//不容易理解是吧，关键点是这个闭包内部的n是在闭包内部声明的，所以闭包不灭n就一直存在。调用同一个闭包，那当然n就一直在累加。
---------闭包的返回-----------
默认最后一个表达式
或者return显示返回
