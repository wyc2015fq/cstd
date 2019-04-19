# 制作Mac OS 系统安装u盘----试了下，确实必须在mac os系统里面去制作！！！ - xqhrs232的专栏 - CSDN博客
2018年12月12日 10:09:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：458
原文地址::[https://blog.csdn.net/ikqmksqm/article/details/78780017](https://blog.csdn.net/ikqmksqm/article/details/78780017)
相关文章
1、Mac OS安装启动盘的制作----[https://blog.csdn.net/ZhangKui0418/article/details/73650819](https://blog.csdn.net/ZhangKui0418/article/details/73650819)
2、制作mac系统正式版U盘USB启动安装盘方法教程 (全新安装 Mac 系统)----[https://blog.csdn.net/liuyangsds/article/details/72478519](https://blog.csdn.net/liuyangsds/article/details/72478519)
3、Windows环境下制作MACOS X U盘安装盘----[https://blog.csdn.net/zqj6893/article/details/50602354](https://blog.csdn.net/zqj6893/article/details/50602354)
4、在windows系统制作mac os的U盘启动盘----[https://blog.csdn.net/qidizi/article/details/46117215](https://blog.csdn.net/qidizi/article/details/46117215)
5、windows7下制作苹果mac os x 10.10Yosemiteu盘启动盘----[https://blog.csdn.net/u010098331/article/details/51243628](https://blog.csdn.net/u010098331/article/details/51243628)
6、在win7下制作MAC启动u盘----[https://blog.csdn.net/codeingdog/article/details/39611511](https://blog.csdn.net/codeingdog/article/details/39611511)
7、TransMac(dmg文件打开工具)v11.4----[http://www.pc6.com/softview/SoftView_91686.html](http://www.pc6.com/softview/SoftView_91686.html)
8、制作 macOS Mojave U盘USB启动安装盘方法教程 (全新安装 Mac 系统)----[https://www.iplaysoft.com/macos-usb-install-drive.html](https://www.iplaysoft.com/macos-usb-install-drive.html)
9、U盘安装Mac High Sierra（制作U盘启动必须是在mac系统中）----[https://blog.csdn.net/sunxiaoju/article/details/80783269](https://blog.csdn.net/sunxiaoju/article/details/80783269)
需要：大于8g的u盘(自行备份u盘数据)，Mac电脑一部。
- 
下载操作系统安装程序(APP Store里面下载)
- 
打开 “应用程序 → 实用工具 → 磁盘工具”，将U盘「抹掉」(格式化) 成「Mac OS X 扩展（日志式）」格式、GUID 分区图，并将U盘命名为「MyVolume」。(注意：这个盘符名称将会与后面的命令一一对应，如果你改了这盘符的名字，必须保证后面的命令里的名称也要一致。)![](http://upload-images.jianshu.io/upload_images/3018162-d9523a188e0e086f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
打开应用程序-实用工具-终端,输入命令
High Sierra：
`sudo /Applications/Install\ macOS\ High\ Sierra.app/Contents/Resources/createinstallmedia --volume /Volumes/MyVolume`
Sierra：
`sudo /Applications/Install\ macOS\ Sierra.app/Contents/Resources/createinstallmedia --volume /Volumes/MyVolume --applicationpath /Applications/Install\ macOS\ Sierra.app`
El Capitan：
`sudo /Applications/Install\ OS\ X\ El\ Capitan.app/Contents/Resources/createinstallmedia --volume /Volumes/MyVolume --applicationpath /Applications/Install\ OS\ X\ El\ Capitan.app`
Yosemite：
`sudo /Applications/Install\ OS\ X\ Yosemite.app/Contents/Resources/createinstallmedia --volume /Volumes/MyVolume --applicationpath /Applications/Install\ OS\ X\ Yosemite.app`
Mavericks：
`sudo /Applications/Install\ OS\ X\ Mavericks.app/Contents/Resources/createinstallmedia --volume /Volumes/MyVolume --applicationpath /Applications/Install\ OS\ X\ Mavericks.app`
键入相应的命令后，请按下 Return 键。
