# 最核心4大NFC技术规范详解 - xqhrs232的专栏 - CSDN博客
2013年11月25日 17:40:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1118
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://blog.csdn.net/eager7/article/details/8643302](http://blog.csdn.net/eager7/article/details/8643302)
相关网帖
1、近距离无线通信(NFC)技术标准解析----[http://tech.yktchina.com/2007-11/20071122929107773.html](http://tech.yktchina.com/2007-11/20071122929107773.html)

1998年,飞利浦、索尼和诺基亚创建了 NFC论坛,宗旨是推动NFC的发展普及和规范化。论坛至今共推出了5大类技术规范：协议技术规范(Protocol Technical Specification );数据交换格式技术规范(Data Exchange FormatTechnical Specification); NFC标签类型技术规范(NFC Forum Tag TypeTechnical Specifications ) ; 记录类型定义技术规范(Record Type Definitionf Technical
 Specifications); 参考应用技术规范(Reference Application Technical Specifications )。本文将介绍技术开发最核心的前4点规范。
一、协议技术规范
NFC论坛的协议技术规范又包含了3个技术规范,分别为:
1、NFC的逻辑链路控制协议技术规范(NFC Logical Link Control Protocol(LLCP) Technical Specification)：定义了 OSI模型第2层的协议,以支持两个具有NFC功能的设备之间的对等通信。LLCP是一个紧凑的协议,基于业界标准IEEE802.2,旨在支持有限的数据传输要求,如小文件传输或网络协议,这反过来又会为应用程序提供可靠的服务环境。NFC的LLCP与ISO IIEC18092标准相比,同样为对等应用提供了一个坚实的基础,但前者加强了后者所提供的基本功能,且不会影响原有的NFC应用或芯片组的互操作性。
2、NFC数字协议技术规范：本规范强调了用于NFC设备通信所使用的数字协议,提供了在ISO I IEC 18092和ISO I IEC 14443标准之上的一种实现规范。该规范定义了常见的特征集,这个特征集可以不做进一歩修改就可用于诸如金融服务和公共交通领域的重大NFC技术应用。它还涵盖了 NFC设备作为发起者、目标、读写器和卡仿真器这四种角色所使用的数字接口以及半双工传输的协议。NFC设备间可以使用该规范中给出的位级编码、比特率、帧格式、协议和命令集等来交换数据并绑定到LLCP协议。
3、NFC活动技术规范：该规范解释了如何使用NFC数字协议规范与另–个NFC设备或NFC Forum标签来建立通信协议。参考应用技术规范包括了 NFC论坛连接切换技术标准(NFC Forum Connection Handover Technical Specification),其中定义了使两个[NFC](http://www.o2oway.com/nfcjs/)设备使用其他无线通信技术建立连接所使用的结构和交互序列。该规范一方面使开发人员可以选择交换信息的载体,如两个NFC手机之间选择蓝牙或WiFi来交换数据;另一方面与NFC兼容的通信设备可以定义在连接建立阶段需要在NFC数据交换格式报文中承载的所需的信息。
二、NFC数据交换格式技术规范(NDEF )
NDEF,定义了NFC设备之间以及设备与标签之间传输数据的一种消息封装格式。该协议认为设备之间传输的信息可以封装成一个NDEF消息,而-个消息可以山多个NDEF记录构成,如图所示。
![NDEF消息](http://www.o2oway.com/wp-content/uploads/2012/11/NDEF011.jpg)
单个NDEF记录包含了多部头域和有效载荷域。首部包含了五个标志位(MB,ME,CF,SR,IL)、标签类型分类TNF、长度可变区域的长度信息、类型识别位、一个可选的记录标识符(ID)。如表所示。上图中R1至Rn表示有n个记录,其中R1的MB位值为1是表示一个消息开始,Rn记录的ME位为1表示消息结束,中间的记录这两位值为0。
![NDEF记录格式](http://www.o2oway.com/wp-content/uploads/2012/11/NDEF02.jpg)
MB和ME位:用于标志一个消息相对应的开始和结束的记录。
CF值为1时,说明存在下一个记录。
SR定义了负载域(Payload)的长度。0值表示Payload Length域的大小是一个4字节的无符号整数。1值表示一个字节的无符号整数。该标志位用于减少短记录的内存浪费。
IL为1 ,则需要给出可选ID域以及它的相关长度域的值。
TNF(Type Name Format)的定义如下表所示。
![TNF定义](http://www.o2oway.com/wp-content/uploads/2012/11/NDEF03.jpg)
三、NFC标签类型技术规范
NFC论坛目前提出的标签类型规范可兼容下面4类NFC标签。
第一类型标签是基于14443A协议,标签内存最小为96个字节,可动态扩充。如果标签只涉及到简单的读写存储,例如实现简单的智能海报功能,该类标签是完全可用的。此类标签主要用于实现读取信息,具有操作简单,成本小等优点。
第二类标签同样基于14443A协议,但仅支持phlips公司提供MIFARE UltraLight类型卡。
第三类型标签是由SONY独家提供的Fecila技术类型。
第四类型标签兼容14443A/B协议,该类标签属于智能标签,接收应用协议数据单元(APDU,Application Protocol Data Unit)指令,拥有较大的存储空间,能完成一些认证或安全算法,可用于实现智能交互和双界面标签的相关操作。此类标签应用范围广泛,可以适应未来不断的研究开发。
四、记录类型定义技术规范
NFC论坛给出了多种类型不同的5种RTD，分别是:”U” URI记录、”Sp” Smart Poster记录、”Sig” Signature记录、”T”简单文本记录和”Gc”控制类型记录。
1、简单文本记录(“T”, NFC Text RTD Technical Specification):提供了一种有效的方法通过使用RTD机制和NDEF格式以多种语言存储text字符串。它包含了描述性文本,以及语言和编码信息。一般和别的记录一起使用,用于描述记录的内容或功能。
2、URI记录(“U”, NFC URI RTD Technical Specification):提供了一种有效的方法通过使用RTD机制和NDEF格式以多种语言存储统一资源描述符URI (Uniform Resource Identifier )。该记录涵盖了 URL, e-mail地址,电话号码,以及SMS信息。
3、Smart Poster记录(“Sp ” , NFC Smart Poster RTD Technical Specification):定义了一种用来在NFC标签上存放、或是在设备之间传输URL、SMS或电话号码的类型。Smart Poster RTD构建在RTD机制和NDEF格式的基础之上,并使ffl 了 URI RTD和Text RTD作为构建模块。
4、控制类型记(“Gc”, NFC Generic Control RTD Technical Specification):提供了一个NFC设备、标签或卡(源设备)通过NFC通信以一种简单的方式向另一个NFC设备(目标设备)来请求一个特定动作(例如启动一个应用或设置一种模式)。
5、Signature记录(“Sig”, NFC Signature RTD Technical Specification):规定了对单个或多个NDEF记录进行签名时所使用的格式。定义了需要的和可选的签名RTD域,并提供了一个合适的签名算法和证书类型以用来创建一个签名。并没有定义或强制使用某个特定的PKI或证书系统,也没有定义Signature RTD使用的新算法。证书的验证和撤销过程超出了该规范的范围。
