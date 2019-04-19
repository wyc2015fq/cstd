# python itchat实现微信自动回复 - yixianfeng41的专栏 - CSDN博客
2017年02月16日 22:00:32[第2梦](https://me.csdn.net/yixianfeng41)阅读数：14012
今天在实验楼发现一个特别好玩的，python 微信库itchat,可以实现自动回复等多种功能，好玩到根本停不下来啊，尤其是调戏调戏不懂计算机的，特别有成就感，哈哈！！
代码如下：
```python
#coding=utf8
import requests
import itchat
KEY = '8edce3ce905a4c1dbb965e6b35c3834d'
def get_response(msg):
    apiUrl = 'http://www.tuling123.com/openapi/api'
    data = {
        'key'    : KEY,
        'info'   : msg,
        'userid' : 'wechat-robot',
    }
    try:
        r = requests.post(apiUrl, data=data).json()
        return r.get('text')
    except:
        return
@itchat.msg_register(itchat.content.TEXT)
def tuling_reply(msg):
    defaultReply = 'I received: ' + msg['Text']
    reply = get_response(msg['Text'])
    return reply or defaultReply
itchat.auto_login(hotReload=True)
itchat.run()
```
安装一下 itchat即可跑上面程序，实现与图灵机器人的交互。
更多关于itchat的资料，如下： 
[itchat官网](https://pypi.python.org/pypi/itchat/1.0.11)
[Python微信库:itchat](http://www.tuicool.com/articles/VJZRRfn)
