# 为VS2005工程添加新的SDK----换SDK编译 - xqhrs232的专栏 - CSDN博客
2012年07月01日 22:54:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1448
原文地址::[http://blog.csdn.net/norains/article/details/5600904](http://blog.csdn.net/norains/article/details/5600904)
图文并茂的文章！！！
//=====================================================================
//TITLE:
//    为VS2005工程添加新的SDK
//AUTHOR:
//    norains
//DATE:
//    Monday 17-May-2010
//Environment:
//    Windows CE 5.0
//    Windows CE 6.0
//=====================================================================
    在很多情况下，我们需要为旧的工程添加新的SDK。比如，你有个WinCE 5.0的工程，现在想编译为WinCE 6.0的程序，那么我们就必须为旧的工程添加新的6.0的SDK。听起来是不是很复杂？但实际上却很简单。
    我们就以实例来说明添加的流程。
    这里有一个工程，里面只有两个SDK，分别是TDS_SiRFprima_CE6_SDK和TDS_SMTP37XX，现在需要新添加一个THDP_DEMO的SDK。
    点击菜单上的"Configuration Manager.."
![](http://hi.csdn.net/attachment/201005/17/0_12740833358qw4.gif)

    在弹出来的对话框的Active Solution Platform的下拉表框中选择"<New..>"：
![](http://hi.csdn.net/attachment/201005/17/0_1274083341No55.gif)

    接着弹出"New Solution Platform"对话框如下所示：
![](http://hi.csdn.net/attachment/201005/17/0_127408335244b8.gif)

    然后在"Type or select the new platform:"中选择THDP_DEMO(MIPSII)：
![](http://hi.csdn.net/attachment/201005/17/0_1274083363xa2w.gif)

    如果有一些设置和已经添加的SDK类似，可以在"Copy Setting form:"中选择。因为我这里只是简单地添加一个SDK，不需要复制已存的SDK，所以只是选择<empty>：
![](http://hi.csdn.net/attachment/201005/17/0_1274083370qXxx.gif)

    点击"OK"后，我们就可以在下拉菜单见到我们所需要的THDP_DEMO SDK。
![](http://hi.csdn.net/attachment/201005/17/0_1274083377HLxV.gif)
    最后的事情就变得简单了，选择该SDK，进行编译。一般来说，应该不会有任何问题，都能顺利编译通过。是的，一切就这么简单！
//===============================================================
备注::
  1》如果在设置"Copy Setting form:"时不从其它已经OK的SDK包COPY过来直接选择<empty>的话，是编译不了的
D:\Program Files\Microsoft Visual Studio 8\VC\ce\include\crtdefs.h(100):fatal error c1189:#error :ERROR:Use of runtime library internal header file.
  A._CRTBLD
  B._WIN32_WCE
没有定义！！！
工程/属性/C++/Preprocessor/Preprocessor Definitions
工程/属性/C++/PreCompiled Headers/Create/Use PreCompiled Headers
上面两项都没有定义！！！
  2》拿到别人的工程要在自己的SDK下编译把xxx.sln++++xxx.vcproj文件里面的老的SDK名称换成自己的SDK名称，再打开工程就可以了！
