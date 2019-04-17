# Linux C/C++内存泄漏检测工具:Valgrind - DoubleLi - 博客园






Valgrind 是一款 Linux下(支持 x86、x86_64和ppc32)程序的内存调试工具，它可以对编译后的二进制程序进行内存使用监测(C语言中的malloc和free，以及C++中的new和delete)，找出内存泄漏问题。
![](http://www.embeddedlinux.org.cn/uploads/userup/1504/25202542M54.jpg)
　　Valgrind 中包含的 Memcheck 工具可以检查以下的程序错误：

　　使用未初始化的内存 (Use of uninitialised memory)
　　使用已经释放了的内存 (Reading/writing memory after it has been free’d)
　　使用超过malloc分配的内存空间(Reading/writing off the end of malloc’d blocks)
　　对堆栈的非法访问 (Reading/writing inappropriate areas on the stack)
　　申请的空间是否有释放 (Memory leaks - where pointers to malloc’d blocks are lost forever)
　　malloc/free/new/delete申请和释放内存的匹配(Mismatched use of malloc/new/new [] vs free/delete/delete [])
　　src和dst的重叠(Overlapping src and dst pointers in memcpy() and related functions)
　　重复free

　　1、编译安装 Valgrind：

wget [http://valgrind.org/downloads/valgrind-3.4.1.tar.bz2](http://valgrind.org/downloads/valgrind-3.4.1.tar.bz2)
tar xvf valgrind-3.4.1.tar.bz2
cd valgrind-3.4.1/
./configure --prefix=/usr/local/webserver/valgrind
make
make install



　　2、使用示例：对“ls”程序进程检查，返回结果中的“definitely lost: 0 bytes in 0 blocks.”表示没有内存泄漏。

[root@xoyo42 /]# /usr/local/webserver/valgrind/bin/valgrind --tool=memcheck --leak-check=full ls /
==1157== Memcheck, a memory error detector.
==1157== Copyright (C) 2002-2008, and GNU GPL'd, by Julian Seward et al.
==1157== Using LibVEX rev 1884, a library for dynamic binary translation.
==1157== Copyright (C) 2004-2008, and GNU GPL'd, by OpenWorks LLP.
==1157== Using valgrind-3.4.1, a dynamic binary instrumentation framework.
==1157== Copyright (C) 2000-2008, and GNU GPL'd, by Julian Seward et al.
==1157== For more details, rerun with: -v
==1157== 
bin   data0  dev  home  lib64       media  mnt  opt   root  selinux  sys       tcsql.db.idx.pkey.dec  ttserver.pid  var
boot  data1  etc  lib   lost+found  misc   net  proc  sbin  srv      tcsql.db  tmp                    usr
==1157== 
==1157== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 5 from 1)
==1157== malloc/free: in use at exit: 28,471 bytes in 36 blocks.
==1157== malloc/free: 166 allocs, 130 frees, 51,377 bytes allocated.
==1157== For counts of detected errors, rerun with: -v
==1157== searching for pointers to 36 not-freed blocks.
==1157== checked 174,640 bytes.
==1157== 
==1157== LEAK SUMMARY:
==1157==    definitely lost: 0 bytes in 0 blocks.
==1157==      possibly lost: 0 bytes in 0 blocks.
==1157==    still reachable: 28,471 bytes in 36 blocks.
==1157==         suppressed: 0 bytes in 0 blocks.
==1157== Reachable blocks (those to which a pointer was found) are not shown.
==1157== To see them, rerun with: --leak-check=full --show-reachable=yes



　　3、使用示例：对一个使用libevent库编写的“httptest”程序进程检查，返回结果中的“definitely lost: 255 bytes in 5 blocks.”表示发生内存泄漏。

[root@xoyo42 tcsql-0.1]# /usr/local/webserver/valgrind/bin/valgrind --tool=memcheck --leak-check=full ./httptest
==1274== Memcheck, a memory error detector.
==1274== Copyright (C) 2002-2008, and GNU GPL'd, by Julian Seward et al.
==1274== Using LibVEX rev 1884, a library for dynamic binary translation.
==1274== Copyright (C) 2004-2008, and GNU GPL'd, by OpenWorks LLP.
==1274== Using valgrind-3.4.1, a dynamic binary instrumentation framework.
==1274== Copyright (C) 2000-2008, and GNU GPL'd, by Julian Seward et al.
==1274== For more details, rerun with: -v
==1274== 
==1274== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 1005 from 2)
==1274== malloc/free: in use at exit: 402,291 bytes in 74 blocks.
==1274== malloc/free: 15,939 allocs, 15,865 frees, 6,281,523 bytes allocated.
==1274== For counts of detected errors, rerun with: -v
==1274== searching for pointers to 74 not-freed blocks.
==1274== checked 682,468,160 bytes.
==1274== 
==1274== 255 bytes in 5 blocks are definitely lost in loss record 17 of 32
==1274==    at 0x4A05FBB: malloc (vg_replace_malloc.c:207)
==1274==    by 0x3C1D809BC6: evhttp_decode_uri (http.c:2105)
==1274==    by 0x401C75: tcsql_handler (in /data0/tcsql/cankao/tcsql-0.1/tcsql)
==1274==    by 0x3C1D80C88F: evhttp_get_body (http.c:1582)
==1274==    by 0x3C1D8065F7: event_base_loop (event.c:392)
==1274==    by 0x403E2F: main (in /data0/tcsql/cankao/tcsql-0.1/tcsql)
==1274== 
==1274== LEAK SUMMARY:
==1274==    definitely lost: 255 bytes in 5 blocks.
==1274==      possibly lost: 0 bytes in 0 blocks.
==1274==    still reachable: 402,036 bytes in 69 blocks.
==1274==         suppressed: 0 bytes in 0 blocks.
==1274== Reachable blocks (those to which a pointer was found) are not shown.
==1274== To see them, rerun with: --leak-check=full --show-reachable=yes



　　检查httptest程序，发现有一处“char *decode_uri = evhttp_decode_uri(evhttp_request_uri(req));”中的“decode_uri”没有被free，再程序处理完成后加上“free(decode_uri);”后，再使用Valgrind检查，结果已经是“definitely lost: 0 bytes in 0 blocks.”。









