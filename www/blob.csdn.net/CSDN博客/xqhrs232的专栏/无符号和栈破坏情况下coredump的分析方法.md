# 无符号和栈破坏情况下coredump的分析方法 - xqhrs232的专栏 - CSDN博客
2016年10月21日 15:28:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：506
原文地址::[http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=20205875&id=4994994](http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=20205875&id=4994994)
zhangzhibiao02005 2014-09-20 16:41
r10            0xa      10
r11            0x202    514
r12            0x7f29ceb3ccd0   139817538276560
r13            0x583245af       1479689647
r14            0x21809000       562073600
r15            0x7f24ab8c16e0   139795473635040
rip            0x0      0
eflags         0x10207  66055
cs             0x33     51
ss             0x2b     43
ds             0x0      0
es             0x0      0
fs             0x63     99
gs             0x0      0
 查看rbp和rsp,看起来ok.
但是rip为0,肯定有问题,意味着下次执行的地址是0,显然会core.
说明当前线程的栈已经被写脏了.
再来看当前线程的栈的内容:
(gdb) x/32 0x58323e80
0x58323e80:     0x00000000      0x00000000      0x00000000      0x00000000
0x58323e90:     0x00000001      0x821d183c      0x00007f24      0x00000000
0x58323ea0:     0x00000000      0x00000000      0x00000000      0x00000000
0x58323eb0:     0x00000000      0x00000000      0x00000000      0x00000000
0x58323ec0:     0x00000000      0x00000000      0x00000000      0x00000000
0x58323ed0:     0x00000000      0x00000000      0x00000000      0x00000000
0x58323ee0:     0x00000000      0x00000000      0x00000000      0x00000000
0x58323ef0:     0x00000000      0x00000000      0x00000000      0x00000000
再次说明当前线程栈已经被写脏了. 那么这个线程的栈是被哪个线程写脏的呢?  还要继续追查.
5.查看线程:
(gdb) thread
[Current thread is 1 (process 14554)]
没办法,core里没有任何栈的有用信息.
4.查看dmesg,内核帮我们记录的进程运行信息.
gss[14554]: segfault at 0 ip (null) sp 0000000058323e80 error 14 in gss[400000+112c000]
// 这行是注释, 程序名, 线程号 , ip 指令地址, sp栈顶指针, gss程序名, 最后是链接器装入的地址  400000 长度是 112c000.
INFO: task gss:4461 blocked for more than 120 seconds.                                                                                                                            
"echo 0 > /proc/sys/kernel/hung_task_timeout_secs" disables this message.                                                                                                         
gss           D ffff880c40f51800     0  4461  13021 0x00000000                                                                                                                    
 ffff8808bbcf3900 0000000000000082 ffff8808a39cfc20 ffffffff81023a15                                                                                                              
 000000000000002c ffffffff810dedff ffff8808bbcf6ae0 0000008000000000                                                                                                              
 ffff8808bbcf6d88 0000000000012180 0000000000012180 000000000000d698                                                                                                              
Call Trace:                                                                                                                                                                       
 [] ? get_user_pages_fast+0xbc/0x170                                                                                                                            
 [] ? block_write_end+0x4e/0x59                                                                                                                                 
 [] ? get_futex_key+0x172/0x181                                                                                                                                 
 [] ? get_futex_value_locked+0x1e/0x2d                                                                                                                          
 [] ? __delayacct_add_tsk+0x16e/0x17d                                                                                                                           
 [] ? exit_mm+0x85/0x10e                                                                                                                                        
 [] ? do_exit+0x1f2/0x686                                                                                                                                       
 [] ? do_group_exit+0x70/0x97                                                                                                                                   
 [] ? get_signal_to_deliver+0x308/0x328                                                                                                                         
 [] ? do_signal+0x6c/0x6c2                                                                                                                                      
 [] ? do_notify_resume+0x25/0x64                                                                                                                                
 [] ? sys_write+0x62/0x6e                                                                                                                                       
 [] ? int_signal+0x12/0x17                                                                                                                                      
INFO: task gss:4631 blocked for more than 120 seconds.                         
......
INFO: task gss:14525 blocked for more than 120 seconds.                                                                                                                           
"echo 0 > /proc/sys/kernel/hung_task_timeout_secs" disables this message.                                                                                                         
gss           D ffff88089c4ed698     0 14525  13021 0x00000000                                                                                                                  
  
 ffff88085cd9e3c0 0000000000000082 0000000000000000 ffff8808bcea4000                                                                                                            
  
 0000000000000099 ffffffff810dedff ffff8808bbcf4020 0000000c2f62d225                                                                                                            
  
 ffff8808bbcf42c8 0000000000012180 0000000000012180 000000000000d698                                                                                                            
  
