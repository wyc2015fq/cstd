# java为什么匿名内部类的参数引用时final？ - z69183787的专栏 - CSDN博客
2017年03月30日 17:24:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3406
[https://www.zhihu.com/question/21395848](https://www.zhihu.com/question/21395848)
[https://www.zhihu.com/question/24084277/answer/110176733](https://www.zhihu.com/question/24084277/answer/110176733)
喜欢看生肉的同学就不用看我的回答了，直接看R大的三篇回答，尤其是第一篇后面的回复部分。
我只是试着用大白话做个简单的整理，希望能更容易理解一点。
[关于对象与闭包的关系的一个有趣小故事](https://link.zhihu.com/?target=http%3A//rednaxelafx.iteye.com/blog/245022)
 （这篇的精华在后面的回复，小故事可以跳过）
[JVM的规范中允许编程语言语义中创建闭包(closure)吗？
 - RednaxelaFX 的回答](https://www.zhihu.com/question/27416568/answer/36565794)
[为什么Java闭包不能通过返回值之外的方式向外传递值？
 - RednaxelaFX 的回答](https://www.zhihu.com/question/28190927/answer/39786939)
1.
 闭包（Closure）
什么是闭包，大白话不怎么严谨的说就是：
- 一个依赖于外部环境自由变量的函数
- 这个函数能够访问外部环境里的自由变量
看下面这个Javascript闭包的例子：
![](https://img-blog.csdn.net/20170330173315235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对内部函数function(x)来讲，y就是自由变量，而且function(x)的返回值，依赖于这个外部自由变量
y。而往上推一层，外围Add(y)函数正好就是那个包含自由变量y的环境。而且Javascript的语法允
许内部函数function(x)访问外部函数Add(y)的局部变量。满足这三个条件，所以这个时候，外部函
数Add(y)对内部函数function(x)构成了闭包。
闭包的结构，如果用λ演算表达式来写，就是多参数的Currying技术。
>λx.λy.x+y
但在Java中我们看不到这样的结构。因为Java主流语法不允许这样的直接的函数套嵌和跨域访问变量。
2.
 类和对象
但Java中真的不存在闭包吗？正好相反，Java到处都是闭包，所以反而我们感觉不出来在使用闭
包。因为Java的“对象”其实就是一个闭包。其实无论是闭包也好，对象也好，都是一种数据封装的
手段。看下面这个类
![](https://img-blog.csdn.net/20170330173512642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
看上去x在函数add()的作用域外面，但是通过Add类实例化的过程，变量”x“和数值”2“之间已经绑
定了，而且和函数add()也已经打包在一起。add()函数其实是透过this关键字来访问对象的成员字
段的。
如果对闭包有疑问，可以看这个更详细的回答：
[闭包（计算机科学）是什么？
 - 胖胖的回答](https://www.zhihu.com/question/24084277/answer/110176733)
3.
 Java内部类是闭包：包含指向外部类的指针
那Java里有没有除了实例对象之外的闭包结构？Java中的内部类就是一个典型的闭包结构。例子如下，
![](https://img-blog.csdn.net/20170330173618893?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下图画的就是上面代码的结构。内部类（Inner
 Class）通过包含一个指向外部类的引用，做到自
由访问外部环境类的所有字段，变相把环境中的自由变量封装到函数里，形成一个闭包。
![](https://img-blog.csdn.net/20170330173707450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.
 别扭的匿名内部类
但Java匿名内部类就做得比较尴尬。下面这个例子中，getAnnoInner负责返回一个匿名内部类的引用。
![](https://img-blog.csdn.net/20170330173810660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
匿名内部类因为是匿名，所以不能显式地声明构造函数，也不能往构造函数里传参数。不但返回的只是个叫AnnoInner的接口，而且还没有和它外围环境getAnnoInner()方法的局部变量x和y构成任何类的结构。但它的addXYZ()函数却直接使用了x和y这两个自由变量来计算结果。这就说明，外部方法getAnnoInner()事实上已经对内部类AnnoInner构成了一个闭包。
但这里别扭的地方是这两个x和y都必须用final修饰，不可以修改。如果用一个changeY()函数试图修改外部getAnnoInner()函数的成员变量y，编译器通不过，
error:
 cannot assign a value to final variable y
这是为什么呢？因为这里Java编译器支持了闭包，但支持地不完整。说支持了闭包，是因为编译器编译的时候其实悄悄对函数做了手脚，偷偷把外部环境方法的x和y局部变量，拷贝了一份到匿名内部类里。如下面的代码所示。
![](https://img-blog.csdn.net/20170330173943067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
所以用R大回答里的原话说就是：
Java编译器实现的只是capture-by-value，并没有实现capture-by-reference。
而只有后者才能保持匿名内部类和外部环境局部变量保持同步。
但Java又不肯明说，只能粗暴地一刀切，就说既然内外不能同步，那就不许大家改外围的局部变量。
**5. 其他和匿名内部类相似的结构**
《Think
 in Java》书里，只点出了匿名内部类来自外部闭包环境的自由变量必须是final的。但实际上，其他几种不太常用的内部类形式，也都有这个特性。
比如在外部类成员方法内部的内部类。
**![](https://img-blog.csdn.net/20170330174112817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
比如在一个代码块block里的内部类。
![](https://img-blog.csdn.net/20170330174159943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
