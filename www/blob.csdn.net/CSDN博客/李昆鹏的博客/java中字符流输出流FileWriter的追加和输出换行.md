
# java中字符流输出流FileWriter的追加和输出换行 - 李昆鹏的博客 - CSDN博客


2018年04月04日 11:25:25[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1630


-------------------------------------java中字符流输出流FileWriter的追加和输出换行----------------------------
**在文件对象中写入内容的时候，入过文件对象已经有了其他的内容，这时候我们在使用write****方法写入数据的时候，会覆盖掉原有的内容。**
**这个时候我们就可以使用FileWriter****的追加构造器。**
|构造方法摘要
|
|FileWriter|(|File|file, boolean append)
|根据给定的 File 对象构造一个 FileWriter 对象。
|
|FileWriter|(|String|fileName, boolean append)
|根据给定的文件名以及指示是否附加写入数据的 boolean 值来构造 FileWriter 对象。
|
例子：
**public****class**IOcharDemo4 {
**public****static****void**main(String[]args) {
//创建输出流的对象
FileWriterfw=**null**;
**try**{
//创建输出流的追加构造器
//也可以使用这种构造器fw= new FileWriter(new File("b.txt"),true);
fw=**new**FileWriter("b.txt",**true**);
**for**(**int**i= 0;i<100;i++) {
//在b.txt文件中追加100个I Love You
fw.write("I Love You");
**if**(i% 10 == 0) {
fw.flush();
}
}
fw.flush();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(fw!=**null**) {
**try**{
fw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}

----------------------------------------------------------
**向文件对象写入数据换行**
**把文本写入文件中\n****代表换行。**
**问题是不同的环境下换行的方式也不一样**
**不同平台的换行方式**
**Windows:\r\n****（建议使用）**
**Linux:\n**
**Mac:\r  (****苹果笔记本)**
**内容换行为了兼容记事本我们建议使用\r\n**

**public****class**IOcharDemo5 {
**public****static****void**main(String[]args) {
//创建输出流的对象
FileWriterfw=**null**;
**try**{
//创建输出流
fw=**new**FileWriter("b.txt");
**for**(**int**i= 0;i<100;i++) {
**//****输出换行多加****\r****代表空出一行****,\r****后面加****\n****，任何平台支持换行**
**//****所以内容换行为了兼容记事本我们建议使用****\r\n**
**fw.write("****我爱你****"+ i + "\r\n\r\n");**
**if**(i% 10 == 0) {
fw.flush();
}
}
fw.flush();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(fw!=**null**) {
**try**{
fw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}


