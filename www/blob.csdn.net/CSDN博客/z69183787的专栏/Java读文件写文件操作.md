# Java读文件写文件操作 - z69183787的专栏 - CSDN博客
2012年11月13日 16:47:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：820
个人分类：[文件处理-读写																[IO-X](https://blog.csdn.net/z69183787/article/category/2175881)](https://blog.csdn.net/z69183787/article/category/2229757)
这里，Java的读文件和写文件都是基于字符流的，主要用到下面的几个类：
**1、FileReader----读取字符流2、FileWriter----写入字符流3、BufferedReader----缓冲指定文件的输入**该类的方法有：
void close() 
        关闭该流。 
 void mark(int readAheadLimit) 
        标记流中的当前位置。 
 boolean markSupported() 
        判断此流是否支持 mark() 操作（它一定支持）
int read() 
        读取单个字符。 
 int read(char[] cbuf, int off, int len) 
        将字符读入数组的某一部分。 
 String readLine() 
        读取一个文本行。 
 boolean ready() 
        判断此流是否已准备好被读取。 
 void reset() 
        将流重置为最新的标记。 
 long skip(long n) 
        跳过字符。 
**4、BufferedWriter----将缓冲对文件的输出**
该类的方法有：
void close() 
        关闭该流。 
 void flush() 
        刷新该流的缓冲。 
 void newLine() 
        写入一个行分隔符。 
 void write(char[] cbuf, int off, int len) 
        写入字符数组的某一部分。 
 void write(int c) 
        写入单个字符。 
 void write(String s, int off, int len) 
        写入字符串的某一部分。 
**举个例子如下：**
> 
package aillo;
import java.io.*;
public class FileWriterReader {
//功能:读取f:/aillo.txt文件的内容(一行一行读),并将其内容写入f:/jackie.txt中
//知识点:java读文件、写文件---<以字符流方式>
    public static void main(String[] args) {
        try {
            FileReader fr = new FileReader("f:/aillo.txt");//创建FileReader对象，用来读取字符流
            BufferedReader br = new BufferedReader(fr);    //缓冲指定文件的输入
            FileWriter fw = new FileWriter("f:/jackie.txt");//创建FileWriter对象，用来写入字符流
            BufferedWriter bw = new BufferedWriter(fw);    //将缓冲对文件的输出
            String myreadline;    //定义一个String类型的变量,用来每次读取一行
            while (br.ready()) {
                myreadline = br.readLine();//读取一行
                bw.write(myreadline); //写入文件
                bw.newLine();
                System.out.println(myreadline);//在屏幕上输出
            }
            bw.flush();    //刷新该流的缓冲
            bw.close();
            br.close();
            fw.close();
            br.close();
            fr.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
**********************************************************
