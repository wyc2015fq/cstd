# 在VC里装了easyx并不是万事大吉了~ - qq_37385726的博客 - CSDN博客





2017年02月02日 00:28:13[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1640







easyX是基于C++开发的，与TC的图形库一定程度上保持一致，但是不代表相同的代码可以同样地运行！参考当然是可以参考的，另外，如果你想移植，可以参考下面的
[使用技巧]将 TC 下的绘图和游戏程序移植到 VC 下  

Turbo C（简称tc）是 dos 时代经典的编译器，学习编程时，有很多图形类和游戏类的程序都是用 tc 编写的。由于 c 语言标准的不完善，这些程序大多数都不能直接在 vc 里面编译。本文就讲解怎样将 tc 程序移植到 vc 下。

注：本文以 VC 6.0 为范例（高版本的 vc 大同小异），同时请安装最新版本的 EasyX 图形库。

移植前请注意：

确保 tc 代码可以正确运行。
将 tc 代码复制粘贴到 vc 里面。注意，vc 里面代码文件的扩展名要用 .cpp，不能用 .c。
以下解决办法不分先后顺序，哪条对应就按照哪条修改。
如果没找到对应的解决办法，可以暂时跳过，先改后面的。可能修改了第 3 条错误，第 2 条错误也就没有了。
可能解决一个问题后，会出现 10 个新问题，这不要紧。细心、耐心一些，一个一个问题慢慢处理，并不复杂。

以下正文。

错误提示：fatal error C1083: Cannot open include file: '[bios.h](https://www.baidu.com/s?wd=bios.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf)': No such file or directory
解决办法：删掉 #include <[bios.h](https://www.baidu.com/s?wd=bios.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf)>

错误提示：fatal error C1083: Cannot open include file: '[dos.h](https://www.baidu.com/s?wd=dos.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf)': No such file or directory
解决办法：删掉 #include <[dos.h](https://www.baidu.com/s?wd=dos.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf)>

错误提示：error [C2065](https://www.baidu.com/s?wd=C2065&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf): 'bioskey' : undeclared identifier
解决办法：根据 bioskey 的参数，适当的用 getch / kbhit 代替。由于 bioskey 参数较多，具体修改方法请参考：[http://www.easyx.cn/skills/View.aspx?id=77](http://www.easyx.cn/skills/View.aspx?id=77)

错误提示：error [C2065](https://www.baidu.com/s?wd=C2065&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf): 'getch' : undeclared identifier
解决办法：由于 tc2.0 对于一些常用函数可以不引用头文件，这是不标准的写法，在之后的编译器都会报错。请增加相关的头文件引用：#include <[conio.h](https://www.baidu.com/s?wd=conio.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf)>

错误提示：error [C2065](https://www.baidu.com/s?wd=C2065&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf): 'registerbgidriver' : undeclared identifier
解决办法：删除错误行代码：registerbgidriver(xxx);

错误提示：error C2065: 'detectgraph' : undeclared identifier
解决办法：删除错误行代码：detectgraph(xxx);

错误提示：error C2065: 'restorecrtmode' : undeclared identifier
解决办法：删除错误行代码：restorecrtmode();

错误提示：error C2065: 'setviewport' : undeclared identifier
解决办法：请参考文章：[http://www.easyx.cn/skills/View.aspx?id=53](http://www.easyx.cn/skills/View.aspx?id=53) ;

错误提示：error C2065: 'sleep' : undeclared identifier
解决办法：改用 Sleep，并将参数扩大 1000 倍。例如，原来是 sleep(3);，修改为 Sleep(3000);

错误提示：error C2065: 'delay' : undeclared identifier
解决办法：将 delay 修改为 Sleep 即可。

错误提示：error C2065: 'EGA' : undeclared identifier
或：error C2065: 'EGAHI' : undeclared identifier
解决办法：通常在含有该错误的代码行会有类似定义：

　　int gdriver = EGA, gmode = EGAHI;
将这行代码删掉即可。这两个变量在之后会这样使用：
initgraph(&gdriver, &gmode, "");
将这行代码修改为
initgraph(640, 480);
问题即可解决。

错误提示：error C2065: 'getmaxx' : undeclared identifier
解决办法：将 getmaxx() 修改为 getwidth();

错误提示：error C2065: 'getmaxy' : undeclared identifier
解决办法：将 getmaxy() 修改为 getheight();

错误提示：error C2065: 'graphresult' : undeclared identifier
解决办法：将 graphresult(); 用数字 0 替换。

错误提示：error C2065: 'grOk' : undeclared identifier
解决办法：将 grOk 用数字 0 替换。

错误提示：error C2065: 'grapherrormsg' : undeclared identifier
解决办法：将 grapherrormsg(xxx) 用一对半角空双引号 "" 替换。

错误提示：error C2065: '_AX' : undeclared identifier  （可能是 _AH、_AL、_BX、_CX 等寄存器）
错误提示：error C2065: 'geninterrupt' : undeclared identifier
解决办法：前面两个错误提示通常会一起发生，这些代码是调用中断时使用的，比如在 TC 里面调用鼠标的代码。所以，具体解决办法要根据调用什么中断来解决，需要查中断手册。例如以下的 TC 代码：
void GetMousePos()
{
    _AX=0x33;
    geninterrupt(0x33);
    MouseX=_CX;
    MouseY=_DX;
}
查询中断手册后可以得知，该功能是用来获取鼠标的当前坐标。所以，这个 GetMousePos() 函数就要相应的修改成 VC 版本，例如：
void GetMousePos()
{
    HWND hwnd = GetHWnd();
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hwnd, &point);
    MouseX = point.x;
    MouseY = point.y;
}

错误提示：error C2665: 'settextstyle' : none of the 4 overloads can convert parameter 3 from type 'const int'
解决办法：settextstyle 是 tc 下设置字体的函数，功能很简单，第一个参数设置字形（0-4 共 5 
种），第二个参数设置文字输出方向（0、1 分别表示横、竖），第三个参数设置文字大小（设置为 n 表示高度为 8 * n）。在 easyx 下的 
settextstyle 函数实现了更丰富的字体设置，需要做适当移植，详见帮助中 settextstyle 函数的说明。

错误提示：error C2660: 'ellipse' : function does not take 6 parameters
解决办法：tc 里面的 ellipse() 函数用来画椭圆弧，在 EasyX 里面改用 arc() 函数来完成。除了需要将函数名由 ellipse 修改为 arc 外，还需要调整对应的参数。请参考 EasyX 帮助文档获取函数原型并作适当调整。例如，tc 中代码：
ellipse(x, y, sa, ea, xr, yr);
在 vc 中修改为：
arc(x - xr, y - yr, x + xr, y + yr, sa * PI / 180, ea * PI / 180);

错误提示：error C2660: 'arc' : function does not take 5 parameters
修改方法：tc 里面的 arc() 函数用来画圆弧，和 EasyX 里面 arc() 函数的参数略有不同，需要调整对应的参数。请参考 EasyX 帮助文档获取函数原型并作适当调整。例如，tc 中代码：
arc(x, y, r, sa, ea);
在 vc 中修改为：
arc(x - r, y - r, x + r, y + r, sa * PI / 180, ea * PI / 180);

错误提示：error C2065: 'settextjustify' : undeclared identifier
解决办法：删除 settextjustify(xxx) 语句。该函数目前在 vc 下没有特别理想的替换方案。该函数目的是调整 outtextxy
 输出字符串的位置，删除后，后面的文字输出位置会有偏移（不会影响程序执行流程），因此还需要根据情况调整后面的文字输出语句的坐标，或者替换为 
drawtext 函数等。

错误提示：error C2065: 'gotoxy' : undeclared identifier
解决办法：严格来说，gotoxy 函数并不是在 [graphics.h](https://www.baidu.com/s?wd=graphics.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf) 里面定义的，这个函数是 tc 的 [conio.h](https://www.baidu.com/s?wd=conio.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nHfYPWfkrHm4P1f3nvf40ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHf4PjfvPHfvrHRLnjcsnHR1rf) 里面定义的，在 vc 中没有。解决这个错误，需要在程序中增加一个自定义函数，如下：

// 移动光标到指定位置
void gotoxy(int x, int y)
{
	COORD coord = {x - 1, y - 1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

如果编译没有错误了，但是程序还是不能运行，就需要修改一些运行时导致问题的地方：

1. setcolor 等颜色相关。
例如，setcolor(4);，在 TC 下设置为 4 号调色板颜色，默认是红色，但在 EasyX 下，可以使用 24 位真彩色，每个颜色分量都是 0~255，那么 4 号颜色只表示很暗的红色，肉眼
几乎看不到。因此，setcolor(4) 可以修改为 setcolor(RED)，等等。其他涉及到颜色的语句也要这么修改。

2. setfillstyle
tc 下的 setfillstyle 函数在 EasyX 下对应为两个函数：setfillcolor 和 setfillstyle，因此需要做相应调整。
tc 下的 setfillstyle 语句格式为：
void setfillstyle(int pattern, int color);
常见的 pattern 为 1 或 SOLID_FILL，表示固实填充。
而 EasyX 的 setfillcolor 和 setfillstyle 语句格式为：
void setfillcolor(COLORREF color);
void setfillstyle(int pattern, const char *pupattern = NULL);
例如，tc 中的：
setfillstyle(1, 4); // 设置为红色固实填充
在 EasyX 中可以修改为以下两条语句：
setfillcolor(RED);
setfillstyle(SOLID_FILL);


