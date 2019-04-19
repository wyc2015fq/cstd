# C/C++常用函数使用总结 - fanyun的博客 - CSDN博客
2017年03月31日 12:58:27[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4886
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

1.C/C++常用库函数substr函数:
**substr_函数（）：**
函数原型： substr_(char *dest, const char* src, unsigned int start,unsigned int len)
#include<assert.h>
#include<stdio.h>
//substr截取子串
char * substr_(char *dest, const char* src,unsigned int start, unsigned int len){
strncpy_(dest,src+start,len);//或者调用系统strncpy函数
dest[len] ='/0';
return dest;
}
SUBSTR(cExpression，nStartPosition[，nCharactersReturned])
其中，cExpression指定要从其中返回字符串的字符表达式或备注字段；nStartPosition用于指定返回的字符串在字符表达式或备注字段中的位置，nCharactersReturned用于指定返回的字符数目，缺省时返回字符表达式的值结束前的全部字符。
举例
STORE'abcdefghijklm' To mystring
substr_ (mystring ,1,5) 显示 "abcde"
substr_ (mystring ,6) 显示 "fghijklm"
substr_ (mystring,-2)显示“lm”
substr_ (mystrng,-4)显示“jklm”、
substr_ (char A,char B，int C, int D)
　　这个函数主要用于字符串的运算,参数A是字符数组或是指向字符串的指针，用于接收字符串，参数B一般也为字符数组或指向字符串的指针，表示要截取字符串的地方，参数C表示从B中截取字符串的起始位置，参数D表示要截取字符串的长度，缺省时返回字符表达式的值结束前B的全部字符。
　　上述表达式功能可描述为：从字符串B的第C个字符处开始，截取长度为D的一串字符串，放入字符串数组A中,返回截取的字符串。
```cpp
#include<string>
#include<iostream>
using namespace std;
main()
{
string s("12345asdf");
string a=s.substr(0,5);       //获得字符串s中 从第0位开始的长度为5的字符串//默认时的长度为从开始位置到尾
cout<<a<<endl;
}
```
输出结果为:
12345
函数原型：
```cpp
substr_(char *dest, const char* src, unsigned int start,unsigned int len)
#include<assert.h>
#include<stdio.h>
//substr截取子串
char * substr_(char *dest, const char* src,unsigned int start, unsigned int len){
strncpy_(dest,src+start,len);//或者调用系统strncpy函数
dest[len] ='/0';
return dest;
}
```
**2.C/C++中sscanf()函数: **
　　sscanf() - 从一个字符串中读进与指定格式相符的数据. 
　　函数原型: 
　　Int sscanf( string str, string fmt, mixed var1, mixed var2 ...); 
　　int scanf( const char *format [,argument]... ); 
　　说明： 
　　sscanf与scanf类似，都是用于输入的，只是后者以屏幕(stdin)为输入源，前者以固定字符串为输入源。 
　　其中的format可以是一个或多个 {%[*] [width] [{h | l | I64 | L}]type | ' ' | '\t' | '\n' | 非%符号} 
　　注： 
　　1、 * 亦可用于格式中, (即 %*d 和 %*s) 加了星号 (*) 表示跳过此数据不读入. (也就是不把此数据读入参数中) 
　　2、{a|b|c}表示a,b,c中选一，[d],表示可以有d也可以没有d。 
　　3、width表示读取宽度。 
　　4、{h | l | I64 | L}:参数的size,通常h表示单字节size，I表示2字节 size,L表示4字节size(double例外),l64表示8字节size。 
　　5、type :这就很多了，就是%s,%d之类。 
　　6、特别的：%*[width] [{h | l | I64 | L}]type 表示满足该条件的被过滤掉，不会向目标参数中写入值 。
　　支持集合操作： 
　　%[a-z] 表示匹配a到z中任意字符，贪婪性(尽可能多的匹配) 
　　%[aB'] 匹配a、B、'中一员，贪婪性 
　　%[^a] 匹配非a的任意字符，贪婪性
例子： 
　　1. 常见用法。 
　　char buf[512] = ; 
　　sscanf("123456 ", "%s", buf); 
　　printf("%s\n", buf); 
　　结果为：123456 
　　2. 取指定长度的字符串。如在下例中，取最大长度为4字节的字符串。 
　　sscanf("123456 ", "%4s", buf); 
　　printf("%s\n", buf); 
　　结果为：1234 
　　3. 取到指定字符为止的字符串。如在下例中，取遇到空格为止字符串。 
　　sscanf("123456 abcdedf", "%[^ ]", buf); 
　　printf("%s\n", buf); 
　　结果为：123456 
　　4. 取仅包含指定字符集的字符串。如在下例中，取仅包含1到9和小写字母的字符串。 
　　sscanf("123456abcdedfBCDEF", "%[1-9a-z]",buf); 
　　printf("%s\n", buf); 
　　结果为：123456abcdedf 
　　5. 取到指定字符集为止的字符串。如在下例中，取遇到大写字母为止的字符串。 
　　sscanf("123456abcdedfBCDEF", "%[^A-Z]",buf); 
　　printf("%s\n", buf); 
　　结果为：123456abcdedf 
　　6、给定一个字符串iios/12DDWDFF@122，获取 / 和 @ 之间的字符串，先将 "iios/"过滤掉，再将非'@'的一串内容送到buf中 
　　sscanf("iios/12DDWDFF@122", "%*[^/]/%[^@]",buf); 
　　printf("%s\n", buf); 
　　结果为：12DDWDFF 
　　7、给定一个字符串““hello, world”，仅保留world。（注意：“，”之后有一空格） 
　　sscanf(“hello, world”, "%*s%s", buf); 
　　printf("%s\n", buf); 
　　结果为：world 
　　%*s表示第一个匹配到的%s被过滤掉，即hello被过滤了 
　　如果没有空格则结果为NULL。 
　　sscanf的功能很类似于正则表达式, 但却没有正则表达式强大,所以如果对于比较复杂的字符串处理,建议使用正则表达式. 
　　//------------------------------------------------------- 
　　sscanf,表示从字符串中格式化输入 
　　上面表示从str中，输入数字给x，就是32700 
　　久以前，我以为c没有自己的split string函数，后来我发现了sscanf；一直以来，我以为sscanf只能以空格来界定字符串，现在我发现我错了。 
　　sscanf是一个运行时函数，原形很简单： 
　　int sscanf( 
　　const char *buffer, 
　　const char *format [, 
　　argument ] ... 
　　); 
　　它强大的功能体现在对format的支持上。 
　　我以前用它来分隔类似这样的字符串2006:03:18: 
　　int a, b, c; 
　　sscanf("2006:03:18", "%d:%d:%d", a, b, c); 
　　以及2006:03:18 - 2006:04:18: 
　　char sztime1[16] = "", sztime2[16] = ""; 
　　sscanf("2006:03:18 - 2006:04:18", "%s - %s",sztime1, sztime2); 
　　但是后来，我需要处理2006:03:18-2006:04:18 
　　仅仅是取消了‘-’两边的空格，却打破了%s对字符串的界定。 
　　我需要重新设计一个函数来处理这样的情况？这并不复杂，但是，为了使所有的代码都有统一的风格，我需要改动很多地方，把已有的sscanf替换成我自己的分割函数。我以为我肯定需要这样做，并伴随着对sscanf的强烈不满而入睡；一觉醒来，发现其实不必。 
　　format-type中有%[]这样的type field。如果读取的字符串，不是以空格来分隔的话，就可以使用%[]。 
　　%[]类似于一个正则表达式。[a-z]表示读取a-z的所有字符，[^a-z]表示读取除a-z以外的所有字符。 
　　所以那个问题也就迎刃而解了: 
　　sscanf("2006:03:18 - 2006:04:18", "%[0-9,:] -%[0-9,:]", sztime1, sztime2); 
**3.C/C++标准库函数sprintf和printf:**
        sprintf 跟printf 在用法上几乎一样，只是打印的目的地不同而已，前者打印到字符串中，后者则直接在命令行上输出。这也导致sprintf 比printf 有用得多.下面我们先说以下sprintf的用法.
sprintf 是个变参函数，定义如下：
```cpp
int sprintf( char *buffer, const char *format [, argument] ... )
```
int sprintf( char *buffer, const char *format [, argument] ... )
          除了前两个参数类型固定外，后面可以接任意多个参数。而它的精华，显然就在第二个参数：格式化字符串上。
          printf 和sprintf 都使用格式化字符串来指定串的格式，在格式串内部使用一些以“%”开头的格式说明符（format specifications）来占据一个位置，在后边的变参列表中提供相应的变量，最终函数就会用相应位置的变量来替代那个说明符，产生一个调用者想要的字符串。
 sprintf最常见的应用之一是把整数打印到字符串中.例如:
 //把整数123 打印成一个字符串保存在s 中。
sprintf(s, "%d", 123); //产生"123"
可以指定宽度，不足的左边补空格：
sprintf(s, "%8d%8d", 123, 4567); //产生：" 123 4567"
 当然也可以左对齐：
sprintf(s, "%-8d%8d", 123, 4567); //产生："123 4567"
 也可以按照16 进制打印：
sprintf(s, "%8x", 4567); //小写16 进制，宽度占8 个位置，右对齐
 sprintf(s, "%-8X", 4568); //大写16 进制，宽度占8 个位置，左对齐
这样，一个整数的16 进制字符串就很容易得到，但我们在打印16 进制内容时，通常想要一种左边补0 的等宽格式，那该怎么做呢？很简单，在表示宽度的数字前面加个0 就可以了。sprintf(s, "%08X", 4567); //产生："000011D7"
上面以”%d”进行的10 进制打印同样也可以使用这种左边补0 的方式。
这里要注意一个符号扩展的问题：比如，假如我们想打印短整数（short）-1 的内存16 进制表
示形式，在Win32 平台上，一个short 型占2 个字节，所以我们自然希望用4 个16 进制数字来打
印它：
short si = -1;
sprintf(s, "%04X", si);
产生“FFFFFFFF”，怎么回事？因为spritnf 是个变参函数，除了前面两个参数之外，后面的
参数都不是类型安全的，函数更没有办法仅仅通过一个“%X”就能得知当初函数调用前参数压栈
时被压进来的到底是个4 字节的整数还是个2 字节的短整数，所以采取了统一4 字节的处理方式，
导致参数压栈时做了符号扩展，扩展成了32 位的整数-1，打印时4 个位置不够了，就把32 位整数
-1 的8 位16 进制都打印出来了。如果你想看si 的本来面目，那么就应该让编译器做0 扩展而不是
符号扩展（扩展时二进制左边补0 而不是补符号位）：
sprintf(s, "%04X", (unsigned short)si);
就可以了。或者：
unsigned short si = -1;
sprintf(s, "%04X", si);
sprintf 和printf 还可以按8 进制打印整数字符串，使用”%o”。注意8 进制和16 进制都不会打
印出负数，都是无符号的，实际上也就是变量的内部编码的直接的16 进制或8 进制表示。
控制浮点数打印格式
浮点数的打印和格式控制是sprintf 的又一大常用功能，浮点数使用格式符”%f”控制，默认保
留小数点后6 位数字，比如：
sprintf(s, "%f", 3.1415926); //产生"3.141593"
但有时我们希望自己控制打印的宽度和小数位数，这时就应该使用：”%m.nf”格式，其中m 表
示打印的宽度，n 表示小数点后的位数。比如：
sprintf(s, "%10.3f", 3.1415626); //产生：" 3.142"
sprintf(s, "%-10.3f", 3.1415626); //产生："3.142 "
sprintf(s, "%.3f", 3.1415626); //不指定总宽度，产生："3.142"
注意一个问题，你猜
int i = 100;
sprintf(s, "%.2f", i);
会打出什么东东来？“100.00”？对吗？自己试试就知道了，同时也试试下面这个：
sprintf(s, "%.2f", (double)i);
第一个打出来的肯定不是正确结果，原因跟前面提到的一样，参数压栈时调用者并不知道跟i
相对应的格式控制符是个”%f”。而函数执行时函数本身则并不知道当年被压入栈里的是个整数，
于是可怜的保存整数i 的那4 个字节就被不由分说地强行作为浮点数格式来解释了，整个乱套了。
不过，如果有人有兴趣使用手工编码一个浮点数，那么倒可以使用这种方法来检验一下你手
工编排的结果是否正确。
字符/Ascii 码对照
我们知道，在C/C++语言中，char 也是一种普通的scalable 类型，除了字长之外，它与short，
int，long 这些类型没有本质区别，只不过被大家习惯用来表示字符和字符串而已。（或许当年该把
这个类型叫做“byte”，然后现在就可以根据实际情况，使用byte 或short 来把char 通过typedef 定
义出来，这样更合适些）
于是，使用”%d”或者”%x”打印一个字符，便能得出它的10 进制或16 进制的ASCII 码；反过
来，使用”%c”打印一个整数，便可以看到它所对应的ASCII 字符。以下程序段把所有可见字符的
ASCII 码对照表打印到屏幕上（这里采用printf，注意”#”与”%X”合用时自动为16 进制数增加”0X”
前缀）：
for(int i = 32; i < 127; i++) {
printf("[ %c ]: %3d 0x%#04X\n", i, i, i);
}
连接字符串
sprintf 的格式控制串中既然可以插入各种东西，并最终把它们“连成一串”，自然也就能够连
接字符串，从而在许多场合可以替代strcat，但sprintf 能够一次连接多个字符串（自然也可以同时
在它们中间插入别的内容，总之非常灵活）。比如：
char* who = "I";
char* whom = "CSDN";
sprintf(s, "%s love %s.", who, whom); //产生："I love CSDN. "
strcat 只能连接字符串（一段以’\0’结尾的字符数组或叫做字符缓冲，null-terminated-string），
但有时我们有两段字符缓冲区，他们并不是以’\0’结尾。比如许多从第三方库函数中返回的字符数
组，从硬件或者网络传输中读进来的字符流，它们未必每一段字符序列后面都有个相应的’\0’来结
尾。如果直接连接，不管是sprintf 还是strcat 肯定会导致非法内存操作，而strncat 也至少要求第
一个参数是个null-terminated-string，那该怎么办呢？我们自然会想起前面介绍打印整数和浮点数
时可以指定宽度，字符串也一样的。比如：
char a1[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
char a2[] = {'H', 'I', 'J', 'K', 'L', 'M', 'N'};
如果：
sprintf(s, "%s%s", a1, a2); //Don't do that!
十有八九要出问题了。是否可以改成：
sprintf(s, "%7s%7s", a1, a2);
也没好到哪儿去，正确的应该是：
sprintf(s, "%.7s%.7s", a1, a2);//产生："ABCDEFGHIJKLMN"
这可以类比打印浮点数的”%m.nf”，在”%m.ns”中，m 表示占用宽度（字符串长度不足时补空
格，超出了则按照实际宽度打印），n 才表示从相应的字符串中最多取用的字符数。通常在打印字
符串时m 没什么大用，还是点号后面的n 用的多。自然，也可以前后都只取部分字符：
sprintf(s, "%.6s%.5s", a1, a2);//产生："ABCDEFHIJKL"
在许多时候，我们或许还希望这些格式控制符中用以指定长度信息的数字是动态的，而不是
静态指定的，因为许多时候，程序要到运行时才会清楚到底需要取字符数组中的几个字符，这种
动态的宽度/精度设置功能在sprintf 的实现中也被考虑到了，sprintf 采用”*”来占用一个本来需要一
个指定宽度或精度的常数数字的位置，同样，而实际的宽度或精度就可以和其它被打印的变量一
样被提供出来，于是，上面的例子可以变成：
sprintf(s, "%.*s%.*s", 7, a1, 7, a2);
或者：
sprintf(s, "%.*s%.*s", sizeof(a1), a1, sizeof(a2), a2);
实际上，前面介绍的打印字符、整数、浮点数等都可以动态指定那些常量值，比如：
sprintf(s, "%-*d", 4, 'A'); //产生"65 "
sprintf(s, "%#0*X", 8, 128); //产生"0X000080"，"#"产生0X
sprintf(s, "%*.*f", 10, 2, 3.1415926); //产生" 3.14"
打印地址信息
有时调试程序时，我们可能想查看某些变量或者成员的地址，由于地址或者指针也不过是个32 位的数，你完全可以使用打印无符号整数的”%u”把他们打印出来：
sprintf(s, "%u", &i);
不过通常人们还是喜欢使用16 进制而不是10 进制来显示一个地址：
sprintf(s, "%08X", &i);
然而，这些都是间接的方法，对于地址打印，sprintf 提供了专门的”%p”：
sprintf(s, "%p", &i);
我觉得它实际上就相当于：
sprintf(s, "%0*x", 2 * sizeof(void *), &i);
利用sprintf 的返回值
较少有人注意printf/sprintf 函数的返回值，但有时它却是有用的，spritnf 返回了本次函数调用
最终打印到字符缓冲区中的字符数目。也就是说每当一次sprinf 调用结束以后，你无须再调用一次
strlen 便已经知道了结果字符串的长度。如：
int len = sprintf(s, "%d", i);
对于正整数来说，len 便等于整数i 的10 进制位数。
下面的是个完整的例子，产生10 个[0, 100)之间的随机数，并将他们打印到一个字符数组s 中，以逗号分隔开。
```cpp
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main() {
srand(time(0));
char s[64];
int offset = 0;
for(int i = 0; i < 10; i++) {
offset += sprintf(s + offset, "%d,", rand() % 100);
}
s[offset - 1] = '\n';//将最后一个逗号换成换行符。
printf(s);
return 0;
}
```
设想当你从数据库中取出一条记录，然后希望把他们的各个字段按照某种规则连接成一个字
符串时，就可以使用这种方法，从理论上讲，他应该比不断的strcat 效率高，因为strcat 每次调用
都需要先找到最后的那个’\0’的位置，而在上面给出的例子中，我们每次都利用sprintf 返回值把这
个位置直接记下来了。
使用sprintf 的常见问题
sprintf 是个变参函数，使用时经常出问题，而且只要出问题通常就是能导致程序崩溃的内存访
问错误，但好在由sprintf 误用导致的问题虽然严重，却很容易找出，无非就是那么几种情况，通
常用眼睛再把出错的代码多看几眼就看出来了。
?? 缓冲区溢出
第一个参数的长度太短了，没的说，给个大点的地方吧。当然也可能是后面的参数的问
题，建议变参对应一定要细心，而打印字符串时，尽量使用”%.ns”的形式指定最大字符数。
?? 忘记了第一个参数
低级得不能再低级问题，用printf 用得太惯了。//偶就常犯。：。（
?? 变参对应出问题
通常是忘记了提供对应某个格式符的变参，导致以后的参数统统错位，检查检查吧。尤
其是对应”*”的那些参数，都提供了吗？不要把一个整数对应一个”%s”，编译器会觉得你
欺她太甚了（编译器是obj 和exe 的妈妈，应该是个女的，:P）。
strftime
sprnitf 还有个不错的表妹：strftime，专门用于格式化时间字符串的，用法跟她表哥很像，也
是一大堆格式控制符，只是毕竟小姑娘家心细，她还要调用者指定缓冲区的最大长度，可能是为
了在出现问题时可以推卸责任吧。这里举个例子：
time_t t = time(0);
//产生"YYYY-MM-DD hh:mm:ss"格式的字符串。
char s[32];
strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", localtime(&t));
sprintf 在MFC 中也能找到他的知音：CString::Format，strftime 在MFC 中自然也有她的同道：
CTime::Format，这一对由于从面向对象哪里得到了赞助，用以写出的代码更觉优雅。
下面介绍以下printf.
printf()可以有多个参数,但第一个参数必须为字符串.可以在第一个字符串中加入一些标识符标记要输其它参数的位置和类型.比如:
printf("Theuser is %d years old.\n",age);
其中%d就标识了参数age要显示的位置和类型.当然也可以是多个参数.
printf("userinfo:age :%d :weight %d; height  %d.\n",age,weight,height);
这里后面的参数就有顺序了哈,自己要把握了.这里%d是标识整型变量的,下面我要介绍其它类型变量当如何标识.用%o(是字母o不是零哈)来标识八进制数,用%x和%X来标识十六进制数.x表示后面的十六进制中的X用小写,想法大写后面就是大写咯.用%u来标识无符号整型变量.用%ld来标识长整型值.用%f标识浮点数值.用%c来标识字符型数值.这里要举两个例子:
 printf("Theletter is %c \n",'A');
 printf("Theletter id %c\n",65);
  上面两个例子都打印字母A.
用指数格式显示浮点数:用%e或%E标识符.他们的区别也是大小写的问题如:1.2566e+01和1.2588E.用%s标识符显示一个字符串.用%p标识符显示一个指针所指向向的内存地址.如果要在数值前面加上符号,可以在%后添加一个符号.如%+d.在%后加数字表示至少要显示的位数.如%3d,不够的用空格填充.如用前面的标识符来显示11则显示出来的效果是空格+11.当然前面填充的不一定就是空格哈.也可以用数字0来填充.%03d.显示11的时候就是011了.用%#来要求显示前缀,比如说十六进制的0X.
**4.strcmp函数:**
C/C++函数，比较两个字符串
设这两个字符串为str1，str2，
若str1==str2，则返回零;
若str1>str2，则返回正数;
若str1<str2，则返回负数。
例如：if(0 == strcmp(strType, "gold"))
功 能: 串比较
用 法: int strcmp(char*str1, char *str2);
看Asic码，str1>str2，返回值 > 0；两串相等，返回0
例子1:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char *buf1 = "aaa", *buf2 ="bbb", *buf3 = "ccc";
int ptr;
ptr = strcmp(buf2, buf1);
if (ptr > 0)
printf("buffer 2 is greater than buffer1\n");
else
printf("buffer 2 is less than buffer1\n");
ptr = strcmp(buf2, buf3);
if (ptr > 0)
printf("buffer 2 is greater than buffer3\n");
else
printf("buffer 2 is less than buffer3\n");
return 0;
}
```
**5.函数名: strncmpi**
功 能: 将一个串中的一部分与另一个串比较, 不管大小写
用 法: int strncmpi(char*str1, char *str2, unsigned maxlen);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char *buf1 = "BBB", *buf2 ="bbb";
int ptr;
ptr = strcmpi(buf2, buf1);
if (ptr > 0)
printf("buffer 2 is greater than buffer1\n");
if (ptr < 0)
printf("buffer 2 is less than buffer1\n");
if (ptr == 0)
printf("buffer 2 equals buffer 1\n");
return 0;
}
```
**6.函数名: strncmp**
功 能: 串比较
用 法: int strncmp(char*str1, char *str2, int maxlen);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char *buf1 = "aaabbb", *buf2 = "bbbccc", *buf3 = "ccc";
int ptr;
ptr = strncmp(buf2,buf1,3);
if (ptr > 0)
printf("buffer 2 is greater than buffer 1\n");
else
printf("buffer 2 is less than buffer 1\n");
ptr = strncmp(buf2,buf3,3);
if (ptr > 0)
printf("buffer 2 is greater than buffer 3\n");
else
printf("buffer 2 is less than buffer 3\n");
return(0);
}
```
**7. strcpy函数:**
功 能: 拷贝一个字符串到另一个
用 法: char*stpcpy(char *destin, char *source);
例子1：
```cpp
#include <stdio.h>
#include <string.h>
int main(void)
{
char string[10];
char *str1 = "abcdefghi";
strcpy(string, str1);
printf("%s\n", string);
return 0;
}
```
**8.C/C++标准库函数strncpy():**
标准库函数strncpy()，可以将一字符串的一部分拷贝到另一个字符串中。strncpy()函数有3个参数：第一个参数是目录字符串；第二个参数是源字符串；第三个参数是一个整数，代表要从源字符串拷贝到目标字符串中的字符数。
功 能: 串拷贝
用 法: char*strncpy(char *destin, char *source, int maxlen);
示例程序1：
```cpp
#include <stdio.h>
#include <string.h>
int main(void)
{
char string[10];
char *str1 = "abcdefghi";
strncpy(string, str1, 3);
string[3] = '\0';
printf("%s\n", string);
return 0;
}
```
示例程序2：
```cpp
#include "TestStrncpy.h"
#include "windows.h"
#include <stdio.h>
#include <string.h>
void main(void)
{
	char * source_str = "THIS IS THE SOURCE STRING";
	char dest_strl[40] = { 0 }, dest_str2[40] = { 0 };
	/*Use strncpy() to copy only the first 11 characters. */
		strncpy(dest_str1, source_str, 11);
	printf("How about that! dest_strl is now: '%s'!!!\n", dest_str1);
	/*Now, use strncpy() to copy only the last 13 characters. */
		strncpy(dest_str2, source_str + (strlen(source_str) - 13), 13);
	printf("Whoa! dest_str2 is now: '%s'!!!\n",dest_str2);
	Sleep(10000);
}
```
         第一次调用strncpy()函数时，它将源字符串的头11个字符拷贝到dest_str1中，这是一种相当直接的方法。第二次调用strncpy()函数时，它将源字符串的最后13个字符拷贝到dest_str2中，其实现过程为：
          (1)用strlen()函数计算出source_str字符串的长度，即strlen(source_str)(2)将source_str的长度减去13(13是将要拷贝的字符数)，得出source_str中剩余的字符数，即pstrlen(source_str)-13 (3)将strlen(source_str)-13和source_str的地址相加，得出指向source_str中倒数第13个字符的地址的指针，即source_str+(strlen(source_str)-13)。这个指针就是strncpy()函数的第二个参数 (4)在strncpy()函数的第三个参数中指定要拷贝的字符是13。
9.Strlen函数：
头文件：#include <string.h>
strlen()函数用来计算字符串的长度，其原型为：
    unsigned int strlen (char *s)；
格式：strlen(字符数组名)；
功能：计算字符串s的(unsignedint型)长度,不包括'\0'在内；
说明：返回s的长度，不包括结束符NULL。
Sizeof与Strlen的区别与联系：
1.sizeof操作符的结果类型是[size_t](http://www.so.com/s?q=size_t&ie=utf-8&src=wenda_link)，它在头文件中[typedef](http://www.so.com/s?q=typedef&ie=utf-8&src=wenda_link)为unsignedint类型。
 该类型保证能容纳实现所建立的最大对象的字节大小。 2.sizeof是算符，strlen是函数。3.sizeof可以用类型做参数，strlen只能用char*做参数，且必须是以''\0''结尾的。[sizeof](http://www.so.com/s?q=sizeof&ie=utf-8&src=wenda_link)还可以用函数做参数，比如：short
 f(); printf("%d\n", sizeof(f())); 输出的结果是sizeof(short)，即2。4.数组做sizeof的参数不退化，传递给strlen就退化为指针了。5.大部分编译程序在编译的时候就把sizeof计算过了是类型或是变量的长度这就是sizeof(x)可以用来定义数组维数的原因 charstr[20]="0123456789";
 int a=strlen(str); //a=10; int b=sizeof(str);//而b=20;6.strlen的结果要在运行的时候才能计算出来，时用来计算字符串的长度，不是类型占内存的大小。7.sizeof后如果是类型必须加括弧，如果是变量名可以不加括弧。这是因为sizeof是个操作符不是个函数。8.当适用了于一个结构类型时或变量， sizeof 返回实际的大小，当适用一静态地空间数组， sizeof归还全部数组的尺寸。 sizeof操作符不能返回动态地被分派了的数组或外部的数组的尺寸9.数组作为参数传给函数时传的是指针而不是数组，传递的是数组的首地址，如： fun(char[8])
 fun(char []) 都等价于 fun(char *)在[C++](http://www.so.com/s?q=C%2B%2B&ie=utf-8&src=wenda_link)里参数传递数组永远都是传递指向数组首元素的指针，[编译器](http://www.so.com/s?q=%E7%BC%96%E8%AF%91%E5%99%A8&ie=utf-8&src=wenda_link)不知道数组的大小 如果想在函数内知道数组的大小，
 需要这样做： 进入函数后用[memcpy](http://www.so.com/s?q=memcpy&ie=utf-8&src=wenda_link)拷贝出来，长度由另一个形参传进去fun(unsiged char *p1, int len) { unsigned
 char* buf = new unsigned char[len+1]memcpy(buf, p1, len); }我们能常在用到 sizeof和 strlen的时候，通常是计算字符串数组的长度看了上面的详细解释，发现两者的使用还是有区别的，从这个例子可以看得很清楚：charstr[20]="0123456789";
 int a=strlen(str); //a=10; >>>>strlen计算字符串的长度，以结束符0x00为字符串结束。 intb=sizeof(str); //而b=20;>>>>
 sizeof计算的则是分配的数组 str[20]所占的内存空间的大小，不受里面存储的内容改变。上面是对[静态数组](http://www.so.com/s?q=%E9%9D%99%E6%80%81%E6%95%B0%E7%BB%84&ie=utf-8&src=wenda_link)处理的结果，如果是对指针，结果就不一样了char*ss
 = "0123456789"; sizeof(ss) 结果 4＝＝＝》ss是指向[字符串常量](http://www.so.com/s?q=%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%B8%B8%E9%87%8F&ie=utf-8&src=wenda_link)的字符指针，sizeof获得的是一个指针的之所占的空间,应该是长整型的，所以是4sizeof(*ss)结果1＝＝＝》*ss是第一个字符其实就是获得了字符串的第一位'0'所占的内存空间，是char类型的，占了 1位strlen(ss)=10
 >>>>如果要获得这个字符串的长度，则一定要使用 strlen。
**10.Sleep函数:**
功能: 执行挂起一段时间　
用法: unsigned sleep(unsigned seconds); 　
注意: 　
在VC中使用带上头文件#include <windows.h>,在Linux下,gcc编译器中，使用的头文件因gcc版本的不同而不同#include<unistd.h>　
在VC中,Sleep中的第一个英文字符为大写的"S" ,在linux下不要大写，在标准C中是sleep, 不要大写，简单的说VC用Sleep, 别的一律使用sleep
在VC中，Sleep()里面的单位，是以毫秒为单位，所以如果想让函数滞留1秒的话，应该是Sleep(1000); 在Linux下,sleep()里面的单位是秒，而不是毫秒。
示例：
```cpp
#include <windows.h> 　
int main() 　
{ 　
　　int a;
　　a=1000; 　
　　Sleep(a); 　
　　return 0; 　
}
```
**11.usleep函数:**
功能:　usleep功能把进程挂起一段时间，单位是微秒us（百万分之一秒）。
语法: void usleep(int micro_seconds);
返回值: 无
注意：这个函数不能工作在 Windows 操作系统中。
usleep() 与sleep()类似，用于延迟挂起进程。进程被挂起放到reday queue。只是一般情况下，延迟时间数量级是秒的时候，尽可能使用sleep()函数。且此函数已被废除，可使用nanosleep。　
如果延迟时间为几十毫秒，或者更小，尽可能使用usleep()函数。这样才能最佳的利用CPU时间。
** 12.delay函数:　**
功能: 将程序的执行暂停一段时间，单位是毫秒ms(千分之一秒) 　
用法: void delay(unsigned milliseconds); 　
示例: 　
```cpp
#include<dos.h> 　
int main(void) 　
{ 　
sound(440); 　
delay(500); 　
}
```
**13.c/c++ 转换说明符**:
1. 通配符
      %a(%A)             浮点数、十六进制数字和p-(P-)记数法(C99)
      %c                      表示以(ASCII)字符
      %d                      表示将输出值以整数对待(符号十进制整数) %md,m为指定的输出字段的宽度。如果数据的位数小于m，则左端补以空格，若大于m，则按实际位数输出
      %s            字符串
      %f                       表示以32bit浮点数(包括float和doulbe)
      %e(%E)             浮点数指数输出[e-(E-)记数法]
      %g(%G)             浮点数不显无意义的零"0"
      %i              有符号十进制整数(与%d相同)
      %u             无符号十进制整数
      %o             八进制整数   e.g.     0123
      %x(%X)              十六进制整数0f(0F)  e.g.   0x1234
      %p             指针
      %%            "%"
备注：[c语言，%d %.2d %2d%02d的区别](http://blog.csdn.net/zhaohuaonline/article/details/28241985)
%d就是普通的输出了
%2d是将数字按宽度为2，采用右对齐方式输出，若数据位数不到2位，则左边补空格
%02d，和%2d差不多，只不过左边补0
%.2d没见过，但从执行效果来看，和%02d一样
2．标志
      左对齐："-"   e.g.   "%-20s"
      右对齐："+"  e.g.   "%+20s"
      空格：若符号为正，则显示空格，负则显示"-"   e.g.   "%  6.2f"     
      #：对c,s,d,u类无影响；对o类，在输出时加前缀o；对x类，在输出时加前缀0x；
           对e,g,f 类当结果有小数时才给出小数点。
3．格式字符串（格式）
      〔标志〕〔输出最少宽度〕〔．精度〕〔长度〕类型
     "％-md" ：左对齐，若m比实际少时，按实际输出。
     "%m.ns"：输出m位，取字符串(左起)n位，左补空格，当n>mor m省略时m=n
 e.g.    "%7.2s"   输入CHINA
VFP字符函数的一种。表示的是字符型函数。格式是：SUBSTR(<字符表达式>、<数值表达式1>[,<数值表达式2>].  输出"   CH"
     "%m.nf"：输出浮点数，m为宽度，n为小数点右边数位
e.g.    "%3.1f"    输入3852.99
       输出3853.0 
      长度：为h短整形量,l为长整形量
**14. strcat函数:**
功 能: 字符串拼接函数
用 法: char*strcat(char *destin, char *source);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char destination[25];
char *blank = " ", *c ="C++", *Borland = "Borland";
strcpy(destination, Borland);
strcat(destination, blank);
strcat(destination, c);
printf("%s\n", destination);
return 0;
}
```
**15. strchr函数:**
功 能: 在一个串中查找给定字符的第一个匹配之处\
用 法: char*strchr(char *str, char c);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char string[15];
char *ptr, c = 'r';
strcpy(string, "This is a string");
ptr = strchr(string, c);
if (ptr)
printf("The character %c is at position:%d\n", c, ptr-string);
else
printf("The character was notfound\n");
return 0;
}
```
**16.函数名: strcspn**
功 能: 在串中查找第一个给定字符集内容的段
用 法: int strcspn(char*str1, char *str2);
程序例:
```cpp
#include <stdio.h>
#include <string.h>
#include <alloc.h>
int main(void)
{
char *string1 = "1234567890";
char *string2 = "747DC8";
int length;
length = strcspn(string1, string2);
printf("Character where strings intersect is at position %d\n", length);
return 0;
}
```
**17.函数名: strdup**
功 能: 将串拷贝到新建的位置处
用 法: char*strdup(char *str);
程序例:
```cpp
#include <stdio.h>
#include <string.h>
#include <alloc.h>
int main(void)
{
char *dup_str, *string = "abcde";
dup_str = strdup(string);
printf("%s\n", dup_str);
free(dup_str);
return 0;
}
```
**18.函数名: stricmp**
功 能: 以大小写不敏感方式比较两个串
用 法: int stricmp(char*str1, char *str2);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char *buf1 = "BBB", *buf2 = "bbb";
int ptr;
ptr = stricmp(buf2, buf1);
if (ptr > 0)
printf("buffer 2 is greater than buffer 1\n");
if (ptr < 0)
printf("buffer 2 is less than buffer 1\n");
if (ptr == 0)
printf("buffer 2 equals buffer 1\n");
return 0;
}
```
**19.函数名: strerror**
功 能: 返回指向错误信息字符串的指针
用 法: char*strerror(int errnum);
程序例:
```cpp
#include <stdio.h>
#include <errno.h>
int main(void)
{
char *buffer;
buffer = strerror(errno);
printf("Error: %s\n", buffer);
return 0;
}
```
**20.函数名: strcmpi**
功 能: 将一个串与另一个比较, 不管大小写
用 法: int strcmpi(char*str1, char *str2);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char *buf1 = "BBB", *buf2 = "bbb";
int ptr;
ptr = strcmpi(buf2, buf1);
if (ptr > 0)
printf("buffer 2 is greater than buffer 1\n");
if (ptr < 0)
printf("buffer 2 is less than buffer 1\n");
if (ptr == 0)
printf("buffer 2 equals buffer 1\n");
return 0;
}
```
**21.函数名: strnicmp**
功 能: 不注重大小写地比较两个串
用 法: intstrnicmp(char *str1, char *str2, unsigned maxlen);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char *buf1 = "BBBccc", *buf2 = "bbbccc";
int ptr;
ptr = strnicmp(buf2, buf1, 3);
if (ptr > 0)
printf("buffer 2 is greater than buffer 1\n");
if (ptr < 0)
printf("buffer 2 is less than buffer 1\n");
if (ptr == 0)
printf("buffer 2 equals buffer 1\n");
return 0;
}
```
**22.函数名: strnset**
功 能: 将一个串中的所有字符都设为指定字符
用 法: char*strnset(char *str, char ch, unsigned n);
程序例:
```cpp
#include <stdio.h>
#include <string.h>
int main(void)
{
char *string = "abcdefghijklmnopqrstuvwxyz";
char letter = 'x';
printf("string before strnset: %s\n", string);
strnset(string, letter, 13);
printf("string after strnset: %s\n", string);
return 0;
}
```
**23.函数名: strpbrk**
功 能: 在串中查找给定字符集中的字符
用 法: char*strpbrk(char *str1, char *str2);
程序例:
```cpp
#include <stdio.h>
#include <string.h>
int main(void)
{
char *string1 = "abcdefghijklmnopqrstuvwxyz";
char *string2 = "onm";
char *ptr;
ptr = strpbrk(string1, string2);
if (ptr)
printf("strpbrk found first character: %c\n", *ptr);
else
printf("strpbrk didn't find character in set\n");
return 0;
}
```
**24.函数名: strrchr**
功 能: 在串中查找指定字符的最后一个出现
用 法: char*strrchr(char *str, char c);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char string[15];
char *ptr, c = 'r';
strcpy(string, "This is a string");
ptr = strrchr(string, c);
if (ptr)
printf("The character %c is at position: %d\n", c, ptr-string);
else
printf("The character was not found\n");
return 0;
}
```
**25.函数名: strrev**
功 能: 串倒转
用 法: char*strrev(char *str);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char *forward = "string";
printf("Before strrev(): %s\n", forward);
strrev(forward);
printf("After strrev(): %s\n", forward);
return 0;
}
```
**26.函数名: strset**
功 能: 将一个串中的所有字符都设为指定字符
用 法: char*strset(char *str, char c);
程序例:
```cpp
#include <stdio.h>
#include <string.h>
int main(void)
{
char string[10] = "123456789";
char symbol = 'c';
printf("Before strset(): %s\n", string);
strset(string, symbol);
printf("After strset(): %s\n", string);
return 0;
}
```
**27.函数名: strspn**
功 能: 在串中查找指定字符集的子集的第一次出现
用 法: int strspn(char*str1, char *str2);
程序例:
```cpp
#include <stdio.h>
#include <string.h>
#include <alloc.h>
int main(void)
{
char *string1 = "1234567890";
char *string2 = "123DC8";
int length;
length = strspn(string1, string2);
printf("Character where strings differ is at position %d\n", length);
return 0;
}
```
**28.函数名: strstr**
功 能: 在串中查找指定字符串的第一次出现
用 法: char*strstr(char *str1, char *str2);
程序例:
```cpp
#include <stdio.h>
#include <string.h>
int main(void)
{
char *str1 = "Borland International", *str2 = "nation", *ptr;
ptr = strstr(str1, str2);
printf("The substring is: %s\n", ptr);
return 0;
}
```
**29.函数名: strtod**
功 能: 将字符串转换为double型值
用 法: doublestrtod(char *str, char **endptr);
程序例:
```cpp
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
char input[80], *endptr;
double value;
printf("Enter a floating point number:");
gets(input);
value = strtod(input, &endptr);
printf("The string is %s the number is %lf\n", input, value);
return 0;
}
```
**30.函数名: strtok**
功 能: 查找由在第二个串中指定的分界符分隔开的单词
用 法: char *strtok(char*str1, char *str2);
程序例:
```cpp
#include <string.h>
#include <stdio.h>
int main(void)
{
char input[16] = "abc,d";
char *p;
p = strtok(input, ",");
if (p) printf("%s\n", p);
p = strtok(NULL, ",");
if (p) printf("%s\n", p);
return 0;
}
```
**31.函数名: strtol**
功 能: 将串转换为长整数
用 法: long strtol(char*str, char **endptr, int base);
程序例:
```cpp
#include <stdlib.h>
#include <stdio.h>
int main(void)
{
char *string = "87654321", *endptr;
long lnumber;
lnumber = strtol(string, &endptr, 10);
printf("string = %s long = %ld\n", string, lnumber);
return 0;
}
```
**32.函数名: strupr**
功 能: 将串中的小写字母转换为大写字母
用 法: char*strupr(char *str);
程序例:
```cpp
#include <stdio.h>
#include <string.h>
int main(void)
{
char *string = "abcdefghijklmnopqrstuvwxyz", *ptr;
ptr = strupr(string);
printf("%s\n", ptr);
return 0;
}
```
**33.函数名: swab**
功 能: 交换字节
用 法: void swab (char*from, char *to, int nbytes);
程序例:
```cpp
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char source[15] = "rFna koBlrna d";
char target[15];
int main(void)
{
swab(source, target, strlen(source));
printf("This is target: %s\n", target);
return 0;
```
**34.函数名:strlen**
功能：计算字符串长度，计算字符串以零结尾的长度
用 法:  intstrlen(char *s);
WINDOWS 字符串处理函数  API
lstrcmp 区分大小写;
返回值: -1、0、1, 其中 0 表示相同
```cpp
#include <string.h>
#include<stdio.h> 
int main(void)
{ 　　
char *s="Golden Global View"; 
printf("%s has %d chars",s,strlen(s)); 
getchar(); 　
return 0; 
}
```
if(lstrcmp(str1,str2)!=0)
{
}
str1,str2是两个字符串；
lstrcmpi 不区分大小写
返回值: -1、0、1, 其中 0 表示相同
if(lstrcmpi(str1,str2)!=0)
{
}
str1,str2是两个字符串；
 lstrcpyn - 复制字符串, 同时指定要复制的长度
