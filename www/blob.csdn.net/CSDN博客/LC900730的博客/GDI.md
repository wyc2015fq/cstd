# GDI - LC900730的博客 - CSDN博客
2017年05月22日 21:49:37[lc900730](https://me.csdn.net/LC900730)阅读数：139标签：[图形																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
个人分类：[c++](https://blog.csdn.net/LC900730/article/category/6933816)
GDI是windows图像显示程序与实际物理设备之间的桥梁，GDI使得用户无需关心具体设备的细节，而只需要在一个虚拟的环境中操作； 
GDI为程序提供了图像设备无关的接口，包括视频显示、打印机、画图仪和传真机等。我们利用GDI可以方便的在屏幕或打印机及其他输出设备上输出图形、文本等。 
GDI无非就是几百个函数与一些相关的数据结构和宏组成的整体而已，它的使命就是实现一套通用的图形对象，来向屏幕、内存或者打印机等设备进行绘图操作。 
特点： 
- 不允许程序直接访问物理显示硬件，通常称为“设备环境”的抽象接口间接访问显示硬件
- 程序需要与显示硬件(打印机、显示器等)通讯，必须取得与特定窗口向管理的设备环境
- 用户无需关心具体的物理设备类型
- Windows参考设备环境的数据结构完成数据的输出
## GDI函数类
- 取得和释放设备上下文的函数
- 取得有关设备上下文信息的函数
- 绘图函数
- 设定和取得设备上下文参数的函数
- 使用GDI对象的函数
设备上下文：就是程序可以绘图的地方。 
如果在屏上绘，那么设备Device就是屏幕，DC就是屏幕绘图层； 
如果在窗口中绘制，那么设备就是窗口，DC就是窗口上可以绘图的地方。 
系统中可以有多个设备环境，每一个设备环境都有一个与之对应的关联设备。 
应用程序在进行图像输出时，只需关心设备环境的类型。如果需要将图像输出到特定的设备，只需要创建相应的设备句柄就可以(HDC)。程序在显示区域绘制完毕，必须释放设备环境句柄。句柄就是个数值，程序必须在单个消息期间取得和释放句柄。
### 获取设备环境句柄
#### 方式一：
HDC BeginPaint(*in HWND hwnd,_out* LPPAINTSTRUCT lpPaint 
); 
参数一：HWND类型的hwnd，填写需要获取设备环境句柄的那个窗口句柄； 
参数二：LPPAINTSTRUCT lpPaint 
如HDC g_hdc; 
case WM_PAINT: 
    g_hdc=BeginPaint(hwnd,&paintStruct); 
    Game_Paint(); 
    EndPaint(hwnd,&paintStruct); 
    ValidateRect(hwnd,NULL);  //更新指定窗口无效的矩形区域，使之有效 
    break;
#### 方式二：
HDC GetDc(_in HWND hwnd    //只有一个参数窗口句柄 
); 
但GetDC之后，窗口的DC就处于被占用的状态，使用之后必须释放，不然其他程序无法使用。 
ReleaseDC。
## 定时器
UINT_PTR WINAPI SetTimer(
*In* HWND hWnd, 
*In* UINT_PTR nIDEvent,   //在同一个窗口中必须是唯一，且不为0 
*In* UINT uElapse,    //定时器发出WM_TIMER消息的事件间隔，以千分之一秒为单位，=1000即每隔1秒发出一个WM_TIMER消息 
*In* TIMERPROC lpTimerFunc    //用于指定一个由系统调用处理WM_TIMER消息的响应函数，如果不想通过指定响应函数来处理WM_TIMER消息的话，设置为NULL 
)
### 删除定时器
BOOL WINAPI KillTimer( 
*In_opt* HWND hwnd, 
*In* UINT_PTR uIDEvent     //想要删除的定时器的代号 
)
## 游戏循环
## 透明贴图
### 透明遮罩法
主要是利用Bitblt()函数中的Raster(光栅)值的运算，来将图片中不希望显示的部分处理掉，我们称之为”去背”。 
BOOL BitBlt( 
_in HDC hdcDest, 
_in int nXDest, 
_in int nYDest, 
_in int nWidth, 
_in int nHeight, 
_in HDC hdcSrc, 
_in int nXSrc, 
_in int nYSrc, 
_in DWORD dwRop  
);
### 透明色彩法
