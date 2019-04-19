# 3GPP TS 0710 MUX协议 - xqhrs232的专栏 - CSDN博客
2014年06月06日 22:39:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：972
原文地址::[http://blog.csdn.net/codejoker/article/details/6205898](http://blog.csdn.net/codejoker/article/details/6205898)
相关文章
1、[MUX帧](http://blog.csdn.net/codejoker/article/details/6206132)----[http://blog.csdn.net/codejoker/article/details/6206132](http://blog.csdn.net/codejoker/article/details/6206132)
2、[说的比较通俗mux](http://blog.csdn.net/codejoker/article/details/6206246)----[http://blog.csdn.net/codejoker/article/details/6206246](http://blog.csdn.net/codejoker/article/details/6206246)
3、**3GPP协议阅读指南 ----[http://www.docin.com/p-88376337.html](http://www.docin.com/p-88376337.html)**
4、[linux 中用n_gsm实现3gpp MUX协议](http://blog.chinaunix.net/uid-27875-id-3261447.html) ----[http://blog.chinaunix.net/uid-27875-id-3261447.html](http://blog.chinaunix.net/uid-27875-id-3261447.html)
5、simcom gprs AT command CMUX 的使用 ----[http://bbs.csdn.net/topics/200052639](http://bbs.csdn.net/topics/200052639)
From: [http://hi.baidu.com/jasonzang1982/blog/item/060b0b3b5ad073f9b211c79d.html](http://hi.baidu.com/jasonzang1982/blog/item/060b0b3b5ad073f9b211c79d.html)
TE: terminal equipment
MS: mobile station
Allows a number of simultaneous sessions over a normal serial asynchronous interface.
1. TE通过物理通道发送AT+CMUX=0通知MS进入mux模式;
2. MS响应OK表示同意;
3. TE依次发送SAMB0,1,2,3,建立编号为0~3的逻辑通道;
4. MS响应UA表示同意;
5. TE在通道1上发送命令UIH(1,"AT/r/n");
6. MS也在通道1上响应UIH(1,"OK/r/n").
0710一共定义了6种包（不包括error recover mode）
1. Set Asynchronous Balanced Mode (SABM)--建立逻辑通道;
2. command Disconnect (DISC) command-取消逻辑通道;
3. Unnumbered Acknowledgement (UA) response-响应建立通道;
4. Disconnected Mode (DM) response-响应取消逻辑通道;
5. Unnumbered information with header check (UIH);
6. Unnumbered Information (UI).
0710支持三种模式: Basic,Advanced,Advance with error recovery.
Basic: Flag(1Byte) Adderss (1Byte) Control(1Byte) Length(1~2Byte) Info(Length指定长度,可变) FCS(1Byte) Flag(1Byte).
Flag固定为0xF9;
Adderss 逻辑通道编号,类似于MAC地址.bit1 为 E/A,bit2 为 C/R,bit3~8为逻辑通道号. E/A 为1表示当前字节是否是该项的最后一个字节,这里只能为1; C/R用来指示命令发送方是否是通道发起方,是则为1 如TE建立的逻辑通道1,往该通道上发命令包时,Adderss为0x7,MS响应Adderss也是0x7;
Control 为上面说的包类型,其中bit5为P/F,大致可以这么理解,SAMB,DISC包及其响应时,P/F为1(响应为DM时,该位可以是0或1);
Length 为Info项长度,bit定义依次为 E/A,L1,L2,...,L7,(L8,...,L15),长度小于128时,E/A为1,不编码第二个字节 Info 类容取决域包类型;
FCS 为校验码,UIH,UI包的区别就在这里,UIH不对Info项编码,只编码Adderss,Control,Length.
Advanced模式包格式为 Flag(1Byte) Adderss (1Byte) Control(1Byte) Info(不定长) FCS(1Byte) Flag(1Byte)
Flag固定为0x7E;
两种模式的区别就是Advanced模式没有Length一项,而是采取了特定格式编码,来决定包何时结束,发包方对Flag除外其他项编码,碰到0x7E,0x7D等字符则在其前插入0x7D,接受方去掉相应0x7D.这样碰到单独的0x7E则表示包的结束和开始.
//==========================================================================================================
备注::
1》MUX协议数据包格式举例
     //1111----0xf9----包头+包尾
     //2222----0X5F----往右移1位才是真正的数据包长度----47个数据
 COMR: read 61 bytes from Phsiy port success: 
f9( ) 07( ) ef( ) 5f(_) 7e(~) 21(!) 45(E) 00( ) 00( ) 28(() 01( ) 32(2) 40(@) 00( ) 7f( ) 06( ) 
84( ) c0( ) a8( ) 37(7) 65(e) c0( ) a8( ) 37(7) 64(d) c0( ) 00( ) 16( ) 2f(/) 00( ) 33(3) b9( ) 
49(I) 00( ) 33(3) 2f(/) e4( ) 50(P) 7d(}) 31(1) 82( ) 50(P) 7d(}) 5d(]) a5( ) 00( ) 00( ) 33(3)
2a(*) 7e(~) b1( ) f9( ) 
0d( ) 0a( ) 45(E) 52(R) 52(R) 4f(O) 52(R) 0d( ) 0a( ) 

