# Qt5使用qDebug()在windows控制台中输出信息----本人备注 - xqhrs232的专栏 - CSDN博客
2018年09月01日 21:08:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：116
原文地址::[https://blog.csdn.net/bzhang2012/article/details/78244787](https://blog.csdn.net/bzhang2012/article/details/78244787)
相关文章
1、Window下Qt中用qDebug()输出调试信息到Console控制台的设置方法----[https://blog.csdn.net/yangluoning/article/details/8449572](https://blog.csdn.net/yangluoning/article/details/8449572)
2、QDebug 输出调试信息----[https://blog.csdn.net/Amnes1a/article/details/64438778](https://blog.csdn.net/Amnes1a/article/details/64438778)
3、Qt 调试信息，输出日志----[https://blog.csdn.net/qq_35488967/article/details/76522360](https://blog.csdn.net/qq_35488967/article/details/76522360)
4、Qt 之 qInstallMessageHandler（重定向至文件）----[https://blog.csdn.net/liang19890820/article/details/51838379](https://blog.csdn.net/liang19890820/article/details/51838379)
1. 之前碰过这个问题，qDebug()要在windows控制台中输出信息的话，要在.pro文件中加入以下内容：
` `- 
`QT += widgets`
- 
`QMAKE_CXXFLAGS += -std=c++11 `
- 
`CONFIG += console`
2. 这个问题解决后还做了笔记，但是现在同样的办法居然不行了，编译时会提示：undefined reference to `WinMain@16'，上网搜了一下，在stackoverflow搜到一个类似的问题，但是按照这个问题的解决办法，编译出来的exe文件运行时会提示不是win32的程序，想了想，修改成以下这句编译就不会提示错误了：
`win32:QMAKE_CXXFLAGS += -std=c++11`
3. 试了一下，.pro文件还可以这样写，同样也可以：
` `- 
`QT += widgets`
- 
`CONFIG += console c++11`
![](https://img-blog.csdn.net/20171015223122588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnpoYW5nMjAxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。
不明白这些有什么区别
//=================================================================================
本人备注::
1>.pro文件加了CONFIG += console就可以，不需要 注册qInstallMessageHandler！！！
2>修改了.pro文件要重新构建整个项目修改才会生效

