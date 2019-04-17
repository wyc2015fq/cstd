# 爬虫基础之HTTP基本原理 - miner_zhu的博客 - CSDN博客





2018年08月17日 15:27:53[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：184








# 1.URI和URL

URI的全称为Uniform Resource Identifier，即统一资源标志符。

URL的全称为Universal Resource Locator，即统一资源定位符。

URL是URI的子集，也就是说每个URL都是URI，但不是每个URI都是URL。那么，怎样的URI不是URL呢？URI还包括一个子类叫作URN，它的全称为Universal Resource Name，即统一资源名称。URN只命名资源而不指定如何定位资源，比如urn:isbn:123456789指定了一本书的ISBN，可以唯一标识这本书，但是没有指定到哪里定位这本书，这就是URN。但是在目前的互联网中，URN用得非常少，所以几乎所有的URI都是URL，一般的网页链接我们既可以称为URL，也可以称为URI。 

![](https://qiniu.cuiqingcai.com/wp-content/uploads/2018/02/2-1.jpg)

# 2.超文本

超文本，英文名称hypertext，我们在浏览器里看到的网页就是超文本解析而成的，其网页源代码是一系列HTML代码，里面包含了一系列标签。比如img显示图片，p显示段落等。浏览器解析这些标签后，便形成了我们平常看到的网页，而网页的源代码HTML就可以称作超文本。

我们平时在网页使用检查看到的elements项即是超文本。

# 3.HTTP和HTTPS

URL的开头会有http或https等常见的访问资源需要的协议类型。少数情况，我们还会看到ftp、sftp、smb的协议类型。

HTTP的全称是Hyper Text Transfer Protocol，中文名叫作超文本传输协议。HTTP协议是用于从网络传输超文本数据到本地浏览器的传送协议，它能保证高效而准确地传送超文本文档。HTTP由万维网协会（World Wide Web Consortium）和Internet工作小组IETF（Internet Engineering Task Force）共同合作制定的规范，目前广泛使用的是HTTP 1.1版本。

HTTPS的全称是Hyper Text Transfer Protocol over Secure Socket Layer，是以安全为目标的HTTP通道，简单讲是HTTP的安全加强版，即HTTP下加入SSL层，简称为HTTPS。

HTTPS的安全基础是SSL，因此通过它传输的内容都是经过SSL加密的，它的主要作用可以分为两种：1.建立一个信息安全通道来保证数据传输的安全。2.确认网站的真实性，凡是使用了HTTPS的网站，都可以通过点击浏览器地址栏的锁头标志来查看网站认证之后的真实信息，也可以通过CA机构颁发的安全签章来查询。

# 4.HTTP请求过程

我们在浏览器中输入一个网址在浏览器中浏览内容。实际上，这个过程是浏览器向网站所在的服务器发送了一个请求，网站服务器接收到这个请求后进行处理和解析，然后返回对应的响应，接着传回给浏览器。响应里包含了页面的源代码等内容，浏览器再对其进行解析，便将网页呈现了出来

![](https://qiniu.cuiqingcai.com/wp-content/uploads/2018/02/2-4.jpg)

我们可以用Chrome浏览器的审查中的Network监听组件来做下演示，它可以显示访问当前请求网页时发生的所有网络请求和响应。

其中各列的含义如下：

第一列**Name**：请求的名称，一般会将URL的最后一部分内容当作名称。

第二列**Status**：响应的状态码，这里显示为200，代表响应是正常的。

第三列**Type**：请求的文档类型。这里为document，代表我们这次请求的是一个HTML文档，内容就是一些HTML代码。

第四列**Initiator**：请求源。用来标记请求是由哪个对象或进程发起的。

第五列**Size**：从服务器下载的文件和请求的资源大小。如果是从缓存中取得的资源，则该列会显示from cache。

第六列**Time**：发起请求到获取响应所用的总时间。

第七列**Waterfa**ll：网络请求的可视化瀑布流。

点击具体条目，我们可以看到更多：

首先是General部分，Request URL为请求的URL，Request Method为请求的方法，Status Code为响应状态码，Remote Address为远程服务器的地址和端口，Referrer Policy为Referrer判别策略。再继续往下看，可以看到，有Response Headers和Request Headers，这分别代表响应头和请求头。请求头里带有许多请求信息，例如浏览器标识、Cookies、Host等信息，这是请求的一部分，服务器会根据请求头内的信息判断请求是否合法，进而作出对应的响应。图中看到的Response Headers就是响应的一部分，例如其中包含了服务器的类型、文档类型、日期等信息，浏览器接受到响应后，会解析响应内容，进而呈现网页内容。

# 5.请求

请求，由客户端向服务端发出，可以分为4部分内容：请求方法（Request Method）、请求的网址（Request URL）、请求头（Request Headers）、请求体（Request Body）。

**(1) 请求方法**

常见的请求方法有两种：**GET和POST**。

在浏览器中直接输入URL并回车，这便发起了一个GET请求，请求的参数会直接包含到URL里。例如，在百度中搜索Python，这就是一个GET请求，链接为https://www.baidu.com/s?wd=Python，其中URL中包含了请求的参数信息，这里参数wd表示要搜寻的关键字。

POST请求大多在表单提交时发起。比如，对于一个登录表单，输入用户名和密码后，点击“登录”按钮，这通常会发起一个POST请求，其数据通常以表单的形式传输，而不会体现在URL中。

GET和POST请求方法有如下**区别**：

1.GET请求中的参数包含在URL里面，数据可以在URL中看到，而POST请求的URL不会包含这些数据，数据都是通过表单形式传输的，会包含在请求体中。

2.GET请求提交的数据最多只有1024字节，而POST方式没有限制。

一般来说，登录时，需要提交用户名和密码，其中包含了敏感信息，使用GET方式请求的话，密码就会暴露在URL里面，造成密码泄露，所以这里最好以POST方式发送。上传文件时，由于文件内容比较大，也会选用POST方式。

我们平常遇到的绝大部分请求都是GET或POST请求，另外还有一些请求方法，如GET、HEAD、POST、PUT、DELETE、OPTIONS、CONNECT、TRACE等，我们将其简单总结：
|序号|方法|描述|
|----|----|----|
|1|GET|请求指定的页面信息，并返回实体主体。|
|2|HEAD|类似于get请求，只不过返回的响应中没有具体的内容，用于获取报头|
|3|POST|向指定资源提交数据进行处理请求（例如提交表单或者上传文件）。数据被包含在请求体中。POST请求可能会导致新的资源的建立和/或已有资源的修改。|
|4|PUT|从客户端向服务器传送的数据取代指定的文档的内容。|
|5|DELETE|请求服务器删除指定的页面。|
|6|CONNECT|HTTP/1.1协议中预留给能够将连接改为管道方式的代理服务器。|
|7|OPTIONS|允许客户端查看服务器的性能。|
|8|TRACE|回显服务器收到的请求，主要用于测试或诊断。|

**(2) 请求的网址**

请求的网址，即统一资源定位符URL，它可以唯一确定我们想请求的资源。

**(3) 请求头**

请求头，用来说明服务器要使用的附加信息，比较重要的信息有Cookie、Referer、User-Agent等。下面简要说明一些常用的头信息。

**Accept**：请求报头域，用于指定客户端可接受哪些类型的信息。
**Accept-Language**：指定客户端可接受的语言类型。
**Accept-Encoding**：指定客户端可接受的内容编码。
**Host**：用于指定请求资源的主机IP和端口号，其内容为请求URL的原始服务器或网关的位置。从HTTP 1.1版本开始，请求必须包含此内容。
**Cookie**：也常用复数形式 Cookies，这是网站为了辨别用户进行会话跟踪而存储在用户本地的数据。它的主要功能是维持当前访问会话。例如，我们输入用户名和密码成功登录某个网站后，服务器会用会话保存登录状态信息，后面我们每次刷新或请求该站点的其他页面时，会发现都是登录状态，这就是Cookies的功劳。Cookies里有信息标识了我们所对应的服务器的会话，每次浏览器在请求该站点的页面时，都会在请求头中加上Cookies并将其发送给服务器，服务器通过Cookies识别出是我们自己，并且查出当前状态是登录状态，所以返回结果就是登录之后才能看到的网页内容。
**Referer**：此内容用来标识这个请求是从哪个页面发过来的，服务器可以拿到这一信息并做相应的处理，如作来源统计、防盗链处理等。
**User-Agent**：简称UA，它是一个特殊的字符串头，可以使服务器识别客户使用的操作系统及版本、浏览器及版本等信息。在做爬虫时加上此信息，可以伪装为浏览器；如果不加，很可能会被识别出为爬虫。
**Content-Type**：也叫互联网媒体类型（Internet Media Type）或者MIME类型，在HTTP协议消息头中，它用来表示具体请求中的媒体类型信息。例如，text/html代表HTML格式，image/gif代表GIF图片，application/json代表JSON类型，更多对应关系可以查看此对照表：http://tool.oschina.net/commons。

因此，请求头是请求的重要组成部分，在写爬虫时，大部分情况下都需要设定请求头。

**(4) 请求体**

请求体一般承载的内容是POST请求中的表单数据，而对于GET请求，请求体则为空。

例如我们登录GitHub时，登录之前，我们填写了用户名和密码信息，提交时这些内容就会以表单数据的形式提交给服务器，此时需要注意Request Headers中指定Content-Type为application/x-www-form-urlencoded。只有设置Content-Type为application/x-www-form-urlencoded，才会以表单数据的形式提交。另外，我们也可以将Content-Type设置为application/json来提交JSON数据，或者设置为multipart/form-data来上传文件。

![](https://img-blog.csdn.net/20180817150835662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

爬虫中，如果要构造POST请求，需要使用正确的Content-Type，并了解各种请求库的各个参数设置时使用的是哪种Content-Type，不然可能会导致POST提交后无法正常响应。

# 6.响应

响应，由服务端返回给客户端，可以分为三部分：响应状态码（Response Status Code）、响应头（Response Headers）和响应体（Response Body）。

**(1) 响应状态码**

响应状态码表示服务器的响应状态，如200代表服务器正常响应，404代表页面未找到，500代表服务器内部发生错误。在爬虫中，我们可以根据状态码来判断服务器响应状态，如状态码为200，则证明成功返回数据，再进行进一步的处理，否则直接忽略。表2-3列出了常见的错误代码及错误原因。

![](https://img-blog.csdn.net/20180817151912101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**(2) 响应头**

响应头包含了服务器对请求的应答信息，如Content-Type、Server、Set-Cookie等。下面简要说明一些常用的头信息。

**Date**：标识响应产生的时间。
**Last-Modified**：指定资源的最后修改时间。
**Content-Encoding**：指定响应内容的编码。
**Server**：包含服务器的信息，比如名称、版本号等。
**Content-Type**：文档类型，指定返回的数据类型是什么，如text/html代表返回HTML文档，application/x-javascript则代表返回JavaScript文件，image/jpeg则代表返回图片。
**Set-Cookie**：设置Cookies。响应头中的Set-Cookie告诉浏览器需要将此内容放在Cookies中，下次请求携带Cookies请求。
**Expires**：指定响应的过期时间，可以使代理服务器或浏览器将加载的内容更新到缓存中。如果再次访问时，就可以直接从缓存中加载，降低服务器负载，缩短加载时间。

**(3) 响应体**

最重要的当属响应体的内容了。响应的正文数据都在响应体中，比如请求网页时，它的响应体就是网页的HTML代码；请求一张图片时，它的响应体就是图片的二进制数据。我们做爬虫请求网页后，要解析的内容就是响应体。

在浏览器开发者工具中点击Preview，就可以看到网页的源代码，也就是响应体的内容，它是解析的目标。

在做爬虫时，我们主要通过响应体得到网页的源代码、JSON数据等，然后从中做相应内容的提取。



