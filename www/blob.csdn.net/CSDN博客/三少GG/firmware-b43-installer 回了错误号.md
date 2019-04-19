# firmware-b43-installer: 回了错误号 - 三少GG - CSDN博客
2012年01月18日 15:52:14[三少GG](https://me.csdn.net/scut1135)阅读数：1325标签：[terminal																[ubuntu																[dell																[脚本																[终端](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=dell&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=terminal&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
问题：
firmware-b43-installer: 子进程 已安装 post-installation 脚本 返回了错误号 1
问题补充：
这个系列的问题出现，不过好似也没影响具体的使用，还可以装软件
不过真的想知道为什么
++++++++++++++++++++++﻿﻿﻿﻿
[http://forum.ubuntu.org.cn/viewtopic.php?t=300262](http://forum.ubuntu.org.cn/viewtopic.php?t=300262)
这是由于驱动不兼容硬件造成的。解决的办法是删除它，然后尝试其它驱动。
进入Terminal:
**代码:**
sudo apt-get remove --purge firmware-b43-installer
然后进入 Ubuntu 菜单：
系统 -> 管理 -> 附加驱动
选择一个激活即可。相对常见的选择是`Broadcom B43 wireless driver`。
[superfrog2010](http://passport.baidu.com/?business&aid=6&un=superfrog2010#2)|一级
我也遇到同样的问题，不管装什么软件都会出现这个错误。后来发现我是装错了第三方无线网卡驱动软件导致的。
打开终端:sudo apt-get remove --purge firmware-b43-installer
然后再到“系统”，“系统管理”，“附加驱动“里面找正确的驱动安装就好了。
我的电脑是Dell E6400。开始安装的是“Broadcom STA 无线驱动”，卸载后重新安装的“Broadcom B43 wireless driver". 搞定。 