Call Trace:                                                                                                                                                                     
  
 [] ? block_write_end+0x4e/0x59                                                                                                                               
  
 [] ? generic_file_buffered_write+0x210/0x297                                                                                                                 
  
 [] ? exit_mm+0x85/0x10e                                                                                                                                      
  
 [] ? do_exit+0x1f2/0x686                                                                                                                                     
  
 [] ? thread_return+0x3e/0x106                                                                                                                                
  
 [] ? do_group_exit+0x70/0x97                                                                                                                                 
  
 [] ? get_signal_to_deliver+0x308/0x328                                                                                                                       
  
 [] ? do_signal+0x6c/0x6c2                                                                                                                                    
  
 [] ? update_rmtp+0x4b/0x5e                                                                                                                                   
  
 [] ? hrtimer_nanosleep+0xbd/0x108                                                                                                                            
  
 [] ? do_notify_resume+0x25/0x64                                                                                                                              
  
 [] ? int_signal+0x12/0x17                                                                                                                                    
  
gss[27354]: segfault at 40333868 ip 00007f9ebc0054c8 sp 0000000040333800 error 6 in appstore-mid.so[7f9ebbe40000+36d000]      
gss[13733]: segfault at 0 ip (null) sp 00000000619c7e80 error 14 in gss[400000+112c000]                                                                                           
gss[1246]: segfault at 0 ip (null) sp 0000000063c86e80 error 14 in gss[400000+112c000]                                                                                            
gss[16829]: segfault at 0 ip (null) sp 000000006b009e80 error 14 in gss[400000+112c000]         
 查看log,最后一次 14554 线程是 :
