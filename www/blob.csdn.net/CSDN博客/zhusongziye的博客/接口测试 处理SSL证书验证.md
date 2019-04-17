# 接口测试 处理SSL证书验证 - zhusongziye的博客 - CSDN博客





2018年04月26日 21:50:18[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1230








### requests支持的证书验证方式
- 
SSL证书验证

- 
客户端证书验证

- 
CA证书验证


### SSL证书验证




> 
来个大写的问题，SSL是什么，为什么在发起HTTPS请求的时候需要证书验证？




- 
SSL:Secure Sockets Layer,安全套接层。是为了解决HTTP协议是明文，避免传输的数据被窃取、篡改、劫持等。




- 
TSL：Transport Layer Security，传输层安全协议。TSL其实是SSL标准化后的产物，即SSL/TSL实际上是表示同一个东西。




- 
HTTPS：HTTPS是兼容HTTP的，可以把HTTPS理解为‘HTTP over TSL’，即HTTPS是HTTP协议和TSL协议的组合。




- 
HTTPS在传输数据时，同样会先建立TCP连接，建立起TCP连接之后，会建立TSL连接，这个过程可以通过wireshark抓包查看。




![](https://img-blog.csdn.net/2018042621490338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

TSL.png




- 
事实上，SSL协议分为2层，包括SSL记录协议和SSL握手协议，同时，SSL协议还包括加密算法的一些概念，嗯，太复杂了，我也编不下去了，，希望在后面的学习中，能把前面浏览的知识总结起来。




- 
还有一个问题是，SSL协议处于网络7层结构的哪一层呢？有的资料直接说明SSL协议处于传输层，有的资料则说SSL协议处于传输层与应用层之间。


#### Requests 为 HTTPS 请求验证 SSL 证书



- 
Requests 可以为 HTTPS 请求验证 SSL 证书，就像 web 浏览器一样。SSL 验证默认是开启的，如果证书验证失败，Requests 会抛出 SSLError:


```
>>> import requests>>> requests.get('https://github.com')
requests.exceptions.SSLError: HTTPSConnectionPool(host='github.com', port=443): Max retries exceeded with url: / (Caused by SSLError(SSLError(1, '[SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed (_ssl.c:777)'),))
```
- 
要能成功验证的方式如下，（为什么没有示例？嗯，我构造不出来）：




![](https://img-blog.csdn.net/20180426214929130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

image.png

- 
通常，我们遇到requests的ssl验证，都是直接跳过不验证的，将`verify=False`即可。





```
>>> requests.get('https://github.com', verify=False)
D:\Program Files\Python36\lib\site-packages\urllib3\connectionpool.py:858: InsecureRequestWarning: Unverified HTTPS request is being made. Adding certificate
verification is strongly advised. See: https://urllib3.readthedocs.io/en/latest/advanced-usage.html#ssl-warnings
  InsecureRequestWarning)
<Response [200]>
```



可以看到，将verify设为False后，可以跳过ssl验证，但存在一个告警信息`InsecureRequestWarning`。要忽略这个告警信息可以怎么做呢？




```
import requests
from requests.packages import urllib3

# 关闭告警信息
urllib3.disable_warnings()

url = 'https://github.com'r = requests.get(url, verify=False)
print(r.status_code)
```

### 客户端证书验证方式

![](https://img-blog.csdn.net/20180426214948332?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


image.png

### 总结
- 
https请求进行ssl验证或忽略ssl验证才能请求成功，忽略方式为`verify=False`

- 
SSL证书是由CA机构颁发的，嗯，所以安全也是要钱的

- 
要完全理解HTTP协议，不能只到分辨HTTP的get、post等动作的程度，还要去理解7层网络结构中每层使用的协议，以及数据传输的过程




