
# java中高效缓冲区的扩展方法和文本复制 - 李昆鹏的博客 - CSDN博客


2018年04月06日 18:50:47[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：109


-------------------------------------java中高效缓冲区的扩展方法和文本复制 ----------------------------
高效缓冲区的输出流BufferedWriter的扩展方法newLine()
newline()方法可以输出换行。相当于\r\n
范例：向文件写入十个“java太好学了，真简单！”，并要求换行
**public****class**BufferedWriterDemo2 {
**public****static****void**main(String[]args) {
*writer*();
}
/**
*使用高效缓冲流来向d.txt的文件中导入10个“java太好学了，真简单！”
*/
**public st****atic****void**writer() {
//创建文件输出流
FileWriterfw=**null**;
BufferedWriterbw=**null**;
**try**{
fw=**new**FileWriter("d.txt");
//创建高效缓冲区流的对象
bw=**new**BufferedWriter(fw);
**for**(**int**i= 0;i< 10;i++) {
//输出数据(写入10个“java太好学了，真简单！”)
bw.write("java太好学了，真简单！");
//以前我们需要使用"\r\n"来换行
//高效缓冲区给我们提供了newLine()方法来换行
//相当于\r\n
bw.newLine();
}
//清空缓冲区
bw.flush();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(bw!=**null**) {
**try**{
//关闭高效缓冲区的流就会自动关闭FileWriter
bw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}
----------------------------------------------------
使用高效缓冲区输入流的扩展方法readLine()读取文档数据
readLine()方法可以读取一行数据。
**public****class**BufferedReaderDemo2 {
**public****static****void**main(String[]args) {
*reader*();
}
/**
*使用高效缓冲区readLine()方法来流读取文件中的内容。
*/
**public****static****void**reader() {
//创建文件输出流
FileReaderfr=**null**;
BufferedReaderbr=**null**;
**try**{
fr=**new**FileReader("d.txt");
//创建高效缓冲区流的对象
br=**new**BufferedReader(fr);
//使用高效输入流可以读取一行数据,再次使用readLie()方法会读取到下一行的数据
Stringline=br.readLine();
System.**out**.println(line);
//循环读取全部数据
//当没有数据可以读取的时候会返回null
**while**((line=br.readLine()) !=**null**)
System.**out**.println(line);
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(br!=**null**) {
**try**{
//关闭高效缓冲区的流就会自动关闭FileWriter
br.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}
---------------------------------------------------------
范例：使用高效缓冲区流来复制文件
**public****class**CopyFile3 {
**public****static****void**main(String[]args) {
*copy*();
System.**out**.println("done");
}
**public****static****void**copy() {
//声明文件读取对象
Readerreader=**null**;
//声明高效缓冲区读取对象
BufferedReaderbr=**null**;
//声明文件输出对象
FileWriterwriter=**null**;
//声明高效缓冲区输出对象
BufferedWriterbw=**null**;
**try**{
//创建读取文件对象
reader=**new**FileReader("src/com/rl/io/cha/reader/IOCharReaderDemo.java");
//创建高效缓冲区读取文件对象
br=**new**BufferedReader(reader);
//创建写入文件对象
writer=**new**FileWriter("CopyIOCharWriterDemo.java");
//创建高效缓冲区写入文件对象
bw=**new**BufferedWriter(writer);
Stringline=**null**;
//循环读取字符
**while**((line=br.readLine()) !=**null**) {
//向文件写入一行数据
bw.write(line);
//写入换行
bw.newLine();
}
//释放缓冲区
bw.flush();
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
//关闭高效缓冲区的流对象时会自动关闭输入、输出流的对象
//释放资源,先关闭写，再关闭读取。
**if**(bw!=**null**)
//关闭输出对象
bw.close();
**if**(br!=**null**)
//关闭读取对象
br.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}


