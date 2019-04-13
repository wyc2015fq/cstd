
# Python中更改Tor身份 - leofionn的博客 - CSDN博客


2019年01月08日 15:12:47[leofionn](https://me.csdn.net/qq_36142114)阅读数：108个人分类：[安全																](https://blog.csdn.net/qq_36142114/article/category/7692885)


[https://stackoverflow.com/questions/9887505/how-to-change-tor-identity-in-python](https://stackoverflow.com/questions/9887505/how-to-change-tor-identity-in-python)
[https://www.cnblogs.com/nuolan/p/5729650.html](https://www.cnblogs.com/nuolan/p/5729650.html)
import urllib2
from TorCtl import TorCtl
proxy_support = urllib2.ProxyHandler({"http" : "127.0.0.1:8118"})
opener = urllib2.build_opener(proxy_support) 
def newId():
 conn = TorCtl.connect(controlAddr="127.0.0.1", controlPort=9051, passphrase="your_password")
 conn.send_signal("NEWNYM")
for i in range(0, 10):
 print "case "+str(i+1)
 newId()
 proxy_support = urllib2.ProxyHandler({"http" : "127.0.0.1:8118"})
 urllib2.install_opener(opener)
 print(urllib2.urlopen("http://www.ifconfig.me/ip").read())

