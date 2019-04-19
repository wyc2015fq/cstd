# Java卡应用开发其实并不难（4）-第一个Java卡应用的调试 - xqhrs232的专栏 - CSDN博客
2017年07月01日 18:32:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：785
原文地址::[http://blog.sina.com.cn/s/blog_4df8400a0100cty2.html](http://blog.sina.com.cn/s/blog_4df8400a0100cty2.html)
ava卡应用开发其实并不难（4）
第一个Java卡应用的调试
在完成调试配置之后，可以选择“RUN”,这样Jcop
 shell就会产生如下的调试信息。
- /term
 "Remote|localhost:4229"
--Opening terminal
> /card
 -a a000000003000000 -c
resetCard with timeout: 0 (ms)
--Waiting for card...
ATR=3B FA 13 00 00 81 31 FE 45 4A 43 4F 50 34 31 5632
 32 31 96
ATR: T=1, FI=1/DI=3 (93clk/etu), N=0, IFSC=254, BWI=4/CWI=5,
 Hist="JCOP41V221"
=>
 00 A4 04 00 08 A0 00 00 00 03 00 00 00 00
(911848
 nsec)
<=
 6F 10 84 08 A0 00 00 00 03 00 00 00 A5 04 9F 65
 01
 FF 90 00
Status: No Error
cm> set-key
 255/1/DES-ECB/404142434445464748494a4b4c4d4e4f 255/2/DES-ECB/404142434445464748494a4b4c4d4e4f 255/3/DES-ECB/404142434445464748494a4b4c4d4e4f
cm> init-update
 255
=>
 80 50 00 00 08 D3 08 AF 4A 31 95 39 2F 00
(1621
 usec)
<=
 00 00 57 F3 97 D9 7F 72 88 55 FF 02 00 00 3D 02
 9C
 31 C7 89 98 CD 8C C6 68 87 61 A8 90 00
Status: No Error
cm> ext-auth
 plain
=>
 84 82 00 00 10 16 F5 C8 3C 4E 9B 72 A0 84 D2 01
 66
 9B E1 25 47
(1381
 usec)
<=
 90 00
Status: No Error
cm> delete
 a00000000901
=>
 80 E4 00 00 08 4F 06 A0 00 00 00 09 01 00
(611251
 nsec)
<=
 6A 88
Status: Reference data not found
jcshell: Error code: 6a88 (Reference data not found)
jcshell: Wrong response APDU: 6A88
Ignoring expected error
cm> delete
 a0000000090102
=>
 80 E4 00 00 09 4F 07 A0 00 00 00 09 01 02 00
(684724
 nsec)
<=
 6A 88
Status: Reference data not found
jcshell: Error code: 6a88 (Reference data not found)
jcshell: Wrong response APDU: 6A88
Ignoring expected error
cm> upload
 -b 250 "D:\Eclipse_32 Work Place\testproject\bin\testpackage\javacard"
=>
 80 E6 02 00 14 07 A0 00 00 00 09 01 02 08 A0 00
 00
 00 03 00 00 00 00 00 00 00
(1537
 usec)
<=
 00 90 00
Status: No Error
=>
 80 E8 80 00 EF C4 81 EC 01 00 1D DE CA FF ED 02 ................
 02
 04 00 01 07 A0 00 00 00 09 01 02 0B 74 65 73
 74
 70 61 63 6B 61 67 65 02 00 21 00 1D 00 21 00 tpackage..!...!.
 0A
 00 0B 00 1E 00 0E 00 3D 00 0A 00 0B 00 00 00 ........=.......
 4D
 01 BB 00 00 00 00 00 00 01 01 00 04 00 0B 01 M...............
 02
 01 07 A0 00 00 00 62 01 01 03 00 0A 01 06 A0 .......b........
 00
 00 00 09 01 00 08 06 00 0E 00 00 00 80 03 00 ................
 FF
 00 07 01 00 00 00 1C 07 00 3D 00 01 10 18 8C ..........=.....
 00
 01 7A 05 30 8F 00 04 3D 8C 00 05 18 1D 04 41 ..z.0...=......A
 18
 1D 25 8B 00 03 7A 02 21 18 8B 00 02 60 03 7A ..%...z.!....`.z
 19
 8B 00 00 2D 1A 04 25 73 00 09 00 00 00 00 00 .......
 0F
 11 6D 00 8D 00 06 7A 08 00 0A 00 00 00 00 00 ..m....z........
 00
 00 00 00 00 05 00 1E 00 07 03 80 0A 01 06 80 ................
 03
 00 03 80 03 03 03 80 03 02 01 00 02 00 06 00 ................
 00
 01 06 80 07 01 09 00 0B 00 00 00 07 05 06 04 ................
 0A
 07 07 13 00.....
(9501
 usec)
<=
 00 90 00 ...
Status: No Error
cm> install
 -i a00000000901 -q C9#() a0000000090102 a00000000901
=>
 80 E6 0C 00 1C 07 A0 00 00 00 09 01 02 06 A0 00
 00
 00 09 01 06 A0 00 00 00 09 01 01 00 02 C9 00
 00
 00 ..
(2800
 usec)
<=
 90 00 ..
Status: No Error
Load report:
 239
 bytes loaded in 0.0 seconds
 effective
 code size on card:
 +
 package AID 7
 +
 applet AIDs 13
 +
 classes17
 +
 methods 64
 +
 statics 0
 +
 exports 0
 ------------------------------
 overall 101 bytes
cm> install
 -i a00000000901 -q C9#() a0000000090102 a00000000901
=>
 80 E6 0C 00 1C 07 A0 00 00 00 09 01 02 06 A0 00 ................
 00
 00 09 01 06 A0 00 00 00 09 01 01 00 02 C9 00 ................
 00
 00 ..
(2800
 usec)
<=
 90 00 ..
Status: No Error
