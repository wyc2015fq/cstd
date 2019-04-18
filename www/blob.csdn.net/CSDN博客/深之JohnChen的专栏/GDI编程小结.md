# GDI编程小结 - 深之JohnChen的专栏 - CSDN博客

2010年10月18日 20:20:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：31200


图形设备接口(GDI)是一个可执行程序，它接受Windows应用程序的绘图请求(表现为GDI函数调用)，并将它们传给相应的设备驱动程序，完成特定于硬件的输出，象打印机输出和屏幕输出。GDI负责Windows的所有图形输出，包括屏幕上输出像素、在打印机上输出硬拷贝以及绘制Windows用户界面。也就是Windows的图形编程。

一、GDI体系结构

1、GDI32.DLL导出的函数

GDI提供几百个Windows程序中可以调用的函数。这些函数大多数是从Win32的子系统DLL GDI32.DLL中导出的。窗口管理模块UER32.DLL是使用GDI函数的大用户，它用GDI函数来绘制菜单、图标、滚动条、标题栏和每个窗口的框架等细节内容。有一些绘图函数从USER32.DLL导出，提供给应用程序。仅Windows2000 GDI32.DLL就导出了543个入口点。与DevStudio一起发行的dumpbin工具是一个简单的工具，能列出模块导出的函数。下面就是由dumpbin gdi32.dll /export 产生的部分内容：

