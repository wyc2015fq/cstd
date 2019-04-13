
# java入门教程-8.5Java面向字节流的应用 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:37:04[seven-soft](https://me.csdn.net/softn)阅读数：154个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



## 文件输入输出流
文件输入输出流 FileInputStream 和 FileOutputStream 负责完成对本地磁盘文件的顺序输入输出操作。
【例 10-5】通过程序创建一个文件，从键盘输入字符，当遇到字符“\#”时结束，在屏幕上显示该文件的所有内容（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-5.txt)）。
运行后在程序目录建立一个名称为 ep10_5 的文件，运行结果如图 10-8 所示：![图 10-8   例 10_5 运行结果](http://www.weixueyuan.net/uploads/allimg/130222/8-130222233111D3.jpg)
图 10-8  例 10_5 运行结果
FileDescriptor 是 java.io 中的一个类，该类不能实例化，其中包含三个静态成员：in、out 和err，分别对应于标准输入流、标准输出流和标准错误流，利用它们可以在标准输入输出流上建立文件输入输出流，实现键盘输入或屏幕输出操作。
【例 10-6】实现对二进制图形文件（.gif）的备份（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-6.txt)）。
运行后在程序目录备份了一个名称为 ep10_6_a.gif 的文件，运行结果如图 10-9 所示：![](http://www.weixueyuan.net/uploads/allimg/130222/8-130222233354Y7.png)
图 10-9  例 10_6 运行结果
## 过滤流
FilterInputStream 和 FileOutputStream 是 InputStream 和 OutputStream 的直接子类，分别实现了在数据的读、写操作的同时能对所传输的数据做指定类型或格式的转换，即可实现对二进制字节数据的理解和编码转换。
常用的两个过滤流是数据输入流 DataInputStream 和数据输出流 DataOutputStream。其构造方法为：
DataInputStream(InputStream
 in);  //创建新输入流，从指定的输入流 in 读数据
DataOutputStream(OutputStream out);  //创建新输出流，向指定的输出流 out 写数据
由于 DataInputStream 和 DataOutputStream 分别实现了 DataInput 和 DataOutput 两个接口（这两个接口规定了基本类型数据的输入输出方法）中定义的独立于具体机器的带格式的读写操作，从而实现了对不同类型数据的读写。由构造方法可以看出，输入输出流分别作为数据输入输出流的构造方法参数，即作为过滤流必须与相应的数据流相连。
DataInputStream 和 DataOutputStream 类提供了很多个针对不同类型数据的读写方法，具体内容读者可参看 Java 的帮助文档。
【例 10-7】将三个 int 型数字 100，0，-100 写入数据文件 ep10_6.dat 中（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-7.txt)）。
运行后在程序目录中生成数据文件 ep10_7.dat，用文本编辑器打开后发现内容为二进制的：
00 00 00 64 00 00 00 00 FF FF FF 9C。
【例 10-8】读取数据文件 ep10_6.dat 中的三个 int 型数字，求和并显示（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-8.txt)）。
运行结果：
三个数的和为：0
readInt 方法可以从输入输出流中读入 4 个字节并将其作为 int 型数据直接参与运算。由于已经知道文件中有 3 个数据，所以可以使用 3 个读入语句，但若只知道文件中是 int 型数据而不知道数据的个数时该怎么办呢？因为
 DataInputStream 的读入操作如遇到文件结尾就会抛出 EOFException 异常，所以可将读操作放入 try 中。
try{
while(true)
sum+=a.readInt();
}
catch(EOFException e){
System.out.pritnln("三个数的和为："+sum);
a.close();
}
EOFException 是 IOException 的子类，只有文件结束异常时才会被捕捉到，但如果没有读到文件结尾，在读取过程中出现异常就属于 IOException。
【例 10-9】从键盘输入一个整数，求该数的各位数字之和（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-9.txt)）。
运行结果：
请输入一个整数：26
842403082 的各位数字之和=31
需要注意的是，输入的数据 26 为变成了 842403082，原因在于输入数据不符合基本类型数据的格式，从键盘提供的数据是字符的字节码表示方式，若输入 26，只代表 2 和 6 两个字符的字节数据，而不是代表整数 26 的字节码。
若要从键盘得到整数需要先读取字符串，再利用其他方法将字符串转化为整数。
## 标准输入输出
System.in、System.out、System.err 这 3 个标准输入输流对象定义在 java.lang.System 包中，这 3 个对象在 Java 源程序编译时会被自动加载。标准输入：标准输入 System.in 是 BufferedInputStream 类的对象，当程序需要从键盘上读入数据时，只需要调用 System.in 的 read()方法即可，该方法从键盘缓冲区读入一个字节的二进制数据，返回以此字节为低位字节，高位字节为 0 的整型数据。
标准输出：标准输出 System.out 是打印输出流 PrintStream 类的对象。PrintStream 类是过滤输出流类 FilterOutputStream 的一个子类，其中定义了向屏幕输出不同类型数据的方法print()和 println()。
标准错误输出：System.err 用于为用户显示错误信息，也是由 PrintStream 类派生出来的错误流。Err 流的作用是使 print()和 println()将信息输出到 err 流并显示在屏幕上，以方便用户使用和调试程序。
【例 10-10】输入一串字符显示出来，并显示 System.in 和 System.out 所属的类（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-10.txt)）。
运行结果如图 10-10 所示：![图 10-10  例 10_10 运行结果](http://www.weixueyuan.net/uploads/allimg/130222/8-130222233615401.png)
图 10-10  例 10_10 运行结果
需要注意的是，输入了 3 个字符按回车后，输出的结果显示为 5 个字符。这是由于 Java 中回车被当作两个字符，一个是 ASCⅡ为 13 的回车符，一个是值为 10 的换行符。程序中 getClass()和 ToString()是
 Object 类的方法，作用分别是返回当前对象所对应的类和返回当前对象的字符串表示。

