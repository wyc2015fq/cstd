
# python 百度翻译官方api和破解版方法 - 追求卓越,做到专业 - CSDN博客


2019年03月07日 10:38:46[Waldenz](https://me.csdn.net/enter89)阅读数：37


百度翻译开放平台，收费标准：
若当月翻译字符数≤2百万，当月免费；若超过2百万字符，按照49元/百万字符支付当月全部翻译字符数费用
![](https://img-blog.csdnimg.cn/20190307104734767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
**官方python版调用代码**
```python
# -*- coding: utf-8 -*- 
# 百度翻译平台官方 python版代码， 使用python 2.7 
import httplib
import md5
import urllib
import random
import json
import datetime 
import sys
print(sys.getdefaultencoding())
reload(sys)
sys.setdefaultencoding('utf-8')
print(sys.getdefaultencoding())
appid = 'yourid_yourid' #你的appid
secretKey = 'yoursecret_yoursecret' #你的密钥
myurl = '/api/trans/vip/translate'
def trans_baidu(fromlan,tolan,text):
    httpClient = None
    q = text.encode("utf-8")
    fromLang = fromlan
    toLang = tolan
    salt = random.randint(32768, 65536)
    sign = appid+q+str(salt)+secretKey
    m1 = md5.new()
    m1.update(sign)
    sign = m1.hexdigest()
    the_myurl = myurl+'?appid='+appid+'&q='+urllib.quote(q)+'&from='+fromLang+'&to='+toLang+'&salt='+str(salt)+'&sign='+sign
    
    try:
        httpClient = httplib.HTTPConnection('api.fanyi.baidu.com')
        httpClient.request('GET', the_myurl)
    
        #response是HTTPResponse对象
        response = httpClient.getresponse()
        res=response.read()
        print res
        json_data=json.loads(res)
        ret = json_data["trans_result"][0]["dst"]
        #print ret
        return ret
    except Exception, e:
        print e
        return str(e)
    finally:
        if httpClient:
            httpClient.close()
```
主要将 其中appid 和 secretKeyf赋值为申请百度翻译开放平台中的APP ID 和秘钥。
官方代码是python2.7。 将其转换为python3
以下为 换换过程中的 python2.7和python3的区别：
httplib在python3中替换为了 http.client
```python
# python 2.7
import httplib
httplib.HTTPConnection("api.fanyi.baidu.com")
```
```python
# python3
import http.client
http.client.HTTPConnection("api.fanyi.baidu.com")
```
md5在python3中替换为 hashlib.md5
```python
# python 2.7
import md5
m1 = md5.new()
m1.update(stringstring)
```
```python
# python 3
import hashlib
sign=hashlib.md5(sign.encode()).hexdigest()
```
urllib在python3中的不同
```python
# python 2.7
import urllib
urlib.quote(query_text)
```
```python
# python 3
import urllib
urllib.parse.quote(query_text)
```
Python3 完整代码：
```python
import http.client
import hashlib
import urllib
import random
import json
import datetime 
import sys
appid = 'yourid_yourid' #你的appid
secretKey = 'yoursecret_yoursecret' #你的密钥
myurl = '/api/trans/vip/translate'
def trans_baidu(fromlan,tolan,text):
    httpClient = None
    q = text
    fromLang = fromlan
    toLang = tolan
    salt = random.randint(32768, 65536)
    sign = appid+q+str(salt)+secretKey
    sign = hashlib.md5(sign.encode()).hexdigest()
    the_myurl = myurl+'?appid='+appid+'&q='+urllib.parse.quote(q)+'&from='+fromLang+'&to='+toLang+'&salt='+str(salt)+'&sign='+sign
    
    try:
        httpClient = http.client.HTTPConnection('api.fanyi.baidu.com')
        httpClient.request('GET', the_myurl)
    
        #response是HTTPResponse对象
        response = httpClient.getresponse()
        res=response.read()
        print (res)
        json_data=json.loads(res)
        ret = json_data["trans_result"][0]["dst"]
        #print ret
        return ret
    except Exception as e:
        print (e)
        return str(e)
    finally:
        if httpClient:
            httpClient.close()
```
篇幅有点长，破解版的源码另起一篇：[https://blog.csdn.net/enter89/article/details/88289785](https://blog.csdn.net/enter89/article/details/88289785)

