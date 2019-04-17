# 利用itchat 10行python代码实现微信轰炸 - zhusongziye的博客 - CSDN博客





2018年11月11日 15:25:38[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：127








```
'''
转载：https://github.com/Gin79/WeChat-Bomb/blob/master/README.md
'''
```

### 说明
- 
站在巨人的肩膀上

- 
基于python，这个小程序是用python写的

- 
使用的是python第三方库itchat，itchat模块是一位叫`littlecodersh`的大神写的模块，附上大神的github地址,有兴趣的朋友可以去尝试玩一下itchat模块，很有趣的！！！https://github.com/littlecodersh/ItChat


### 准备
- 
Python3.6 需要安装python3.6并配置好环境路径

- 
安装好python后，打开电脑的CMD终端输入`pip install itchat`安装itchat模块

- 
需要设置好默认看图软件


核心代码

import itchatimport timeprint('扫一下弹出来的二维码')
itchat.auto_login(hotReload=True)
boom_remark_name = input('输入你要轰炸的人的微信备注，按回车建继续')
message = input('输入你要轰炸的内容，按回车键开始轰炸')
boom_obj = itchat.search_friends(remarkName=boom_remark_name)[0]['UserName']while True:
    time.sleep(0.5)    print('正在轰炸。。。')
    itchat.send_msg(msg=message, toUserName=boom_obj)
### 代码讲解

`import itchat`

导入itchat模块

`import time`

导入time模块

`itchat.auto_login(hotReload=True)`

登录微信，采用热加载的方式登录网页版的微信，会生成一个itchat.pkl的文件，用于保持登录状态，有点类似于cookie

`boom_remark_name = input('输入你要轰炸的人的微信备注，按回车建继续')` 这里一定要输入你给微信好友的`备注名`，否者无法定位到好友

`message = input('输入你要轰炸的内容，按回车键开始轰炸')`

这里输入要轰炸的内容

`boom_obj = itchat.search_friends(remarkName=boom_remarkname)[0]['UserName']`

这里通过微信好友的`备注名`找到微信好友的信息，再通过`UserName`定位到好友

`while True:` 死循环轰炸 `time.sleep(0.5)`设置睡眠，以免出现消息发送频繁导致不能发送微信消息，此处可自行设置，睡眠时间不能太短 `print('正在轰炸。。。')``itchat.send_msg(msg=message, toUserName=boom_obj)`

### 如何使用
- 
打开CMD终端

- 
输入python

- 
将We.py文件拖入终端

- 
按回车

- 
扫一下弹出来的二维码，确认登录微信

- 
输入你要轰炸的人的微信备注(!!!一定要是微信好友的备注，没有备注请添加备注)

- 
输入要轰炸的内容

- 
按回车键开始轰炸


### 如何退出

按Ctrl + C 退出轰炸



