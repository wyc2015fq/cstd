
# java入门教程-8.7Java中文件的随机读写 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:39:02[seven-soft](https://me.csdn.net/softn)阅读数：173


Java.io 包提供了 RandomAccessFile 类用于随机文件的创建和访问。使用这个类，可以跳转到文件的任意位置读写数据。程序可以在随机文件中插入数据，而不会破坏该文件的其他数据。此外，程序也可以更新或删除先前存储的数据，而不用重写整个文件。
RandomAccessFile类是Object类的直接子类，包含两个主要的构造方法用来创 建RandomAccessFile 的对象，如表 10-11 所示。
表 10-11 RandomAccessFile 类的构造方法|构造方法|功能描述|
|public RandomAccessFile(String name, String mode)|指定随机文件流对象所对应的文件名，以 mode 表示对文件的访问模式|
|public RandomAccessFile (File file, String mode)|以 file 指定随机文件流对象所对应的文件名，以 mode 表示访问模式|
需要注意的是，mode 表示所创建的随机读写文件的操作状态，其取值包括：r：表示以只读方式打开文件。
rw：表示以读写方式打开文件，使用该模式只用一个对象即可同时实现读写操作。
表 10-12 列出了 RandowAccessFile 类常用的方法及说明。
表 10-12 RandowAccessFile 的常用方法|方法|功能描述|
|long length()|返回文件长度|
|void seek(long pos)|移动文件位置指示器，pos 指定从文件开头的偏离字节数|
|int skipBytes(int n)|跳过 n 个字节，返回数为实际跳过的字节数|
|int read()|从文件中读取一个字节，字节的高 24 位为 0，若遇到文件结尾，返回-1|
|final byte readByte()|从文件中读取带符号的字节值|
|final char readChar()|从文件中读取一个 Unicode 字符|
|final void writeChar(inte c)|写入一个字符，两个字节|
【例 10-12】模仿系统日志，将数据写入到文件尾部。
//********** ep10_12.java **********
import java.io.*;
class ep10_12{
public static void main(String args[]) throws IOException{
try{
BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
String s=in.readLine();
RandomAccessFile myFile=new RandomAccessFile("ep10_12.log","rw");
myFile.seek(myFile.length());  //移动到文件结尾
myFile.writeBytes(s+"\n");  //写入数据
myFile.close();
}
catch(IOException e){}
}
}
程序运行后在目录中建立一个 ep10_12.log 的文件，每次运行时输入的内容都会在该文件内容的结尾处添加。

