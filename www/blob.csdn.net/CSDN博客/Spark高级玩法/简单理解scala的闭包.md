# 简单理解scala的闭包 - Spark高级玩法 - CSDN博客
2019年01月23日 22:21:20[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：202
闭包这个词大家都不陌生，尤其是做spark的同学，经常会看到，但是很多人还是对闭包比较懵懂，就像前面说的watermark一样，大家都很熟悉朗朗上口，但是又貌似一头雾水没有理解。
[不懂watermark？来吧～](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486324&idx=1&sn=b1132367e26e6da1c1982de4af436e91&chksm=9f38e85ca84f614a201eb20c5b365cfd9e93b2c2b93f1906a12181e8720afc7d0541309c1dc4&scene=21#wechat_redirect)
那么，浪尖今天就说说闭包～
首先给出浪尖理解的定义吧：
「函数」和「函数内部能访问到的变量」（也叫环境）的总和，就是一个闭包。
按照这个定义呢？下面就应该是一个闭包：
var factor = 3
val adder = (i:Int) => i * factor
可能会有同学有疑问：浪尖，这不对啊？我看网上说的闭包构造是：
闭包首先有函数嵌套，内部函数引用外部函数的变量，然后返回的是一个函数。
应该是这个样子的：
```
object closure {
  def main(args: Array[String]): Unit = {
    println(makeAdd()(1))
  }
  def makeAdd() = {
    val more = 10
    (x: Int) => x + more
  }
}
```
**为啥要用函数嵌套？**
需要外部函数的作用主要是隐藏变量，限制变量作用的范围。
有些人看到「闭包」这个名字，就一定觉得要用什么包起来才行。其实这是翻译问题，闭包的原文是 Closure，跟「包」没有任何关系。
所以函数套函数只是为了造出一个局部变量，跟闭包无关。
***为啥要return函数呢？***
很明显，不return函数无法使用闭包～～
那么现在换个脑子吧，我们将more 变成makeAdd的参数，那么就是下面的形式：
`def makeAdd(more : Int) = (x: Int) => x + more`
**请问这种结构看着眼熟么？****欢迎留言～**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
490+球友～
