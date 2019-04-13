
# Windows GDI绘图基础与轻量进阶 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:36:23[seven-soft](https://me.csdn.net/softn)阅读数：2785


GDI 是 Graphics Device Interface 的缩写，称为图形设备接口，主要用来绘图，由动态链接库 GDI32.DLL 提供支持。
GDI 就是一个函数库，提供了很多绘图函数（也就是GDI32.DLL 中的导出函数），上节使用的 TextOut 就是其中之一。GDI 非常重要，不但应用程序使用它来绘图，Windows 本身也使用GDI来显示用户界面，比如菜单、滚动条、图标和鼠标指针等。
## GDI 基础
这一部分讲解如何绘制简单的图形，而在轻量进阶部分讲解如何美化图形。
#### 1) 绘制矩形
Rectangle 函数可以在窗口上绘制一个矩形，它的原型为：BOOLRectangle(
HDChdc,//设备环境句柄
intnLeftRect,//矩形左上角x坐标
intnTopRect,//矩形左上角y坐标
intnRightRect,//矩形右下角x坐标
intnBottomRect//矩形右下角y坐标
);

示例代码：caseWM_PAINT:
hdc=BeginPaint(hwnd,&ps);
Rectangle(hdc,50,50,150,150);
EndPaint(hwnd,&ps);
return0;

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150724/1-150H41K1551L.png)
注意：坐标的原点都是客户区的左上角。
#### 2) 带圆角的矩形
如果您觉得上面矩形过于方正，那么可以使用 RoundRect 函数，它可以画出带有圆角边框的矩形，原型为：BOOLRoundRect(
HDChdc,//设备环境句柄
intnLeftRect,//矩形左上角x坐标
intnTopRect,//矩形左上角y坐标
intnRightRect,//矩形右下角x坐标
intnBottomRect,//矩形右下角y坐标
intnWidth,//用来画圆角的椭圆的宽度
intnHeight//用来画圆角的椭圆的高度
);

注意：当 nHeight >= nBottomRect 且 nWidth = nRightRect 时，那么绘制出的就是一个圆。
示例代码：caseWM_PAINT:
hdc=BeginPaint(hwnd,&ps);
RoundRect(hdc,20,20,150,150,25,25);
EndPaint(hwnd,&ps);
return0;

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150724/1-150H41P012b2.png)
#### 3) 绘制椭圆
Ellipse() 函数可以用来绘制椭圆，它的原型为：BOOLEllipse(
HDChdc,//设备环境句柄
intnLeftRect,//左上角x坐标
intnTopRect,//左上角y坐标
intnRightRect,//右下角x坐标
intnBottomRect//右下角y坐标
);

注意：当 nRightRect - nLeftRect = nBottomRect - nRightRect 时绘制出的是一个圆。
示例代码：caseWM_PAINT:
hdc=BeginPaint(hwnd,&ps);
Ellipse(hdc,20,20,180,90);
EndPaint(hwnd,&ps);
return0;

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150724/1-150H41PHK24.png)
#### 4) 绘制直线
绘制直线需要确定起点和终点。
确定起点使用 MoveToEx 函数。MoveToEx 用来指定画笔的起始位置，也就是从哪里开始画，它的原型为：BOOLMoveToEx(
HDChdc,//设备环境句柄
intx,//起始位置x坐标
inty,//起始位置y坐标
LPPOINTlpPoint//指向用于保存当前位置的POINT结构体的指针
);

对于参数 lpPoint，我们并不需要保存当前位置，所以直接指定为 NULL 即可。
> 注意：win32不再支持 MoveTo，只支持它的扩展函数 MoveToEx。
有了起点，接下来就可以使用 LineTo 函数画直线了。LineTo 函数用于从当前绘图位置向指定点绘制一条直线，它的原型为：BOOLLineTo(
HDChdc,//设备环境句柄
intxEnd,//终点的x坐标
intyEnd//终点的y坐标
);

示例代码：caseWM_PAINT:
hdc=BeginPaint(hwnd,&ps);
MoveToEx(hdc,150,150,NULL);//设定起始点,不保存当前点坐标
LineTo(hdc,200,60);//第一条线
LineTo(hdc,250,150);//第二条线
LineTo(hdc,150,150);//第三条线
EndPaint(hwnd,&ps);
return0;

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150724/1-150H4211219C0.png)
## GDI 绘图轻量进阶--画笔和画刷
画笔和画刷都用来在画布上绘图。画布就是用来绘画的一块背景，可以有颜色也可以没有，也可以有图案（比如条纹、网格等）；画布可以理解为我们平时作图时使用的纸。
画笔用来画线，可以是封闭的也可以是开放的，比如直线、曲线、圆形、矩形等。
画刷用来填充背景或者一块区域，一般带颜色或图案。
比如画一个矩形，需要先找一块画布，然后用画笔画出矩形的轮廓（线条），再用画刷给矩形区域喷涂上颜色。
画笔与画刷的区别：画笔一般用来画线条，画轮廓；画刷一般用来进行大面积绘制，比如给背景着色，填充画笔画出的一块封闭的区域等。
#### 1) 创建和使用画笔
上面的绘图使用的是Windows的默认画笔，也就是宽度为1个像素，颜色为黑色的画笔。我们也可以创建自己的画笔。
创建画笔的API函数为 CreatePen：HPENCreatePen(
intnPenStyle,//画笔的样式
intnWidth,//画笔的宽度
COLORREFcrColor//画笔的颜色
);

