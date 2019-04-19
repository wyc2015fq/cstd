# Google 开源技术protobuf - fanyun的博客 - CSDN博客
2018年06月23日 22:52:10[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：80

1.  Protobuf简介
       protobuf是google提供的一个开源序列化框架，类似于XML，JSON这样的数据表示语言，其最大的特点是基于二进制，因此比传统的XML表示高效短小得多。虽然是二进制数据格式，但并没有因此变得复杂，开发人员通过按照一定的语法定义结构化的消息格式，然后送给命令行工具，工具将自动生成相关的类，可以支持php、java、c++、python等语言环境。通过将这些类包含在项目中，可以很轻松的调用相关方法来完成业务消息的序列化与反序列化工作。
     protobuf在google中是一个比较核心的基础库，作为分布式运算涉及到大量的不同业务消息的传递，如何高效简洁的表示、操作这些业务消息在google这样的大规模应用中是至关重要的。而protobuf这样的库正好是在效率、数据大小、易用性之间取得了很好的平衡。
官方文档
[http://code.google.com/p/protobuf/](http://code.google.com/p/protobuf/)
2.  Protobuf如何工作
        你首先需要在一个 .proto 文件中定义你需要做串行化的数据结构信息。每个ProtocolBuffer信息是一小段逻辑记录，包含一系列的键值对。这里有个非常简单的 .proto 文件定义了个人信息:
message Person {
    required string name=1;
    required int32 id=2;
    optional string email=3;
    enum PhoneType {
        MOBILE=0;
        HOME=1;
        WORK=2;
    }
    message PhoneNumber {
        required string number=1;
        optional PhoneType type=2 [default=HOME];
    }
    repeated PhoneNumber phone=4;
}
有如你所见，消息格式很简单，每个消息类型拥有一个或多个特定的数字字段，每个字段拥有一个名字和一个值类型。值类型可以是数字(整数或浮点)、布尔型、字符串、原始字节或者其他ProtocolBuffer类型，还允许数据结构的分级。你可以指定可选字段，必选字段和重复字段。你可以在( http://code.google.com/apis/protocolbuffers/docs/proto.html )找到更多关于如何编写 .proto 文件的信息。
一旦你定义了自己的报文格式(message)，你就可以运行ProtocolBuffer编译器，将你的 .proto 文件编译成特定语言的类。这些类提供了简单的方法访问每个字段(像是 query() 和 set_query() )，像是访问类的方法一样将结构串行化或反串行化。例如你可以选择C++语言，运行编译如上的协议文件生成类叫做 Person 。随后你就可以在应用中使用这个类来串行化的读取报文信息。你可以这么写代码:
Person person;
person.set_name("John Doe");
person.set_id(1234);
person.set_email("jdoe@example.com");
fstream.output("myfile",ios::out | ios::binary);
person.SerializeToOstream(&output);
然后，你可以读取报文中的数据:
fstream input("myfile",ios::in | ios:binary);
Person person;
person.ParseFromIstream(&input);
cout << "Name: " << person.name() << endl;
cout << "E-mail: " << person.email() << endl;
你可以在不影响向后兼容的情况下随意给数据结构增加字段，旧有的数据会忽略新的字段。所以如果使用ProtocolBuffer作为通信协议，你可以无须担心破坏现有代码的情况下扩展协议。
你可以在API参考( http://code.google.com/apis/protocolbuffers/docs/reference/overview.html )中找到完整的参考，而关于ProtocolBuffer的报文格式编码则可以在( http://code.google.com/apis/protocolbuffers/docs/encoding.html )中找到。
3.  Protobuf消息定义
      要通信，必须有协议，否则双方无法理解对方的码流。在protobuf中，协议是由一系列的消息组成的。因此最重要的就是定义通信时使用到的消息格式。
消息由至少一个字段组合而成，类似于C语言中的结构。每个字段都有一定的格式。
字段格式：限定修饰符① | 数据类型② | 字段名称③ | = | 字段编码值④ | [字段默认值⑤]
①．限定修饰符包含 required\optional\repeated
Required: 表示是一个必须字段，必须相对于发送方，在发送消息之前必须设置该字段的值，对于接收方，必须能够识别该字段的意思。发送之前没有设置required字段或者无法识别required字段都会引发编解码异常，导致消息被丢弃。
Optional：表示是一个可选字段，可选对于发送方，在发送消息时，可以有选择性的设置或者不设置该字段的值。对于接收方，如果能够识别可选字段就进行相应的处理，如果无法识别，则忽略该字段，消息中的其它字段正常处理。---因为optional字段的特性，很多接口在升级版本中都把后来添加的字段都统一的设置为optional字段，这样老的版本无需升级程序也可以正常的与新的软件进行通信，只不过新的字段无法识别而已，因为并不是每个节点都需要新的功能，因此可以做到按需升级和平滑过渡。
Repeated：表示该字段可以包含0~N个元素。其特性和optional一样，但是每一次可以包含多个值。可以看作是在传递一个数组的值。
②．数据类型
Protobuf定义了一套基本数据类型。几乎都可以映射到C++\Java等语言的基础数据类型.
|protobuf 数据类型|描述|打包|C++语言映射|
|----|----|----|----|
|bool|布尔类型|1字节|bool|
|double|64位浮点数|N|double|
|float|32为浮点数|N|float|
|int32|32位整数、|N|int|
|uin32|无符号32位整数|N|unsigned int|
|int64|64位整数|N|__int64|
|uint64|64为无符号整|N|unsigned __int64|
|sint32|32位整数，处理负数效率更高|N|int32|
|sing64|64位整数 处理负数效率更高|N|__int64|
|fixed32|32位无符号整数|4|unsigned int32|
|fixed64|64位无符号整数|8|unsigned __int64|
|sfixed32|32位整数、能以更高的效率处理负数|4|unsigned int32|
|sfixed64|64为整数|8|unsigned __int64|
|string|只能处理 ASCII字符|N|std::string|
|bytes|用于处理多字节的语言字符、如中文|N|std::string|
|enum|可以包含一个用户自定义的枚举类型uint32|N(uint32)|enum|
|message|可以包含一个用户自定义的消息类型|N|object of class|
N 表示打包的字节并不是固定。而是根据数据的大小或者长度。
例如int32，如果数值比较小，在0~127时，使用一个字节打包。
关于枚举的打包方式和uint32相同。
关于message，类似于C语言中的结构包含另外一个结构作为数据成员一样。
关于 fixed32 和int32的区别。fixed32的打包效率比int32的效率高，但是使用的空间一般比int32多。因此一个属于时间效率高，一个属于空间效率高。根据项目的实际情况，一般选择fixed32，如果遇到对传输数据量要求比较苛刻的环境，可以选择int32.
③．字段名称
字段名称的命名与C、C++、Java等语言的变量命名方式几乎是相同的。
protobuf建议字段的命名采用以下划线分割的驼峰式。例如 first_name 而不是firstName.
④．字段编码值
有了该值，通信双方才能互相识别对方的字段。当然相同的编码值，其限定修饰符和数据类型必须相同。
编码值的取值范围为 1~2^32（4294967296）。
其中 1~15的编码时间和空间效率都是最高的，编码值越大，其编码的时间和空间效率就越低（相对于1-15），当然一般情况下相邻的2个值编码效率的是相同的，除非2个值恰好实在4字节，12字节，20字节等的临界区。比如15和16.
1900~2000编码值为Google protobuf 系统内部保留值，建议不要在自己的项目中使用。
protobuf 还建议把经常要传递的值把其字段编码设置为1-15之间的值。
消息中的字段的编码值无需连续，只要是合法的，并且不能在同一个消息中有字段包含相同的编码值。
建议：项目投入运营以后涉及到版本升级时的新增消息字段全部使用optional或者repeated，尽量不实用required。如果使用了required，需要全网统一升级，如果使用optional或者repeated可以平滑升级。
⑤．默认值。当在传递数据时，对于required数据类型，如果用户没有设置值，则使用默认值传递到对端。当接受数据是，对于optional字段，如果没有接收到optional字段，则设置为默认值。
关于import
protobuf 接口文件可以像C语言的h文件一个，分离为多个，在需要的时候通过 import导入需要对文件。其行为和C语言的#include或者java的import的行为大致相同。
关于package
避免名称冲突，可以给每个文件指定一个package名称，对于java解析为java中的包。对于C++则解析为名称空间。
关于message
支持嵌套消息，消息可以包含另一个消息作为其字段。也可以在消息内定义一个新的消息。
关于enum
枚举的定义和C++相同，但是有一些限制。
枚举值必须大于等于0的整数。
使用分号(;)分隔枚举变量而不是C++语言中的逗号(,)
eg.
enum VoipProtocol 
{
    H323 = 1;
    SIP  = 2;
    MGCP = 3;
    H248 = 4;
}
4.  Protobuf的PHP实例
以下，为了深刻理解[protobuf](http://code.google.com/p/protobuf/)。我们使用php示例：
php protobuf 下载地址http://code.google.com/p/pb4php/downloads/list
C# protobuf 下载地址http://code.google.com/p/protobuf/downloads/list
protobuf语言使用 http://www.cnblogs.com/dkblog/archive/2012/03/27/2419010.html
php使用protobuf，然后再测试通讯。
下载的example的pb_proto_test_new.php是由问题的。
### 1、 下载：php protobuf 
下载地址http://code.google.com/p/pb4php/downloads/list
http://pb4php.googlecode.com/files/protocolbuf_025.zip
将下载好的proto扩展库，解压到wwwroot目录下
### 2、先写一个proto文件
我们使用库里面提供的proto文件：test_new.proto。这个文件是在example。我们把它移到新建的文件mytest目录下。
message Person
{
  required string name = 1;
  required int32 id = 2;
  optional string email = 3;
  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }
  message PhoneNumber {
    required string number = 1;
    optional PhoneType type = 2 [default = HOME];
  }
  // a simple comment
  repeated PhoneNumber phone = 4;
  optional string surname = 5;
}
message AddressBook {
  repeated Person person = 1;
}
message Test {
  repeated string person = 2;
}
### 3、生成pb_proto_test_new.php文件
其实该文件已经存在example目录下啦。
但有原始生成的这个文件有问题。根本没有这个常量：var $wired_type = PBMessage::WIRED_STRING;
php不支持proto里的package，所以php版编译之前先要删掉package语句。然后在mytest目录建立一个create_test_new.php文件存放编译命令：
<?php
require_once('../parser/pb_parser.php');
$parser = new PBParser();
$parser->parse('./test_new.proto');
echo 'ok;
结果在mytest目录下生成一个文件:pb_proto_test_new.php
到此，假如这个数据协议是在客户端。那么我们客户端也使用php代码：我们直接使用代码库example里面的示例：
### 4、运行实例：
即运行test_new.php：
<?php
// first include pb_message
require_once('../message/pb_message.php');
// include the generated file
require_once('./pb_proto_test_new.php');
// generate message with the new definition with surname
// now just test the classes
$book = new AddressBook();
$person = $book->add_person();
$person->set_name('Nikolai');
$person = $book->add_person();
$person->set_name('Kordulla');
$person->set_surname('MySurname');
$phone_number = $person->add_phone();
$phone_number->set_number('0711');
$phone_number->set_type(Person_PhoneType::WORK);
$phone_number = $person->add_phone();
$phone_number->set_number('0171');
$phone_number->set_type(Person_PhoneType::MOBILE);
$phone_number = $person->add_phone();
$phone_number->set_number('030');
// serialize
$string = $book->SerializeToString();
// write it to disk
file_put_contents('test.pb', $string);
?>
test.pb是生成的二进制文件 基本结构一个字节类型+ 字节长度
从以上操作和类库源代码来看打包速度可能慢很多。 空间节省倒是非常好。符合 protobuf 定义：效率、数据大小、易用性之间的平衡。
### 5、服务器读取协议内容.
假设test.pb文件是经过网络传输到服务器上的（这里都是在本地）。
然后服务器端也可以根据这个协议，生成对应类。例如example下面的test.proto:
message Person
{
  required string name = 1;
  required int32 id = 2;
  optional string email = 3;
  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }
  message PhoneNumber {
    required string number = 1;
    optional PhoneType type = 2 [default = HOME];
  }
  // a simple comment
  repeated PhoneNumber phone = 4;
}
message AddressBook {
  repeated Person person = 1;
}
运行test.php:
<?php
// EXECUTE test_new.php first 
// first include pb_message
require_once('../message/pb_message.php');
// now read it with the old file
// include the generated file
require_once('./pb_proto_test.php');
$string = file_get_contents('./test.pb');
// Just read it
$book = new AddressBook();
$book->parseFromString($string);
var_dump($book->person_size());
$person = $book->person(0);
var_dump($person->name());
$person = $book->person(1);
var_dump($person->name());
var_dump($person->phone(0)->number());
var_dump($person->phone(0)->type());
var_dump($person->phone(1)->number());
var_dump($person->phone(1)->type());
var_dump($person->phone(2)->number());
var_dump($person->phone(2)->type());
?>
读取出客户端相应的内容。
5.  Protobuf与Thrift
数据类型
|protobuf|thrift|protobuf|thrift|protobuf|thrift|protobuf|thrift|
|----|----|----|----|----|----|----|----|
|double|double|float|||byte||i16|
|int32|i32|int64|i64|uint32||uint64||
|sint32||sint64||fixed32||fixed64||
|sfixed32||sfixed64||bool|bool|string|string|
|bytes|binary|message|struct|enum|enum|service|service|
综合对比||protobuf|thrift|
|----|----|----|
|功能特性|主要是一种序列化机制|提供了全套RPC解决方案，包括序列化机制、传输层、并发处理框架等|
|支持语言|C++/Java/Python|C++, Java, Python, Ruby, Perl, PHP, C#, Erlang, Haskell|
|易用性|语法类似，使用方式等类似| |
|生成代码的质量|可读性都还过得去，执行效率另测| |
|升级时版本兼容性|均支持向后兼容和向前兼容| |
|学习成本|功能单一，容易学习|功能丰富、学习成本高|
|文档&社区|官方文档较为丰富，google搜索protocol buffer有2000W+结果，google group被墙不能访问|官方文档较少，没有API文档，google搜索apache thrift仅40W结果，邮件列表不怎么活跃|
性能对比
由于thrift功能较protobuf丰富，因此单从序列化机制上进行性能比较，按照序列化后字节数、序列化时间、反序列化时间三个指标进行，对thrift的二进制、压缩、protobuf三种格式进行对比。
测试方法：取了15000+条样本数据，分别写了三个指标的测试程序，在我自己的电脑上执行，其中时间测试循环1000次，总的序列化/反序列化次数1500W+。
平均字节数：
|thrift二进制|535|
|----|----|
|thrift压缩|473|
|protobuf|477|
序列化（1500W次）时间（ms）：|thrift二进制|306034|
|----|----|
|thrift压缩|304256|
|protobuf|177652|
反序列化（1500W次）时间（ms）：|thrift二进制|287972|
|----|----|
|thrift压缩|315991|
|protobuf|157192|
thrift的时间测试可能不是很准，由于thrift产生代码的复杂性，编写的测试代码为了适应其接口，在调用堆栈上可能有一些额外开销。
