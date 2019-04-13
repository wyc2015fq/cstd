
# java中字符流输出流FileWriter的5种写入方法 - 李昆鹏的博客 - CSDN博客


2018年04月04日 11:47:30[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1559


-------------------------------------java中字符流输出流FileWriter的5种写入方法 ----------------------------
FileWriter的写入功能
|void
|write|(char[] cbuf)
|写入字符数组。
|
|abstract  void
|write|(char[] cbuf, int off,  int len)
|写入字符数组的某一部分。
|
|void
|write|(int c)
|写入单个字符。
|
|void
|write|(|String|str)
|写入字符串。
|
|void
|write|(|String|str, int off,  int len)
|写入字符串的某一部分。
|
五种写入方法的例子:

public class IOcharDemo6 {
public  static void main(String[] args) {
//创建输出流的对象
FileWriter  fw = null;
try{
//创建输出流
fw = new FileWriter("test.txt");
//写入字符数组
char[]  chs = {'a','b','c','d'};
fw.write(chs);
//写入的内容  abcd
//把一部分字符数组写入到文件中
//第一个参数是字符数组，第二个是开始索引，
//第三个是从开始索引开始取得字符串的长度
fw.write(chs,1,3);
//文本内容  abcdbcd
//写入int类型会被转换成相对应的ascll码的字符
fw.write(100);
//文本内容   abcdbcdd
//如果要写入100，我们可以用写入字符串方式
fw.write("100");
//文本内容  abcdbcdd100
//截取一段字符串写入
//第一个字符串，第二个是开始索引，
//第三个是从开始索引开始取得字符串的长度
fw.write("helloworld",2,2);
//文本内容  abcdbcdd100ll

//把缓冲区残留的数据写入文件
fw.flush();
}catch (IOException e) {
e.printStackTrace();
}finally{
if(fw!= null) {
try{
fw.close();
}catch (IOException e) {
e.printStackTrace();
}
}
}
}
}


