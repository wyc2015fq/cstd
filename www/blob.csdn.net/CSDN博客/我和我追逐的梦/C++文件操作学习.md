
# C++文件操作学习 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月22日 17:38:53[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2361


C++文件操作
文件流对象：\#include<fstream>
ifstream：输入流对象
ofstream：输出文件流对象
fstream：输入输出文件流对象
ifstream file_in;
ofstream file_out;
fstream file_inout;
打开文件一（利用构造函数）：
ofstream file_out(”c:\\...”,ios::out|ios::binary);//以二进制方式打开输出文件
ifstream file_in(“c:\\...”,ios::in|ios::binary);//以二进制方式打开输入文件
常用的文件流构造函数如下：
ifstream::ifstream(const char *,int=ios::in,int=filebuf::openprot);
ofstream::ofstream(const char *,int=ios::out,int=filebuf::openprot);
fstream::fstream(const char *,int,int=filebuf::openprot);
打开文件二（利用成员函数open()）：
void::ifstream::open(const char*,int=ios::in);
void::ofstream::open(const char*,int=ios::out);
void::fstream::open(const char*,int);
路径名一般要用完整的路径名。如果文件在于exe文件相同的目录中，则可以用相对路径。
打开方式（以|组合起来）：
ios::in  用于数据输入，即读文件
ios::out  用于数据输出，即写文件
ios::ate  文件打开后，将文件指针放置在文件尾部
ios::app  打开文件用于追加数据，文件指针始终指向文件尾部
ios::trunc 当打开文件已存在，则清除其内容，使之成为空文件
ios::nocreate  如果文件不存在，则不建立任何文件，返回打开失败信息
ios::noreplace 如果打开文件已存在，则不建立任何文件，返回错误消息
ios::binary   以二进制方式打开文件
用fail() 判断文件打开是否成功，成功返回false，否则返回true。
文件操作完成之后，需要用close()来关闭文件。
文本文件输入输出：
向文本文件输出数据（写文件）
ofstream file_out(“c:\\text.txt”,ios::out);
一：使用插入操作符<<
原型：ostream& operator<<(C++标准类型 &);
file_out<<” A B C D E F G”<<endl;
二：调用成员函数put()
原型：ostream& put(char);
file_out.put(‘A’);
file_out.put(10); //\n的ASC码值
从文本文件中读出数据（读文件）
ifstream file_in(“c:\\text.txt”,ios::in);
一：利用提取操作符>>
原型：istream & operator<<(C++标准类型&);
string s;
file_in>>s;
二：调用成员函数get()
原型：int get();//读取字符，会读出空格字符和换行符
istream&get(char &c); //读取一个字符到c中
三：调用成员函数getline()
原型：istream & getline(char *buffer,int len,char = “\n”);
buffer为读字符串到buffer中
len 为读字符串长度
char = “\n” 默认为当遇到换行符时，停止读取
bool eof() 判断文件指针是否指向文件末尾，是则返回true，否则为false
二进制文件输入输出：
向二进制文件写入数据利用流函数write()
原型：ostream& write(const char *buffer,int len)；
buffer用于存储需要写入文件的内容  len为需要写入数据的长度
从二进制文件中读入数据利用流函数read()
原型：istream & read(char buffer,int len);
buffer用于存储读出的数据 len为需要读取数据的长度
文件定位：
流类istream的成员函数 seekg(),用于把读文件指针移到指定位置
原型 istream &seekg(long dis,seek_dir ref=ios::beg);
l流类ostream的成员函数seekp()，用于把写文件指针移到指定位置
原型ostream&seekp(long dis,seek_dir ref =ios::beg);
dis是文件指针需要移动的字节数，当其为正数时，向后移，为负数，向前移
seek_dir是ios根基类中定义的枚举变量，表示文件指针开始移动的基准位置，如下：
enum seek_dir{beg=0,cur=1,end=2};
其中ios::beg表示文件的开始位置，cur表示当前位置，end表示结束位置。


