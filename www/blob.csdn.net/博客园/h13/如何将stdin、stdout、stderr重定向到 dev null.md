# 如何将stdin、stdout、stderr重定向到/dev/null - h13 - 博客园
如何将stdin、stdout、stderr重定向到/dev/null
Q: 我使用了如下代码将stdin、stdout、stderr重定向到/dev/null
   freopen( "/dev/null", "w", stdout );
   freopen( "/dev/null", "w", stderr );
   freopen( "/dev/null", "r", stdin ); 
   这样做正确吗，是否使用"w+"或者"a"更正确一些。在很多代码中是这样完成重定
   向的:
   close( 0 );
   close( 1 );
   close( 2 );
   open( "/dev/null", O_RDWR );
   dup( 0 );
   dup( 0 );
   这两种方式中哪一种更好、更具可移植性。
A: Andrew Gierth <andrew@erlenstar.demon.co.uk>
第一种方式不是总能达到目的。freopen()并不确保新的文件流描述符一定重用底层
原有文件句柄。假如未能重用，向stderr流输出的标准I/O函数最终输出到/dev/null，
但那些向STDERR_FILENO句柄输出的标准I/O函数就没这么幸运了，可能输出到一些不
可预期的文件中去。换句话说，2号句柄此时不再是标准错误输出了。比如:
write( 2, ... )
这样的调用存在安全问题。第二种方式可以避免上述问题，然而存在竞争环境问题。
现在看下述代码:
--------------------------------------------------------------------------
    int fd = open( "/dev/null", O_RDWR );
    /*
     * handle failure of open() somehow
     */
    dup2( fd, 0 );
    dup2( fd, 1 );
    dup2( fd, 2 );
    if ( fd > 2 )
    {
        close( fd );
    }
--------------------------------------------------------------------------
与第二种方式相比，这种代码是线程安全的。
有人认为对于后台守护进程做此类重定向操作浪费资源，建议直接关闭0、1、2号句
柄拉倒，这是非常不正确的。假设它们确实被关闭了，则一些普通数据文件句柄将等
于0、1、2。以2号句柄为例，某些库函数失败后会向2号句柄输出错误信息，这将破
坏原有数据。
D: 小四 <scz@nsfocus.com> 2002-04-25 16:47
2号句柄的此类安全问题在2002年4月23日得到了实际印证，可参看<<x86/FreeBSD 
4.5-RELEASE IO Smash及S/Key机制分析>>。
1987年，Henry Spencer在setuid(7)手册页中做了如下建议，一切标准I/O句柄都可
能因关闭过而不再是真实的标准I/O句柄，在使用printf()一类的函数前，务必确认
这些句柄是期待中的标准I/O句柄。1991年，在comp news上有人重贴了这份文档。
内核补丁应该确保对于SUID、SGID进程而言，0、1、2号句柄不会被打开后指向一个
普通文件。这有很多实现方式，比如使它们全部指向/dev/null。这种限制不应该在
库函数一级实现，可能有些SUID、SGID程序直接使用系统调用。
stdin、stdout、stderr中某一个被关闭，都可能潜在存在问题。
1992年W. Richard Stevens在<<Advanced Programming in the UNIX Environment>>
中建议Daemon进程应该关闭所有不必要的文件句柄，并将stdin、stdout、stderr指
向/dev/null。
自1998年以来，OpenBSD内核中execve()里有一个检查，如果句柄0、1、2是关闭的，
就打开/dev/null，使之对应0、1、2号句柄。这样就可以安全地执行setuid程序了。
FreeBSD/NetBSD直至最近才再次暴露出类似问题，而Linux在glibc中做了一些检查。
但是，OpenBSD这个检查存在一个问题，当falloc()失败时，应该转向错误处理，而
不是简单地跳出循环。art在注释中指出了这点，却无人去修正它。
--------------------------------------------------------------------------
sys/kern/kern_exec.c
在一个循环中，内核试图打开/dev/null，使之对应0-2号句柄
(...)
    if ( ( error = falloc( p, &fp, &indx ) ) != 0 )
    {
        break;
    }
(...)
--------------------------------------------------------------------------
于是本地用户获得一个内核文件表项相关的竞争环境，可以获取root权限。
