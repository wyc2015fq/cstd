# 显示驱动相关 -- DrvEscape和ExtEscape - xqhrs232的专栏 - CSDN博客
2012年09月02日 09:35:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：704标签：[wince																[system																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://hi.baidu.com/even_xf/item/efe691cb7740fc1350505861](http://hi.baidu.com/even_xf/item/efe691cb7740fc1350505861)
相关网帖
1.请问一下ExtEscape函数的用法----[http://topic.csdn.net/t/20050422/16/3959047.html](http://topic.csdn.net/t/20050422/16/3959047.html)
2.[应用层调用接口修改输出显示方式 ExtEscape](http://blog.csdn.net/lin415804/article/details/6990335)----[http://blog.csdn.net/lin415804/article/details/6990335](http://blog.csdn.net/lin415804/article/details/6990335)
3.ExtEscape_关于WINCE显示通道的简单介绍----[http://wenku.baidu.com/view/8184941cff00bed5b9f31d63.html](http://wenku.baidu.com/view/8184941cff00bed5b9f31d63.html)
# 请问一下ExtEscape函数的用法
DrvEscape是提供给应用程序的一个直接访问显示驱动的接口/机制,它类似流接口的IOCTLs方式的,完成标准GDI无法完成一些功能.上层应用程序调用ExtEscape()发送查询,设置等控制命令给显示(打印)驱动.这些命令字叫做escape code.系统已经定义的escape
 code有如下:
> 
QUERYESCSUPPORT             8      查询显示驱动是否支持用户请求的escape code.
GETVFRAMEPHYSICAL           6144   得到显存的物理地址
GETVFRAMELEN                6145   得到显存的大小
DBGDRIVERSTAT               6146   返回显示驱动的状态
SETPOWERMANAGEMENT          6147   设置电源管理状态,比如背灯亮度调节等
GETPOWERMANAGEMENT          6148   查询当前电源管理状态
CONTRASTCOMMAND             6149   对比度设定
DRVESC_GETGAMMAVALUE        6202   获得伽马值.文档说使用SystemParameterInfo来取代.
DRVESC_SETGAMMAVALUE        6201   设置伽马值.使用SystemParametersInfo来驱动
DRVESC_SETSCREENROTATION    6301   设置屏幕的旋转,可以转90,180,270.
DRVESC_GETSCREENROTATION    6302   当前的旋转值.
DRVESC_SAVEVIDEOMEM         6501   当挂起时候,如果注册表HKLM\system\GWE\PORepaint等于3,将保存显存内容
DRVESC_RESTOREVIDEOMEM      6502   唤醒时候,恢复显存内容
DRVESC_QUERYVIDEOMEMUSED    6503   得到当前已经使用了多少显存.
Reserved                    6150 through 99,999   保留将来使用.
    在使用前应该先用QUERYSCSUPPORT来查询是否支持指定的escape code,如果返回1,表示支持,返回0,表示不支持. 一个简单的例子:
> 
HDC hdc;
int EscCode = 6301;
hdc = GetDC(hWnd);
if (ExtEscape(hdc, 8, sizeof(EscCode), (LPSTR)&EscCode, 0, 0)) 
{
    EscCode = 1;
    ExtEscape(hdc, 6301, sizeof(EscCode), (LPSTR)&EscCode, 0, 0);
}
ReleaseDC(hWnd, hdc);
     说明:首先查询DRVESC_SETSCREENROTATION这个命令字是否被支持.通过发送命令字QUERYESCSUPPORT来查询,如果返回1表示支持.如上面第4行.第2个参数8就是QUERYESCSUPPORT(如果要使用这个宏,需要加入相应头文件,我懒得查,直接使用了.).接下来,使用6301,即设置旋转的命令.第4个参数是旋转的角度.一般的驱动会支持0,1,2,4分别代表DMDO_0,DMDO_90,DMDO_180,DMDO_270,表示各个旋转的角度.如上面第7行.此外,上面是动态旋转.如果要使得每次开机都维持旋转后的场景,需要修改注册表HKLM\system\GDI\Rotate
