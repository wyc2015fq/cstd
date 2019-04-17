# Doubango简介-sip - DoubleLi - 博客园






## Welcome



##  Android http://code.google.com/p/imsdroid/

##  http://code.google.com/p/imsdroid/source/browse/#svn%253Fstate%253Dclosed

[3GPP IMS](http://en.wikipedia.org/wiki/IP_Multimedia_Subsystem) (IP Multimedia Subsystem) is the next generation network for delivering IP multimedia services. IMS is standardized by the 3rd Generation Partnership Project (3GPP). 
IMS services could be used over any type of network, such as [3GPP LTE](http://en.wikipedia.org/wiki/3GPP_Long_Term_Evolution), GPRS, Wireless LAN, CDMA2000 or fixed line.

3GPP IMS（基于IP的多媒体子系统）是下一代IP多媒体子系统网络传输规范。IMS已被3GPP组织标准化. IMS服务将被使用在任一类型的网络上，如3GP LTE ，GPRS ,WLAN, CDMA2000 等已经被定义好的通信基带



Doubango框架是3GPP IMS(多媒体子系统(移动业务))的一种实现，目前IMS已被3GPP国际组织标准化，IMS所支持的服务将能在任一种网络中实现,这些网络包括3GP LTE(4G), GPRS, WLAN, CDMA2000等已经公用的通信网络协议







**doubango is the world's most advanced open source 3GPP IMS/RCS framework** for both embedded and desktop systems.
The framework is written in ANSI-C to ease portability and has been carefully designed to efficiently work on embedded systems with limited memory and low computing power and to be extremely portable.

**Doubango ****是当前世界上最好的一个基于****3GPP IMS/RCS ****并能用于嵌入式和桌面系统的开源框架。该框架使用****ANSCI-C****编写，具有很好的可移植性。并且已经被设计成****非常轻便且能有效的工作在低内存和低处理能力的嵌入式系统上**

## Proof Of Concept （已有的验证开源版本）
|Client-side components| |
|----|----|
|[Boghe](http://code.google.com/p/boghe/)|IMS/RCS Client for Windows|
|[IMSDroid](http://code.google.com/p/imsdroid/)|SIP/IMS Client for Android|
|[iDoubs](http://code.google.com/p/idoubs/)|SIP/IMS VideoPhone for iOS (iPhone, iPad and iPod Touch)|
|Server-side components| |
|[OpenVCS](http://code.google.com/p/openvcs/)|OpenVCS stands for Open Source Video Conferencing Server and is used to manage Multipoint Control Units ([MCU](http://en.wikipedia.org/wiki/Multipoint_Control_Unit)). Each MCU (a.k.a Bridge) can handle **up to 64 participants**.|
|[Flash2IMS](http://code.google.com/p/flash2ims/)|Adobe® Flash® to SIP/IMS Gateway.|

## Features 支持特性
- SIP (RFC 3261, 3GPP TS 24.229 Rel-9)
- IMS-AKA (RFC 3310, 3GPP TS 33.203, 35.205, 35.206, 35.207, 35.208 and 35.909)
- Private extension headers for 3GPP
- IPv4/IPv6 dual stack
- UDP, TCP, TLS and SCTP
- Service-Route Discovery (RFC 3608)
- Proxy-CSCF discovery using DHCPv4/v6 or/and DNS NAPTR
- SigComp (RFC 3320, 3485, 4077, 4464, 4465, 4896, 5049, 5112 and 1951)
- IPSec
- Security Agreement (RFC 3329)
- NAT Traversal (STUN, TURN and ICE)
- Preconditions (RFC 3312, 4032 and 5027)
- SMS over IP (3GPP TS 23.038, 24.040, 24.011, 24.341 and 24.451)
- ENUM (RFC 3761)
- The tel URI for Telephone Numbers (RFC 3966)
- SIP SIMPLE (Presence Subscription/Publication and Pager Mode IM)
- MMTel (UNI)
- SDP (RFC 3264)
- RTP/RTCP
- XCAP (RFC 4825)
- MSRP (RFC 4975 and 5547)
- Audio/Video (AMR, Speex, G.711, GSM, H.264, H.263, Theora...)
- ...and many other features

## The One Voice Profile (4G/LTE)

doubango supports both Voice and SMS over LTE as defined by the **One Voice initiative**. 
For more information, please refer to [Voice over IMS profile V1.0.0(2009-11)](http://news.vzw.com/OneVoiceProfile.pdf).



## Multimedia Telephony (MMTel)
- MMTel Supplementary Services (3GPP TS 24.173, 24607, 24.608, 24.607, 24.604, 24.611, 24.610, 24.606, 24.615 and 24.605)
- Supplementary Service Configuration (3GPP TS 24.623)
- Ad-Hoc Multi Party Conference (3GPP TS 24.605)
- Communication Waiting (3GPP TS 24.615)
- Message Waiting Indication (3GPP TS 24.606 and RFC 3842)
- Originating/Terminating Identification Restriction (3GPP TS 24.607/608)
- Communication Diversion (3GPP TS 24.604)
- Communication Barring (3GPP TS 24.611)

Supplementary Service Configuration, Communication Diversion and Communication Barring services use **XCAP** protocol, which is supported throught [tinyXCAP](http://www.doubango.org/API/tinyXCAP/).

All these features are used as part of One Voice Profile (LTE).

Supplementary Services：补充服务,

## Protocols 协议
- SIP/SigComp
- SDP
- RTP/RTCP
- XCAP (RFC 4825)
- MSRP/MSRPS (RFC 4975 and 5547)
- HTTP/HTTPS (RFC 2616 and 2617)
- DHCPv4 (RFC 2131)
- DHCPv6 (RFC 3315)
- DNS (RFC 1034, 1035, 3401, 3402, 3403, 3404, 3596, 2035, 2671, and 2782)

## SDP Offer-Answer （SDP自动应答系统）

The media layer contains a complete SOA (SDP Offer-Answer) framework with MMTel and PoC extensions to develop Multimedia telephony applications without having to read hundreds of technical specifications. The SOA machine is designed as per RFC 3264 and draft-ietf-sipping-sip-offeranswer-12.

## OMA SIMPLE IM （OMA规范下的IM）
- Pager Mode IM (MESSAGE)
- Session Mode Messaging (MSRP)
- Large Message IM (OMA)
- File Transfer(MSRP) and Image Sharing (GMSA IR.79)

## IPTV over IMS（基于IPTV协议的IMS）

   IMS-based IPTV (ETSI TS 182 027)
   Comming soon...





SDP：会话描述协议（Session Description Protocol，[SDP](http://www.ietf.org/rfc/rfc2327.txt))


参考：SIP简介

[http://blog.csdn.net/IOKE/archive/2006/04/28/694445.aspx](http://blog.csdn.net/IOKE/archive/2006/04/28/694445.aspx)




OMA：移动开放联盟（OMA）组织


 参考：OMA技术进展介绍

[http://blog.csdn.net/gobitan/archive/2007/06/07/1642870.aspx](http://blog.csdn.net/gobitan/archive/2007/06/07/1642870.aspx)




IPTV：IP电视协议。当前网络电视，手机电视使用协议



IMS: 多媒体子系统


参考：基于IMS架构的业务应用

[http://blog.csdn.net/IOKE/archive/2007/03/28/1543595.aspx](http://blog.csdn.net/IOKE/archive/2007/03/28/1543595.aspx)




LTE 长期演化(LTE)计划(Long Term Evolution)


参考:  LTE TDD在未来4G技术标准中的位置

[http://blog.csdn.net/bigwindlee/archive/2008/11/26/3379839.aspx](http://blog.csdn.net/bigwindlee/archive/2008/11/26/3379839.aspx)




# Getting Started

If you are a new developer, the best way to start programing with doubango is to download the [Programmer's Guide v1.0.0](http://doubango.googlecode.com/svn/trunk/documentation/programmer-guide.pdf)

## Supported Operating Systems
- Windows Mobile (5.0 and later), Windows XP/Vista/7
- Google Android
- Symbian S60
- Mac OS X, iPhone, iPad and iPod Touch
- Unix-like systems (Debian, Ubuntu, ...)
- Embedded Linux systems



## Bindings
- Microsoft.NET (C#, VB.NET ...)
- Java (with support for Google Dalvik VM)
- Perl
- Python
- Ruby

## API Reference

The API reference is available [here.](http://www.doubango.org/apiref.html)

## Source Code
- To download source code, please visit [http://code.google.com/p/doubango/source/checkout](http://code.google.com/p/doubango/source/checkout)
- If you need information about how to compile the source code, please visit ...soon
- Release notes
- [Issue tracker](http://code.google.com/p/doubango/issues/list)

## Blog

To stay tuned, please visit our blog at [http://doubango.blogspot.com](http://doubango.blogspot.com/)

## Compliance

The list of technical specifications (IETF, 3GPP/3GPP2/TISPAN, GSMA, OMA) implemented by doubango could be found at ..soon



# API Reference

For newbies, please refer to the [Programmer's Guide v1.0.0](http://doubango.googlecode.com/svn/trunk/documentation/programmer-guide.pdf)

## [tinySAK(a.k.a tiny Swiss Army Knife)](http://www.doubango.org/API/tinySAK/)
- ANSI-C Object Programing(ANSI-C面向对象程序)
- Linked lists(链表)
- String utility functions( 字符处理函数工具集)
- Memory management(内存管理)
- Dynamic buffers(动态缓存)
- Threading( 线程)
- Runnable (Runnable接口)
- Mutexes
- Semaphores
- Conditional Variables
- Timers
- Time
- Final State Machine (FSM) manager
- Base64 encoder/decoder( Base64 编码与解码)
- UUID generator (UUID 生成器)
- CRC32 and CRC16 (16位和32为循环冗余校验)
- URL encoder/decoder (URL 编码和解码)
- SHA-1, MD5, HMAC-MD5, HMAC-SHA-1 (支持的加密方式有)

No dependencies.

## [tinyNET (Networking)](http://www.doubango.org/API/tinyNET/)
- IPv4/IPv6 Sockets (UDP, TCP, TLS and SCTP) IPv4/IPv6 Sockets解析
- DHCPv4/v6  DHCPv4/v6解析
- DNS (NAPTR, PTR, SRV, MX, A, AAAA, OPT, CNAME ...)DNS解析
- ENUM
- NAT Traversal (STUN, TURN and ICE) 虚拟网络地址映射(NAT)

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/)

## [tinyHTTP (HTTP/HTTPS stack)](http://www.doubango.org/API/tinyHTTP/)
- Digest/Basic Authentication 数字/基础认证
- Pipelining 管道(管线)技术
- CONNECT, DELETE, GET, HEAD, OPTIONS, PATCH, POST, PUT and TRACE (基本的网络处理：连接，删除连接 GET/HEAD/POST请求 网络请求参数，patch, put, trace)

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/) and [tinyNET](http://www.doubango.org/API/tinyNET/)

## [tinyXCAP (XCAP implementation)](http://www.doubango.org/API/tinyXCAP/)
- AUID manager
- URL generator

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/), [tinyNET](http://www.doubango.org/API/tinyNET/) and [tinyHTTP](http://www.doubango.org/API/tinyHTTP/)

## [tinySMS (SMS over IP)](http://www.doubango.org/API/tinySMS/)
- SM-TL (3GPP TS 23.040)
- SM-RL (3GPP TS 24.011)
- SMS over 3GPP IMS/LTE networks (3GPP TS 24.341)

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/)

## [tinySigComp (Signaling Compression)](http://www.doubango.org/API/tinySigComp/)
- Stream and Dgram compression/decompression
- SIP/SDP and Presence dictionaries
- Deflate Compressor
- UDVM

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/)

## tinyIPSec (IPSec)
- Mode: Transport and Tunnel
- Protocol: AH, ESP or both
- IP Protocol: UDP and TCP
- Algorithm: HMAC-MD5-96 and HMAC-SHA-1-96
- Encryption Algorithm: NULL, DES-EDE3-CBC and AES

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/)

## tinyMSRP(MSRP)
- Large Message IM (RFC 4975 and OMA SIMPLE IM)
- File Transfer (RFC 5547)
- Image Sharing (GSMA IR.79)

Depends on ...

## tinySDP (SDP)
- SDP Offer Answer (RFC 3262 and draft-ietf-sipping-sip-offeranswer-12)

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/)

## tinyMEDIA (Audio, Video, File transfer, Large Message IM ...)
- Plugins, codecs ...

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/), [tinyNET](http://www.doubango.org/API/tinyNET/) and [tinySDP](http://www.doubango.org/API/tinySAK/)

## tinySIP (3GPP IMS/LTE framework)
- SIP (RFC 3261, 3GPP TS 24.229 Rel-9)
- IMS-AKA (RFC 3310, 3GPP TS 33.203)
- IPv4/IPv6 dual stack
- UDP, TCP, TLS and SCTP
- Service-Route Discovery (RFC 3608)
- Proxy-CSCF discovery using DHCPv4/v6 or/and DNS NAPTR
- SigComp (RFC 3320, 3485, 4077, 4464, 4465, 4896, 5049, 5112 and 1951)
- IPSec
- Security Agreement (RFC 3329)
- NAT Traversal (STUN, TURN and ICE)
- Preconditions (RFC 3312, 4032 and 5027)
- SMS over IP (3GPP TS 23.038, 24.040, 24.011, 24.341 and 24.451)
- ENUM (RFC 3761)
- The tel URI for Telephone Numbers (RFC 3966)
- SIP SIMPLE (Presence subsciption/publication, Pager Mode IM, ...)
- MMTel (UNI)
- SDP Offer-Answer (SOA)
- Session Timers
- File transfer (RFC 5547) and Image Sharing(GSMA IR.79)
- Large Message IM (OMA SIMPLE IM)
- To be continued....

Depends on [tinySAK](http://www.doubango.org/API/tinySAK/), [tinyNET](http://www.doubango.org/API/tinyNET/), [tinySDP](http://www.doubango.org/API/tinySDP/), [tinyMEDIA](http://www.doubango.org/API/tinyMEDIA/), [tinyHTTP](http://www.doubango.org/API/tinyHTTP/) and [tinyIPSec](http://www.doubango.org/API/tinyIPSec/)





# Doubango演示

配置服务端OpenCVS

 OpenVCS_1.0.52.551.zip

配置客户端

Android平台 [IMSDroid](http://code.google.com/p/imsdroid/)

iPod,iPad平台[iDoubs](http://code.google.com/p/idoubs/)









