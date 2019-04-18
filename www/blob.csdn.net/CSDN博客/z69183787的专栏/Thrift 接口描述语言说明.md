# Thrift 接口描述语言说明 - z69183787的专栏 - CSDN博客
2016年09月21日 11:31:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2507
# **1. **本文档大部分内容翻译自文章：[“Thrift:The missing Guide“](http://diwakergupta.github.com/thrift-missing-guide/#_versioning_compatibility)。
       Thrift 官方文档方面做得比较差. 我们将不定期更新此文档,  欢迎大家多提建议, 我们随时补充.
        第一 部分主要翻译自 [http://wiki.apache.org/thrift/ThriftFeatures](http://wiki.apache.org/thrift/ThriftFeatures)
## 1.1 主要特性:
IDL  名字空间 /语言 名字空间 /
       基本类型 / 常量&枚举 / 容器 /
       结构体 / 结构体的演化(升级) /
       服务 / 服务继承 /
       异步调用 / 自定义异常 /
## 1.2  不支持的特性 
- 不支持多态 / 重载.
- 没有异构容器: 容器中元素的类型必须一样.
- 参数 & 返回值不能为 null(强烈建议不要将 null 赋予业务意义).
- 容器中元素不能为 null, 在插入容器前需要做检查， 另外建议将 struct 中 string 元素的默认值设置为 ""
- thrift 0.8 基本类型无法返回自定义异常
# 2. 语法参考
## **2.1 Types**
Thrift类型系统包括预定义基本类型，用户自定义结构体，容器类型，异常和服务定义
**(1) 基本类型**
```java
bool：布尔类型(
```
```java
true
```
```java
or
 value)，占一个字节
```
```java
```
```java
byte
```
```java
：有符号字节
```
```java
```
```java
i16:
```
```java
16
```
```java
位有符号整型
```
```java
```
```java
i32:
```
```java
32
```
```java
位有符号整型
```
```java
```
```java
i64:
```
```java
64
```
```java
位有符号整型
```
```java
```
```java
double
```
```java
：
```
```java
64
```
```java
位浮点数
```
```java
string：未知编码或者二进制的字符串
```

注意，thrift不支持无符号整型，因为很多目标语言不存在无符号整型（如java）。
**(2) 容器类型**
Thrift容器与类型密切相关，它与当前流行编程语言提供的容器类型相对应，采用java泛型风格表示的。Thrift提供了3种容器类型：
- List<t1>：一系列t1类型的元素组成的有序表，元素可以重复
- Set<t1>：一系列t1类型的元素组成的无序表，元素唯一
- Map<t1,t2>：key/value对（key的类型是t1且key唯一，value类型是t2）。
容器中的元素类型可以是除了service以外的任何合法thrift类型（包括结构体和异常）。
**(3)  结构体和异常**
Thrift结构体在概念上同C语言结构体类型：一种将相关属性聚集（封装）在一起的方式。在面向对象语言中，thrift结构体被转换成类。
异常在语法和功能上类似于结构体，只不过异常使用关键字exception而不是struct关键字声明。但它在语义上不同于结构体——当定义一个RPC服务时，开发者可能需要声明一个远程方法抛出一个异常。
结构体和异常的声明将在下一节介绍。
**(4)  服务**
服务的定义方法在语法上等同于面向对象语言中定义接口。Thrift编译器会产生实现这些接口的client和server桩。具体参见下一节。
**(5)  类型定义**
Thrift支持C/C++风格的typedef:
```java
typedef
 i32 MyInteger   \\a
```
```java
typedef
 Tweet ReTweet  \\b
```

说明：
a.  末尾没有逗号
b.   struct可以使用typedef
## **2.2   枚举类型**
可以像C/C++那样定义枚举类型，如：
```java
enum
```
```java
TweetType
 {
```
```java
```
```java
TWEET,
```
```java
//a
```
```java
```
```java
RETWEET
 =
```
```java
2
```
```java
,
```
```java
//b
```
```java
```
```java
DM
 =
```
```java
0xa
```
```java
,
```
```java
//c
```
```java
```
```java
REPLY
```
```java
```
```java
}
```
```java
//d
```
```java
```
```java
struct
 Tweet {
```
```java
```
```java
1
```
```java
:
 required i32 userId;
```
```java
```
```java
2
```
```java
:
 required string userName;
```
```java
```
```java
3
```
```java
:
 required string text;
```
```java
```
```java
4
```
```java
:
 optional Location loc;
```
```java
```
```java
5
```
```java
:
 optional TweetType tweetType = TweetType.TWEET
```
```java
//
 e
```
```java
```
```java
16
```
```java
:
 optional string language =
```
```java
"english"
```
```java
```
```java
}
```

说明：
a.  编译器默认从0开始赋值
b.  可以赋予某个常量某个整数
c.  允许常量是十六进制整数
d.  末尾没有逗号
e.  给常量赋缺省值时，使用常量的全称
注意，不同于protocol buffer，thrift不支持枚举类嵌套，枚举常量必须是32位的正整数
## **2.3   注释**
Thrfit支持shell注释风格，C/C++语言中单行或者多行注释风格
```java
#
 This is a valid comment.
```
```java
```
```java
/*
```
```java
*
 This is a multi-line comment.
```
```java
*
 Just like in C.
```
```java
*/
```
```java
```
```java
//
 C++/Java style single-line comments work just as well.
```
## **2.4   命名空间**
Thrift中的命名空间同C++中的namespace和java中的package类似，它们均提供了一种组织（隔离）代码的方式。因为每种语言均有自己的命名空间定义方式（如python中有module），thrift允许开发者针对特定语言定义namespace：
```java
namespace
 cpp com.example.project
```
```java
//
 a
```
```java
namespace
 java com.example.project
```
```java
//
 b
```

说明：
a．  转化成namespace com { namespace example { namespace project {
b．  转换成package com.example.project
## **2.5   文件包含**
Thrift允许thrift文件包含，用户需要使用thrift文件名作为前缀访问被包含的对象，如：
```java
include
```
```java
"tweet.thrift"
```
```java
//
 a
```
```java
```
```java
...
```
```java
```
```java
struct
 TweetSearchResult {
```
```java
```
```java
1
```
```java
:
 list<tweet.Tweet> tweets;
```
```java
//
 b
```
```java
```
```java
}
```
说明：
a．  thrift文件名要用双引号包含，末尾没有逗号或者分号
b．  注意tweet前缀
## **2.6   常量**
Thrift允许用户定义常量，复杂的类型和结构体可使用JSON形式表示。
```java
const
```
```java
i32
 INT_CONST =
```
```java
1234
```
```java
;
```
```java
//
 a
```
```java
const
```
```java
map<string,string>
 MAP_CONST = {
```
```java
"hello"
```
```java
:
```
```java
"world"
```
```java
,
```
```java
"goodnight"
```
```java
:
```
```java
"moon"
```
```java
}
```

说明：
a．  分号是可选的，可有可无；支持十六进制赋值。
## 2.7   定义结构体
结构体由一系列域组成，每个域有唯一整数标识符，类型，名字和可选的缺省参数组成。如：
```java
struct
 Tweet {
```
```java
```
```java
1
```
```java
:
 required i32 userId;
```
```java
//
 a
```
```java
```
```java
2
```
```java
:
 required string userName;
```
```java
//
 b
```
```java
```
```java
3
```
```java
:
 required string text;
```
```java
```
```java
4
```
```java
:
 optional Location loc;
```
```java
//
 c
```
```java
```
```java
16
```
```java
:
 optional string language =
```
```java
"english"
```
```java
//
 d
```
```java
```
```java
}
```
```java
```
```java
struct
 Location {
```
```java
//
 e
```
```java
```
```java
1
```
```java
:
 required
```
```java
double
```
```java
latitude;
```
```java
```
```java
2
```
```java
:
 required
```
```java
double
```
```java
longitude;
```
```java
```
```java
}
```

说明：
a.  每个域有一个唯一的，正整数标识符
b.  每个域可以标识为required或者optional（也可以不注明, 默认 optional）
c.  结构体可以包含其他结构体
d.  域可以有缺省值
e.  一个thrift中可定义多个结构体，并存在引用关系
规范的struct定义中的每个域均会使用required或者optional关键字进行标识。如果required标识的域没有赋值，thrift将提示错误。如果optional标识的域没有赋值，该域将不会被序列化传输。如果某个optional标识域有缺省值而用户没有重新赋值，则该域的值一直为缺省值。
与service不同，结构体不支持继承，即，一个结构体不能继承另一个结构体。
## **2.8   定义服务**
在流行的序列化/反序列化框架（如protocol buffer）中，thrift是少有的提供多语言间RPC服务的框架。
Thrift编译器会根据选择的目标语言为server产生服务接口代码，为client产生桩代码。
```java
//“Twitter”与“{”之间需要有空格！！！
```
```java
service
 Twitter {
```
```java
```
```java
//
 方法定义方式类似于C语言中的方式，它有一个返回值，一系列参数和可选的异常
```
```java
```
```java
//
 列表. 注意，参数列表和异常列表定义方式与结构体中域定义方式一致.
```
```java
```
```java
void
```
```java
ping(),
```
```java
//
 a
```
```java
```
```java
bool
 postTweet(
```
```java
1
```
```java
:Tweet
 tweet);
```
```java
//
 b
```
```java
```
```java
TweetSearchResult
 searchTweets(
```
```java
1
```
```java
:string
 query);
```
```java
//
 c
```
```java
```
```java
//
 ”oneway”标识符表示client发出请求后不必等待回复（非阻塞）直接进行下面的操作，
```
```java
```
```java
//
 ”oneway”方法的返回值必须是void
```
```java
```
```java
oneway
```
```java
void
```
```java
zip()
```
```java
//
 d
```
```java
```
```java
}
```

说明：
a． 函数定义可以使用逗号或者分号标识结束
b． 参数可以是基本类型或者结构体，参数是只读的（const），不可以作为返回值！！！
c． 返回值可以是基本类型或者结构体
d． 返回值可以是void
注意，函数中参数列表的定义方式与struct完全一样
Service支持继承，一个service可使用extends关键字继承另一个service
# **3.  产生代码**
本节介绍thrift产生各种目标语言代码的方式。本节从几个基本概念开始，逐步引导开发者了解产生的代码是怎么样组织的，进而帮助开发者更快地明白thrift的使用方法。
**概念**
Thrift的网络栈如下所示：
![](http://wiki.sankuai.com/download/attachments/102631218/thrift.jpg?version=1&modificationDate=1411547083000&api=v2)
## **3.1   Transport**
Transport层提供了一个简单的网络读写抽象层。这使得thrift底层的transport从系统其它部分（如：序列化/反序列化）解耦。以下是一些Transport接口提供的方法：
```cpp
open
```
```cpp
close
```
```cpp
read
```
```cpp
write
```
```cpp
flush
```
除了以上几个接口，Thrift使用ServerTransport接口接受或者创建原始transport对象。正如名字暗示的那样，ServerTransport用在server端，为到来的连接创建Transport对象。
```cpp
open
```
```cpp
listen
```
```cpp
accept
```
```cpp
close
```
## **3.2   Protocol**
Protocol抽象层定义了一种将内存中数据结构映射成可传输格式的机制。换句话说，Protocol定义了datatype怎样使用底层的Transport对自己进行编解码。因此，Protocol的实现要给出编码机制并负责对数据进行序列化。
Protocol接口的定义如下：
```cpp
writeMessageBegin(name,
 type, seq)
```
```cpp
writeMessageEnd()
```
```cpp
writeStructBegin(name)
```
```cpp
writeStructEnd()
```
```cpp
writeFieldBegin(name,
 type, id)
```
```cpp
writeFieldEnd()
```
```cpp
writeFieldStop()
```
```cpp
writeMapBegin(ktype,
 vtype, size)
```
```cpp
writeMapEnd()
```
```cpp
writeListBegin(etype,
 size)
```
```cpp
writeListEnd()
```
```cpp
writeSetBegin(etype,
 size)
```
```cpp
writeSetEnd()
```
```cpp
writeBool(
```
```cpp
bool
```
```cpp
)
```
```cpp
writeByte(byte)
```
```cpp
writeI16(i16)
```
```cpp
writeI32(i32)
```
```cpp
writeI64(i64)
```
```cpp
writeDouble(
```
```cpp
double
```
```cpp
)
```
```cpp
writeString(string)
```
```cpp
name,
 type, seq = readMessageBegin()
```
```cpp
readMessageEnd()
```
```cpp
name
 = readStructBegin()
```
```cpp
readStructEnd()
```
```cpp
name,
 type, id = readFieldBegin()
```
```cpp
readFieldEnd()
```
```cpp
k,
 v, size = readMapBegin()
```
```cpp
readMapEnd()
```
```cpp
etype,
 size = readListBegin()
```
```cpp
readListEnd()
```
```cpp
etype,
 size = readSetBegin()
```
```cpp
readSetEnd()
```
```cpp
bool
```
```cpp
= readBool()
```
```cpp
byte
 = readByte()
```
```cpp
i16
 = readI16()
```
```cpp
i32
 = readI32()
```
```cpp
i64
 = readI64()
```
```cpp
double
```
```cpp
= readDouble()
```
```cpp
string
 = readString()
```

下面是一些对大部分thrift支持的语言均可用的protocol：
(1)     binary：简单的二进制编码
(2)     Compact：具体见THRIFT-11
(3)     Json
## **3.3   Processor**
Processor封装了从输入数据流中读数据和向数据数据流中写数据的操作。读写数据流用Protocol对象表示。Processor的结构体非常简单:
```java
interface TProcessor {
 
bool process(TProtocol in, TProtocol out) throws TException
 
}
```
与服务相关的processor实现由编译器产生。Processor主要工作流程如下：从连接中读取数据（使用输入protocol），将处理授权给handler（由用户实现），最后将结果写到连接上（使用输出protocol）。
## **3.4   Server**
Server将以上所有特性集成在一起：
（1）  创建一个transport对象
（2）  为transport对象创建输入输出protocol
（3）  基于输入输出protocol创建processor
（4）  等待连接请求并将之交给processor处理
## **3.5   应用举例**
下面，我们讨论thrift文件产生的特定语言代码。下面给出thrift文件描述：
```
namespace cpp thrift.example
 
namespace java thrift.example
 
enum TweetType {
 
TWEET,
 
RETWEET = 2,
 
DM = 0xa,
 
REPLY
 
}
 
struct Location {
 
1: required double latitude;
 
2: required double longitude;
 
}
 
struct Tweet {
 
1: required i32 userId;
 
2: required string userName;
 
3: required string text;
 
4: optional Location loc;
 
5: optional TweetType tweetType = TweetType.TWEET;
 
16: optional string language = "english";
 
}
 
typedef list<Tweet> TweetList
 
struct TweetSearchResult {
 
1: TweetList tweets;
 
}
 
const i32 MAX_RESULTS = 100;
 
service Twitter {
 
void ping(),
 
bool postTweet(1:Tweet tweet);
 
TweetSearchResult searchTweets(1:string query);
 
oneway void zip()
 
}
```
**(1) Java语言**
**(a)  产生的文件**
一个单独的文件（Constants.java）包含所有的常量定义。
每个结构体，枚举或者服务各占一个文件
$ tree gen-java
`– thrift
`– example
|– Constants.java
|– Location.java
|– Tweet.java
|– TweetSearchResult.java
|– TweetType.java
`– Twitter.java
**(b)  类型**
thrift将各种基本类型和容器类型映射成java类型：
```cpp
bool
```
```cpp
:
 boolean
```
```cpp
byte:
 byte
```
```cpp
i16:
```
```cpp
short
```
```cpp
i32:
```
```cpp
int
```
```cpp
i64:
```
```cpp
long
```
```cpp
double
```
```cpp
:
```
```cpp
double
```
```cpp
string:
 String
```
```cpp
list<t1>:
 List<t1>
```
```cpp
set<t1>:
 Set<t1>
```
```cpp
map<t1,t2>:
 Map<t1, t2>
```
**(c)  typedef**
Java不支持typedef，它只使用原始类型，如，在上面的例子中，产生的代码中，TweetSearchResult会被还原成list<Tweet> tweets
**(d)  Enum**
Thrift直接将枚举类型映射成java的枚举类型。用户可以使用geValue方法获取枚举常量的值。此外，编译器会产生一个findByValue方法获取枚举对应的数值。
**(e)  常量**
Thrift把所有的常量放在一个叫Constants的public类中，每个常量修饰符是public static final。
**(2)  C++语言**
**(a)  产生的文件**
所有变量均存放在一个.cpp/.h文件对中
所有的类型定义（枚举或者结构体）存放到另一个.cpp/.h文件对中
每一个service有自己的.cpp/.h文件
$ tree gen-cpp
|– example_constants.cpp
|– example_constants.h
|– example_types.cpp
|– example_types.h
|– Twitter.cpp
|– Twitter.h
`– Twitter_server.skeleton.cpp
其他语言
Python，Ruby，javascript等
# **4.  注意事项**
thrift文件内容可能会随着时间变化的。如果已经存在的消息类型不再符合设计要求，比如，新的设计要在message格式中添加一个额外字段，但你仍想使用以前的thrift文件产生的处理代码。如果想要达到这个目的，只需：
（1）  不要修改已存在的field id或者已经存在的接口.
（2）  新添加的域必须是optional的，以便格式兼容。对于一些语言，如果要为optional的字段赋值，需要特殊处理，比如对于C++语言，要为
```cpp
struct Example{
 
1 : i32 id,
 
2 : string name,
 
3 : optional age,
 
}
```
中的optional字段age赋值，需要将它的__isset值设为true，这样才能序列化并传输或者存储（不然optional字段被认为不存在，不会被传输或者存储），
如：
```cpp
Example example;
 
......
 
example.age=10,
 
example.__isset.age = true; //__isset是每个thrift对象的自带的public成员，来指定optional字段是否启用并赋值。
 
......
```
（3）  非required域可以删除，前提是它的整数编号不会被其他域使用。对于删除的字段，名字前面可添加“OBSOLETE_”以防止其他字段使用它的整数编号。
（4） thrift文件应该是unix格式的（windows下的换行符与unix不同，可能会导致你的程序编译不过），如果是在window下编写的，可使用dos2unix转化为unix格式。
  (5)    thrift 既然不支持继承,  也就不支持多态; 另外, 在同一thrfit service中，方法名必须是唯一的,  所以不支持方法重载（overload).
  (6)    thrift `Struct`（对应java中一个对象）下每个Field(对就java 对象的属性)的fieldId是唯一的，且可以乱序可以不连续.
 (7)   thrift struct 的fieldName在rpc过程中是无意义的，thrift根据field Id 与field type进行反序列化.
  (8)  thrift struct 下field id有范围限制（2字节）
  (9)   长连接（多次复用同一个TTransport）时，不要使用oneway修饰符
  (10) 关于变量修饰符, 
        Required: 读写总是要求有这个字段, 总是做检查, 没有则报异常;
        Optional: 字段可有可无, 没有就不写了; 
        无修饰符: 总是写这个字段, 不做检查, 发送者不赋值, 会接收到空对象(对基本类型就是默认值);
        另外, 处理过的一些相关的服务故障,  使用 Required 字段如果没有通知到位, 会引起服务访问故障, 请谨慎使用.

