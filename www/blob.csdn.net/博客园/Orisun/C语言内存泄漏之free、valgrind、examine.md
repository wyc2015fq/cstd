# C语言内存泄漏之free、valgrind、examine - Orisun - 博客园







# [C语言内存泄漏之free、valgrind、examine](https://www.cnblogs.com/zhangchaoyang/articles/2315879.html)





先来介绍valgrind和examine这两个工具的使用。

使用examine命令（简写是x）来查看内存地址中的值。x命令的语法如下所示：
x/<n/f/u> <addr>
n、f、u是可选的参数。
n 是一个正整数，表示显示内存的长度。

f 表示显示的格式，其取值如下：

```
x 按十六进制格式显示变量
d 十进制
u 十六进制无符号整型
o 八进制
t 二进制
c 字符
s 字符串
f 浮点数
i 指令
```

u 表示从当前地址往后请求的字节数，其取值如下：

```
b表示单字节
h表示双字节
w表示四字 节（默认）
g表示八字节
```

Valgrind 可以对编译后的二进制程序进行内存使用监测（C语言中的malloc和free，以及C++中的new和delete），找出内存泄漏问题。
Valgrind 中包含的 Memcheck 工具可以检查以下的程序错误：

```
使用未初始化的内存 (Use of uninitialised memory)
使用已经释放了的内存 (Reading/writing memory after it has been free’d)
使用超过malloc分配的内存空间(Reading/writing off the end of malloc’d blocks)
对堆栈的非法访问 (Reading/writing inappropriate areas on the stack)
申请的空间是否有释放 (Memory leaks – where pointers to malloc’d blocks are lost forever)
malloc/free/new/delete申请和释放内存的匹配(Mismatched use of malloc/new/new [] vs free/delete/delete [])
src和dst的重叠(Overlapping src and dst pointers in memcpy() and related functions)
重复free
```

一般这样使用就可以了：valgrind   --tool=memcheck   --leak-check=full   ./test

常见错误解释：

Invalid write of size 4

这可能是数组越界了

40 bytes in 1 blocks are definitely lost in loss record 1 of 1

内存没有释放

下面讲一下malloc()和free()到底干了些什么事。

malloc()之后这片内存就是你的了，你就拥有使用权了。

free()只是将你以前申请的内存块插入到 heap 中，并检测是否与 heap 中已有的内存块相邻，如相邻，则合并这两块，至于你自己定义的指针，仍保持原值，但已没有空间，需要你自己手动将其 指向 NULL。

free并没有把内存里的值清0，内存里的值仍保持原样。free之后仍然可以通过你的指针去访问内存，如果这片内存没有并操作系统挪作他用，那你访问到的还是原来的值。当然你这样做是很危险的，尤其在多线程环境下，你根本不知道这片内存是否并操作系统挪作他用了。

解决一下下面的问题。

```
1 #include<stdio.h>
 2 #include<string.h>
 3 #include<stdlib.h>
 4 
 5 struct url{
 6     char *domain;
 7     char *path;
 8     int depth;
 9 };
10 
11 void getUrl(struct url* rect){
12     char *line;
13     line=calloc(128,sizeof(char));
14     strcpy(line,"http://www.baidu.com    /gaoji/preferences.html      2");
15     char *delim=" ";
16     rect->domain=strtok(line,delim);
17     rect->path=strtok(NULL,delim);
18     rect->depth=atoi(strtok(NULL,delim));
19     free(line);
20 }
21 
22 main(){
23     struct url* rect;
24     rect=calloc(1,sizeof(struct url));
25     getUrl(rect);
26     printf("|%s|%s|%d|\n",rect->domain,rect->path,rect->depth);
27     free(rect);
28     printf("%s\n",rect->path);
29     printf("%d\n",sizeof(struct url));
30 }
```

