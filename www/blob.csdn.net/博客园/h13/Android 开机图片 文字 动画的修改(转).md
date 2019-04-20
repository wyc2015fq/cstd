# Android 开机图片/文字/动画的修改(转) - h13 - 博客园
Android 开机会出现3个画面：
a. [Linux](http://www.2cto.com/os/linux/) 系统启动，出现Linux小企鹅画面(reboot)；
b. Android平台启动初始化，出现"A N D R I O D"文字字样画面；
c. Android平台上层图形系统启动，出现含闪动的ANDROID字样的动画图片(start)。
1/ 开机图片(Linux小企鹅)
Linux Kernel引导启动后，加载该图片。
/kernel/trunk/drivers/video/fbmem.c
/kernel/trunk/drivers/video/fbmem.h
/kernel/trunk/drivers/video/logo/logo.c
/kernel/trunk/drivers/video/logo/logo.h
/kernel/trunk/drivers/video/logo/Kconfig
/kernel/trunk/include/linux/linux_logo.h
2/ 开机文字("A N D R I O D")
Android 系统启动后，读取 /initlogo.rle（一张565 rle压缩的位图），如果读取成功，测在/dev/graphics/fb0显示Logo图片；如果读取失败，则将/dev/tty0设为TEXT模式， 并打开/dev/tty0，输出文本“A N D R I O D”字样。
相关代码：
/system/core/init/init.c
/system/core/init/init.h
/system/core/init/init.rc
/system/core/init/logo.c
*.rle文件的制作步骤:
a. 使用GIMP或者Advanced Batch Converter软件，将图象转换为RAW格式；
b. 使用android自带的rgb2565工具，将RAW格式文件转换为RLE格式(如：rgb2565 -rle < initlogo.raw > initlogo.rle)。
3/ 开机动画(闪动的ANDROID字样的动画图片)
Android 的系统登录动画类似于Windows系统的滚动条，是由前景和背景两张PNG图片组成，这两张图片存在于/system/framework /framework-res.apk文件当中。前景图片（android-logo-mask.png）上的Android文字部分镂空，背景图片 （android-logo-shine.png）则是简单的纹理。系统登录时，前景图片在最上层显示，程序代码控制背景图片连续滚动，透过前景图片文字 镂空部分滚动显示背景纹理，从而实现动画效果。
相关代码：
/frameworks/base/cmds/bootanimation/BootAnimation.h
/frameworks/base/cmds/bootanimation/BootAnimation.cpp
/frameworks/base/core/res/assets/images/android-logo-mask.png
Android默认的前景图片，文字部分镂空，大小256×64
/frameworks/base/core/res/assets/images/android-logo-shine.png
Android默认的背景图片，有动感效果，大小512×64
参考文档：
图说Android开机画面和开机动画
[http://www.shudoo.com/09/1030/15/13418431.html](http://www.shudoo.com/09/1030/15/13418431.html)
initlogo.rle: display an image on boot
[php](http://forum.xda-developers.com/showthread.<a%20href=)?t=443431">http://forum.xda-developers.com/showthread.php?t=443431
分析Android 根文件系统启动过程（init守护进程分析）
[http://crazier9527.javaeye.com/blog/454635](http://crazier9527.javaeye.com/blog/454635)
