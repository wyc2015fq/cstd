
# java入门教程-8.8Java中文件的压缩处理 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:40:02[seven-soft](https://me.csdn.net/softn)阅读数：174


Java.util.zip 包中提供了可对文件的压缩和解压缩进行处理的类，它们继承自字节流类OutputSteam 和 InputStream。其中
 GZIPOutputStream 和 ZipOutputStream 可分别把数据压缩成 GZIP 和 Zip 格式，GZIPInpputStream 和 ZipInputStream 又可将压缩的数据进行还原。
将文件写入压缩文件的一般步骤如下：生成和所要生成的压缩文件相关联的压缩类对象。
压缩文件通常不只包含一个文件，将每个要加入的文件称为一个压缩入口，使用ZipEntry(String FileName)生成压缩入口对象。
使用 putNextEntry(ZipEntry entry)将压缩入口加入压缩文件。
将文件内容写入此压缩文件。
使用 closeEntry()结束目前的压缩入口，继续下一个压缩入口。
将文件从压缩文件中读出的一般步骤如下：生成和所要读入的压缩文件相关联的压缩类对象。
利用 getNextEntry()得到下一个压缩入口。
【例 10-13】输入若干文件名，将所有文件压缩为“ep10_13.zip”，再从压缩文件中解压并显示。
//********** ep10_13.java **********
import java.io.*;
import java.util.*;
import java.util.zip.*;
class ep10_13{
public static void main(String args[]) throws IOException{
FileOutputStream a=new FileOutputStream("ep10_13.zip");
//处理压缩文件
ZipOutputStream out=new ZipOutputStream(new BufferedOutputStream(a));
for(int i=0;i<args.length;i++){  //对命令行输入的每个文件进行处理
System.out.println("Writing file"+args[i]);
BufferedInputStream in=new BufferedInputStream(new FileInputStream(args[i]));
out.putNextEntry(new ZipEntry(args[i]));  //设置 ZipEntry 对象
int b;
while((b=in.read())!=-1)
out.write(b);  //从源文件读出，往压缩文件中写入
in.close();
}
out.close();
//解压缩文件并显示
System.out.println("Reading file");
FileInputStream d=new FileInputStream("ep10_13.zip");
ZipInputStream  inout=new  ZipInputStream(new BufferedInputStream(d));
ZipEntry z;
while((z=inout.getNextEntry())!=null){  //获得入口
System.out.println("Reading file"+z.getName());  //显示文件初始名
int x;
while((x=inout.read())!=-1)
System.out.write(x);
System.out.println();
}
inout.close();
}
}
例 10-13 运行后，在程序目录建立一个 ep10_13.zip 的压缩文件，使用解压缩软件（如 WinRAR等），可以将其打开。命令提示符下，程序运行结果如图 10-12 所示：![图 10-12  例 10_13 运行结果](http://www.weixueyuan.net/uploads/allimg/130223/8-130223010044D3.png)
图 10-12  例 10_13 运行结果

