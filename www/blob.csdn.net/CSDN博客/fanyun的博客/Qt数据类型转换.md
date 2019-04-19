# Qt数据类型转换 - fanyun的博客 - CSDN博客
2017年09月30日 21:33:48[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：434
     Qt兼容标准C++的数据类型，但是Qt又创造了很多自己的数据类型，而我们在实际开发中，数据类型之间的转换是很多的，现在总结一下：
1、QString转QByteArray
QByteArray byte;
QString string;
byte = string.toLatin1();
2、QByteArray转QString
QByteArray byte;
QString string;
string = QString(byte);
3、QString转String
QString string;
std::string str;
str = string.toStdString();
4、String转QString
QString string;
std::string str;
string = QString::fromStdString(str);
5、QByteArray转char *
char *ch;         //不能定义成ch[n]，数组和指针不一样;
QByteArray byte;
ch = byte.data();
6、char *转QByteArray
char *ch;
QByteArray byte;
byte = QByteArray(ch);
7、QString转char *
QString string;
char *ch;
ch = string.toLatin1.data();
8、char *转QString
char *ch;
QString string= QString(QLatin1String(ch));
9、QHostAddress转QString
QHostAddress address;
QString string = address.toString();
10、QString转QHostAddress
QString string;
QHostAddress address(string);
11、QString转unsigned char
QString string;
bool ok;
unsigned char uch = string.toInt(&ok, 16);
12、unsigned char转QString
unsigned char uch;
QString string = QString::number(uch,16);
13、int转QString
int m;
QString string;
string=QString::number(m);
14、QString转int
QString string;
int m;
m=string.toInt();
15、QDatatime转string
public QDateTime qdate = QDateTime.currentDateTime();
datetime = qdate.toString("yyyy年MM月dd日ddddhh:mm:ss");
如果不是QTime和QDate类比如说：通过TCP/IP接收到的char unsigned char 类等如何转换为QString类
QString Time2String( DWORD dwTime)
{
char cTime[50] = {0};
memset(cTime,0,50);
strftime(cTime,32,"%Y-%m-%d %H:%M:%S",localtime(&time_t(dwTime)));
return QString(cTime);
}
16、Qstring转换为十进制整形
注意：基数默认为10。当基数为10时，并且基数必须在2到36之间。如果基数为0，若字符串是以0x开头的就会转换为16进制，若以0开头就转换为八进制，否则就转换为十进制。
Qstring str="FF";
bool ok;
int dec=str.toInt(&ok,10); //dec=255 ; ok=rue
int hex =str.toInt(&ok,16); //hex=255;ok=true;
17、常整形转换为Qstring形
ong a =63;
QString str=QString::number(a,16); //str="3f";
QString str=QString::number(a,16).toUpper(); //str="3F";
18、QString转换为 double
1）.QString str="123.45";
double val=str.toDouble(); //val=123.45
2）.很适合科学计数法形式转换
bool ok;
double d;
d=QString("1234.56e-02").toDouble(&ok); //ok=true;d;12.3456.
19、QString转换为float形
1）.QString str="123.45";
float d=str.toFloat(); //d=123.45
2）.QString str="R2D2";
bool ok;
float d=str.toFloat(&ok); //转换是被时返回0.0,ok=false;
