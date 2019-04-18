# base64简介与代码实现 - 深之JohnChen的专栏 - CSDN博客

2008年05月31日 23:10:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2569


base64简介

Base64是网络上最常见的用于传输8Bit字节代码的编码方式之一，大家可以查看RFC2045～RFC2049，上面有MIME的详细规范。

Base64要求把每三个8Bit的字节转换为四个6Bit的字节（3*8 = 4*6 = 24），然后把6Bit再添两位高位0，组成四个8Bit的字节，也就是说，转换后的字符串理论上将要比原来的长1/3。

这样说会不会太抽象了？不怕，我们来看一个例子：

转换前 aaaaaabb ccccdddd eeffffff   

转换后 00aaaaaa 00bbcccc 00ddddee 00ffffff

应该很清楚了吧？上面的三个字节是原文，下面的四个字节是转换后的Base64编码，其前两位均为0。

转换后，我们用一个码表来得到我们想要的字符串（也就是最终的Base64编码），这个表是这样的：（摘自RFC2045）

                            Table 1: The Base64 Alphabet

      Value Encoding  Value Encoding  Value Encoding  Value Encoding

           0 A            17 R            34 i            51 z

           1 B            18 S            35 j            52 0

           2 C            19 T            36 k            53 1

           3 D            20 U            37 l            54 2

           4 E            21 V            38 m            55 3

           5 F            22 W            39 n            56 4

           6 G            23 X            40 o            57 5

           7 H            24 Y            41 p            58 6

           8 I            25 Z            42 q            59 7

           9 J            26 a            43 r            60 8

          10 K            27 b            44 s            61 9

          11 L            28 c            45 t            62 +

          12 M            29 d            46 u            63 /

          13 N            30 e            47 v

          14 O            31 f            48 w         (pad) =

          15 P            32 g            49 x

          16 Q            33 h            50 y

让我们再来看一个实际的例子，加深印象！

转换前 10101101 10111010 01110110   

转换后 00101011 00011011 00101001 00110110

十进制 43 27 41 54

对应码表中的值 r b p 2

所以上面的24位编码，编码后的Base64值为 rbp2

解码同理，把 rbq2 的二进制位连接上再重组得到三个8位值，得出原码。

（解码只是编码的逆过程，在此我就不多说了，另外有关MIME的RFC还是有很多的，如果需要详细情况请自行查找。）

用更接近于编程的思维来说，编码的过程是这样的：

第一个字符通过右移2位获得第一个目标字符的Base64表位置，根据这个数值取到表上相应的字符，就是第一个目标字符。

然后将第一个字符左移4位加上第二个字符右移4位，即获得第二个目标字符。

再将第二个字符左移2位加上第三个字符右移6位，获得第三个目标字符。

最后取第三个字符的右6位即获得第四个目标字符。

在以上的每一个步骤之后，再把结果与 0x3F 进行 AND 位操作，就可以得到编码后的字符了。

可是等等……聪明的你可能会问到，原文的字节数量应该是3的倍数啊，如果这个条件不能满足的话，那该怎么办呢？

我们的解决办法是这样的：原文的字节不够的地方可以用全0来补足，转换时Base64编码用=号来代替。这就是为什么有些Base64编码会以一个或两个等号结束的原因，但等号最多只有两个。因为：

余数 = 原文字节数 MOD 3

所以余数任何情况下都只可能是0，1，2这三个数中的一个。如果余数是0的话，就表示原文字节数正好是3的倍数（最理想的情况啦）。如果是1的话，为了让Base64编码是4的倍数，就要补2个等号；同理，如果是2的话，就要补1个等号。

在线转换:http://www1.tc711.com/tool/BASE64.htm

base64编码与解码工具有很多,这里推荐eCrypt软件,可以从[http://sourceforge.net/project/showfiles.php?group_id=190563&package_id=223563&release_id=490837](http://sourceforge.net/project/showfiles.php?group_id=190563&package_id=223563&release_id=490837)中下载软件和源代码。

base64编解码类

base64.h

```cpp
#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);
```

base64.cpp

```cpp
#include "base64.h"
#include <iostream>

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) 
{
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) 
{
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}
```

