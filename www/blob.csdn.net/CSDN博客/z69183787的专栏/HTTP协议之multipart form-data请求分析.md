# HTTP协议之multipart/form-data请求分析 - z69183787的专栏 - CSDN博客
2015年07月17日 09:45:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3323
首先来了解什么是multipart/form-data请求：
根据[*http*/*1.1* rfc 2616](http://www.faqs.org/rfcs/rfc2616.html)的协议规定，我们的请求方式只有OPTIONS、GET、HEAD、POST、PUT、DELETE、TRACE等，那为为何我们还会有multipart/form-data请求之说呢？这就要从头来说了。
http协议大家都知道是规定了以ASCII码传输，建立在tcp、ip协议之上的应用层规范，规范内容把http请求分为3个部门：状态行，请求头，请求体。所有的方法、实现都是围绕如何运用和组织这三部分来完成的。换句话来说就是万变不离其中，只要我们了解了http请求的组成部分后，自然就可以应变任何实际工作中的需求和问题了。
关于状态行，请求头，请求体等三部分的具体内容，大家可以参考官方的协议文档http://www.faqs.org/rfcs/rfc2616.html，这里主要分析multipart/form-data请求具体是怎么一回事。
既然http协议本身的原始方法不支持multipart/form-data请求，那这个请求自然就是由这些原始的方法演变而来的，具体如何演变且看下文：
1、multipart/form-data的基础方法是post，也就是说是由post方法来组合实现的
2、multipart/form-data与post方法的不同之处：请求头，请求体。
3、multipart/form-data的请求头必须包含一个特殊的头信息：Content-Type，且其值也必须规定为multipart/form-data，同时还需要规定一个内容分割符用于分割请求体中的多个post的内容，如文件内容和文本内容自然需要分割开来，不然接收方就无法正常解析和还原这个文件了。具体的头信息如下：
**[html]**[view
 plain](http://blog.csdn.net/five3/article/details/7181521#)[copy](http://blog.csdn.net/five3/article/details/7181521#)
- Content-Type: multipart/form-data; boundary=${bound}      
//其中${bound} 是一个占位符，代表我们规定的分割符，可以自己任意规定，但为了避免和正常文本重复了，尽量要使用复杂一点的内容。如：--------------------56423498738365
4、multipart/form-data的请求体也是一个字符串，不过和post的请求体不同的是它的构造方式，post是简单的name=value值连接，而multipart/form-data则是添加了分隔符等内容的构造体。具体格式如下:
**[html]**[view
 plain](http://blog.csdn.net/five3/article/details/7181521#)[copy](http://blog.csdn.net/five3/article/details/7181521#)
- --${bound}  
- Content-Disposition: form-data; name="Filename"
- 
- HTTP.pdf  
- --${bound}  
- Content-Disposition: form-data; name="file000"; filename="HTTP协议详解.pdf"
- Content-Type: application/octet-stream  
- 
- %PDF-1.5  
- file content  
- %%EOF  
- 
- --${bound}  
- Content-Disposition: form-data; name="Upload"
- 
- Submit Query  
- --${bound}--  
其中${bound}为之前头信息中的分割符，如果头信息中规定为123，那么这里也要为123,；可以很容易看出，这个请求体是多个相同的部分组成的：每一个部分都是以--加分隔符开始的，然后是该部分内容的描述信息，然后一个回车，然后是描述信息的具体内容；如果传送的内容是一个文件的话，那么还会包含文件名信息，以及文件内容的类型。上面的第二个小部分其实是一个文件体的结构，最后会以--分割符--结尾，表示请求体结束。
综上，可以知道要发送一个multipart/form-data的请求，其实任何支持post请求的工具或语言都可以支持，只是自己要稍微包装一下便可。
**参考资料：**
百度百科： http://baike.baidu.com/view/9472.htm  
http1.1协议规范： http://www.faqs.org/rfcs/rfc2616.html
分析工具：httpAnalyzer
