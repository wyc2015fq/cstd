# Centos 右上角面板里没有wired network图标的问题 - 在思索中前行！ - CSDN博客





2014年10月13日 14:58:11[_Tham](https://me.csdn.net/txl16211)阅读数：9788标签：[wired Network																[Centos 面板																[网络图标消失](https://so.csdn.net/so/search/s.do?q=网络图标消失&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)






开了很多的网页查看解决这个问题，都不是很有效，最后很简单的改了下一个文件就ok了，自己记录下，以免忘记！


打入命令：sudo gedit /etc/NetworkManager/nm-system-settings.conf（Ubuntu下）


出现文件内容：




# This file is installed into /etc/NetworkManager, and is loaded by

# NetworkManager by default.  To override, specify: '--config file'

# during NM startup.  This can be done by appending to DAEMON_OPTS in

# the file:

#

# /etc/default/NetworkManager

#

[main]

plugins=ifupdown,keyfile


[ifupdown]

managed=true

（这里false改成true）




还有一种情况是上面情况已经使用，但是你又误删了网络管理：（Centos下可用）

具体办法是将面板的默认配置去掉，然后就会恢复面板的默认配置，就能“找回”丢失的网络管理的那个图标：

打开终端 ，输入下列命令：
gconftool --recursive-unset /apps/panel （有可能要重启）

接下来删除：
rm -rf ~/.gconf/apps/panel

最后执行：
pkill gnome-panel](https://so.csdn.net/so/search/s.do?q=Centos 面板&t=blog)](https://so.csdn.net/so/search/s.do?q=wired Network&t=blog)




