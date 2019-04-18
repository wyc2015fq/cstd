# UTF8,UTF16,UTF32,UTF16-LE,UTF16-BE,GBK 之间的转换 - 深之JohnChen的专栏 - CSDN博客

2008年05月11日 12:41:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3470


UTF8,UTF16,UTF32,UTF16-LE,UTF16-BE,GBK 之间的转换

  Unicode是Unicode.org制定的编码标准，目前得到了绝大部分操作系统和编程语言的支持。Unicode.org官方对Unicode的定义是：Unicode provides a unique number for every character。可见，Unicode所做的是为每个字符定义了一个相应的数字表示。比如，“a“的Unicode值是0x0061,“一”的Unicde值是0x4E00，这是最简单的情况，每个字符用2个字节表示。

        Unicode.org定义了百万个以上的字符，如果将所有的字符用统一的格式表示，需要的是4个字节。“a“的Unicode表示就会变成0x00000061，而“一“的Unicode值是0x00004E00。实际上，这就是UTF32，Linux操作系统上所使用的Unicode方案。

但是，仔细分析可以发现，其实绝大部分字符只使用2个字节就可以表示了。英文的Unicode范围是0x0000-0x007F，中文的Unicode范围是0x4E00-0x9F**，真正需要扩展到4个字节来表示的字符少之又少，所以有些系统直接使用2个字节来表示Unicode。比如Windows系统上，Unicode就是两个字节的。对于那些需要4个字节才能表示的字符，使用一种代理的手法来扩展(其实就是在低两个字节上做一个标记，表示这是一个代理，需要连接上随后的两个字节，才能组成一个字符)。这样的好处是大量的节约了存取空间，也提高了处理的速度。这种Unicode表示方法就是UTF16。一般在Windows平台上，提到Unicode,那就是指UTF16了。

至于UTF16-LE和UTF16-BE，与计算机的CPU构架有关。LE指Little Endian，而BE指Big Endian。关于这方面的信息，网上有很多相关的帖子。我们一般的X86系统都是Little Endian的，可以认为UTF16=UTF16-LE.

由于对于欧洲和北美，实际上使用的编码范围在0x0000-0x00FF之间，只需要一个字符就可以表示所有的字符。即使是使用UTF16来作为内存的存取方式，还是会带来巨大的空间浪费，因此就有了UTF8的编码方式。这是一种很灵活的编码，对于只需要1个字节的字符，就使用一个字节，对于中日韩等原本需要两个字节才能表示的字符，则通过一个UTF16－UTF8的算法实现相互之间的转换（一般需要3个字节才能表示），而对于需要4个字节才能表示的字符，UTF8可以扩展到6个字节每个字符。UTF8使用的算法很有意思，大致映射关系如下：
        UTF-32                                    UTF8
0x00000000 - 0x0000007F          0xxxxxxx 

         0x00000080 - 0x000007FF           110xxxxx 10xxxxxx

         0x00000800 - 0x0000FFFF          1110xxxx 10xxxxxx 10xxxxxx

        0x00010000 - 0x001FFFFF          11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

         0x00200000 - 0x03FFFFFF          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

         0x04000000 - 0x7FFFFFFF          1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
可以发现这和IP的分址算法很是相像。
由于UTF8可以方便的转换为UTF16和UTF32（不需要码表，转换算法可以在Unicode.org上找到[C代码](http://ftp//www.unicode.org/Public/PROGRAMS/CVTUTF/)）。而且UTF8在每个操作系统平台上的实现都是一样的，也不存在跨平台的问题，所以UTF8成为跨平台的Unicode很好的解决方案。当然，对于中文来说，由于每个字符需要3个字节才能表示，还是有点浪费的。

UTF8文本头为 EF BB BF

UTF16 文本头: Big-Endian的FEFF; 表明这个字节流是；Little-Endian的FFFE
` `` `
int convertUTF8UTF16(unsigned char* utf8, int& size8, char* utf16, int& size16)

{

    int count =0, i;

    char tmp1, tmp2;

    unsigned short int integer;

    unsigned short int *p;

    for(i=0;i<size8;i+=1)

    {   

        p = (unsigned short int*)&utf16[i];

        if( utf8[count] < 0x80)

        {

            // <0x80

            integer = utf8[count];           

            count++;

        }

        else if( (utf8[count] < 0xDF) && (utf8[count]>=0x80))

        {

            integer = utf8[count] & 0x1F;

            integer = integer << 6;

            integer += utf8[count+1] &0x3F;

            count+=2;

        }

        else if( (utf8[count] <= 0xEF) && (utf8[count]>=0xDF))

        {

            integer = utf8[count] & 0x0F;

            integer = integer << 6;

            integer += utf8[count+1] &0x3F;

            integer = integer << 6;

            integer += utf8[count+2] &0x3F;

            count+=3;

        }

        else

        {

            printf("error!/n");

        }

        *p = integer;

    }

    size8 = count;

    size16 = i;
` `` `
    return size16;

}
` `` `
int convertUTF16UTF8(char* utf16, int& size16, char* utf8, int& size8)

{

    int i=0, count=0;

    char tmp1, tmp2;

    unsigned short int integer;

    for(i=0;i<size16;i+=2)

    {   

        integer = *(unsigned short int*)&utf16[i];

        if( integer<0x80)

        {           

            utf8[count] = utf16[i] & 0x7f;

            count++;

        }

        else if( integer>=0x80 && integer<0x07ff)

        {

            tmp1 = integer>>6;

            utf8[count] = 0xC0 | (0x1F & integer>>6);

            utf8[count+1] = 0x80 | (0x3F & integer);

            count+=2;

        }

        else if( integer>=0x0800 )

        {           

            tmp1 = integer>>12;

            utf8[count] = 0xE0 | (0x0F & integer>>12);

            utf8[count+1] = 0x80 | ((0x0FC0 & integer)>>6);

            utf8[count+2] = 0x80 | (0x003F & integer);

            count += 3;

        }   

        else

        {

            printf("error/n");

        }

    }
` `` `
    size16 = i;

    size8  = count;

    return count;

}


**UTF-8 ****转 Unicode, Unicode ****转 GBK, UTF-8 ****转 GBK**

#include <windows.h>

#include <stdio.h>

void main() {

//三个不同版本的老徐

unsigned char utf8[] = "/xe8/x80/x81/xe5/xbe/x90";

unsigned char unicode[] = "/x01/x80/x90/x5f";

unsigned char ansi[] = "/xc0/xcf/xd0/xec";

int len;

//UTF-8转UNICODE

len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8, -1, NULL,0);

WCHAR * wszUtf8 = new WCHAR[len+1];

memset(wszUtf8, 0, len * 2 + 2);

MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8, -1, wszUtf8, len);

MessageBoxW(NULL, (const wchar_t*)wszUtf8, NULL, MB_OK);

//UNICODE转ANSI，实际上经过两次转换 UTF-8 已经变成了 GBK 编码

len = WideCharToMultiByte(CP_ACP, 0, wszUtf8, -1, NULL, 0, NULL, NULL);

char *szGBK=new char[len + 1];

memset(szGBK, 0, len + 1);

WideCharToMultiByte (CP_ACP, 0, wszUtf8, -1, szGBK, len, NULL,NULL);

MessageBoxA(NULL, (const char*)szGBK, NULL, MB_OK);

delete[] szGBK;

delete[] wszUtf8;

}

