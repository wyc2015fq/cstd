
# java中字符流输出流FileWriter详解 - 李昆鹏的博客 - CSDN博客


2018年04月04日 11:22:59[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：206


-------------------------------------java中字符流输出流FileWriter详解----------------------------

IO流
IO流在java中从输入输出角度分类：
1.    输入流：从文件中读取数据，或者从键盘输入数据都属于输入流
2.    输出流：向文件中写入数据
**IO****流在java****中从数据的角度来分类：**
**1.****字符流**
**文本，我们能读的懂的都可以认为是字符流**
**字符输入流的超类：Reader(****抽象类)**
**子类：FileReader****，BufferedReader**
**字符输出流的超类：writer(****抽象类)**
**子类：FileWriter****，BufferedWriter**
**2.****字节流**
**二进制的数据，这种数据我们一般读不懂。**
**比如：图片文件，MP3****文件。**
**字节输入流的超类：InputStream(****抽象类)**
**子类：FileInputStream**
**字节输出流的超类：OutputStream(****抽象类)**
**类：FileOutputStream**
**=========================================**
1.、字符流
使用字符流向一个文件输入helloworld
分析：
步骤：
1．需要创建文件
2．创建输出流对象
3．把流指向指定的文件
4．释放资源
输出字符流的FileWriter的构造器
|构造方法摘要
|
|FileWriter|(|File|file)
|根据给定的 File 对象构造一个 FileWriter 对象。
|
|FileWriter|(|File|file, boolean append)
|根据给定的 File 对象构造一个 FileWriter 对象。
|
|FileWriter|(|FileDescriptor|fd)
|构造与某个文件描述符相关联的 FileWriter 对象。
|
|FileWriter|(|String|fileName)
|根据给定的文件名构造一个 FileWriter 对象。
|
|FileWriter|(|String|fileName, boolean append)
|根据给定的文件名以及指示是否附加写入数据的 boolean 值来构造 FileWriter 对象。
|
第一种方法：
**public****class**IOcharDemo {
**public****static****void**main(String[]args) {
//创建文件
Filefile=**new**File("a.txt");
//创建输出流的对象
FileWriterfw=**null**;
**try**{
//为什么要处理异常？
//当我们在往文件对象写入的时候，文件对象被删除了，我们就要处理异常
//对于IO来说都有异常的问题，都要处理
fw=**new**FileWriter(file);
//把内容写入文件
fw.write("helloworld");
//字符流的特点：
//字符流在写完东西后必须要清空缓冲区，把内容写入到文件中
fw.flush();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
//释放支援
**if**(fw!=**null**) {
**try**{
//只要使用IO就必须要处理异常
//close会关闭输出流时会先清理缓冲区
fw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}

}


第二种方法
直接使用FileWriter的构造器创建文件
**public****class**IOcharDemo2 {
**public****static****void**main(String[]args) {
//创建输出流的对象
FileWriterfw=**null**;
**try**{
//为什么要处理异常？
//当我们在往文件对象写入的时候，文件对象被删除了，我们就要处理异常
//对于IO来说都有异常的问题，都要处理
//使用此构造器会自动的创建文件
**fw = new  FileWriter("b.txt");**
//把内容写入文件
fw.write("helloworld");
//字符流的特点：
//字符流在写完东西后必须要清空缓冲区，把内容写入到文件中
fw.flush();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
//释放支援
**if**(fw!=**null**) {
**try**{
//只要使用IO就必须要处理异常
//close会关闭输出流时会先清理缓冲区
fw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}
----------------------------------------
练习：向一个文件中写入一百个helloworld
注意 flush方法最好分段使用，如果写入文件量足够的大效率会大大下降
**public****class**IOcharDemo3 {
**public****static****void**main(String[]args) {
//创建输出流的对象
FileWriterfw=**null**;
**try**{
//为什么要处理异常？
//当我们在往文件对象写入的时候，文件对象被删除了，我们就要处理异常
//对于IO来说都有异常的问题，都要处理
//使用此构造器会自动的创建文件
fw=**new**FileWriter("b.txt");
//在文件中写入100个helloworld
**for**(**int**i= 0;i<100;i++) {
//把内容写入文件
fw.write("helloworld");
//每写入十条数据就将次缓冲的文件写入文件对象中
**if**(i% 10 == 0) {
fw.flush();
}
}
//把缓冲区残留的数据写入文件
fw.flush();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
//释放支援
**if**(fw!=**null**) {
**try**{
//只要使用IO就必须要处理异常
//close会关闭输出流时会先清理缓冲区
fw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}


