# Rose 2003 在 Xp Home不能安装的解决方案（转载） - xqhrs232的专栏 - CSDN博客
2009年06月07日 22:01:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：811
“Rose 2003 在 Xp Home不能安装”解决方案 
问题描述： 
Rose 2003 在 Xp Home下安装会出现“安装的操作系统不被支持”提示，并终止安装。 
Rational官方网站称Xp Home是"Unsupported OS" 
解决方法： 
绕过安装程序对操作系统版本的检查 
注意：该方法只能保证安装程序得以运行，不涉及Rose在Xp Home的实际运行效果。目前为止Rose在笔者的Xp Home系统上表现良好。 
使用工具：Microsoft Orca 
提供文件：Orca.rar 
使用方法（清先备份相应文件）： 
用Orca打开Setup/rose.msi,查找"xph",会定位到该行： 
(VersionNT AND (NOT RSWINXPHOME))OR (DISABLE_PLATFORM_BLOCKS = 1)|You are attempting to install on an...
用Table/drop row删除该行,保存该文件，退出 
重新运行安装程序 
注意：同样的限制条件出现在setup文件夹其他的msi文件中，但笔者在安装过程中没有遇到问题。如果你在安装某个特性的时候出现问题，可以使用上面的步骤修改相应的msi文件。
[http://topic.csdn.net/u/20080708/10/c1c2eaaa-7af7-4d44-b694-0ffca77614fc.html](http://topic.csdn.net/u/20080708/10/c1c2eaaa-7af7-4d44-b694-0ffca77614fc.html)
[**修改windows**](http://writeblog.csdn.net/sort/tag/%E4%BF%AE%E6%94%B9windows)[**installer**](http://writeblog.csdn.net/sort/tag/installer)[**orca**](http://writeblog.csdn.net/sort/tag/orca)
[http://download.csdn.net/source/1069267](http://download.csdn.net/source/1069267)
