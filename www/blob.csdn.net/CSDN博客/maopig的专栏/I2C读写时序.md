# I2C读写时序 - maopig的专栏 - CSDN博客
2017年02月21日 22:37:16[maopig](https://me.csdn.net/maopig)阅读数：1667
个人分类：[I2C/SPI](https://blog.csdn.net/maopig/article/category/6739244)

1. I2C写时序图：
    注意：最后一个byte后，结束标志在第十个CLK上升沿之后：
![](https://img-blog.csdn.net/20150910123658608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2. I2C读时序图：
    注意：restart信号格式；读操作结束前最后一组clk的最后一个上升沿，主机应发送NACK（之前的读clk后最后一个bit发送ACK），然后再发送结束标志。
![](https://img-blog.csdn.net/20150910123714060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
