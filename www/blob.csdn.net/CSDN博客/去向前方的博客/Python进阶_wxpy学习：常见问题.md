# Python进阶_wxpy学习：常见问题 - 去向前方的博客 - CSDN博客





2018年07月16日 10:20:07[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：1568








# 目录：



- [目录：](#目录)
- [前言](#前言)
- [必看: 常见问题 FAQ](#必看-常见问题-faq)
- [提示](#提示)
- [每次登陆都要扫码？](#每次登陆都要扫码)
- [小技巧](#小技巧)
- [为什么要开发 wxpy？](#为什么要开发-wxpy)




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

原始数据可以参考前文：[原始数据](https://blog.csdn.net/lvsehaiyang1993/article/details/81050034)
# 必看: 常见问题 FAQ

# 提示

这里罗列了一些常见的问题，在提出新的问题前，请先看完本文。 

启动后马上退出了？

因为主线程执行完成了，程序自然会退出。

只需在代码结尾加一句 embed() 即可堵塞线程，还能进入 Python 命令行:

```
from wxpy import *

# 你的其他代码...

# 堵塞线程，并进入 Python 命令行
embed()

或者，也可以使用 Bot.join() 仅仅堵塞线程:

bot = Bot()

# 你的其他代码...

# 仅仅堵塞线程
bot.join()

# 机器人登出后会继续往下执行
```

# 每次登陆都要扫码？

可启用登陆状态缓存功能，在短时间内重新运行程序，可自动登录。

具体请见 Bot 中的 cache_path 参数说明。 

可以在 Linux 中使用吗？

wxpy 不依赖于图形界面，因此完全兼容各种纯终端的服务器。

但有一点需要注意，在纯终端环境中，登陆时必须使用”终端二维码”参数。

具体请见 Bot 中的 console_qr 参数说明。

# 小技巧

遇到以下错误？请使用 Bot 的 console_qr 参数。

FileNotFoundError: [Errno 2] No such file or directory: ‘xdg-open’

支持 红包、转账、朋友圈… 吗？

wxpy 使用了 Web 微信的通讯协议，因此仅能覆盖 Web 微信本身所具备的功能。

所以以下功能目前 均不支持

```
支付相关 - 红包、转账、收款 等都不支持
在群聊中@他人 - 是的，Web 微信中被人@后也不会提醒
发送名片 - 但可以通过 send_raw_msg() 转发
发送分享链接 - 也无法转发
发送语音消息
朋友圈相关
```

# 为什么要开发 wxpy？

wxpy 的初衷是帮助人们利用微信来使生活和工作更轻松。

注解

希望每位使用者在使用机器人时

```
维护良好的交流环境
永远不骚扰他人
遵守法律和平台规则
```




