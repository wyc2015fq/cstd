# SNMP的应用 - DoubleLi - 博客园






前两天项目要求一个附加功能，远程监视服务器的运行状况，要定期监视指定端口，指定业务，还包括服务器的磁盘空间，内存，CPU使用率等等。这头俩事还好说，ping和telnet也就搞定了，实在不行就开个socket连一下，关键后边的事有点抓瞎，要是在本地，可以通过API得到系统信息，分析磁盘空间，内存啥的，可是远程得到别的机子的信息，那不成木马了么。到网上一查，原来有SNMP可以帮着做这件事情。于是就了解了一下，发现这个东西挺有意思。简单地做个笔记。

SNMP简单网络管理协议，是一个用来进行网络管理的协议，之所以称其简单，是因为它是另一个网管协议的简化版。并不是说简化版就不顶用。相反，那个不简化的网管协议号称因为太复杂，几乎不可能实现并推广。不过说实在的，SNMP我都没觉得它多简单，它至今经过十多年的演进，广泛用于各种网络设备及工业控制中，我个人感觉，随着网络进一步发展，电器都会有IP地址的时候，SNMP或它的替代品的作用，应该是会更加重要的。



SNMP简单说，包括了被管的设备，代理和网管系统三个部分。被管的设备指的就是我们要获取信息的设备，这个任务里我需要获取它的磁盘空间。但是被管理设备只是产生这些数据，真正将数据报告回来的工作是由代理完成的。代理说白了就是一个运行在被管设备上的程序。网络编程的角度讲它就是一个小的socket服务程序，监听着好像是161端口。通过在被管设备上，需要启动snmp服务。比如在linux上，就需要执行service snmpd start来启动snmp服务，也就是把代理跑起来。实际过程中，代理包含在各种网络设备上，包括交换机，路由器甚至打印机，手机，调制解调器上。网管系统就是我们要从被管设备获取数据并进行分析处理的这个程序。可以想像它运行在本地。只要我将一个合格的请求发往正在运行的代理处，代理从请求中，理解请求的内容，并从被管设备上用它自己的方式搜集到所需的信息，并发送回来，这样一个网管的简单过程就完成了。

这样的管理方式，还是很灵活的，只要请求包符合协议标准，用哪种语言开发网管系统无所谓；从代理角度讲，只要能正常监听并处理请求，代理可以任意实现。我不太清楚国际上有没有一个统一的标准，比如让我写代理，有请求要查询网络端口的数量，我非把显示器尺寸返回去，不知道警察抓不抓我，至少客户会要求退货吧。也正是因为这种灵活，SNMP也可以支持大规模的集中管理，我同样一个查询存储空间的请求，可以发给我的PC，也可以发给我的诺基亚手机，可能得到的会是存储卡的容量吧，就看代理怎么实现了。



至于如何表达我的请求，这里有一个统一的标准，就是MIB，管理信息库的缩写，就好像一个注册表一样，但是这个注册表不实际存值，不像Windows的注册表。它只是给每一种设备信息编了一个号，称为OID。这个号是以树的形式组织的，中间用点隔开，便于管理。约定好了查的时候直接报告这个号就知道查什么值了。这个号是大家统一的，好比如每个人都有眼睛，眼睛的好坏我们可以用视力来衡量，我们就可以这样定：“人（1）.身体（2）.五官（3）.眼睛（4）.视力（5）”。那如果我向一个人发请求，问他的视力，那可以冲他说：那谁谁谁，我是那谁家的谁谁谁，告诉我.1.2.3.4.5.0 这里的"1.2.3.4.5"是对象标识符，后面再加上.0就是实例标识符。



snmp为了简单，规定的操作类型较少，基本常用的有获取（Get）,设置（Set），获取下一个（GetNext），还有一个由代理主动发出的（Trap）用于报告事件。另外snmp已经发展到V3了，这个过程中又新增了什么GetBulk等等。我还没有用到，不敢乱说。回头试试再作笔记。



在linux下，配置SNMP服务要修改/etc/snmp/snmpd.conf，也可以通过snmpconf命令配，挺方便的。

在windows下，直接在控制面板，添加删除程序里，添加Windows组件，然后选网络管理，就装上snmp服务了。在服务列表里找到它，可以通过属性进行配置。



基本上概念也就这些了。实际在开发的时候，有几种选择：

1.直接发UDP包，这样最原始，最灵活，基本上完全自主。不过太麻烦。没有非要重新造轮子玩。

2.在Windows上用winsnmp这个API。拼包的工作它就管了。

3.选择一些开源的SNMP开发包。比如像NET－SNMP,SNMP++，还有其它的很多。.net下还有一个现成的组件OLEPRNLib，我不知道它应该算那种。



我这次主要用了winsnmp和SNMP++。

首先SnmpStartup一下。成了，就会通过5个参数传回5个值。关于本地SNMPAPI的基本信息，版本号和SNMP版本。头两个版本用团体名作认证。方便，但安全性较差。V3就比较安全了。

