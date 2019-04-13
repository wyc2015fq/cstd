
# Linux命令行(console)屏幕分辨率调整 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月29日 23:57:17[Jlins](https://me.csdn.net/dyllove98)阅读数：16809标签：[Linux命令行console屏幕分辨率																](https://so.csdn.net/so/search/s.do?q=Linux命令行console屏幕分辨率&t=blog)


在root用户模式下，输入$ vi /boot/grub/grub.conf（路径可能会不一样，也可以是 /etc/grub.conf），打开grub.conf文件

grub是系统启动器之一，用来存放启动项设置和信息，修改的时候必须特别小心，否则系统会瘫痪掉~~
下面简单介绍一下：
文件里面一大段的注解，主要是说明了一些情况，英文好的看看就能理解了。
default=0 ：菜单项，0表示第一项，如果是多系统可以修改此项改变默认光标停留位置；
timeout=5 ：菜单到自动启动系统前的提留时间，可根据需要修改；
splashimage=······ ： 启动菜单背景图表，可根据需要修改；
title······ ：启动项名称，可根据需要修改；
kernel······ ：一些版本信息和root路径；
rhgb ：redhat graphics boot，就是会看到图片来代替启动过程中显示的文本信息，这些信息在启动后用dmesg也可以看到
quiet ： 在启动过程中只有重要信息显示，类似硬件自检的消息不回显示
我们修改分辨率，需要在kernel那行加入 vga=791
![wKiom1R5k7LARDHOAAKH3b3g62o767.jpg](http://s3.51cto.com/wyfs02/M02/54/26/wKiom1R5k7LARDHOAAKH3b3g62o767.jpg)
注意：vga的参数不是随便输入的，可以根据需要进行修改，我的电脑上面最高只有16位，24位的参数不能生效，因此选择 vga=791。
![wKioL1R5lE2S0wbCAAD2NnWsg38215.jpg](http://s3.51cto.com/wyfs02/M00/54/25/wKioL1R5lE2S0wbCAAD2NnWsg38215.jpg)
rhgb quiet 可以删除也可以保留，看自己喜欢。完成设置后，需要reboot系统，然后可以看到分辨率变大了，看上去舒服很多~~~~
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)

