# linux下cp覆盖原so文件时引起的段错误原因确定 - xqhrs232的专栏 - CSDN博客
2016年10月18日 22:29:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：403
原文地址::[http://www.cnblogs.com/leo0000/p/5694416.html](http://www.cnblogs.com/leo0000/p/5694416.html)
原创作品，转载请注明出处[http://www.cnblogs.com/leo0000/p/5694416.html](http://www.cnblogs.com/leo0000/p/5694416.html)
　　最近因为一个很有意思的段错误学习了一些新的东西。
　　当时现象是这样的，程序正在运行，系统升级，此时某些so已经被该程序所使用，现在把这些so文件覆盖了，导致了该程序崩溃。
　　调试dump文件可以发现是崩溃在了ld解析函数符号的时候，然后查看libc的源码，发现崩溃的函数checkmatch传入的参数是空指针，所以导致了崩溃。因为受到以前写裸机代码的影响，裸机是这样的，如果前2M stepstorm不够用，那么在stepstorm中的代码就把nandflash中的代码拷贝到内存中，然后跳转到内存中去运行，所以此时就算源文件再怎么被修改也不会受到nandflash中的内容影响。下面先讲两个需要用到的知识点。
　　linux下很重要的一点是，一个文件可以被很多应用程序打开，同一时间的确只有一个应用程序可以对该文件读写，但是在不同时刻，所有应用程序对文件的操作都会影响到其他已打开该文件的应用程序，因为在每次读写前，系统调用read和write会对内存中的内容进行有效性判断。
　　再讲一个有关mmap或者mmap2的事情，
　　void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
　　addr是要映射到的内存地址，返回值也是被映射到的内存地址，因为一般指定为0，有内核选择一段可用的内存空间。
　　len表示要映射的内存大小。
　　prot表示这段内存的访问权限。
　　flags表示映射后内存的类型，主要是对该内存的写是否会影响到原文件。
　　fd表示文件描述符，
　　offset表示需要映射的文件内容相对文件头偏移量。
　　映射完了之后，对这个内存的访问就是对文件的访问。
　　下面看栗子：
　　源码:
　　共享库：
```
#include <stdio.h>
int fun1()
{
        printf("fun1\n");
}
```
　　main函数
```
int fun1();
int main()
{
        while(1){
                sleep(10);
                fun1();
        }
}
```
　　这个代码很简单，下面先用strace跟踪下test的运行:
```
strace ./test
execve("./test", ["./test"], [/* 22 vars */]) = 0
brk(0)                                  = 0x9653000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
mmap2(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb7731000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
open("tls/i686/sse2/cmov/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/i686/sse2/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/i686/cmov/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/i686/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/sse2/cmov/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/sse2/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/cmov/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("i686/sse2/cmov/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("i686/sse2/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("i686/cmov/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("i686/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("sse2/cmov/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("sse2/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("cmov/lib1.so", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("lib1.so", O_RDONLY|O_CLOEXEC)     = 3
read(3, "\177ELF\1\1\1\0\0\0\0\0\0\0\0\0\3\0\3\0\1\0\0\0\260\3\0\0004\0\0\0"..., 512) = 512
fstat64(3, {st_mode=S_IFREG|0755, st_size=6732, ...}) = 0
getcwd("/home/keda/caozhenhua/test/updateso", 128) = 36
mmap2(NULL, 8212, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0xb772e000
mmap2(0xb772f000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0) = 0xb772f000
close(3)                                = 0
open("tls/i686/sse2/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/i686/sse2/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/i686/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/i686/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/sse2/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/sse2/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("i686/sse2/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("i686/sse2/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("i686/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("i686/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("sse2/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("sse2/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("libc.so.6", O_RDONLY|O_CLOEXEC)   = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/tls/i686/sse2/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/tls/i686/sse2/cmov", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/tls/i686/sse2/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/tls/i686/sse2", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/tls/i686/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/tls/i686/cmov", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/tls/i686/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/tls/i686", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/tls/sse2/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/tls/sse2/cmov", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/tls/sse2/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/tls/sse2", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/tls/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/tls/cmov", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/tls/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/tls", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/i686/sse2/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/i686/sse2/cmov", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/i686/sse2/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/i686/sse2", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/i686/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/i686/cmov", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/i686/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/i686", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/sse2/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/sse2/cmov", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/sse2/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/sse2", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/cmov/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso/cmov", 0xbf9bd320) = -1 ENOENT (No such file or directory)
open("/home/keda/caozhenhua/test/updateso/libc.so.6", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
stat64("/home/keda/caozhenhua/test/updateso", {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat64(3, {st_mode=S_IFREG|0644, st_size=83733, ...}) = 0
mmap2(NULL, 83733, PROT_READ, MAP_PRIVATE, 3, 0) = 0xb7719000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
open("/lib/i386-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\1\1\1\0\0\0\0\0\0\0\0\0\3\0\3\0\1\0\0\0000\226\1\0004\0\0\0"..., 512) = 512
fstat64(3, {st_mode=S_IFREG|0755, st_size=1713640, ...}) = 0
mmap2(NULL, 1723100, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0xb7574000
mmap2(0xb7713000, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x19f) = 0xb7713000
mmap2(0xb7716000, 10972, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0xb7716000
close(3)                                = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb7573000
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb7572000
set_thread_area({entry_number:-1 -> 6, base_addr:0xb75726c0, limit:1048575, seg_32bit:1, contents:0, read_exec_only:0, limit_in_pages:1, seg_not_present:0, useable:1}) = 0
mprotect(0xb7713000, 8192, PROT_READ)   = 0
mprotect(0xb772e000, 4096, PROT_READ|PROT_WRITE) = 0
mprotect(0xb772e000, 4096, PROT_READ|PROT_EXEC) = 0
mprotect(0xb772f000, 4096, PROT_READ)   = 0
mprotect(0x8049000, 4096, PROT_READ)    = 0
mprotect(0xb7754000, 4096, PROT_READ)   = 0
munmap(0xb7719000, 83733)               = 0
rt_sigprocmask(SIG_BLOCK, [CHLD], [], 8) = 0
rt_sigaction(SIGCHLD, NULL, {SIG_DFL, [], 0}, 8) = 0
rt_sigprocmask(SIG_SETMASK, [], NULL, 8) = 0
nanosleep({10, 0}, 
0xbf9bd958)          = 0
fstat64(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 1), ...}) = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb772d000
write(1, "fun1\n", 5fun1
)                   = 5
rt_sigprocmask(SIG_BLOCK, [CHLD], [], 8) = 0
rt_sigaction(SIGCHLD, NULL, {SIG_DFL, [], 0}, 8) = 0
rt_sigprocmask(SIG_SETMASK, [], NULL, 8) = 0
nanosleep({10, 0},
```
　　可以看到每打开一个共享库，linux利用的是mmap2，而不是像裸机一样的read和write。
　　所以这样的话，比如需要跳转到共享库中的某个函数，如果是第一次的话，那么需要从文件中把内容加载到内存，然后再运行。
　　那么如果在程序运行时，出现缺页，那么就需要从内存中重新读取该文件的该段内容，而如果该文件被修改了而且该段是第一次被访问，那么读取到的内容将会导致不可预知的错误。
　　接下来再对mmap实验。
　　源码：
```
#include <sys/mman.h>
#include <fcntl.h>
int main()
{
        int fd;
        int i = 0;
        char *buf;
        fd = open("./libvsipstack.a",O_RDONLY);
        buf = mmap(0,10,PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE,fd,0);
        while(1)
        {
                i++;
                sleep(10);
                printf("%c\n",buf[i]);
        }
}
```
　　mmap的映射方式和加载库的方式一致，
　　然后在程序运行时我用vi修改了libvsipstack.a，修改之后可以看到libvsipstack.a是有一个备份文件的。此时是可以保证数据是正确的。
```
-rwxrwxrwx 1 root root 886056 2016-07-22 12:15 \
-rwxrwxrwx 1 root root   7249 2016-07-22 12:12 a.out
-rwxrwxrwx 1 root root     12 2016-07-22 12:12 hello.c
-rwxrwxrwx 1 root root 886056 2016-07-22 12:16 libvsipstack.a
-????????? ? ?    ?         ?                ? libvsipstack.a~
-rwxrwxrwx 1 root root    263 2016-07-22 12:12 test.c
```
　　而当我们在上面的实验中，用libvsipstack.a覆盖lib1.so，并没有导致lib1.so有一个备份，而是变成了libvsipstack.a一样的文件。所以下一次读取将会和源文件不一致。
```
-rwxrwxrwx 1 root root      0 2016-07-22 09:08 core
-rwxrwxrwx 1 root root     54 2016-07-21 13:37 lib1.c
-rwxrwxrwx 1 root root 886022 2016-07-22 12:21 lib1.so
-rwxrwxrwx 1 root root   6732 2016-07-22 09:22 lib2.so
-rwxrwxrwx 1 root root 886022 2016-07-21 15:39 libvsipstack.a
-rwxrwxrwx 1 root root   7164 2016-07-22 09:16 test
-rwxrwxrwx 1 root root     65 2016-07-22 09:10 test.c
```
　　需要知道当源文件更新时，使用mmap到的内存，系统会去重新读取文件中的内容。
　　所以当库被更新时，同时会更新内存中的内容。
　　总结一下,
　　是否崩在ld中，是由是否是第一次调用该函数决定的，因为只有第一次调用才会需要去解析plt表中的内容。
　　首先内存的布局是根据elf文件头来部署的。所以替换前后的文件不是同一个那么就会导致非法指令之类的错误，根本无法确定错误来源。
　　但如果替换前后库是一致的，那么差别就在于，重定向表的内容变的无效了。
　　重定向表有两种一种是在程序启动时就完成符号解析的，看栗子
```
Dump of assembler code for function fun1:
   0xb7fd846c <+0>:     55      push   %ebp
   0xb7fd846d <+1>:     89 e5   mov    %esp,%ebp
   0xb7fd846f <+3>:     83 ec 18        sub    $0x18,%esp
   0xb7fd8472 <+6>:     c7 04 24 d4 04 00 00    movl   $0x4d4,(%esp)
   0xb7fd8479 <+13>:    e8 fc ff ff ff  call   0xb7fd847a <fun1+14>
   0xb7fd847e <+18>:    c9      leave  
   0xb7fd847f <+19>:    c3      ret
```
　　本来+13位置处的代码已经完成了重定向，但是重新读取之后呢，恢复了重定向之前的情况，结果崩溃。
　　另一种是懒惰式的加载，是先跳到plt表的情况，看栗子
```
0xb7fd845c <+0>:     push   %ebp
   0xb7fd845d <+1>:     mov    %esp,%ebp
   0xb7fd845f <+3>:     push   %ebx
   0xb7fd8460 <+4>:     sub    $0x14,%esp
   0xb7fd8463 <+7>:     call   0xb7fd8457 <__i686.get_pc_thunk.bx>
   0xb7fd8468 <+12>:    add    $0x1b8c,%ebx
   0xb7fd846e <+18>:    lea    -0x1b12(%ebx),%eax
   0xb7fd8474 <+24>:    mov    %eax,(%esp)
   0xb7fd8477 <+27>:    call   0xb7fd8380 <puts@plt>
   0xb7fd847c <+32>:    add    $0x14,%esp
   0xb7fd847f <+35>:    pop    %ebx
   0xb7fd8480 <+36>:    pop    %ebp
   0xb7fd8481 <+37>:    ret    
End of assembler dump.
(gdb) disassemble 0xb7fd8380
Dump of assembler code for function puts@plt:
   0xb7fd8380 <+0>:     jmp    *0x10(%ebx)
   0xb7fd8386 <+6>:     push   $0x8
   0xb7fd838b <+11>:    jmp    0xb7fd8360
End of assembler dump.
(gdb) i r
eax            0xb7fd84e2       -1208122142
ecx            0xbffff6a8       -1073744216
edx            0xb7fbeff4       -1208225804
ebx            0xb7fd9ff4       -1208115212
esp            0xbffff6ac       0xbffff6ac
ebp            0xbffff6c8       0xbffff6c8
esi            0x0      0
edi            0x0      0
eip            0x386    0x386
eflags         0x210292 [ AF SF IF RF ID ]
cs             0x73     115
ss             0x7b     123
ds             0x7b     123
es             0x7b     123
fs             0x0      0
gs             0x33     51
(gdb) disassemble 0x00000386
No function contains specified address.
(gdb) x/x 0xb7fd9ff4+0x10 
0xb7fda004:     0x00000386
```
　　可以看到先跳到了plt表，因为是重新读入，相当于还没有重定位，所以跳转到下一句话，可以反汇编看到0x386就是原反汇编没有偏移地址的地址大小。
```
00000380 <puts@plt>:
 380:    ff a3 10 00 00 00        jmp    *0x10(%ebx)
 386:    68 08 00 00 00           push   $0x8
 38b:    e9 d0 ff ff ff           jmp    360 <_init+0x3c>
```
　　所以这里面的情况相当于基地址被初始为了0，导致崩溃。
　　因为mmap被调用时指明的是private，所以内存的修改不会影响到文件的内容，但是文件的修改会影响到内存的内容，所以导致了崩溃的产生。
　　说这个是系统的bug也可以，说是自己的使用不当也可以。
　　还有一个是权限问题，除了拥有root权限的用户，是不可以这样随意覆盖文件的。
