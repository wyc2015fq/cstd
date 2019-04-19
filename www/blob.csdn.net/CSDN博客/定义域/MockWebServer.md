# MockWebServer - 定义域 - CSDN博客
2016年02月23日 14:15:32[定义域](https://me.csdn.net/u010278882)阅读数：1492
# MockWebServer
> 
okhttp-mockwebserver 参考
MockWebServer 是一个可脚本化的用于测试HTTP客户端的Web服务器。
## 目标
使用该工具，你可以轻松测试你的应用在进行HTTP或HTTPS调用时是否按照预期的行为动作。使用该工具，你可以验证应用的请求是否符合预期，你可以选择返回的响应。
因为该工具测试了你的全部HTTP栈，所以你可以对你的测试的抱有信心。你甚至可以直接将真实Web服务器中的HTTP响应复制粘贴过来，以创建相应的测试用例。此外，还可以测试你的应用在糟糕情况下的表现，比如500错误或者响应返回缓慢。
## 示例
使用 MockWebServer 的方式与使用模拟框架，比如[Mockito](https://code.google.com/p/mockito/) 的方式类似：
- 编写模拟脚本
- 运行应用程序代码
- 验证做出的请求是否符合预期
下面是一个完整的示例：
```java
public void test() throws Exception() {
    // 创建一个 MockWebServer，因为足够的简单所以你可以创建一个新的实例用于单元测试
    MockWebServer server = new MockWebServer();
    // 设定一些响应
    server.enqueue(new MockResponse().setBody("hello, world!"));
    server.enqueue(new MockResponse().setBody("sup, bra?"));
    server.enqueue(new MockResponse().setBody("yo dog"));
    // 启动服务
    server.start();
    // 设置服务端的URL，客户端请求中使用
    HttpUrl baseUrl = server.url("/v1/chat");
    // 运行你的应用程序代码，进行HTTP请求
    // 响应会按照上面设置中放入队列的顺序被返回
    Chat chat = new Chat(baseUrl);
    chat.loadMore();
    assertEquals("hello, world!", chat.message());
    chat.loadMore();
    chat.loadMore();
    assertEquals(""
                + "hello, world!\n"
                + "sup, bra?\n"
                + "yo dog", chat.message());
    // 可选：确认你的应用做出了正确的请求
    RecordedRequest request1 = server.takeRequst();
    assertEquals("/v1/chat/messages/", request1.getPath());
    assertNotNull(request1.getHeader("Authorization"));
    RecordedRequest request2 = server.takeRequest();
    assertEquals("/v1/chat/message/2", request.getPath());
    RecordedRequest request3 = server.takeRequest();
    assertEquals("/v1/chat/message/3", request.getPath());
    // 关闭服务，因为不能重用
    server.shutdown();
}
```
单元测试中，可能需要把 server 作为一个字段，然后在 tearDown() 方法中关闭服务。
## API
### MockResponse
模拟响应默认情况是一个响应体为空，响应状态码为 200。响应体可以进行设置，可以设置为字符串、输入流或字节数组。还可以通过流式 API 添加响应头。
```java
MockResponse response = new MockResponse()
    .addHeader("Content-Type", "application/json,charset=utf-8")
    .addHeader("Cache-Control", "no-cache")
    .setBody("{}");
```
模拟响应还可以用来模拟低速率网络的情况。这一点在测试超时和交互式测试时非常有用。
```java
response.throttleBody(1024, 1, TimeUnit.SECONDS);
```
### RecordedRequest
可以验证请求的请求方法、路径、HTTP版本、请求体、请求头。
```java
RecordedRequest request = server.takeRequest();
assertEquals("POST /v1/chat/send HTTP/1.1", request.getRequestLine());
assertEquals("application/json; charset=utf-8", request.getHeader("Content-Type"));
assertEquals("{}", request.getUtf8Body());
```
### Dispatcher
默认情况下 MockWebServer 使用队列来指定响应。对于需要另一种响应策略的请求，可以通过转发器来处理器。可以通过请求的路径来选择转发策略。比如，可以过滤请求替代 server.enqueue()。
```java
final Dispatcher dispatcher = new Dispatcher(){
    @Override
    public MockResponse dispatch(RecordedRequest request) throws InterruptedExcption() {
        if(request.getPath().equals("/v1/login/auth")) {
            return new MockResponse().setResponseCode(200);
        } else if(request.getPath().equals("/v1/check/version")) {
            return new MockResponse().setBody("version=9");
        } else if(request.getPath().equals("/v1/profile/info")) {
            return new MockResponse().setBody("{\\\"info\\\":{\"name\":\"Lucas Albuqureque\", \"age\": \"21\", \"gender\": \"male\"}}");
        }
        return new MockResponse().setResponseCode(404);
    }
};
server.setDispatcher(dispatcher);
```
