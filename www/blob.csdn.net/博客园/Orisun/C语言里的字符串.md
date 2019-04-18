# C语言里的字符串 - Orisun - 博客园







# [C语言里的字符串](https://www.cnblogs.com/zhangchaoyang/articles/1958228.html)





C语言里的字符串一定纠结了大家太多的感情。

1.char *

char *buff="orisun";

sizeof(buff)=4　　用32位存放一个指针

strlen(buff)=6　　strlen返回字符串的实际长度，不包括末尾的'\0'

2.char [n]

char bu[100]="orisun";　　从第7位到第100位都是'\0'

sizeof(bu)=100　　　bu是一个数组，sizeof返回数组的长度

strlen(bu)=6　　　　strlen返回字符串的实际长度，不包括末尾的'\0'

但注意在printf("%s\n",bu);之前必须先bu[100]='\0';

3.char[]

char bu[]="orisun";　　首先bu是一个数组，它的长度由 "orisun"的长度决定， sizeof(bu)=7

我们知道数组在第1次出现时必须指明长度，因为在编译时编译器就要为它分配空间，所以下面的写法是不能通过编译的：

char bu[];

bu="orisun";


4.不能把const char*赋给char *

const char*的含义并非是指针不可被修改，而是指针指向的内容不可被修改，因此，当把一个const  char*赋给一个char*，结果就导致原来加那个const的目的形同虚设，一点意义也没有了。

5.可以把const char赋给char

基本类型的“赋值”，也就是拷贝一个值而已，拷贝之后，不管你对后面的那个char怎么修改，都不会影响原先那个const  char

6.可以strcpy(char*,const char*)

表头文件	#include<string.h>


定义函数	char
*strcpy(char *dest,const char *src);


函数说明	strcpy()会将参数src字符串拷贝至参数dest所指的地址。

返回值返回参数dest的字符串起始地址。

附加说明如果参数dest所指的内存空间不够大，可能会造成缓冲溢出(buffer
Overflow)的错误情况，在编写程序时请特别留意，或者用strncpy()来取代。

7.strncpy

定义函数	　　char
* strncpy(char *dest,const char *src,size_t n);


函数说明　　	strncpy()会将参数src字符串拷贝前n个字符至参数dest所指的地址。

8.strdup复制字符串

定义函数	　　	char
* strdup( const char *s);


函数说明	　　	strdup()会先用maolloc()配置与参数s字符串相同的空间大小，然后将参数s字符串的内容复制到该内存地址，然后把该地址返回。该地址最后可以利用free()来释放。

所以strdup和strcpy的区别在于：使用strdup目标指针不需要（也不能）事先分配空间，而使用strcpy则事先把空间分配好。

返回值返回一字符串指针，该指针指向复制后的新字符串地址。若返回NULL表示内存不足。

9.strcat连接字符串（同理有strncat）

char * strcat( char *dest , const char * src)

返回dest的首地址，dest必须有足够的空间来容纳拷贝的字符串

10.strncasecmp（忽略大小写比较字符串）

int strncasecmp(const char *s1,const char *s2,size_t n);

11.strtok　　分割字符串

char *strtok(char *s, char *delim);
分解字符串为一组字符串。s为要分解的字符串，delim为分隔符字符串。实质上的处理是，strtok在s中查找包含在delim中的字符并用NULL(’\0′)来替换,直到找遍整个字符串。

char buff[]="abc$sw$21";

char *delim="$";

char *title=strtok(buff,delim);　　　　//title="abc"

char *address=strtok(NULL,delim);　　//address="sw"

char *area=strtok(NULL,delim);　　//area="21"

注意：不能使用char ”buff="abc$sw$21";因为strtok函数在运行时会改变第一个参数的值，如果把buff定义为一个常量指针，其值无法被修改，在使用strtok时会发生段错误。

12.

sprintf（格式化字符串复制）

char sql[200];

memset(sql,0,200);

sprintf(sql,"INSERT INTO T1(title,address,area)VALUES('%s','%s','%s')",title,address,area);

#include<stdio.h>
#include<string.h>
int main(){
	char *buff="10 0x1b abc3.14";
	int a,b;
	float c;
	char s[5];
	sscanf(buff,"%d %x %3s%f",&a,&b,s,&c);
	printf("%d  %d  %f  %s\n",a,b,c,s);
	return 0;
}


 输出：10  27  3.140000  abc

13.strstr

char *strstr(const char *haystack, const char *needle);

从haystack中找到子串needle第一次出现的位置

14.strchr

char *strchr(const char *s, int c);

查找字符串s中字符c第一次出现的位置

strrchr逆向查找

strchr和index的功能及用法完全一样。

strrchr和rindex的功能及用法完全一样。

15.strpbrk

char *strpbrk(const char *s, const char *accept);

在s中查找accept中任一个字符首次出现的位置

16.memchr

void *memchr(const void *s, int c, size_t n);

在s的前n个字节中查找c，如果找到就返回指向c的指针，否则返回NULL

17.strspn

size_t strspn(const char *s, const char *accept);

返回字符串s中第一个不在指定字符串accept中出现的字符下标

size_t strcspn(const char *s, const char *reject);

返回字符串s中第一个在指定字符串reject中出现的字符下标

18.strsep

char *strsep(char **stringp, const char *delim);

分解字符串为一组字符串。从stringp指向的位置起向后扫描，遇到delim指向位置的字符后，将此字符替换为NULL，返回stringp指向的地址。


#include<stdio.h>
#include<string.h>

main(){
    char str[]="root:x::0:root:/root:/bin/bash:";
    char *token;
    char *buf;
    buf=str;
    while((token=strsep(&buf,":"))!=NULL){
        printf("%s\n",token);
    }
}



输出

root
x

0
root
/root
/bin/bash


 注意在第9行把buf替换为str是不行的，编译时警告：期待的参数类型是char **，但传进来的是char(*)[32]，运行时会发生段错误。str是一个char数组，buf是一个char指针，第8行buf=str是那允许的，因为数组名buf存放的就是数组的首地址，即数组名本身也是一个指针。把一个char(*)[10] 赋给一个char**也是可以的，因为都是指向指针的指针。但是你想，现有

char *arr[]={"asd","werr"};

char *str="tgb";

char **ptr=&str;

arr++就可以指向二维数组的下一行，而ptr++只能前进一个字节。













