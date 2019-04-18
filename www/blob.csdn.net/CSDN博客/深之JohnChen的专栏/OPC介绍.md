# OPC介绍 - 深之JohnChen的专栏 - CSDN博客

2017年10月06日 13:17:20[byxdaz](https://me.csdn.net/byxdaz)阅读数：6472


一、OPC概念

OPC(OLE forProcess Control, 用于过程控制的OLE)是一种利用微软的COM/DCOM技术来达成自动化控制的协定，一个工业标准，管理这个标准国际组织是OPC基金会。

为什么需要OPC？[](http://www.hifreud.com/2014/12/27/opc-2-what-is-opc/#%E4%B8%BA%E4%BB%80%E4%B9%88%E9%9C%80%E8%A6%81opc)

OPC是为了不同供应厂商的设备和应用程序之间的软件接口标准化，使其间的数据交换更加简单化的目的而提出的。作为结果，从而可以向用户提供不依靠于特定开发语言和开发环境的可以自由组合使用的过程控制软件组件产品。

利用驱动器的系统连接：

![](https://img-blog.csdn.net/20171006131738045)

利用OPC的控制系统构成:

![](https://img-blog.csdn.net/20171006131748413)

OPC是一种利用微软的COM/DCOM技术来达成自动化控制的协定，采用典型的C/S模式，针对硬件设备的驱动程序由硬件厂商完成，提供统一OPC接口标准的Server程序，软件厂商只需按照OPC标准接口编写Client程序就访问Server程序进行读写，即可实现与硬件设备的通信。

OPC协定包括：

1.DA(Data Access)规范：访问数据主要采用该规范

2.A&E(Alarm and Event)规范：基于事件，提供Client端订阅，事件触发后Server端主动提交回数据的数据访问方式。

3.HDA(History data access)规范

4.OPC XML-DA规范，XML-basedexchange of real-time values

二、OPC UA

为了应对标准化和跨平台的趋势，为了更好的推广OPC，OPC基金会近些年在之前OPC成功应用的基础上推出了一个新的OPC标准-OPC UA。OPC UA接口协议包含了之前的 A&E, DA,OPC XMLDA or HDA，只使用一个地址空间就能访问之前所有的对象，而且不受WINDOWS平台限制，具有跨平台性，因为它是从传输层Scoket以上来定义的，导致了灵活性和安全性比之前的OPC都提升了。同时OPC UA也使用了可靠的通信机制，接口简单一致。比如：对传统的三种不同类型OPC服务器的访问
 - 数据访问（DA）、报警和事件（AE）、历史数据访问（HDA），要获得一个温度传感器的当前值、一个高温度事件和温度的历史平均值，要依次使用不同的命令执行；而使用OPC UA，仅用一个组件就非常容易地完成了。配置和工程的时间也因此可以大大缩短。

OPC UA的优势：

1.一个通用接口集成了之前所有OPC的特性和信息，A&E,DA,OPC XML DA or HDA

2.更加开放，平台无关性，WINDOWS,LINUX都能兼容

3.扩展了对象类型，支持更复杂的数据类型比如变量，方法和事件

4.在协议和应用层集成了安全功能，更加安全

5.易于配置和使用

OPC和OPC UA协议区别：

OPC虽然通过配置COM/DOM来提供数据加密和签名功能，配置防火墙，用户权限来让数据访问变得更加安全，但是会增加额外的工作量，尤其是对非IT的工程师来说；对于OPCUA，数据加密和签名，防火墙等都是默认的功能。比如基于DOM的OPC使用的动态端口分配，端口不固定，让防火墙难以确定，而OPC UA的端口都是唯一的，比如SINUMERIK 840D是PORT 4840，SIMATIC S7是PORT 4845。DOM/COM也可以生成不同级别的事件日志，但日志内容不够详细，只会提供“谁连接上服务器”这种，而对于OPC
 UA来说都是默认的功能，生成的日志内容更全面。

核心的区别是因为OPC和OPC UA协议使用的TCP层不一样，如下：

OPC是基于DOM/COM上，应用层最顶层；OPC UA是基于TCP IPscoket 传输层。

三、OPC逻辑对象模型

OPC逻辑对象模型包括３类对象：OPC server对象、OPC group对象、OPC item对象，每类对象都包括一系列接口。

![](https://img-blog.csdn.net/20171006131805947)

这里最需要注意的是项并不是数据源，项代表了到数据源的连接。

例如一个在一个DCS 系统中的TAG 不论OPC 客户程序是否访问都是实

际存在的。项应该被认为是到一个地址的数据。大家一定要注意项的概

念。不同的组对象里可以拥有相同的项，如组1 中有对应于一个开关的

ITEMAAA，组2 中也可以有同样意义对应于一个开关的ITEMAAA，即

同样的项可以出现在不同的组中。

**3.1、OPC Server对象：**

主要功能：

1、创建和管理OPC Group对象；

2、管理服务器内部的状态信息。

**3.2、OPC Group对象：**

主要功能：

1、管理OPC Group对象的内部状态信息；

2、创建和管理Items对象；

3、OPC服务器内部的实时数据存取服务（同步与异步方式）。

属性：

name：组的名字，由客户端自己定义。

active：组的激活状态，如果为false则组内全部Item将不再从数据源取数据，在客户端中的表现是读取不到数据，同时写数据也影响不到服务器端。

update rate：服务器向客户端提交变化数据的更新速率(这个数值应比服务器设定的最小值大)。

Percent Dead band：数据死区，即能引起数据变化的最小数值百分比。

**3.3、OPC Item对象：**

主要功能：

用来描述实时数据，代表了与服务器数据源的连接，而不是数据源。一个项不能被OPC客户端访问，因此在OPC规范中没有对应于项的COM接口，所有对项的访问都需要通过OPC Group对象来实现。

属性：

name：项的名称，在服务器对应于Item ID。

active：项的激活状态。

value：项的数据值。

quality：项的品质，代表数值的可信度。

The OPC_QUALITY_xxx 定义了一个过程值或者事件的品质。并且分为3类，good, uncertain和bad。

下面的类型为合法的:

类型值说明

OPC_QUALITY_GOOD                   0xC0       值是好的。

OPC_QUALITY_LOCAL_OVERRIDE         0xD8       值被覆盖。典型意思为输入失去连接和手动被强制。

下面的类型为不确定:

类型值说明

OPC_QUALITY_UNCERTAIN              0x40       没有指定原因说明值为什么不确定。

OPC_QUALITY_LAST_USABLE            0x44       最后的可用值。

OPC_QUALITY_SENSOR_CAL             0x50       传感器达到了它的一个限值或者超过了它的量程。

OPC_QUALITY_EGU_EXCEEDED           0x54       返回值越限。

OPC_QUALITY_SUB_NORMAL             0x58       值有几个源，并且可用的源少于规定的品质好的源。

下面的类型为坏的:

类型值说明

OPC_QUALITY_BAD                    0x00       值为坏的，没有标明原因。

OPC_QUALITY_CONFIG_ERROR           0x04       服务器特定的配置问题。

OPC_QUALITY_NOT_CONNECTED   0x08      输入没有可用的连接。

OPC_QUALITY_DEVICE_FAILURE        0x0c       设备故障。

OPC_QUALITY_LAST_KNOWN             0x14       通讯失败。最后的值是可用的。

OPC_QUALITY_COMM_FAILURE           0x18       通讯失败，最后的值不可用。

OPC_QUALITY_OUT_OF_SERVICE        0x1C       块脱离扫描或者被锁。

OPC_QUALITY_SENSOR_FAILURE         0x10       传感器故障。

The OPC_LIMIT_xxx定义了值的限制范围。

类型值说明

OPC_LIMIT_OK                              0x00       值在上低限，高限之内。

OPC_LIMIT_LOW                             0x01       值低限。

OPC_LIMIT_HIGH                            0x02       值高限。

OPC_LIMIT_CONST                           0x03       值是常数。

Timestamp：时间戳，代表数据的存取时间。

![](https://img-blog.csdn.net/20171006132914056)

注：

1.Item的存储类型————VARIANT（编写程序时使用的主要也是它）

2.Item的数据类型(VARTYPE)：

3.Item的Alias：项的别名，由服务器设置，避免了项名称过长所带来的麻烦。

4.Group类型：公共组和私有组。公共组由多个客户共享，私有组只属于某一OPC客户。公共组对连接到服务器的所有客户都有效，而私有组只能对建立它的客户程序有效。（大多服务器未实现有公共组）

5.Item在服务器端定义，对应于硬件实际地址，客户端连接服务器端后创建并添加OPC Group，并创建一系列OPC Item（连接到服务器端定义的Item），将逻辑上等价的一组OPCItem添加到OPC Group中才能通过组对象来对数据进行存取操作。

四、OPC通信

1. 同步通信：OPC客户端对OPC服务端进行读取操作时，OPC客户端必须等到OPC服务器端完成对应操作后才能返回，在此期间OPC客户端处于一直等待的状态。

2. 异步通信：OPC客户端对OPC服务器端进行读取操作时，OPC客户端发送请求后立即返回，不用等待服务器端，当OPC服务器端完成操作后再通知客户端程序。

3. 订阅：需要服务器端支持OPC A&E规范，由客户端设定数据的变化限度，如果数据源的实时数据变化超过了该限度，服务器则通过回调返回数据给客户端。

五、OPC测试常用的OPCClient和OPCServer软件推荐

**OPCDA Client**

1、OPCClient.exe

把它放在第一位并不是有多好，而是100KB的体积，单文件，也很符合OPC标准，所以是个超便携的OPCClient。

2、Matrikon公司的OPCClient.

很不错的一个OPCClient，标准，稳定，单文件，通讯过程的信息还比较丰富，我是比较喜欢用它在创建OPC组时定义同步方式或异步方式来验证远程计算机的OPC配置是否正确。缺点就是大了点，2M多。

3、Kepware公司的OPCClient

功能很强大，标准，稳定，日志信息很丰富，最推荐的功能是支持对OPCServer中点名的条件过滤，支持点表的导入导出，支持自动导入OPCServer的所有点，根据点表识别好点坏点，按照列排序，用它可以弥补很多国产组态软件不能在线遍历OPCServer点表的功能、不能过滤OPCServer点的功能、不能识别OPCServer中好点坏点的功能等。缺点就是非单文件。

**OPCDA Server**

1、Knight.OPCServer

简单，还是简单，单文件运行起来啥都有了，148KB，很适合做简单的测试。

2、Matrikon公司的OPCServer Simulator

简单，单文件注册后，各种数据类型，各种点类型（只读点，只写点，读写点）都有了，更好的一点是，如果想做大规模的测试，10万点，那么只需要在它的Random.下添加任意名称即可，OPCServer会自动给你建出对应的测点，还是随机数变化。

3、Kepware公司的OPCServer

这是一个商业软件，需要花费金钱购买License的，但Kepware公司的OPCServer需要花钱的不是OPCServer本身，而是采集驱动。不过Kepware公司的OPCServer提供了不花钱的仿真驱动，用来测试上正是杀人越货的好东西啊，推荐理由的第三条就是说它老兄了。缺点就是太庞大，不简单，学习成本较高。但为了能满足OPC这类产品的各种测试，它真的是最佳选择。由于它是商业软件，我就不提供下载地址了，各位去它的官方网站可以下载，做好的几万点的测试工程需要的找我要。

4、iFiX

    支持双向OPC支持所有类型的ActiveX、OLE，对不健全的控件所引发的错误进行保护，对控件的属性操作完全控制。有全面解决扩展点的报警、报警记录、历史记录的方法，有查找替换功能，可以替换整个图画以及画面中的对象的属性、组态点信息，对于同类型物体，避免重复组态。内嵌VBA，具有自己的内部函数，又有广泛的VB函数，功能扩展更为有利。编辑与运行是切换进行的，这有利于对现场生产安全的保障；有独立的报警监视程序，支持在线修改，具有画面分层功能，运行时可以根据程序很方便地更换对象的连接数据源，可以使控制更灵活。支持Oracle、SQL
 Server2000、Access等关系型数据库。

5、Cimplicity

    支持OPC服务器，编辑与运行分开，有独立的报警、历史趋势运行管理程序，内嵌VBA，具有自己的内部函数，又有广泛的VB函数，组VBA与通用运行方式不一样，支持ActiveX、OLE插入，但对控件其中的一些属性进行了锁定。点的扩展功能与iFIX一样强大，但对于扩展点的报警设定比较难解决，输出问题，历史记录是没问题的。支持Oracle，SQLServer2000，Access关系型数据库。

6、InTouch：

    提供双向OPC支持，支持ActiveX控件，但不具有第三方控件的出错保护，不健全的控件会造成系统出错。采用有限的内部函数，其功能也只是常用监控的功能，复杂一点的功能如报表就只能借助于其他工具。支持关系型数据库。

7、WinCC

    双向OPC支持，支持ActiveX。使用内部语言，环境如同C语言。同样使得其功能扩展变得容易。最新的WinCC 6.0只支持连接SQL2000数据库。

OPC测试软件的下载地址：

[百度网盘](http://pan.baidu.com/share/link?shareid=388409&uk=453431525)：https://pan.baidu.com/share/link?shareid=388409&uk=453431525#list/path=%2F

六、开源OPC项目

LIGHTOPC：OPC服务器，是C/C++语言开发的，[http://www.ipi.ac.ru/lab43/lopc-en.html](http://www.ipi.ac.ru/lab43/lopc-en.html)

OpenOpcUa：是C/C++语言开发的跨平台项目,http://www.openopcua.org/

OpenOPC：是基于Python语言的项目，[http://openopc.sourceforge.net/](http://openopc.sourceforge.net/)

freeopcua：https://github.com/FreeOpcUa/freeopcua

open source and free implementation of OPC UA：[https://github.com/open62541/open62541](https://github.com/open62541/open62541)

[OPC-Client-X64](https://github.com/edimetia3d/OPC-Client-X64)：[https://github.com/edimetia3d/OPC-Client-X64](https://github.com/edimetia3d/OPC-Client-X64)

[node-opcda](https://github.com/lizhengzhou/node-opcda)：[https://github.com/lizhengzhou/node-opcda](https://github.com/lizhengzhou/node-opcda)

Utgard：OpenSCADA项目底下的子项目，纯Java编写，具有跨平台特性，全部基于DCOM实现(划重点)，目前只支持DA 2.0协议，3.0协议的支持还在开发中。[http://openscada.org/projects/utgard/](http://openscada.org/projects/utgard/)

JEasyOPC Client：底层依赖JNI，只能跑在windows环境，不能跨平台。整个类库比较古老，使用的dll是32位的，整个项目只能使用32位的JRE运行。同时支持DA 2.0与3.0协议。但64位系统兼容性不好，容易出错。[https://sourceforge.net/projects/jeasyopc/](https://sourceforge.net/projects/jeasyopc/)

参考资料[](http://www.hifreud.com/2014/12/27/opc-2-what-is-opc/#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99)

司纪刚：《OPCDA 服务器与客户程序开发指南》

OPC中国官网：[http://opcfoundation.cn/](http://opcfoundation.cn/)

OPC core components：[http://www.cnblogs.com/opcconnect/archive/2010/12/20/1911604.html](http://www.cnblogs.com/opcconnect/archive/2010/12/20/1911604.html)

