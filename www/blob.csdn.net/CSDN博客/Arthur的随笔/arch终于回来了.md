# arch终于回来了 - Arthur的随笔 - CSDN博客
2012年03月24日 15:09:09[largetalk](https://me.csdn.net/largetalk)阅读数：760
昨天闲得无聊更新下arch，结果今天早上发现开不了机了，连命令行也进不去，各种试都没有效果， 手边得arch安装盘好像也坏了，插进去重启也没什么反应，还以为电脑坏了，后来找了张gentoo得启动盘，进去之后改了run level 为3， 重启后能进命令行了，但一startx就不行，不管gonme， xfce， xterm都不行，后来才怀疑是显卡驱动得问题，我显卡驱动装得是官方驱动，上网一查，果然，最新Xorg和ati官方驱动不兼容，只得把官方驱动卸掉再装arch源里驱动
```
/bin/bash /usr/share/ati/fglrx_uninstall.sh --force
mv /etc/X11/xorg.conf .
pacman -S xf86-video-ati
```
这样，我得arch终于回来了，不过用这个驱动慢好多阿，进去之后各种慢，xfce好像也变了，鼠标样式变了， chrome全屏不行了，alt+tab没反应了，我擦，换flux-box玩吧
