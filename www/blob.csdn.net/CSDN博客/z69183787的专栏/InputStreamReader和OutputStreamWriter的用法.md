# InputStreamReader和OutputStreamWriter的用法 - z69183787的专栏 - CSDN博客
2012年11月13日 19:08:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6556
首先说明下字节流和字符流：
FileReader，FileWriter 是读字符的，1个字符是2个字节，16位
OutputStream ，InputStream是读字节的，1个字节是8位
如果要读汉字，一个汉字是2个字节，用InputStream读就把汉字折成两半，不太适合吧。
字符流主要读文本效率比较高。
字节流主要读二进制文件，图片 ，音频，视频等，就是为了防止出现读取的单位放大，出现无法还原本程序的目的
如果把文件按字节读取，然后按照UTF-8的格式编码显示，怎么办？
InputStreamReader就有这功能，对原字节再编码的过程。
-----------------------------------------------------------------------------------
下面切入正题：
转换流：InputStreamReader和OutputStreamWriter
一。InputStreamReader 是字符流Reader的子类，是字节流通向字符流的桥梁。你可以在构造器重指定编码的方式，如果不指定的话将采用底层操作系统的默认编码方式，例如 GBK 等
要启用从字节到字符的有效转换，可以提前从底层流读取更多的字节，使其超过满足当前读取操作所需的字节。
为了达到最高效率，可要考虑在 BufferedReader 内包装 InputStreamReader。例如：
 BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
二。OutputStreamWriter 是字符流Writer的子类，是字符流通向字节流的桥梁。
每次调用 write() 方法都会导致在给定字符（或字符集）上调用编码转换器。在写入底层输出流之前，得到的这些字节将在缓冲区中累积
为了获得最高效率，可考虑将 OutputStreamWriter 包装到 BufferedWriter 中，以避免频繁调用转换器。例如：
 BufferedWriter out  = new BufferedWriter(new OutputStreamWriter(System.out));
------------------------------------------------------------------------------------------
（1）Reader和Writer类（文本字符流读写类）：提供的对字符流处理的类，它们为抽象类。一般通过其子类来实现。
（2）InputStreamReader(InputStream in) 和OutputStreamWriter(OutputStream out)：它们可以使用指定的编码规范并基于字节流生成对应的字符流。
（3）BufferedReader(InputStreamReader isr, int size) 和 BufferedWriter(OutputStreamWriter osr, int size)：
为提高字符流的处理效率，可以采用缓冲机制的流实现对字符流作成批的处理，避免了频繁的从物理设备中读取信息 。
示例一：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.zlc.io;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
public class StreamReaderWriterDemo {
 public static void main(String[] args) {
  try { 
   //1.从文本文件中获得输入字节到字节输入流中
   FileInputStream fis = new FileInputStream("f:\\01.txt"); 
   
   //2. 为FileInputStream加上字符处理功能
   InputStreamReader isr = new InputStreamReader(fis); //将字节流转换成字符流
   
   //3.为了达到最高效率，可要考虑在 BufferedReader内包装 InputStreamReader
   BufferedReader bufr = new BufferedReader(isr);
   
   
   //4.创建将数据写入到文本文件的文件输出流
   FileOutputStream fos = new FileOutputStream("f:\\02.txt"); 
   
   //5.为FileOutputStream加上字符处理功能
   OutputStreamWriter osw = new OutputStreamWriter(fos); 
   
   //6.为了获得最高效率，可考虑将 OutputStreamWriter 包装到 BufferedWriter 中，以避免频繁调用转换器
   BufferedWriter bufw = new BufferedWriter(osw);
   
   int ch = 0;
   
   // 以字符方式显示文件内容 
   while((ch = bufr.read()) != -1) {
    System.out.print((char)ch); 
    osw.write(ch); 
   }
   if(bufr!=null)
    bufr.close();
   if(bufw!=null)
    bufw.close();
  } catch(ArrayIndexOutOfBoundsException e) {
   e.printStackTrace();
  } catch(IOException e) {
   e.printStackTrace();
  } 
 }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
在通常编码时，下面的方式更为常用。下面的代码是对上面进行了简化。示例二：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 package com.zlc.io;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
public class StreamReaderWriterDemo2 {
 public static void main(String[] args) {
  try { 
   
   BufferedReader bufr = new BufferedReader(new InputStreamReader(new FileInputStream("f:\\01.txt")));
   
   BufferedWriter bufw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("f:\\02.txt")));
   
   int ch = 0;
   
   // 以字符方式显示文件内容 
   while((ch = bufr.read()) != -1) {
    System.out.print((char)ch); 
    bufw.write(ch); 
   }
   if(bufr!=null)
    bufr.close();
   if(bufw!=null)
    bufw.close();
  } catch(ArrayIndexOutOfBoundsException e) {
   e.printStackTrace();
  } catch(IOException e) {
   e.printStackTrace();
  } 
 }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
