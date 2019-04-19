# C++游戏 - LC900730的博客 - CSDN博客
2017年05月19日 21:48:39[lc900730](https://me.csdn.net/LC900730)阅读数：356
Windows下： 
API：win32API  GDI GID+ 
SDK: MFC ：microsoft foundation classes 
C++能做什么： 
桌面应用程序； 
游戏；
## 创建窗口
1.设计一个窗口类； 
WNDCLASSEXW //标准窗口类 
WNDCLASSEXW wndClass={0}; 
wnd.style=CS_HREDRAW|CS_VREDRAW|CS_ 
2.注册一个窗口类； 
3.创建一个窗口； 
4.显示窗口和更新窗口 
取得或者释放设备上下文(Device Context)函数 
取得DC信息函数 
绘图函数 
设置和取得DC参数函数 
使用GDI对象函数
### Windows API
基本上指的是Windows 系统提供的函数，主要的Windows函数都在Windows.h文件中声明
### SDK软件开发包
SDK说白了就是一个开发所需资源的一个组合。 
Win32 SDK即Windows32位平台下的软件开发，包括了API函数、帮助文档、微软提供的一些辅助开发工具等
### 1.WinMain
Windows程序的入口点函数 
int WINAPI WinMain(){ 
*IN* HINSTANCE hInstance, 
*IN* HINSTANCE hPrevInstance, 
*IN* LPSTR lpCmdLine, 
*IN* int nCmdShow 
}
在WinDef.h头文件中
```
#define WINAPI __stdcall
#define CALLBACK __stdcall
```
- HINSTANCE类型的hInstance，它表示该程序当前运行的实例句柄。当一个程序在windows下运行时，它唯一对应一个运行中的实例，也只有运行中的程序实例，才有资格分配实例句柄。一个应用程序可以运行多个实例，每一个实例，系统都会分配一个句柄值，并且通过hInstance参数传递给程序的入口点WinMain函数
- HINSTANCE hprevInstance:当前实例的前一个实例的句柄。在Win32环境下，这个参数没有存在感，不起任何作用。只是在写WinMain函数时候需要将它作为一个参数而已
- LPSTR类型 的lpCmdLine，它是一个以空终止的字符串符，指定传递给应用程序的命令行参数：lp前缀表示这是一个指针。如在win7系统E盘的ForTheDream.txt,鼠标双击这个文件启动记事本程序，此时系统将这个txt作为命令行参数传递给记事本程序的WinMain函数，记事本程序在得到这个文件的文件路径后，就会在窗口中正确显示这个文件的内容
- int类型的nCmdShow，指定程序窗口应该如显示，最小化，最大化还是隐藏等
### 2.MessageBox函数
```
int WINAPI MessageBox(){
_In_opt HWND hWnd,
_In_opt LPCTSTR lpText,
_In_opt LPCTSTR lpCaption,
_In_ UINT uType,
}
```
_IN_opt：opt表示可选的 
- HWND类型 的hWnd,表示我们显示的消息框所属的窗口的句柄。设置为NULL，表示消息框是属于桌面的
- LPCTSTR类型的lpText，它是一个NULL结尾的字符串，表示所有显示的消息内容
- LPCTSTR类型 的lpCaption，它也是一个以NULL结尾的字符串，在其中填我们想要显示的消息框的内容
- UINT类型 uType，表示我们消息窗口需要以什么样的样式。如带3个按钮的，2个按钮的。。。 
MessageBox函数的返回值，根据返回值可以判断哪个键被按下了，在作出相应的响应
### playSound函数
BOOL PlaySound( 
LPCTSTR pszSound, 
HMODULE hmod, 
DWORD fdwSound 
); 
如果要使用PlaySound函数的话，必须在编译之前链接winmm.lib库文件 
- LPCTSTR类型 的pszSound，字符串，指定了要播放的声音文件。如果为NULL，就会把所有当前播放的声音全部停掉
- HMODULE类型的hmod，包含了我们在第一个从参数中指定的声音文件作为资源的可执行文件的句柄
- DWORD类型的fdwSound，是一个用来控制声音播放的一个标志
### 3.窗口
每一个Windows应用程序至少要有一个窗口，称为主窗口。利用窗口，应用程序可以接受用户的输入输出以及对输出进行显示。 
一个应用程序通常包含标题栏，菜单栏、系统菜单栏、最小化框、最大化框，有的应用程序还包含滚动条。 
窗口可以分为客户区和非客户区。
### 4.句柄
在Windows下，窗口是通过句柄(HWND)来标识的,我们对某个窗口来进行操作的话，首先是得到这个窗口的句柄。 
句柄(HANDLER)是windows程序中极其重要的一个概念，在windows中，有各种各样的资源，如窗口，图标，光标等，系统创建这些资源会为它们分配内存，并返回标识这些资源的标识号，这些标识号就是句柄。图标句柄(HICON)、光标句柄(HCURSOR)、画刷句柄(HBRUSH)
### 4.1消息与消息队列
Windows程序设计师一种基于事件驱动方式的程序设计模式，Windows程序与操作系统间的通信主要是基于消息的。
#### 消息的表示形式—MSG结构体
```
typedef struct tagMSG{
HWND hwnd; //指定消息所属窗口
UINT message;//指定消息标识符
WPARAM wParam; //指定此msg附加信息
LPARAM lParam; //指定此msg的附加信息
DWOED time;//指定投递到消息队列的时间
POINT pt;//指定投递到消息队列中时鼠标的当前位置
}MSG
```
- HWND类型 的hwnd，指定了消息是发给哪个窗口的,通常一个消息是与某个窗口相关联的。如在某个活动窗口中按下鼠标左键，产生的按键信息就是发送给这个窗口的
- UINT类型的message，指定了消息的标识符。在Windows中，消息是通过一个数值来表示，不同的消息对应不同的数值。Windows将消息对应的数值定义为WM_XXX宏的形式。如鼠标右键按下消息是WM_RBUTTONDOWN,鼠标左键点击WM_LBUTTONCLICK
- 第三个参数及第四个参数WPARAM，都是用于指定消息的附加信息。如收到一个字符信息时，message程序按变量的值就是WM_CHAR，但是到底输入的是什么，就由wParam和lParam来说明
- DWORD类型 time，表示投递到消息队列中 的时间
- POINT类型的pt,表示投递到消息队列中鼠标的当前位置
#### 4.2关于消息队列
每个程序开始执行，系统会为为该程序创建一个消息队列，这个消息队列用于存放程序创建的窗口信息。如按下左键，产生WM_LBUTTONDOWN,系统将这个消息放到窗口所属的应用程序消息队列中，等待应用程序的处理。Windows将产生的依次放到消息队列中，而应用程序通过一个消息循环不断从消息队列中取出消息并进行响应，这种机制，就是Windows程序运行的机制。
### 5.窗口创建
- 窗口类的设计
- 窗口类的注册
- 窗口的正式创建
- 窗口的显示与更新
#### 5.1窗口类的设计
一个完整的窗口类有许多特征：包括光标、图标、背景颜色等。
```
typedef struct tagWNDCLASSEX{
    UINT cbSize;
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCTSTR lpszMenuName;
    LPCTSTR lpszClassName;
    HICON hIconSm;
}WNDCLASSEX,*PWNDCLASSEX
```
1.WNDCLASSEX wndClass={0};//用WINDCLASSEX定义了一个窗口类，即用wndclass实例化了一个WINDCLASSEX，用于之后窗口的各项初始化。 
- 第一个参数UINT类型 的cbsize，表示该结构体字节数大小，一般是sizeof(WNDCLASSEX)
wndclass.cbSize=sizeof(WNDCLASSEX); 
- 第二个参数UINT类型的style：指定这一类型窗口的风格样式
|CS_HREDRAW|当窗口水平方向上宽带发生变化，将重新绘制整个窗口。当窗口发生重新绘制，文字和图像将被擦除。如果没有指定这一样式，那么在水平方向调整宽度，将不会发生重绘窗口|
|----|----|
|CS_VERDRAW|当窗口垂直方向上高度发生变化，将重新绘制整个窗口。如果没有指定，则垂直方向上调整窗口高度，不会重绘制窗口|
|CS_NOCLOSE|禁用系统菜单的Close命令，这将导致窗口没有关闭按钮|
|CS_DBLCLKS|当用户在窗口中双击鼠标，向窗口发送鼠标双击信息|
- 第三个参数：WNDPROC类型的lpfnWndProc，它是一个函数指针，指向窗口中的过程函数，而窗口过程函数是一个回调函数。其中，回调函数不是由该函数的实现方法直接调用的，而是在特定的事件或者条件发生时由另外一方调用的，用于对该事件或条件进行响应
针对Windows消息处理机制，窗口过程函数被调用的过程： 
第一步：在设计窗口类的时候，将窗口过程函数地址赋值给lpfnWndProc成员变量。 
第二步：调用RegisterClass(&wndclass)注册窗口类，那么系统就有了我们编写的窗口过程函数的地址； 
第三步：当应用程序接收到某一个窗口信息，调用DispatchMessage(&msg)将消息回传给系统。系统则利用先前注册窗口类时得到的函数指针，调用窗口过程函数对消息进行处理 
一个 Windows程序可以保护多个窗口过程函数，一个而窗口过程总是与某一个特点的窗口类相关联(通过WNDCLASS结构体中的lpfnWndProc成员变量来指定)，而基于该窗口类创建的窗口使用的是同一个窗口过程。
## 5.GDI函数分类
1.取得(或者建立)和释放(或者清除)设备上下文的函数 
2.取得有关设备上下文信息的函数 
3.绘图函数 
4.设定和取得设备上下文参数的函数 
5.使用GDI对象的函数
### 5.1设备环境
设备上下文，或者设备描述表。简单来讲DC就是程序可以进行绘图的地方。如在屏幕上绘图，那么Device就是屏幕，而DC就是屏幕区的绘图层。如在窗口中绘图，那么Device就是窗口，设备环境就是窗口上可以绘图 的地方。 
当程序需要用GDI来绘图，那么先要取得设备环境的句柄。在取得句柄之后，windows 
利用内定属性将填入到内部设备的内容结构中。我们可以调用不同的GDI函数里改变这些默认值，也可以利用一些GDI函数取得这些属性的当前值。 
当程序在显示区域绘图完毕，必须释放掉设备句柄环境，句柄就是一个数值。句柄被程序释放就不在有效，而且不能再使用。而且，程序必须在处理单个消息处理期间取得和释放句柄
#### 5.1.11取得设备环境句柄(HDC)的方式
一：需要在窗口过程函数处理WM_PAINT消息的那个case之后使用。使用这套方法取得设备环境句柄中涉及到了BeginPaint和EndPaint这2个函数。 
BeginPaint函数:为指定窗口进行绘图工作的准备，并用将和绘图相关的信息填充到PAINTSTRUCT结构中
```bash
HDC BeginPaint(
_in HWND hwnd,
_out LPPAINTSTRUCT lpPaint
```
第一个参数：HWND类型的hwnd，填写需要获取设备环境句柄的那个窗口句柄 
第二个参数：LPPAINTSTRUCT类型的lpPaint 
HDC g_hdc；//全局设备环境句柄 
case WM_PAINT： 
    g_hdc=BeginPaint(hwnd,&paintStruct);//指定窗口进行绘图工作的准备，并用将和绘图有关的信息填充到paintStruct结构体中。 
    Game_Paint(); 
    EndPaint(hwnd,&paintStruct); //EndPaint函数标记指定窗口的绘图过程结束 
    ValidateRect(hwnd,NULL);  //更新客户区的显示 
    break; //跳出switch语句 
    ValidateRect用于更新指定窗口的无效矩形区域 
    BOOL VAlidateRect( 
    _in HWND hWnd, 
    _in const RECT *lpRect 
    );
### 5.1.2取得HDC方式二
可以通过调用GetDC函数来取得句柄，且在调用完GetDC之后，需要调用ReleaseDc对环境设备进行释放。 
HDC GetDC( 
_in HWND hWnd 
); 
使用GetDc函数取得窗口的DC之后，窗口的DC就处于被占用的状态，使用完成之后就必须及时将设备环境释放掉，不然其他应用程序无法使用。 
ReleaseDC函数用于释放设备上下文。 
int ReleaseDC( 
_in HWND hWnd, 
_in HDC hDC 
);
### Windows屏幕区域相关概念
#### 屏幕区、窗口区、客户区
任何一个网络游戏，无论是使用全屏显示还是窗口，都要建立一个窗口。当窗口建立之后，对于程序而言，屏幕就划分了3个区域，是屏幕区、窗口区、客户区； 
窗口区可以理解为砸客户区周围包含了一层窗口的边框，这些窗口边框包围着客户区，共同组成了窗口区。 
ClientToScreen()函数、ScreenToClient()函数
### 创建画笔
HPEN是画笔对象的句柄数据类型用于标识一个画笔对象。 
HPEN CreatePen( 
_in int fnPenStyle,  //画笔样式 
_in int nWidth,    //线条宽度 
_in COLORREF crColor  //线条颜色额 
 ); 
 d第一个参数：int类型的fnPenStyle表示画笔的 样式。如果设置W为PS_SOLID表示绘制的是实线，PS_DAS/PS_DOT/PS_DASHDOT/PS_DASHDOTDOT表示了4中虚线样式，分别是短线、点、短线和点间隔、短线间间隔2个点。PS_NULL表示线不可见。 
 第二个参数表示：int类型的nWidth，画笔所绘制的线条的宽度 
 第三个参数：COLORREF类型的crColor，指定了我们线条的颜色 
 这里的COLORREF类型是一个32位的整型数值，它代表了一种颜色，我们可以使用RGB的宏来初始化COLORREF 
 COLORREF RGB（BYTE byRed, 
 BYTE byGreen, 
 BYTE byBlue 
 ）; 
如创建一个宽带为20的蓝色实线， 
COLORREF Colorblue=RGB(0,0,255); 
HPEN Bluepen=CreatePen(PS_SOLID,20,Colorblue);
### 创建画刷
HBRUSH是画刷对象句柄数据类型，用于标识一个画刷对象。 
- CreateSolidBrush函数，它的功能是创建实心画刷
HBRUSH CreateSolidBrush( 
_in COLORREF crColor 
); 
- CreateHatchBrush:用于创建一个阴影画刷，即不是实心的。当用于填充时，填充的内容就是阴影线
HBRUSH CreateHatchBrush( 
_in int fnStyle,//阴影样式 
_in COLORREF clrref  //前景色颜色值 
);
### 图像对象的选择
有笔之后，想要绘制就要拿起相应的笔，才能画出相应的画。这个拿起笔的过程，就是SelectObject函数 
HGDIOBJ SelectObject( 
_in HDC hdc,    //设备环境句柄 
_in HGDIOBJ hgdiobj   //被选用的对象的句柄 
); 
第一个参数：HDC类型的hdc，也就是需要使用SelectObject函数的设备环境的句柄 
第二个参数：HGDIOBJ类型的hgdiobj，表示被选择的对象的句柄。除了画笔画刷是GDI对象之外，其他的GDI对象还有位图、字体、区域及调色板等。 
一个设备环境同一时刻只能有一个画笔对象(即使有多个，当前被选中的也只有一个)，也只能有一个画刷对象。GDI对象一经创建便会占用内存，一旦不用就要释放，删除的函数是DeleteObject 
BOOL DeleteObject( 
_in HGDIOBJ hObject 
); 
使用过程：创建—>选用—>删除
### 绘制图形和线条
创建完画笔和画刷，我们下面可以进行绘制。绘制线条LineTo与MoveToEx函数。 
BOOL LineTo( 
_in HDC hdc,   //设备环境句柄 
_in int nxEnd,  //线条终点x坐标值 
_in int nYEnd   //线条终点y坐标值 
); 
LineTo函数不能指定线的起点，而是从画笔对象的当前点开始画，画完后当前点就变成了绘制的终点，当前点初始位置是(0,0) 
使用MoveToEx函数来移动画笔的当前点 
BOOL MoveToEx( 
_in HDC hdc, 
_in int X, 
_in int Y, 
_out LPPOINT lpPoint  //一个指向POINT结构的指针，用来存放上一个点的位置 
); 
绘制矩形的GDI函数Rectangle() 
BOOL Rectangle( 
_in HDC hdc, 
_in int nLeftRect, 
_in int nTopRect, 
_in int nRightRect, 
_in int nBottomRect, 
)
### 游戏随机系统
rand()函数 
int AnswerNumber； 
AnswerNumber =rand()%101   ;产生的0-100之间的随机数 
srand()函数用于设置供rand()使用的随机数种子。如果我们在第一次调用rand()之前没有调用srand()的话，那么系统会为我们自动调用一次srand() 
void srand (unsigned int seed);
通常是利用系统返回当前时间的time的返回值来当做种子(seed) 
srand((unsigned time (NULL));//用系统时间初始化随机种子 
随机数产生的算法
- 产生一个范围内的随机数
j=(int)(100.0*rand()/RAND_MAX+1.0)) 
- 筛选型随机数
x=random(100); 
while(x==6){ 
x=random(100); //0-99的随机数，但不是6 
} 
x=random(100); 
while(x%5==0){ 
x=random(100); //0-99的随机数，但不是5的倍数 
} 
- 从连续的一段范围内取随机数
x=random(11)+40; 
- 从一组乱数中取随机数
a=new Array(67,87,34,12,5,9,108,999,378); 
j=random(10); 
x=a[j]
### 文字输出函数TextOut
用当前选择的字符、背景颜色和正文颜色将一个字符串写到指定位置。简单来讲，就是在指定地方输出一段文字。 
BOOL TextOut( 
 HDC hdc,//需要进行文本输出的DC的句柄 
 int nXStart, //开始书写的位置X坐标 
 int nYStart, //开始书写的位置Y坐标 
 LPCTSTR lpString, //指向字符串的指针 
 int cbString //字符串的字符数 
)
### 进阶文字输出函数DrawText
在指定的矩形里面写入格式化文本，且根据指定的方法对文本格式化 
int DrawText( 
HDC hDC,      //需要进行文本输出的Dc设备环境句柄 
LPCTSTR lpString,//指向需要写入字符串指针 
int nCount,      //需要写入 的字符串的长度 
LPRECT lpRect//指向包含了绘图区域的矩形结构体的指针 
UINTuFormat //书写模式的标识，有多种定制标识可以选择 
);
