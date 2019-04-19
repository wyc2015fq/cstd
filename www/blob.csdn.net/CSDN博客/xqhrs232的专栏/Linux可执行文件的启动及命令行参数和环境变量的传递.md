# Linux可执行文件的启动及命令行参数和环境变量的传递 - xqhrs232的专栏 - CSDN博客
2017年02月22日 17:32:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1306
原文地址::[http://blog.csdn.net/xiaojizhong183/article/details/8192129](http://blog.csdn.net/xiaojizhong183/article/details/8192129)
### 问题的由来
我们知道，普通的C程序可以从命令行上接收参数，也可以使用、设置SEHLL环境变量（getenv,setenv）,
**[plain]**[view
 plain](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)[copy](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)
- /*一个简单的C例子 test.c*/  
- 
- #include "stdio.h"  
- 
- extern char** environ;   /*C库定义的全局变量,环境变量字符串数组的起始地址*/  
- 
- int main(int argc, char* argv[])  
- 
- {  
- 
-     int i;  
- 
-     char** p = environ;  
- 
-     for(i=0; i<argc; i++)  
- 
-         printf("argv[%d]: %s\n", i, argv[i]);  
- 
-     printf("\n\nEnviroment Varibles\n\n");  
- 
-     while(*p != NULL)  
- 
-         printf("%s\n", *(p++));  
- 
-     return 0;  
- 
- }  
编译后在命令行上运行:
./test arg1 arg2.
可以将运行结果与env输出结果比较
执行普通的C程序时，因为有SHELL的存在，命令行参数和环境变量可以由SHELL传给C程序，这一点似乎容易理解。然而，对于[Linux](http://lib.csdn.net/base/linux)系统启动时运行的第一个用户程序init来说情况变得有些特别。init不是由用户在命令行上启动的（这时候根被还没有SHELL），而是由内核启动的，init程序里也使用了命令行参数和环境变量。
**[plain]**[view
 plain](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)[copy](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)
- /*Busybox-1.19.3/init/init.c*/  
- 
- int init_main(int argc UNUSED_PARAM, char **argv)  
- {  
- 
-     if (argv[1] && strcmp(argv[1], "-q") == 0) {  
- 
-        return kill(1, SIGHUP);  
- 
-     }  
- 
-     //……  
- 
-    console_init();  
- 
-    //……  
- 
- }  
- 
- static void console_init(void)  
- 
- {  
-     //.......  
-     s = getenv("CONSOLE");  
-     s = getenv("TERM");  
-    //......  
-  }  
从上面的代码片段里可以看到init程序和普通应用程序一样，也使用到了命令行参数和环境变量，既然这个时候还没有SHELL，那么init使用的命令行参数和环境变量保存在何处、从何而来？
### 保存在何处
Linux采用了虚拟内存技术，进程里使用的均是虚拟地址，内核通过为进程建立不同的页表，可以将两个相互独立的进程的相同的虚拟地址映射到不同的物理地址上，因此，Linux下所有可执行文件运行时在地址空间里的映像布局结构是一样的，这简化了系统设计。
下图是C程序执行时，在进程的用户空间里的映像布局分布（没有画出C库及动态连接器映射的部分）。
![](https://img-my.csdn.net/uploads/201211/16/1353067031_2042.png)
程序
**[plain]**[view
 plain](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)[copy](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)
- #include <stdio.h>  
- #include <stdlib.h>  
- #include <unistd.h>  
- 
- extern char** environ;  
- int in_data_segment = 2;  
- int in_bss_segment;  
- 
-  int main(int argc, char* argv[])  
- {  
- 
-     char cmd[32];  
-     char line[1024];  
-     FILE *fp;  
- 
-     printf("addr of argv = %x\n\n", (void*)argv);  
-     printf("addr of environ = %x\n", (void*)environ);  
-     printf("addr of in_data_segment=%x\n", (void*)&in_data_segment);  
-     printf("addr of in_bss_segment=%x\n\n", (void*)&in_bss_segment);  
- 
-     if ((fp = fopen("/proc/self/maps","r"))==NULL)  
-     {  
-          printf("open /proc/self/maps error\n");  
- 
-          return 0;  
-     }  
- 
-     while(fgets(line, 1024, fp) != NULL)  
-        fputs(line, stdout);  
- 
-      return 0;  
- }  
程序运行结果
addr of argv = bfccb4f4
addr of environ = bfccb4fc
addr of in_data_segment = 80498e8
addr of in_bss_segment = 804992c
08048000-08049000 r-xp 00000000 75:00 106839     /home/temp/memlayout          /*text段*/
08049000-0804a000 rw-p 00000000 75:00 106839     /home/temp/memlayout        /*text、data、bss段*/
0804a000-0806b000 rw-p 0804a000 00:00 0               [heap]
b7f2f000-b809d000 r-xp 00000000 75:00 204907       /lib/libc-2.9.so
b809d000-b809f000 r--p 0016e000 75:00 204907     /lib/libc-2.9.so
b809f000-b80a0000 rw-p 00170000 75:00 204907     /lib/libc-2.9.so
b80a0000-b80a4000 rw-p b80a0000 00:00 0 
b80a9000-b80ab000 rw-p b80a9000 00:00 0 
b80ab000-b80cb000 r-xp 00000000 75:00 204900     /lib/ld-2.9.so
b80cb000-b80cc000 rw-p b80cb000 00:00 0 
b80cc000-b80cd000 r--p 00020000 75:00 204900     /lib/ld-2.9.so
b80cd000-b80ce000 rw-p 00021000 75:00 204900     /lib/ld-2.9.so
bfcb8000-bfccd000 rw-p bffeb000 00:00 0          [stack]  /*命令行参数、环境变量、stack*/
ffffe000-fffff000 r-xp 00000000 00:00 0          [vdso] /*这部分属于内核空间*/
从上面的内存分布可以看出命令行参数和环境变量保存在进程地址空间的末端，下面的问题是参数和变量是从何而来，又是如何映射到了指定的位置。
### 从何而来
内核在完成一系列的软硬件初始化、成功挂载根文件系统之后，内核尝试在根文件系统里寻找init程序并加载运行，从这里进入到用户空间。
**[plain]**[view
 plain](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)[copy](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)
- run_init_process("/sbin/init");  
- 
- static void run_init_process(char *init_filename)  
- {  
-     argv_init[0] = init_filename;  
-     kernel_execve(init_filename, argv_init, envp_init);  
- }  
kernel_execve是在内核空间里执行用户空间程序的接口，从调用形式上可以猜测，argv_init、 envp_init就是传递给init的命令行参数和环境变量参数。
**[plain]**[view
 plain](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)[copy](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)
- /*体系结构相关,arch/arm/kernel/sys_arm.c*/  
- int kernel_execve(const char *filename, char *const argv[], char *const envp[])  
- {  
- 
-     struct pt_regs regs;  
-     int ret;  
- 
-     memset(&s, 0, sizeof(struct pt_regs));  
- 
-     ret = do_execve((char *)filename, (char __user * __user *)argv,(char __user * __user *)envp, &s);  
- 
-     // do_execve调用成功的话kernel_execve就不会再返回内核空间而是到用户空//间去了，这通过操作栈来实现  
- 
- }  
**[plain]**[view
 plain](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)[copy](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)
- int do_execve(char * filename, char __user *__user *argv, char __user *__user *envp,struct pt_regs * regs)  
- {  
- 
-     struct linux_binprm *bprm;  
-     struct file *file;  
- 
-     //......  
-     bprm = kzalloc(sizeof(*bprm), GFP_KERNEL);  
- 
-     file = open_exec(filename);  
- 
-     bprm->file = file;  
- 
-     retval = bprm_mm_init(bprm);  
- 
-     bprm->argc = count(argv, MAX_ARG_STRINGS);  
- 
-     bprm->envc = count(envp, MAX_ARG_STRINGS);  
-     retval = copy_strings(bprm->envc, envp, bprm);  
-     retval = copy_strings(bprm->argc, argv, bprm);  
-     retval = search_binary_handler(bprm,regs);     
-  }  
copy_strings里会申请物理内存，将字符串拷贝到新申请的内存里。
search_binary_handler会根据可执行文件格式搜索系统已注册的的处理函数处理。
Linux下标准的可执行文件格式是ELF，除此之外,Linux还支持多种可执行文件格式。Linux用struct linux_binfmt[数据结构](http://lib.csdn.net/base/datastructure)来描述可执行文件格式，系统在启动时，由子系统调用register_binfmt来注册所支持的可执行文件格式，所有注册的struct linux_binfmt在内核里形成链表。内核在启动程序时，分析可执行文件的信息，在这个链表里搜索匹配的项。
struct linux_binfmt {
    struct list_head lh;
    struct module *module;
    int (*load_binary)(struct linux_binprm *, struct  pt_regs * regs);
    int (*load_shlib)(struct file *);
    int (*core_dump)(struct coredump_params *cprm);
    unsigned long min_coredump; /* minimal dump size */
    int hasvdso;
};
对应到ELF格式，这里的load_binary的值是load_elf_binary
/*fs/binfmt_elf.c*/
static int load_elf_binary(struct linux_binprm *bprm, struct pt_regs *regs)
{
    /* load_elf_binary 函数会根据bprm 的内容以及分析将要执行的elf格式的可执行文件的信息，将text、data、bss段映射到进程地址空间的相应位置，调用一个名为  
 setup_arg_pages的函数，将之前保存到新申请的物理内存页的命令行参数和环境变量映射到进程地址空间的指定位置，构建出图一所示的内存分布，建立起运行环境，最后跳转到程序的入口处开始执行*/
    start_thread(regs, elf_entry, bprm->p);
}
从上面的分析可以看出，传递给init的参数和环境变量的来自于argv_init和 envp_init。定义如下:
/*init/main.c*/
static char *argv_init[MAX_INIT_ARGS+2] = { "init", NULL, };
char *envp_init[MAX_INIT_ENVS+2] = { "HOME=/", "TERM=linux", NULL, };
这是两个内核全局变量，那么应该就处在内核空间，通过copy_strings、 setup_arg_pages的处理被映射到用户空间的指定位置，最终完成从内核到用户空间的传递。
除了定义时的默认值，bootloader启动内核时，还可以通过内核启动参数传递新的值给这两个变量。内核处理bootloader传递过来的启动参数时，分三类，
1. 内核子系统通过__setup注册的参数处理函数将会处理掉它能够识别的参数，如
__setup("init=", init_setup);启动参数里“init=”将被init_setup函数使用掉。
2. 剩下的形如”foo=bar”形式的参数解析为环境变量，保存到envp_init中。
3. 其余的参数保存到argv_init中。
### 小结
-  Bootloader引导Linux时，通过内核启动参数设置argv_init和envp_init的值
- 内核调用kernel_execv启动init程序，将argv_init和envp_init作为调用参数传递进去
- do_execve里申请新的物理内存页，将argv_init和envp_init新申请到物理内存处，通过setup_arg_pages将物理内存页映射到进程地址空间的指定位置处
-  init程序运行起来以后，从进程地址空间的指定位置处获取命令行参数和环境变量
### 普通应用程序的情形
Linux下面一个应用程序运行起来分两个步骤。首先通过fork系统调用创建出一个新的进程，这个新创建的进程会继承父进程的地址空间里的一些内容。然后，在新创建的子进程中通过execve加载将要运行的可执行文件，加载的过程里会释放从父进程那里继承而来的旧的地址空间，并根据可执行文件的解析结果创建新的地址空间，自此，新的应用程序就运行起来了。execve函数原型：
int execve(const char*pathname,char*const argv[],char*const envp[]);
execve最终由内核系统调用处理函数：sys_execv处理。
/*体系结构相关,arch/arm/kernel/sys_arm.c*/
**[plain]**[view
 plain](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)[copy](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)
- int sys_execve(char __user *filenamei, char __user * __user *argv,  char __user * __user *envp, struct pt_regs *regs)  
- {  
-     int error;  
-     char * filename;  
- 
-     //......  
-     filename = getname(filenamei);  
-     error = PTR_ERR(filename);  
-     if (IS_ERR(filename))  
-        goto out;  
- 
-    //......  
-    error = do_execve(filename, argv, envp, regs);  
-    //.....  
-    putname(filename);  
- 
-    out:  
- 
-    return error;  
- 
- }  
可以看到，通过execve->sys_execve->do_execve执行普通用户程序和内核启动init程序，最终都到了do_execve，与init不同的是，argv和envp参数来源不一样。显然，普通用户程序的argv和envp参数只能是调用execve进程来指定。可以猜测，通过SHELL启动的用户程序，将由SHELL来准备好这两个参数。
/*以下例子来自于《UNIX环境高级编程》Figure 8.16*/
**[plain]**[view
 plain](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)[copy](http://blog.csdn.net/xiaojizhong183/article/details/8192129#)
- #include <stdio.h>  
- #include <sys/wait.h>  
- 
- char *env_init[] = {"USER=unkown", "PATH=tmp", NULL};  
- 
-  int main(void)  
- {  
- 
-     pid_t pid;  
- 
-     if ((pid=fork()) < 0)  
-     {  
-         printf("fork error\n");  
-         return -1;  
-     }  
-     else if (pid == 0)  /*child*/  
-     {  
- 
-         if (execle("/home/temp/exec/echo", "echo", "arg1", "arg2", (char*)0, env_init) < 0)  
-         {  
-               printf("execle error\n");  
-               return -1;  
-         }  
-     }  
- 
-     if (waitpid(pid, NULL, 0) < 0)  
-     {  
-         printf("wait error");  
-         return -1;  
-     }  
- 
-     printf("test done!\n");  
-     return 0;  
- }  
- 
- /*/home/temp/exec/echo */  
- 
- #include <stdio.h>  
- 
- extern char** environ;  
- 
- int main(int argc, char* argv[])  
- {  
-     int i;  
-     char** ptr;  
- 
-     for(i=0; i<argc; i++)  
-         printf("argv[%d]: %s\n", i, argv[i]);  
- 
-     for(ptr=environ; *ptr!=NULL; ptr++)  
- 
-         printf("%s\n", *ptr);  
- }  
运行结果:
argv[0]: echo
argv[1]: arg1
argv[2]: arg2
USER=unkown
PATH=tmp
test done!
