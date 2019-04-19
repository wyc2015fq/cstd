# Linux字符串操作函数 - xqhrs232的专栏 - CSDN博客
2016年05月11日 15:25:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：361
原文地址::[http://blog.csdn.net/amaowolf/article/details/8373807](http://blog.csdn.net/amaowolf/article/details/8373807)
相关文章
1、Linux字符串转换函数汇总----[http://blog.sina.com.cn/s/blog_5f84dc840100orcm.html](http://blog.sina.com.cn/s/blog_5f84dc840100orcm.html)
2、[strstr(str1,str2)函数使用时注意事项](http://blog.csdn.net/ludaoyi88/article/details/52819448)----[http://blog.csdn.net/ludaoyi88/article/details/52819448](http://blog.csdn.net/ludaoyi88/article/details/52819448)
## [zz]Linux字符串操作函数
发表于1年前
(1)strcat（连接两字符串）
相关函数
bcopy，memccpy，memcpy，strcpy，strncpy
头文件
#include <string.h>
函数定义
char *strcat (char *dest,const char *src);
函数说明
strcat()会将参数src字符串拷贝到参数dest所指的字符串尾。第一个参数dest要有足够的空间来容纳要拷贝的字符串。
返回值
返回参数dest的字符串起始地址
范例
#include <string.h>
#include <stdio.h>
main()
{
char a[30]="string(1)";
char b[]="string(2)";
printf("before strcat() : %s\n",a);
printf("after strcat() : %s\n",strcat(a,b));
}
执行
before strcat () : string(1)
after strcat () : string(1)string(2)
(2)strchr（查找字符串中第一个出现的指定字符）
相关函数
index，memchr，rinex，strbrk，strsep，strspn，strstr，strtok
头文件
#include <string.h>
函数定义
char * strchr (const char *s,int c);
函数说明
strchr()用来找出参数s字符串中第一个出现的参数c地址，然后将该字符出现的地址返回。
返回值
如果找到指定的字符则返回该字符所在地址，否则返回0。
范例
#include <string.h>
#include <stdio.h>
main()
{
char *s="0123456789012345678901234567890";
char *p;
p=strchr(s,'5');
if (p)
{
printf("%s\n",p);
while ((p) && ( *(p+1) != '\0' ))
{
p=strchr(p+1,'5');
if (p)
{
printf("%s\n",p);
}
}
执行
56789012345678901234567890
5678901234567890
567890
(3)strcmp（比较字符串）
相关函数
bcmp，memcmp，strcasecmp，strncasecmp，strcoll
头文件
#include <string.h>
函数定义
int strcmp(const char *s1,const char *s2);
函数说明
strcmp()用来比较参数s1和s2字符串。字符串大小的比较是以ASCII 码表上的顺序来决定，此顺序亦为字符的值。strcmp()首先将s1第一个字符值减去s2第一个字符值，若差值为0则再继续比较下个字符，若差值不为0则将差值返回。例如字符串"Ac"和"ba"比较则会返回字符"A"(65)和'b'(98)的差值(－33)。
返回值
若参数s1和s2字符串相同则返回0。s1若大于s2则返回大于0的值。s1若小于s2则返回小于0 的值。
范例
#include <string.h>
#include <stdio.h>
main()
{
char *a="aBcDeF";
char *b="AbCdEf";
char *c="aacdef";
char *d="aBcDeF";
printf("strcmp(a,b) : %d\n",strcmp(a,b));
printf("a-b = %d\n",(*a)-(*b));
printf("strcmp(a,c) : %d\n",strcmp(a,c));
printf("strcmp(a,d) : %d\n",strcmp(a,d));
}
执行
strcmp(a,b) : 1
a-b = 32
strcmp(a,c) : -1
strcmp(a,d) : 0
(4)strcoll（采用目前区域的字符排列次序来比较字符串）
相关函数
strcmp，bcmp，memcmp，strcasecmp，strncasecmp
头文件
#include <string.h>
定义函数
int strcoll( const char *s1, const char *s2);
函数说明
strcoll()会依环境变量LC_COLLATE所指定的文字排列次序来比较s1和s2 字符串。
返回值
若参数s1和s2字符串相同则返回0。s1若大于s2则返回大于0的值。s1若小于s2则返回小于0 的值。
附加说明
若LC_COLLATE为"POSIX"或"C"，则strcoll()与strcmp()作用完全相同。
范例
参考strcmp()。
(5)strcpy（拷贝字符串）
相关函数
bcopy，memcpy，memccpy，memmove
头文件
#include <string.h>
定义函数
char *strcpy(char *dest,const char *src);
函数说明
strcpy()会将参数src字符串拷贝至参数dest所指的地址。
返回值
返回参数dest的字符串起始地址。
附加说明
如果参数dest所指的内存空间不够大，可能会造成缓冲溢出(buffer Overflow)的错误情况，在编写程序时请特别留意，或者用strncpy()来取代。
范例
#include <string.h>
#include <stdio.h>
main()
{
char a[30]="string(1)";
char b[]="string(2)";
printf("before strcpy() :%s\n",a);
printf("after strcpy() :%s\n",strcpy(a,b));
}
执行
before strcpy() :string(1)
after strcpy() :string(2)
(6)strcspn（返回字符串中连续不含指定字符串内容的字符数）
相关函数
strspn
头文件
#include <string.h>
定义函数
size_t strcspn ( const char *s,const char * reject);
函数说明
strcspn()从参数s字符串的开头计算连续的字符，而这些字符都完全不在参数reject 所指的字符串中。简单地说，若strcspn()返回的数值为n，则代表字符串s开头连续有n个字符都不含字符串reject内的字符。
返回值
返回字符串s开头连续不含字符串reject内的字符数目。
范例
#include <string.h>
#include <stdio.h>
main()
{
char *str="Linux was first developed for 386/486-based pcs.";
printf("%d\n",strcspn(str," "));
printf("%d\n",strcspn(str,"/-"));
printf("%d\n",strcspn(str,"1234567890"));
}
执行
5
33
30
(7)strdup（复制字符串）
相关函数
calloc，malloc，realloc，free
表头文件
#include <string.h>
定义函数
char * strdup( const char *s);
函数说明
strdup()会先用maolloc()配置与参数s字符串相同的空间大小，然后将参数s字符串的内容复制到该内存地址，然后把该地址返回。该地址最后可以利用free()来释放。
返回值
返回一字符串指针，该指针指向复制后的新字符串地址。若返回NULL表示内存不足。
范例
#include <string.h>
#include <stdio.h>
main()
{
char a[]="strdup";
char *b;
b=strdup(a);
printf("b[ ]="%s"\n",b);
}
执行
b[ ]="strdup"
(8)strlen（返回字符串长度）
相关函数
头文件
#include <string.h>
定义函数
size_t strlen (const char *s);
函数说明
strlen()用来计算指定的字符串s的长度，不包括结束字符"\0"。
返回值
返回字符串s的字符数。
范例
#include <string.h>
#include <stdio.h>
main()
{
char *str = "12345678";
printf("str length = %d\n", strlen(str));
}
执行
str length = 8
(9)strncasecmp（忽略大小写比较字符串）
相关函数
bcmp，memcmp，strcmp，strcoll，strncmp
头文件
#include <string.h>
定义函数
int strncasecmp(const char *s1,const char *s2,size_t n);
函数说明
strncasecmp()用来比较参数s1和s2字符串前n个字符，比较时会自动忽略大小写的差异。
返回值
若参数s1和s2 字符串相同则返回0。s1 若大于s2则返回大于0的值，s1若小于s2则返回小于0 的值。
范例
#include <string.h>
#include <stdio.h>
main()
{
char *a="aBcDeF";
char *b="AbCdEf";
if(!strncasecmp(a,b,strlen(a)))
{
printf("%s =%s\n",a,b);
}
}
执行
aBcDeF =AbCdEf
(10)strncat（连接两字符串）
相关函数
bcopy，memccpy，memecpy，strcpy，strncpy
表头文件
#include <string.h>
定义函数
char * strncat(char *dest,const char *src,size_t n);
函数说明
strncat()会将参数src字符串拷贝n个字符到参数dest所指的字符串尾。第一个参数dest要有足够的空间来容纳要拷贝的字符串。
返回值
返回参数dest的字符串起始地址。
范例
#include <string.h>
#include <stdio.h>
main()
{
char a[30]="string(1)";
char b[]="string(2)";
printf("before strnact() :%s\n", a);
printf("after strncat() :%s\n", strncat(a,b,6));
}
执行
before strnact() :string(1)
after strncat() :string(1)string
(11)strncpy（拷贝字符串）
相关函数
bcopy，memccpy，memcpy，memmove
头文件
#include <string.h>
定义函数
char * strncpy(char *dest,const char *src,size_t n);
函数说明
strncpy()会将参数src字符串拷贝前n个字符至参数dest所指的地址。
返回值
返回参数dest的字符串起始地址。
范例
#include <string.h>
#include <stdio.h>
main()
{
char a[30]="string(1)";
char b[]="string(2)";
printf("before strncpy() : %s\n",a);
printf("after strncpy() : %s\n",strncpy(a,b,strlen(b)));
}
执行
before strncpy() : string(1)
after strncpy() : string(2)
(12)strpbrk（查找字符串中第一个出现的指定字符）
相关函数
index，memchr，rindex，strpbrk，strsep，strspn，strstr，strtok
头文件
#include <string.h>
定义函数
char *strpbrk(const char *s,const char *accept);
函数说明
strpbrk()用来找出参数s 字符串中最先出现存在参数accept 字符串中的任意字符。
返回值
如果找到指定的字符则返回该字符所在地址，否则返回0。
范例
#include <string.h>
#include <stdio.h>
main()
{
char *s="0123456789012345678901234567890";
char *p;
p=strpbrk(s,"a1 839");
printf("%s\n",p);
p=strpbrk(s,"4398");
printf("%s\n",p);
}
执行
123456789012345678901234567890
3456789012345678901234567890
(13)strrchr（查找字符串中最后出现的指定字符）
相关函数
index，memchr，rindex，strpbrk，strsep，strspn，strstr，strtok
头文件
#include <string.h>
定义函数
char * strrchr(const char *s, int c);
函数说明
strrchr()用来找出参数s字符串中最后一个出现的参数c地址，然后将该字符出现的地址返回。
返回值
如果找到指定的字符则返回该字符所在地址，否则返回0。
范例
#include <string.h>
#include <stdio.h>
main()
{
char *s="S0123456789012345678901234567890E";
char *p;
p=strrchr(s,'5');
printf("%s\n",p);
}
执行
567890E
(14)strspn（返回字符串中连续不含指定字符串内容的字符数）
相关函数
strcspn，strchr，strpbrk，strsep，strstr
表头文件
#include <string.h>
定义函数
size_t strspn (const char *s,const char * accept);
函数说明
strspn()从参数s 字符串的开头计算连续的字符，而这些字符都完全是accept 所指字符串中的字符。简单的说，若strspn()返回的数值为n，则代表字符串s 开头连续有n 个字符都是属于字符串accept内的字符。
返回值
返回字符串s开头连续包含字符串accept内的字符数目。
范例
#include <string.h>
#include <stdio.h>
main()
{
char *str="Linux was first developed for 386/486-based PCs.";
char *t1="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
printf("%d\n",strspn(str,t1));
}
执行
5 　
(15)strstr（在一字符串中查找指定的字符串）
相关函数
index，memchr，rindex，strchr，strpbrk，strsep，strspn，strtok
头文件
#include <string.h>
定义函数
char *strstr(const char *haystack,const char *needle);
函数说明
strstr()会从字符串haystack 中搜寻字符串needle，并将第一次出现的地址返回。
返回值
返回指定字符串第一次出现的地址，否则返回0。
范例
#include <string.h>
#include <stdio.h>
main()
{
char * s="012345678901234567890123456789";
char *p;
p= strstr(s,"901");
printf("%s\n",p);
}
执行
901234567890123456789
(16)strtok（分割字符串）
相关函数
index，memchr，rindex，strpbrk，strsep，strspn，strstr
头文件
#include <string.h>
定义函数
char * strtok(char *s,const char *delim);
函数说明
strtok()用来将字符串分割成一个个片段。参数s指向欲分割的字符串，参数delim则为分割字符串，当strtok()在参数s的字符串中发现到参数delim的分割字符时则会将该字符改为\0 字符。在第一次调用时，strtok()必需给予参数s字符串，往后的调用则将参数s设置成NULL。每次调用成功则返回下一个分割后的字符串指针。
返回值
返回下一个分割后的字符串指针，如果已无从分割则返回NULL。
范例
#include <string.h>
#include <stdio.h>
main()
{
char s[]="ab-cd : ef;gh :i-jkl;mnop;qrs-tu: vwx-y;z";
char *delim="-: ;";
char *p;
printf("%s ",strtok(s,delim));
while((p=strtok(NULL,delim)))
{
printf("%s ",p);
}
printf("\n");
}
执行
ab cd ef gh i jkl mnop qrs tu vw
