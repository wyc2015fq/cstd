# 华为Y300-0000手机解锁Bootloader步骤 - xqhrs232的专栏 - CSDN博客
2013年05月31日 22:07:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2776
原文地址::[http://www.huaweidevice.com/cn/servicePolicy.do?method=getUnlockStep](http://www.huaweidevice.com/cn/servicePolicy.do?method=getUnlockStep)
相关网帖
1、**华为Y300如何root----[http://www.ijiajiao.cc/view-4-4351.html](http://www.ijiajiao.cc/view-4-4351.html)**
2、华为Y300如何root----[http://jingyan.baidu.com/article/ca2d939dc2058aeb6d31ce7d.html](http://jingyan.baidu.com/article/ca2d939dc2058aeb6d31ce7d.html)
3、[[已解决]](http://bbs.hiapk.com/forum.php?mod=forumdisplay&fid=113&filter=typeid&typeid=491)[华为Y300解锁和root教程，和C8813完全通用。亲测完全没有问题！](http://bbs.hiapk.com/thread-4551879-1-1.html) ----[http://bbs.hiapk.com/thread-4551879-1-1.html](http://bbs.hiapk.com/thread-4551879-1-1.html)
## 解锁步骤
![](http://www.huaweidevice.com/resource/cn/Image/line_460-120623222144.gif)
第一步：准备工作
- 
在PC上正确安装手机驱动程序，驱动请在华为终端官网上下载。
- 
下载Google提供的ADB工具包并在PC上安装，如安装到D:\adb_tools-2.0目录，确认目录中带有fastboot.exe文件。
第二步：解锁操作
- 
手机进入fastboot模式：
先将手机关机（注意需要首先在设置菜单中，关闭“快速启动”功能，或者关机后拔下电池，超过2秒后再重新插入），然后同时按下音量下键以及开机键（平板请同时按下音量上键及开机键），并保持10秒钟以上时间，就可以进入fastboot模式。
- 
连接手机和PC机：
使用USB线连接手机和PC机，打开PC机的命令行窗口，进入ADB安装目录，确认手机与PC连接正常，确认方法为在命令行窗口输入fastboot devices，可以看到正常连接的信息，如：9e42530e fastboot。
![](http://www.huaweidevice.com/resource/cn/Image/deblock_process_clip_image002-120623223006.jpg)
- 
执行解锁命令：
在PC机命令行窗口中输入fastboot oem unlock ****************，*号为16位解锁密码，例如：fastboot oem unlock 1234567812345678。
![](http://www.huaweidevice.com/resource/cn/Image/deblock_process_clip_image004-120623223024.jpg)
- 
等待手机解锁完成：
输入解锁密码后，手机将自动重启，如果输入密码正确，手机将进入恢复出厂设置模式。恢复出厂设置完成后，手机自动重启，进入待机界面，完成整个解锁操作。如果用户输入密码信息不正确，手机将提示出错信息，并进入待机界面。
- 
查询手机解锁是否成功：
重复1、2步，然后在PC的命令行窗口中输入fastboot oem get-bootinfo，将显示当前手机bootloader的状态信息，例如“Bootloader Lock State: LOCKED”，表示bootloader仍处于锁定状态，请重新进行解锁操作或者确认密码是否正确后再进行；如果显示“Bootloader Lock State: UNLOCKED”，表示手机已经解锁，可以进行刷机操作。
![](http://www.huaweidevice.com/resource/cn/Image/deblock_process_clip_image006-120623223033.jpg)
注意：解锁成功后，如果希望将手机重新加锁请按以下步骤操作：首先，通过SD卡升级方式，将手机版本重新恢复为华为官方发布版本；然后，进入fastboot模式，在PC的命令行下输入命令fastboot oem relock ****************，*号为16位解锁密码。手机将自动重启，bootloader转换为“RELOCKED”状态。