if (SNMPAPI_SUCCESS == SnmpStartup((smiLPUINT32)&m_nMajorVersion, (smiLPUINT32)&m_nMinorVersion, (smiLPUINT32)&m_nLevel, (smiLPUINT32)&m_nTranslateMode, (smiLPUINT32)&m_nRetransmitMode))
在我的机子上返回的分别是：2,0,2,1,1

头两个数说明我的WinSnmp版本是2.0.

第3个2,说明WinSnmp支持标准的SNMPv2

第四个1,说明我的SNMP采用SNMPAPI_UNTRANSLATED_V1这种转换模式。

最后一个1说明支持重传。

任务比较紧，这几个参数的具体意思，我回头研究研究再作笔记。

m_hSession = SnmpCreateSession(NULL, 0, CSNMPManager::snmp_callbacknext, this);

建立一个会话。用这个会话，就可以发送请求了。

第一个参数是窗口句柄，用来接收消息。是这样，具我了解，SNMP首先可以异步传输。也就是说发送出请求后，并不阻塞。那么如果有回应消息了，就会给这个窗口句柄一个消息，如果在MFC下，可以做消息映射。第二个参数就是将来收到响应时，窗口会收到的对应的消息值。然后具体的响应是什么，到消息的参数里去看。

这里我传的是空，因为我测试的程序没有窗口。

第三个参数是一个函数指针。不发消息给窗口，就可以申请回调。我现在用的就是回调。第四个参数是调回调的时候，传给我的自定义参数，可以传空。我现在传this是因为我注册的snmp_callbacknext是个静态函数。我把实例指针传进去，是为了把实例信息带进去。

接下来，就可以通过SnmpStrToEntity，得到SNMP实体了。这里把客户端和服务端统称为实体。m_hsrcAgent   =   SnmpStrToEntity(m_hSession,   m_pSrcAddr))得到实体句柄

再接下来，m_hContext = SnmpStrToContext(m_hSession, &community);得到上下文句柄。这里的团体名，有人也叫共同体。总之是个帐户一样的东西。在配置SNMP的时候，要设置这个，并给它设置只读还是读写。一般安全起见，设成只读。已经够用了。

现在可以建立变量绑定表了，HSNMP_VBL hVbl = SnmpCreateVbl(m_hSession, NULL, NULL);后两个参数是OID和值。可以一次性建好变量绑定表。我这里是先建了一个空的，一会儿再往里填值。

这里就用到OID了，但我们习惯的“1.3.6.1...”这种字符串形式，需要通过SnmpStrToOid转成smiOID类型。这是为了API拼SNMP包方便，定义的一个新类型。因为最终的SNMP请求的UDP包，是字节形式的。是要符合BER编码规范的。所谓BER编码，就是将一个数据封装成：标志，长度，内容这样一个数据段。比如规定整形标志是1,再假设1234的十六进制是0x12，0x34。那么我要发一个1234出去就要这样编码：01 02 12 34。

将变量加入到变量绑定表：SnmpSetVb(hVbl, 0, &oid, NULL)，第二个参数是索引，第三个参数是变量，第四个参数是值。

hSendPDU = SnmpCreatePdu(m_hSession, SNMP_PDU_GETNEXT, 0, 0, 0, hVbl);构造实际的UDP包。PDU:协议数据单元。第二个参数说明请求的类型：可以传一开始提到的那几种类型，snmp.h里面有定义：

#define SNMP_PDU_GET                (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x0)
#define SNMP_PDU_GETNEXT            (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x1)
#define SNMP_PDU_RESPONSE           (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x2)
#define SNMP_PDU_SET                (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x3)
#define SNMP_PDU_V1TRAP             (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x4)
#define SNMP_PDU_GETBULK            (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x5)
#define SNMP_PDU_INFORM             (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x6)
#define SNMP_PDU_TRAP               (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x7)



一切就绪，最后就要发包了：

SnmpSendMsg(m_hSession, m_hsrcAgent, m_hdstAgent, m_hContext, hSendPDU)

这里面几个参数都是前面准备好的，这个方法就没啥可说的了。正确调用后，就可以等着回应了。



在回调函数SNMPAPI_STATUS CALLBACK CSNMPManager::snmp_callback(HSNMP_SESSION hSession,
  HWND hWnd,
  UINT wMsg,
  WPARAM wParam,
  LPARAM lParam,
  LPVOID lpClientData
)

里，我们可以得到一个hSession.这个是代理那边扔过来的。和我们的hSession没啥关系。

之后我们通过这个hSession，调用SnmpRecvMsg(hSession, &hsrcAgent, &hdstAgent, &hContext, &hRecvPDU))，得回实体，上下文，以及响应的PDU。

得到响应的PDU，比较激动的时刻就到了，因为我们要开始拆响应包了，如果从里面拆出了我们想查的值，那就说明OK了，于是调用：

 SnmpGetPduData(hRecvPDU, NULL, NULL, (smiLPINT)&nErrStatus, (smiLPINT)&nErrIndex, &hRecvVbl)

晕，得到了变量绑定表，那就是说还有一层，不过保佑啊，可千万别一层层剥下来，最后得到4个字节值，转换编码一瞅，俩字“挠挠”，呵呵。

