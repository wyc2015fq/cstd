# 有些东西还得记下来，看过了又忘了，找起来又难找 （fluxbox, ab, seige) - Arthur的随笔 - CSDN博客
2012年01月16日 15:19:43[largetalk](https://me.csdn.net/largetalk)阅读数：889标签：[测试工具																[测试																[apache																[2010																[web](https://so.csdn.net/so/search/s.do?q=web&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=测试工具&t=blog)
fluxbox 1.0以上一定要 emerge feh， 才能正确显示背景，和透明桌面。
设置随机桌面背景：
~/.fluxbox/init 文件内容
session.screen0.rootCommand: fbsetbg -c -f -r /home/photos/favorites
设置自动启动文件
~/.fluxbox/apps 文件内容
[startup] {/usr/bin/conky -d}
[startup] {/usr/bin/fcitx -d}
[startup] {/usr/bin/xscreensaver}
[startup] {/usr/bin/gmail-notify}
设置桌面字体
~/.Xresources 文件内容
Xft.antialias: 1
Xft.dpi: 96
Xft.hinting: 1
Xft.hintstyle: hintfull
Xft.rgba: none
============
add 
/usr/bin/ibus-daemon &
/usr/bin/xscreensaver -no-splash &
to .fluxbox/startup before exec fluxbox
============
add
# open gnome-terminal
Control Mod1 g :Exec gnome-terminal
# lock screen
Control Mod1 l :Exec xscreensaver-command -lock
to ~/.fluxbox/keys
5 分钟 Fluxbox 指南: [http://linuxtoy.org/archives/5-mins-fluxbox-guide.html](http://linuxtoy.org/archives/5-mins-fluxbox-guide.html)
使用Siege进行WEB压力测试: [http://www.kuqin.com/web/20080726/12472.html](http://www.kuqin.com/web/20080726/12472.html)
三种压力测试工具 http_load 和 apache ab 、 siege 压力测试（转）: [http://www.sphinxsearch.org/archives/305](http://www.sphinxsearch.org/archives/305)
[调优] 压力测试软件 Siege 的正确用法: [http://www.php-oa.com/2010/06/25/siege.html](http://www.php-oa.com/2010/06/25/siege.html)
 ss - another utility to investigate sockets
ss -a #all socket
ss -l #listen socket
ss -m #memory used
ss -s #summary
