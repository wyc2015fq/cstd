# Python进阶_wxpy学习：用微信监控你的程序 - 去向前方的博客 - CSDN博客





2018年07月11日 16:14:21[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：808








# 目录：



- [目录：](#目录)
- [前言](#前言)
- [用微信监控你的程序](#用微信监控你的程序)
- [获得专用 Logger](#获得专用-logger)
- [指定接收者](#指定接收者)




# 前言

学习完了python的基本概念和相应操作后，就要进入实战阶段了，首先选的和微信端口自动交互的wxpy。 

本文操靠wxpy的官方文档：[官方文档](http://wxpy.readthedocs.io/zh/latest/bot.html#id2)

机器人对象可以参考前文：[机器人对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80957704)

聊天对象可以参考前文：[聊天对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80973533)

消息处理可以参考前文：[消息处理](https://blog.csdn.net/lvsehaiyang1993/article/details/80980411)
# 用微信监控你的程序

通过利用微信强大的通知能力，我们可以把程序中的警告/日志发到自己的微信上。

wxpy 提供以下两种方式来实现这个需求。

# 获得专用 Logger
`wxpy.get_wechat_logger(receiver=None, name=None, level=30)[源代码]`
获得一个可向指定微信聊天对象发送日志的 Logger

```
参数:
•receiver – ◦当为 None, True 或字符串时，将以该值作为 cache_path 参数启动一个新的机器人，并发送到该机器人的”文件传输助手”
◦当为 机器人 时，将发送到该机器人的”文件传输助手”
◦当为 聊天对象 时，将发送到该聊天对象

•name – Logger 名称
•level – Logger 等级，默认为 logging.WARNING


返回:

Logger
```

```
from wxpy import get_wechat_logger

# 获得一个专用 Logger
# 当不设置 `receiver` 时，会将日志发送到随后扫码登陆的微信的"文件传输助手"
logger = get_wechat_logger()

# 发送警告
logger.warning('这是一条 WARNING 等级的日志，你收到了吗？')

# 接收捕获的异常
try:
    1 / 0
except:
    logger.exception('现在你又收到了什么？')
```

加入到现有的 Logger
`class wxpy.WeChatLoggingHandler(receiver=None)[源代码]`
可向指定微信聊天对象发送日志的 Logging Handler

参数: 

receiver – •当为 None, True 或字符串时，将以该值作为 cache_path 参数启动一个新的机器人，并发送到该机器人的”文件传输助手” 

•当为 机器人 时，将发送到该机器人的”文件传输助手” 

•当为 聊天对象 时，将发送到该聊天对象
```python
import logging
from wxpy import WeChatLoggingHandler

# 这是你现有的 Logger
logger = logging.getLogger(__name__)

# 初始化一个微信 Handler
wechat_handler = WeChatLoggingHandler()
# 加到入现有的 Logger
logger.addHandler(wechat_handler)

logger.warning('你有一条新的告警，请查收。')
```

# 指定接收者

当然，我们也可以使用其他聊天对象来接收日志。

比如，先在微信中建立一个群聊，并在里面加入需要关注这些日志的人员。然后把这个群作为接收者。

```
from wxpy import *

# 初始化机器人
bot = Bot()
# 找到需要接收日志的群 -- `ensure_one()` 用于确保找到的结果是唯一的，避免发错地方
group_receiver = ensure_one(bot.groups().search('XX业务-告警通知'))

# 指定这个群为接收者
logger = get_wechat_logger(group_receiver)

logger.error('打扰大家了，但这是一条重要的错误日志...')
```





