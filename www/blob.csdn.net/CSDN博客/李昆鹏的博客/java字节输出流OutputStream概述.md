
# java字节输出流OutputStream概述 - 李昆鹏的博客 - CSDN博客


2018年04月12日 04:47:25[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：89


------------------------------------java字节输出流OutputStream概述-----------------------------
**字节输入流：InputStream****常用子类 FileInputStream**
**字节输出流：OutputStream****常用子类 FileOutputStream**
**字节输出流在写入文本信息的时候是不用释放缓冲区的，字符输出流是要释放缓冲区的数据，使用flush()****方法。**

**OutputStream构造器**
|FileOutputStream|(|File|file)
|创建一个向指定 File 对象表示的文件中写入数据的文件输出流。
|
|FileOutputStream|(|File|file, boolean append)
|创建一个向指定 File 对象表示的文件中写入数据的文件输出流。
|
|FileOutputStream|(|FileDescriptor|fdObj)
|创建一个向指定文件描述符处写入数据的输出文件流，该文件描述符表示一个到文件系统中的某个实际文件的现有连接。
|
|FileOutputStream|(|String|name)
|创建一个向具有指定名称的文件中写入数据的输出文件流。
|
|FileOutputStream|(|String|name, boolean append)
|创建一个向具有指定 name 的文件中写入数据的输出文件流。
|

写入方法
|void
|write|(byte[] b)
|将 b.length 个字节从指定 byte 数组写入此文件输出流中。
|
|void
|write|(byte[] b, int off, int len)
|将指定 byte 数组中从偏移量 off 开始的 len 个字节写入此文件输出流。
|
|void
|write|(int b)
|将指定字节写入此文件输出流。
|


**范例： 使用字节流向文件中写入helloworld**
分析：使用三种写入方法

**public****class**OutputStreamDemo {
**public****static****void**main(String[]args) {
*writeFile*();
}
**public****static****void**writeFile() {
//创建字节输出流的对象
OutputStreamout=**null**;
**try**{
//构造器的演示
//out = new FileOutputStream(newFile("d.txt");
out=**new**FileOutputStream("d.txt");
//创建可以写入追加数据的构造器
//out = new FileOutputStream("d.txt",true);
//out = new FileOutputStream(newFile("d.txt"),true);

Stringstr="helloworld";
//把字符串变成字节数组
**byte**[]bs=str.getBytes();
//单个字节写入方法
**for**(**byte**b:bs) {
//需要处理IOexception异常
out.write(b);
}
//写入字节数组方法
out.write(bs);
//写入字节数组的一部分
//第一个是字节数组，第二个是开始索引，第三个是截取的长度
//注意数组越界问题
out.write(bs,8,2);
//最终写入内容helloworldhelloworldld
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(out!=**null**) {
**try**{
//关闭字节输出流需要处理异常
out.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}


