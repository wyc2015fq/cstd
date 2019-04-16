# Base64编码介绍及基于Java语言实现 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2010年11月27日 22:59:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：4585标签：[java																[byte																[string																[encoding																[output																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[网络协议研究](https://blog.csdn.net/jia20003/article/category/871634)





Base64编码介绍及基于Java语言实现



Base64编码被设计用来表示任意顺序的八位一个字节单元，允许使用大写和小写字符而不要求内容可读。65个US-ASCII子集被用来表示Base64编码，所以每六位表示一个可以打印的Base64的字符，第65字符’=’被用来表示特殊处理的功能字符，后面会详细说明。



预编码处理一次输入24位（三个字符）作为一组，输出的是四个Base64的编码字符24位（每个字符六位）。编码处理遵循从左到右的顺序，一个24位的输入可以看作是三个八位的字符组，

编码以后，24位的字符则被转换为四个6位的字符组，每个字符被翻译成Base64字符表中的匹配的一个字符。



每六位都被转换成为一个index数值去匹配Base64编码表中的一个字符输出，Base64的字符表如下：



Value Encoding Value Encoding Value Encoding Value Encoding

0 A17 R 34 i51 z

1 B18 S 35 j 52 0

2 C19 T 36 k53 1

3 D20 U 37 l 54 2

4 E21 V 38 m 55 3

5 F22 W 39 n 56 4

6 G23 X 40 o 57 5

7 H24 Y 41 p 58 6

8 I25 Z 42 q 59 7

9 J26 a 43 r 60 8

10 K27 b 44 s 61 9

11 L28 c 45 t 62 +

12 M29 d 46 u 63 /

13 N30 e 47 v

14 O31 f 48 w (pad) =

15 P 32 g49 x

16 Q33 h 50 y







字符’=’作为填充字符，对于任何不满足24为输入的编码字符，以零做作为填充成为24位（三字节）输入，对于任何输出的编码字符小于24位的都应当以’=’字符填充。对于以下情形：



Ø最后输入正好是24位的编码字符，则不需要’=’字符填充，输出即为四个Base64字符编码.

Ø最后输入只有八位的编码字符，则需要填充两个特殊字符’=’，输出即为两个Base64字符编码加上两个’=’.

Ø最后输入只有十六位的编码字符，则需要填充一个特殊字符’=’，输出即为三个Base64字符加上一个’=’.



把一个二进制的数据转换为Base64的编码，下面的例子就说明Base64怎么编码三个字节的二进制数据的。

+--first octet--+-second octet--+--third octet--+

|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|

+-----------+---+-------+-------+---+-----------+

|5 4 3 2 1 0|5 4 3 2 1 0|5 4 3 2 1 0|5 4 3 2 1 0|

+--1.index--+--2.index--+--3.index--+--4.index--+



许多网络程序中用来传输二进制文件也经常采用Base64的编码算法，一个最典型的例子就是Email的应用协议SMPT发送MIME数据的时候就常采用Base64来发送，而且已经被写入标准的RFC中，在Java语言中，JDK已经提供了Base64的API。



下面基于Java实现的Base64的编码和解码程序，相比于JDK6中把Base64编码和解码分在两个不同的API
 Class中，因此还把许多人搞得很晕来说，也许本人的实现会略微方便点。



