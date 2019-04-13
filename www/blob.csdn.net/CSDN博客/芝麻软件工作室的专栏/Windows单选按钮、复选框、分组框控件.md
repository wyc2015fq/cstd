
# Windows单选按钮、复选框、分组框控件 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:42:18[seven-soft](https://me.csdn.net/softn)阅读数：4558个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



单选按钮（Radio Button）和复选框（Check Box）是常见的Windows控件，用于从给出的选项中选择一项或多项，如下图所示：
![](http://c.biancheng.net/cpp/uploads/allimg/150807/1-150PH21JL18.png)
单选按钮是互斥的，只能选择其中一项；而复选框没有限制，可以选择一项或多项。
单选按钮和复选框都是一种特殊的按钮，窗口类名称都是`button`，只不过增加了一些特殊的窗口样式罢了。单选按钮的样式为`BS_AUTORADIOBUTTON`，复选框的样式为`BS_AUTOCHECKBOX`。例如：//创建一个单选按钮
CreateWindow(
TEXT("button"),TEXT("单选按钮"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTORADIOBUTTON/*单选按钮*/,
235,40,100,26,
hWnd,(HMENU)7,hInst,NULL
);
//创建一个复选框
CreateWindow(
TEXT("button"),TEXT("复选框"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTOCHECKBOX/*复选框*/,
95,70,50,26,
hWnd,(HMENU)9,hInst,NULL
);

如上图所示，单选按钮一般是分组的，一组单选按钮之间是互斥的，只能选择其中一个。一个窗口可以包含多组单选按钮。
要想为单选按钮分组，可以增加`WS_GROUP`样式。设置了
 WS_GROUP 样式的单选框为一组中的首元素，随后的所有单选按钮都和它在同一组，直到下一个设置了 WS_GROUP 样式的单选按钮。
下面的代码，将实现上图中所示的单选按钮和复选框：//窗口过程
LRESULTCALLBACKWndProc(HWNDhWnd,UINTmessage,WPARAMwParam,LPARAMlParam){
PAINTSTRUCTps;
HDChdc;
staticHFONThFont;//逻辑字体
//一组单选按钮
staticHWNDlabSex;//静态文本框--性别
staticHWNDradioMale;//单选按钮--男
staticHWNDradioFemale;//单选按钮--女
//一组单选按钮
staticHWNDlabMarriage;//静态文本框--婚姻状况
staticHWNDradioMarried;//单选按钮--已婚
staticHWNDradioSingle;//单选按钮--未婚
staticHWNDradioSecrecy;//单选按钮--保密
//一组复选框
staticHWNDlabPet;//静态文本框--你的宠物
staticHWNDcheckboxDog;//复选框--狗
staticHWNDcheckboxCat;//复选框--猫
staticHWNDcheckboxFish;//复选框--鱼
staticHWNDcheckboxOther;//复选框--其他
switch(message){
caseWM_CREATE:
hFont=CreateFont(
-14,-7,0,0,400,
FALSE,FALSE,FALSE,DEFAULT_CHARSET,
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
FF_DONTCARE,TEXT("微软雅黑")
);
//选择性别
labSex=CreateWindow(
TEXT("static"),TEXT("你的性别："),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_RIGHT/*文字居右*/,
10,10,80,26,
hWnd,(HMENU)1,hInst,NULL
);
radioMale=CreateWindow(
TEXT("button"),TEXT("男"),
WS_CHILD|WS_VISIBLE|BS_LEFT/*文字居左*/|BS_AUTORADIOBUTTON/*单选按钮*/|WS_GROUP,
95,10,50,26,
hWnd,(HMENU)2,hInst,NULL
);
radioFemale=CreateWindow(
TEXT("button"),TEXT("女"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTORADIOBUTTON,
150,10,50,26,
hWnd,(HMENU)2,hInst,NULL
);
//选择婚姻状况
labMarriage=CreateWindow(
TEXT("static"),TEXT("婚姻状况："),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_RIGHT,
10,40,80,26,
hWnd,(HMENU)4,hInst,NULL
);
radioMarried=CreateWindow(
TEXT("button"),TEXT("已婚"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTORADIOBUTTON|WS_GROUP,
95,40,65,26,
hWnd,(HMENU)5,hInst,NULL
);
radioSingle=CreateWindow(
TEXT("button"),TEXT("未婚"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTORADIOBUTTON,
165,40,65,26,
hWnd,(HMENU)6,hInst,NULL
);
radioSecrecy=CreateWindow(
TEXT("button"),TEXT("保密"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTORADIOBUTTON,
235,40,100,26,
hWnd,(HMENU)7,hInst,NULL
);
//你的宠物
labPet=CreateWindow(
TEXT("static"),TEXT("你的宠物："),
WS_CHILD|WS_VISIBLE|SS_CENTERIMAGE|SS_RIGHT,
10,70,80,26,
hWnd,(HMENU)8,hInst,NULL
);
checkboxDog=CreateWindow(
TEXT("button"),TEXT("狗"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTOCHECKBOX/*复选框*/,
95,70,50,26,
hWnd,(HMENU)9,hInst,NULL
);
checkboxCat=CreateWindow(
TEXT("button"),TEXT("猫"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTOCHECKBOX,
150,70,50,26,
hWnd,(HMENU)10,hInst,NULL
);
checkboxFish=CreateWindow(
TEXT("button"),TEXT("鱼"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTOCHECKBOX,
205,70,50,26,
hWnd,(HMENU)11,hInst,NULL
);
checkboxOther=CreateWindow(
TEXT("button"),TEXT("其他"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTOCHECKBOX,
260,70,65,26,
hWnd,(HMENU)11,hInst,NULL
);

SendMessage(labSex,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(radioMale,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(radioFemale,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(labMarriage,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(radioMarried,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(radioSingle,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(radioSecrecy,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(labPet,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(checkboxDog,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(checkboxCat,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(checkboxFish,WM_SETFONT,(WPARAM)hFont,NULL);
SendMessage(checkboxOther,WM_SETFONT,(WPARAM)hFont,NULL);
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

## 分组框控件
使用分组框控件（GroupBox）可以对单选按钮、复选框进行更好的分组，如下图所示：
![](http://c.biancheng.net/cpp/uploads/allimg/150815/1-150Q5134K13N.png)
分组框控件也是一种特殊的按钮，它的样式为`BS_GROUPBOX`，例如：//创建分组框控件
hGroupSex=CreateWindow(
TEXT("button"),TEXT("你的性别"),
WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
20,20,300,70,
hWnd,(HMENU)1,hInst,NULL
);
//接下来添加单选按钮
hRadioMale=CreateWindow(
TEXT("button"),TEXT("男"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTORADIOBUTTON/*单选按钮*/,
15,30,50,26,
hGroupSex/*父窗口为 GroupBox 控件*/,(HMENU)2,hInst,NULL
);
hRadioFemale=CreateWindow(
TEXT("button"),TEXT("女"),
WS_CHILD|WS_VISIBLE|BS_LEFT|BS_AUTORADIOBUTTON,
80,30,50,26,
hGroupSex,(HMENU)3,hInst,NULL
);

该代码将会创建上图中的第一组按钮。
需要注意的是：单选按钮的父窗口为分组框控件 hGroupSex，而不是顶级窗口 hWnd。
完整的示例代码较长，就不在这里展示了，请大家下载本节完整代码。
本节代码完整版下载地址：[http://pan.baidu.com/s/1jGImzSa](http://pan.baidu.com/s/1jGImzSa)提取密码：ve1d

