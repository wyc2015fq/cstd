# SRWebSocket源码浅析 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月12日 09:26:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2495
![](http://upload-images.jianshu.io/upload_images/2702646-0daec129db3a6bc1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 本文来自简书，原文地址:[http://www.jianshu.com/p/cdb7a886789a](http://www.jianshu.com/p/cdb7a886789a)
#### 一. 前言：
- WebSocket协议是基于`TCP`的一种新的网络协议。它实现了浏览器与服务器全双工(full-duplex)通信——可以通俗的解释为服务器主动发送信息给客户端。
- 区别于`MQTT`、`XMPP`等聊天的应用层协议，它是一个传输通讯协议。它有着自己一套连接握手，以及数据传输的规范。
- 而本文要讲到的`SRWebSocket`就是iOS中使用`websocket`必用的一个框架，它是用`Facebook`提供的。
关于WebSocket起源与发展，是怎么由：轮询、长轮询、再到websocket的，可以看看冰霜这篇文章：
[微信,QQ这类IM app怎么做——谈谈Websocket](http://www.jianshu.com/p/bcefda55bce4)
关于SRWebSocket的API用法，可以看看楼主之前这篇文章：
[iOS即时通讯，从入门到“放弃”？](http://www.jianshu.com/p/2dbb360886a8)
#### 二. SRWebSocket的对外的业务流程：
首先贴一段`SRWebSocket`的API调用代码：
```
//初始化socket并且连接
- (void)connectServer:(NSString *)server port:(NSString *)port
{
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"ws://%@:%@",server,port]]];
    _socket = [[SRWebSocket alloc] initWithURLRequest:request];
    _socket.delegate = self;
    [_socket open];
}
- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessage:(id)message
{
}
- (void)webSocketDidOpen:(SRWebSocket *)webSocket
{
}
- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error
{
}
- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean
{
}
```
要简单使用起来，总共就4行代码，并且实现你需要的代理即可，整个业务逻辑非常简洁。
但是就这么几个对外的方法，`SRWebSocket.m`里面用了2000行代码来进行封装，那么它到底做了什么？我们接着往下看：
#### 三. SRWebSocket的初始化以及连接流程：
###### 1首先我们初始化：
```
//初始化
- (void)_SR_commonInit;
{
    //得到url schem小写
    NSString *scheme = _url.scheme.lowercaseString;
        //如果不是这几种，则断言错误
    assert([scheme isEqualToString:@"ws"] || [scheme isEqualToString:@"http"] || [scheme isEqualToString:@"wss"] || [scheme isEqualToString:@"https"]);
    _readyState = SR_CONNECTING;
    _webSocketVersion = 13;
     //初始化工作的队列，串行
    _workQueue = dispatch_queue_create(NULL, DISPATCH_QUEUE_SERIAL);
    //给队列设置一个标识,标识为指向自己的，上下文对象为这个队列
    dispatch_queue_set_specific(_workQueue, (__bridge void *)self, maybe_bridge(_workQueue), NULL);
    //设置代理queue为主队列
    _delegateDispatchQueue = dispatch_get_main_queue();
    //retain主队列？
    sr_dispatch_retain(_delegateDispatchQueue);
    //读Buffer
    _readBuffer = [[NSMutableData alloc] init];
    //输出Buffer
    _outputBuffer = [[NSMutableData alloc] init];
    //当前数据帧
    _currentFrameData = [[NSMutableData alloc] init];
    //消费者数据帧的对象
    _consumers = [[NSMutableArray alloc] init];
    _consumerPool = [[SRIOConsumerPool alloc] init];
    //注册的runloop
    _scheduledRunloops = [[NSMutableSet alloc] init];
    ....省略了一部分代码
}
```
会初始化一些属性：
- 包括对schem进行断言，只支持ws/wss/http/https四种。
- 当前socket状态，是正在连接，还是已连接、断开等等。
- 初始化工作队列，以及流回调线程等等。
- 初始化读写缓冲区：`_readBuffer`、`_outputBuffer`。
###### 2. 输入输出流的创建及绑定：
```
//初始化流
- (void)_initializeStreams;
{
    //断言 port值小于UINT32_MAX
    assert(_url.port.unsignedIntValue <= UINT32_MAX);
    //拿到端口
    uint32_t port = _url.port.unsignedIntValue;
    //如果端口号为0，给个默认值，http 80 https 443;
    if (port == 0) {
        if (!_secure) {
            port = 80;
        } else {
            port = 443;
        }
    }
    NSString *host = _url.host;
    CFReadStreamRef readStream = NULL;
    CFWriteStreamRef writeStream = NULL;
    //用host创建读写stream,Host和port就绑定在一起了
    CFStreamCreatePairWithSocketToHost(NULL, (__bridge CFStringRef)host, port, &readStream, &writeStream);
    //绑定生命周期给ARC  _outputStream = __bridge transfer
    _outputStream = CFBridgingRelease(writeStream);
    _inputStream = CFBridgingRelease(readStream);
    //代理设为自己
    _inputStream.delegate = self;
    _outputStream.delegate = self;
}
```
在这里，我们根据传进来的url，类似`ws://localhost:80`,进行输入输出流CFStream的创建及绑定。
![](http://upload-images.jianshu.io/upload_images/2702646-5e53e4be0cb08ada.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Output&Iput.png
到这里，初始化工作就完成了，接着我们调用了`open`开始建立连接：
```
//开始连接
- (void)open;
{
    assert(_url);
    //如果状态是正在连接，直接断言出错
    NSAssert(_readyState == SR_CONNECTING, @"Cannot call -(void)open on SRWebSocket more than once");
    //自己持有自己
    _selfRetain = self;
    //判断超时时长
    if (_urlRequest.timeoutInterval > 0)
    {
        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, _urlRequest.timeoutInterval * NSEC_PER_SEC);
        //在超时时间执行
        dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
            //如果还在连接，报错
            if (self.readyState == SR_CONNECTING)
                [self _failWithError:[NSError errorWithDomain:@"com.squareup.SocketRocket" code:504 userInfo:@{NSLocalizedDescriptionKey: @"Timeout Connecting to Server"}]];
        });
    }
    //开始建立连接
    [self openConnection];
}
```
open方法定义了一个超时，如果超时了还在`SR_CONNECTING`，则报错，并且断开连接，清除一些已经初始化好的参数。
```
//开始连接
- (void)openConnection;
{
    //更新安全、流配置
    [self _updateSecureStreamOptions];
    //判断有没有runloop
    if (!_scheduledRunloops.count) {
        //SR_networkRunLoop会创建一个带runloop的常驻线程，模式为NSDefaultRunLoopMode。
        [self scheduleInRunLoop:[NSRunLoop SR_networkRunLoop] forMode:NSDefaultRunLoopMode];
    }
    //开启输入输出流
    [_outputStream open];
    [_inputStream open];
}
- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode;
{
    [_outputStream scheduleInRunLoop:aRunLoop forMode:mode];
    [_inputStream scheduleInRunLoop:aRunLoop forMode:mode];
    //添加到集合里，数组
    [_scheduledRunloops addObject:@[aRunLoop, mode]];
}
```
开始连接主要是给输入输出流绑定了一个`runloop`，说到这个`runloop`，不得不提一下`SRWebSocket`线程的问题：
- 一开始初始化我们提过`SRWebSocket`有一个工作队列：
`dispatch_queue_t _workQueue;`
这个工作队列是串行的，所有和控制有关的操作，除了一开始初始化和`open`操作外，所有后续的回调操作，数据写入与读取，出错连接断开，清除一些参数等等这些操作，全部是在这个`_workQueue`中进行的。
- 
而这里的`runloop`:
```
+ (NSRunLoop *)SR_networkRunLoop {
 static dispatch_once_t onceToken;
 dispatch_once(&onceToken, ^{
     networkThread = [[_SRRunLoopThread alloc] init];
     networkThread.name = @"com.squareup.SocketRocket.NetworkThread";
     [networkThread start];
     //阻塞方式拿到当前runloop
     networkRunLoop = networkThread.runLoop;
 });
 return networkRunLoop;
}
```
是新创建了一个`NSThread`的线程，然后起了一个`runloop`，这个是以单例的形式创建的，所以`networkThread`作为属性是一直存在的，而且起了一个`runloop`，这个`runloop`没有调用过退出的逻辑，所以这个`networkThread`是个常驻线程，即使`socket`连接断开，即使`SRWebSocket`对象销毁，这个常驻线程仍然存在。
可能很多朋友会觉得，那我都不用`websocket`了，什么都置空了，凭什么还有一个常驻线程，不停的空转，给内存和CPU造成一定开销呢？
楼主的理解是，作者这么做，可能考虑的是既然用户有长连接的需求，肯定断开连接甚至清空`websocket`对象只是一时的选择，肯定是很快会重新初始化并且重连的，这样这个常驻线程就可以得到复用，省去了重复创建，以及获取`runloop`等开销。
- 
那么`SRWebSocket`总共就有一个串行的`_workQueue`和一个常驻线程`networkThread`,前者用来控制连接，后者用来注册输入输出流，那么为什么这些操作不在一个常驻线程中去做呢？
我觉得这里就涉及一个线程的任务调度问题了，试想，如果控制逻辑和输入输出流的回调都是在同一个线程，对于输入输出流来说，回调是会非常频繁的，首先写`_outputStream`是在当前流`NSStreamEventHasSpaceAvailable`还有空间可写的时候，一直会回调，而读`_inputStream`则在有数据到达时候，也会不停的回调，试想如果这时候，控制逻辑需要做什么处理，是不是会有很大的延迟？它需要等到排在它前面插入线程中的任务调度完毕，才能轮得到这些控制逻辑的执行。所以在这里，把控制逻辑放在一个串行队列，而数据流的回调放在一个常驻线程，两个线程不会互相污染，各司其职。
接着主流程往下走，我们open了输入输出流后，就调用到了流的代理方法了：
```
//开启流后，收到事件回调
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
{
    __weak typeof(self) weakSelf = self;
    // 如果是ssl,而且_pinnedCertFound 为NO，而且事件类型是有可读数据未读，或者事件类型是还有空余空间可写
    if (_secure && !_pinnedCertFound && (eventCode == NSStreamEventHasBytesAvailable || eventCode == NSStreamEventHasSpaceAvailable)) {
           //省略SSL的一些处理....
            //如果为NO，则验证失败，报错关闭
            if (!_pinnedCertFound) {
                  //关闭连接
                dispatch_async(_workQueue, ^{
                    NSDictionary *userInfo = @{ NSLocalizedDescriptionKey : @"Invalid server cert" };
                    [weakSelf _failWithError:[NSError errorWithDomain:@"org.lolrus.SocketRocket" code:23556 userInfo:userInfo]];
                });
                return;
            } else if (aStream == _outputStream) {
                //如果流是输出流，则打开流成功
                dispatch_async(_workQueue, ^{
                    [self didConnect];
                });
            }
        }
    }
    dispatch_async(_workQueue, ^{
        [weakSelf safeHandleEvent:eventCode stream:aStream];
    });
}
```
这里如果我们一开始初始化的url是 wss/https，会做SSL认证，认证流程基本和楼主之前讲的`CocoaAsyncSocket`，这里就不赘述了，认证失败，会断开连接，
最终SSL或者非SSL都会走到这么一个方法：
```
//流打开成功后的操作，开始发送http请求建立连接
- (void)didConnect;
{
    SRFastLog(@"Connected");
    //创建一个http request  url
    CFHTTPMessageRef request = CFHTTPMessageCreateRequest(NULL, CFSTR("GET"), (__bridge CFURLRef)_url, kCFHTTPVersion1_1);
    // Set host first so it defaults
    //设置head, host:  url+port
    CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Host"), (__bridge CFStringRef)(_url.port ? [NSString stringWithFormat:@"%@:%@", _url.host, _url.port] : _url.host));
    //密钥数据（生成对称密钥）
    NSMutableData *keyBytes = [[NSMutableData alloc] initWithLength:16];
    //生成随机密钥
    SecRandomCopyBytes(kSecRandomDefault, keyBytes.length, keyBytes.mutableBytes);
    //根据版本用base64转码
    if ([keyBytes respondsToSelector:@selector(base64EncodedStringWithOptions:)]) {
        _secKey = [keyBytes base64EncodedStringWithOptions:0];
    } else {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        _secKey = [keyBytes base64Encoding];
#pragma clang diagnostic pop
    }
    //断言编码后长度为24
    assert([_secKey length] == 24);
    // Apply cookies if any have been provided
    //提供cookies
    NSDictionary * cookies = [NSHTTPCookie requestHeaderFieldsWithCookies:[self requestCookies]];
    for (NSString * cookieKey in cookies) {
        //拿到cookie值
        NSString * cookieValue = [cookies objectForKey:cookieKey];
        if ([cookieKey length] && [cookieValue length]) {
            //设置到request的 head里
            CFHTTPMessageSetHeaderFieldValue(request, (__bridge CFStringRef)cookieKey, (__bridge CFStringRef)cookieValue);
        }
    }
    // set header for http basic auth
    //设置http的基础auth,用户名密码认证
    if (_url.user.length && _url.password.length) {
        NSData *userAndPassword = [[NSString stringWithFormat:@"%@:%@", _url.user, _url.password] dataUsingEncoding:NSUTF8StringEncoding];
        NSString *userAndPasswordBase64Encoded;
        if ([keyBytes respondsToSelector:@selector(base64EncodedStringWithOptions:)]) {
            userAndPasswordBase64Encoded = [userAndPassword base64EncodedStringWithOptions:0];
        } else {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
            userAndPasswordBase64Encoded = [userAndPassword base64Encoding];
#pragma clang diagnostic pop
        }
        //编码后用户名密码
        _basicAuthorizationString = [NSString stringWithFormat:@"Basic %@", userAndPasswordBase64Encoded];
        //设置head Authorization
        CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Authorization"), (__bridge CFStringRef)_basicAuthorizationString);
    }
    //web socket规范head
    CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Upgrade"), CFSTR("websocket"));
    CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Connection"), CFSTR("Upgrade"));
    CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Sec-WebSocket-Key"), (__bridge CFStringRef)_secKey);
    CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Sec-WebSocket-Version"), (__bridge CFStringRef)[NSString stringWithFormat:@"%ld", (long)_webSocketVersion]);
    //设置request的原始 Url
    CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Origin"), (__bridge CFStringRef)_url.SR_origin);
    //用户初始化的协议数组，可以约束websocket的一些行为
    if (_requestedProtocols) {
        CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Sec-WebSocket-Protocol"), (__bridge CFStringRef)[_requestedProtocols componentsJoinedByString:@", "]);
    }
    //吧 _urlRequest中原有的head 设置到request中去
    [_urlRequest.allHTTPHeaderFields enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
        CFHTTPMessageSetHeaderFieldValue(request, (__bridge CFStringRef)key, (__bridge CFStringRef)obj);
    }];
    //返回一个序列化 , CFBridgingRelease和 __bridge transfer一个意思， CFHTTPMessageCopySerializedMessage copy一份新的并且序列化，返回CFDataRef
    NSData *message = CFBridgingRelease(CFHTTPMessageCopySerializedMessage(request));
    //释放request
    CFRelease(request);
    //把这个request当成data去写
    [self _writeData:message];
    //读取http的头部
    [self _readHTTPHeader];
}
```
这个方法有点长，大家都知道，WebSocket建立连接前，都会以http请求作为握手的方式，这个方法就是在构造http的请求头。
我们来看看RFC规范的标准客户端请求头：
> 
GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Origin: [http://example.com](http://example.com/)
Sec-WebSocket-Protocol: chat, superchat
Sec-WebSocket-Version: 13
标准的服务端响应头：
> 
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
Sec-WebSocket-Protocol: chat
这里需要讲的是这`Sec-WebSocket-Key`和`Sec-WebSocket-Accept`这一对值，前者是我们客户端自己生成一个16字节的随机data，然后经过base64转码后的一个随机字符串。
而后者则是服务端返回回来的，我们需要用一开始的`Sec-WebSocket-Key`与服务端返回的`Sec-WebSocket-Accept`进行校验：
```
//检查握手信息
- (BOOL)_checkHandshake:(CFHTTPMessageRef)httpMessage;
{
    //是否是允许的header
    NSString *acceptHeader = CFBridgingRelease(CFHTTPMessageCopyHeaderFieldValue(httpMessage, CFSTR("Sec-WebSocket-Accept")));
    //为空则被服务器拒绝
    if (acceptHeader == nil) {
        return NO;
    }
    //得到
    NSString *concattedString = [_secKey stringByAppendingString:SRWebSocketAppendToSecKeyString];
    //期待accept的字符串
    NSString *expectedAccept = [concattedString stringBySHA1ThenBase64Encoding];
    //判断是否相同，相同就握手信息对了
    return [acceptHeader isEqualToString:expectedAccept];
}
```
服务端这个Accept会用这么一个字符串拼接加密：
`static NSString *const SRWebSocketAppendToSecKeyString = @"258EAFA5-E914-47DA-95CA-C5AB0DC85B11";`
这个字符串是RFC规范定死的，至于为什么是这么一串，楼主也不知所以然。
我们发出这个http请求后，得到服务端的响应头，去按照服务端的方式加密`Sec-WebSocket-Key`，判断与`Sec-WebSocket-Accept`是否相同，相同则表明握手成功，否则失败处理。
![](http://upload-images.jianshu.io/upload_images/2702646-50514a2f96f9df0a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
handshake.png
至此都成功的话，一个WebSocket连接建立完毕。
#### 四. 接着来讲讲数据的读和写：
当建立连接成功后，就会循环调用这么一个方法：
```
//读取http头部
- (void)_readHTTPHeader;
{
    if (_receivedHTTPHeaders == NULL) {
        //序列化的http消息
        _receivedHTTPHeaders = CFHTTPMessageCreateEmpty(NULL, NO);
    }
    //不停的add consumer去读数据
    [self _readUntilHeaderCompleteWithCallback:^(SRWebSocket *self,  NSData *data) {
        //拼接数据，拼到头部
        CFHTTPMessageAppendBytes(_receivedHTTPHeaders, (const UInt8 *)data.bytes, data.length);
        //判断是否接受完
        if (CFHTTPMessageIsHeaderComplete(_receivedHTTPHeaders)) {
            SRFastLog(@"Finished reading headers %@", CFBridgingRelease(CFHTTPMessageCopyAllHeaderFields(_receivedHTTPHeaders)));
            [self _HTTPHeadersDidFinish];
        } else {
            //没读完递归调
            [self _readHTTPHeader];
        }
    }];
}
```
记得楼主之前写过一篇[即时通讯下数据粘包、断包处理实例（基于CocoaAsyncSocket）](http://www.jianshu.com/p/2e16572c9ddc)，因此抛出一个问题，WebSocket需要处理数据的断包和粘包么？
答案是基本不需要。引用知乎上的一段回答：
> 
RFC规范指出，WebSocket是一个message-based的协议，它可以自动将数据分片，并且自动将分片的数据组装。
也就是说，WebSocket的RFC标准是不会产生粘包、断包问题的。无需应用层开发人员关心缓存以及手工组装message。
然而理想与现实的不一致：RFC规范与实现的不一致，现实当中有几个问题：
- 每个message可以是一个或多个分片。message不记录长度，分片才记录长度。
- message最大的长度可以达到 9,223,372,036,854,775,807 字节，是由于Payload的数据长度有63bit的限制。
- 很多WebSocket的实现其实并不按照标准的RFC实现完全，很多仅仅实现了50%就拿来用了。这就导致了，在WebSocket实现上的最大长度很难达到这个大小，于是，很多API的实现上是会有限制的，可能会限制你的发送的长度，也可能会把过长的数据直接以流式发送。
而`SRWebSocket`中实现的方式上彻底解决了数据粘包，断包的可能。
数据是通过`CFStream`流的方式回调回来的，每次拿到流数据，都是先放在数据缓冲区中，然后去读当前消息帧的头部，得到当前数据包的大小，然后再去创建消费者对象`consumer`，去读取缓冲区指定数据包大小的内容，读完才会回调给我们上层用户，所以，我们如果用`SRWebSocket`完全不需要考虑数据断包、粘包的问题，每次到达的数据，都是一条完整的数据。
接着我们大概来看看这个流程：
```cpp
```cpp
//读取CRLFCRLFBytes,直到回调回来
- (void)_readUntilHeaderCompleteWithCallback:(data_callback)dataHandler;
{
    [self _readUntilBytes:CRLFCRLFBytes length:sizeof(CRLFCRLFBytes) callback:dataHandler];
}
//读取数据 CRLFCRLFBytes，边界符
- (void)_readUntilBytes:(const void *)bytes length:(size_t)length callback:(data_callback)dataHandler;
{
    // TODO optimize so this can continue from where we last searched
    //消费者需要消费的数据大小
    stream_scanner consumer = ^size_t(NSData *data) {
        __block size_t found_size = 0;
        __block size_t match_count = 0;
        //得到数据长度
        size_t size = data.length;
        //得到数据指针
        const unsigned char *buffer = data.bytes;
        for (size_t i = 0; i < size; i++ ) {
            //匹配字符
            if (((const unsigned char *)buffer)[i] == ((const unsigned char *)bytes)[match_count]) {
                //匹配数+1
                match_count += 1;
                //如果匹配了
                if (match_count == length) {
                    //读取数据长度等于 i+ 1
                    found_size = i + 1;
                    break;
                }
            } else {
                match_count = 0;
            }
        }
        //返回要读取数据的长度，没匹配成功就是0
        return found_size;
    };
    [self _addConsumerWithScanner:consumer callback:dataHandler];
}
```
```
上面这个方法就是一个读取头部的方法，之前我写过断包粘包的文章就是用一个`\r\n`来分割头部和正文，这里是用了`\r\n\r\n`，每次读到这个标识符为止，就是读取了一个完整的WebSocket的消息帧头部。
这里我们先需要说清楚的是，数据一到达，就在`stream`的代理中回调中，写到了我们的`_readBuffer`缓冲区中去了：
```
case NSStreamEventHasBytesAvailable: {
    SRFastLog(@"NSStreamEventHasBytesAvailable %@", aStream);
    const int bufferSize = 2048;
    uint8_t buffer[bufferSize];
    //如果有可读字节
    while (_inputStream.hasBytesAvailable) {
        //读取数据，一次读2048
        NSInteger bytes_read = [_inputStream read:buffer maxLength:bufferSize];
        if (bytes_read > 0) {
            //拼接数据
            [_readBuffer appendBytes:buffer length:bytes_read];
        } else if (bytes_read < 0) {
            //读取错误
            [self _failWithError:_inputStream.streamError];
        }
        //如果读取的不等于最大的，说明读完了，跳出循环
        if (bytes_read != bufferSize) {
            break;
        }
    };
    //开始扫描，看消费者什么时候消费数据
    [self _pumpScanner];
    break;
}
```
接着我们来看添加消费者这个方法：
```
//指定数据读取
- (void)_addConsumerWithScanner:(stream_scanner)consumer callback:(data_callback)callback;
{
    [self assertOnWorkQueue];
    [self _addConsumerWithScanner:consumer callback:callback dataLength:0];
}
//添加消费者，用一个指定的长度，是否读到当前帧
- (void)_addConsumerWithDataLength:(size_t)dataLength callback:(data_callback)callback readToCurrentFrame:(BOOL)readToCurrentFrame unmaskBytes:(BOOL)unmaskBytes;
{   
    [self assertOnWorkQueue];
    assert(dataLength);
    //添加到消费者队列去
    [_consumers addObject:[_consumerPool consumerWithScanner:nil handler:callback bytesNeeded:dataLength readToCurrentFrame:readToCurrentFrame unmaskBytes:unmaskBytes]];
    [self _pumpScanner];
}
- (void)_addConsumerWithScanner:(stream_scanner)consumer callback:(data_callback)callback dataLength:(size_t)dataLength;
{    
    [self assertOnWorkQueue];
    [_consumers addObject:[_consumerPool consumerWithScanner:consumer handler:callback bytesNeeded:dataLength readToCurrentFrame:NO unmaskBytes:NO]];
    [self _pumpScanner];
}
```
其实就是添加了一个`stream_scanner`类型的对象，到我们的`_consumers`数组中去了，以后我们读取数据，都会先取出`_consumers`中的消费者，要读取多少，就给你从`_readBuffer`里去读多少数据。
```
//开始扫描
-(void)_pumpScanner;
{
    [self assertOnWorkQueue];
    //判断是否在扫描
    if (!_isPumping) {
        _isPumping = YES;
    } else {
        return;
    }
    //只有为NO能走到这里，开始循环检测，可读可写数据
    while ([self _innerPumpScanner]) {
    }
    _isPumping = NO;
}
```
这个方法就是做这么一件事，根据`consumer`的要求，循环去`_readBuffer`中读取数据。
至于读的过程，大家可以自己去看下吧，楼主提供的源码注释里已经写的很清楚了，有点略长，这里就不放代码了，方法如下：
```
- (BOOL)_innerPumpScanner 
{
    ...
}
```
至此我们讲了握手的头部信息的读取，与判断是否握手成功，然后数据到达是怎么从stream到_readBuffer中去的，并且简单介绍了`_pumpScanner`会根据消费者对象，去从`_readBuffer`中读取数据，读取完成并且回调`consumer`的`handler`
##### 现在我们来讲讲一个数据从头部开始，到内容的读取过程：
每次我们读取新的一帧数据，都会调用这么个方法：
```
//读取新的消息帧
- (void)_readFrameNew;
{
    dispatch_async(_workQueue, ^{
        //清空上一帧的
        [_currentFrameData setLength:0];
        _currentFrameOpcode = 0;
        _currentFrameCount = 0;
        _readOpCount = 0;
        _currentStringScanPosition = 0;
        //继续读取
        [self _readFrameContinue];
    });
}
```
会清空上一帧的一些信息，然后开始当前帧的读取，我们来简单看看一个`WebSocket`消息帧里包含什么：
```
0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-------+-+-------------+-------------------------------+
 |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
 |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
 |N|V|V|V|       |S|             |   (if payload len==126/127)   |
 | |1|2|3|       |K|             |                               |
 +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
 |     Extended payload length continued, if payload len == 127  |
 + - - - - - - - - - - - - - - - +-------------------------------+
 |                               |Masking-key, if MASK set to 1  |
 +-------------------------------+-------------------------------+
 | Masking-key (continued)       |          Payload Data         |
 +-------------------------------- - - - - - - - - - - - - - - - +
 :                     Payload Data continued ...                :
 + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
 |                     Payload Data continued ...                |
 +---------------------------------------------------------------+
```
就是这么一张图，大家应该经常见，这个图是RFC的标准规范。简单的说明下这些标识着什么：
> 
FIN 1bit 表示信息的最后一帧，flag，也就是标记符
RSV 1-3 1bit each 以后备用的 默认都为 0
Opcode 4bit 帧类型，稍后细说
Mask 1bit 掩码，是否加密数据，默认必须置为1
Payload 7bit 数据的长度 (2^7 -1 最大到127)
Masking-key 1 or 4 bit 掩码 //用来编码数据
Payload data (x + y) bytes 数据 //
Extension data x bytes 扩展数据
Application data y bytes 程序数据
更详细的可以看看：[WebSocket数据帧规范](https://chenjianlong.gitbooks.io/rfc-6455-websocket-protocol-in-chinese/content/section5/section5.html)
接着我们读取消息，会用到其中的一些字段，包括`FIN`、 `MASK`、
```
Payload
 len
```
等等。
然后来看看这个读取当前消息帧的方法：
```
//开始读取当前消息帧
- (void)_readFrameContinue;
{
    //断言要么都为空，要么都有值
    assert((_currentFrameCount == 0 && _currentFrameOpcode == 0) || (_currentFrameCount > 0 && _currentFrameOpcode > 0));
    //添加一个consumer，数据长度为2字节 frame_header 2个字节
    [self _addConsumerWithDataLength:2 callback:^(SRWebSocket *self, NSData *data) {
        //
        __block frame_header header = {0};
        const uint8_t *headerBuffer = data.bytes;
        assert(data.length >= 2);
        //判断第一帧 FIN
        if (headerBuffer[0] & SRRsvMask) {
            [self _closeWithProtocolError:@"Server used RSV bits"];
            return;
        }
        //得到Qpcode
        uint8_t receivedOpcode = (SROpCodeMask & headerBuffer[0]);
        //判断帧类型，是否是指定的控制帧
        BOOL isControlFrame = (receivedOpcode == SROpCodePing || receivedOpcode == SROpCodePong || receivedOpcode == SROpCodeConnectionClose);
        //如果不是指定帧，而且receivedOpcode不等于0，而且_currentFrameCount消息帧大于0，错误关闭
        if (!isControlFrame && receivedOpcode != 0 && self->_currentFrameCount > 0) {
            [self _closeWithProtocolError:@"all data frames after the initial data frame must have opcode 0"];
            return;
        }
        // 没消息
        if (receivedOpcode == 0 && self->_currentFrameCount == 0) {
            [self _closeWithProtocolError:@"cannot continue a message"];
            return;
        }
        //正常读取
        //得到opcode
        header.opcode = receivedOpcode == 0 ? self->_currentFrameOpcode : receivedOpcode;
        //得到fin
        header.fin = !!(SRFinMask & headerBuffer[0]);
        //得到Mask
        header.masked = !!(SRMaskMask & headerBuffer[1]);
        //得到数据长度
        header.payload_length = SRPayloadLenMask & headerBuffer[1];
        headerBuffer = NULL;
        //如果是带掩码的，则报错，因为客户端是无法得知掩码的值得。
        if (header.masked) {
            [self _closeWithProtocolError:@"Client must receive unmasked data"];
        }
        size_t extra_bytes_needed = header.masked ? sizeof(_currentReadMaskKey) : 0;
        //得到长度
        if (header.payload_length == 126) {
            extra_bytes_needed += sizeof(uint16_t);
        } else if (header.payload_length == 127) {
            extra_bytes_needed += sizeof(uint64_t);
        }
        //如果多余的需要的bytes为0
        if (extra_bytes_needed == 0) {
            //
            [self _handleFrameHeader:header curData:self->_currentFrameData];
        } else {
            //读取payload
            [self _addConsumerWithDataLength:extra_bytes_needed callback:^(SRWebSocket *self, NSData *data) {
                size_t mapped_size = data.length;
                #pragma unused (mapped_size)
                const void *mapped_buffer = data.bytes;
                size_t offset = 0;
                if (header.payload_length == 126) {
                    assert(mapped_size >= sizeof(uint16_t));
                    uint16_t newLen = EndianU16_BtoN(*(uint16_t *)(mapped_buffer));
                    header.payload_length = newLen;
                    offset += sizeof(uint16_t);
                } else if (header.payload_length == 127) {
                    assert(mapped_size >= sizeof(uint64_t));
                    header.payload_length = EndianU64_BtoN(*(uint64_t *)(mapped_buffer));
                    offset += sizeof(uint64_t);
                } else {
                    assert(header.payload_length < 126 && header.payload_length >= 0);
                }
                if (header.masked) {
                    assert(mapped_size >= sizeof(_currentReadMaskOffset) + offset);
                    memcpy(self->_currentReadMaskKey, ((uint8_t *)mapped_buffer) + offset, sizeof(self->_currentReadMaskKey));
                }
                //把已读到的数据，和header传出去
                [self _handleFrameHeader:header curData:self->_currentFrameData];
            } readToCurrentFrame:NO unmaskBytes:NO];
        }
    } readToCurrentFrame:NO unmaskBytes:NO];
}
```
这个方法是先去读取了当前消息帧的前2个字节，大概就是这么一部分：
```
0                   1                   
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 
 +-+-+-+-+-------+-+-------------+
 |F|R|R|R| opcode|M| Payload len |    
 |I|S|S|S|  (4)  |A|     (7)     |            
 |N|V|V|V|       |S|             |   
 | |1|2|3|       |K|             |                    
 +-+-+-+-+-------+-+-------------+
```
然后会去对头部信息进行一些判断，但是最主要的还是去获取payload，也就是真实数据的长度，然后还是调用：
```
[self _addConsumerWithDataLength:extra_bytes_needed callback:^(SRWebSocket *self, NSData *data) {
    ...
}];
```
去读取真实数据的长度，然后会在下面这个方法中判断当前帧的数据是否读取完成：
```
- (void)_handleFrameHeader:(frame_header)frame_header curData:(NSData *)curData;
{
    ...
    if(complete)
    {
       [self _handleFrameWithData:_currentFrameData opCode:frame_header.opcode];
    }else{
       [self _readFrameContinue];
    }
    ...
}
```
如果没读取完成，会继续去读取，否则就调用完成的方法，在完成的方法中会回调暴露给我们的代理：
```
[self _performDelegateBlock:^{
    [self.delegate webSocket:self didReceiveMessage:message];
}];
```
并且继续去读下一帧的数据
`[self _readFrameNew];`
整个数据读取过程就完成了。
###### 接着我们来看看数据的写:
```
//写数据
- (void)_writeData:(NSData *)data;
{
    //断言当前queue
    [self assertOnWorkQueue];
    //如果标记为写完成关闭，则直接返回
    if (_closeWhenFinishedWriting) {
            return;
    }
    //输出buffer拼接数据
    [_outputBuffer appendData:data];
    //开始写
    [self _pumpWriting];
}
```
```
- (void)_pumpWriting
{
    ...
    //写入进去，就会直接发送给对方了！这一步send
    NSInteger bytesWritten = [_outputStream write:_outputBuffer.bytes + _outputBufferOffset maxLength:dataLength - _outputBufferOffset];
    ...
}
```
基本上非常简单，区别于之前`CocoaAsyncSocket`，读和写都没多少代码，原因是因为`CocoaAsyncSocket`整篇都用的是`CFStream`等相对上层的API。
SRWebSocket全篇代码注释地址：[SRWebSocket注释](https://github.com/tuyaohui/SRWebSocketTip/tree/master)。
