# GDB 结合Core File巧妙分析Crash原因 - xqhrs232的专栏 - CSDN博客
2012年06月01日 17:15:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：836标签：[crash																[file																[thread																[server																[360																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=360&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=crash&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://jimmyleeee.blog.163.com/blog/static/930961820103822435873/](http://jimmyleeee.blog.163.com/blog/static/930961820103822435873/)
编写服务器端程序，很容易遇到Crash问题，比较幸运的是Linux提供了core
 file，保留了Crash的现场。有时候，根据当前的调用栈，并且打印出当前栈的变量就可以分析出crash的原因，但是，有时候看到调用栈却束手无策。下面就介绍自己通过GDB的几个命令的结合，发现一个crash的原因的过程。
下面让我们一起进入现场，来逐步发现其中的原因。
首先，还是运行gdb 命令，**gdb wbxgs core.5797**,来看看现场。
[root@hfgs126 bin]# **gdb wbxgs_crash core.5797**
GNU gdb Red Hat Linux (6.3.0.0-1.132.EL4rh)
**……**
#0  0x00000038e8d70540 in strlen () from /lib64/tls/libc.so.6
(gdb) **bt**
#0  0x00000038e8d70540 in strlen () from /lib64/tls/libc.so.6
#1  0x000000000057cfc0 in T120_Trace::Text_Formator::advance (this=0x7e800a70, lpsz=0x1 <Address 0x1 out of bounds>)
    at ./t120trace.cpp:1464
#2  0x000000000057ceb1 in T120_Trace::Text_Formator::operator<< (this=0x7e800a70, lpsz=0x1 <Address 0x1 out of bounds>)
    at ./t120trace.cpp:1411
#3  0x0000000000407927 in ~func_tracer (this=0x7e804bd0) at ../h/t120trace.h:381
#4  0x00000000004432fd in CGSSocketServer::readHeader (this=0x8e4130, socketfd=1088,
    buf=0x7e806cc0 "GET /detectService?cmd=selfcheck HTTP/1.1\r\nConnection: Close\r\nHost: 10.224.122.94\r\n\r\n", bufsize=1024)
    at mgr/gssocketserver.cpp:337
#5  0x0000000000443981 in CGSSocketServer::handle (this=0x8e4130, socketfd=1088, strRet=@0x7e807190) at mgr/gssocketserver.cpp:424
#6  0x0000000000442f5e in CGSSocketServer::readThread (pArg=0x9ae9c0) at mgr/gssocketserver.cpp:304
#7  0x00000038e980610a in start_thread () from /lib64/tls/libpthread.so.0
#8  0x00000038e8dc68b3 in clone () from /lib64/tls/libc.so.6
#9  0x0000000000000000 in ?? ()
通过这个调用栈，可以看出，程序crash在打log的时候。虽然遇到过类似的crash，但是，当时的原因是有死循环，通过review
 code，没有发现死循环。但是当前的调用栈对于分析Crash的原因是一点用也没有，如果分析具体的原因呢？会不会是其他得线程出现错误导致程序Crash在这个线程呢？为了找到深一层的原因，尝试着通过GDB的一些关于线程的命令，来看看其他的线程是否有问题。于是，使用info
 threads，查看了一下当时线程的情况。
(gdb)** info threads**
**21 process 5797  0x00000038e8d7186d in memset () from /lib64/tls/libc.so.6**
  20 process 5839  0x00000038e8dc6c8c in epoll_wait () from /lib64/tls/libc.so.6
  19 process 5842  0x00000038e8d8f7d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6
  18 process 5845  0x00000038e8d8f7d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6
  17 process 5846  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  16 process 5847  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  15 process 5848  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  14 process 5849  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  13 process 5850  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  12 process 5852  0x00000038e8dbf946 in __select_nocancel () from /lib64/tls/libc.so.6
  11 process 5854  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  10 process 5856  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  9 process 5857  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  8 process 5858  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  7 process 5859  0x00000038e8d8f7d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6
  6 process 5861  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  5 process 5862  0x00000038e980a66f in sem_wait () from /lib64/tls/libpthread.so.0
  4 process 5863  0x00000038e8d8f7d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6
  3 process 5864  0x00000038e8d8f7d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6
  2 process 5883  0x00000038e8d8f7d5 in __nanosleep_nocancel () from /lib64/tls/libc.so.6
* 1 process 5853  0x00000038e8d70540 in strlen () from /lib64/tls/libc.so.6
对于线程如果停止在sleep或者wait的情况，都是正常的，但是我们看到thread
 21有些异常，程序停止在memset，不管是否有问题，都需要看看这样的线程具体有没有出错。
于是通过命令thread 21，进入到thread 21的调用栈。
(gdb) **thread 21**
[Switching to thread 21 (process 5797)]#0  0x00000038e8d7186d in memset () from /lib64/tls/libc.so.6
(gdb)** bt**
#0  0x00000038e8d7186d in memset () from /lib64/tls/libc.so.6
#1  0x000000000049da0d in CGSPduFactory::streamStringFrom (is=@0x7fff9b436360, strFrom=@0x2aaaec979760) at common/pdu/gspdu.cpp:422
#2  0x00000000004d1f25 in CGSOthShardUserRspPdu::streamFrom (this=0x2aaaec951650, is=@0x7fff9b436360) at common/pdu/pdugs.cpp:2707
#3  0x000000000049cb2d in CGSPduFactory::derivePdu (is=@0x7fff9b436360, ulPDULen=30506) at common/pdu/gspdu.cpp:79
#4  0x000000000049c78e in CGSPduFactory::streamPduFrom (pDataPacket=0x2aaaeca31d70) at common/pdu/gspdu.cpp:35
#5  0x0000000000449681 in CGSWDMSManager::on_wdms_message_indication (this=0x8e3680, msg=0x2aaae9894360)
    at mgr/gswdmsmanager.cpp:344
**……**
#18 0x0000000000407733 in main (argc=1, argv=0x7fff9b44ac98) at gsmain.cpp:118
(gdb)** f 3**
#3  0x000000000049cb2d in CGSPduFactory::derivePdu (is=@0x7fff9b436360, ulPDULen=30506) at common/pdu/gspdu.cpp:79
79      common/pdu/gspdu.cpp: No such file or directory.
        in common/pdu/gspdu.cpp
使用命令　i locals，打印所有的变量的值。
(gdb)** i locals**
pPdu = (CBasePdu *) 0x2aaaec951650
pPduHeader = (CPduHeader *) 0x2aaaea1c4190
ulPduType = 50
到现在还没有看出有什么明显的异常，然后再把ＰＤＵ的头打印出来如下：
(gdb) p *pPduHeader
$1 = {m_ulHeadLen = 61, m_ulVersion = 2080000, m_ulPduType = 50, m_ulSrcSvrType = WEBEX_CONNECT_GS, m_strSrcSvrAddr = {
    static npos = 18446744073709551615,
    _M_dataplus = {<std::allocator<char>> = {<__gnu_cxx::new_allocator<char>> = {<No data fields>}, <No data fields>},
      _M_p = 0x2aaaeca52a68 "**10.224.95.109:9900**"}}, m_strSubject = {static npos = 18446744073709551615,
    _M_dataplus = {<std::allocator<char>> = {<__gnu_cxx::new_allocator<char>> = {<No data fields>}, <No data fields>},
      _M_p = 0x2aaaec929b28 "qawin.qazone.GS"}}, m_ulSequence = 0}
从蓝色的字的部分可以看出，这个ＰＤＵ是从**10.224.95.109**这台ｓｅｒｖｅｒ上发过来的。
当时ＱＡ测试的环境，都是10.224.122开头的ＩＰ的ｓｅｒｖｅｒ，怎么会有这个ＩＰ的ＰＤＵ，于是，询问ＱＡ，发现**10.224.95.109**这个ｓｅｒｖｅｒ是其他DataCenter的Server,而且还是老的版本，由于当前测试环境的版本删除了两个PDU，同时又增加了四个ＰＤＵ，导致了老的ＰＤＵ发来的时候，新的版本的把它当作新的PUD解析，从而导致不能正确解析，最终导致了解析出来的长度不对。可以通过f
 1命令进入第一级调用栈查看所有的局部变量。
(gdb) f 1
#1  0x000000000049da0d in CGSPduFactory::streamStringFrom (is=@0x7fff9b436360, strFrom=@0x2aaaec979760) at common/pdu/gspdu.cpp:422
422in common/pdu/gspdu.cpp
(gdb)** i locals**
strTmp = 0x2aaaf1c00010 ""
iRet = 0
**ulLen = 1179995975**
可以看出解析出来的长度是一个很大的值**1179995975**，而线程２１正式停止在分配内存之后，使用ｍｅｍｓｅｔ时，停止在那里。从Log中也可以看到，thread
 21也一致阻塞在这里，而且没有再继续运行。
由于当时有两台ｓｅｒｖｅｒ crash，通过查看另外一台ｓｅｒｖｅｒ的core file，发现另外一台ｓｅｒｖｅｒ也是和本台ｓｅｒｖｅｒ一样的调用栈。在ＱＡ更新了**10.224.95.109**的版本后，crash没有再出现。
通过这个实例，可以看出，当server出现crash的时候，虽然当前的调用栈可能没有什么价值，但是，通过分析所有线程的调用栈，还是可能分析出蛛丝马迹的，从而对于解决Crash的问题带来帮助。
通过这个问题可以得到一个教训，在修改Server之间的接口时，一定要考虑到和老版本的兼容问题，即使这个PDU可能永远也不会使用，仍然需要保留，因为Production上，是先上GSB，然后再上Primary，肯定会存在两个版本同时运行的情况。如果出现删除或者改变PDU顺序的情况，可能会导致整个系统不能工作。
希望本文章，对解决Crash问题和避免类似的Crash问题有一定的借鉴作用。

