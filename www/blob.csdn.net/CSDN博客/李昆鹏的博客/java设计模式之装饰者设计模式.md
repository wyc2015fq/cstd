
# java设计模式之装饰者设计模式 - 李昆鹏的博客 - CSDN博客


2018年04月12日 10:04:54[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：63标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


--------------------------java设计模式之装饰者设计模式--------------------------
Decorator装饰器
**装饰者模式的目的是给类的功能增强。**
**继承也是给类的功能增强。**

比如：
Writer类是写文件的类，有三个子类TxtWriter,MP3Writer, AVIWriter.
在Writer中定义了写文件的标准，三个子类分别去实现Writer中的方法。
使用继承方式类对类增强
Writer
|--TxtWriter
|--MP3Writer
|--AVIWriter
通过继承方式加入高效缓冲区
Writer
|--TxtWriter
|--BufferedTxtWriter
|--MP3Writer
|--BufferedMP3Writer
|--AVIWriter
|--BufferedAVIWriter
如果想继续添加写文件的子类，如果想要对它做增强，还需要创建相应的缓冲区。
装饰器的设计模式可以解决我们使用高效缓冲区，但是不需要每一个写文件的类都添加高效缓冲区。
如果写文件的缓冲区是同样的原理，那么我们就可以把缓冲区抽取出来作为装饰器。
角色：
1.抽象构件角色（定义写文件标准）：Writer
2.具体的构件角色（实现写文件标准）：三个子类TxtWriter, MP3Writer, AVIWriter.
3.装饰角色（高效缓冲区）;
注意：也可以定义成抽象的，如果是抽象的就必须要有具体的抽象角色。
4.具体的装饰角色：来实现抽象装饰器中的增强方法。

代码：
创建writer类
**public****abstract****class**Writer {
/**
*写文件的方法
*/
**public****abstract****void**writeFile();
}
创建Writer的子类TxtWriter,MP3Writer, AVIWriter.
**public****class**TxtWriter**extends**Writer {
@Override
**public****void**writeFile() {
System.**out**.println("写入文本文件");
}
}
----------------
**public****class**MP3Writer**extends**Writer {
@Override
**public****void**writeFile() {
System.**out**.println("写入MP3文件");
}
}
----------------

创建装饰器（高效缓冲区）
**public****class**BufferedWriterFile**extends**Writer {
**private**Writerwriter;
**public**BufferedWriterFile(Writerwriter) {
**this**.writer=writer;
}
@Override
**public****void**writeFile() {
**this**.bufferedWriter();
writer.writeFile();
}
**public****void**bufferedWriter() {
System.**out**.println("加入高效缓冲区");
}
}

创建测试类
**public****class**Client {
**public****static****void**main(String[]args) {
Writerwriter=**new**BufferedWriterFile(**new**TxtWriter());
Writerwriter1=**new**BufferedWriterFile(**new**MP3Writer());
writer.writeFile();
writer1.writeFile();
//输出
//加入高效缓冲区
//写入文本文件
//加入高效缓冲区
//写入MP3文件
}
}

=================================================================
**抽象装饰者设计代码演示：**
建立抽象装饰类
**public****abstract****class**BufferedWriterFile**extends**Writer {
**private**Writerwriter;
**public**BufferedWriterFile(Writerwriter) {
**this**.writer=writer;
}
@Override
**public****void**writeFile() {
**this**.bufferedWriter();
writer.writeFile();
**this**.clean();
}
/**
*高效缓冲的方法
*/
**public****abstract****void**bufferedWriter();
**public****abstract****void**clean();
}

建立具体实现抽象装饰类

**public****class**BufferedWriterFileImpl**extends**BufferedWriterFile {

**public**BufferedWriterFileImpl(Writerwriter) {
**super**(writer);
}
@Override
**public****void**bufferedWriter() {
System.**out**.println("加入高效缓冲区");
}
@Override
**public****void**clean() {
System.**out**.println("做清理工作");
}
}

测试类
**public****class**Client {
**public****static****void**main(String[]args) {
Writerwriter=**new**BufferedWriterFileImpl(**new**TxtWriter());
Writerwriter1=**new**BufferedWriterFileImpl(**new**MP3Writer());
writer.writeFile();
writer1.writeFile();
//输出
//加入高效缓冲区
//写入文本文件
//做清理工作
//加入高效缓冲区
//写入MP3文件
//做清理工作
}
}

==============================================================================
**装饰者模式扩展**
**对于某些需求无法满足的情况下，我们可以对装饰角色做出扩展类，生产具体的扩展方法来满足我们的需求**
比如我们现在要写入Zip文件，我们可以扩展一个具体的装饰角色
**public****class**BufferedWriterZipFile**extends**BufferedWriterFile {
**private**Writerwriter;
**public**BufferedWriterZipFile(Writerwriter) {
**super**(writer);
}

@Override
**public****void**writeFile() {
**super**.writeFile();
}
/**
*高效缓冲区扩展方法
*/
@Override
**public****void**clean() {
System.**out**.println("做清理工作");
}

@Override
**public****void**bufferedWriter() {
System.**out**.println("加入扩展高效缓冲冲");
}
}

