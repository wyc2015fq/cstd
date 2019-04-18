# 从form的enctype属性到Content-Type再到request.getInputStream() - z69183787的专栏 - CSDN博客
2016年08月15日 12:54:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2700
还是一个异常引发的故事：
需要实现一个手机客户端行为分析的需求，手机客户端需要上传一些数据文件。手机客户端通过http协议post方式上传数据文件的时候，我们发现在服务器端无法通过request.getInputStream()获取到相应的数据，调用request.getInputStream()读取数据里头啥都木有。
经过各种尝试我们注意到如下的情况：
1.在不做任何修改的情况下，调用request.getParameter()可以获取到部分数据，即url拼接参数的数据。
2.在不做任何修改的情况下，如果在之前任何地方都不调用request.getParameter()，而是直接调用request.getInputStream()是可以获取到数据的。
3.原来客户端使用默认的请求头Content-Type:application/x-www-form-urlencoded，修改此值为multipart/form-data或者application/octet-stream之后，通过request.getInputStream()可以获取到数据，即使之前调用过request.getParameter()。
最后的解决方法当然是客户端修改Content-Type，因为服务器端request.getInputStream()的方法不方便修改。今天具体分析下里头的原理。
一.从form的enctype属性到Content-Type
写html的时候我们都知道form有个属性enctype，默认值是application/x-www-form-urlencoded，这个值表示会将表单数据用&符号做一个简单的拼接。例如：
[plain][view
 plain](http://blog.csdn.net/shootyou/article/details/7182004#)[copy](http://blog.csdn.net/shootyou/article/details/7182004#)[print](http://blog.csdn.net/shootyou/article/details/7182004#)[?](http://blog.csdn.net/shootyou/article/details/7182004#)
- POST /post_test.php HTTP/1.1   
- Accept-Language: zh-CN  
- User-Agent: Mozilla/4.0   
- Content-Type: application/x-www-form-urlencoded   
- Host: 192.168.12.102  
- Content-Length: 42  
- Connection: Keep-Alive  
- Cache-Control: no-cache  
- 
- title=test&content=%B3%AC%BC%B6%C5%AE%C9%FA&submit=post+article  
我们注意到这个时候Content-Type为application/x-www-form-urlencoded。
如果enctype的值为multipart/form-data，这个值一般用于表单中包含文件上传的情况，它会将表单中的数据使用一个boundary作为分隔上传。例如：
[plain][view
 plain](http://blog.csdn.net/shootyou/article/details/7182004#)[copy](http://blog.csdn.net/shootyou/article/details/7182004#)[print](http://blog.csdn.net/shootyou/article/details/7182004#)[?](http://blog.csdn.net/shootyou/article/details/7182004#)
- POST /post_test.php?t=1 HTTP/1.1  
- Accept-Language: zh-CN  
- User-Agent: Mozilla/4.0    
- Content-Type: multipart/form-data; boundary=---------------------------7dbf514701e8  
- Accept-Encoding: gzip, deflate  
- Host: 192.168.12.102  
- Content-Length: 345  
- Connection: Keep-Alive  
- Cache-Control: no-cache  
- 
- -----------------------------7dbf514701e8  
- Content-Disposition: form-data; name="title"  
- test  
- -----------------------------7dbf514701e8  
- Content-Disposition: form-data; name="content"  
- ....  
- -----------------------------7dbf514701e8  
- Content-Disposition: form-data; name="submit"  
- post article  
- -----------------------------7dbf514701e8--  
我们注意到这个时候Content-Type也相应的变为multipart/form-data，同时后面还加上了分隔符boundary的描述。
所以，其实form的enctype属性某种程度上决定了Content-Type值和请求body里头的数据格式。
详细的可以参考：http://imzc.net/archives/131
二.从Content-Type到request.getInputStream()
上面说到了form的处理情况，但是其实如果我们不是使用浏览器，而是自己实现的客户端来传递数据的话，这些头信息就得都由自己处理。
所以上面就出现了，上传文件的时候仍然使用了application/x-www-form-urlencoded的不标准用法。
但是为啥Content-Type会影响request的处理呢？这得从request的一些实现说起。
 request.getParameter()、 request.getInputStream()、request.getReader()这三种方法是有冲突的，因为流只能被读一次。
 比如： 当form表单内容采用 enctype=application/x-www-form-urlencoded编码时，先通过调用request.getParameter() 方法得到参数后，再调用 request.getInputStream()或request.getReader()已经得不到流中的内容，因为在调用 request.getParameter()时系统可能对表单中提交的数 据以流的形式读了一次,反之亦然。 
当form表单内容采用 enctype=multipart/form-data编码时，即使先调用request.getParameter()也得不到数据，但是这时调用 request.getParameter()方法对 request.getInputStream()或request.getReader()没有冲突，即使已经调用了 request.getParameter()方法也 可以通过调用request.getInputStream()或request.getReader()得 到表单中的数据,而request.getInputStream()和request.getReader()在同
 一个响应中是不能混合使用的,如果混合使用就会抛异常。
更多也可以参考：http://robert-liu.iteye.com/blog/713568
来源于：[http://blog.csdn.net/shootyou/article/details/7182004](http://blog.csdn.net/shootyou/article/details/7182004)
