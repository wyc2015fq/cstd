# MockWebServer API Javadoc - 定义域 - CSDN博客
2016年02月23日 16:41:23[定义域](https://me.csdn.net/u010278882)阅读数：799
# MockWebServer API Javadoc
主要翻译 MockWebServer 2.7.4 `com.squareup.okhttp.mockwebserver` 包下的类。
- Dispatcher 转发器：模拟服务器的请求处理器
- MockResponse 模拟响应：可以被模拟服务器回放的可配置的响应
- MockWebServe 模拟服务器：可以通过脚本配置的Web服务器
- PushPromise 服务器创建的HTTP请求
- QueueDispatcher 队列转发器：默认的转发器实现
- RecordedRequest 请求记录：发往模拟服务器的HTTP请求
- SocketPolicy 套接字策略
## MockWebServer
```java
public final class MockWebSocket
    extends Object
    implements org.junit.rules.TestRule
// 构造器
MockWebServer()
// 方法
void start() // 等价于 start(0)
void start(int port) // 启动服务并指定端口号，传入0则自动使用未被占用的端口号
void start(InetAddress inetAddress, int port) // ? 不知道何时使用
void shutdown() // 关闭服务
// post
void setBodyLimit(long maxBodyLength) // 设置用于保存Post body的内存字节大小
// url
HttpUrl url(String path) // 传入服务路径，返回用于链接服务的完整URL
String getHostName()
int getPort()
// response
void enqueue(MockResponse response) // 配置返回的请求队列
void setDispatcher() // 设置转发器，对符合条件的请求进行转发
// request
int getRequestCount()
RecordedRequest takeRequest()
RecordedRequest takeRequest(long timeout, TimeUnit unit)
// https
void useHttps(SSLSocketFactory sslSocketFactory, boolean tunnelProxy) // 仅对HTTPS请求进行服务
void setProtocols(List<Protocol> protocols)     
void setProtocolNegotiationEnabled(boolean protocolNegotiationEnabled)
// proxy
void toProxyAddress()
```
## MockResponse
```java
public final class MockResponse
    extends Object
    implements Clonable
// 构造器
MockResponse() 创建一个模拟响应，响应代码为200，响应体为空内容
// 响应码
MockResponse setResponseCode()
// 响应状态
MockResponse setStatus(String status)
Strign getStatus()
// 响应头
MockResponse addHeader(String header) // header 格式"name:value"
MockResponse addHeader(String name, Object value) // 添加响应头，可以添加多个同名响应头
MockResponse addHeaderLenient(String name, Object value) // 同addHeader(name, value)，但是不会对响应头名进行验证
MockResponse setHeader(String name, Object value) // 移除掉所有名为 name 的头，然后添加头
MockResponse setHeaders(Headers headers)
MockResponse clearHeaders() // 移除所有的响应头，包括默认添加的"Content-Length"和"Transfer-encoding"
MockResponse removeHeader(String name) // 移除所有名为 name 的响应头
Headers getHeaders()
// 响应体
MockResponse setBody(String body) // 设置响应体为UTF-8编码的字节
MockResponse setBody(okio.Buffer body)
MockResponse setChunkedBody(String body, int maxChunkedSize) // 将响应体分块传播（响应头的 Transfer-encoding = chunked），分块大小通过第二个参数指定
MockResponse setChunkedBody(okio.Buffer body, int maxChunkedSize)
MockResponse setBodyDelay(long delay, TimeUnit unit) // 设置响应体的响应延迟
MockResponse throttleBody(long bytesPerPeriod, long period, TimeUnit unit) // 设置响应体的传输方式是没隔指定的时间就传输指定大小的响应
oki.Buffer getBody()
long getBodyDelay()
long getThrottleBytesPerPeriod()
long getThrottlePeriod()
// 特殊响应
MockResponse withPush(PushPromise promise) // 当MockWebServer的protocols中包含Protocol.HTTP_2时，响应将会附着一个推送流
MockResponse withSettings(Settings settings) // 当MockWebServer的protocols中包含Protocol.HTTP/2时，则会在响应前推动settings
MockResponse withWebSocketUpgrade(WebSocketListener listener) // 尝试执行一个WebSocket以升级Connection
Settings getSettings()
List<PushPromise> getPushPromise() // 返回服务将会返回的推动流
// 其他
MockResponse clone()
MockResponse setSocketPolicy(SocketPolicy policy) //?
SocketPolicy getSocketPolicy()
```
## RecordedRequest
```java
public final class RecordedRequest
    extends Object
// 构造器
RecordedRequest(String requestLine, Headers headers, List<Integer> chunkSizes, long bodySize, okio.Buffer body, int sequenceNumber, Socket socket)
String getMethod() // 返回请求方法
String getPath()
String getRequestLine() // 返回请求行
Headers getHeaders()
String getHeader(String name)
List<Integer> getChunkSize() // 返回请求的分段大小，如果请求没有分段或者为空则返回空的List
long getBodySize() // 返回服务器接收的POST请求的大小（在服务器截断之前 MockWebServer#setBodyLimit）
String getBody() // 返回POST body
String getBody().readUtf8()
int getSequenceNumber() // 返回该请求是HTTP连接中的第几个
TlsVersion getTlsVersion() // 返回 Tls 的版本，如果没有使用则返回 null
```
## Dispatcher
```java
public abstract class Dispatcher
    extends Object
// 构造器
Dispatcher()
public abstract MockResponse dispatch(RecordedRequest request) // 对满足条件的请求进行响应
public MockResponse peek() // 返回一个对下一个请求的进行猜测后给出的响应，默认返回一个空响应
```
## QueueDispatcher
```java
public class QueueDispatcher
    extends Dispatcher
// 构造器
QueueDispatcher()
// 字段
protected BlockingQueue<MockResponse> responseQueues
// 方法
MockResponse dispatcher(RecordedRequest request) // 对满足条件的请求进行响应
void enqueueResponse(MockResponse response) 
void setFailFast(boolean failFast) //?
void setFailFast(MockResponse mockReponse) //?
```
## PushPromise
```java
public final class PushPromise
    extends Object
PushPromise(String method, String path, Headers headers, MockResponse reponse)
String getMethod()
String getPath()
Headers getHeaders()
MockResponse getResponse()
```
## SocketPolicy
```java
public enum SocketPolicy
    extends Enum<SocketPolicy>
static SocketPolicy valueOf(String name)
static SocketPolicy[] values()
DISCONNECT_AFTER_REQUEST // 请求后中断连接(3)
DISCONNECT_AT_END // 响应后中断连接(5)
DISCONNECT_AT_START // 一旦有请求就中断连接，请求还未读取(1)
DISCONNECT_DURING_REQUEST_BODY // 读取请求体的一半内容后中断连接(2)
DISCONNECT_DURING_RESPONSE_BODY // 写入响应体的一半时中断连接(4)
FAIL_HANDSHAKE // SSL 握手过程中不信任客户端
KEEP_OPEN // 响应后也保持Socket打开(6)
NO_RESPONSE // 保持 Socket打开但不进行任何响应(7)
SHUTDOWN_INPUT_AT_END // 发送响应后关闭 Socket 的输入(8)
SHUTDOWN_OUTPUT_AT_END // 发送响应后关闭 Socket 的输出(9)
UPGRADE_TO_SSL_AT_END // 在请求/响应对完成后用SSL封装Socket
```
![这里写图片描述](https://img-blog.csdn.net/20160223164056954)