返回值:该函数返回指向缓冲区的指针（即使源字符串长于第三参数所指定长度，函数succeed）。
如果失败，返回NULL,lpString1不以空字符串"\0"结束。 
LPTSTR lstrcpyn( __out LPTSTR lpString1, __inLPCTSTR lpString2, __in int iMaxLength);
该函数把第二个参数指定的字符串复制到第一个参数指定的字符串，第三个参数指定复制的长度。
  lstrcat - 合并字符串
 p1 := 'Delphi';
 p2 := '2016';
 lstrcat(p, p1);
 lstrcat(p, ' ');
 lstrcat(p, p2);
 ShowMessage(p); {Delphi 2016}
**35、malloc/calloc**|名称：|Malloc/calloc|
|----|----|
|功能：|动态内存分配函数|
|头文件：|#include <stdlib.h>|
|函数原形：|void *malloc(size_t size);void *calloc(size_t num,size_t size);|
|参数:|size    分配内存块的大小num　　分配内存块的个数|
|返回值：|成功返回分配内存块的首地址，失败返回NULL.|
malloc和calloc都可以分配内存区，但malloc一次只能申请一个内存区，calloc一次可以申请多个内存区．另外calloc会把分配来的内存区初试化为０，malloc不会进行初始化．
#include <stdio.h>
#include <stdlib.h>
main()
{
    int *p=NULL;
    p=(int *)malloc(sizeof(int));
    if(p==NULL)
    {
        printf("malloc error/n");
        exit(1);
    }
    *p=3;
    printf("%d/n",*p);
    free(p);
}
**36、malloc/calloc**
|名称：|free|
|----|----|
|功能：|动态内存释放函数|
|头文件：|#include <stdlib.h>|
|函数原形：|void free(void *ptr);|
|参数:|ptr   使用malloc或calloc等内存分配函数所返回的内存指针|
|返回值：|无|
free 可以释放由malloc或calloc等内存分配函数分配的内存．当程序很大时，期间可能要多次动态分配内存，如果不及时释放的话，程序将要占用很大内存．
要注意，如果ptr所指内存已被释放或是未知的内存地址，则可能有无法预期的情况发生．若参数为NULL，则free不会有任何作用．
**37、memset**
|名称：|memset|
|----|----|
|功能：|初始化所指定的内存空间|
|头文件：|#include <stdlib.h>|
|函数原形：|void *memset(void *buffer,int c,int count);|
|参数:|buffer      分配的内存c 　　　　 初始化内容count      初始化的字节数|
|返回值：|返回指向buffer的指针|
memset把buffer所指内存区域的前count个字节设置成某个字符的ASCLL值．一般用于给数组，字符串等类型赋值．
main()
{
    int *p=NULL;
    int i;
    char *q=NULL;
    p=(int *)malloc(sizeof(int)*10);
    if(p==NULL)
        exit(1);
    memset(p,0,sizeof(int)*10);
    q=p;
    for(i=0;i<10;i++)
        printf("%d",*(q++));
    free(p);
｝
 执行结果是10个０．
**38、memcpy**
|名称：|memcpy|
|----|----|
|功能：|拷贝内存空间|
|头文件：|#include <stdlib.h>|
|函数原形：|void *memcpy(void *dest,void *src,unsigned int count);|
|参数:|dest       目标内存区src　　　　原内存区count      要复制的字节数|
|返回值：|指向dest的指针|
memcpy会把src所指内存区复制count个字节到dest所指内存区．如果count比src字节数大，strcpy会拷贝'/0'后结束．要注意dest和src不要重叠．memcpy只是拷贝内存空间，不处理空间重叠的问题．
main()
{
    int *p1=NULL;
    int *p2=NULL;
　　int q;
    int i;
    p1=malloc(sizeof(int)*10);
    if(p1==NULL)
        exit(1);
    p2=malloc(sizeof(int)*5);
    if(p2==NULL)
        exit(1);
    memset(p1,0,sizeof(int)*10);
    memcpy(p2,p1,sizeof(int)*5);
    q=p2;
    for(i=0;i<5;i++)
        printf("%d",*(q++));
    free(p1);
    free(p2);
)
运行结果为５个０．
**39、memmove**
|名称：|memmove|
|----|----|
|功能：|拷贝（移动）内存空间|
|头文件：|#include <stdlib.h>|
|函数原形：|void *memmove(void *dest,void *src,unsigned int count);|
|参数:|dest       目标内存区src　　　　原内存区count      要复制的字节数|
|返回值：|指向dest的指针|
Memmove和函数memcpy函数功能一样，但只是拷贝内存空间，不处理空间重叠的问题．Memmove会处理空间重叠问题．当dest和src重叠时，仍能正确处理，但src内容发生改变．
**40、memmove**
|名称：|memcmp|
|----|----|
|功能：|比较两个内存空间的字符|
|头文件：|#include <stdlib.h>|
|函数原形：|int memcmp(void *buf1,void *buf2,unsigned int count);|
|参数:|buf1       内存区buf2　　　 内存区count      要比较的字符数|
|返回值：|见下面|
Memcmp会比较内存区域buf1和buf2的前count个字节．Memcmp回根据ASCLL码表顺序依次比较．当buf1<buf2时，返回<0;当buf1=buf2时，返回０;当buf1>buf2时，返回＞０．
main()
{
    int *p1=NULL;
    int *p2=NULL;
    int rt;
    p1=malloc(sizeof(int)*10);
    if(p1==NULL)
        exit(1);
    p2=malloc(sizeof(int)*10);
    if(p2==NULL)
        exit(1);
    memset(p1,'a',sizeof(int)*10);
    memset(p2,'b',sizeof(int)*10);
    rt=memcmp(p1,p2,sizeof(int)*10);
    if(rt>0)
        printf("p1>p2);
    if(rt<0)
        printf("p1<p2");
    if(rt==0)
        printf("p1=p2");
    free(p1);
    free(p2);
}
运行结果：p1<p2
