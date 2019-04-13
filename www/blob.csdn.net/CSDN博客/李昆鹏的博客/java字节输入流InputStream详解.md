
# java字节输入流InputStream详解 - 李昆鹏的博客 - CSDN博客


2018年04月12日 04:42:34[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1321


-------------------------java字节输入流InputStream详解---------------------
因为InputStream是抽象类，我们要学习的是它的子类。
构造器
|FileInputStream|(|File|file)
|通过打开一个到实际文件的连接来创建一个 FileInputStream，该文件通过文件系统中的 File 对象 file 指定。
|
|FileInputStream|(|FileDescriptor|fdObj)
|通过使用文件描述符 fdObj 创建一个  FileInputStream，该文件描述符表示到文件系统中某个实际文件的现有连接。
|
|FileInputStream|(|String|name)
|通过打开一个到实际文件的连接来创建一个 FileInputStream，该文件通过文件系统中的路径名 name 指定。
|

读取方法
|int
|read|()
|从此输入流中读取一个数据字节。
|
|int
|read|(byte[] b)
|从此输入流中将最多 b.length 个字节的数据读入一个  byte 数组中。
|
|int
|read|(byte[] b, int off, int len)
|从此输入流中将最多 len 个字节的数据读入一个 byte 数组中。
|

范例： 使用字节输入流来读取文件并且输出到控制台
**public****class**InputStreamDemo2 {
**public****static****void**main(String[]args) {
*readFile2*();
}
/**
*单个字节读取
*效率低
*/
**public****static****void**readFile() {
//创建字节输入流的对象
InputStreamin=**null**;
**try**{
//构造器的演示
in=**new**FileInputStream("d.txt");
//从文件中读取一个字节，返回字节所对应的是ascii码
//如果读取的字节超过了ascii码就会返回一个？，所以单个字节读取方法是有限制的
**int**num=in.read();
System.**out**.println(num);
//输出104
//将字节转换成字符输出
System.**out**.println((**char**)num);
//输出h

}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(in!=**null**) {
**try**{
//关闭字节输出流需要处理异常
in.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
/**
*字节数组读取
*效率高
*/
**public****static****void**readFile1() {
//创建字节输入流的对象
InputStreamin=**null**;
**try**{
//构造器的演示
in=**new**FileInputStream("d.txt");
//定义一个字节数组
**byte**[]bs=**new****byte**[1024];
//向字节数组中存储数据，返回读取的长度
**int**len=in.read(bs);
System.**out**.println(len);
//输出22
System.**out**.println(**new**String(bs,0,len));
//输出helloworldhelloworldld
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(in!=**null**) {
**try**{
//关闭字节输出流需要处理异常
in.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
/**
*读取指定的字节数量
*效率高
*/
**public****static****void**readFile2() {
//创建字节输入流的对象
InputStreamin=**null**;
**try**{
//构造器的演示
in=**new**FileInputStream("d.txt");
//定义一个字节数组
**byte**[]bs=**new****byte**[1024];
//从输入字节流的对象中读取指定的字节数,从字节输入流来取三个字节
//放入字节数组的指定索引开始位置，也就是第二个参数
**int**len=in.read(bs,1,3);
//打印bs字节数组
**for**(**byte**b:bs) {
System.**out**.print(b);
//输出01041011080000000000000000...
}
System.**out**.println();
System.**out**.println(len);
//输出3
System.**out**.println(**new**String(bs,0,len));
//输出(空格+ he)
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(in!=**null**) {
**try**{
//关闭字节输出流需要处理异常
in.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}


