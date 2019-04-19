# NXP PN53x系列芯片介绍 - xqhrs232的专栏 - CSDN博客
2017年04月18日 16:04:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1946
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://nfc-tools.org/index.php?title=PN53x](http://nfc-tools.org/index.php?title=PN53x)
相关文章
1、NXPPN533PB----[https://wenku.baidu.com/view/11f9d0600b1c59eef8c7b4d2.html](https://wenku.baidu.com/view/11f9d0600b1c59eef8c7b4d2.html)
2、
# PN533_schematics_fangka----[https://wenku.baidu.com/view/25338c27bcd126fff7050bc4.html](https://wenku.baidu.com/view/25338c27bcd126fff7050bc4.html)


# PN53x
[](http://nfc-tools.org/index.php?title=PN53x#mw-head)[](http://nfc-tools.org/index.php?title=PN53x#p-search)
## Contents
- [1Overview](http://nfc-tools.org/index.php?title=PN53x#Overview)
- [2Example](http://nfc-tools.org/index.php?title=PN53x#Example)
- [3Connectivity](http://nfc-tools.org/index.php?title=PN53x#Connectivity)
- [4Capabilities
 as initiator (reader)](http://nfc-tools.org/index.php?title=PN53x#Capabilities_as_initiator_.28reader.29)
- [5Capabilities
 as target (emulation)](http://nfc-tools.org/index.php?title=PN53x#Capabilities_as_target_.28emulation.29)
- [6Chip
 Instructions](http://nfc-tools.org/index.php?title=PN53x#Chip_Instructions)
# Overview
There are three branches of the NFC Controllers based on the [PN53x
 chipset](http://www.nxp.com/acrobat_download/literature/other/Z-card_nfc.pdf). They are all manufactured by [NXP](http://www.nxp.com/).
- PN531 - [ISO14443](http://nfc-tools.org/index.php?title=ISO14443)-A, [MIFARE](http://nfc-tools.org/index.php?title=MIFARE), [FeliCa](http://nfc-tools.org/index.php?title=FeliCa) RF,
 UART, SPI, I²C, USB 2.0
- Historically the first and not supported by NXP anymore
- PN532 - [ISO14443](http://nfc-tools.org/index.php?title=ISO14443)-A/B, [MIFARE](http://nfc-tools.org/index.php?title=MIFARE), [FeliCa](http://nfc-tools.org/index.php?title=FeliCa) RF,
 UART, SPI, I²C, low-battery mode
- PN533 - [ISO14443](http://nfc-tools.org/index.php?title=ISO14443)-A/B, [MIFARE](http://nfc-tools.org/index.php?title=MIFARE), [FeliCa](http://nfc-tools.org/index.php?title=FeliCa) RF,
 UART, USB 2.0
We encourage manufacturers to [contact us](http://nfc-tools.org/index.php?title=Contact) for
 new PN53x based products so we can test and support them.
# Example
![PN532 chip on a ACS ACR122 board](http://nfc-tools.org/images/thumb/8/84/Acr122-top-view-wires-pn532.jpg/400px-Acr122-top-view-wires-pn532.jpg)
An example of assembled chip on the internal board of the [ACR122
 NFC Reader](http://nfc-tools.org/index.php?title=ACR122). The [PN532](http://www.nxp.com/acrobat/literature/9397/75016623.pdf) is marked with a red circle. The PN53x chip
 has a set of registers and a firmware that can parse a simple instruction set. It is possible to control the chip using one of these options.
The registers are quite similar to the predecessor chip PN51x. By changing the registers you are able to activate advanced options (bit-rate, modulation impedance, fault tolerance, etc.).
The instructions in stead are completely new. With help of these instructions it is very easy to let the chip do standard NFC functionality for you (polling, anti-collision, initiator-mode,
 target-mode, MIFARE-commands). This is very convenient for software developers. They don't need to know much about embedded hardware before they can start building applications using this chip.
# Connectivity
||PN531|PN532|PN533|
|----|----|----|----|
|UART¹|**YES**|**YES**|**NO**|
|USB²|**YES**|**NO**|**YES**|
|SPI|**YES**|**YES**|**NO**|
|I²C|**YES**|**YES**|**NO**|
- High-Speed UART (up to 1 288 000 bauds)
- USB 2.0 Full-speed
# Capabilities as initiator (reader)
||PN531|PN532|PN533|
|----|----|----|----|
|[ISO14443](http://nfc-tools.org/index.php?title=ISO14443)-A|**YES** (106 kbps)|**YES** (106 kbps)|**YES** (106 kbps)|
|[ISO14443](http://nfc-tools.org/index.php?title=ISO14443)-B|**NO**|**YES** (106 kbps)|**YES** (106/212/424/847 kbps)|
|[FeliCa](http://nfc-tools.org/index.php?title=FeliCa)|**YES** (212/424 kbps)|**YES** (212/424 kbps)|**YES** (212/424 kbps)|
|[Jewel/Topaz](http://nfc-tools.org/index.php?title=Jewel/Topaz)|**NO**|**YES** (106 kbps)|**YES** (106 kbps)|
|D.E.P.|**YES**|**YES**|**YES**|
# Capabilities as target (emulation)
||PN531|PN532|PN533|
|----|----|----|----|
|[ISO14443](http://nfc-tools.org/index.php?title=ISO14443)-3|**YES** (4 bytes UID¹)|**YES** (4 bytes UID¹)|**YES** (4 bytes UID¹)|
|[ISO14443](http://nfc-tools.org/index.php?title=ISO14443)-4|**YES** (software)|**YES** (hardware)|**YES** (software)|
|[ISO14443](http://nfc-tools.org/index.php?title=ISO14443)-B|**NO**|**NO**|**NO**|
|[FeliCa](http://nfc-tools.org/index.php?title=FeliCa)|**YES** (212/424 kbps)|**YES** (212/424 kbps)|**YES** (212/424 kbps)|
|[Jewel/Topaz](http://nfc-tools.org/index.php?title=Jewel/Topaz)|**NO**|**NO**|**NO**|
|D.E.P.|**YES**|**YES**|**YES**|
- First byte of UID will be masked by 0x08 by the PN53x firmware as security countermeasure against real UID emulation.
# Chip Instructions
The instruction set of the chip used to be proprietary. There are a lot documents available on the internet that reveal these commands. With help of the available information it was possible
 to construct this library. Now User Manuals are publicly available. libnfc provide most operations by just invoking the commands available in the PN53x instruction set. For some advanced features it changes a few register values. Currently only a small set
 of instructions are supported by libnfc, but it already provides a very powerful functionality.
Official resources:
- PN531 support is discontinued, no user manual available
- [PN532 User Manual UM0701-02 (pdf)](http://www.nxp.com/documents/user_manual/141520.pdf)
- [PN533 User Manual UM0801-03 (pdf)](http://www.nxp.com/documents/user_manual/157830_PN533_um080103.pdf)
Other resources:
- [Digant technologies DTNFCxx](http://www.diganttechnologies.com/nfc_1.pdf)
- [ACS ACR122 API
 v1.0](http://www.nfc-reader.com/NFC-smart-card-reader/ACR122-API-Manual/API_ACR122.pdf), [ACS ACR122 API v1.2](http://www.acs.com.hk/download/ACR122/API_ACR122U.pdf),
- [ECMA-340 Near Field Communication
 Interface and Protocol (NFCIP-1)](http://www.ecma-international.org/publications/files/ECMA-ST/Ecma-340.pdf)
- [ARYGON
 NFC Reader / Module - Frequently Asked Questions v0.5](http://www.proxmark.org/files/index.php?dir=Documents%2FNFC%2F&download=ARYGON_FREQUENTLY_ASKED_QUESTIONS_AFAQ_05.pdf)
- [Java library to facilitate NFCIP (P2P) communication between
 NFC devices](http://code.google.com/p/nfcip-java/), [ACR122_PN53x.txt](http://code.google.com/p/nfcip-java/source/browse/trunk/nfcip-java/doc/ACR122_PN53x.txt)
- [A User Interaction Model
 for NFC Enabled Applications](http://rfid.cs.washington.edu/images/anokwa-nfcinteraction-pertec07.pdf)
[Categories](http://nfc-tools.org/index.php?title=Special:Categories): 
- [Hardware](http://nfc-tools.org/index.php?title=Category:Hardware)
- [Devices](http://nfc-tools.org/index.php?title=Category:Devices)
- [Log in / create
 account](http://nfc-tools.org/index.php?title=Special:UserLogin&returnto=PN53x)
- [Page](http://nfc-tools.org/index.php?title=PN53x)
- [Discussion](http://nfc-tools.org/index.php?title=Talk:PN53x&action=edit&redlink=1)
- [Read](http://nfc-tools.org/index.php?title=PN53x)
- [View
 source](http://nfc-tools.org/index.php?title=PN53x&action=edit)
- [View
 history](http://nfc-tools.org/index.php?title=PN53x&action=history)
[](http://nfc-tools.org/index.php?title=Main_Page)
##### Navigation
- [Main page](http://nfc-tools.org/index.php?title=Main_Page)
- [Community portal](http://www.libnfc.org/community)
- [Recent changes](http://nfc-tools.org/index.php?title=Special:RecentChanges)
- [Random page](http://nfc-tools.org/index.php?title=Special:Random)
- [Help](http://nfc-tools.org/index.php?title=Help:Contents)
##### Documentation
- [Hardware](http://nfc-tools.org/index.php?title=Category:Hardware)
- [Software](http://nfc-tools.org/index.php?title=Category:Software)
##### Misc
- [Sponsors](http://nfc-tools.org/index.php?title=Sponsors)
- [Contact us](http://nfc-tools.org/index.php?title=Contact)
- [Related links](http://nfc-tools.org/index.php?title=Related_links)
##### Toolbox
- [What links here](http://nfc-tools.org/index.php?title=Special:WhatLinksHere/PN53x)
- [Related changes](http://nfc-tools.org/index.php?title=Special:RecentChangesLinked/PN53x)
- [Special pages](http://nfc-tools.org/index.php?title=Special:SpecialPages)
- [Printable version](http://nfc-tools.org/index.php?title=PN53x&printable=yes)
- [Permanent link](http://nfc-tools.org/index.php?title=PN53x&oldid=706)
##### Partners
- [Car Lock Systems](http://www.carlocksystems.com/)
- [Jifeline Remote Diagnostics](http://www.jifeline.com/)
- 
This page was last modified on 12 May 2013, at 09:11.
- 
This page has been accessed 35,300 times.
- 
Content is available under [GNU Free Documentation License 1.3 or later](http://www.gnu.org/copyleft/fdl.html).
