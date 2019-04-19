# FeliCa卡介绍 - xqhrs232的专栏 - CSDN博客
2014年05月12日 15:05:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2518
原文地址::[http://baike.c114.net/view.asp?id=14127-C6C05782](http://baike.c114.net/view.asp?id=14127-C6C05782)
目录·[概要](http://baike.c114.net/view.asp?id=14127-C6C05782#l1)
·[FeliCa 芯片的使用](http://baike.c114.net/view.asp?id=14127-C6C05782#l2)
·[FeliCa 芯片的安全性](http://baike.c114.net/view.asp?id=14127-C6C05782#l3)
·[FeliCa 相关规格](http://baike.c114.net/view.asp?id=14127-C6C05782#l4)
·[历史](http://baike.c114.net/view.asp?id=14127-C6C05782#l5)
FeliCa是Sony所开发出来的非接触式IC卡技术。名称由英语中代表“幸福”的 "Felicity" 和 "Card"（卡片）组合而成，是Sony的注册商标。
[](http://baike.c114.net/view.asp?id=14127-C6C05782#)概要
FeliCa 是为了非接触式IC卡而开发出来的通信技术。非接触式IC卡由读写时送出的载波引导而供给电力，由载波的调变与卡片读写沟通。例如 ISO 14443 type B，使用ASK10%调变，及NRZ编码。与此相比，FeliCa的调变同样是 ASK10%，但不同的是采用曼彻斯特编码。
最初被提案为 ISO 14443 type C, 但未被采纳。之后，FeliCa和其向后相容方式被标准化为 ISO 18092(Near Field Communication, NFC, 近距离通信)。在日本国内，被当作 JICSAP IC卡规格 V2.0 “第四部份 高速处理用IC卡”和日本鉄道サイバネティクス协议会的IC卡规格而予以标准化。
FeliCa和一般的IC卡同样有适用于现金卡或识别卡的技术，但为了要求高速处理特性(自动充值设备、大楼进出管制等)或结帐(便利商店)等等的应用，将指令集加以特殊化。因此和 ISO 7816-3 的基本指令并不相容。且 IC芯片内部的内存固定为16字节长的纪录，因此和 ISO 7816-3规定的档案结构亦不相容。
加密处理方面，相互认证使用Triple DES，通讯使用DES或Triple DES。没有公开密钥加密的规格。双模型式(接触/非接触)虽然可以有公开密钥加密，但只在接触通讯时使用。
相互认证时、缩退码被作为加解的密码来使用。不是说每一个项目个别认证、它是通过复数的访问码加密生成的键称为退缩码，这个退缩码最多可供１６个项目使用。缩退码生不成原来的密码。这样，不降低安全级别的情况下实现高速化处理。
[](http://baike.c114.net/view.asp?id=14127-C6C05782#)FeliCa
 芯片的使用
Felica 芯片除了使用在IC卡的IC芯片上，也使用在手机或手表上。
最初只有 Sony 生产制造，但之后亦与英飞凌共同开发(2001年11月发表)，日立随后采用(2002年6月发表)等等，因而有多重来源供应商。
IC卡上的使用
1997年9月由香港的“八达通”首先采用。之后，2001年11月日本的“Suica”，2002年4月新加坡的“易通卡”等等陆续采用。
在日本国内，诸如 Suica 之类的IC卡车票，bitWallet的电子钱包服务Edy，和新力金融公司(Sony Finance International Inc.)所发行的信用卡eLIO使用了 FeliCa 技术。 此外，Yodobashi Camera附有 Edy 功能并整合点数卡的 eLIO 信用卡“Gold Point Card IC eLIO”，或是信用卡和Suica合一，附加Edy
 IC金融卡的“VIEW Suica”，及 Bic Camera 和 Suica 合一的“Bic Camera Suica Card”等等的结合服务也在进行中。 在过去，采用 FeliCa 的有，在1999年到2003年间，东京临海副都心?青海的palette town内的MEGA WEB发行的“MEGA WEB Member&＃39;&＃39;&＃39;&＃39;s Card”，或2000年到2002年间在东京临海副都心?お台场 Sony Entertainment 的游乐设施 Mediage 发行的“Mediage
 Fan Card”等等。
手机上的使用
手机所使用的FeliCa芯片称作 Mobile FeliCa IC Chip，是由 Sony 和 NTT DoCoMo 合资成立的公司 FeliCa Networks 所开发的。
2004年7月使用了FeliCa芯片的手机开始上市销售。因为有FeliCa芯片，因此可以把手机当作Edy或 Suica(Mobile Suica)使用。 在使用 Mobile FeliCa Chip 的服务开始上路之前，NTT DoCoMo 就已经将钱包手机的商标定为服务标志。 Mobile FeliCa芯片最早只有Sony制造，后来的Mobile FeliCa芯片由东芝和瑞萨在2006年5月公开加入，成为三家公司共同供应。
 这个新一代的Mobile FeliCa芯片，增加了容量且通信功能加强。使用此种芯片的手机在2006年10月公开。
“iMode FeliCa”(NTT DoCoMo)
“EZ FeliCa”(au by KDDI)
“S!FeliCa(旧名为Vodafone!FeliCa)”(Softbank Mobile)
在手机上的普及状况，请参考钱包手机条目。
[](http://baike.c114.net/view.asp?id=14127-C6C05782#)FeliCa
 芯片的安全性
使用 FeliCa 芯片的卡片，经过ISO 15408 EAL4 的认证。
虽然有报道[1][2]指称 FeliCa 存在着危险性，但报道的内文中并没有明确的事实证据。
[](http://baike.c114.net/view.asp?id=14127-C6C05782#)FeliCa
 相关规格
JISX6319-4:ＩＣカード実装仕様－第４部：高速处理用ＩＣカード (日本工业标准调查会)[3]
特开平10-20780[4](相互认证?通信路暗号化)
[](http://baike.c114.net/view.asp?id=14127-C6C05782#)历史
1988年 索尼开始研发无线ＩＣ。
1994年 名称确定为FeliCa。
1994年 香港的八达通卡有限公司决定采用。这是日本以外第一次被采用。
1997年 八达通卡正式发行。这是日本以外正式被使用。
