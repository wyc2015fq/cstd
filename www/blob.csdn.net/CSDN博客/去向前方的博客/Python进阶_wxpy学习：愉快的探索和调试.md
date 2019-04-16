# Python进阶_wxpy学习：愉快的探索和调试 - 去向前方的博客 - CSDN博客





2018年07月12日 08:53:47[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：314








# 目录：



- [目录：](#目录)
- [前言](#前言)
- [愉快的探索和调试](#愉快的探索和调试)
- [使用 wxpy 命令](#使用-wxpy-命令)
- [初始化一个名为 bot 的机器人:](#初始化一个名为-bot-的机器人)




# 前言

学习完了python的基本概念和相应操作后，就要进入实战阶段了，首先选的和微信端口自动交互的wxpy。 

本文操靠wxpy的官方文档：[官方文档](http://wxpy.readthedocs.io/zh/latest/bot.html#id2)

机器人对象可以参考前文：[机器人对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80957704)

聊天对象可以参考前文：[聊天对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80973533)

消息处理可以参考前文：[消息处理](https://blog.csdn.net/lvsehaiyang1993/article/details/80980411)

微信监控可以参考前文：[微信监控](https://blog.csdn.net/lvsehaiyang1993/article/details/81002467)
# 愉快的探索和调试

想要做点小试验，调试代码，或是探索 wxpy 的功能特性？反复修改和运行太麻烦。

试试下面两种玩法，告别涂涂改改的摸索方式。

使用 embed()

注解

适用于在现有的代码中进行探索和调试

只需将 embed() 放在代码中的任何位置。运行后，就可以从那儿开始探索和调试。

例如，初始化一个机器人，然后看看它能做些什么:

```python
from wxpy import *
bot = Bot()
embed() # 进入 Python 命令行

# 输入对象名称并回车
>>> bot
# Out[1]: <Bot: 游否>
>>> bot.friends()
# Out[2]: [<Friend: 路人甲>, <Friend: 路人乙>, <Friend: 路人丙>]

wxpy.embed(local=None, banner='', shell=None)[源代码]

进入交互式的 Python 命令行界面，并堵塞当前线程

支持使用 ipython, bpython 以及原生 python





参数:
•shell (str) – 

指定命令行类型，可设为 ‘ipython’，’bpython’，’python’，或它们的首字母；

若为 None，则按上述优先级进入首个可用的 Python 命令行。

•local (dict) – 设定本地变量环境，若为 None，则获取进入之前的变量环境。
•banner (str) – 设定欢迎内容，将在进入命令行后展示。
```

# 使用 wxpy 命令

注解

适用于在命令行中边写边探索

第二种情况：想要简单写几行，而不想创建脚本，那么使用 wxpy 命令行边写边探索，更方便。

在命令行中输入 wxpy -h 可快速查看使用说明。

选项 

•bot1 bot2 bot3…◦一个或多个需要初始化的机器人对象的名称，以空格分割 

◦默认：不初始化机器人 

◦例子: bot1 bot2
•-c / –cache◦使用会话缓存功能，将创建 wxpy_*.pkl 缓存文件 

◦默认：不缓存会话 

◦例子：-c
•-q 宽度 / –console_qr 宽度◦终端二维码的单元格宽度 

◦默认：不使用终端二维码 

◦例子：-q 2
•-l 等级 / –logging_level 等级 (注意是小写 L，不是 I)◦日志等级 

◦默认：INFO 

◦例子：-l DEBUG
•-s 交互界面 / –shell 交互界面◦选择所需使用的 Python 交互界面 

◦可为：ipython，bpython，python，或它们的首字母 

◦默认：以上首个可用的 Python 命令行 

◦例子：-s bpython
•-v / –version◦展示版本信息并退出z 

◦例子：-v

例子

# 初始化一个名为 bot 的机器人:

```
wxpy bot


在此基础上，使用终端二维码，且单元格宽度为 2:


wxpy bot -q 2


分别初始化名为 bot1 和 bot2 的两个机器人:


wxpy bot1 bot2


在此基础上，使用会话缓存功能:


wxpy bot1 bot2 -c


在此基础上，指定使用 bpython:


wxpy bot1 bot2 -c -s bpython
```









