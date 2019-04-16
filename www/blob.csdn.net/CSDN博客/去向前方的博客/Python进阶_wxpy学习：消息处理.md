# Python进阶_wxpy学习：消息处理 - 去向前方的博客 - CSDN博客





2018年07月10日 09:19:12[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：2402








# 目录：



- [目录：](#目录)
- [前言](#前言)
- [消息处理](#消息处理)
- [消息对象](#消息对象)
- [内容数据](#内容数据)
- [用户相关](#用户相关)
- [群聊相关](#群聊相关)
- [回复方法](#回复方法)
- [转发消息](#转发消息)
- [自动处理消息](#自动处理消息)
- [开始运行](#开始运行)
- [示例代码](#示例代码)
- [已发送消息](#已发送消息)
- [历史消息](#历史消息)




# 前言

学习完了python的基本概念和相应操作后，就要进入实战阶段了，首先选的和微信端口自动交互的wxpy。 

本文操靠wxpy的官方文档：[官方文档](http://wxpy.readthedocs.io/zh/latest/bot.html#id2)

机器人对象可以参考前文：[机器人对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80957704)

聊天对象可以参考前文：[聊天对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80973533)
# 消息处理

每当机器人接收到消息时，会自动执行以下两个步骤 

    将消息保存到 Bot.messages 中 

    查找消息预先注册的函数，并执行(若有匹配的函数)
# 消息对象

```
消息对象代表每一条从微信获取到的消息。
基本属性

Message.type

    消息的类型，目前可为以下值:

    # 文本
    TEXT = 'Text'
    # 位置
    MAP = 'Map'
    # 名片
    CARD = 'Card'
    # 提示
    NOTE = 'Note'
    # 分享
    SHARING = 'Sharing'
    # 图片
    PICTURE = 'Picture'
    # 语音
    RECORDING = 'Recording'
    # 文件
    ATTACHMENT = 'Attachment'
    # 视频
    VIDEO = 'Video'
    # 好友请求
    FRIENDS = 'Friends'
    # 系统
    SYSTEM = 'System'

    返回类型:   str

Message.bot

    接收此消息的 机器人对象

Message.id

    消息的唯一 ID (通常为大于 0 的 64 位整型)
```

# 内容数据

```
Message.text

    消息的文本内容

Message.get_file(save_path=None)[源代码]

    下载图片、视频、语音、附件消息中的文件内容。

    可与 Message.file_name 配合使用。
    参数: save_path – 文件的保存路径。若为 None，将直接返回字节数据

Message.file_name

    消息中文件的文件名

Message.file_size

    消息中文件的体积大小

Message.media_id

    文件类消息中的文件资源 ID (但图片视频语音等其他消息中为空)

Message.raw

    原始数据 (dict 数据)
```

# 用户相关

```
Message.chat

    消息所在的聊天会话，即:

        对于自己发送的消息，为消息的接收者
        对于别人发送的消息，为消息的发送者

    返回类型:   wxpy.User, wxpy.Group

Message.sender

    消息的发送者
    返回类型:   wxpy.User, wxpy.Group

Message.receiver

    消息的接收者
    返回类型:   wxpy.User, wxpy.Group

Message.member

        若消息来自群聊，则此属性为消息的实际发送人(具体的群成员)
        若消息来自其他聊天对象(非群聊)，则此属性为 None

    返回类型:   NoneType, wxpy.Member

Message.card

        好友请求中的请求用户
        名片消息中的推荐用户
```

# 群聊相关

```
Message.member

        若消息来自群聊，则此属性为消息的实际发送人(具体的群成员)
        若消息来自其他聊天对象(非群聊)，则此属性为 None

    返回类型:   NoneType, wxpy.Member

Message.is_at

    当消息来自群聊，且被 @ 时，为 True

时间相关

Message.create_time

    服务端发送时间

Message.receive_time

    本地接收时间

Message.latency

    消息的延迟秒数 (发送时间和接收时间的差值)

其他属性

Message.url

    分享类消息中的网页 URL

Message.articles

    公众号推送中的文章列表 (首篇的 标题/地址 与消息中的 text/url 相同)

    其中，每篇文章均有以下属性:

        title: 标题
        summary: 摘要
        url: 文章 URL
        cover: 封面或缩略图 URL

Message.location

    位置消息中的地理位置信息

Message.img_height

    图片高度

Message.img_width

    图片宽度

Message.play_length

    视频长度

Message.voice_length

    语音长度
```

# 回复方法

```
Message.reply(...)

    等同于 Message.chat.send(...)

Message.reply_image(...)

    等同于 Message.chat.send_image(...)

Message.reply_file(...)

    等同于 Message.chat.send_file(...)

Message.reply_video(...)

    等同于 Message.chat.send_video(...)

Message.reply_msg(...)

    等同于 Message.chat.send_msg(...)

Message.reply_raw_msg(...)

    等同于 Message.chat.send_raw_msg(...)
```

# 转发消息

```python
Message.forward(chat, prefix=None, suffix=None, raise_for_unsupported=False)[源代码]

    将本消息转发给其他聊天对象

    支持以下消息类型

            文本 (TEXT)

            视频（VIDEO)

            文件 (ATTACHMENT)

            图片/自定义表情 (PICTURE)

                    但不支持表情商店中的表情

            名片 (CARD)

                    仅支持公众号名片，以及自己发出的个人号名片

            分享 (SHARING)

                    会转化为 标题 + 链接 形式的文本消息

            语音 (RECORDING)

                    会以文件方式发送

            地图 (MAP)

                    会转化为 位置名称 + 地图链接 形式的文本消息

    参数: 

        chat (Chat) – 接收转发消息的聊天对象
        prefix (str) – 转发时增加的 前缀 文本，原消息为文本时会自动换行
        suffix (str) – 转发时增加的 后缀 文本，原消息为文本时会自动换行
        raise_for_unsupported (bool) –
        为 True 时，将为不支持的消息类型抛出 NotImplementedError 异常

    例如，将公司群中的老板消息转发出来:

    from wxpy import *

    bot = Bot()

    # 定位公司群
    company_group = ensure_one(bot.groups().search('公司微信群'))

    # 定位老板
    boss = ensure_one(company_group.search('老板大名'))

    # 将老板的消息转发到文件传输助手
    @bot.register(company_group)
    def forward_boss_message(msg):
        if msg.member == boss:
            msg.forward(bot.file_helper, prefix='老板发言')

    # 堵塞线程
    embed()
```

# 自动处理消息

可通过 预先注册 的方式，实现消息的自动处理。

“预先注册” 是指 

    预先将特定聊天对象的特定类型消息，注册到对应的处理函数，以实现自动回复等功能。

注册消息

提示 

每当收到新消息时，将根据注册规则找到匹配条件的执行函数。 

并将 消息对象 作为唯一参数传入该函数。
将 Bot.register() 作为函数的装饰器，即可完成注册。

```python
# 打印所有*群聊*对象中的*文本*消息
@bot.register(Group, TEXT)
def print_group_msg(msg):
    print(msg)
```

**注意**

优先匹配 后注册 的函数，且仅匹配 一个 注册函数。

```python
Bot.register(chats=None, msg_types=None, except_self=True, run_async=True, enabled=True)[源代码]

    装饰器：用于注册消息配置
    参数: 

        chats – 消息所在的聊天对象：单个或列表形式的多个聊天对象或聊天类型，为空时匹配所有聊天对象
        msg_types – 消息的类型：单个或列表形式的多个消息类型，为空时匹配所有消息类型 (SYSTEM 类消息除外)
        except_self – 排除由自己发送的消息
        run_async – 是否异步执行所配置的函数：可提高响应速度
        enabled – 当前配置的默认开启状态，可事后动态开启或关闭

小技巧

    chats 和 msg_types 参数可以接收一个列表或干脆一个单项。按需使用，方便灵活。
    chats 参数既可以是聊天对象实例，也可以是对象类。当为类时，表示匹配该类型的所有聊天对象。
    在被注册函数中，可以通过直接 return <回复内容> 的方式来回复消息，等同于调用 msg.reply(<回复内容>)。
```

# 开始运行

```python
注解
在完成注册操作后，若没有其他操作，程序会因主线程执行完成而退出。
因此务必堵塞线程以保持监听状态！
wxpy 的 embed() 可在堵塞线程的同时，进入 Python 命令行，方便调试，一举两得。

from wxpy import *

bot = Bot()

@bot.register()
def print_messages(msg):
    print(msg)

# 堵塞线程，并进入 Python 命令行
embed()

wxpy.embed(local=None, banner='', shell=None)[源代码]
    进入交互式的 Python 命令行界面，并堵塞当前线程
    支持使用 ipython, bpython 以及原生 python
    参数: 

        shell (str) –
        指定命令行类型，可设为 ‘ipython’，’bpython’，’python’，或它们的首字母；
        若为 None，则按上述优先级进入首个可用的 Python 命令行。
        local (dict) – 设定本地变量环境，若为 None，则获取进入之前的变量环境。
        banner (str) – 设定欢迎内容，将在进入命令行后展示。
```

# 示例代码

在以下例子中，机器人将

```
忽略 “一个无聊的群” 的所有消息
回复好友 “游否” 和其他群聊中被 @ 的 TEXT 类消息
打印所有其他消息
```

初始化机器人，并找到好友和群聊:

```python
from wxpy import *
bot = Bot()
my_friend = bot.friends().search('游否')[0]
boring_group = bot.groups().search('一个无聊的群')[0]
```

打印所有其他消息:

```python
@bot.register()
def just_print(msg):
    # 打印消息
    print(msg)
```

回复好友”游否”和其他群聊中被 @ 的 TEXT 类消息:

```
@bot.register([my_friend, Group], TEXT)
def auto_reply(msg):
    # 如果是群聊，但没有被 @，则不回复
    if isinstance(msg.chat, Group) and not msg.is_at:
        return
    else:
        # 回复消息内容和类型
        return '收到消息: {} ({})'.format(msg.text, msg.type)

忽略”一个无聊的群”的所有消息:

@bot.register(boring_group)
def ignore(msg):
    # 啥也不做
    return

堵塞线程，并进入 Python 命令行:

embed()

动态开关注册配置

注解

该操作需要在额外的线程中进行！

查看当前的注册配置情况:

bot.registered
# [<MessageConfig: just_print (Async, Enabled)>,
#  <MessageConfig: auto_reply (Async, Enabled)>,
#  <MessageConfig: ignore (Async, Enabled)>]

关闭所有注册配置:

bot.registered.disable()

重新开启 just_print 函数:

bot.registered.enable(just_print)

查看当前开启的注册配置:

bot.registered.enabled
# [<MessageConfig: just_print (Async, Enabled)>]

class wxpy.api.messages.Registered(bot)[源代码]

    保存当前机器人所有已注册的消息配置
    参数: bot – 所属的机器人

    get_config(msg)[源代码]

        获取给定消息的注册配置。每条消息仅匹配一个注册配置，后注册的配置具有更高的匹配优先级。
        参数: msg – 给定的消息
        返回: 匹配的回复配置

    get_config_by_func(func)[源代码]

        通过给定的函数找到对应的注册配置
        参数: func – 给定的函数
        返回: 对应的注册配置

    enable(func=None)[源代码]

        开启指定函数的对应配置。若不指定函数，则开启所有已注册配置。
        参数: func – 指定的函数

    disable(func=None)[源代码]

        关闭指定函数的对应配置。若不指定函数，则关闭所有已注册配置。
        参数: func – 指定的函数

    enabled

        检查处于开启状态的配置
        返回: 处于开启状态的配置

    disabled

        检查处于关闭状态的配置
        返回: 处于关闭状态的配置
```

# 已发送消息

```
class wxpy.SentMessage(attributes)[源代码]

    程序中通过 .send/reply() 系列方法发出的消息

    使用程序发送的消息也将被记录到历史消息 bot.messages 中

    提示

    大部分属性与 Message 相同

    recall()[源代码]

        撤回本条消息 (应为 2 分钟内发出的消息)
```

# 历史消息

可通过访问 bot.messages 来查看历史消息列表。

消息列表为 Messages 对象，具有搜索功能。

例如，搜索所有自己在手机上发出的消息:

```
sent_msgs = bot.messages.search(sender=bot.self)
print(sent_msgs)

class wxpy.Messages(msg_list=None, max_history=200)[源代码]

    多条消息的合集，可用于记录或搜索

    max_history

        设置最大保存条数，即：仅保存最后的 n 条消息。

        bot = Bot()
        # 设置历史消息的最大保存数量为 10000 条
        bot.messages.max_history = 10000

    search(keywords=None, **attributes)[源代码]

        搜索消息记录
        参数: 

            keywords – 文本关键词
            attributes – 属性键值对

        返回: 

        所有匹配的消息
        返回类型:   

        wxpy.Messages

        # 搜索所有自己发送的，文本中包含 'wxpy' 的消息
        bot.messages.search('wxpy', sender=bot.self)
```






