# 科学网—U盘里存的文件（夹）突然都变成快捷方式怎么办？ - 王伟华的博文




# U盘里存的文件（夹）突然都变成快捷方式怎么办？                           

已有 16887 次阅读2014-7-4 18:31|个人分类:[计算机&大数据](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11842&view=me)|系统分类:[生活其它](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=4)|关键词:1K文件快捷方式病毒,免疫|[免疫](misc.php?mod=tag&id=4902), [1K文件快捷方式病毒](misc.php?mod=tag&id=212228)



文件夹快捷方式病毒大约在2009年前，现在很少见了，但不常见，并不意味着没有了，今天我和一个老师的U盘又都“巧遇”这个病毒了，那么U盘里存的文件突然都变成快捷方式怎么办？（记录在这里，权当资料保存！）

     1. 文件夹快捷方式病毒。该病毒只感染文件夹，隐藏正常的文件夹，显示的只是快捷方式，快捷方式指向病毒脚本。


解决方法：将一下代码复制到记事本里另存为 杀毒.bat 将它放到U盘里运行一下，然后删除掉vbs后缀的那个脚本病毒 还有文件夹快捷方式，同名文件夹都有两份 注意辨别，硬盘感染的话就把这个bat文件放到各个盘符下运行 然后手动删除病毒体~
祝你好运~~~ 代码：

@echo off
@echo +-------------------------------------------------------------+
@echo 本程序消除文件夹被病毒设置的隐藏属性 
@echo 
@echo 2014.7
@echo +-------------------------------------------------------------+
@echo.
@ECHO 可能需要一段时间，请耐心等待
@echo 耐心等待...
attrib -s -h *. /S /D
attrib +s +h System~1
attrib +s +h Recycled
attrib +s +h +a ntldr
@ECHO 完成！


@echo on

或用金山U盘专杀和金山网盾这两个软件，百度一下就找到了。先运行金山网盾，点击一键修复，处理掉病毒和系统错误。然后再运行金山U盘专杀这个工具，勾选全盘扫描，他能处理掉病毒母体和那些产生的快捷方式（如有残留请自己删掉，一般没残留）。


   2. 文件快捷方式病毒免疫用下属代码，保存为UPAN.cmd，放入U盘，双击即可。


echo 正在杀毒并生成免疫 请勿关闭


attrib -r -a -s -h /s /d

del /f /s /q *.inf

MKDIR autorun.infKill........

attrib +r +a +s +h *.inf /s /d

echo 完成

然后手动删除扩展名为“*.vbs”的病毒文件即可。





也可以使用

[U盘文件夹变快捷方式的解决办法](http://wangchunlai-2005.blog.163.com/blog/static/572309982009934323441/)

当然最简单的办法就是下载一个专杀工具：1KB文件夹快捷方式病毒专杀工具[1kbwjjkjfsbdzsgj.zip](http://blog.sciencenet.cn/home.php?mod=attachment&id=56407)

以下转自[http://www.qqtn.com/down/42275.html#szdz](http://www.qqtn.com/down/42275.html#szdz) 使用说明：

1KB文件夹快捷方式病毒清除专用附件包含三部分。

1、清理工具

2、数据流清除工具

3、wscript文件权限恢复

在系统盘为FAT32的系统内，只需要使用“清理工具”清理即可。

在系统盘为ntfs的系统内，首先使用“清理工具”清理，然后再用“数据流清除工具”清除检测到的数据流。

最后再使用“wscript文件权限恢复”恢复wscript文件权限即可。

全部内附说明图。

其中各盘被隐藏的文件夹的隐藏属性的恢复，在清除工具内有相关操作，看内附操作图即知。

图文操作介绍：

**wscript文件权限恢复：**

操作说明：

![](http://image.sciencenet.cn/album/201407/04/1847277f3o3fzuy9x70p9t.jpg)












转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-809036.html](http://blog.sciencenet.cn/blog-81613-809036.html)

上一篇：[蒙特卡罗(Monte Carlo Method)方法学习资料](blog-81613-793209.html)
下一篇：["数"说世界杯](blog-81613-810885.html)


