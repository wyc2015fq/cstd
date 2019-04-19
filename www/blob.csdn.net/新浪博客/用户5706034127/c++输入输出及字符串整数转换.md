# c++输入输出及字符串整数转换_用户5706034127_新浪博客
||分类：[cPlusPlus](http://blog.sina.com.cn/s/articlelist_5706034127_1_1.html)|
			从命令行读入不固定个数的整数，
字符串和整数之间的转换，
最好是char
str[123]数组或char*sp，配合getline，配合sscanf，sprintf来实现转换。
c++内置的string和stringstream并不会提供太大的帮助。
![c++输入输出及字符串整数转换](http://s8.sinaimg.cn/mw690/006e9W8vzy79sTf0co797&690)
![c++输入输出及字符串整数转换](http://s16.sinaimg.cn/mw690/006e9W8vzy79sTfdYaHaf&690)
**1、字符串数字之间的转换**
(1)string --> char *
string
str("OK");
char * p =
str.c_str();
(2)char * -->string
char *p =
"OK";
string
str(p);
(3)char * -->CString 
char *p
="OK";
CString
m_Str(p);
//或者
CString
m_Str;
m_Str.Format("%s",p);
(4)CString --> char *
CString
str("OK");
char * p =
str.GetBuffer(0);
...
str.ReleaseBuffer();
(5)string --> CString  
CString.Format("%s",
string.c_str());  
(6)CString --> string
string
s(CString.GetBuffer(0));  
GetBuffer()后一定要ReleaseBuffer()，否则就没有释放缓冲区所占的空间，CString对象不能动态增长了。
(7)double/float->CString
double data;
CString.Format("%.2f",data);
//保留2位小数
(8)CString->double
CString
s="123.12";
double  
d=atof(s);  
(9)string->double
 double d=atof(s.c_str());
**2、数字转字符串：使用sprintf()函数**
char str[10];
int a=1234321;
sprintf(str,"%d",a);
--------------------
char str[10];
double a=123.321;
sprintf(str,"%.3lf",a);
--------------------
char str[10];
int a=175;
sprintf(str,"%x",a);//10进制转换成16进制，如果输出大写的字母是sprintf(str,"%X",a)
--------------------
char *itoa(int value, char* string, int
radix); 
同样也可以将数字转字符串，不过itoa()这个函数是平台相关的（不是标准里的），故在这里不推荐使用这个函数。
**3、字符串转数字：使用sscanf()函数**
char str[]="1234321";
int a;
sscanf(str,"%d",&a);
.............
char str[]="123.321";
double a;
sscanf(str,"%lf",&a);
.............
char str[]="AF";
int a;
sscanf(str,"%x",&a); //16进制转换成10进制
另外也可以使用atoi(),atol(),atof().
**4、使用stringstream类**
用ostringstream对象写一个字符串，类似于sprintf() 
 ostringstream s1;
 int i = 22;
 s1 << "Hello " << i <<
endl;
 string s2 = s1.str();
 cout << s2;
用istringstream对象读一个字符串，类似于sscanf() 
 istringstream stream1;
 string string1 = "25";
 stream1.str(string1);
 int i;
 stream1 >> i;
 cout << i << endl;
 // displays 25
