# ssacanf\Sprintf格式化字符串 - maopig的专栏 - CSDN博客
2013年12月08日 10:50:00[maopig](https://me.csdn.net/maopig)阅读数：1270
个人分类：[c/C++](https://blog.csdn.net/maopig/article/category/847675)
一、sscanf
sscanf() - 从一个　　int
 sscanf(const char *buffer,const char *format,[argument ]...);
　　buffer 存储的数据
　　format 格式控制字符串
　　argument 选择性设定字符串
　　sscanf会从buffer里读进数据，依照argument的设定将数据写回。字符串中读进与指定格式相符的数据.
说明;sscanf与scanf类似，都是用于输入的，只是后者以键盘(stdin)为输入源，前者以固定字符串为输入源。　
　　1.
 常见用法。
　　char buf[512] ;
　　sscanf("123456 ", "%s", buf);//此处buf是数组名，它的意思是将123456以%s的形式存入buf中！
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
  4、格式字符串可以有多个
 
    int data1,data2,data3,data4;
 
   char a[]="192**.**16**.**1**.**1";
sscanf(a,"%d**.**%d**.**%d**.**%d",&data1,&data2,&data3,&data4);
printf("data1=%d,data2=%d,data=3%d,data=4%d\n",data1,data2,data3,data4);
结果为：data1 =192,data2 =16,data3= 1,data4 =1;
5\格式字符串可以有多个,可以非常方便的分割IP地址和MAC地址等数据
     int data1,data2,data3,data4;
    char a[]="192**:**16**:**1**:**1";
 sscanf(a,"%d**:**%d**:**%d**:**%d",&data1,&data2,&data3,&data4);
printf("data1=%d,data2=%d,data=3%d,data=4%d\n",data1,data2,data3,data4);
结果为：data1 =192,data2 =16,data3= 1,data4 =1;
6、类似字符串中的用法
　　用它来分隔类似这样的字符串2006:03:18:
　　int a, b, c;
　　sscanf("2006:03:18", "%d:%d:%d", &a, &b, &c);
　　以及2006:03:18 - 2006:04:18:
　　char sztime1[16] = "", sztime2[16] = "";
　　sscanf("2006:03:18 - 2006:04:18", "%s - %s", sztime1, sztime2);
　　但是后来，我需要处理2006:03:18-2006:04:18
　　仅仅是取消了‘-’两边的空格，却打破了%s对字符串的界定。
　　我需要重新设计一个函数来处理这样的情况？这并不复杂，但是，为了使所有的代码都有统一的风格，我需要改动很多地方，把已有的sscanf替换成我自己的分割函数。我以为我肯定需要这样做，并伴随着对sscanf的强烈不满而入睡；一觉醒来，发现其实不必。
　　format-type中有%[]这样的type field。如果读取的字符串，不是以空格来分隔的话，就可以使用%[]。
　　%[]类似于一个正则表达式。[a-z]表示读取a-z的所有字符，[^a-z]表示读取除a-z以外的所有字符。
　　所以那个问题也就迎刃而解了:
　　sscanf("2006:03:18 - 2006:04:18", "%[0-9,:] - %[0-9,:]", sztime1, sztime2);
8、给定一个字符串"hello,
 world"，仅保留world。（注意："，"之后有一空格，%s遇空格停止，加*****则是忽略第一个读到的字符串）
　　sscanf("hello, world", "%*s%s", buf);
　　printf("%s\n", buf);
　　结果为：world
%*s表示第一个匹配到的%s被过滤掉，即hello被过滤了
二、sprintf函数：函数功能：把格式化的数据写入某个字符串
　　sprintf 是个变参函数，定义如下：
　　int sprintf( char *buffer, const char *format [, argument] ... );
除了前两个参数类型固定外，后面可以接任意多个参数。而它的精华，显然就在第二个参数：
例子：
char* who = "I";
char* whom = "CSDN";
sprintf(s, "%s love %s.", who, whom); //产生："I love CSDN. "  这字符串写到s中
sprintf(s, "%10.3f", 3.1415626); //产生：" 3.142"
