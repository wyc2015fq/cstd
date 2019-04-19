# YUV422/420 format - maopig的专栏 - CSDN博客
2012年09月11日 20:03:58[maopig](https://me.csdn.net/maopig)阅读数：2416
(在本文中，U 一词相当于 Cb，V 一词相当于 Cr。)
YUV422 format as shown below
4:2:2 表示 2:1 的水平取样，没有垂直下采样
![](https://img-my.csdn.net/uploads/201209/11/1347365269_5529.JPG)
YUV420 format as shown below
4:2:0 表示 2:1 的水平取样，2:1 的垂直下采样. YUV4:2:0并不是说只有U（即 Cb）, V（即 Cr）一定为 0，而是指U：V互相援引，时见时隐，也就是说对于每一个行，只有一个U或者V份量，如果一行是4:2:0的话，下一行就是4:0:2，再下一行是4:2:0...以此类推。
![](https://img-my.csdn.net/uploads/201209/11/1347365274_6837.JPG)
YUV411 format
4:1:1 表示 4:1 的水平取样，没有垂直下采样
![](https://img-my.csdn.net/uploads/201209/11/1347365282_1187.JPG)