int nVblCnt = SnmpCountVbl(hRecvVbl);

得一下变量绑定表的长度。

smiOID oid;
 smiVALUE val;
 for (int i=1; i<=nVblCnt; i++)
 {

  SnmpGetVb(hRecvVbl, i, &oid, &val); 
  parseVb(val);


 }

循环，得一下每一个变量。得到OID的值，可以用SnmpOidToStr得回成字符串的形式看一下，是哪个OID的值。

然后值的类型是smiOID，很像COM里的variant_t。一个类型值拖着一个大的联合体。

有：

#define SNMP_SYNTAX_INT         (ASN_UNIVERSAL | ASN_PRIMITIVE | 0x02)
#define SNMP_SYNTAX_BITS        (ASN_UNIVERSAL | ASN_PRIMITIVE | 0x03)
#define SNMP_SYNTAX_OCTETS      (ASN_UNIVERSAL | ASN_PRIMITIVE | 0x04)
#define SNMP_SYNTAX_NULL        (ASN_UNIVERSAL | ASN_PRIMITIVE | 0x05)
#define SNMP_SYNTAX_OID         (ASN_UNIVERSAL | ASN_PRIMITIVE | 0x06)
#define SNMP_SYNTAX_INT32       SNMP_SYNTAX_INT
#define SNMP_SYNTAX_IPADDR      (ASN_APPLICATION | ASN_PRIMITIVE | 0x00)
#define SNMP_SYNTAX_CNTR32      (ASN_APPLICATION | ASN_PRIMITIVE | 0x01)
#define SNMP_SYNTAX_GAUGE32     (ASN_APPLICATION | ASN_PRIMITIVE | 0x02)
#define SNMP_SYNTAX_TIMETICKS   (ASN_APPLICATION | ASN_PRIMITIVE | 0x03)
#define SNMP_SYNTAX_OPAQUE      (ASN_APPLICATION | ASN_PRIMITIVE | 0x04)
#define SNMP_SYNTAX_NSAPADDR    (ASN_APPLICATION | ASN_PRIMITIVE | 0x05)
#define SNMP_SYNTAX_CNTR64      (ASN_APPLICATION | ASN_PRIMITIVE | 0x06)
#define SNMP_SYNTAX_UINT32      (ASN_APPLICATION | ASN_PRIMITIVE | 0x07)

这几种值类型。常用的有SNMP_SYNTAX_UINT32、SNMP_SYNTAX_OCTETS、SNMP_SYNTAX_OID这几种吧，用法都差不多。没啥问题。



代码到这里，架子就基本上搭好了，我们可以传一个OID过去，看看返回值了，我只知道想查什么，可我怎么知道它的OID是什么呀？别急，有一个网站很好，[[url]www.mibdepot.com[/url]](http://www.mibdepot.com/)这上面有很全的MIB资料。分平台，设备，很齐全。可以上它上面去查。我查了一下，在linux下，如果想查磁盘空间，应该去查

# HOST-RESOURCES-MIB (v2)Tree

子树，下面有

|**hrStorage**|GROUP|1.3.6.1.2.1.25.2iso(1). org(3). dod(6). internet(1). mgmt(2). mib-2(1). host(25). hrStorage(2)| | | | | |
|----|----|----|----|----|----|----|----|
||hrMemorySize|SCALAR|read-only|KBytes|1.3.6.1.2.1.25.2.2.0| | |
||hrStorageTable|TABLE|not-accessible|SEQUENCE OF|1.3.6.1.2.1.25.2.3| | |
|||hrStorageEntry|ENTRY|not-accessible|HrStorageEntry|1.3.6.1.2.1.25.2.3.1| |
||||hrStorageIndex|TABULAR|read-only|Integer32 ( 1..2147483647 )|1.3.6.1.2.1.25.2.3.1.1|
||||hrStorageType|TABULAR|read-only|AutonomousType|1.3.6.1.2.1.25.2.3.1.2|
||||hrStorageDescr|TABULAR|read-only|DisplayString|1.3.6.1.2.1.25.2.3.1.3|
||||hrStorageAllocationUnits|TABULAR|read-only|Integer32 ( 1..2147483647 )|1.3.6.1.2.1.25.2.3.1.4|
||||hrStorageSize|TABULAR|read-write|Integer32 ( 0..2147483647 )|1.3.6.1.2.1.25.2.3.1.5|
||||hrStorageUsed|TABULAR|read-only|Integer32 ( 0..2147483647 )|1.3.6.1.2.1.25.2.3.1.6|
||||hrStorageAllocationFailures|TABULAR|read-only|Counter32|1.3.6.1.2.1.25.2.3.1.7|
|hrDeviceTypes|OBJ ID|||1.3.6.1.2.1.25.3.1| | | |



这里面有一个表，表下面有行，一个分区是一行，每行有索引，有分区描述（一般是卷标），有分区尺寸，有已用空间。

至此，眼前的任务，算是应付下来了。这个过程中，还会牵扯到传输设置，表的操作，GetNext操作的实现等内容，今天实在来不及全记下来了，我稍后再补充完整。











