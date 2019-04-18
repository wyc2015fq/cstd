# Http权威指南读书笔记之Http Web基础（HTTP报文） - weixin_33985507的博客 - CSDN博客
2016年08月07日 16:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：19
`原创文章转载请注明出处，谢谢`
#### 报文流
**HTTP报文是在HTTP应用程序之间发送的数据块，HTTP通过inbound和outbound来描述事物处理的方向，报文流入源端服务器，工作完成之后会流回用户的Agent代理中。**`如下图所示:`
![2039514-4627a70cdc6eb1d9.jpg](https://upload-images.jianshu.io/upload_images/2039514-4627a70cdc6eb1d9.jpg)
1.pic.jpg
**不管是请求报文还是响应报文，所有报文都是向下流动（downstream），所有报文的发送者都是在接收者的上游(upstream)。**`如下图所示：`
![2039514-03c555105aa2aac6.jpg](https://upload-images.jianshu.io/upload_images/2039514-03c555105aa2aac6.jpg)
2.pic.jpg
#### 报文的组成
**报文通常由由三部分组成：对报文进行描述的起始行(startline)，包含属性的首部(header)块，以及可选的包含数据的主体(body)部分。**见下图：
![2039514-d25cae0fcea0149a.jpg](https://upload-images.jianshu.io/upload_images/2039514-d25cae0fcea0149a.jpg)
3.pic.jpg
**一般起始行和首部就是由行分隔的ASCII文本，每行都以一个由两个字符组成的行终止序列作为结束，其中包括一个回车符和一个换行符，这个行终止序列可以写做CRLF。**
#### 报文的语法
HTTP报文可以分为两类:请求报文(request message)和响应报文(response message)。
**请求报文会向Web服务器请求一个动作:**
<'method'><'request-URL'><'version'>
<'headers'>
<'entity-body'>
**响应报文会将请求的结果返回给客户端：**
<'version'> <'status'> <'reason-phrase'>
<'headers'>
<'entity-body'>
接下来我们来讲述一下这些结构部分:
- 方法(method)
`客户端希望服务器对资源执行的动作，请求的起始行以方法作为开始，方法用来告知服务器做些什么。HTTP规范中定义了一组常用的请求方法。但是并不是所有的服务器都实现了这些方法，同时不同的服务器还有可能实现其它的一些方法。如下图`:
![2039514-ddfb832740d19171.jpg](https://upload-images.jianshu.io/upload_images/2039514-ddfb832740d19171.jpg)
5.pic.jpg
- 请求URL(request-URL)
`命名了所有请求资源，或者URL路径组件的完整URL`
- 版本(verison)
`报文所使用的HTTP版本，格式如下HTTP/<major>.<minor>，其中主要版本号(major)和次要版本号(minor)都是整数。版本号不会被当作分数来处理，版本中的每个数字都会被当作一个单独的数字来处理，因此在比较HTTP版本时每个数字都应该单独比较，来决定哪个版本更高。`
- 状态码(status-code)
`每个状态码的第一个数字都用于描述状态的一般类别(成功，出错等)，用来告诉客户端请求的结果，下图就列出了常用的状态码。`
![2039514-8903de93815c6d83.jpg](https://upload-images.jianshu.io/upload_images/2039514-8903de93815c6d83.jpg)
6.pic.jpg
- 原因短语(reason-phrase)
`数字状态码的可读版本，包含行终止序列之前的所有文本。`
- 首部(header)
`可以有零个或者多个首部，每个首部都包含一个名字，后面跟着一个冒号，然后是一个可选空格，接着是一个值，最后是一个CRLF。首部有可以分为以下几类：`- 通用首部:`既可以出现在请求报文中，也可以出现在响应报文中`
- 请求首部:`提供更多有关请求的信息`
- 响应首部:`提供更多有关响应的信息`
- 实体首部:`描述主体的长度和内容，或者资源自身`
- 扩展首部:`规范中没有定义的新首部`
`常见首部如下图所示`
![2039514-2ecb8b1ca1fa790b.jpg](https://upload-images.jianshu.io/upload_images/2039514-2ecb8b1ca1fa790b.jpg)
7.pic.jpg
- 实体的主体部分(entity-body)
`实体的主体部分包含一个任意数据组成的数据块。并不是所有的报文都包含实体的主体部分，有时报文只是以一个CRLF结束。HTTP报文可以承载很多类型的数字数据：图片，视频，HTML文档，软件应用程序，电子邮件等。参考如下的格式`
![2039514-53073236b49d3ac9.jpg](https://upload-images.jianshu.io/upload_images/2039514-53073236b49d3ac9.jpg)
4.pic.jpg
`对于版本0.9的报文是HTTP协议的早期版本，报文也是由请求和响应组成，但是请求中只包含方法和请求URL，响应中只包含实体，它没有版本信息(当时唯一的版本)，没有状态码或者原因短语，也没有首部。 如下图所示`
![2039514-4a961e72d34fe32a.jpg](https://upload-images.jianshu.io/upload_images/2039514-4a961e72d34fe32a.jpg)
8.pic.jpg
#### 报文中的方法（method）
**HTTP定义了一组成为安全方法的方法。GET方法和HEAD方法都被认为是安全的，因为使用GET或者HEAD方法的HTTP请求都不会对服务器产生什么结果。但是安全方法并不一定是什么动作都不执行，因为GET或者HEAD方法具体的操作是由开发者自己定义的，其实使用安全方法的目的就是允许HTTP应用程序开发者通知用户什么时候使用某个可能会引发某些动作的不安全方法。**
- GET
`GET通常用于请求服务器发送某个资源。见下图`
![2039514-0112fb2afb4482d3.jpg](https://upload-images.jianshu.io/upload_images/2039514-0112fb2afb4482d3.jpg)
9.pic.jpg
- HEAD
`HEAD方法与GET方法的行为很类似，但是服务器在响应中只返回首部，不返回实际的主体部分，允许客户端在未获取实际资源的情况下，对资源的首部进行检查，使用HEAD方法：`- `在不获取资源的情况下了解资源的情况`
- `通过查看响应中的状态码，看看某个对象是否存在`
- `通过查看首部，测试资源是否被修改了`
`见下图`
![2039514-91099883e3f5556c.jpg](https://upload-images.jianshu.io/upload_images/2039514-91099883e3f5556c.jpg)
10.pic.jpg
- PUT
`与GET从服务器读取文档相反，PUT方法会向服务器写入文档，PUT方法的语义就是让服务器用请求的主体部分来创建一个由所请求的URL命名的新文档，如果那个URL已经存在就用这个主体代替它。因为PUT允许用户对内容进行修改，所以很多web服务器都要求在执行PUT前用密码登录。见下图`
![2039514-7b41ac159193ad6a.jpg](https://upload-images.jianshu.io/upload_images/2039514-7b41ac159193ad6a.jpg)
11.pic.jpg
- POST
`POST方法可以用来向服务器输入数据，见下图`
![2039514-eecda645dc70482c.jpg](https://upload-images.jianshu.io/upload_images/2039514-eecda645dc70482c.jpg)
12.pic_hd.jpg
- TRACE
`TRACE方法主要用于诊断，TRACE允许客户端在最终将请求发送给服务器后，查看其中穿过防火墙，代理，网关或者其它应用程序每个可能修改原始HTTP请求的过程。TRACE请求会在目的服务器发起一个环路的诊断，行程的最后一站的服务器会弹回一条TRACE响应，并在响应主体中携带它收到的原始请求报文，这样客户端就可以查看在所有中间HTTP应用程序组成的请求/响应链上，原始报文是否以及如何被毁坏或者修改过。见下图`
![2039514-ab5a1782e150f499.jpg](https://upload-images.jianshu.io/upload_images/2039514-ab5a1782e150f499.jpg)
13.pic_hd.jpg
- OPTIONS
`OPTIONS方法请求Web服务器告知其支持的各种功能，可以询问服务器通常支持那些方法，或者对某些特殊资源支持哪些方法。见下图`
![2039514-e098d27e98ff95f7.jpg](https://upload-images.jianshu.io/upload_images/2039514-e098d27e98ff95f7.jpg)
14.pic.jpg
- DELETE
`DELETE方法所做的事情就是请服务器删除请求URL所指定的资源，但是客户端应用程序无法保证删除操作一定会被执行，因为HTTP规范允许服务器在不通知客户端的情况下撤销请求。见下图`
![2039514-c546c934fbc25a60.jpg](https://upload-images.jianshu.io/upload_images/2039514-c546c934fbc25a60.jpg)
15.pic.jpg
- 扩展方法
`HTTP被设计成字段可扩展，扩展方法指的就是没有在HTTP/1.1规范中定义的方法。见下图`
![2039514-25b827550e073cae.jpg](https://upload-images.jianshu.io/upload_images/2039514-25b827550e073cae.jpg)
16.pic.jpg
#### 报文中的状态码（status-code）
- 信息状态码（100-199）
`信息状态码是在HTTP/1.1中引入的，包括以下两种`
`100 Continue的目的是对HTTP客户端应用程序在有一个实体的主体部分要发送服务器，但希望在发送之前查看一下服务器是否会接受在这个实体这种情况进行优化。见下图`
![2039514-811bad8386b0b072.jpg](https://upload-images.jianshu.io/upload_images/2039514-811bad8386b0b072.jpg)
17.pic.jpg
- 客户端与100 Continue
`如果客户端在向服务器发送一个实体，并愿意在发送实体之前等待100 Continue响应，那么客户端就要发送一个携带了值为100 Continue的Expect请求首部。如果客户端没有发送实体，就不应该发送100 Continue Expect首部，因为这样会使服务器误以为客户端要发送一个实体。`
- 服务器与100 Continue
`如果服务器收到一条带有值为100 Continue的Expect首部的请求，它会用100 Continue响应或一条错误码来进行响应。服务器永远也不应该向没有发送100 Continue期望的客户端发送100 Continue状态码。同时如果服务器在有机会发送100 Continue响应之前就收到了部分（或者全部）的实体，说明服务器已经打算继续发送数据了，这样服务器就不需要发送这个状态码了，但是服务器完成请求之后，还是应该为请求发送一个最终状态码。`
- 成功状态码（200-299)
`服务器有一组用来表示成功的状态码。见下图`
![2039514-3c74d6651f503ef3.jpg](https://upload-images.jianshu.io/upload_images/2039514-3c74d6651f503ef3.jpg)
18.pic_hd.jpg
- 重定向状态码（300-399）
`重定向状态码要么告知客户端使用替代位置来访问他们感兴趣的资源。要么就提供一个替代的响应而不是资源内容。如果资源已被移动，可以发送一个重定向状态码和一个可选的Location首部来告知客户端资源已被移走，以及现在可以在哪里找到它。见下图 也可以通过某些重定向状态码对资源的应用程序本地副本与源端服务器的资源进行验证。见下图`
![2039514-a2483f942a470109.jpg](https://upload-images.jianshu.io/upload_images/2039514-a2483f942a470109.jpg)
19.pic_hd.jpg
`重定向状态码见下图`
![2039514-691489f7121b79c3.jpg](https://upload-images.jianshu.io/upload_images/2039514-691489f7121b79c3.jpg)
20.pic_hd.jpg
- 客户端错误状态码（400-499）
`有时候客户端会发送一些服务器无法处理的东西，比如格式错误的请求报文，或者常见是请求一个不存在的URL，这些时候服务器就会返回客户端的错误状态码。见下图`
![2039514-39fbfdd5af487928.jpg](https://upload-images.jianshu.io/upload_images/2039514-39fbfdd5af487928.jpg)
21.pic_hd.jpg
- 服务器错误状态码（500-599）
`有时候客户端发送一条有效的请求，但是服务器自身却出错了，这个时候服务器就会返回服务器错误状态码。见下图`
![2039514-3c6f7ac1cf9be695.jpg](https://upload-images.jianshu.io/upload_images/2039514-3c6f7ac1cf9be695.jpg)
34.pic_hd.jpg
#### 报文中的首部（header）
- 
通用首部
`那些提供了与报文相关的最基本的信息的首部称为通用首部，无论报文是请求报文还是响应报文，它们都会包含通用首部。见下图`
![2039514-9045f102f6c2608f.jpg](https://upload-images.jianshu.io/upload_images/2039514-9045f102f6c2608f.jpg)
22.pic.jpg
`HTTP/1.0引入了第一个允许HTTP应用程序缓存对象本地副本的首部，通用缓存首部，这样就不需要总是直接从源端服务器获取了。如下图`
![2039514-0de617bd155444d3.jpg](https://upload-images.jianshu.io/upload_images/2039514-0de617bd155444d3.jpg)
23.pic.jpg
- 
请求首部
`请求首部是只在请求报文中有意义的首部，用于说明是谁或者什么在发送请求，请求源于何处，或者客户端的喜好以及能力。服务器可以根据请求首部给出的客户端信息试着为客户端提供更好的响应。而请求首部包括以下几部分：`
- Accept首部
`Accept首部为客户端提供了一种将其喜好和能力告知服务器的方式，这样会使两端都受益，客户端会得到它们想要的结果，而服务器则不会浪费时间和宽带来发送客户端无法使用的东西。见下图`
![2039514-a869135e93801809.jpg](https://upload-images.jianshu.io/upload_images/2039514-a869135e93801809.jpg)
25.pic.jpg
- 条件请求首部
`条件请求首部用来给客户端的请求加上限制，要求服务器在对请求进行响应之前，确保某个条件为真。见下图`
![2039514-650337dd3edb0c35.jpg](https://upload-images.jianshu.io/upload_images/2039514-650337dd3edb0c35.jpg)
26.pic.jpg
- 安全请求首部
`安全请求首部用来在客户端获取特定的资源之前，先对自身进行认证，使事物安全一些。本质上就是HTTP的质询/响应认证机制的请求侧。见下图`
![2039514-79efe472ce97ad61.jpg](https://upload-images.jianshu.io/upload_images/2039514-79efe472ce97ad61.jpg)
27.pic.jpg
- 代理请求首部
`关于在代理方面会用到一些首部，见下图`
![2039514-23934a1f6211580c.jpg](https://upload-images.jianshu.io/upload_images/2039514-23934a1f6211580c.jpg)
28.pic.jpg
`最后还有一些请求的信息性首部，见下图`
![2039514-dd0f0b87e42c4419.jpg](https://upload-images.jianshu.io/upload_images/2039514-dd0f0b87e42c4419.jpg)
24.pic.jpg
- 
响应首部
`响应首部的主要目的是帮助客户端处理响应，并在将来发起更好的请求。`
- 协商首部
`协商首部主要是为了处理当资源有多种表示方法的时候，HTTP/1.1可以为服务器和客户端提供进行协商的能力。见下图`
![2039514-a691c3e13cb3dedf.jpg](https://upload-images.jianshu.io/upload_images/2039514-a691c3e13cb3dedf.jpg)
30.pic.jpg
- 安全响应首部
`安全响应首部和安全请求首部互相对应，本质上就是HTTP的质询/响应认证机制的响应侧。见下图`
![2039514-71f828a77d2f5307.jpg](https://upload-images.jianshu.io/upload_images/2039514-71f828a77d2f5307.jpg)
31.pic.jpg
`最后还有关于信息性的首部。见下图`
![2039514-3e3ccba580b06272.jpg](https://upload-images.jianshu.io/upload_images/2039514-3e3ccba580b06272.jpg)
29.pic.jpg
- 
实体首部
`实体首部提供了有关实体及其内容的大量信息，从有关对象类型的信息，到能够对资源使用的各种有效的请求方法。`
- 内容首部
`内容首部提供了与实体内容有关的特定信息，说明了其类型，尺寸以及处理它所需要的其它有用信息。见下图`
![2039514-7f861b4b499084c3.jpg](https://upload-images.jianshu.io/upload_images/2039514-7f861b4b499084c3.jpg)
32.pic.jpg
- 实体缓存首部
`通用缓存首部说明了如何或什么时候进行缓存，而实体缓存首部提供了与被缓存实体有关的信息。比如验证已缓存的资源副本是否仍然有效所需的信息，以及更好的估计已缓存资源的实效所需的条件。见下图`
![2039514-c01f437a793c05d5.jpg](https://upload-images.jianshu.io/upload_images/2039514-c01f437a793c05d5.jpg)
33.pic.jpg
