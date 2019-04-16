# Python进阶_wxpy学习：原始数据 - 去向前方的博客 - CSDN博客





2018年07月15日 08:46:55[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：284








# 目录：



- [目录：](#目录)
- [前言](#前言)
- [itchat 与原始数据](#itchat-与原始数据)
- [使用 itchat 的原接口](#使用-itchat-的原接口)
- [使用原始数据](#使用原始数据)




# 前言

学习完了python的基本概念和相应操作后，就要进入实战阶段了，首先选的是和微信端口自动交互的wxpy。 

本文操靠wxpy的官方文档：[官方文档](http://wxpy.readthedocs.io/zh/latest/bot.html#id2)

机器人对象可以参考前文：[机器人对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80957704)

聊天对象可以参考前文：[聊天对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80973533)

消息处理可以参考前文：[消息处理](https://blog.csdn.net/lvsehaiyang1993/article/details/80980411)

微信监控可以参考前文：[微信监控](https://blog.csdn.net/lvsehaiyang1993/article/details/81002467)

探索调试可以参考前文：[探索调试](https://blog.csdn.net/lvsehaiyang1993/article/details/81009728)

实用组件可以参考前文：[实用组件](https://blog.csdn.net/lvsehaiyang1993/article/details/81026695)

异常处理可以参考前文：[异常处理](https://blog.csdn.net/lvsehaiyang1993/article/details/81043811)
# itchat 与原始数据

正是得益于 itchat 的坚实基础，wxpy 才能够在短时间内快速实现这些新的接口和功能。

感谢 itchat 维护者们的辛勤付出。

以下为如何在 wxpy 中混合使用 itchat 的原接口和原始数据。

# 使用 itchat 的原接口

只需在 wxpy 的 Bot 对象后紧跟 .core.* 即可调用 itchat 的原接口。

例如，使用 itchat 的 search_friends 接口:

```python
from wxpy import *
bot = Bot()
found = bot.core.search_friends('游否')
```

注意

通过 itchat 原接口所获取到的结果为原始数据，可能无法直接传递到 wxpy 的对应方法中。

# 使用原始数据

wxpy 的所有 聊天对象 和 消息对象 均基于从 itchat 获取到的数据进行封装。若需使用原始数据，只需在对象后紧跟 .raw。

例如，查看一个 好友 对象的原始数据:

```python
from wxpy import *
bot = Bot()
a_friend = bot.friends()[0]
print(a_friend.raw)
```




