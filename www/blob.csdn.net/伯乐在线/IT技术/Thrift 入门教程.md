# Thrift 入门教程 - 文章 - 伯乐在线
原文出处： [roc](http://roclinux.cn/?p=3316)
**【引子】**
在英语里，thrift是个名词，表示的是“节俭、节约”，给个例句会印象更深些：
Farmers know a lot about value and thrift。（译：农场主深谙价值与节俭之道。）
然而，这篇文章并非是一篇英语学习教程，而是要和大家介绍计算机技术里一款著名的通信框架 – thrift框架。
好，现在有请thrift登场。
**【thrift是什么】**
thrift的全名叫做Apache thrift，是一款软件开发RPC框架，可以很高效地实现跨语言的RPC服务。
如果你还不了解RPC是什么，赶快看看[这里](http://baike.baidu.com/link?url=wst8a1mwmS3ZcRoq8zV-i-R0HVCQgMEgRzi8vP6NwU1SRyWOce2IMu6o7q81Da0ewYgz6Pd-wAWm1tCHRfJF7jy4jJSkzKok1fV-pvNDtax13OHxtSdRJmjok3IFRd0l)。
如果想参观参观thrift的官方网站，请点击[这里](http://thrift.apache.org)。
**【thrift生于何地】**
thrift最初生于Facebook，并茁壮成长，在2007年由Facebook正式开源出来，2008年由Apache软件基金会开始负责项目孵化直至今日。
**【还有哪些RPC框架】**
protobuf、Avro、MessagePack等，如果你有兴趣，可以搜索一下他们，也有利于你更好的了解RPC这个领域的发展情况。
**【下载thrift】**
目前的最新版本是thrift-0.9.1
**【安装thrift】**
首先建议你安装如下这些软件包：


```
automake libtool flex bison pkgconfig gcc-c++ boost-devel 
libevent-devel zlib-devel python-devel ruby-devel
```
然后从官网下载thrift源码包，并进行编译链接：


```
./configure –-prefix=/your/program/path/ --enable-libtool-lock
make
make install
```
安装完成后，你会看到thrift其实包含了三部分：一个bin程序、一坨头文件和若干库文件


```
#小知识 ls使用-F选项的话，文件列表中会用符号表示文件属性，
#如*表示可执行文件，@表示软连接，/表示文件夹
 
[rocrocket@li218-69 thrift]$ ls -1F bin
thrift*
 
[rocrocket@li218-69 thrift]$ ls -1F lib
libthrift-0.9.1.so*
libthrift.a
libthrift.la*
libthriftnb-0.9.1.so*
libthriftnb.a
libthriftnb.la*
libthriftnb.so@
libthrift.so@
libthriftz-0.9.1.so*
libthriftz.a
libthriftz.la*
libthriftz.so@
pkgconfig/
 
[rocrocket@li218-69 thrift]$ ls -1F include/thrift/
async/
concurrency/
config.h
cxxfunctional.h
processor/
protocol/
qt/
server/
TApplicationException.h
TDispatchProcessor.h
thrift-config.h
Thrift.h
TLogging.h
TProcessor.h
transport/
TReflectionLocal.h
```
**【为什么需要thrift】**
如果你之前没有接触过RPC框架的话，可能理解起来会比较困难。为了照顾这些新用户的感受，我尝试着用一种好理解的思路来解释：
研发工程师小吴接到了一个新任务，给“托福考试成绩数据库”增加一个“成绩查询”的功能，客户端提供“用户ID”向服务器端发起查询请求，服务器端接到查询请求后从数据库中取回此用户ID对应的姓名和成绩，并返回给客户端。
就是这样一个简单的Client-Server通信过程，其实就形成了一个典型的RPC场景。服务器端提供“成绩查询服务”，客户端会通过约定的方法来查询成绩。
小吴设计的方法调用和数据传输是这样的：
![thrift example](http://jbcdn2.b0.upaiyun.com/2016/11/0f0eaa01deaa46c5444bde308a6f30df.png)
thrift example
通过上图可以看到，服务器端处于监听状态（等待请求的到来），客户端发起一个名为Search的动作，参数是用户ID，而这个动作的返回是一个结构体struct UserGradeInfo，其中包含了用户的名字（UserName)和用户的成绩（UserGrade）。
设计做完了，小吴要开始编码了。如果按照“手工作坊”的思路，小吴至少需要完成如下几个方面：
（1）“客户端向服务器端发送数据”的代码
（2）“客户端接收服务器端查询结果”的代码
（3）“服务器端接收客户端数据”的代码
（4）“服务器端向客户端发送查询结果”的代码
（5）如果客户端会大批量发起查询，那可能还需要考虑改成多线程模型或异步模型
（6）而且还有可能因为某种原因，要求客户端和服务器端使用不同的语言进行开发
照此思路，小吴至少要3周时间来开发和自测。（时间很长，对吧）
但，自从thrift出现后（更准确的说，是自从RPC开发框架出现后），上述工作量被大大简化了。我们只要调用一个thrift工具就可以自动生成上述的所有代码，即便服务器端和客户端使用不同的语言，thrift也照样支持。
至此，我相信你应该大概理解thrift能帮我们做什么了吧。
**【thrift到底怎么用】**
依然拿上面的“成绩数据库”的例子来说，thrift的使用可以被分为四步：
第1步： 明确要交互的数据格式（如上例中的UserGradeInfo）和具体的方法（如上例中的Search），定义出thrift接口描述文件（英文叫做Inteface Description File）；
第2步： 调用thrift工具，依据thrift接口文件，生成RPC代码；
第3步： 你的服务器端程序引用thrift生成的RPC代码，并实现其中的Search动作的逻辑，然后启动监听，等待客户端发来请求。
第4步： 客户端同样引入并调用RPC代码来与服务器端通信；
*（如果你觉得这样描述太空虚，别急，稍后会有完整的例子）*
**【thrift接口描述文件怎么编写】**
如果你是学院派，那么我推荐你研究thrift IDL（Interface Definition Language）规范，在[这里](http://thrift.apache.org/docs/idl/)。虽然有些晦涩，但你可以从中了解到一个接口文件可以如何来写。
在编写接口文件时，需要对你要传输的数据设定数据类型，比如UserName是字符串型，UserGrade是整型等。因为thrift是支持众多开发语言的，所以thrift提供了一套自己的数据类型编写规范，只有用这套独立于任何语言的类型规范来编写接口文件，thrift才能把它转换成你指定的那种开发语言的代码。
thrift中的类型包括基础类型、结构、容器、异常、服务等几个部分。*（官网中有专门介绍数据类型的页面，在[这里](http://thrift.apache.org/docs/types/)）*
**【类型 之 基础类型】**
基础类型，其实非常简单和明确：
（1）bool：布尔类型（true或false）
（2）byte：8位有符号整数
（3）i16：16位有符号整数
（4）i32：32位有符号整数
（5）i64：64位有符号整数
（6）double：64位浮点数
（7）string：文本字符串，使用UTF-8编码
*（有些细心的同学会询问“为什么不支持无符号整数类型呢？”，这是因为在很多开发语言中并没有原生的无符号整型。）*
**【类型 之 容器】**
thrift容器包括了各种语言中最常用的容器，共三种：
（1）list容器：一个元素可重复的有序列表。会被转换成C++中的vector，Java中的ArrayList，脚本语言中的数组等。
（2）set容器：一个元素不可重复的无序集合。会转换成C++中的set，Java中的HashSet、Python中的Set等。*（熟悉PHP的同学可能会问“PHP并不支持set类型，怎么办”，在PHP语言中，thrift会将set容器转换成List。）*
（3）map容器：一个含有多个key:value键值对的结构。会被转换成C++中的map，Java中的HashMap，PHP中的关联数组，Python/Ruby中的dictionary等。
对于上述三种容器，其元素的类型原则上可以是任何一种thrift类型。但是值得注意的是，map的key类型需要是基础类型，因为很多开发语言并不支持map的key类型为复杂数据类型。
**【类型 之 结构体】**
结构体类型，在形式上和C/C++中的结构体类型非常相似，就是一坨类型的组合，比如上文图中的UserGradeInfo便是一个thrift结构体类型。
thrift接口文件中的结构体类型，都会被转换成一个独立的类（Class）。类的属性便是结构体中的各个类型，而类的方法便是对这些类型进行处理的相关函数。
我们来看一个结构体定义的例子：


```
struct UserGradeInfo {
 
1: required string UserName = "Anonymous";
 
2: required i16 UserGrade = 0;
 
}
```
可以看到，结构体中每一个域都有一个正整数标识符，这个标识符并不要求连续，但一旦定义，不建议再进行修改。
另外，每个域前都会有required或optional的限定，前者表示是必填域，后者则表示是可选域。域是可以有默认值的，比如上例中的“Anonymous”和0。
（1）如果一个域设置了required，但是在实际构造结构体时又没有给这个域赋值，那么thrift会认为这是一个异常。
（2）如果一个域设置为optional且在构造结构体时没有给这个域赋值，那么在使用这个结构体时，就会忽略掉这个optional的域。
**【类型 之 异常】**
除了使用exception来替代struct以外，“异常”这个类型，在语法上和刚才介绍过的结构体的用法是完全一致的。但是从语义上讲，exception和struct却大相径庭。exception是在远程调用发生异常时用来抛出异常用的。
**【类型 之 服务】**
服务的定义，与面向对象技术中定义一个接口很类似，而这些接口其实就是纯虚函数。thrift编译工具会根据服务的定义来产生相应的方法和函数。
每个服务，都包括了若干个函数，每个函数包括了若干参数和一个返回值（返回值可以是void）。
*（小技巧：返回值为void的函数，你可以在函数名前加上oneway标识符，将此函数以异步模式执行，这样在调用此函数后，函数会立即返回。）*
对于返回void的函数，thrift仍然会确保函数返回，这样就表示这个函数已被正确执行，且服务器端已有返回信息了。但是如果给void的函数前加上oneway，那么此函数的返回只能表示数据已经进入传输层，并不能表示服务器端已经收到并返回了数据。
**【我们来看一个thrift接口描述文件的例子吧】**


```
# 例子 - thrift接口描述文件
#
# 编写这个文件是为了教会你如何写thrift接口描述文件。
# 第一个你应该掌握的知识点就是.thrift文件
# 支持shell的注释方式，那就是用#符号。
 
/**
* 我们首先来复习一下thrift的常用数据类型，如下所示：
*
* bool 布尔型，1个字节
* byte 有符号整数，1个字节
* i16 有符号16位整型
* i32 有符号32位整型
* i64 有符号64位整型
* double 64位浮点数值
* string 字符串类型
* binary 二进制数据类型（字节数组）
* list 单类型有序列表，允许有重复元素
* set 单类型无需集合，不允许有重复元素
* map<t1,t2> Map型（key:value）
*
* 你发现了么，.thrift文件还支持C语言的多行注释形式。
*/
 
// 不卖关子了，其实我们还支持C语言的单行注释形式呢 ^_^
 
/**
* .thrift文件可以引用其他.thrift文件，这样就可以方便地把一些公共结构和服务囊括进来。
* 在引用其他.thrift文件时，既可以直接引用当前文件夹下的文件，也可以引用其他路径下的
* 文件，但后者需要在thrift编译工具编译时加上-I选项来设定路径。
*
* 如果希望访问被包含的.thrift文件中的内容，则需要使用.thrift文件的文件名作为前缀，
* 比如shared.SharedObject。我们在本例中引用了文件shared.thrift。
*/
include "shared.thrift"
 
/**
* Thrift支持对.thrift文件中的类型设定namespace，这样可以有效避免名字冲突。
* 这种机制在C++中也叫做namespace，而在Java中叫做Package。
* thrift支持针对不同的语言设置不同的namespace，比如下面的例子。
* thrift会在生成不同语言代码时，进行相应的设置。
*/
namespace cpp tutorial
namespace go tutorial
namespace java tutorial
namespace php tutorial
namespace perl tutorial
 
/**
* thrift还可以使用typedef来给类型起别名。
*/
typedef i32 MyInteger
 
/**
* Thrift也支持定义常量。
* 对于结构复杂的常量，支持使用JSON形式来表示。
*/
const i32 MY_NUM = 9853
const map<string,string> MY_MAP = {'hello':'world', 'goodnight':'moon'}
 
/**
* 你还可以定义枚举类型, 其被指定为32位整型。域的值是可以自定义的，而且
* 当不提供域的值时，默认会从1开始编号并递增。
*/
enum Operation {
ADD = 1,
SUBTRACT = 2,
MULTIPLY = 3,
DIVIDE = 4
}
 
/**
* 结构体则是一个复杂的数据类型。它由多个域组成，每个域会对应一个整数标识符，
* 每一行的格式为：一个冒号，一个类型，一个域名称和一个（非必填的）默认值。
*
* 每个域都可以设置为optional或required来表示是否为必填域，以便thrift决定是否
* 在数据传输时要包含这个域。不指定时，默认为required。
*/
struct Work {
1: i32 num1 = 0,
2: i32 num2,
3: Operation op,
4: optional string comment,
}
 
/**
* 在语法上，异常的定义方式和结构体是完全一样的。在发生问题时，可以抛出异常。
*/
exception InvalidOperation {
1: i32 what,
2: string why
}
 
/**
* 啊哈，我们现在到了最Cool的环节，即定义服务。
* （一个服务可以使用extends来继承另一个服务。）
*/
service Calculator extends shared.SharedService {
 
/**
* 服务中方法的定义非常类似于C语言的语法。它会包括一个返回值，
* 一个参数列表以及一个可以抛出的异常列表（可选）
* 可以提前告诉大家的是，定义参数列表的方法、定义异常列表的方法，
* 和定义结构体的方法都是相似的，可以从下面的例子中看出。
* 除了最后一个方法，其他的方法最后都要有一个逗号，大家可不要忽略这个细节。
*/
 
void ping(),
 
i32 add(1:i32 num1, 2:i32 num2),
 
/**
* 在异常列表前，需要加throws关键字。
*/
i32 calculate(1:i32 logid, 2:Work w) throws (1:InvalidOperation ouch),
 
/**
* 如下的这个方法有一个oneway修饰符，还记得他的作用么
* 这表示这个方法在调用后会立即返回，不会等待远端的回复。
* 要注意的是，oneway只能修饰void返回类型。
* oneway在英语里就是“单向”的意思，还是很形象滴。
*/
oneway void zip()
 
}
 
/**
* 在你使用thrift编译工具编译此文件后，
* 会在当前目录产生一个“gen-<你选择的开发语言>”
* 文件夹，比如你选择的是C++语言，则会产生gen-cpp文件夹，
* 里面放着的便是thrift帮你生成好的代码，
* 代码并不那么晦涩，你可以打开看一看。
*/
```
**【使用thrift编译工具】**
在我们编写好thrift接口描述文件之后，thrift编译工具就要派上用场了，它的作用就是根据thrift接口描述文件来生成相应开发语言的RPC代码，以便用户可以在自己的程序中调用。
thrift编译工具的名称就是thrift，其最常见的使用方式是这样的：


```
thrift --gen ${开发语言} ${thrift接口描述文件}
 
# 运行了上述命令之后，就会在当前文件夹下生成一个以“gen-${开发语言}”命名的文件夹，
# 里面便是自动生成的代码。
```
**【thrift会自动生成哪些代码呢】**
在编译之后，thrift会生成这些文件：（我们以mytime.thrift为例）


```
$ cd gen-cpp
 
$ ls -1 mytime_types.*
mytime_types.cpp
mytime_types.h
 
# 和数据类型有关的内容，会命名为
# ${thrift_file_name}_types.h/${thrift_file_name}_types.cpp。
# 比如你使用typedef定义了类型别名，
# 或者你定义了一个struct类型，都会在这两个文件中记录。
 
# 对于struct类型，有必要多说一下，thrift会针对每一个struct类型
# 生成一个对应的类，类中会包括一个构造函数、一个析构函数、
# 域变量定义、用于设置域值的__set_XXX()方法、
# 重载比较符（==，!=，<）、设定读写此结构体的方法read/write，
# 以及一个用于表示域是否设置了值的_${struct name}__isset结构体。
 
# 另外，还会有一个独立定义的swap方法用来进行两个结构体的值交换。
 
$ ls -1 mytime_constants.*
mytime_constants.cpp
mytime_constants.h
 
# 和常量有关的内容，会命名为
# ${thrift_file_name}_constants.h/${thrift_file_name}_constants.cpp。
# 在.h头文件中会有一个${thrift_file_name}Constants类，
# 其中会包括一个显式的构造函数，以及常量的域。
# 而在cpp文件中则会在相应的构造函数中对这个常量进行赋值。
 
$ ls -1 myservice*
myservice.cpp
myservice.h
myservice_server.skeleton.cpp
 
# 针对每一个service会产生一套对应的文件，命名为
# ${service_name}.cpp/${service_name}.h/${service_name}_server.skeleton.cpp，
# 在${service_name}.h中会看到有若干个类，他们都是以服务名作为前缀的，
# 其中包括${service_name}If、${service_name}IfFactory、
# ${service_name}IfSingletonFactory、${service_name}Null、
# ${service_name}Client、${service_name}Processor、
# ${service_name}ProcessorFactory、${service_name}Multiface。
 
# 另外，thrift还会针对服务中的每一个具体的方法分别产生四个对应的类，即
# ${service_name}_${method}_args、${service_name}_${method}_pargs、
# ${service_name}_${method}_result、${service_name}_${method}_presult。
 
# 另外，${service_name}_server.skeleton.cpp是一个server的模板例子。
```
**【一起用thrift来做个项目！】**
从我的学习经验来看，框架的学习路线是“了解应用场景 -> 了解用法 -> 看例子 -> 深入用户 -> 自己写例子”。我相信，如果你能和我一起走完这个例子，一定会消除对thrift的恐惧，爱上这款RPC框架的。
我们的例子很简单，就是一个“时间问答”机器人，英文叫做WhatTime，客户会向服务器端询问现在几点啦，服务器端会把现在的时间回答给客户端。就像这样：


```
客户端：请问，现在几点啦？
服务器端：现在是上午10点01分。
```
我们会在服务器端使用C++来实现，而在客户端会使用C++语言来实现一版，还会使用最近很流行的Go语言实现一版。*(Go语言可是未来可能撼动IT界的语言之一哦)*
thrift接口描述文件WhatTime.thrift：


```
namespace cpp roctime
 
service TimeService {
i32 TellMeTime()
}
```
需要经过thrift编译工具编译：


```
$ thrift --gen cpp WhatTime.thrift
 
$ ls -1F
gen-cpp/
WhatTime.thrift
 
$ cd gen-cpp/
$ ls -1
TimeService.cpp
TimeService.h
TimeService_server.skeleton.cpp
WhatTime_constants.cpp
WhatTime_constants.h
WhatTime_types.cpp
WhatTime_types.h
```
然后，我们把server的样例文件重命名一下：


```
$ mv TimeService_server.skeleton.cpp server.cpp
```
我们将server.cpp中的TellMeTime方法做一些修改，加入报告时间的逻辑：


```
int32_t TellMeTime() {
// Your implementation goes here
time_t now_time = time(NULL);
return now_time;
}
```
好了，server.cpp完工，我们对server.cpp进行编译链接：


```
g++ -I /home/roc/program/thrift/include -c TimeService.cpp
g++ -I /home/roc/program/thrift/include -c WhatTime_constants.cpp
g++ -I /home/roc/program/thrift/include -c WhatTime_types.cpp
g++ -I /home/roc/program/thrift/include -c server.cpp
g++ -L /home/roc/program/thrift/lib/ TimeService.o WhatTime_constants.o WhatTime_types.o server.o -o server -lthrift
```
如果提示找不到thrift动态链接库，那就需要把thrift的lib路径（如/home/roc/program/thrift/lib）加入到ld.so.conf中，然后执行ldconfig命令在重新将动态链接库装载到cache中。
然后就可以直接运行./server了，可以看到9090端口打开，已经开始服务了。
下面，我们继续编写客户端的代码。thrift并没有给出客户端的代码样例，所以需要自己来开发。


```
#include "TimeService.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
 
#include <iostream>
using namespace std;
 
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
 
using namespace roctime;
 
int main(int argc, char *argv[]) {
boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
 
time_t mytime = 0;
 
TimeServiceClient client(protocol);
transport->open();
mytime = client.TellMeTime();
 
cout << "Now is " << ctime(&mytime) << endl;
transport->close();
 
return 0;
}
```
开发完成后，我们对client进行编译链接：


```
g++ -I /home/roc/program/thrift/include -c TimeService.cpp
g++ -I /home/roc/program/thrift/include -c WhatTime_constants.cpp
g++ -I /home/roc/program/thrift/include -c WhatTime_types.cpp
g++ -I /home/roc/program/thrift/include -c client.cpp
g++ -L /home/roc/program/thrift/lib/ TimeService.o WhatTime_constants.o WhatTime_types.o client.o -o client -lthrift
```
好了，服务器端程序server和客户端程序client都生成好了，可以试着运行这个例子：
在一个终端运行服务器端程序：


```
$ ./server
```
在另一个终端打开客户端程序：


```
$ ./client
Now is Fri Nov 1 12:14:06 2013
```
顺利的话，你应该可以看到运行server的终端窗口会输出“Now is Fri Nov 1 12:14:06 2013”啦！RPC通信成功了！
至此，C++版本的客户端和服务器端都已经实现了。是不是并没有那么的难呢！下面，我们来看看Go语言的客户端如何实现。
**【Go语言版客户端】**
首先通过thrift来生成go的代码：


```
$ thrift -gen go WhatTime.thrift
```
会生成gen-go文件夹，进入其中，可以看到这里面有什么东东：


```
$ cd gen-go/
$ ls -1F
WhatTime/
```
在gen-go文件夹中，我们作如下的操作：


```
#有人会问为什么要搞个src文件夹呢，稍后你就会知道答案
$ mkdir src
 
#将thrift自动生成的WhatTime文件夹移动到src中
$ mv WhatTime src/
 
#将当初源码安装thrift时的文件夹（也就是tar.gz解包后的文件夹）中的lib/go/thrift拷贝到src中。
#拷贝过来的thrift文件夹中全都是.go文件，这些便是thrift支持go语言的库文件，用于我们稍后编译链接所用。
$ cp -r /path/to/source/thrift/lib/go/thrift/ .
 
#都完成后，我们看看gen-go/src文件夹中的目录结构
$ ls -1F
thrift/
WhatTime/
 
#设置GOPATH全局变量，以便Go语言能查找到所需的包
#当我们设置了GOPATH之后，Go语言会默认在$GOPATH/src下来查找相应的包，
#这下你应该明白为什么当初要创建这个src文件夹了吧。
#（画外音：除了上述方法，Go语言还支持使用相对路径来引用一个包。）
export GOPATH="$GOPATH:/path/to/gen-go"
 
#除此之外，你还要确保Go语言已经正确安装，且PATH和GOROOT也已正确设置。
export PATH="/path/to/go_dir/bin:${PATH}"
export GOROOT=""/path/to/go_dir"
```
下一步，我们需要对$GOPATH/src/WhatTime中的constants.go、time_service.go和ttypes.go三个文件做一下小的修改：


```
我们将import区域中的
"git.apache.org/thrift.git/lib/go/thrift"
修改为
"thrift"
作如上修改的原因，其一是我们已经在本地准备好了支持thrift的go语言包，
其二是因为这样可以避免在无法连接到互联网的情况下，程序编译失效。
```
说实话，Go语言的准备工作确实有些繁琐，希望你还有耐心看最关键的内容，那就是编写client.go的代码！
我们在src的同级目录中来编写，client.go的代码如下：


```
package main
import (
"fmt"
"time"
"thrift"
"WhatTime"
)
 
func handleClient(client *WhatTime.TimeServiceClient) (err error) {
t,_ := client.TellMeTime()
fmt.Println(time.Unix(int64(t), 0).String())
return nil
}
 
func runClient(transportFactory thrift.TTransportFactory, protocolFactory thrift.TProtocolFactory, addr string) error {
var transport thrift.TTransport
transport, err := thrift.NewTSocket(addr)
if err != nil {
fmt.Println("Error opening socket:", err)
return err
}
transport = transportFactory.GetTransport(transport)
defer transport.Close()
if err := transport.Open(); err != nil {
return err
}
 
return handleClient(WhatTime.NewTimeServiceClientFactory(transport, protocolFactory))
}
 
func main() {
var protocolFactory thrift.TProtocolFactory
protocolFactory = thrift.NewTBinaryProtocolFactoryDefault()
 
var transportFactory thrift.TTransportFactory
transportFactory = thrift.NewTBufferedTransportFactory(1024)
 
addr := "localhost:9090"
 
if err := runClient(transportFactory, protocolFactory, addr); err != nil {
fmt.Println("error running client:", err)
}
}
```
然后进行编译链接和运行：


```
$ go build client.go
$ ./client
2013-11-01 12:37:31 +0800 CST
```
至此，我们的Go语言版本也大功告成了！
**【结语】**
如果你耐心地看到了这里，说明你完成了thrift的入门。但是“纸上得来终觉浅，自己动手才是真”。
后面还会有Thrift进阶篇，敬请期待。
