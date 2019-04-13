
# Windows edit控件（编辑框控件） -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:40:47[seven-soft](https://me.csdn.net/softn)阅读数：5475


在C语言控制台程序（Console Application）中，可以使用 scanf 函数获取用户的输入，而在Windows程序中使用编辑框控件。
## 创建编辑框控件
编辑框控件的窗口类名是`edit`。除了子窗口常用的风格 WS_CHILD、WS_VISIBLE、WS_BORDER
 外，edit 控件也有自己的风格，都是以`ES`开头。
edit 控件常用风格|风格|说明|
|ES_AUTOHSCROLL|当输入文字超出横向显示范围时自动滚动（一般必选）。|
|ES_AUTOVSCROLL|当输入文字超出纵向显示范围时自动滚动。|
|ES_CENTER|文本居中对齐。|
|ES_LEFT|文本左对齐（默认）。|
|ES_RIGHT|文本右对齐。|
|ES_MULTILINE|是否允许多行输入。|
|ES_PASSWORD|是否为密码输入框，如果指明该风格则输入的文字显示为“***”。|
|ES_READONLY|是否为只读。禁止用户输入或修改编辑控件中的文本。|
|ES_UPPERCASE|显示大写字符。|
|ES_LOWERCASE|显示小写字符。|
|ES_LOWERCASE|将用户输入到编辑控件的字符全部转换为小写。|
|ES_UPPERCASE|将用户输入到编辑控件的字符全部转换为大写。|
|ES_MULTILINE|指明了一个多行编辑控件（缺省的是单行的）。
|1) 如果指定了ES_AUTOVSCROLL风格，编辑控件将显示尽可能多的文本，并且当用户按下ENTER键时会自动地垂直滚动文本。
|2) 如果没有指定ES_AUTOVSCROLL风格，则编辑控件将显示尽可能多的行，如果在按下ENTER键却没有更多的行要显示的话，就发出蜂鸣声。
|3) 如果指定了ES_AUTOHSCROLL风格，当光标到达控件的右边时，多行编辑控件会自动地水平滚动文本。如果要开始一个新行，用户必须按下ENTER键。
|4) 如果没有指定ES_AUTOHSCROLL风格，控件会在有必要时自动将单词折合到下一行的开始。如果按下ENTER键，则另起一行。折回单词的位置是由窗口的大小决定的。如果窗口的大小发生改变，折回单词的位置也会反生改变，将会重新显示文本。|
【示例】C语言中文本登录框演示：LRESULTCALLBACKWndProc(HWNDhWnd,UINTmessage,WPARAMwParam,LPARAMlParam){
PAINTSTRUCTps;
HDChdc;
staticHFONThFont;//逻辑字体
staticHWNDhLabUsername;//静态文本框--用户名
staticHWNDhLabPassword;//静态文本框--密码
staticHWNDhEditUsername;//单行文本输入框
staticHWNDhEditPassword;//密码输入框
staticHWNDhBtnLogin;//登录按钮
switch(message){
caseWM_CREATE:
//创建逻辑字体
hFont=CreateFont(-14/*高*/,-7/*宽*/,0,0,400/*一般这个值设为400*/,
FALSE/*斜体?*/,FALSE/*下划线?*/,FALSE/*删除线?*/,DEFAULT_CHARSET,
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
FF_DONTCARE,TEXT("微软雅黑")
);
//创建静态文本框控件--用户名
hLabUsername=CreateWindow(TEXT("static"),TEXT("用户名："),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE/*垂直居中*/|SS_RIGHT/*水平居右*/,
0/*x坐标*/,20/*y坐标*/,70/*宽度*/,26/*高度*/,
hWnd/*父窗口句柄*/,(HMENU)1/*控件ID*/,hInst/*当前程序实例句柄*/,NULL
);
//创建静态文本框控件--密码
hLabPassword=CreateWindow(TEXT("static"),TEXT("密码："),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE/*垂直居中*/|SS_RIGHT/*水平居右*/,
0,56,70,26,
hWnd,(HMENU)2,hInst,NULL
);
//创建单行文本框控件
hEditUsername=CreateWindow(TEXT("edit"),TEXT(""),
WS_CHILD|WS_VISIBLE|WS_BORDER/*边框*/|ES_AUTOHSCROLL/*水平滚动*/,
80,20,200,26,
hWnd,(HMENU)3,hInst,NULL
);
//创建密码输入框
hEditPassword=CreateWindow(TEXT("edit"),TEXT(""),
WS_CHILD|WS_VISIBLE|WS_BORDER|ES_PASSWORD/*密码*/|ES_AUTOHSCROLL/*水平滚动*/,
80,56,200,26,
hWnd,(HMENU)4,hInst,NULL
);
//创建按钮控件
hBtnLogin=CreateWindow(TEXT("button"),TEXT("登录"),
WS_CHILD|WS_VISIBLE|WS_BORDER|BS_FLAT/*扁平样式*/,
80,92,200,30,
hWnd,(HMENU)5,hInst,NULL
);
//依次设置控件的字体
SendMessage(hLabUsername,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(hLabPassword,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(hEditUsername,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(hEditPassword,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(hBtnLogin,WM_SETFONT,(WPARAM)hFont,NULL);
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
DeleteObject(hFont);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150806/1-150P61I003201.png)
## 获取编辑框控件文本
在《[Windows
 static控件](http://c.biancheng.net/cpp/html/2958.html)》一节中，我们介绍了 getWindowText 函数，它可以用来获取控件的文本。
下面的例子演示用户点击登录按钮时，如何获取用户的输入内容：LRESULTCALLBACKWndProc(HWNDhWnd,UINTmessage,WPARAMwParam,LPARAMlParam){
intwmId,wmEvent;
PAINTSTRUCTps;
HDChdc;
staticHFONThFont;//逻辑字体
staticHWNDhLabUsername;//静态文本框--用户名
staticHWNDhLabPassword;//静态文本框--密码
staticHWNDhEditUsername;//单行文本输入框
staticHWNDhEditPassword;//密码输入框
staticHWNDhBtnLogin;//登录按钮
//定义缓冲区
TCHARszUsername[100];
TCHARszPassword[100];
TCHARszUserInfo[200];
switch(message){
caseWM_CREATE:
//创建逻辑字体
hFont=CreateFont(-14/*高*/,-7/*宽*/,0,0,400/*一般这个值设为400*/,
FALSE/*斜体?*/,FALSE/*下划线?*/,FALSE/*删除线?*/,DEFAULT_CHARSET,
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
FF_DONTCARE,TEXT("微软雅黑")
);
//创建静态文本框控件--用户名
hLabUsername=CreateWindow(TEXT("static"),TEXT("用户名："),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE/*垂直居中*/|SS_RIGHT/*水平居右*/,
0/*x坐标*/,20/*y坐标*/,70/*宽度*/,26/*高度*/,
hWnd/*父窗口句柄*/,(HMENU)1/*控件ID*/,hInst/*当前程序实例句柄*/,NULL
);
//创建静态文本框控件--密码
hLabPassword=CreateWindow(TEXT("static"),TEXT("密码："),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE/*垂直居中*/|SS_RIGHT/*水平居右*/,
0,56,70,26,
hWnd,(HMENU)2,hInst,NULL
);
//创建单行文本框控件
hEditUsername=CreateWindow(TEXT("edit"),TEXT(""),
WS_CHILD|WS_VISIBLE|WS_BORDER/*边框*/|ES_AUTOHSCROLL/*水平滚动*/,
80,20,200,26,
hWnd,(HMENU)3,hInst,NULL
);
//创建密码输入框
hEditPassword=CreateWindow(TEXT("edit"),TEXT(""),
WS_CHILD|WS_VISIBLE|WS_BORDER|ES_PASSWORD/*密码*/|ES_AUTOHSCROLL/*水平滚动*/,
80,56,200,26,
hWnd,(HMENU)4,hInst,NULL
);
//创建按钮控件
hBtnLogin=CreateWindow(TEXT("button"),TEXT("登录"),
WS_CHILD|WS_VISIBLE|WS_BORDER|BS_FLAT/*扁平样式*/,
80,92,200,30,
hWnd,(HMENU)5,hInst,NULL
);
//依次设置控件的字体
SendMessage(hLabUsername,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(hLabPassword,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(hEditUsername,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(hEditPassword,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(hBtnLogin,WM_SETFONT,(WPARAM)hFont,NULL);
break;
caseWM_COMMAND:
wmId=LOWORD(wParam);
wmEvent=HIWORD(wParam);
switch(wmId){
case5://按钮的HMENU参数
//获取输入框的数据
GetWindowText(hEditUsername,szUsername,100);
GetWindowText(hEditPassword,szPassword,100);
wsprintf(szUserInfo,TEXT("C语言中文网提示：\r\n您的用户账号：%s\r\n您的用户密码：%s"),szUsername,szPassword);
MessageBox(hWnd,szUserInfo,TEXT("信息提示"),MB_ICONINFORMATION);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
DeleteObject(hFont);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

程序运行后会出现一个窗口（与上面相同），输入用户名和密码，点击“登录”按钮，弹出提示框，如下图所示：
![](http://c.biancheng.net/cpp/uploads/allimg/150807/1-150PG03Jb58.png)
细心的读者会发现，static 控件带有灰色背景，影响美观，我们将在下节《[修改static控件背景颜色和文字颜色](http://c.biancheng.net/cpp/html/2965.html)》中讲解如何修改背景颜色和文字颜色。

