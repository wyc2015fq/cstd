# Qt 之高级网络操作（HTTP/FTP 快速上手） - xqhrs232的专栏 - CSDN博客
2018年09月05日 18:02:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：90
原文地址::[https://blog.csdn.net/liang19890820/article/details/52535755](https://blog.csdn.net/liang19890820/article/details/52535755)
# **简述**
Qt Network 模块中提供了一些高级别的类，例如：QNetworkRequest、QNetworkReply 和 QNetworkAccessManager，使用常见的协议执行网络操作。
在分享的过程中，顺便介绍下 Fiddler，便于我们调试。Fiddler 是一个 HTTP 协议调试代理工具。当然，也可以使用其它 Web/HTTP 调试工具。
**| **版权声明：一去、二三里，未经博主允许不得转载。
# **HTTP 消息结构**
先来看一下 HTTP 的消息结构。
![这里写图片描述](https://img-blog.csdn.net/20160915133908739)
- 
Request
请求行：Request 消息中的第一行，由请求方式、请求URL、HTTP协议及版本三部分组成。
请求头：其中 Content-Type 指定了客户端发送的内容格式。例如：`Content-Type: application/json`，指客户端发送的内容格式为 Json。
请求体：要发送的表单数据。
- 
Response
状态行：Response 消息中的第一行，由 HTTP 协议版本号、状态码、状态消息三部分组成。状态码用来告诉 HTTP 客户端，HTTP 服务器是否产生了预期的 Response。HTTP/1.1 中定义了 5 类状态码， 状态码由三位数字组成，第一个数字定义了响应的类别：
- 1XX：提示信息 - 表示请求已被成功接收，继续处理。
- 2XX：成功 - 表示请求已被成功接收、理解、接受。
- 3XX：重定向 - 要完成请求必须进行更进一步的处理。
- 4XX：客户端错误 - 请求有语法错误或请求无法实现。
- 5XX：服务器端错误 - 服务器未能实现合法的请求。
响应头：其中 Content-Type 指定了服务器返回的内容格式。例如：`Content-Type: application/json`，指服务器返回的内容格式为 Json。
响应体：服务器返回的内容。
**注意：**这里，针对 HTTP 协议只做了一些简单的介绍，便于下面讲解，关于协议中的其他部分，有兴趣的童鞋可自行研究。
# **支持的协议**
在进行网络请求之前，首先，要查看 QNetworkAccessManager 支持的协议：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#4f4f4f">QNetworkAccessManager</span> *manager = new <span style="color:#4f4f4f">QNetworkAccessManager</span>(this);
<span style="color:#009900">qDebug</span><span style="color:#4f4f4f">()</span> << <span style="color:#009900">manager</span>->supportedSchemes();</code></span>
```
- 1
- 2
通过调用 supportedSchemes()，列出了支持的所有 URL schemes：
> 
(“ftp”, “file”, “qrc”, “http”, “https”, “data”)
下面，我们主要以 HTTP 为例。
# **请求**
构建一个请求非常简单，本例中，我们尝试获取某个网页，以 CSDN 为例：
```
<span style="color:#000000"><code class="language-Qt">QString baseUrl = <span style="color:#009900">"http://www.csdn.net/"</span>;
QNetworkRequest <span style="color:#4f4f4f">request</span>;
<span style="color:#4f4f4f">request</span>.setUrl(QUrl(baseUrl));</code></span>
```
- 1
- 2
- 3
- 4
现在，名为 request 的 QNetworkRequest 请求对象就构建成功了，为了获取所有想要的信息，我们需要将该请求发送出去。
`<span style="color:#000000"><code class="language-Qt">QNetworkReply *pReplay = manager-><span style="color:#000088">get</span>(<span style="color:#4f4f4f">request</span>);</code></span>`- 1
这时，会获取一个名为 pReplay 的 QNetworkReply 响应对象。等待响应完成，就可以从这个对象中获取所有想要的信息。
```
<span style="color:#000000"><code class="language-Qt">QByteArray <span style="color:#4f4f4f">bytes</span> <span style="color:#4f4f4f">=</span> pReplay<span style="color:#4f4f4f">-></span>readAll();
qDebug() <span style="color:#4f4f4f"><<</span> <span style="color:#4f4f4f">bytes</span>;</code></span>
```
- 1
- 2
完整的源码：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#880000">// URL</span>
QString baseUrl <span style="color:#4f4f4f">=</span> <span style="color:#009900">"http://www.csdn.net/"</span>;
<span style="color:#880000">// 构造请求</span>
QNetworkRequest request;
request<span style="color:#4f4f4f">.</span>setUrl(QUrl(baseUrl));
QNetworkAccessManager <span style="color:#4f4f4f">*</span>manager <span style="color:#4f4f4f">=</span> <span style="color:#006666">new</span> QNetworkAccessManager(this);
<span style="color:#880000">// 发送请求</span>
QNetworkReply <span style="color:#4f4f4f">*</span>pReplay <span style="color:#4f4f4f">=</span> manager<span style="color:#4f4f4f">-></span>get(request);
<span style="color:#880000">// 开启一个局部的事件循环，等待响应结束，退出</span>
QEventLoop eventLoop;
QObject<span style="color:#006666">::connect</span>(manager, <span style="color:#4f4f4f">&</span>QNetworkAccessManager<span style="color:#006666">::finished</span>, <span style="color:#4f4f4f">&</span>eventLoop, <span style="color:#4f4f4f">&</span>QEventLoop<span style="color:#006666">::quit</span>);
eventLoop<span style="color:#4f4f4f">.</span>exec();
<span style="color:#880000">// 获取响应信息</span>
QByteArray <span style="color:#4f4f4f">bytes</span> <span style="color:#4f4f4f">=</span> pReplay<span style="color:#4f4f4f">-></span>readAll();
qDebug() <span style="color:#4f4f4f"><<</span> <span style="color:#4f4f4f">bytes</span>;</code></span>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
因为请求的过程是异步的，所以此时使用 QEventLoop 开启一个局部的事件循环，等待响应结束，事件循环退出。
**注意：**开启事件循环的同时，程序界面将不会响应用户操作（界面被阻塞）。
简便的 API 意味着所有 HTTP 请求类型都是显而易见的。那么其他 HTTP 请求类型：POST，PUT 又是如何的呢？都是一样的简单：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#4f4f4f">QNetworkReply</span> *pPostReplay = manager->post(request, <span style="color:#4f4f4f">QByteArray</span>());
<span style="color:#4f4f4f">QNetworkReply</span> *pPutReplay = manager->put(request, <span style="color:#4f4f4f">QByteArray</span>());</code></span>
```
- 1
- 2
漂亮，对吧？但这也仅是冰山一角。
# **传递 URL 参数**
你也许经常想为 URL 的查询字符串（query string）传递某种数据。如果你是手工构建 URL，那么数据会以键/值对的形式置于 URL 中，跟在一个问号的后面。例如：`http://www.zhihu.com/search?type=content&q=Qt`。Qt 提供了 QUrlQuery 类，可以很便利地提供这些参数。
举例来说，如果你想传递 `type=content` 和 `q=Qt` 到 `http://www.zhihu.com/search`，可以使用如下代码：
```
<span style="color:#000000"><code class="language-Qt">// <span style="color:#4f4f4f">URL</span>
<span style="color:#4f4f4f">QString</span> base<span style="color:#4f4f4f">Url</span> = <span style="color:#009900">"http://www.zhihu.com/search"</span>;
<span style="color:#4f4f4f">QUrl</span> url(base<span style="color:#4f4f4f">Url</span>);
// key-value 对
<span style="color:#4f4f4f">QUrlQuery</span> <span style="color:#000088">query</span>;
<span style="color:#000088">query</span>.add<span style="color:#4f4f4f">QueryItem</span>(<span style="color:#009900">"type"</span>, <span style="color:#009900">"content"</span>);
<span style="color:#000088">query</span>.add<span style="color:#4f4f4f">QueryItem</span>(<span style="color:#009900">"q"</span>, <span style="color:#009900">"Qt"</span>);
url.set<span style="color:#4f4f4f">Query</span>(<span style="color:#000088">query</span>);</code></span>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
通过打印输出该 URL，你能看到 URL 已被正确编码：
```
<span style="color:#000000"><code class="language-Qt">qDebug() << url.url();
<span style="color:#008800">//</span> <span style="color:#009900">"http://www.zhihu.com/search?type=content&q=Qt"</span></code></span>
```
- 1
- 2
更多参考：[Qt之QUrlQuery](http://blog.csdn.net/liang19890820/article/details/52503570)
# **代理**
在发送请求的时候，如果要通过 Fiddler 分析，就必须设置代理，主要使用 QNetworkProxy ：
```
<span style="color:#000000"><code class="language-Qt">QNetworkProxy proxy<span style="color:#880000">;</span>
proxy<span style="color:#009900">.setType</span>(QNetworkProxy::HttpProxy)<span style="color:#880000">;</span>
proxy<span style="color:#009900">.setHostName</span>(<span style="color:#009900">"127.0.0.1"</span>)<span style="color:#880000">;</span>
proxy<span style="color:#009900">.setPort</span>(<span style="color:#006666">8888</span>)<span style="color:#880000">;</span>
manager->setProxy(proxy)<span style="color:#880000">;</span></code></span>
```
- 1
- 2
- 3
- 4
- 5
更多参考：[Qt之QNetworkProxy（网络代理）](http://blog.csdn.net/liang19890820/article/details/51492611)
# **更加复杂的 POST 请求**
httpbin 是一个使用 Python + Flask 编写的 HTTP 请求和响应服务，主要用于测试 HTTP 库。
通常，你想要发送一些编码为表单形式的数据 - 非常像一个 HTML 表单。要实现这个，只需简单地传递一个QByteArray 给 data 参数。你的数据在发出请求时会自动编码为表单形式：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#880000">// URL</span>
QString baseUrl <span style="color:#4f4f4f">=</span> <span style="color:#009900">"http://httpbin.org/post"</span>;
QUrl url(baseUrl);
<span style="color:#880000">// 表单数据</span>
QByteArray dataArray;
dataArray<span style="color:#4f4f4f">.</span>append(<span style="color:#009900">"key1=value1&"</span>);
dataArray<span style="color:#4f4f4f">.</span>append(<span style="color:#009900">"key2=value2"</span>);
<span style="color:#880000">// 构造请求</span>
QNetworkRequest request;
request<span style="color:#4f4f4f">.</span>setHeader(QNetworkRequest<span style="color:#006666">::ContentTypeHeader</span>, <span style="color:#009900">"application/x-www-form-urlencoded"</span>);
request<span style="color:#4f4f4f">.</span>setUrl(url);
QNetworkAccessManager <span style="color:#4f4f4f">*</span>manager <span style="color:#4f4f4f">=</span> <span style="color:#006666">new</span> QNetworkAccessManager(this);
<span style="color:#880000">// 发送请求</span>
manager<span style="color:#4f4f4f">-></span>post(request, dataArray);</code></span>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
![这里写图片描述](https://img-blog.csdn.net/20160914161105711)
还可以使用 json 参数直接传递，然后它就会被自动编码：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#880000">// URL</span>
QString baseUrl <span style="color:#4f4f4f">=</span> <span style="color:#009900">"http://httpbin.org/post"</span>;
QUrl url(baseUrl);
<span style="color:#880000">// Json数据</span>
QJsonObject json;
json<span style="color:#4f4f4f">.</span>insert(<span style="color:#009900">"User"</span>, <span style="color:#009900">"Qter"</span>);
json<span style="color:#4f4f4f">.</span>insert(<span style="color:#009900">"Password"</span>, <span style="color:#009900">"123456"</span>);
QJsonDocument document;
document<span style="color:#4f4f4f">.</span>setObject(json);
QByteArray dataArray <span style="color:#4f4f4f">=</span> document<span style="color:#4f4f4f">.</span>toJson(QJsonDocument<span style="color:#006666">::Compact</span>);
<span style="color:#880000">// 构造请求</span>
QNetworkRequest request;
request<span style="color:#4f4f4f">.</span>setHeader(QNetworkRequest<span style="color:#006666">::ContentTypeHeader</span>, <span style="color:#009900">"application/json"</span>);
request<span style="color:#4f4f4f">.</span>setUrl(url);
QNetworkAccessManager <span style="color:#4f4f4f">*</span>manager <span style="color:#4f4f4f">=</span> <span style="color:#006666">new</span> QNetworkAccessManager(this);
<span style="color:#880000">// 连接信号槽</span>
connect(manager, SIGNAL(finished(QNetworkReply <span style="color:#4f4f4f">*</span>)), this, SLOT(replyFinished(QNetworkReply <span style="color:#4f4f4f">*</span>)));
<span style="color:#880000">// 发送请求</span>
manager<span style="color:#4f4f4f">-></span>post(request, dataArray);</code></span>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
![这里写图片描述](https://img-blog.csdn.net/20160914161954417)
为了不阻塞界面，我们不再使用 QEventLoop，而用 QNetworkAccessManager 对应的信号，当响应结束就会发射 finished() 信号，将其链接到对应的槽函数上即可。
# **定制请求头**
如果你想为请求添加 HTTP 头部，只要简单地调用 setHeader() 就可以了。
例如，发送的请求时，使用的 User-Agent 是 `Mozilla/5.0` , 为了方便以后追踪版本信息，可以将软件的版本信息写入到 User-Agent 中。
```
<span style="color:#000000"><code class="language-Qt">QNetworkRequest <span style="color:#4f4f4f">request</span>;
<span style="color:#4f4f4f">request</span>.setHeader(QNetworkRequest::UserAgentHeader, <span style="color:#009900">"my-app/0.0.1"</span>);</code></span>
```
- 1
- 2
User-Agent：包含发出请求的用户信息。
![这里写图片描述](https://img-blog.csdn.net/20160914115209964)
当然，除了 User-Agent 之外，QNetworkRequest::KnownHeaders 还包含其他请求头，它就是为 HTTP 头部而生的。根据 [RFC 2616](https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html)， HTTP 头部是大小写不敏感。
如果 QNetworkRequest::KnownHeaders 不满足需要，使用 setRawHeader()。
# **响应内容**
要获取响应的内容，可以调用 readAll()，由于上述的 POST 请求返回的数据为 Json 格式，将响应结果先转化为 Json，然后再对其解析：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#006666">void</span> replyFinished(QNetworkReply <span style="color:#4f4f4f">*</span>reply)
{
    <span style="color:#880000">// 获取响应信息</span>
    QByteArray <span style="color:#4f4f4f">bytes</span> <span style="color:#4f4f4f">=</span> reply<span style="color:#4f4f4f">-></span>readAll();
    QJsonParseError jsonError;
    QJsonDocument doucment <span style="color:#4f4f4f">=</span> QJsonDocument<span style="color:#006666">::fromJson</span>(<span style="color:#4f4f4f">bytes</span>, <span style="color:#4f4f4f">&</span>jsonError);
    <span style="color:#000088">if</span> (jsonError<span style="color:#4f4f4f">.</span>error <span style="color:#4f4f4f">!=</span> QJsonParseError<span style="color:#006666">::NoError</span>) {
        qDebug() <span style="color:#4f4f4f"><<</span> QStringLiteral(<span style="color:#009900">"解析Json失败"</span>);
        <span style="color:#000088">return</span>;
    }
    <span style="color:#880000">// 解析Json</span>
    <span style="color:#000088">if</span> (doucment<span style="color:#4f4f4f">.</span>isObject()) {
        QJsonObject obj <span style="color:#4f4f4f">=</span> doucment<span style="color:#4f4f4f">.</span>object();
        QJsonValue value;
        <span style="color:#000088">if</span> (obj<span style="color:#4f4f4f">.</span>contains(<span style="color:#009900">"data"</span>)) {
            value <span style="color:#4f4f4f">=</span> obj<span style="color:#4f4f4f">.</span><span style="color:#000088">take</span>(<span style="color:#009900">"data"</span>);
            <span style="color:#000088">if</span> (value<span style="color:#4f4f4f">.</span>isString()) {
                QString <span style="color:#4f4f4f">data</span> <span style="color:#4f4f4f">=</span> value<span style="color:#4f4f4f">.</span>toString();
                qDebug() <span style="color:#4f4f4f"><<</span> <span style="color:#4f4f4f">data</span>;
            }
        }
    }
}</code></span>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
响应的内容可以是 HTML 页面，也可以是字符串、Json、XML等。最上面所发送的 GET 请求 获取的就是 CSDN 的首页 HTML。
# **响应状态码**
我们可以检测响应状态码：
```
<span style="color:#000000"><code class="language-Qt">QVariant variant <span style="color:#4f4f4f">=</span> pReplay<span style="color:#4f4f4f">-></span>attribute(QNetworkRequest<span style="color:#006666">::HttpStatusCodeAttribute</span>);
<span style="color:#000088">if</span> (variant<span style="color:#4f4f4f">.</span>isValid())
    qDebug() <span style="color:#4f4f4f"><<</span> variant<span style="color:#4f4f4f">.</span>toInt();
<span style="color:#880000">// 200    </span></code></span>
```
- 1
- 2
- 3
- 4
HTTP 状态码请参考：
- [Status codes](https://www.w3.org/Protocols/HTTP/HTRESP.html)
- [List of HTTP status codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)
最常见的是 200 OK，表示请求已成功，请求所希望的响应头或数据体将随此响应返回。
# **响应头**
进入 Response Headers：
![这里写图片描述](https://img-blog.csdn.net/20160914121353009)
可以看到包含很多，和上面一样，使用任意 QNetworkRequest::KnownHeaders 来访问这些响应头字段。例如，Content Type：
```
<span style="color:#000000"><code class="language-Qt">QVariant variant <span style="color:#4f4f4f">=</span> pReplay<span style="color:#4f4f4f">-></span><span style="color:#000088">header</span>(QNetworkRequest<span style="color:#006666">::ContentTypeHeader</span>);
<span style="color:#000088">if</span> (variant<span style="color:#4f4f4f">.</span>isValid())
    qDebug() <span style="color:#4f4f4f"><<</span> variant<span style="color:#4f4f4f">.</span>toString();
<span style="color:#880000">// "text/html; charset=utf-8"</span></code></span>
```
- 1
- 2
- 3
- 4
如果 QNetworkRequest::KnownHeaders 不满足需要，可以使用 rawHeader()。例如，响应包含了登录后的 TOKEN，位于原始消息头中：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#009900">if</span> <span style="color:#4f4f4f">(reply->hasRawHeader(<span style="color:#009900">"TOKEN"</span>))</span>
    QB<span style="color:#009900">yteArray</span> <span style="color:#009900">byte</span> = <span style="color:#009900">reply</span>->rawHeader(<span style="color:#009900">"TOKEN"</span>);</code></span>
```
- 1
- 2
它还有一个特殊点，那就是服务器可以多次接受同一 header，每次都使用不同的值。Qt 会将它们合并，这样它们就可以用一个映射来表示出来，参见 [RFC 7230](https://tools.ietf.org/html/rfc7230#section-3.2)：
> 
A recipient MAY combine multiple header fields with the same field name into one “field-name: field-value” pair, without changing the semantics of the message, by appending each subsequent field value to the combined field value in order, separated by a comma.
接收者可以合并多个相同名称的 header 栏位，把它们合为一个 `"field-name: field-value"` 配对，将每个后续的栏位值依次追加到合并的栏位值中，用逗号隔开即可，这样做不会改变信息的语义。
# **错误**
如果请求的处理过程中遇到错误（如：DNS 查询失败、拒绝连接等）时，则会产生一个 QNetworkReply::NetworkError。
例如，我们将 URL 改为这样：
`<span style="color:#000000"><code class="language-Qt">QString baseUrl = <span style="color:#009900">"http://www.csdnQter.net/"</span>;</code></span>`- 1
发送请求后，由于主机名无效，必然会发生错误，根据 error() 来判断：
```
<span style="color:#000000"><code class="language-Qt">QNetworkReply::NetworkError <span style="color:#000088">error</span> = pReplay-><span style="color:#000088">error</span>();
<span style="color:#000088">switch</span> (<span style="color:#000088">error</span>) {
<span style="color:#000088">case</span> QNetworkReply::ConnectionRefusedError:
    qDebug() << QStringLiteral(<span style="color:#009900">"远程服务器拒绝连接"</span>);
    <span style="color:#000088">break</span>;
<span style="color:#000088">case</span> QNetworkReply::HostNotFoundError:
    qDebug() << QStringLiteral(<span style="color:#009900">"远程主机名未找到（无效主机名）"</span>);
    <span style="color:#000088">break</span>;
<span style="color:#000088">case</span> QNetworkReply::TooManyRedirectsError:
    qDebug() << QStringLiteral(<span style="color:#009900">"请求超过了设定的最大重定向次数"</span>);
    <span style="color:#000088">break</span>;
deafult:
    qDebug() << QStringLiteral(<span style="color:#009900">"未知错误"</span>);
}
<span style="color:#880000">// "远程主机名未找到（无效主机名）"</span></code></span>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
这时，会产生一个 QNetworkReply::HostNotFoundError 错误。
**注意：** QNetworkReply::TooManyRedirectsError 是 5.6 中引进的，默认限制是 50，可以使用 QNetworkRequest::setMaxRedirectsAllowed() 来改变。
如果要获取到可读的错误信息，使用：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#009900">QString</span> strError = pReplay->errorString();
qDebug() << strError;
<span style="color:#008800">//</span> <span style="color:#009900">"Host www.csdnqter.net not found"</span></code></span>
```
- 1
- 2
- 3
准备好了吗？赶快去试试吧！
