
# java字符编码详解 - 李昆鹏的博客 - CSDN博客


2018年04月13日 11:15:55[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：58


--------------------------------------java字符编码详解----------------------------------------
存储：
在计算机中存储字符都是存储的字符所对应的数值以二进制的形式表示。
展示：
去相关的编码表中去查找该值（存储的值）所对应的字符。

常见的编码表：
ASCII表：
用7bit来表示存储数据。
ISO-8859-1：拉丁码表
用8bit来表示存储数据
GB2312：
简体中文编码（国标码）
**GBK****：（使用较多）**
**在GB2312****上做了增强。**
GB18030：
在GBK上做了增强
BIG5:
支持繁体简体。
Unicode:
支持多种国家的语言，这是国际标准。用两个字节来存储
不管是什么字符都用两个字节，会有浪费。
**UTF-8****：（使用较多）**
**支持多种国家的语言，针对不同的字符的范围给出不同的字节来表示**
**如：0, a, A****用一个字节**
**中文就是使用三个字节**

**写入的编码和读取的编码必须要保持一致，否则会乱码。**
范例：使用GBK的编码方式来写入文件和读取文件
**public****class**CharsetDemo {
**public****static****void**main(String[]args) {
//使用指定编码向文件写入数据
*printCharset*();
//我们是使用GBK编码写入文件的，所以读取时也要是GBK编码
//如果我们使用UTF-8编码来打开文件就会出现乱码情况
//所以我们在写文件和读取时的编辑一定要保持一致

//使用指定编码读取文件数据
*readCharset*();
//输出中国
}
/**
*使用指定编码写入文件
*/
**public****static****void**printCharset() {
OutputStreamWriterow=**null**;
**try**{
//创建字符输出转换流对象，并且指定编码
ow=**new**OutputStreamWriter(**new**FileOutputStream("2.txt"),"GBK");
//使用指定编码写入文件，需要处理IO异常
ow.write("中国");
}**catch**(UnsupportedEncodingExceptione) {
e.printStackTrace();
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(ow!=**null**)
ow.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}

/**
*使用指定编码读取文件
*/
**public****static****void**readCharset() {
InputStreamReaderiw=**null**;
**try**{
//创建字节输出转换流对象，并且指定编码
iw=**new**InputStreamReader(**new**FileInputStream("2.txt"),"GBK");
//创建一个字符数组
**char**[]chs=**new****char**[1024];
//把文件读取到数组中，返回读取的个数
**int**len=iw.read(chs);
//把字符数组转换成字符串
System.**out**.println(**new**String(chs,0,len));
}**catch**(UnsupportedEncodingExceptione) {
e.printStackTrace();
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(iw!=**null**)
iw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
----------------------------------------------------
**字符串编码问题**
|byte[]
|getBytes|(|String|charsetName)
|使用指定的字符集将此 String 编码为 byte 序列，并将结果存储到一个新的 byte 数组中。
|

**public****class**CharsetDemo2 {
**public****static****void**main(String[]args)**throws**UnsupportedEncodingException {
Stringstr="中国";
//返回默认编码字符数组
**byte**[]bsGBK=str.getBytes();
//返回指定编码字符数组
**byte**[]bsUTF=str.getBytes("UTF-8");
*printByte*(bsGBK);
System.**out**.println(**new**String(bsGBK));
//输出-42 -48  -71  -6中国
*printByte*(bsUTF);
System.**out**.println(**new**String(bsUTF));
//指定了编码编码，解码时也要是相同的编码
//输出-28  -72  -83 -27  -101  -67涓浗
//指定解码解码
*printByte*(bsUTF);
System.**out**.println(**new**String(bsUTF,"UTF-8"));
//输出-28  -72  -83 -27  -101  -67中国
}
/**
*打印字符数组方法
*/
**public****static****void**printByte(**byte**[]bs) {
**for**(**byte**b:bs) {
System.**out**.print(b+"  ");
}
}
}


