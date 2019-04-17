# Python也能聊微信 - zhusongziye的博客 - CSDN博客





2018年11月25日 12:54:48[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：131








最近研究微信API，发现个非常好用的python库：wxpy。wxpy基于itchat，使用了 Web 微信的通讯协议，实现了微信登录、收发消息、搜索好友、数据统计等功能。这里我们就来介绍一下这个库，并在最后实现一个聊天机器人。有没有很兴奋？有没有很期待？好了，接下来，开始我们的正题。

## **准备工作**

安装非常简单，从官方源下载安装
`pip install -U wxpy`
或者从豆瓣源安装
`pip install -U wxpy -i "https://pypi.doubanio.com/simple/"`
## **模块初探**

安装完成后，我们来试下几个基本的功能

1. 扫码登录微信

```
from wxpy import *

bot = Bot()
```

运行上面的程序，会弹出二维码，用手机微信扫一扫即可实现登录。

但上面的程序有一个缺点，每次运行都要扫二维码。不过wxpy非常贴心地提供了缓存的选项，如下
`bot = Bot(cache_path=True)`
这样把登录信息保存下来，就不用每次都扫二维码了。



2. 发送消息
`bot.file_helper.send("hello")`
这里的file_helper就是微信的文件传输助手，我们给文件传输助手发送一条消息，可以在手机端收到如下的消息

![](https://img-blog.csdnimg.cn/20181125124742916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



3. 接受消息

我们实现一个收到消息自动回复的功能。

```
@bot.register()
def print_message(msg):     
   print(msg.text)     
   return msg.text

# 进入Python命令行，让程序保持运行
embed()
```

强哥打开自己的公众号管理平台，在后端向自己发送消息，就可以收到如下的消息回复

![](https://img-blog.csdnimg.cn/20181125124839481.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



4. 搜索好友及微信群

我们来实现一个搜索公司群、定位老板并转发老板消息的功能

```
from wxpy import *

bot = Bot(cache_path=True)

# 定位公司群
company_group = bot.groups().search('公司微信群')[0]

# 定位老板

boss = company_group.search('老板大名')[0]

# 将老板的消息转发到文件传输助手
@bot.register(company_group)
def forward_boss_message(msg):     
   if msg.member == boss:         
       msg.forward(bot.file_helper, prefix='老板发言')

# 堵塞线程
embed()
```

对于老板喜欢在群里面喊话的同学来说，这是一个福音，以后再也不用担心错失老板的重要信息了~~

**数据统计**

wxpy的好友统计功能非常好用，可以很方便地统计好友的地理位置分布和性别分布。

下面的代码中，强哥统计了下自己的好友的分布情况，并打印出人数最多的10个地区。

```
from wxpy import *
bot = Bot(cache_path=True)
friends_stat = bot.friends().stats()

friend_loc = [] # 每一个元素是一个二元列表，分别存储地区和人数信息
for province, count in friends_stat["province"].iteritems():
   if province != "":
       friend_loc.append([province, count])

# 对人数倒序排序
friend_loc.sort(key=lambda x: x[1], reverse=True)

# 打印人数最多的10个地区
for item in friend_loc[:10]:
   print item[0], item[1]
```

统计出来的地区分布数据画成图表如下

![](https://img-blog.csdnimg.cn/20181125125122993.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

强哥身在上海，大多数的好友也来自上海，上面的图表也如实地反应了这一情况。统计性别分布的代码如下

```
for sex, count in friends_stat["sex"].iteritems():
   # 1代表MALE, 2代表FEMALE
   if sex == 1:
       print "MALE %d" % count
   elif sex == 2:
       print "FEMALE %d" % count
```

把性别分布的数据生成图表如下

![](https://img-blog.csdnimg.cn/20181125125220237.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

可以看到好友中男性占了大多数。男性朋友多多，老婆放心多多，嗯~~

**聊天机器人**

有了上面功能简介部分的基础，我们来实现一个聊天机器人。

聊天机器人基于图灵机器人。图灵机器人可以在图灵机器人－中文语境下智能度最高的机器人大脑注册账号，并创建机器人。

```
# -*- coding: utf-8 -*-
import json
import requests
from wxpy import *

# 调用图灵机器人API，发送消息并获得机器人的回复
def auto_reply(text):     
   url = "http://www.tuling123.com/openapi/api"
   api_key = "你的api key"
   payload = {
       "key": api_key,
       "info": text,
       "userid": "123456"
   }
   r = requests.post(url, data=json.dumps(payload))     
   result = json.loads(r.content)     
   return "[tuling] " + result["text"]

bot = Bot(console_qr=True, cache_path=True)

@bot.register(mp)
def forward_message(msg):     
   return auto_reply(msg.text)

embed()
```

运行上面的程序，发送消息给自己，可以看到如下的对话

![](https://img-blog.csdnimg.cn/20181125125259235.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

机器人太逗了，上来就要红包，还把我当男朋友了~

**附录**

wxpy文档：wxpy: 用 Python 玩微信

github地址：youfou/wxpy



