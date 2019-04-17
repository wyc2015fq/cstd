# ldd 查看程序/动态库 的依赖 - DoubleLi - 博客园






今天在帮同事查看一个问题时, 需要用到ldd, 于是就顺便看了一下ldd的实现. 好在ldd本身只是一个脚本, 而不是executable, 可以直接查看实现的代码.

根据注释:

 21 # This is the `ldd' command, which lists what shared libraries are
 22 # used by given dynamically-linked executables.  It works by invoking the
 23 # run-time dynamic linker as a command and setting the environment
 24 # variable LD_TRACE_LOADED_OBJECTS to a non-empty value.


LDD只是设置了LD_TRACE_LOADED_OBJECTS后, 再运行程序或调用/lib/ld-linux.so.2(或/lib64/ld-linux-x86-64.so.2或其他, 取决于系统的architecture等等)来处理动态库, 再将输出分析从而得到整个依赖关系.


于是对于正常的exectuable(以a.out为例)而言, 另一种获得依赖关系列表的方式为:

export LD_TRACE_LOADED_OBJECTS=1
./a.out
输出的结果为:

- linux-gate.so.1 => (0xb78b5000)
- librt.so.1 => /lib/tls/i686/cmov/librt.so.1 (0xb78a4000)
- libselinux.so.1 => /lib/libselinux.so.1 (0xb7889000)
- libacl.so.1 => /lib/libacl.so.1 (0xb7880000)
- libc.so.6 => /lib/tls/i686/cmov/libc.so.6 (0xb7726000)
- libpthread.so.0 => /lib/tls/i686/cmov/libpthread.so.0 (0xb770d000)
- /lib/ld-linux.so.2 (0xb78b6000)
- libdl.so.2 => /lib/tls/i686/cmov/libdl.so.2 (0xb7709000)
- libattr.so.1 => /lib/libattr.so.1 (0xb7703000)



而对于.so(假设有个liba.so)而言, 另一种获得依赖关系的方式为:
export LD_TRACE_LOADED_OBJECTS=1
/lib/ld-linux.so.2 ./liba.so
输出结果为:

- linux-gate.so.1 => (0xb777e000)
- libdb-4.8.so => /home/db/opt/dbxml/install/lib/libdb-4.8.so (0xb7360000)
- libxqilla.so.5 => /home/db/opt/dbxml/install/lib/libxqilla.so.5 (0xb6f10000)
- libxerces-c-3.0.so => /home/db/opt/dbxml/install/lib/libxerces-c-3.0.so (0xb6b6c000)
- libz.so.1 => /lib/libz.so.1 (0xb6b51000)
- libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0xb6a5b000)
- libm.so.6 => /lib/tls/i686/cmov/libm.so.6 (0xb6a35000)
- libc.so.6 => /lib/tls/i686/cmov/libc.so.6 (0xb68db000)
- libgcc_s.so.1 => /lib/libgcc_s.so.1 (0xb68bb000)
- libpthread.so.0 => /lib/tls/i686/cmov/libpthread.so.0 (0xb68a2000)
- libnsl.so.1 => /lib/tls/i686/cmov/libnsl.so.1 (0xb688b000)
- /lib/ld-linux.so.2 (0xb777f000)



运行中发现貌似/lib/ld-linux.so.2后面的参数是需要带绝对路径或者相对路径的(当前目录则需要用./作为前缀), 目前还不知道ld-linux.so.2是如何确定参数路径的. 当然, 实际上executable也可以用/lib/ld-linux.so.2来获得依赖列表.

实际上/lib/ld-linux.so.2是用来运行程序的. 当执行一个动态链接的程序时, 该工具被自动调用用来获得并解析程序的依赖关系而后执行程序本身. 关于/lib/ld-linux.so.2命令的详情, 参见 http://www.kernel.org/doc/man-pages/online/pages/man8/ld.so.8.html









