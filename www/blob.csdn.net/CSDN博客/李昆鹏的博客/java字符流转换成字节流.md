
# java字符流转换成字节流 - 李昆鹏的博客 - CSDN博客


2018年04月12日 12:44:06[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：523


--------------------------------java字符流转换成字节流------------------
**OutputStreamWriter是字符流通向字节流的桥梁**
范例：从一个文件中读取内容使用标准字节流来输出到控制台（使用字符串操作）；
**public  c****lass**ConverterOutDemo {
**public****static****void**main(String[]args) {
BufferedReaderbr=**null**;
BufferedWriterbw=**null**;
**try**{
br=**new**BufferedReader(**new**FileReader("c.txt"));
//创建字符流转换成字节流
bw=**new**BufferedWriter(**new**OutputStreamWriter(System.**out**));
Stringline=**null**;
**while**((line=br.readLine())!=**null**) {
bw.write(line);
bw.newLine();
bw.flush();
}
}**catch**(FileNotFoundExceptione) {
e.printStackTrace();
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(bw!=**null**)
bw.close();
**if**(br!=**null**)
br.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}

}
}

