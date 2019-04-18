# memmove 和 memcpy 的区别 - 文章 - 伯乐在线
原文出处： [But_Bueatiful](http://blog.chinaunix.net/uid-26495963-id-3080058.html)
memcpy和memmove（）都是C语言中的库函数，在头文件string.h中，作用是拷贝一定长度的内存的内容，原型分别如下：
void *memcpy(void *dst, const void *src, size_t count);
void *memmove(void *dst, const void *src, size_t count);
他们的作用是一样的，唯一的区别是，当内存发生局部重叠的时候，memmove保证拷贝的结果是正确的，memcpy不保证拷贝的结果的正确。
![26495963_1329914944xr3g](http://jbcdn2.b0.upaiyun.com/2016/12/40699771476b78fef5150bbb87afa026.jpg)
第一种情况下，拷贝重叠的区域不会出现问题，内容均可以正确的被拷贝。
第二种情况下，问题出现在右边的两个字节，这两个字节的原来的内容首先就被覆盖了，而且没有保存。所以接下来拷贝的时候，拷贝的是已经被覆盖的内容，显然这是有问题的。
实际上，memcpy只是memmove的一个子集。
二者的c语言实现很简单，有兴趣的朋友可以去看看。在实际情况下，这两个函数都是用汇编实现的。
memmove在copy两个有重叠区域的内存时可以保证copy的正确，而memcopy就不行了，但memcopy比memmove的速度要快一些，如：


```
char s[] = "1234567890";
char* p1 = s;
char* p2 = s+2;
```
memcpy(p2, p1, 5)与memmove(p2, p1, 5)的结果就可能是不同的，memmove()可以将p1的头5个字符”12345″正确拷贝至p2，而memcpy()的结果就不一定正确了
**memcpy()、 memmove()和memccpy()**
——————————————————-
这三个函数的功能均是将某个内存块复制到另一个内存块。前两个函数的区别在于它们处理内存区域重叠(overlapping)的方式不同。第三个函数的功能也是复制内存，但是如果遇到某个特定值时立即停止复制。
对于库函数来说，由于没有办法知道传递给他的内存区域的情况，所以应该使用memmove()函数。通过这个函数，可以保证不会出现任何内存块重叠问题。而对于应用程序来说，因为代码“知道”两个内存块不会重叠，所以可以安全地使用memcpy()函数。
原型：extern void *memccpy(void *dest, void *src, unsigned char ch, unsigned int count);
用法：#include
功能：由src所指内存区域复制不多于count个字节到dest所指内存区域，如果遇到字符ch则停止复制。
说明：返回指向字符ch后的第一个字符的指针，如果src前n个字节中不存在ch则返回NULL。ch被复制。


```
char s[]="Goldenx Global View";
char d[20];
char *p;
p=(char *)memccpy(d,s,'x',strlen(s));
if(p)
{
   *p='\0'; // MUST Do This
   printf("Char found: %s.\n",d);
}
else
   printf("Char not found.\n");
```
关于memmove的实现：


```
void *mymemmove(void *dest, const void *src, size_t n)
{
    char temp[n];
    int i;
    char *d = dest;
    const char *s = src;
    for (i = 0; i < n; i++) 
        temp[i] = s[i];
    for (i = 0; i < n; i++) 
        d[i] = temp[i];
    return dest;
}
```
关于memcpy的实现：

```
void *mymemcpy(void *dest, const void *src, size_t n)
{
    char *d = dest;
    const char *s = src;
    int *di;
    const int *si;
    int r = n % 4;
    
    while (r--)
        *d++ = *s++;
    di = (int *)d;
    si = (const int*)s;
    n /= 4;
    while (n--)
        *di++ = *si++;
    return dest;
}
```
