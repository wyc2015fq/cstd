# NFC协议学习分享-1 - xqhrs232的专栏 - CSDN博客
2013年11月25日 18:01:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：968
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://blog.csdn.net/eager7/article/details/8525637](http://blog.csdn.net/eager7/article/details/8525637)
很多同学在学习NFC协议的时候，觉得NFC的规范从底层到上层的应有尽有，有点无处下手的感觉。这里就和大家分享下我曾经学习NFC规范的经验。如果有不对的地方，也请各位同学批评指正。NFC
 Forum中有一个所有规范的列表，可以看到目前绝大部分的规范都已经正式发布了，当然了虽然是正式发布了，但是NFC FORUM还在不停的bug fix。表里面并没有列出NFC
 FORUM的一些新项目，如NCI NG和Wireless charging等。
在对NFC的某一个规范进行学习之前，首先要对NFC规范的整体架构有一个大致的了解，这样的话，才能从整体上进行把握。这里引用了NFC FORUM中一个公开文档中的三个架构图。NFC有三种工作模式，读卡器（R/W）,卡模拟（CE）和点对点（P2P），那么这三种模式也对应着不同的协议架构，这个其实与蓝牙中的profile很类似，即针对不同的应用，有不同的协议组合。
读卡器模式或读写模式：file:///C:/DOCUME~1/QWJ/LOCALS~1/Temp/msohtmlclip1/01/clip_image002.gif
Analogue，也就是NFC物理层的协议，主要定义了NFC设备的射频特性，如射频场的波形、强度等。
Digital Protocol，数字协议。实际上这里就可以把它理解成NFC的MAC层协议，它定义了三种NFC技术，NFC-A/B/F，分别对应着ISO14443
 TYPEA TYPEB以及Felica的规范，其中包括了初始化、冲突检测等。同时它也定义了传输协议，如NFC-DEP，ISO-DEP等。这个数字协议实际上就是ISO/IEC14443,
 ISO18092的一个合体。这里要说明一下，在这个架构图中其实还有一个Activity的规范，它可以说是Digital的一个补充。Digital中定义了帧格式，编码等通信的功能，但是如何使用Digital中的定义的帧来完成通信，则是在Activity规范中定义的。Activity中定义了一大堆的流程图及状态转移图，用来描述Digital规范中的内容是如何工作的，这样看来其实Acitivity规范也应该就是Digital中的一部分。
以上这三个协议是NFC通信的基础，在NFC的三种模式下，都要使用这三个底层的协议。可以说这三个协议是整个NFC通信的基础，掌握了这三个协议可以说对于NFC的理解就成了一半。当然这也是NFC中阅读起来最难的三个协议，当然了如果你对ISO14443/18092有了解的话，理解起来就比价轻松了，毕竟这些底层协议与非接触卡规范是一脉相承的。
TYPE1-4 TAG OPERATION: 定义了如何从TYPE1/2/3/4中读写NDEF消息。这里TYPE1/2对应着ISO14443TYPEA，而TYPE3对应着SONY的Felica，TYPE4是一个开放的TAG标准，即ISO14443
 TYPEA TYPEB和ISO 7816-4；
NDEF REFERENCE APPLICATON：是NFC Forum的RAF工作组定义的一些应用规范，这些应用规范并不是强制性的，主要是提供一些参考，但实际上很多厂家都会使用RAF提供的参考规范来开发应用，比如说连接切换（Connection
 handover）用于NFC为蓝牙或WIFI进行连接的建立。
Third Party NDEF Application：是由第三方开发的基于NDEF消息的应用。NON-NDEF Application：如果不是基于NDEF的应用，实际上就是直接调用Digital协议进行应用数据的封装，即将应用数据直接映射到Digital中定义的帧格式中。
点对点模式：
Analogue/Digital/Activity上面已经介绍过了。
LLCP：既然是P2P连接，那么必然就牵涉到逻辑链路的管理，所以需要一个LLCP层，用于链路的创建、拆除、维护等功能，同时还提供面向连接和无连接的服务等。
Simple NDEF Exchange protocol（SNEP）：在P2P模式下定义了如何进行NDEF消息的交互。SNEP的通信模式就是请求/相应模式，及initator向Target发送SNEPrequest，Target向Initiator回应SNEP
 Response消息。SNEP的请求和响应消息中包含了NDEF格式的数据。这里对NDEF做一个简单的说明，其实NDEF应该是做上层应用的开发人员直接面对的，如在ANDROID中，基于Framework中的NFC
 API很多都是与NDEF格式相关的。NDEF其实就是NFC Forum中定义的一种标准的消息格式，不仅用在了P2P中，也用在了读卡器模式中。NDEF是由RECORD(RTD, 记录)组成的，真正的用户数据就是存放在RTD中的。NFC
 FORUM定义了不同的RTD规范，如TEXT, URI，Signature等等。注意一点，在这P2P的图和R/W的图中，并没有显示的描述NDEF规范。
Protocol Bindings，这里NFC FORUM提了两个规范一个是OBEX的映射，另一个是IP协议的映射，是将这些协议能够映射到LLCP协商中。但是在NFC
 FORUM中，最终取消这两个协议，是因为这两个协议并没有起到任何作用。这些都会在以后给大家介绍。file:///C:/DOCUME~1/QWJ/LOCALS~1/Temp/msohtmlclip1/01/clip_image004.gif
卡模拟模式：
从协议架构上来说，这个架构确实是这样的，貌似是比较简单的，但是个人感觉卡模拟模式还是蛮复杂的，因为随着SE(安全单元，之前也称为NFCEE)及NCI规范的引入，不仅SE可以模拟成卡、DH(主机)和NFCC(NFC控制器)也都可以模拟成卡。这在以后的介绍中也也会向大家说明。file:///C:/DOCUME~1/QWJ/LOCALS~1/Temp/msohtmlclip1/01/clip_image006.gif
介绍完基本的NFC协议框架后，这里简单的提一下NCI规范，全称NFC Controller interface，主要的作用是将DH（Device
 host，主机，在手机中可以看做是手机的AP）和NFCC(可以看做手机中的NFC芯片，如PN544等)之间的消息标准化，让我们的手机CPU不用对每一个厂家的NFC芯片都做适配，只要CPU和NFC芯片都遵守NCI规范就可以了，当然了NFC芯片的驱动还是要适配的，呵呵。了解蓝牙的同学应该很容易理解，这就是蓝牙规范中的HCI接口，它们的设计理念是一致的。据说，Google正在为Android开发NCI。
好了，今天就介绍到这吧，以后我会对每一个协议做详细的描述。欢迎同学们批评指正，共同进步。
