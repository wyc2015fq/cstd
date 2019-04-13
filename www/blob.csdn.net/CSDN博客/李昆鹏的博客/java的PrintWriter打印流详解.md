
# java的PrintWriter打印流详解 - 李昆鹏的博客 - CSDN博客


2018年04月12日 18:07:56[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：927


-----------------------------java的PrintWriter打印流详解------------------------------------
**打印流：只做输出没有输入**
**打印流分为字节打印流和字符打印流**
**PrintWriter****：字符打印流**
**特点**
**1.****可以打印各种数据类型。**
**2.****封装了字符输出流，还可以做字符流和字节流的转换**
**3.****可以使用自动刷新。只有在调用println****、printf或者format的其中一个方法时才可能完成此操作。**
**4.****可以直接向文件中写数据。**

**使用字符打印流向文件中写入数据**

**public****class**PrintDemo {
**public****static****void**main(String[]args) {
PrintWriterpw=**null**;
**try**{
//创建字符打印流对象
pw=**new**PrintWriter("e.txt");
//向文件中分别打印boolean、字符、int、字符串
pw.print(**true**);
pw.print('a');
pw.print(12);
pw.print("李昆鹏");
//刷新
pw.flush();
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**finally**{
//字符打印流也是需要关闭的，但是不用处理异常
**if**(pw!=**null**)
pw.close();
}
}
}

**范例：从文件中读取数据并且打印在控制台**

**public****class**PrintDemo2 {
**public****static****void**main(String[]args) {
BufferedReaderbr=**null**;
PrintWriterpw=**null**;
**try**{
//创建高效缓冲区字符输入流对象
br=**new**BufferedReader(**new**FileReader("Student.txt"));
//创建打印流对象
//pw= new PrintWriter(System.out);
//设置自动刷新的打印流在文件对象后面加true
pw=**new**PrintWriter(System.**out**,**true**);
Stringline=**null**;
**while**((line=br.readLine()) !=**null**) {
pw.println(line);
//当使用自动刷新构造器时就不需要手动刷新
//pw.flush();
}
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(br!=**null**)
br.close();
**if**(pw!=**null**)
pw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}

**范例：使用打印流来复制文本文件**
**public****class**PrintDemo3 {
**public****static****void**main(String[]args) {
BufferedReaderbr=**null**;
PrintWriterpw=**null**;
**try**{
//创建高效缓冲区字符输入流对象
br=**new**BufferedReader(**new**FileReader("Student.txt"));
//创建打印流对象
//pw= new PrintWriter(System.out);
//设置自动刷新的打印流在文件对象后面加true
pw=**new**PrintWriter(**new**FileWriter("Student1.txt"));
Stringline=**null**;
**while**((line=br.readLine()) !=**null**) {
pw.println(line);
//当使用自动刷新构造器时就不需要手动刷新
//pw.flush();
}
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(br!=**null**)
br.close();
**if**(pw!=**null**)
pw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}



