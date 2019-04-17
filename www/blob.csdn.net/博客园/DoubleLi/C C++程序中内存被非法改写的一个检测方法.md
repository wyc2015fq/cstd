# C/C++程序中内存被非法改写的一个检测方法 - DoubleLi - 博客园






本文所讨论的“内存”主要指(静态)数据区、堆区和栈区空间(详细的布局和描述参考《[Linux虚拟地址空间布局](http://www.cnblogs.com/clover-toeic/p/3754433.html)》一文)。数据区内存在程序编译时分配，该内存的生存期为程序的整个运行期间，如全局变量和static关键字所声明的静态变量。函数执行时在栈上开辟局部自动变量的储存空间，执行结束时自动释放栈区内存。堆区内存亦称动态内存，由程序在运行时调用malloc/calloc/realloc等库函数申请，并由使用者显式地调用free库函数释放。堆内存比栈内存分配容量更大，生存期由使用者决定，故非常灵活。然而，堆内存使用时很容易出现内存泄露、内存越界和重复释放等严重问题。

     数据区的内存访问越界可以分为读越界和写越界，数据区内存越界主要指读写某一数据区内存(如全局或静态变量、数组或结构体等)时，超出该内存区域的合法范围。读越界表示读取不属于自己的数据，如读取的字节数多于分配给目标变量的字节数。若所读的内存地址无效，则程序立即崩溃；若所读的内存地址有效，则可读到随机的数据，导致不可预料的后果。写越界亦称“缓冲区溢出”，所写入的数据对目标地址而言也是随机的，因此同样导致不可预料的后果。

     内存越界访问会严重影响程序的稳定性，其危险在于后果和症状的随机性。这种随机性使得故障现象和本源看似无关，给排障带来极大的困难。你永远也不知道是不是有其他线程操作时候偷偷改动了你的数据。如果是一般的业务数据，唔，一个bug。但是是如果该内存块指向一个对象，然后就呵呵了——你持有了一个无效的内存地址，一般来说会crash，无止境的debug在等待你。

     写越界的主要原因有两种：1) memset/memcpy/memmove等内存覆写调用；2) 数组下标超出范围。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)


#include <string.h>
#include <stdio.h>



#define NAME_SIZE 8
#define NAME_LEN  9



char name1[NAME_SIZE] = "ABCDEFGH";
char name2[NAME_LEN]  = "123456789";



int main() {
  strncpy(name1, name2, NAME_LEN);
  printf("name2: %s\n", name2);

  return 0;
}


