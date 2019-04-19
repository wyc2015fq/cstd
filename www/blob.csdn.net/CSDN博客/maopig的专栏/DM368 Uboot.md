# DM368 Uboot - maopig的专栏 - CSDN博客
2012年05月16日 13:57:01[maopig](https://me.csdn.net/maopig)阅读数：2167标签：[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)
个人分类：[DM368](https://blog.csdn.net/maopig/article/category/919673)
这三个参数均有UBOOT直接传递给内核，所以要想知道他们具体的作用，需要根系内核模块的结构。
dm365_imp.oper_mode                    是指在内核模块中内存空间采用连续、或者不连续模式。
davinci_capture.device_type              是你的捕获设备的设备类型
davinci_enc_mngr.ch0_mode           是输出编码信号的格式，PAL或者NTSC。
我印象中dm365_imp.oper_mode=0 表示continuous mode； =1表示single shot mode。是关于Previewer和Resizer的工作模式。
请专家解答。
