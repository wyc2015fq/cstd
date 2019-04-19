# python post get请求 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月29日 13:58:20[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：141
安装 Requests
pip install requests
**import**requests
requests.get**(**'https://github.com/timeline.json'**)**

使用 params关键字参数，以一个json字符串字典来提供这些参数。举例来说，如果你想传递 key1=value1 和 key2=value2 到 httpbin.org/get ，那么你可以使用如下代码：
>>>
payload=**{**'key1'**:**'value1'**,**'key2'**:**'value2'**}**>>>
r=requests.get**(**"http://httpbin.org/get"**,**params=payload**)**
requests.post
读取响应
r.text
二进制的用r.content
json: r.json**()**如果 JSON 解码失败， r.json() 就会抛出一个异常
要检查请求是否成功，请使用 r.raise_for_status() 或者检查 r.status_code(响应状态码) 是否和你的期望相同
>>>
r.status_code==requests.codes.okTrue
如果发送了一个错误请求(一个 4XX 客户端错误，或者 5XX 服务器错误响应)，我们可以通过[**Response.raise_for_status()**](http://docs.python-requests.org/zh_CN/latest/api.html#requests.Response.raise_for_status) 来抛出异常
遇到网络问题（如：DNS 查询失败、拒绝连接等）时，Requests 会抛出一个 **ConnectionError** 异常。
如果 HTTP 请求返回了不成功的状态码， [**Response.raise_for_status()**](http://docs.python-requests.org/zh_CN/latest/api.html#requests.Response.raise_for_status) 会抛出一个 **HTTPError**异常。
若请求超时，则抛出一个 **Timeout** 异常。
若请求超过了设定的最大重定向次数，则会抛出一个 **TooManyRedirects** 异常。
所有Requests显式抛出的异常都继承自 **requests.exceptions.RequestException** 。
[http://docs.python-requests.org/zh_CN/latest/user/quickstart.html](http://docs.python-requests.org/zh_CN/latest/user/quickstart.html)
[https://stackoverflow.com/questions/11322430/how-to-send-post-request](https://stackoverflow.com/questions/11322430/how-to-send-post-request)
