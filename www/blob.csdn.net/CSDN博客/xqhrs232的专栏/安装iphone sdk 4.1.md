# 安装iphone sdk 4.1 - xqhrs232的专栏 - CSDN博客
2012年10月09日 11:20:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：582标签：[iphone																[os																[磁盘																[工具																[虚拟机																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)](https://so.csdn.net/so/search/s.do?q=iphone&t=blog)
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[http://blog.csdn.net/hipeterfeng/article/details/5890325](http://blog.csdn.net/hipeterfeng/article/details/5890325)
iphone sdk 4.1已经于昨天夜里完成下载。
在mac os x 10.6安装iphone sdk 4.1时，程序提示必须在mac os x10.6.2下才允许安装。
先是在“软件更新”菜单检查并更新mac os x等，更新到272M时，mac os x卡住了。
我发现这个问题，就到苹果网站找到mac os x10.6.4的更新包[MacOSXUpdCombo10.6.4.dmg](http://supportdownload.apple.com/download.info.apple.com/Apple_Support_Area/Apple_Software_Updates/Mac_OS_X/downloads/061-8422.20100615.Gerg5/MacOSXUpdCombo10.6.4.dmg)。
更新包要求现有系统必须是Mac OS X v10.6 - Mac OS X 10.6.3，正好当前系统满足这个要求。
安装MacOSX10.6.4的补丁后，再安装iphone sdk 4.1时，安装程序提示空间不够：要求的空间是8.17G，现有可用空间是6.07G，剩余-2.1G，无法继续安装。
在虚拟机扩展当前15G的磁盘空间到20G
在“应用程序”|“实用工具”|“磁盘工具”
重新安装mac os x和iphone sdk。
