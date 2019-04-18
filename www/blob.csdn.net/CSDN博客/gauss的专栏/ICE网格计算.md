# ICE网格计算 - gauss的专栏 - CSDN博客
2014年07月14日 00:43:47[gauss](https://me.csdn.net/mathlmx)阅读数：385
IceGrid用于支持分布式网络服务应用，一个IceGrid域由一个注册表（Registry）和任何数目的节点(Node)构成。注册表（Registry）和节点(Node)一起合作管理一些信息以及包含一些应用（Application）的服务进程。每项应用（Application）被指定在特定节点上的服务。这个注册表（Registry）持久记录了这些信息，而节点（Node）负责启动和监测其指定的服务器进程。对于一个典型的配置，一个节点（Node）运行在一台计算机(称之为Ice服务器主机)。注册表（Registry）并不消耗很多处理器时间，所以它常常是和一个节点（Node）运行在同一台计算机上的，注册表（Registry）还可以和一个节点（Node）可以运行在同一进程中.如果需要容错，注册表（Registry）还可以用主从式的设计支持复制（Replication）。
注册表（Registry）的主要责任，是解决作为Ice定位服务的间接代理问题，当客户端第一次尝试使用一种间接代理，客户端Ice run time首先连接注册表（registry），注册表将间接代理的符号信息转化为直接代理的endpoint，然后客户端和直接代理建立一个连接。通过适配器复制，同名适配器可以分布在多个节点上，间接代理可以映射到多个节点上的直接代理，在运行时由注册表服务根据负载均衡自动选择一个直接代理给客户端。
使用间接代理时，客户端可以用以下方式直接获取服务对象代理：
MyProxy=theObject@theAdapter  // 对象@适配器
更简单一点的话可以用以下方式
MyProxy=theObject  // 对象
### 2.6.1 分布式部署
在部署IceGrid分布式服务时，需要启动注册表服务（icegridregistry），并配置注册表服务地址端口、通信协议和注册信息保存的目录地址（ICE的注册信息保存为BerkeleyDB的数据库文件）：
IceGrid.Registry.Client.Endpoints=tcp-p 4061
IceGrid.Registry.Data=/opt/ripper/registry
在服务器节点中和客户端都需要配置注册表服务的地址端口和通信协议：
Ice.Default.Locator=IceGrid/Locator:tcp -h 172.0.0.1 -p4061
然后分别启动注册表服务(icegridregistry)和节点服务(icegridnode).
ICE提供了部署工具icegridadmin,这个icegridadmin工具也需要定义Ice.Default.Locator属性.
接下需要编写应用部署文件，应用部署文件以XML方式保存。以下为支持适配器复制的应用配置文件，使用了服务模板：
<icegrid>
<applicationname="Ripper">
<replica-groupid="EncoderAdapters"> //定义适配器复制组
<objectidentity="EncoderFactory"   //identity将在客户端中使用。
type="::Ripper::MP3EncoderFactory"/>
</replica-group>
<server-templateid="EncoderServerTemplate"> //定义服务器模板
<parametername="index"/>
<parametername="exepath"
default="/opt/ripper/bin/server"/>
<serverid="EncoderServer${index}"
exe="${exepath}"
activation="on-demand">
<adaptername="EncoderAdapter"
replica-group="EncoderAdapters"
endpoints="tcp"/>
</server>
</server-template>
<nodename="Node1">
<server-instancetemplate="EncoderServerTemplate"
index="1"/>
</node>
<nodename="Node2">
<server-instancetemplate="EncoderServerTemplate"
index="2"/>
</node>
</application>
</icegrid>
然后在客户端可以用以下方式获取对象代理：
Ice::ObjectPrx obj =communicator->stringToProxy("EncoderFactory");
### 2.6.2 负载均衡
ICE平台内嵌负载均衡功能，对于分布大多个节点上的应用服务提供多种负载均衡方案，只需要通过XML配置文件即可完成负载均衡配置。配置项包括Type (负载均衡类型)、Sampling interval（负载信息收集间隙）、Number of replicas（返回给客户端的适配器个数）。
负载均衡类型有以下4种方式：
² Random (随机方式):注册中心随机选择一个适配器给客户端，不检查适配器的负载。
² Adaptive(适配方式)：注册中心从所有适配器中选择一个负载最轻的适配器给客户端，Sampling interval参数只有在该类型的负载均衡中有效，这个参数指定节点定期向注册中心报告本地系统负载信息（system load information）;
² Round Robin(最近最少使用):注册中心从对应的适配器组中选择一个最近最少使用的适配器给客户。
² Ordered（顺序方式）：注册中心根据适配器的优先级，从高到低顺序选择一个适配器给客户端。
配置示例：
        <replica-groupid="EncoderAdapters">
<load-balancingtype="adaptive"/>  //配置为适配方式
<object identity="EncoderFactory"
type="::Ripper::MP3EncoderFactory"/>
</replica-group>
### 2.6.3 注册中心集群
前两节中描述的是属于用户应用的分布部署，分布式部署一个很重要的支撑是ICE的注册中心，所有客户端都向注册中心查询服务代理的真实端点，从而建立通信连接，在这里注册中心又成了一个单点服务，为了避免注册中心成为应用的瓶颈，提高系统的可靠性，ICE3.3.0以上版本提供了注册中心集群功能。
ICE注册中心集群通过主从式的注册中心复制来实现，一个集群中有一个主注册中心，若干个副注册中心，主从的区别通过IceGrid.Registry.ReplicaName属性配置来实现，主注册中心的名称为Master，其他的名字可以任意取。启动时先启动主注册中心，再启动其他注册中心，通过主注册中心更新的信息都将同步给副注册中心，各副注册中心之间不通信。如果主注册中心失效，需要从其他副注册中心提拨一个成为主注册中心，但是从3.3版本的说明文档中来看，如果需要把某个副注册中心提拨成为主注册中心需要重新启动相应进程并修改IceGrid.Registry.ReplicaName属性值为Master，或者删除该属性，默认情况下该属性值为Master.
使用集群方式时，在客户端配置时把所有的主从注册中心地址端口全部填到Ice.Default.Locator,例如：
Ice.Default.Locator=IceGrid/Locator:default -p12000:default -p 12001
在应用节点也把所有的注册中心地址端口绑定，这样应用的更新会同时通知所有的注册中心。
## 2.7    IceSSL应用
ICE平台可以通过简单的配置来支持SSL应用，配置过程如下：
² 首先需要通过修改配置文件来启用SSL插件，C++服务端的配置方法为：Ice.Plugin.IceSSL=IceSSL:createIceSSL
只需要把IceSSL动态库放到LD_LIBRARY_PATH包含的路径下即可。
² 然后修改适配器的监听选项：
MyAdapter.Endpoints=tcp -p 8000:ssl-p 8001:udp -p 8000 //表示该适配器在三种协议端口上同时监听。
ICE还提供了多种配置属性来满足实际应用，例如下例所示：
Ice.Plugin.IceSSL=IceSSL:createIceSSL
IceSSL.DefaultDir=/opt/certs  //默认证书目录
IceSSL.CertFile=pubkey.pem //证书文件
IceSSL.KeyFile=privkey.pem //私钥文件
IceSSL.CertAuthFile=ca.pem //信任的根证书文件
IceSSL.Password=password   //私钥文件查看密码
## 2.8    持久化存储（IceFreeze）
ICE提供的持久化方案可以支持普通用户数据（键/值对）的持久化存储和服务对象实例的持久化管理，普通用户数据的持久化存储使用比较简单，服务对象实例的管理相对复杂一点，暂时不关注。
ICE的持久存储介质为BerkeleyDB,对普通数据的持久化在C++实现中采用Map的方式进行操作，用户需要用Slice定义需要存储的数据，并用slice2freeze 生成相应的Map操作类，然后对数据的操作就可以使用Map容器函数来进行。示例如下：
首先生成需要存储的数据类型：
slice2freeze --dictStringIntMap,string,int StringIntMap
代码使用：
Ice::CommunicatorPtr communicator=
Ice::initialize(argc,argv);
// Create a Freeze databaseconnection.
Freeze::ConnectionPtr connection =Freeze::createConnection(communicator, "db"); //连接到数据库文件。
// Instantiate the map.
StringIntMap map(connection,"simple");//创建表。
// Clear the map.
map.clear();
Ice::Int i;
StringIntMap::iterator p;
// Populate the map.
for (i = 0; i < 26;i++)
{
std::string key(1, 'a' +i);
map.insert(make_pair(key,i));
}
// Iterate over the map and changethe values.
for (p = map.begin(); p != map.end();++p)
p.set(p->second +1);
// Find and erase the lastelement.
p = map.find("z");
assert(p != map.end());
map.erase(p);
// Clean up.
connection->close();
communicator->destroy();
IceFreeze还允许使用结构体和类对象作为值进行存储，但是只有public的成员变量会被存储，其他成员变量不会被存储。
对于较高版本的ICE，还允许对值建立索引，如果值为结构体或者类对象，那么还允许以结构体/对象变量作为索引，通过slice2freeze编译后会生成对应的索引查询函数。例如定义了如下需要存储的数据结构：
module Demo
{
struct Struct1
{
   long l;
};
class Class1
{
   string s;
};
};
然后执行以下命令生成映射表，同时生成索引，以class1的成员变量s为索引。
Slice2freeze
--dictDemo::IndexedStruct1Class1Map,Demo::Struct1,Demo::Class1        --dict-indexDemo::IndexedStruct1Class1Map,s,case-sensitive         BenchTypesTest.ice
编译后代码中会自动生成findByS(string&),在程序中可以按以下方式直接调用：
IndexedStruct1Struct2Map& m=...;
IndexedStruct1Struct2Map::iterator p= m.findByS(os.str());
# 3      ICE平台功能研究小结
ICE平台提供的功能比较多，除了文档中罗列的部分，还支持程序包分发(IcePath2)、防火墙穿透（Glacier），鉴于目前的项目应用暂时不对这两部分作介绍。
从ICE官方提供的Demo和自己编写的测试程序在Iinux(opensuse)运行良好，对于适用于AIX的3.1.1版本在AIX上运行异步编程的测试也很顺利，但是目前还未对ICE平台的应用做性能测试。
从文档的介绍，ICE平台支持同步/异步、订阅/发布、分布式部署、内部持久化存储，支持接口描述语言到各种面向对象开发语言的映射，可以满足ESB系统开发的技术需求，但是也有一定的风险，ICE3.1以后的版本对AIX、HP-UX操作系统不保证完全支持，对3.1以后各个升级版本需要进行编译并运行测试
            