WARNING: 09-11 18:09:10:  gss. * 14554 [  logid:6feb2ab664e183d8  ][  reqip:  ][default_handler.cpp:751]not match any rule
上面加粗的字体非常重要. 
gss[27354]: segfault at 40333868 ip 00007f9ebc0054c8 sp 0000000040333800 error 6 in appstore-mid.so[7f9ebbe40000+36d000]     
来分析下这里.
6. 根据core文件, 第一次core是在:
$ stat ~work/opdir/coresave/core.201409111809.dump.gss                                                                      
  File: `/home/work/opdir/coresave/core.201409111809.dump.gss'
  Size: 63571140608     Blocks: 124283768  IO Block: 4096   regular file
Device: 803h/2051d      Inode: 82477057    Links: 1
Access: (0644/-rw-r--r--)  Uid: (  500/    work)   Gid: (  502/    work)
Access: 2014-09-11 18:20:16.000000000 +0800
Modify: 2014-09-11 18:12:25.000000000 +0800
Change: 2014-09-11 18:12:25.000000000 +0800
但是打开wf, 发现在:
WARNING: 09-11 18:18:45:  gss. * 27486 [  logid:7045a6705e0ff029  ][  reqip:10.40.26.15 uip:112.97.36.78  ][gss_work.cpp:1005]get_info() fail[6813]
WARNING: 09-11 18:18:45:  gss. * 27486 [  logid:7045a6705e0ff029  ][  reqip:10.40.26.15 uip:112.97.36.78  ][gss_work.cpp:1005]get_info() fail[6814]
WARNING: 09-11 18:18:45:  gss. * 27486 [  logid:7045a6705e0ff029  ][  reqip:10.40.26.15 uip:112.97.36.78  ][gss_work.cpp:1005]get_info() fail[6693]
WARNING: 09-11 18:18:45:  gss. * 27486 [  logid:7045a6705e0ff029  ][  reqip:10.40.26.15 uip:112.97.36.78  ][gss_work.cpp:1005]get_info() fail[6852]
WARNING: 09-11 18:18:48:  gss. * 10875 --------------------------------------------------------------- open log.wf ------------
WARNING: 09-11 18:18:48:  gss. * 10875 [  logid:  ][  reqip:  ][ub_conf.cpp:698]int [_svr_gss_query_read_bufsize] no found, use default value [1024000]
WARNING: 09-11 18:18:48:  gss. * 10875 [  logid:  ][  reqip:  ][ub_conf.cpp:698]int [_svr_gss_query_write_bufsize] no found, use default value [1024000]
WARNING: 09-11 18:18:48:  gss. * 10875 [  logid:  ][  reqip:  ][ub_conf.cpp:698]int [_svr_gss_cmd_bufsize] no found, use default value [1024000]
WARNING: 09-11 18:18:48:  gss. * 10875 [  logid:  ][  reqip:  ][ub_conf.cpp:762]load uint [_svr_gss_query_netio_threadnum] fail, use default value
 [1]
WARNING: 09-11 18:18:48:  gss. * 10875 [  logid:  ][  reqip:  ][ub_conf.cpp:762]load uint [_svr_gss_query_callback_directly] fail, use default value
 [0]
WARNING: 09-11 18:18:48:  gss. * 10875 [  logid:  ][  reqip:  ][ub_conf.cpp:499]no found [_svr_gss_cmd_port] range check item
但是这个core没有dump出来,因为上一个core是在18:20才dump完成,op为了控制连续出core打满磁盘,所以这里的core并没有dump.
结合上述的dmesg的消息,
gss[27354]: segfault at 40333868 ip 00007f9ebc0054c8 sp 0000000040333800 error 6 in appstore-mid.so[7f9ebbe40000+36d000]    
发现线程27354在40333868地址出core.指令寄存器是 00007f9ebc0054c8 , 栈顶指针是 0000000040333800 , error 6 是内核的定义,与这次core关系不大.
出core是在appstore-mid.so, 这个so装入的地址是 7f9ebbe40000  长度是 36d000.
因为我们的so是使用-fPIC编译的,所以里面的地址都是相对地址, 只有在最终启动gss的时候才有链接器给符号分配最终的地址 
所以,要想知道了core在appstore-mid.so的哪一行,还需要做一次运算,取出该地址在so中的相对地址:
实际很简单, 用ip寄存器地址减去装入地址即可 :
00007f9ebc0054c8  -  7f9ebbe40000 = 1C54C8
再使用objdump反汇编appstore-mid.so:
$ objdump -d /tmp/appstore-mid.so |grep -i 1C54C8
  1c54c8:       48 89 7c 24 68          mov    %rdi,0x68(%rsp)
这样就定位了该地址所在的指令.
用vim打开反汇编的文件,查看调用上下文,
00000000001c54b0 <_ZN16InnerSearchFrame16pack_data_us_midEP16gss_client_req_t>:
  1c54b0:       41 57                   push   %r15
  1c54b2:       41 56                   push   %r14
  1c54b4:       41 55                   push   %r13
  1c54b6:       41 54                   push   %r12
  1c54b8:       55                      push   %rbp
  1c54b9:       53                      push   %rbx
  1c54ba:       48 81 ec 58 ff 5b 00    sub    $0x5bff58,%rsp
  1c54c1:       48 8b 1d 20 84 2a 00    mov    2786336(%rip),%rbx        # 46d8e8 <_DYNAMIC+0x658>
  1c54c8:       48 89 7c 24 68          mov    %rdi,0x68(%rsp)
  1c54cd:       48 8b 03                mov    (%rbx),%rax
 发现是
int InnerSearchFrame::pack_data_us_mid(gss_client_req_t* csi)
 这个函数里面的语句.
为了更好的获取到具体的行,使用addr2line工具,
$ addr2line -e /tmp/appstore-mid.so 1c54c8 -f 
_ZN16InnerSearchFrame16pack_data_us_midEP16gss_client_req_t
/home/scmbuild/workspaces_cluster/ps.se.gss.kv.kv_1-3-124_BRANCH/ps/se/gss/kv/so/appstore_innersearch.cpp:361
下面就比较简单了, review下该函数的前后调用, 发现该cpp文件在栈上开辟了非常大的数组空间:
    bs_page_res bps;                                                                                                                                                              
    bs_page_res new_bps;  
 这两个数组,大致算了下,有近6M的空间.
我们的线上机器的栈大小是:
$ ulimit -a
core file size          (blocks, -c) unlimited
data seg size           (kbytes, -d) unlimited
file size               (blocks, -f) unlimited
pending signals                 (-i) 515362
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 65536
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
stack size              (kbytes, -s) 10240
cpu time               (seconds, -t) unlimited
max user processes              (-u) 30720
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
导致core已经定位: 线程栈溢出导致写脏其他线程的数据,最终core dump.
本次core之前在上线前,已经预测到可能会导致core,但是qa和流水线都没有core,所以就上线了,结果因为线上资源多,流量大,最终触发了coredump.
总结:
写代码的时候,避免在栈上直接开辟大数组,更不要写成:char arr[BIG_BUF_SIZ] = {0}; 这个语句相当与menset了这个数组,如果上线的话,甚至直接会增加几个ms的延时.
本次修改虽然只改了一个宏,但真正core的地方不是直接使用该宏,而是使用了包含该宏的大数组,间接地扩大了buffer. 后续修改类似的宏的时候,一定要仔细排查所有与宏相关的结构体和class.
