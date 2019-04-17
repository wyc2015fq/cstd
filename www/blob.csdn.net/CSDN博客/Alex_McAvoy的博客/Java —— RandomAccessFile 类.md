# Java —— RandomAccessFile 类 - Alex_McAvoy的博客 - CSDN博客





2018年10月31日 20:27:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

RandomAccessFile 类可以读取任意位置数据的文件，其既不是输入流的子类也不是输出流的子类。

该类支持随机访问的方式，程序可以跳到文件的任意地方来读、写文件，除此之外，支持只访问文件的部分内容，向已存在的文件后追加内容等。

RandomAccessFile 类的对象包含一个记录指针，用于标识当前读写出的位置，可以通过如下方法自由移动指针：
- getFilePointer()：获取文件记录指针的当前位置
- seek(long pos)：将文件记录指针定位到 pos 位置

# 【构造器】

创建 RandomAccessFile 类实例需要指定一个 mode 参数，该参数指定 RandomAccessFile 对象实例的访问模式
- r：只读形式打开
- rw：可读写形式打开
- rws：同步写入，即同步文件内容和元数据的更新
- rwd：更新同步写入，即同步文件内容的更新

构造方法：
- RandomAccessFile(String name,String mode)
- RandomAccessFile(File file,Sting mode)

# 【常用方法】
- getFilePointer()：返回文件中的当前偏移量
- length()：获取文件长度
- readByte()/writeByte()：从文件中读取/写入一个字节
- write(Byte bytes[])：将 byters.length 个字节写入文件
- readChar()/writeChar()：从文件中读取/写入一个字符
- writeChars(String str)：从文件中写入一个作为字符数据的字符串
- readInt()/writeInt()：从文件中读取/写入一个 int 值
- readLine()/writeLine()：从文件中读取/写入一个文本行
- readBoolean()/writeBoolean()：从文件中读取/写入一个布尔值
- readUTF()/writeUTF()：从文件中读取/写入一个 UTF 字符串
- close()：关闭文件

# 【实例】

```java
File file=new File("D:\\Test\\Example.txt");
RandomAccessFile raf=new RandomAccessFile(file,"rw");//创建随机访问文件为读写
long filePoint=0;//定义文件总长度变量
long filel=raf.length();//获取文件长度

while(filePoint<filel){
    String str=raf.readLine();//从文件中读取数据
    System.out.println(str);
    filepoint=raf.gerFilePointer();//更新文件长度变量
}
raf.close();
```





