# Mifare plus卡调试总结 - xqhrs232的专栏 - CSDN博客
2018年04月25日 22:06:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：338
原文地址::[https://blog.csdn.net/wwwtovvv/article/details/9240783](https://blog.csdn.net/wwwtovvv/article/details/9240783)
相关文章
1、Mifare PLUS卡读写模块用户手册----[http://www.docin.com/p-1252890695.html](http://www.docin.com/p-1252890695.html)
2、Mifare Plus卡读写模块----[http://tieba.baidu.com/p/3968383719?traceid=](http://tieba.baidu.com/p/3968383719?traceid=)
3、Mifare PLUS CPU读卡软件 绿色免费版----[http://dl.pconline.com.cn/download/547464.html](http://dl.pconline.com.cn/download/547464.html)
4、MIFARE Plus-X和MIFARE Plus-S的区别----[https://blog.csdn.net/zoomdy/article/details/51750286](https://blog.csdn.net/zoomdy/article/details/51750286)
5、MifarePlus简介----[https://wenku.baidu.com/view/3b65203e0912a21614792904.html](https://wenku.baidu.com/view/3b65203e0912a21614792904.html)
默认nxp发行的plus卡为L0级别未初始化的卡片，此时active操作表现为支持ISO1443-4的CPU卡特性(ATQA:
 02 00 ,SAK:20  UID: CD 65 E5 03),当执行写相关AES KEY及数据块初始化并commit后则进入L1安全级别，其active操作表现为一张M1S70卡特性(ATQA:
 02 00 ,SAK:18  UID: CD 65 E5 03),但实质其仍然可支持ISO1443-4，执行rats成功（也必须支持，否则无法执行switchL2及switch3操作），当在L1级别执行switchL2认证转换为L2级别后，其active操作后返回值为（ATQA:
 02 00 SAK:11, UID: CD 65 E5 03）,此时现有驱动中无此匹配SAK则表现为未知卡类型，在L2安全级别执行switch3认证后卡片升级至L3安全级别，重新执行active操作后返回值为(ATQA:
 02 00 ,SAK:20 , UID:CD 65 E5 03)。
将L1及L2级别的操作均运行于active态(ISO14443-3层)；当卡片位于L1级别时，可完全执行M1相关接口，也可先加执行AES
 SL1认证后再执行M1认证，但此时执行AES SL1认证后不需要产生session
 key base并与M1密钥计算推导新的M1密钥作为真正的M1认证密钥，直接使用M1原密钥即可（此点与L2级别不同）。
在L0级别，执行后PICC的返回码为与M1卡一致的ACK/NAK,为半字节返回码并不带CRC，故此时当打开CRC校验则出现CRC错误；
在L1级别的AES SL1认证过程，当执行正确则返回带CRC返回码及信息，而当出错时(AES密钥错误，RNDB解密错误等)则返回与M1卡一致的NAK，为半字节并不带CRC错误，故此时打开CRC校验则出现CRC错误。
在ISO14443-4级别执行switchL2指令，当执行成功后则PICC进入L2安全级别，此时再执行switchL2指令在第一次发送Cmd+BNo+LenCap+PCDCap2时即返回两个状态字节0x02,0x09其中0x09可理解为无效块号，但0x02无法理解。
在L2级别中，如手册所说，必须先进行AES认证在进行M1认证，并且将AES产生的session
 key base的低6字节与实际块的M1密钥异或而为真正M1块认证密钥。经过测试得AES
 Key类型必须与M1 Key类型保持一致（即均为A或者均为B类型），否则导致M1认证失败！当扇区A的AES与M1密钥类型和值与扇区B保持一致，则当对扇区A认证通过后对扇区B的访问与对扇区A的访问一致，无需再对扇区B认证。
在L2安全级别调试mifPLAuthInPro期间，调用s_AESCbcEnDecrypt时，由于其输入iv在加解密完成后会改写iv导致下次进行加解密时iv改变而导致加解密失败！必须注意iv的值！
在L2安全级别，使用强制AES+M1密钥认证中必须保持AES密钥类型与M1密钥类型一致（即AES
 TypeA Key+M1 TypeA key或者AES TypeB Key+M1 TypeB key，如AES
 TypeA Key+M1 TypeB key认证即使密钥均正确都会认证失败！）
完成L2级别的调试，可反复进行FirstAuth,并且每次FirstAuth得到的TI值均不一样，在执行完一次正确的FirstAuth认证并得到TI后才可进行正确的followAuth（followAuth的iv基于FirstAuth认证）。当执行一次正确FirstAuth后可反复进行followAuth。由于FirstAuth
 and followAuth运行于ISO14443-4 mode，故当任何错误发生PICC还是保持在ISO14443-4
 mode，无须重寻卡操作。
L2安全级别的MultiWriteBlock与MultiReadBlock命令仅支持同扇区内的多数据块读写操作！而L3安全级别的ReadBlock与WriteBlock命令支持跨扇区的连续数据块读写（仅限本扇区内！！！）
L3安全级别的followAuth操作的获取ENC
 KEY及MAC KEY加密所使用的iv向量为0而非 TI+W_ctr+R_ctr！！！
在ISO14443-4 mode操作中，只要任何错误则必须进行FirstAuth操作！
对于M1卡值相关操作的理解：Restore命令的实质是将传入的数据块对应值(必须为钱包格式)复制到M1卡内部的一个16字节的Transfer
 Buffer内，而Transfer命令的实质是将M1卡内部的Transfer
 Buffer值复制入传入的数据块内。Increment命令的实质是将输入的钱包块值加上传入的增加值并复制到Transfer Buffer内，故必须将再调用Transfer命令将Transfer
 Buffer值复制入指定的数据块内。

