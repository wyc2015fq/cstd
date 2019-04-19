# WebSocket 和 Socket 的区别 - 文章 - 伯乐在线
原文出处： [TheAlchemist](http://www.jianshu.com/p/59b5594ffbb0)
- 《[刨根问底 HTTP 和 WebSocket 协议（上）](http://blog.jobbole.com/105953/)》
- 《[刨根问底 HTTP和 WebSocket 协议（下）](http://blog.jobbole.com/105958/)》
> 
当我们探讨两件事物的区别和联系时，我们想探讨些什么？
前段时间写了两篇介绍 HTTP 和 WebSocket 的文章，回复中有人说希望了解下WebSocket和Socket的区别。这个问题之前也有想过，自己对此是有大概的答案，可是并不太确定，于是去搜集了些资料（其实就是各种Google），看了很多以前的文档，觉得有些故事十分有趣，整理如下，算是一个外传。
文中图片全来自Google图片搜索，如侵删。
## 短答案
就像Java和JavaScript，并没有什么太大的关系，但又不能说完全没关系。可以这么说：
- 命名方面，Socket是一个深入人心的概念，WebSocket借用了这一概念；
- 使用方面，完全两个东西。
![1966024-28aa9e546a7c7528](http://jbcdn2.b0.upaiyun.com/2016/09/cc45811f75d76e8f8f14a7fc05403319.jpg)
*Java和JavaScript的关系*
## 长答案
> 
当我们探讨两件事物的区别和联系时，我们想探讨些什么？
对于我来说，大多数情况是想知道两件事物本身，而并不是想只想了解「区别」本身。那么对这个问题最直接的解决方法应该是去了解Socket和WebSocket的来源和用法，那么它们的区别和联系就不言自明了。
### Socket
Socket可以有很多意思，和IT较相关的本意大致是指**在端到端的一个连接中，这两个端叫做Socket**。对于IT从业者来说，它往往指的是TCP/IP网络环境中的两个连接端，大多数的API提供者（如操作系统，JDK）往往会提供基于这种概念的接口，所以对于开发者来说也往往是在说一种编程概念。同时，操作系统中进程间通信也有Socket的概念，但这个Socket就不是基于网络传输层的协议了。
#### Unix 中的 Socket
操作系统中也有使用到Socket这个概念用来进行进程间通信，它和通常说的基于TCP/IP的Socket概念十分相似，代表了在操作系统中传输数据的两方，只是它不再基于网络协议，而是操作系统本身的文件系统。
#### 网络中的 Socket
通常所说的Socket API，是指操作系统中（也可能不是操作系统）提供的对于传输层（TCP/UDP）抽象的接口。现行的Socket API大致都是遵循了BSD Socket规范（包括Windows）。这里称规范其实不太准确，规范其实是POSIX，但BSD Unix中对于Socket的实现被广为使用，所以成为了实际的规范。如果你要使用HTTP来构建服务，那么就不需要关心Socket，如果你想基于TCP/IP来构建服务，那么Socket可能就是你会接触到的API。
![1966024-5c1411059ca316a8](http://jbcdn2.b0.upaiyun.com/2016/09/59e11b41252fd2182997e8f2ff77fc7f.png)
*在TCP/IP网络中HTTP的位置*
从上图中可以看到，HTTP是基于传输层的TCP协议的，而Socket API也是，所以只是从使用上说，可以认为Socket和HTTP类似（但一个是成文的互联网协议，一个是一直沿用的一种编程概念），是对于传输层协议的另一种*直接*使用，因为按照设计，网络对用户的接口都应该在应用层。
##### Socket 名称的由来
和很多其他Internet上的事物一样，Socket这个名称来自于大名鼎鼎的ARPANET（Advanced Research Projects Agency），早期ARPANET中的Socket指的是一个源或者目的地址——大致就是今天我们所说的IP地址和端口号。最早的时候一个Socket指的是一个40位的数字（RFC33中说明了此用法，但在RFC36中并没有*明确*地说使用40位数字来标识一个地址），其中前32为指向的地址（socket number，大致相当于IP），后8位为发送数据的源（link，大致相当于端口号）。对他们的叫法有很多的版本，这里列举的并不严谨。
##### 端口号的野史
随着ARPANET的发展，后来（RFC433，Socket Number List）socket number被明确地定义为一个40位的数字，其中后8位被用来制定某个特定的应用使用（比如1是Telnet）。这8位数有很多名字：link、socket name、AEN（another eight number，看到这个名字我也是醉了），工程师逗逼起来也是挺拼的。
后来在Internet的规范制定中，才真正的用起了port number这个词。至于为什么端口号是16位的，我想可能有两个原因，一是对于当时的工程师来说，如果每个端口号来标识一个程序，65535个端口号也差不多够用了。二可能是为了对齐吧，^_^!!。
#### Socket 原本的意思
在上边提到的历史中使用到的Socket，包括TCP文档中使用到的Socket，其实指的是网络传输中的一端，是一个虚拟化的概念。
### WebSocket
上边简单叙述了Socket的意义，由于年代久远，很多事情也搞不了那么清楚。但WebSocket是一个很晚近的东西，可以让我们看到它是如何成为现在我们看到的这个样子的。
#### WHATWG(Web Hypertext Application Technology Working Group)
关于HTML5的故事很多人都是知道的，w3c放弃了HTML，然后有一群人（也有说是这些人供职的公司，不过官方的文档上是说的个人）创立了WHATWG组织来推动HTML语言的继续发展，同时，他们还发展了很多关于Web的技术标准，这些标准不断地被官方所接受。WebSocket就属于WHATWG发布的Web Application的一部分（即HTML5）的产物。
#### 为什么会有 WebSocket
大约在08年的时候，WG的工程师在讨论网络环境中需要一种全双工的连接形式，刚开始一直叫做「TCPConnection」，并讨论了这种协议需要支持的功能，大致已经和我们今天看到的WebSocket差不多了。他们认为基于现有的HTTP之上的一些技术（如长轮询、Comet）并满足不了这种需求，有必要定义一个全新的协议。
#### 名称的由来
在很多的关于HTML5或者WebSocket的文档中，都能看到一个名字，Hixie（Ian Hickson），他是WHATWG组织的发言人，曾供职于Netscape、Opera、Google，看工作的公司就知道这个人的背景了。
![1966024-8bbdd8180ff9d069](http://jbcdn2.b0.upaiyun.com/2016/09/e6cd25c18dd3a82901a6cd93523cf93e.jpg)
*hixie*
08年6月18日，一群WHATWG的工程师在讨论一些技术问题，一个工程师提到说「我们之前讨论的那个东西，不要叫TCPConnection 了，还是起个别的名字吧 」，接着几个名字被提及，DuplexConnection，TCPSocket，SocketConnection ，一个叫mcarter（Michael Carter ）的工程师说他马上要写一篇关于Comet的文章，如果可以确定这个名称，想在文章中引用这个名字。
Socket一直以来都被人用来表示网络中一个连接的两端，考虑到怎么让工程师更容易接受，后来Hixie说了一句「我看WebSocket这个名字就很适合嘛（Hixie briefly pops back online to record that “WebSocket” would probably be a good new name for the TCPConnection object）」，大家都没有异议，紧接着mcarter在Comet Daily中发表了文章[Independence Day: HTML5 WebSocket Liberates Comet From Hacks](http://cometdaily.com/2008/07/04/html5-websocket/index.html)，后来随着各大浏览器对WebSocket的支持，它变成了实际的标准，IETF也沿用了这个名字。
下边是在WHATWG文档中对WebSocket接口的定义


```
enum BinaryType { "blob", "arraybuffer" };
[Constructor(USVString url, optional (DOMString or sequence<DOMString>) protocols = []), Exposed=(Window,Worker)]
interface WebSocket : EventTarget {
  readonly attribute USVString url;
  // ready state
  const unsigned short CONNECTING = 0;
  const unsigned short OPEN = 1;
  const unsigned short CLOSING = 2;
  const unsigned short CLOSED = 3;
  readonly attribute unsigned short readyState;
  readonly attribute unsigned long long bufferedAmount;
  // networking
  attribute EventHandler onopen;
  attribute EventHandler onerror;
  attribute EventHandler onclose;
  readonly attribute DOMString extensions;
  readonly attribute DOMString protocol;
  void close([Clamp] optional unsigned short code, optional USVString reason);
  // messaging
  attribute EventHandler onmessage;
  attribute BinaryType binaryType;
  void send(USVString data);
  void send(Blob data);
  void send(ArrayBuffer data);
  void send(ArrayBufferView data);
};
```
#### 内容的确定
大多数新技术的出现都是建立在已有技术的铺垫之上的，WebSocket内容的确定也是如此，其中就有Comet看不到的贡献，Comet是一个很有趣的技术，有兴趣可以[看看这里](http://www.ibm.com/developerworks/cn/web/wa-lo-comet/)
### 结论
可以把WebSocket想象成HTTP，HTTP和Socket什么关系，WebSocket和Socket就是什么关系。
