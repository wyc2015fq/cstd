# Qt5 编译 & 打包依赖dll发布 - DoubleLi - 博客园






十年前学C++的时候，无聊到把windows 文件夹下几乎所有的*.dll 都看过一遍。偶尔在程序运行时看到缺少 *.dll 的提示，都会直接找出来解决。



随着“开发平台”和“编译器”版本的逐年升级，一个程序的依赖链接库体积也越来越大，这给码农、程序猿、软件攻城狮们测试和发布都造成了比较大的麻烦！

六年前做过某专业软件汉化包的安装程序打包，感觉专业的安装程序制作工具对一些小程序来说太浪费了，而且用户会觉得不够“绿色”。如果能够简单的完成打包，并满足基本要求，那是比较好的解决方案。

前段时间有不少人都问到我“用什么软件做上位机GUI比较好？”，最近看了两天Qt，就拿这个说事吧。

Qt，是一个1991年由奇趣科技开发的跨平台C++图形用户界面应用程序开发框架。和Java的“一次编译，到处运行”所不同的是，Qt是源代码级的跨平台，具有“一次编写，到处编译”的特性。Google Earth、Skype、Opera……很多软件都是基于Qt编写的。

目前最新版本是 5.1.1，由于前段时间玩linux 的编译上瘾了，也懒得去装MS Virual Studio2010/2012，于是下载了qt-windows-opensource-5.1.1-mingw48_opengl-x86-offline.exe（集成mingw4.8、opengl）。

**一、    ****软件环境安装与设置**

5.1.1 版本在windows平台的移植和集成进一步完善了，不需要像网上那些针对旧版本的教程中说的需要繁琐的设置。只需要像典型的windows安装程序一样鼠标点击即可完成安装（当然在安装中要注意的是：mingw 默认并未选中）。

这个是能够持续看上两天Qt的主要原因！！！

**二、    ****项目设置与程序编译**

这个看看教程就行了，只是5.1.1 的教程暂时还没有出现！

随便找了个经典的“Hello world!”来跑了一下，有惊无险的“构建成功”。

PS：和所有英文软件一样，项目路径里不要有中文、日文、韩文、火星文等等说英语的家伙不认识的符号！

又PS：不要认为别人编译通过的程序就一定能编译成功，编译环境和软件升级中的更新都可能导致失败！

**三、    ****程序发布**

程序编译的时候默认是“debug”的，其实和“release”生成的exe 都差不多，只是依赖链接库的大小不一样。对于普通用户，其实对程序大小是不敏感的，更多的人会关心华丽的人机界面或者繁杂的功能。只有要求苛刻的嵌入式系统用户和有强迫症的码农、程序猿、软件攻城狮们会追求极限的压缩体积！

**四、    ****程序打包**

要求苛刻的嵌入式系统用户有时候宁愿舍弃新的功能，都会要求稳定可靠的应用，而版本升级有时候并没有软件开发商的广告说的那么美好。

eg：随便下了个“串口调试助手”来做Model。

Qt4编译这个“串口调试助手”，生成的依赖链接库只有4个，文件大小都还能够接受；

![](http://www.51hei.com/UploadFiles/2014-04/huqin/psb(67).png)

换成Qt5 来编译，会发现一下多了很多东西出来！



![](http://www.51hei.com/UploadFiles/2014-04/huqin/psb(68).png)
即使是用压缩工具压缩打包以后（ ![](http://www.51hei.com/UploadFiles/2014-04/huqin/psb(69).png)
）的体积，仍然比未压缩的旧版本要大！



![](http://www.51hei.com/UploadFiles/2014-04/huqin/psb(70).png)
一个小工具（一个mingw编译的Qt gui程序），依赖的DLL实在是太多，着实有点不太方便。而在社区里更多人提到的都是“静态编译”，但是使用“静态编译”又会受到开源许可的一些限制。

现在该是我们的小伙伴“Enigma Virtual Box”表现的时候了！！！

**五、    ****打包依赖链接库**

EnigmaVirtual Box 最新版本是 6.80 Build 20131003，启动后如下图所示：



![](http://www.51hei.com/UploadFiles/2014-04/huqin/psb(71).png)
在“Enter Input File Name”一栏指定Qt编译生成的 *.exe 文件，同时在“EnterOutput File Name”一栏会自动生成打包后输出文件的路径和文件名称。（请无视中间的广告！）

点击左下方的“Add…”按钮，添加所有的依赖链接库文件；然后点击右下方的“FilesOptions”，在弹出的对话框里选中“Enable Files Virtualization”和“CompressFiles”两项；“OK”后返回点击“Process”并等待小伙伴干完该干的活。



![](http://www.51hei.com/UploadFiles/2014-04/huqin/psb(72).png)
小伙伴生成的*.exe 文件（18.247KB ）的尺寸要小了很多，并且这是一个纯粹的“绿色软件”，直接复制就能在任何存储介质上运行了！有了这个，妈妈再也不用担心系统会突然弹个框框出来，提示缺少××.dll 文件了！

**六、    ****Qt5 ****主要依赖链接库说明**


1.QT模块库



|1234|Qt5Core.dll      #QT核心库Qt5Gui.dll       #QT Gui库Qt5Widgets.dll   #QT Widgets库，QT 5中GUI程序基本都需要此dll#还有其他程序用到的Qt5XXX.dll|
|----|----|




2.ICU依赖库

（International Component for Unicode，Unicode工具）



|123|icudt51.dll     icuin51.dll    icuuc51.dll|
|----|----|




3.QT插件库（新增库，路径必须正确）



|123456|# 根据不同的程序，需要不同的插件库#例如 QT_DIR/plugins/*/*.dll# 需要将 platforms/*.dll,accessible/*.dll的文件结构保留# 放在你所编译的程序所在目录plugins/platforms/qwindows.dllplugins/accessible/qtaccessiblewidgets.dll|
|----|----|




4.EGL依赖库，为OpenGL,OpenGL es提供接口



|12|libEGL.dlllibGLESv2.dll|
|----|----|




5.mingw依赖库(msvc编译则无需这些库)



|123|libgcc_s_dw2-1.dlllibstdc++-6.dlllibwinpthread-1.dll|
|----|----|




6.VC运行库（mingw编译则无需这些库)



|12|msvcr110.dll(对应VS2012）msvcp110.dll|
|----|----|





*最后，让新的小伙伴** Qt5 **给我打个招呼，得瑟一下！![](http://www.51hei.com/UploadFiles/2014-04/huqin/psb(73).png)*









