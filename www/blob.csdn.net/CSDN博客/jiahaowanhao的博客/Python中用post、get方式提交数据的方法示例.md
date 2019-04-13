
# Python中用post、get方式提交数据的方法示例 - jiahaowanhao的博客 - CSDN博客


2018年06月19日 21:25:11[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：149


[Python中用post、get方式提交数据的方法示例](http://cda.pinggu.org/view/25865.html)
最近在使用Python的过程中，发现网上很少提到在使用post方式时，怎么传一个数组作为参数的示例，此处根据自己的实践经验，给出相关示例，下面话不多说了，来一起跟着小编学习学习吧。
示例如下：
单纯的post请求：
def http_post():
url = "http://152.1.12.11:8080/web"
postdata = dict(d=2, p=10)
post = []
post.append(postdata)
req = urllib2.Request(url, json.dumps(post)) \#需要是json格式的参数
req.add_header('Content-Type', 'application/json') \#要非常注意这行代码的写法
response = urllib2.urlopen(req)
result = json.loads(response.read())
print result
需要token时写法如下：
def http_post():
url = "http://152.1.12.11:8080/web"
postdata = dict(d=2, p=10)
post = []
post.append(postdata)
req = urllib2.Request(url, json.dumps(post))
access_token = 'eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJ1bmlxdWVfbmFtZSI6I..........'
req.add_header('Authorization', access_token) \#header中添加token
req.add_header('Content-Type', 'application/json') \#要非常注意这行代码的写法
response = urllib2.urlopen(req)
result = json.loads(response.read())
print result
get方式的写法如下：
def get_access_token():
local_url = 'http://152.1.1.1:8080/web'
response = urllib2.urlopen(local_url).read()
resp = json.loads(response)
print resp
总结
以上就是这篇文章的全部内容了，希望本文的内容对大家的学习或者工作能带来一定的帮助

