# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之60---BREW 在线 工具 - 我相信...... - CSDN博客





2010年07月15日 16:35:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1345标签：[工具																[brew																[测试																[手机																[mobile																[互联网](https://so.csdn.net/so/search/s.do?q=互联网&t=blog)
个人分类：[消失的技术](https://blog.csdn.net/wireless_com/article/category/597607)





不论是典型的注册开发商，还是运营商管理的CP/SP，都可以通过开发商互联网或者运营商互联网使用另外的两个在线工具：ClassID生成器和测试签名生成器。

通过ClassID生成器可以为应用生成一个全球唯一的ClassID，在线工具的主要功能部分如图12-28所示。



**图**** 12-28****：**** ClassID****的在线生成工具**

输入应用名称，在线系统会为开发者生成一个相应的bid文件，开发者下载该bid文件，就可以安心的使用了。下面是RUIM地址簿接口的bid文件内容。

ifndef ADDRBOOK_RUIM_BID

#define ADDRBOOK_RUIM_BID



#define AEECLSID_ADDRBOOK_RUIM0x010128c4



#endif //ADDRBOOK_RUIM_BID

通过测试签名生成器可以为测试手机产生一个测试签名文件，使应用开发商可以在手机上测试目标应用。在线工具的主要功能部分如图12-29所示。



**图**** 12-29****：****测试签名的在线生成工具**

其中ESN是电子序列号Electronic Serial Number的缩写，是应用于CDMA手机中的独特不变的数字,并且是通信系统中传输时的自我标识。一个有效的ESN是一个8个数字的十六进数，例如0x11bb22ff.需要注意的是一定要以0x作为ESN的前缀。

IMEI是国际移动通讯设备识别号International Mobile Equipment Identity的缩写，是GSM手机的序列号，存储在手机的EEPROM（俗称码片）里，每一个移动设备都对一个唯一的IMEI。一个有效的IMEI有15数字，例如123456789012345。生成数字签名时不用以"0x"作为前缀. 如果你输入的IMEI只有前面14位, 需要在末尾增加“0”作为第15位数字。

MEID是移动设备标识Mobile Equipment ID的缩写，由于CDMA移动设备增多，导致原来8位的ESN不够用，所以开始使用MEID。现在的CDMA手机一般ESN和MEID两者都有。MEID也是用16进制来表示的。例如**0xA1000002B0BEB2**。前面的0xA表示CDMA手机; 如果是0x9, 就表示多模手机。在生成测试签名时，可以不用以"0x"作为前缀，而用10进制数表示。

输入ESN或者IMEI或者MEID后，在线系统会生成一个对于的测试签名，开发者下载后就可以自由使用了。](https://so.csdn.net/so/search/s.do?q=mobile&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)




