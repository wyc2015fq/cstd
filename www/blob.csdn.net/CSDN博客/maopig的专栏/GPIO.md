# GPIO - maopig的专栏 - CSDN博客
2012年02月16日 23:29:52[maopig](https://me.csdn.net/maopig)阅读数：895标签：[linux																[框架](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
                
 GPIO的驱动主要就是读取GPIO口的状态，或者设置GPIO口的状态。就是这么简单，但是为了能够写好的这个驱动，在LINUX上作了一些软件上的分层。
为了让其它驱动可以方便的操作到GPIO，在LINUX里实现了对GPIO操作的统一接口，这个接口实则上就是GPIO驱动的框架，具体的实现文件为gpiolib.c
在配置内核的时候，我们必须使用CONFIG_GENERIC_GPIO这个宏来支持GPIO驱动。
