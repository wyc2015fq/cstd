# Thrift - 深之JohnChen的专栏 - CSDN博客

2017年07月04日 11:16:13[byxdaz](https://me.csdn.net/byxdaz)阅读数：1542



Thrift最初是由
 Facebook 开发用做系统内各语言之间的RPC通信的一个可扩展且跨语言的软件框架，它结合了功能强大的软件堆栈和代码生成引擎，允许定义一个简单的定义文件中的数据类型和服务接口，以作为输入文件，编译器生成代码用来方便地生成RPC客户端和服务器通信的无缝跨编程语言。它采用接口描述语言定义并创建服务，支持可扩展的跨语言服务开发，所包含的代码生成引擎可以在多种语言中，如
 C++, Java, Python, PHP, Ruby, Erlang, Perl, Haskell, C#, Cocoa,Smalltalk 等创建高效的、无缝的服务，其传输数据采用二进制格式，相对 XML和
 JSON体积更小，对于高并发、大数据量和多语言的环境更有优势。

**Thrift与其他传输方式的比较**
 1、xml与JSON相比体积太大，但是xml传统，也不算复杂。
 2、json体积较小，新颖，但不够完善。
 3、thrift体积超小，使用起来比较麻烦，不如前两者轻便，但是对于高并发、数据传输量大、多语言环境，使用
 thrift还是值得的。

thrift源代码：

http://thrift.apache.org/

https://github.com/apache/thrift/

# **Thrift基础架构**

Thrift是一个服务端和客户端的通讯架构体系，Thrift具有自己内部定义的传输协议规范(TProtocol)和传输数据标准(TTransports)，通过IDL脚本对传输数据的数据结构(struct)和传输数据的业务逻辑(service)根据不同的运行环境快速的构建相应的代码，并且通过自己内部的序列化机制对传输的数据进行简化和压缩提高高并发、大型系统中数据交互的成本，下图描绘了Thrift的整体架构，分为6个部分。![](https://img-blog.csdn.net/20170704110751880)

# 数据类型

Thrift脚本可定义的数据类型包括以下几种类型：

·基本类型：

·bool：布尔值，true或
 false，对应 Java
的 boolean

·byte：8位有符号整数，对应
 Java的 byte

·i16：16位有符号整数，对应
 Java的 short

·i32：32位有符号整数，对应
 Java的 int

·i64：64位有符号整数，对应
 Java的 long

·double：64位浮点数，对应
 Java的 double

·string：未知编码文本或二进制字符串，对应
 Java 的String

·结构体类型：

struct：定义公共的对象，类似于
 C 语言中的结构体定义，在 Java中是一个
 JavaBean类，约束：struct不能继承，但是可以嵌套，不能嵌套自己

其成员都是有明确类型

成员是被正整数编号过的，其中的编号使不能重复的，这个是为了在传输过程中编码使用（详情往下看备注1）

成员分割符可以是逗号（,）或是分号（;），而且可以混用，但是为了清晰期间，建议在定义中只使用一种，比如java学习者可以就使用逗号（;）

字段会有optional和required之分（详情往下看备注2）

每个字段可以设置默认值

同一文件可以定义多个struct，也可以定义在不同的文件，进行include引入

struct例子，定义如下：

`struct UserDemo {`

`　　``1: i32 id;`

`　　``2: string name;`

`　　``3: i32 age =25;`

`　　``4``：`
```
string phone;
```

`}`

**备注1：**数字标签作用非常大，随着项目开发的不断发展，也许字段会有变化，但是建议不要轻易修改这些数字标签，修改之后如果没有同步客户端和服务器端会让一方解析出问题。

**备注2：**关于
 struct 字段类型，规范的 struct定义中的每个域均会使用
 required或者 optional关键字进行标识，但是如果不指定则为无类型，可以不填充该值，但是在序列化传输的时候也会序列化进去。其中
 optional是不填充则不序列化，required是必须填充也必须序列化。如果
 required标识的域没有赋值，Thrift将给予提示；如果optional标识的域没有赋值，该域将不会被序列化传输；如果某个
 optional标识域有缺省值而用户没有重新赋值，则该域的值一直为缺省值；如果某个 optional标识域有缺省值或者用户已经重新赋值，而不设置它的
 __isset为 true，也不会被序列化传输。
异常在语法和功能上相当于结构体，差别是异常使用关键字 exception而不是
 struct声明。它在语义上不同于结构体：当定义一个 RPC服务时，开发者可能需要声明一个远程方法抛出一个异常。

·容器类型：
- list<t>:元素类型为t的有序表，容许元素重复。对应c++的vector，java的ArrayList或者其他语言的数组（官方文档说是ordered
 list不知道如何理解？排序的？c++的vector不排序
- set<t>:元素类型为t的无序表，不容许元素重复。对应c++中的set，java中的HashSet,python中的set，php中没有set，则转换为list类型了
- map<t,t>:键类型为t，值类型为t的kv对，键不容许重复。对用c++中的map,
 Java的HashMap, PHP对应
 array, Python/Ruby的dictionary。

·异常类型：

oexception：对应
 Java 的 Exception，异常在语法和功能上类似于结构体，差别是异常使用关键字exception，而且异常是继承每种语言的基础异常类。

o例子：

exception Extest {

1: i32 errorCode,

2:string
message,

3: StUser userinfo

}

·服务类型：

service：对应服务的类，约束：继承类必须实现这些方法

参数可以是基本类型或者结构体

所有的参数都是const类型，不能作为返回值

返回值可以是void（oneway的返回值一定是void）

服务支持继承，一个service可使用extends关键字继承另一个service

服务不支持重载

例子：

service QuerySrv{

/**

*本方法实现根据名字和年龄来找到对应的用户信息

*/

User
 queryUser(1:string name, 2:i32 age);

/**

*本方法实现根据id找到对应用户的手机号码

*/

string
 queryPhone(1:i32 id);

}

Enum

不同于protocal
 buffer，thrift不支持枚举类嵌套，枚举常量必须是32位的正整数

·编译器默认从0开始赋值

·可以赋予某个常量某个整数

·允许常量是十六进制整数

·末尾没有分号

·给常量赋缺省值时，使用常量的全称

例子：

enum EnOpType {

CMD_OK =0,//
 (0)

CMD_NOT_EXIT =2000,//
 (2000)

CMD_EXIT =2001,//
 (2001)

CMD_ADD =2002//
 (2002)

}

struct StUser {

1: required i32 userId;

2: required
string userName;

3: optional EnOpType cmd_code = EnOpType.CMD_OK;
 // (0)

4: optional
string language =“english”

}

Namespace

Thrift中的命名空间类似于C++中的namespace和java中的package，它们提供了一种组织（隔离）代码的简便方式。名字空间也可以用于解决类型定义中的名字冲突。

例子：

namespace cpp
com.example.test   //(1)

namespace java
com.example.test  //(2)

namespace php
com.example.test

(1)． 转化成namespace com { namespace example { namespace project {

(2)． 转换成package com.example.project

Includes

便于管理、重用和提高模块性/组织性，常常分割Thrift定义在不同的文件中。包含文件搜索方式与c++一样。Thrift允许文件包含其它thrift文件，用户需要使用thrift文件名作为前缀访问被包含的对象

include"test.thrift"

...

struct StSearchResult {

1:
 in32 uid;

...

}

# Thrift IDL

基于Thrift Type书写IDL,文件后缀名thrift

例子：

struct User{

1:i64
 id,

2:string
 name,

3:i64
 timestamp,

4:bool
 vip

}

service UserService{

User getById(1:i64
 id)

}

# 传输协议

Thrift支持多种传输协议，用户可以根据实际需求选择合适的类型。Thrift传输协议上总体可划分为文本
 (text) 和二进制 (binary)传输协议两大类，一般在生产环境中使用二进制类型的传输协议为多数（相对于文本和
 JSON 具有更高的传输效率）。常用的协议包含：

TBinaryProtocol：是Thrift的默认协议，使用二进制编码格式进行数据传输，基本上直接发送原始数据。

TCompactProtocol：压缩的、密集的数据传输协议，基于Variable-length
 quantity的zigzag编码格式。

TJSONProtocol：以JSON
 (JavaScript Object Notation)数据编码协议进行数据传输。

TDebugProtocol：常常用以编码人员测试，以文本的形式展现方便阅读。

# 传输方式

与传输协议一样，Thrift也支持几种不同的传输方式。
**1. TSocket：**阻塞型
 socket，用于客户端，采用系统函数
 read 和 write进行读写数据。
**2. TServerSocket：**非阻塞型
 socket，用于服务器端，accecpt到的socket类型都是
 TSocket（即阻塞型
 socket）。
**3. TBufferedTransport**和**TFramedTransport**都是有缓存的，均继承TBufferBase，调用下一层
 TTransport 类进行读写操作吗，结构极为相似。其中
 TFramedTransport 以帧为传输单位，帧结构为：4个字节（int32_t）+传输字节串，头4个字节是存储后面字节串的长度，该字节串才是正确需要传输的数据，因此
 TFramedTransport 每传一帧要比
 TBufferedTransport 和TSocket多传4个字节。
**4. TMemoryBuffer**继承
 TBufferBase，用于程序内部通信用，不涉及任何网络I/O，可用于三种模式：（1）OBSERVE模式，不可写数据到缓存；（2）TAKE_OWNERSHIP模式，需负责释放缓存；（3）COPY模式，拷贝外面的内存块到TMemoryBuffer。
**5. TFileTransport**直接继承
 TTransport，用于写数据到文件。对事件的形式写数据，主线程负责将事件入列，写线程将事件入列，并将事件里的数据写入磁盘。这里面用到了两个队列，类型为
 TFileTransportBuffer，一个用于主线程写事件，另一个用于写线程读事件，这就避免了线程竞争。在读完队列事件后，就会进行队列交换，由于由两个指针指向这两个队列，交换只要交换指针即可。它还支持以
 chunk（块）的形式写数据到文件。
**6. TFDTransport**是非常简单地写数据到文件和从文件读数据，它的
 write 和read函数都是直接调用系统函数
 write和read进行写和读文件。
**7. TSimpleFileTransport**直接继承
 TFDTransport，没有添加任何成员函数和成员变量，不同的是构造函数的参数和在
 TSimpleFileTransport 构造函数里对父类进行了初始化（打开指定文件并将fd传给父类和设置父类的close_policy为CLOSE_ON_DESTROY）。
**8. TZlibTransport**跟
 TBufferedTransport和TFramedTransport一样，调用下一层
 TTransport 类进行读写操作。它采用<zlib.h>提供的
 zlib 压缩和解压缩库函数来进行压解缩，写时先压缩再调用底层
 TTransport 类发送数据，读时先调用
 TTransport 类接收数据再进行解压，最后供上层处理。
**9. TSSLSocket**继承TSocket，阻塞型
 socket，用于客户端。采用
 openssl 的接口进行读写数据。checkHandshake(）函数调用
 SSL_set_fd 将fd和
 ssl绑定在一起，之后就可以通过
 ssl 的 SSL_read和SSL_write接口进行读写网络数据。
**10. TSSLServerSocket**继承
 TServerSocket，非阻塞型
 socket，用于服务器端。accecpt到的
 socket类型都是 TSSLSocket类型。
**11. THttpClient**和**THttpServer**是基于Http1.1协议的继承
 Transport类型，均继承
 THttpTransport，其中
 THttpClient 用于客户端，THttpServer用于服务器端。两者都调用下一层
 TTransport 类进行读写操作，均用到TMemoryBuffer作为读写缓存，只有调用
 flush()函数才会将真正调用网络
 I/O接口发送数据。

TTransport是所有
 Transport 类的父类，为上层提供了统一的接口而且通过
 TTransport 即可访问各个子类不同实现，类似多态。

# 服务端类型

TSimpleServer–简单的单线程服务模型，常用于测试

TThreadedServer–多线程服务模型，使用阻塞式IO，每个请求创建一个线程。(java不支持)

TThreadPoolServer–多线程服务模型，使用标准的阻塞式IO，预先创建一组线程处理请求。

TThreadedSelectorServer允许你用多个线程来处理网络I/O。它维护了两个线程池，一个用来处理网络I/O，另一个用来进行请求的处理

TNonblockingServer–多线程服务模型，使用非阻塞式IO（需使用TFramedTransport数据传输方式）,只有一个线程来处理消息

THsHaServer-半同步半异步的服务模型，一个单独的线程用来处理网络I/O，一个worker线程池用来进行消息的处理。

# 服务端编码基本步骤：

·实现服务处理接口impl

·创建TProcessor

·创建TServerTransport

·创建TProtocol

·创建TServer

·启动Server

# 客户端编码基本步骤：

·创建Transport

·创建TProtocol

·基于TTransport和TProtocol创建Client

·调用Client的相应方法

# 问题汇总：

1、NULL问题

我们在对服务的某个方法调用时，有时会出现该方法返回 null值的情况，在 Thrift
中，直接调用一个返回 null值的方法会抛出 TApplicationException
异常。HelloServiceImpl 里实现了 helloNull方法，返回 null
值，我们在HelloServiceClient .java中加入调用该方法的代码，出现TApplicationException
异常。

为了处理返回 null值情况，我们要捕获该异常，并进行相应的处理，具体客户端代码实现如下：

```java
packageservice.client; 
importorg.apache.thrift.TApplicationException; 
importorg.apache.thrift.TException; 
importorg.apache.thrift.protocol.TBinaryProtocol; 
importorg.apache.thrift.protocol.TProtocol; 
importorg.apache.thrift.transport.TSocket; 
importorg.apache.thrift.transport.TTransport; 
importorg.apache.thrift.transport.TTransportException; 
importservice.demo.Hello; 
 
public classHelloServiceClient { 
   /**
    *调用 Hello 服务，并处理 null 值问题
    *@param args 
    */
   publicstatic void main(String[] args) { 
       try{ 
           TTransporttransport = new TSocket("localhost", 7911); 
           transport.open();
           TProtocolprotocol = new TBinaryProtocol(transport); 
           Hello.Clientclient = new Hello.Client(protocol); 
           System.out.println(client.helloNull());
           transport.close();
       }catch (TTransportException e) { 
           e.printStackTrace();
       }catch (TException e) { 
           if(e instanceof TApplicationException 
                   &&((TApplicationException) e).getType() ==   
                                TApplicationException.MISSING_RESULT){ 
               System.out.println("Theresult of helloNull function is NULL"); 
           }
       }
   }
}
```

调用 helloNull方法后，会抛出 TApplicationException异常，并且异常种类为 MISSING_RESULT，本段代码显示，捕获该异常后，直接在控制台打印“Theresult
 of helloNull function is NULL”信息。

2、用c++的时候，service名为hello，不能有helloNull的接口，否则编译不过，原因是thrift会生成helloNull类，java不知道是否有此问题。

3、服务端在采用TBufferedTransport时，该对象实际会自己缓存read/write
 buffer，当catch到异常时，仅仅close对应的socket，buffer并不会清空。此时如果再次open
 socket发送数据，会导致buffer数据混乱，transport出现异常。解决的方法是把指向该TBufferedTransport的shared_ptr.reset()。

4、服务端和客户端采用的协议必须一样。服务端用TBufferedTransport，客户端不能用TFrameworkTransport，否则会异常。客户端报can't
 writexxx bytes的异常。

5、有时候会设置服务端和客户端的超时，这时候如果另外一方在接/发数据的时候，未能在timeout时间内完成数据传输，则会由于另外一方的强制断开而报异常。异常的内容是：brokenpipe。

6、如果thrift服务经常catch到异常，且异常的内容每次都不同，如broken
 pipe, no more data to read等，很有可能是在客户端，多个线程共用一个socket，却没有加锁导致的Heisenbug。

7、一般情况下在使用多线程时,建议使用TNonBlocking模式,可以极大的提高性能,避免出现I/O瓶颈。

8、thrfit的client不能在多个线程间不加同步的复用，因为client不是线程安全的。

9、注意生成thrift文件时,会出现

Assertion `docstring.length() <= strlen(doctext)' failed 

该问题是由于linux换行符导致的,所以请保证文件换行符的正确性

10、接口即所谓的函数（方法）不可以单独出现，需放在service服务之内；

11、包含了其他thrift文件例如aaa.thrift包含xxx.thrift以后，在aaa.thrift中需include“xxx.thrift”。要使用xxx.thrift中的service，
在aaa.thrift中的service之后需extends xxx.service(该service名称是xxx.thrift中的service名称)；要使用xxx.thrift中的数据结构，需在

  aaa.thrift用到的地方之前加上xxx.（xxx.thrift中定义的数据结构）

12、单独定义一个基本类型而不赋值是不允许的，比如 i32 num /bool flag ;即使赋值不将其作为常量也是无法通过的例如bool flag = true；

13、oneway的返回值必须是void；

14、struct不支持继承；service中的方法不支持重载；

15、thrift文件定义时请勿自引用，会引起编译器崩溃。

16、Thrift是RPC，不是具有双向通信能力的socket-tcp长连接。socket-tcp长连接，在连接建立后，双方都可以向另一方发信息，并且连接可以保持很久。但Thrift不同，虽然Thrift也基于tcp，不过在应用层，它仅仅是一个rpc，遵守rpc模型。rpc模型的运作方式与http很像：客户端向服务端发起连接并请求数据
 -> 服务端返回数据 -> 连接断开。因此，rpc模型仅仅只适合一次性的、单向的通信。需要多次的（持久的）、双向通信的场景。Tcp协议keep alive保持连接内部自动是2小时发送5次检测包，如果使用tcp连接上去后，对方在几分钟内就异常掉线，并且这段时间内没有显式的数据传输，那么tcp有可能在最坏情况下，几个小时后才发现对方掉线。因此，真正的长连接，是需要自己手工去发心跳包的。使用轮询机制，不断询问服务器有无数据。有就取下来。这个流程实际上是模拟了心跳包的机制，并且这个流程是高可控的，因此推荐这个做法。并且，使用tcp长连接的，基本上是想用这种方式来逃避nat造成的单向连接问题。

[thrift双向消息](http://www.cnblogs.com/xiaosuiba/p/4122459.html)实例参考：http://www.cnblogs.com/xiaosuiba/p/4122459.html

参考资料：

https://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/


