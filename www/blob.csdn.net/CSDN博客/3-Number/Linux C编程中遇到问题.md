
# Linux C编程中遇到问题 - 3-Number - CSDN博客


置顶2015年07月10日 15:01:35[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：420标签：[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=c&t=blog)个人分类：[C编程																](https://blog.csdn.net/pangyemeng/article/category/5429119)


**1、memcpy****和****memmove**
功能：拷贝一定长度内存内容到指定的内存；
区别：当内存发生局部重叠的时候，memmove保证拷贝的结果是正确的，memcpy不保证拷贝的结果的正确；
函数原型：
void *memcpy(void *dst, const void *src, size_t count);
void *memmove(void *dst, const void *src, size_t count);
图形看出两者的区别:
![](https://img-blog.csdn.net/20150715154229611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第二种体现出两者的区别；
**2、sprintf****和****snprintf**
功能：格式化的数据写入某个字符串中
原型：
int sprintf( char *buffer, const char *format, [ argument] … );
int snprintf(char *str, size_t size, const char *format,[ argument] … )
(1) 如果格式化后的字符串长度 < size，则将此字符串全部复制到str中，并给其后添加一个字符串结束符('\0')；
(2) 如果格式化后的字符串长度 >= size，则只将其中的(size-1)个字符复制到str中，并给其后添加一个字符串结束符('\0')，返回值为欲写入的字符串长度。
区别：（1）sprintf存在内存溢出的问题；
（2）返回参数不同，看如下测试程序以及输出：
```python
#include <stdio.h>
int main()
{
        int ret_value = 0;
        char buffer[1024] = { 0 };
        const char *str = "1234567890";
        char buff1[1024] = { 0 };
        char buff2[1024] = { 0 };
        int len1 = 0, len2 = 0;
        int i;
        ret_value = sprintf(buffer, "%s", str);
        printf("ret_value = %d\nbuffer = %s\n", ret_value, buffer);
        ret_value = snprintf(buffer, 5, "%s", str);
        printf("ret_value = %d\nbuffer = %s\n", ret_value, buffer);
    
        len1 = snprintf(buff1, 1024, "%s", "a");
        printf("len1 = %d\n buff1 = %s\n", len1, buff1);
        len2 = sprintf(buff2, "%s", "a");
        printf("len2 = %d\n buff2 = %s\n", len2, buff2);    
        for(i = 0; i < 3; i++)
        {   
              snprintf(buff1, 1024, "%s%s", buff1, "a");
              sprintf(buff2, "%s%s", buff2, "a");
        }   
        printf("buff1 = %s\n", buff1);
        printf("buff2 = %s\n", buff2);
        return 0;
}
```
输出：
![](https://img-blog.csdn.net/20150715154618751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（3）snprintf每次会将缓存区buffer[0] =’\0’，而sprintf没有这个参数，可以参照glibc标准源码。上面用测试用例验证。
类似：strcpy() sprintf() strcat() 存在安全隐患， 其对应的安全版为：
strncpy() snprintf() strncat()
**3、指针数组和数组指针**
指针数组：array of pointers，即用于存储指针的数组，也就是数组元素都是指针
数组指针：a pointer to an array，即指向数组的指针
还要注意的是他们用法的区别，下面举例说明。
int* a[4]指针数组
表示：数组a中的元素都为int型指针
元素表示：*a[i]   *(a[i])是一样的，因为[]优先级高于*
int (*a)[4]数组指针
表示：指向数组a的指针
元素表示：(*a)[i]

**4、for****循环中三条语句执行顺序：**
看下面例子，以及汇编代码来查看for循环中的执行顺序：

```python
#include <stdio.h>
int main()
{
        int i;
        for(i = 0; i < 10; i++)
        {
                printf("%d\n", i);
        }
        return 0;
}
```
编译成汇编：gcc -S for_test.c
查看汇编代码：

```python
.file   "for_test.c"
        .section        .rodata
.LC0:
        .string "%d\n"
        .text
.globl main
        .type   main, @function
main:
        pushl   %ebp
        movl    %esp, %ebp
        andl    $-16, %esp
        subl    $32, %esp
        movl    $0, 28(%esp)
        jmp     .L2
.L3:
        movl    $.LC0, %eax
        movl    28(%esp), %edx
        movl    %edx, 4(%esp)
        movl    %eax, (%esp)
        call    printf
        addl    $1, 28(%esp)
.L2:
        cmpl    $9, 28(%esp)
        jle     .L3
        movl    $0, %eax
        leave
        ret
        .size   main, .-main
        .ident  "GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-4)"
        .section        .note.GNU-stack,"",@progbits
```
很清晰的看到，
先对整数赋值，条件判断，for体执行，变量递增。




