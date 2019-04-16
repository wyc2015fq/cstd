# Linux c 开发 - 内存管理 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年10月02日 09:27:06[initphp](https://me.csdn.net/initphp)阅读数：1175
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









### C语言内存分配

在linux上开发c语言的时候，最复杂的就是内存的分配和回收，经常一不小心就有可能造成内存的泄露。

一般情况下，内存分配有三种方式：
-  从静态存储区域分配。内存在程序编译的时候就已经分配好，这块内存在程序的整个运行期间都存在。例如全局变量，static变量。
- 在栈上创建。在执行函数时，函数内局部变量的存储单元都可以在栈上创建，函数执行结束时这些存储单元自动被释放。栈内存分配运算内置于处理器的指令集中，效率很高，但是分配的内存容量有限。
- 从堆上分配，亦称动态内存分配。在程序运行过程中，由用户自己申请分配的内存称为动态内存。动态内存必须手工申请，手工释放。

动态内存的自由性让内存分配变得十分方便，但是也会存在巨大的隐患，一部小心如果申请的内存块没有释放， 就会出现内存泄露。在大型 程序中，由于释放某块内存前将指向该区域内存块的指针重新赋值，则导致内存区域无法被释放的情况会经常出现。


操作系统分配给其运行的内存块分为5个区域：
- **栈区（stack）**—　 由编译器自动分配释放 ，存放为运行函数而分配的局部变量、函数参数、返回数据、返回地址等。其操作方式类似于数据结构中的栈。
- **堆区（heap）** —　 一般由程序员分配释放， 若程序员不释放，程序结束时可能由OS回收 。
- **全局区（静态区）（static）**—存放全局变量、静态数据、常量。程序结束后由系统释放
- **文字常量区**  — 常量字符串就是放在这里的。 程序结束后由系统释放。
- **程序代码区** — 存放函数体（类成员函数和全局函数）的二进制代码。

