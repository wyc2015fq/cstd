# Python进阶_wxpy学习：聊天对象 - 去向前方的博客 - CSDN博客





2018年07月09日 16:38:41[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：1339








# 目录：



- [目录：](#目录)
- [前言](#前言)
- [聊天对象](#聊天对象)
- [各类型的继承关系](#各类型的继承关系)- [基础类](#基础类)
- [实际类](#实际类)

- [基本聊天对象](#基本聊天对象)
- [公众号](#公众号)




# 前言

学习完了python的基本概念和相应操作后，就要进入实战阶段了，首先选的和微信端口自动交互的wxpy。 

本文操靠wxpy的官方文档：[官方文档](http://wxpy.readthedocs.io/zh/latest/bot.html#id2)

机器人对象可以参考前文：[机器人对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80957704)
# 聊天对象

通过机器人对象 Bot 的 chats(), friends()，groups(), mps() 方法, 可分别获取到当前机器人的 所有聊天对象、好友、群聊，以及公众号列表。 

而获得到的聊天对象合集 Chats 和 Groups 具有一些合集方法，例如：Chats.search() 可用于按条件搜索聊天对象:

```
from wxpy import *
bot = Bot()
my_friend = bot.friends().search('bb', sex=MALE, city='深圳')[0]
# <Friend: bb>
在找到好友(或其他聊天对象)后，还可使用该聊天对象的 send 系列方法，对其发送消息:
# 发送文本
my_friend.send('Hello, WeChat!')
# 发送图片
my_friend.send_image('my_picture.png')
# 发送视频
my_friend.send_video('my_video.mov')
# 发送文件
my_friend.send_file('my_file.zip')
# 以动态的方式发送图片
my_friend.send('@img@my_picture.png')
```

# 各类型的继承关系

在继续了解各个聊天对象之前，我们需要首先 理解各种不同类型聊天对象的继承关系

## 基础类

所有聊天对象，均继承于以下两种基础类，并拥有相应的属性和方法。 

基本聊天对象 Chat•所有的聊天对象均继承于此类型 

•拥有 微信ID、昵称 等属性 

•拥有 发送消息 Chat.send(), 获取头像 Chat.get_avatar() 等方法 

单个聊天对象 User•继承于 Chat，表示个体聊天对象 (而非群聊)。 

•被以下聊天对象所继承◦好友对象 Friend 

◦群成员对象 Member 

◦公众号对象 MP
•拥有 性别、省份、城市、是否为好友 等属性 

•拥有 加为好友 User.add(), 接受为好友 User.accept() 等方法

## 实际类

在实际使用过程中，我们会更多的用到以下实际聊天对象类型。 

小技巧

请牢记，除了自身私有的属性和方法外，它们还拥有对应基础类的属性和方法 (未重复列出)。 

•好友 Friend 

•群聊 Group 

•群成员 Member 

•公众号 MP
注解

阅读以下内容，你将了解: 

   •  如何获取他们的各种属性 (ID、昵称、性别、地区、是否为好友关系等) 

   •  如何对他们进行发送消息、加为好友、加入群聊、下载头像 等操作
# 基本聊天对象

所有聊天对象都继承于”基本聊天对象”，并拥有相应的属性和方法。 

class wxpy.Chat(raw, bot)[源代码] 

单个用户 (User) 和群聊 (Group) 的基础类 

bot 

所属的 机器人对象 

raw 

原始数据 

puid 

持续有效，且稳定唯一的聊天对象/用户ID，适用于持久保存 

请使用 Bot.enable_puid() 来启用 puid 属性
小技巧

puid 是 wxpy 特有的聊天对象/用户ID 

不同于其他 ID 属性，puid 可始终被获取到，且具有稳定的唯一性 

注意**puid 映射数据 不可跨机器人使用**

nick_name 

该聊天对象的昵称 (好友、群员的昵称，或群名称) 

name 

该聊天对象的友好名称 

具体为: 从 备注名称、群聊显示名称、昵称(或群名称)，或微信号中 

按序选取第一个可用的
```
send(content=None, media_id=None)[源代码]
动态发送不同类型的消息，具体类型取决于 msg 的前缀。

参数:
•content – ◦由 前缀 和 内容 两个部分组成，若 省略前缀，将作为纯文本消息发送
◦前缀 部分可为: ‘@fil@’, ‘@img@’, ‘@msg@’, ‘@vid@’ (不含引号)
◦分别表示: 文件，图片，纯文本，视频
◦内容 部分可为: 文件、图片、视频的路径，或纯文本的内容

•media_id – 填写后可省略上传过程


返回类型:

wxpy.SentMessage
```

send_msg(msg=None)

发送文本消息 

参数: 

msg – 文本内容 
返回类型: 

wxpy.SentMessage  

send_image(path, media_id=None) 

发送图片 

参数: 

•path – 文件路径 

•media_id – 设置后可省略上传
返回类型: 

wxpy.SentMessage

send_file(path, media_id=None)[源代码] 

发送文件 

参数: 

•path – 文件路径 

•media_id – 设置后可省略上传 

返回类型: 

wxpy.SentMessage
send_video(path=None, media_id=None)[源代码] 

发送视频 

参数: 

•path – 文件路径 

•media_id – 设置后可省略上传 

返回类型: 

wxpy.SentMessage 

send_raw_msg(raw_type, raw_content, uri=None, msg_ext=None)[源代码] 

以原始格式发送其他类型的消息。 

参数: 

•raw_type (int) – 原始的整数消息类型 

•raw_content (str) – 原始的消息内容 

•uri (str) – 请求路径，默认为 ‘/webwxsendmsg’ 

•msg_ext (dict) – 消息的扩展属性 (会被更新到 Msg 键中)
返回类型: 

wxpy.SentMessage

例如，发送好友或公众号的名片:

my_friend.send_raw_msg( 

    # 名片的原始消息类型 

    raw_type=42, 

    # 注意 `username` 在这里应为微信 ID，且被发送的名片必须为自己的好友 

    raw_content=’’ 

) 

mark_as_read()[源代码] 

消除当前聊天对象的未读提示小红点 

pin() 

将聊天对象置顶 

unpin() 

取消聊天对象的置顶状态 

get_avatar(save_path=None) 

获取头像 

参数: 

save_path – 保存路径(后缀通常为.jpg)，若为 None 则返回字节数据  

uin 

微信中的聊天对象ID，固定且唯一 

因微信的隐私策略，该属性有时无法被获取到 

建议使用 puid 作为用户的唯一 ID 

alias 

若用户进行过一次性的 “设置微信号” 操作，则该值为用户设置的”微信号”，固定且唯一 

因微信的隐私策略，该属性有时无法被获取到 

建议使用 puid 作为用户的唯一 ID 

wxid 

聊天对象的微信ID (实际为 .alias 或 .uin) 

因微信的隐私策略，该属性有时无法被获取到 

建议使用 puid 作为用户的唯一 ID 

user_name 

该聊天对象的内部 ID，通常不需要用到 

注意 

同个聊天对象在不同用户中，此 ID 不一致 ，且可能在新会话中 被改变！ 

单个聊天对象 

class wxpy.User(raw, bot)[源代码] 

好友(Friend)、群聊成员(Member)，和公众号(MP) 的基础类 

remark_name 

备注名称 

set_remark_name(remark_name) 

设置或修改好友的备注名称 

参数: 

remark_name – 新的备注名称  

sex 

性别，目前有:
```
# 男性
MALE = 1
# 女性
FEMALE = 2
```

未设置时为 None 

province 

省份 

city 

城市 

signature 

个性签名 

is_friend 

判断当前用户是否为好友关系 

返回: 

若为好友关系，返回对应的好友，否则返回 False  

add(verify_content=”) 

把当前用户加为好友 

参数: 

verify_content – 验证信息(文本)  

accept(verify_content=”) 

接受当前用户为好友 

参数: 

verify_content – 验证信息(文本) 
返回: 

新的好友对象 

返回类型: 

wxpy.Friend 

好友 

class wxpy.Friend(raw, bot) 

好友对象
群聊 

class wxpy.Group(raw, bot) 

群聊对象 

members 

群聊的成员列表 

search(keywords=None, **attributes) 

在群聊中搜索成员 

注解 

搜索结果为一个 Chats (列表) 对象 

建议搭配 ensure_one() 使用 

参数: 

•keywords – 成员名称关键词 

•attributes – 属性键值对 

返回: 

匹配的群聊成员 

返回类型: 

wxpy.Chats 

owner 

返回群主对象 

is_owner 

判断所属 bot 是否为群管理员 

self 

机器人自身 (作为群成员) 

update_group(members_details=False)[源代码] 

更新群聊的信息 

参数: 

members_details – 是否包括群聊成员的详细信息 (地区、性别、签名等)  

add_members(users, use_invitation=False)[源代码] 

向群聊中加入用户 

参数: 

•users – 待加入的用户列表或单个用户 

•use_invitation – 使用发送邀请的方式 

remove_members(members) 

从群聊中移除用户 

参数: 

members – 待移除的用户列表或单个用户  

rename_group(name) 

修改群聊名称 

参数: 

name – 新的名称，超长部分会被截断 (最长32字节)  

群成员 

class wxpy.Member(raw, group) 

群聊成员对象 

display_name 

在群聊中的显示昵称 

remove() 

从群聊中移除该成员 

name 

该群成员的友好名称 

具体为: 从 群聊显示名称、昵称(或群名称)，或微信号中，按序选取第一个可用的 

实用技巧 

判断一位用户是否在群中只需用 in 语句: 

friend = bot.friends().search(‘bb’)[0] 

group = bot.groups().search(‘wxpy 交流群’)[0] 

if friend in group: 

    print(‘是的，{} 在 {} 中！’.format(friend.name, group.name)) 

    # 是的，bb 在 wxpy 交流群 中！ 

若要遍历群成员，可直接对群对象使用 for 语句:
```
# 打印所有群成员
for member in group:
    print(member)


若需查看群成员数量，直接使用 len() 即可:


len(group) # 这个群的成员数量


若需判断一位群成员是否就是某个好友，使用 == 即可:


member = group.search('游否')[0]
if member == friend:
    print('{} is {}'.format(member, friend))
    # <Member: 游否> is <Friend: 游否>
```

# 公众号

class wxpy.MP(raw, bot)[源代码] 

公众号对象 

聊天对象合集 

好友、公众号、群聊成员的合集 

在 Chats 对象中，除了最常用到的 search() 外，还有两个特别的方法，stats() 与 stats_text()，可用来统计好友或群成员的性别和地区分布:
```
bot.friends().stats_text()
# bb 共有 100 位微信好友\n\n男性: 67 (67.0%)\n女性: 23 (23.0%) ...

class wxpy.Chats(chat_list=None, source=None)[源代码]
多个聊天对象的合集，可用于搜索或统计
search(keywords=None, **attributes)[源代码]
在聊天对象合集中进行搜索
```

注解 

搜索结果为一个 Chats (列表) 对象 

建议搭配 ensure_one() 使用 

参数: 

•keywords – 聊天对象的名称关键词 

•attributes – 属性键值对，键可以是 sex(性别), province(省份), city(城市) 等。例如可指定 province=’广东’ 

返回: 

匹配的聊天对象合集 

返回类型: 

wxpy.Chats 

stats(attribs=(‘sex’, ‘province’, ‘city’))[源代码] 

统计各属性的分布情况 

参数: 

attribs – 需统计的属性列表或元组  

返回: 

统计结果  

stats_text(total=True, sex=True, top_provinces=10, top_cities=10) 

简单的统计结果的文本 

参数: 

•total – 总体数量 

•sex – 性别分布 

•top_provinces – 省份分布 

•top_cities – 城市分布 

返回: 

统计结果文本 

add_all(interval=3, verify_content=”)[源代码] 

将合集中的所有用户加为好友，请小心应对调用频率限制！ 

参数: 

•interval – 间隔时间(秒) 

•verify_content – 验证说明文本 

群聊的合集 

class wxpy.Groups(group_list=None)[源代码] 

群聊的合集，可用于按条件搜索 

search(keywords=None, users=None, **attributes)[源代码] 

在群聊合集中，根据给定的条件进行搜索 

参数: 

•keywords – 群聊名称关键词 

•users – 需包含的用户 

•attributes – 属性键值对，键可以是 owner(群主对象), is_owner(自身是否为群主), nick_name(精准名称) 等。 

返回: 

匹配条件的群聊列表 

返回类型: 

wxpy.Groups


















