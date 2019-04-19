# C++精进篇（五）之―字符串和字符串函数 - fanyun的博客 - CSDN博客
2016年03月25日 22:17:06[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：596
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
在C++中，有char*的字符串，有string的字符串，我们平时大多选用C++标准程序库中的string类，是因为他和前者比较起来，不必担心内存是否足够、字符串长度等等，而且作为一个类出现，他集成的操作函数足以完成我们绝大多数情况下的需要。
在使用string字符串时，首先先包含头文件:#include <string> //注意这里不是string.h string.h是C字符串头文件
其次，声明: string Str;
这样我们就声明了一个字符串变量，但既然是一个类，就有构造函数和析构函数。上面的声明没有传入参数，所以就直接使用了string的默认的构造函数，这个函数所作的就是把Str初始化为一个空字符串。String类的构造函数和析构函数如下：
1. string有多种构造方式,按使用频率排列为:
    string()  or  string s;                            //构造空字符串
    string( conststring& s );   or  string s(str)         //拷贝构造
    string(input_iterator start, input_iterator end );    //迭代器构造
    string( conststring& str, size_type index, size_type length );//拷贝一个字符串的子串
    string( size_typelength, const char& ch );   //用单个字符构造
    string( const char*str);                                   //用C风格字符串构造
    string( constchar* str, size_type length );//截取C风格字符窜的前length个字符构造
   strings(str,stridx) //将字符串str内“始于位置stridx”的部分当作字符串的初值
   string s(cstr) //将C字符串作为s的初值
   string s(num,c) //生成一个字符串，包含num个c字符
   strings(str,stridx,strlen) //将字符串str内“始于stridx且长度顶多strlen”的部分作为字符串的初值
   strings(beg,end) //以区间beg;end(不包含end)内的字符作为字符串s的初值
   strings(chars,chars_len) //将C字符串前chars_len个字符作为字符串s的初值。
  s.~string()//销毁所有字符，释放内存
2. 为了方便使用string重载了以下操作符:
     //比较 全部是以字典序为基础的
    booloperator==(const string& c1, const string& c2);
    booloperator!=(const string& c1, const string& c2);
    booloperator<(const string& c1, const string& c2);
    booloperator>(const string& c1, const string& c2);
    booloperator<=(const string& c1, const string& c2);
    booloperator>=(const string& c1, const string& c2);
    //+   可与 string,C风格字符串,字符 相加
    stringoperator+(const string& s1, const string& s2 );
    stringoperator+(const char* s, const string& s2 );
    string operator+(char c, const string& s2 );
    string operator+(const string& s1, const char* s );
    string operator+(const string& s1, char c );
    basic_string&operator+=(const basic_string& append);
    basic_string&operator+=(const char* append);
    basic_string&operator+=(const char  append);
    //输入输出
    ostream&operator<<( ostream& os, const string& s );
    istream& operator>>(istream& is, string& s );
    string&operator=( const string& s );
    //赋值
    string&operator=( const char* s );
    string&operator=( char ch );       //不常用的赋值
    char&operator[]( size_type index );
     其中最后一个可能是使用的最频繁的,它让程序员可以像访问字符数组一样访问字符串的元素.
函数名: stpcpy
功 能: 拷贝一个字符串到另一个
用 法: char*stpcpy(char *destin, char *source);
程序例:
#include <stdio.h>
#include <string.h>
int main(void)
{
   char string[10];
   char *str1 = "abcdefghi";
   stpcpy(string, str1);
   printf("%s\n", string);
   return 0;
}
函数名: strcat
功 能: 字符串拼接函数
用 法: char*strcat(char *destin, char *source);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char destination[25];
   char *blank = " ", *c = "C++", *Borland ="Borland";
   strcpy(destination, Borland);
   strcat(destination, blank);
   strcat(destination, c);
   printf("%s\n", destination);
   return 0;
}
函数名: strchr
功 能: 在一个串中查找给定字符的第一个匹配之处\
用 法: char*strchr(char *str, char c);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
    char string[15];
    char *ptr, c = 'r';
    strcpy(string, "This is a string");
    ptr = strchr(string, c);
    if (ptr)
       printf("The character %c is atposition: %d\n", c, ptr-string);
    else
       printf("The character was notfound\n");
    return 0;
}
函数名: strcmp
功 能: 串比较
用 法: intstrcmp(char *str1, char *str2);
看Asic码，str1>str2，返回值 > 0；两串相等，返回0
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
    char *buf1 = "aaa", *buf2 = "bbb",*buf3 = "ccc";
    int ptr;
    ptr = strcmp(buf2, buf1);
    if (ptr > 0)
       printf("buffer 2 is greater thanbuffer 1\n");
    else
       printf("buffer 2 is less thanbuffer 1\n");
    ptr = strcmp(buf2, buf3);
    if (ptr > 0)
       printf("buffer 2 is greater thanbuffer 3\n");
    else
       printf("buffer 2 is less thanbuffer 3\n");
    return 0;
}
函数名: strncmpi
功 能: 将一个串中的一部分与另一个串比较, 不管大小写
用 法: intstrncmpi(char *str1, char *str2, unsigned maxlen);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char *buf1 = "BBB", *buf2 = "bbb";
   int ptr;
   ptr = strcmpi(buf2, buf1);
   if (ptr > 0)
      printf("buffer 2 is greater thanbuffer 1\n");
   if (ptr < 0)
      printf("buffer 2 is less than buffer1\n");
   if (ptr == 0)
      printf("buffer 2 equals buffer1\n");
   return 0;
}
函数名: strcpy
功 能: 串拷贝
用 法: char*strcpy(char *str1, char *str2);
程序例:
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
函数名: strcspn
功 能: 在串中查找第一个给定字符集内容的段
用 法: intstrcspn(char *str1, char *str2);
程序例:
#include <stdio.h>
#include <string.h>
#include <alloc.h>
int main(void)
{
    char *string1 = "1234567890";
    char *string2 = "747DC8";
    int length;
    length = strcspn(string1, string2);
    printf("Character where strings intersect is atposition %d\n", length);
    return 0;
}
函数名: strdup
功 能: 将串拷贝到新建的位置处
用 法: char*strdup(char *str);
程序例:
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
函数名: stricmp
功 能: 以大小写不敏感方式比较两个串
用 法: intstricmp(char *str1, char *str2);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char *buf1 = "BBB", *buf2 = "bbb";
   int ptr;
   ptr = stricmp(buf2, buf1);
   if (ptr > 0)
      printf("buffer 2 is greater thanbuffer 1\n");
   if (ptr < 0)
      printf("buffer 2 is less than buffer1\n");
   if (ptr == 0)
      printf("buffer 2 equals buffer 1\n");
   return 0;
}
函数名: strerror
功 能: 返回指向错误信息字符串的指针
用 法: char*strerror(int errnum);
程序例:
#include <stdio.h>
#include <errno.h>
int main(void)
{
   char *buffer;
   buffer = strerror(errno);
   printf("Error: %s\n", buffer);
   return 0;
}
函数名: strcmpi
功 能: 将一个串与另一个比较, 不管大小写
用 法: intstrcmpi(char *str1, char *str2);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char *buf1 = "BBB", *buf2 = "bbb";
   int ptr;
   ptr = strcmpi(buf2, buf1);
   if (ptr > 0)
      printf("buffer 2 is greater thanbuffer 1\n");
   if (ptr < 0)
      printf("buffer 2 is less than buffer1\n");
   if (ptr == 0)
      printf("buffer 2 equals buffer1\n");
   return 0;
}
函数名: strncmp
功 能: 串比较
用 法: intstrncmp(char *str1, char *str2, int maxlen);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char *buf1 = "aaabbb", *buf2 = "bbbccc",*buf3 = "ccc";
   int ptr;
   ptr = strncmp(buf2,buf1,3);
   if (ptr > 0)
      printf("buffer 2 is greater thanbuffer 1\n");
   else
      printf("buffer 2 is less than buffer1\n");
   ptr = strncmp(buf2,buf3,3);
   if (ptr > 0)
      printf("buffer 2 is greater thanbuffer 3\n");
   else
      printf("buffer 2 is less than buffer3\n");
   return(0);
}
函数名: strncmpi
功 能: 把串中的一部分与另一串中的一部分比较, 不管大小写
用 法: intstrncmpi(char *str1, char *str2);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char *buf1 = "BBBccc", *buf2 = "bbbccc";
   int ptr;
   ptr = strncmpi(buf2,buf1,3);
   if (ptr > 0)
      printf("buffer 2 is greater thanbuffer 1\n");
   if (ptr < 0)
      printf("buffer 2 is less than buffer1\n");
   if (ptr == 0)
      printf("buffer 2 equals buffer1\n");
   return 0;
}
函数名: strncpy
功 能: 串拷贝
用 法: char*strncpy(char *destin, char *source, int maxlen);
程序例:
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
函数名: strnicmp
功 能: 不注重大小写地比较两个串
用 法: intstrnicmp(char *str1, char *str2, unsigned maxlen);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char *buf1 = "BBBccc", *buf2 = "bbbccc";
   int ptr;
   ptr = strnicmp(buf2, buf1, 3);
   if (ptr > 0)
      printf("buffer 2 is greater thanbuffer 1\n");
   if (ptr < 0)
      printf("buffer 2 is less than buffer1\n");
   if (ptr == 0)
      printf("buffer 2 equals buffer1\n");
   return 0;
}
函数名: strnset
功 能: 将一个串中的所有字符都设为指定字符
用 法: char*strnset(char *str, char ch, unsigned n);
程序例:
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
函数名: strpbrk
功 能: 在串中查找给定字符集中的字符
用 法: char*strpbrk(char *str1, char *str2);
程序例:
#include <stdio.h>
#include <string.h>
int main(void)
{
   char *string1 = "abcdefghijklmnopqrstuvwxyz";
   char *string2 = "onm";
   char *ptr;
   ptr = strpbrk(string1, string2);
   if (ptr)
      printf("strpbrk found first character:%c\n", *ptr);
   else
      printf("strpbrk didn't find characterin set\n");
   return 0;
}
函数名: strrchr
功 能: 在串中查找指定字符的最后一个出现
用 法: char*strrchr(char *str, char c);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char string[15];
   char *ptr, c = 'r';
   strcpy(string, "This is a string");
   ptr = strrchr(string, c);
   if (ptr)
      printf("The character %c is atposition: %d\n", c, ptr-string);
   else
      printf("The character was not found\n");
   return 0;
}
函数名: strrev
功 能: 串倒转
用 法: char*strrev(char *str);
程序例:
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
函数名: strset
功 能: 将一个串中的所有字符都设为指定字符
用 法: char*strset(char *str, char c);
程序例:
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
函数名: strspn
功 能: 在串中查找指定字符集的子集的第一次出现
用 法: intstrspn(char *str1, char *str2);
程序例:
#include <stdio.h>
#include <string.h>
#include <alloc.h>
int main(void)
{
   char *string1 = "1234567890";
   char *string2 = "123DC8";
   int length;
   length = strspn(string1, string2);
   printf("Character where strings differ is at position%d\n", length);
   return 0;
}
函数名: strstr
功 能: 在串中查找指定字符串的第一次出现
用 法: char*strstr(char *str1, char *str2);
程序例:
#include <stdio.h>
#include <string.h>
int main(void)
{
   char *str1 = "Borland International", *str2 ="nation", *ptr;
   ptr = strstr(str1, str2);
   printf("The substring is: %s\n", ptr);
   return 0;
}
函数名: strtod
功 能: 将字符串转换为double型值
用 法: doublestrtod(char *str, char **endptr);
程序例:
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
   char input[80], *endptr;
   double value;
   printf("Enter a floating point number:");
   gets(input);
   value = strtod(input, &endptr);
   printf("The string is %s the number is %lf\n",input, value);
   return 0;
}
函数名: strtok
功 能: 查找由在第二个串中指定的分界符分隔开的单词
用 法: char*strtok(char *str1, char *str2);
程序例:
#include <string.h>
#include <stdio.h>
int main(void)
{
   char input[16] = "abc,d";
   char *p;
   p = strtok(input, ",");
   if (p)   printf("%s\n", p);
   p = strtok(NULL, ",");
   if (p)   printf("%s\n", p);
   return 0;
}
函数名: strtol
功 能: 将串转换为长整数
用 法: longstrtol(char *str, char **endptr, int base);
程序例:
#include <stdlib.h>
#include <stdio.h>
int main(void)
{
   char *string = "87654321", *endptr;
   long lnumber;
   lnumber = strtol(string, &endptr, 10);
   printf("string = %s long = %ld\n", string,lnumber);
   return 0;
}
函数名: strupr
功 能: 将串中的小写字母转换为大写字母
用 法: char*strupr(char *str);
程序例:
#include <stdio.h>
#include <string.h>
int main(void)
{
   char *string = "abcdefghijklmnopqrstuvwxyz", *ptr;
   ptr = strupr(string);
   printf("%s\n", ptr);
   return 0;
}
函数名: swab
功 能: 交换字节
用 法: void swab(char *from, char *to, int nbytes);
程序例:
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
}
