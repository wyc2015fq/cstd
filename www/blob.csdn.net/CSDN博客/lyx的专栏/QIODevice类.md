# QIODevice类 - lyx的专栏 - CSDN博客





2015年11月03日 21:25:19[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1514









简介：


1. QIODevice类是所有输入输出IO类的基础类，为IO类提供了统一的调用接口，因此我们称QIODevice类以及其派生类为IO类。


2. 在访问IO类，必须先调用open()方法打开设备，之后才能调用读写方法对类进行操作。结束操作后需要调用close方法关闭设备。


3. IO类支持随机存储，和顺序储存设备。其中顺序设备不支持pos(),size()方法，一般用于网络通讯等设备如：QTcpSocket 。


    isSequential() 方法调用可以判断该类对象所支持的是否是顺序设备。


4. IO类发射readyRead() 信号表示有数据可以读取，对应的可以调用bytesAvailable()方法了解可以读取多少字节的数据。


   同理，发射bytesWritten()信号表示数据写入完成，对应的可以调用bytesToWrite()方法了解写入了多少字节的数据


5. IO类的读写函数是非阻塞的，调用方法后不会等待数据读写完成方法立即返回。因此类还提供了waitForReadyRead()和


   waitForBytesWriten()方法实现阻塞（在调用读写方法后调用对应的wait...方法实现阻塞）


6. IO类继承于QIODevice类，只需要实现自己的writeData()和readData()方法。其他读写方法QIODevice都是调用writeData()


   和readData()实现的


7. IO类例如QFile,QTcpSocket提供了buffer机制，用于减少底层驱动系统调用，提高访问速度。特别是提高了getChar,putChar方法的速度


   。但是在多对象需要读取同一个设备的大批量数据时，buffer会导致内存中存在多个同样的数据副本，内存开销巨大。这个情况，可以


   在调用open()方法时设置Unbuffered模式关闭buffer机制。





常用的方法：


1. 设置相关


   open(OpenMode mode):打开设备。mode参数用于设置读写模式，buffer机制，读写机制等。


   close():关闭设备


   isOpen():判断设备是否被打开。


   isWriteable:判断设备是否支持写入模式。（Open方法设置的）


   isReadable:判断设备是否支持读取。


   isSequential():判断设备是否是顺序设备


   isTextModeEnable():Text模式getChar方法将忽略'/r'换行符，返回下个字符。


   setTextModeEnable():设置text模式


2. 读相关


   QByteArray read (qint64 maxlen),qint64 read (char *data, qint64 maxlen):


   peek():偷窥方法，读取数据后不会移动数据指针，peek调用后可以继续调用read获取同样的内容


   readAll():


   qint64 readLine (char *data, qint64 maxlen)：读取一行遇到'n'或者读取长度maxlen-1，或者END返回


   canReadLine ：在buffer中检查是否有'n',也因此如果在关闭buffer功能的情况下该方法永远返回false


   getChar():读取一个字符，如果在text模式下，忽略'r'换行标识符，返回下一个字符


3. 写相关


   write:


   ungetChar():回写一个字符到设备，并且当前读写指针减少一个(读写指针pos() 如果是0不会减少)，用户“undo”


   putChar()：写入一个字符到设备。



