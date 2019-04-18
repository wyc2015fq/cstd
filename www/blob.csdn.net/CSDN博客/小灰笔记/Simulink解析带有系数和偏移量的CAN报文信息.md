# Simulink解析带有系数和偏移量的CAN报文信息 - 小灰笔记 - CSDN博客





2017年08月02日 23:52:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1953








       这部分功能我关注的很少，我用到的CAN协议中的因子都是在消息传完之后自己增加。这样，我用到的Simulink仅仅是一个最简单的翻译功能。

       有一阵子我觉得带有factor和bias的信号使用Simulink实现就是第一个定标的实现，后来我发现无论我怎么进行定标信息的修正始终出现数据类型的不匹配。直到有一天，随手拽了一个强制数据类型转换的模块实现了代码生成。从代码中，我发现其实定标信息的实现实际上使用的是浮点方式。

       这样，进行一个信号解析就非常简单了。搭建一个如下的模型：

![](https://img-blog.csdn.net/20170802235121825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       其中，CAN消息中的信号定义如下：

![](https://img-blog.csdn.net/20170802235132267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       进行代码生成之后，查看代码中的相应信息如下：

![](https://img-blog.csdn.net/20170802235143854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       从上面的代码可以看出，在数据定义中的定标信息已经实现。而输入的信号实际上使用的是浮点的数据类型。而我们定标最终的数据其实也仅仅是一个最终的物理数值，使用这种方式最多也就会损失一点精度。整体的功能实现上应该基本能够满足我们的要求。

       解决了消息的打包发送，其实，消息帧的解析拆包也是类似的处理方式。这里暂且不进行相应的总结。



