# Thrift 原理与使用实例 - z69183787的专栏 - CSDN博客
2016年05月06日 16:54:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2513
一、Thrift 框架介绍
1、前言
Thrift是一个跨语言的服务部署框架，最初由Facebook于2007年开发，2008年进入Apache开源项目。Thrift通过一个中间语言(IDL, 接口定义语言)来定义RPC的接口和数据类型，然后通过一个编译器生成不同语言的代码（目前支持C++,Java, Python, PHP, Ruby, Erlang, Perl, Haskell, C#, Cocoa, Smalltalk和OCaml）,并由生成的代码负责RPC协议层和传输层的实现。
本文组织结构如下：1）引言 2）架构3）支持的数据传输格式、数据传输方式和服务模型 4）Thrift安装 5）利用Thift部署服务
2、架构
![image](http://img0.tuicool.com/NR36zq.gif)
Thrift实际上是实现了C/S模式，通过代码生成工具将接口定义文件生成服务器端和客户端代码（可以为不同语言），从而实现服务端和客户端跨语言的支持。用户在Thirft描述文件中声明自己的服务，这些服务经过编译后会生成相应语言的代码文件，然后用户实现服务（客户端调用服务，服务器端提服务）便可以了。其中protocol（协议层, 定义数据传输格式，可以为二进制或者XML等）和transport（传输层，定义数据传输方式，可以为TCP/IP传输，内存共享或者文件共享等）被用作运行时库。上图的详细解释参考引用【1】。
3、 支持的数据传输格式、数据传输方式和服务模型
TBinaryProtocol – 二进制格式.
TCompactProtocol – 压缩格式
TJSONProtocol – JSON格式
TSimpleJSONProtocol –提供JSON只写协议, 生成的文件很容易通过脚本语言解析。
TDebugProtocol – 使用易懂的可读的文本格式，以便于debug
（2） 支持的数据传输方式
TFramedTransport – 以frame为单位进行传输，非阻塞式服务中使用。
TFileTransport – 以文件形式进行传输。
TMemoryTransport – 将内存用于I/O. java实现时内部实际使用了简单的ByteArrayOutputStream。
TZlibTransport – 使用zlib进行压缩， 与其他传输方式联合使用。当前无java实现。
TSimpleServer – 简单的单线程服务模型，常用于测试
TThreadPoolServer – 多线程服务模型，使用标准的阻塞式IO。
TNonblockingServer – 多线程服务模型，使用非阻塞式IO（需使用TFramedTransport数据传输方式）
4、 Thrift安装
下载： [http://incubator.apache.org/thrift/download/](http://incubator.apache.org/thrift/download/)
Unix/linux 系统，windows+cygwin
java 语言：JDK、Apache Ant
其他语言：Python、PHP、Perl, etc…
编译安装：./configure –》make –》make install
主要流程：编写服务说明，保存到.thrift文件–》根据需要， 编译.thrift文件，生成相应的语言源代码–》根据实际需要， 编写client端和server端代码。
一般将服务放到一个.thrift文件中，服务的编写语法与C语言语法基本一致，在.thrift文件中有主要有以下几个内容：变量声明、数据声明（struct）和服务接口声明（service, 可以继承其他接口）。
下面分析Thrift的tutorial中带的例子tutorial.thrift
59行：include “shared.thrift” 
–
65行：namespace cpp tutorial 
–
80行：const i32 INT32CONSTANT = 9853 
–
4: optional string comment,
service Calculator extends shared.SharedService {
i32 add(1:i32 num1, 2:i32 num2),
i32 calculate(1:i32 logid, 2:Work w) throws (1:InvalidOperation ouch),
`**     要生成C++代码：./**thrift --gen cpp tutorial.thrift，结果代码存放在gen-cpp目录下``**     要生成java代码：./**thrift --gen java tutorial.thrift，结果代码存放在gen-java目录下**       ….. **`
（2） client端和server端代码编写
client端和sever端代码要调用编译.thrift生成的中间文件。
下面分析cpp文件下面的CppClient.cpp和CppServer.cpp代码
![image](http://img1.tuicool.com/bAVFjq2.png!web)
在client端，用户自定义CalculatorClient类型的对象（用户在.thrift文件中声明的服务名称是Calculator， 则生成的中间代码中的主类为CalculatorClient）， 该对象中封装了各种服务，可以直接调用（如client.ping()）, 然后thrift会通过封装的rpc调用server端同名的函数。
在server端，需要实现在.thrift文件中声明的服务中的所有功能，以便处理client发过来的请求。
【参考资料】
1、 [http://wiki.apache.org/thrift/](http://wiki.apache.org/thrift/)
2、 [http://jnb.ociweb.com/jnb/jnbJun2009.html](http://jnb.ociweb.com/jnb/jnbJun2009.html)
3、 [http://blog.rushcj.com/tag/thrift/](http://blog.rushcj.com/tag/thrift/)
4、 [http://www.vvcha.cn/c.aspx?id=31984](http://www.vvcha.cn/c.aspx?id=31984)
5、 [http://www.thoss.org.cn/mediawiki/index.php/Thrift的通信机制及其在cassandra中的应用](http://www.thoss.org.cn/mediawiki/index.php/Thrift%E7%9A%84%E9%80%9A%E4%BF%A1%E6%9C%BA%E5%88%B6%E5%8F%8A%E5%85%B6%E5%9C%A8cassandra%E4%B8%AD%E7%9A%84%E5%BA%94%E7%94%A8)
原创文章，转载请注明： 转载自 [董的博客](http://dongxicheng.org/)
本文链接地址:[http://dongxicheng.org/search-engine/thrift-framework-intro/](http://dongxicheng.org/search-engine/thrift-framework-intro/)
二、Thrift 使用指南
1. 内容概要
本文档比较全面的介绍了thrift语法，代码生成结构和应用经验。本文主要讲述的对象是thrift文件，并未涉及其client和server的编写方法。
本文档大部分内容翻译自文章： [“Thrift:The
 missing Guide“](http://diwakergupta.github.com/thrift-missing-guide/#_versioning_compatibility) 。
2. 语法参考
2.1 Types
Thrift类型系统包括预定义基本类型，用户自定义结构体，容器类型，异常和服务定义
(1) 基本类型
`bool``：布尔类型(``true`
```
or
 value)，占一个字节
```
`byte：有符号字节`
`i16:16位有符号整型`
`i32:32位有符号整型`
`i64:64位有符号整型`
`double``：64位浮点数`
`string：未知编码或者二进制的字符串`
注意，thrift不支持无符号整型，因为很多目标语言不存在无符号整型（如java）。
(2) 容器类型
Thrift容器与类型密切相关，它与当前流行编程语言提供的容器类型相对应，采用java泛型风格表示的。Thrift提供了3种容器类型：
List<t1>：一系列t1类型的元素组成的有序表，元素可以重复
Set<t1>：一系列t1类型的元素组成的无序表，元素唯一
Map<t1,t2>：key/value对（key的类型是t1且key唯一，value类型是t2）。
容器中的元素类型可以是除了service意外的任何合法thrift类型（包括结构体和异常）。
(3)  结构体和异常
Thrift结构体在概念上同C语言结构体类型—-一种将相关属性聚集（封装）在一起的方式。在面向对象语言中，thrift结构体被转换成类。
异常在语法和功能上类似于结构体，只不过异常使用关键字exception而不是struct关键字声明。但它在语义上不同于结构体—当定义一个RPC服务时，开发者可能需要声明一个远程方法抛出一个异常。
结构体和异常的声明将在下一节介绍。
(4)  服务
服务的定义方法在语法上等同于面向对象语言中定义接口。Thrift编译器会产生实现这些接口的client和server桩。具体参见下一节。
(5)  类型定义
Thrift支持C/C++风格的typedef:
`typedef`
```
i32
 MyInteger   \\a
```
`typedef`
```
Tweet
 ReTweet  \\b
```
b.   struct可以使用typedef
2.2   枚举类型
可以像C/C++那样定义枚举类型，如：
`enum`
```
TweetType
 {
```
`TWEET,      ``//a`
`RETWEET = 2,``//b`
`DM = 0xa, ``//c`
`REPLY`
`}       ``//d`
`struct`
```
Tweet
 {
```
`1: required i32 userId;`
`2: required string userName;`
`3: required string text;`
`4: optional Location loc;`
`5: optional TweetType tweetType = TweetType.TWEET`
```
//
 e
```
`16: optional string language =``"english"`
`}`
a.  编译器默认从0开始赋值
b.  可以赋予某个常量某个整数
c.  允许常量是十六进制整数
e.  给常量赋缺省值时，使用常量的全称
注意，不同于protocol buffer，thrift不支持枚举类嵌套，枚举常量必须是32位的正整数
2.3   注释
Thrfit支持shell注释风格，C/C++语言中单行或者多行注释风格
`# This is a valid comment.`
`// C++/Java style single-line comments work just as well.`
2.4   命名空间
Thrift中的命名空间同C++中的namespace和java中的package类似，它们均提供了一种组织（隔离）代码的方式。因为每种语言均有自己的命名空间定义方式（如python中有module），thrift允许开发者针对特定语言定义namespace：
`namespace`
```
cpp
 com.example.project
```
`// a`
`namespace`
```
java
 com.example.project
```
`// b`
a．  转化成namespace com { namespace example { namespace project {
b．  转换成package com.example.project
2.5   文件包含
Thrift允许thrift文件包含，用户需要使用thrift文件名作为前缀访问被包含的对象，如：
`include``"tweet.thrift"`
```
//
 a
```
`...`
`struct`
```
TweetSearchResult
 {
```
`1: list<tweet.Tweet> tweets;`
```
//
 b
```
`}`
a．  thrift文件名要用双引号包含，末尾没有逗号或者分号
2.6   常量
Thrift允许用户定义常量，复杂的类型和结构体可使用JSON形式表示。
`const`
```
i32
 INT_CONST = 1234;
```
`// a`
`const`
```
map<string,string>
 MAP_CONST = {
```
`"hello"``:``"world"``,``"goodnight"``:``"moon"``}`
a．  分号是可选的，可有可无；支持十六进制赋值。
结构体由一系列域组成，每个域有唯一整数标识符，类型，名字和可选的缺省参数组成。如：
`struct`
```
Tweet
 {
```
`1: required i32 userId;                 `
```
//
 a
```
`2: required string userName;            `
```
//
 b
```
`3: required string text;`
`4: optional Location loc;               `
```
//
 c
```
`16: optional string language =``"english"`
```
//
 d
```
`}`
`struct`
```
Location
 {
```
`// e`
`1: required``double``latitude;`
`2: required``double``longitude;`
`}`
a.  每个域有一个唯一的，正整数标识符
b.  每个域可以标识为required或者optional（也可以不注明）
c.  结构体可以包含其他结构体
e.  一个thrift中可定义多个结构体，并存在引用关系
规范的struct定义中的每个域均会使用required或者optional关键字进行标识。如果required标识的域没有赋值，thrift将给予提示。如果optional标识的域没有赋值，该域将不会被序列化传输。如果某个optional标识域有缺省值而用户没有重新赋值，则该域的值一直为缺省值。
与service不同，结构体不支持继承，即，一个结构体不能继承另一个结构体。
2.8   定义服务
在流行的序列化/反序列化框架（如protocol buffer）中，thrift是少有的提供多语言间RPC服务的框架。
Thrift编译器会根据选择的目标语言为server产生服务接口代码，为client产生桩代码。
`//“Twitter”与“{”之间需要有空格！！！`
`service Twitter {`
`// 方法定义方式类似于C语言中的方式，它有一个返回值，一系列参数和可选的异常`
`// 列表. 注意，参数列表和异常列表定义方式与结构体中域定义方式一致.`
`void``ping(),                                   `
```
//
 a
```
`bool`
```
postTweet(1:Tweet
 tweet);
```
`// b`
`TweetSearchResult searchTweets(1:string query);`
```
//
 c
```
`// ”oneway”标识符表示client发出请求后不必等待回复（非阻塞）直接进行下面的操作，`
`// ”oneway”方法的返回值必须是void`
`oneway``void``zip()                              `
```
//
 d
```
`}`
a． 函数定义可以使用逗号或者分号标识结束
b． 参数可以是基本类型或者结构体，参数是只读的（const），不可以作为返回值！！！
c． 返回值可以是基本类型或者结构体
注意，函数中参数列表的定义方式与struct完全一样
Service支持继承，一个service可使用extends关键字继承另一个service
3.  产生代码
本节介绍thrift产生各种目标语言代码的方式。本节从几个基本概念开始，逐步引导开发者了解产生的代码是怎么样组织的，进而帮助开发者更快地明白thrift的使用方法。
概念
Thrift的网络栈如下所示：
![image](http://img1.tuicool.com/eIZzEv.png!web)
3.1   Transport
Transport层提供了一个简单的网络读写抽象层。这使得thrift底层的transport从系统其它部分（如：序列化/反序列化）解耦。以下是一些Transport接口提供的方法：
`open`
`close`
`read`
`write`
`flush`
除了以上几个接口，Thrift使用ServerTransport接口接受或者创建原始transport对象。正如名字暗示的那样，ServerTransport用在server端，为到来的连接创建Transport对象。
`open`
`listen`
`accept`
`close`
3.2   Protocol
Protocol抽象层定义了一种将内存中数据结构映射成可传输格式的机制。换句话说，Protocol定义了datatype怎样使用底层的Transport对自己进行编解码。因此，Protocol的实现要给出编码机制并负责对数据进行序列化。
Protocol接口的定义如下：
`writeMessageBegin(name, type, seq)`
`writeMessageEnd()`
`writeStructBegin(name)`
`writeStructEnd()`
`writeFieldBegin(name, type, id)`
`writeFieldEnd()`
`writeFieldStop()`
`writeMapBegin(ktype, vtype, size)`
`writeMapEnd()`
`writeListBegin(etype, size)`
`writeListEnd()`
`writeSetBegin(etype, size)`
`writeSetEnd()`
`writeBool(``bool``)`
`writeByte(byte)`
`writeI16(i16)`
`writeI32(i32)`
`writeI64(i64)`
`writeDouble(``double``)`
`writeString(string)`
`name, type, seq = readMessageBegin()`
`readMessageEnd()`
`name = readStructBegin()`
`readStructEnd()`
`name, type, id = readFieldBegin()`
`readFieldEnd()`
`k, v, size = readMapBegin()`
`readMapEnd()`
`etype, size = readListBegin()`
`readListEnd()`
`etype, size = readSetBegin()`
`readSetEnd()`
`bool`
```
=
 readBool()
```
`byte = readByte()`
`i16 = readI16()`
`i32 = readI32()`
`i64 = readI64()`
`double`
```
=
 readDouble()
```
`string = readString()`
下面是一些对大部分thrift支持的语言均可用的protocol：
(1)     binary：简单的二进制编码
(2)     Compact：具体见THRIFT-11
3.3   Processor
Processor封装了从输入数据流中读数据和向数据数据流中写数据的操作。读写数据流用Protocol对象表示。Processor的结构体非常简单:
`interface TProcessor {`
`bool`
```
process(TProtocol
 in, TProtocol out) throws TException
```
`}`
与服务相关的processor实现由编译器产生。Processor主要工作流程如下：从连接中读取数据（使用输入protocol），将处理授权给handler（由用户实现），最后将结果写到连接上（使用输出protocol）。
3.4   Server
Server将以上所有特性集成在一起：
（1）  创建一个transport对象
（2）  为transport对象创建输入输出protocol
（3）  基于输入输出protocol创建processor
（4）  等待连接请求并将之交给processor处理
3.5   应用举例
下面，我们讨论thrift文件产生的特定语言代码。下面给出thrift文件描述：
`namespace`
```
cpp
 thrift.example
```
`namespace`
```
java
 thrift.example
```
`enum`
```
TweetType
 {
```
`TWEET,`
`RETWEET = 2,`
`DM = 0xa,`
`REPLY`
`}`
`struct`
```
Location
 {
```
`1: required``double``latitude;`
`2: required``double``longitude;`
`}`
`struct`
```
Tweet
 {
```
`1: required i32 userId;`
`2: required string userName;`
`3: required string text;`
`4: optional Location loc;`
`5: optional TweetType tweetType = TweetType.TWEET;`
`16: optional string language =``"english"``;`
`}`
`typedef`
```
list<Tweet>
 TweetList
```
`struct`
```
TweetSearchResult
 {
```
`1: TweetList tweets;`
`}`
`const`
```
i32
 MAX_RESULTS = 100;
```
`service Twitter {`
`void``ping(),`
`bool`
```
postTweet(1:Tweet
 tweet);
```
`TweetSearchResult searchTweets(1:string query);`
`oneway``void``zip()`
`}`
(1) Java语言
(a)  产生的文件
一个单独的文件（Constants.java）包含所有的常量定义。
每个结构体，枚举或者服务各占一个文件
|– TweetSearchResult.java
(b)  类型
thrift将各种基本类型和容器类型映射成java类型：
`bool`
```
:
 boolean
```
`byte: byte`
`i16:``short`
`i32:``int`
`i64:``long`
`double``:``double`
`string: String`
`list<t1>: List<t1>`
`set<t1>: Set<t1>`
`map<t1,t2>: Map<t1, t2>`
(c)  typedef
Java不支持typedef，它只使用原始类型，如，在上面的例子中，产生的代码中，TweetSearchResult会被还原成list<Tweet> tweets
(d)  Enum
Thrift直接将枚举类型映射成java的枚举类型。用户可以使用geValue方法获取枚举常量的值。此外，编译器会产生一个findByValue方法获取枚举对应的数值。
(e)  常量
Thrift把所有的常量放在一个叫Constants的public类中，每个常量修饰符是public static final。
(2)  C++语言
(a)  产生的文件
所有变量均存放在一个.cpp/.h文件对中
所有的类型定义（枚举或者结构体）存放到另一个.cpp/.h文件对中
每一个service有自己的.cpp/.h文件
|– example_constants.cpp
|– example_constants.h
`– Twitter_server.skeleton.cpp
Python，Ruby，javascript等
4.  实践经验
thrift文件内容可能会随着时间变化的。如果已经存在的消息类型不再符合设计要求，比如，新的设计要在message格式中添加一个额外字段，但你仍想使用以前的thrift文件产生的处理代码。如果想要达到这个目的，只需：
（1）  不要修改已存在域的整数编号
（2）  新添加的域必须是optional的，以便格式兼容。对于一些语言，如果要为optional的字段赋值，需要特殊处理，比如对于C++语言，要为
`struct``Example{`
`1 : i32 id,`
`2 : string name,`
`3 : optional age,`
`}`
中的optional字段age赋值，需要将它的__isset值设为true，这样才能序列化并传输或者存储（不然optional字段被认为不存在，不会被传输或者存储），
`Example example;`
`......`
`example.age=10,`
`example.__isset.age =``true``;``//__isset是每个thrift对象的自带的public成员，来指定optional字段是否启用并赋值。`
`......`
（3）  非required域可以删除，前提是它的整数编号不会被其他域使用。对于删除的字段，名字前面可添加“OBSOLETE_”以防止其他字段使用它的整数编号。
（4） thrift文件应该是unix格式的（windows下的换行符与unix不同，可能会导致你的程序编译不过），如果是在window下编写的，可使用dos2unix转化为unix格式。
（5）  貌似当前的thrift版本（0.6.1）不支持常量表达式的定义（如 const i32 DAY = 24 * 60 * 60），这可能是考虑到不同语言，运算符不尽相同。
原创文章，转载请注明： 转载自 [董的博客](http://dongxicheng.org/)
本文链接地址:[http://dongxicheng.org/search-engine/thrift-guide/](http://dongxicheng.org/search-engine/thrift-guide/)
作者： [Dong](http://dongxicheng.org/about/)，作者介绍： [http://dongxicheng.org/about/](http://dongxicheng.org/about/)
三、使用Thrift RPC编写程序
1． 概述
本文以C++语言为例介绍了thrift RPC的使用方法，包括对象序列化和反序列化，数据传输和信息交换等。
本文采用了一个示例进行说明，该示例主要完成传输（上报日志或者报表）功能，该示例会贯穿本文，内容涉及thrift定义，代码生成，thrift类说明，client编写方法，server编写方法等。
2． 示例描述
假设我们要使用thrift RPC完成一个数据传输任务，数据格式和PRC接口用一个thrift文件描述，具体如下：
(1) book.thrift，用于描述书籍信息的thrift接口
`//book.thrift，`
`namespace`
```
cpp
 example
```
`struct`
```
Book_Info
 {
```
`1: i32 book_id,`
`2: string book_name,`
`3: string book_author,`
`4:``double``book_price,`
`5: string book_publisher,`
`}`
(2) rpc.thrift，client向server传输数据（上报日志或者报表）的RPC接口
`//rpc.thrift`
`namespace`
```
cpp
 example
```
`include``"book.thrift"`
`service BookServlet {`
`bool`
```
Sender(1:
 list<book.Book_Info> books);
```
`oneway``void`
```
Sender2(1:
 list<book.Book_Info> books);
```
`}`
说明：该thrift文件定义了一个service，它包含两个接口，server端需要实现这两个接口以对client提供服务。其中，第一个接口函数是阻塞式的，即要等待server返回值以后才能继续，另外一个声明为oneway类型（返回值为void），表明该函数是非阻塞式的，将数据发给 server后不必等待返回结果，但使用该函数时，需要考虑server的承受能力，适度的调整发送频率。
3． Thrift文件与生成的代码对应关系
每个thrift文件会产生四个文件，分别为：${thrift_name}_constants.h，${thrift_name}_constants.cpp，${thrift_name}_types.h，${thrift_name}_types.cpp
对于含有service的thrift文件，会额外生成两个文件，分别为：${service_name}.h，${service_name}.cpp
对于含有service的thrift文件，会生成一个可用的server桩：${service_name}._server.skeleton.cpp
对于本文中的例子，会产生以下文件：
> 
book_constants.h book_constants.cpp
book_types.h book_types.cpp
rpc_constants.h rpc_constants.cpp
rpc_types.h rpc_types.cpp
BookServlet.h BookServlet.cpp
BookServlet_server.skeleton.cpp
4． Thrift类介绍
Thrift代码包（位于thrift-0.6.1/lib/cpp/src）有以下几个目录：
> 
concurrency：并发和时钟管理方面的库
processor：Processor相关类
protocal：Protocal相关类
transport：transport相关类
4.1 Transport类（how is transmitted？）
负责数据传输，有以下几个可用类：
TFileTransport：文件（日志）传输类，允许client将文件传给server，允许server将收到的数据写到文件中。
THttpTransport：采用Http传输协议进行数据传输
TSocket：采用TCP Socket进行数据传输
TZlibTransport：压缩后对数据进行传输，或者将收到的数据解压
下面几个类主要是对上面几个类地装饰（采用了装饰模式），以提高传输效率。
TBufferedTransport：对某个Transport对象操作的数据进行buffer，即从buffer中读取数据进行传输，或者将数据直接写入buffer
TFramedTransport：同TBufferedTransport类似，也会对相关数据进行buffer，同时，它支持定长数据发送和接收。
TMemoryBuffer：从一个缓冲区中读写数据
4.2 Protocol类（what is transmitted？）
负责数据编码，主要有以下几个可用类：
TBinaryProtocol：二进制编码
TJSONProtocol：JSON编码
TCompactProtocol：密集二进制编码
TDebugProtocol：以用户易读的方式组织数据
4.3 Server类（providing service for clients）
TSimpleServer：简单的单线程服务器，主要用于测试
TThreadPoolServer：使用标准阻塞式IO的多线程服务器
TNonblockingServer：使用非阻塞式IO的多线程服务器，TFramedTransport必须使用该类型的server
5． 对象序列化和反序列化
Thrift中的Protocol负责对数据进行编码，因而可使用Protocol相关对象进行序列化和反序列化。
由于对象序列化和反序列化不设计传输相关的问题，所以，可使用TBinaryProtocol和TMemoryBuffer，具体如下：
（1） 使用thrift进行对象序列化
//对对象object进行序列化，保存到str中
`template``<``typename``Type>`
`void`
```
Object2String(Type&
 object, string &str) {
```
`shared_ptr<TMemoryBuffer> membuffer(``new``TMemoryBuffer());`
`shared_ptr<TProtocol> protocol(``new``TBinaryProtocol(membuffer));`
`object.write(protocol.get());`
`str.clear();`
`str = membuffer.getBufferAsString();`
`}`
（2）使用thrift进行对象反序列化
`//对str中保存的对象进行反序列化，保存到object中`
`template``<``typename``Type>`
`void`
```
String2Object(string&
 buffer, Type &object) {
```
`shared_ptr<TMemoryBuffer> membuffer(``new``TMemoryBuffer(`
`reinterpret_cast``<uint*>(buffer.data())));`
`shared_ptr<TProtocol> protocol(``new``TBinaryProtocol(membuffer));`
`object.read(protocol.get());`
`}`
6． 编写client和server
6.1 client端代码编写
Client编写的方法分为以下几个步骤：
（1） 定义TTransport，为你的client设置传输方式（如socket， http等）。
（2） 定义Protocal，使用装饰模式（Decorator设计模式）封装TTransport，为你的数据设置编码格式（如二进制格式，JSON格式等）
（3） 实例化client对象，调用服务接口。
说明：如果用户在thrift文件中定义了一个叫${server_name}的service，则会生成一个叫${server_name}Client的对象，比如，我给出的例子中，thrift会自动生成一个叫BookServletClient的类，Client端的代码编写如下：
`#include " gen-cpp/BookServlet.h" //一定要包含该头文件`
`//其头文件，其他using namespace …….`
`int``main(``int``argc,``char`
```
**
 argv) {
```
`shared_ptr<TTransport> socket(``new``TSocket(``"localhost"`
```
,
 9090));
```
`shared_ptr<TTransport> transport(``new``TBufferedTransport(socket));`
`shared_ptr<TProtocol> protocol(``new``TBinaryProtocol(transport));`
`example::BookServletClient client(protocol);`
`try``{`
`transport->open();`
`vector<example::Book_Info> books;`
`…...`
`client.Sender(books);``//RPC函数，调用serve端的该函数`
`transport->close();`
`}``catch`
```
(TException
 &tx) {
```
`printf``(`
```
"ERROR:
 %s\n"
```
`, tx.what());`
`}`
`}`
6.2 Server端代码编写
（1） 定义一个TProcess，这个是thrift根据用户定义的thrift文件自动生成的类
（2） 使用TServerTransport获得一个TTransport
（3） 使用TTransportFactory，可选地将原始传输转换为一个适合的应用传输（典型的是使用TBufferedTransportFactory）
（4） 使用TProtocolFactory，为TTransport创建一个输入和输出
（5） 创建TServer对象（单线程，可以使用TSimpleServer；对于多线程，用户可使用TThreadPoolServer或者TNonblockingServer），调用它的server()函数。
说明：thrift会为每一个带service的thrift文件生成一个简单的server代码（桩），在例子中，thrift会生成BookServlet_server.skeleton.cpp，用户可以在这个文件基础上实现自己的功能。
`#include "gen-cpp/BookServlet.h"`
`#include <protocol/TBinaryProtocol.h>`
`#include <server/TSimpleServer.h>`
`#include <transport/TServerSocket.h>`
`#include <transport/TBufferTransports.h>`
`using``namespace``::apache::thrift;`
`using``namespace``::apache::thrift::protocol;`
`using``namespace``::apache::thrift::transport;`
`using``namespace``::apache::thrift::server;`
`using``boost::shared_ptr;`
`using``namespace``example;`
`class`
```
BookServletHandler
 :
```
`virtual``public`
```
BookServletIf
 {
```
`public``:`
`BookServletHandler() {`
`// Your initialization goes here`
`}`
`//用户需实现这个接口`
`bool``Sender(``const`
```
std::vector<example::Book_Info>
 & books) {
```
`// Your implementation goes here`
`printf``(``"Sender\n"``);`
`}`
`//用户需实现这个接口`
`void``Sender2(``const`
```
std::vector<example::Book_Info>
 & books) {
```
`// Your implementation goes here`
`printf``(``"Sender2\n"``);`
`}`
`};`
`int``main(``int``argc,``char`
```
**argv)
 {
```
`int`
```
port
 = 9090;
```
`shared_ptr<BookServletHandler> handler(``new``BookServletHandler());`
`shared_ptr<TProcessor> processor(``new``BookServletProcessor(handler));`
`shared_ptr<TServerTransport> serverTransport(``new``TServerSocket(port));`
`shared_ptr<TTransportFactory> transportFactory(``new``TBufferedTransportFactory());`
`shared_ptr<TProtocolFactory> protocolFactory(``new``TBinaryProtocolFactory());`
`TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);`
`server.serve();`
`return``0;`
`}`
7． 总结
至此，关于thrift框架的三篇文章已经全部完成，包括：
(1) Thrift框架介绍: [Thrift框架介绍](http://dongxicheng.org/search-engine/thrift-framework-intro/)
(2) Thrift文件编写方法： [Thrift使用指南](http://dongxicheng.org/search-engine/thrift-guide/)
(3) Thrift RPC使用方法： [利用Thrift
 RPC编写程序](http://dongxicheng.org/search-engine/thrift-rpc/)
与thrift类似的开源RPC框架还有google的protocal buffer，它虽然支持的语言比较少，但效率更高，因而受到越来越多的关注。
由于thrift开源时间很早，经受了时间的验证，因而许多系统更愿意采用thrift，如Hadoop，Cassandra等。
附：thrift与protocal buffer比较
![image](http://img0.tuicool.com/NR36zq.gif)
从上面的比较可以看出，thrift胜在“丰富的特性“上，而protocal buffer胜在“文档化”非常好上。在具体实现上，它们非常类似，都是使用唯一整数标记字段域，这就使得增加和删除字段与不会破坏已有的代码。
它们的最大区别是thrift支持完整的client/server RPC框架，而protocal buffer只会产生接口，具体实现，还需要用户做大量工作。
另外，从序列化性能上比较，Protocal Buffer要远远优于thrift，具体可参考：[http://www.ibm.com/developerworks/cn/linux/l-cn-gpb/?ca=drs-tp4608](http://www.ibm.com/developerworks/cn/linux/l-cn-gpb/?ca=drs-tp4608) 。
8. 参考资料
(1) [http://stuartsierra.com/2008/07/10/thrift-vs-protocol-buffers](http://stuartsierra.com/2008/07/10/thrift-vs-protocol-buffers)
(2) Thrift: Scalable Cross-Language Services Implementation. Mark Slee, Aditya Agarwal and Marc Kwiatkowski. Facebook
(3) Thrift网站： [http://thrift.apache.org/](http://thrift.apache.org/)
(4) Protocal Buffer网站：
[http://code.google.com/intl/zh-CN/apis/protocolbuffers/docs/overview.html](http://code.google.com/intl/zh-CN/apis/protocolbuffers/docs/overview.html)
原创文章，转载请注明： 转载自 [董的博客](http://dongxicheng.org/)
本文链接地址:[http://dongxicheng.org/search-engine/thrift-rpc/](http://dongxicheng.org/search-engine/thrift-rpc/)
四、Thrift 内部实现原理
Thrift由两部分组成：编译器（在compiler目录下，采用C++编写）和服务器（在lib目录下），其中编译器的作用是将用户定义的thrift文件编译生成对应语言的代码，而服务器是事先已经实现好的、可供用户直接使用的RPC Server（当然，用户也很容易编写自己的server）。同大部分编译器一样，Thrift编译器（采用C++语言编写）也分为词法分析、语法分析等步骤，Thrift使用了开源的flex和Bison进行词法语法分析（具体见thrift.ll和thrift.yy），经过语法分析后，Thrift
 根据对应语言的模板（在compiler\cpp\src\generate目录下）生成相应的代码。对于服务器实现而言，Thrift仅包含比较经典的服务器模型，比如单线程模型（TSimpleServer），线程池模型（TThreadPoolServer）、一个请求一个线程（TThreadedServer）和非阻塞模型(TNonblockingServer)等。本文将以C++为例进行一个实例分析。
假设用户编写了以下Thrift文件：
> 
1: required string name,
2: optional string content,
void SendLog(1:list<LogInfo> loglist);
用户使用命令“thrift –gen cpp example.thrift”可生成C++代码，该代码包含以下文件：
> 
example_types.h  //struct定义
example_types.cpp  //struct实现
LogSender.h  //service定义
LogSender.cpp  //service实现和LogSenderClient实现
LogSender_server.skeleton.cpp //一个实例RPC Server
用户可以这样编写Client：
> 
shared_ptr socket(new TSocket(“8.8.8.8″, 9090));
shared_ptr transport(new TBufferedTransport(socket));
shared_ptr protocol(new TBinaryProtocol(transport));
LogSenderClient client(protocol);
vector<LogInfo> logInfos;
LogInfo logInfo(“image”, “10:9:0 visit:xxxxxx”);
logInfos.push_back(logInfo);
client.SendLog(logInfos);
} catch (TException &tx) {
printf(“ERROR: %s\n”, tx.what());
为了深入分析这段代码，我们看一下client.SendLog()函数的内部实现（在LogSender.cpp中）：
> 
void LogSenderClient::SendLog(const std::vector<LogInfo> & loglist)
send_SendLog(loglist);
void LogSenderClient::send_SendLog(const std::vector<LogInfo> & loglist)
oprot_->writeMessageBegin(“SendLog”, ::apache::thrift::protocol::T_CALL, cseqid);
LogSender_SendLog_pargs args;
args.loglist = &loglist;
oprot_->writeMessageEnd();
oprot_->getTransport()->flush();
oprot_->getTransport()->writeEnd();
void LogSenderClient::recv_SendLog()
::apache::thrift::protocol::TMessageType mtype;
iprot_->readMessageBegin(fname, mtype, rseqid);
if (mtype == ::apache::thrift::protocol::T_EXCEPTION) {
if (mtype != ::apache::thrift::protocol::T_REPLY) {
if (fname.compare(“SendLog”) != 0) {
LogSender_SendLog_presult result;
iprot_->readMessageEnd();
iprot_->getTransport()->readEnd();
阅读上面的代码，可以看出，RPC函数SendLog()实际上被转化成了两个函数：send_SendLog和recv_SendLog，分别用于发送数据和接收结果。数据是以消息的形式表示的，消息头部是RPC函数名，消息内容是RPC函数的参数。
我们再进一步分析RPC Server端，一个server的编写方法（在LogSender.cpp中）如下：
> 
shared_ptr protocolFactory(new TBinaryProtocolFactory());
shared_ptr handler(new LogSenderHandler());
shared_ptr processor(new LogSenderProcessor(handler));
shared_ptr serverTransport(new TServerSocket(9090));
shared_ptr transportFactory(new TBufferedTransportFactory());
TSimpleServer server(processor,
printf(“Starting the server…\n”);
Server端最重要的类是LogSenderProcessor，它内部有一个映射关系processMap_，保存了所有RPC函数名到函数实现句柄的映射，对于LogSender而言，它只保存了一个RPC映射关系：
> 
processMap_[" SendLog"] = &LogSenderProcessor::process_SendLog;
其中，process_SendLog是一个函数指针，它的实现如下：
> 
void LogSenderProcessor::process_SendLog(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot)
LogSender_SendLog_args args;
iprot->readMessageEnd();
iprot->getTransport()->readEnd();
LogSender_SendLog_result result;
iface_->SendLog(args.loglist);//调用用户编写的函数
} catch (const std::exception& e) {
oprot->writeMessageBegin(“SendLog”, ::apache::thrift::protocol::T_REPLY, seqid);
oprot->writeMessageEnd();
oprot->getTransport()->flush();
oprot->getTransport()->writeEnd();
LogSenderProcessor中一个最重要的函数是process()，它是服务器的主体函数，服务器端（socket server）监听到客户端有请求到达后，会检查消息类型，并检查processMap_映射，找到对应的消息处理函数，并调用之（注意，这个地方可以采用各种并发模型，比如one-request-one-thread，thread pool等）。
通过上面的分析可以看出，Thrift最重要的组件是编译器（采用C++编写），它为用户生成了网络通信相关的代码，从而大大减少了用户的编码工作。
