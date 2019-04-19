# （Qt Installer Framework）程序简易打包教程 - baidu_33570760的博客 - CSDN博客
2017年06月19日 14:38:47[carman_风](https://me.csdn.net/baidu_33570760)阅读数：4561
## （Qt Installer Framework）程序简易打包教程
- - [Qt Installer Framework程序简易打包教程](#qt-installer-framework程序简易打包教程)- [第一步下载Qt Installer Framework](#第一步下载qt-installer-framework)
- [第二步 打包程序安装和环境变量的配置](#第二步-打包程序安装和环境变量的配置)
- [第三步准好要打包的程序](#第三步准好要打包的程序)
- [第四步以DQtQtIFW203examplesstartmenu这个例子为参照修改自己要打包的程序](#第四步以dqtqtifw203examplesstartmenu这个例子为参照修改自己要打包的程序)
### 第一步：下载Qt Installer Framework
- 
网址: [http://download.qt.io/official_releases/qt-installer-framework/](http://download.qt.io/official_releases/qt-installer-framework/)
![](http://i.imgur.com/sFFL8Lq.png)
- 
选择2.0.3文件夹,跳转页面选择QtInstallerFramework-win-x86版本,还可可根据目标操作系统，分别选择win,mac,linux版本的打包程序
![](http://i.imgur.com/iDYPVtu.png)
### 第二步 打包程序安装和环境变量的配置
- 
安装QtInstallerFramework-win-x86.exe到指定目录, 本教程安装到D:\Qt
![](http://i.imgur.com/I2l1WQy.png)
- 
配置环境变量路径 右键计算机—>属性—–>高级选项卡—->环境变量—->系统变量,添加变量Path的值:D:\Qt\QtIFW2.0.3\bin
![](http://i.imgur.com/yDBI5Q0.png)
![](http://i.imgur.com/QyBIr5u.png)
- 
添加Qt installer framework Manual帮助文档到qt creator. 单击qt creator菜单工具—>选项—>左侧选中帮助—>右侧选文档选项卡,单击添加按钮，选中已安装的文档路径.选中ifw.qch文件，然后单击apply，最后单击ok确认。
![](http://i.imgur.com/LYkwopP.png)
添加后,就可在帮助索引目录找到Qt Installer Framework Manual
![](http://i.imgur.com/80j8Tsj.png)
### 第三步:准好要打包的程序
- 
注意，qt编译生成release版本程序，默认的exe目录不带一些依赖动态库，笨拙的方法是用Depends.Exe工具查看exe所依赖的库，都拷贝到exe的所在目录，这样就准备好了程序安装的必要文件。更为聪明的做法是用qt自带的工具windeployqt.exe,该执行程序的路径在D:\Qt\Qt5.5.1_msvc2010\5.5\msvc2010\bin下。路径根据qt的安装目录来确定。打开qt命令行窗口,开始菜单里面找,Qt 5.5 32-bit for Desktop…, 命令格式: windeployqt.exe D\exmple.exe(完整文件路径)
![](http://i.imgur.com/QpHV5bm.png)
![](http://i.imgur.com/hSsaRA6.png)
工具运行前的目录如下： 
![](http://i.imgur.com/0ilCVnN.png)
工具运行后目录如下图:
![](http://i.imgur.com/y8jFtnB.png)
发现运行后程序目录已经自动补齐了所有依赖库文件
### 第四步:以D:\Qt\QtIFW2.0.3\examples\startmenu这个例子为参照，修改自己要打包的程序
- 
将D:\Qt\QtIFW2.0.3\examples\startmenu文件夹拷贝到桌面,用编辑器打开C:\Users\pc\Desktop\startmenu\config\config.xml文件,修改如下（箭头指示部分）
![](http://i.imgur.com/gHdXK2C.png)
- 
用编辑器打开C:\Users\pc\Desktop\startmenu\packages\org.qtproject.ifw.example\meta\installscript.qs
![](http://i.imgur.com/ULgz0ID.png)
- 
用编辑器打开C:\Users\pc\Desktop\startmenu\packages\org.qtproject.ifw.example\meta\package.xml
![](http://i.imgur.com/e9FDhL4.png)
- 
将要打包的程序文件拷贝至C:\Users\pc\Desktop\startmenu\packages\org.qtproject.ifw.example\data
![](http://i.imgur.com/yGCCIjh.png)
- 
打包程序,打开qt命令窗口，转到C:\Users\pc\Desktop\startmenu目录， 然后敲如下命令生成名为DuPaintInstaller.exe的安装包. 命令格式在C:\Users\pc\Desktop\startmenu\README文件中有说明
![](http://i.imgur.com/CncJ0yM.png)
打包成功如下图,DuPaintInstaller就是安装包:
![](http://i.imgur.com/B87agLH.png)
