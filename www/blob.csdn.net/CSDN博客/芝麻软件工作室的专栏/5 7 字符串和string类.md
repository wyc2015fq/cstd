
# 5.7 字符串和string类 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月13日 08:57:31[seven-soft](https://me.csdn.net/softn)阅读数：261



## 一.C风格字符串
◆ 1、字符串是用字符型数组存储的，字符串要求其尾部以’\0’作为结束标志。如：
char string[ ]=”C++ programming language”;
用sizeof来测string长度为25个字节，而实际串本身长度(含空格)为24个字节，多出来的一个就是串结束符’\0’(含0)。
◆2、指针与字符串
char *pstr=”C++ is a object_oriented language”;
这里绝不是建立了一个字符串*pstr，并用赋值号后的字符串常量给它赋初值。而是编译器将字符串常量“C++ is a object language”的第一个字符的存储地址赋给字符指针作初值。字符串常量是放在字符串常量区中，它不属于指针pstr，反过来是pstr依附于它。
用指针输出字符串：C++不对数组的边界进行检测，但字符串有一个结束符’\0’(全0)，在程序运行时是可以知道实际串长度的。设有定义int a[10]，如有语句：
cout<<a<<endl;
则打印出a数组在内存中的首地址，一个16进制的数。
◆3、字符串处理函数
C++标准库有很多字符串处理函数，在头文件<cstring>中。
1）字符串复制函数：
char *strcpy(char *s,const char ct)
返回值是指向复制后字符串中首字符的指针。
【例5.10】字符串复制函数 strcpy()。（[查看源码](http://www.weixueyuan.net/templets/default/cpp/source/%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%A4%8D%E5%88%B6%E5%87%BD%E6%95%B0%20strcpy().txt)）
2）串连接函数：char *strcat(char *s,const char *ct)
将串ct复制到串s的后面，形成一个长串。例如：
char str1[30]=“张成”；
char str2[20]=“是电子系的学生”；
strcat(str1,str2);
结果是str1中放的是“张成是电子系的学生”。
3）字符串比较函数：int strcmp(const char *cs,const char ct)。
两字符串比较是按字典排序方法进行。两个字符串从第一个字符比较起，如果相同，则比较第二个字符，依此类推，直到不同。哪个字符串对应的字符的ASCII码值大，串就大。如果两串直到结束符都相同，则一样大。若两个字符串相等，则返回0；串cs
 大，则返回正整数；串ct大，则返回负整数。
该函数也可很简洁地表达如下：
int strcmp(const char *cs,const char *ct){
int k;
while((k= *cs - * ct)== 0 && *cs++ && *ct++);
//注意*cs++和*ct++是条件判断后才++的
return k;
}
4）求字符串长度函数：int strlen(const char *s)
这里求出的串长度不包含串结束符在内。而sizeof运算符包括结束符，还包括没有使用的单元。
◆ 4、void指针及应用：称无类型或泛型指针。
任何类型的指针都可以赋给void类型的指针变量，例如：
int val=5;
int *pi=&val; //必须为指针赋一个有效地址，才能引用指针
void *pv=pi;
但反过来，将泛类型指针赋给其他类型指针必须使用强制类型转换： pi=(int *)pv;
【例5.11】将字符指针强制转换为泛型指针输出字符串地址。
\#include<iostream>
using namespace std;
int main(){
char *string1="欢迎学习C++程序设计课程！";
cout<<"串值是："<<string1<<"\n串地址是："<<(void*)string1<<endl;
return 0;
}
## 二.标准的C++string类
◆ 1、建议使用C++的string类，它重载了运算符，连接、索引和复制等操作不必使用函数，使运算更加方便，而且不易出错。string类包含在名字空间std中的头文件<string>。
\#include<string>
using namespace std;
◆2、string类的使用方法
(1) string类有三个构造函数：
string str; //调用默认的构造函数，建立空串
string str("OK"); //调用采用C字符串初始化的构造函数
string str(str1); //调用复制构造函数，str是str1的复制
(2) string类字符元素的访问比C字符串有所增强：
str[i] //返回str中索引i处字符的引用，不查是否出界
str.at(i) //返回str中索引i处字符的引用，查是否出界
(3) string类重载了一些运算符，特别注意当目标串较小，无法容纳新的字符串，系统会自动分配更多的空间给目标串，不必顾虑出界：
str1=str2; //str1成为str2的代码
str1+=str2; //str2的字符数据连接到str1的尾部
str1+str2; //返回一个字符串，它将str2连接到str1的尾部
str1==str2; str1!=str2; //比较串是否相等，返回布尔值
str1<str2; str1>str2; str1<=str2; str1>=str2;
//基于字典序的比较，返回布尔值
(4) string类的输入输出。
输出与C风格字符串同样方便，使用插入运算符<<和cout。输入如用提取运算符>>，代码读取的是以空白字符结束的字符串，输入完整的字符串可用非成员函数getline，注意格式：
getline(cin,str); //串以’\n’结束
getline(cin,str,ch); //串以ch结束
(5) string类有一些常用的成员函数可进行字符串处理：
str.substr(pos,length1); //返回对象的一个子串，从pos位置起，长length1个字符
str.empty(); //查是否空串
str.insert(pos,str2); //将str2插入str的pos位置处
str.remove(pos,length1); //在str位置pos处起，删除长度为length1的字串
str.find(str1); //返回str1首次在str中出现时的索引
str.find(str1,pos); //返回从pos处起str1首次在str中出现时的索引
str.length(str); //返回串长度
(6) C字符串到string类对象是由构造函数隐式自动进行，而string类对象到C字符串的转换必须执行显示的类型转换，应调用成员函数
str.c_str(); //将string类转换为C风格字符串，返回char*
◆ 3、【例5.12】判断字符串是否为回文。（[查看源码](http://www.weixueyuan.net/templets/default/cpp/source/%E5%88%A4%E6%96%AD%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%98%AF%E5%90%A6%E4%B8%BA%E5%9B%9E%E6%96%87.txt)）
回文是指顺读和反读都一样的串，这里不分大小写，并滤去所有非字母字符，如：
Madam,I’m Adam.
Golf,No Sir,prefer prison flog
都是回文。
◆4、string类的优点：string类有自己的构造函数和析构函数，如果它作为类或结构的成员，要记住它是成员对象，当整个类对象建立和撤销时，会自动调用作为成员对象的string字符串的构造和析构函数。
## 三.自定义字符串类
class mystring{
char str[21]; int maxsize; int last;
public:
mystring(){
last=-1;
axsize=21;
str[0]='\0';
}
mystring(char *s); ~mystring(){}
void show(){
//如需重载<<,则请参见9.3节,暂时未学到,替代方法是改用show()函数
cout<<str<<'\t';
}
bool operator<(mystring &);
mystring & operator=(char * ms);
//这里重载的=是把C风格字符串赋给mystring
};
mystring:: mystring(char *s){//当C字符串过长，初始化时采用截尾处理
last=-1;
maxsize=21;
do{
last++;
str[last]=s[last];
}while(s[last]!='\0'&&last<maxsize-1);
str[last] ='\0'; //截尾处理时，必须加串结束符
}
mystring & mystring::operator=(char * ms){
//这里返回值为引用，不调用复制构造函数
last=-1;
do{
last++;
str[last]=ms[last];
}while(ms[last]!='\0'&&last<maxsize-1);
str[last] ='\0'; //截尾处理时，必须加串结束符
return *this;
}
bool mystring::operator<(mystring & ms){ //重载<运算符
int i=0,k;
do{
k=str[i]-ms.str[i];
i++;
}while(k==0&&i<last&&i<ms.last);
if(k<0) return true;
if(i==last&&i!=ms.last) return true;
return false;
}
可以用mystring代替标准字符串类string，但注意串长限制在20个字符或10个汉字。在类定义中运算符和函数的重载是面向对象程序设计实现通用性的非常得力的工具。

