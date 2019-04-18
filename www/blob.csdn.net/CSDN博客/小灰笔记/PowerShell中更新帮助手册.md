# PowerShell中更新帮助手册 - 小灰笔记 - CSDN博客





2017年04月09日 12:46:11[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1781








       接触了一段时间的PowerShell还是觉得这种shell设计的似乎偏复杂一些，学习上手感觉特别困难。几天的空余时间学习了一下，到现在还是没有掌握到它的经典以及妙处。

       关于帮助手册，这个是我们平时可以依赖的一个帮手。能够保证自己手中的帮助手册是最新的版本也是很有必要的。

       在管理员模式下运行PowerShell，输入Update-help即可启动帮助手册的升级。如果电脑已经连网，会有以下提示：



 正在更新模块Microsoft.PowerShell.Utility 的帮助

    正在查找帮助内容...

   [oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo]

       如果已经下载了帮助手册的升级包，这个命令也可以通过path参数进行离线升级。如果运行结束，命令的执行不会提示什么信息：

PSC:\Windows\system32> Update-help

PSC:\Windows\system32>





