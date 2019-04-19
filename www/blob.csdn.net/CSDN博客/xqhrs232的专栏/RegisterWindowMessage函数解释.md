# RegisterWindowMessage函数解释 - xqhrs232的专栏 - CSDN博客
2012年09月02日 09:14:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：631
原文地址::[http://blog.csdn.net/jiahehao/article/details/1837861](http://blog.csdn.net/jiahehao/article/details/1837861)
相关网帖
1.RegisterWindowMessage自定义消息怎么用？**----[http://bbs.bccn.net/thread-200904-1-5.html](http://bbs.bccn.net/thread-200904-1-5.html)**
2.进程间通信问题--RegisterWindowMessage和PostMessage----[http://topic.csdn.net/t/20030522/23/1821663.html](http://topic.csdn.net/t/20030522/23/1821663.html)
3.[RegisterWindowMessage自定义消息](http://eddiezh.iteye.com/blog/1507906)----[http://eddiezh.iteye.com/blog/1507906](http://eddiezh.iteye.com/blog/1507906)
4.[【Win32 API学习】RegisterWindowMessage小记](http://www.cnblogs.com/Vicky-Lee/archive/2011/09/08/2171566.html)----[http://www.cnblogs.com/Vicky-Lee/archive/2011/09/08/2171566.html](http://www.cnblogs.com/Vicky-Lee/archive/2011/09/08/2171566.html)
# 进程通信问题——RegisterWindowMessage和PostMessage
UINT RegisterWindowMessage(lpsz)
函数说明：RegisterWindowMessage函数定义一个新
的窗口消息，该消息保证在整个系统范围内是唯一的。
调用SendMessage或PostMessage函数时可以使用该
函数返回的消息值。
参数说明： lpsz
　　指向一个以NULL结束的字符串，该字符串指定待登记的消息。
返回值：若成功地登记了消息，返回值是一个消息标识符。该标识符值的范围在0XC000到0XFFFF之间，否则，返回值为0。
注释：RegisterWindowMessage函数通常用于为两个合作应用程序之间的通信登记消息。
　　若两个不同的应用程序登记了相同的消息字符串，则这两个应用程序返回相同的消息值。该消息一直维持被登记状态，直到Windows会话结束。当多个应用程序必须处理同一消息时才使用RegisterWindowMessage函数。在一个窗口类范围内发送私有消息时，应用程序可使用范围在WM_USER到0X7FFF之间的任意整数。
