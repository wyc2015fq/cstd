# archlinux gedit乱码解决 - Arthur的随笔 - CSDN博客
2011年12月29日 02:23:54[largetalk](https://me.csdn.net/largetalk)阅读数：1290标签：[ubuntu																[vim](https://so.csdn.net/so/search/s.do?q=vim&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[linux																[杂七杂八](https://blog.csdn.net/largetalk/article/category/823511)](https://blog.csdn.net/largetalk/article/category/727912)
在ubuntu中gedit乱码解决是打开gconf-editor, 在﻿﻿/apps/gedit-2/preferences/encodings中给auto-detected键添加一个‘GB18030’，并将其排到列表顶部
在archlinux中得gconf-editor /apps下没有gedit，但dconf-editor有， 如果没有dconf-editor，就安装一个， 打开dconf-editor,
展开org/gnome/gedit/preferences/encodingsauto-detected的value中加入’GB18030′，加在uft8后面；
show-in-menu的value中加入’GB18030′。
===============================================================
vi中文：Vim在~/.vimrc中加入下面的代码 没有.vimrc文件可以新建一个let &termencoding=&encodingset
 fileencodings=utf-8,gbk,ucs-bom,cp936
