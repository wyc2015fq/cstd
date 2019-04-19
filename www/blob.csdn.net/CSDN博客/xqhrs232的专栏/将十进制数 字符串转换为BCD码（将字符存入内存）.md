# 将十进制数/字符串转换为BCD码（将字符存入内存） - xqhrs232的专栏 - CSDN博客
2013年06月28日 17:59:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：937
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.kaixinwenda.com/article-MPowell-8657770.html](http://www.kaixinwenda.com/article-MPowell-8657770.html)
需求：将十进制数按字面数值直接存入内容，如int nDec=12，要存入byte b中，存入后b对应内存地址显示0x12；
分析：如直接使用b = nDec，会按照ACSII码值存值，b内存地址显示0x0c，不符合要求；而需要的0x12为十进制值18，因此可以先对int nDec进行转换；
实现：
（1）针对十进制数
法一：先将int nDec=12转换为十六进制值，再存入byte b中；
**[cpp]**[view plain](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[copy](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[print](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[?](http://www.kaixinwenda.com/article-MPowell-8657770.html#)
- #define BUF_LEN     10 
- char szBuf[BUF_LEN] = {0};  // 中间字符串
- int nDec = 12;      // 原始十进制数据
- int nHex = 0;       // 转换的十六进制数据
- byte b = 0;     // 目标字符 
- 
- sprintf_s(szBuf, BUF_LEN, "%d", nDec);  // 十进制数据-->中间字符串
- sscanf_s(szBuf, "%x", &nHex);   // 中间字符串-->十六进制数据
- 
- b = nHex;           // 存入字符，获得结果
```cpp
#define BUF_LEN		10
	char szBuf[BUF_LEN] = {0};	// 中间字符串
	int nDec = 12;		// 原始十进制数据
	int nHex = 0;		// 转换的十六进制数据
	byte b = 0;		// 目标字符
	sprintf_s(szBuf, BUF_LEN, "%d", nDec);	// 十进制数据-->中间字符串
	sscanf_s(szBuf, "%x", &nHex);	// 中间字符串-->十六进制数据
	b = nHex;			// 存入字符，获得结果
```
法二：假设数字最多有6位，用3字节表示
**[cpp]**[view plain](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[copy](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[print](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[?](http://www.kaixinwenda.com/article-MPowell-8657770.html#)
- byte* itoBCD(byte* const buf, int n)  
- {  
- if (n < 0 || n > 999999)  
-     {  
- return NULL;  
-     }  
- 
-     buf[0] = ((n / 100000) << 4) & 0xF0;  
-     n %= 100000;  
-     buf[0] |= (n / 10000) & 0x0F;  
- 
-     n %= 10000;  
-     buf[1] = ((n / 1000) << 4) & 0xF0;  
-     n %= 1000;  
-     buf[1] |= (n / 100) & 0x0F;  
- 
-     n %= 100;  
-     buf[2] = ((n / 10) << 4) & 0xF0;  
-     n %= 10;  
-     buf[2] |= n & 0x0F;  
- 
- return buf;  
- }  
```cpp
byte* itoBCD(byte* const buf, int n)
{
	if (n < 0 || n > 999999)
	{
		return NULL;
	}
	buf[0] = ((n / 100000) << 4) & 0xF0;
	n %= 100000;
	buf[0] |= (n / 10000) & 0x0F;
	n %= 10000;
	buf[1] = ((n / 1000) << 4) & 0xF0;
	n %= 1000;
	buf[1] |= (n / 100) & 0x0F;
	n %= 100;
	buf[2] = ((n / 10) << 4) & 0xF0;
	n %= 10;
	buf[2] |= n & 0x0F;
	return buf;
}
```
假设数字最多有12位，用6字节表示
**[cpp]**[view plain](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[copy](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[print](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[?](http://www.kaixinwenda.com/article-MPowell-8657770.html#)
- byte* itoBCD_6Bit(byte* const buf, int n)  
- {  
- if (n < 0 || n > 999999)  
-     {  
- return NULL;  
-     }  
- 
-     buf[0] = 0x00;  
-     buf[0] |= (n / 100000) & 0x0F;  
- 
-     n %= 100000;  
-     buf[1] = 0x00;  
-     buf[1] |= (n / 10000) & 0x0F;  
- 
-     n %= 10000;  
-     buf[2] = 0x00;  
-     buf[2] |= (n / 1000) & 0x0F;  
- 
-     n %= 1000;  
-     buf[3] = 0x00;  
-     buf[3] |= (n / 100) & 0x0F;  
- 
-     n %= 100;  
-     buf[4] = 0x00;  
-     buf[4] |= (n / 10) & 0x0F;  
- 
-     n %= 10;  
-     buf[5] = 0x00;  
-     buf[5] |= (n / 1) & 0x0F;  
- 
- 
- return buf;  
- }  
```cpp
byte* itoBCD_6Bit(byte* const buf, int n)
{
	if (n < 0 || n > 999999)
	{
		return NULL;
	}
	buf[0] = 0x00;
	buf[0] |= (n / 100000) & 0x0F;
	n %= 100000;
	buf[1] = 0x00;
	buf[1] |= (n / 10000) & 0x0F;
	n %= 10000;
	buf[2] = 0x00;
	buf[2] |= (n / 1000) & 0x0F;
	n %= 1000;
	buf[3] = 0x00;
	buf[3] |= (n / 100) & 0x0F;
	n %= 100;
	buf[4] = 0x00;
	buf[4] |= (n / 10) & 0x0F;
	n %= 10;
	buf[5] = 0x00;
	buf[5] |= (n / 1) & 0x0F;
	return buf;
}
```
（2）针对字符串
使用时注意：在传入字符串前需先明确字符串最大长度，然后在左侧完成补0
**[cpp]**[view plain](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[copy](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[print](http://www.kaixinwenda.com/article-MPowell-8657770.html#)[?](http://www.kaixinwenda.com/article-MPowell-8657770.html#)
- void CharToBcd(char* Cbuf, byte* Bbuf, int* blen)  
- {  
- int i = 0;  
- int j = 0;  
- int tem_len = 0;  
-     byte tem_h = 0;  
-     byte tem_l = 0;  
- 
-     tem_len = (int)strlen(Cbuf);  
- 
- for (i=0; i<tem_len; i++)  
-     {  
- switch (Cbuf[i])  
-         {  
- case'0':  
- case'1':  
- case'2':  
- case'3':  
- case'4':  
- case'5':  
- case'6':  
- case'7':  
- case'8':  
- case'9':  
-             Cbuf[i] = ((Cbuf[i] - 0x30) & 0x0f);  
- break;  
- case'A':  
- case'a':  
-             Cbuf[i] = 0x0A;  
- break;  
- case'B':  
- case'b':  
-             Cbuf[i] = 0x0B;  
- break;  
- case'C':  
- case'c':  
-             Cbuf[i] = 0x0C;  
- break;  
- case'D':  
- case'd':  
-             Cbuf[i] = 0x0D;  
- break;  
- case'E':  
- case'e':  
-             Cbuf[i] = 0x0E;  
- break;  
- case'F':  
- case'f':  
-             Cbuf[i] = 0x0F;  
- break;  
- default:  
- break;  
-         }  
-     }  
- 
- if ((tem_len % 2) == 0)  
-     {  
- for (i=0,j=0; i<tem_len; i++,j++)  
-         {  
-             tem_h = (byte)(Cbuf[i]<<4) & 0xf0;  
-             tem_l = (byte)(Cbuf[++i] & 0x0f);  
-             Bbuf[j] = tem_h | tem_l;  
-         }  
-         *blen = (tem_len/2);  
-     }  
- else
-     {  
- if (tem_len == 1)  
-         {  
-             Bbuf[0] = (byte)(Cbuf[tem_len-1] & 0x0f);  
-             *blen = tem_len;  
-         }  
- else
-         {  
- for (i=0,j=0; i<tem_len-1; i++,j++)  
-             {  
-                 tem_h = (byte)(Cbuf[i]<<4) & 0xf0 ;  
-                 tem_l = (byte)(Cbuf[++i] & 0x0f);  
-                 Bbuf[j] = tem_h | tem_l;  
-             }  
-             Bbuf[tem_len/2] = (byte)(Cbuf[tem_len-1] & 0x0f);  
-             *blen = (tem_len/2)+1;  
-         }  
-     }  
- }  
```cpp
void CharToBcd(char* Cbuf, byte* Bbuf, int* blen)
{
	int i = 0;
	int j = 0;
	int tem_len = 0;
	byte tem_h = 0;
	byte tem_l = 0;
	tem_len = (int)strlen(Cbuf);
	for (i=0; i<tem_len; i++)
	{
		switch (Cbuf[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			Cbuf[i] = ((Cbuf[i] - 0x30) & 0x0f);
			break;
		case 'A':
		case 'a':
			Cbuf[i] = 0x0A;
			break;
		case 'B':
		case 'b':
			Cbuf[i] = 0x0B;
			break;
		case 'C':
		case 'c':
			Cbuf[i] = 0x0C;
			break;
		case 'D':
		case 'd':
			Cbuf[i] = 0x0D;
			break;
		case 'E':
		case 'e':
			Cbuf[i] = 0x0E;
			break;
		case 'F':
		case 'f':
			Cbuf[i] = 0x0F;
			break;
		default:
			break;
		}
	}
	if ((tem_len % 2) == 0)
	{
		for (i=0,j=0; i<tem_len; i++,j++)
		{
			tem_h = (byte)(Cbuf[i]<<4) & 0xf0;
			tem_l = (byte)(Cbuf[++i] & 0x0f);
			Bbuf[j] = tem_h | tem_l;
		}
		*blen = (tem_len/2);
	}
	else
	{
		if (tem_len == 1)
		{
			Bbuf[0] = (byte)(Cbuf[tem_len-1] & 0x0f);
			*blen = tem_len;
		}
		else
		{
			for (i=0,j=0; i<tem_len-1; i++,j++)
			{
				tem_h = (byte)(Cbuf[i]<<4) & 0xf0 ;
				tem_l = (byte)(Cbuf[++i] & 0x0f);
				Bbuf[j] = tem_h | tem_l;
			}
			Bbuf[tem_len/2] = (byte)(Cbuf[tem_len-1] & 0x0f);
			*blen = (tem_len/2)+1;
		}
	}
}
```
//=================================================
是一种用二进制编码的十进制数，又称二一十进制数。它
是用4位二进制效表示一个十进制数码的，由于这4位二进制数的权为8421，所以跃9
码又称8421码。
如1234的BCD码是：0001，0010，0011，0100

