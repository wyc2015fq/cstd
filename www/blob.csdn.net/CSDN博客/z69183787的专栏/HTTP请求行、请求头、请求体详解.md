# HTTP请求行、请求头、请求体详解 - z69183787的专栏 - CSDN博客
2018年12月11日 20:53:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：681
[https://www.jianshu.com/p/eb3e5ec98a66](https://www.jianshu.com/p/eb3e5ec98a66)
[HTTP响应头和请求头信息对照表](https://link.jianshu.com?t=http://tools.jb51.net/table/http_header)
[HTTP请求方法对照表](https://link.jianshu.com?t=http://tools.jb51.net/table/http_request_method)
[HTTP状态码对照表](https://link.jianshu.com?t=http://tools.jb51.net/table/http_status_code)
**HTTP请求报文解剖**
HTTP Request :HTTP请求
Request Line:请求行
Header:请求头
Request Body:请求体
HTTP请求报文由3部分组成（请求行+请求头+请求体）：
![](https://upload-images.jianshu.io/upload_images/1461379-470ef3154ed82331.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/147/format/webp)
image.png
下面是一个实际的请求报文：
![](https://upload-images.jianshu.io/upload_images/1461379-e27e2159621a6020.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/897/format/webp)
image.png
①是请求方法，HTTP/1.1 定义的请求方法有8种：GET、POST、PUT、DELETE、PATCH、HEAD、OPTIONS、TRACE,最常的两种GET和POST，如果是RESTful接口的话一般会用到GET、POST、DELETE、PUT。
②为请求对应的URL地址，它和报文头的Host属性组成完整的请求URL
③是协议名称及版本号。
④是HTTP的报文头，报文头包含若干个属性，格式为“属性名:属性值”，服务端据此获取客户端的信息。
⑤是报文体，它将一个页面表单中的组件值通过param1=value1¶m2=value2的键值对形式编码成一个格式化串，它承载多个请求参数的数据。不但报文体可以传递请求参数，请求URL也可以通过类似于“/chapter15/user.html? param1=value1¶m2=value2”的方式传递请求参数。
对照上面的请求报文，我们把它进一步分解，你可以看到一幅更详细的结构图：
![](https://upload-images.jianshu.io/upload_images/1461379-72ba86f80df4988e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/427/format/webp)
image.png
**HTTP响应报文解剖**
HTTP的响应报文也由三部分组成（响应行+响应头+响应体）：
![](https://upload-images.jianshu.io/upload_images/1461379-b5ec47d31aeac034.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/173/format/webp)
image.png
以下是一个实际的HTTP响应报文：
![](https://upload-images.jianshu.io/upload_images/1461379-96c60fde2c3fc19c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/629/format/webp)
image.png
①报文协议及版本；
②状态码及状态描述；
③响应报文头，也是由多个属性组成；
④响应报文体，即我们真正要的“干货”。
**响应状态码**
和请求报文相比，响应报文多了一个“响应状态码”，它以“清晰明确”的语言告诉客户端本次请求的处理结果。
HTTP的响应状态码由5段组成：
```
1xx 消息，一般是告诉客户端，请求已经收到了，正在处理，别急...
2xx 处理成功，一般表示：请求收悉、我明白你要的、请求已受理、已经处理完成等信息.
3xx 重定向到其它地方。它让客户端再发起一个请求以完成整个处理。
4xx 处理发生错误，责任在客户端，如客户端的请求一个不存在的资源，客户端未被授权，禁止访问等。
5xx 处理发生错误，责任在服务端，如服务端抛出异常，路由出错，HTTP版本不支持等。
```
***200 OK***
你最希望看到的，即处理成功！
***303 See Other***
我把你redirect到其它的页面，目标的URL通过响应报文头的Location告诉你。
```
悟空：师傅给个桃吧，走了一天了[图片上传失败...(image-3001d7-1513152011799)]
唐僧：我哪有桃啊！去王母娘娘那找吧[图片上传失败...(image-a39592-1513152011799)]
```
***304 Not Modified***
告诉客户端，你请求的这个资源至你上次取得后，并没有更改，你直接用你本地的缓存吧，我很忙哦，你能不能少来烦我啊！
***404 Not Found***
你最不希望看到的，即找不到页面。如你在google上找到一个页面，点击这个链接返回404，表示这个页面已经被网站删除了，google那边的记录只是美好的回忆。
***500 Internal Server Error***
看到这个错误，你就应该查查服务端的日志了，肯定抛出了一堆异常，别睡了，起来改BUG去吧！
![](https://upload-images.jianshu.io/upload_images/1461379-189a548b48eea8e3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
文件上传抓包.png
