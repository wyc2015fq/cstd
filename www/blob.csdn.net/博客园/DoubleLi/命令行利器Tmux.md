# 命令行利器Tmux - DoubleLi - 博客园






Tmux是一个优秀的终端复用软件，类似GNU Screen，但是对两个软件评价已经是一边倒了，大多数人认为tmux功能更加强大，使用更加方便。

Tmux不仅可以提高终端工作效率，是服务器管理工作必不可少的工具，更拉风的是可以做远程的Pair Programming。

![](http://www.linuxidc.com/upload/2015_08/150830094939871.png)

下面是我的一些.tmux.conf设置

unbind C-b
set -g prefix C-a
setw -g mode-keys vi

# split window like vim
# vim's defination of a horizontal/vertical split is revised from tumx's
bind s split-window -h
bind v split-window -v
# move arount panes wiht hjkl, as one would in vim after C-w
bind h select-pane -L
bind j select-pane -D
bind k select-pane -U
bind l select-pane -R

# resize panes like vim
# feel free to change the "1" to however many lines you want to resize by,
# only one at a time can be slow
bind < resize-pane -L 10
bind > resize-pane -R 10
bind - resize-pane -D 10
bind + resize-pane -U 10

# bind : to command-prompt like vim
# this is the default in tmux already
bind : command-prompt

**下面关于Tmux的文章您也可能喜欢，不妨看看：**

Tmux：终端复用器 [http://www.linuxidc.com/Linux/2013-07/86776.htm](http://www.linuxidc.com/Linux/2013-07/86776.htm)

tmux使用简单教程 [http://www.linuxidc.com/Linux/2014-10/107644.htm](http://www.linuxidc.com/Linux/2014-10/107644.htm)

Tmux 入门指南  [http://www.linuxidc.com/Linux/2015-07/119843.htm](http://www.linuxidc.com/Linux/2015-07/119843.htm)

用 Tmux 和 Vim 打造 IDE  [http://www.linuxidc.com/Linux/2015-06/119165.htm](http://www.linuxidc.com/Linux/2015-06/119165.htm)

[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14)下Tmux安装和使用 [http://www.linuxidc.com/Linux/2014-11/109375.htm](http://www.linuxidc.com/Linux/2014-11/109375.htm)

用 Tmux 和 Vim 打造 IDE  [http://www.linuxidc.com/Linux/2015-06/119165.htm](http://www.linuxidc.com/Linux/2015-06/119165.htm)

**Tmux 的详细介绍**：[请点这里](http://www.linuxidc.com/Linux/2014-10/107642.htm)
**Tmux 的下载地址**：[请点这里](http://sourceforge.net/projects/tmux/files/latest/download)

**本文永久更新链接地址**：[http://www.linuxidc.com/Linux/2015-08/122486.htm](http://www.linuxidc.com/Linux/2015-08/122486.htm)









