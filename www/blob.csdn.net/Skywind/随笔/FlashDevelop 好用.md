# Skywind Inside » FlashDevelop 好用
## FlashDevelop 好用
July 27th, 2015[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
好几年没碰过 Flash 了，最近需要给 ActionScript 导出一些 C 接口，又抽空捡起来。项目大了以后 Flash Builder 卡的要死，经常是一个构建你就可以休息了，按一下 “.“ 它就开始搜索补全提示，你的符号多了以后，有时候 Flash Builder 近乎假死了。实在难以忍受，咨询了一些正在做页游的朋友，是否还在用 Flash Builder 。得到答案是：早就投奔 Flash Develop 了。
试了一下 FlashDevelop 果然腰也不酸了，腿也不疼了，十分流畅，界面类似 Visual Studio，同时还是免费的，可以彻底和笨重的 Flash Builder 说再见了。再次感叹 AS3 写起来真爽之余，记录一下安装配置过程：
![image](http://www.skywind.me/blog/wp-content/uploads/2015/07/image_thumb8.png)
[安装设置]
安装下载 FlashDevelop 安装到比如 d:\Program Files\FlashDevelop 下面。    
管理员运行 FDMT.cmd，打开下面两项     
standalone mode (SDK和插件安装于 FlashDevelop/Apps 而不是系统用户目录）     
multi instance mode（可以启动多个 FlashDevelop IDE）     
成功后 FlashDevelop 目录会有 .local 和 .multi 两个文件，.local 以后插件会安装 Apps 目录
[安装应用]
FlashDevelop->Tools->Install Software 打开软件安装面板，选择必要软件：    
1. Flex SDK + AIR SDK     
2. Adobe AIR     
3. Flash Player (SA)
[播放器设置]
1. 下载播放器，位于 FlashDevelop/Apps/flashsa下面，    
2. FlashDevelop->Tools->Program Settings, 选择FlashViewer，填写 External Player Path 为播放器文件。
[SDK]
FlashDevelop->Tools->Program Settings 选择 AS3Context    
在 Installed Flex SDKs 中增加路径 Apps/flexairsdk/4.6.0+18.0.0
[JAVA]
如果 SDK 需要依赖 java 1.7 而系统中是 java 1.8 1.9 的话    
修改 Apps/flexairsdk/4.6.0+18.0.0/bin/jvm.config 里面的 java.home 以及 env 两项     
java.home=D:/Program Files/FlashDevelop/runtimes/java/jre-1.7.0_60 
注意路径分割使用 "/"，而不是 “\”，设置到你的 jre 1.7 对应目录去，我自己是单独下载了一个 jre 直接扔 FlashDevelop 安装目录的 runtimes 下面了，这里你需要改为你对应的 jre 路径。
视情况如果内存报错，继续设置：    
java.args=-Xmx512m -Dsun.io.useCanonCaches=false -Djava.util.Arrays.useLegacyMergeSort=true
[SWC]
flashdevelop没有直接支持生成swc的工程，但flashdevelop生成swc也比较方便，不用任何插件。    
swc库是由 flexsdk的compc.exe生成的，其实我们通过这个命令行也可以直接生成swc。     
但还是直接在flashdevelop里F8一下，编译+生成来得方便。     
我先建立一个AS3 Project，取名为logic吧。
1. 打开project Properties对话框，output那一页，把输出名字logic.swf改成logic.swc。
2. 选择build tab页    
   在Post-Build Command Line加入生成swc的执行命令，注意是Post-Build，不是Pre-Build。     
   "$(CompilerPath)\bin\compc.exe" -include-sources "$(ProjectDir)\role" "$(ProjectDir)\item" –
compiler.library-path "$(ProjectDir)\..\..\lib\protobuf.swc" -output "$(OutputFile)"    
   这个命令把role和item目录下的AS代码生成swc的。-compiler.library-path,     
   这个是指定引用其他swc的路径。-include-sources 和 -compiler.library-path 后面都可以接多个路径
3. 关于Document-Class    
   经过上面两项设置，F8，会提示须要一个Document-Class，我们要的是swc，不须要程序运行入口。我们争取能不
用Document-Class也能编译。    
   打开第一步的output那一页，把Compilation Target 从Application改成"Custom Build"。     
设置完后，F8，编译，接着就生成swc了。
我的flashdevelop版本是：5.0.1.3
[禁止更新]
每次  FlashDevelop 更新都会打乱所有配置，不是特别必要，可以禁止更新：    
FlashDevelop->Tools->Program Settings，左边选择 FlashDevelop 然后右边查找 State 下面的     
Check for Update 改为 Never
[插件]
ColorBox      – 不安装，干扰太大，不推荐    
NavigationBar – 安装，查看类结构，类似 VS 好用，关闭 FlashDevelop 双击 NavigationBar_2.5.1.fdz 安装     
EditorMiniMap – 缩小代码展示，可要可不要     
CustomizeTool – 不用安装     
QuickLaunch   – 可以安装也可以不安装
