# dmalloc用法快速入门 - DoubleLi - 博客园






[原文链接](http://blog.chinaunix.net/space.php?uid=796091&do=blog&id=2035331)

常用内存泄露检测手段有
- [1 mtrace](http://elinux.org/Memory_Debuggers#mtrace)
- [2 memwatch](http://elinux.org/Memory_Debuggers#memwatch)
- [3 mpatrol](http://elinux.org/Memory_Debuggers#mpatrol)
- [4 dmalloc](http://elinux.org/Memory_Debuggers#dmalloc)
- [5 dbgmem](http://elinux.org/Memory_Debuggers#dbgmem)
- [6 valgrind](http://elinux.org/Memory_Debuggers#valgrind)
- [7 Electric Fence](http://elinux.org/Memory_Debuggers#Electric_Fence)

dmalloc是一个简单易用的C/C++内存leak检查工具，以一个运行库的方式发布。

dmalloc能够检查出直到程序运行结束还没有释放的内存，并且能够精确指出在
哪个源文件的第几行。

dmalloc 主页： [http://dmalloc.com](http://dmalloc.com/)

支持的平台：AIX, BSD/OS, DG/UX, Free/Net/OpenBSD, GNU/Hurd, HPUX, Irix, Linux, MS-DOG, NeXT, OSF, SCO, Solaris, SunOS, Ultrix, Unixware, Windoze, and even Unicos on a Cray T3E

最新版本： 5.5.2

安装：下载 [http://dmalloc.com/releases/dmalloc-5.5.2.tgz](http://dmalloc.com/releases/dmalloc-5.5.2.tgz)




- 
tar zxvf dmalloc-5.5.2.tgz

- 
cd dmalloc-5.5.2

- 
./configure

- 
make

- 
make install






设置环境变量：
对于 Bash, ksh, and zsh ([http://www.zsh.org/](http://www.zsh.org/)), 在 `.bashrc', `.profile', or `.zshrc'
文件中加入一行 ( -b 参数表示针对Bash的输出):

function dmalloc { eval `command dmalloc -b $*`; }

然后重新登陆用户，或者执行: source ~/.bashrc 或 source ~/.profile

接下面执行:


- 
dmalloc -l logfile -i 100 low






在源文件中添加下面的C代码：

#ifdef DMALLOC
#include "dmalloc.h"
#endif

编译使用的CFLAGS: -DDMALLOC -DDMALLOC_FUNC_CHECK

如： gcc -DDMALLOC -DDMALLOC_FUNC_CHECK dm_test.c

执行:
./a.out

运行上面的命令会在当前目录下生成 logfile文件，查看logfile的内容如下：

cat logfile


- 
1214894489: 2: Dmalloc version '5.5.2' from 'http://dmalloc.com/'

- 
1214894489: 2: flags = 0x4e48503, logfile 'logfile'

- 
1214894489: 2: interval = 100, addr = 0, seen # = 0, limit = 0

- 
1214894489: 2: starting time = 1214894489

- 
1214894489: 2: process pid = 9560

- 
1214894489: 2: Dumping Chunk Statistics:

- 
1214894489: 2: basic-block 4096 bytes, alignment 8 bytes

- 
1214894489: 2: heap address range: 0xb8020000 to 0xb8029000, 36864 bytes

- 
1214894489: 2:     user blocks: 1 blocks, 4043 bytes (10%)

- 
1214894489: 2:    admin blocks: 8 blocks, 32768 bytes (89%)

- 
1214894489: 2:    total blocks: 9 blocks, 36864 bytes

- 
1214894489: 2: heap checked 1

- 
1214894489: 2: alloc calls: malloc 2, calloc 0, realloc 0, free 0

- 
1214894489: 2: alloc calls: recalloc 0, memalign 0, valloc 0

- 
1214894489: 2: alloc calls: new 0, delete 0

- 
1214894489: 2:   current memory in use: 11 bytes (2 pnts)

- 
1214894489: 2:  total memory allocated: 11 bytes (2 pnts)

- 
1214894489: 2:  max in use at one time: 11 bytes (2 pnts)

- 
1214894489: 2: max alloced with 1 call: 6 bytes

- 
1214894489: 2: max unused memory space: 53 bytes (82%)

- 
1214894489: 2: top 10 allocations:

- 
1214894489: 2:  total-size  count in-use-size  count  source

- 
1214894489: 2:           6      1           6      1  dm_test.c:71

- 
1214894489: 2:           5      1           5      1  dm_test.c:69

- 
1214894489: 2:          11      2          11      2  Total of 2

- 
1214894489: 2: Dumping Not-Freed Pointers Changed Since Start:

- 
1214894489: 2:  not freed: '0xb8028fc8|s1' (6 bytes) from 'dm_test.c:71'

- 
1214894489: 2:  not freed: '0xb8028fe8|s1' (5 bytes) from 'dm_test.c:69'

- 
1214894489: 2:  total-size  count  source

- 
1214894489: 2:           6      1  dm_test.c:71

- 
1214894489: 2:           5      1  dm_test.c:69

- 
1214894489: 2:          11      2  Total of 2

- 
1214894489: 2: ending time = 1214894489, elapsed since start = 0:00:00






那么，哪个地方的内存leak就一目了然了。

//====== dm_test.c 源代码 =============


- 
#include <stdio.h>

- 
#include <stdlib.h>

- 
#include <string.h>

- 


- 
#ifdef DMALLOC

- 
#include <dmalloc.h>

- 
#endif

- 


- 
int main(int argc, char **argv)

- 
{

- 


- 
    char *str;

- 


- 
    str = malloc(5);

- 


- 
    str = malloc(6);

- 


- 
    return 0;

- 
}












