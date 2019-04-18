# Linux虚拟内存实现原理 - nosmatch的专栏 - CSDN博客
2012年07月18日 10:23:44[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：1970
下面是一篇翻译文章，原文出自[MongoDB](http://blog.nosqlfan.com/tags/mongodb)的核心开发工程师
[Kristina Chodorow](http://www.snailinaturtleneck.com/) 的个人博客，由NoSQLFan翻译整理。
我们都知道，MongoDB 使用[内存映射](http://blog.nosqlfan.com/tags/%E5%86%85%E5%AD%98%E6%98%A0%E5%B0%84)的方式来进行数据文件的存取操作。本文的目的就在于描述操作系统[虚拟内存](http://blog.nosqlfan.com/tags/%E8%99%9A%E6%8B%9F%E5%86%85%E5%AD%98)的使用及内存映射的内部实现。
**以下是译文**
当你运行一个程序，程序中有许多东西需要存储，堆、栈以及各种功能库。而这一切在你写程序时可能都不需要自己控制，Linux内核会帮你完成这些存储的调度，你只需要告诉它你需要做什么，内核就会在合适的地方给你分配内存空间。本文主要通过几个实例程序的内存使用研究，来为大家展示Linux的内存使用状况。
第一个例子：下面一段程序会打印出程序的pid（进程号）后挂起。
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main() {
  printf("run `pmap %d`\n", getpid());
  pause();
}
将上面代码保存成文件 mem_munch.c 然后运行下面程序编译并执行：
$ gcc mem_munch.c -o mem_munch
$ ./mem_munch
run `pmap 25681`
上面进程号是25681，可能你试验的结果会不太一样。
下面我们通过pmap命令来查看一下这个小程序的内存使用情况
$ pmap 25681
25681:   ./mem_munch
0000000000400000      4K r-x--  /home/user/mem_munch
0000000000600000      4K r----  /home/user/mem_munch
0000000000601000      4K rw---  /home/user/mem_munch
00007fcf5af88000   1576K r-x--  /lib/x86_64-linux-gnu/libc-2.13.so
00007fcf5b112000   2044K -----  /lib/x86_64-linux-gnu/libc-2.13.so
00007fcf5b311000     16K r----  /lib/x86_64-linux-gnu/libc-2.13.so
00007fcf5b315000      4K rw---  /lib/x86_64-linux-gnu/libc-2.13.so
00007fcf5b316000     24K rw---    [ anon ]
00007fcf5b31c000    132K r-x--  /lib/x86_64-linux-gnu/ld-2.13.so
00007fcf5b512000     12K rw---    [ anon ]
00007fcf5b539000     12K rw---    [ anon ]
00007fcf5b53c000      4K r----  /lib/x86_64-linux-gnu/ld-2.13.so
00007fcf5b53d000      8K rw---  /lib/x86_64-linux-gnu/ld-2.13.so
00007fff7efd8000    132K rw---    [ stack ]
00007fff7efff000      4K r-x--    [ anon ]
ffffffffff600000      4K r-x--    [ anon ]
 total             3984K
上面的结果是这个程序的内存使用情况，其实更确切的说是这个程序认为它使用内存的情况。从上面的结果我们能看到，当你访问libc库时，实际上是对内存地址00007fcf5af88000的访问，当你访问ld库时，实际上是对内存地址00007fcf5b31c000的访问。
上面的输出可能还比较抽象，下面我们修改一下上面的程序，我们在程序的堆和栈上各放一块数据。
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int main() {
  int on_stack, *on_heap;
  //局部变量是放在栈上的，所以 on_stack 的地址就是栈的初始地址
  on_stack = 42;
  printf("stack address: %p\n", &on_stack);
  //malloc 的内存是在堆上分配的
  on_heap = (int*)malloc(sizeof(int));
  printf("heap address: %p\n", on_heap);
  printf("run `pmap %d`\n", getpid());
  pause();
}
编译运行：
$ ./mem_munch
stack address: 0x7fff497670bcheap address: 0x1b84010
run `pmap 11972`
然后再用pmap命令查看一下内存使用：
$ pmap 11972
11972:   ./mem_munch
0000000000400000      4K r-x--  /home/user/mem_munch
0000000000600000      4K r----  /home/user/mem_munch
0000000000601000      4K rw---  /home/user/mem_munch
0000000001b84000    132K rw---    [ anon ]00007f3ec4d98000   1576K r-x--  /lib/x86_64-linux-gnu/libc-2.13.so
00007f3ec4f22000   2044K -----  /lib/x86_64-linux-gnu/libc-2.13.so
00007f3ec5121000     16K r----  /lib/x86_64-linux-gnu/libc-2.13.so
00007f3ec5125000      4K rw---  /lib/x86_64-linux-gnu/libc-2.13.so
00007f3ec5126000     24K rw---    [ anon ]
00007f3ec512c000    132K r-x--  /lib/x86_64-linux-gnu/ld-2.13.so
00007f3ec5322000     12K rw---    [ anon ]
00007f3ec5349000     12K rw---    [ anon ]
00007f3ec534c000      4K r----  /lib/x86_64-linux-gnu/ld-2.13.so
00007f3ec534d000      8K rw---  /lib/x86_64-linux-gnu/ld-2.13.so
00007fff49747000    132K rw---    [ stack ]
00007fff497bb000      4K r-x--    [ anon ]
ffffffffff600000      4K r-x--    [ anon ]
 total             4116K
这次多出了上面红色的一行内容，红色内容就是堆的起始位置：
0000000001b84000    132K rw---    [ anon ]
在我们程序运行的输出里也有一行红色的输出，这是这个地址在程序中的内存地址：
heap address: 0x1b84010
这两个地址基本上是一样的，其中的anon是Anonymous的缩写，表明这段内存是没有文件映射的。
我们再看上面绿色的两行，与上面相对应，这两行分别是用pmap 和应用程序看到的栈起始地址：
00007fff49747000    132K rw---    [ stack ]stack address: 0x7fff497670bc
上面说到的内存使用，都只是程序认为自己对内存的使用，实际上程序在分配内存是不知道系统内存的状态的。所以上面的输出都只是从程序自己的角度看到的内存使用状况。比如在上面的例子中，我们看到程序的内存地址空间是从0×0000000000400000到0xffffffffff600000的所有地址（而0xffffffffff600000到0×00007fffffffffffffff之间的地址是有[特殊用处](http://en.wikipedia.org/wiki/X86-64#Canonical_form_addresses)的，这里不多讲）。这样算下来，我们总共可以使用的内存空间有1千万TB。
但是实际上目前没有硬件能有1千万TB的物理内存。为什么操作系统会如此设计呢？原因有很多，可以看[这里](http://en.wikipedia.org/wiki/Virtual_memory)，但也正因此，我们可以使用远远超出物理内存大小的内存空间。
### 内存映射
内存映射的原理就是让操作系统将一个文件映射到一段内存中，然后在操作这个文件内存就可以像操作内存一样。比如我们创建一个完全内容随机的文件，然后将它用内存映射的方式映射到一段内存空间中。那么我们在这段内存中随便取一位就相当于取到了一个随机数。下面就让我们来做这个实验，先用下面命令生成一个内容随机的文件。
$ dd if=/dev/urandom bs=1024 count=1000000 of=/home/user/random
1000000+0 records in
1000000+0 records out
1024000000 bytes (1.0 GB) copied, 123.293 s, 8.3 MB/s
$ ls -lh random
-rw-r--r-- 1 user user 977M 2011-08-29 16:46 random
然后我们用下面程序来将这个文件内容映射到内存，再从中取出随机数
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/mman.h>
int main() {
  char *random_bytes;
  FILE *f;
  int offset = 0;
  // open "random" for reading
  f = fopen("/home/user/random", "r");
  if (!f) {
    perror("couldn't open file");
    return -1;
  }
  // we want to inspect memory before mapping the file
  printf("run `pmap %d`, then press ", getpid());
  getchar();
  random_bytes = mmap(0, 1000000000, PROT_READ, MAP_SHARED, fileno(f), 0);
  if (random_bytes == MAP_FAILED) {
    perror("error mapping the file");
    return -1;
  }
  while (1) {
    printf("random number: %d (press  for next number)", *(int*)(random_bytes+offset));
    getchar();
    offset += 4;
  }
}
然后运行这个程序：
 $ ./mem_munch
run `pmap 12727`, then press
下面我们通过一次次的按下回车键来从这个文件中读取随机数，按下几次后我们可以再通过pmap来查看其内存空间的情况：
$ pmap 12727
12727:   ./mem_munch
0000000000400000      4K r-x--  /home/user/mem_munch
0000000000600000      4K r----  /home/user/mem_munch
0000000000601000      4K rw---  /home/user/mem_munch
000000000147d000    132K rw---    [ anon ]
00007fe261c6f000 976564K r--s-  /home/user/random
00007fe29d61c000   1576K r-x--  /lib/x86_64-linux-gnu/libc-2.13.so
00007fe29d7a6000   2044K -----  /lib/x86_64-linux-gnu/libc-2.13.so
00007fe29d9a5000     16K r----  /lib/x86_64-linux-gnu/libc-2.13.so
00007fe29d9a9000      4K rw---  /lib/x86_64-linux-gnu/libc-2.13.so
00007fe29d9aa000     24K rw---    [ anon ]
00007fe29d9b0000    132K r-x--  /lib/x86_64-linux-gnu/ld-2.13.so
00007fe29dba6000     12K rw---    [ anon ]
00007fe29dbcc000     16K rw---    [ anon ]
00007fe29dbd0000      4K r----  /lib/x86_64-linux-gnu/ld-2.13.so
00007fe29dbd1000      8K rw---  /lib/x86_64-linux-gnu/ld-2.13.so
00007ffff29b2000    132K rw---    [ stack ]
00007ffff29de000      4K r-x--    [ anon ]
ffffffffff600000      4K r-x--    [ anon ]
 total           980684K
上面的输出和之前的大同小异，但是多出了上面红色的一行。这是我们上面的随机文件映射到内存中的内存。我们再使用pmap -x 选项来查看一下程序的内存使用，会得到下面的内容，其中RSS（resident set size）列表示真实占用的内存。
pmap -x 12727
12727:   ./mem_munch
Address           Kbytes     RSS   Dirty Mode   Mapping
0000000000400000       0       4       0 r-x--  mem_munch
0000000000600000       0       4       4 r----  mem_munch
0000000000601000       0       4       4 rw---  mem_munch
000000000147d000       0       4       4 rw---    [ anon ]
00007fe261c6f000       0       4       0 r--s-  random
00007fe29d61c000       0     288       0 r-x--  libc-2.13.so
00007fe29d7a6000       0       0       0 -----  libc-2.13.so
00007fe29d9a5000       0      16      16 r----  libc-2.13.so
00007fe29d9a9000       0       4       4 rw---  libc-2.13.so
00007fe29d9aa000       0      16      16 rw---    [ anon ]
00007fe29d9b0000       0     108       0 r-x--  ld-2.13.so
00007fe29dba6000       0      12      12 rw---    [ anon ]
00007fe29dbcc000       0      16      16 rw---    [ anon ]
00007fe29dbd0000       0       4       4 r----  ld-2.13.so
00007fe29dbd1000       0       8       8 rw---  ld-2.13.so
00007ffff29b2000       0      12      12 rw---    [ stack ]
00007ffff29de000       0       4       0 r-x--    [ anon ]
ffffffffff600000       0       0       0 r-x--    [ anon ]
----------------  ------  ------  ------
total kB          980684     508     100
如果你的虚拟内存占用（上面的Kbytes列）都是0，不用担心，这是一个在Debian/Ubuntu系统上pmap -x命令的[bug](http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=614513)。最后一行输出的总占用量是正确的。
现在你可以看一下RSS那一列，这就是实际内存占用。在random文件上，你的程序实际上可以访问在00007fe261c6f000之前的数十亿字节的内存地址，但是只要你访问的地址超过4KB，那么操作系统就会去磁盘上查找内容。也就是说实际上只有4KB的物理内存被使用了。只有访问这4KB的东西时，才是真正的内存操作。其它部分虽然你使用的也是内存操作函数来访问它，但是由于它没有被加载到内存中，所以在这些内容被访问的时候，操作系统会先去磁盘读random中读取内容到内存中。
如果我们把程序再修改一下，修改成下面这样，让程序把整个random文件都访问一遍。
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/mman.h>
int main() {
  char *random_bytes;
  FILE *f;
  int offset = 0;
  // open "random" for reading
  f = fopen("/home/user/random", "r");
  if (!f) {
    perror("couldn't open file");
    return -1;
  }
  random_bytes = mmap(0, 1000000000, PROT_READ, MAP_SHARED, fileno(f), 0);
  if (random_bytes == MAP_FAILED) {
    printf("error mapping the file\n");
    return -1;
  }
  for (offset = 0; offset < 1000000000; offset += 4) {
    int i = *(int*)(random_bytes+offset);
    // to show we're making progress
    if (offset % 1000000 == 0) {
      printf(".");
    }
  }
  // at the end, wait for signal so we can check mem
  printf("\ndone, run `pmap -x %d`\n", getpid());
  pause();
}
现在我们的pmap -x命令就会得到如下输出：
$ pmap -x 5378
5378:   ./mem_munch
Address           Kbytes     RSS   Dirty Mode   Mapping
0000000000400000       0       4       4 r-x--  mem_munch
0000000000600000       0       4       4 r----  mem_munch
0000000000601000       0       4       4 rw---  mem_munch
0000000002271000       0       4       4 rw---    [ anon ]
00007fc2aa333000       0  976564       0 r--s-  random
00007fc2e5ce0000       0     292       0 r-x--  libc-2.13.so
00007fc2e5e6a000       0       0       0 -----  libc-2.13.so
00007fc2e6069000       0      16      16 r----  libc-2.13.so
00007fc2e606d000       0       4       4 rw---  libc-2.13.so
00007fc2e606e000       0      16      16 rw---    [ anon ]
00007fc2e6074000       0     108       0 r-x--  ld-2.13.so
00007fc2e626a000       0      12      12 rw---    [ anon ]
00007fc2e6290000       0      16      16 rw---    [ anon ]
00007fc2e6294000       0       4       4 r----  ld-2.13.so
00007fc2e6295000       0       8       8 rw---  ld-2.13.so
00007fff037e6000       0      12      12 rw---    [ stack ]
00007fff039c9000       0       4       0 r-x--    [ anon ]
ffffffffff600000       0       0       0 r-x--    [ anon ]
----------------  ------  ------  ------
total kB          980684  977072     104
我们可以看到，random文件映射实际占用内存量已经和random文件大小一致了，也就是也random文件通过循环访问，其内容已经完全加载到内存中了。现在我们再访问random文件的任何部分，实际上都是内存操作。而不会穿透到磁盘。
话说回来，这就是为什么MongoDB的内存使用，可以远远超出操作系统物理内存大小。
