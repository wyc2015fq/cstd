# CSerialPort类解析（一） - xqhrs232的专栏 - CSDN博客
2013年04月30日 22:37:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1157
原文地址::[http://xiaozhekobe.blog.163.com/blog/static/175646098201211075723333/](http://xiaozhekobe.blog.163.com/blog/static/175646098201211075723333/)
相关网帖
1、CSerialPort类解析（二）----[http://xiaozhekobe.blog.163.com/blog/static/175646098201211075929729/](http://xiaozhekobe.blog.163.com/blog/static/175646098201211075929729/)
2、CSerialPort类解析(三）----[http://xiaozhekobe.blog.163.com/blog/static/17564609820121108553439/](http://xiaozhekobe.blog.163.com/blog/static/17564609820121108553439/)
3、CSerialPort类解析（四）----[http://xiaozhekobe.blog.163.com/blog/static/175646098201211145743853/](http://xiaozhekobe.blog.163.com/blog/static/175646098201211145743853/)
4、CSerialPort类解析（五）——源代码----[http://xiaozhekobe.blog.163.com/blog/static/175646098201211145953852/](http://xiaozhekobe.blog.163.com/blog/static/175646098201211145953852/)
5、CSerialPort类解析（六）——CSerialPort.h----[http://xiaozhekobe.blog.163.com/blog/static/17564609820121115117131/](http://xiaozhekobe.blog.163.com/blog/static/17564609820121115117131/)
CserialPort类的功能及成员函数介绍
CserialPort类是免费提供的串口累，Codeguru是一个非常不错的源代码网站
CserialPort类支持线连接（非MODEM）的串口编程操作。
CserialPort类是基于多线程的，其工作流程如下：首先设置好串口参数，再开启串口检测工作线程，串口检测工作线程检测到串口接收到的数据、流控制事件或其他串口事件后，就以消息方式通知主程序，激发消息处理函数来进行数据处理，这是对接受数据而言的，发送数据可直接向串口发送。
CserialPort类定义的消息如表
|消息名称|消息号|功能说明|
|----|----|----|
|WM_COMM_BREAK_DETECTED|WM_USER+1|检测到输入中断|
|WM_COMM_CTS_DETECTED|WM_USER+2|检测到CTS（清除发送）信号状态改变|
|WM_COMM_DSR_DETECTED|WM_USER+3|检测到DSR（数据设备准备就绪）信号状态改变|
|WM_COMM_ERR_DETECTED|WM_USER+4|发生线状态错误（包括CE_FRAME，CE_OVERRUN，和CE_RXPARITY）|
|WM_COMM_RING_DETECTED|WM_USER+5|检测到响铃指示信号|
|WM_COMM_RLSD_DETECTED|WM_USER+6|检测到RLSD（接收线信号）状态改变|
|WM_COMM_RXCHAR|WM_USER+7|接收到一个字符并已放入接受缓冲区|
|WM_COMM_RXFLAG_DETECTED|WM_USER+8|检测到接受到字符（该字符已放入接受缓冲区）事件|
|WM_COMM_TXEMPTY_DETECTED|WM_USER+9|检测到发送缓冲区最后一个字符已经被发送|
