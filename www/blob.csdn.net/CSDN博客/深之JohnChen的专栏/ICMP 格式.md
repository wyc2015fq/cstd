# ICMP 格式 - 深之JohnChen的专栏 - CSDN博客

2007年08月01日 13:25:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6614


![](http://x5meghwa.onlyblog.com/UploadFiles/2007-6/54869.87943801.jpg)

![](http://x5meghwa.onlyblog.com/UploadFiles/2007-6/54889.61701937.jpg)

因特网控制消息协议（ICMP）允许路由器或目的主机向数据包源结点报告在数据报处理过程中出现的错误，是IP的必要组成部分，所有使用IP的主机和路由器都必须使用ICMP。只有在处理未分段的数据报或在分段数据报的第一段发生错误时才发送ICMP消息！

ICMP报文分为两种类型:

CMP差错报文
ICMP询问报文

ICMP消息的类型：

|类型字段|消息类型|
|----|----|
|0|回送应答|
|3|目的结点不可达|
|4|源结点抑制|
|5|重定向|
|8|回送请求|
|11|时间超出|
|12|参数问题|
|13|时标请求|
|14|时标应答|
|15|信息请求|
|16|信息应答|
|17|地址掩码请求|
|18|地址掩码应答|

与ping有关：回送请求（8）和回送应答（0）消息提供了一种用于确定两台计算机之间是否可以进行通信的机制。回送请求消息的接收者应该在回送就答消息中返回消息。Ping命令使用回送请求和回送应答这两种ICMP消息检查两台主机之间的可达性。其他略，请参见相关RFC文档！

以下是在ADSL环境中抓取的ping　DNS的数据！

![](http://x5meghwa.onlyblog.com/UploadFiles/2007-6/64844.93468821.jpg)

ICMP请求数据报，类型为8。ping的默认长度为32字节!

![](http://x5meghwa.onlyblog.com/UploadFiles/2007-6/65034.73262814.jpg)

       ICMP应答：类型为0

       更多信息：

![](http://x5meghwa.onlyblog.com/UploadFiles/2007-6/65333.67518877.jpg)

ICMP TYPE NUMBERS

(last updated 2007-05-23)

The Internet Control Message Protocol (ICMP) has many messages that
are identified by a "type" field.

Type Name     Reference
---- -------------------------  ---------
  0 Echo Reply     [RFC792]
  1 Unassigned        [JBP]
  2 Unassigned        [JBP]
  3 Destination Unreachable    [RFC792]
  4 Source Quench      [RFC792]
  5 Redirect     [RFC792]
  6 Alternate Host Address       [JBP]
  7 Unassigned        [JBP]
  8 Echo      [RFC792]
  9 Router Advertisement   [RFC1256]
 10 Router Solicitation   [RFC1256]
 11 Time Exceeded     [RFC792]
 12 Parameter Problem    [RFC792]
 13 Timestamp     [RFC792]
 14 Timestamp Reply     [RFC792]
 15 Information Request    [RFC792]
 16 Information Reply    [RFC792]
 17 Address Mask Request                     [RFC950]
 18 Address Mask Reply    [RFC950]
 19 Reserved (for Security)      [Solo]
 20-29 Reserved (for Robustness Experiment)     [ZSu]
 30 Traceroute    [RFC1393]
 31 Datagram Conversion Error  [RFC1475]
 32     Mobile Host Redirect              [David Johnson]
 33     IPv6 Where-Are-You                 [Bill Simpson]
 34     IPv6 I-Am-Here                     [Bill Simpson]
 35     Mobile Registration Request        [Bill Simpson]
 36     Mobile Registration Reply          [Bill Simpson]
 37     Domain Name Request                     [RFC1788]
 38     Domain Name Reply                       [RFC1788]
 39     SKIP                                    [Markson]
 40     Photuris                                [RFC2521]
 41     ICMP messages utilized by experimental  [RFC4065]
        mobility protocols such as Seamoby
 42-255 Reserved        [JBP]

Many of these ICMP types have a "code" field.  Here we list the types
again with their assigned code fields.

Type    Name                                    Reference
----    -------------------------               ---------
  0     Echo Reply                               [RFC792]

        Codes
            0  No Code

  1     Unassigned                                  [JBP]

  2     Unassigned                                  [JBP]

  3     Destination Unreachable                  [RFC792]

 Codes
     0  Net Unreachable
     1  Host Unreachable
            2  Protocol Unreachable
            3  Port Unreachable
            4  Fragmentation Needed and Don't Fragment was Set
            5  Source Route Failed
            6  Destination Network Unknown
            7  Destination Host Unknown
            8  Source Host Isolated
            9  Communication with Destination Network is
               Administratively Prohibited
           10  Communication with Destination Host is
               Administratively Prohibited
           11  Destination Network Unreachable for Type of Service
           12  Destination Host Unreachable for Type of Service
           13  Communication Administratively Prohibited      [RFC1812]
           14  Host Precedence Violation                      [RFC1812]
           15  Precedence cutoff in effect                    [RFC1812]

  4     Source Quench                            [RFC792]
        Codes
            0  No Code

  5     Redirect                                 [RFC792]

        Codes
            0  Redirect Datagram for the Network (or subnet)
            1  Redirect Datagram for the Host
            2  Redirect Datagram for the Type of Service and Network
            3  Redirect Datagram for the Type of Service and Host

  6     Alternate Host Address                      [JBP]

        Codes
            0  Alternate Address for Host

  7     Unassigned                                  [JBP]

  8     Echo                                     [RFC792]

        Codes
            0  No Code

  9     Router Advertisement                    [RFC1256]

        Codes
            0  Normal router advertisement      
           16  Does not route common traffic    [RFC2002]

 10     Router Selection                        [RFC1256]

        Codes
            0  No Code

 11     Time Exceeded                            [RFC792]

        Codes
            0  Time to Live exceeded in Transit
            1  Fragment Reassembly Time Exceeded

 12     Parameter Problem                        [RFC792]

        Codes
            0  Pointer indicates the error
            1  Missing a Required Option        [RFC1108]
            2  Bad Length

 13     Timestamp                                [RFC792]

        Codes
            0  No Code

 14     Timestamp Reply                          [RFC792]

        Codes
            0  No Code

 15     Information Request                      [RFC792]

        Codes
            0  No Code

 16     Information Reply                        [RFC792]

        Codes
            0  No Code

 17     Address Mask Request                     [RFC950]

        Codes
            0  No Code

 18     Address Mask Reply                       [RFC950]

        Codes
            0  No Code

 19     Reserved (for Security)                    [Solo]

 20-29  Reserved (for Robustness Experiment)        [ZSu]

 30     Traceroute                              [RFC1393]

 31     Datagram Conversion Error               [RFC1475]

 32     Mobile Host Redirect              [David Johnson]

 33     IPv6 Where-Are-You                 [Bill Simpson]

 34     IPv6 I-Am-Here                     [Bill Simpson]

 35     Mobile Registration Request        [Bill Simpson]

 36     Mobile Registration Reply          [Bill Simpson]

 39     SKIP                                    [Markson]

 40     Photuris                                [RFC2521]

 Codes
            0 = Bad SPI
            1 = Authentication Failed
            2 = Decompression Failed
            3 = Decryption Failed
            4 = Need Authentication
            5 = Need Authorization

41-252  Unassigned

253     RFC3692-style Experiment 1 (*)        [RFC4727]
254     RFC3692-style Experiment 2 (*)        [RFC4727]

   (*)  It is only appropriate to use these values in explicitly-
      configured experiments; they MUST NOT be shipped as defaults in
      implementations.  See RFC 3692 for details.

ICMP Extension Objects Classes and Class Sub-types - per [RFC4884]
Registration Procedures: 
0-246 First Come First Serve
247-255 Private Use 

Object
Class
Value   Class Name                  Reference
------  --------------------------  ---------
  1     MPLS Label Stack Class      [RFC-ietf-mpls-icmp-08.txt]

        Sub-types                          [RFC-ietf-mpls-icmp-08.txt]
     C-Type      
           0          Reserved                      [RFC-ietf-mpls-icmp-08.txt]
           1          Incoming MPLS Label Stack     [RFC-ietf-mpls-icmp-08.txt]
           0x02-0xF6  Available for assignment      [RFC-ietf-mpls-icmp-08.txt]
           0xF7-0xFF  Reserved for private use      [RFC-ietf-mpls-icmp-08.txt]

        Allocation Policy: C-Type values for Class-num 1 are 
        assignable on a first-come-first-serve (FCFS) basis
        [RFC2434].

