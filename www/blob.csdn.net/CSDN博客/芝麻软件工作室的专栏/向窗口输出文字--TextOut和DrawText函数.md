
# 向窗口输出文字--TextOut和DrawText函数 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:35:34[seven-soft](https://me.csdn.net/softn)阅读数：9560个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



在《[一个完整的Windows程序框架](http://c.biancheng.net/cpp/html/2949.html)》一节中，我们使用
 DrawText() 向客户区的中间位置输出了一段文本，但并没有详细解释，这节我们来重点说一下文字的输出。
## 设备环境（DC）
在Windows中，一切能在显示器上看到的都是图形，不但包括点、线、矩形、圆等，也包括文字、字体。对用户来说文字和图片是不同的，但它们在本质上都是图形，都需要在显示器上画出来。这也是Windows被称作图形操作系统的原因。
Windows不允许我们直接访问硬件，如键盘、显示器、打印机等，在与这些硬件通信前要获得设备环境（Device
 Context，简称 DC），进而间接的访问硬件。
> 设备环境有时也被称为设备上下文或设备描述表。
所谓设备环境，就是与当前硬件设备有关的各种信息，它是硬件设备的抽象。也就是说，设备环境就代表当前的硬件。
与文字输出、图形绘制有关的函数，在使用时大都需要传入一个参数，就是设备环境句柄。获取设备环境句柄有多种方式，常用的是 BeginPaint 函数。绘图完成后，还要释放句柄，使用 EndPaint 函数。
## BeginPaint 和 EndPaint 函数
当发生 WM_PAINT 事件时，应用程序开始绘图。绘图之前需要先调用 BeginPaint 函数通知 Windows，告诉它当前程序需要使用显示器，你安排一下吧。BeginPaint 函数执行完毕就返回设备环境句柄，接下来就可以使用
 GDI 函数绘图了。
绘图结束后，也需要调用 EndPaint 函数通知Windows，告诉它绘图结束了，你可以继续后面的工作了。
## TextOut 函数
TextOut 函数可以在客户区输出一段文本，原型为：BOOLTextOut(
HDChdc,//设备环境句柄
intnXStart,//开始输出位置的x坐标
intnYStart,//开始输出位置的y坐标
LPCTSTRlpString,//要输出的字符串
intcbString//字符串的长度
);

每当发生WM_PAINT事件时，窗口就会发生重绘，这个时候需要向窗口输出文字：LRESULTCALLBACKWndProc(
HWNDhwnd,
UINTmessage,
WPARAMwParam,
LPARAM lParam
){
HDChdc;//设备环境句柄
PAINTSTRUCTps;//存储绘图环境的相关信息
TCHARszText[20]=TEXT("欢迎来到C语言中文网");
switch(message){
caseWM_PAINT:
//开始绘图并返回环境句柄
hdc=BeginPaint(hwnd,&ps);
//输出文字
TextOut(hdc,50,50,szText,wcslen(szText));
//结束绘图并释放环境句柄
EndPaint(hwnd,&ps);
return0;
caseWM_DESTROY:
PostQuitMessage(0);
return0;
}
returnDefWindowProc(hwnd,message,wParam,lParam);
}

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150725/1-150H5095QH51.png)
几点说明：
1) PAINTSTRUCT 结构体用来保存与绘图环境有关的信息，本教程不会使用到，不再讲解，有兴趣的读者可自行Google或百度。
2) BeginPaint 和 EndPaint 函数用来开始绘图和结束绘图，必须成对出现，输出文字或绘制图形的代码也必须放在两个函数之间。
3) 字符串一般使用宽字符，对应的计算字符串长度的函数为 wcslen()。
## DrawText函数
TextOut 在一个起点开始输出文本，可以精确定位，不能自动换行；DrawText 在指定区域内输出文本，可以控制格式对齐（居左、居中、居右），可以换行。
DrawText 在内部其实也是调用TextOut ，不过它作了很多处理，比如 DrawText 无需知道输出多少个字符。更重要的是，DrawText 可以处理换行符而 TextOut 不能，从这个角度看，DrawText
 应该是一种更高级的用法，是在TextOut基础上的封装。
DrawText 的原型是：intDrawText(
HDChDC,//设备环境句柄
LPCTSTRlpString,//将要输出的字符串
intnCount,//字符串的长度
LPRECTlpRect,//指向一个矩形区域的结构体 RECT 的指针
UINTuFormat//输出格式
);

两点说明：
1) 如果 nCount 为 -1，则表明 lpString 指向的字符串是以`'\0'`结束的，DrawText
 会自动计算字符数。
2) 参数 uFormat 有四种取值，它们可以任意组合：DT_CALCRECT：这个参数比较重要，可以使 DrawText 函数计算出输出文本的尺寸。如果输出文本有多行，DrawText 函数使用 lpRect 定义的矩形的宽度，并扩展矩形的底部以容纳输出文本的最后一行。如果输出文本只有一行，则 DrawText 函数改变矩形的右边界，以容纳下正文行的最后一个字符。出现上述任何一种情况。
DT_CENTER：指定文本水平居中显示。
DT_VCENTER：指定文本垂直居中显示。该标记只在单行文本输出时有效，所以它必须与DT_SINGLELINE结合使用。
DT_SINGLELINE：单行显示文本，回车和换行符都不断行。
DrawText 示例：//窗口过程
LRESULTCALLBACKWndProc(HWNDhwnd,UINTmessage,WPARAMwParam,LPARAMlParam){
HDChdc;//设备环境句柄
PAINTSTRUCTps;
RECTrect;
switch(message){
//窗口绘制消息
caseWM_PAINT:
hdc=BeginPaint(hwnd,&ps);
GetClientRect(hwnd,&rect);
DrawText(
hdc,
TEXT("你好，欢迎来到C语言中文网"),
-1,
&rect,
DT_SINGLELINE|DT_CENTER|DT_VCENTER
);
EndPaint(hwnd,&ps);
return0;
//窗口销毁消息
caseWM_DESTROY:
PostQuitMessage(0);
return0;
}
returnDefWindowProc(hwnd,message,wParam,lParam);
}

运行结果：
![](http://c.biancheng.net/cpp/uploads/allimg/150801/1-150P1105301N1.png)
改变窗口大小时，文本会自动调整位置，一直处于窗口中间。
GetClientRect 函数用来获得窗口客户区的坐标，也就是获得客户区这个巨型。

