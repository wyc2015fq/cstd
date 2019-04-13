
# java中字符流输入流FileReader详解 - 李昆鹏的博客 - CSDN博客


2018年04月06日 11:52:19[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：110标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


-------------------------------------java中字符流输入流FileReader详解 ----------------------------

Reader‘是一个抽象类，我们需要学习它的孙子类FileReader’
FileReader的爷爷类是Reader
FileReader的构造器
|构造方法摘要
|
|FileReader|(|File|file)
|在给定从中读取数据的|File|的情况下创建一个新|FileReader|。
|
|FileReader|(|String|fileName)
|在给定从中读取数据的文件名的情况下创建一个新|FileReader|。
|
读取文件helloworld.txt打印到控制台。
分析：
1.    创建输入流对象。
2.    读取数据
3.    关闭输入流

**public****class**IOCharReaderDemo {
**public****static****void**main (String[]args) {
*readFile*();
}
**public****static****void**readFile() {
FileReaderfr=**null**;
**try**{
//创建FileReader读取对象
//也可以使用这种构造器
//fr= new FileReader(new File("helloword.txt"))
fr=**new**FileReader("helloworld.txt");
//读取文件也会产生异常，我们需要处理
//直接再加一个catch (IOException e)
//读取单个字符，返回的是字符的ascll码
**int**num=fr.read();
//打印读取的字符
//num是字符所对应的ASCll码
System.**out**.println(num);//输出97
//我们可以将它转换从字符
System.**out**.println((**char**)num);//输出a
//我们再读取一遍fr
num=fr.read();
//结果输出的是下一个字符
System.**out**.println((**char**)num);//输出b
//我们可以使用while循环来读取所有的字符
//循环结束条件，如果整个文本读取完毕最后没有字符的时候，read方法会返回-1，
**int**num1= 0;
//通过循环来读取字符，判断跳出循环的标志就是num1 = -1;
**while**((num1=fr.read()) != -1) {
//打印字符
System.**out**.print((**char**)num1);
//输出cdbcdd100ll
}
//处理创建时可能产生的异常
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
//处理读取时可能产生的异常
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(fr!=**null**) {
**try**{
//释放资源
fr.close();
//处理关闭时可能产生的异常
//比如：在关闭的时候文件正在被写入东西或者别人把这个文件给删除了
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}

---------------------------------------------------------------------------
字符输入流的读取方式
**Read()****方法在读取字符的时候，换行也能读取到，相对应的ascll****码也能读取到。**
**如果读取的是中文字符，就会返回中文字符相对应的unicode****码，unicode是在ascll码上的扩展。**
|int
|read|()
|读取单个字符。
|
|int
|read|(char[] cbuf)
|将字符读入数组。
|

**使用 read(char[] cbuf)****读取字符方法比read()方法读取速度要快很多。**
例子：
**public****class**IOCharReaderDemo2 {
**public****static****void**main (String[]args) {
*readFile*();

}
**public****static****void**readFile() {
FileReaderfr=**null**;
**try**{
//创建FileReader读取对象
fr=**new**FileReader("helloworld.txt");
//helloworld.txt文件中的内容是：abcdbcdd100ll
//创建一个字符的数组
**char**[]chs=**new****char**[5];
//读取五个字符放入字符数组中,返回的数值是读取到的字符数量
**int**num=fr.read(chs);
//把字符数组转换成字符串
Stringstr=**new**String(chs);
System.**out**.println(num);
//输出5
System.**out**.println(str);
//输出abcdb
//第二次读取
//把上一次的字符数组全部替换了
num=fr.read(chs);
str=**new**String(chs);
System.**out**.println(num);
//输出5
System.**out**.println(str);
//输出cdd10
//第三次读取
//读取到的是3个字符
num=fr.read(chs);
//因为只读取到3个字符，所以最后两个字符还是保留了上一次读取的字符
str=**new**String(chs);
System.**out**.println(num);
//输出3
System.**out**.println(str);
//输出0ll10
//我们可以使用str=new String(chs,0,num);构造器来解决这种问题
str=**new**String(chs,0,num);
System.**out**.println(str);
//输出0ll
//如果再次读取就没有数据，num就会是-1.
//使用while循环读取文件信息
FileReaderfr1=**new**FileReader("helloworld2.txt");
//hellworld2中的文件内容是abcdbcdd100ll
**char**[]chas1=**new****char**[1024];
**int**len= -1 ;
//循环条件判断的边界是fr.read(chas1)如果返回的数值是-1说明文件已经读取完毕
**while**((len=fr1.read(chas1)) != -1){
System.**out**.print(**new**String(chas1,0,len));
//输出abcdbcdd100ll
}
//处理创建时可能产生的异常
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
//处理读取时可能产生的异常
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**if**(fr!=**null**) {
**try**{
//释放资源
fr.close();
//处理关闭时可能产生的异常
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
}

|abstract  int
|read|(char[] cbuf, int off,  int len)
|将字符读入数组的某一部分。
|
此方法是读取字符串数组中的一部分。很少使用。


