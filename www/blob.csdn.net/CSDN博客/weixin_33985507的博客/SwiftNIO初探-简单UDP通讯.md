# SwiftNIO初探-简单UDP通讯 - weixin_33985507的博客 - CSDN博客
2018年06月14日 10:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
最近苹果开源了[Swift版的Netty](https://github.com/apple/swift-nio)
SwiftNIO是一个跨平台异步事件驱动的网络应用程序框架，用于快速开发可维护的高性能协议服务器和客户端。简单来说就是可以用来实现各种高性能服务端和客户端，如http、tcp。
因为最近项目刚好要用到udp，所以趁机把平常用Netty实现的该用Swift。
首先简单看一下几个用到的类
#### MultiThreadedEventLoopGroup
顾名思义，这东西是一个线程池。每个`EventLoopGroup`里面有多个`EventLoop`，而每个`EventLoop`都与一个线程绑定。
#### DatagramBootstrap
`Bootstrap`是开发Netty程序的基础，SwiftNIO也是一样。通过`Bootstrap`的bind方法来创建连接，我们也可以通过该方法返回的`Channel`来判断是否创建成功。
#### ChannelHandler
`ChannelHandler` 是用来处理数据，如客户端向服务端发送数据，服务端的数据处理就是在`ChannelHandler`中完成。`ChannelHandler` 本身是一个protocol，我们用到的有`ChannelInboundHandler`和`ChannelOutboundHandler`这两个，`ChannlPipeline`会从头到尾顺序调用`ChannelInboundHandler`处理数据，从尾到头调用`ChannelOutboundHandler`数据。
接下来看服务端代码：
```
class UDPServer {
    private let loopGroup = MultiThreadedEventLoopGroup(numberOfThreads: System.coreCount)
    
    func listen(on port: Int) {
        let bootstrap = DatagramBootstrap(group: loopGroup)
            .channelOption(ChannelOptions.socket(SocketOptionLevel(SOL_SOCKET), SO_REUSEADDR), value: 1)
            .channelInitializer { (channel) -> EventLoopFuture<Void> in
                channel.pipeline.add(handler: UDPServerHandler())
        }
        
        do {
            let channel = try bootstrap.bind(host: "127.0.0.1", port: port).wait()
            print("listen on \(channel.localAddress!)")
            try channel.closeFuture.wait()
        } catch {
            print(error)
        }
    }
    
    final class UDPServerHandler: ChannelInboundHandler {
        typealias InboundIn = AddressedEnvelope<ByteBuffer>
        typealias OutboundOut = AddressedEnvelope<ByteBuffer>
        
        func channelRead(ctx: ChannelHandlerContext, data: NIOAny) {
            let getData = unwrapInboundIn(data)
            print(getData.data)
        }
    }
}
let server = UDPServer()
server.listen(on: 5656)
```
运行代码就会可以看到控制台输出:
`listen on [IPv4]127.0.0.1:5656`
我们通过内部类的形式实现了一个 `ChannelInboundHandler` ，并把它添加到
`ChannlPipeline`。`Handler` 需要设置两个东西 `InboundIn` 和 `OutboundOut`。
`InboundIn`： 是入站数据的类型，就是接收客户端发过来的数据类型。
`OutboundOut`： 是出站数据的类型，就是返回给客户端的数据类型，同时也是传递给下一个`ChannelOutboundHandler`的类型。
我们这里用的是 `AddressedEnvelope<ByteBuffer>` 它里面是一个 `SocketAddress` 加上 `ByteBuffer` 。
当客户端发来数据的时候会调用`Handler`的`channelRead(ctx: , data: )`
这里进来的是`NIOAny`类型，需要调用 `Handler` 的 `unwrapInboundIn()` 方法把 `NIOAny` 转成 `InboundIn` 类型。
就这样一个简单的UDP服务端就完成了，可以通过各种UDP工具进行测试，或者用SwiftNIO再写一个客户端：
```
class UDPClient {
    private let loopGroup = MultiThreadedEventLoopGroup(numberOfThreads: System.coreCount)
    private var channel: Channel!
    
    init(port: Int) {
        let bootstrap = DatagramBootstrap(group: loopGroup)
            .channelOption(ChannelOptions.socket(SocketOptionLevel(SOL_SOCKET), SO_REUSEADDR), value: 1)
            .channelInitializer { (channel) -> EventLoopFuture<Void> in
                channel.pipeline.add(handler: UDPClientHandler())
        }
        
        do {
            channel = try bootstrap.bind(host: "0.0.0.0", port: port).wait()
            sent(with: "test".data(using: .utf8)!)
        } catch {
            print(error)
        }
    }
    
    func sent(with data: Data) {
        var byteBuffer = ByteBufferAllocator().buffer(capacity: data.count)
        byteBuffer.write(bytes: data)
        let address = try! SocketAddress(ipAddress: "127.0.0.1", port: 5656)
        channel.writeAndFlush(NIOAny(AddressedEnvelope<ByteBuffer>(remoteAddress: address, data: byteBuffer)), promise: nil)
    }
    
    final class UDPClientHandler: ChannelInboundHandler {
        typealias InboundIn = AddressedEnvelope<ByteBuffer>
        
        func channelRead(ctx: ChannelHandlerContext, data: NIOAny) {
            print(data)
        }
    }
}
let client = UDPClient(port: 22222)
```
到这里，用SwiftNIO构建简单的UDP通讯已经OK了。
