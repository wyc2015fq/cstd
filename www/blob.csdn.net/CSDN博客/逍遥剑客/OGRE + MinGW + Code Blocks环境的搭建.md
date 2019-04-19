# OGRE + MinGW + Code::Blocks环境的搭建 - 逍遥剑客 - CSDN博客
2007年08月14日 12:44:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：7859标签：[compiler																[windows																[编译器																[linux																[direct3d																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)
### OGRE + MinGW + Code::Blocks环境的搭建
**首先,****介绍一下这三个名词:**
[Ogre](http://www.ogre3d.org/)是一款开源的图形渲染引擎，它的全名叫（Object-oriented Graphics Rendering Engine），它是由一个核心小组开发并维护，由于它是一款开源引擎，所以它也像Linux一样，被各种不同的团体和组织改造和扩充，并使它能够适应不同的应用领域和拥有更强大的功能。
[MinGW](http://www.mingw.org/)是一个Windows下的编译器（实际上是一系列开发工具）。与Windows下其它编译器不同的是，MinGW与Linux下广泛使用的GNU（基本上）完全兼容，这意味着，在Linux下如何编译源代码，在MinGW中也可以以完全相同的方式编译。
[Code::Blocks](http://www.codeblocks.org/)是一个开放源码、跨平台的 C/C++ IDE（集成开发环境）。由于它使用 wxWidgets，所以能够支持 Linux 和 Windows 系统。Code::Blocks 使用 GNU C++ 开发，具有开发包和插件支持。
**环境安装**
- 下载并安装[OGRE 1.4.3 SDK for Code::Blocks + MinGW C++ Toolbox](http://downloads.sourceforge.net/ogre/OgreSDKSetup1.4.3_CBMingW_.exe)
- 下载并安装[Mingw C++ Toolbox](http://downloads.sourceforge.net/ogre/MinGW_Toolbox_Setup_wr1.exe?modtime=1172757829&big_mirror=1) for the SDK above 
- 下载并安装[Code::Blocks IDE, without compiler](http://prdownloads.sourceforge.net/codeblocks/codeblocks-1.0rc2.exe?download) (因为compiler要用上面第二项的) 
- 启动Code::Blocks，设定compiler(会自动检测到GCC,即上面的Mingw)。完工。
**环境配置**
先创建一个工程,由于C::B里内置了OGRE的模板,所以起个工程名就OK了
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/1.JPG)
编译,提示找不到OgreMain_d……….(注意Debug对应_d,Release正常)
这是因为工程默认的target link目录下没有OgreMain_d.dll导致的,所以,在工程属性里改一下:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/2.JPG)
当然,还有OIS_d,加在这儿:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/3.JPG)
不然又会报一堆链接错误
OK啦,现在应该能编译成功了
运行之,铛!一个对话框出来了,说是缺少d3dx9d_??.dll
这说明DirectX版本太老了,如果要_d.dll的话,要去下载DirectX SDK,有点大,无奈,先放弃DirectX渲染方式好了,用OpenGL:
打开OgreSDK/bin/Debug/下的Plugins.cfg
把Plugin=RenderSystem_Direct3D9_d这句用#注释掉
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/4.JPG)
运行,盼望已久的画面终于出来拉,哈哈
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/5.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/6.JPG)
为什么不用VisualStudio?因为那是商业软件
为什么不用VS Express?因为用那个还要下载400M的windows SDK
而且配置起来也要麻烦一些
Code::Blocks真的很好用,大家可以用来代替VisualC++,再加上GCC编译器,简直没得说!
OGRE更是现在很热门的一款开源图形引擎,应用范围很广,再加上它是面向对象的,很适合我们来学习,HOHO
