# C++常量---初识 - Koma Hub - CSDN博客
2017年12月03日 20:32:47[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：96
```cpp
#include<iostream>
using namespace std;
int main(int argc, char*argv[]){
    char ch = 'a';//-128 ~ 127, unsigned char: 0 ~ 255
    int a = sizeof ch;
    short b = sizeof(a);
    long c = sizeof(a) + sizeof(b);
    float e = c*1.1;
    double f = e/1.3;
    cout<<"Hello World !"<<a+b+c<<endl;
    cout<<a<<endl;
    cout<<b<<endl;
    cout<<c<<endl;
    cout<<e<<endl;
    cout<<f<<endl;
    int g = sizeof(f);
    
    cout<<g<<endl;
    auto int h = 0xff;// 十六进制
    cout<<h<<endl;
    unsigned int i = 024;//八进制
    cout<<i<<endl;
    static int j = 12U;//加上u 或U 将其指定为一个无符号数
    int k = 13L;//加一个 L 或l 字母L 的大写形式或者小写形式将其指定为long 类型
    
    register float l = 2.5e+3;//使用科学计数法, 指数可写作e 或E
    cout<<l<<endl;
    volatile float m = 3.14159265354987f;//浮点型文字常量在缺省情况下被认为是double 型;
                        //单精度文字常量由值后面的f 或F 来标示
    cout<<m<<endl;
    const bool n = true;
    cout<<n<<endl;
    /*
    newline(换行符) \n
    horizontal tab(水平制表键) \t
    vertical tab(垂直制表键) \v
    backspace(退格键) \b
    carriage return (回车键) \r
    formfeed (进纸键) \f
    alert (beel) (响铃符) \a
    backslash (反斜杠键) \\
    question mark (问号) \?
    single quote (单引号) \'
    double quote (双引号) \" */
    cout<<"\n"<<"\t"<<"\v"<<"\b"<<"\r"<<"\f"<<"\a"<<"\\"<<"\?"<<"\'"<<"\""<<endl;
    cout<<"\ooo"<<endl;//这里的ooo 代表三个八进制数字组成的序列.
                            //八进制序列的值代表该字符在机器字符集里的数字值
    /*使用ASCII 码字符集表示文字常量*/
    cout<<"\7"<<endl;//(bell)
    cout<<"\14"<<endl;//(newline)
    cout<<"\0"<<endl;//(null)
    cout<<"\062"<<endl;//("2")
    /*宽字符常量用来支持某些语言的字符集合如汉语
    日语这些语言中的某些字符不能用单个字符来表示*/
    cout<<L'a'<<endl;//这称为宽字符文字类型为wchar_t
    cout<<L"rong tao"<<endl;
    char chr[25] = "rong tao";
    cout<<chr<<endl;
    cout<<"rong \
            tao"<<"\nCC\toptions\tfile.[cC]\n"<<endl;
    cout<<"Who goes with F\144rgus?\014"<<endl;
    
    float o = 3.14e1L;
    cout<<o<<endl;
    float p = 3.14l;
    cout<<p<<endl;
    return 0;
}
```
编译与运行：
```
-bash-4.1$ g++ -o a Demo.cpp
Demo.cpp: In function ‘int main(int, char**)’:
Demo.cpp:59: 警告：未知的转义序列：‘\o’
-bash-4.1$ ./a
Hello World !11
1
4
6
6.6
5.07692
8
255
20
2500
3.14159
1
	
\?'"
ooo
2
97
0x401048
rong tao
rong             tao
CC	options	file.[cC]
Who goes with Fdrgus?
31.4
3.14
-bash-4.1$
```
