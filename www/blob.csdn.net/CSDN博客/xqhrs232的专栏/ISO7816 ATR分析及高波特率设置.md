# ISO7816 ATR分析及高波特率设置 - xqhrs232的专栏 - CSDN博客
2018年07月21日 11:14:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：95
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[https://blog.csdn.net/zy1049677338/article/details/54945152](https://blog.csdn.net/zy1049677338/article/details/54945152)
相关文章
1、智能卡 7816协议----[https://blog.csdn.net/u013776495/article/details/70271884](https://blog.csdn.net/u013776495/article/details/70271884)
7816 ATR分析及高波特率设置：
ATR组成参考下表，
![](https://img-blog.csdn.net/20151117094311929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
TS:参考上表 通常使用3B。
T0:格式字符
高四位（B8-B5）分别对应接口字符TD1-TA1是否存在，可根据需要进行设定，当相应的bit为1时需添加对应的字符。低四位（B4-B1）为表示历史字节的个数（范围0-15），历史字节跟在接口字符后面。
![](https://img-blog.csdn.net/20151117094404034?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
TA1:高四位（B8-B5）为FI，低四位（B4-B1）为DI，
7816中的波特率使用etu来表示， 1etu =FI/DI*1/f ，其中f默认为3.579545MHz，FI和DI的值可以通过查询下表获得。
![](https://img-blog.csdn.net/20151117094444688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
例如：缺省值FI=1，DI=1则TA1=0x11，通过查表可以获得1etu=372/3579545(s), etu的单位为秒，对其取倒数相当于串口的bps， 1/1etu`=9600bps，
TB1: b1~b5确定最大编程电压，b6~b7确定最大编程电流，b8不使用。一般情况下此字节不使用，即将T0中的B6设为0。
TC1:用于设定从终端到IC卡的两相邻字符间的定时。
TD1:表示有无更多的接口字节传送，以及关于传输协议类型的信息。
根据7816 PPS的定义，可以调节TA1的值来设置波特率。
//11--74 01 11为ATR的第三个字节, 74写入IOBAUD0  01写入IOBAUD1  0x0174 = 372
//12--BA 00 0x00BA = 186
//13--5D 00 
//91--00 02
//92--00 01
//93--80 00
//94--40 00
//95--10 00   0x0010 = 16
参考ATR设置：3B 94 11 00 xx xx xx xx 
                               3B D4 9503 10 80 xx xx xx xx
