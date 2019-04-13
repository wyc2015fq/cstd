
# java使用字符流来做文件的拷贝 - 李昆鹏的博客 - CSDN博客


2018年04月06日 11:55:09[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：167


-------------------------------------java使用字符流来做文件的拷贝 ----------------------------
字符流文件拷贝只能拷贝文本类型，像MP3，MP4等等需要使用字节流。
范例：把一个java文件拷贝到项目的根目录
分析：
1.    创建字符输入流的对象。
2.    创建字符输出流的对象。
3.    把输入流输入的数据写入输出流中。
4.    关闭资源。
第一种方式
**public****class**CopyFile {
**public****static****void**main(String[]args) {
*copy*();
System.**out**.println("done");
}
**public****static****void**copy() {
//声明文件读取对象
Readerreader=**null**;
//声明文件输出对象
FileWriterwriter=**null**;
**try**{
//创建读取文件对象
reader=**new**FileReader("src/com/rl/io/cha/reader/IOCharReaderDemo.java");
//创建写入文件对象
writer=**new**FileWriter("IOCharWriterDemo.java");
**int**num= -1;
//循环读取字符
**while**((num=reader.read()) != -1) {
//读取字符写入到输出文件中
writer.write(num);
}
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
//释放资源,先关闭写，再关闭读取。
**if**(writer!=**null**)
//关闭输出对象
writer.close();
**if**(reader!=**null**)
//关闭读取对象
reader.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
---------------------------------------------------
第二种方式
使用字符数组读取方式
效率高，建议使用
**public****class**CopyFile2 {
**public****static****void**main(String[]args) {
*copy*();
System.**out**.println("done");
}
**public****static****void**copy() {
//声明文件读取对象
Readerreader=**null**;
//声明文件输出对象
FileWriterwriter=**null**;
**try**{
//创建读取文件对象
reader=**new**FileReader("src/com/rl/io/cha/reader/IOCharReaderDemo.java");
//创建写入文件对象
writer=**new**FileWriter("IOCharWriterDemo.java");
//定义每次读取长度的对象
**int**len= -1;
//设置一个存储读取内容的字符数组
**char**[]chs=**new****char**[1024];
//循环读取字符
//当len不等于-1就读取
**while**((len=reader.read(chs)) != -1) {
//把读取的文件写入到目标文件中
writer.write(chs,0,len);
}
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
//释放资源,先关闭写，再关闭读取。
**if**(writer!=**null**)
//关闭输出对象
writer.close();
**if**(reader!=**null**)
//关闭读取对象
reader.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}


