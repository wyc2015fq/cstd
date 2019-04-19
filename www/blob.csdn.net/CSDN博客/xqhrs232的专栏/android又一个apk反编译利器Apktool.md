# android又一个apk反编译利器Apktool - xqhrs232的专栏 - CSDN博客
2011年04月13日 09:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1482
原文地址::[http://apps.hi.baidu.com/share/detail/20381960](http://apps.hi.baidu.com/share/detail/20381960)
上周[蛋蛋](http://kugou.me/)给了我一个神奇的东西[Apktool](http://code.google.com/p/android-apktool/)。神在他能把加密的XML文档转化为可见可编辑的形式。
但一时我的电脑也没有装linux，前几天拿到本本后，一直到今天才把ubuntu配置好。
顺手打开google code发现Apktool更新了，居然支持win了。。。。我说我怎么就那么悲情呢。。
那本文就是一篇介绍在windows环境下使用Apktool的笔记。
# 安装
1.先装JAVA环境，JDK/JRE都行，[官网下载](http://java.sun.com/javase/downloads/index.jsp) 装过的就跳过吧
2.下载apktool.jar及相关文件，[这里](http://code.google.com/p/android-apktool/downloads/list)下[apktool-1.0.0.tar.bz2](http://android-apktool.googlecode.com/files/apktool-1.0.0.tar.bz2)和[apktool-install-windows-2.1_r01-1.zip](http://android-apktool.googlecode.com/files/apktool-install-windows-2.1_r01-1.zip)
3.解压apktool.jar到 C:/Windows 解压apktool-install-windows.zip到任意文件夹(例如E盘根目录)
4.Win+R 运行CMD，用cd命令转到apktool-install-windows所在文件夹，输入apktool看看。会列出一些帮助的话就成功了。
# Apktool 命令
apktool d XXX.apk ABC    反编译XXX.apk到文件夹ABC
apktool b ABC               从文件夹ABC重建APK，输出到ABC/dist/out.apk
# 然后我们反编译一枚软件玩玩…
AutoMemoryManager的免费版底部有一条广告，去掉它吧。
把com.lim.android.automemman.apk放到同文件夹(我的就是E盘根目录)
Win+R 运行CMD
E:<回车>
E:/>apktool d com.lim.android.automemman.apk AMM <回车>
I: Baksmaling…
I: Decoding resource table…
I: Decoding resources…
I: Copying assets and libs…
现在文件被decode到E:/AMM了，打开E:/AMM/res/layout/main.xml看，所有都可见了吧~
编辑第59行
<com.admob.android.ads.AdView android:id=”@id/ad” android:layout_width=”fill_parent” android:layout_height=”wrap_content” admobsdk:backgroundColor=”#ff000000″ admobsdk:textColor=”#ffffffff” admobsdk:keywords=”Android application” />
改为
<com.admob.android.ads.AdView android:id=”@id/ad” android:layout_width=”0.0dip” android:layout_height=”0.0dip” admobsdk:backgroundColor=”#ff000000″ admobsdk:textColor=”#ffffffff” admobsdk:keywords=”Android application” />
然后CMD输入
E:/>apktool b AMM
I: Checking whether sources has changed…
I: Smaling…
I: Checking whether resources has changed…
I: Building resources…
I: Building apk file…
用Auto-sign签名E:/AMM/dist/out.apk 安装
这样广告就不见了，如图②
![screenshot](http://hiphotos.baidu.com/429680270/pic/item/bad83b3e2f22a08e9d3d6268.jpg)
再看图③效果,嘿嘿~很帅吧…
这其实就是改了AndroidManifest.xml里的ADmob广告ID罢了。。
以上两个版本下载 [无广告版](https://dl.dropbox.com/u/4510877/automemman_LvWind_no_ad.apk)[个人广告版](https://dl.dropbox.com/u/4510877/automemman_LvWind_ad_myself.apk)
P.S.1：希望AMM的作者不要介意我那你的软件来做实验~如果你觉得不合适，请联系我删除下载连接。
P.S.2：前面试着去掉dolphin browser的广告…然后我崩溃了..居然google和admob的广告同时存在…
Tags : [android](http://www.alaxy.cn/tag/android/),
[apk](http://www.alaxy.cn/tag/apk/),
[Apktool](http://www.alaxy.cn/tag/apktool/),
[反编译](http://www.alaxy.cn/tag/%E5%8F%8D%E7%BC%96%E8%AF%91/)