画笔样式 nPenStyle 有7种取值：
|宏定义|宏定义对应的值|说明|
|PS_SOLID|0|实线|
|PS_DASH|1|虚线（段线），要求画笔宽度 <= 1|
|PS_DOT|2|点线，要求画笔宽度 <= 1|
|PS_DASHDOT|3|线、点，要求画笔宽度 <= 1|
|PS_DASHDOTDOT|4|线、点、点，要求画笔宽度 <= 1|
|PS_NULL|5|不可见|
|PS_INSIDEFRAME|6|实线，但画笔宽度是向里扩展的|
画笔宽度 nWidth 指逻辑宽度。iWidth为 0 则意味着画笔宽度为一个像素。如果画笔样式为点线或者虚线，同时又指定一个大于 1 的画笔宽度，那么Windows将使用实线画笔来代替。
画笔的颜色 crColor 可以直接使用 RGB 颜色。RGB 是一种标准颜色，通过红(R)、绿(G)、蓝(B)三原色的叠加得到各种不同的颜色。
细心的读者可能已经发现，CreatePen 函数在创建画笔时并没有指定设备环境，也就是说，新创建的画笔与当前设备环境并没有关联，无法使用。
画笔、画刷、字体等被称为GDI对象。你可以将GDI对象理解为工具，可以供
 GDI 函数使用。新创建的 GDI 对象必须通过 SelectObject 函数选入设备环境才能使用。
SelectObject 函数将GDI对象与设备环境关联起来，它的原型为：HGDIOBJSelectObject(
HDChdc,//设备环境句柄
HGDIOBJho//GDI对象句柄
);

下面的代码会创建一个红色的画笔，并画出一个三角形：//窗口过程
LRESULTCALLBACKWndProc(HWNDhWnd,UINTmessage,WPARAMwParam,LPARAMlParam){
PAINTSTRUCTps;
HDChdc;
//定义一个画笔句柄,请定义为静态变量
staticHPENhPen;
switch(message){
caseWM_CREATE:
//创建宽度为2个像素的红色点线画笔，保存句柄到 hPen 变量
hPen=CreatePen(PS_SOLID,2,RGB(255,0,0));
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
//选入画笔到设备环境
SelectObject(hdc,hPen);
//绘制三角形
MoveToEx(hdc,150,150,NULL);
LineTo(hdc,200,60);//第一条线
LineTo(hdc,250,150);//第二条线
LineTo(hdc,150,150);//第三条线
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
//请做好善后工作,处理 WM_DESTROY 消息时删除之前我们创建的一切GDI对象
DeleteObject(hPen);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150724/1-150H421522R05.png)
两点注意：
A) 画笔最好在 WM_CREATE 事件中创建，因为当应用程序运行时会频繁地触发 WM_PAINT 事件，比如窗口被覆盖后再显示、窗口被拖动、窗口被拉伸等，每次都需要重新创建画笔，浪费资源，也没有必要。
B) 所有创建的GDI对象，在窗口被关闭时（会触发 WM_DESTROY 事件）都要删除掉，以释放内存。
#### 2) 创建和使用画刷
Windows API 中有两个函数可以用来创建画刷。
CreateSolidBrush 函数可以用来创建一个指定颜色的实心画刷，原型为：HBRUSH CreateSolidBrush( COLORREF crColor );  // crColor为画刷颜色CreateHatchBrush 函数可以用来创建一个指定颜色的含有特定阴影样式的画刷，原型为：HBRUSHCreateHatchBrush(
intfnStyle,//画刷样式
COLORREFcrColor//画刷颜色
);

fnStyle 可以有6种取值：HS_BDIGONAL：45度向上，自左至右的阴影(///)
HS_CROSS：表示水平直线和垂直直线交叉阴影(+++)
HS_DIAGCROSS：45度交叉阴影(XXX)
HS_FDIAGONAL：45度向下自左至右的阴影(\\\)
HS_HORIZONTAL：水平阴影(---)
HS_VERTICAL：垂直阴影
画刷使用举例：[复制](http://c.biancheng.net/cpp/html/2953.html#)[纯文本](http://c.biancheng.net/cpp/html/2953.html#)[新窗口](http://c.biancheng.net/cpp/html/2953.html#)
//窗口过程
LRESULTCALLBACKWndProc(HWNDhWnd,UINTmessage,WPARAMwParam,LPARAMlParam){
PAINTSTRUCTps;
HDChdc;
//定义两个画刷,请定义为静态变量
staticHBRUSHhSolidBrush;
staticHBRUSHhHatchBrush;
switch(message){
caseWM_CREATE:
//创建蓝色实心画刷，保存句柄到 hSolidBrush 变量
hSolidBrush=CreateSolidBrush(RGB(0,0,255));
//创建绿色交叉阴影画刷，保存句柄到 hHatchBrush 变量
hHatchBrush=CreateHatchBrush(HS_DIAGCROSS,RGB(0,255,0));
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
//选入蓝色实心画刷到设备环境
SelectObject(hdc,hSolidBrush);
Rectangle(hdc,0,0,200,100);//绘制矩形
//选入绿色交叉画刷到设备环境
SelectObject(hdc,hHatchBrush);
Ellipse(hdc,0,100,200,200);//绘制椭圆
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
//请做好善后工作,处理 WM_DESTROY 消息时删除之前我们创建的一切GDI对象。
DeleteObject(hSolidBrush);
DeleteObject(hHatchBrush);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150724/1-150H4221625138.png)