![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

     输出结果显然是name2: 923456789。常见的所谓数组越界方法实现起来比较繁琐。用工具（VALGRIND等）可以发现，但是对于生产系统（采用了全局数组+多线程之类的高级技巧……），一般来说是难以查找到的，特别是如果其他线程由其他团队成员开发，你对其代码缺少相关知识的时候。

      对于这个问题，gdb提供了一种可能的方法：观察点（watch命令）。用法如下：watch name2[0]。这样当该变量被改写的时候进程将会停下来。当然你也可以watch某个地址：watch *(data type*)addr。如果你怀疑是特定线程改写了该变量的时候，可以使用watch expr thread threadnum，在某个线程改写的时候让进程停止。使用这个方法，在绝大多数情况下可以发现未知的变量改写问题。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

(gdb) watch name2[0]
Hardware watchpoint 1: name2[0]
(gdb) r
Starting program: /home/afreet/sourcecodes/memdemo/build/bin/memdemo 
Hardware watchpoint 1: name2[0]

Old value = 49 '1'
New value = 57 '9'
__strncpy_ssse3 () at ../sysdeps/x86_64/multiarch/strcpy-ssse3.S:2443
2443    ../sysdeps/x86_64/multiarch/strcpy-ssse3.S: No such file or directory.
(gdb) bt
#0  __strncpy_ssse3 () at ../sysdeps/x86_64/multiarch/strcpy-ssse3.S:2443
#1  0x000000000040080e in main () at /home/afreet/sourcecodes/memdemo/memdemo.c:11

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

      如果在调试状态下运行仍然没有发现问题或者是嵌入式环境根本无法调试，那么是不是就只能去烧香？或者拜基督（取决于你的宗教信仰，但是财神我相信大多数现代中国人是不会拒绝去拜拜的）。Linux还提供了一个杀手锏级的API：mprotect。

mprotect函数的原型如下： 

**int mprotect(const void *addr, size_t len, int prot);**

      其中addr是待保护的内存首地址，必须按页对齐；len是待保护内存的大小，必须是页的整数倍，prot代表模式，可能的取值有PROT_READ（表示可读）、PROT_WRITE（可写）等。

      不同体系结构和操作系统，一页的大小不尽相同。如何获得页大小呢？通过PAGE_SIZE宏或者getpagesize()系统调用即可。下面是另一个简单的例子：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <thread>

#define BUF_LEN 4096

using namespace std;

int buf[BUF_LEN] = {0};
int* p = &buf[2048];

void func1() { 
  char* q = reinterpret_cast<char*>(p);
  *q = 0xFF;
}

void func2() {
  sleep(5);
  
  for ( auto x: buf) {
    assert(x == 0);
  }
}

int main() {
   std::thread t1(func1);
   std::thread t2(func2);
   t1.join();
   t2.join();

   return 0;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

由于buf[2048]在func1中被改写，所以断言会失败。因此引入mprotect函数，对问题所在进行检测。改进后的版本如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/mman.h>

#include <thread>

#define BUF_LEN 1024

using namespace std;

int buf[BUF_LEN] = {0};
int* p = &buf[512];

void func2() {
  char* q = reinterpret_cast<char*>(p);
  *q = 0xFF;
}

void func1() {
  long pageSize = sysconf(_SC_PAGESIZE);
  void *pageStart = (void*)((long)p - (long)p % pageSize);

  int rst = mprotect(pageStart, pageSize, PROT_READ);
  if ( rst == -1 )
    printf("mprotect failed: %s", strerror(errno));
  sleep(10);
}

int main() {
   std::thread t1(func1);
   std::thread t2(func2);
   t1.join();
   t2.join();

   return 0;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

然后再来测试一下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

(gdb) r
Starting program: /home/afreet/sourcecodes/memdemo/build/bin/memdemo 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[New Thread 0x7ffff6ff1700 (LWP 2794)]
[New Thread 0x7ffff67f0700 (LWP 2795)]
[Thread 0x7ffff67f0700 (LWP 2795) exited]

Program received signal SIGSEGV, Segmentation fault.
[Switching to Thread 0x7ffff6ff1700 (LWP 2794)]
_dl_fixup (l=<optimized out>, reloc_arg=<optimized out>) at ../elf/dl-runtime.c:148
148     ../elf/dl-runtime.c: No such file or directory.
(gdb) bt
#0  _dl_fixup (l=<optimized out>, reloc_arg=<optimized out>) at ../elf/dl-runtime.c:148
#1  0x00007ffff7df02e5 in _dl_runtime_resolve () at ../sysdeps/x86_64/dl-trampoline.S:45
#2 ** 0x000000000040740b in func1 () at /home/afreet/sourcecodes/memdemo/memdemo.cpp:33**
#3  0x0000000000408aac in void std::_Bind_simple<void (*())()>::_M_invoke<>(std::_Index_tuple<>) (this=0x60eda8)
    at /usr/include/c++/4.9/functional:1700
#4  0x00000000004089d2 in std::_Bind_simple<void (*())()>::operator()() (this=0x60eda8)
    at /usr/include/c++/4.9/functional:1688
#5  0x0000000000408939 in std::thread::_Impl<std::_Bind_simple<void (*())()> >::_M_run() (this=0x60ed90)
    at /usr/include/c++/4.9/thread:115
#6  0x00007ffff796a970 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#7  0x00007ffff7bc70a4 in start_thread (arg=0x7ffff6ff1700) at pthread_create.c:309
#8  0x00007ffff70da87d in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:111

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

注意调用栈#2，明确的指出了试图改写buf[256]的函数名。这样就可以轻松的找到犯罪分子早点下班吃饭了。

      但是还是没有彻底解决问题：如果被测代码无法使用调试器运行怎么办？另一个问题，mprotect需要保护整个页面，那么很多时候被保护的数据会和其他全局数据共存在一个页面上。如果其他线程访问了这个页面，一样会发生segment fault。显然这不是我们需要的结果。如果在被保护的数据之前人为加padding，让被改写的数组后退到某个页面起始处，那么越界访问往往就不会发生了——因为访问到了padding上，这样也无法重现错误。办法还是有的：利用信号处理函数，判定发生页面访问错误的地址是否是我们期望的某个元素所在，如果不是，那么什么都不做就可以了；如果是，那就打印调用栈到指定文件。用一个包装类来实现这个目的：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

class MemoryDetector
{
public:
  typedef void (*segv_handler) (int sig, siginfo_t *si, void *unused);

  static void init(const char *path)
  {
    register_handler(handler);
    fd_ = open(path, O_RDWR|O_CREAT, 777);
  }

  static int protect(void *ptr, int len)
  {
    address_ = reinterpret_cast<uint64_t>(ptr);
    len_ = len;
    uint64_t start_address = (address_ >> PAGE_SHIFT) << PAGE_SHIFT;
    return mprotect(reinterpret_cast<void *>(start_address), PAGE_SIZE, PROT_READ);
  }

  static int umprotect(void *ptr, int len)
  {
    uint64_t addr = reinterpret_cast<uint64_t>(ptr);
    uint64_t start_address = (addr >> PAGE_SHIFT) << PAGE_SHIFT;
    return mprotect(reinterpret_cast<void *>(start_address), PAGE_SIZE, PROT_READ | PROT_WRITE);
  }

  static int umprotect()
  {
    uint64_t start_address = (address_ >> PAGE_SHIFT) << PAGE_SHIFT;
    return mprotect(reinterpret_cast<void *>(start_address), PAGE_SIZE, PROT_READ | PROT_WRITE);
  }

  static void finish()
  {
    close(fd_);
  }

private:
  static void register_handler(segv_handler sh)
  {
    struct sigaction act;
    act.sa_sigaction = sh;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    if(sigaction(SIGSEGV, &act, NULL) == -1){
      perror("Register hanlder failed");
      exit(EXIT_FAILURE);
    }
  }

  static void handler(int sig, siginfo_t *si, void *unused)
  {
    uint64_t address = reinterpret_cast<uint64_t>(si->si_addr);
    if (address >= address_ && address < address_ + len_) {
      umprotect(si->si_addr, PAGE_SIZE);
      my_backtrace();
    }
  }

  static void my_backtrace()
  {
    const int N = 100;
    void* array[100];
    int size = backtrace(array, N);
    backtrace_symbols_fd(array, size, fd_);
  }

  static uint64_t address_;
  static int len_;
  static int fd_;
};

uint64_t MemoryDetector::address_;
int MemoryDetector::len_;
int MemoryDetector::fd_;

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

随后我们把测试程序改成这个样子：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

void func() {
  char* q = reinterpret_cast<char*>(p);
  *q = static_cast<char>(0xFF); //Line 101
}

int main() {
  MemoryDetector::init("memdemo.rst");
  MemoryDetector::protect(p, 4);

  std::thread t(func);
  t.join();
  sleep(5);
  MemoryDetector::finish();

  return 0;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

再运行一把，得到了memdemo.rst文件，内容如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

./memdemo(_ZN14MemoryDetector12my_backtraceEv+0x2b)[0x407b79]
./memdemo(_ZN14MemoryDetector7handlerEiP9siginfo_tPv+0x64)[0x407b4c]
/lib/x86_64-linux-gnu/libpthread.so.0(+0xf8d0)[0x7fb039e928d0]
./memdemo(_Z4funcv+0x1c)[0x4076fc]
./memdemo(_ZNSt12_Bind_simpleIFPFvvEvEE9_M_invokeIIEEEvSt12_Index_tupleIIXspT_EEE+0x2a)[0x408fa4]
./memdemo(_ZNSt12_Bind_simpleIFPFvvEvEEclEv+0x22)[0x408eca]
./memdemo(_ZNSt6thread5_ImplISt12_Bind_simpleIFPFvvEvEEE6_M_runEv+0x21)[0x408e31]
/usr/lib/x86_64-linux-gnu/libstdc++.so.6(+0xb6970)[0x7fb039c2e970]
/lib/x86_64-linux-gnu/libpthread.so.0(+0x80a4)[0x7fb039e8b0a4]
/lib/x86_64-linux-gnu/libc.so.6(clone+0x6d)[0x7fb03939e87d]

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

接着addr2line命令看看：

addr2line -e memdemo 0x4076fc
/home/afreet/sourcecodes/memdemo/memdemo.cpp:101

也很轻松的找到了肇事者所在。









