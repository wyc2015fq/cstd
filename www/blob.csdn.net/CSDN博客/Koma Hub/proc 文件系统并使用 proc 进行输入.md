# /proc 文件系统并使用/proc 进行输入 - Koma Hub - CSDN博客
2019年02月02日 09:12:13[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：63

在 Linux 中有一个另外的机制来使内核及内核模块发送信息给进程——/proc 文件系统。/proc 文件系统最初是设计使得容易得到进程的信息（从名字可以看出），现在却被任意一块有内容需要报告的内核使用，比如拥有模块列表的/proc/modules 和拥有内存使用统计信息的/proc/meminfo。
使用 proc 文件系统的方法很象使用设备驱动——你创建一个数据结构，使之包含/proc文件需要的全部信息，包括所有函数的句柄（在我们的例子里只有一个，在试图读取/proc文件时调用）。然后，用init_module 注册这个结构，用cleanup_module 注销。
我们使用 proc_register_dynamic（注3.1）的原因是我们不希望决定以后在文件中使用的索引节点数，而是让内核来决定它，为了防止冲突。标准的文件系统是在磁盘上而不是在内存（/proc 的位置在内存），在这种情况下节点数是一个指向文件的索引节点所在磁盘地址的指针。这个索引节点包含了文件的有关信息比如文件的访问权限以及指向磁盘地址的指真或者文件数据的位置。
因 为 在 文 件 打 开 或 关 闭 时 我 们 没 有 调 用 ， 所以在模块里无处可放宏MOD_INC_USE_COUNT 和MOD_DEC_USE_COUNT，而且如果文件被打开了或者模块被删除了，就没有办法来避免这个结果。下一章我们将会看到一个更困难的处理/proc 的方法，但是也更加灵活，也能够解决这个问题。
### ex procfs.c
```cpp
/* procfs.c - create a "file" in /proc
* Copyright (C) 1998-1999 by Ori Pomerantz
*/
/* The necessary header files */
/* Standard in kernel modules */
#include <linux/kernel.h> /* We're doing kernel work */
#include <linux/module.h> /* Specifically, a module */
/* Deal with CONFIG_MODVERSIONS */
#if CONFIG_MODVERSIONS==1
 #define MODVERSIONS
 #include <linux/modversions.h>
#endif
/* Necessary because we use the proc fs */
#include <linux/proc_fs.h>
/* In 2.2.3 /usr/include/linux/version.h includes a
* macro for this, but 2.0.35 doesn't - so I add it
* here if necessary. */
#ifndef KERNEL_VERSION
 #define KERNEL_VERSION(a,b,c) ((a)*65536+(b)*256+(c))
#endif
/* Put data into the proc fs file.
Arguments
=========
1. The buffer where the data is to be inserted, if
you decide to use it.
2. A pointer to a pointer to characters. This is
useful if you don't want to use the buffer
allocated by the kernel.
3. The current position in the file.
4. The size of the buffer in the first argument.
5. Zero (for future use?).
Usage and Return Value
======================
If you use your own buffer, like I do, put its
location in the second argument and return the
number of bytes used in the buffer.
A return value of zero means you have no further
information at this time (end of file). A negative
return value is an error condition.
For More Information
====================
The way I discovered what to do with this function
wasn't by reading documentation, but by reading the
code which used it. I just looked to see what uses
the get_info field of proc_dir_entry struct (I used a
combination of find and grep, if you're interested),
and I saw that it is used in <kernel source
directory>/fs/proc/array.c.
If something is unknown about the kernel, this is
usually the way to go. In Linux we have the great
advantage of having the kernel source code for
free - use it.
*/
int procfile_read(char *buffer,
                    char **buffer_location,
                    off_t offset,
                    int buffer_length,
                    int zero)
{
    int len; /* The number of bytes actually used */
    /* This is static so it will still be in memory
    * when we leave this function */
    static char my_buffer[80];
    static int count = 1;
    /* We give all of our information in one go, so if the
    * user asks us if we have more information the
    * answer should always be no.
    *
    * This is important because the standard read
    * function from the library would continue to issue
    * the read system call until the kernel replies
    * that it has no more information, or until its
    * buffer is filled.
    */
    if (offset > 0)
        return 0;
    /* Fill the buffer and get its length */
    len = sprintf(my_buffer,
                "For the %d%s time, go away!\n", count,
                (count % 100 > 10 && count % 100 < 14) ? "th" :
                (count % 10 == 1) ? "st" :
                (count % 10 == 2) ? "nd" :
                (count % 10 == 3) ? "rd" : "th" );
    count++;
    /* Tell the function which called us where the
    * buffer is */
    *buffer_location = my_buffer;
    /* Return the length */
    return len;
}
struct proc_dir_entry Our_Proc_File =
{
    0, /* Inode number - ignore, it will be filled by
        * proc_register[_dynamic] */
    4, /* Length of the file name */
    "test", /* The file name */
    S_IFREG | S_IRUGO, /* File mode - this is a regular
                        * file which can be read by its
                        * owner, its group, and everybody
                        * else */
    1, /* Number of links (directories where the
        * file is referenced) */
    0, 0, /* The uid and gid for the file - we give it
            * to root */
    80, /* The size of the file reported by ls. */
    NULL, /* functions which can be done on the inode
            * (linking, removing, etc.) - we don't
            * support any. */
    procfile_read, /* The read function for this file,
                    * the function called when somebody
                    * tries to read something from it. */
    NULL /* We could have here a function to fill the
            * file's inode, to enable us to play with
            * permissions, ownership, etc. */
};
/* Initialize the module - register the proc file */
int init_module()
{
    /* Success if proc_register[_dynamic] is a success,
    * failure otherwise. */
    #if LINUX_VERSION_CODE > KERNEL_VERSION(2,2,0)
    /* In version 2.2, proc_register assign a dynamic
    * inode number automatically if it is zero in the
    * structure , so there's no more need for
    * proc_register_dynamic
    */
    return proc_register(&proc_root, &Our_Proc_File);
    #else
    return proc_register_dynamic(&proc_root, &Our_Proc_File);
    #endif
    /* proc_root is the root directory for the proc
    * fs (/proc). This is where we want our file to be
    * located.
    */
}
/* Cleanup - unregister our file from /proc */
void cleanup_module()
{
    proc_unregister(&proc_root, Our_Proc_File.low_ino);
}
```
## 使用/proc 进行输入
现在我们已经有了两种方法从内核模块中产生输出：注册一个设备驱动并且mknod 一个设备文件，或者创建一个/proc 文件。这可以使内核告诉我们任何信息。现在的问题是我们没有办法回答给内核。我们象内核输入的第一种方法是写给/proc 文件。
因为 proc 文件系统主要是为满足内核向进程报告其状态的，没有为输入留出特别的规定。数据结构proc_dir_entry 没有包含一个指向某个输入函数的指针，就象指向输出函数那样。如果我们要向一个/proc 文件写入，我们需要使用标准文件系统机制。
在 Linux 里有一个文件系统注册的标准机制。每个文件系统都有自己的函数来处理索引节点和文件操作， 所以就有一个特殊的机构来存放指向所有函数的指针， structinode_operations，它有一个指向struct file_operations 的指针。在/proc 里，无论何时我们注册一个新文件，我们就被允许指定用inod_operations 访问哪个结构。这就是我们要用的机制，一个inode_operations，包括一个指向file_operations 的指针，file_operations 里包含我们的module_input 和module_output 函数。
必须指出标准的读写角色在内核中被倒置了，读函数用来输出，而写函数用来输入。这是因为读和写是在用户的观点看，如果一个进程从内核中读取一些内容，那么内核就必须输出处理。而进程要写入内核，内核就要接受输入。
另一个有趣的地方是 module_permission 函数。这个函数每当进程试图对/proc 文件进行处理时调用，它可以决定是否允许访问。目前这个函数只定义在操作和当前使用的uid（当前可用的是一个指针指向一个当前运行进程的信息的结构）的基础上，但是它可以在我们希望的任何事物的基础上定义，比如其他进程正在对文件做的操作，日期时间或者接收到的最后一个输入。
使用 put_usr 和get_user 的原因是Linux 的内存是分段的（在Intel 结构下，其他系列的处理器下可能不同）。这意味着一个指针本身不代表内存中的一个唯一地址，而是段中的一个地址，所以你还需要知道哪一个段可以使用它。内核占有一个段，每个进程都各占有一个段。
一个进程可以访问的唯一的段就是它自己拥有的那个，所以当你写作为进程运行的程序时可以不用关心段的问题。如果你要写内核模块，一般你希望访问内核的段，这由系统自动处理。然而，如果内存缓冲区的内容需要在当前运行的进程和内核之间传递时，内核函数会接到在此进程段里的指向内存缓冲区的一个指针。Put_user 和get_user 允许你访问那块内存。
### ex procfs.c
```cpp
/* procfs.c - create a "file" in /proc, which allows
* both input and output. */
/* Copyright (C) 1998-1999 by Ori Pomerantz */
/* The necessary header files */
/* Standard in kernel modules */
#include <linux/kernel.h> /* We're doing kernel work */
#include <linux/module.h> /* Specifically, a module */
/* Deal with CONFIG_MODVERSIONS */
#if CONFIG_MODVERSIONS==1
#define MODVERSIONS
#include <linux/modversions.h>
#endif
/* Necessary because we use proc fs */
#include <linux/proc_fs.h>
/* In 2.2.3 /usr/include/linux/version.h includes a
* macro for this, but 2.0.35 doesn't - so I add it
* here if necessary. */
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) ((a)*65536+(b)*256+(c))
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)
#include <asm/uaccess.h> /* for get_user and put_user */
#endif
/* The module's file functions ********************** */
/* Here we keep the last message received, to prove
* that we can process our input */
#define MESSAGE_LENGTH 80
static char Message[MESSAGE_LENGTH];
/* Since we use the file operations struct, we can't
* use the special proc output provisions - we have to
* use a standard read function, which is this function */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)
static ssize_t module_output(
                struct file *file, /* The file read */
                char *buf, /* The buffer to put data to (in the
                * user segment) */
                size_t len, /* The length of the buffer */
                loff_t *offset) /* Offset in the file - ignore */
#else
static int module_output(
                struct inode *inode, /* The inode read */
                struct file *file, /* The file read */
                char *buf, /* The buffer to put data to (in the
                * user segment) */
                int len) /* The length of the buffer */
#endif
{
    static int finished = 0;
    int i;
    char message[MESSAGE_LENGTH+30];
    /* We return 0 to indicate end of file, that we have
    * no more information. Otherwise, processes will
    * continue to read from us in an endless loop. */
    if (finished) {
        finished = 0;
        return 0;
    }
    /* We use put_user to copy the string from the kernel's
    * memory segment to the memory segment of the process
    * that called us. get_user, BTW, is
    * used for the reverse. */
    sprintf(message, "Last input:%s", Message);
    for(i=0; i<len && message[i]; i++)
        put_user(message[i], buf+i);
    /* Notice, we assume here that the size of the message
    * is below len, or it will be received cut. In a real
    * life situation, if the size of the message is less
    * than len then we'd return len and on the second call
    * start filling the buffer with the len+1'th byte of
    * the message. */
    finished = 1;
    return i; /* Return the number of bytes "read" */
}
/* This function receives input from the user when the
* user writes to the /proc file. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)
static ssize_t module_input(
                struct file *file, /* The file itself */
                const char *buf, /* The buffer with input */
                size_t length, /* The buffer's length */
                loff_t *offset) /* offset to file - ignore */
#else
static int module_input(
                struct inode *inode, /* The file's inode */
                struct file *file, /* The file itself */
                const char *buf, /* The buffer with the input */
                int length) /* The buffer's length */
#endif
{
    int i;
    /* Put the input into Message, where module_output
    * will later be able to use it */
    for(i=0; i<MESSAGE_LENGTH-1 && i<length; i++)
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)
        get_user(Message[i], buf+i);
    /* In version 2.2 the semantics of get_user changed,
    * it not longer returns a character, but expects a
    * variable to fill up as its first argument and a
    * user segment pointer to fill it from as the its
    * second.
    *
    * The reason for this change is that the version 2.2
    * get_user can also read an short or an int. The way
    * it knows the type of the variable it should read
    * is by using sizeof, and for that it needs the
    * variable itself.
    */
    #else
    Message[i] = get_user(buf+i);
    #endif
    Message[i] = '\0'; /* we want a standard, zero
    * terminated string */
    /* We need to return the number of input characters
    * used */
    return i;
}
/* This function decides whether to allow an operation
* (return zero) or not allow it (return a non-zero
* which indicates why it is not allowed).
*
* The operation can be one of the following values:
* 0 - Execute (run the "file" - meaningless in our case)
* 2 - Write (input to the kernel module)
* 4 - Read (output from the kernel module)
*
* This is the real function that checks file
* permissions. The permissions returned by ls -l are
* for referece only, and can be overridden here.
*/
static int module_permission(struct inode *inode, int op)
{
    /* We allow everybody to read from our module, but
    * only root (uid 0) may write to it */
    if (op == 4 || (op == 2 && current->euid == 0))
        return 0;
    /* If it's anything else, access is denied */
    return -EACCES;
}
/* The file is opened - we don't really care about
* that, but it does mean we need to increment the
* module's reference count. */
int module_open(struct inode *inode, struct file *file)
{
    MOD_INC_USE_COUNT;
    return 0;
}
/* The file is closed - again, interesting only because
* of the reference count. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)
int module_close(struct inode *inode, struct file *file)
#else
void module_close(struct inode *inode, struct file *file)
#endif
{
    MOD_DEC_USE_COUNT;
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)
    return 0; /* success */
    #endif
}
/* Structures to register as the /proc file, with
* pointers to all the relevant functions. ********** */
/* File operations for our proc file. This is where we
* place pointers to all the functions called when
* somebody tries to do something to our file. NULL
* means we don't want to deal with something. */
static struct file_operations File_Ops_4_Our_Proc_File =
{
    NULL, /* lseek */
    module_output, /* "read" from the file */
    module_input, /* "write" to the file */
    NULL, /* readdir */
    NULL, /* select */
    NULL, /* ioctl */
    NULL, /* mmap */
    module_open, /* Somebody opened the file */
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)
    NULL, /* flush, added here in version 2.2 */
    #endif
    module_close, /* Somebody closed the file */
    /* etc. etc. etc. (they are all given in
    * /usr/include/linux/fs.h). Since we don't put
    * anything here, the system will keep the default
    * data, which in Unix is zeros (NULLs when taken as
    * pointers). */
};
/* Inode operations for our proc file. We need it so
* we'll have some place to specify the file operations
* structure we want to use, and the function we use for
* permissions. It's also possible to specify functions
* to be called for anything else which could be done to
* an inode (although we don't bother, we just put
* NULL). */
static struct inode_operations Inode_Ops_4_Our_Proc_File =
{
    &File_Ops_4_Our_Proc_File,
    NULL, /* create */
    NULL, /* lookup */
    NULL, /* link */
    NULL, /* unlink */
    NULL, /* symlink */
    NULL, /* mkdir */
    NULL, /* rmdir */
    NULL, /* mknod */
    NULL, /* rename */
    NULL, /* readlink */
    NULL, /* follow_link */
    NULL, /* readpage */
    NULL, /* writepage */
    NULL, /* bmap */
    NULL, /* truncate */
    module_permission /* check for permissions */
};
/* Directory entry */
static struct proc_dir_entry Our_Proc_File =
{
    0, /* Inode number - ignore, it will be filled by
        * proc_register[_dynamic] */
    7, /* Length of the file name */
    "rw_test", /* The file name */
    S_IFREG | S_IRUGO | S_IWUSR,
       /* File mode - this is a regular file which
        * can be read by its owner, its group, and everybody
        * else. Also, its owner can write to it.
        *
        * Actually, this field is just for reference, it's
        * module_permission that does the actual check. It
        * could use this field, but in our implementation it
        * doesn't, for simplicity. */
    1, /* Number of links (directories where the
        * file is referenced) */
    0, 0, /* The uid and gid for the file -
            * we give it to root */
    80, /* The size of the file reported by ls. */
    &Inode_Ops_4_Our_Proc_File,
    /* A pointer to the inode structure for
     * the file, if we need it. In our case we
     * do, because we need a write function. */
    NULL
    /* The read function for the file. Irrelevant,
     * because we put it in the inode structure above */
};
/* Module initialization and cleanup ******************* */
/* Initialize the module - register the proc file */
int init_module()
{
    /* Success if proc_register[_dynamic] is a success,
    * failure otherwise */
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)
    /* In version 2.2, proc_register assign a dynamic
    * inode number automatically if it is zero in the
    * structure , so there's no more need for
    * proc_register_dynamic
    */
    return proc_register(&proc_root, &Our_Proc_File);
    #else
    return proc_register_dynamic(&proc_root, &Our_Proc_File);
    #endif
}
/* Cleanup - unregister our file from /proc */
void cleanup_module()
{
    proc_unregister(&proc_root, Our_Proc_File.low_ino);
}
```
