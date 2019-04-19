# Qt通过句柄操作外部窗口 - xqhrs232的专栏 - CSDN博客
2016年10月13日 22:23:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2903
原文地址::[http://blog.csdn.net/junbujianwpl/article/details/47192249](http://blog.csdn.net/junbujianwpl/article/details/47192249)
相关文章
1、
## Qt进程间通信(使用HWND进行发消息)
 ----[http://blog.sina.com.cn/s/blog_615e7b3f0101rfjf.html](http://blog.sina.com.cn/s/blog_615e7b3f0101rfjf.html)
2、
# [Qt引用Windows API方法](http://blog.csdn.net/ys_073/article/details/7770693)----[http://blog.csdn.net/ys_073/article/details/7770693](http://blog.csdn.net/ys_073/article/details/7770693)
之前很长时间内用的都是MFC，后来VC的多线程与内存泄露让我焦头烂额，牵扯东西太多，于是乎，转战qt了。
编程过程中，不可避免要操作外部窗口，MFC中操作窗口用的都是CWND::fromHandle()；
qt中操作外部窗口与CWND对应的是QWindow类。
HWNDhwndLogin=FindWindow(NULL,L"登录");QWidget*widgetLogin=QWidget::find((WId)hwndLogin);QWindow*windowLogin=QWindow::fromWinId((WId)hwndLogin);windowLogin->showFullScreen();
用上面的语句即可将相应句柄对应的窗口最大化显示。当然有不足的地方，比如调用title显示不了窗口的标题的，调用show方法，有时候不能把窗口提到顶层，取决于窗口状态，如果窗口是最小化的，show会将窗口提到最前面，否则没有变化的。。。。我还一度以为方法不好使。。。还是有坑啊。
另外，如果单纯想实现某些窗口的最大化、最小化、放在最前面，可以用API函数，原生态的，既快捷又安全。
    ::SetForegroundWindow(hwndSilver);
可以将窗口提到最上面。
用QWidget会报错，提示对应句柄的QWidget为空，但是用QWindow是可以的。
使用的是Qt 5.2版本，编译器是minGw。
关于这个知识点    就这样啦。。。
积极主动的人有更多的机会  ：）
[](http://blog.csdn.net/junbujianwpl/article/details/47192249#)[](http://blog.csdn.net/junbujianwpl/article/details/47192249#)[](http://blog.csdn.net/junbujianwpl/article/details/47192249#)[](http://blog.csdn.net/junbujianwpl/article/details/47192249#)[](http://blog.csdn.net/junbujianwpl/article/details/47192249#)[](http://blog.csdn.net/junbujianwpl/article/details/47192249#)
- 
顶
0- 
踩
