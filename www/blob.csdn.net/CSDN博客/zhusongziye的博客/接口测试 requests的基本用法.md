# 接口测试 requests的基本用法 - zhusongziye的博客 - CSDN博客





2018年04月17日 20:27:14[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：439








requests的官方文档：`http://cn.python-requests.org/zh_CN/latest/`

requests是一个处理http请求的第三方库，所以，要了解requests的用法，需要先了解http协议，了解http协议的方法请出门右转。
看一下requests支持的web特性，根据运用过程中的需要去查看requests的官方文档或者搜索吧。

### requests支持的web特性
- 
Keep-Alive & 连接池

- 
国际化域名和 URL

- 
带持久 Cookie 的会话

- 
浏览器式的 SSL 认证

- 
自动内容解码

- 
基本/摘要式的身份认证

- 
优雅的 key/value Cookie

- 
自动解压

- 
Unicode 响应体

- 
HTTP(S) 代理支持

- 
文件分块上传

- 
流下载

- 
连接超时

- 
分块请求

- 
支持 .netrc


### requests发送请求

使用requests发送网络请求非常简单，发送请求方式与HTTP类型相关。
示例接口地址：`https://developer.github.com/v3/``http://httpbin.org/`

#### GET请求

###### 1、不传递参数的get请求

```
import requests

r = requests.get('http://httpbin.org/get')
print(r.status_code)
```

###### 2、传递参数的get请求

```
import requests

payload = {'key1': 'hello', 'key2': 'world'}
r = requests.get('http://httpbin.org/get', params=payload)
print(r.url)
print(r.text)
```




![](https://img-blog.csdn.net/20180417202631935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




#### POST请求

    post向服务器提交数据有4中常见的方式：

> 
`1 application/x-www-form-urlencoded`

`2 multipart/form-data`

`3 application/json`

`4 text/xml`






提交数据的方式在headers的`Content-Type`中进行设定。

###### 1、提交json格式的数据

```
import requests

headers = {"Content-Type": "application/json"}

payload = {'key1': 'hello', 'key2': 'world'}

r = requests.post('http://httpbin.org/post', 
    data=payload, headers=headers)
    
print(r.url)

print(r.text)
```

![](https://img-blog.csdn.net/20180417202646880?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

image.png

###### 2、POST上传文件

```
import requests

files = {'file': open('interface_url.xlsx', 'rb')}
r = requests.post('http://httpbin.org/post', files=files)
print(r.text)
```

![](https://img-blog.csdn.net/20180417202701279?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### PUT\DELETE\PATCH请求

请求格式与post一样。

### requests中请求的响应类型
- 
r.text：响应内容的格式与r.encoding的编码有关

- 
r.content:二进制响应内容

- 
r.json():json格式的响应内容

- 
r.raw:原始响应内容，需要在发送请求时，设置stream=True


```
import requests

username = 'catleer'url = 'https://api.github.com'path = '/users/' + username

r = requests.get(url+path, stream=True)
print(r.url)
print(r.status_code)
print(r.encoding)
print("响应内容：", r.text)
print("二进制响应内容：", r.content)
print("json格式的响应内容：", r.json())
print("原始响应内容：", r.raw)
print(r.raw.read(100))with open("11.txt", 'wb') as fd:
    c = 1
    for chunk in r.iter_content(100):
        fd.write(chunk)
        c = c + 1
    print(c)
```



