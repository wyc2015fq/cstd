# bcd码 - 深之JohnChen的专栏 - CSDN博客

2018年12月15日 11:39:45[byxdaz](https://me.csdn.net/byxdaz)阅读数：523


        BCD码（Binary-Coded Decimal）亦称二进码十进数或二-十进制代码。用4位二进制数来表示1位十进制数中的0~9这10个数码。是一种二进制的数字编码形式，用二进制编码的十进制代码。BCD码这种编码形式利用了四个位元来储存一个十进制的数码，使二进制和十进制之间的转换得以快捷的进行。这种编码技巧最常用于会计系统的设计里，因为会计制度经常需要对很长的数字串作准确的计算。相对于一般的浮点式记数法，采用BCD码，既可保存数值的精确度，又可免去使电脑作浮点运算时所耗费的时间。此外，对于其他需要高精确度的计算，BCD编码亦很常用。

BCD码可分为有权码和无权码两类：有权BCD码有8421码、2421码、5421码，其中8421码是最常用的；无权BCD码有余3码，余3循环码等。

8421 BCD码是最基本和最常用的BCD码，它和四位自然二进制码相似，各位的权值为8、4、2、1，故称为有权BCD码。和四位自然二进制码不同的是，它只选用了四位二进制码中前10组代码，即用0000~1001分别代表它所对应的十进制数，余下的六组代码不用。

8421 BCD码与十进制转换代码

```
//bcd转十进制
unsigned char bcd_decimal(unsigned char bcd)
{
	return bcd - (bcd >> 4) * 6;
}
//十进制转bcd
unsigned char decimal_bcd(unsigned char decimal)
{
	return (unsigned char)(decimal + (decimal / 10) * 6);
}

//BCD转十进制
int BCDToInt(unsigned char bcd) 
{
	return (0xff & (bcd >> 4)) * 10 + (0xf & bcd);
}
//十进制转BCD
int IntToBCD(unsigned char decimal)
{
	int nDest = (decimal / 10) << 4;
	nDest += ((decimal % 10) & 0x0f);
	return nDest;
}
```

