# C++常用的几种内存数据处理算法总结 - fanyun的博客 - CSDN博客
2018年11月28日 21:27:27[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：47
1.strcpy()的算法实现
(1).算法作用及内部实现
        strcpy()会将参数src 字符串拷贝至参数dest 所指的地址。即strcpy只用于字符串复制，并且它不仅复制字符串内容之外，还会复制字符串的结束符'\0'。
```cpp
char *strcpy(char*strDest, const char *strSrc)
{
    assert((strDest!=NULL) && (strSrc!=NULL));
    char *address = strDest;    
    while( (*strDest++ = * strSrc++) != '\0')
       NULL ;
    return address ;      
 
}
```
(2).算法使用实例
```cpp
#include <string.h>
main(){
    char a[30] = "string(1)";
    char b[] = "string(2)";
    printf("before strcpy() :%s\n", a);
    printf("after strcpy() :%s\n", strcpy(a, b));
}
```
(3).算法输出
```cpp
before strcpy() :string(1)
after strcpy() :string(2)
```
2.memcpy()的算法实现
(1).算法作用及内部实现
        memcpy提供了一般内存的复制。即memcpy对于需要复制的内容没有限制，可以复制字符串，字符数组，结构体，类等，因此用途更广。strcpy不需要指定长度，它遇到被复制字符的串结束符"\0"才结束，所以容易溢出。memcpy则是根据其第3个参数决定复制的长度。
```cpp
void *memcpy(void*pvTo, void* pvFrom, size_t size) 
{
         void * ret = dst;
 
         while (count--) {
                   *(char *)dst = *(char *)src;
                   dst = (char *)dst + 1;
                   src = (char *)src + 1;
         }
         return(ret);
}
```
(2).算法使用实例
```cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 10
int main(void)
{
    char* target=(char*)malloc(sizeof(char)*N);
    memcpy(target,"0123456789",sizeof(char)*N);
    puts(target);
    free(target);
    return 0;
}
```
(3).算法输出
```cpp
0123456789
```
3.memset()的内部算法实现
(1).算法作用及内部实现
       memset()的函数， 它可以一字节一字节地把整个数组设置为一个指定的值。 memset()函数在mem.h头文件中声明，它把数组的起始地址作为其第一个参数，第二个参数是设置数组每个字节的值，第三个参数是数组的长度(字节数，不是元素个数)。
```cpp
void* memset(void* buffer, int c, int count)
{
       char* pvTo=(char*)buffer;
       assert(buffer != NULL);
       while(count-->0)
              *pvTo++=(char)c;
       return buffer;
}
```
(2).算法使用实例
```cpp
#include<mem.h>
　void main()
　{
　int ia1[50]；
　int ia2[500]；
　memset(iai,0,50*sizeof(int))；
　memset(ia2,0,500*sizeof(int))；
　//
　}
```
4.strncpy的算法实现
(1).算法作用及内部实现
     它的功能是将字符串src中最多n个字符复制到字符数组dest中(它并不像strcpy一样只有遇到NULL才停止复制，而是多了一个条件停止，就是说如果复制到第n个字符还未遇到NULL，也一样停止），返回指向dest的指针。
```cpp
char *strncpy(char *dest, const char *src, int len)
{
    assert(dest!=NULL && src!=NULL);
    char *temp;
    temp = dest;
    for(int i =0;*src!='\0' && i<len; i++,temp++,src++)
         *temp = *src;
   *temp = '\0';
    return dest;
}
```
(2).算法使用实例
```cpp
#include <iostream>
#include <string>
#include <assert.h>
//#pragma pack(2)
using namespace std;
int main()
{
 char *strncpy(char *,const char *,int);
 const char *str1 = "I love China";
 char str[13];
 char *str2;
 str2 = str;
 str2 = strncpy(str2,str1,12);
 cout<<"str1="<<str1<<endl;
 cout<<"str2="<<str2<<endl;
}
```
5.memmove的算法实现
(1).算法作用及内部实现
       memmove用于从src拷贝count个字节到dest，如果目标区域和源区域有重叠的话，memmove能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中。但复制后src内容会被更改。但是当目标区域与源区域没有重叠则和memcpy函数功能相同。
```
void *memmove(void *dst, const void*src, size_tcount)
{
       void* ret = dst;
       if(dst <= src || (char *)dst >= ((char *)src + count)) {//不重叠或，拷贝不覆盖还未读的src
              while (count--) {
                     *(char *)dst = *(char *)src;
                     dst = (char *)dst + 1;
                     src = (char *)src + 1;
              }
       }
       else{//逆向拷贝
              dst= (char *)dst + count - 1;
              src= (char *)src + count - 1;
              while(count--) {
                     *(char*)dst = *(char *)src;
                     dst= (char *)dst - 1;
                     src= (char *)src - 1;
              }
       }
       return(ret);
 
}
```
(2).算法使用实例
```cpp
#include <stdio.h>
#include <string.h>
int main(void)
{
char s[]="Golden Global View";
memmove(s,s+7,strlen(s)+1-7);
printf("%s",s);
getchar();
return 0;
}
```
(3).算法输出
```cpp
Global View
```
