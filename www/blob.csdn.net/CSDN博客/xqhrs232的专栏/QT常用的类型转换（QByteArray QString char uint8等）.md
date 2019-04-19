# QT常用的类型转换（QByteArray/QString/char*/uint8等） - xqhrs232的专栏 - CSDN博客
2019年02月22日 11:18:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：37
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/qq_31073871/article/details/79881354](https://blog.csdn.net/qq_31073871/article/details/79881354)
后续会继续补充。
目录：
1、字节数组(char*、uint8、int8)转QByteArray类
2、QByteArray类转字节数组(char*、uint8、int8)
3、字节数组转hex字符串
4、字节数组(char*、uint8、int8)转QString
5、QString转字节数组(char*、uint8、int8)
6、单个数字、字符串，互转
QByteArray类提供了类似数组的功能，但功能更强大，常用函数：
只读元素at(n)、读写元素[n]、追加元素append（可以追加QString、char*、char、QByteArray）、迭代器、元素存储区指针data()
1、字节数组转QByteArray类（字节数组是指uint8、int8、char的数组）
直接使用QByteArray的构造函数即可，
    const char *str="abcde";
    char u8_arr[3]={0xa1,0xC2,0x3E};
    QByteArray qa1(str);     
    QByteArray qa2(u8_arr,3);
QByteArray 的元素存储地址可以通过成员函数data()获得，使用*data可以访问QByteArray 的元素
注意：如果数组中有'\0'，那么构造QByteArray对象时，碰到数组的\0构造就停止了
2、QByteArray类转字节数组(char*、uint8、int8)
直接使用QByteArray的成员函数即可
QByteArray ba("abcd");
char *pArr = ba.data();
3、字节数组转hex字符串（字节数组是指uint8、int8的数组）
    uint8 u8_arr[3]={0xF5,0x07,0xa5};
    QByteArray qa2(u8_arr,3);
    QString qstr(qa2.toHex());//
    qDebug()<<qstr.toUpper();//输出字符串：F507A5
QByteArray的toHex()的返回值仍为QByteArray型，因为每一个字节都会变成2个hex字符，所以返回的QByteArray的长度是原来的两倍。
4、字节数组(char*、uint8、int8)转QString
如果字节数组有字符串结尾符\0，则可以直接使用QString的构造函数构造QString对象。
如果字节数组没有\0，可以先把字节数组转成QByteArray，然后用QByteArray构造QString，参考第1条
5、QString转字节数组(char*、uint8、int8)
QString转成了char*，也就相当于转成了字节数组，
参考：https://blog.csdn.net/lu_embedded/article/details/52465203
这篇文章提到了两种方法：
（1）QString中无中文：
QString  str;
char*  ch;
QByteArray ba = str.toLatin1(); // 这一行不可少
ch=ba.data();//不允许这样用：ch=str.toLatin1().data();
QString中有中文：避免乱码，需要使用QTextCodec
#include <QTextCodec>
QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
QByteArray ba = str.toLocal8Bit();  // toLocal8Bit 支持中文
注意：QString的toLatin1函数返回的QByteArray 是带\0的
另外这篇文章还提到，第三行不可少，如果直接这样转：ch=str.toLatin1().data()，会出错。我认为原因是这样的：toLatin1的返回值是值传递，而不是引用传递，使用值传递来传返回值，返回值是位于内存栈上的，语句结束之后，返回值作为一个临时局部变量，被出栈，这时再使用.data()取指针，指针指向的位置已被出栈，不存在了，为了避免这种情况，就是把toLatin1的返回值使用局部变量甚至局部静态变量接收过来，例如这里的ba变量，toLatin1的返回值出栈消失了，但是ba还在，所以再使用ba.data()就可以获取一个有效指针。
（2）先将 QString 转为标准库中的 string 类型，然后将 string 转为 char *
QString  filename;
std::string str = filename.toStdString();
const char* ch = str.c_str();
6、单个数字、字符串，互转
（1）数字转字符串两种方法:
①
int num = 1000;// 1000 = 0x03E8;
QString  str = QString("%1").arg(num,4,16,(QChar)'0').toUpper();
这种方法可以把数组转为16进制样式"03E8"、10进制样式的字符串"1000"、8进制样式等
② QString::number(double n, char format = 'g', int precision = 6)，
如QString str = QString::number(1000);//str="1000"
（2）字符串转数字：
使用QString的成员函数即可：.toInt、toDouble等
字符串的样式可通过形参指定，如
QString str = "123";
bool OK;
int a = str.toInt(&OK, 10);// a=123，OK=true
int b = str.toInt(&OK, 16);// b=0x123，OK=true
