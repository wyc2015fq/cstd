# Xcode简明教程（使用Xcode编写C语言程序） - xqhrs232的专栏 - CSDN博客
2018年12月05日 10:05:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：419
原文地址::[http://c.biancheng.net/view/476.html](http://c.biancheng.net/view/476.html)
相关文章
1、IOS开发：Xcode入门开发第一个HelloWorld程序----[https://jingyan.baidu.com/article/a17d5285c9afc48099c8f279.html](https://jingyan.baidu.com/article/a17d5285c9afc48099c8f279.html)
2、30分钟学会iOS 11开发环境xcode 9图文教程----[https://blog.csdn.net/epubit17/article/details/79476105](https://blog.csdn.net/epubit17/article/details/79476105)
在 Mac OS X 下学习[C语言](http://c.biancheng.net/c/)使用 Xcode。Xcode 是由Apple官方开发的IDE，支持C、[C++](http://c.biancheng.net/cplus/)、Objective-C、Swift等，可以用来开发 Mac OS X 和 iOS 上的应用程序。Xcode最初使用[GCC](http://c.biancheng.net/gcc/)作为编译器，后来由于GCC的不配合，改用LLVM/Clang。
Xcode 的安装非常简单，在 APP Store 上直接下载即可，这里不再赘述。
## 在Xcode上运行C语言程序
在 Xcode 上运行C语言程序需要先创建工程，再在工程中添加源代码。
1) 打开 Xcode，选择“Create a new Xcode project”创建一个新工程，如下图所示：
![](http://c.biancheng.net/uploads/allimg/180912/1HS92553-0.jpg)
2) 接下来，选择要创建的工程类型，如下图所示：
![](http://c.biancheng.net/uploads/allimg/180912/1HS96493-1.jpg)
3) 选择“OS X --> Application --> Command Line Tool”，点击“Next”。Command Line Tool 是“命令行工具”的意思，也就是控制台程序。
![](http://c.biancheng.net/uploads/allimg/180912/1HS9D09-2.jpg)
这里需要填写和工程相关的一些信息：
- Product Name：产品名称，即工程名称。
- Organization Name：组织名称，即公司、个人、协会、团队等的名称。
- Organization Identifier：组织标识符，即有别于其他组织的一个标记，例如身份证号、公司网址、组织机构代码证等。
- Bundle Identifier：程序标识符，即有别于其他程序的一个标记，由 Organization Identifier + Product Name 组成。
- Language：工程所用的编程语言，这里选择C语言。
4) 点击“Next”，保存文件后即可进入当前工程，如下图所示：
![](http://c.biancheng.net/uploads/allimg/180912/1HS91O3-3.jpg)
左侧是工程目录，主要包含了工程所用到的文件和资源。单击“main.c”，即可进入代码编辑模式，这里 Xcode 已经为我们创建好了一个“Hello World”小程序。点击上方的“运行”按钮，即可在右下角的选项卡中看到输出结果。
