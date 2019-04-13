
# OKHttp源码分析3 - HttpEngine底层实现 - 谢杨易的博客 - CSDN博客

2016年09月04日 11:51:26[谢杨易](https://me.csdn.net/u013510838)阅读数：3375



## 1 概述
上篇文章，我们详细分析了OKHttp中Request的创建和发送过程。其中sendRequest(), readResponse(), followUpRequest()三个关键方法在底层HttpEngine中实现。革命尚未成功，我们接下来在这篇文章中分析HttpEngine中的这三个方法。由于底层HttpEngine涉及到很多Http协议方面东西，对Http协议不熟悉的同学可以先阅读我的这篇文章[Http协议简介](http://blog.csdn.net/u013510838/article/details/52431558)
## 2 sendRequest()源码分析
sendRequest()方法是client向server发送request的主要方法。它先对request的header添加了一些默认字段，如keep-alive。然后对cache进行处理，判断是否可以直接使用cache。如果不行，才真正发送网络request。
![Markdown](http://i2.buimg.com/573600/ec77143c6dd095af.png)
```python
public
```
```python
void
```
```python
sendRequest
```
```python
() throws RequestException, RouteException, IOException {
```
```python
if
```
```python
(cacheStrategy !=
```
```python
null
```
```python
)
```
```python
return
```
```python
;
```
```python
// Already sent.
```
```python
if
```
```python
(transport !=
```
```python
null
```
```python
)
```
```python
throw
```
```python
new
```
```python
IllegalStateException();
```
```python
// 对header的处理，利用app中用户构造的原始request
```
```python
// 主要是对header进行添加。如添加"Connection: Keep-Alive"首部。后面单独分析
```
```python
Request request = networkRequest(userRequest);
```
```python
// 对cache的处理
```
```python
InternalCache responseCache = Internal.instance.internalCache(client);
```
```python
// 利用request为key，从cache中取出response
```
```python
Response cacheCandidate = responseCache !=
```
```python
null
```
```python
? responseCache.
```
```python
get
```
```python
(request)
        :
```
```python
null
```
```python
;
```
```python
// 判断cache是否可用，利用Expires，Last-Modified，Date，Age等header字段，后面详细分析
```
```python
long
```
```python
now = System.currentTimeMillis();
    cacheStrategy =
```
```python
new
```
```python
CacheStrategy.Factory(now, request, cacheCandidate).
```
```python
get
```
```python
();
```
```python
// cache可用或网络被禁止使用则networkRequest为null
```
```python
networkRequest = cacheStrategy.networkRequest;
```
```python
// cache不可用，则cacheResponse为null。对应情况有，不允许使用cache，没有对应cache，cache过期需要重新验证
```
```python
cacheResponse = cacheStrategy.cacheResponse;
```
```python
if
```
```python
(responseCache !=
```
```python
null
```
```python
) {
      responseCache.trackResponse(cacheStrategy);
    }
```
```python
if
```
```python
(cacheCandidate !=
```
```python
null
```
```python
&& cacheResponse ==
```
```python
null
```
```python
) {
```
```python
// cache已过期，不可用，关闭它
```
```python
closeQuietly(cacheCandidate.body());
    }
```
```python
if
```
```python
(networkRequest !=
```
```python
null
```
```python
) {
```
```python
// networkRequest不为空，代表cache不可用，且网络可用
```
```python
// 从这儿可以看出，cache可用时会直接使用cache，不可用才走网络数据。这也是符合Http常规做法的。
```
```python
if
```
```python
(connection ==
```
```python
null
```
```python
) {
```
```python
// 连接到server，直接连接或通过代理均可
```
```python
connect();
      }
```
```python
// 构造HttpTransport，与发送request到网络中去有关
```
```python
transport = Internal.instance.newTransport(connection,
```
```python
this
```
```python
);
```
```python
// 将start line，headers，body写入到buffer中，以等待发送出去
```
```python
if
```
```python
(callerWritesRequestBody && permitsRequestBody(networkRequest) && requestBodyOut ==
```
```python
null
```
```python
) {
```
```python
// 从request的header中获取content-length
```
```python
long
```
```python
contentLength = OkHeaders.contentLength(request);
```
```python
if
```
```python
(bufferRequestBody) {
```
```python
// bufferRequestBody表示body在内存中了，这可能是多次发送重试等情况
```
```python
// content-length太大
```
```python
if
```
```python
(contentLength > Integer.MAX_VALUE) {
```
```python
throw
```
```python
new
```
```python
IllegalStateException(
```
```python
"Use setFixedLengthStreamingMode() or "
```
```python
+
```
```python
"setChunkedStreamingMode() for requests larger than 2 GiB."
```
```python
);
          }
```
```python
if
```
```python
(contentLength != -
```
```python
1
```
```python
) {
```
```python
// content-length已知，是个准确值
```
```python
// 可以将start line和header写入HttpConnection中，此处涉及到Http报文结构和发送，后面重点讲解
```
```python
transport.writeRequestHeaders(networkRequest);
```
```python
// 构造request body的buffer，长度为content-length
```
```python
requestBodyOut =
```
```python
new
```
```python
RetryableSink((
```
```python
int
```
```python
) contentLength);
          }
```
```python
else
```
```python
{
```
```python
// content-length还不确定，此时不能设置content-length首部，因为它还不确定。
```
```python
// 要等到整个body准备好后，才能计算出content-length
```
```python
requestBodyOut =
```
```python
new
```
```python
RetryableSink();
          }
        }
```
```python
else
```
```python
{
          transport.writeRequestHeaders(networkRequest);
          requestBodyOut = transport.createRequestBody(networkRequest, contentLength);
        }
      }
    }
```
```python
else
```
```python
{
```
```python
// networkRequest为null，要么cache可用，要么网络被禁止使用
```
```python
if
```
```python
(connection !=
```
```python
null
```
```python
) {
```
```python
// 回收网络connection，并关闭它
```
```python
Internal.instance.recycle(client.getConnectionPool(), connection);
        connection =
```
```python
null
```
```python
;
      }
```
```python
if
```
```python
(cacheResponse !=
```
```python
null
```
```python
) {
```
```python
// cache可用。可用代表有此request的cache response，且没有过期
```
```python
this
```
```python
.userResponse = cacheResponse.newBuilder()
            .request(userRequest)
            .priorResponse(stripBody(priorResponse))
            .cacheResponse(stripBody(cacheResponse))
            .build();
      }
```
```python
else
```
```python
{
```
```python
// 网络被禁止使用，自己构造一个504的response，gateway timeout
```
```python
this
```
```python
.userResponse =
```
```python
new
```
```python
Response.Builder()
            .request(userRequest)
            .priorResponse(stripBody(priorResponse))
            .protocol(Protocol.HTTP_1_1)
            .code(
```
```python
504
```
```python
)
            .message(
```
```python
"Unsatisfiable Request (only-if-cached)"
```
```python
)
            .body(EMPTY_BODY)
            .build();
      }
```
```python
// 将利用cache或自己生成的504response，进行gzip压缩
```
```python
// 前面提到过，request的headers中声明了支持gzip压缩，故response中最好加入gzip压缩。
```
```python
userResponse = unzip(userResponse);
    }
  }
```
我们接下来分析sendRequest()中使用到的一些比较重要的方法。networkRequest()方法作用为，在原有的request基础上添加一些header。从这些header中我们可以看出，OKHttp默认是使用Keep-Alive，response body支持gzip压缩，支持Cookie的使用。看到了吧，分析底层代码有助于我们对Http协议的理解和对OKHttp特性的掌握。
```python
private
```
```python
Request networkRequest(Request request) throws IOException {
    Request
```
```python
.
```
```python
Builder result
```
```python
=
```
```python
request
```
```python
.
```
```python
newBuilder();
```
```python
// 利用url解析出host，然后添加host header。它指明了server地址
```
```python
if
```
```python
(request
```
```python
.
```
```python
header
```
```python
(
```
```python
"Host"
```
```python
)
```
```python
==
```
```python
null
```
```python
) {
      result
```
```python
.
```
```python
header
```
```python
(
```
```python
"Host"
```
```python
, Util
```
```python
.
```
```python
hostHeader(request
```
```python
.
```
```python
httpUrl()));
    }
```
```python
// 添加Connection首部，Keep-Alive表示持久连接，一次request和response完成后，HTTP并不立刻关闭。
```
```python
if
```
```python
(request
```
```python
.
```
```python
header
```
```python
(
```
```python
"Connection"
```
```python
)
```
```python
==
```
```python
null
```
```python
) {
      result
```
```python
.
```
```python
header
```
```python
(
```
```python
"Connection"
```
```python
,
```
```python
"Keep-Alive"
```
```python
);
    }
```
```python
// 添加Accept-Encoding首部，gzip表示可接收gzip格式的压缩编码
```
```python
if
```
```python
(request
```
```python
.
```
```python
header
```
```python
(
```
```python
"Accept-Encoding"
```
```python
)
```
```python
==
```
```python
null
```
```python
) {
      transparentGzip
```
```python
=
```
```python
true
```
```python
;
      result
```
```python
.
```
```python
header
```
```python
(
```
```python
"Accept-Encoding"
```
```python
,
```
```python
"gzip"
```
```python
);
    }
```
```python
// 处理cookie header
```
```python
CookieHandler cookieHandler
```
```python
=
```
```python
client
```
```python
.
```
```python
getCookieHandler();
```
```python
if
```
```python
(cookieHandler
```
```python
!=
```
```python
null
```
```python
) {
```
```python
// 将用户构建的原始request中的header弄成Map结构
```
```python
Map
```
```python
<
```
```python
String
```
```python
,
```
```python
List
```
```python
<
```
```python
String
```
```python
>>
```
```python
headers
```
```python
=
```
```python
OkHeaders
```
```python
.
```
```python
toMultimap(result
```
```python
.
```
```python
build()
```
```python
.
```
```python
headers(),
```
```python
null
```
```python
);
```
```python
// 从URI中解析出cookie，并添加到Map中，其key为"Cookie"
```
```python
Map
```
```python
<
```
```python
String
```
```python
,
```
```python
List
```
```python
<
```
```python
String
```
```python
>>
```
```python
cookies
```
```python
=
```
```python
cookieHandler
```
```python
.
```
```python
get(request
```
```python
.
```
```python
uri(), headers);
```
```python
// 添加Cookie和Cookie2 header
```
```python
OkHeaders
```
```python
.
```
```python
addCookies(result, cookies);
    }
```
```python
// 添加User-Agent header，它表示client端是啥东西，比如浏览器
```
```python
// 对于OKHttp来说，就是okhttp和它的版本号
```
```python
if
```
```python
(request
```
```python
.
```
```python
header
```
```python
(
```
```python
"User-Agent"
```
```python
)
```
```python
==
```
```python
null
```
```python
) {
      result
```
```python
.
```
```python
header
```
```python
(
```
```python
"User-Agent"
```
```python
, Version
```
```python
.
```
```python
userAgent());
    }
```
```python
return
```
```python
result
```
```python
.
```
```python
build();
  }
```
下面分析下cache是否可用的判断逻辑，也就是下面这行代码的执行逻辑。
cacheStrategy = new CacheStrategy.Factory(now, request, cacheCandidate).get();
```python
public
```
```python
final
```
```python
class
```
```python
CacheStrategy {
```
```python
// 构造方法，nowMillis为传入的系统此刻时间
```
```python
public
```
```python
Factory
```
```python
(
```
```python
long
```
```python
nowMillis, Request request, Response cacheResponse) {
```
```python
this
```
```python
.nowMillis = nowMillis;
```
```python
this
```
```python
.request = request;
```
```python
this
```
```python
.cacheResponse = cacheResponse;
```
```python
if
```
```python
(cacheResponse !=
```
```python
null
```
```python
) {
```
```python
// 取出response中的headers
```
```python
Headers headers = cacheResponse.headers();
```
```python
// 遍历所有headers，解析出与cache过期有关的headers，并给相应成员变量赋值
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
, size = headers.size(); i < size; i++) {
          String fieldName = headers.name(i);
          String
```
```python
value
```
```python
= headers.
```
```python
value
```
```python
(i);
```
```python
if
```
```python
(
```
```python
"Date"
```
```python
.equalsIgnoreCase(fieldName)) {
```
```python
// Date header处理
```
```python
servedDate = HttpDate.parse(
```
```python
value
```
```python
);
            servedDateString =
```
```python
value
```
```python
;
          }
```
```python
else
```
```python
if
```
```python
(
```
```python
"Expires"
```
```python
.equalsIgnoreCase(fieldName)) {
```
```python
// Expires header处理
```
```python
expires = HttpDate.parse(
```
```python
value
```
```python
);
          }
```
```python
else
```
```python
if
```
```python
(
```
```python
"Last-Modified"
```
```python
.equalsIgnoreCase(fieldName)) {
```
```python
// Last-Modified header处理
```
```python
lastModified = HttpDate.parse(
```
```python
value
```
```python
);
            lastModifiedString =
```
```python
value
```
```python
;
          }
```
```python
else
```
```python
if
```
```python
(
```
```python
"ETag"
```
```python
.equalsIgnoreCase(fieldName)) {
```
```python
// ETag header处理
```
```python
etag =
```
```python
value
```
```python
;
          }
```
```python
else
```
```python
if
```
```python
(
```
```python
"Age"
```
```python
.equalsIgnoreCase(fieldName)) {
```
```python
// Age header处理
```
```python
ageSeconds = HeaderParser.parseSeconds(
```
```python
value
```
```python
, -
```
```python
1
```
```python
);
          }
```
```python
else
```
```python
if
```
```python
(OkHeaders.SENT_MILLIS.equalsIgnoreCase(fieldName)) {
            sentRequestMillis = Long.parseLong(
```
```python
value
```
```python
);
          }
```
```python
else
```
```python
if
```
```python
(OkHeaders.RECEIVED_MILLIS.equalsIgnoreCase(fieldName)) {
            receivedResponseMillis = Long.parseLong(
```
```python
value
```
```python
);
          }
        }
      }
    }
```
```python
public
```
```python
CacheStrategy
```
```python
get
```
```python
() {
```
```python
// CacheStrategy生成的主要方法
```
```python
CacheStrategy candidate = getCandidate();
```
```python
if
```
```python
(candidate.networkRequest !=
```
```python
null
```
```python
&& request.cacheControl().onlyIfCached()) {
```
```python
// 网络被用户禁止使用，并且cache不可用，此时networkRequest和cacheResponse都为null
```
```python
return
```
```python
new
```
```python
CacheStrategy(
```
```python
null
```
```python
,
```
```python
null
```
```python
);
      }
```
```python
return
```
```python
candidate;
    }
```
```python
private
```
```python
CacheStrategy
```
```python
getCandidate
```
```python
() {
```
```python
// 没有此request的cache response
```
```python
if
```
```python
(cacheResponse ==
```
```python
null
```
```python
) {
```
```python
return
```
```python
new
```
```python
CacheStrategy(request,
```
```python
null
```
```python
);
      }
```
```python
// 对于HTTPS，必须有handshake字段，否则认为此cache不可用
```
```python
if
```
```python
(request.isHttps() && cacheResponse.handshake() ==
```
```python
null
```
```python
) {
```
```python
return
```
```python
new
```
```python
CacheStrategy(request,
```
```python
null
```
```python
);
      }
```
```python
// 此response不能使用cache，比如金融类数据，一般追求实时性，不适合使用cache
```
```python
if
```
```python
(!isCacheable(cacheResponse, request)) {
```
```python
return
```
```python
new
```
```python
CacheStrategy(request,
```
```python
null
```
```python
);
      }
```
```python
// 使用cache前需要先验证一下保存的response，或者request中有条件GET的headers
```
```python
// noCache()方法命名不好，有歧义。它不是表示不能使用cache或者没有cache，而是表示使用前要先验证。
```
```python
CacheControl requestCaching = request.cacheControl();
```
```python
if
```
```python
(requestCaching.noCache() || hasConditions(request)) {
```
```python
return
```
```python
new
```
```python
CacheStrategy(request,
```
```python
null
```
```python
);
      }
```
```python
// 计算cache是否过期
```
```python
long
```
```python
ageMillis = cacheResponseAge();
```
```python
// 目前response生成时的绝对时间
```
```python
long
```
```python
freshMillis = computeFreshnessLifetime();
```
```python
// 到什么时候为止仍然是新鲜的，绝对时间
```
```python
if
```
```python
(requestCaching.maxAgeSeconds() != -
```
```python
1
```
```python
) {
        freshMillis = Math.min(freshMillis, SECONDS.toMillis(requestCaching.maxAgeSeconds()));
      }
```
```python
long
```
```python
minFreshMillis =
```
```python
0
```
```python
;
```
```python
// 剩余的最低的保鲜期，相对时间
```
```python
if
```
```python
(requestCaching.minFreshSeconds() != -
```
```python
1
```
```python
) {
        minFreshMillis = SECONDS.toMillis(requestCaching.minFreshSeconds());
      }
```
```python
long
```
```python
maxStaleMillis =
```
```python
0
```
```python
;
```
```python
// 最大过期时间，相对时间
```
```python
CacheControl responseCaching = cacheResponse.cacheControl();
```
```python
if
```
```python
(!responseCaching.mustRevalidate() && requestCaching.maxStaleSeconds() != -
```
```python
1
```
```python
) {
        maxStaleMillis = SECONDS.toMillis(requestCaching.maxStaleSeconds());
      }
```
```python
if
```
```python
(!responseCaching.noCache() && ageMillis + minFreshMillis < freshMillis + maxStaleMillis) {
        Response.Builder builder = cacheResponse.newBuilder();
```
```python
if
```
```python
(ageMillis + minFreshMillis >= freshMillis) {
          builder.addHeader(
```
```python
"Warning"
```
```python
,
```
```python
"110 HttpURLConnection \"Response is stale\""
```
```python
);
        }
```
```python
long
```
```python
oneDayMillis =
```
```python
24
```
```python
*
```
```python
60
```
```python
*
```
```python
60
```
```python
*
```
```python
1000
```
```python
L;
```
```python
if
```
```python
(ageMillis > oneDayMillis && isFreshnessLifetimeHeuristic()) {
          builder.addHeader(
```
```python
"Warning"
```
```python
,
```
```python
"113 HttpURLConnection \"Heuristic expiration\""
```
```python
);
        }
```
```python
// cache可用时，将networkRequest赋值为null，可以看出OKHttp是优先使用cache的
```
```python
return
```
```python
new
```
```python
CacheStrategy(
```
```python
null
```
```python
, builder.build());
      }
```
```python
// 条件GET的处理。
```
```python
// 条件GET一般是cache过期了，需要发送验证request给server，以判断cache response是否修改了。如果没有修改，还是可以接着使用cache的。
```
```python
Request.Builder conditionalRequestBuilder = request.newBuilder();
```
```python
if
```
```python
(etag !=
```
```python
null
```
```python
) {
        conditionalRequestBuilder.header(
```
```python
"If-None-Match"
```
```python
, etag);
      }
```
```python
else
```
```python
if
```
```python
(lastModified !=
```
```python
null
```
```python
) {
        conditionalRequestBuilder.header(
```
```python
"If-Modified-Since"
```
```python
, lastModifiedString);
      }
```
```python
else
```
```python
if
```
```python
(servedDate !=
```
```python
null
```
```python
) {
        conditionalRequestBuilder.header(
```
```python
"If-Modified-Since"
```
```python
, servedDateString);
      }
      Request conditionalRequest = conditionalRequestBuilder.build();
```
```python
return
```
```python
hasConditions(conditionalRequest)
          ?
```
```python
new
```
```python
CacheStrategy(conditionalRequest, cacheResponse)
          :
```
```python
new
```
```python
CacheStrategy(conditionalRequest,
```
```python
null
```
```python
);
    }
}
```
writeRequestHeaders()向HttpConnection的buffer中以UTF-8的编码格式写入start line和headers，合适的时机会发送到socket中传输出去
```python
public
```
```python
final
```
```python
class
```
```python
HttpTransport
```
```python
implements
```
```python
Transport
```
```python
{
```
```python
// 这个名字起的不好，这个方法不仅写入了headers，还写入了start line
```
```python
public
```
```python
void
```
```python
writeRequestHeaders
```
```python
(Request request)
```
```python
throws
```
```python
IOException {
```
```python
// 发送request之前必须立刻调用，它记录了发送request的系统时间
```
```python
httpEngine.writingRequestHeaders();
```
```python
// 生成start line，后面有详细分析
```
```python
String requestLine = RequestLine.get(
        request, httpEngine.getConnection().getRoute().getProxy().type());
```
```python
// 将start line和headers写入到buffer中，UTF-8格式，合适的时机再将buffer中数据通过socket传输出去
```
```python
httpConnection.writeRequest(request.headers(), requestLine);
  }
}
```
```python
public
```
```python
final
```
```python
class
```
```python
RequestLine
```
```python
{
```
```python
// 生成request的start line，Http协议中它的格式为 method url version
```
```python
static
```
```python
String get(Request request, Proxy.Type proxyType) {
    StringBuilder result =
```
```python
new
```
```python
StringBuilder();
```
```python
// 写入method
```
```python
result.append(request.method());
    result.append(
```
```python
' '
```
```python
);
```
```python
// 写入url
```
```python
if
```
```python
(includeAuthorityInRequestLine(request, proxyType)) {
      result.append(request.httpUrl());
    }
```
```python
else
```
```python
{
      result.append(requestPath(request.httpUrl()));
    }
```
```python
// 写入version，可以看到OKHttp支持的是HTTP/1.1版本
```
```python
result.append(
```
```python
" HTTP/1.1"
```
```python
);
```
```python
return
```
```python
result.toString();
  }
}
```
## 3 readResponse()源码分析
```python
public
```
```python
void
```
```python
readResponse
```
```python
()
```
```python
throws
```
```python
IOException {
```
```python
if
```
```python
(userResponse !=
```
```python
null
```
```python
) {
```
```python
// response已经有了，这可能是利用cache生成的response或其他情况，
```
```python
// 此时我们就不用去接收server端的response了，其实一般此时也没有server端的response让我们去接收，哈哈~
```
```python
return
```
```python
; 
    }
```
```python
if
```
```python
(networkRequest ==
```
```python
null
```
```python
&& cacheResponse ==
```
```python
null
```
```python
) {
```
```python
throw
```
```python
new
```
```python
IllegalStateException(
```
```python
"call sendRequest() first!"
```
```python
);
    }
```
```python
if
```
```python
(networkRequest ==
```
```python
null
```
```python
) {
```
```python
return
```
```python
;
```
```python
// No network response to read.
```
```python
}
    Response networkResponse;
```
```python
if
```
```python
(forWebSocket) {
```
```python
// 先将start line和header写入socket中
```
```python
transport.writeRequestHeaders(networkRequest);
```
```python
// 发送request，并读取response，后面详细分析
```
```python
networkResponse = readNetworkResponse();
    }
```
```python
else
```
```python
if
```
```python
(!callerWritesRequestBody) {
```
```python
// 先执行拦截器，再写入request到HttpConnection的buffer中，最后发送buffer，并读取response
```
```python
// 和上面情况比较像，这里就不展开分析了
```
```python
networkResponse =
```
```python
new
```
```python
NetworkInterceptorChain(
```
```python
0
```
```python
, networkRequest).proceed(networkRequest);
    }
```
```python
else
```
```python
{
```
```python
// 将request body的buffer发出去，这样requestBodyOut中就有了body
```
```python
if
```
```python
(bufferedRequestBody !=
```
```python
null
```
```python
&& bufferedRequestBody.buffer().size() >
```
```python
0
```
```python
) {
        bufferedRequestBody.emit();
      }
```
```python
// 处理request headers，并将start line和header写入socket中
```
```python
if
```
```python
(sentRequestMillis == -
```
```python
1
```
```python
) {
```
```python
if
```
```python
(OkHeaders.contentLength(networkRequest) == -
```
```python
1
```
```python
&& requestBodyOut
```
```python
instanceof
```
```python
RetryableSink) {
```
```python
// 如果之前content-length值不清楚，此时在body已经ready的情况下，可以计算出content-length，并将它添加到header中
```
```python
long
```
```python
contentLength = ((RetryableSink) requestBodyOut).contentLength();
          networkRequest = networkRequest.newBuilder()
              .header(
```
```python
"Content-Length"
```
```python
, Long.toString(contentLength))
              .build();
        }
```
```python
// 将start line和header写入socket中
```
```python
transport.writeRequestHeaders(networkRequest);
      }
```
```python
// 将body写入socket中
```
```python
if
```
```python
(requestBodyOut !=
```
```python
null
```
```python
) {
```
```python
if
```
```python
(bufferedRequestBody !=
```
```python
null
```
```python
) {
```
```python
// This also closes the wrapped requestBodyOut.
```
```python
bufferedRequestBody.close();
        }
```
```python
else
```
```python
{
          requestBodyOut.close();
        }
```
```python
if
```
```python
(requestBodyOut
```
```python
instanceof
```
```python
RetryableSink) {
```
```python
// body 写入socket中
```
```python
transport.writeRequestBody((RetryableSink) requestBodyOut);
        }
      }
```
```python
// 发送request，并读取response，后面会详细分析
```
```python
networkResponse = readNetworkResponse();
    }
```
```python
// 开始处理获取到的response
```
```python
// 读取并处理response的headers
```
```python
receiveHeaders(networkResponse.headers());
```
```python
// cache response存在的情况下，应该是cache过期了，做了一次条件GET来验证cache的内容是否有变更。
```
```python
// 根据Http协议，如果cache未变，回复304，not modified。且response中不会包含body，
```
```python
// 如果cache改变，回复200, OK。response中包含body
```
```python
if
```
```python
(cacheResponse !=
```
```python
null
```
```python
) {
```
```python
if
```
```python
(validate(cacheResponse, networkResponse)) {
```
```python
// 再验证通过，cache内容未变，使用cache构造response
```
```python
userResponse = cacheResponse.newBuilder()
            .request(userRequest)
            .priorResponse(stripBody(priorResponse))
            .headers(combine(cacheResponse.headers(), networkResponse.headers()))
            .cacheResponse(stripBody(cacheResponse))
            .networkResponse(stripBody(networkResponse))
            .build();
        networkResponse.body().close();
        releaseConnection();
```
```python
// 更新cache
```
```python
InternalCache responseCache = Internal.instance.internalCache(client);
        responseCache.trackConditionalCacheHit();
        responseCache.update(cacheResponse, stripBody(userResponse));
        userResponse = unzip(userResponse);
```
```python
return
```
```python
;
      }
```
```python
else
```
```python
{
```
```python
// cache未命中，response中会包含我们想要的body的。关闭cache body流
```
```python
closeQuietly(cacheResponse.body());
      }
    }
```
```python
// cache未命中，利用server返回的response string构造client使用的Response对象
```
```python
// 此时会将response缓存起来，以便下次使用
```
```python
userResponse = networkResponse.newBuilder()
        .request(userRequest)
        .priorResponse(stripBody(priorResponse))
        .cacheResponse(stripBody(cacheResponse))
        .networkResponse(stripBody(networkResponse))
        .build();
```
```python
if
```
```python
(hasBody(userResponse)) {
      maybeCache();
      userResponse = unzip(cacheWritingResponse(storeRequest, userResponse));
    }
  }
```
下面详细分析readNetworkResponse()，它会通过socket流读取response string的start line，headers和body。
```python
private
```
```python
Response
```
```python
readNetworkResponse
```
```python
()
```
```python
throws
```
```python
IOException {
```
```python
// 将HttpTransport中的buffer flush出去
```
```python
transport.finishRequest();
```
```python
// 读取server的response string，并构造出Response对象
```
```python
Response networkResponse = transport.readResponseHeaders()
        .request(networkRequest)
        .handshake(connection.getHandshake())
        .header(OkHeaders.SENT_MILLIS, Long.toString(sentRequestMillis))
        .header(OkHeaders.RECEIVED_MILLIS, Long.toString(System.currentTimeMillis()))
        .build();
```
```python
if
```
```python
(!forWebSocket) {
      networkResponse = networkResponse.newBuilder()
          .body(transport.openResponseBody(networkResponse))
          .build();
    }
```
```python
return
```
```python
networkResponse;
  }
```
```python
public
```
```python
final
```
```python
class
```
```python
HttpTransport
```
```python
implements
```
```python
Transport
```
```python
{
```
```python
@Override
```
```python
public
```
```python
Response.Builder
```
```python
readResponseHeaders
```
```python
()
```
```python
throws
```
```python
IOException {
```
```python
return
```
```python
httpConnection.readResponse();
  }
}
```
```python
public
```
```python
final
```
```python
class
```
```python
HttpConnection
```
```python
{
```
```python
public
```
```python
Response.Builder
```
```python
readResponse
```
```python
()
```
```python
throws
```
```python
IOException {
```
```python
if
```
```python
(state != STATE_OPEN_REQUEST_BODY && state != STATE_READ_RESPONSE_HEADERS) {
```
```python
throw
```
```python
new
```
```python
IllegalStateException(
```
```python
"state: "
```
```python
+ state);
    }
```
```python
try
```
```python
{
```
```python
while
```
```python
(
```
```python
true
```
```python
) {
```
```python
// 解析start line，response的start line格式为 protocol，code， message
```
```python
StatusLine statusLine = StatusLine.parse(source.readUtf8LineStrict());
```
```python
// 将解析出的protocol， code， message分别放入构造的Response对象中
```
```python
Response.Builder responseBuilder =
```
```python
new
```
```python
Response.Builder()
            .protocol(statusLine.protocol)
            .code(statusLine.code)
            .message(statusLine.message);
```
```python
// 解析response string的headers
```
```python
Headers.Builder headersBuilder =
```
```python
new
```
```python
Headers.Builder();
```
```python
// 一行行读取headers, 直到遇到空行结束
```
```python
readHeaders(headersBuilder);
        headersBuilder.add(OkHeaders.SELECTED_PROTOCOL, statusLine.protocol.toString());
```
```python
// 将headers添加到Response对象中
```
```python
responseBuilder.headers(headersBuilder.build());
```
```python
// 如果返回code不是100， continue，则可以直接将Response对象返回
```
```python
// 对于100，continue，server还会继续返回response string，我们需要在while循环中继续接收并解析
```
```python
if
```
```python
(statusLine.code != HTTP_CONTINUE) {
          state = STATE_OPEN_RESPONSE_BODY;
```
```python
return
```
```python
responseBuilder;
        }
      }
    }
```
```python
catch
```
```python
(EOFException e) {
```
```python
// Provide more context if the server ends the stream before sending a response.
```
```python
IOException exception =
```
```python
new
```
```python
IOException(
```
```python
"unexpected end of stream on "
```
```python
+ connection
          +
```
```python
" (recycle count="
```
```python
+ Internal.instance.recycleCount(connection) +
```
```python
")"
```
```python
);
      exception.initCause(e);
```
```python
throw
```
```python
exception;
    }
  }
}
```
## 4 followUpRequest()源码分析
client发送一个request之后，server可能回复一个重定向的response，并在这个response中告知client需要重新访问的server的IP。此时，client需要重新向新的server发送request，并等待新server的回复。所以我们需要单独判断重定向response，并发送多次request。有了OKHttp，这一切你都不用管，它会自动帮你完成所有这一切。OKHttp中followUpRequest()方法就是完成这个功能的。是不是瞬间感觉OKHttp强大到不要不要的啊~。这个方法流程比较简单，就不给出流程图了。亲们如果对Http协议不熟悉，可以先看我的这篇文章[Http协议简介](http://blog.csdn.net/u013510838/article/details/52431558)
```python
public
```
```python
Request followUpRequest() throws IOException {
```
```python
if
```
```python
(userResponse ==
```
```python
null
```
```python
)
```
```python
throw
```
```python
new
```
```python
IllegalStateException();
    Proxy selectedProxy = getRoute() !=
```
```python
null
```
```python
? getRoute().getProxy()
        :
```
```python
client
```
```python
.getProxy();
```
```python
int
```
```python
responseCode = userResponse.code();
```
```python
// 利用responseCode来分析是否需要自动发送后续request
```
```python
switch
```
```python
(responseCode) {
```
```python
// 未认证用户，不能访问server或代理，故需要发送认证的request
```
```python
case
```
```python
HTTP_PROXY_AUTH:
```
```python
if
```
```python
(selectedProxy.type() != Proxy.Type.HTTP) {
```
```python
throw
```
```python
new
```
```python
ProtocolException(
```
```python
"Received HTTP_PROXY_AUTH (407) code while not using proxy"
```
```python
);
        }
```
```python
case
```
```python
HTTP_UNAUTHORIZED:
```
```python
return
```
```python
OkHeaders.processAuthHeader(
```
```python
client
```
```python
.getAuthenticator(), userResponse, selectedProxy);
```
```python
// 永久重定向，暂时重定向，永久移动了等和重定向相关的response,response code中以3打头的都是
```
```python
// 它们需要重新发送request给新的server，新sever的ip在response中会给出
```
```python
case
```
```python
HTTP_PERM_REDIRECT:
```
```python
case
```
```python
HTTP_TEMP_REDIRECT:
```
```python
if
```
```python
(!userRequest.method().equals(
```
```python
"GET"
```
```python
) && !userRequest.method().equals(
```
```python
"HEAD"
```
```python
)) {
```
```python
return
```
```python
null
```
```python
;
        }
```
```python
case
```
```python
HTTP_MULT_CHOICE:
```
```python
case
```
```python
HTTP_MOVED_PERM:
```
```python
case
```
```python
HTTP_MOVED_TEMP:
```
```python
case
```
```python
HTTP_SEE_OTHER:
```
```python
// Does the client allow redirects?
```
```python
if
```
```python
(!
```
```python
client
```
```python
.getFollowRedirects())
```
```python
return
```
```python
null
```
```python
;
```
```python
// 新的server的IP地址在response的Location header中给出
```
```python
String location = userResponse.header(
```
```python
"Location"
```
```python
);
```
```python
if
```
```python
(location ==
```
```python
null
```
```python
)
```
```python
return
```
```python
null
```
```python
;
        HttpUrl url = userRequest.httpUrl().resolve(location);
```
```python
// Don't follow redirects to unsupported protocols.
```
```python
if
```
```python
(url ==
```
```python
null
```
```python
)
```
```python
return
```
```python
null
```
```python
;
```
```python
// If configured, don't follow redirects between SSL and non-SSL.
```
```python
boolean
```
```python
sameScheme = url.scheme().equals(userRequest.httpUrl().scheme());
```
```python
if
```
```python
(!sameScheme && !
```
```python
client
```
```python
.getFollowSslRedirects())
```
```python
return
```
```python
null
```
```python
;
```
```python
// Redirects don't include a request body.
```
```python
Request.Builder requestBuilder = userRequest.newBuilder();
```
```python
if
```
```python
(HttpMethod.permitsRequestBody(userRequest.method())) {
          requestBuilder.method(
```
```python
"GET"
```
```python
,
```
```python
null
```
```python
);
          requestBuilder.removeHeader(
```
```python
"Transfer-Encoding"
```
```python
);
          requestBuilder.removeHeader(
```
```python
"Content-Length"
```
```python
);
          requestBuilder.removeHeader(
```
```python
"Content-Type"
```
```python
);
        }
```
```python
// 删掉用户认证信息
```
```python
if
```
```python
(!sameConnection(url)) {
          requestBuilder.removeHeader(
```
```python
"Authorization"
```
```python
);
        }
```
```python
return
```
```python
requestBuilder.url(url).build();
```
```python
default
```
```python
:
```
```python
return
```
```python
null
```
```python
;
    }
  }
```
## 5 总结
OKHttp底层源码还是相当复杂的，毕竟它的功能如此之强大嘛。OKHttp默认采用了Keep-Alive持久连接技术，可支持gzip编码的response。在cache的处理上，如果cache可用，则直接使用cache，否则使用网络数据。OKHttp会做cache过期的判断和过期后的再验证。有了OKHttp，这一切你都不用管，它帮你cover掉了！
当需要做用户验证和重定向时，我们一般需要发送认证request，或向新server发送request，也就是要重新再生成新request并发送出去。有了OKHttp，这一切你都不用管，它又帮你cover掉了！
OKHttp功能实在是强大到爆表，掌握好了它的实现原理和底层流程之后，你就可以在项目中游刃有余的放心使用它了！另外，小编可是花了整个周末才完成了这几篇文章，走过路过的朋友帮忙写写评论吧，谢谢！