```java
package com.gloomyfish.smtp.util;


public class Base64Coder {
	
	public final static char[] base64_alphabet = new char[]{
            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S'
            ,'T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l'
            ,'m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4'
            ,'5','6','7','8','9','+','/','='
	};
	
	public static String encode(String content) {
		
		byte[] data = content.getBytes();
		int length = data.length;
		byte[] char_array_3 = new byte[]{0, 0, 0};
		byte[] char_array_4 = new byte[]{'=','=','=','='};
		String retContent = "";
		int i = 0;
		int j = 0;
		int reversePos = 0;
		while(length > 0) {
			length--;
			char_array_3[i++] = data[reversePos++];
			if(i==3) {
				char_array_4[0] = (byte)((char_array_3[0] & 0xfc) >> 2); // convert the char
			    char_array_4[1] = (byte)(((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4));
			    char_array_4[2] = (byte)(((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6));
			    char_array_4[3] = (byte)(char_array_3[2] & 0x3f);
			    for(i = 0; (i <4) ; i++)
			    	retContent += base64_alphabet[char_array_4[i]];
			    i = 0;
			}
		}
		
		// handling the last input content
		  if (i > 0 )
		  {
		    for(j = i; j < 3; j++)
		      char_array_3[j] = 0; // padding of zero

		    char_array_4[0] = (byte)((char_array_3[0] & 0xfc) >> 2); // right shift
		    char_array_4[1] = (byte)(((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4));
		    char_array_4[2] = (byte)(((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6));
		    char_array_4[3] = (byte)(char_array_3[2] & 0x3f);

		    for (j = 0; (j < i + 1); j++)
		    	retContent += base64_alphabet[char_array_4[j]];

		    while((i++ < 3)) // padding of '=' of output string
		    	retContent += '=';

		  }
		return retContent;
	}
	
	public static String decode(String enContent) {
		byte[] data = enContent.getBytes();
		int i = 0, j = 0, enCode = 0;
		int mLength = data.length;
		byte[] char_array_4 = new byte[4];
		byte[] char_array_3 = new byte[3];
		String retContent = "";

		// filter out the padding '=' chars
		  while (mLength > 0 && (((char)data[enCode]) != '=') && isBase64((char)data[enCode])) 
			{
			  mLength--;
			  char_array_4[i++] = data[enCode++];
			  if (i ==4) {
		      for (i = 0; i <4; i++)
		        char_array_4[i] = findChar((char)char_array_4[i]);

		      char_array_3[0] = (byte)((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
		      char_array_3[1] = (byte)(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
		      char_array_3[2] = (byte)(((char_array_4[2] & 0x3) << 6) + char_array_4[3]);

		      for (i = 0; (i < 3); i++)
		    	  retContent += (char)char_array_3[i];
		      i = 0;
		    }
		  }

		  // last content handling
		  if (i > 0) 
			{
		    for (j = i; j <4; j++)
		      char_array_4[j] = 0;

		    for (j = 0; j <4; j++)
		      char_array_4[j] = findChar((char)char_array_4[j]);

		    char_array_3[0] = (byte)((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
		    char_array_3[1] = (byte)(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
		    char_array_3[2] = (byte)(((char_array_4[2] & 0x3) << 6) + char_array_4[3]);

		    for (j = 0; (j < i - 1); j++) 
		    	retContent += (char)char_array_3[j];
		  }

		  return retContent;
	}
	
	public static boolean isBase64(char c) 
	{
		boolean base64 = false;
		for(int i=0; i<64; i++) {
			if( c == base64_alphabet[i]) {
				base64 = true;
				break;
			}
		}
	  return base64;
	}
	
	public static byte findChar(char x) {
		byte index = 64; // 65th char '='
		for(int i=0; i<64; i++) {
			if( x == base64_alphabet[i]) {
				index = (byte)i;
				break;
			}
		}
		return index;
	}
	
	/**
	 * <p> test data and result should like below output , RFC4648 Sample </p>
	 * 	BASE64("") = ""
	 *	BASE64("f") = "Zg=="
	 *	BASE64("fo") = "Zm8="
	 *	BASE64("foo") = "Zm9v"
	 *	BASE64("foob") = "Zm9vYg=="
	 *	BASE64("fooba") = "Zm9vYmE="
	 *	BASE64("foobar") = "Zm9vYmFy"
	 *
	 *
	 * @param args
	 */
	public static void main(String[] args) {
		// BASE64Encoder coder = new BASE64Encoder();
		// System.out.println(coder.encode("foobar".getBytes()));
		
		System.out.println("#--------------encode---------------#");
		System.out.println(encode(""));
		System.out.println(encode("f"));
		System.out.println(encode("fo"));
		System.out.println(encode("foo"));
		System.out.println(encode("foob"));
		System.out.println(encode("fooba"));
		System.out.println(encode("foobar"));
		System.out.println(encode("123456789sS{1}quot;));
		System.out.println("#--------------decode---------------#");
		System.out.println(decode(""));
		System.out.println(decode("Zg=="));
		System.out.println(decode("Zm8="));
		System.out.println(decode("Zm9v"));
		System.out.println(decode("Zm9vYg=="));
		System.out.println(decode("Zm9vYmE="));
		System.out.println(decode("Zm9vYmFy"));
		System.out.println(decode("MTIzNDU2Nzg5c1Mk"));
		
	}

}
```](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




