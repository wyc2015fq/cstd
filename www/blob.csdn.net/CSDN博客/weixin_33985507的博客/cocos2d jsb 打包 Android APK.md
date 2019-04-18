# cocos2d jsb 打包 Android APK - weixin_33985507的博客 - CSDN博客
2014年06月05日 21:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

## 1.首先要会普通的cpp 打包成Android APK
下面所说的是在cocos2d-x 2.2.2 或者 2.3 版本号中。本文在Eclipse总用ndk编译cocos2d-x。
![](https://img-blog.csdn.net/20140510103034953)
老生常谈cocos2d-x JSB不是简单的js代码，涉及到C++代码，假设是Android的话又涉及到Java代码，有点复杂，假设搞过Android下的Jni的话会熟悉些。能够看下这篇文章：[《Android Jni 样例 Hello JNI，ndk》](http://www.waitingfy.com/archives/146)
Android为了提高开发人员开发应用的速度，减少难度，选择了Java，可是也能够用c或者c++，在某个版本号引入了ndk。cocos2d-x是用C++开发的，所以刚好也能够在Android中使用，可是就涉及到ndk了。事实上假设有Linux环境，Android完整源代码，ndk编译还是比較easy的。可是大部分cocos2d-x都用windows开发，而在windows下搞ndk就比較复杂。
## 2.首先是一些软件准备
安装JDK，弄好环境变量，略。
去www.android.com下载ADT
google搜索ndk下载最新ndk，环境变量设置
去www.cocos2d-x.org 下载2.3版本号的cocos2d-x，解压。
google搜索python 27 ，环境变量设置  (老的cocos2d-x版本号不是用python 来创建项目的，vs和xcode都有模版，刚開始比較好用，可是跨平台比較麻烦，后来改用python，仅仅须要一次创建，跨平台比較好用)
把C:\Python27;增加到PATH中。
这里还是扯下环境变量的作用，非常多人不熟悉环境变量，来看下Python这个样例，如果我是安装在C盘，打开C:\Python27, 是不是看到一个exe叫做python.exe，它能够解析python代码，可是每次都要弄完整路径来解析会非常麻烦的，所以就把C:\Python27增加到PATH中，这样不管在哪个文件夹下都能够直接訪问到phthon.exe了。
## 3.首先建立一个javascript的cocos2d-x项目
进入cocos2d-x/tools/project-creator 文件夹，win7的，能够按住Shift，右击鼠标"此处打开命令窗体"，输入python create_project.py -project TestJs -package com.test.example -language javascript
基本能够。大家能够打开create_project.py查看下源代码，里面就有创建的样例的。也能够把以下的代码保存到txt中，后缀改成bat，双击执行下，但本质还是执行create_project.py。
```
@echo off    
    set /p projectName=请输入项目名称(比如：test):    
    if "%projectName%"=="" goto inputError    
    set /p packageName=请输入项目包名（比如：org.cocos2dx.test）:    
    if "%packageName%"=="" goto inputError    
    set /p projectLanguage=请输入编程语言（比如：选填cpp ,lua ,javascript):   
    if "%projectLanguage%"=="" goto inputError   
    python create_project.py -project %projectName% -package %packageName% -language %projectLanguage% 
    pause     
       
    :inputError    
    @echo 项目名称或者包标示名不能为空！    
    pause
```
![](https://img-blog.csdn.net/20140510105525187)
复制JS代码到assets文件夹。这里要扯下，Android开发一般把图片，xml等资源放在res文件夹下的，等打包成APK的时候，会打包成一个文件。Java代码中也非常easy訪问的。可是除了标准的资源，像js,音乐等仅仅能放在assets下了，打包成APK也不会是一个文件，解压APK，assets里面的资源还是原来的，不会做不论什么处理。这些jsb开头的js好像要下载Cocos2d-JS才有。
![](https://img-blog.csdn.net/20140511085515437)
注意到这里有一个文件叫cocos2d-jsb.js ，这个是启动js，要跟Classes/AppDelegate.cpp中的相应起来，applicationDidFinishLaunching中有个调用 ScriptingCore::getInstance()->runScript("cocos2d-jsb.js");
## 3.ADT中导入两个项目
![](https://img-blog.csdn.net/20140510110607671)
在左边，右击Import 
![](https://img-blog.csdn.net/20140510110717218)
位置是以下两个地方：
cocos2d-x-2.3\cocos2dx\platform\android\java
cocos2d-x-2.3\projects\TestJs\proj.android
## 4.设置Build 属性
打开菜单Window->Preferences->Android->NDK 编辑NDK路径到你NDK的路径，注意我这里改动了默认的ndk名字。通常是比較长。
![](https://img-blog.csdn.net/20140510111138968)
右击TestJs项目，选择Properties->Builders->New->Program,
        在Main标签下的Location，选择Browse File Systerm...，选择到android NDK以下的ndk-build.cmd文件 。
                                 Working Directory，选择Browse Workspace...，选择本项目。
![](https://img-blog.csdn.net/20140510111644156)
在Environment标签下，点击new，
Name：NDK_MODULE_PATH
Value：
C:\D4\cocos2d-x-2.3;C:\D4\cocos2d-x-2.3\cocos2dx\platform\third_party\android\prebuilt(注意自己的实际文件夹，这里定义了两个值，一个是cocos2d引擎的根文件夹，另外一个是....prebuilt)
![](https://img-blog.csdn.net/20140510112050468)
选择Ok确认，返回Builders界面，去掉CDT Builder前面的勾选，并将你自己刚刚创建的Builder移动到最上面，所有配置好之后OK确认。
连接手机，执行应该就能够了。APK会生成在Bin文件夹下，所有的C++文件会打包成一个libcocos2djs.so放在libs/armeabi文件夹下。
## 5.常见错误
1."bash" is not found in PATH，这个错误我也不知道怎么就攻克了，用Eclipse编译有时候就是会抽风，多Project->Clean几次，或者删除项目又一次Import试试，再或者仅仅能下载cygwin了，把cygwin的bin文件夹增加到系统环境变量中。
2.**`Syntax and Semantic Errors`**，这个错误是点击了jni文件夹下的main.cpp产生的，能够在Eclipse中对这个错误进行关闭：`右击你的项目 Properties-> C/C++ General -> Code Analysis -> Syntax and Semantic Errors`，去掉勾。
3.
unable to process assets while packaging xxx，这个错误是由于assets的权限不够，像我在Win7的C盘中建立项目，就会有这个问题，在proj.android文件夹中，按shift右击打开**命令窗体，给assets较高权限就可以: chmod 777 -R assets/**
4.还有些错误，记得右击项目属性，打开Builder删除出错的Builder.
![](https://img-blog.csdn.net/20140511091502640)
5。还有个错误关于模拟器不支持OpenglES2.0的，能够用BlueStacks软件，它比ADT中的模拟器强大非常多。当然不妨用真机。
## 6.TestJs APK下载：
[http://www.waitingfy.com/archives/1126/testjs](http://www.waitingfy.com/archives/1126/testjs)
參考：
## [cocos2d jsb  Android](http://www.waitingfy.com/archives/1126)
