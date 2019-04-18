# MFC消息详解  - ljx0305的专栏 - CSDN博客
2009年09月20日 19:18:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：551标签：[mfc																[command																[null																[制造																[存储																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=制造&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[VC																[MFC](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/401790)
1. 怎样使用MFC发送一个消息用MFC发送一个消息的方法是，
    首先，应获取接收消息的CWnd类对象的指针；
    然后,调用CWnd的成员函数SendMessage( )。
        LRESULT Res=pWnd->SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
        pWnd指针指向目标CWnd类对象。变量Msg是消息，wParam和lParam变量包含消息的参数，如鼠标单击哪里或选择了什么菜单项。目标窗口返回的消息结果放在变量Res中。
        发送消息到一个没有CWnd类对象的窗口，可以用下列目标窗口的句柄直接调用Windows API：
        LRESULT Res=::SendMessage(HWND hWnd, UINT Msg,  WPARAM wParam, LPARAM lParam);
        这里的hWnd是目标窗口的句柄。
2. 怎样用MFC寄送一个消息
    用MFC寄送一个消息与发送一个消息几乎相同，但寄送时用PostMessage( ) ，而不是用SendMessage( )；返回值Res也不一样，Res不是一个由目标窗口返回的值，而是一个布尔值，用来表示消息是否成功地放到消息队列中。
3. 检索一个寄送消息
    正常情况下，一旦消息被寄送后，应用程序在后台发送它。但是在特殊情况下，需要你自己去删除一个消息，例如想在应用程序接收到某种消息之前停止应用程序。有两种方法可以从应用程序消息队列中删除一个消息，但这两种方法都没有涉及MFC。
■ 第一种方法：在不干扰任何事情之下窥视消息队列，看看一个消息是否在那里。
    BOOL res=::PeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg ) ;
■ 第二种方法：实际上是等待，一直等到一个新的消息到达队列为止，然后删除并返回该消息。
    BOOL res=::GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
    在这两种方法中，变量hWnd指定要截获消息的窗口，如果该变量设为NULL，所有窗口消息将被截获。wMsgFilterMin和wMsgFilterMax变量与SendMessage( )中的变量Msg相对应，指定查看消息的范围。如果用"0,0"，则所有的消息都将被截获。如果用WM_KEYFIRST,WM_KEYLAST或WM_MOUSEFIRST,WM_MOUSELAST，则所有键盘或鼠标的消息将被截获。wRemoveMsg变量指定PeekMessage( )是否应该真正地从队列中删除该消息。(GetMessage( )总是删除消息)。该变量可以取两个值：
    ■ PM_REMOVE，PeekMessage( )将删除消息。
    ■ PM_NOREMOVE，PeekMessage( )将把消息留在队列里，并返回它的一个拷贝。
    当然，如果把消息留在消息队列中，然后再次调用PeekMessage( )查看相同类型的消息，则将返回完全相同的消息。
    lpMsg变量是一个指向MSG结构的指针，MSG包含检索到的消息。
    typedef struct tagMSG {
                        HWND hwnd; // window handle message is intended for
                        UINT message;
                        WPARAM wParam;
                        LPARAM lParam;
                        DWORD time; // the time the message was put in the queue
                        POINT pt; // the location of the mouse cursor when the
                                       // message was put in the queue
                        } MSG;
4. MFC怎样接收一个寄送的消息
    MFC处理一个寄送和发送消息的唯一明显不同是寄送的消息要在应用程序的消息队列中花费一些时间。在消息泵(message pump)弹出它之前，它要一直在队列中。
    消息泵
    MFC应用程序中的消息泵在CWinApp的成员函数Run()中。应用程序开始运行时，Run()就被调用,Run()把时间分割成两部分。一部分用来执行后台处理，如取消临时CWnd对象；另一部分用来检查消息队列。当一个新的消息进来时，Run()抽取它—即用GetMessage( )从队列中取出该消息，运行两个消息翻译函数，然后用DispatchMessage( )函数调用该消息预期的目标窗口进程。
    消息泵调用的两个翻译函数是PreTranslateMessage( )和::TranslateMessage( )。目标窗口的MFC类可调用reTranslateMessage在发送消息给它之前进行消息翻译，例如，CFrameWnd用PreTranslateMessage( )将加速键(如，Ctrl+S存储文件)转换为命令消息。翻译前的消息通常被处理掉，而翻译后的消息(如果有的话)将被重新寄送到队列里。::TranslateMessage是一个窗口函数，将原始键码转换为键字符。消息一旦被DispatchMessage()发送，MFC处理它就像处理SendMessage()发送的消息一样。
5. MFC怎样处理一个接收到的消息
    处理接收到的消息的目的非常简单：将消息指向一个函数，该函数通过消息中的消息标识符处理它。非MFC窗口用简单的case语句来实现该目标，每个case语句执行一些函数，或调用其他一些函数。
    MainWndProc(HWND hWnd, UINT message, W PARAM wParam,LPARAM lParam)
    {
        switch(message)
        {
        case WM_CREATE:
            : : :
        break;
        case WM_PAINT:
            : : :
        break;
        default:
        return(DefWindowProc(hWnd,message,wParam,lParam));
        }
        return(NULL);
    }
    任何遗漏的消息将被传输到一个默认的消息处理函数，但是，case语句不能很好地适应C++和封装技术。在C++环境中，要求消息被一个专门处理该类型消息的类的成员函数处理。因此，MFC不采用case语句，而采用更加复杂和回旋的方法。但它允许用私有类处理消息，而只需做下面三件事情：
    ■ 从将要接收消息的CWnd类对象派生类(对于命令消息是CCmdTarget)。
    ■ 在派生类中写一个处理消息的成员函数。
    ■ 在类中定义一个查找表(叫做消息映像)，该表具有成员函数的条目和它要处理的消息的标识符。
    然后，MFC依次调用下面的函数，指引输入消息到处理函数。
    1) AfxWndProc( )接收消息，寻找消息所属的CWnd对象，然后调用AfxCallWndProc( )。
    2) AfxCallWndProc( )存储消息(消息标识符和参数)供未来参考，然后调用WindowProc( )。
    3) WindowProc( ) 发送消息给OnWndMsg( ) ，然后，如果消息未被处理，则发送给DefWindowproc( )。
    4) OnWndMsg( )要么为WM_COMMAND消息调用OnCommand( )，要么为WM_NOTIFY消息调用OnNotify( )。任何被遗漏的消息都将是一个窗口消息。OnWndMsg( )搜索类的消息映像，以找到一个能处理任何窗口消息的处理函数。如果OnWndMsg( )不能找到这样的处理函数，则把消息返回到WindowProc( )，由它将消息发送给DefWindowProc( )。
    5) OnCommand()查看这是不是一个控件通知(lParam不是NULL)；如果它是，OnCommand( )就试图将消息映射到制造通知的控件；如果它不是一个控件通知，或者控件拒绝映射的消息，OnCommand( )就调用OnCmdMsg( )。
    6) OnNotify( )也试图将消息映射到制造通知的控件；如果映射不成功， OnNotify( )就调用相同的OnCmdMsg( )函数。
    7) 根据接收消息的类，OnCmdMsg( )将在一个称为命令传递(Command Routing)的过程中潜在地传递命令消息和控件通知。例如，如果拥有该窗口的类是一个框架类，则命令和通知消息也被传递到视图和文档类，并为该类寻找一个消息处理函数。
为什么要消息映像？
    这毕竟是C++语言；为什么OnWndMsg( )不为每个窗口消息调用一个预定义的虚拟函数？因为它太占CPU。若是那样，当扫描一个消息映像以加速该过程时，OnWndMsg( )可能会做出意想不到的事情，并陷入汇编器。注意通过重载WindowProc( )、OnWndMsg( )、OnCommand( )、OnNotify( ) 或OnCmdMsg( )可以修改这一过程。重载OnWndMsg( )可以在窗口消息被排序之前插入该过程。重载OnCommand( )或OnNotify( )可以在消息被反射之前插入该过程。
引用：[http://www.cppblog.com/dbkong/archive/2007/03/09/19479.aspx](http://www.cppblog.com/dbkong/archive/2007/03/09/19479.aspx)
