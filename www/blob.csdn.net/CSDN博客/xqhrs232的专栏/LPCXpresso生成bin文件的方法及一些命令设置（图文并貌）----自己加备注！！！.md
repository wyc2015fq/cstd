# LPCXpresso生成bin文件的方法及一些命令设置（图文并貌）----自己加备注！！！ - xqhrs232的专栏 - CSDN博客
2017年06月05日 11:23:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：782
原文地址::[http://bbs.eeworld.com.cn/thread-107754-1-1.html](http://bbs.eeworld.com.cn/thread-107754-1-1.html)
相关文章
1、如何LPCXpresso建立HEX、BIN等文件----[http://download.eeworld.com.cn/download/gs001588/277510](http://download.eeworld.com.cn/download/gs001588/277510)
一直在找使用LPCXpresso生成*.bin文件的方法，不成想竟然同时找到了LPCXpresso一些，其他的命令，如生成*.hex，*.lss及console显示程序代码长度的；现在和大家分享；
首先，罗嗦一下：
a.  *.bin     二进制代码；
b.  *.hex    十六进制代码；
c.  *.lss    C/ASM清单；
d.  build后，在console面板 显示代码长度；
1、*.bin 的生成方法
（1）第一种方法，手动生成
操作非常简单，Build project 之后，在项目管理器对应工程的Debug文件中，找到*.axf文件，然后，操作看下图：
![](http://5.eewimg.cn/data/attachment/forum/month_1006/20100610_69142c0d9285d2c6a6c0M1e0RNul5ZER.bmp)
然后，*.bin文件在Debug中显示，下图：
![](http://5.eewimg.cn/data/attachment/forum/month_1006/20100610_20b84283ea6c3e7707baTkQvc1w8i8dx.bmp)
（2）第二种方法，自动生成
设置如下：
1 、打开菜单栏：Project （工程）-> Properties （属性），属性对话框如下：
![](http://5.eewimg.cn/data/attachment/forum/month_1006/20100610_958b007755e93774035aFrigZzAzXqNz.bmp)
2 、在Properties （属性）窗口里, 打开 "C/C++ Build" 然后选择 "Settings".
3 、选择 "Build steps" 选项卡
4 、在"Post-build steps - Command" 输入框里会看到如下命令：
arm-none-eabi-size ${BuildArtifactFileName}; # arm-none-eabi-objcopy -O binary ${BuildArtifactFileName} ${BuildArtifactFileBaseName}.bin ; checksum -d ${BuildArtifactFileBaseName}.bin;
5、去掉中间的# ；
6、点击应用Apply；然后OK。
7、build project，*.bin自动生成，下图：
![](http://5.eewimg.cn/data/attachment/forum/month_1006/20100610_d94dc8b34abacb5958cbo0S9kvU2ilB2.bmp)
还是按上面的操作，在"**Post-build steps - Command**"框内，输入下列命令，build
 project后，可分别生成*.hex，*.lss文件及在console面板显示代码长度等；
命令如下：
arm-none-eabi-size ${BuildArtifactFileName};
arm-none-eabi-objdump -S ${BuildArtifactFileName} >${BuildArtifactFileBaseName}.lss;
arm-none-eabi-objcopy -O ihex ${BuildArtifactFileName} ${BuildArtifactFileBaseName}.hex;
（命令包括;分号）
build project 后：
![](http://5.eewimg.cn/data/attachment/forum/month_1006/20100610_e19cabcf246d7d69d837B7rYSDzySaLJ.bmp)
console面板显示代码长度
![](http://5.eewimg.cn/data/attachment/forum/month_1006/20100610_f0355afb3c736c1fe4f00noj7m2p0EGc.bmp)
备注::
1》手工生成的bin必须用 安装目录下bin目录下的checksum.exe运行下就可以了

//====================================================================================================================
备注::
1>>通过上面的方式产生的hex文件通过flash magic烧到lpc单片机是可以跑起来的！！！
