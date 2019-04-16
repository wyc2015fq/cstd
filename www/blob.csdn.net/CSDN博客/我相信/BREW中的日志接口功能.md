# BREW中的日志接口功能 - 我相信...... - CSDN博客





2011年03月19日 11:46:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1526标签：[brew																[程序开发																[file																[存储																[手机																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)




SDK中的ILOGGER 接口为 AEE 层提供了通用记录的功能，共有三种主要的实现，应用程序作者通过使用以下一个ClassID 创建 ILOGGER 实例：

–  AEECLSID_LOGGER_FILE 向文件发送日志项；

–  AEECLSID_LOGGER_SERIAL 向串行端口发送日志项；

–  AEECLSID_LOGGER_WIN 向 BREW Simulator输出窗口发送日志项。

不管是文件实现还是串行记录实现都按以下数据包格式将数据输出到各自 的输出媒介：

----------------------------- | BREW 头文件 | 包数据 | -----------------------------

但串行记录实现将这些日志包按以下格式包装在 HDLC 帧中：

---------------- | BREW 头文件 | 包数据 | 16b FCS | 8b HDLC 标记 | ------------------



其中16 位 FCS（帧检查序列）是 RFC 1171 中指定的 16 位 CRC；8 位 HDLC 标记的值为 0x7E，如果在 BREW 头文件、包数据或 16b FCS 中存在此标记， 则必须用 8 位值 0x7D 进行转义并与 0x20 进行“异或”运算。 而且，如果转义值 0x7D 存在，也必须以相同方法进行转义。例如： 在数据中 -> 在输出流中 0x7E -> 0x7D、0x5E 0x7D -> 0x7D、0x5D 这些值稍后必须由接收软件进行反转义。记录到串行端口时，在 ILOGGER 接口指示数据包已成功发送和数据包完全送出串行端口之间会存在一个延迟，这是由于低级别串行接口引入的缓冲延迟引起的。

因此，应用程序开发者应该注意：如果发送完最后一个包后，ILOGGER 接口被过快释放，则通过串行端口接收的数据包有可能被截断。为避免这种情况，可能要在发送完最后一个数据包和释放 ILOGGER 接口之间引入一个延迟。

ILogger 接口的窗口实现将用以下格式，将所有外发日志写入模拟器的输出窗口：

bkt:XX typ:XX cID:XX iID:XX FILENAME LINENUMBER MESSAGE ARGS

其中：

bkt： 日志存储

typ： 日志类型

cID： 当前正在运行的 BREW 应用程序的ClassID

iID： 用户定义的实例 ID

FILENAME： 发送日志的可选文件名

LINENUMBER： 发送日志的可选行号

MESSAGE： 用户定义的文本消息

ARGS： 使用 ILOGGER_PutMsg() 的可选参数

编译某个版本的应用时，如果定义 AEE_LOG_DISABLE 常量，该常量可以使用预处理器删除几乎所有的 ILOGGER 接口函数功能，实例创建过程函数、获取参数函数、设置参数函数除外。如果希望如此，必须在应用程序包含 AEELogger.h 之前定义此常量。

这样，开发者不但可以在模拟器上实时调测应用，还可以通过模拟器的输出窗口以及日志文件来调试目标手机上的应用程序。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/11/30/6045556.aspx](http://blog.csdn.net/wireless_com/archive/2010/11/30/6045556.aspx)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=程序开发&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




