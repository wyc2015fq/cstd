# 嵌入式平台下的ldd - DoubleLi - 博客园






x86平台有ldd可以很方便的查看对库的依赖关系，但在嵌入式linux环境中没有这个命令，替而代之是

CC=$(CROSS_COMPILE)gcc

LDD=$(CROSS_COMPILE)readelf

范例





**[plain]**[view plain](http://blog.csdn.net/jiazhen/article/details/50071349)[copy](http://blog.csdn.net/jiazhen/article/details/50071349)



- $(CROSS_COMPILE)readelf -d SDKDemo_video   
- Dynamic section at offset 0x1a88 contains 22 entries:  
-   Tag        Type                         Name/Value  
-  0x00000001 (NEEDED)                     Shared library: [libtxdevicesdk.so]  
-  0x00000001 (NEEDED)                     Shared library: [libpthread.so.0]  
-  0x00000001 (NEEDED)                     Shared library: [libdl.so.0]  
-  0x00000001 (NEEDED)                     Shared library: [libstdc++.so.6]  
-  0x00000001 (NEEDED)                     Shared library: [libc.so.0]  
-  0x0000000c (INIT)                       0x87cc  
-  0x0000000d (FINI)                       0x96d0  
-  0x00000019 (INIT_ARRAY)                 0x11a7c  
-  0x0000001b (INIT_ARRAYSZ)               4 (bytes)  
-  0x0000001a (FINI_ARRAY)                 0x11a80  
-  0x0000001c (FINI_ARRAYSZ)               4 (bytes)  
-  0x00000004 (HASH)                       0x8108  
-  0x00000005 (STRTAB)                     0x84ec  
-  0x00000006 (SYMTAB)                     0x824c  
-  0x0000000a (STRSZ)                      512 (bytes)  
-  0x0000000b (SYMENT)                     16 (bytes)  
-  0x00000015 (DEBUG)                      0x0  
-  0x00000003 (PLTGOT)                     0x11b60  
-  0x00000002 (PLTRELSZ)                   224 (bytes)  
-  0x00000014 (PLTREL)                     REL  
-  0x00000017 (JMPREL)                     0x86ec  
-  0x00000000 (NULL)                       0x0  
- 










