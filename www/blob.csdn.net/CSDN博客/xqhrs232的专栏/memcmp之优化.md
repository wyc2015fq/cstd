# memcmp之优化 - xqhrs232的专栏 - CSDN博客
2013年04月01日 16:24:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1284
原文地址::[http://blog.chinaunix.net/uid-25627207-id-3556923.html](http://blog.chinaunix.net/uid-25627207-id-3556923.html)
前段时间在公司移植一个工具的时候，历经千辛万难,终于把工具移植过去了。因为工具涉及到写Nand Flash，所以需要将写入的数据读出来，确认是否数据被正确的写入到flash上。结果，发现两者的效率确实有相当大的差异。于是，就着手开始找问题出在哪里。由于是和nand flash相关，最开始是以为移植后的nand驱动的效率不高导致。但是最后，定位到是memcmp。
1. 编译的工具使用的是memcmp，但是memcmp通过__attribute__("alisa")定义为另外一个自己写的函数的别名.
int my_memcmp(void *s1, void *s2, int len)__attribute__((alias("my_memcmp"));
但是，my_memcmp的效率太低了。导致工具移植后的效率也降下来。
my_memcmp的实现大致如下：
- int my_memcmp_1(void *s1, void *s2,intlen)
- {
- int count;
- const char *str1,*str2;
- 
- 
    str1 =(const char *)s1;
- 
    str2 =(const char *)s2;
- 
- for(count = 0; count <len; count++){
- if(*(str1 + count)!=*(str2 + count)){
- 
            return *(str1 + count )>*(str2 + count)? 1 :-1;
- }
- }
- 
    return 0;
- }
所以，需要着手优化my_memcmp。
2. 因为旧工具中，memcmp使用的是gcc的库函数，所以，就直接下来了[gcc-4.5.3](http://ftp.gnu.org/gnu/gcc/gcc-4.5.3/)的源码，找到memcmp函数，然后将代码直接拿过来用。
gcc中间的memcmp实现很简洁，在gcc-4.5.3/libiberty/memcmp.c中，实现如下：
- int
- 
memcmp (const PTR str1,const PTR
 str2, size_t count)
- {
- 
    register const unsigned char *s1 =(const unsigned
 char*)str1;
- 
    register const unsigned char *s2 =(const unsigned
 char*)str2;
- 
- while(count--> 0){
- if(*s1++!=*s2++)
- 
     return s1[-1]< s2[-1]?-1 : 1;
- }
- 
    return 0;
- }
但是，发现效率也并没有太大的改善。
3. 将电脑中gcc库，通过obj-dump工具，将gcc的库反汇编出来，看看工具链中gcc的memcmp是怎么实现的。
结果发现，gcc库中实现的memcmp一次跳4个字节。突然就茅塞顿开。根据汇编代码实现我自己的memcmp。效率工具链中的memcmp效率基本一致。
- int my_memcmp(void *s1, void *s2,intlen)
- {
- constint*p1,*q1;
- const char *p2,*q2;
- int off,mod;
- 
- 
    off =len>> 2;
- mod=len-(off << 2);
- 
- if(mod> 0){
- 
        p2 =(const char *)s1;
- 
        q2 =(const char *)s2;
- while(mod--){
- if(*p2 ++!=*q2 ++){
- 
                return p2[-1]> q2[-1]? 1 :-1;
- }
- }
- }
- 
- /*if p1 & q1
 address don't align with 4 bytes,
- 
       how about the efficiency ?*/
- mod=len-(off << 2);
- 
    p1 =(constint*)(s1 +mod);
- 
    q1 =(constint*)(s2 +mod);
- 
- while(off --){
- if(*p1 ++!=*q1 ++){
- 
            return p1[-1]> q1[-1]? 1 :-1;
- }
- }
- 
- 
    return 0;
- }
4.经过测试，基本OK.也算大功告成。
不过还有一个疑惑就是，在将char类型指针转换为int类型指针的时候，是否会存在因为地址没有对齐，而降低效率呢？
