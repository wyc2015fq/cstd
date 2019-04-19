# 位反转的最佳算法（C语言实现） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [kimylrong](http://www.jobbole.com/members/kimylrong) 翻译。未经许可，禁止转载！
英文出处：[Stackoverflow](http://stackoverflow.com/questions/746171/best-algorithm-for-bit-reversal-from-msb-lsb-to-lsb-msb-in-c)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
## [green_t](http://stackoverflow.com/users/17329/green-t) 提问：
实现如下转换的最佳算法是什么？
```
0010 0000 => 0000 0100
```
具体的转换是从MSB->LSB 到 LSB->MSB，所有的位都必须反转，那意味着，这并不是字节顺序的交换。
LSB(Least Significant Bit)，意为最低有效位；MSB(Most Significant Bit)，意为最高有效位。
## 最佳答案（来自[Matt J](http://stackoverflow.com/users/18528/matt-j)）
**注意**：下面的算法都用C实现，但应该可以迁移到其它语言(只是不那么快的时候可别找我)。
### **可选方案**
**内存占用少(32位int，32位机器)(来源于[这里](http://aggregate.org/MAGIC/#Bit%20Reversal))**

C
```
unsigned int
reverse(register unsigned int x)
{
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return((x >> 16) | (x << 16));
}
```
**最快(查找表)**

C
```
static const unsigned char BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};
unsigned int v; // reverse 32-bit value, 8 bits at time
unsigned int c; // c will get v reversed
// Option 1:
c = (BitReverseTable256[v & 0xff] << 24) | 
    (BitReverseTable256[(v >> 8) & 0xff] << 16) | 
    (BitReverseTable256[(v >> 16) & 0xff] << 8) |
    (BitReverseTable256[(v >> 24) & 0xff]);
// Option 2:
unsigned char * p = (unsigned char *) &v;
unsigned char * q = (unsigned char *) &c;
q[3] = BitReverseTable256[p[0]]; 
q[2] = BitReverseTable256[p[1]]; 
q[1] = BitReverseTable256[p[2]]; 
q[0] = BitReverseTable256[p[3]];
```
来自于著名的[Bit Twiddling Hacks page:](http://graphics.stanford.edu/~seander/bithacks.html)
你可以扩展这个算法到64位int的场景，或者为了更快的速度而牺牲多一些的内存(假设你的L1数据缓存足够大)，有一个64K的查找表且每次反转16位。
### **其它方案**
**简单**

C
```
unsigned int v;     // input bits to be reversed
unsigned int r = v; // r will be reversed bits of v; first get LSB of v
int s = sizeof(v) * CHAR_BIT - 1; // extra shift needed at end
for (v >>= 1; v; v >>= 1)
{   
  r <<= 1;
  r |= v & 1;
  s--;
}
r <<= s; // shift when v's highest bits are zero
```
**更快(32位处理器)**

C
```
unsigned char b = x;
b = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
```
**更快(64位处理器)**

C
```
unsigned char b; // reverse this (8-bit) byte
b = (b * 0x0202020202ULL & 0x010884422010ULL) % 1023;
```
如果你想在32位int环境这样做，那么只需要把每一个byte反转，然后再颠倒byte的次序即可。如下:

C
```
unsigned int toReverse;
unsigned int reversed;
unsigned char inByte0 = (toReverse & 0xFF);
unsigned char inByte1 = (toReverse & 0xFF00) >> 8;
unsigned char inByte2 = (toReverse & 0xFF0000) >> 16;
unsigned char inByte3 = (toReverse & 0xFF000000) >> 24;
reversed = (reverseBits(inByte0) << 24) | (reverseBits(inByte1) << 16) | (reverseBits(inByte2) << 8) | (reverseBits(inByte3);
```
### **结果**
我测试了两个最有效的方案，查找表和按位与(第一个方案)。测试机器为一台笔记本电脑，配置为4G DDR2内存，2.4GHz的
双核T7500处理器，4MB的L2缓存。我使用的是gcc 4.3.2，64位Linux。OpenMP(外加GCC绑定)被用来提高计时器的调度能力。
**reverse.c**

C
```
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
unsigned int
reverse(register unsigned int x)
{
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return((x >> 16) | (x << 16));
}
int main()
{
    unsigned int *ints = malloc(100000000*sizeof(unsigned int));
    unsigned int *ints2 = malloc(100000000*sizeof(unsigned int));
    for(unsigned int i = 0; i < 100000000; i++)
      ints[i] = rand();
    unsigned int *inptr = ints;
    unsigned int *outptr = ints2;
    unsigned int *endptr = ints + 100000000;
    // Starting the time measurement
    double start = omp_get_wtime();
    // Computations to be measured
    while(inptr != endptr)
    {
      (*outptr) = reverse(*inptr);
      inptr++;
      outptr++;
    }
    // Measuring the elapsed time
    double end = omp_get_wtime();
    // Time calculation (in seconds)
    printf("Time: %f seconds\n", end-start);
    free(ints);
    free(ints2);
    return 0;
}
```
**reverse_lookup.c**

C
```
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
static const unsigned char BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};
int main()
{
    unsigned int *ints = malloc(100000000*sizeof(unsigned int));
    unsigned int *ints2 = malloc(100000000*sizeof(unsigned int));
    for(unsigned int i = 0; i < 100000000; i++)
      ints[i] = rand();
    unsigned int *inptr = ints;
    unsigned int *outptr = ints2;
    unsigned int *endptr = ints + 100000000;
    // Starting the time measurement
    double start = omp_get_wtime();
    // Computations to be measured
    while(inptr != endptr)
    {
    unsigned int in = *inptr;  
    // Option 1:
    //*outptr = (BitReverseTable256[in & 0xff] << 24) | 
    //    (BitReverseTable256[(in >> 8) & 0xff] << 16) | 
    //    (BitReverseTable256[(in >> 16) & 0xff] << 8) |
    //    (BitReverseTable256[(in >> 24) & 0xff]);
    // Option 2:
    unsigned char * p = (unsigned char *) &(*inptr);
    unsigned char * q = (unsigned char *) &(*outptr);
    q[3] = BitReverseTable256[p[0]]; 
    q[2] = BitReverseTable256[p[1]]; 
    q[1] = BitReverseTable256[p[2]]; 
    q[0] = BitReverseTable256[p[3]];
      inptr++;
      outptr++;
    }
    // Measuring the elapsed time
    double end = omp_get_wtime();
    // Time calculation (in seconds)
    printf("Time: %f seconds\n", end-start);
    free(ints);
    free(ints2);
    return 0;
}
```
在不同的优化级别(Optimizations)，两个方案我都尝试了，每个级别跑3个案例，每个案例反转
1亿个随机的无符号整数。对于查找表方案，bitwise hacks page上面的两种方法(Option 1 and Option 2)我都测试过。
结果如下:
**按位与**

C
```
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -o reverse reverse.c
mrj10@mjlap:~/code$ ./reverse
Time: 2.000593 seconds
mrj10@mjlap:~/code$ ./reverse
Time: 1.938893 seconds
mrj10@mjlap:~/code$ ./reverse
Time: 1.936365 seconds
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -O2 -o reverse reverse.c
mrj10@mjlap:~/code$ ./reverse
Time: 0.942709 seconds
mrj10@mjlap:~/code$ ./reverse
Time: 0.991104 seconds
mrj10@mjlap:~/code$ ./reverse
Time: 0.947203 seconds
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -O3 -o reverse reverse.c
mrj10@mjlap:~/code$ ./reverse
Time: 0.922639 seconds
mrj10@mjlap:~/code$ ./reverse
Time: 0.892372 seconds
mrj10@mjlap:~/code$ ./reverse
Time: 0.891688 seconds
```
**查找表(Option 1)**

C
```
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -o reverse_lookup reverse_lookup.c
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.201127 seconds              
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.196129 seconds              
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.235972 seconds              
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -O2 -o reverse_lookup reverse_lookup.c
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 0.633042 seconds              
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 0.655880 seconds              
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 0.633390 seconds              
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -O3 -o reverse_lookup reverse_lookup.c
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 0.652322 seconds              
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 0.631739 seconds              
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 0.652431 seconds
```
**查找表(Option 2)**

C
```
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -o reverse_lookup reverse_lookup.c
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.671537 seconds
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.688173 seconds
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.664662 seconds
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -O2 -o reverse_lookup reverse_lookup.c
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.049851 seconds
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.048403 seconds
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.085086 seconds
mrj10@mjlap:~/code$ gcc -fopenmp -std=c99 -O3 -o reverse_lookup reverse_lookup.c
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.082223 seconds
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.053431 seconds
mrj10@mjlap:~/code$ ./reverse_lookup
Time: 1.081224 seconds
```
### **结论**
如果你比较在意性能，那么使用查找表Option 1(Byte的寻址不出意外的慢)。如果你需要尽可能的利用完每一个Byte内存
(且你也在意bit反转的性能)，那么优化后的按位与方案也还不赖。
### **附加说明**
我知道上面的代码只是一个粗略的版本，非常欢迎大家提供一些优化的建议。以下是我知道的几点:
- 我没有权限访问ICC，那可能更快些(如果你可以测试请在评论中回复)。
- 在一些L1缓存比较大的现代机器上面，64K的查找表可能工作得更好。
- `-mtune=native`对 -O2/-O3(发生符号重定义的错误)无效，所以我不相信产生的代码是为我的微架构而优化。
- SSE环境下应该有一种方法处理得更快。我不知道怎么做，但又更快的内存复制，批量的按位与，调整的指令集，
总是有一些手段的。
- 我知道仅仅x86的指令集是危险的，下面是GCC在-O3环境产生的代码，所以比我更厉害的大牛可以检查一下。
**32-bit**

Shell
```
.L3:
movl    (%r12,%rsi), %ecx
movzbl  %cl, %eax
movzbl  BitReverseTable256(%rax), %edx
movl    %ecx, %eax
shrl    $24, %eax
mov     %eax, %eax
movzbl  BitReverseTable256(%rax), %eax
sall    $24, %edx
orl     %eax, %edx
movzbl  %ch, %eax
shrl    $16, %ecx
movzbl  BitReverseTable256(%rax), %eax
movzbl  %cl, %ecx
sall    $16, %eax
orl     %eax, %edx
movzbl  BitReverseTable256(%rcx), %eax
sall    $8, %eax
orl     %eax, %edx
movl    %edx, (%r13,%rsi)
addq    $4, %rsi
cmpq    $400000000, %rsi
jne     .L3
```
更改: 我也尝试在自己机器上使用uint64，看看是否性能有所提高。相对于32-bit性能大概提高了10%。
无论你是每次用64-bit类型去反转2个32-bit的int，或者实际上看作64-bit并分两次来反转，性能都大致相当。
代码如下(对于前者，每次反转2个32-bit的int):

Shell
```
.L3:
movq    (%r12,%rsi), %rdx
movq    %rdx, %rax
shrq    $24, %rax
andl    $255, %eax
movzbl  BitReverseTable256(%rax), %ecx
movzbq  %dl,%rax
movzbl  BitReverseTable256(%rax), %eax
salq    $24, %rax
orq     %rax, %rcx
movq    %rdx, %rax
shrq    $56, %rax
movzbl  BitReverseTable256(%rax), %eax
salq    $32, %rax
orq     %rax, %rcx
movzbl  %dh, %eax
shrq    $16, %rdx
movzbl  BitReverseTable256(%rax), %eax
salq    $16, %rax
orq     %rax, %rcx
movzbq  %dl,%rax
shrq    $16, %rdx
movzbl  BitReverseTable256(%rax), %eax
salq    $8, %rax
orq     %rax, %rcx
movzbq  %dl,%rax
shrq    $8, %rdx
movzbl  BitReverseTable256(%rax), %eax
salq    $56, %rax
orq     %rax, %rcx
movzbq  %dl,%rax
shrq    $8, %rdx
movzbl  BitReverseTable256(%rax), %eax
andl    $255, %edx
salq    $48, %rax
orq     %rax, %rcx
movzbl  BitReverseTable256(%rdx), %eax
salq    $40, %rax
orq     %rax, %rcx
movq    %rcx, (%r13,%rsi)
addq    $8, %rsi
cmpq    $400000000, %rsi
jne     .L3
```