![](https://img-blog.csdn.net/20151029161150792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int a = 0; //全局初始化区
int *p; //全局未初始化区
int main (int argc, char *argv[])
{
        int b = 0; //在栈上分配
        static int st = 1; //在全局区上分配
        char *s = "woshishen"; //文字常量区
        char *x = (char *)malloc(sizeof(char) * 10); //在堆上分配
        x[0] = 'a';
        x[1] = 'b';
        x[2] = '\0';
        printf("s:%s", x);
        free(x);

}
```




内存分配地址值：

1. 栈上分配的内存地址值，**高地址到低地址**。



```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main (int argc, char *argv[])
{
        int a = 5; //栈上分配，高地址到低地址
        int b = 6;
        printf("a:%d\r\n", &a);
        printf("b:%d\r\n", &b);
}
```
输出：





```cpp
[admin@localhost ~]$ ./main 
a:746356348
b:746356344
```


2. 堆上分配的内存地址值，**低地址到高地址**。





```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main (int argc, char *argv[])
{
        //堆上分配，是低地址到高地址
        int *a = (int *) malloc(sizeof(int));
        int *b = (int *) malloc(sizeof(int));
        *a = 5;
        *b = 6;
        printf("a:%d; value:%d\r\nb:%d value:%d\r\n", a,*a, b, *b);
        free(a);
        free(b);
}
```
输出：





```cpp
[admin@localhost ~]$ ./main 
a:37855248; value:5
b:37855280 value:6
```






### 动态内存管理操作

#### 1. malloc分配内存函数



```cpp
void *malloc(unsigned int num_bytes);
```



返回指向内存块的指针，参数值为申请的内存块的大小，如果内存申请失败则返回NULL。



```cpp
#include <stdio.h>
    #include <stdlib.h>

    int main() {
        char *x = (char *)malloc(sizeof(char) * 20); //分配一块内存块，大小为sizeof(char) * 20,返回 char *类型
        if (x != NULL) {
                x[0] = 'H';
                x[1] = 'e';
                x[2] = 'l';
                x[3] = 'l';
                x[4] = 'o';
                printf("malloc Value:%s\n", x);
                free(x); //释放malloc申请的内存块
        }

        return 0;
    }
```







#### 2. calloc分配内存并初始化函数



```cpp
void *calloc( size_t numElements, size_t sizeOfElement );
```



第一个参数为分配内存个数，第二个参数为每个月的大小例如sizeof(char),如果内存申请失败则返回NULL

calloc和malloc最大的区别就是calloc函数初始化会将所分配的内存空间把所有的位设置为0



```cpp
#include <stdio.h>
    #include <stdlib.h>

    int main() {
        char *x = (char *)malloc(sizeof(char) * 20); //分配一块内存块，大小为sizeof(char) * 20,返回 char *类型
        if (x != NULL) {
                x[0] = 'H';
                x[1] = 'e';
                x[2] = 'l';
                x[3] = 'l';
                x[4] = 'o';
                printf("malloc Value:%s\n", x);
                free(x); //释放malloc申请的内存块
        }

        char *y = (char *)calloc(20, sizeof(char)); //使用calloc分配一块内存，每个大小sizeof(char),共20个
        if (y != NULL) {
                y[0] = 'w';
                y[1] = 'o';
                y[2] = 's';
                y[3] = 'h';
                y[4] = 'i';
                printf("calloc Value:%s \n", y);
                free(y); //释放calloc申请的内存块
        }

        return 0;
    }
```







#### 3. free释放内存函数



```cpp
void free(void *ptr)
```



参数为内存块指针，free函数主要释放malloc和calloc申请的内存块的内存。




#### 4. realloc调整动态内存的大小



```cpp
void *realloc(void *mem_address, unsigned int newsize);
```



mem_address：已经通过malloc/calloc分配的内存地址的指针。如果为NULL，则相当于malloc重新分配一块内存块

newsize：分配的新内存块的大小。如果size为0，则相当于free函数。

newsize可以大小或者小于原来的动态内存块的大小。通常操作系统会在原来的内存块上进行大小的调整操作；如果调整的内存块大小大于原来的内存块并且操作系统无法执行调整的时候则会重新开辟新的空间，将原有的内存块进行拷贝。



```cpp
#include <stdio.h>
    #include <stdlib.h>

    int main() {
        char *x = (char *)malloc(sizeof(char) * 5); //分配一块内存块，大小为sizeof(char) * 20,返回 char *类型
        if (x != NULL) {
                x[0] = 'H';
                x[1] = 'e';
                x[2] = 'l';
                x[3] = 'l';
                x[4] = 'o';
                printf("malloc Value:%s\n", x);
                //free(x); //释放malloc申请的内存块
        }

        //将x指向的内存块进行调整，调整到大小为sizeof(char) * 6
        char *z = (char *)realloc(x, sizeof(char) * 6);
        if (z != NULL) {
                z[5] = '!';
                printf("realloc Value:%s\n", z);
        }
        free(z);

        return 0;
    }
```




#### 5. alloca从栈中分配内存空间函数



```cpp
void * alloca(size_t size);
```



使用方法和malloc相同。但是不同的是alloca是从栈中分配内存空间的，当调用这个方法的函数返回了之后会自动销毁栈上的内存空间。




**6. 内存块设置**



```cpp
void *memset(void *s,int c,size_t n)
```


设置内存块内容。





### 内存锁定操作



为了更好的利用系统资源，当一块内存区域在一段时间内不被使用，则可以暂时置换到磁盘上。只有超级用户才可以使用内存锁定相关的函数，这里只做API介绍。

#### 1. mlock 内存锁定函数

```cpp
int mlock(const void *addr, size_t   len);
```

addr：要锁定的内存地址指针

len：要锁定的内存区域的大小




#### 2. munlock 内存锁定解除函数



```cpp
int munlock(const void *addr, size_t   len);
```





addr：要解除锁定的内存地址指针

len：要解除锁定的内存区域的大小

#### 3. mlockall 一次锁定多个内存页



```cpp
int mlockall(int flags);
```



flags：MCL_CURRENT 锁定所有内存页；MCL_FUTURE 锁定所有为进程的地址空间添加的内存页。




#### 4. munlockall 用于解除所有多顶的内存空间页



```cpp
int munlockall(void);
```








### 内存映像操作



内存映像其实就是在内存中创建一个和外存文件完全相同的映像。用户可以将整个文件映射到内存中也可以部分映射到内存。系统会将对内存映像的改动如实的反映到外存文件中。从而实现了通过内存映像对外存文件的操作。

**特点：**

1. 加快对IO的操作速度

2. 用户可以通过指针间接对文件进行操作。

3. 实现了文件数据的共享，将外存文件映射到共享内存中，很方便的实现了数据共享，并能完成把数据保存到外存的工作。


内存映像需要加载：



```cpp
#include<sys/types.h>   
#include<sys/mman.h>
```



#### 1. mmap 创建内存映像



```cpp
void *mmap(void *start,size_t length,intport,int flag,int fd,off_t offset)
```



start：为指针通常设为NULL,表示映像内存有系统决定。因为指定内存会经常出错。

length：内存映像需要占用的内存空间大小。

port：内存印象的安全属性。**PROT_EXEC**表示被映像内存可能有机器码，可执行。**PORT_NONE**表示被映像内存不能被访问。**PORT_READ**表示被映像内存可。**PORT_WRITE**表示被映像内存可写

flag：内存映像标志。**MAP_FIXED**表示如果无法从start地址建立内存映像，则出错返回。**MAP_PRIVATE**表示对内存映像进行的改动不反映到外存文件中。**MAP_SHARED**表示对内存映像进行的改动反映到外存文件中。

fd：文件描述符

offset：表示所映像的内容距文件头的距离。



```cpp
#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
int main() {
        int fd;
        char *x;
        fd = open("test.bat", O_RDWR); //打开一个文件
        if (fd == -1) {
                printf("Open File Error!");
                return -1;
        }
        x = mmap(NULL,1000,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);//对打开的文件进行内存映像
        if (x != NULL) {
                printf("mmap:%s\n", x); //文件内容被映射到了内存上，打印出来的是文件的内容
                x[0] = 'P'; //这边对文件开头的两个字符进行修改，因为我们设置了MAP_SHAPED ，所以内存内容有改变就会映射到文件上，文件上也会做修改。
                x[1] = 'X';
                printf("change:%s\n", x); //这个时候打印的内容就是修改过后的内容了
        }
        return 0;
}
```





#### 2. munmap 撤销内存映像

```cpp
int munmap(void *start,size_t length);
```



start：内存映像的指针地址

length：长度



```cpp
#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
int main() {
        int fd;
        char *x;
        fd = open("test.bat", O_RDWR); //打开一个文件
        if (fd == -1) {
                printf("Open File Error!");
                return -1;
        }
        x = mmap(NULL,1000,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);//对打开的文件进行内存映像
        if (x != NULL) {
                printf("mmap:%s\n", x); //文件内容被映射到了内存上，打印出来的是文件的内容
                x[0] = 'P'; //这边对文件开头的两个字符进行修改，因为我们设置了MAP_SHAPED ，所以内存内容有改变就会映射到文件上，文件上也会做修改。
                x[1] = 'X';
                printf("change:%s\n", x); //这个时候打印的内容就是修改过后的内容了
                munmap(x, 1000); //撤销
        }
        return 0;
}
```




#### 3. msync 将内存映像写入外存
此函数将内存映像中的改动刷新到外存文件中去。





```cpp
int msync ( void * start, size_t len, int flags)
```



start：内存映像指针

len：长度

flags：操作属性。MY_ASYNC 调度一个写操作并且返回；MS_INVALIDATE 使映射到相同文件的内存映像无效以便使他们更改为新的数据。MY_SYNC 完成写操作后函数返回。



```cpp
#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
int main() {
        int fd;
        char *x;
        fd = open("test.bat", O_RDWR); //打开一个文件
        if (fd == -1) {
                printf("Open File Error!");
                return -1;
        }
        x = mmap(NULL,1000,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);//对打开的文件进行内存映像
        if (x != NULL) {
                printf("mmap:%s\n", x); //文件内容被映射到了内存上，打印出来的是文件的内容
                x[0] = 'P'; //这边对文件开头的两个字符进行修改，因为我们设置了MAP_SHAPED ，所以内存内容有改变就会映射到文件上，文件上也会做修改。
                x[1] = 'X';
                printf("change:%s\n", x); //这个时候打印的内容就是修改过后的内容了
                msync(x, 1000, MS_SYNC); //同步写入外存
                munmap(x, 1000); //撤销
        }
        return 0;
}
```




#### 4. 修改内存映像的保护值



```cpp
int protect(const void *addr,size_tlength,int prot);
```
addr：表示地址和上面相同。

length：内存映像大小同上。

prot：重新设定的保护值。

成功返回0失败返回-1






#### 5. 修改内存镜像大小



```cpp
void *mremap(void *old_addr,size_told_length,size_t new_length,unsigned long *flag)
```















