
# java字节流转换成字符流- - 李昆鹏的博客 - CSDN博客


2018年04月12日 12:43:03[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：202


--------------------------------java字节流转换成字符流-------------------------------
**InputStreamReader是字节流通向字符流的桥梁。**
范例：把从键盘输入的文本写入到文件中
**public****class**ScannerDemo {
**public****static****void**main(String[]args) {
//创建Scanner对象
Scannersc=**new**Scanner(System.**in**);
BufferedWriterbw=**null**;
**try**{
bw=**new**BufferedWriter(**new**FileWriter("c.txt"));
Stringline=**null**;
**while**((line=sc.nextLine()) !=**null**) {
//给出退出条件
**if**("exit".equals(line)) {
**break**;
}
bw.write(line);
bw.newLine();
//释放缓冲区
bw.flush();
}
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(bw!=**null**)
bw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}

=============================================================
范例：把字节流转换成字符流
**public****class**ConverterInDemo {
**public****static****void**main(String[]args) {
InputStreamis= System.**in**;
//要想使用字符流的高效缓冲区来操作字节流需要转换
BufferedReaderbr=**new**BufferedReader(**new**InputStreamReader(is));
//定义要写入的文件流
BufferedWriterbw=**null**;
Stringline=**null**;
**try**{
bw=**new**BufferedWriter(**new**FileWriter("b.txt"));
**while**((line=br.readLine()) !=**null**) {
**if**("exit".equals(line))
**break**;
bw.write(line);
bw.newLine();
bw.flush();
}
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(br!=**null**)
br.close();
**if**(bw!=**null**)
bw.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}

}
}

