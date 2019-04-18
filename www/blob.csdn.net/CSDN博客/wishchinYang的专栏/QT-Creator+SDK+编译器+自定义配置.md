# QT-Creator+SDK+编译器+自定义配置 - wishchinYang的专栏 - CSDN博客
2016年05月19日 11:55:59[wishchin](https://me.csdn.net/wishchin)阅读数：3422
QT4.8的软件曾经耗费巨大的功夫进行构建，不舍得扔掉！重新安装Qt4.8版本
**1.安装qt-creator**
           安装qt-creator-win-opensource-2.4.0.exe版本，不建议使用最新版本4.0
**2.安装QtSDK**
         # 安装qt-creator-opensource-windows-x86-4.0.0.exe  版本，为使用PCL库
         安装qt-opensource-windows-x86-vs2010-4.8.6.exe  版本，为使用PCL库
**3.关联—配置构建工具**
         creator 暂时还不能构建4.8版本的工程，需要进行编译器的关联。在 工具-选项 里面找到 构建和运行：
                    Qt版本选择安装目录下的qmake，目录为 c:\qt\qt4.8.6x86vs10\bin\qmake.exe ，**不要选错位置**
                    工具链 可以自动检测
**4. 构建Qt工程**
......./snip.................................
常见错误：
        对于未安装VS2010的用户，可能需要安装 VC++ 2008 Redistributable -X86安装包，需要在 微软官网下载。
原因：
       无法打开包括文件:“QMainWindow”   是因为QT5 在此刻暂时不能编译QT4.8的工程；
       无法编译QWT6.0，也是次原因。
