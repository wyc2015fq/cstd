# 格式化输出，c++中指针的指针，数组的指针_用户5706034127_新浪博客
||分类：[cPlusPlus](http://blog.sina.com.cn/s/articlelist_5706034127_1_1.html)|
			c++格式化输出、数组的指针和指针的指针
#include
#include
using namespace std;
int main()
{
 cout << "Hello
world!" << endl;
 int a[] =
{1,2,3,4,5,5,6,7,8,9,0,11,12,13,14,15,16};
 int *b = a;
 cout <<
endl;
 cout << "int *b =
a;"<< endl;
 cout << "b: "
<<b << "   b+1: " << b+1
<<"   sizeof(b[0]):
"<<sizeof(b[0])<< endl;
 int **b2 = (int
**)a;
 cout <<
endl;
 cout << "int **b2
= (int **)a;"<< endl;
 cout << "b2: "
<<b2 << "   b2+1: " << b2+1
<<"   sizeof(b2[0]):
"<<sizeof(b2[0])<< endl;
 int (*b3)[] = (int
(*)[])a;
 cout <<
endl;
 cout << "int
(*b3)[] = (int (*)[])a;"<< endl;
 cout <<"b3: "
<< (int)b3 << "  
因为没有指定数组的长度，所以任何访问都是非法的，虽然可以这样声明。除非强制转换来指定长度或其它类型"<<
endl;
 int (*b4)[3] = (int
(*)[3])a;
 cout <<
endl;
 cout << "int
(*b4)[3] = (int (*)[3])a;"<< endl;
 cout << "b4: "
<<b4 << "   b4+1: " << b4+1
<<"   sizeof(b4[0]):
"<<sizeof(b4[0])<< endl;
 cout <<
"指针加1就是指针加上其指向的元素的宽度的字节" <<endl;
 cout <<
endl;
 cout <<
"cout设置格式化输出有两种方式，一种是通过cout中添加控制符，另一种是调用cout的流成员函数"<<
endl;
 cout <<
endl;
 cout <<
"第一种方法举例：cout<<hex<<123<<endl;"<<
endl;
 cout <<
setbase(16) <<(int)b4 << " " <<
setbase(16)<< (int)(b4+1) << endl;
 cout << setbase(8)
<<(int)b4 << "  "<<
setbase(8)<< (int)(b4+1) << endl;
 cout << showbase
<< hex <<(int)b4 << "  "<<
oct<< (int)(b4+1) << endl;
 cout <<
resetiosflags(ios::showbase) << hex <<(int)b4 <<
"  "<< oct<< (int)(b4+1) <<
endl;
 cout <<
setfill('*')<< setw(10) << b4[0][0] <<
endl;
 cout <<
setiosflags(ios::scientific) << setprecision(8) <<
3.1415926 << endl;
 cout <<
setiosflags(ios::fixed) << setprecision(4) << 3.1415926
<< endl;
 cout <<
endl;
 cout <<
"第二种方法举例：cout.setf(),cout.precision(),cout.width(),cout.unsetf;"<<
endl;
 int a2 = 2111;
cout.setf(ios::showbase);
cout.unsetf(ios::dec);
cout.setf(ios::hex);
cout<<"hex:"<<a2<<endl;
cout.unsetf(ios::hex);
 cout.width(10);
cout.setf(ios::dec);
cout<<a2<<endl;
 cout.width(1);
cout<<a2<<endl;
 cout <<
endl;
 cout <<
"两种方法对于对应关系：" << endl;
 cout <<
"cout.precision(n) setprecision(n)"<< endl;
 cout <<
"cout.width(n)  setw(n)"<< endl;
 cout <<
"cout.fill(c)  setfill(c)"<< endl;
 cout <<
"cout.setf()  setiosflags()"<< endl;
 cout <<
"cout.unsetf()  resetioflags()"<< endl;
 cout <<
endl;
 cout << "格式标志："
<< endl;
 cout << "ios::left
输出数据在本域宽范围内向左对齐" << endl;
 cout <<
"ios::right 输出数据在本域宽范围内向右对齐" << endl;
 cout <<
"ios::internal 数值的符号位在域宽内左对齐，数值右对齐，中间由填充字符填充" << endl;
 cout << "ios::dec
设置整数的基数为10" << endl;
 cout << "ios::oct
设置整数的基数为8" << endl;
 cout << "ios::hex
设置整数的基数为16" << endl;
 cout <<
"ios::showbase 强制输出整数的基数(八进制数以0打头，十六进制数以0x打头)" << endl;
 cout <<
"ios::showpoint 强制输出浮点数的小点和尾数0" << endl;
 cout <<
"ios::uppercase 在以科学记数法格式E和以十六进制输出字母时以大写表示" << endl;
 cout <<
"ios::showpos 对正数显示“+”号" << endl;
 cout <<
"ios::scientific 浮点数以科学记数法格式输出" << endl;
 cout <<
"ios::fixed 浮点数以定点格式(小数形式)输出" << endl;
 cout <<
"ios::unitbuf 每次输出之后刷新所有的流" << endl;
 cout <<
"ios::stdio 每次输出之后清除stdout, stderr" << endl;
 return 0;
}
c，python格式化输出
#include
int main()
{
 printf("%.*s
\n", 8, "abcdefgggggg");
printf("%*.*f \n", 3,3, 1.25456f);
 printf("%%A
浮点数、十六进制数字和p-记法（C99）\n");
 printf("%%c
一个字符\n");
 printf("%%d
有符号十进制整数\n");
 printf("%%e
浮点数、e-记数法\n");
 printf("%%E
浮点数、E-记数法\n");
 printf("%%f
浮点数、十进制记数法\n");
 printf("%%g
根据数值不同自动选择%f或%e.\n");
 printf("%%G
根据数值不同自动选择%f或%e.\n");
 printf("%%i
有符号十进制数（与%d相同）\n");
 printf("%%o
无符号八进制整数\n");
 printf("%%p
指针\n");
 printf("%%s
字符串\n");
 printf("%%u
无符号十进制整数\n");
 printf("%%x
使用十六进制数字0f的无符号十六进制整数\n");
 printf("%%X
使用十六进制数字0f的无符号十六进制整数\n");
 printf("%%%%
打印一个百分号\n");
printf("\n");
printf("0.3f,%f\n",1234.2123f,1234.2123f); 
// 填充
printf("%-10.3f,%f\n",1234.2123f,1234.2123f); 
// 左对齐
printf(".3f,%f\n",1234.2123f,1234.2123f); 
// 精度和宽度
printf(".3e,%f\n",1234.2123f,1234.2123f); 
// 科学计数法
printf("0X,0o,%d\n",123567,123567,123567); // 16进制，8进制
 return
0;
}

