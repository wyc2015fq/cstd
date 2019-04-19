# ubuntu下载串口调试工具，查看串口信息 - xqhrs232的专栏 - CSDN博客
2016年03月21日 21:36:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3860
原文地址：[http://blog.csdn.net/xuyuefei1988/article/details/8660974](http://blog.csdn.net/xuyuefei1988/article/details/8660974)
相关文章
1、[Ubuntu linux中怎么查看自己的串口连接情况](http://blog.csdn.net/mybelief321/article/details/8987658)----[http://blog.csdn.net/mybelief321/article/details/8987658](http://blog.csdn.net/mybelief321/article/details/8987658)
2、ubuntu下查看和配置串口 
 ----[http://5deweb.blog.163.com/blog/static/4902191120138601058790/](http://5deweb.blog.163.com/blog/static/4902191120138601058790/)

当然首要要安装图形界面串口调试工具：ubuntu软件中心中输入：cutecom ，
每个UART控制器包含一个波特率发生器，发送器，接收器和控制单元，发送和接收器包括FIFO和数据移位器，首先数据被写到FIFO中，然后复制到发送移位器中准备发送，最后数据被发送数据管脚移位发出。数据接收过程是：首先从接收管脚上面的到数据，然后将数据从移位器中复制到FIFO中。
rs232串口通信接口：当通信距离较近时（<12m），可以使用电缆线直接连接，若距离较远，需附加调制解调器。
9个脚针的定义：
CDC数据载波检测，RXD接收数据，TXD发送数据，DTR数据中断准备，GND信号地，DSR数据设备准备好，RTS请求发送，CTS清楚发送，DELL振铃指示。
linux下查看串口信息
查看串口是否可用，可以对串口发送数据比如对com1口，echo helloworld >/dev/ttyS0
查看串口名称使用 ls -l /dev/ttyS*一般情况下串口的名称全部在dev下面，如果你没有外插串口卡的话默认是dev下的ttyS*,一般ttyS0对应com1，ttyS1对应com2，当然也不一定是必然的；
查看串口驱动：cat /proc/tty/drivers/serial
[root@dy root]# cat /proc/tty/driver/*
serinfo:1.0 driver revision:
0: uart:MPC52xx PSC mmio:0x80011300 irq:40 tx:10 rx:129 CTS|DSR|CD
usbserinfo:1.0 driver:2.0
例如，在终端输入：dmesg | grep ttyS*显示从系统启动到现在串口插入拔出的信息，让后我在电脑上插入一个usb转rs232线，再在终端输入：dmesg | grepttyS* ，会显示：usb
 2-2.1:pl2303 converter now attached tottyUSB0;可以通过此法确定我新插入的串口线对应的串口号。
如此动作，我的提示信息为：[ 8100.048072] pl2303 ttyUSB0: pl2303 converter now disconnected from ttyUSB0
插入串口后为：[ 8112.467905] usb 6-1: pl2303 converter now attached to ttyUSB0
如此可以看出是USB0啦，串口调试工具中选择/dev/ttyS0。具体如图所示：Device中一定要注意书写：/dev/ttyUSB0而不是原来下拉菜单的/dev/ttyS0
xuyuefei@ubuntu:/dev$ ls
adsp             loop7               ram8        tty12  tty41  ttyUSB0
audio            mapper              ram9        tty13  tty42  urandom
这是分配的设备名称，注意对应起来
![](https://img-my.csdn.net/uploads/201303/11/1363009331_2247.png)
