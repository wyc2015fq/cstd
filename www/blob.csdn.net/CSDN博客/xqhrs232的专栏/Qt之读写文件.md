# Qt之读写文件 - xqhrs232的专栏 - CSDN博客
2018年05月14日 16:22:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：205
原文地址::[https://blog.csdn.net/zhuyunfei/article/details/51249378](https://blog.csdn.net/zhuyunfei/article/details/51249378)
## 1.思路
读写文件的基本操作：
|读文件|写文件|
|----|----|
|打开文件|打开文件|
|读取文件|写入文件|
|关闭文件|关闭文件|
2.QFile实现读写文件
QFile类提供读写文件的接口。 
它的构造函数如下： 
QFile(const QString & name) 
QFile(QObject * parent) 
QFile(const QString & name, QObject * parent) 
通过它的构造函数我们可以知道，可以直接在调用构造函数的时候直接传递文件名给QFile类，这是常用的方法，或者我们先生成QFile对象，然后调用setFileName()方法来设置文件。 
其中文件名中的路径分隔符要求必须是’/’,其他分隔符QFile不支持。 
然后再了解下QFile的常用函数以及打开文件的模式类型： 
QFile的常用函数：
|函数名|功能|
|----|----|
|copy()|复制文件|
|exists()|检查文件是否存在|
|open()|打开文件|
|remove()|删除文件|
|rename()|修改文件名|
|setFileName()|设置文件名|
|size()|文件大小|
|pos()|文件光标当前位置|
|seek()|设置文件光标位置|
|atEnd()|判断当前是否为文件尾|
|read()|读取文件内容|
|close()|关闭文件|
文件打开模式|模式|值|描述|
|----|----|----|
|QIODevice::NotOpen|0x0000|不打开|
|QIODevice::ReadOnly|0x0001|只读方式|
|QIODevice::WriteOnly|0x0002|只写方式，如果文件不存在则会自动创建文件|
|QIODevice::ReadWrite|ReadOnly | WriteOnly|读写方式|
|QIODevice::Append|0x0004|此模式表明所有数据写入到文件尾|
|QIODevice::Truncate|0x0008|打开文件之前，此文件被截断，原来文件的所有数据会丢失|
|QIODevice::Text|0x0010|读的时候，文件结束标志位会被转为’\n’;写的时候，文件结束标志位会被转为本地编码的结束为，例如win32的结束位’\r\n’|
|QIODevice::UnBuffered|0x0020|不缓存|
下面通过代码来看看QFile的使用：
```
//example 1
    QFile file("test.csv");
    //已读写方式打开文件，
    //如果文件不存在会自动创建文件
    if(!file.open(QIODevice::ReadWrite)){
        //ReadOnly文件不存在，打开失败
        //WriteOnly文件不存在，会自动创建文件
        //ReadWrite文件不存在，会自动创建文件
        //Append文件不存在，会自动创建文件
        //Truncate文件不存在，打开失败
        //Text文件不存在，打开失败
        //Unbuffered文件不存在，打开失败
        qDebug()<<"打开失败";
    }else{
        qDebug()<<"打开成功";
    }
    //操作文件
    //关闭文件
    file.close();
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
写文件
```
//example 2
    QFile file("test.csv");
    //判断文件是否存在
    if(file.exists()){
        qDebug()<<"文件已存在";
    }else{
        qDebug()<<"文件不存在";
    }
    //已读写方式打开文件，
    //如果文件不存在会自动创建文件
    if(!file.open(QIODevice::ReadWrite)){
        qDebug()<<"打开失败";
    }else{
        qDebug()<<"打开成功";
    }
    //获得文件大小
    qint64 pos;
    pos = file.size();
    //重新定位文件输入位置，这里是定位到文件尾端
    file.seek(pos);
    QString content = "welcome to BeiJing";
    //写入文件
    qint64 length = -1;
    length = file.write(content.toLatin1(),content.length());
    if(length == -1){
        qDebug()<<"写入文件失败";
    }else{
        qDebug()<<"写入文件成功";
    }
    //关闭文件
    file.close();
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
控制台运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20160426112313801)
打开文件看看里面的内容： 
![这里写图片描述](https://img-blog.csdn.net/20160426112716755)
正是我们程序中写入的内容。
读文件 
我们现在test.csv文件内准备两行数据 
![这里写图片描述](https://img-blog.csdn.net/20160426114125357)
代码如下：
```
QFile file("test.csv");
    //判断文件是否存在
    if(file.exists()){
        qDebug()<<"文件已存在";
    }else{
        qDebug()<<"文件不存在";
    }
    //已读写方式打开文件，
    //如果文件不存在会自动创建文件
    if(!file.open(QIODevice::ReadWrite)){
        qDebug()<<"打开失败";
    }else{
        qDebug()<<"打开成功";
    }
    //读取文件
    int line = 1;
    //判断文件是否已经读到末尾了
    while(!file.atEnd()){
        char buffer[1024];
        //读取一行数据
        qint64 length = file.readLine(buffer,1024);
        if(length != -1){
            qDebug()<<line++<<":"<<buffer;
        }
    }
    //关闭文件
    file.close();
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
控制台输出结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20160426114322109)
和我们开始文件中准备的内容一致，表明已成功读取。
