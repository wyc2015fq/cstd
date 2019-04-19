# 实现和调试 Malloc、Free、Calloc 和 Realloc 的快速教程 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [汤晓](http://www.jobbole.com/members/ashiontang) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[danluu](http://danluu.com/malloc-tutorial/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
让我们编写一个[malloc](http://man7.org/linux/man-pages/man3/malloc.3.html)函数，看看它在既有程序中如何工作！
本教程假定你了解指针，知道C语言中 `*ptr` 间接引用一个指针， `ptr->foo` 表示 `(*ptr).foo`，malloc用于[内存动态分配](http://duartes.org/gustavo/blog/post/how-the-kernel-manages-your-memory/)，并且熟悉链表的概念。如果想要学习本教程但你不了解C，请[告知我](https://twitter.com/danluu)哪些部分需要更详细的论述。如果你想要马上浏览所有代码，可以再[这里](https://github.com/danluu/malloc-tutorial/blob/master/malloc.c)查看。该测试代码由Andrew Roth提供，他的github代码仓库中存放了一些malloc函数的测试代码。
暂且不管引导部分，malloc函数的定义如下：

C
```
void *malloc(size_t size);
```
函数输入字节大小，返回指向输入字节大小内存的指针。
实现方法有很多。我们直接选择使用[sbrk](http://man7.org/linux/man-pages/man2/sbrk.2.html)系统调用。操作系统为进程预留了堆和栈空间，sbrk允许我们操作堆。sbrk(0)会返回指向当前堆顶部的指针。sbrk(foo)会增加foo字节的堆空间并返回指向当前堆顶部的指针。
![](http://ww2.sinaimg.cn/mw690/63918611gw1enkfzlvgzzj205e08dmxi.jpg)
如果想要实现一个很简单的malloc，我们可以这样做：

C
```
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
void *malloc(size_t size) {
  void *p = sbrk(0);
  void *request = sbrk(size);
  if (request == (void*) -1) {
    return NULL; // sbrk failed
  } else {
    assert(p == request); // Not thread safe.
    return p;
  }
}
```
当程序调用malloc进行空间分配时，malloc调用sbrk增加堆空间并返回指向堆上新分配区域起始位置的指针。这里丢失了一些技术细节， `malloc(0)`应该返回 `NULL` 或者另一个可以传递给free函数而不造成破坏的指针，但它基本上可以工作。
但说到free函数，free是如何工作的？Free的原型如下：

C
```
void free(void *ptr);
```
当free函数中传入一个由malloc返回的指针时，它应该释放这块空间。但如果传入的指针由我们编写的malloc函数返回，我们就无法获取指针关联的空间大小。我们在哪里存储相关信息？如果我们的malloc正常工作，我们可以分配一些空间并在那存储这些信息，但如果每次调用malloc函数分配更多空间都必须调用malloc函数来分配更多空间，我们将陷入困境。
解决上述问题常见的方法是在返回指针之后某处存储当前内存区域的元信息。假设当前堆顶地址为 `0x1000` ，我们需要分配 `0x400` 字节空间。我们当前的malloc函数会使用sbrk函数申请 `0x400` 字节空间并返回指向0x1000的指针。如果说使用 `0x10` 字节空间存储块信息，我们的malloc需要调用 `sbrk` 分配 `0x410` 字节空间并返回一个指向 `0x1010`地址的指针，将0x10字节的元信息与调用malloc函数的代码分离开。
这就允许我们释放内存单元，但接着该怎么做？从操作系统中分配的堆空间必须是连续的，所以我们不能返回操作系统中间的内存块。即使我们愿意复制新释放区域上的所有内容向下填补这块空白，这样我们可以返回空间的尾部,但依旧没办法通知的所有代码的堆指针必须进行调整。
相反，我们可以标记已经释放的内存块而不必将其返回操作系统，这样以后调用malloc可以使用回收的内存块。但那样做必须能访问每一块内存的元信息。可行的方案有很多，为简单起见这里我们直接选用单链表。
那么，对于每一块内存，我们需要有以下信息：

C
```
struct block_meta {
  size_t size;
  struct block_meta *next;
  int free;
  int magic;    // For debugging only. TODO: remove this in non-debug mode.
};
#define META_SIZE sizeof(struct block_meta)
```
我们需要知道内存块的大小，无论是否空闲，也不管下一块内存是什么。magic参数是为了便于调，但实际是不必要的；我们将其设为任意值，方便我们查看最后修改结构体的代码。
我们还需要给链表添加一个头指针：

C
```
void *global_base = NULL;
```
对于编写的malloc，我们希望尽可能的重用空闲空间，在不能重新使用已存在空间时进行空间分配。假设我们有这样的链表结构，检查其中是否包含空闲内存并直接返回。当需要分配内存空间时，我们会遍历整个链表来查看是否存在足够的空闲空间。

C
```
struct block_meta *find_free_block(struct block_meta **last, size_t size) {
  struct block_meta *current = global_base;
  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }
  return current;
}
```
如果找不到空闲内存块，我们必须使用sbrk从操作系统中申请空间并将新申请的内存块添加至链表结尾。

C
```
struct block_meta *request_space(struct block_meta* last, size_t size) {
  struct block_meta *block;
  block = sbrk(0);
  void *request = sbrk(size + META_SIZE);
  assert((void*)block == request); // Not thread safe.
  if (request == (void*) -1) {
    return NULL; // sbrk failed.
  }
  if (last) { // NULL on first request.
    last->next = block;
  }
  block->size = size;
  block->next = NULL;
  block->free = 0;
  block->magic = 0x12345678;
  return block;
}
```
和原来的实现一样，我们使用sbrk申请空间。但我们增加了一些额外空间来存储结构体并对结构体变量进行了合理设置。
既然我们已经拥有函数能够检查是否拥有空闲空间并申请空间，malloc非常简单。如果全局头指针为 `NULL`，我们需要分配空间并将头指针指向新分配的空间。如果头指针非空，我们需要查看能否重用任何已存在的空间。如果能，那么就重用；如果不能，那么我们分配空间并使用新分配的空间。

C
```
void *malloc(size_t size) {
  struct block_meta *block;
  // TODO: align size?
  if (size <= 0) {
    return NULL;
  }
  if (!global_base) { // First call.
    block = request_space(NULL, size);
    if (!block) {
      return NULL;
    }
    global_base = block;
  } else {
    struct block_meta *last = global_base;
    block = find_free_block(&last, size);
    if (!block) { // Failed to find free block.
      block = request_space(last, size);
      if (!block) {
        return NULL;
      }
    } else {      // Found free block
      // TODO: consider splitting block here.
      block->free = 0;
      block->magic = 0x77777777;
    }
  }
  return(block+1);
}
```
对于那些不熟悉C的人，我们返回block+1，因为我们想在block_meta结构之后返回一个指向该区域的指针。因为block是指向 `struct block_meta`类型的指针，所以`+1` 会将指针的地址向后增加 `sizeof(struct(block_meta))` 个字节。
如果我们仅仅要一个malloc，不需要free，我们可以使用原来的malloc函数，更简单。所以我们来编写free函数！free函数主要是要设置 `->free`参数。
因为在代码中，我们需要在很多地方获取结构体的地址，所以下面我们来定义这个函数。

C
```
struct block_meta *get_block_ptr(void *ptr) {
  return (struct block_meta*)ptr - 1;
}
```
既然实现了这个函数，下面给出free函数的实现：

C
```
void free(void *ptr) {
  if (!ptr) {
    return;
  }
  // TODO: consider merging blocks once splitting blocks is implemented.
  struct block_meta* block_ptr = get_block_ptr(ptr);
  assert(block_ptr->free == 0);
  assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
  block_ptr->free = 1;
  block_ptr->magic = 0x55555555;
}
```
除设置 `->free`参数外，调用free函数释放空指针是合法的，所以我们必须检查空指针。因为free函数不应该被任意已释放的地址或内存块调用，所以我们可以断言这些情况永远不会发生。
你其实不必做出任何断言，但这通常会让调试变得更加容易。事实上，在编写代码时，我曾遇到过bug，如果没有断言这些bug将导致不明的数据崩溃。然而，代码在断言处出错，这使得调试更加细致。
既然已实现malloc和free函数，我们可以使用我们编写的内存分配函数来编写程序！但在将我们的分配函数添加到已有代码前，需要实现一些更加常用的函数，realloc和calloc。Calloc仅仅是在malloc之后将分配的内存初始化为0，所以我们首先来看看realloc。Realloc应该可以调整使用malloc，calloc以及realloc分配的内存块大小。
Realloc的函数原型如下：

C
```
void *realloc(void *ptr, size_t size)
```
如果传递一个空指针给realloc，它应该向malloc一样工作。如果传入一个已分配指针，如果空间小于已分配大小应释放当前空间，如果空间大于已分配大小应分配更大空间并复制已存在的数据。
当空间减小时，如果我们不调整空间大小，不释放任何空间，一切都将正常工作，但当空间增大时，我们必须分配更多空间，所以下面我们来实现这个功能。

C
```
void *realloc(void *ptr, size_t size) {
  if (!ptr) {
    // NULL ptr. realloc should act like malloc.
    return malloc(size);
  }
  struct block_meta* block_ptr = get_block_ptr(ptr);
  if (block_ptr->size >= size) {
    // We have enough space. Could free some once we implement split.
    return ptr;
  }
  // Need to really realloc. Malloc new space and free old space.
  // Then copy old data to new space.
  void *new_ptr;
  new_ptr = malloc(size);
  if (!new_ptr) {
    return NULL; // TODO: set errno on failure.
  }
  memcpy(new_ptr, ptr, block_ptr->size);
  free(ptr);
  return new_ptr;
}
```
至于calloc，该函数仅仅是在指针返回之前清空内存。

C
```
void *calloc(size_t nelem, size_t elsize) {
  size_t size = nelem * elsize;
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}
```
在linux下新分配页空间（不是重用的空闲块）可以删除memset，因为linux保证新分配的内存全部初始化为0，至少到目前为止是这样的。
好了，我们现在的代码足以用到既有的程序（而且我们甚至不需要重新编译）！
首先，我们需要编译代码。linux下使用如下命令编译：

C
```
clang -O0 -g -W -Wall -Wextra -shared -fPIC malloc.c -o malloc.so
```
应该能够工作。
``-g` 添加调试标识，这样我们可以使用 `gdb` 或 `lldb`查看代码。`-O0`可以防止局部变量被优化，便于调试。 `-W -Wall -Wextra` 添加额外警告。 `-shared -fPIC` 允许代码动态链接，这样我们可以在代码中使用[已存在的二进制代码库](http://jvns.ca/blog/2014/11/27/ld-preload-is-super-fun-and-easy/)！`
在macs下，我们使用如下编译命令：

C
```
clang -O0 -g -W -Wall -Wextra -dynamiclib malloc.c -o malloc.dylib
```
注意最新版本的OS X上sbrk已被废弃。苹果对已废弃的函数使用非正统定义——一些废弃的系统调用被严重破坏。我并没有在Mac上进行测试，所以这可能会在mac引起奇怪的错误或者不能工作。
现在，为了在linux下调用二进制库来使用我们的malloc函数，我们需要设置`LD_PRELOAD` 环境变量。如果你正在使用bash，你可以这样做：

C
```
export LD_PRELOAD=/absolute/path/here/malloc.so
```
如果你使用的是mac，你可以这样做：

C
```
export DYLD_INSERT_LIBRARIES=/absolute/path/here/malloc.so
```
如果一切正常，你可以运行一些任意二进制文件，它能够正常工作（除了有点慢）。

C
```
$ ls
Makefile  malloc.c  malloc.so  README.md  test  test-0  test-1  test-2  test-3  test-4  wrapper  wrapper.c
```
如果有bug，你可能会看到如下信息：

C
```
$ ls
Segmentation fault (core dumped)
```
#### 调试
下面我们来谈谈调试！如果熟悉使用调试器断点的设置，内存查看以及单步调试，你可以跳过本节内容，直接阅读[练习部分](http://danluu.com/malloc-tutorial/#exercises)。
本节假设你知道如何在你的系统上安装gdb。如果你使用的是mac，你可能只需要使用lldb并合理转换指令。因为不知道你可能会遇到哪些bug，我将介绍一些bug并说明我是如何解决的。
首先，需要明白如何运行gdb而不遇到段错误。如果ls出错，我们运行 `gdb ls`，gdb几乎肯定也会出现段错误。
[Andrew Roth在github上有这方面用途的代码](https://github.com/ps2dude756/malloc)，所以我们直接使用：

C
```
int main(int argc, char **argv) {
  // Check that we have at least one arg.
  if (argc == 1) {
    printf("You must supply a program to be invoked to use your replacement malloc() script.n");
    printf("...you may use any program, even system programs, such as `ls`.n");
    printf("n");
    printf("Example: %s /bin/lsn", argv[0]);
    return 1;
  }
  /*
   * Set up the environment to pre-load our 'malloc.so' shared library, which
   * will replace the malloc(), calloc(), realloc(), and free() that is defined
   * by standard libc.
   */
  char **env = malloc(2 * sizeof(char *));
  env[0] = malloc(100 * sizeof(char));
  sprintf(env[0], "LD_PRELOAD=./malloc.so");
  env[1] = NULL;
  /*
   * Replace the current running process with the process specified by the command
   * line options.  If exec() fails, we won't even try and recover as there's likely
   * nothing we could really do; however, we do our best to provide useful output
   * with a call to perror().
   */
  execve(argv[1], argv + 1, env);    /* Note that exec() will not return on success. */
  perror("exec() failed");
  free(env[0]);
  free(env);
  return 2;
}
```
既然文件中设置了LD_PRELOAD宏，我们可以在gdb下运行该文件，这样gdb会使用标准malloc而该文件中的代码会调用我们有bug的malloc。我要首先介绍的bug是在free中没有检查`NULL` 指针。

C
```
$ ./wrapper /bin/ls  # note that we need the full path here
Segmentation fault (core dumped)
```
一旦我们启动了gdb，我们可以使用`run`向`wrapper`传递参数。

C
```
$ gdb wrapper
(gdb) run /bin/ls
Starting program: /home/dluu/dev/dump/malloc/wrapper /bin/ls
process 26604 is executing new program: /bin/ls
Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7bd7dbd in free (ptr=0x0) at malloc.c:113
113       assert(block_ptr->free == 0);
```
和预期的一样，我们遇到了段错误。我们可以使用 `list` 查看段错误附近的的代码。

C
```
(gdb) list
108     }
109
110     void free(void *ptr) {
111       // TODO: consider merging blocks once splitting blocks is implemented.
112       struct block_meta* block_ptr = get_block_ptr(ptr);
113       assert(block_ptr->free == 0);
114       assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
115       block_ptr->free = 1;
116       block_ptr->magic = 0x55555555;  
117     }
```
然后我们可以使用参数`p` （用于打印信息）来查看这里的变量发生了什么变化：

C
```
(gdb) p ptr
$6 = (void *) 0x0
(gdb) p block_ptr
$7 = (struct block_meta *) 0xffffffffffffffe8
```
``ptr` 值为 `0`，即为 `NULL`，这是导致这个问题的原因：我们忘记检查空指针。`
既然弄明白原因，我们要尝试稍微难一点的bug。假设我们决定用以下结构体替换我们的结构体：

C
```
struct block_meta {
  size_t size;
  struct block_meta *next;
  int free;
  int magic;    // For debugging only. TODO: remove this in non-debug mode.
  char data[1];
};
```
然后malloc会返回 `block->data` 而不是 `block+1` ，其余保持不变。这和我们已经做的极为相似——我们只需在结构体末尾定义一个成员，并返回一个指向该变量的指针。
但如果我们尝试使用新的malloc函数，会出现下面的问题：

C
```
$ ./wrapper /bin/ls
Segmentation fault (core dumped)
gdb wrapper
(gdb) run /bin/ls
Starting program: /home/dluu/dev/dump/malloc/wrapper /bin/ls
process 26633 is executing new program: /bin/ls
Program received signal SIGSEGV, Segmentation fault.
_IO_vfprintf_internal (s=s@entry=0x7fffff7ff5f0, format=format@entry=0x7ffff7567370 "%s%s%s:%u: %s%sAssertion `%s' failed.n%n", ap=ap@entry=0x7fffff7ff718) at vfprintf.c:1332
1332    vfprintf.c: No such file or directory.
1327    in vfprintf.c
```
这并不像上一个错误一样简单——我们可以看到其中一个断言失败，但断言失败时gdb会抛出一些调用的print函数。但print函数也使用了有bug的malloc并且失败！
这里我们可以查看 `ap` 的值来弄清楚 `assert` 要打印什么信息：

C
```
(gdb) p *ap
$4 = {gp_offset = 16, fp_offset = 48, overflow_arg_area = 0x7fffff7ff7f0, reg_save_area = 0x7fffff7ff730}
```
这样就可以了；我们可以溜达一会直到我们弄明白哪些信息应该打印输出以及为什么会失败。其他的一些解决方案是编写自定义断言或使用挂钩避免断言使用我们的malloc。
但在这种情况下，我们知道代码中有一些断言。malloc函数中的一个断言检查我们没有在多线程程序中使用该函数，free中的两个断言检查我们没有释放不该释放的。我们首先设置断点查看free函数。

C
```
$ gdb wrapper
(gdb) break free
Breakpoint 1 at 0x400530
(gdb) run /bin/ls
Starting program: /home/dluu/dev/dump/malloc/wrapper /bin/ls
process 26700 is executing new program: /bin/ls
Breakpoint 1, free (ptr=0x61c270) at malloc.c:112
112       if (!ptr) {
```
``block_ptr` 尚未被置位，但是如果我们使用几次​ `s`进行单步调试直到它被置位，我们可以看到它的具体值：`

C
```
(gdb) s
(gdb) s
(gdb) s
free (ptr=0x61c270) at malloc.c:118
118       assert(block_ptr->free == 0);
(gdb) p/x *block_ptr
$11 = {size = 0, next = 0x78, free = 0, magic = 0, data = ""}
```
我使用 `p/x` 参数替代 `p` 参数，这样可以查看十六进制形式。 `magic` 域值为0，对于我们要释放的有效结构体来说是不可能的。也许 `get_block_ptr` 会返回一个错误的偏移量？我们可以查看 `ptr` 的值，所以我们能够查看不同偏移量。因为它是`void *`类型，我们必须进行类型转换，这样gdb才知道如何计算结果。

C
```
(gdb) p sizeof(struct block_meta)
$12 = 32
(gdb) p/x *(struct block_meta*)(ptr-32)
$13 = {size = 0x0, next = 0x78, free = 0x0, magic = 0x0, data = {0x0}}
(gdb) p/x *(struct block_meta*)(ptr-28)
$14 = {size = 0x7800000000, next = 0x0, free = 0x0, magic = 0x0, data = {0x78}}
(gdb) p/x *(struct block_meta*)(ptr-24)
$15 = {size = 0x78, next = 0x0, free = 0x0, magic = 0x12345678, data = {0x6e}}
```
如果从我们使用的地址往后退一点，我们可以看到正确的偏移量是24而不是32。这是因为结构体后有附加数据，所以 `sizeof(struct block_meta)` ​的值为32，尽管最终有效的成员偏移为24。如果想要去掉附加空间，我们必须修改 `get_block_ptr`。
以上是调试的内容！
#### 练习
就个人而言，我从未遇到这种问题直到做了一些练习，所以这里为感兴趣的任何人提供一些练习。
- malloc应该返回一个指针，该指针能够和任意内建类型对齐。我们的malloc函数是这样吗？如果是，为什么？如果没有，修改对齐。注意C中“任意内建类型”基本上达到8个字节，因为SSE/AVX类型不是内建类型。
- 如果尝试重新使用一块已有空间但又不需要整块空间，我们的malloc函数很低效。实现一个能分割内存块的函数，这样就能够使用所需的最小空间。
- 在完成 `2`后，如果我们频繁调用malloc和free分配释放任意空间大小，最终会产生大量小内存块，这些内存块只有在我们分配少量空间时才能被重用。实现一种合并相邻空闲内存的机制，这样任意连续的空闲内存块可以合并成一个完整的内存空间。
- 在现有代码中查找bug！我并没有过多测试，所以我肯定这里有很多bug，尽管基本上八九不离十。
#### Parts 2-N
接下来，我们要弄明白如何提高速度并实现线程安全。
#### 资源
在坐下来编写自己的实现之前，我阅读了Marwan Burelle的[教程](http://www.inf.udec.cl/~leo/Malloc_tutorial.pdf)，所以实现非常相似。实现的主要不同之处在与我的版本更加简单，但更容易产生内存碎片。在解释方面，那个不同风格作者​的教程可能会更加适合你。
更多关于Linux如何进行内存管理的内容，可以阅读Gustavo Duarte的[文章](http://duartes.org/gustavo/blog/post/anatomy-of-a-program-in-memory/)。
更多关于实际使用malloc函数实现的工作原理，[dlmalloc](http://g.oswego.edu/dl/html/malloc.html)和[tcmalloc](http://goog-perftools.sourceforge.net/doc/tcmalloc.html)值得阅读。我没有阅读过[jemalloc](http://www.canonware.com/jemalloc/)的代码，据说有点难以理解，但它仍然是广泛使用的高性能malloc实现。
为方便调试，[Address Sanitizer](https://code.google.com/p/address-sanitizer/wiki/AddressSanitizer)功能强大。如果你想要编写一个线程安全的版本，[Thread Sanitizer](https://code.google.com/p/data-race-test/wiki/ThreadSanitizer)也是一个很棒的工具。
#### 致谢
非常感谢Gustavo Duarte提供的sbrk函数说明图，Ian Whitlock和Danielle Sucher找出文中的拼写错误，以及Nathan Kurz建议的额外资源。如果你发现本文中的其他问题，请告诉我（无论是写作或者代码）。
