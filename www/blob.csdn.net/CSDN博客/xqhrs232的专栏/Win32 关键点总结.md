# Win32 关键点总结 - xqhrs232的专栏 - CSDN博客
2012年06月19日 16:23:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1766标签：[windows																[timer																[command																[null																[图形																[radiobutton](https://so.csdn.net/so/search/s.do?q=radiobutton&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/weiqubo/article/details/7238660](http://blog.csdn.net/weiqubo/article/details/7238660)
**一 窗口和消息**
**1 前缀**
|前缀|全称|释义|
|----|----|----|
|CS|class style|类风格选项|
|CW|create windows|创建窗口选项|
|DT|draw text|绘制文本选项|
|IDI|Icon ID|图标ID号|
|IDC|Cursor ID|光标ID号|
|MB|Message Box|消息框选项|
|SND|Sound|声音选项|
|WM|Windows Message|窗口消息|
|WS|Windows Style|窗口风格|
**2 WPARAM和LPARAM的意义**
   在Windows是一种16位系统时，WndProc的第三个参数被定义为WORD，是一个16位的无符号整数，而第四个参数被定义为一个LONG，是一个32位有符号整数，所以导致对单词PARAM（参数）加前缀W和L。
但在32位Windows中，WPARAM被定义为一个UINT，而LPARAM被定义为一个LONG，因此窗口过程的这两个参数都是32位的值。
**3 新的函数类型**
WndProc函数返回一个类型为LRESULT的值，该值是一个LONG型，32位有符号。
WndProc函数被指定为CALLBACK类型，WinMain函数被指定为WINAPI类型。这些类型指在Windows本身和用户的应用程序之间发生的函数调用的特殊调用序列。
**4 窗口类结构WNDCLASS**
有10个域。分别是：
① style：类风格，用于在什么时候发出窗口变化消息
② cbClsExtra：在类结构保存的窗口结构中预留一些额外空间
       ③ cbWndExtra：在Windows内部保存的窗口结构中预留一些额外空间
       ④ hbrBackground：指定基于这个类创建的窗口背景颜色
       ⑤ hCursor：读取光标
       ⑥ hIcon：读取图标
       ⑦ hInstance：程序的实例句柄
       ⑧ lpfnWndProc：指定处理基于这个窗口类创建的所有窗口的窗口过程
       ⑨ lpszClassName：指定类名
       ⑩ lpszMenuName：指定窗口类菜单
**5 注册窗口类RegisterClass**
一般在Windows XP及以后都可以很顺利的注册成功。
所以可以只写RegisterClass(&wndclass);
**6 创建窗口CreateWindow**
    窗口类定义了窗口的一般特征，调用CreateWindow可以指定有关窗口的更详细的信息。
hwnd = CreateWindow (szAppName,          // 指定一个窗口类，基于该窗口类创建窗口
                   TEXT ("Hello Win"),    // 这个字符串会出现在标题栏中
                   WS_OVERLAPPEDWINDOW, // 本窗口风格
                   CW_USEDEFAULT,         //窗口左上角的X坐标
                   CW_USEDEFAULT,         //窗口左上角的Y坐标
                   CW_USEDEFAULT,         //窗口的宽度
                   CW_USEDEFAULT,         //窗口的高度
                   NULL,                    //窗口对象的父窗口句柄
                   NULL,                    //窗口对象的菜单句柄或者子窗口编号
                   hInstance,                  //当前进程的实例句柄
                   NULL) ;                   //窗口对象的参数指针句柄
创建窗口返回的是窗口句柄。
**7 显示窗口**
窗口创建成功后，系统将在内存中为其分配一块内存，但是此时窗口并未显示在显示器上，所以需要使用两个调用。
① ShowWindow(窗口句柄，iCmdShow)；
其中的第二个参数用于确定如何儿子屏幕上显示窗口，是最小化还是常规还是最大化。
② UpdateWindow(窗口句柄)
调用上句将导致客户区被绘制。它通过给窗口过程发送一个WM_PAINT消息来做到这一点。
**8 消息循环**
调用UpdateWindow之后，窗口就出现在显示器上。
Windows为当前运行的每个Windows程序维护一个“消息队列”。在发生事件的时候，Windows将事件转换为一个“消息”，并将消息放入程序的消息队列中。
程序通过执行一个叫做“消息循环”的代码从消息队列中取出消息。
while (GetMessage(&msg, NULL, 0, 0))
           {
                  TranslateMessage(&msg);
                  DispatchMessage(&msg);
           }
其中msg是一个类型为MSG的结构，MSG结构在WINUSER.H中定义如下：
typedef struct tagMSG {
    HWND       hwnd;
    UINT         message;
    WPARAM     wParam;
    LPARAM      lParam;
    DWORD      time;
    POINT        pt;
} MSG, *PMSG
其中，hwnd 是消息发向的窗口句柄。
      message 是消息标识符，以WM_开头。
      wParam 一个32位的消息参数。
      lParam 一个32位的消息参数。
      time 消息放入消息队列的时间。
      pt 消息放入消息队列时鼠标的坐标。
消息循环以GetMessage调用开始，它从消息队列中取出一个消息，GetMessage(&msg, NULL, 0, 0)这一调用传给Windows一个指向名为msg的MSG结构的指针。其余三个参数设置为NULL或0，表示程序接收自己创建的所有窗口的所有消息。
只要从消息队列中取出的消息的message域不为WM_QUIT，GetMessage就返回一个非0值，while循环就可以继续。
TranslateMessage(&msg);将msg结构传给Windows，进行一些键盘转换。
DispatchMessage(&msg);将msg结构传给Windows，然后Windows将里面的消息发给相应的窗口过程进行处理。处理后，WndProc返回到Windows，Windows返回到程序，程序继续下一个while循环。
**9 窗口过程WndProc**
实际的动作发生在窗口过程中。窗口过程确定了在窗口的客户区显示什么，以及怎么处理用户输入。
窗口过程是命名为WndProc的函数。（也可以其他不冲突的名字）
一个Windows程序可以包含多个窗口过程。
一个窗口过程总是与调用RegisterClass注册的特定窗口类相关联。
CreateWindow函数根据特定的窗口类创建一个窗口，返回该窗口的句柄。
但是基于一个窗口类可以创建多个窗口。
1程序     包括   n个窗口过程
1窗口过程 关联   1窗口类
1窗口类   创建   n个窗口
CreateWindows根据窗口类创建一个窗口。
窗口过程总是声明成如下形式：
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM)；
这4个参数跟MSG结构的前4个域是相同的。
第一个参数：接收消息的窗口句柄。
第二个参数：消息类型，标识消息的数字
最后两个参数：32位的消息参数。
程序通常不直接调用窗口过程，而是由Windows调用窗口过程。
**10 窗口过程处理消息**
窗口过程接收的每个消息均是用一个数值来标识的，也就是传给窗口过程的message参数。
窗口过程处理消息时，必须返回0.
窗口过程不予处理的所有消息应该被传给名为DefWindowsProc的Windows函数。
用switch语句来处理不同消息，消息以WM_开头。
**11 WM_CREATE消息**
当Windows在处理CreateWindow函数时，窗口过程就会接收到WM_CREATE消息。
通常，窗口过程在WM_CREATE处理期间进行一次窗口初始化。
**12 WM_PAINT消息**
当窗口客户区域的一部分或全部变成“无效”时，必须进行刷新，WM_PAINT将通知程序。
在最初创建窗口的时候，整个客户区都是无效的，因为程序还没有在窗口上画任何东西。在调用UpdateWindow时，通常会触发第一个WM_PAINT消息，指示窗口过程在客户区域上画一些东西。
在改变程序窗口大小后，客户区也会变得无效，至于怎么变得无效由CS_引导的类风格选项确定。
对WM_PAINT的处理几乎总是从一个BeginPaint调用开始，以一个EndPaint结束。
hdc = BeginPaint(hwnd, &pt);
do something;（如GetClientRect(hwnd, &rect)；）
EndPaint(hwnd, &pt);
hwnd是要刷新的窗口的窗口句柄。
pt是指向类型为PAINTSTRUCT的结构指针。
在BeginPaint调用中，如果客户区域的背景还未被擦除，就由Windows来擦除。然后使用注册窗口类的WNDCLASS结构中的hbrBackground域中第一的刷子来删除背景。
BeginPaint调用使整个客户区有效，并返回一个“设备描述表句柄”。设备描述表是指物理输出设备及其驱动程序。可以利用该“设备描述表句柄”在客户区域显示文本和图形。
EndPaint调用释放设备描述表句柄。
GetClientRect(hwnd, &rect)；
第一个参数：程序的窗口句柄；
第二个参数：指向RECT类型的rectangle结构。该结构有4个LONG域，标识客户区域的尺寸。
当改变窗口大小时，WndProc通过调用GetClientRect来获取变化后的窗口大小，重新绘制客户区。
**13 WM_DESTROY消息**
    当用户点击关闭按钮时发生。
程序可以通过调用PostQuitMessage以标准方式响应WM_DESTROY消息；
PostQuitMessage(0);
该函数在程序的消息队列插入一个WM_QUIT消息。
GetMessage对于除了WM_QUIT消息之外的从消息队列中取出的所有消息都返回非0值。而当GetMessage取到一个WM_QUIT消息时，返回0.
**14 关闭程序时的消息传递**
① 用户点击关闭按钮
② 产生WM_SYSCOMMAND消息；
③ 产生WM_CLOSE消息响应WM_SYSCOMMAND；
④ 产生WM_DESTROY消息响应WM_CLOSE；
⑤ 产生WM_QUIT消息响应WM_DESTROY。
**15 进队消息和不进队消息**
消息能够分为：进队消息和不进队消息。
进队消息是由Windows放入程序消息队列中的。在程序的消息循环中，重新返回并分配给窗口过程。
不进队消息在Windows调用窗口时直接发送给窗口过程。
也就是说，进队消息发送给消息队列，不进队消息发送给窗口过程。
在任何情况下，窗口过程都将获得窗口所有的消息。窗口过程是窗口的消息中心。
进队消息基本上是用户输入的结果，还包括时钟消息、刷新消息、退出消息。
不进队消息基本上是来自调用特定的Windows函数。
**二 输出文本**
**16 有效矩形和无效矩形**
窗口过程一旦接受到WM_PAINT消息之后，就准备更新整个客户区，但往往只需更新一个较小的区域。这个区域就称为“无效区域”。正是客户区内存在无效区域，才提示Windows将一个WM_PAINT消息放入消息队列。
Windows内部为每个窗口保存一个“绘图信息结构”，这个结构包含了包围无效区域的最小矩形的坐标以及其他信息，这个矩形就叫做“无效矩形”。
如果在窗口过程处理WM_PAINT消息之前，客户区又有一个区域变为无效，那么Windows计算出一个包围两个无效区域的新的无效矩形，并将这个变化后的信息放在绘制信息结构中。
一个消息队列在一个时刻只能有一个WM_PAINT消息在队列中。
窗口过程可以调用InvalidateRect使客户区变为无效。如果消息队列包含一个WM_PAINT消息，那么Windows将计算出新的无效矩形；否则，就在消息队列中添加一个WM_PAINT消息。
在处理WM_PAINT消息期间，窗口过程在调用了BeginPaint之后，整个客户区就会变得有效。
程序也可以显式调用ValidateRect函数使客户区内的任意矩形区域变得有效。如果这条调用使整个客户区都有效，那么将在当前消息队列中删除WM_PAINT消息。
**17 设备描述表**
要在窗口的客户区绘图，可以使用Windows的图形设备接口GDI函数。
设备描述表DC是GDI内部保存的数据结构。
设备描述表与特定的显示设备有关。
设备描述表中的有些值是图形化的“属性”，如指出颜色、背景色、坐标映射方式等。
当程序要绘图时，必须先获取设备描述表句柄。在获取了该句柄之后，Windows用默认的属性值填充设备描述表结构的内部各域。
当程序在客户区绘图完毕后，必须释放设备描述表句柄。句柄被释放后不再有效，也不再使用。程序必须在处理单个消息期间获取和释放句柄。
**18 获取设备描述表句柄的方法之一**
在使用WM_PAINT消息时，使用这种方法。它涉及到BeginPaint和EndPaint两个函数。
在处理WM_PAINT消息时，窗口过程首先调用BeginPaint。BeginPaint函数一般在准备绘制时导致无效区域的背景被擦除。BeginPaint返回的值是设备描述表句柄，这一返回值通常被保持在叫做hdc的变量中。
HDC hdc；
HDC数据类型定义为32位的无符号数。
然后，程序就可以使用需要设备描述表句柄的GDI函数了。
调用EndPaint即可释放设备描述表句柄。
一般地，处理WM_PAINT消息的形式如下：
case WM_PAINT：
    hdc = BeginPaint(hwnd, &ps);
    使用GDI函数
    EndPaint(hwnd, &ps);
    return 0;
处理WM_PAINT消息时，必须成对地调用BeginPaint和EndPaint。
**19 绘图信息结构**
Windows为每一个窗口保存一个绘图信息结构。这就是PAINTSTRUCT，定义如下：
typedef struct tagPAINTSTRUCT {
    HDC         hdc;          
    BOOL        fErase;        
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT
在程序调用BeginPaint时，Windows填充该结构的各个字段。用户程序只需要使用前三个字段。
hdc是设备描述表句柄。
fErase通常被标识为FLASE，这意味着Windows已经擦除了无效矩形的背景。
如果程序通过调用Windows函数InvalidateRect使客户区中的矩形失效，那么该函数的最后一个参数会指定fErase的值。如果指定0，那么在稍后的PAINTSTRUCT里面的fErase会被设置为TRUE。
rcPaint是RECT结构，定义了无效矩形的边界。RECT结构中的left、top、right、bottom以像素点为单位。此时，Windows将绘图操作限制在此RECT结构定义的矩形范围内，如果要在无效矩形外绘图，应该在调用BeginPaint之前，使用如下调用：
InvalidateRect(hwnd, NULL, TRUE)；
它将使整个客户区无效，并擦除背景。
**20获取设备描述表句柄的方法之二**
要得到窗口客户区的设备描述表句柄，可以调用GetDC来获取句柄。在使用完后调用ReleaseDC；
hdc = GetDC(hwnd);
使用GDI函数
ReleaseDC(hwnd, hdc);
GetDC和ReleaseDC函数必须成对地使用。
GetDC返回的设备描述表句柄具有一个剪取矩形，等于整个客户区。
GetDC不会使任何无效区域变为有效，要是整个客户区有效，需要调用：
ValidateRect(hwnd, NULL);
一般可以调用GetDC和ReleaseDC来对键盘消息、鼠标消息作出反应。
**21 TextOut细节**
TextOut是用于显示文本的最常用的GDI函数。语法是：
TextOut(hdc, x, y, psText, iLength);
第一个参数：设备描述表句柄，既可以是GetDC的返回值，也可以是BeginPaint的返回值。
第二个参数：定义客户区内字符串的开始位置的水平坐标。
第三个参数：定义客户区内字符串的开始位置的垂直坐标。
第四个参数：指向要输出的字符串的指针。
第五个参数：字符串中字符的个数。如果psText中的字符是Unicode的，那么串中的字节数就是iLength值的两倍。
设备描述表还定义了一个剪取区域。
对于从GetDC获取的设备描述表句柄，默认的剪取区是整个客户区。
对于从BeginPaint获取的设备描述表句柄，默认的剪取区是无效区域。
Windows不会在剪取区域之外的任何位置显示字符串。
**22 字符大小**
要用TextOut显示多行文本，就必须确定字体的字符大小，可以根据字符的高度来定位字符的后续行，以及根据字符的宽度来定位字符的后续列。
系统字体的字符高度和平均宽度取决于视频显示器的像素大小。
程序可以调用GetSystemMetrics函数来确定关于用户界面构件大小的信息。
程序可以调用GetTextMetrics函数来确定字体大小。
metric是度量的意思。
TEXTMETRIC的结构：
typedef struct tagTEXTMETRIC
{
    LONG        tmHeight;
    LONG        tmAscent;
    LONG        tmDescent;
    LONG        tmInternalLeading;
    LONG        tmExternalLeading;
    LONG        tmAveCharWidth;
    LONG        tmMaxCharWidth;
    其他域
} TEXTMETRIC, *PTEXTMETRIC；
要使用GetTextMetrics函数，需要先定义一个通常被称为tm的结构变量：
TEXTMETRIC tm；
在需要确定文本尺寸时，先要获取设备描述表句柄，再调用GetTextMetrics：
hdc = GetDC(hwnd);
GetTextMetrics(hdc, &tm);
操作；
ReleaseDC(hwnd,hdc);
**23 文本尺寸**
字体的纵向大小由5个值确定：
① tmHeight，等于tmAscent加上tmDescent。这两个值表示了基线上下字符的最大纵向高度。
② tmAscent，基线以上的高度
③ tmDescent，基线以下的高度
④ tmInternalLeading，重音号和字符之间的距离，如ü中的u和两点的距离。
⑤ tmExternalLeading，一般用于多行文本间行距的调整。
字符的横向大小由2个值确定：
① tmAveCharWidth，小写字母加权平均宽度。
② tmMaxCharWidth，字体中最宽字符的宽度。
对于等宽字体，tmAveCharWidth和tmMaxCharWidth这两个值相等。
大写字母的平均宽度比较复杂，如果：
① 字体是等宽字体，那么大写字母的平均宽度等于tmAveCharWidth。
② 字体是变宽字体，那么大写字母的平均宽度等于tmAveCharWidth*1.5。
判断字体是否是变宽字体，可以通过TEXTMETRIC结构中的tmPitchAndFamily域的低位判断，如果低位是1，那么是变宽字体，如果是0，那么是等宽字体。
大写字母宽度 = (tm.tmPitchAndFamily & 1 ? 3 : 2) / 2 * 小写字母宽度
**24 格式化文本**
在一次Windows对话期间，系统字体的大小不会改变，因此在程序运行过程中，只需要调用一次GetTextMetric。最好是在窗口过程中处理WM_CREATE消息时进行此调用。
假设要编写一个Windows程序，在客户区显示多行文本，这需要先获取字符宽度和高度。可以在窗口过程内定义两个变量来保存字符宽度和总的字符高度。
case WM_CREATE：
   hdc = BeginPaint(hwnd, &pt);
   GetTextMetric(hdc, &tm);
   cxChar = tm.tmAveCharWidth;                                  小写字母宽度
   cyChar = tm.Height + tm.tmExternalLeading;                      字母高度
   cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) / 2 * tm.tmAveCharWidth; 大写字母宽度
   EndPaint(hwnd, &pt);
   return 0;
**25 客户区的大小**
窗口最大化之后的客户区大小，可以通过以SM_CXFULLSCREEN和SM_CYFULLSCREEN为参数调用GetSystemMetric来获得。
要确定客户区的大小，最好的方法是在窗口过程处理WM_SIZE消息。在窗口大小改变时，就会产生WM_SIZE消息。传给窗口过程的lParam参数的低位字中包含客户区的宽度x，高位字中包含客户区的高度y。要保存这些尺寸，可以定义两个int型变量来保存。
static int cxClient，cyClient；
然后在WM_SIZE消息处理中：
case WM_SIZE：
    cxClient = LOWORD(lParam);
    cyClient = HIWORD(lParam);
    return 0;
用cyClient/cyChar可以得到客户区可以显示的文本总行数。
**26 滚动条的范围和位置**
每个滚动条都有一个相关的范围和位置。这是一对整数。当滚动框在滚动条的顶部（左部）时，滚动框的位置是范围的最小值；在滚动条的底部（右部）时，滚动框的位置是范围的最大值。
在默认情况下，滚动条的范围是0~100，但将范围改变为更方便于程序的数值也是很容易的：
SetScrollRange(hwnd, iBar, iMin, iMax, bRedraw);
其中hwnd为该窗口的句柄。
    iBar为SB_VERT或SB_HORZ。
    iMin和iMax为范围。
    bRedraw，如果要Windows根据新范围重绘滚动条，则设置为TRUE。
滚动框的位置不是连续的，而是离散的整数值。
可以使用SetScrollPos在滚动条范围内设置新的滚动框位置：
SetScrollPos(hwnd, iBar, iPos, bRedraw)；
参数iPos是新位置，必须在iMin至iMax的范围内。
Windows提供了类似的函数GetScrollRange和GetScrollPos来获取滚动条的当前范围和位置。
**27 滚动条消息**
在用鼠标单击滚动条或者拖动滚动框时，Windows都给窗口过程发生WM_VSCROLL或WM_HSCROLL消息。在滚动条上的每个鼠标动作都至少产生两个消息，一个在按下鼠标键时产生，一个在释放鼠标键时产生。
WM_VSCROLL和WM_HSCROLL也带有wParam和lParam消息参数。
lParam只用于作为子窗口而创建的滚动条（通常在对话框内）。
wParam消息参数被分为一个低位字和一个高位字。
低位字是一个数值，指出了鼠标对滚动条进行的操作。这个数值被看作一个“通知码”。通知码以SB开头。
#define SB_LINEUP           0
#define SB_LINELEFT         0
#define SB_LINEDOWN        1
#define SB_LINERIGHT        1
#define SB_PAGEUP           2
#define SB_PAGELEFT         2
#define SB_PAGEDOWN        3
#define SB_PAGERIGHT        3
#define SB_THUMBPOSITION   4
#define SB_THUMBTRACK     5
#define SB_TOP               6
#define SB_LEFT              6
#define SB_BOTTOM           7
#define SB_RIGHT             7
#define SB_ENDSCROLL        8
当把鼠标的光标放在滚动框上并按住鼠标键时，就产生SB_THUMBPOSITION和SB_THUMBTRACK消息。
当wParam的低位字是SB_THUMBTRACK时，wParam的高位字是用户在拖动滚动框时的当前位置。
当wParam的低位字是SB_THUMBPOSITION时，wParam的高位字是用户释放鼠标后滚动框的最终位置。
**28 滚动条信息函数**
滚动条文档指出SetScrollPos、SetScrollRange、GetScrollPos、GetScrollRange函数是过时的。
在Win32 API中，升级了2个滚动条函数，称作SetScrollInfo和GetScrollInfo。这些函数完成上述4个函数的全部功能，并增加了2个新特性。
第一个功能设计滚动框的大小。滚动框的大小称作页面大小。算法是：
滚动框大小 / 滚动长度 ≈ 页面大小 / 范围 ≈ 显示的文档数量 / 文档的总大小
可以使用SetScrollInfo来设置页面大小。
第二个功能是GetScrollInfo函数，它可以获取32位的范围值。
SetScrollInfo和GetScrollInfo函数的语法是：
SetScrollInfo(hwnd, iBar, &si, bRedraw);
GetScrollInfo(hwnd, iBar, &si);
iBar参数是SB_VERT或SB_HORZ。
bRedraw可以是TRUE或FALSE，指出了是否要Windows重新绘制计算了新信息后的滚动条。
两个函数的第三个参数是SCROLLINFO结构，定义为：
typedef struct tagSCROLLINFO
{
    UINT    cbSize;
    UINT    fMask;
    int       nMin;
    int       nMax;
    UINT    nPage;
    int       nPos;
    int       nTrackPos;
}   SCROLLINFO
在程序中，可以定义如下的SCROLLINFO结构类型：
SCROLLINFO si；
在调用SetScrollInfo或GetScrollInfo函数之前，必须将cbSize自动设置为结构的大小：
si.cbSize = sizeof(si);或si.cbSize = sizeof(SCROLLINFO);
fMask：把fMask字段设置为以SIF前缀开头的一个或多个标识，来获取或设置里面的结构中域的值。
① SIF_RANGE：用于获取或设置滚动条的范围
② SIF_POS：用于获取或设置滚动框的位置
③ SIF_PAGE：用于获取或设置滚动条的页面大小
④ SIF_TRACKPOS：用于获取或设置滚动框移动时的位置
**三 图形基础**
**29 GDI基础**
图形设备接口GDI是Windows的子系统，它负责在视频显示器和打印机上显示图形。
Windows NT中的图形主要由GDI32.DLL动态链接库输出的函数来处理。
GDI的主要目的之一是支持与设备无关的图形。
图形输出设备分为两大类：光栅设备和矢量设备。
大多数PC机显示器、打印机都是光栅设备。
绘图仪是矢量设备。
组成GDI的函数可以分为这样几类：
① 获取（或重建）和释放（或清除）设备描述表的函数；
② 获取有关设备描述表信息的函数；
③ 绘图函数；
④ 设置和获取设备描述表参数的函数；
⑤ 使用GDI对象的函数。
**30 GDI图元**
在屏幕或打印机上显示的图形类型本身可以被分为几类，通常被称为“图元”。
① 直线和曲线
② 填充区域
③ 位图：位图是位的矩形数组，位对应于显示设备上的像素，它们是光栅图形的基本工具。GDI支持两种类型的位图——老的“设备有关”位图，新的“设备无关”位图。
④ 文本
**31 GDI其他方面**
① 映射模式和变化；
② 元文件：元文件是以二进制形式存储的GDI命令的集合。元文件主要用于通过剪贴板传输矢量图形表示。
③ 区域：区域是形状任意的复杂区；
④ 路径：路径是GDI内部存储的直线和曲线的集合；
⑤ 剪裁：绘图可以限制在客户区的某一部分中，这就是剪裁。剪裁通常是通过区域或者路径定义的。
⑥ 调色板：定制调色板通常限于显示256色的显示器。Windows仅保留这些色彩之中的20种供系统使用，但可以改变其他236种色彩。
⑦ 打印
**32 进一步探讨设备描述表**
想在一个图形输出设备上绘图时，首先必须获得一个设备描述表的句柄。将句柄返回给程序时，Windows就给了用户使用设备的权限。然后在GDI函数中将该句柄作为一个参数，向Windows标识想在其上进行绘图的设备。
（1）获取设备描述表句柄
如果在处理一条消息时获取了设备描述表句柄，应该在退出窗口函数之前释放它。
获取设备描述表句柄的几种方法：
① 在处理WM_PAINT消息时，使用BeginPaint和EndPaint调用
hdc = BeginPaint(hwnd, &ps);
GDI操作
EndPaint(hwnd, &ps);
注：变量ps是类型为PAINTSTRUCT的结构，该结构的hdc字段是BeginPaint返回的设备描述表句柄。PAINTSTRUCT结构包含了一个名为rcRect的RECT结构，该结构定义了包围窗口无效范围的矩形。使用从BeginPaint获得的设备描述表句柄，只能在这个区域内绘图。BeginPaint调用使这个区域有效。
② 可以在处理非WM_PAINT消息时获取设备描述表句柄
hdc = GetDC(hwnd);
GDI操作
ReleaseDC(hwnd, hdc);
注：这个设备描述表适用于窗口句柄为hwnd的客户区。这个调用可以在整个客户区上绘图。
③ Windows程序还可以获取适用于整个窗口的设备描述表句柄
hdc = GeWindowstDC(hwnd);
GDI操作
ReleaseDC(hwnd, hdc);
注：这个设备描述表除了客户区之外，还包括窗口的标题栏、菜单、滚动条和框架。如果要使用该函数，必须捕获WM_NCPAINT消息。
④ 获取整个屏幕的设备描述表句柄
hdc = CreateDC(TEXT(“DISPLAY”), NULL, NULL, NULL);
原型是：hdc = CreateDC(pszDriver, pszDevice, pszOutput, pData);
        GDI操作
        DeleteDC(hdc);
⑤ 如果只需要获取关于某设备描述表的一些信息，而并不进行任何绘画，在这种情况下，可以使用CreateIC来获取一个“信息描述表”的句柄，其参数和CreateDC一样。
⑥ 一个设备描述表通常是指一个物理显示设备。通常，需要获取有关该设备的信息，其中包括显示器的显示尺寸和色彩范围。可以通过GetDeviceCaps函数来获取这些信息。
iValue = GetDeviceCaps(hdc, iIndex);
参数iIndex的取值为WINGDI.H头文件中定义的29个标识符之一。
**33 用TextOut输出整型的方法**
设一开始有整型：int i = 100；
要用TextOut函数将i输出，需要用到三个函数：
① wsprintf
② TEXT宏
③ TextOut
首先得先说明下wsprintf的原型：
int wsprintf(LPTSTR lpOut, LPCTSTR lpFmt,...);
第一个参数：缓冲区，是一个字符数组，一般定义为TCHAR型。
第二个参数：格式字符串，因为第一个参数是TCHAR类型,一定要和TEXT宏联合使用,这样才能在不同的编译环境下都可以顺利编译。
后续参数：要输出的的整型变量。
针对第一个参数：要定义一个TCHAR的字符数组作为缓冲区。
TCHAR szBuffer[10]; //足够大就行了
针对第二个参数，需要使用到TEXT宏。
TEXT宏的原型：
TEXT(LPTSTR string  //ANSI or Unicode string);
用来处理要转换的整型，具体用法是：
TEXT(“%d”);
那么上述的两个调用应该写成：
int iLength = 0; //用来保存字符串中的字符个数；
iLength = wsprintf(szBuffer, TEXT(“%d”), i);
上述语句的作用是：将i存进szBuffer中，返回szBuffer存有的字符个数到iLength中。
TextOut的原型是：
TextOut(hdc, x, y, psText, iLength);
第一个参数是设备描述表句柄；
第二个参数是输出的文本的x坐标；
第三个参数是输出的文本的y坐标；
第四个参数是是指向要输出的字符串的指针；
第五个参数是字符串中的字符个数；
那么TextOut函数应该写成：
TextOut(hdc, x, y, szBuffer, iLength);
**34 设备的大小**
使用GetDeviceCaps函数能获取有关输出设备物理大小的信息。
对于打印机，用“每英寸的点数dpi”表示分辨率。
对于显示器，用水平和垂直的总的像素数来表示分辨率。
用“像素大小”或“像素尺寸”表示设备水平或垂直显示的总像素数。
用“度量大小”或“度量尺寸”表示以每英寸或毫米为单位的显示区域的大小。
像素大小 / 度量大小 = 分辨率
使用SM_CXSCREEN和SM_CYSCREEN参数从GetDeviceCaps得到像素大小；
使用HORZSIZE和VERTSIZE参数从GetDeviceCaps得到度量大小；
两者相除就可以得到水平分辨率和垂直分辨率。
如果设备的水平分辨率和垂直分辨率相等，就称该设备具有“正方形像素”。
    因为整个屏幕的度量大小是固定的，所以可以根据分辨率调整水平或垂直显示的像素数。如果分辨率小，那么“像素大小”也就小，也就是说，总像素数少了，那么每个像素的尺寸也就变得大些。
**35 字体的大小**
现在讨论字体的大小问题，这里不是说字号，而是说字体显示的dpi值。Windows系统默认是每英寸96点，所另外一种选择，就是每英寸120点。
我们在调整分辨率的时候，从小分辨率变化到大分辨率时，会觉得图标的文字变小，那是因为在大分辨率下，每个像素的面积变小，假设一个字需要100个像素来显示，那么从小分辨率变化到大分辨率时，字的总面积就变小了，所以字的大小也就发生变化，而这一变化是字体的大小变化，而不是该字的字号发生变化。
在传统的排版中，字体的字母大小由“磅”表示。1磅≈1/72英寸，在计算机排版中1磅正好为1/72英寸。
理论上，字体的磅值是从字体中最高的字符顶部到字符下部的字符底部的距离，其中不包括重音号。根据TEXTMETRIC结构，字体的磅值等于tmHeight – tmInternalLeading。
36 关于色彩
“全色”视频显示器的分辨率是每个像素24位：8位红色、8位绿色、8位蓝色。
“高彩色”显示分辨率是每个像素16为：5位红色、6位绿色、5位蓝色。
显示256种颜色的视频适配器每个像素需要8位。然而这些8位的值一般由定义实际颜色的调色板表组织。
使用GetDeviceCaps可以使程序员确定视频适配器的存储组织，以及能够表示的色彩数目。
这个调用返回色彩平面的数目：iPlanes = GetDeviceCaps(hdc, PLANES);
这个调用返回每个像素的色彩位数：iBitsPixel = GetDeviceCaps(hdc, BITSPIXEL);
大多数彩色图形显示设备要么使用多个色彩平面，要么每像素有多个色彩位，但是不能同时二者兼用；即这两个调用必须有一个返回1.（一般都是第一个返回1）。
在大多数GDI函数调用中，使用COLORREF值（32位）来表示一种色彩。
|31|…|24|23|…|16|15|…|8|7|…|0|
|----|----|----|----|----|----|----|----|----|----|----|----|
|0|蓝|绿|红| | | | | | | | |
理论上，COLORREF可以指定2的24次方或1600万种色彩。
这个无符号长整数常常称为一个“RGB色彩”。在使用RGB(r, g, b)；宏时注意参数的顺序是红、绿、蓝。而在无符号长整数中，由高位到低位是0、蓝、绿、红。
当三个参数都是0时，表示黑色，当三个参数都是255时，表示白色。
黑色 = RGB(0,0,0) = 0x00000000
白色 = RGB(255, 255, 255) = 0x00FFFFFF
**37 保存设备描述表**
通常，在调用GetDC或BeginPaint时，Windows会用默认值创建一个新的设备描述表，对设备描述表其属性所做的一切修改在调用ReleaseDC或EndPaint被释放掉。
如果需要使用非默认的设备描述表属性，则必须在每次获取设备描述表句柄时初始化设备描述表。
如果需要在释放设备描述表之后，仍然保存程序中对设备描述表所做的改变，以便在下一次调用GetDC和BeginPaint时它们仍起作用。则应该在窗口类那将CS_OWNDC标志包含进窗口类风格中。
wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
现在，基于这个窗口类所创建的每个窗口都将拥有自己的设备描述表，它一直存在，直到窗口被删除。
如果使用了CS_OWNDC风格，就只需初始化设备描述表一次，可以在处理WM_CREATE消息期间完成这一操作。
CS_OWNDC风格只影响GetDC和BeginPaint获得的设备描述表，不影响其他函数获得的设备描述表，如GetWindowDC获得的设备描述表。
在某些情况下，可以需要改变某些设备描述表，用改变后的属性进行绘图，然后又要恢复回改变前的属性。这时，可以通过如下调用来保存设备描述表的状态。
保存：int idSaved = 0; idSaved = SaveDC(hdc);
恢复：RestoreDC(hdc, idSaved);
也可以不保存SaveDC的返回值，这时候如果要恢复，就只能恢复到最近保存的状态，RestoreDC(hdc, -1);
**38 写像素**
写像素SetPixel(hdc, x, y, crColor);其中：
hdc是设备描述表句柄；
x, y是像素点的坐标；
crColor是要设置的颜色，一般可以用RGB(r, g, b)设置。
**39 线条**
几种画线函数：
① LineTo：画直线
② Polyline和PolylineTo：画一系列相连的直线
③ PolyPolyline：画多组相连的线
④ Arc和ArcTo和AngleArc：画椭圆线
⑤ PolyBezier和PolyBezierTo：画贝塞尔线条
⑥ PolyDraw：画一系列相连的线以及贝塞尔线条
几种填充函数：
① Rectangle：画矩形
② Ellipse：画椭圆
③ RoundRect：画带圆角的矩形
④ Pie：画椭圆的一部分，使其看起来像一个扇形
⑤ Chord：画椭圆的一部分，使其看起来像弓形
⑥ Polygon：画多边形
⑦ PolyPolygon：画多个多边形
设备描述表的5个属性影响着用这些函数所画线条的外观：
① 当前画笔的位置；
② 画笔；
③ 背景方式；
④ 背景色；
⑤ 绘图模式。
画一条直线，必须调用2个函数，第一个函数指定了线的开始点坐标，第二个函数指出了线的终点坐标：
MoveToEx(hdc, x1, y1, NULL);
LineTo(hdc, x2, y2);
MoveToEx不会画线，只是设置了设备描述表的“当前位置”属性。然后LineTo函数从当前的位置到它所指定的点画一直线。在默认的设备描述表中，当前位置最初是在点(0,0)。
MoveToEx最后一个参数是指向POINT结构的指针。从该函数返回后，POINT结构的x和y字段指出了之前的“当前位置”，如果不需要这个信息，直接填NULL。
如果需要获取当前位置，先定义一个POINT的结构变量pt，然后通过下面的调用：
GetCurrentPositionEx(hdc, &pt);
几个函数的原型：
Rectangle(hdc, xLeft, yTop, xRight, yBottom);
Ellipse(hdc, xLeft, yTop, xRight, yBottom);
RoundRect(hdc, xLeft, yTop, xRight, yBottom, xCorner, yCorner);
Chord(hdc, xLeft, yTop, xRight, yBottom, xStart, yStart, xEnd, yEnd);
       Pie(hdc, xLeft, yTop, xRight, yBottom, xStart, yStart, xEnd, yEnd);
       Arc(hdc, xLeft, yTop, xRight, yBottom, xStart, yStart, xEnd, yEnd);
一个二维的贝塞尔线条由4个点定义——两个端点和两个控制点。曲线的控制点固定，将曲线从两个端点间的直线处拉伸构造曲线。
**40 使用画笔**
调用任何画笔函数时，Windows使用设备描述表中当前选中的“画笔”来画线。画笔决定线的色彩、宽度、线型。线型可以是实线、点划线、虚线，默认设备描述表中画笔是BLACK_PEN，一个像素宽，实线。
Windows提供三种现有画笔，分别是：BLACK_PEN, WHITE_PEN和NULL_PEN。
Windows使用句柄来引用画笔。用HPEN的类型定义，即画笔的句柄。
HPEN hPen；
调用GetStockObject，可以获得现有画笔的句柄。
hPen = GetStockObject(WHITE_PEN)；
调用SelectObject将画笔选进设备描述表。
SelectObject(hdc，hPen);
SelectObject的返回值是选进前设备描述表的画笔句柄。
使用CreatePen或CreatePenIndirect创建一个“逻辑画笔”，这仅仅是对画笔的描述。这些函数返回逻辑画笔的句柄，然后调用SelectObject将画笔选进设备描述表，之后才可以使用新的画笔来画线。
在任何时候，只能有一种画笔选进设备描述表。
在释放设备描述表或在选择了另一种画笔到设备描述表中之后，就可以调用DeleteObject来删除所创建的逻辑画笔。
逻辑画笔是一种“GDI对象”，GDI对象有六种：画笔、刷子、位图、区域、字体、调色板。
CreatePen的原型是：
HPEN CreatePen(iPenStyle, iWidth, crColor);
iPenStyle参数确定画笔是实线、虚线还是点线。
iWidth参数确定线宽，如果iPenStyle不是实线，且iWith大于1，那么画笔将变成实线。
crColor是RGB颜色。
获取当前画笔句柄：
hPen = GetCurrentObject(hdc, OBJ_PEN);
还可以建立一个逻辑画笔LOGPEN结构，调用CreatePenIndirect来创建画笔。
LOGPEN logpen；
此结构有三个成员：UINT lopnStyle 是画笔线型；POINT lopnWidth是按逻辑单位度量的画笔宽度，只用其中的x值；COLORREF lopnColor是画笔颜色。
**41 填充空隙**
点式画笔和虚线画笔的空隙的着色取决于设备描述表的两个属性——背景模式和背景颜色。默认的背景模式是OPAQUE，在这种方式下，Windows使用背景色填充空隙，默认的背景色为白色。
下述调用用来改变和获取Windows用来填充空隙的背景色：
改变：SetBkColor(hdc, crColor);
获取：GetBkColor(hdc);
下述调用用来改变和获取背景模式：
改变：SetBkMode(hdc, 模式);
      模式：TRANSPARENT，忽略背景色，并且不填充空隙。
            OPAQUE默认。
获取：GetBkMode(hdc);
**42 绘图方式**
设备描述表中定义的绘图方式也影响显示器上所画线的外观。
当Windows使用画笔来画线时，实际上执行画笔像素与目标位置处原来像素之间的某种按位布尔运算。像素间的按位布尔运算叫做“光栅运算”，简称为“ROP”。由于画一条直线只涉及两种像素（画笔和目标），因此这种布尔运算又称为“二元光栅运算”，简称为“ROP2”。
在默认设备描述表中，绘图方式定义为R2_COPYPEN，这意味着Windows只是将画笔像素复制到目标像素代替之。
Windows定义了16种不同的ROP2码，用来设置不同的绘图方式。
设置绘图方式：SetROP2(hdc, iDrawMode);
获取绘图方式：iDrawMode = GetROP2(hdc);
**43 绘制填充区域**
Windows中有7个用来画带边缘的填充图形的函数：
① Rectangle：画矩形
② Ellipse：画椭圆
③ RoundRect：画带圆角的矩形
④ Pie：画椭圆的一部分，使其看起来像一个扇形
⑤ Chord：画椭圆的一部分，使其看起来像弓形
⑥ Polygon：画多边形
⑦ PolyPolygon：画多个多边形
Windows用设备描述表中选择的当前画笔来画图形的边界框，边界框还使用当前背景方式、背景色彩和绘图方式，跟画线时一样。
图形以当前设备描述表中选择的刷子来填充。默认情况下，使用现有对象，这意味着图形内部将画成白色。
Windows定义6种现有刷子：WHITE_BRUSH、LTGRAY_BRUSH、GRAY_BRUSH、DKGRAY_BRUSH、BLACK_BRUSH和NULL_BRUSH。
也可以自己定义刷子 HBRUSH hBrush；
通过GetStockObject来获取现有刷子：
hBrush = GetStockObject(WHITE_BRUSH);
通过SeletctObject将刷子选进设备描述表：
SelectObject(hdc, bBrush)；
如果要画一个没有边界框的图形，可以将NULL_PEN选进设备描述表。
SelectObject(hdc, GetStockObject(NULL_PEN))；
如果要画一个没有填充内部的图像，可以将NULL_BRUSH选进设备描述表。
SelectObject(hdc, GetStockObject(NULL_BRUSH));
画多边形函数的原型：
Polygon(hdc, apt, iCount);
apt参数是POINT结构的一个数组，iCount是点的数目。如果该数组中的最后一个点和第一个点不同，则Windows将会再加一条线，将最后一个点与第一个点连起来。
画多个多边形函数的原型：
PolyPolygon(hdc, apt, aiCounts, iPolyCount);
apt数组具有全部多边形的所有点。
aiCounts数组给出了多边形的端点数。
iPolyCount给出了所画的多边形的个数。
**44 用画刷填充内部**
Rectangle、RoundRect、Ellipse、Chord、Pie、Polygon和PolyPolygon图形的内部是用选进设备描述表的当前画刷来填充的。画刷是一个8×8的位图，它水平和垂直地重复使用来填充内部区域。
Windows有5个函数，可以自己创建逻辑画刷，然后用SelectObject将画刷选进设备描述表。
① hBrush = CreateSolidBrush(crColor); 纯颜色刷子
② hBrush = CreateHatchBrush(iHatchStyle, crColor); 带影射线的刷子
   crColor是影线的颜色，影线的间隙用设备描述表定义的背景方式和背景色来着色。
③ CreatePatternBrush()
④ CreateDIBPatternBrushPt() 基于位图的刷子
⑤ hBrush = CreateBrushIndirect(&logbrush)；
   该函数包含其他4个函数。
变量logbrush是一个类型为LOGBRUSH的结构，该结构有三个字段
UINT lbStyle；
COLORREF lbColor；
LONG lbHatch；
**45 矩形函数**
Windows包含了几种使用RECT结构和“区域”的绘图函数。区域就是屏幕上的一块地方，是矩形，多边形和椭圆的组合。
FillRect(hdc, &rect, hBrush);
用指定画刷来填充矩形。该函数不需要事先将画刷选进设备描述表。
FrameRect(hdc, &rect, hBrush);
使用画刷画矩形框，但不填充矩形。
InvertRect(hdc, &rect)；
将矩形中所有像素反转。
常用矩形函数：
① SetRect(&rect, xLeft, yTop, xRight, yBottom); 设置矩形的4个字段值。
② OffsetRect(&rect, x, y); 将矩形沿x轴和y轴移动几个单元。
③ InflateRect(&rect, x, y); 增减矩形尺寸
④ SetRectEmpty(&rect);  将矩形各字段设为0
⑤ CopyRect(&DestRect, &SrcRect); 将矩形复制给另一个矩形。
⑥ IntersectRect(&DestRect, &SrcRect1，&ScrRect2)；获取两个矩形的交集
⑦ UnionRect(&DestRect, &SrcRect1，&ScrRect2); 获取两个矩形的并集
⑧ bEmpty = IsRectEmpty(&rect); 确定矩形是否为空
⑨ binRect = PtinRect(&rect, point)；确定点是否在矩形内
**46 创建和绘制区域**
区域是对显示器上一个范围的描述，这个范围是矩形、多边形和椭圆的组合。
区域可以用于绘制和剪裁，通过将区域选进设备描述表，就可以用区域来进行剪裁。
当创建一个区域时，Windows返回一个该区域的句柄，类型为HRGN。
HRGN hRgn；
① 创建矩形区域：
hRgn = CreateRectRgn(xLeft, yTop, xRight, yBottom);
或
hRgn = CreateRectRgnIndirect(&rect)；
② 创建椭圆区域：
hRgn = CreateEllipticRgn(xLeft, yTop, xRight, yBottom);
或
hRgn = CreateEllipticRgnIndirect(&rect);
③ 创建多边形区域：
hRgn = CreatePolygonRgn(&point, iCount, iPolyFillMode)；
point参数是个POINT类型的结构数组；
iCount是点的数目；
iPolyFillMode是ALTERNATE或者WINDING
④ 区域的融合
iRgnType = CombineRgn(hDestRgn, hSrcRgn1, hSrcRgn2, iCombine);
这一函数将两个源区域组合起来并用句柄hDestRgn指向组合成的目标区域。
iCombine参数说明了hSrcRgn1和hSrcRgn2是怎么组合的。
RGN_AND   公共部分
RGN_OR     全部
RGN_XOR   全部除去公共部分
RGN_DIFF   hSrcRgn1不在hSrcRgn2的部分
RGN_COPY  hSrcRgn1的全部，忽略hSrcRgn2
区域的句柄可以用到4个绘图函数：
FillRgn(hdc, hRgn, hBrush);
FrameRgn(hdc, hRgn, xFrame, yFrame);
xFrame, yFrame是画在区域周围边框的宽度和高度。
InvertRgn(hdc, hRgn);
PaintRgn(hdc, hRgn);
**47 矩形与区域的剪裁**
区域也在剪裁中扮演了一个角色。
InvalidateRect函数使显示的一个矩形区域失效，并产生一个WM_PAINT消息。
InvalidateRect(hwnd, NULL, TRUE); 清除客户区；
可以通过调用GetUpdateRect来获取失效矩形的坐标。
使用ValidateRect函数使客户区的矩形有效。
当接收到一个WM_PAINT消息时，无效矩形的坐标可以从PAINTSTRUCT结构中得到，该结构是用BeginPaint函数填充的。
Windows中有两个作用于区域而不是矩形的函数：
InvalidateRgn(hwnd, hRgn, bErase);
和
ValidateRgn(hwnd, hRgn);
所以当接收到一个WM_PAINT消息时，可能由无效区域引起的。剪裁区域不一定是矩形。
SelectObject(hdc, hRgn);
或
SelectClipObject(hdc, hRgn);
通过将一个区域选进设备描述表来创建自己的剪裁区域。
**四 键盘**
**48 键盘基础**
Windows程序获得键盘输入的方式：键盘输入以消息的形式传递给程序的窗口过程。
Windows用8种不同的消息来传递不同的键盘事件。
Windows程序使用“键盘加速键”来激活通用菜单项。加速键通常是功能键或字母同ctrl键的组合。Windows将这些键盘加速键转换为菜单命令消息。
程序用来从消息队列中检索消息的MSG结构包括hwnd字段。此字段指出接收消息的窗口句柄。消息循环中的DispatchMessage函数向窗口过程发生该消息，此窗口过程与需要消息的窗口相联系。当按下键盘上的键时，只有一个窗口过程接收键盘消息，并且此消息包括接收消息的窗口句柄。
接收特定键盘事件的窗口具有输入焦点。
窗口过程通过捕获WM_SETFOCUS和WM_KILLFOCUS消息来判定它的窗口何时拥有输入焦点。WM_SETFOCUS指示窗口正在得到输入焦点，WM_KILLFOCUS表示窗口正在失去输入焦点。
当用户按下并释放键盘上的键时，Windows和键盘驱动程序将硬件扫描码转换为格式消息。Windows在“系统消息队列”中保存这些消息。系统消息队列是单消息队列，它由Windows维护，用于初步保存用户从键盘和鼠标输入的信息。只有当Windows应用程序处理完前一个用户输入消息时，Windows才会从系统消息队列中取出下一个消息，并放入应用程序的消息队列。
**49 击键和字符**
应用程序从Windows接受的关于键盘事件的消息可以分为击键和字符两类。
按下键是一次击键，释放键也是一次击键。
对产生可显示字符的击键组合，Windows不仅给程序发送击键消息，而且还发送字符消息。有些键不产生字符，对于这些键，Windows只产生击键消息。
**50 击键消息**
当按下一个键时，Windows把WM_KEYDOWN或者WM_SYSKEYDOWN消息放入有输入焦点的窗口的消息队列。
当释放一个键时，Windows把WM_KEYUP或者WM_SYSKEYUP消息放入消息队列。
可以有多个KEYDOWN，但相对来说只有一个KEYUP。
通过调用GetMessageTime可以获得按下或者释放键的相对时间。
**51 系统击键和非系统击键**
WM_SYSKEYDOWN和WM_SYSKEYUP中的“SYS”代表“系统”，它表示该击键对Windows比对Windows应用程序更加重要。
程序通常可以忽略WM_SYSKEYDOWN和WM_SYSKEYUP消息，并将它们传送到DefWindowProc。
如果想在自己的窗口过程中包括捕获系统击键的代码，那么在处理这些消息之后再传送到DefWindowProc，Windows就仍然可以将它们用于通常的目的。
对所有4类击键消息，wParam是虚拟键代码，表示按下或释放的键。而lParam则包含属于击键的其他数据。
**52 虚拟键码**
虚拟键码保存在WM_KEYDOWN、WM_KEYUP、WM_SYSKEYDOWN、WM_SYSKEYUP消息的wParam参数中。
**53 lParam信息**
在4个击键消息中，wParam消息参数含有虚拟键码，而lParam消息参数则含有对了解击键非常有用的其他信息。
在lParam的32位中，分为6个域。
0~15：重复计数；
16~23：8位OEM；
24：扩展键标志；
29：环境代码；
30：键的先前状态；
31：转换状态。
（1）重复计数
重复计数是该消息所表示的击键次数。大多数情况下，重复计数设置为1。
在KEYDOWN消息中，重复计数可以大于1，表示该键重复n次。
在KEYUP消息中，重复计数总是1.
（2）OEM扫描码
OEM扫描码是由硬件（键盘）产生的代码。Windows程序能够忽略几乎所有的OEM扫描码，除非它取决于键盘的物理布局。
（3）扩展位标识
如果击键结构来自IBM增强键盘的附加键之一，那么扩展键标志为1.
（4）环境代码
环境代码在按下Alt键后为1。对WM_SYSKEYDOWN和WM_SYSKEYUP消息，这一位总是1；对WM_KEYDOWN和WM_KEYUP消息，这一位总是0；
但是有2个例外：
① 如果活动窗口最小化了，则它没有输入焦点。这时候所有的击键都产生WM_SYSKEYDOWN和WM_SYSKEYUP消息。
② 如果Alt键未按下，则环境代码域被设置为0.
（5）键的先前状态
如果在此之前键是释放的，则键的先前状态为0，否则为1.
对WM_KEYUP或者WM_SYSKEYUP消息，它总是设置为1.
对WM_KEYDOWN和WM_SYSKEYDOWN消息，此位可以是1，也可以是0.
（6）转换状态
如果键正在被按下，则转换状态为0；
如果键正在被释放，则转换状态为1.
对WM_KEYDOWN和WM_SYSKEYDOWN消息，此域为0；
对WM_KEYUP或者WM_SYSKEYUP消息，此域为1.
**54 换挡状态**
在处理击键消息是，可能需要知道是否按下了换挡键(Shift, Ctrl, Alt)或开关键(Caps Lock, Num Lock, Scroll Lock)。通过调用GetKeyState函数，就能获得此信息。
int iState；
iState = GetKeyState(VK_SHIFT)；
如果按下了Shift，则iState值为负。(高位被置1)。
**55 字符消息**
在WinMain中，有这样一个消息循环：
while (GetMessage(&msg, NULL, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
GetMessage函数用队列中的下一个消息填充msg结构的字段。
DispatchMessage以此消息为参数调用适当的窗口过程。
TranslateMessage函数将击键消息转换为字符消息。如果消息为WM_KEYDOWN或WM_SYSKEYDOWN，并且击键与换挡状态相结合产生一个字符，则TranslateMessage把字符消息放入消息队列中。此字符消息将是GetMessage从消息队列中得到的击键消息之后的下一个消息。
**56 四类字符消息**
字符消息可以分为四类：WM_CHAR、WM_DEADCHAR和WM_SYSCHAR、WM_SYSDEADCHAR。
其中，WM_CHAR、WM_DEADCHAR是从WM_KEYDOWN消息得到的。WM_SYSCHAR、WM_SYSDEADCHAR是从WM_SYSKEYDOWN消息得到的。
大多数情况下，Windowns程序会忽略除WM_CHAR消息之外的任何消息。
伴随四个字符消息的lParam参数与产生字符代码消息的击键消息的lParam参数相同。不过，参数wParam不是虚拟键码，而是ANSI或Unicode字符代码。
**57 消息顺序**
因为TranslateMessage函数从WM_KEYDOWN和WM_SYSKEYDOWN消息产生了字符消息，所以字符消息是夹在击键消息之间传递给窗口过程的。
（1）如果按下A键，再释放A键，将产生3个消息。
① WM_KEYDOWN   “A”的虚拟键码(0x41)
② WM_CHAR        “a”的字符代码(0x61)
③ WM_KEYUP       “A”的虚拟键码(0x41)
（2）如果按下Shift键和A键，然后释放，将产生5个消息。
① WM_KEYDOWN    虚拟键码VK_SHIFT
② WM_KEYDOWN   “A”的虚拟键码(0x41)
③ WM_CHAR        “a”的字符代码(0x61)
④ WM_KEYUP       “A”的虚拟键码(0x41)
⑤ WM_KEYUP        虚拟键码VK_SHIFT
（3）如果按住A键不释放，将自动重复产生一系列的击键，那么对每个WM_KEYDOWN消息，就会得到一个字符消息。
① WM_KEYDOWN   “A”的虚拟键码(0x41)
② WM_CHAR        “a”的字符代码(0x61)
③ WM_KEYDOWN   “A”的虚拟键码(0x41)
④ WM_CHAR        “a”的字符代码(0x61)
⑤ WM_KEYDOWN   “A”的虚拟键码(0x41)
⑥ WM_CHAR        “a”的字符代码(0x61)
⑦ WM_KEYUP       “A”的虚拟键码(0x41)
**58 处理控制字符**
处理击键和字符消息的基本规则是：如果需要读取输入到窗口的键盘字符，那么可以处理WM_CHAR消息。如果需要读取光标键、功能键、Delete键、Insert键、Shite键、Ctrl键、以及Alt键，那么可以处理WM_KEYDOWN消息。
对于删除、制表、回车、退出键，它们产生WM_CHAR消息和WM_KEYDOWN消息，应该将这些按键处理成控制字符而不是虚拟键码。
case WM_CHAR：
{
  case ‘/b’:    删除键←
      …..
      break;
  case ‘/t’:     制表键tab
       …..
      break;
  case ‘/n’:      回车
      …..
      break;
case ‘/r’:      换行
      …..
      break;
default:
      …..
      break;
}
**59 插入符函数(鼠标闪烁)**
主要有8个插入符函数：
① CreateCaret 创建与窗口相关的插入符；
② SetCaret 在窗口中设置插入符的位置
③ ShowCaret 显示插入符
④ HideCaret 隐藏插入符
⑤ DestroyCaret 撤销插入符
⑥ GetCaretPos 获取插入符位置
⑦ GetCaretBlinkTime 获取插入符闪烁时间
⑧ SetCaretBlinkTime 设置插入符闪烁时间
在Windows中，插入符定义为水平线、与字符大小相同的方框，或者与字符等高的竖线。
只有当窗口有输入焦点时，窗口内显示插入符才有意义。
通过处理WM_SETFOCUS和WM_KILLFOCUS消息，程序就可以确定它是否有输入焦点。窗口过程在有输入焦点的时候接受WM_SETFOCUS消息，失去输入焦点的时候接受WM_KILLFOCUS消息。
使用插入符的规则：窗口过程在WM_SETFOCUS消息期间调用CreateCaret，在处理WM_KILLFOCUS消息期间调用DestroyCaret。
插入符刚创建的时候是隐蔽的，必须显式使用ShowCaret函数将插入符设为可见。
当窗口过程处理一个非WM_PAINT消息而且希望在窗口内绘制某些东西时，必须调用HideCaret隐藏插入符。在绘制完毕之后，再调用ShowCaret显式插入符。
**五 鼠标**
**60 鼠标基础**
用GetSystemMetrics函数来确定鼠标是否存在：
fMouse = GetSystemMetrics(SM_MOUSEPRESENT);
要确定所安装鼠标上键的个数，可使用：
cButtons = GetSystemMetrics(SM_CMOUSEBUTTONS); //如果没有安装鼠标，返回0.
当Windows用户移动鼠标时，Windows在显示屏上移动一个称为“鼠标光标”的小位图。鼠标光标有一个指向显示屏上精确位置的单像素的“热点”。
Windows支持几种预定义的鼠标光标，程序可以使用这些光标。最常见的是称为IDC_ARROW的斜箭头。热点在箭头的顶端。IDC_CROSS光标的热点在十字交叉线的中心。IDC_WAIT光标是一个沙漏，用于指示程序正在运行。
鼠标键动作的术语：
① 单击 按下并放开一个鼠标键
② 双击 快速按下并放开鼠标键两次
③ 拖曳 按住鼠标键并移动鼠标
对于三键鼠标，三个键分别被称为左键、中键、右键。在Windows头文件中定义的与鼠标有关的标识符使用缩写LBUTTON、MBUTTON、RBUTTON。
**61 客户区鼠标消息**
Windows只把键盘消息发送给拥有输入焦点的窗口。鼠标消息与此不同，只要鼠标跨越窗口或者在某窗口中按下鼠标键，那么窗口过程就会收到鼠标消息，而不管该窗口是否活动或者拥有输入焦点。
当鼠标移过窗口的客户区时，窗口过程收到WM_MOUSEMOVE消息。当在窗口的客户区按下或者释放一个鼠标键时，窗口过程会收到如下消息：
|键|按下|释放|双击键|
|----|----|----|----|
|左|WM_LBUTTONDOWN|WM_LBUTTONUP|WM_LBUTTONDBLCLK|
|中|WM_MBUTTONDOWN|WM_MBUTTONUP|WM_MBUTTONDBLCLK|
|右|WM_RBUTTONDOWN|WM_RBUTTONUP|WM_RBUTTONDBLCLK|
仅当定义的窗口类能接收DBLCLK消息之后，窗口过程才能接收到双击消息。
对于所有这些消息来说，其lParam值均含有鼠标的位置：低位是x坐标，高位是y坐标。这两个坐标是相对于窗口客户区左上角的位置。
wParam的值指示鼠标键和Shift和Ctrl键的状态。MK_前缀代表“鼠标键”。
MK_LBUTTON 按下左键
MK_MBUTTON 按下中键
MK_RBUTTON 按下右键
MK_SHIFT 按下Shift键
MK_CONTROL 按下Ctrl键
如果接收到WM_LBUTTONDOWN消息，而且值wParam & MK_SHIFT是TRUE，就知道当左键按下时，也按下了右键。
当把鼠标移过窗口的客户区时，Windows并不为鼠标的每个可能的像素位置都产生一条WM_MOUSEMOVE消息。程序接收到WM_MOUSEMOVE消息的次数取决于鼠标硬件以及窗口过程处理鼠标移动消息的速度。
**62 处理Shift键**
要判断鼠标移动时，是否按下了Shift键，可以通过wParam进行判断，具体方法如下：
if (wParam & MK_SHIFT)
{
    if (wParam & MK_CONTROL)
    {
        //按下了Shift和Ctrl键
    }
    else
{
//按下了Shift键
        }
}
**63 双击鼠标键**
要确定为双击，这两次单击必须发生在其相互的物理位置十分接近的状况下，默认时范围是一个平均系统字体字符的宽，半个字符的高，并且发生在指定的时间间隔内。
如果希望窗口过程能够接收到双击键的鼠标消息，那么在调用RegisterClass初始化窗口类结构时，必须在窗口风格中包含CS_DBCLKS标识符。
wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBCLKS;
如果在窗口风格中未包含CS_DBCLKS，那么用户在短时间内双击了鼠标键，窗口过程将接收到如下消息：
WM_LBUTTONDOWN
WM_LBUTTONUP
WM_LBUTTONDOWN
WM_LBUTTONUP
如果窗口类风格中包含了CS_DBCLKS，那么双击鼠标键时窗口过程将接收到如下消息：
WM_LBUTTONDOWN
WM_LBUTTONUP
WM_LBUTTONDBCLK
WM_LBUTTONUP
**64 非客户区鼠标消息**
在窗口的客户区内移动或按下鼠标键时，将产生10个消息。如果鼠标在窗口的客户区之外，但在窗口内，Windows将给窗口过程发生一个“非客户区”鼠标消息。窗口非客户区包括标题栏、菜单、和窗口滚动条。
通常不需要处理非客户区鼠标消息，而将这些消息传给DefWindowProc，从而使Windows执行系统功能。
消息中包含字母“NC”以表示是非客户区消息。
如果鼠标在窗口的非客户区移动，那么窗口过程接收到WM_NCMOUSEMOVE消息。其他动作产生如下消息：
|键|按下|释放|双击|
|----|----|----|----|
|左|WM_NCLBUTTONDOWN|WM_NCLBUTTONUP|WM_NCLBUTTONDBCLK|
|中|WM_NCMBUTTONDOWN|WM_NCMBUTTONUP|WM_NCMBUTTONDBCLK|
|右|WM_NCRBUTTONDOWN|WM_NCRBUTTONUP|WM_NCRBUTTONDBCLK|
非客户区鼠标消息的wParam和lParam参数意义如下：
wParam：指明移动或者单击鼠标键的非客户区位置。以HT开头。
lParam：包含低位字的x坐标和高位字的y坐标，但是它们都是屏幕坐标，而不是客户区坐标。
使用以下两个函数将屏幕坐标和客户区坐标互换。
ScreenToClient(hwnd, &pt);
ClientToScreen(hwnd, &pt);
如果屏幕坐标点在窗口客户区的上面或者左边，客户区坐标x或y值就是负值。
**65 命中测试**
WM_NCHITTEST代表“非客户区命中测试”。此消息优先于所有其他的客户区和非客户区鼠标消息。lParam参数含有鼠标位置的x和y屏幕坐标。wParam参数没有用。
Windows应用程序通常把这个消息传送给DefWindowProc，然后Windows用WM_NCHITTEST消息产生基于鼠标位置的所有其他鼠标消息。对于非客户区鼠标消息，在处理WM_NCHITTEST消息时，从DefWindowProc返回的值将成为鼠标消息中的wParam参数，这个值可以是任意非客户区鼠标消息的wParam值再加上以下内容：
HTCLIENT        客户区
HTNOWHERE     不在窗口中
HTTRANSPARENT 窗口由另一个窗口覆盖
HTERROR         使DefWindowProc产生蜂鸣声
如果DefWindowProc在其处理WM_NCHITTEST消息后返回HTCLIENT，那么Windows将把屏幕坐标转换为客户区坐标并产生客户区鼠标消息。
**66 从消息产生消息**
如果在一个Windows程序的系统菜单图标上双击一下，那么程序将会终止。双击产生一些了的WM_NCHITTEST消息。由于鼠标定位在系统菜单图标上，所以DefWindowProc将返回HTSYSMENU的值，并且Windows把wParam等于HTSYSMENU的WM_NCLBUTTONDBCLK消息放在消息队列中。
当DefWindowProc接收到wParam参数为HTSYSMENU的WM_NCLBUTTONDBCLK消息时，就把wParam参数为SC_CLOSE的WM_SYSCOMMAND消息放入消息队列中。同样，窗口过程也把这个消息传给DefWindowProc。DefWindowProc通过给窗口过程发生WM_CLOSE消息来处理该消息。
如果一个程序在终止前需要来自用户的确认，那么窗口过程就必须捕获WM_CLOSE，否则，DefWindowProc将调用DestroyWindow函数来处理WM_CLOSE。
**67 捕获鼠标**
捕获鼠标，只要调用SetCapture(hwnd)；在这个函数调用之后，Windows将所有鼠标消息发给窗口句柄为hwnd的窗口过程。鼠标消息总是客户区消息，即使鼠标正在窗口的非客户区。lParam参数将指示鼠标在客户区坐标中的位置。不过，当鼠标位于客户区的左边或者上方的时候，这些x和y坐标可以是负的。
当需要释放鼠标时，调用ReleaseCapture();就可恢复正常。
如果鼠标被捕获，而鼠标键并没有被按下，并且鼠标光标移到了另一个窗口上，那么将不是由捕获鼠标的那个窗口而是由光标下面的窗口来接收鼠标消息。
**68 鼠标轮**
鼠标轮的转动产生一个WM_MOUSEWHEEL消息。
lParam参数将获得鼠标的位置，坐标是相对于屏幕左上角的，不是客户区的。
wParam参数低字包含一系列的标识，用于表明鼠标键和Shift与Ctrl键的状态。
wParam的高字中有一个“delta”值，该值默认可以是120或-120，这取决于滚轮是向前转动还是向后转动。值120或-120表明文档将分别向上或向下滚动三行。
**六 计时器**
**69 计时器基础**
计时器是一种输入设备，它周期性地每经过一个指定的时间间隔就用WM_TIMER消息通知应用程序一次。
可以通过调用SetTimer函数为Windows应用程序分配一个计时器。SetTimer有一个时间间隔范围为1~4294967295毫秒的整型参数，这个值指示Windows每隔多长时间给程序发送WM_TIMER消息。
当程序用完计时器时，就调用KillTimer函数停止计时器消息。
KillTimer调用清除消息队列中尚未被处理的WM_TIMER消息，从而使程序在调用KillTimer之后就不会再受到WM_TIMER消息。
**70 系统和计时器**
Windows计时器是PC的硬件和ROM BIOS构造的计时器逻辑的一种相对简单的扩展。
BIOS的“计时器滴答”中断约每54.915毫秒或者大约每秒18.2次。
在Microsoft Windows NT中，计时器的分辨率为10毫秒。
Windows应用程序不能以高于这个分辨率的速率接收WM_TIMER消息。在SetTimer调用中指定的时间间隔总是截尾为时钟滴答的整数倍。例如，1000毫秒的间隔除以54.925毫秒≈18.207个时钟滴答，截尾后为18个时钟滴答，它实际上是989毫秒。对每个小于55毫秒的间隔，每个时钟滴答都产生一个WM_TIMER消息。
**71 计时器消息不是异步的**
计时器是基于硬件计时器中断。但WM_TIMER消息却不是异步的。
WM_TIMER消息放在正常的消息队列之中，和其他消息一起参加排序，因此，如果在SetTimer调用中指定间隔为1000毫秒，那么不能保持程序每1000毫秒就会收到一个WM_TIMER消息。如果其他程序的运行事件超过一秒，在此期间，程序将不会收到任何WM_TIMER消息。
Windows不能持续向消息队列放入多个WM_TIMER消息，而是将多余的WM_TIMER消息合并成一个消息。
**72 计时器的使用（方法一）**
如果需要在整个程序期间使用计时器，那么可以在处理WM_CREATE消息时调用SetTimer，并在处理WM_DESTROY消息时调用KillTimer。
SetTimer函数如下所示：
SetTimer(hwnd, 1, uiMsecInterval, NULL);
第一个参数是其窗口过程将接收WM_TIMER消息的窗口的句柄；
第二个参数是计时器ID，只要是非0的整数就可以，如果设置多个计时器，那么各个计时器的ID应该不同。
第三个参数是一个32位无符号整数，以毫秒为单位指定一个时间间隔。
第四个参数是回调函数的地址，如果处理WM_TIMER消息不是回调函数，那么设置为NULL。
KillTimer函数调用如下：
KillTimer(hwnd, 1)；
第一个参数是其窗口过程将接收WM_TIMER消息的窗口的句柄；
第二个参数是计时器ID。
KillTimer用于在任何时刻停止WM_TIMER消息。
当窗口过程收到一个WM_TIMER消息时，wParam参数等于计时器的ID值，lParam参数为0.如果需要设置多个计时器，那么对每个计时器都使用不同的计时器ID。wParam的值将随传递到窗口过程的WM_TIMER消息的不同而不同。
**73 计时器的使用（方法二）**
第一种方法是把WM_TIMER消息发送到通常的窗口过程。
第二种方法是让Windows直接将计时器消息发送给程序的另一个函数。
接收这些计时器消息的函数称为“回调函数”，这是一个在程序之中，但是由Windows而不是程序本身调用的函数。先告诉Windows这个函数的地址，然后Windows调用此函数。
窗口过程其实就是一种回调函数。
回调函数必须定义为CALLBACK，因为它是由Windows从程序的代码段调用的。回调函数的参数和回调函数的返回值依赖于回调函数的目的。同计时器相关的回调函数中，输入参数同窗口过程的输入参数一样。计时器回调函数不向Windows返回值，可以设置为VOID。
假设计时器回调函数称为TimerProc，那么可以定义如下：
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    WM_TIMER消息的处理过程；
}
TimerProc的参数hwnd是在调用SetTimer时指定的窗口句柄。Windows只把WM_TIMER消息消息送过TimerProc，因此消息参数总是等于WM_TIMER。iTimerID值是计时器ID，dwTimer值是与从GetTickCount函数的返回值兼容的值，是反映Windows启动后所经过的毫秒数。
在使用回调函数处理WM_TIMER消息时，窗口过程中设置SetTimer的第4个参数由回调函数的地址取代，如下所示：
SetTimer(hwnd, iTimerID, iMsecInterval, TimerProc);
回调函数必须和窗口过程函数一样，一起被声明在程序的开始处，像TimerProc的函数声明如下：
VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD);
**74计时器的使用（方法三）（少用）**
设置计时器的第三种方法类似于第二种方法，只是传递给SetTimer的hwnd参数被设置为NULL，并且第二个参数计时器ID也被忽略，设置为0，最后才函数返回计时器ID。
iTimerID = SetTimer(NULL, 0, iMsecInterval, TimerProc);
如果没有可用的计时器，那么从SetTimer返回的iTimerID值将为NULL。
KillTimer的第一个参数也必须是NULL，计时器ID必须是SetTimer的返回值。
传递给TimerProc计时器函数的hwnd参数也必须是NULL。
**75 获取当前时间**
首先介绍下SYSTEMTIME结构，如下所示：
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME；
SYSTEMTIME结构包含日期和时间。月份由1开始递增，星期由0开始递增（星期天是0）。wDay是本月的当前日子，从1开始递增。
SYSTEM主要用于GetLocalTime和GetSystemTime函数。
GetSystemTime函数返回当前的世界时间（格林尼治时间）
GetSystemTime函数返回当地时间。
**76 WM_SETTINGCHANGE消息**
如果用户改变了任何系统设置，Windows会产生WM_SETTINGCHANGE消息，并传送给所有的应用程序。
**七 子窗口控制**
**77 子窗口控制概述**
当子窗口的状态发生改变时，子窗口处理鼠标和键盘消息并通知父窗口。子窗口这时就变成了其父窗口高一级的输入设备。
可以建立自己的子窗口控制，也可以利用一些预定义的窗口类和窗口过程来建立标准的子窗口控制。
子窗口控制采用的形式有：按钮、复选框、编辑框、列表框、组合框、文本串、滚动条。
子窗口控制在对话框中最常用。
子窗口控制的位置和尺寸是在程序的资源描述文中的对话框模板中定义的。也可以使用预定义的、位于普通窗口客户区表面的子窗口控制。可以调用一次CreateWindow来建立一个子窗口，并通过调用MoveWindow来调整子窗口的位置和尺寸。父窗口过程向子窗口控制发送消息，子窗口控制向父窗口过程返回消息。
**78 按钮类**
按钮属于窗口。
按钮窗口风格都以字母BS开头，它代表“按钮风格”。按钮使用CreateWindow创建。
HWND hwndButton；
hwndButton = CreateWindow{
参数1 ClassName 类名，
参数2 Window text 按钮显示的文本，
参数3 Window Style 窗口风格 有WS_CHILD | WS_VISIBLE | 按钮种类
参数4 x位置
参数5 y位置 说明按钮左上角相对于父窗口客户区左上角的位置
参数6 Width 宽度 按钮宽度
参数7 Height 高度 按钮的高度
参数8 父窗口句柄
参数9 子窗口ID（强制HMENU类型）
参数10 实例句柄（（LPCREATESTRUCT）lParam -> hInstance）
参数11 额外参数（一般为NULL）}
注：参数3中的按钮种类分为：
下压按钮：BS_PUSHBUTTON、BS_DEFPUSHBUTTON 下压按钮
复选框：BS_CHECKBOX、BS_AUTOCHECKBOX
三状态复选框：BS_3STATE、BS_AUTO3STATE
单选按钮：BS_RADIOBUTTON、BS_AUTORADIOBUTTON
组合框：BS_GROUPBOX
自定义按钮：BS_OWNEDRAW
参数9的子窗口ID对于每个子窗口都不同。在处理来自子窗口的WM_COMMAND消息时，ID帮助窗口过程识别出相应的子窗口。子窗口ID必须被强制转换为HMENU。
参数10的实例句柄利用了如下事实：在处理WM_CREATE消息的过程中，lParam实际上是指向CREATESTRUCT结构的指针，该结构有一个hInstance成员。
也可以使用GetWindowLong(hwnd, GWL_HINSTANCE)函数调用来获取实例句柄。
**79 子窗口向父窗口发送消息**
当用鼠标点击按钮时，子窗口控制就向其父窗口发送一个WM_COMMAND消息。父窗口过程捕获WM_COMMAND消息，其wParam和lParam消息参数含义如下：
LOWORD(wParam)   子窗口ID
HIWORD(wParam)    通知码
lParam              子窗口句柄
子窗口ID是在创建子窗口时传递给CreateWindow的值。
通知码详细表明了消息的含义。
当用鼠标单击按钮时，该按钮文本的周围会有虚线。这表明该按钮拥有了输入焦点，所有键盘输入都将传送给子窗口按钮控制。然后按钮即使拥有输入焦点，也只能处理空格键。
**80 父窗口向子窗口发送消息**
父窗口可以给子窗口发送消息，这些消息包括以前缀为WM开头的许多消息。另外，还有8个按钮说明消息，以BM开头。
BM_GETCHECK：获取复选框和单选按钮的选中标记
BM_SETCHECK：设置复选框和单选按钮的选中标记
BM_GETSTATE：获取按钮状态（正常还是按下）
BM_SETSTATE：设置按钮状态
BM_GETIMAGE
BM_SETIMAGE
BM_CLICK
BM_SETSTYLE：允许在按钮创建后改变按钮风格
每个子窗口控制都具有一个在其兄弟中唯一的窗口句柄和ID值，对于句柄和ID值这两者，知道其中一个就可以获得另一个。
id = GetWindowLong(hwndChild, GWL_ID);
hwndChild = GetDlgItem(hwndParent, id);
**81 下压按钮**
下压按钮控制主要用来触发一个立即响应的动作，而不保**何开关指示。有两种类型的按钮控制窗口风格。分别是BS_PUSHBUTTON和BS_DEFPUSHBUTTON。
当用来设计对话框时，两种风格作用不同，但当用作子窗口控制时，两种类型的按钮作用相同。
当按钮的高度为文本字符高度的7/4倍时，按钮的外观最好。
按钮的文本尺寸除了之前介绍的从TEXTMETRIC结构中获取之外，更简便的方法是通过GetDialogBaseUnits函数来获得默认字体字符的高度和宽度。
此函数返回一个32位的值，其中低字位表示宽度，高字位表示高度。
当鼠标在按钮中按下时，按钮使用三维阴影重画自己。
当鼠标放开时，就恢复按钮原因，并向父窗口发送一个WM_COMMAND消息和BN_CLICKED通知码。
**82 复选框**
复选框是一个文本框，文本通常出现在复选框的右边。复选框通常用于允许用户对选项进行选择的应用程序中。复选框的常用功能如同一个开关：单击一次显示复选标记，再次单击清除复选标记。
复选框最常用的2种风格是BS_CHECKBOX和BS_AUTOCHECKBOX。在使用BS_CHECKBOX时，需要自己向该控制发送BM_SETCHECK消息来设置复选标记。wParam参数置1时设置复选标记，置0时清除复选标记。通过向该控制发送BM_GETCHECK消息，可以得到该复选框的当前状态。
可以用如下指令来翻转复选标记：
SendMessage((HWND)lParam, BM_SETCHECK, (WPARAM)
!SendMessage((HWND)lParma, BM_GETCHECK, 0, 0), 0);
对于BS_AUTOCHECKBOX风格，按钮自己触发复选标记的开和关，窗口过程可以忽略WM_COMMAND消息。当需要知道按钮的当前状态时，可以向控制发送BM_GETCHECK消息：
iCheck = (int)SendMessage(hwndButton, BM_GETCHECK, 0, 0);
如果按钮被选中，则iCheck返回非0值。
其余两种复选框风格是BS_3STATE和BS_AUTO3STATE，这两种风格能显示第三种状态——复选框内是灰色的——它出现在向控制发送wParam = 2的WM_SETCHECK消息时。
**83 单选按钮**
单选按钮的形状是个圆圈。圆圈内的加重圆点表示该单选按钮已经被选中。单选按钮有窗口风格BS_RADIOBUTTON或BS_AUTORADIOBUTTON两种，后者只用于对话框。
当收到来自单选按钮的WM_COMMAND消息时，应该向它发送wParam等于1的BM_SETCHECK消息来显示其选中状态：
SendMessage(hwndButton, BM_SETCHECK, 1, 0);
对相同组中的其他所有单选按钮，可以通过向它们发送wParam等于0的BM_SETCHECK消息来显示其未选中状态。
SendMessage(hwndButton, BM_SETCHECK, 0, 0);
**84 分组框**
   分组框即风格为BS_GROUPBOX的选择框，它不处理鼠标输入和键盘输入，也不向其父窗口发送WM_COMMAND消息。分组框是一个矩形框，窗口文本在其顶部显示。分组框常用来包含其他的按钮控制。
**85 更改按钮文本**
可以通过调用SetWindowText来更改按钮内的文本：
SetWindowText(hwnd, pszString);
hwnd是窗口句柄，pszString是一个指向NULL终结串的指针。
对于一般的窗口，更改的是窗口的标题栏文本，对于按钮控制来说，更改的是按钮的显示文本。
可以获取窗口的当前文本：
iLength = GetWindowText(hwnd, pszBuffer, iMaxLength);
iMaxLength指定复制到pszBuffer指向的缓冲区中的最大字符数，该函数返回复制的字符数。
可以通过调用
iLength = GetWindowTextLength(hwnd)；获取文本的长度。
**86 可见和启用的按钮**
为了接受鼠标和键盘输入，子窗口必须是可见的和被启用的。当窗口是可见的但是非启用时，窗口以灰色显示正文。
如果在建立子窗口时，没有将WS_VISIBLE包含在窗口类中，那么直到调用
ShowWindow(hwndChild, SW_SHOWNORMAL);
时子窗口才被显示出来。
调用ShowWindow(hwndChild, SW_HIDE);
将子窗口隐藏起来。
使用EnableWindow(hwndChild, TRUE)来启用窗口。
**87 按钮和输入焦点**
当Windows将输入焦点从一个窗口转换到另一个窗口时，首先给正在失去输入焦点的窗口发送一个WM_KILLFOCUS消息，wParam参数是接收输入焦点的窗口的句柄。然后，Windows向正在接收输入焦点的窗口发送一个WM_SETFOCUS消息，同时wParam参数是正在失去输入焦点的窗口的句柄。
可以通过调用SetFocus来恢复输入焦点，如：
case WM_KILLFOCUS：
if (hwnd == GetParent((HWND)wParam))
        SetFoucs(hwnd);
    return 0;
**88 静态类**
在CreateWindow函数中指定窗口类为“static”，就可以建立静态的子窗口控制。这些子窗口既不接收鼠标或键盘输入，也不向父窗口发送WM_COMMAND消息。
当在静态子窗口上移动或按下鼠标时，这个子窗口将捕获WM_NCHITTEST消息，并将HTTRANSPARENT的值返回给Windows，这将使Windows向其下层窗口发送相同的WM_NCHITTEST消息。
**89 滚动条类**
滚动条类是可以在父窗口的客户区的任何地方出现的子窗口。可以使用预先定义的窗口类“scrollbar”以及两个滚动条风格SBS_VERT和SBS_HORZ中的一个来建立子窗口滚动条控制。
与按钮控制不同，滚动条控制不向父窗口发送WM_COMMAND消息，而是像窗口滚动条一样发送WM_VSCROLL和WM_HSCROLL消息。在处理滚动条消息时，可以通过lParam参数来区分开窗口滚动条与滚动条控制。对于窗口滚动条，其值是0.对于滚动条控制，其值是滚动条窗口句柄。对于窗口滚动条和滚动条控制来说，wParam参数的高位字和低位字的含义相同。
窗口滚动条有固定的宽度，但滚动条控制可以自己修改尺寸。使用CreateWindow调用时，给出矩形尺寸来确定滚动条控制的尺寸。
如果想建立与窗口滚动条相同的滚动条控制，那么可以使用GetSystemMetrics获取水平滚动条的高度GetSystemMetrics(SM_CYHSCROLL)或者垂直滚动条的宽度GetSystemMetrics(SM_CXVSCROLL);
滚动条窗口风格标识符SBS_LEFTALIGN、SBS_RIGHTALIGN、SBS_TOPALIGN和SBS_BUTTOMALIGN给出滚动条的标准尺寸，但是这些风格只在对话框中对滚动条有效。
对于滚动条控制，可以使用如下调用来设置范围和位置：
SetScrollRange(hwndScroll, SB_CTL, iMin, iMax, bRedraw)
SetScrollPos(hwndScroll, SB_CTL,iPos, bRedraw)
SetScrollInfo(hwndScroll, SB_CTL, &si, bRedraw)
滚动条两端按钮之间较大的区域颜色是有COLOR_BTNFACE和COLOR_BTNHIGHLIGHT一起来确定的。
如果捕获了WM_CTLCOLORSCROLLBAR消息，那么可以在消息处理中返回画刷以取代该颜色。
**90 窗口子类化**
一般我们的消息都是传给Windows程序的WndProc窗口过程的，但是当窗口内有子窗口控制时，我们可以给这个子窗口设置一个新的窗口过程，这个技术叫做“窗口子类化”。它能让我们给现存的窗口过程（新的）设置“钩子”，以便在程序中处理一些消息，同时将其他所有消息传递给旧的窗口过程（WndProc）。
Win32的子类化的原理是靠拦截Windows系统中的某些消息来自己进行处理，而不是交给WndProc或DefWindowProc。
将GWL_WNDPROC标识符作为参数来调用GetWindowLong，可以得到这个窗口过程的地址。
可以调用SetWindowLong给子窗口设置一个新的窗口过程。
可以用函数指针的办法，将我们感兴趣的消息拦截下来，处理完之后再让预定义的窗口过程处理。这个过程大致如下：
WNDPROC OldProc;（用来保存旧的WndProc窗口过程）
OldProc = (WNDPROC)SetWindowsLong(hWnd, GWL_WNDPROC, (LONG)NewProc);
当然，这里的新窗口过程NewProc是预先由你实现好的。上述代码执行以后，系统在处理hwnd的窗口消息时，就会先进入你实现的NewProc回调过程，然后在处理过你感兴趣的消息之后，通过CallWindowProc函数和你预先保存的OldProc再次回到原来的回调过程中完成剩余的工作。
**91 编辑类**
当建立子窗口时，CreateWindow第一个参数，即类名使用“edit”，根据CreateWindow调用中的x位置、y位置、宽度、高度等这些参数定义了一个矩形。此矩形含有可编辑文本。当子窗口控制拥有输入焦点时，可以输入文本，移动光标，使用鼠标或者Shift键与一个光标键来选取部分文本，也可以删除、剪切、复制、粘帖文本。
**92 编辑类风格**
① 是WS_CHILD风格。
② 是编辑控制中的文本对齐方式，可以左对齐ES_LEFT、右对齐ES_RIGHT、居中ES_CENTER。
③ 编辑控制是单行文本还是多行文本，默认是单行，如果要处理回车键，需要增加风格ES_MULTILINE。
④ 滚动条功能，纵向是ES_AUTOVSCROLL，横向是ES_AUTOHSCROLL。
⑤ 边框，默认是没边框的，可以使用风格WS_BORDER。
**93 编辑控制通知**
编辑控制给父窗口过程发生WM_COMMAND消息，wParam和lParam参数和按钮控制一样。
LOWORD(wParam)   子窗口ID
HIWORD(wParam)    通知码（EN开头）
lParam              子窗口句柄
通知码如下所示：
EN_SETFOCUS 获得输入焦点
EN_KILLFOCUS 失去输入焦点
EN_CHANGE内容将改变
EN_UPDATE 内容已经改变
EN_ERRSPACE 输入的文本超过30000个字符
EN_MAXTEXT 插入之后的文本超过30000个字符
EN_HSCROLL 编辑控制的水平滚动条被单击
EN_VSCROLL 编辑控制的垂直滚动条被单击
**94 发送给编辑控制的消息**
发送给编辑控制的消息运行剪切、复制、清除当前的选择。用户使用鼠标或Shift键减少光标控制键选择文本并进行上面的操作。
剪切：SendMessage(hwndEdit, WM_CUT, 0, 0);
复制：SendMessage(hwndEdit, WM_COPY, 0, 0);
清除；SendMessage(hwndEdit, WM_CLEAR, 0, 0);
粘帖：SendMessage(hwndEdit, WM_PASTE, 0, 0);
获取当前选中文本的起始位置和末尾位置：
SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&iStart, (LPARAM)&iEnd);
末尾位置实际是最后一个选择字符的位置加1。
选择文本：
SendMessage(hwndEdit, EM_SETSEL, iStart, iEnd);
文本置换：
SendMessage(hwndEdit, EM_REPLACESEL, 0, (LPARAM)szString);
获取多行文本的行数：
iCount = SendMessage(hwndEdit, EM_GETLINECOUNT, 0, 0);
对任何特定的行，可以获取距离编辑缓冲区文本开头的偏移量：
iOffset = SendMessage (hwndEdit, EM_LINEINDEX, iLine, 0);
其中，行数从0开始计算，iLine值为-1时返回包含光标所在行的偏移量。
获取行的长度：
iLength = SendMessage (hwndEdit, EM_LINELENGTH, iLine, 0);
将一行复制到一个缓冲区：
iLength = SendMessage (hwndEdit, EM_GETLINE, iLine, (LPARAM) szBuffer) ;
**95 列表框类**
列表框也属于子窗口控制。列表框是文本串的集合，这些文本串是一个矩形中可以滚动显示的列状列表。程序通过向列表框窗口过程发送消息，可以在列表中增加或者删除串。当列表框中的某项被选定时，列表框控制就向其父窗口发送WM_COMMAND消息，父窗口也就可以确定选定是哪一项。
列表框可以是单选的，也可以是多选的。选定的项被加亮显示，并且是反显的。
在单项选择的列表框中，用户按空格键就可以选定光标所在位置的项。方向键移动光标和当前选择指示，并且能够滚动列表框的内容。
**96 列表框风格**
当使用CreateWindow建立列表框子窗口时，应该将“listbox”作为窗口类，将WS_CHILD作为窗口风格。但是，这个默认列表框风格不向其父窗口发送WM_COMMAND消息。所以，一般都要包括列表框风格标识符LBS_NOTIFY。它允许父窗口接收来自列表框的WM_COMMAND消息。如果希望列表框对其中各项进行排序，那么可以使用另一个风格LBS_SORT。
如果想建立一个多选选择的列表框，那么可以使用风格LBS_MULTIPLESEL。
默认的列表框是无边界的，所以一般都要加上WS_BORDER来加上边界。
使用WS_VSCROLL来增加垂直滚动条。
有一个列表框风格，综合了上述各种风格，那就是LBS_STANDARD风格。
**97 将文本串放入列表框**
将文本串放入列表框可以通过调用SendMessage给列表框窗口过程发消息来实现这一点。文本串通常通过以0开始计数的下标数来引用，其中0对应于最顶上的项。
一般子窗口列表框控制的句柄定义为hwndList
下标值定义为iIndex
在使用SendMessage传递文本串的情况下，lParam参数是指向null结尾串的指针。
当窗口过程存储的列表框内容超过了可用内存空间时，SendMessage将返回LB_ERRSPACE(定义为-2)。如果是其他原因出错，那么将返回LB_ERR(-1).
如果采用LBS_SORT风格，那么填充列表框最简单的方法是借助LB_ADDSTRING消息：SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)szString);
如果没有采用LBS_SORT，那么可以使用LB_INSERTSTRING指定一个下标值，将字符串插入到列表框中：
SendMessage (hwndList, LB_INSERTSTRING, iIndex, (LPARAM) szString) ;
如果iIndex等于4，那么szString将变为下标值为4的串——从顶头开始算起的第5个串。下标值为-1时，将串增加在最后。
可以在指定下标值的同时使用LB_DELETESTRING参数，这就可以从列表框中删除串：
SendMessage (hwndList, LB_DELETESTRING, iIndex, 0)；
可以使用LB_RESETCONTENT清除列表框中的所有内容：
SendMessage (hwndList, LB_RESETCONTENT, 0, 0)；
**98 选择和获取项**
获取列表框项数：（LB_GETCOUNT）
iCount = SendMessage (hwndList, LB_GETCOUNT, 0, 0);
加亮选中项：（LB_SETCURSEL）
SendMessage (hwndList, LB_SETCURSEL, iIndex, 0)；
将lParam设置为-1，取消所有选择。
根据项的第一个字母来选择：（LB_SELSECTSTRING）
iIndex = SendMessage (hwndList, LB_SELECTSTRING, iIndex, (LPARAM) szSearchString)；
iIndex等于-1时，从头开始搜索。
当得到来自列表框的WM_COMMAND消息时，可以通过使用LB_GETCURSEL来确定当前选项的下标:（LB_GETCURSEL）
iIndex = SendMessage (hwndList, LB_GETCURSEL, 0, 0);
可以确定列表框中串的长度：（LB_GETTEXTLEN）
iLength = SendMessage (hwndList, LB_GETTEXTLEN, iIndex, 0)；
可以将某项复制到文本缓冲区：（LB_GETTEXT）
iLength = SendMessage (hwndList, LB_GETTEXT, iIndex, (LPARAM) szBuffer);
**99 接收来自列表框的消息**
当用户用鼠标单击列表框时，列表框将接收输入焦点。
列表框控制向其父窗口发送WM_COMMAND消息，对按钮和编辑控制来说wParam和lParam参数的含义是相同的。
LOWORD(wParam)   子窗口ID
HIWORD(wParam)    通知码（LBN开头）
lParam              子窗口句柄
通知码及其值如下所示：
LBN_ERRSPACE    -2  表示列表框已经超出运行空间
LBN_SELCHANGE   1  表明当前选择已经被改变
LBN_DBLCLK       2  表明某项已经被鼠标双击
LBN_SELCANCEL   3
LBN_SETFOCUS    4  列表框获得焦点
LBN_KILLFOCUS    5 列表框失去焦点
只有列表框窗口风格包括LBS_NOTIFY时，列表框控制才向父窗口发送LBN_SELCHANGE和LBN_DBLCLK码
**100 文件列表**
LB_DIR是功能最强的列表框消息，它用文件目录表填充列表框，并且可以选择将子目录和有效的磁盘驱动器也包括进来：
SendMessage(hwndList, LB_DIR, iAttr, (LPARAM)szFileSpec);
① 使用文件属性码：
|iAttr|值|属性|
|----|----|----|
|DDL_READWRITE|0x0000|普通文件|
|DDL_READONLY|0x0001|只读文件|
|DDL_HIDDEN|0x0002|隐藏文件|
|DDL_SYSTEM|0x0004|系统文件|
|DDL_DIRECTORY|0x0010|子目录|
|DDL_ARCHIVE|0x0020|有归档位集的文件|
|DDL_DRIVES|0x4000|包括驱动器的盘符|
|DDL_EXCLUSIVE|0x8000|互斥搜索|
当LB_DIR消息的iAttr值为DDL_READWRITE时，列表框列出普通文件、只读文件和具有归档位集的文件。
当值为DDL_DIRECTORY时，列表框除列出上述文件之外，还列出子目录，目录位于方括号之内。
当值为DDL_DRIVES | DDL_DIRECTORY时，那么列表将扩展到包括所有有效的驱动器，驱动器字母显示在虚线之间。
当值为DDL_EXCLUSIVE | DDL_ARCHIVE时，即将iAttr的最高位置位可以列出带标志的文件，而不包括普通文件。
② 文件列表的排序
lParam参数是指向文件说明串如“*.*”的指针，这个文件说明串不影响列表框中的子目录。
用户也许希望给列有文件清单的列表框使用LBS_SORT消息，列表框首先列出符合文件说明的文件，再列出子目录名。列出的第一个子目录名将采用下面的格式：
[..]
这种两个点的子目录项允许用户向根目录返回一级。
最后，具体的子目录名采用下面的形式：
[SUBDIR]
后面是以下面的形式列出的有效磁盘驱动器
[-A-]
