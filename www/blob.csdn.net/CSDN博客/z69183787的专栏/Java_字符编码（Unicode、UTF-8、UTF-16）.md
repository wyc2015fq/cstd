# Java_字符编码（Unicode、UTF-8、UTF-16） - z69183787的专栏 - CSDN博客
2014年05月13日 21:36:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1265
首先看一下下面的程序（测试英文和中文在Unicode、UTF-8、UTF-16这三种编码下，一个字符占几个字节）
**[java]**[view
 plain](http://blog.csdn.net/tianjf0514/article/details/7854624#)[copy](http://blog.csdn.net/tianjf0514/article/details/7854624#)
- System.out.println("a(Unicode)    ：" + "a".getBytes("Unicode").length);  
- System.out.println("a(Unicode)    ：" + "aa".getBytes("Unicode").length);  
- System.out.println("啊(Unicode)   ：" + "啊".getBytes("Unicode").length);  
- System.out.println("啊啊(Unicode) ：" + "啊啊".getBytes("Unicode").length);  
- System.out.println("");  
- System.out.println("a(UTF-8)    ：" + "a".getBytes("UTF-8").length);  
- System.out.println("aa(UTF-8)   ：" + "aa".getBytes("UTF-8").length);  
- System.out.println("啊(UTF-8)   ：" + "啊".getBytes("UTF-8").length);  
- System.out.println("啊啊(UTF-8) ：" + "啊啊".getBytes("UTF-8").length);  
- System.out.println("");  
- System.out.println("a(UTF-16)    ：" + "a".getBytes("UTF-16").length);  
- System.out.println("aa(UTF-16)   ：" + "aa".getBytes("UTF-16").length);  
- System.out.println("啊(UTF-16)   ：" + "啊".getBytes("UTF-16").length);  
- System.out.println("啊啊(UTF-16) ：" + "啊啊".getBytes("UTF-16").length);  
运行结果如下：
a(Unicode)      ：4
a(Unicode)      ：6
啊(Unicode)     ：4
啊啊(Unicode) ：6
a(UTF-8)      ：1
aa(UTF-8)    ：2
啊(UTF-8)     ：3
啊啊(UTF-8) ：6
a(UTF-16)      ：4
aa(UTF-16)    ：6
啊(UTF-16)     ：4
啊啊(UTF-16) ：6
可以看到UTF-8的情况：一个英文字符占一个字节，一个汉字占三个字节
但是Unicode和UTF-16的情况比较奇怪，不管是英文还是汉字，看不出占几个字节。其实正确的答案是：Unicode和UTF-16的编码下，不管是英文字符还是汉字字符，都占两个字节（至于上面结果中多出来的两个字节是用来表示字节顺序的默认字节）。至于为什么，继续往下看。
Unicode规范中推荐的标记字节顺序的方法是BOM。BOM不是“Bill Of Material”的BOM表，而是Byte Order Mark。
(Unicode是一种字符编码方法，不过它是由国际组织设计，可以容纳全世界所有语言文字的编码方案。Unicode的学名是"Universal Multiple-Octet Coded Character Set"，简称为UCS。UCS可以看作是"Unicode Character Set"的缩写。)
在UCS编码中有一个叫做"ZERO WIDTH NO-BREAK SPACE"的字符，它的编码是FEFF。而FFFE在UCS中是不存在的字符，所以不应该出现在实际传输中。UCS规范建议在传输字节流前，先传输字符"ZERO WIDTH NO-BREAK SPACE"。
这样如果接收者收到FEFF，就表明这个字节流是Big-Endian的；如果收到FFFE，就表明这个字节流是Little-Endian的。因此字符"ZERO WIDTH NO-BREAK SPACE"又被称作BOM。
在 Java 中直接使用Unicode 转码时会按照UTF-16LE 的方式拆分，并加上 BOM。 如果采用 UTF-16 拆分，在 Java 中默认采用带有 BOM 的 UTF-16BE 拆分。
再来看一个程序：
**[java]**[view
 plain](http://blog.csdn.net/tianjf0514/article/details/7854624#)[copy](http://blog.csdn.net/tianjf0514/article/details/7854624#)
- publicclass Test {  
- privatefinalstaticchar[] HEX = "0123456789abcdef".toCharArray();  
- 
- publicstaticvoid main(String[] args) throws UnsupportedEncodingException {  
-          String str = "中国";  
-          String[] encoding = { "Unicode", "UnicodeBig", "UnicodeLittle", "UnicodeBigUnmarked",  
- "UnicodeLittleUnmarked", "UTF-16", "UTF-16BE", "UTF-16LE" };  
- 
- for (int i = 0; i < encoding.length; i++) {  
-          System.out  
-          .printf("%-22s %s%n", encoding[i], bytes2HexString(str.getBytes(encoding[i])));  
-          }  
-     }  
- 
- publicstatic String bytes2HexString(byte[] bys) {  
- char[] chs = newchar[bys.length * 2 + bys.length - 1];  
- for (int i = 0, offset = 0; i < bys.length; i++) {  
- if (i > 0) {  
-                 chs[offset++] = ' ';  
-             }  
-             chs[offset++] = HEX[bys[i] >> 4 & 0xf];  
-             chs[offset++] = HEX[bys[i] & 0xf];  
-         }  
- returnnew String(chs);  
-     }  
- }  
运行结果如下：
Unicode                fe ff 4e 2d 56 fd
UnicodeBig             fe ff 4e 2d 56 fd
UnicodeLittle          ff fe 2d 4e fd 56
UnicodeBigUnmarked     4e 2d 56 fd
UnicodeLittleUnmarked  2d 4e fd 56
UTF-16                 fe ff 4e 2d 56 fd
UTF-16BE               4e 2d 56 fd
UTF-16LE               2d 4e fd 56
可以看到几个不同的Unicode和UTF-16编码的字节顺序是不同的，有的是fe ff，有的是ff fe，有的没有。
总上所述：
> 
Unicode和UTF-16：1个字符占2个字节（不管是哪国语言）
UTF-8：1个英文字符占1个字节，一个汉字（包括日文和韩文等）占3个字节
Java中的char默认采用Unicode编码，所以Java中char占2个字节
另外，顺便提一个知识点：1个字节(byte)占8位(bit)
