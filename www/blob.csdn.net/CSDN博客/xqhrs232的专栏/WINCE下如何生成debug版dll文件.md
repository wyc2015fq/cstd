# WINCE下如何生成debug版dll文件 - xqhrs232的专栏 - CSDN博客
2013年02月01日 17:37:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1063
原文地址::[http://hi.baidu.com/donghaozheng/item/894881125d29a94be75e06f0](http://hi.baidu.com/donghaozheng/item/894881125d29a94be75e06f0)
相关网帖
1、[WinCE中的Debug Zone调试(转载)](http://blog.csdn.net/xqhrs232/article/details/5194121)----[http://blog.csdn.net/xqhrs232/article/details/5194121](http://blog.csdn.net/xqhrs232/article/details/5194121)
wince系统分有三种编译形式：retail、debug、ship
在实际应用中，因为debug 模式生成的镜像过于庞大，而ship模式没有log输出，一般用于最终发布，所以，多为 retail 模式。
三种模式是靠一个sysgen变量 wincedebug来决定的。运行BaseOSEnv.bat，输入“set wincedebug”可看到当前工程的编译模式。
retail 模式下，源码中RETAILMSG是可以输出log的，但DEBUGMSG的信息不会输出。Debug 模式下，RETAILMSG及DEBUGMSG的信息都会输出。这是两者的主要区别。
我们常会碰到需要更多log信息的场合（特别是在驱动不正常时），所以将指定模块编译成debug模式而整个工程保持retail模式，便成了一种常见的需求。实现方法如下：
运行BaseOSEnv.bat（起环境），通过“set wincedebug=debug”命令设置编译模式，然后转到驱动所在目录，“build -c”，编译很可能不成功，因为在debug模式下，连接库的目录都发生了改变（路径里含retail的都变成了debug）。解决方法：根据屏幕上提示的信息，在相应路径位置建立debug文件夹，然后从retail中拷贝一份需要的文件到debug目录里，然后再“build
 -c”，如此反复，直到编译成功为止。
编译成功，即dll文件生成成功，一般系统会自动复制一份到release\WPC_DracoBSP_Retail\wpc目录下。如果系统之前buildAll过，那现在可以直接“makeimg”，生成的镜像中便会包含debug版的你所需的dll文件。如果你的系统允许通过activesync覆盖windows目录下的dll文件，那就更简单了。直接将新生成的dll文件，覆盖掉原来的，重启设备即可。
//===============================
备注：：
1、按上面的方法编译获得DEBUG版本的LIB/DLL后，把DEBUG版本的LIB/DLL代替RETAIL版本的LIB/DLL，然后选中那个模块，单单执行SYSGEN就可以很快把DEBUG版本的LIB/DLL打进NK。BIN了。记得把wincedebug设置后需要修改回去，要不然影响后面的编译！好像不需要设置回去，重新打开DOS窗口，这个环境变量又是原值了。
2、一般定位到对应的路径下，set
 wincedebug=debug，然后编译----build/build -c，这样产生的LIB/DLL就是DEBUG版本的，然后把这个DEBUG版换掉对应的RELEASE版本（找对好路径与位置），再在VS2005的左边视图中选中它执行SYSGEN命令就可以了！！！
3、一般WINCE下自带的LIB/DLL有一个Debug
 Zones，如下所示，一般你要使能那一个就设置那一位为1
//
 Debug Zones.
#ifdef DEBUG
DBGPARAM dpCurSettings = {
    TEXT("Unimodem"), {
        TEXT("Init"),   TEXT("Temp"),       TEXT("Async"),      TEXT(""),
        TEXT(""),       TEXT(""),           TEXT(""),           TEXT("Dial"),
        TEXT("Thread"), TEXT("Lists"),      TEXT("Call State"), TEXT("Misc"),
        TEXT("Alloc"),  TEXT("Function"),   TEXT("Warning"),    TEXT("Error") },
    0  //2013--2--2  xqh  你想调试输出那一个调试消息，对应位置1就可以，0xffff就是把16个调试消息全部使能输出
}; 
#endif
