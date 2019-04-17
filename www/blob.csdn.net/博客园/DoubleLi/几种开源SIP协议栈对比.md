# 几种开源SIP协议栈对比 - DoubleLi - 博客园





几种开源[SIP协议栈](https://www.baidu.com/s?wd=SIP%E5%8D%8F%E8%AE%AE%E6%A0%88&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4nH79uHI9uH6smHfYuhRv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPjn1nWc4P1c1nWfYP1RdPj0Y)对比

随着VoIP和NGN技术的发展，H.323时代即将过渡到SIP时代，在H.323的开源协议栈中，Openh323占统治地位，它把一个复杂而又先进的H.323协议栈展现在普通程序员的眼前，为H.323普及立下了汗马功劳。而然当在SIP时代，则出现了群雄割据的状况，SIP相对于H.323简单，灵活，于是各种协议栈层出不穷，下面将详细对比最具有代表性的5个开源项目：OPAL,VOCAL,sipX,ReSIProcate,oSIP 

1、OPAL
OPAL是Open Phone Abstraction Library，是Openh323的下一个版本，它仍然使用了Openh323的体系结构，并在其基础上进行扩展，同时实现了SIP,H.323，但在音频和视频的编码和传输部分有较大改动。OPAL初衷设计是包含任何电话通信协议，所以其底层进行了高度的抽象化，所以也能够很容易的支持MGCP,PSTN和将来会出现的协议。不过由于Openh323的最后一个版本还在开发中，所以原本6月发布的OPAL也被推迟，现有的OPAL还非常不完善，BUG也非常多，不过相信以Openh323的开发班底，一定能让OPAL十分优秀。
CVS : :pserver:anonymous@cvs.sourceforge.net:/cvsroot/openh323/opal
Language : C++
VxWorks port : Yes 
Win32 port : Yes 
Linux port : Yes 
Supports RFC 3261 : Yes 
Supports RFC 2327 : Yes 
Supports RFC 3264 : Yes 
Supports RFC 3263 : No 
Supports RFC 3515 : Yes 
Supports RFC 3262 : No 
Supports RFC 3311 : No 
TCP : Yes 
UDP : Yes 
SIZE :  8MB
License : MPL
Document : None
Samples : UA,GK 

2、VOCAL
VOCAL是vovida.org开发的SIP系统，VOCAL应该是目前功能最完善，使用者最多的开源[SIP协议栈](https://www.baidu.com/s?wd=SIP%E5%8D%8F%E8%AE%AE%E6%A0%88&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4nH79uHI9uH6smHfYuhRv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPjn1nWc4P1c1nWfYP1RdPj0Y)了.它不只包括了协议栈，还包括了h323与sip转换网关，对SIP的各种Server的功能支持也非常完善.不过很可惜，不支持windows平台，而且自从vovida被CISCO收购以后就停止了开发，最后的版本是2003年4月的1.5.0。
CVS : :pserver:anonymous@cvs.vovida.org:/cvsroot/vocal
Language : C++
VxWorks port : No 
Win32 port : Partial 
Linux port : Yes 
Supports RFC 3261 : Partial 
Supports RFC 2327 : Yes 
Supports RFC 3264 : 
Supports RFC 3263 : 
Supports RFC 3515 : Yes 
Supports RFC 3262 : 
Supports RFC 3311 : 
TCP : Yes 
UDP : Yes 
SIZE : 6MB
License: Vovida software licencse
Document : Few
Samples : UA,GK,GW 

3、sipX
sipX是一个SIP系统，由SIPFoundry开发。sipX是从reSIProcate分离出来的，sipX除了包括SIP stack外，还包括了sipXphone,sipXproxy,sipXregistry等等...,由它们构成了完整的SIP系统，而且sipx还支持嵌入式系统，各个模块可以按需取舍。不过可惜是几乎没有任何开发文档。
SVN : [http://scm.sipfoundry.org/viewsvn/](http://scm.sipfoundry.org/viewsvn/)
Language : C++
VxWorks port : Yes 
Win32 port : Yes 
Linux port : Yes 
Supports RFC 3261 : Yes 
Supports RFC 2327 : Yes 
Supports RFC 3264 : Yes 
Supports RFC 3263 : Yes 
Supports RFC 3515 : Yes 
Supports RFC 3262 : No 
Supports RFC 3311 : No 
TCP : Yes 
UDP : Yes 
SIZE : <4 Mb 
License : LGPL 
Document : None
Samples : UA,GK,GW 

4、ReSIProcate
ReSIProcate同样也是由SIPFoundry开发，ReSIProcate最开始起源于Vocal,由于Vocal开始只支持rfc3254，为了支持最新的rfc3261,ReSIProcate诞生了，但现在，ReSIProcate已经成为一个独立[SIP协议栈](https://www.baidu.com/s?wd=SIP%E5%8D%8F%E8%AE%AE%E6%A0%88&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4nH79uHI9uH6smHfYuhRv0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPjn1nWc4P1c1nWfYP1RdPj0Y)了，它十分稳定，并且很多商业程序都在使用。
SVN : [http://scm.sipfoundry.org/viewsvn/](http://scm.sipfoundry.org/viewsvn/)resiprocate/main/sip/
Language : C++
VxWorks port : No 
Win32 port : Yes 
Linux port : Yes 
Supports RFC 3261 : Yes 
Supports RFC 2327 : Yes 
Supports RFC 3264 : Yes 
Supports RFC 3263 : Partial 
Supports RFC 3515 : Yes 
Supports RFC 3262 : No 
Supports RFC 3311 : No 
TCP : Yes 
UDP : Yes 
SIZE : < 2.5 Mb 
License : Vovida 
Document : Few
Samples : None








