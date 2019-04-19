# 必须掌握的C++常用String函数 - yixianfeng41的专栏 - CSDN博客
2017年01月22日 14:53:51[第2梦](https://me.csdn.net/yixianfeng41)阅读数：362
所属专栏：[C++编程](https://blog.csdn.net/column/details/14514.html)
本文将对常用的string函数的用法以及实现做一个总结，将介绍的函数如下： 
strcspn、strcpy、strcat、 strchr、strncmp、strcmp、strstr、strlen
## 一、strcpy函数
### 1、strcpy概述
函数原型：
```cpp
char * strcpy ( char * destination, const char * source );
```
把从src地址开始且含有’\0’结束符的字符串复制到以dest开始的地址空间
src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。复制发生在两个重叠的对象中，则这种行为未定义。如果dest空间不够，则会引起 buffer overflow。
最终返回指向dest的指针。
### 2、实现
#### 2.1、实现一
```
//
//C语言标准库函数strcpy的一种典型的工业级的最简实现。
//返回值：目标串的地址。
//对于出现异常的情况ANSI-C99标准并未定义，故由实现者决定返回值，通常为NULL。
//参数：des为目标字符串，source为原字符串。
char* strcpy(char* des,const char* source)
　
{
    assert((des != NULL) && (source != NULL));
    char* r=des;
    　while((*r++ = *source++)!='\0');
    　return des;
}
```
#### 2.2、实现二
```
char * strcpy(char * strDest,const char * strSrc)
{
    if ((NULL==strDest) || (NULL==strSrc)) 
    //[1]
    throw "Invalid argument(s)"; 
    //[2]
    char * strDestCopy = strDest; 
    //[3]
    while ((*strDest++=*strSrc++)!='\0'); 
    //[4]
    return strDestCopy;
}
```
**这个代码有几个容易出错的地方需要注意：**
错误[1]： 
- 不检查指针的有效性； 
- 检查指针的有效性时使用（（！strDest)||(!strSrc））或（！（strDest&&strSrc））。在本例中char *转换为bool是类型隐式转换，这种功能虽然灵活，但更多的是导致出错概率增大和维护成本升高。所以C++专门增加了bool、true、false三个关键字以提供更安全的条件表达式。 
- 检查指针的有效性时使用（strDest==0)这种做法。直接使用字面常量（如本例中的0）会减少程序的可维护性。0虽然简单，但程序中可能出现很多处对指针的检查，万一出现笔误，编译器不能发现，生成的程序内含逻辑错误，很难排除。而使用NULL代替0，如果出现拼写错误，编译器就会检查出来。
错误[2]： 
- return new string(“Invalid argument(s)”)；，说明答题者根本不知道返回值的用途，并且他对内存泄漏也没有警惕心。从函数中返回函数体内分配的内存是十分危险的做法，他把释放内存的义务抛给不知情的调用者，绝大多数情况下，调用者不会释放内存，这导致内存泄漏。 
- return 0；，说明答题者没有掌握异常机制。调用者有可能忘记检查返回值，调用者还可能无法检查返回值（见后面的链式表达式）。妄想让返回值肩负返回正确值和异常值的双重功能，其结果往往是两种功能都失效。应该以抛出异常来代替返回值，这样可以减轻调用者的负担、使错误不会被忽略、增强程序的可维护性。
错误[3]： 
忘记保存原始的strDest值，说明答题者逻辑思维不严密。因为如果没保存strDest，那么返回的就是’\0’，因为此时strDest指向末尾’\0’
错误[4]： 
- 循环写成while (*strDestCopy++=*strSrc++);忘记检查指针有效性。 
- 循环写成while (*strSrc!=’\0’)  *strDest++=*strSrc++；，说明答题者对边界条件的检查不力。循环体结束后，strDest字符串的末尾没有正确地加上’\0’。
**为什么要返回strDest了？**
返回strDest的原始值使函数能够支持链式表达式，增加了函数的“附加值”。
链式表达式的形式如：
```cpp
int iLength=strlen(strcpy(strA,strB));
又如：
char * strA=strcpy(new char[10],strB);
```
返回strSrc的原始值是错误的。 
- 其一，源字符串肯定是已知的，返回它没有意义； 
- 其二，不能支持形如第二例的表达式； 
- 其三，为了保护源字符串，形参用const限定strSrc所指的内容，把const char *作为char *返回，类型不符，编译报错。
### 3、应用实例
```cpp
void main( int argc, char * argv[] )
{
    char a[20], c[] = "i am teacher!";
    try
    {
       strcpy(a,c);
    }
    catch(char* strInfo)
    {
       cout << strInfo << endl;
       exit(-1);
    }
    cout << a << endl;
}
```
### 4、strncpy
函数原型：
```cpp
char *strncpy(char *dest,char *src,size_t n);
```
复制字符串src中的内容（字符，数字、汉字….）到字符串dest中，复制多少由size_tn的值决定。如果src的前n个字符不含’\0’字符，则结果不会以’\0’字符结束。如果n
### 5、strcpy_s
strcpy函数，就象gets函数一样，它没有方法来保证有效的缓冲区尺寸，所以它只能假定缓冲足够大来容纳要拷贝的字符串。在程序运行时，这将导致不可预料的行为。用strcpy_s就可以避免这些不可预料的行为。它加强了对参数合法性的检查以及缓冲区边界的检查，如果发现错误，会返回errno或抛出异常
函数原型：
`errno_t strcpy_s(char *strDestination,  size_t numberOfElements,  const char *strSource); `
或者，
```
template <size_t size>  
errno_t strcpy_s(char (&strDestination)[size], const char *strSource   ); // C++ only
```
举个例子：
```
char szBuf[3];
 memset(szBuf,0,sizeof(szBuf));
 strcpy_s(szBuf, sizeof(szBuf), "12131");  //新的CRT函数
 strcpy(szBuf,  "12131");    //老的CRT函数
```
上述代码，明显有缓冲区溢出的问题。 使用strcpy_s函数则会抛出一个异常。而使用strcpy函数的结果则未定，因为它错误地改变了程序中其他部分的内存的数据，可能不会抛出异常但导致程序数据错误，也可能由于非法内存访问抛出异常。
## 二、strlen函数
### 1、strlen概述
strlen()函数用来计算字符串的长度，不包括结束字符”\0”。,其原型为：
```cpp
unsigned int strlen (char *s);
```
头文件：#include 
### 2、strlen实现
#### 2.1、实现一
```
int strlen(const char *str)  
{  
    int i=0;  
    while(s[i] != '\0')  
         ++i;  
    return i;  
}
```
#### 2.2、实现二
```
int strlen(const char *str)  
{  
    int count=0;  
    while((*str++)!='\0')  
    {   
        count++;  
    }
    return count;  
}
```
#### 2.3、实现三
```
int strlen(char *s)  
{  
    char *p = s;  
    while (*p != '\0')  
        p++;  
    return p - s;  
}
```
#### 2.4、实现四
```
int strlen(const char *str)  
{  
    assert(NULL !=str);  
    if(*str == '\0')   
        return 0;  
    return (1+strlen(++str));  
}
```
#### 2.5、实现五
```
int strlen(const char*str)  
{  
    assert(NULL != str);  
    return ('\0'!= *str)?(1+strlen(++str)):0;  
}
```
### 3、strlen与sizeof区别
- strlen() 函数计算的是字符串的实际长度，遇到第一个’\0’结束。如果你只定义没有给它赋初值，这个结果是不定的，它会从首地址一直找下去，直到遇到’\0’停止。
- sizeof返回的是变量声明后所占的内存数，不是实际长度，此外sizeof不是函数，仅仅是一个操作符，strlen()是函数。
- sizeof可以用类型做参数，也可以用函数做参数；strlen只能用char*做参数，且必须是以”\0”结尾的。
- sizeof编译时计算，strlen运行才计算出来
```cpp
char str[10];
cout<<strlen(str)<<endl; //结果是不定的
cout<<sizeof(str)<,endl; //结果是10
char str[10]={'\0'}; 
cout<<strlen(str)<<endl; //结果为0
cout<<sizeof(str)<,endl; //结果是10
```
#### 3.1、处理静态数组
```
char str[20]="0123456789"; 
int a=strlen(str); // 结果为a=10
int b=sizeof(str); // 结果b=20;
```
#### 3.2、处理指针
```cpp
char* ss = "0123456789";
int a=sizeof(ss); // 结果4
int b=strlen(ss); //结果10
```
## 三、strstr函数
### 1、strstr概述
strstr函数的原型为：
```cpp
const char * strstr ( const char * str1, const char * str2 );
      char * strstr (char * str1, const char * str2 );
```
strstr(str1,str2) 函数用于判断字符串str2是否是str1的子串。如果是，则该函数返回str2在str1中首次出现的地址；否则，返回NULL。
举个例子：
```cpp
/* strstr example */
#include <stdio.h>
#include <string.h>
int main ()
{
    char str[] ="This is a simple string";
    char * pch;
    pch = strstr (str,"simple");
    puts (pch); //输出simple string
    return 0;
}
```
### 2、strstr实现
#### 2.1、实现一
这个是我写的最原始版本，基本没用库函数
```cpp
char *mstrstr(const char *s1, const char *s2)
{
    if (!s1 || !s2 || strlen(s2) > strlen(s1)) return NULL;
    if(strlen(s2)==0) return (char*)s1;
    while (*s1 != '\0')
    {
        const char *pos = s1;
        while (*(s1) == *(s2))
        {
            s1++,s2++;
            if (*s2 == '\0')
            {
                return (char *)pos;
            }
        }
        s1 == pos ? ++s1: pos;
    }
    return NULL;
}
```
#### 2.2、实现二
用了库函数strncmp。
```cpp
char *mstrstr(const char *s1, const char *s2)
{
    while (*s1&&*s2)
    {
        if (strncmp(s1, s2, strlen(s2)) == 0)
            return (char*)s1;
        s1++;
    }
    return NULL;
}
```
## 四、strcmp函数
### 1、strcmp概述
函数原型为：
```cpp
int strcmp ( const char * str1, const char * str2 );
```
具体比较过程是这样的，将s1 第一个字符值减去s2 第一个字符值，若差值为0 则再继续比较下个字符，直到字符结束标志’\0’，若差值不为0，则将差值返回。
因此结果，有以下三种情况： 
- 若str1==str2，则返回零； 
- 若str1
### 2、strcmp实现
#### 2.1、实现一
```
int mstrcmp(const char * str1, const char * str2)
{
    assert(NULL != str1&&NULL != str2);
    while (str1&&str2)
    {
        if (*str1 != *str2)
            return *str1 - *str2;
        str1++, str2++;
    }
    return 0;   
}
```
#### 2.2、实现二
```
int mstrcmp(const char * str1, const char * str2)
{
    assert(NULL != str1&&NULL != str2);
    while (*str1&&*str2&&*str1 == *str2) //到'\0'就停止，不然取值就会有问题
    {
        str1++, str2++;
    }
    return *str1 - *str2 ;
}
```
## 五、strncmp函数
### 1、strncmp概述
函数原型为：
```cpp
int strncmp ( const char * str1, const char * str2, size_t num );
```
此函数与strcmp极为类似。不同之处是，strncmp函数是指定比较num个字符。也就是说，如果字符串s1与s2的前num个字符相同，函数返回值为0。
### 2、实现
#### 2.1、实现一
```
int mstrncmp(const char * str1, const char * str2, size_t num)
{
    assert(NULL != str1&&NULL != str2);
    while (*str1&&*str2&&num--)
    {
        if (*str1 != *str2)
            return *str1 - *str2;
        str1++, str2++;
    }
    return 0;
}
```
#### 2.2、实现二
```
int mstrncmp(const char * str1, const char * str2, size_t num)
{
    assert(NULL != str1&&NULL != str2);
    while (*str1&&*str2&&*str1==*str2&&num--)
    {
        str1++, str2++;
    }
    return *str1-*str2;
}
```
## 六、strchr函数
### 1、strchr概述
#### 1.1、函数原型
```
const char * strrchr ( const char * str, int character );
      char * strrchr (       char * str, int character );
```
找到在字符串str中最后初选character的位置。
应用举例：
```cpp
/* strrchr example */
#include <stdio.h>
#include <string.h>
int main ()
{
    char str[] = "This is a sample string";
    char * pch;
    pch=strrchr(str,'s');
    printf ("Last occurence of 's' found at %d \n",pch-str+1); //Last occurrence of 's' found at 18
    return 0;
}
```
### 2、实现
#### 2.1、实现一
顺着往后找。
```
const char * mstrrchr(const char * str, int character)
{
    assert(str != NULL);
    const char *pos = NULL;
    while (*str)
    {
        if (*str == (char)character)
            pos = str;
        str++;
    }
    return pos;
}
```
#### 2.2、实现二
从后往前来。
```
const char * mstrrchr(const char * str, int character)
{
    assert(str != NULL);
    const char *start = str;
    while (*str++);
    while (str-- != start&&*str != (char)character);
    return *str == (char)character?str:NULL;
}
```
## 七、strcat函数
### 1、strcat概述
#### 1.1、函数原型为：
```cpp
char * strcat ( char * destination, const char * source );
```
- destination：是目标字符串，这个字符串应该有足够的内存来保存source.  
- source：要跟destination合并的字符串，这个字符串不能跟destination有内存重叠。
#### 1.2、应用举例：
```cpp
/* strcat example */
#include <stdio.h>
#include <string.h>
int main ()
{
  char str[80];
  strcpy (str,"these ");
  strcat (str,"strings ");
  strcat (str,"are ");
  strcat (str,"concatenated.");
  puts (str);  //these strings are concatenated. 
  return 0; 
}
```
### 2、实现
对于strcat，必须要考虑的就是合并后要有足够的内存来保存新的合并后的字符串。对于这个问题，可以有两种解决办法，分别是调用者和实现者： 
- 如果由调用者来解决这个问题的话，那么调用者就提前申请好内存，然后进行合并操作； 
- 如果由strcat代码实现者来解决这个问题的话，那么在strcat代码内就必须申请内存来容纳增加的字符串.我的想法是用relloc或malloc申请一段内存来保存合并后的字符串。
从strcat官方文档上的介绍来看，对于strcat的使用，内存问题似乎是调用者需要考虑的。由调用者来解决strcat的内存问题，那strcat的实现就简单了。
代码如下：
```
char * mstrcat(char *dest, const char *src)
{
    char *rdest = dest;
    while (*dest)
      dest++;
    while (*dest++ = *src++);
    return rdest;
}
```
### 3、strcat_s
对于strcat,可以发现，它是由隐患的，如果内存不够，出现溢出怎么办，所以为了安全起见，除了一个“安全版本”strcat_s,它安全在哪儿了，它首先检查内存大小师傅足够，如果够，才进行合并操作，如果不够抛出异常。
函数原型：
`errno_t strcat_s(char *strDestination,size_t numberOfElements,const char *strSource );`
举个例子：
```
char str_des[5]="a";
char *str_source = "bcd";
strcat_s(str_des, sizeof(str_des), str_source);
```
上面刚好5个字符，5个字符内，没问题,’\0’要占据一个字符位置。
```
char str_des[5]="a";
char *str_source = "bcde";
strcat_s(str_des, sizeof(str_des), str_source);
```
现在超过5个字符了，抛出异常。
注意，str_des一定要初始化，如果当前没值，可以用如下语句：
```
char str_des[5];
memset(str_des, 0, sizeof(str_des));
```
## 八、strcspn函数
### 1、函数原型：
```cpp
size_t strcspn ( const char * str1, const char * str2 );
```
返回对于str2中任意一个字符最早出现在str1中的位置，也就是str1前n个全部是str2中字符。
### 2、应用举例：
```cpp
/* strcspn example */
#include <stdio.h>
#include <string.h>
int main ()
{
  char str[] = "fcba73";
  char keys[] = "1234567890";
  int i;
  i = strcspn (str,keys);
  printf ("The first number in str is at position %d.\n",i+1); //结果是5
  return 0;
}
```
### 3、实现
#### 实现一
```
size_t mstrcspn(const char *s1, const char* s2)
{
    assert(NULL!=s1&&NULL!=s2);
    int i, j;
    for (i = 0; s1[i]; i++)
    {
        for (j = 0; s2[j]; j++)
        {
            if (s1[i] == s2[j]) return i;
        }
    }
    return i;   
}
```
#### 实现二
```
size_t  mstrcspn(const char * string, const char * control)
{
    const  char *str = string;
    const  char *ctrl = control;
    unsigned char map[32];
    int count;
    /* Clear out bit map */
    for (count = 0; count<32; count++)
        map[count] = 0;
    /* Set bits in control map */
    while (*ctrl)   
    {
        map[*ctrl >> 3] |= (1 << (*ctrl & 7));
        ctrl++;
    }
    /* 1st char in control map stops search */
    count = 0;
    map[0] |= 1;    /* null chars not considered */
    while (!(map[*str >> 3] & (1 << (*str & 7))))
    {
        count++;
        str++;
    }
    return(count);
}
```
一个ASCII码有8位,把它分为2部分， 高5 位i和低3位j两部分. 
- 高5位表示的范围有32个.所以申请了 32 大小的数组map。高5位通过*str>>3得到； 
- 然后低3位表示 0~7 这8个数值.正好map的每一项是 char (8位)的,所以 后3位就存放到相应的位上。低三位通过*str&7得到。
这就相当于一个二维数组map[i][j]
## 九、strspn函数
### 1、函数原型：
```cpp
size_t strspn ( const char * str1, const char * str2 );
```
strspn与strcspn恰恰相反，返回str1中最早的一个不是str2中字符的位置，也就是str1前n个都是str2中的字符。
### 2、应用举例
```cpp
/* strspn example */
#include <stdio.h>
#include <string.h>
int main ()
{
  int i;
  char strtext[] = "129th";
  char cset[] = "1234567890";
  i = strspn (strtext,cset);
  printf ("The initial number has %d digits.\n",i); //结果是3
  return 0;
}
```
### 3、实现
#### 实现一
```
size_t mstrspn(const char *s1, const char* s2)
{
    assert(NULL!=s1&&NULL!=s2);
    int i, j;
    for (i = 0; s1[i]; i++)
    {
        for (j = 0; s2[j]; j++)
        {
            if (s1[i] == s2[j]) break;
        }
        if (s1[i] != s2[j]) return i;
    }
    return i;   
}
```
#### 实现二
```
size_t  mstrspn(const char * string, const char * control)
{
    const  char *str = string;
    const  char *ctrl = control;
    unsigned char map[32];
    int count;
    /* Clear out bit map */
    for (count = 0; count<32; count++)
        map[count] = 0;
    /* Set bits in control map */
    while (*ctrl)   
    {
        map[*ctrl >> 3] |= (1 << (*ctrl & 7));
        ctrl++;
    }
    /* 1st char NOT in control map stops search */
    if (*str)
    {
        count = 0;
        while (map[*str >> 3] & (1 << (*str & 7)))
        {
            count++;
            str++;
        }
        return(count);
    }
    return(0);
}
```
### 3、两者合成版本实现
通过条件编译命令实现，这是完整源码，果然不一样
```
#define _STRSPN         1  
#define _STRCSPN        2  
#define _STRPBRK        3  
#if defined (SSTRCSPN)  
#define ROUTINE _STRCSPN  
#elif defined (SSTRPBRK)  
#define ROUTINE _STRPBRK  
#else    
#define ROUTINE _STRSPN    //默认  
#endif    
/* Routine prototype */
#if ROUTINE == _STRSPN  
size_t __cdecl strspn(
#elif ROUTINE == _STRCSPN  
size_t __cdecl strcspn(
#else  /* ROUTINE == _STRCSPN */  
char * __cdecl strpbrk(
#endif  /* ROUTINE == _STRCSPN */  
    const char * string,
    const char * control
    )
{
    const  char *str = string;
    const  char *ctrl = control;
    unsigned char map[32];
    int count;
    /* Clear out bit map */
    for (count = 0; count<32; count++)
        map[count] = 0;
    /* Set bits in control map */
    while (*ctrl)         //跟strtok()函数中的使用方式是一样的  
    {
        map[*ctrl >> 3] |= (1 << (*ctrl & 7));
        ctrl++;
    }
#if ROUTINE == _STRSPN  //  strspn()  
    /* 1st char NOT in control map stops search */
    if (*str)
    {
        count = 0;
        while (map[*str >> 3] & (1 << (*str & 7)))
        {
            count++;
            str++;
        }
        return(count);
    }
    return(0);
#elif ROUTINE == _STRCSPN   //strcspn()  
    /* 1st char in control map stops search */
    count = 0;
    map[0] |= 1;    /* null chars not considered */
    while (!(map[*str >> 3] & (1 << (*str & 7))))
    {
        count++;
        str++;
    }
    return(count);
#else  /* ROUTINE == _STRCSPN */  //strpbrk()  
    /* 1st char in control map stops search */
    while (*str)
    {
        if (map[*str >> 3] & (1 << (*str & 7)))
            return((char *)str);
        str++;
    }
    return(NULL);
#endif  /* ROUTINE == _STRCSPN */  
}
```
### 4、推荐资料
[1][位操作总结](http://blog.csdn.net/morewindows/article/details/7354571)
[2][strtok源码剖析 位操作与空间压缩](http://blog.csdn.net/morewindows/article/details/8740315/)
## 十、总结
知其然，还得知其所以然。平时只是用，没有探究过如何实现，现在总结一下，收获颇丰。多学习，多思考，多总结！
