
# shell中的wait - Augusdi的专栏 - CSDN博客


2016年10月20日 12:41:42[Augusdi](https://me.csdn.net/Augusdi)阅读数：3157


# wait命令
[Shell内建命令](http://man.linuxde.net/sub/shell%E5%86%85%E5%BB%BA%E5%91%BD%E4%BB%A4)
**wait命令**用来等待指令的指令，直到其执行完毕后返回终端。该指令常用于shell脚本编程中，待指定的指令执行完成后，才会继续执行后面的任务。该指令等待作业时，在作业标识号前必须添加备份号"%"。
### 语法
wait(参数)
### 参数
进程或作业标示：指定进程号或者作业号。
### 实例
使用命令wait等待作业号为1的作业完成后再返回，输入如下命令：
wait %1\#等待作业号为3的作业完成执行上面的指令后，将输出指定作业号的指令，如下所示：
[find](http://man.linuxde.net/find)/ -name password

[http://man.linuxde.net/wait](http://man.linuxde.net/wait)
﻿﻿

