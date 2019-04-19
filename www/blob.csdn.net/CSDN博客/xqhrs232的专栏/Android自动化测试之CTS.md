# Android自动化测试之CTS - xqhrs232的专栏 - CSDN博客
2012年02月29日 16:07:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：575
原文地址::[http://blog.csdn.net/andy_android/article/details/7242105](http://blog.csdn.net/andy_android/article/details/7242105)
根据Android官网给出的android-cts-manual 配置好了device后，开始测试。
首先配置软件环境：
1、下载Android-2.1系统，并编译。
$ . build/envsetup.sh          //配置选项，并编译android源码
$ make cts                                 //android源码编译好后，在编译cts
2、编译好cts后生成的文件位置如下
#mydroid/out/host/linux-x86/
在该目录下包含如下测试文件
- Package CTS: out/host/linux-x86/cts/android-cts.zip 
- cts make file: mydroid/build/core/tasks/cts.mk 
- run cts program: mydroid/out/host/linux-x86/bin/cts 
- test plans: mydroid/out/host/linux-x86/cts/android-cts/repository/plans
- test packages: mydroid/out/host/linux-x86/cts/android-cts/repository/testcases
- test results: mydroid/out/host/linux-x86/cts/android-cts/repository/results
- CTS program settings value: mydroid/cts/tools/utils/host_config.xml
3、连接上Device后，进入如下目录
#mydroid/out/host/linux-x86/bin/
并输入./cts来启动cts测试，效果如下：
Android  CTS  version  2.1_pre_r2  
device(0123456789ABCEDF)  connected
cts_host >
此时输入 help  将会列出cts所有的操作。
cts_host> ls  --plan   //list all  test plan 
cts_host> ls  -p           //list all testcase
cts_host>start  --plan  [plan name]     //test   plan
.............
.............
4、进入目录 
#mydroid/out/host/linux-x86/cts/android-cts/repository/plans
将会看到有8个测试的基类，它们分别是
 Android.xml     
AppSecurity.xml
CTS.xml
Java.xml
Performance.xml
RefApp.xml
Signature.xml
VM.xml
用UltraEdit打开这些文件后，将会看到所有基类里包含的package 都给出了相应的uri，
cts将根据这些uri去测试每个基类里的package，
5、cts_host > start  --plan  Android  
输入上面的命令后，就开始测试 Android API, 其它的类测试命令和这个一致，如下：
cts_host > start  --plan  Android           //test    Android  API
cts_host > start  --plan  CTS               //contains all tests and will run ~21,000 tests on your device  
cts_host > start  --plan  Java             //test   Java  core  libary
。。。。。。。。。
6、测试好后，通过输入如下命令来查看测试情况
cts_host > ls  -r
7、CTS测试会自动生成相应的测试包，该包位于如下目录：
#mydroid/out/host/linux-x86/cts/android-cts/repository/results
每个测试包中包含了如下文件;
cts_result.css
cts_result.xsl
logo.gif
newrule-green.png
testResult.xml
该包的测试情况都在 testResult.xml 文件中，通过查看该文件可以知道，那些是和
Android兼容的，，
8、未解决的问题
在测试中会遇到adb重启，adb重启后，断开Device连接，此时需要拔下usb再插上，才能再次测试，
如何解决这一情况有待研究，，
9、几个不错的参考网站
1、http://source.android.com/compatibility/index.html        //  Android  compatibility
2、http://androidboss.com/android-cts-compatibility-test-suite-introduction-2/       // CTS  how to add  new testcase    
3、 http://www.at91.com/android4sam/bin/view/Android4SAM/                  // Atmel_android  sam9m10   website
