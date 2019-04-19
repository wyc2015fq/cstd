# iOS HTTP详解 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月07日 09:38:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：258

# 一、简介
- 在介绍HTTP之前，我们先来了解一下URL
> 
**1、URL**
- 
什么是URL
- URL的全称是UniformResource Locator（`统一资源定位符`）
- URL的基本格式 =协议://主机地址/路径- 协议：不同的协议，代表着不同的资源查找方式、资源传输方式
- 主机地址：存放资源的主机（服务器）的IP地址（域名）
- 资源在主机（服务器）中的具体位置
- 
URL中常见的协议
- HTTP ： 超文本传输协议， 格式是http://
- file ：访问的是本地计算机上的资源，格式是file://（不用加主机地址）
- mailto：访问的是电子邮件地址，格式是mailto:
- FTP：访问的是共享主机的文件资源，格式是ftp://
> 
**2、Http**
- 不管是移动客户端还是PC端，访问远程的网络资源经常使用HTTP协议
- 
Http -> `超文本传输协议`
- Http协议即超文本传输协议(Hypertext Transfer Protocol )，是Web联网的基础，也是手机联网常用的协议之一，`http协议是在网络开发中最常用的协议`
- `Http协议是建立在TCP协议之上的一种应用`。由于HTTP在每次请求结束后都会主动释放连接，因此HTTP连接是一种“短连接”，要保持客户端程序的在线状态，需要不断地向服务器发起连接请求。
- Http 是应用层协议，TCP 是传输层协议（位于应用层之下）。
- 
访问的是远程的网络资源，格式是http://
- 
Http协议的作用
- Http的全称是HypertextTransfer Protocol，超文本传输协议
- 规定客户端和服务器之间的数据传输格式
- 让客户端和服务器能有效地进行数据沟通
![](http://upload-images.jianshu.io/upload_images/831339-6f2d0af6c05df9be.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Http.png
- 
完整的http通信过程，可以分为2大步骤
- 请求 ：客户端向服务器索要数据
- 响应：服务器返回客户端相应的数据
- 
发送Http请求的方法
- 在HTTP/1.1协议中，定义了8种发送http请求的方法
- 最常用的是`GET和POST`（实际上GET和POST都能办到增删改查）
> 
3、GET和POST对比
- 主要区别表现在数据传递上
- 
GET：
- 在请求URL后面以`?`的形式跟上发给服务器的参数，多个参数之间`用&隔开`
- 由于浏览器和服务器对URL长度有限制，因此在URL后面附带的参数是有限制的，通常不能超过1KB
- 
POST：
- 发给服务器的参数全部放在`请求体`中
- 理论上，POST传递的数据量没有限制
> 
GET和POST的选择
- 如果要传递大量数据，比如文件上传，只能用`POST`请求
- `GET`的安全性比`POST`要差些，如果包含机密\敏感信息，建议用`POST`
p如果仅仅是索取数据（数据查询），建议使用`GET`
p如果是增加、修改、删除数据，建议使用`POST`
# 二、iOS中发送Http请求的方案
> 
苹果原生（自带）
- `NSURLConnection`：用法简单，最古老最经典最直接的一种方案【坑比较多】
- `NSURLSession`：功能比NSURLConnection更加强大，苹果目前比较推荐使用这种技术【2013推出，iOS7开始出的技术】
- CFNetwork：NSURL*的底层，纯C语言
> 
第三方框架
- ASIHttpRequest：外号“HTTP终结者”，功能极其强大，可惜早已停止更新
- `AFNetworking`：简单易用，提供了基本够用的常用功能，维护和使用者多
- MKNetworkKit：简单易用，产自三哥的故乡印度，维护和使用者少
# 三、Http通信过程
![](http://upload-images.jianshu.io/upload_images/831339-8d8020b2d0f5590f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Http通信过程.png
> 
1、请求
- 
HTTP协议规定：1个完整的由客户端发给服务器的HTTP请求中包含以下内容 ： 请求头、请求体
- 
`请求头`：包含了对客户端的环境描述、客户端请求信息等
```cpp
```cpp
GET /minion.png HTTP/1.1   //包含了请求方法、请求资源路径、HTTP协议版本
Host: 120.25.226.186:32812   //客户端想访问的服务器主机地址
User-Agent: Mozilla/5.0  //客户端的类型，客户端的软件环境
Accept: text/html,*/*   //客户端所能接收的数据类型
Accept-Language: zh-cn  //客户端的语言环境
Accept-Encoding: gzip  //客户端支持的数据压缩格式
```
```
- 
`请求体`：客户端发给服务器的具体数据，比如文件数据(POST请求才会有)
> 
**2、响应**
- 
客户端向服务器发送请求，服务器应当做出响应，即返回数据给客户端
- 
HTTP协议规定：1个完整的HTTP响应中包含以下内容
- `响应头`: 包含了对服务器的描述、对返回数据的描述
```cpp
```cpp
HTTP/1.1 200 OK            //包含了HTTP协议版本、状态码、状态英文名称
Server:Apache-Coyote/1.1   //服务器的类型
Content-Type: image/jpeg  //返回数据的类型
Content-Length: 56811  //返回数据的长度
Date: Mon, 23 Jun2014 12:54:52 GMT  //响应的时间
```
```
- `响应体`：服务器返回给客户端的具体数据，比如文件数据