![](https://pic002.cnblogs.com/images/2012/103496/2012010717205333.png)

结果输出sizeof(struct url)为12。想一想为什么？两个指针加一个int当然是12字节了。

在19行free掉line之后，按照常理，rect->domain,rect->path,rect->depth就已经不能再使用了，因为它们指向的都是line里面的内容。但是我们却用rect->domain,rect->path,rect->depth访问到了内存中相应的值。这就解释了free到底做了什么。

使用valgrind检测内存使用是否有问题：$ valgrind --tool=memcheck --leak-check=full ./mem

下面是截取的一些命令输出片段：

```
==9763== Invalid read of size 1
==9763==    at 0x407FD6E: vfprintf (vfprintf.c:1620)
==9763==    by 0x408789F: printf (printf.c:35)
==9763==    by 0x80485DC: main (mem.c:26)
==9763==  Address 0x41a3068 is 0 bytes inside a block of size 128 free'd
==9763==    at 0x4025BF0: free (vg_replace_malloc.c:366)
==9763==    by 0x804857F: getUrl (mem.c:19)
==9763==    by 0x80485AF: main (mem.c:25)


==9763== Invalid read of size 4
==9763==    at 0x80485ED: main (mem.c:28)
==9763==  Address 0x41a302c is 4 bytes inside a block of size 12 free'd
==9763==    at 0x4025BF0: free (vg_replace_malloc.c:366)
==9763==    by 0x80485E8: main (mem.c:27)


==9763== HEAP SUMMARY:
==9763==     in use at exit: 0 bytes in 0 blocks
==9763==   total heap usage: 2 allocs, 2 frees, 140 bytes allocated
==9763== 
==9763== All heap blocks were freed -- no leaks are possible
```
”Address 0x41a302c is 4 bytes inside a block of size 12 free'd“这句话翻译过来大概就是说要访问的地址0x41a302c已经在被free掉的内存块中了。”total heap usage:2 allocs, 2 frees, 140bytes allocated“--line是128字节，sizeof(structurl)=12,所以一共是140字节，alloc了2次，free了2次，所以no leaks are possible。我们把断点设在16、20、28行，来看一下free之后，指针存储的地址变了没，通过该指针还能否访问内存。
![](https://pic002.cnblogs.com/images/2012/103496/2012010718194756.png)

为了使26行”合法“运行，我们把19行注释掉。这时用valgrind工具又会检查出”可能“有内存泄漏--事实上也确实是有内存泄漏。

![](https://pic002.cnblogs.com/images/2012/103496/2012010718225292.png)
报告说alloc了2次，free了1次，可能在一个块里面有128 bytes的内存泄漏。这line变量free也不是，不free也不是，肿么办？解决办法：
```
1 #include<stdio.h>
 2 #include<string.h>
 3 #include<stdlib.h>
 4 
 5 struct url{
 6     char *domain;
 7     char *path;
 8     int depth;
 9 };
10 
11 void freeUrl(struct url* rect){
12     free(rect->domain);
13     free(rect->path);
14     free(rect);
15 }
16 
17 void getUrl(struct url* rect){
18     char *line;
19     line=calloc(128,sizeof(char));
20     strcpy(line,"http://www.baidu.com    /gaoji/preferences.html      2");
21     char *delim=" ";
22     char *d=calloc(40,sizeof(char));
23     char *p=calloc(80,sizeof(char));
24     strcpy(d,strtok(line,delim));
25     strcpy(p,strtok(NULL,delim));
26     rect->domain=d;
27     rect->path=p;
28     rect->depth=atoi(strtok(NULL,delim));
29     free(line);
30 }
31 
32 main(){
33     struct url* rect;
34     rect=calloc(1,sizeof(struct url));
35     printf("%d\n",sizeof(struct url));
36     getUrl(rect);
37     printf("|%s|%s|%d|\n",rect->domain,rect->path,rect->depth);
38     freeUrl(rect);
39 }
```

valgrind没有检查出任何问题：

![](https://pic002.cnblogs.com/images/2012/103496/2012010718560750.png)
















