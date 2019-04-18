# 消息循环中的TranslateMessage函数 - ljx0305的专栏 - CSDN博客
2008年03月06日 22:28:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1216
# 消息循环中的TranslateMessage函数
### [开发者在线 Builder.com.cn](http://www.builder.com.cn/) 更新时间:2007-09-28作者：ghost 来源:CSDN
## 本文关键词： [消息循环](http://www.builder.com.cn/list-0-0-86183-1-1.htm)[ghost](http://www.builder.com.cn/list-0-0-82652-1-1.htm)[函数](http://www.builder.com.cn/list-0-0-70574-1-1.htm)
函数功能描述:将虚拟键消息转换为字符消息。字符消息被送到调用线程的消息队列中，在下一次线程调用函数GetMessage或PeekMessage时被读出。
.函数原型：
    BOOL TranslateMessage(  CONST MSG *lpMsg );
.参数：
    lpMsg 
        指向一个含有用GetMessage或PeekMessage函数从调用线程的消息队列中取得消息信息的MSG结构的指针。
.返回值：
    如果消息被转换（即，字符消息被送到线程的消息队列中），返回非零值。
    如果消息是 WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, 或 WM_SYSKEYUP，返回非零值，不考虑转换。
    如果消息没有转换（即，字符消息没被送到线程的消息队列中），返回值是零。
.备注：
    TranslateMessage函数不修改由参数lpMsg指向的消息。
    消息WM_KEYDOWN和WM_KEYUP组合产生一个WM_CHAR或WM_DEADCHAR消息。消息WM_SYSKEYDOWN和WM_SYSKEYUP组合产生一个WM_SYSCHAR或 WM_SYSDEADCHAR 消息。
    TtanslateMessage仅为那些由键盘驱动器映射为ASCII字符的键产生WM_CHAR消息。
    如果应用程序为其它用途而处理虚拟键消息，不应调用TranslateMessage函数。例如，如果TranslateAccelerator函数返回一个非零值，则应用程序将不调用TranslateMessage函数。
    Windows CE：Windows CE不支持扫描码或扩展键标志，因此，它不支持由TranslateMessage函数产生的WM_CHAR消息中的lKeyData参数（lParam）16-24的取值。
    TranslateMessage函数只能用于转换由GetMessage或PeekMessage函数接收到的消息。
 转自[http://www.builder.com.cn/2007/0928/527419.shtml](http://www.builder.com.cn/2007/0928/527419.shtml)
