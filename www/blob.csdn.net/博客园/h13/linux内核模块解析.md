# linux内核模块解析 - h13 - 博客园
转自： [http://blog.csdn.net/linweig/article/details/5085487](http://blog.csdn.net/linweig/article/details/5085487)
Linux 就是通常所说的*单内核（monolithic kernel）*，即操作系统的大部分功能都被称为*内核*，并在特权模式下运行。它与*微型内核* 不同，后者只把基本的功能（进程间通信 [IPC]、调度、基本的输入/输出 [I/O] 和内存管理）当作内核运行，而把其他功能（驱动程序、网络堆栈和文件系统）排除在特权空间之外。因此，您可能认为 Linux 是一个完全静态的内核，但事实恰恰相反。通过 Linux 内核模块（LKM）可以在运行时动态地更改 Linux。
*可动态更改* 是指可以将新的功能加载到内核、从内核去除某个功能，甚至添加使用其他 LKM 的新 LKM。LKM 的优点是可以最小化内核的内存占用，只加载需要的元素（这是嵌入式系统的重要特性）。
Linux 不是可以进行动态更改的惟一（也不是第一个）单内核。Berkeley Software Distribution（BSD）的变体、Sun Solaris、更老的内核（比如 OpenVMS），以及其他流行的操作系统（比如 Microsoft® Windows® 和 Apple Mac OS X）都支持可加载模块。
LKM，它是以一个什么样的东西呢？我们知道，他编译之后会是一个ko后缀名的东西，但是里面是一个什么样的。我们看，一般的模块的模板。
/*
 * hello.c  Hello, World! As a Kernel Module
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
/*
 * hello_init  the init function, called when the module is loaded.
 * Returns zero if successfully loaded, nonzero otherwise.
 */
static int hello_init(void)
{
        printk(KERN_ALERT "I bear a charmed life./n");
        return 0;
}
/*
 * hello_exit  the exit function, called when the module is removed.
 */
static void hello_exit(void)
{
        printk(KERN_ALERT "Out, out, brief candle!/n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shakespeare");
模块并不是驱动，只是Linux的一种内核扩展机制，驱动就是借助这种机制的，理论上应该这样的理解。
这就是一个模块的基本模板，通过编译之后，会生成一个ko，注意，编译的时候与其他的应用程序有很大的不同，原因是它生成的东西是一个完全独立的ELF。
LKM 与直接编译到内核或典型程序的元素有根本区别。典型的程序有一个 main 函数，其中 LKM 包含 entry 和 exit 函数（在 2.6 版本，您可以任意命名这些函数）。当向内核插入模块时，调用 entry 函数，从内核删除模块时则调用 exit 函数。因为 entry 和 exit 函数是用户定义的，所以存在 `module_init` 和 `module_exit` 宏，用于定义这些函数属于哪种函数。LKM 还包含一组必要的宏和一组可选的宏，用于定义模块的许可证、模块的作者、模块的描述等等。
2.6 版本的 Linux 内核提供了一个新的更简单的方法，用于构建 LKM。构建 LKM 时，可以使用典型的用户工具管理模块（尽管内部已经改变）：标准 `insmod`（安装 LKM），`rmmod` （删除 LKM），`modprobe`（`insmod` 和 `rmmod` 的包装器），`depmod`（用于创建模块依赖项），以及 `modinfo`（用于为模块宏查找值）。
LKM 只不过是一个特殊的可执行可链接格式（Executable and Linkable Format，ELF）对象文件。通常，必须链接对象文件才能在可执行文件中解析它们的符号和结果。由于必须将 LKM 加载到内核后 LKM 才能解析符号，所以 LKM 仍然是一个 ELF 对象。您可以在 LKM 上使用标准对象工具（在 2.6 版本中，内核对象带有后缀 *.ko,*）。例如，如果在 LKM 上使用 `objdump` 实用工具，您将发现一些熟悉的区段（section），比如 *.text*（说明）、*.data*（已初始化数据）和 *.bss*（块开始符号或未初始化数据）。
您还可以在模块中找到其他支持动态特性的区段。.init.text 区段包含 `module_init` 代码，.exit.text 区段包含 `module_exit` 代码（参见图 2）。.modinfo 区段包含各种表示模块许可证、作者和描述等的宏文本。
![LKM](http://p.blog.csdn.net/images/p_blog_csdn_net/jinlking/EntryImages/20090521/figure2.jpg)
在用户空间中，`insmod`（插入模块）启动模块加载过程。`insmod` 命令定义需要加载的模块，并调用 `init_module` 用户空间系统调用，开始加载过程。2.6 版本内核的 `insmod` 命令经过修改后变得非常简单（70 行代码），可以在内核中执行更多工作。`insmod` 并不进行所有必要的符号解析（处理 `kerneld`），它只是通过 `init_module` 函数将模块二进制文件复制到内核，然后由内核完成剩余的任务。
`init_module` 函数通过系统调用层，进入内核到达内核函数 `sys_init_module`（参见图 3）。这是加载模块的主要函数，它利用许多其他函数完成困难的工作。类似地，`rmmod` 命令会使 `delete_module` 执行 `system call` 调用，而 `delete_module` 最终会进入内核，并调用 `sys_delete_module` 将模块从内核删除。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/jinlking/EntryImages/20090521/figure3.jpg)
在模块的加载和卸载期间，模块子系统维护了一组简单的状态变量，用于表示模块的操作。加载模块时，状态为 `MODULE_STATE_COMING`。如果模块已经加载并且可用，状态为 `MODULE_STATE_LIVE`。此外，卸载模块时，状态为 `MODULE_STATE_GOING`。
下面开始剖析一下，这样的一个elf文件是如何加入进内核的：
现在，我们看看加载模块时的内部函数。当调用内核函数 `sys_init_module` 时，会开始一个许可检查，查明调用者是否有权执行这个操作（通过 `capable` 函数完成）。然后，调用 `load_module` 函数，这个函数负责将模块加载到内核并执行必要的调试（后面还会讨论这点）。`load_module` 函数返回一个指向最新加载模块的模块引用。这个模块加载到系统内具有双重链接的所有模块的列表上，并且通过 notifier 列表通知正在等待模块状态改变的线程。最后，调用模块的 `init()` 函数，更新模块状态，表明模块已经加载并且可用。
/* This is where the real work happens */
asmlinkage long
sys_init_module(void __user *umod,
        unsigned long len,
        const char __user *uargs)
{
    struct module *mod;
    int ret = 0;
    /* Must have permission */
    if (!capable(CAP_SYS_MODULE))
        return -EPERM;
    /* Only one module load at a time, please */
    if (mutex_lock_interruptible(&module_mutex) != 0)
        return -EINTR;
    /* Do all the hard work */
    mod = load_module(umod, len, uargs);
    if (IS_ERR(mod)) {
        mutex_unlock(&module_mutex);
        return PTR_ERR(mod);
    }
    /* Drop lock so they can recurse */
    mutex_unlock(&module_mutex);
    blocking_notifier_call_chain(&module_notify_list,
            MODULE_STATE_COMING, mod);
    /* Start the module */
    if (mod->init != NULL)
        ret = do_one_initcall(mod->init);
    if (ret < 0) {
        /* Init routine failed: abort.  Try to protect us from
                   buggy refcounters. */
        mod->state = MODULE_STATE_GOING;
        synchronize_sched();
        module_put(mod);
        blocking_notifier_call_chain(&module_notify_list,
                         MODULE_STATE_GOING, mod);
        mutex_lock(&module_mutex);
        free_module(mod);
        mutex_unlock(&module_mutex);
        wake_up(&module_wq);
        return ret;
    }
    if (ret > 0) {
        printk(KERN_WARNING "%s: '%s'->init suspiciously returned %d, "
                    "it should follow 0/-E convention/n"
               KERN_WARNING "%s: loading module anyway.../n",
               __func__, mod->name, ret,
               __func__);
        dump_stack();
    }
    /* Now it's a first class citizen!  Wake up anyone waiting for it. */
    mod->state = MODULE_STATE_LIVE;
    wake_up(&module_wq);
    mutex_lock(&module_mutex);
    /* Drop initial reference. */
    module_put(mod);
    unwind_remove_table(mod->unwind_info, 1);
    module_free(mod, mod->module_init);
    mod->module_init = NULL;
    mod->init_size = 0;
    mod->init_text_size = 0;
    mutex_unlock(&module_mutex);
    return 0;
}
加载模块的内部细节是 ELF 模块解析和操作。`load_module` 函数（位于 ./linux/kernel/module.c）首先分配一块用于容纳整个 ELF 模块的临时内存。然后，通过 `copy_from_user` 函数将 ELF 模块从用户空间读入到临时内存。作为一个 ELF 对象，这个文件的结构非常独特，易于解析和验证。
这个函数的代码比较长，实现的就是一个模块的解析，找出各个Section，这个有兴趣的人可以看看Linux内核的源码。
下一步是对加载的 ELF 映像执行一组健康检查（它是有效的 ELF 文件吗？它适合当前的架构吗？等等）。完成健康检查后，就会解析 ELF 映像，然后会为每个区段头创建一组方便变量，简化随后的访问。因为 ELF 对象的偏移量是基于 0 的（除非重新分配），所以这些方便变量将相对偏移量包含到临时内存块中。在创建方便变量的过程中还会验证 ELF 区段头，确保加载的是有效模块。
任何可选的模块参数都从用户空间加载到另一个已分配的内核内存块（第 4 步），并且更新模块状态，表明模块已加载（`MODULE_STATE_COMING`）。如果需要 per-CPU 数据（这在检查区段头时确定），那么就分配 per-CPU 块。
在前面的步骤，模块区段被加载到内核（临时）内存，并且知道哪个区段应该保持，哪个可以删除。步骤 7 为内存中的模块分配最终的位置，并移动必要的区段（ELF 头中的 `SHF_ALLOC`， 或在执行期间占用内存的区段）。然后执行另一个分配，大小是模块必要区段所需的大小。迭代临时 ELF 块中的每个区段，并将需要执行的区段复制到新的块中。接下来要进行一些额外的维护。同时还进行符号解析，可以解析位于内核中的符号（被编译成内核映象）， 或临时的符号（从其他模块导出）。
然后为每个剩余的区段迭代新的模块并执行重新定位。这个步骤与架构有 关，因此依赖于为架构（./linux/arch/<arch>/kernel/module.c）定义的 helper 函数。最后，刷新指令缓存（因为使用了临时 .text 区段），执行一些额外的维护（释放临时模块内存，设置系统文件），并将模块最终返回到 `load_module`。
