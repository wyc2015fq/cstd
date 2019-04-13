
# Windows CreateFont：创建自己的字体 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:38:13[seven-soft](https://me.csdn.net/softn)阅读数：7805个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



前面无论是使用文本输出函数还是 static 控件，字体都是默认的，比较丑陋，我们完全可以自己创建更加炫酷的字体。
创建字体使用 CreateFont 函数，它的原型是：HFONTCreateFont(
intcHeight,//字体的逻辑高度
intcWidth,//字体的逻辑宽度
intcEscapement,//指定移位向量相对X轴的偏转角度
intcOrientation,//指定字符基线相对X轴的偏转角度
intcWeight,//设置字体粗细程度
DWORDbItalic,//是否启用斜体
DWORDbUnderline,//是否启用下划线
DWORDbStrikeOut,//是否启用删除线
DWORDiCharSet,//指定字符集
DWORDiOutPrecision,//输出精度
DWORDiClipPrecision,//剪裁精度
DWORDiQuality,//输出质量
DWORDiPitchAndFamily,//字体族
LPCSTRpszFaceName//字体名
);

上面的14个参数完全记住几乎不可能，下面是简单的说明，其他的“照猫画虎”即可。
1) 参数 cWidth 和 cHeight 通常取负值，且高度为宽度的2倍，如 -18 和 -9，即可指定 13.5 |(H+W)/2| 磅的字体。
2) 对于字体粗细程度 cWeight，可以使用 FW_ 开头的宏定义；一般取 FW_NORMAL (400)，此参数需要值的值域为 [0,1000]。
3) 对于参数 bItalic、bUnderline、bStrikeOut，直接使用 TRUE 或 FALSE 传值即可。
4) 对于字符集 iCharSet，一般可以直接使用 DEFAULT_CHARSET 让系统自动处理。
> 注意：字体也是一种 GDI 对象，使用完后也要在 WM_DESTROY 消息中删除。
创建完字体后并不能立即使用，还需要手动触发 WM_SETFONT 消息，让Windows 将当前字体设置为我们创建的字体。
发送消息使用 SendMessage 函数，它可以让我们在必要时主动向窗口发送各种消息，原型为：LRESULTSendMessage(
HWNDhWnd,//发送消息的目标窗口句柄
UINTmsg,//将要发送的消息
WPARAMwParam,//附加消息1
LPARAMlParam//附加消息2
);

这些参数和 WndProc 函数非常类似。没错，SendMessage 发送的消息就是由窗口过程来处理的。
设置字体的消息是 WM_SETFONT，只要向附加参数 wParam 传入 CreateFont 返回的句柄即可（需要转换到 WPARAM 类型）。请看下面的例子：//窗口过程
LRESULTCALLBACKWndProc(
HWNDhWnd,
UINTmessage,
WPARAMwParam,
LPARAM lParam
){
PAINTSTRUCTps;
HDChdc;
staticHFONThFont;//声明一个逻辑字体句柄为静态变量
staticHWNDlabExample;//在初期，我们将其声明为静态变量
switch(message){
caseWM_CREATE:
//创建逻辑字体
hFont=CreateFont(
-15/*高度*/,-7.5/*宽度*/,0/*不用管*/,0/*不用管*/,400/*一般这个值设为400*/,
FALSE/*不带斜体*/,FALSE/*不带下划线*/,FALSE/*不带删除线*/,
DEFAULT_CHARSET,//这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,//这行参数不用管
DEFAULT_QUALITY,//默认输出质量
FF_DONTCARE,//不指定字体族*/
L"微软雅黑"//字体名
);
//创建静态文本框控件
labExample=CreateWindow(
L"static",//静态文本框的类名
L"你好，欢迎来到C语言中文网",//控件的文本
WS_CHILD/*子窗口*/|WS_VISIBLE/*创建时显示*/|WS_BORDER/*带边框*/,
60/*X坐标*/,60/*Y坐标*/,150/*宽度*/,100/*高度*/,hWnd/*父窗口句柄*/,
(HMENU)1,//为控件指定一个唯一标识符
hInst,//当前实例句柄
NULL
);
//设置控件字体
SendMessage(
labExample,//欲设置字体的控件句柄
WM_SETFONT,//消息名（消息类型）
(WPARAM)hFont,//字体句柄
NULL//传空值即可
);
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
//请做好善后工作
DeleteObject(hFont);//删除创建的字体
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150729/1-150H9141K5W1.png)
如上设置字体的方法几乎适用于所有的 Win32 标准控件。

