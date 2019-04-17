# 用Eclipse替代Keil&IAR来开发ARM应用（升级版） - DoubleLi - 博客园






Eclipse GNU ARM Plugin

2014/7/16
 作者 kiya

几个月前写了一篇《[用Eclipse替代Keil&IAR来开发ARM应用](http://bbs.21ic.com/icview-730340-1-1.html)》，介绍了GNU ARM这个优秀的Eclipse插件。但由于配置过于复杂，不太实用。好消息是这个插件升级了，提供了只有商业IDE才有的向导功能，这下对于新建工程来说，不用再手动配置link、gdb等功能。最强悍的是，除了原有的GDB调试插件，现在也支持最新的JLInk, OpenOCD了！注意这个OpenOCD，有了它，意味着世面上的绝大部分JTAG Adaptor都支持！比如stlink。闲话少叙，我们来看看效果。

1、新建工程
2、编译
3、调试

1、新建C/C++工程

福利来了，ST的死粉激动吗？目前该插件支持全系列STM32 MCU。Freescale的这两个新建工程我没用过，有兴趣可以测测看，不过Freescale的CW也是基于Eclipse，用不用这个插件无所谓了，就是苦了NXP的兄弟们，还是继续用原来的方法新建工程吧。

继续下一步：

很多东西可以选选嘛，手痒的话自己装一个点点看。

继续：

帮手痒的同学点一个，Toolchain很丰富啊，有没有！好了，接下来进入主界面。

2、编译

编译下，速度杠杠的。懒人们有福了，直接往上填代码就好了，boot、link、lib啥都不用管了。唉~~

来调试下吧。

3、调试

直接双击GDB SEGGER J-Link Debugging后就会生成一个xx Debug的界面，点Debug。靠，直接用默认值，有木有很傻瓜！！如果你有stlink，试试OpenOCD，也是用默认值就可以。

好了，来看下最终的调试界面：



行了，太tm方便了。

为了让你更爽一些，我把装好插件的Eclipse放在QQ群共享里了，君随意哦~

想讨论的来我们QQ群（STM32 USB开源库，号码185275258），想自己丰衣足食的参考我的上一篇文章，以及GNU ARM的官网。

用Eclipse替代Keil&IAR来开发ARM应用
[http://bbs.21ic.com/icview-730340-1-1.html](http://bbs.21ic.com/icview-730340-1-1.html)

GNU ARM插件的官网：
[http://gnuarmeclipse.livius.net/blog/](http://gnuarmeclipse.livius.net/blog/)