AbortDocAbortPathAddFontMemResourceExAddFontResourceAAddFontResourceExAAddFontResourceExWAddFontResourceTrackingAddFontResourceWAngleArcAnimatePaletteAnyLinkedFontsArcArcTo具体查看：[http://www.cnblogs.com/del/archive/2008/03/11/1101291.html](http://www.cnblogs.com/del/archive/2008/03/11/1101291.html)![](http://hi.csdn.net/attachment/201010/18/0_1287403379W84B.gif)

2、GDI函数分类

GDI的功能太多了，所以我们需要一种办法对Win32 GDI API的函数分类，以便理解GDI的结构，MSDN库将GDI API分成17个领域，清楚地描述了GDI的功能。

(1)位图：处理创建、绘制设备相关位图(DDB)、设备无关位图(DIB)、DIB段、像素和区域填充的函数。

(2)画刷：处理创建、修改GDI画刷对象的函数。

(3)剪裁：处理设备上下文可绘制区域的函数。

(4)颜色：调色板管理。

(5)坐标和变换：处理映射模式、设备坐标映射逻辑和通用变换矩阵的函数。

(6)设备上下文：创建设备上下文，查询、设置其属性，及选择GDI对象的函数。

(7)填充形状：绘制闭合区域及其周线的函数。

(8)字体和文本：在系统中安装和枚举字体，并用它们绘制文本字符串的函数。

(9)直线和曲线：绘制直线、椭圆曲线和贝赛尔曲线的函数。

(10)元文件：处理Windows格式的元文件或增强型元文件的生成和回放的函数。

(11)多显示监视器：允许在一个系统中使用多个显示监视器的函数。这些函数实际上是从uer32.dll导出的。

(12)画图和绘图：负责绘图消息管理和窗口已绘图区域的函数。其中一些函数实际上是从uer32.dll导出的。

(13)路径：负责将一系列直线和曲线组成名为路径的GDI对象，并用它来绘制的函数。

(14)画笔：处理直线绘制属性的函数。

(15)打印和打印池：负责将GDI绘图命令发送到硬拷贝设备(如行式打印机和绘图仪)并平滑地管理这些任务的。打印池函数是由Win32打印池提供的，包括几个系统提供的DLL和销售自定义的模块。

(16)矩形：user32.dll提供的处理RECT结构的函数。

(17)区域：负责用区域GDI对象描述一个点集的函数，并对该点集进行操作。

还有一些没有文档记载的函数。有一些GDI函数在DDK中说明，还一些没有文档记载但系统DLL使用的函数，另处还有一些函数则没有用过。下面是这些函数的粗略分类：

用户模式打印机驱动程序。

OPENGL。

EUDC。

其他系统DLL支持。

其他没有文档记载的函数。

3、GDI对象句柄

32位的GDI句柄由8位未知的高位、1位堆对象标记、7位对象类型信息和高4位是0的16位索引组成。借助7位对象类型信息，可以确定设备上下文、区域、位图、调色板、字体、画刷、加强型图元文件、画笔和扩展画笔。

![](http://hi.csdn.net/attachment/201010/18/0_1287403406hu0O.gif)

4、GDI对象类型

GDI对象表

typedef struct

{

void * pKernel;

unsigned short nProcess;

unsigned short nCount;

unsigned short nUpper;

unsigned short nType;

void * pUser;

}GdiTableCell;

（1） pKernel指向页面池：对每个有效GDI对象，pKernel从不为空，并且值总是唯一的。因此看起来对每个GDI对象有一个相应的数据结构，这个数据结构只能从内核模式代码存取，甚至不能从GDI32.DLL直接存取。对于不同进程的对象，从pKernel的值中看不出明显区分区域来。pKernel指向的对象起始地址是0xE1000000，根据《Inside Windows NT》，起始地址是0xE1000000的区域一般是被称为“页面池”的可分页系统的内存堆。

（2）nCount 是一个部分选择计数器：在Windows 2000下，nCount总是零，就是说未使用。但在Windows NT 4.0中，某些GDI对象用了它。为了理解nCount的意义，我们试着将对象句柄选入和取消一个或多个设备上下文中，观察选入和取消是否能根据nCount值的变化而成功。

（3）nProcess使得GDI句柄绑定到进程：如果程序想使用另一个进程的GDI对象句柄，Win32 API调用一般会失败。GdiTableCell中的nProcess字段就是这种现象背后的原因。对于库存（stock）对象，如GetStockObject（BLACK_PEN），nProcess被置为零。对于用户进程创建的其他 GDI对象，nProcess是创建进程的进程标识符。

有了这个字段，GDI就会很容易地检查当前进程标识符是否和GDI对象的nProcess字段一致，目的是强制对象句柄不能在另一个进程中访问的规则。

根据微软的文档，进程终止时，由该进程创建的所有GDI对象会被释放。如果你想知道这是怎样实现的，我们现在有一点线索了。GDI只需搜索GDI对象表并删除有指定进程标识符的对象。

（4）nUpper：再次检查句柄：我们发现GDI对象表入口的nUpper字段是4字节GDI对象句柄的高两个字节的完全拷贝——对GDI对象句柄进行错误检查是低成本的冗余校验。

（5）nType:内部对象类型：nType的低字节通常和HGDIOBJ中的是7位类型信息相同，高位字节通常是零。

（6）pUser指向用户模式数据结构。

GDI对象类型列表：

![](http://hi.csdn.net/attachment/201010/18/0_1287403426r4Dr.gif)

5、GDI函数大致分类

   设备上下文函数(如GetDC、CreateDC、DeleteDC)、画线函数(如LineTo、Polyline、Arc)、填充画图函数(如Ellipse、FillRect、Pie)、画图属性函数(如SetBkColor、SetBkMode、SetTextColor)、文本、字体函数(如TextOut、GetFontData)、位图函数(如SetPixel、BitBlt、StretchBlt)、坐标函数(如DPtoLP、LPtoDP、ScreenToClient、ClientToScreen)、映射函数(如SetMapMode、SetWindowExtEx、SetViewportExtEx)、元文件函数(如PlayMetaFile、SetWinMetaFileBits)、区域函数(如FillRgn、FrameRgn、InvertRgn)、路径函数(如BeginPath、EndPath、StrokeAndFillPath)、裁剪函数(如SelectClipRgn、SelectClipPath)等。

二、三种图形输出类型
　　应用程序可以使用GDI创建三种类型的图形输出：矢量输出、位图图形输出和文本输出。

**矢量图形输出**
　　矢量图形输出指的是创建线条和填充图形，包括点、直线、曲线、多边形、扇形和矩形的绘制。
**位图输出**
　　位图图形的输出是指位图图形函数对以位图形式存储的数据进行操作，它包括各种位图和图标的输出。
　　在屏幕上表现为对若干行和列的像素的操作，在打印机上则是若干行和列的点阵的输出。
　　位图图形输出的优点是速度很快，它是直接从内存到显存的拷贝操作。缺点是需要额外的内存空间。
**文本输出**
　　与DOS字符方式下的输出不同，Windows是按图形方式输出的。
　　这样，在输出文本时，必须以逻辑坐标为单位计算文本的输出位置，而不是象DOS下以文本行为单位输出文本。这比DOS下的文本输出要难一些。
　　按图形方式输出文本给文本输出带来很大的灵活性。用户可以通过调用各种GDI函数，制造出各种文本输出效果，包括加粗、斜体、设置颜色等。
Windows还提供了一种TrueType(写真字体)。TrueType字体用一组直线和曲线命令及一些参数来描述字体的轮廓。Windows可以通过参数来调整直线的长度和曲线的形状，从而实现对字体的自由缩放。

三、设备描述表(DC)

在Windows环境中，各程序的输出必须限制在自己的窗口中。

ＧＤＩ使用一种简单的机制保证在窗口中画图的各程序遵循这个规则。这种机制即为设备描述表(DC);当Windows程序在屏幕、打印机或其它设备上画图时，它并不是将像素直接输出到设备上，而是将图绘制到由设备描述表表示的逻辑意义上的"显示平面"上去。设备描述表是深寓于Windows中的一种数据结构，它包含ＧＤＩ需要的所有关于显示平面情况的描述字段，包括相连的物理设备和各种各样的状态信息。

![](http://hi.csdn.net/attachment/201010/18/0_12874034726yyv.gif)

获取与窗口关联的设备上下文，应用程序不能使用CreateDC创建与一个窗口相关的设备上下文，但是WIN32 API提供了几个获取与窗口相关的设备上下文的函数，包括：

HDC GetWindowDC(HWND hWnd)

HDC GetDC（HWND hWnd）

HDC GetDCEx（HWND hWnd，HRGN hrgnClip，DWORD flags）

HDC BeginPaint（HWND hWnd，LPPAINTSTRUCT lpPaint）

使用GetDCEx可以代替其他的函数。例如，GetDCEx（hWnd，NULL,DCX_WIND|DCX_NORESETATTRS）可以轻松地替代GetWindowDC（hWnd），而GetDCEx（hWnd，NULL,DCX_NORESETATTRS）可以替代GetDC（hWnd）。

设备上下文类包括CDC和它的派生类CClientDC、CPaintDC、CWindowDC、CMetaFileDC。

CDC、CclientDC、CpaintDC、CwindowDC、CmetaFileDC含义与区别
CDC是设备上下文类的基类，除了一般的窗口显示外，还用于基于桌面的全屏幕绘制和非屏幕显示的打印机输出。CDC类封装了所有图形输出函数，包括矢量、位图和文本输出。
CClientDC（客户区设备上下文）用于客户区的输出，它在构造函数中封装了GetDC()，在析构函数中封装了ReleaseDC()函数。一般在响应非窗口重画消息（如键盘输入时绘制文本、鼠标绘图）绘图时要用到它。
CClientDC dc(this);//this一般指向本窗口或当前活动视图
dc.TextOut(10,10,str,str.GetLength());
CPaintDC用于响应窗口重绘消息（WM_PAINT）时的绘图输出。CPaintDC在构造函数中调用BeginPaint()取得设备上下文，在析构函数中调用EndPaint()释放设备上下文。EndPaint()除了释放设备上下文外，还负责从消息队列中清除WM_PAINT消息。因此，在处理窗口重画时，必须使用CPaintDC，否则WM_PAINT消息无法从消息队列中清除，将引起不断的窗口重画。CPaintDC也只能用在WM_PAINT消息处理之中。
CWindowDC用于窗口客户区和非客户区（包括窗口边框、标题栏、控制按钮等）的绘制。除非要自己绘制窗口边框和按钮（如一些CD播放程序等），否则一般不用它。
CMetaFileDC专门用于图元文件的绘制。图元文件记录一组GDI命令，可以通过这一组GDI命令重建图形输出。使用CMetaFileDC时，所有的图形输出命令会自动记录到一个与CMetaFileDC相关的图元文件中。

BeginPaint和GetDC区别

为什么WINDOWS要提出无效区域的概念呢？这是为了加速。因为BeginPaint和EndPaint用到的设备描述符只会在当前的无效区域内绘画，在有效区域内的绘画会自动被过滤，大家都知道，WIN GDI的绘画速度是比较慢的，所以能节省一个象素就节省一个，不用吝啬，这样可以有效加快绘画速度。
可见BeginPaint、EndPaint是比较“被动”的，只在窗口新建时和被摧残时才重画。
而GetDC用于主动绘制，只要你指到哪，它就打到哪。它不加判断就都画上去，无效区域跟它没关系。对话框没被覆盖没被摧残，它很健康，系统没要求它重画，但开发者有些情况下需要它主动重画：比如一个定时换外观的窗口，这时候就要在WM_TIMER处理代码用GetDC。这时候再用BeginPaint、EndPaint的话，会因为无效区域为空，所有绘画操作都将被过滤掉。

由于WM_PAINT消息的优先级很低，这样，由于窗口对象不能及时收到WM_PAINT消息而影响用户对屏幕对象的视觉感觉。为弥补这个缺陷，程序员可以考虑使用函数UpdateWindows()，它在应用程序的消息队列中存在WM_PAINT消息的情况下，强使Windows立即向窗口对象发送WM_PAINT消息.

//由于WM_PAINT的优先级别很低（甚至在虚拟按键消息之后，见《windows核心编程》窗口消息一章），它只是改变了消息结构体中的QS_PAINT标志。所以呢，如果是使用函数UpdateWindows()，会直接send一个WM_PAINT消息，那样会直接调用窗口处理函数，比普通的WM_PAINT消息处理的快很多。

case WM_LBUTTONDOWN:
               MessageBox(hwnd,"mouse clicked","message",0);
               HDC hdc;
              hdc=GetDC(hwnd);
              TextOut(hdc,0,50,"程序员之家",strlen("程序员之家"));
             ReleaseDC(hwnd,hdc);
       break;
case WM_PAINT:
           HDC hDC;
PAINTSTRUCT ps;//看出区别了吗？
          hDC=BeginPaint(hwnd,&ps);
          TextOut(hDC,0,0,"http://www.csdn.net/",strlen("http://www.csdn.net/"));
        EndPaint(hwnd,&ps);
   break;

PAINTSTRUCT ps;这个结构就是为了填充无效区域的坐标等等， 这样BeginPaint就可以只画无效区域了，提高了效率。

BeginPaint可以使无效区域变有效，GetDC不改变区域属性，无效的还是无效，有效的依然是有效。
BeginPaint在WM_PAINT消息里使用，GetDC则可以在所有的消息中使用，一般是非WM_PAINT消息。

四、图形对象

图形对象类包括CGdiObject、画笔、刷子、字体、位图、调色板、区域等。
CGdiObject是图形对象类的基类，但该类不能直接为应用程序所使用。
　　要使用GDI对象，必须使用它的派生类：画笔、刷子、字体、位图、区域等等。
　　使用图形对象要注意两点：
a.同其他MFC对象一样，GDI对象的创建也要分为两步：第一步，是定义一个GDI绘图对象类的实例；第二步调用该对象的创建方法真正创建对象。
b.创建对象：使用该对象，首先要调用CDC::SelectObject()将它选入到设备上下文中，同时保存原来的设置到一个GDI对象指针比如说pOldObject中。在使用完后，再用SelectObject(pOldObject)恢复原来的设置。

1、GDI画笔和CPen类

Windows用当前选入设备描述表的画笔绘制直线和曲线，并给用Rectangle,Ellipse以及其他图形生成函数画出的图形镶画边框。默认画笔画出的是一个像素点宽的黑色实线。

如果要改变画线方式，则需创建一个GDI画笔，并由CDC::SelectOjbect将它选设备描述表，MFC用类CPen表示GDI画笔。

创建画笔的方法

创建画笔的最简单的方法是构造一个CPen对象并把定义画笔所用的参数都传送给该对象

CPen pen(PS_SOLID,1,RGB(255,0,0));

创建画笔的第二种方法是构造　一个没有初始化的CPen对象并调用CPen::CreatePen:

CPen pen;

pen.CreatePen(PS_SOLID,1,RGB(255,0,0));

创建画笔的第三种方法是构造一个没有初始化的CPen对象，向LOGPEN结构中填充描述画笔特性的参数，然后调用CPen::CreatePenIndirect生成画笔。

CPen pen;

LOGPEN lp;

lp.lognStyle = PS_SOLID;//样式

lp.lopnWidth.x = 1;//宽度

lp.lopnColor = RGB(255,0,0);//颜色

pen.CreatePenIndirect(&lp);

从以上的画笔定义可以看出，定义画笔需要三个特性：样式，宽度和颜色。画笔的样式说明请参见MSDN。

线条大约有七种风格，如下：

PS_DASH指定一支绘制虚线的画笔

PS_DASHDOT指定一支虚线和点交替的画笔

PS_DASHDOTDOT指定一支虚线和两点交替的画笔

PS_DOT指定一支点线画笔

PS_INSIDEFRAME指定一支带有限定形状的画笔，线的厚度不能延伸出此形状的边界

PS_NULL指定一支空画笔

PS_SOLID创建一支实线画笔

2、GDI画刷和CBrush类

在默认情况下，则Rectangle,Ellipse以及其它CDC函数画出的封闭图形填充着白色像素点。通过创建GDI画刷并在画图之前将它选入设备描述表，可以改变图形的填充颜色。

MFC的CBrush类封装了GDI画刷。

画刷有三种基本类型：单色、带阴影和带图案。

单色画刷填充的单色。

阴影线画刷采用预先定义好的交叉线图案填充图形。

图案画刷用位图来填充图形。

创建单色画刷的方法类似于创建画笔，如下所示：

CBrush brush(RGB(255,0,0));

或

CBrush brush;

brush.CreateSolidBrush(RGB(255,0,0));

创建带阴影线画刷

CBrush brush(HS_DIAGCROSS,RGB(255,0,0));//阴影索引，COLORREF值

或

CBrush brush;

brush.CreateHatchBrush(HS_DIAGCROSS,RGB(255,0,0));

HS_DIAGCROSS是可供选择的六种阴影线样式之一，其它样式请参见MSDN

注意：

在用阴影线画刷填充时，除非用CDC::SetBkColor改变设备描述表的当前背景色，或用CDC::SetBkMode把背景模式OPAQUE改成TRANSPARENT，禁止背景填充，否则Windows就以默认的背景色(白色)填充阴影色线间的空白处。

3、字体与CFont

封装了GDI字体对象，用户可以建立一种GDI字体，并使用CFont的成员函数来访问它。字体包括字样(FontFamily) 、风格、尺寸三个主要属性；字体是指宋体、黑体等；风格指字体的粗细、是否倾斜等；FontStyleStrikeout是指删除线风格；可以使用GDI中的Font类，直接构造一个字体对象，如：Font font(&fontFamily, 12, FontStyleRegular, UnitPoint)；为了与原来的GDI字体兼容，Font的构造函数还有另外一种形式：Font(HDC hdc, const LOGFONTW *logfont)；hdc是一个窗口的设备环境句柄，logfont是指向LOGFONT(逻辑字体)数据结构的指针。

逻辑字体选到设备上下文后，可用下面几个函数来查询它所影射的物理字体的细节，及当前物理字体实例的度量信息。

int GetTextFace(HDC hDC,int nCount,LPSTR lpFaceName);

DWORD GetFontLanguageInfo(HDC hDC);

int GetTextCharSet(HDC hDC);

int GetTextCharSetInfo(HDC hDC,LPFONTSIGNATURES lpSig,DWORD dwFlags);

BOOL GetTextMetrics(HDC hDC,LPTEXTMETRIC lptm);

UINT GetOutlineTextMetrics(HDC hDC,UINT cbData,LPOUTLINETEXTMETRIC lpOTM);

4、位图

封装了GDI位图，它提供成员函数装载和操作位图。GDI支持三种位图类型：设备无关位图、设备相关位图、DIB段。

GDI绘图之位图处理函数

BitBlt把由一个矩形中的像素组成的位块从源设备环境转移到目标设备环境中

CreateBitmap使用宽度、调试和颜色格式创建位图

CreateBitmapIndirect同上，只是参数不一样

CreateCompatibleBitmap创建与确定的设备环境关联的设备相兼容的位图

CreateDIBitmap从一个设备无关位图中创建设备无关位图，同时设置该位图的位

CreateDIBSection创建应用程序能够直接写入的设备无关位图，返回一个指向该位图存入位置的指针

ExtFloodFill用当前画刷填充显示平面的某一区域

GetBitmapDimensionEx获取一个位图的大小

GetDIBColorTable获取当前DIB位图颜色表中某一范围的RGB颜色值

GetDIBits获取位图的位并使用特定格式把它们复制到一个缓冲区中

GetPixel获取当前坐标位置处像素的RGB颜色值

GetStretchBltMode获取当前的伸缩模式

LoadBitmap从一个模板或可执行文件中装入确定的位图资源

MaskBlt使用特定的掩模和光栅操作合并源位图和目的位图的颜色数据

PatBlt使用当前被选入设备环境的画刷绘制给定的矩形

PlgBlt把一个平行四边形中的像素颜色数据组成的位块从源设备环境转移到目标设备环境中

SetBitmapDimensionEx为位图指定预定的大小

SetDIBColorTable设置当前DIB位图颜色表中某一范围的RGB颜色值

SetDIBits使用一个特定的设备无关位图中得到的颜色数据来设置位图的像素

SetDIBitsToDevice使用从一个特定的设备无关位图中得到的颜色数据来设置与目标环境关联的设备中特定矩形的像素

SetPixel给特定坐标位置处的像素设置确定的颜色

SetPixelV给特定坐标位置处的像素设置与确定颜色最近似的颜色值

SetStretchBltMode在确定的设备环境中设置位图伸缩模式

StretchBlt把位图从源矩形区域复制到目标矩形区域，并在需要时对位图进行拉伸或收缩

StretchDIBits把DIB位图中矩形区域的像素颜色数据复制到确定的目标矩形中

5、调色板

封装GDI调色板，它保存着系统可用的色彩信息，是应用程序和彩色输出设备上下文的接口。

与某个设备上下文相关联的逻辑调色板可以用GetCurrentObject(hDC,OBJ_PAL)函数来获取。对于一个新的设备上下文，它的缺省调色板是由GetStockObject(DEFAULT_PALETTE)函数返回的库存缺省调色板。

6、输出文本

GetTextMetrics();　返回当前使用字体的尺寸描述，用于进行字体大小计算。

SetTextColor(int nColor);　设置前景色。

SetBkColor(int nColor);　设置背景色。

TextOut：文字输出。

TabbedTextOut：象TextOut一样显示正文，可以使用制表键Tab。

ExtTextOut：在指定的矩形中显示正文。可以用该函数删去超出矩形的正文，用正文背景填充矩形，调整字符间隔。

DrawText：在指定矩形种显示正文，可以使用制表键Tab。在格式化矩形时调整正文左对齐、右对齐或居中；还可以在一个词中断开以适应矩形边界。

GDI+中的文本输出只有一个函数：DrawString 
DrawString(const WCHAR *string, INT length, const Font *font, const Rect &layoutRect, const StringFormat *stringFormat, const Brush *brush)；
DrawString(const WCHAR *string, INT length, const Font *font, const PointF &origin, const Brush *brush)；
DrawString(const WCHAR *string, INT length, const Font *font, const PointF &origin, const StringFormat *stringFormat, const Brush *brush)；
brush用来指定一个画刷，这个画刷既可以是SolidBrush和HatchBrush，也可以是TextureBrush(纹理画刷)，甚至是渐变画刷。 StringFormat类来控制这些格式属性，文本的格式属性通常包括对齐方式、字符间隔以及文本调整等。

7、画点

SetPixel：在指定坐标处按指定色彩画一点。

8、画线

MoveToEx将直线起点移动到指定坐标处，LineTo从起点开始画直线到终点处。使用的线型由当前所用画笔指定。

9、画弧

Arc(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4); 

10、封闭图形

　　矩形：Rectangle

　　圆角矩形：RoundRect

　　椭圆：Ellipse

　　饼形图：Pie

　　封闭的多边形：Polygon

11、其它绘图函数

FillRect：用指定颜色填充矩形且不画边线。

FloodFill:用给定的颜色，利用当前所选的刷子填充显示的底面被一个边线所包围的区域，如多边形区域的填充。

ExtFloodFill：用给定的颜色，利用当前刷子填充表面被一个边线包围的区域，用户可以有选择地填充一个由指定颜色组成的区域。

InvertRect:在某一矩形区域内反显现有颜色。

12、元文件

用于记录和回放GDI函数调用。首先通过CMetaFileDC:Create来创建新的对象，如果有文件名，则保存到文件内，如果没有则建立在内存中。完毕后，用Close()关闭。
可以调用PlayMetaFile()来回放元文件。还可以用CopyMetaFile()将文件存盘。当结束元文件的时候，用DeleteMetaFile()从内存中删除。
另外有增强型元文件。
CMetaFileDC    dc;
BOOL bCreate=dc.CeeateEnblanced(pDC,”metaest,wmf/0”,0,vectosd/0 this is l/0/0”);
if(!bCreated) return;
(具体的绘制内容……)
//关闭文件，并返回文件句柄
HENHMETAFILE    hemf=dc.CloseEnhanced();
//播放文件
pDC->SaveDC();
pDC->SetMapMode();
pDC->PlayMetaFile(hemf,&rc);
pDC->RestoreDC(-1);
//当文件不再需要
if(hemf)
    ::DeleteEnhMetaFile(hemf);

13、打印

打印相关函数

int StartDoc(HDC hDC,CONST DOCINFO *lpdi);//告诉GDI启动新打印任务

int StartPage(HDC hDC);//打印任务新页开始

int EndPage(HDC hDC);//打印任务一页结束

int EndDoc(HDC hDC);//结束StartDoc开始的打印任务

HDC ResetDC(HDC hDC,const DEVMODE *lpInitData);//改变打印设备上下文设置信息

int AbortDoc(HDC hDC);//用于非正常结束打印任务

int SetAbortProc(HDC hdc,ABORTPROC lpAbortProc);//设置回调例程，GDI周期性用它检查还是放弃打印任务

14、设备描述表属性修改/设置函数

当使用ＣＤＣ输出函数在屏幕上画图时，输出的某些特性并没有在函数调用过程中规定(采用系统默认的属性画图)，但我们可以通过设备描述表自身获得或得新设置属性。

ＭＦＣ提供了获得和设置这些属性的函数，我们可以用这些函数方便地改变画图时的默认属性。

例如：

CDC::SetTextColor//设置文本颜色

CDC::GetTextColor//获得文本颜色

CDC::SetBkColor//设置背景颜色

CDC::SetBkMode//设置背景模式

CDC::SetMapMode//设置映射模式

CDC::CDC::SetROP2//设置绘图模式

CDC::MoveTo//当前位置

CDCL::SelectObject//当前画笔，当前画刷，当前字体

（如果想忽略背景色，可将背景设置为"Transparent",dc.SetBkMode(TRANSPARENT);）

SelectObject函数

最常用来定义设备描述表属性的ＣＤＣ函数是SelectObject。

例如：

CPen pen(PS_SOLID,2,RGB(0,192,0));

CPen* pOldPen = dc->SelectObject(&pen);//把新的ＣＤＣ对象选入设备描述表，同时保存旧的ＣＤＣ对象

dc->Ellipse(0,0,100,100);

dc.SelectObject(pOldPen);//

绘图模式与SetROP2

ＧＤＩ将像素点输出到逻辑平面上时，它不只是简单地输出像素点颜色。相反，它能过一系列的布尔运算将输出像素点的颜色和目标位置上像素点的颜色合成一起。它所使用的逻辑关系由设备描述表当前的绘图模式确定。使用CDC::SetROP2(Set Raster Operation To)可更改绘图模式。默认绘图模式为R2_COPYPEN,它将将像素点复制到显示平面上。

映射模式

默认映射模式

固定比例映射模式

可编程映射模式

简单地说，映射模式是设备描述表的属性，用于确定从逻辑坐标值到设备坐标值的转换方式。

默认的映射模式

默认的映射模式使MM_TEXT,它使以象素为单位的。X轴向左为正，Y轴向下为正。默认的坐标原点在左上角。

固定比例映射模式

固定比例的映射模式有MM_LOMETRIC、MM_HIMETRIC、MM_LOENGLISH、MM_HIENGLISH、MM_TWIPS种。

它们默认的坐标原点都使在左上角。其区别在于每一个逻辑单位对应的物理大小不一样。

所对用的逻辑单位分别为0.1毫米，0.01毫米，0.01英寸，0.001英寸，1/1440英寸（0.0007英寸）。

可变比例映射模式(可编程映射模式)

对于可变比例的映射模式用户可以自己定义一个逻辑单位代表的大小，其大小可以任意。

也可以让这个大小随环境改变而改变。有MM_ISOTROPIC，MM_ANISOTROPIC这两种映射模式。

其逻辑单位的大小等于视口范围和窗口范围的比值。

两者的不同在于前者要求X轴和Y轴的度量单位必须相同，而后者没有这样的限制。

可编程映射模式

MM_ISOTROPIC和MM_ANISOTROPIC是由用户决定从逻辑坐标值转换成设备坐标值的方式，

即是由用户决定一个逻辑单位等于多少个设备单位(cm,mm,m等)，而不是由Windows决定。

所以被称之为可编程映射模式；

MM_ISOTROPIC和MM_ANISOTROPIC映射模式最常用于根据窗口尺寸按比例自动调节画图的输出大小的场合。

MM_ISOTROPIC和MM_ANISOTROPIC映射模式的区别

前者中X方向和Y方向具有同一个缩放比例因子，而后者两个方向可以单独缩放；

例子：

CRect rect;

GetClientRect(&rect);

dc.SetMapMode(MM_ISOTROPIC);

dc.SetWindowExt(500,500);//设置窗口范围，将窗口的逻辑尺寸高为500单位×500单位

dc.SetViewportExt(rect.Width(),rect.Height());//设置视口范围

dc.Ellipse(0,0,500,500);

注：窗口的尺寸以逻辑单位计算，视口的尺寸以设备单位或像素计算。

特别说明

MM_TEXT为默认映射模式，其原点在窗口的左上角，X轴的正向向右，Y轴的正向向下，

并且一个逻辑单位对应于设备坐标下的一个象素

其它映射模式则原点不变，只是Y轴会翻转使正向朝上，并且逻辑单位被按比例转换为实际距离大小，而不是像素数。

需要注意的是，使用公制映射模式(非默认映射模式)时，为使输出可见，Y坐标必须为负值。

例如：

dc.Rectangle(0,0,200,100);//默认模式下画图

dc.SetMapMode(MM_LOENGLISH);//改变映射模式

dc.Rectangle(0,0,200,-100);//画图

传送给ＣＤＣ输出函数的是逻辑坐标值。

设备坐标值是指窗口中相应的像素点位置。

GDI映射模式总结

http://ideage.javaeye.com/blog/294778

GDI坐标变换详细总结

http://d.download.csdn.net/down/2596722/junco007

编程中注意问题：

1、设备上下文5大区域：剪裁区域、元区域，系统区域，API区域，RAO区域。对于由函数BeginPaint、GetDC或者CreateDC所返回的设备上下文来说，其剪裁区域的值是NULL,剪裁区域的值为NULL,也就意味着没有剪裁区域。没有剪裁区域意味着系统区域氛围内的所有图形都将被显示出来，而不是不显示。

2、库存对象(StcokObject)是由操作系统预先创建的，并为所有在系统中运行的过程共享。在应用程序使用完库存对象之后，没有必要删除他们的句柄。但是调用函数DeleteObject来处理库存对象句柄是十分安全的，当函数DeleteObject没有做任何事情时，它将返回TRUE值。

3、SetDIBitsToDevice函数与StretchDIBits函数都是显示DIB图象，二者区别是SetDIBitsToDevice函数需要的内存很少；SetDIBitsToDevice不处理拉伸，需要自己写拉伸算法；StretchDIBits可以通过SetStretchBltMode/GetStretchBltMode来控制每个设备上下文的拉伸模式。

4、在DIB和DDB间拷贝位图，GDI函数：SetDIBits/GetDIBits；

存取原始的DDB像素阵列：LONG GetBitmapBits(HBITMAP hBmp,LONG cbBuffer,LPVOID lpvBits);如何知道需要分配多大的缓冲区？在设置大小为0和缓冲区指针为NULL的情况下，GetBitmapBits函数就返回所需要的缓冲区大小。

LONG SetBitmapBits(HBITMAP hBmp,LONG cBytes,LPVOID lpBits);

DDB显示，GDI函数：BitBlt,StretchBlt

5、SetTextColor、SetBkColor函数设置新颜色之后，使用完成之后，需要再把旧颜色设置回去（通过SetTextColor、SetBkColor）。此种操作跟SelectObject类似。

字体：CreateFont、CreateFontIndirect和CreateFontIndirectEx创建GDI逻辑字体对像，将句柄返回给调用者。通过GDI对象句柄，你可以用GetObject得到定义逻辑字体的LOGFONT或ENUMLOGFONTEX结构。与其他GDI对象一样，不需要逻辑字体对象时，应用DeleteObject删除掉。

6、GDI内存泄露问题

编写GDI程序时运行多次后出现异常,除了众所周知的内存泄露以外,gdi资源泄露也是一个很直接的原因。预防gdi资源泄露措施。

1）Create出来的gdi对象,一定要用DeleteObject来释放,释放顺序是先Create的后释放,后Create的先释放.
这里的Create指的是以它为开头的gdi函数,比如,CreateDIBitmap,CreateFont等等,最后都要调用DeleteObject来释放.

2）Create出来的dc要用DeleteDC来释放,Get到的要用ReleaseDC释放.

3）确保释放DC的时候DC中的各gdi对象都不是你自己创建的;确保个gdi对象在释放的时候不被任何dc选中使用.
假如我们要使用gdi函数画图,正确的步骤应该如下:
a.创建一个内存兼容dc(CreateCompatibleDC)
b.创建一个内存兼容bitmap(CreateCompatibleBitmap)
c.关联创建的内存兼容dc和bitmap(SelectObject)
d.画图
e.BitBlt到目的dc上
f.断开内存兼容dc和bitmap关联(SelectObject)
g.销毁内存兼容bitmap
h.销毁内存兼容dc
由于SelectObject在选入一个新的gdi对象的时候会返回一个原来的gdi对象(假如成功的话),所以需要在步骤c的时候保存返回值,在步骤f的时候当作入口参数使用.还有,步骤g和步骤h实际上顺序可以随意,因为他们两个此刻已经没有关系了,但是为了结构清晰,我建议按照"先Create的后释放,后Create的先释放"的原则进行.
关于步骤f,可能会有争议,因为即使省略这一步,步骤g和步骤h看起来照样可以返回一个成功的值.但实际上可能并没有执行成功,至少boundschecker会报告有错,错误信息大致是说,在释放dc的时候还包含有非默认的gdi对象,在释放gdi对象的时候又说这个gdi对象还被一个dc在使用.所以,我建议保留步骤f.

4）关于98下使用CreateCompatibleBitmap
按照msdn的说法,创建出来的size不能超过16m.实际情况是这样吗?非也~!从我自己做的测试结果来看(win98se-sc),这个值在2044*2043和2044*2044之间,然而,后来在另外一个98系统上这个值也不行,后来我干脆把上限给成了2000*2000.很幸运,到现在还没有出问题,但我不能保证这个数字就是正确的.还有一点,假如宽或高有一个超过32768,哪怕另外一个值是1,也会创建失败,有兴趣的可以自己做个测试.如果要想保证这个函数在98下永远成功,可以试试下面的代码:
float factor = 10.f;
while(!bitmap.CreateCompatibleBitmap(&dc ,nWidth*factor ,nHeight*factor))
{
   factor -= 0.01f;  
}
这样至少可以保证宽和高是成比例的

5）关于在打印机上使用BitBlt
有时候在内存兼容dc里面已经做好图了,但在使用BitBlt的时候却会失败.这个时候,首先确认创建的内存兼容dc和bitmap是不是使用打印机的dc,如果确认无误,还是执行BitBlt失败,那80%可能是内存兼容bitmap太大了,请按如下方法再试试:
创建另外一个内存兼容dc2和一个比较小的内存兼容biimap2,大概是1000*1000吧,我是这样用的，然后把dc里面的内容分成块(1000*1000),把每一块BitBlt到dc2上面,再从dc2里面BitBlt到打印dc上.有人可能会有这样的疑问:那为什么不直接把dc里面的内容分几次BitBlt到打印机上呢?有区别吗?答案是肯定的,如果dc里面的bitmap太大,哪怕你想BitBlt一个10*10的区域到打印机上都会失败.

7、InvalidateRec、Invalidate、Validate、UpdateWindow

InvalidateRect使窗口的指定区域无效，Invalidate使窗口全部区域无效，Validate使窗口全部区域有效。如果窗口有区域无效，则Windows发WM_PAINT，参数中指定无效区域的最大矩形，程序中响应此消息，用BeginPaint使无效区域为剪裁区，后面的绘画活动不会超出这个剪裁区，归后由EndPaint使无效区域变为有效，这时WM_PAINT被清除。MFC为窗口类提供了WM_PAINT的消息处理。MFC为窗口类提供了WM_PAINT的消息处理函数OnPaint，OnPaint负责重绘窗口。视图类有一些例外，在视图类的OnPaint函数中调用了OnDraw函数，实际的重绘工作由OnDraw来完成。参数bErase为TRUE时，重绘区域内的背景将被擦除，否则，背景将保持不变。

UpdateWindow( )的作用是使窗口立即重绘。调用Invalidate等函数后窗口不会立即重绘，这是由于WM_PAINT消息的优先级很低，它需要等消息队列中的其它消息发送完后才能被处理。调用UpdateWindow函数可使WM_PAINT被直接发送到目标窗口，从而导致窗口立即重绘。UpdateWindow只向窗体发送WM_PAINT消息，在发送之前判断GetUpdateRect(hWnd,NULL,TRUE)看有无可绘制的客户区域，如果没有，则不发送WM_PAINT。

Invalidate在消息队列中加入一条WM_PAINT消息，其无效区为整个客户区。

UpdateWindow直接发送一个WM_PAINT消息，其无效区范围就是消息队列中WM_PAINT消息（最多只有一条）的无效区。

效果很明显，当调用Invalidate之后，屏幕不一定马上更新，因为WM_PAINT消息不一定在队列头部，而调用UpdateWindow会使WM_PAINT消息马上执行的，绕过了消息队列。

8、双缓冲绘图

主要是建立内存兼容DC和内存兼容位图，如下：

HDC hDC = ::GetDC(m_hWnd);

hFrceDC = CreateCompatibleDC(hDC); //内存兼容DC

hFrceBmp = CreateCompatibleBitmap(hDC, WinWidth, WinHeight); //内存兼容位图

SelectObject(hFrceDC, hFrceBmp); //选入内存兼容DC

::ReleaseDC(m_hWnd, hDC);

以后所有的GDI操作，比如LineTo等，都只对hFrceDC，当要刷新的时候：

void CST_CurveCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid) 

{

BitBlt(pdc->m_hDC, rcInvalid.left, rcInvalid.top, rcInvalid.Width(), rcInvalid.Height(), hFrceDC, rcInvalid.left, rcInvalid.top, SRCCOPY);

...

}

这就是为什么双缓冲是GDI编程中最为基本的技巧了。这样不管你的绘制有多少的复杂，屏幕不会再有闪烁，绘制的复杂只会占用CPU多一点而已。

9、刷新背景问题

是否刷新背景很重要，有些新手将上面的双缓冲用到对话框之上，然后告诉我，还是闪烁！那是因为刷了背景的原因，因为用BitBlt绘制的东西，在windows看来，它不是一个窗口，它不像一个按钮，因为按钮是一个窗口，所以windows在刷背景的时候，会从刷新区域里面减掉按钮所占的区域，那么回到正题，在对话框上BitBlt的时候，如果位图显示占满了对话框的整个客户区，这就简单不过了，直接响应WM_ERASEBKGND消息，然后返回TRUE：

BOOL CXXX::OnEraseBkgnd(CDC* pDC)

{

return TRUE;

}

如果BitBlt不会占满整个客户区，那么很抱歉，有点麻烦了，你要生成一个刚好不包括BitBlt显示区域的区域，然后对DC执行SelectClipRgn函数，请在MSDN里面搜索“Region Functions”，里面的函数大多你可能都用得上，主要是创建区域，操作区域（比如求AND，求OR，求DIFF等）。

10、GDI函数调用次数很重要，用上双缓冲和消除刷新背景之后，闪烁的问题可以认为是解决了，下面我们将讨论减少CPU使用率的问题。GDI操作是比较浪费CPU资源的，比如频率的调用GDI函数，势必浪费CPU资源，这其中又特别是字符的打印操作，为此GDI提供PolyTextOut函数，调用它一次，可以输出任意多条的字符，每一条都是独立设置输出位置的，这个函数的使用场合我举个例子，比如你在绘制一个坐标上的刻度值，那么这个函数再好不过了。与此功能类似的还有PolylineTo、Polyline、PolyPolyline、Polygon、PolyPolygon等函数。

11、区域刷新技术，就是将客户区分为若干个区域（一般是矩形就行了，速度快），具体怎么分，分多少个没有定论，理论上，可能需要单独刷新的地方，就分为一个区域，每当这个区域需要刷新的时候，只对这个区域进行绘制，并且BitBlt到屏幕上。具体来说，每一个区域应该对应一个刷新函数（或者一个switch语句的某一个case项），这个函数执行GDI操作，当这个区域需要刷新的时候，调用这个函数将新的东西绘制到一个内存兼容DC上，然后让这个矩形失效，那么windows会在适当的时候，调用OnDraw（或类似的函数），在OnDraw里面，直接用BitBlt对无效区域（就是前面提到的那个矩形区域）进行贴图即可。

12、调用CreateCompatibleBitmap创建DDB位图。此函数创建位图时用的是系统内核的分页内存，这是稀有资源。CreateDIBSection的好处是，它使用虚拟内存创建位图。这样运行程序的实例数就只限于pagefile和磁盘空间大小了。不过CreateDIBSection比CreateCompatibleBitmap要难调用一些。

参考资料：《Windows图形编程》

