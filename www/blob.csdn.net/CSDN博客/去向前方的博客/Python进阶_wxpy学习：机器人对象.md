# Python进阶_wxpy学习：机器人对象 - 去向前方的博客 - CSDN博客





2018年07月08日 11:12:24[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：768








# 目录：



- [目录：](#目录)
- [前言](#前言)
- [机器人对象](#机器人对象)
- [初始化/登陆](#初始化登陆)
- [搜索聊天对象](#搜索聊天对象)
- [搜索好友:](#搜索好友)
- [搜索群聊:](#搜索群聊)
- [在群聊中搜素:](#在群聊中搜素)
- [搜索任何类型的聊天对象 (但不包含群内成员)](#搜索任何类型的聊天对象-但不包含群内成员)
- [加好友和建群](#加好友和建群)
- [自动接受好友请求:](#自动接受好友请求)
- [其他](#其他)
- [控制多个微信 (多开)](#控制多个微信-多开)




# 前言

学习完了python的基本概念和相应操作后，就要进入实战阶段了，首先选的和微信端口自动交互的wxpy。 

本文操靠wxpy的官方文档：[官方文档](http://wxpy.readthedocs.io/zh/latest/bot.html#id2)

# 机器人对象

机器人 Bot 对象可被理解为一个 Web 微信客户端。

> 
解关于发送消息，请参见 聊天对象。  

   关于消息对象和自动处理，请参见 消息处理。


# 初始化/登陆

> 
注解Bot 在初始化时便会执行登陆操作，需要手机扫描登陆。


主类

```
class wxpy.Bot(cache_path=None, console_qr=False, qr_path=None, qr_callback=None, login_callback=None, logout_callback=None)

    机器人对象，用于登陆和操作微信账号，涵盖大部分 Web 微信的功能:

    from wxpy import *
    bot = Bot()

    # 机器人账号自身
    myself = bot.self

    # 向文件传输助手发送消息
    bot.file_helper.send('Hello from wxpy!')

    参数: 

        cache_path –
            设置当前会话的缓存路径，并开启缓存功能；为 None (默认) 则不开启缓存功能。
            开启缓存后可在短时间内避免重复扫码，缓存失效时会重新要求登陆。
            设为 True 时，使用默认的缓存路径 ‘wxpy.pkl’。
        console_qr –
            在终端中显示登陆二维码，需要安装 pillow 模块 (pip3 install pillow)。
            可为整数(int)，表示二维码单元格的宽度，通常为 2 (当被设为 True 时，也将在内部当作 2)。
            也可为负数，表示以反色显示二维码，适用于浅底深字的命令行界面。
            例如: 在大部分 Linux 系统中可设为 True 或 2，而在 macOS Terminal 的默认白底配色中，应设为 -2。
        qr_path – 保存二维码的路径
        qr_callback – 获得二维码后的回调，可以用来定义二维码的处理方式，接收参数: uuid, status, qrcode
        login_callback – 登陆成功后的回调，若不指定，将进行清屏操作，并删除二维码文件
        logout_callback – 登出时的回调
```

```
Bot.enable_puid(path='wxpy_puid.pkl')

    可选操作: 启用聊天对象的 puid 属性:

    # 启用 puid 属性，并指定 puid 所需的映射数据保存/载入路径
    bot.enable_puid('wxpy_puid.pkl')

    # 指定一个好友
    my_friend = bot.friends().search('qkl')[0]

    # 查看他的 puid
    print(my_friend.puid)
    # 'edfe8468'

    小技巧
    puid 是 wxpy 特有的聊天对象/用户ID
    不同于其他 ID 属性，puid 可始终被获取到，且具有稳定的唯一性
    参数: path – puid 所需的映射数据保存/载入路径
```

“` 

Bot.auto_mark_as_read

```
为 True 时，将自动消除手机端的新消息小红点提醒 (默认为 False)
```

```
#获取聊天对象
```

Bot.self

```
机器人自身 (作为一个聊天对象)

若需要给自己发送消息，请先进行以下一次性操作:

# 在 Web 微信中把自己加为好友
bot.self.add()
bot.self.accept()

# 发送消息给自己
bot.self.send('能收到吗？')
```

```
```
Bot.file_helper

    文件传输助手
```

“` 

Bot.friends(update=False)[源代码]

```
获取所有好友
参数: update – 是否更新
返回: 聊天对象合集
返回类型:   wxpy.Chats
```

```
```
Bot.groups(update=False, contact_only=False)

    获取所有群聊对象

    一些不活跃的群可能无法被获取到，可通过在群内发言，或修改群名称的方式来激活
    参数: 

        update – 是否更新
        contact_only – 是否限于保存为联系人的群聊

    返回: 

    群聊合集
    返回类型:   

    wxpy.Groups
```

```
Bot.mps(update=False)

    获取所有公众号
    参数: update – 是否更新
    返回: 聊天对象合集
    返回类型:   wxpy.Chats
```

```
Bot.chats(update=False)

    获取所有聊天对象
    参数: update – 是否更新
    返回: 聊天对象合集
    返回类型:   wxpy.Chats
```

# 搜索聊天对象

注解

```
通过 .search() 获得的搜索结果 均为列表
若希望找到唯一结果，可使用 ensure_one()
```

# 搜索好友:

```
# 搜索名称包含 'keil' 的深圳男性好友
found = bot.friends().search('keil', sex=MALE, city='深圳')
# [<Friend: keil>]
# 确保搜索结果是唯一的，并取出唯一结果
keil = ensure_one(found)
# <Friend: keil>
```

# 搜索群聊:

```
# 搜索名称包含 'wxpy'，且成员中包含 `keil` 的群聊对象
wxpy_groups = bot.groups().search('wxpy', [keil])
# [<Group: wxpy 交流群 1>, <Group: wxpy 交流群 2>]
```

# 在群聊中搜素:

```
# 在刚刚找到的第一个群中搜索
group = wxpy_groups[0]
# 搜索该群中所有浙江的群友
found = group.search(province='浙江')
# [<Member: 浙江群友 1>, <Group: 浙江群友 2>, <Group: 浙江群友 3> ...]
```

# 搜索任何类型的聊天对象 (但不包含群内成员)

```
# 搜索名称含有 'wxpy' 的任何聊天对象
found = bot.search('wxpy')
# [<Friend: wxpy 机器人>, <Group: wxpy 交流群 1>, <Group: wxpy 交流群 2>]
```

# 加好友和建群

```
Bot.add_friend(user, verify_content='')

    添加用户为好友
    参数: 

        user – 用户对象，或 user_name
        verify_content – 验证说明信息

Bot.add_mp(user)


    添加/关注 公众号
    参数: user – 公众号对象，或 user_name

Bot.accept_friend(user, verify_content='')

    接受用户为好友
    参数: 

        user – 用户对象或 user_name
        verify_content – 验证说明信息

    返回: 

    新的好友对象
    返回类型:   

    wxpy.Friend
```

# 自动接受好友请求:

```python
# 注册好友请求类消息
@bot.register(msg_types=FRIENDS)
# 自动接受验证信息中包含 'wxpy' 的好友请求
def auto_accept_friends(msg):
    # 判断好友请求中的验证文本
    if 'wxpy' in msg.text.lower():
        # 接受好友 (msg.card 为该请求的用户对象)
        new_friend = bot.accept_friend(msg.card)
        # 或 new_friend = msg.card.accept()
        # 向新的好友发送消息
        new_friend.send('哈哈，我自动接受了你的好友请求')

Bot.create_group(users, topic=None)

    创建一个新的群聊
    参数: 

        users – 用户列表 (不含自己，至少 2 位)
        topic – 群名称

    返回: 

    若建群成功，返回一个新的群聊对象
    返回类型:   

    wxpy.Group
```

# 其他

```
Bot.user_details(user_or_users, chunk_size=50)

    获取单个或批量获取多个用户的详细信息(地区、性别、签名等)，但不可用于群聊成员
    参数: 

        user_or_users – 单个或多个用户对象或 user_name
        chunk_size – 分配请求时的单批数量，目前为 50

    返回: 

    单个或多个用户用户的详细信息

Bot.upload_file(path)
    上传文件，并获取 media_id
    可用于重复发送图片、表情、视频，和文件
    参数: path – 文件路径
    返回: media_id
    返回类型:   str

Bot.join()

    堵塞进程，直到结束消息监听 (例如，机器人被登出时)

Bot.logout()

    登出当前账号
```

# 控制多个微信 (多开)

仅需初始化多个 Bot 对象，即可同时控制多个微信:

bot1 = Bot() 

bot2 = Bot()



