# Nebula3学习笔记(5): IO系统 - 逍遥剑客 - CSDN博客
2008年07月07日 14:54:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2421
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
**IO子系统**
Nebula3的IO系统相对于Nebula1和2是一个巨大的进步, 新系统的主要设计目标有:
- 使用更标准的机制, 如用URI来定位资源, 用MIME类型来区分数据格式 
- 一个灵活的流模型, 它不关心数据是来自文件, 内存, HTTP连接还是其它地方 
- 从流读写不数据的数据类型也更方便, 例如要读取的XML格式数据来自文件/内存/网络都没问题 
- 另外, 新的流和读写类可以在运行时注册到IO系统中 
- 相对于系统平台的特定IO函数, 像fopen()这样的C Lib函数会有额外的性能或内存损失. 所以在保证可移植性的前提下不损失性能, 必须使用特定平台的IO函数
IO子系统的一些主要概念:
- 一个中枢的**IO::Console** 对象连接控制台处理器(console handler)来进行文本的输入和输出. 这保证了所有的Nebula3的文本输出都通过一个集中的进出通道. 特定的控制台处理器可以用特定的方式处理文本输出(例如输出到stdout, 游戏控制台, 日志文件或网络连接). 
- **重定向符**做为路径别名. 大体的功能跟Nebula1和2差不多, 除了从AmigaOS 的重定向符得到的灵感. Nebula3重定向符的一个新特性就是它们可以做为URI的别名. 例如, 重定向符”textures:”可以定义为 "http://www.radonlabs.de/textures", 这样简化的资源路径"textures:mytexture.dds"就会解释成这个绝对路径: "http://www.radonlabs.de/textures/mytexture.dds" (太NB了, 把纹理放到网站上加载? 哈哈, 拿来做内置广告肯定很爽)
- **流(Stream)**做为基本的数据进出通道. 它提供了基本的API函数 Open()/Close()/Read()/Write(), 但是可能完全隐藏了传输和存储通道. 典型的例子有IO::FileStream, IO::MemoryStream, 或 Net::HttpStream
- **Stream reader ****和 writer** 是连接到流上并且实现了简单易用的接口来读写数据格式. 例如你可以把IO::XmlReader连接到IO::FileStream来从文件系统读取XML格式的数据, 或者连接到IO::HttpStream来从HTTP连接读取XML格式的数据.
这里有个很好的代码例子可以反映出Nebula3输入输出系统的强大:
1: IO::FileServer::Instance**()**->CopyFile**(**"http://www.radonlabs.de/index.html"**,**"temp:index.html"**);**
这一行代码从HTTP服务器拷贝了一个文件到当用户的临时目录里去. 再多加几行代码, 你可以创建一个流对象指向HTTP服务器上的HTML文件, 连接一个XML reader到这个流上, 然后就可以在不存储中间文件的基础上进行解析HTML了. 
**标准重定向符**
Nebula3初始化了以下几个重定向符:
- **home:** 指向应用程序目录, 一般在” C:/Program Files “下. Nebula3把这个目录当成只读的, 为的是不需要管理员权限就能运行. 
- **user:** 这个指向当前登录的用户目录, 一般是指” C:/Documents and Settings/[username] “. Nebula3会自动创建一个本地目录来避免不同程序覆写掉它们的数据. 所以说一般情况下把数据写入用户目录是安全的. 这个地方可以用于保存游戏数据和配置, 或者程序需要调用的持久性数据. 
- **temp:** 这个指向当前用户的临时目录, 一般是可写的, 但是不要假设下一次启动程序时数据还存在. 
- **bin:** 这个指向应用程序可执行文件的目录. 它可以跟home相同, 也可能不同. 这个目录应该也当成是只读的来对待.
其它重定向符可以在程序运行时进行定义. 通常情况下会定义一些抽象资源路径, 如textuers, sound, data等等. 这样的话资源的路径就可以只更改重定向符的定义而是不是去替换所有的路径. 重定向符的另一个好处就是减少了路径字符串的长度, 在一定程序上节省了内存占用.
**URI(统一资源定位符)**
在Nebula3中的资源位置通常都是用URI定义的. URI一般包括下面这几部, 有一些是可选的:
- 模式(协议?), 如"http:", "file:", 等... Nebula3 没有硬编码任何模式, 而跟流类绑定在一起注册到IO::StreamServer 单件 
- 一个可选的用户信息字段, 这是一个用户名和密码用于HTTP或FTP主机的身份验证 
- 一个主机名, 如"www.radonlabs.de" 
- 一个在主机名后可选的端口号
- 一个本地路径, 指向主机上的一个资源 
- 一个可选的片段, 通常指向资源内部的一个位置 
- 一个可选的查询部分, 一般包含一个PHP脚本或其它相似的动态响应机制的参数
IO::URI类用来传递URI并且解析URI字符串到它的各个部分中. 值得注意的是URI对象比字符串占用更多的内存, 所以有时把URI保存在字符串中, 并在需要分割的时候才使用IO::URI类会更好一些.
这里有一些URI的例子:
1: file:///c:/temp/bla.txt
2: file://samba/temp/bla.txt
3: http://www.radonlabs.de/index.html
4: http://user:password@www.myserver.com:8080/index.html#main
通过使用重定位符会大大简化路径名称. 要引用一个程序目录的文件你可以使用”home:bla.txt”, 等价于file:///c:/Program Files/[myapp]/bla.txt.
**Stream, Reader 和 Writer**
流(Stream)提供了用于储存和传输原始数据的接口. 一个流对象提供了传统的Open()/Close()/Read()/Write()/Seek()接口, 其中有些还提供内存映射, 这样数据的读写可以直接通过内存访问来实现. Stream对象用一个IO::URI对象来定义它们的资源位置. 通常情况下, 一个URI格式映射到一个特定的流对象. 例如”http:”URI格式一般映射到Net::HttpStream类, 而”file:”格式则映射到IO:FileStream类. 这个映射由StreamServer构造一个流对象并匹配一个URI. 一个Nebula3应用程序通过StreamServer::Register()方法来注册这个映射关系, 这也是新的流对象和URI格式的注册方法. 
让我们来看看有哪些重要的类:
- IO::FileStream: 提供了访问主机文件系统的功能 
- IO::MemoryStream: 一个具有流接口的动态内存缓冲 
- IO::HttpStream: 提供了一个流接口来访问HTTP服务器文件
Stream reader和writer类提供了一些舒适的接口专门处理特定的数据格式. 这里有一些stream reader和writer:
- IO::BinaryReader/IOBinaryWriter: 读写二进制数据 
- IO::TextReader/IOTextWriter: 读写文本数据 
- IO::XmlReader/IOXmlWriter: 读写XML格式的数据 
- Messaging::MessageReader/MessagingMessageWriter: 消息序列化 
这里有一个用XmlReader从HTTP服务器访问文件的简单例子
 1:     using namespace IO**;**
 2: 
 3:     Ptr<Stream> stream = StreamServer::Instance**()**->CreateStream**(**"http://www.radonlabs.de/index.html"**);**
 4:     Ptr<XmlReader> xmlReader = XmlReader::Create**();**
 5:     xmlReader->SetStream**(**stream**);**
 6: **if****(**xmlReader->Open**())**
 7: **{**
 8: *// parse content here using the XmlReader interface*
 9: **}**
