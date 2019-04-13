
# SAE 搭建微信公众平台 - 阳光岛主 - CSDN博客

2014年01月21日 23:09:47[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：16582


最近在玩微信公众账号，开发者模式需要验证，自己没有公网服务器，于是考虑通过免费的云服务器解决了
下面是通过新浪云SAE进行的微信公众平台开发者模式的验证方法。
**index.wsgi**

```python
# coding: UTF-8
import sae
import os
import web
from weixinInterface import WeixinInterface
 
urls = (
	'/', 'Hello',
    '/weixin','WeixinInterface'
)
 
class Hello:
	def GET(self):
		return ("你好, Sunboy_2050")
 
app = web.application(urls, globals()).wsgifunc()
application = sae.create_wsgi_app(app)
```
**weixinInterface.py**

```python
#coding:UTF-8
import hashlib
import web
class WeixinInterface:
    def GET(self):
        
        data = web.input()		# 获取输入参数
        signature = data.signature
        timestamp = data.timestamp
        nonce = data.nonce
        echostr = data.echostr
        
        token="sunboy_2050"				# 自己的token
        
        list=[token,timestamp,nonce]	# 字典序排序
        list.sort()
        
        sha1=hashlib.sha1()				# sha1加密算法
        map(sha1.update, list)
        hashcode=sha1.hexdigest()
        
        if hashcode == signature:		# 如果是来自微信的请求，则回复echostr
            return echostr				# print "true"
```
**运行结果：**
在浏览器输入网址： http://weixin.ithomer.net
![](https://img-blog.csdn.net/20140121112042234)

**验证微信**
成为微信公众平台开发者，需要进行验证，输入URL和Token
在SAE上，已经配置了Token（sunboy_2050)，URL连接为http://weixin.ithomer.net/weixin
![](https://img-blog.csdn.net/20140121112407359)
点击“提交”按钮，自动进行验证，验证结果如下：
![](https://img-blog.csdn.net/20140121112517765)

**搭建的微信公众账号：****ithomer**
![](https://img-my.csdn.net/uploads/201401/26/1390701376_9741.jpg)


**参考推荐：**
[利用SAE搭建微信公众平台](http://www.nilday.com/%E5%88%A9%E7%94%A8sae%E6%90%AD%E5%BB%BA%E5%BE%AE%E4%BF%A1%E5%85%AC%E4%BC%97%E5%B9%B3%E5%8F%B0%EF%BC%88%E4%BA%8C%EF%BC%89%E5%BE%AE%E4%BF%A1%E7%9A%84%E9%AA%8C%E8%AF%81%E5%92%8C%E8%87%AA%E5%8A%A8/)
[SAE Python环境](http://sae.sina.com.cn/?m=devcenter&catId=289)（SAE官方）
[微信开发者接入平台](http://mp.weixin.qq.com/wiki/index.php?title=%E6%8E%A5%E5%85%A5%E6%8C%87%E5%8D%97)（微信官方）


