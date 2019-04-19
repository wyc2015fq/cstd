# Gzip压缩减少网络通信 - 三少GG - CSDN博客
2014年01月24日 01:11:58[三少GG](https://me.csdn.net/scut1135)阅读数：1807
** 核心内容：**
**Question:**
Gzip format files (created with the `gzip` program,
 for example) use the "deflate" compression algorithm, which is the same compression algorithm as what [zlib](http://www.zlib.net/) uses.
 However, when using zlib to inflate a gzip compressed file, the library returns a `Z_DATA_ERROR`.
How can I use zlib to decompress a gzip file?
**Answer:**
To decompress a gzip format file with zlib, call `inflateInit2` with
 the `windowBits` parameter
 as`16+MAX_WBITS`,
 like this:
```
inflateInit2(&stream, 16+MAX_WBITS);
```
If you don't do this, zlib will complain about a bad stream format. By default, zlib creates streams with a zlib header, and on inflate does not recognise the different gzip header unless you tell it so. Although this is documented starting in version 1.2.1
 of the `zlib.h` header
 file, it is not in the [zlib
 manual](http://www.zlib.net/manual.html#inflateInit2). From the header file:
> 
`windowBits` can
 also be greater than 15 for optional gzip decoding. Add 32 to `windowBits` to
 enable zlib and gzip decoding with automatic header detection, or add 16 to decode only the gzip format (the zlib format will return a `Z_DATA_ERROR`).
 If a gzip stream is being decoded, `strm->adler` is
 a crc32 instead of an adler32.
**核心内容**
[gzip压缩的数据如何在内存中解压](http://bbs.chinaunix.net/thread-1361609-1-1.html)
内存中解压压缩数据只有ZLIB，GZIP不可以。
使用zlib实现gzip格式数据的压缩和解压 [http://www.chengxuyuans.com/code/C++/65459.html](http://www.chengxuyuans.com/code/C++/65459.html)**核心内容**
**[http://www.oschina.net/code/piece_full?code=22542](http://www.oschina.net/code/piece_full?code=22542)**
**Question: [http://bbs.csdn.net/topics/350183493](http://bbs.csdn.net/topics/350183493)**
现在正在学习用zlib库解压抓来的HTTP包（gzip压缩包）
我看了各位老鸟的文档或是代码，有些了解.
但我有地方始终不明白:
1.解压gzip数据包需要用inflateInit2（z_streamp strm,int windowBits）进行初始化，然后再用inflate（）进行解压。
问题就在这里，inflateInit2的第二个参数到底是什么意思（请详细解释下，谢了）；
我自己写了个gzip压缩数据,我用inflateInit2进行初始化解压时，第二个参数，设为-MAX_WBITS就OK 但设为47就不好用..
(但是我看各位老鸟都是设定的47，具体有什么区别，谢谢)
2.如果我要压缩一个gzip数据（我要测试下我写的解压代码），我要用什么函数（请讲解下，关键是函数的参数设定为什么才会，
可以用inflateInit2(XX,47)进行解压..）
3.请各位大虾帮助，主要是inflateInit2和inflateInit的区别，还有设定47 和 MAX_WBITS（15）的区别（谢了！！！）
**Answer:**
deflate与gzip解压的代码几乎相同，应该可以合成一块代码。
区别仅有：
deflate使用inflateInit()，而gzip使用inflateInit2()进行初始化，比 inflateInit()多一个参数: -MAX_WBITS，表示处理raw deflate数据。因为gzip数据中的zlib压缩数据块没有zlib header的两个字节。使用inflateInit2时要求zlib库忽略zlib header。在zlib手册中要求windowBits为8..15，但是实际上其它范围的数据有特殊作用，见zlib.h中的注释，如负数表示raw deflate。
Apache的deflate变种可能也没有zlib header，需要添加假头后处理。即MS的错误deflate (raw deflate).zlib头第1字节一般是0×78, 第2字节与第一字节合起来的双字节应能被31整除，详见rfc1950。例如Firefox的zlib假头为0×7801，python zlib.compress()结果头部为0×789c。
++++++++++++++++++++++++++++++++++++++++++华丽丽的分割符+++++++++++++++++++++++++++++++++++
  zlib是一个通用的压缩开源库，提供了在内存中压缩和解压的函数，包括对解压后数据的校验。目前版本的zlib只支持deflate方法，但是其它的方法将会被添加进来并且拥有同样的接口。
—— zlib manaul
    deflate算法在rfc1951中有详细的说明。
    zlib同时又是一种数据格式，使用zlib库压缩后的数据会在deflate数据的头和尾添加信息，形成zlib格式的数据。
   gzip也是一种数据压缩格式，可以大体分为头部，数据部和尾部三个部分，其中头部和尾部主要是一些文档属性和校验信息（rfc1952），数据部主要是用deflate方法压缩得到的数据。
    zlib库默认的压缩方法并不是gzip的，而是zlib的，因此使用zlib压缩得到gzip格式的数据有两种方法：
- 使用zlib提供的gz***系列函数可以直接把想要的内容写入一个磁盘gzip文件； 
- 如果想在内存中生成gzip格式的数据，可以在初始化的时候调用inflateInit2函数，并指定为gzip格式，代码如下：
```
z_stream d_stream;
  d_stream.zalloc = NULL;
  d_stream.zfree = NULL;
  d_stream.opaque = NULL;
  int ret 
= deflateInit2(&d_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, DEFAULT_WINDOWSIZE,
 DEFAULT_MEMLEVEL,  Z_DEFAULT_STRATEGY);
  if (Z_OK 
!= ret)
  {
    printf("init deflate error\n");
    return ret;
  }
```
    之后，用deflate压缩出来的数据就是gzip的了。
[C语言 HTTP GZIP数据解压](http://www.cnblogs.com/dsblab/archive/2012/01/23/2328904.html)
这个代码在[http://www.j286.com/](http://www.j286.com/)有更新详情访问。
好吧~我承认..我是为了写博文在拆分工程- -!!!
在想要对gzip进行解压之前请确定你的数据是否需要做这个处理[http://www.cnblogs.com/dsblab/articles/2328890.html](http://www.cnblogs.com/dsblab/articles/2328890.html)
![](http://images.cnblogs.com/cnblogs_com/dsblab/350374/r_http.png)
HTTP 头中标识数据经过gzip的如上面第二个框。
对gzip的处理我们可以用强大的zlib库进行处理
C语言的zlib编译
[http://www.cnblogs.com/dsblab/articles/2328883.html](http://www.cnblogs.com/dsblab/articles/2328883.html)
[http://www.cnblogs.com/dsblab/articles/2328880.html](http://www.cnblogs.com/dsblab/articles/2328880.html)
我不是在推销...没这你做不下去，如果你是linux下作开发的话编译应该那应该是小菜了。
在使用前需要包含相关文件和库
#include "zlib/zlib.h"
#pragma comment(lib, "zlib.lib")
当然解压数据长度你可以分配一个稍微大一些的空间来存储再realloc还是索性一个大数组，或者用其他zlib提供的函数就出在分配，这个要看你自己了。
注意，使用的时候不要把头文件穿进去，把DATA段传进去就行了，否则会怎么样我不知道。。。没试过C#的stream
```
[]()
 1 /* HTTP gzip decompress */
 2 /* 参数1.压缩数据 2.数据长度 3.解压数据 4.解压后长度 */
 3 int httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
 4 {
 5     int err = 0;
 6     z_stream d_stream = {0}; /* decompression stream */
 7     static char dummy_head[2] = 
 8     {
 9         0x8 + 0x7 * 0x10,
10         (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
11     };
12     d_stream.zalloc = (alloc_func)0;
13     d_stream.zfree = (free_func)0;
14     d_stream.opaque = (voidpf)0;
15     d_stream.next_in  = zdata;
16     d_stream.avail_in = 0;
17     d_stream.next_out = data;
18     if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
19     while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) 
20     {
21         d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
22         if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
23         if(err != Z_OK )
24         {
25             if(err == Z_DATA_ERROR)
26             {
27                 d_stream.next_in = (Bytef*) dummy_head;
28                 d_stream.avail_in = sizeof(dummy_head);
29                 if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
30                 {
31                     return -1;
32                 }
33             }
34             else return -1;
35         }
36     }
37     if(inflateEnd(&d_stream) != Z_OK) return -1;
38     *ndata = d_stream.total_out;
39     return 0;
40 }
[]()
```
如果对平台没要求的话还是建议用C#来做好了，省力省时
```
[]()
        HttpWebResponse getRespone = (HttpWebResponse)get.GetResponse();
                if (getRespone != null && getRespone.StatusCode == HttpStatusCode.OK)
                {
                    recv = getRespone.ContentEncoding;
                    System.IO.Stream resStream = getRespone.GetResponseStream();
                    if (recv == "gzip")
                    {
                        resStream = new GZipStream(resStream, CompressionMode.Decompress);
                    }
                    System.IO.StreamReader sr = new System.IO.StreamReader(resStream, Encoding.UTF8);
                    recv = sr.ReadToEnd();
                    sr.Close();
                }
                getRespone.Close();
[]()
```
# Java和C++进行gzip数据交换时的Java端代码
[](http://ju.outofmemory.cn/entry/27644#)[](http://ju.outofmemory.cn/entry/27644#)[](http://ju.outofmemory.cn/entry/27644#)[](http://ju.outofmemory.cn/entry/27644#)[](http://ju.outofmemory.cn/entry/27644#)更多[0](http://ju.outofmemory.cn/entry/27644#)
[性能优化](http://ju.outofmemory.cn/tag/%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96)
公司的手持设备和服务端数据传输量比较大,有几百兆,所以手持设备接受数据比较慢,于是经理想双方用gzip进行压缩后传送。
但是两端都是字符串类型传送的,C++那段压缩传过来的就是两个乱码字符,这怎么解压缩啊,你要是流还好点,就俩字符无法解析啊。
最后决定C++压缩后对压缩内容进行base64加密然后把密文传给我,我这解密密文后再解压缩。
结果测试可行。
这里记一下base64代码和gzip压缩和解压代码
#### gzip压缩和解压缩
publicstaticString compressString(String str)throwsIOException{if(str ==null¦¦ str.length()==0){return str;}ByteArrayOutputStreamout=newByteArrayOutputStream();GZIPOutputStream gzip =newGZIPOutputStream(out);
	    gzip.write(str.getBytes());
	    gzip.close();returnout.toString("ISO-8859-1");}publicstaticString uncompressString(String str)throwsIOException{if(str ==null¦¦ str.length()==0){return str;}ByteArrayOutputStreamout=newByteArrayOutputStream();ByteArrayInputStreamin=newByteArrayInputStream(str
        .getBytes("ISO-8859-1"));GZIPInputStream gunzip =newGZIPInputStream(in);byte[] buffer =newbyte[256];int n;while((n = gunzip.read(buffer))>=0){out.write(buffer,0, n);}// toString()使用平台默认编码，也可以显式的指定如toString("GBK")returnout.toString();}
#### Java base64加密解密代码:
package com.gb.soa.core.util;import java.io.IOException;import java.io.UnsupportedEncodingException;import sun.misc.BASE64Decoder;publicclassBase64Util{publicstaticString getBASE64(byte[] b){String s =null;if(b !=null){
		   s =new sun.misc.BASE64Encoder().encode(b);}return s;}publicstaticbyte[] getFromBASE64(String s){byte[] b =null;if(s !=null){
		   BASE64Decoder decoder =new BASE64Decoder();try{
		    b = decoder.decodeBuffer(s);return b;}catch(Exception e){
		    e.printStackTrace();}}return b;}privatestaticchar[] base64EncodeChars =newchar[]{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};privatestaticbyte[] base64DecodeChars =newbyte[]{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1};publicstaticString encode(byte[] data){StringBuffer sb =newStringBuffer();int len = data.length;int i =0;int b1, b2, b3;while(i < len){
         b1 = data[i++]&0xff;if(i == len){
             sb.append(base64EncodeChars[b1 >>>2]);
             sb.append(base64EncodeChars[(b1 &0x3)<<4]);
             sb.append("==");break;}
         b2 = data[i++]&0xff;if(i == len){
             sb.append(base64EncodeChars[b1 >>>2]);
             sb.append(base64EncodeChars[((b1 &0x03)<<4)¦((b2 &0xf0)>>>4)]);
             sb.append(base64EncodeChars[(b2 &0x0f)<<2]);
             sb.append("=");break;}
         b3 = data[i++]&0xff;
         sb.append(base64EncodeChars[b1 >>>2]);
         sb.append(base64EncodeChars[((b1 &0x03)<<4)¦((b2 &0xf0)>>>4)]);
         sb.append(base64EncodeChars[((b2 &0x0f)<<2)¦((b3 &0xc0)>>>6)]);
         sb.append(base64EncodeChars[b3 &0x3f]);}return sb.toString();}publicstaticbyte[] decode(String str)throwsUnsupportedEncodingException{StringBuffer sb =newStringBuffer();byte[] data = str.getBytes("US-ASCII");int len = data.length;int i =0;int b1, b2, b3, b4;while(i < len){do{
             b1 = base64DecodeChars[data[i++]];}while(i < len && b1 ==-1);if(b1 ==-1)break;do{
             b2 = base64DecodeChars
                     [data[i++]];}while(i < len && b2 ==-1);if(b2 ==-1)break;
         sb.append((char)((b1 <<2)¦((b2 &0x30)>>>4)));do{
             b3 = data[i++];if(b3 ==61)return sb.toString().getBytes("utf-8");
             b3 = base64DecodeChars[b3];}while(i < len && b3 ==-1);if(b3 ==-1)break;
         sb.append((char)(((b2 &0x0f)<<4)¦((b3 &0x3c)>>>2)));do{
             b4 = data[i++];if(b4 ==61)return sb.toString().getBytes("utf-8");
             b4 = base64DecodeChars[b4];}while(i < len && b4 ==-1);if(b4 ==-1)break;
         sb.append((char)(((b3 &0x03)<<6)¦ b4));}return sb.toString().getBytes("utf-8");}publicstaticvoid main(String[] args)throwsIOException{//  String s = "abcd";//  System.out.println("加密前：" + s);// String x = encode(s.getBytes());// System.out.println("加密后：" + x);String x ="H4sIAAAAAAAACwEkANv/5Lit5Zu95ZWG6LS45pyJ6ZmQ5YWs5Y+4Z29vZGJhYnkuY29t3DEE+CQAAADN";String myX ="H4sIAAAAAAAAAAEkANv/5Lit5Zu95ZWG6LS45pyJ6ZmQ5YWs5Y+4Z29vZGJhYnkuY29t3DEE+CQAAAA=";String x1 =newString(decode(myX));System.out.println(x1);String gzipString =GzipUtil.uncompressString(x1);System.out.println("解密后："+ gzipString);}}
本文出自：[http://www.ij2ee.com](http://www.ij2ee.com/), 原文地址：[http://www.ij2ee.com/50874.html](http://www.ij2ee.com/50874.html), 感谢原作者分享。
