# linux history 命令 禁用history - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月05日 17:35:18[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2503
保存在.bash_history文件中，默认1000条，你也可以更改这个 值
！！:上一个指令
!number 运行第几个指令
查看命令历史的时间戳，那么可以执行：
# export HISTTIMEFORMAT='%F %T '# history | more
1 2008-08-05 19:02:39 service network restart2 2008-08-05 19:02:39 exit3 2008-08-05 19:02:39 id
**注意**：这个功能只能用在当 HISTTIMEFORMAT 这个环境变量被设置之后，之后的那些新执行的 bash 命令才会被打上正确的时间戳。

**使用 HISTSIZE 控制历史命令记录的总行数**
将下面两行内容追加到 .bash_profile 文件并重新登录 bash shell，命令历史的记录数将变成 450 条：
# vi ~/.bash_profileHISTSIZE=450HISTFILESIZE=450
**使用 HISTFILE 更改历史文件名称**
HISTFILE=/root/.commandline_warrior
**强制 history 不记住特定的命令**
将 HISTCONTROL 设置为 ignorespace，并在不想被记住的命令前面输入一个空格：
# export HISTCONTROL=ignorespace
**使用 HISTSIZE 禁用 history**
如果你想禁用 history，可以将 HISTSIZE 设置为 0：
# export HISTSIZE=0
[https://linuxtoy.org/archives/history-command-usage-examples.html](https://linuxtoy.org/archives/history-command-usage-examples.html)

