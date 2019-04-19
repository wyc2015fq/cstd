# 从select函数谈及系统调用原理 - Koma Hub - CSDN博客
2019年01月12日 16:15:10[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：63
**目录**
[系统调用流程](#%E7%B3%BB%E7%BB%9F%E8%B0%83%E7%94%A8%E6%B5%81%E7%A8%8B)
[select函数](#select%E5%87%BD%E6%95%B0)
[在glibc-2.28中查找select函数](#%E5%9C%A8glibc-2.28%E4%B8%AD%E6%9F%A5%E6%89%BEselect%E5%87%BD%E6%95%B0)
[在linux内核linux-4.20.1中查找select](#%E5%9C%A8linux%E5%86%85%E6%A0%B8linux-4.20.1%E4%B8%AD%E6%9F%A5%E6%89%BEselect)
[参考：](#%E5%8F%82%E8%80%83%EF%BC%9A)
linux的select机制在socket编程中非常有用，其中存放文件描述符的fd_set已经给出代码：
[https://blog.csdn.net/Rong_Toa/article/details/86359099](https://blog.csdn.net/Rong_Toa/article/details/86359099)
# 系统调用流程
![](https://img-blog.csdnimg.cn/20190112152754784.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
图片来源：[https://www.cnblogs.com/fasionchan/p/9431784.html](https://www.cnblogs.com/fasionchan/p/9431784.html)
# select函数
## 在glibc-2.28中查找select函数
在glibc中只能找到类似select.h这样的外部函数声明，
```cpp
/* Check the first NFDS descriptors each in READFDS (if not NULL) for read
   readiness, in WRITEFDS (if not NULL) for write readiness, and in EXCEPTFDS
   (if not NULL) for exceptional conditions.  If TIMEOUT is not NULL, time out
   after waiting the interval specified therein.  Returns the number of ready
   descriptors, or -1 for errors.
   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int select (int __nfds, fd_set *__restrict __readfds,
		   fd_set *__restrict __writefds,
		   fd_set *__restrict __exceptfds,
		   struct timeval *__restrict __timeout);
```
## 在linux内核linux-4.20.1中查找select
发现经过source insight4.0创建的项目找不到select函数，然后查找select的文件名，找到了一个名字为select.c的文件，打开之，找到如下这行
```cpp
COMPAT_SYSCALL_DEFINE5(select, int, n, compat_ulong_t __user *, inp,
	compat_ulong_t __user *, outp, compat_ulong_t __user *, exp,
	struct old_timeval32 __user *, tvp)
{
	return do_compat_select(n, inp, outp, exp, tvp);
}
//还有一个
COMPAT_SYSCALL_DEFINE1(old_select, struct compat_sel_arg_struct __user *, arg)
{
	struct compat_sel_arg_struct a;
	if (copy_from_user(&a, arg, sizeof(a)))
		return -EFAULT;
	return do_compat_select(a.n, compat_ptr(a.inp), compat_ptr(a.outp),
				compat_ptr(a.exp), compat_ptr(a.tvp));
}
```
再继续找到do_compat_select：
```cpp
static int do_compat_select(int n, compat_ulong_t __user *inp,
	compat_ulong_t __user *outp, compat_ulong_t __user *exp,
	struct old_timeval32 __user *tvp)
{
	struct timespec64 end_time, *to = NULL;
	struct old_timeval32 tv;
	int ret;
	if (tvp) {
		if (copy_from_user(&tv, tvp, sizeof(tv)))
			return -EFAULT;
		to = &end_time;
		if (poll_select_set_timeout(to,
				tv.tv_sec + (tv.tv_usec / USEC_PER_SEC),
				(tv.tv_usec % USEC_PER_SEC) * NSEC_PER_USEC))
			return -EINVAL;
	}
	ret = compat_core_sys_select(n, inp, outp, exp, to);
	ret = compat_poll_select_copy_remaining(&end_time, tvp, 1, ret);
	return ret;
}
```
很显然调用了这个接口：
```cpp
copy_from_user(&tv, tvp, sizeof(tv))
```
well，显然这实在内核中发生的事情，不过不要着急，这个只是select中的timeval参数，继续搜索COMPAT_SYSCALL_DEFINE5。结果没有直接的这个名字的宏，那么断定，这是一个宏定义的宏，至于宏定义了几“层”，尚且未知。那么想办法，等一下，方法这个select.c里面有一个do_select的函数，目前觉得可能最终是调用了这个接口。据说linux内核中的接口都是用这个“SYSCALL”调用的，找一下SYSCALL，在名为systbl_chk.c中找到了一个
```cpp
#define SYSCALL(func)		__NR_##func
```
此外还找到一个
```cpp
#define COMPAT_SYS(func)	__NR_##func
```
回顾一下上面的select接口的宏定义声明或者定义方式：
```cpp
COMPAT_SYSCALL_DEFINE5(select, int, n, compat_ulong_t __user *, inp,
	compat_ulong_t __user *, outp, compat_ulong_t __user *, exp,
	struct old_timeval32 __user *, tvp)
```
这样看起来就对应上了，难不成有个叫做CALL_DEFINE5的宏？没找到。
那再找找__NR_select？这个还真有，在unistd.h文件里的宏定义：
```cpp
#define __NR_select			358
```
此外还有一个：
```cpp
#define __NR_select 				 14
__SYSCALL( 14, sys_select, 5)
```
还记得COMPAT_SYSCALL_DEFINE5吧，？难不成还有个COMPAT这个宏？不管了，在搜一下sys_select：
在syscall.h中找到：
```cpp
asmlinkage long sys_select(int n, fd_set __user *inp, fd_set __user *outp,
			fd_set __user *exp, struct timeval __user *tvp);
```
其中：
```cpp
#define asmlinkage CPP_ASMLINKAGE __attribute__((syscall_linkage))
```
在syscall.c中：
```cpp
#ifdef CONFIG_PPC32
/*
 * Due to some executables calling the wrong select we sometimes
 * get wrong args.  This determines how the args are being passed
 * (a single ptr to them all args passed) then calls
 * sys_select() with the appropriate args. -- Cort
 */
int
ppc_select(int n, fd_set __user *inp, fd_set __user *outp, fd_set __user *exp, struct timeval __user *tvp)
{
	if ( (unsigned long)n >= 4096 )
	{
		unsigned long __user *buffer = (unsigned long __user *)n;
		if (!access_ok(VERIFY_READ, buffer, 5*sizeof(unsigned long))
		    || __get_user(n, buffer)
		    || __get_user(inp, ((fd_set __user * __user *)(buffer+1)))
		    || __get_user(outp, ((fd_set  __user * __user *)(buffer+2)))
		    || __get_user(exp, ((fd_set  __user * __user *)(buffer+3)))
		    || __get_user(tvp, ((struct timeval  __user * __user *)(buffer+4))))
			return -EFAULT;
	}
	return sys_select(n, inp, outp, exp, tvp);
}
#endif
```
最终找到了select.c里：
```cpp
/*
 * This is a virtual copy of sys_select from fs/select.c and probably
 * should be compared to it from time to time
 */
/*
 * We can actually return ERESTARTSYS instead of EINTR, but I'd
 * like to be certain this leads to no problems. So I return
 * EINTR just for safety.
 *
 * Update: ERESTARTSYS breaks at least the xview clock binary, so
 * I'm trying ERESTARTNOHAND which restart only when you want to.
 */
static int compat_core_sys_select(int n, compat_ulong_t __user *inp,
	compat_ulong_t __user *outp, compat_ulong_t __user *exp,
	struct timespec64 *end_time)
{
    /*此处省略一万个字*/
    ret = do_select(n, &fds, end_time);
    /*此处省略一万个字*/
}
```
其实我还是没有找到具体如何调用的，反正最终就是这么调用的。
# 参考：
[https://my.oschina.net/fileoptions/blog/908682](https://my.oschina.net/fileoptions/blog/908682)
[https://www.cnblogs.com/fasionchan/p/9431784.html](https://www.cnblogs.com/fasionchan/p/9431784.html)
