
# Windows button控件（按钮控件） -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:38:59[seven-soft](https://me.csdn.net/softn)阅读数：2780


按钮既是标准控件，也是子窗口，窗口类名是`button`。
Button 控件样式以`BS`开头，常用的有：
|样式|说明|
|BS_LEFT|文本居左。|
|BS_RIGHT|文本居右。|
|BS_CENTER|文本水平居中（默认为 BS_CENTER）。|
|BS_BOTTOM|文本位于按钮底部。|
|BS_TOP|文本位于按钮顶部。|
|BS_VCENTER|文本垂直居中（默认为 BS_VCENTER）。|
|BS_FLAT|扁平样式。默认情况下按钮具有3D阴影效果。|
|BS_MULTILINE|允许显示多行文本。也就是说当文本过长时会自动换行。|
按钮控件示例：//窗口过程
LRESULTCALLBACKWndProc(
HWNDhWnd,
UINTmessage,
WPARAMwParam,
LPARAM lParam
){
intwmId,wmEvent;
PAINTSTRUCTps;
HDChdc;
staticHFONThFont;
staticHWNDhBtn;
switch(message){
caseWM_CREATE:
//创建逻辑字体
hFont=CreateFont(
-15/*高度*/,-7.5/*宽度*/,0,0,400/*一般这个值设为400*/,
FALSE/*不带斜体*/,FALSE/*不带下划线*/,FALSE/*不带删除线*/,
DEFAULT_CHARSET,//使用默认字符集
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,//这行参数不用管
DEFAULT_QUALITY,//默认输出质量
FF_DONTCARE,//不指定字体族*/
TEXT("微软雅黑")//字体名
);
//创建按钮控件
hBtn=CreateWindow(
TEXT("button"),//按钮控件的类名
TEXT("这是按钮"),
WS_CHILD|WS_VISIBLE|WS_BORDER|BS_FLAT/*扁平样式*/,
30/*X坐标*/,20/*Y坐标*/,150/*宽度*/,50/*高度*/,
hWnd,(HMENU)2/*控件唯一标识符*/,hInst,NULL
);
SendMessage(hBtn,WM_SETFONT,(WPARAM)hFont,NULL);//设置按钮字体
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
//请做好善后工作
DeleteObject(hFont);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

运行结果：
![](http://c.biancheng.net/cpp/uploads/allimg/150814/1-150Q4113H4b6.png)
## 捕获按钮点击事件
当用户点击按钮、菜单、下拉列表框等控件的时候，会产生`WM_COMMAND`消息。WM_COMMAND
 消息会附带控件 ID。
我们来回顾一下 CreateWindow 的第9个参数`HEMU
 hMenu`，对于每一个控件它的值都是唯一的，在按钮控件中，我们需要根据它来识别到底是哪个按钮被点击了。
按钮控件示例：LRESULTCALLBACKWndProc(
HWNDhWnd,
UINTmessage,
WPARAMwParam,
LPARAM lParam
){
intwmId,wmEvent;
PAINTSTRUCTps;
HDChdc;
staticHFONThFont;
staticHWNDhBtn;
staticHWNDhStatic;
switch(message){
caseWM_CREATE:
//创建逻辑字体
hFont=CreateFont(
-15/*高度*/,-7.5/*宽度*/,0,0,400/*一般这个值设为400*/,
FALSE/*不带斜体*/,FALSE/*不带下划线*/,FALSE/*不带删除线*/,
DEFAULT_CHARSET,//使用默认字符集
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,//这行参数不用管
DEFAULT_QUALITY,//默认输出质量
FF_DONTCARE,//不指定字体族*/
TEXT("微软雅黑")//字体名
);
//创建静态文本控件
hStatic=CreateWindow(
TEXT("static"),//静态文本框的类名
TEXT("你好，欢迎来到C语言中文网"),//控件的文本
WS_CHILD/*子窗口*/|WS_VISIBLE/*创建时显示*/|WS_BORDER/*带边框*/,
30/*X坐标*/,20/*Y坐标*/,150/*宽度*/,80/*高度*/,hWnd/*父窗口句柄*/,
(HMENU)1,//为控件指定一个唯一标识符
hInst,//当前实例句柄
NULL
);
//创建按钮控件
hBtn=CreateWindow(
TEXT("button"),//按钮控件的类名
TEXT("点击这里试试"),
WS_CHILD|WS_VISIBLE|WS_BORDER|BS_FLAT/*扁平样式*/,
30/*X坐标*/,110/*Y坐标*/,150/*宽度*/,50/*高度*/,
hWnd,(HMENU)2/*控件唯一标识符*/,hInst,NULL
);
SendMessage(hStatic,WM_SETFONT,(WPARAM)hFont,NULL);//设置文本框字体
SendMessage(hBtn,WM_SETFONT,(WPARAM)hFont,NULL);//设置按钮字体
break;
caseWM_COMMAND:
wmId=LOWORD(wParam);
wmEvent=HIWORD(wParam);
switch(wmId){
case2://按下按钮
//更改文本框的内容
SetWindowText(hStatic,TEXT("你点击了下面的按钮"));
break;
default:
//不处理的消息一定要交给 DefWindowProc 处理。
returnDefWindowProc(hWnd,message,wParam,lParam);
}
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
//请做好善后工作
DeleteObject(hFont);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

运行结果：
![](http://c.biancheng.net/cpp/uploads/allimg/150814/1-150Q4150122348.png)
点击按钮，文本框中的文本会改变。
对于上面的代码，你肯定还有很多不解的地方，我们将在下一节深入讲解。
本节示例代码完整版下载地址：[http://pan.baidu.com/s/1bnbNn59](http://pan.baidu.com/s/1bnbNn59)提取密码：21iw

