# linux--函数劫持--基于LD_PRELOAD - xqhrs232的专栏 - CSDN博客
2016年10月20日 18:40:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：524
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://m.2cto.com/os/201406/306008.html](http://m.2cto.com/os/201406/306008.html)
相关文章
1、Analyzing Jynx and LD_PRELOAD Based Rootkits----[http://www.infosecisland.com/blogview/22440-Analyzing-Jynx-and-LDPRELOAD-Based-Rootkits.html](http://www.infosecisland.com/blogview/22440-Analyzing-Jynx-and-LDPRELOAD-Based-Rootkits.html)
2、The magic of LD_PRELOAD for Userland Rootkits----http://fluxius.handgrep.se/2011/10/31/the-magic-of-ld_preload-for-userland-rootkits/
3、JynxKit is an LD_PRELOAD userland rootkit for Linux systems with reverse connection SSL backdoor----[https://github.com/chokepoint/jynxkit](https://github.com/chokepoint/jynxkit)

Recently i am facing a problem, how to differentiate a problem of library-function from application problems for solving this problem, we need to know some
Recently i am facing a problem, how to differentiate a problem of library-function from application problems.for solving this problem, we need to know some knowledge about share-library and basics in [Linux](http://m.2cto.com/os/linux/).For
 dynamic libraries, they are loaded to memory at program running. There are many benefits for this.On the other hand, we have a chance to do something. For example: replace the function, which will be loaded,as our function.Usually,this is maybe a very professional
 problem, at least for me.But thanks for some masters, they have been provide a mature way to our goal.
1). How to replace a function of shared libraries?
the answer is LD_PRELOAD, this is a environment variable for GUN-Linker.It is used to indicated some pre-load shared libraries.This meaning that functions in
this libraries will get a higher priority than normal libraries.
Normally, we use this technique just want to intercept some functions.So we can do some other thing( evil thing?)
above the original function.But 
1). How can i get the original function?
some functions: dlopen(), dlsym(), dlclose() and dlerror().There have been many articles to explain them.
In the next section, I will try to translate a nice article. The original article is there
http://fluxius.handgrep.se/2011/10/31/the-magic-of-ld_preload-for-userland-rootkits/
这篇文章的目录如下:
一.共享库
二.简单的LD_PRELOAD(相对于后面的被加工过的LD_PRELOAD)
2.1. 制作和使用的共享库
2.2. dlsym()
2.3. 使用限制
三.相关的隐匿技术
3.1 Jynx-Kit
3.2 检查方法
下面是正文：
一.共享库
正如我们所知道的，动态库的链接是在程序加载时实现.在我的电脑上这个功能是由ld-linux-x86-64.so.X实现，但对于其他机型也可能是ld-linux.so.X。如果有兴趣，可以这样验证
```java
fluxiux@handgrep:~$ readelf -l /bin/ls  [...] INTERP 0x0000000000000248 0x00000000004purposes00248 0x0000000000400248  0x000000000000001c 0x000000000000001c R 1  [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]  [...]
```
（PS: 这里的readelf 是一种辅助[阅读](http://book.2cto.com/)elf文件的工具，对于需要动态加载的程序文件一般会指定一个动态加载工具。这涉及到elf格式的解析，在下一篇文章中会尝试对elf格式进行一些简单分析。）
相对于静态编译庞大的体积而言，动态编译就小多了。对于其中的部分库函数只保留了一个指向相关库的指针，并没有包含函数的实体。如果要查看一个程序包含了那些库的调用，可以用"ldd"命令检查，例如:
```java
<a target=_blank href="mailto:fluxiux@handgrep:~$">fluxiux@handgrep:~$</a> ldd /bin/ls  linux-vdso.so.1 => (0x00007fff0bb9a000)  libselinux.so.1 => /lib/x86_64-linux-gnu/libselinux.so.1 (0x00007f7842edc000)  librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f7842cd4000)  libacl.so.1 => /lib/x86_64-linux-gnu/libacl.so.1 (0x00007f7842acb000)  libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f7842737000)  libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f7842533000)  /lib64/ld-linux-x86-64.so.2 (0x00007f7843121000)  libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f7842314000)  libattr.so.1 => /lib/x86_64-linux-gnu/libattr.so.1 (0x00007f784210f000)
```
我们看一个例子，（例子程序如下）
```java
#include <stdio.h>  main()  {  printf("huhu la charrue");  }
```
分别编译成动态和静态两种
```java
fluxiux@handgrep:~$ gcc toto.c -o toto-dyn  fluxiux@handgrep:~$ gcc -static toto.c -o toto-stat  fluxiux@handgrep:~$ ls -l | grep "toto-"  -rwxr-xr-x 1 fluxiux fluxiux 8426 2011-10-28 23:21 toto-dyn  -rwxr-xr-x 1 fluxiux fluxiux 804327 2011-10-28 23:21 toto-stat
```
我们看到"toto-stat"几乎是"toto-dyn"的96倍，这是为什么呢?
```java
fluxiux@handgrep:~$ ldd toto-stat  is not a dynamic executable
```
(因为"toto-stat"是静态的呗)
动态链接是一种非常棒的方法，给了我们很多好处，例如:
■update libraries and still support programs that want to use older, non-backward-compatible versions of those libraries,
■override specific libraries or even specific functions in a library when executing a particular program,
■do all this while programs are running using existing libraries.
对于共享库的命名，我们有一些习惯。如果一个库的名字是"soname"，那么通常会加一个前缀"lib",一个后缀".so",以及一个版本号。
(PS:虽然说只是约定:但你不这么做的话，你的库是不能被正确识别出来的)
现在我们可以来看一下LD_PRELOAD了，
二.简单的LD_PRELOAD应用
我们知道，库文件一般存放在"/lib"目录下。所以如果想修改一个库，最容易想到的办法就是找到该库的[源码](http://m.2cto.com/ym)，修改之后再重新编译一遍。但除了这种方案，我们还有另一种很酷的方法，那就是使用Linux提供给我们的一个外部接口:LD_PRELOAD。（先看看下面这个吧）
2.1. 制作和使用的共享库
假如你希望重写"printf"的行为，可以这样做先编写一个你的"printf"函数
```java
#define _GNU_SOURCE #include <stdio.h>  int printf(const char *format, ...) { exit(153); }
```
然后编译把它编译成一个共享库。像这样，
```java
fluxiux@handgrep:~$ gcc -Wall -fPIC -c -o my_printf.o my_printf.c  my_printf.c: In function ‘printf’: my_printf.c:6:2: warning: implicit declaration of function ‘exit’ my_printf.c:6:2: warning: incompatible implicit declaration of built-in function ‘exit’ fluxiux@handgrep:~$ gcc -shared -fPIC -Wl,-soname -Wl,libmy_printf.so -o libmy_printf.so my_printf.o
```
然后我们修改一个环境变量后再运行自己的测试程序
```java
fluxiux@handgrep:~$ export LD_PRELOAD=$PWD/libmy_printf.so fluxiux@handgrep:~$ ./toto-dyn
```
你会看到"printf"的行为已经被改变了，它并没有打印"huhu la charrue"。好吧，让我们看看"ltrace"是怎么说的
```java
fluxiux@handgrep:~$ ltrace ./toto-dyn  __libc_start_main(0x4015f4, 1, 0x7fffa88d0908, 0x402530, 0x4025c0 <unfinished ...> printf("huhu la charrue" <unfinished ...> +++ exited (status 153) +++
```
有趣吧，我们的"printf"在[系统](http://m.2cto.com/os/)的"printf"之前被调用了。那么现在来看一个新的问题，如果我们的目标仅仅是简单的修改一下"printf"的行为，但不破坏原有的功能。那要怎么办呢，重写整个函数?！！
 那样明显是不合适的，为了处理这个问题，可以看下面的几个函数。
2.2. dlsym()
在库"libdl"中有几个有趣的函数
dlopen() : 加载一个库
dlsym() : 获取一个特定符号的指针
dlclose() : 卸载一个库
在这里，因为库已经在程序装载时就加载好了，我们只需要直接调用"dlsym"就可以了。我们给"dlsym"传"RTLD_NEXT"参数，用来获取指向原有的"printf"函数的指针。就像这样
```java
[...] typeof(printf) *old_printf; [...]	 //DO HERE SOMETHING VERY EVIL old_printf = dlsym(RTLD_NEXT, "printf"); [...]
```
然后我们需要对格式化字符串进行一点特殊处理(对应一般的参数是不需要这么麻烦的)，处理完了之后就可以直接用了
```java
#include <stdio.h> #include <dlfcn.h> #include <stdlib.h> #include <stdarg.h>  int printf(const char *format, ...) { va_list list; char *parg; typeof(printf) *old_printf;  // format variable arguments va_start(list, format); v[asp](http://m.2cto.com/kf/web/asp/)rintf(&parg, format, list); va_end(list);  //DO HERE SOMETHING VERY EVIL  // get a pointer to the function "printf" old_printf = dlsym(RTLD_NEXT, "printf"); (*old_printf)("%s", parg); // and we call the function with previous arguments  free(parg); }
```
再重新编译一下
```java
fluxiux@handgrep:~$ gcc -Wall -fPIC -c -o my_printf.o my_printf.c  my_printf.c: In function ‘printf’: my_printf.c:21:1: warning: control reaches end of non-void function fluxiux@handgrep:~$ gcc -shared -fPIC -Wl,-soname -Wl libmy_printf.so -ldl -o libmy_printf.so my_printf.o
```
再试一次
```java
fluxiux@handgrep:~$ export LD_PRELOAD=$PWD/libmy_printf.so fluxiux@handgrep:~$ ./toto-dyn  huhu la charrue
```
就这样一个调用"printf"的用户不希望的事情就悄悄的发生了。但这种机制也有一些缺陷。
2.3. 使用限制
这种方式虽然很酷，但却有一些限制。比如对于静态编译的程序是无效的。因为静态编译的程序不需要连接动态库的面的函数。而且，假如文件的SUID或SGID位被置1，加载的时候会忽略LD_PRELOAD(这是ld的开发者出于安全考虑做的)。
三.相关的隐匿技术
3.1 Jynx-Kit
大约在两个周以前，我们介绍了一种新的隐匿技术。这玩意利用了一个自动运行的shell脚本，并且一直没有被rkhunter 和chkrootkit检查到。先让我们来看看实际的代码，在"ld_poison.c"中，有14个函数被劫持了
```java
[...] old_fxstat = dlsym(RTLD_NEXT, "__fxstat"); old_fxstat64 = dlsym(RTLD_NEXT, "__fxstat64"); old_lxstat = dlsym(RTLD_NEXT, "__lxstat"); old_lxstat64 = dlsym(RTLD_NEXT, "__lxstat64"); old_open = dlsym(RTLD_NEXT,"open"); old_rmdir = dlsym(RTLD_NEXT,"rmdir"); old_unlink = dlsym(RTLD_NEXT,"unlink");  old_unlinkat = dlsym(RTLD_NEXT,"unlinkat"); old_xstat = dlsym(RTLD_NEXT, "__xstat"); old_xstat64 = dlsym(RTLD_NEXT, "__xstat64"); old_fdopendir = dlsym(RTLD_NEXT, "fdopendir"); old_opendir = dlsym(RTLD_NEXT, "opendir"); old_readdir = dlsym(RTLD_NEXT, "readdir"); old_readdir64 = dlsym(RTLD_NEXT, "readdir64"); [...]
```
我们随便调一个"open"函数来看看，在内部看到"__xstat"被调用了
```java
[...] struct stat s_fstat; [...] old_xstat(_STAT_VER, pathname, &s_fstat); [...]
```
接下来是一个检查操作，检查了文件的组ID，路径，以及文件名。(我们要确保这个文件不是"ld.so.preload",因为我们想隐藏它)。如果是我们想隐藏的文件，就不将结果返回给用户
```java
[...] if(s_fstat.st_gid == MAGIC_GID || (strstr(pathname, MAGIC_DIR) != NULL) || (strstr(pathname, CONFIG_FILE) != NULL)) { errno = ENOENT; return -1; } [...]
```
按照这种思路将上面的函数都处理一遍后，我们就可以向用户(被攻击者)隐藏一些文件以及行为。但我们有什么方法可以用来检查吗?继续看下面
3.2 检查隐匿
不论这种隐匿方法是不是足够让你眼前一亮，但它确实把rkhunter 和chkrootkit干掉了。因为那两种工具使用的是基于符号的检查方式，这种方式确实不是最好的方法。
来看下面这个例子：
首先，如果我们在清空LD_PRELOAD变量后，对一个指定程序文件产生一个校验文件.就像这样
```java
fluxiux@handgrep:~$ sha1sum toto-dyn  a659c72ea5d29c9a6406f88f0ad2c1a5729b4cfa toto-dyn fluxiux@handgrep:~$ sha1sum toto-dyn > toto-dyn.sha1
```
然后再在设置了LD_PRELOAD后检查这个文件的校验和，就像这样
```java
fluxiux@handgrep:~$ export LD_PRELOAD=$PWD/libmy_printf.so fluxiux@handgrep:~$ sha1sum -c toto-dyn.sha1  toto-dyn: OK
```
(貌似通过了对这个文件的身份验证)
但这样真的是正确的吗?
很显然不是，因为我们实际上没有修改过程序文件，所以随便什么时候文件的校验和都会是相同的。如果反隐匿工具是基于校验和工作，那么无疑这是不行的。其他的检查技术还有:检查可疑文件，符号，port binding detection等，但也失败了，因为这种隐匿方法太灵活了，and in Jynx we have a sort of port knocking to open the remote shell for our host.
好吧，对于这些东西我们还能做什么呢?检查LD_PRELOAD指向的所有库，或"/etc/ld.so.preload"吧。我们知道"dlsym"经常被用来查找原始函数，所有可以这样
```java
$ strace ./bin/ls [...] open("/home/fluxiux/blabla/Jynx-Kit/ld_poison.so", O_RDONLY) = 3 read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\240\n\0\0\0\0\0\0"..., 832) = 832 fstat(3, {st_mode=S_IFREG|0755, st_size=17641, ...}) = 0 mmap(NULL, 2109656, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f5e1a586000 mprotect(0x7f5e1a589000, 2093056, PROT_NONE) = 0 mmap(0x7f5e1a788000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7f5e1a788000 close(3)  [...] open("/lib/x86_64-linux-gnu/libdl.so.2", O_RDONLY) = 3 [...]
```
解析其中的"ld_poison.so"文件，在里面看到了很多的替换，它们都是有可能藏污纳垢的地方。看看这些二进制文件的字符串信息，都很有可能给我们提供一些有趣的暗示。当然如果它们被巧妙的处理过了，那就没办法了。但再换个角度来思考，那个正常的程序文件需要去掩饰自己的这些字符串信息呢。
```java
fluxiux@handgrep:~/blabla/Jynx-Kit$ strings ld_poison.so [...] libdl.so.2 [...] dlsym fstat [...] lstat hooked. ld.so.preload xochi <-- sounds familiar [...] /proc/%s <-- hmmm... strange! [...]
```
这种叫做Jynx-kit的隐匿方法证明了，想通过基于符号的检查来应付上文的隐匿方法是不太现实的。而启发式的检查效果会不错。
原文在这里：http://fluxius.handgrep.se/2011/10/31/the-magic-of-ld_preload-for-userland-rootkits/
