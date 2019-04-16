# Python进阶_wxpy学习：实用组件 - 去向前方的博客 - CSDN博客





2018年07月13日 10:01:09[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：514








# 目录：



- [目录：](#目录)
- [前言](#前言)
- [实用组件](#实用组件)
- [聊天机器人](#聊天机器人)- [图灵](#图灵)
- [小 i](#小-i)

- [查找共同好友](#查找共同好友)
- [忽略 ResponseError 异常](#忽略-responseerror-异常)




# 前言

学习完了python的基本概念和相应操作后，就要进入实战阶段了，首先选的是和微信端口自动交互的wxpy。 

本文操靠wxpy的官方文档：[官方文档](http://wxpy.readthedocs.io/zh/latest/bot.html#id2)

机器人对象可以参考前文：[机器人对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80957704)

聊天对象可以参考前文：[聊天对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80973533)

消息处理可以参考前文：[消息处理](https://blog.csdn.net/lvsehaiyang1993/article/details/80980411)

微信监控可以参考前文：[微信监控](https://blog.csdn.net/lvsehaiyang1993/article/details/81002467)

探索调试可以参考前文：[探索调试](https://blog.csdn.net/lvsehaiyang1993/article/details/81009728)
# 实用组件

额外内置了一些实用的小组件，可按需使用。 

需要使用直接调用就行。

# 聊天机器人

目前提供了以下两种自动聊天机器人接口。

## 图灵

```
class wxpy.Tuling(api_key=None)
与 wxpy 深度整合的图灵机器人


内置的 api key 存在调用限制，建议自行申请。

免费申请: http://www.tuling123.com/





参数:
api_key – 你申请的 api key 


bot = Bot()
my_friend = ensure_one(bot.search('游否'))
tuling = Tuling(api_key='你申请的 API KEY')

# 使用图灵机器人自动与指定好友聊天
@bot.register(my_friend)
def reply_my_friend(msg):
    tuling.do_reply(msg)

do_reply(msg, at_member=True)
回复消息，并返回答复文本





参数:
•msg – Message 对象
•at_member – 若消息来自群聊，回复时 @发消息的群成员


返回:

答复文本


返回类型:

str

reply_text(msg, at_member=True)
仅返回消息的答复文本





参数:
•msg – Message 对象
•at_member – 若消息来自群聊，回复时 @发消息的群成员


返回:

答复文本


返回类型:

str
```

## 小 i

```
class wxpy.XiaoI(key, secret)
与 wxpy 深度整合的小 i 机器人


需要通过注册获得 key 和 secret

免费申请: http://cloud.xiaoi.com/





参数:
•key – 你申请的 key
•secret – 你申请的 secret



bot = Bot()
my_friend = ensure_one(bot.search('寒风'))
xiaoi = XiaoI('你申请的 Key', '你申请的 Secret')

# 使用小 i 机器人自动与指定好友聊天
@bot.register(my_friend)
def reply_my_friend(msg):
    xiaoi.do_reply(msg)

do_reply(msg)[源代码]
回复消息，并返回答复文本





参数:
msg – Message 对象 

返回:
答复文本 
reply_text(msg)[源代码]
仅返回答复文本





参数:
msg – Message 对象，或消息文本 

返回:
答复文本
```

# 查找共同好友

```python
wxpy.mutual_friends(*args)
找到多个微信用户的共同好友





参数:
args – 每个参数为一个微信用户的机器人(Bot)，或是聊天对象合集(Chats) 

返回:
共同好友列表 

返回类型:
wxpy.Chats 


bot1 = Bot()
bot2 = Bot()

# 打印共同好友
for mf in mutual_friends(bot, bot2):
    print(mf)



确保查找结果的唯一性
wxpy.ensure_one(found)
确保列表中仅有一个项，并返回这个项，否则抛出 ValueError 异常

通常可用在查找聊天对象时，确保查找结果的唯一性，并直接获取唯一项





参数:
found – 列表 

返回:
唯一项 


bot = Bot()
# 确保只找到了一个叫"游否"的好友，并返回这个好友
my_friend = ensure_one(bot.search('游否'))
# <Friend: 游否>



在多个群中同步消息
wxpy.sync_message_in_groups(msg, groups, prefix=None, suffix=None, raise_for_unsupported=False, run_async=True)[源代码]
将消息同步到多个微信群中
支持以下消息类型
•文本 (TEXT)


•视频（VIDEO)


•文件 (ATTACHMENT)


•图片/自定义表情 (PICTURE)


◦但不支持表情商店中的表情


•名片 (CARD)


◦仅支持公众号名片，以及自己发出的个人号名片


•分享 (SHARING)


◦会被转化为 标题 + 链接 形式的纯文本


•语音 (RECORDING)


◦会以文件方式发送


•地图 (MAP)


◦会转化为 位置名称 + 地图链接 形式的文本消息






参数:
•msg (Message) – 需同步的消息对象
•groups (Group) – 需同步的群列表
•prefix (str) – ◦转发时的 前缀 文本，原消息为文本时会自动换行
◦若不设定，则使用默认前缀作为提示

•suffix (str) – ◦转发时的 后缀 文本，原消息为文本时会自动换行
◦默认为空

•raise_for_unsupported (bool) – 

为 True 时，将为不支持的消息类型抛出 NotImplementedError 异常

•run_async (bool) – 是否异步执行，为 True 时不堵塞线程



my_groups = [group1, group2, group3 ...]

@bot.register(my_groups, except_self=False)
def sync_my_groups(msg):
    sync_message_in_groups(msg, my_groups)



检测频率限制
wxpy.detect_freq_limit(func, *args, **kwargs)
检测各类 Web 微信操作的频率限制，获得限制次数和周期





参数:
•func – 需要执行的操作函数
•args – 操作函数的位置参数
•kwargs – 操作函数的命名参数


返回:

限制次数, 限制周期(秒数)


例如，测试发送文本消息的频率限制:


bot = Bot('test.pkl')

# 定义需要检测的操作
def action():
    bot.file_helper.send()

# 执行检测
result = detect_freq_limit(action)
# 查看结果
print(result)
# (120, 120.111222333)
```

# 忽略 ResponseError 异常

```
wxpy.dont_raise_response_error(func)
装饰器：用于避免被装饰的函数在运行过程中抛出 ResponseError 错误
```




