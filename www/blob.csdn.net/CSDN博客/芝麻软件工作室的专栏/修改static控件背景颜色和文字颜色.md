
# 修改static控件背景颜色和文字颜色 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:41:44[seven-soft](https://me.csdn.net/softn)阅读数：4925


当 static 控件或具有 ES_READONLY 风格的 edit 控件被绘制时，会向父窗口发送 WM_CTLCOLORSTATIC 消息。如果我们在窗口过程中处理该消息，就必须返回一个画刷句柄，Windows 会使用该画刷来绘制控件背景（子窗口背景）。
也就是说，改变 static 控件的背景颜色只有这一种方式，就是处理 WM_CTLCOLORSTATIC 消息。WM_CTLCOLORSTATIC 消息就是用来设置背景画刷的。
改变 static 控件的背景颜色：LRESULTCALLBACKWndProc(HWNDhWnd,UINTmessage,WPARAMwParam,LPARAMlParam){
PAINTSTRUCTps;
HDChdcWnd;
staticHFONThFont;//逻辑字体
staticHWNDhStatic;//静态文本框控件
staticHBRUSHhBrush;//画刷
switch(message){
caseWM_CREATE:
//创建画刷
hBrush=CreateSolidBrush(RGB(0x41,0x96,0x4F));//翠绿色
//创建逻辑字体
hFont=CreateFont(-14/*高*/,-7/*宽*/,0,0,400/*400表示正常字体*/,
FALSE/*斜体?*/,FALSE/*下划线?*/,FALSE/*删除线?*/,DEFAULT_CHARSET,
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
FF_DONTCARE,TEXT("微软雅黑")
);
//创建静态文本框控件
hStatic=CreateWindow(TEXT("static"),TEXT("欢迎来到C语言中文网！"),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_CENTER,
20/*x坐标*/,20/*y坐标*/,240/*宽度*/,60/*高度*/,
hWnd/*父窗口句柄*/,(HMENU)1/*控件ID*/,hInst/*当前程序实例句柄*/,NULL
);
//设置控件的字体
SendMessage(hStatic,WM_SETFONT,(WPARAM)hFont,NULL);
break;
caseWM_CTLCOLORSTATIC:
return(INT_PTR)hBrush;//这里必须返回画刷句柄
caseWM_PAINT:
hdcWnd=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
DeleteObject(hBrush);
DeleteObject(hFont);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

运行结果：
![](http://c.biancheng.net/cpp/uploads/allimg/150814/1-150Q4220532R0.png)
## 修改文字背景颜色
上图中，我们的代码只是改变了控件的背景颜色（整个子窗口的背景颜色），却忽略了文字的背景颜色。
修改文字背景颜色使用 SetBkColor 函数，它的原型为：COLORREFSetBkColor(
HDChdc,//设备环境句柄
COLORREFcrColor//背景颜色
);

修改文字前景色（文字颜色）使用 SetTextColor 函数，它的原型为：COLORREFSetTextColor(
HDChdc,//设备环境句柄
COLORREFcrColor//背景颜色
);

这里我们修改的是 static 控件中的文本颜色，所以需要获取 static 控件的设备环境句柄。非常巧妙的是，发送 WM_CTLCOLORSTATIC 消息时，wParam 参数表示的就是 static 控件的设备环境句柄（lParam
 表示控件句柄）。
下面的代码会修改文本的背景色和前景色：LRESULTCALLBACKWndProc(HWNDhWnd,UINTmessage,WPARAMwParam,LPARAMlParam){
PAINTSTRUCTps;
HDChdcWnd;
HDChdcStatic;
staticHFONThFont;//逻辑字体
staticHWNDhStatic;//静态文本框控件
staticHBRUSHhBrush;//画刷
switch(message){
caseWM_CREATE:
//创建画刷
hBrush=CreateSolidBrush(RGB(0x41,0x96,0x4F));
//创建逻辑字体
hFont=CreateFont(-14/*高*/,-7/*宽*/,0,0,700/*700表示粗体*/,
FALSE/*斜体?*/,FALSE/*下划线?*/,FALSE/*删除线?*/,DEFAULT_CHARSET,
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
FF_DONTCARE,TEXT("微软雅黑")
);
//创建静态文本框控件
hStatic=CreateWindow(TEXT("static"),TEXT("欢迎来到C语言中文网！"),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_CENTER,
20/*x坐标*/,20/*y坐标*/,240/*宽度*/,60/*高度*/,
hWnd/*父窗口句柄*/,(HMENU)1/*控件ID*/,hInst/*当前程序实例句柄*/,NULL
);
//设置控件的字体
SendMessage(hStatic,WM_SETFONT,(WPARAM)hFont,NULL);
break;
caseWM_CTLCOLORSTATIC:
hdcStatic=(HDC)wParam;
SetTextColor(hdcStatic,RGB(255,255,255));//白色
SetBkColor(hdcStatic,RGB(0x41,0x96,0x4F));//翠绿色
return(INT_PTR)hBrush;
caseWM_PAINT:
hdcWnd=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
DeleteObject(hBrush);
DeleteObject(hFont);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

运行结果：
![](http://c.biancheng.net/cpp/uploads/allimg/150814/1-150Q422243E40.png)
所以要想达到修改背景颜色的目的，除了修改static 控件的背景颜色，还要修改文本的背景颜色。
## 透明背景
实现透明背景需要设置文本背景颜色透明，同时返回没有颜色的画刷。
1) SetBkMode 函数可以用来设置文本的背景模式，它的原型为：intSetBkMode(
HDChdc,//设备环境句柄
intiBkMode//背景模式
);

iBkMode 有下面两种取值：
|取值|说明|
|OPAQUE|使用当前背景颜色来填充背景。|
|TRANSPARENT|背景透明。|
2) GetStockObject (NULL_BRUSH); 语句可以返回没有颜色的画刷。
示例代码：[复制](http://c.biancheng.net/cpp/html/2965.html#)[纯文本](http://c.biancheng.net/cpp/html/2965.html#)[新窗口](http://c.biancheng.net/cpp/html/2965.html#)
caseWM_CTLCOLORSTATIC:
hdcStatic=(HDC)wParam;
SetTextColor(hdcStatic,RGB(0x41,0x96,0x4F));//翠绿色
SetBkMode(hdcStatic,TRANSPARENT);//透明背景
return(INT_PTR)GetStockObject(NULL_BRUSH);//无颜色画刷

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150815/1-150Q5104504341.png)

