# 使用WM_COPYDATA进行进程间通信的一个问题 - xqhrs232的专栏 - CSDN博客
2014年02月10日 11:04:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：868
原文地址::[http://blog.sina.com.cn/s/blog_5737e7ad0100ts80.html](http://blog.sina.com.cn/s/blog_5737e7ad0100ts80.html)
相关文章
1、[WM_COPYDATA实现进程间数据共享](http://www.cppblog.com/TechLab/archive/2005/12/30/2272.html)----[http://www.cppblog.com/TechLab/archive/2005/12/30/2272.aspx](http://www.cppblog.com/TechLab/archive/2005/12/30/2272.aspx)
2、为什么用SendMessage发WM_COPYDATA消息无效呢?----[http://bbs.csdn.net/topics/80401624](http://bbs.csdn.net/topics/80401624)
3、WM_COPYDATA message----[http://msdn.microsoft.com/en-us/library/ms649011(VS.85).aspx](http://msdn.microsoft.com/en-us/library/ms649011(VS.85).aspx)

 开发中有时需要进程间传递数据，比如对于只允许单实例运行的程序，当已有实例运行时，再次打开程序，可能需要向当前运行的实例传递信息进行特殊处理。对于传递少量数据的情况，最简单的就是用SendMessage发送WM_COPYDATA消息，所带参数wParam和lParam可以携带相关数据。由于SendMessage是阻塞的，在接收数据进程处理完数据之前不会返回，发送方不会删除或修改数据，因此这种方法是简单且安全的，不过数据量不能太大，否则会由于处理时间过长造成阻塞假死。
    用SendMessage发送WM_COPYDATA的方法如下：
|lResult = SendMessage(|// returns LRESULT in lResult|
|----|----|
|(HWND) hWndControl,|// handle to destination control|
|(UINT) WM_COPYDATA,|// message ID|
|(WPARAM) wParam,|// = (WPARAM) () wParam;|
|(LPARAM) lParam|// = (LPARAM) () lParam;|
|);| |
    其中，wParam为发送数据方的窗口句柄，lParam为指向一个COPYDATASTRUCT类型结构体的指针，该结构体中包含了传递的数据信息。COPYDATASTRUCT定义如下：
    typedef struct tagCOPYDATASTRUCT {
        ULONG_PTR dwData;
        DWORD cbData;
        PVOID lpData;
    } COPYDATASTRUCT, *PCOPYDATASTRUCT;
    其中，dwData为自定义的数据，cbData指定lpData指向数据的大小，lpData为指向数据的指针。按照前面所说，在使用WM_COPYDATA时要保证数据的只读属性，即不能有发送方的其他线程对传递数据进行改写。（这也解释了为什么不允许用PostMessage发送WM_COPYDATA，因为PostMessage函数是异步的。还有一点需要注意的是由于SendMessage是阻塞的，所以容易引起死锁，可以考虑用SendMessageTimeout代替。）另外，如果传递数据中涉及到对象或系统资源，必须确保接收方可以对其进行处理，比如HDC、HBITMAP之类的资源是无效的，他们属于不同的进程。
    在使用的时候，要用FindWindow等API找到接收方的窗口句柄；接收方的程序中要添加对WM_COPYDATA消息的响应。
    前几天写程序用到WM_COPYDATA进行进程间通信，但是接收方怎么也收不到消息。调试发现找到的窗口句柄是没有问题的，查看MSDN也没有什么提示，百思不得其解。
    后来看了一些示例代码，发现不同之处是我的SendMessage调用中wParam和lParam参数都是0，因为我只是需要通过WM_COPYDATA消息通知一下接收程序即可，不用传递任何数据。试着将这两个参数改为非空，接收方就可以收到消息了。总结结论为：wParam参数是否为0没有影响，但是lParam参数必须为非空，即必须指向一个有效的COPYDATASTRUCT结构体。
    原因是什么呢？查了一些资料发现，SendMessage（WM_COPYDATA）底层是通过文件映射（File Mapping）完成的，大概流程是发送方线程根据COPYDATASTRUCT结构体中的传递数据信息，在共享内存中进行数据复制，接收方线程则会到共享内存中读取数据进行处理。因此如果指向COPYDATASTRUCT结构的指针为空的话，流程是无法进行的，所以接收方也理所当然收不到消息。
//===============================================================================================================================
备注：
1》WM_COPYDATA消息是基于内存映射文件机制的，必须使用SendMessage函数来发，SendMessage函数发WM_COPYDATA消息时，LPARAM参数必须指定到一个有效地址/内存数据！！！
