# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之63---BREW 应用的开发流程 - 我相信...... - CSDN博客





2010年07月18日 12:04:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1198标签：[brew																[wizard																[编译器																[application																[测试																[手机](https://so.csdn.net/so/search/s.do?q=手机&t=blog)
个人分类：[消失的技术](https://blog.csdn.net/wireless_com/article/category/597607)





为了开发的方便，一个基于BREW的移动增值业务一般要先开发它的模拟器版本，在模拟器上调测之后，再通过交叉编译器将代码编译成在目标手机上执行的目标代码，并完成在手机上的测试。下面以Visual C++ 6.0的集成开发环境为例，基于Visual Studio 2003或者以上的IDE版本的开发流程与之类似。

1启动 Visual C++ 6.0

2使用 BREW Application Wizard 来创建你的工程, 创建.c，.bid， .mif等文件

3在.c文件中包含.bid文件

4利用 Visual C++将你的文件编译成一个.dll文件

5启动BREW模拟器，设置其应用目录的位置

6在模拟器上即可看见您刚才设置的应用的名字和图标

7利用SDK和模拟器，在PC机上开发和测试

8利用交叉编译器生成.mod文件

9获取测试数字签名

10按一定的目录形式，将文件上传到手机上](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=wizard&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




