# Python调用webservice接口 - _天枢 - 博客园
## [Python调用webservice接口](https://www.cnblogs.com/yhleng/p/6756660.html)
2017-04-24 14:17 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=6756660)
python调用webservice接口，这里我们使用suds包
如果没有直接cmd命令安装subs包 
pip install suds
安装成功会有提示成功字样。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
首先导入包
先查看一下webservice提供哪些类型及方法，之后我们才知道怎么调用，传哪些参数。
```python;gutter
from suds.client import Client
from suds.transport.https import HttpAuthenticated #webservice需要安全难证
t = HttpAuthenticated(username='RFCATUSER',password='q123456') #安全验证所需用户，密码
url = "http://hqxt1.sinopec.com:50000/dir/wsdl?p=sa/65bd52bd76de381daf15736a37d13ab1"
test = Client(url,transport=t)
print test
```
![](https://images2015.cnblogs.com/blog/1149221/201704/1149221-20170424140931475-2009703895.png)
下面开始调用传参：
```python;gutter
#coding=utf-8
#导入包
from suds.client import Client
from suds.transport.https import HttpAuthenticated #webservice需要安全难证
 
#安全验证，获取service服务
t = HttpAuthenticated(username='RFCATUSER',password='q123456') #安全验证所需用户，密码
url = "http://hqxt1.sinopec.com:50000/dir/wsdl?p=sa/65bd52bd76de381daf15736a37d13ab1"
test = Client(url,transport=t)
print test
 
#用test.factory.create创建实例
request=test.factory.create('DT_IPM_SEND_PSBUDGET_REQ')
IPM_MSG_HEAD = test.factory.create('MSG_HEAD')
request.IS_MSG_HEAD = IPM_MSG_HEAD
IPM_MSG_HEAD.SENDTIME = '20170421102908'
IPM_MSG_HEAD.RECIVER = 'ECC'
IPM_MSG_HEAD.SENDER = 'IPM'
IPM_MSG_HEAD.INTERFACE_ID = 'PROJECT_CREATE'
IPM_MSG_HEAD.SPRAS = None
IPM_MSG_HEAD.OPERATOR = None
IPM_MSG_HEAD.SYSTEM_ID = None
IPM_MSG_HEAD.PROXY_ID = None
IPM_MSG_HEAD.GUID = None
IPM_MSG_HEAD.MANDT = None
request.IV_BUKRS = "X060"
request.IV_TESTRUN = "X"
request.IT_PSBUDGET=[]
#用test.service.XXXXX去调用方法
m=test.service.SI_IPM_SEND_PSBUDGET_REQ_A_OUT(request)
print IPM_MSG_HEAD
print request
```

