# 关于16位色在arm cpu上的快速alpha算法 - xqhrs232的专栏 - CSDN博客
2012年10月26日 22:43:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：848
原文地址::[http://blog.csdn.net/pocketmatrix/article/details/3218107](http://blog.csdn.net/pocketmatrix/article/details/3218107)
相关网帖
1.alpha blend for rgb565----[http://hi.baidu.com/liuhuman/item/529650547f7aba9408be1734](http://hi.baidu.com/liuhuman/item/529650547f7aba9408be1734)
2.[](http://tctianchi.yo2.cn/articles/tag/AlphaBlend)ALPHABLEND
 FOR X86（无优化）----[http://tctianchi.yo2.cn/articles/alphablend-for-x86%EF%BC%88%E6%97%A0%E4%BC%98%E5%8C%96%EF%BC%89.html](http://tctianchi.yo2.cn/articles/alphablend-for-x86%EF%BC%88%E6%97%A0%E4%BC%98%E5%8C%96%EF%BC%89.html)
3.再谈ALPHABLEND，详解本次优化----[http://tctianchi.yo2.cn/articles/%E5%86%8D%E8%B0%88alphablend%EF%BC%8C%E8%AF%A6%E8%A7%A3%E6%9C%AC%E6%AC%A1%E4%BC%98%E5%8C%96.html](http://tctianchi.yo2.cn/articles/%E5%86%8D%E8%B0%88alphablend%EF%BC%8C%E8%AF%A6%E8%A7%A3%E6%9C%AC%E6%AC%A1%E4%BC%98%E5%8C%96.html)
4.ALPHABLEND FOR ARM----[http://tctianchi.yo2.cn/articles/alphablend-for-arm.html](http://tctianchi.yo2.cn/articles/alphablend-for-arm.html)

转自:[http://blog.chinaunix.net/u/26691/showart_1089631.html](http://blog.chinaunix.net/u/26691/showart_1089631.html)
最近在优化一个小程序，其中涉及到了alpha混合的代码，如果是在
台式机上可以通过mmx指令来加速，但是在arm cpu上mmx指令并不普及，所以
采用了一个小技巧来做这个alpha的优化。
alpha混合其实很简单，要做的只是分解源颜色c1，目的颜色c2，然后将颜色分
量r1,g1,b1和r2,g2,b2分别按照公式(clr*alpha+clr*(32-alpha))/32来计算，
最后再组合成一个颜色值即可，可是如此计算，运算量很大速度很慢。
所以现在就要用到一个技巧，首先，就16位色来说一般的格式是565,表示rgb
分量的二进制位数，如图1。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/pocketmatrix/EntryImages/20081104/080723133623.jpg)
那么我们就可以利用一个32位的变量来把这个颜色的绿色分量提前，变为如图2的格式。
这样每个颜色分量中间就有了进位的空间，也就不用分解这个颜色值了。
然后，将变形完的两个颜色值按照上面公式进行计算，计算完毕再变回565的格式
就完成了一次alpha混合的计算。
c语言源代码如下：
- __inline void MakeAlpha(WORD* wpSrc, WORD* wpDes, WORD wAlpha)
- {
-    register DWORD d1; // 计算用的中间变量，声明为寄存器变量快些
-    register WORD wa = *wpSrc; // 源颜色 
-    register WORD wb = *wpDes; // 目的颜色
-    register DWORD alpha = wAlpha; // alpha值，16位色的色深为32级，
- 
-                                   //故alpha取值0-32
- // (c1-c2)*alpha/32+c2 由(c1*alpha+c2*(32-alpha))/32变形
- 
- // 而来，减少了一次乘法运算
- // 下面的式子故意写成这样，编译器会把它处理的很好
- // 要比这样快一些
- // c1 = (((wa << 16) | wa) & 0x7e0f81f); 
- 
- // 16位变形32位 0x7e...f为二进制的00000111111000001111100000011111
- // c2 = (((wb << 16) | wb) & 0x7e0f81f); 
- // d1 = ((c1-c2)*alpha)>>5+c2; 
- 
- // 除以32等于右移5位，但是位移操作要比乘除法快的多，
- 
- // 例如：a*320可以写成a*256+a*64=>(a<<8)+(a<<6)
- 
-    d1 = (((((((wa << 16) | wa) & 0x7e0f81f) - (((wb << 16) | wb) & 0x7e0f81f)) * alpha) >> 5) + (((wb << 16) | wb) & 0x7e0f81f)) & 0x7e0f81f;
-         wa = (d1 & 0xffff0000)>>16; // g...r...b => ..g..
-         wb = d1 & 0xffff; // g...r...b => r...b
-         *wpDes = wa | wb; // rgb
- }
- 
我们用C语言写出来的乘法，编译器只能做部分优化，生成的汇编是由一些移位相加的循环组成的；有时数组元素的寻址也类似这样，所以除了用汇编改写之外也都有这样的优化空间。这个方法可以取得和汇编接近的计算效率，且和CPU无关，便于移植。
**原文地址**[http://blog.pdafans.com/?72643/viewspace-1056.html](http://blog.pdafans.com/?72643/viewspace-1056.html)
