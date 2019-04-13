
# java字节流做文件拷贝 - 李昆鹏的博客 - CSDN博客


2018年04月12日 04:44:21[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：85


------------------------------java字节流做文件拷贝--------------------------------

**字节流拷贝不但能能对文件进行拷贝，还能对字节文件进行拷贝，比如：图片，MP3****，MP4。**

范例： 使用字节流来拷贝文件
**public****class**CopyFileDemo {
**public****static****void**main(String[]args) {
*copyFile*();
}
/**
*
*创建字节输入流的对象
*创建字节输出流的对象
*读取数据并且写入数据
*关闭资源
*
*/
**public****static****void**copyFile() {
//声明字节输入流
InputStreamin=**null**;
//声明字节输出流
OutputStreamout=**null**;
**try**{
//创建字节输入输出流对象
in=**new**FileInputStream("src/com/rl/byt/out/InputStreamDemo2.java");
out=**new**FileOutputStream("InputStreamDemo2.java");
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


