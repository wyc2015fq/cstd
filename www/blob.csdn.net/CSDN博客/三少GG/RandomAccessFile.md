# RandomAccessFile - 三少GG - CSDN博客
2012年04月17日 15:13:43[三少GG](https://me.csdn.net/scut1135)阅读数：1065标签：[string																[file																[character																[java																[encoding																[byte](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[重构Java](https://blog.csdn.net/scut1135/article/category/621771)
## [RandomAccessFile](http://www.cnblogs.com/kelin1314/archive/2010/08/05/1793320.html)
RandomAccessFile是用来访问那些保存数据记录的文件的，你就可以用seek( )方法来访问记录，并进行读写了。这些记录的大小不必相同；但是其大小和位置必须是可知的。但是该类仅限于操作文件。
RandomAccessFile不属于InputStream和OutputStream类系的。实际上，除了实现DataInput和DataOutput接口之外(DataInputStream和DataOutputStream也实现了这两个接口)，它和这两个类系毫不相干，甚至不使用InputStream和OutputStream类中已经存在的任何功能；它是一个完全独立的类，所有方法(绝大多数都只属于它自己)都是从零开始写的。这可能是因为RandomAccessFile能在文件里面前后移动，所以它的行为与其它的I/O类有些根本性的不同。总而言之，它是一个直接继承Object的，独立的类。
基本上，RandomAccessFile的工作方式是，把DataInputStream和DataOutputStream结合起来，再加上它自己的一些方法，比如定位用的getFilePointer( )，在文件里移动用的seek( )，以及判断文件大小的length( )、skipBytes()跳过多少字节数。此外，它的构造函数还要一个表示以只读方式("r")，还是以读写方式("rw")打开文件的参数 (和C的fopen( )一模一样)。它不支持只写文件。
只有RandomAccessFile才有seek搜寻方法，而这个方法也只适用于文件。BufferedInputStream有一个mark( )方法，你可以用它来设定标记(把结果保存在一个内部变量里)，然后再调用reset( )返回这个位置，但是它的功能太弱了，而且也不怎么实用。
RandomAccessFile的绝大多数功能，但不是全部，已经被JDK 1.4的nio的"内存映射文件(memory-mapped files)"给取代了，你该考虑一下是不是用"内存映射文件"来代替RandomAccessFile了。
Java代码 
- import java.io.IOException;  
- import java.io.RandomAccessFile;  
- 
- publicclass TestRandomAccessFile {  
- publicstaticvoid main(String[] args) throws IOException {  
-         RandomAccessFile rf = new RandomAccessFile("rtest.dat", "rw");  
- for (int i = 0; i < 10; i++) {  
- //写入基本类型double数据
-             rf.writeDouble(i * 1.414);  
-         }  
-         rf.close();  
-         rf = new RandomAccessFile("rtest.dat", "rw");  
- //直接将文件指针移到第5个double数据后面
-         rf.seek(5 * 8);  
- //覆盖第6个double数据
-         rf.writeDouble(47.0001);  
-         rf.close();  
-         rf = new RandomAccessFile("rtest.dat", "r");  
- for (int i = 0; i < 10; i++) {  
-             System.out.println("Value " + i + ": " + rf.readDouble());  
-         }  
-         rf.close();  
-     }  
- }   
## 内存映射文件
内存映射文件能让你创建和修改那些因为太大而无法放入内存的文件。有了内存映射文件，你就可以认为文件已经全部读进了内存，然后把它当成一个非常大的数组来访问。这种解决办法能大大简化修改文件的代码。
fileChannel.map(FileChannel.MapMode mode, long position, long size)将此通道的文件区域直接映射到内存中。注意，你必须指明，它是从文件的哪个位置开始映射的，映射的范围又有多大；也就是说，它还可以映射一个大文件的某个小片断。
MappedByteBuffer是ByteBuffer的子类，因此它具备了ByteBuffer的所有方法，但新添了force()将缓冲区的内容强制刷新到存储设备中去、load()将存储设备中的数据加载到内存中、isLoaded()位置内存中的数据是否与存储设置上同步。这里只简单地演示了一下put()和get()方法，除此之外，你还可以使用asCharBuffer( )之类的方法得到相应基本类型数据的缓冲视图后，可以方便的读写基本类型数据。
Java代码 
- import java.io.RandomAccessFile;  
- import java.nio.MappedByteBuffer;  
- import java.nio.channels.FileChannel;  
- 
- publicclass LargeMappedFiles {  
- staticint length = 0x8000000; // 128 Mb
- 
- publicstaticvoid main(String[] args) throws Exception {  
- // 为了以可读可写的方式打开文件，这里使用RandomAccessFile来创建文件。
-         FileChannel fc = new RandomAccessFile("test.dat", "rw").getChannel();  
- //注意，文件通道的可读可写要建立在文件流本身可读写的基础之上
-         MappedByteBuffer out = fc.map(FileChannel.MapMode.READ_WRITE, 0, length);  
- //写128M的内容
- for (int i = 0; i < length; i++) {  
-             out.put((byte) 'x');  
-         }  
-         System.out.println("Finished writing");  
- //读取文件中间6个字节内容
- for (int i = length / 2; i < length / 2 + 6; i++) {  
-             System.out.print((char) out.get(i));  
-         }  
-         fc.close();  
-     }  
- }  
尽管映射写似乎要用到FileOutputStream，但是映射文件中的所有**输出** 必须使用RandomAccessFile，但如果只需要读时可以使用FileInputStream，写映射文件时一定要使用随机访问文件，可能写时要读的原因吧。
该程序创建了一个128Mb的文件，如果一次性读到内存可能导致内存溢出，但这里访问好像只是一瞬间的事，这是因为，真正调入内存的只是其中的一小部分，其余部分则被放在交换文件上。这样你就可以很方便地修改超大型的文件了(最大可以到2
 GB)。注意，Java是调用操作系统的"文件映射机制"来提升性能的。
+++++++++++++++++++++++++++++++++++++++++++
[Java中正确使用RandomAccessFile向文件中写入中文](http://hi.baidu.com/perfecthouse/blog/item/d9109e23dbbbe444ac34de4a.html)
Java中正确使用RandomAccessFile向文件中写入中文
最近有一个客户询问使用Java中的RandomAccessFile不能向文件中正确写入中文，出来的都是乱码。经过分析和验证后，发现的问题的原因和解决办法。下面将主要的回复内容贴出来和大家分享：
-------------------------------------------------------------------------------
        先将您昨天上午描述的问题总结如下：
        使用RandomAccessFile向数据库写入中文的时候，
        *使用write(String.getBytes()), 能够正常写入
        *使用writeBytes(String), writeChars(String), writeUTF(String)均产生乱码。
        如果我对您的问题理解正确的话，经过分析，我认为如果您是使用RandomAccessFile来
访问数据库的话，为了正确写入中文，您最好使用write(String.getBytes())的方式。这主要有如下两方面的原因：
1、当java运行时，实际上存在两种字符编码方式。nativecode编码和unicode编码。
    * 文件被操作系统保存时，都是以nativecode的编码方式保存的。这也是我们有时候在浏览器或电子邮件客户端软件中看到乱码后，改变浏览器或电子邮件客户端软件的编码设置（例如：GB2312,GB18030或者是BIG5）就可以正确的显示。
    * 在JAVA程序内部，字符串都是以UNICODE的方式来表示的。
    Java的内核是unicode的，其class文件也是这样的。另外在java代码中，string中的char是用unicode编码方式来表示的，string的bytes是用相应的nativecode编码方式来表示的。
    由于RandomAccessFile是同native file来打交道，所以必然存在一个nativecode和unicode的转化过程。
2、RandomAccessFile的文件写入方式。
在RandomAccessFile的Javadoc中，对于各种文件写入方式有不同的定义。
    * public void write(byte[] b)：Writes b.length bytes from the specified byte array to this file, starting at the current file pointer.
    * public final void writeBytes(String s) throws IOException
    Writes the string to the file as a sequence of bytes. Each character in the string is written out, in sequence, by discarding its high eight bits. The write starts at the current position of the file pointer.（请注意每个字符的高8位都会被抛弃掉。）
        * public final void writeChar(int v) throws IOException
    Writes a char to the file as a two-byte value, high byte first. The write starts at the current position of the file pointer.（采用的是Big-endian的存储方式,注意由于x86架构的限制，Windows默认采用Little-endian）
    * public final void writeChars(String s) throws IOException
    Writes a string to the file as a sequence of characters. Each character is written to the data output stream as if by the writeChar method. The write starts at the current position of the file pointer.(注意writeChars采用的是writeChar的写入方式。)
    * public final void writeUTF(String str) throws IOException
    Writes a string to the file using modified UTF-8 encoding in a machine-independent manner.
    First, two bytes are written to the file, starting at the current file pointer, as if by the writeShort method giving the number of bytes to follow. This value is the number of bytes actually written out, not the length of the string. Following the length,
 each character of the string is output, in sequence, using the modified UTF-8 encoding for each character. （注意writeUTF会首先写入两个字节，表示其后实际写入的字节数，然后才是对应字符串的UTF-8编码。）
下面是我编写的一段测试代码，供您参考。
--------------------------------------------------------------------------------
/*
* RandomAFTest.java
*
* Created on 2005年5月8日, 下午3:38
*
*/
import java.io.*;
/**
*
* @author Paul
*/
public class RandomAFTest {
    //按照指定的charset,将字符串转换为bytes，并打印出来
    public static void printBytes(String str, String charsetName) {
        try {
            byte strBytes[] = str.getBytes(charsetName);
            String strBytesContent = "";
            for (int i = 0; i < strBytes.length; i++) {
                strBytesContent = strBytesContent.concat(Integer.
                        toHexString(strBytes*) + ",");            }            System.out.println("The Bytes of String " + str +                     " within charset " + charsetName + " are: " +                     strBytesContent);        } catch (UnsupportedEncodingException e) {            //Not handle;        }    }    //将字符串的chars打印出来    public static void printChars(String str) {        int strlen = str.length();        char strChars[] = new char[strlen];        str.getChars(0, strlen, strChars, 0);        String strCharsContent = "";        for (int i = 0; i < strlen; i++) {            strCharsContent = strCharsContent.concat(Integer.                    toHexString(strChars*) + ",");        }        System.out.println("The chars of String " + str + " are: " +         strCharsContent);    }    public static void main(String args[]) {        try {            RandomAccessFile rfWrite =                     new RandomAccessFile("c:\\testWrite.dat", "rw");            RandomAccessFile rfWriteBytes =                     new RandomAccessFile("c:\\testWriteBytes.dat", "rw");            RandomAccessFile rfWriteChars =                     new RandomAccessFile("c:\\testWriteChars.dat", "rw");            RandomAccessFile rfWriteUTF =                     new RandomAccessFile("c:\\testWriteUTF.dat", "rw");            String chStr = "中";            //打印字符串在GB2312下的bytes            printBytes(chStr, "GB2312");            //打印字符串在UTF-8下的bytes            printBytes(chStr, "UTF-8");            //打印字符串的UNICODE的chars            printChars(chStr);            try {                rfWrite.write(chStr.getBytes());                rfWrite.close();                System.out.println("Done write!");                rfWriteBytes.writeBytes(chStr);                rfWriteBytes.close();                System.out.println("Done writeBytes!");                rfWriteChars.writeChars(chStr);                rfWriteChars.close();                System.out.println("Done writeChars!");                rfWriteUTF.writeUTF(chStr);                rfWriteUTF.close();                System.out.println("Done writeUTF!");            } catch (IOException e) {                // Do not handle the IOException            }        } catch (FileNotFoundException e) {            //Do not handle         }    }}---------------------------------------------------------------------------以下是该程序的部分运行结果：The Bytes of String 中 within charset GB2312 are: ffffffd6,ffffffd0,The Bytes of String 中 within charset UTF-8 are: ffffffe4,ffffffb8,ffffffad,The chars of String 中 are: 4e2d,我们可以看到"中"的    * GB2312编码为D6 D0    * UTF-8编码为 E4 B8 AD    * UNICODE编码为 4E 2D那么实际写入的文件是什么样的呢,下面给出各个文件内容的16进制描述：文件testWrite.dat:D6 D0文件testWriteBytes.dat:2D文件testWriteChars.dat:4E 2D文件testWriteUTF.dat:00 03 E4 B8 AD结合我们上述的1和2，我们不难看出：1、String.getBytes（）将会按照当前系统默认的encoding方式获得字符串的Bytes，RandomAccessFile.write(byte[])将这个byte数组正确写入。由于写入的实际就是Windows平台的nativecode编码，所以文件还能够被正确的阅读。2、RandomAccessFile.writeBytes(String)将字符串的各个字符（当然是用unicode编码的）的高8位去掉，写入文件。3、RandomAccessFile.writeChars(String)将字符串的各个字符按照unicode的编码，以Big-endian的方式写入文件。Windows平台上默认文件的编码方式为Little-endian，所以用写字板打开看到的是乱码，但是如果我们用浏览器打开这个文件（testWriteChars.dat）并指定编码方式为Unicode Big-endian，就能看到正常的“中”字了。4、RandomAccessFile.writeUTF(String)首先写入00 03表示其后将写入3个实际的字节，然后写入“中”的UTF-8编码：E4 B8 AD通过上面的分析，我建议如果使用RandomAccessFile来写入中文的话，最好用RandomAccessFile.write(String.getBytes())的方式，如果为了保险起见，还可以进一步指定运行平台的默认nativecode编码方式，例如使用：RandomAccessFile.write(String.getBytes("gb2312"))**

