
# java中BufferedWriter和 BufferedReader高效缓冲区流 - 李昆鹏的博客 - CSDN博客


2018年04月06日 12:00:54[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：505


-------------------------------------java中BufferedWriter和 BufferedReader高效缓冲区流 ----------------------------
高效缓存区输入输出流
BufferedReader用于输入
**从字符输入流中读取文本，缓冲各个字符，从而实现字符，数组和行的高效读取。**
BufferedWriter用于输出
**将文本写入字符输出流，缓冲各个字符，从而提供单个字符、数组和字符串的高效写入。**

**------------------------------------------------------------------------**
BufferedWriter
|构造方法摘要
|
|BufferedWriter|(|Writer|out)
|创建一个使用默认大小输出缓冲区的缓冲字符输出流。
|
|BufferedWriter|(|Writer|out, int sz)
|创建一个使用给定大小输出缓冲区的新缓冲字符输出流。
|

|方法摘要
|
|void
|close|()
|关闭此流，但要先刷新它。
|
|void
|flush|()
|刷新该流的缓冲。
|
|void
|newLine|()
|写入一个行分隔符。
|
|void
|write|(char[] cbuf, int off,  int len)
|写入字符数组的某一部分。
|
|void
|write|(int c)
|写入单个字符。
|
|void
|write|(|String|s, int off, int len)
|写入字符串的某一部分。
|

范例：使用高效缓冲区流来向文件中写入一句话
**public****class**BufferedWriterDemo {
**public****static****void**main(String[]args) {
*writer*();
}
/**
*使用高效缓冲流来向d.txt的文件中导入“java太好学了，真简单！”
*/
**public****static****void**writer() {
//创建文件输出流
FileWriterfw=**null**;
BufferedWriterbw=**null**;
**try**{
fw=**new**FileWriter("d.txt");
//创建高效缓冲区流的对象
bw=**new**BufferedWriter(fw);
//输出数据
bw.write("java太好学了，真简单！");
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

-----------------------------------------------------------------------------
BufferedReader
|构造方法摘要
|
|BufferedReader|(|Reader|in)
|创建一个使用默认大小输入缓冲区的缓冲字符输入流。
|
|BufferedReader|(|Reader|in, int sz)
|创建一个使用指定大小输入缓冲区的缓冲字符输入流。
|

|方法摘要
|
|void
|close|()
|关闭该流并释放与之关联的所有资源。
|
|void
|mark|(int readAheadLimit)
|标记流中的当前位置。
|
|boolean
|markSupported|()
|判断此流是否支持 mark() 操作（它一定支持）。
|
|int
|read|()
|读取单个字符。
|
|int
|read|(char[] cbuf, int off,  int len)
|将字符读入数组的某一部分。
|
|String
|readLine|()
|读取一个文本行。
|
|boolean
|ready|()
|判断此流是否已准备好被读取。
|
|void
|reset|()
|将流重置到最新的标记。
|
|long
|skip|(long n)
|跳过字符。
|

范例：使用高效缓冲区流读取文件中的内容。
**public****class**BufferedReaderDemo {
**public****static****void**main(String[]args) {
*reader*();
System.**out**.println();
*reader1*();
//输出
//java太好学了，真简单！
//java太好学了，真简单！
}
/**
*使用高效缓冲区流读取文件中的内容。
*/
**public****static****void**reader() {
//创建文件输出流
FileReaderfr=**null**;
BufferedReaderbr=**null**;
**try**{
fr=**new**FileReader("d.txt");
//创建高效缓冲区流的对象
br=**new**BufferedReader(fr);
//循环打印输入的字符
**int**num= -1;
**while**((num=br.read()) != -1 ) {
System.**out**.print((**char**)num);
}
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

/**
*使用高效缓冲区流字符数组方式读取文件中的内容。
*/
**public****static****void**reader1() {
//创建文件输出流
FileReaderfr=**null**;
BufferedReaderbr=**null**;
**try**{
fr=**new**FileReader("d.txt");
//创建高效缓冲区流的对象
br=**new**BufferedReader(fr);
//循环打印输入的字符
**int**len= -1;
**char**[]chs=**new****char**[1024];
**while**((len=br.read(chs)) != -1 ) {
System.**out**.print(**new**String(chs,0,len));
}
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


