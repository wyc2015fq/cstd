# JAVA 十六进制与字符串的转换 - z69183787的专栏 - CSDN博客
2015年11月26日 17:08:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2908
toHexString 
public static String toHexString(int i)以十六进制的无符号整数形式返回一个整数参数的字符串表示形式。 
如果参数为负，那么无符号整数值为参数加上 232；否则等于该参数。将该值转换为十六进制（基数 16）的无前导 0 的 ASCII 数字字符串。如果无符号数的大小值为零，则用一个零字符 '0' ('\u0030') 表示它；否则，无符号数大小的表示形式中的第一个字符将不是零字符。用以下字符作为十六进制数字： 
0123456789abcdef 
这些字符的范围是从 '\u0030' 到 '\u0039' 和从 '\u0061' 到 '\u0066'。如果希望得到大写字母，可以在结果上调用 String.toUpperCase() 方法： 
Integer.toHexString(n).toUpperCase() 
参数： 
i - 要转换成字符串的整数。 
返回： 
用十六进制（基数 16）参数表示的无符号整数值的字符串表示形式。 
// 转化字符串为十六进制编码 
public static String toHexString(String s) 
{ 
String str=""; 
for (int i=0;i<s.length();i++) 
{ 
int ch = (int)s.charAt(i); 
String s4 = Integer.toHexString(ch); 
str = str + s4; 
} 
return str; 
} 
// 转化十六进制编码为字符串 
public static String toStringHex(String s) 
{ 
byte[] baKeyword = new byte[s.length()/2]; 
for(int i = 0; i < baKeyword.length; i++) 
{ 
try 
{ 
baKeyword[i] = (byte)(0xff & Integer.parseInt(s.substring(i*2, i*2+2),16)); 
} 
catch(Exception e) 
{ 
e.printStackTrace(); 
} 
} 
try 
{ 
s = new String(baKeyword, "utf-8");//UTF-16le:Not 
} 
catch (Exception e1) 
{ 
e1.printStackTrace(); 
} 
return s; 
} 
// 转化十六进制编码为字符串 
public static String toStringHex(String s) 
{ 
byte[] baKeyword = new byte[s.length()/2]; 
for(int i = 0; i < baKeyword.length; i++) 
{ 
try 
{ 
baKeyword[i] = (byte)(0xff & Integer.parseInt(s.substring(i*2, i*2+2),16)); 
} 
catch(Exception e) 
{ 
e.printStackTrace(); 
} 
} 
try 
{ 
s = new String(baKeyword, "utf-8");//UTF-16le:Not 
} 
catch (Exception e1) 
{ 
e1.printStackTrace(); 
} 
return s; 
} 
public static void main(String[] args) { 
System.out.println(encode("中文")); 
System.out.println(decode(encode("中文"))); 
} 
/* 
* 16进制数字字符集 
*/ 
private static String hexString="0123456789ABCDEF"; 
/* 
* 将字符串编码成16进制数字,适用于所有字符（包括中文） 
*/ 
public static String encode(String str) 
{ 
//根据默认编码获取字节数组 
byte[] bytes=str.getBytes(); 
StringBuilder sb=new StringBuilder(bytes.length*2); 
//将字节数组中每个字节拆解成2位16进制整数 
for(int i=0;i<bytes.length;i++) 
{ 
sb.append(hexString.charAt((bytes[i]&0xf0)>>4)); 
sb.append(hexString.charAt((bytes[i]&0x0f)>>0)); 
} 
return sb.toString(); 
} 
/* 
* 将16进制数字解码成字符串,适用于所有字符（包括中文） 
*/ 
public static String decode(String bytes) 
{ 
ByteArrayOutputStream baos=new ByteArrayOutputStream(bytes.length()/2); 
//将每2位16进制整数组装成一个字节 
for(int i=0;i<bytes.length();i+=2) 
baos.write((hexString.indexOf(bytes.charAt(i))<<4 |hexString.indexOf(bytes.charAt(i+1)))); 
return new String(baos.toByteArray()); 
} 
第二种方法：
将指定byte数组以16进制的形式打印到控制台
复制代码代码如下:
package com.nantian.iclient.atm.sdb; 
public class Util { 
public Util() { 
} 
/** 
* 将指定byte数组以16进制的形式打印到控制台 
* @param hint String 
* @param b byte[] 
* @return void 
*/ 
public static void printHexString(String hint, byte[] b) { 
System.out.print(hint); 
for (int i = 0; i < b.length; i++) { 
String hex = Integer.toHexString(b[i] & 0xFF); 
if (hex.length() == 1) { 
hex = '0' + hex; 
} 
System.out.print(hex.toUpperCase() + " "); 
} 
System.out.println(""); 
} 
/** 
* 
* @param b byte[] 
* @return String 
*/ 
public static String Bytes2HexString(byte[] b) { 
String ret = ""; 
for (int i = 0; i < b.length; i++) { 
String hex = Integer.toHexString(b[i] & 0xFF); 
if (hex.length() == 1) { 
hex = '0' + hex; 
} 
ret += hex.toUpperCase(); 
} 
return ret; 
} 
/** 
* 将两个ASCII字符合成一个字节； 
* 如："EF"--> 0xEF 
* @param src0 byte 
* @param src1 byte 
* @return byte 
*/ 
public static byte uniteBytes(byte src0, byte src1) { 
byte _b0 = Byte.decode("0x" + new String(new byte[]{src0})).byteValue(); 
_b0 = (byte)(_b0 << 4); 
byte _b1 = Byte.decode("0x" + new String(new byte[]{src1})).byteValue(); 
byte ret = (byte)(_b0 ^ _b1); 
return ret; 
} 
/** 
* 将指定字符串src，以每两个字符分割转换为16进制形式 
* 如："2B44EFD9" --> byte[]{0x2B, 0x44, 0xEF, 0xD9} 
* @param src String 
* @return byte[] 
*/ 
public static byte[] HexString2Bytes(String src){ 
byte[] ret = new byte[8]; 
byte[] tmp = src.getBytes(); 
for(int i=0; i<8; i++){ 
ret[i] = uniteBytes(tmp[i*2], tmp[i*2+1]); 
} 
return ret; 
} 
} 
