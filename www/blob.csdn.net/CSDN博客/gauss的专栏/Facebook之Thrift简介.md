# Facebook之Thrift简介 - gauss的专栏 - CSDN博客
2015年11月03日 23:22:26[gauss](https://me.csdn.net/mathlmx)阅读数：271
个人分类：[开源库](https://blog.csdn.net/mathlmx/article/category/944352)
                
以下内容是从网上各处简单整理而来，因为前段时间自己一直在研究Thrift，把研究的一点资料分享给需要的同行中人！
#### 第一节 RPC技术及实现简介
首先思考一下分布式系统中的 RPC (Remote Procedure Call) 问题，一个完整的 RPC 模块需要可以分为三个层次
· 服务层（service）：RPC 接口定义与实现
· 协议层（protocol）：RPC 报文格式和数据编码格式
· 传输层（transport）：实现底层的通信（如 socket）以及系统相关的功能（如事件循环、多线程）
在实际的大型分布式系统中，不同的服务往往会使用不同的语言来实现，所以一般的 RPC 系统会提供一种跨语言的过程调用功能，比如一段用C++实现的客户端代码可以远程调用一个用 Java 实现的服务。实现跨语言 RPC 有两种方法：
· 静态代码生成：开发者用一种中间语言（IDL，接口定义语言）来定义 RPC 的接口和数据类型，然后通过一个编译器来生成不同语言的代码（如C++, Java, Python），并由生成的代码来负责 RPC 协议层和传输层的实现。例如，服务的实现用C++，则服务端需要生成实现RPC协议和传输层的C++代码，服务层使用生成的代码来实现与客户端的通信；而如果客户端用 Python，则客户端需要生成Python代码。
· 基于“自省”的动态类型系统来实现：协议和传输层可以只用一种语言实现成一个库，但是这种语言需要关联一个具备“自省”或者反射机制的动态类型系统，对外提供其他语言的绑定，客户端和服务端通过语言绑定来使用 RPC。比如，可以考虑用 C 和 GObject 实现一个 RPC 库，然后通过 GObject 实现其他语言的绑定。
第一种方法的优点是RPC的协议层和传输层的实现不需要和某种动态类型系统（如GObject）绑定在一起，同时避免了动态类型检查和转换，程序效率比较高，但是它的缺点是要为不同语言提供不同的 RPC 协议层和传输层实现。第二种方法的主要难度在于语言绑定和通用的对象串行化机制的实现，同时也需要考虑效率的问题。
Thrift 是一个基于静态代码生成的跨语言的RPC协议栈实现，它可以生成包括C++, Java, Python, Ruby, PHP 等主流语言的代码，这些代码实现了 RPC 的协议层和传输层功能，从而让用户可以集中精力于服务的调用和实现。Cassandra 的服务访问协议是基于 Thrift 来实现的。
#### 第二节 thrift介绍
Thrift源于大名鼎鼎的facebook之手，在2007年facebook提交Apache基金会将Thrift作为一个开源项目，对于当时的facebook来说创造thrift是为了解决facebook系统中各系统间大数据量的传输通信以及系统之间语言环境不同需要跨平台的特性。所以thrift可以支持多种程序语言，例如:  C++, C#, Cocoa, Erlang, Haskell, Java, Ocami, Perl, PHP, Python, Ruby, Smalltalk. 在多种不同的语言之间通信thrift可以作为二进制的高性能的通讯中间件，支持数据(对象)序列化和多种类型的RPC服务。Thrift适用于程序对程 序静态的数据交换，需要先确定好他的数据结构，他是完全静态化的，当数据结构发生变化时，必须重新编辑IDL文件，代码生成，再编译载入的流程，跟其他IDL工具相比较可以视为是Thrift的弱项，Thrift适用于搭建大型数据交换及存储的通用工具，对于大型系统中的内部数据传输相对于JSON和xml无论在性能、传输大小上有明显的优势。
Thrift 主要由5个部分组成：
· 类型系统以及 IDL 编译器：负责由用户给定的 IDL 文件生成相应语言的接口代码
· TProtocol：实现 RPC 的协议层，可以选择多种不同的对象串行化方式，如 JSON, Binary。
· TTransport：实现 RPC 的传输层，同样可以选择不同的传输层实现，如socket, 非阻塞的 socket, MemoryBuffer 等。
· TProcessor：作为协议层和用户提供的服务实现之间的纽带，负责调用服务实现的接口。
· TServer：聚合 TProtocol, TTransport 和 TProcessor 几个对象。
上述的这5个部件都是在 Thrift 的源代码中通过为不同语言提供库来实现的，这些库的代码在 Thrift 源码目录的 lib 目录下面，在使用 Thrift 之前需要先熟悉与自己的语言对应的库提供的接口。
#### 第三节 使用thrift的项目
（1） Thrift用于Quara系统后端数据的通信，服务端是用C++来实现的，客户端则是python。
Quara背景:Quara是在线问答服务公司，类似新浪微博和百度知道的合体，消息灵通人士透露，去年Quara获得了1400万美元投资，目前他们只有9名员工。
（2）Thrift用于在多种Evernote API平台开发的客户端与Evernote服务器之间的通信与数据传输，Evernote API定义了自己的Evernote Data Access and Management (EDAM) 协议规范，让客户端使用更小的网络带宽上传、下载文件和在线即时搜索服务。
Evernote 背景:EverNote是一款非常著名的免费软件，它最大的特点就是支持多平台，而且数据能通过网络互相同步。譬如说，你可以随时在手机上的Evernote新增笔记，回家后在电脑上也能看到它了！
（3）HBase 中的Thrift：Thrift用于HBase中是为了提供跨平台的服务接口，在HBase 中可以使用[hbase-root]/bin/hbase thrift start 命令启动涵盖Thrift的HBase服务端，客户端通过thrift的命令生成不同版本的客户端代码，根据定义的数据格式，对远程HBase服务端进行操作，是除了REST远程方法调用的另一种途径。
（4）其他系统：如facebook的scribe系统、淘宝的timetunnel系统和Hive等等。
