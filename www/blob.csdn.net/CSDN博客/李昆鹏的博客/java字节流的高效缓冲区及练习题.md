
# java字节流的高效缓冲区及练习题 - 李昆鹏的博客 - CSDN博客


2018年04月12日 04:52:44[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：360


----------------------java字节流的高效缓冲区及练习题--------------------------
**高效缓冲区输入输出流构造器**
**BufferedInputStream**`(`InputStream`in)`
创建一个`BufferedInputStream`并保存其参数，即输入流`in`，以便将来使用。

**BufferedOutputStream**`(`OutputStream`out)`
创建一个新的缓冲输出流，以将数据写入指定的底层输出流。

范例：使用高效缓冲区拷贝文件
**public****class**CopyFileDemo2{
**public****static****void**main(String[]args) {
//计算文件拷贝时间
**long**startTime= System.*currentTimeMillis*();
*copyFile*();
**long**endTime= System.*currentTimeMillis*();
System.**out**.println("文件拷贝时间："+ (endTime-startTime));
}

**public****static****void**copyFile() {
//声明高效缓冲区字节输入流
BufferedInputStreamin=**null**;
//声明高效缓冲区字节输出流
BufferedOutputStreamout=**null**;
**try**{
//创建字节输入输出流对象
in=**new**BufferedInputStream(**new**FileInputStream("src/com/rl/byt/out/InputStreamDemo2.java"));
out=**new**BufferedOutputStream(**new**FileOutputStream("InputStreamDemo2.java"));
**byte**[]bs=**new****byte**[1024];
**int**len= -1;
**while**((len=in.read(bs)) != -1) {
//把字节数组中的数据接入到文件中
out.write(bs,0,len);
}
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(out!=**null**) {
out.close();
}
**if**(in!=**null**) {
in.close();
}
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
=========================================
作业题：从键盘输入信息存储到文件中，学生按照年龄排序。
创建一个学生类
**public****class**Student**implements**Comparable<Student>{
**private**Stringname;
**private**Integerage;
@Override
**public****int**compareTo(Studento) {
**int**num=**this**.age-o.age;
**if**(num== 0)
num=**this**.name.compareTo(o.name);
**return**num;
}
**public**String getName() {
**return**name;
}
**public****void**setName(Stringname) {
**this**.name=name;
}
**public**Integer getAge() {
**return**age;
}
**public****void**setAge(Integerage) {
**this**.age=age;
}
}
--------------------------------------------
创建一个输入并且向文件中写入学生信息的类
**public****class**ScannerTest {
**public****static****void**main(String[]args) {
//定义学生类的集合
TreeSet<Student>ts=**new**TreeSet<Student>();
//创建键盘输入对象
Scannersc=**new**Scanner(System.**in**);
System.**out**.println("请输入你要输入几个学生：");
//获得要输入学生的数量
**int**count=sc.nextInt();
**for**(**int**i= 0;i<count;i++) {
sc=**new**Scanner(System.**in**);
//获得学生的姓名
System.**out**.println("请输入学生的姓名：");
Stringname=sc.nextLine();
//获得学生的年龄
System.**out**.println("请输入学生的年龄：");
Integerage=sc.nextInt();
//创建一个学生对象
Students=**new**Student();
s.setAge(age);
s.setName(name);
//把学生加入到集合中
ts.add(s);
}

//将学生写入文本中
BufferedWriterbw=**null**;
**try**{
//创建高效缓冲区字符输出流
bw=**new**BufferedWriter(**new**FileWriter("Student.txt"));
//遍历学生类的集合
**for**(Students:ts) {
//将学生信息写入文本并且换行
bw.write(s.getName() +"------"+s.getAge());
bw.newLine();
}
//释放缓冲区
bw.flush();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
//关闭高效缓冲区字符输出流
**try**{
**if**(bw!=**null**)
bw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
System.**out**.println("写入文件完毕！");
}
}


