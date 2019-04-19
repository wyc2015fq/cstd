# Golang的方法集继承规则示例 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [v7](http://www.jobbole.com/members/vincent7) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9o8kmz4y5j20go09oab3.jpg)本篇通过具体示例来展现 Golang 机制中 methods set 的继承规则。
## 0x00 简单method的规则
首先，我们声明示例的基本操作结构体。

Go
```
type Cat struct{
    Name string
    Color string
}
```
我们声明了一个 Cat 的 type，在它的基础上我们声明一个Meow的method:

Go
```
func (c Cat) Meow() {
    fmt.Println("Name:", c.Name, "Color:", c.Color)
}
```
在上面的代码上， 我们声明了一个method， 它的receiver是Cat（区别于Cat的指针），在这时，无论是我们通过 Cat类型的变量来调用method还是用 *Cat的类型来调用，Go都会正常的编译并正常输出，如下图：
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9o8knb0bzj20go0g2jsv.jpg)
如图，main函数中的 a 和 b 变量，一个是 Cat 类型，一个是Cat的指针类型，他们都有Meow方法。
那么如果我们声明的函数receiver是 *Cat呢？


```
func (c *Cat) Meow() {
    fmt.Println("Name:", c.Name, "Color:", c.Color)
}
```
在这种情况下，方法也是继承的：
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9o8kn98m3j20go0ffgmz.jpg)
上面是简单的 Cat 和 *Cat 上的方法集的继承规则：
> 
普通情况下，类型 T 和 *T 上的方法集是互相继承的。
## 0x01 接口中methods set的规则
接下来我们声明一个接口来做对比：


```
type Meower interface{
    Meow()
}
func Greet(meower Meower){
    meower.Meow()
}
```
首先我们在Cat类型上实现Meower接口：


```
func (c Cat) Meow(){
    fmt.Println("Name:", c.Name, " Color:", c.Color)
}
```
此时编译是没问题的
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9o8knuzdmj20go0fgwg9.jpg)
也就是说，如果我们给Cat类型实现Meower的接口上Meow的方法，无论是Cat还是*Cat都是可以成为接口调用的。
那如果我们实现接口方式时候，选择的receiver是*Cat呢？


```
func (c *Cat) Meow(){
    fmt.Println("Name:", c.Name, " Color:", c.Color)
}
```
编译不通过，如下图：
![](http://ww2.sinaimg.cn/mw690/63918611gw1f9o8kock4kj20go0d6dho.jpg)
如果给指针实现Meow方法，在第26行出现了类型错误，提示我们Meow方法的receiver是一个指针类型，说明此时方法不能继承。
于是这里的规则是：
> 
在接口中的method，对于普通类型T：
T的methods set里不会继承包含*T实现的method，除非T自己实现相对应的method。
但是，*T会继承T的method set。
## 0x02 嵌入类型中methods set的规则
我们讨论了上面两种情况，那考虑如果Cat作为**嵌入类型**（**Embedded Types**）时会发生什么呢？
于是在上面的基础上，我们来声明这样一个类型来做实验：


```
type BlackCat struct{
    Cat
    Age int
}
```
此时我们是直接可以通过BlackCat 来调用Cat实现的接口method的， 如图：
![](http://ww2.sinaimg.cn/mw690/63918611gw1f9o8kop7p7j20go0gxmyx.jpg)
上图中，我们并没有为BlackCat实现接口，仅仅为Cat实现了接口，不过BlackCat里面嵌入了Cat的一个内部类型，也是可以通过接口调用函数Greet来调用到Meow method。
同时我们修改main函数，发现BlackCat的methods set 里面包含了Meow(), 如下图：
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9o8kp2aghj20go0j276g.jpg)
此时：
> 
嵌入类型的类型中，外部类型自己未曾实现的methods被携带的内部函数实现时，外部类型也会将这些methods加入到自己的methods set里。
那么，如果外部类型BlackCat自己实现了Meow()函数（同时也实现了接口），内部和外部都实现的情况下会怎样呢？
下面我们给BlackCat实现Meow()方法试一下：


```
func (c BlackCat) Meow(){
    fmt.Println("BlackName:", c.Cat.Name, " Age:", c.Age)
}
```
首先，编译是没问题的：
![](http://ww2.sinaimg.cn/mw690/63918611gw1f9o8kpdtlej20go0i7wgv.jpg)
我们看一下a.Cat.Meow() 和 a.Meow()的运行结果：
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9o8kpjzvvj20go03j74n.jpg)
发现BlackCat的使用了自己实现的方法，或者说它把内部Cat的Meow()方法覆盖了。
将直接调用改成使用接口时也是如此：
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9o8kq3xoyj20go06u3z6.jpg)
![](http://ww4.sinaimg.cn/mw690/63918611gw1f9o8kq7xxnj20go06vt9w.jpg)
两次结果是一样的。
## 0x03 结论
搞清Golang中的方法集继承对我们写出不啰嗦的代码很有用处，而在Golang中，对于指针的处理也比C/C++中灵活了一些。
## 0x04 参考资料
[Methods, Interfaces and Embedded Types in Go](https://link.zhihu.com/?target=https%3A//www.goinggo.net/2014/05/methods-interfaces-and-embedded-types.html)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/10/30091aff79e77ecbab99678473d262ea.jpeg)![](http://jbcdn2.b0.upaiyun.com/2016/10/d0895096265ab585cdb52a0119e21608.jpeg)