**File Server(文件服务器)**
Nebula3 IO::FileServer类提供了一个单件用于访问主机的文件系统进行一些全局操作, 像定义重定向符, 复制, 删除和检查文件是否存在, 列出目录内容, 等等. 
这个代码片断介绍FileServer的一些有用的方法:
usingnamespaceIO;
usingnamespaceUtil;
FileServer* fs = FileServer::Instance();
// check if a file or directory exists
boolfileExists = fs->FileExists("home:bla.txt");
booldirExists = fs->DirectoryExists("temp:bla/blub");
// resolve a path with assigns into an absolute filesystem
// path, this is sometimes necessary to interface with 
// 3rd party libraries which don't understand Nebula3 paths directly
StringabsPath = fs->ResolveAssings("user:myapp/savegames");
// create a directory, note that all missing subdirectories will
// be created as well
fs->CreateDirectory("user:myapp/savegames");
// copy and delete files
fs->CopyFile("home:movie.mpg", "temp:movie.mpg");
fs->DeleteFile("temp:movie.mpg");
// list files in a directory matching a pattern
Array<String> files = fs->ListFiles("temp:", "*.txt");
// list all subdirectories in temp:
Array<String> dirs = fs->ListDirectories("temp:", "*");
**控制台**
一般不直接调用IO::Console, 直接n_printf(), n_error(), n_dbgout(), n_warning()@_@
