# linux下查看动态链接库依赖关系的命令  x86: ldd    *.so  arm: arm-linux-readelf    -d    *.so   实际例子: 以项目中用到的库librtsp.so分析： lijun@ubuntu:~/workspace$ arm-hisiv100nptl-linux-ld -d librtsp.so  arm-hisiv100nptl-linux-ld: - DoubleLi - 博客园






**linux下查看动态链接库依赖关系的命令 **
x86:
ldd    *.so

arm:
arm-linux-readelf    -d    *.so


**实际例子:**
以项目中用到的库librtsp.so分析：
[lijun@ubuntu:~/workspace$](mailto:lijun@ubuntu:~/workspace$) arm-hisiv100nptl-linux-ld -d librtsp.so 
*arm-hisiv100nptl-linux-ld: warning: liblog.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libcutils.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libutils.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libbinder.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libssl.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libcrypto.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libgui.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libmedia.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libstagefright_foundation.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libc.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: libstdc++.so, needed by librtsp.so, not found (try using -rpath or -rpath-link)*
*arm-hisiv100nptl-linux-ld: warning: cannot find entry symbol _start; defaulting to 00008204*
*librtsp.so: undefined reference to `__aeabi_unwind_cpp_pr0'*
*librtsp.so: undefined reference to `operator new[](unsigned int)'*
*librtsp.so: undefined reference to `__sF'*
*librtsp.so: undefined reference to `operator delete[](void*)'*
*librtsp.so: undefined reference to `_Unwind_Resume_or_Rethrow'*
*librtsp.so: undefined reference to `_Unwind_VRS_Get'*
*librtsp.so: undefined reference to `_toupper_tab_'*
*librtsp.so: undefined reference to `__gnu_unwind_frame'*
*librtsp.so: undefined reference to `__memset_chk'*
*librtsp.so: undefined reference to `pthread_key_create'*
*librtsp.so: undefined reference to `_Unwind_RaiseException'*
*librtsp.so: undefined reference to `__strcat_chk'*
*librtsp.so: undefined reference to `operator delete(void*)'*
*librtsp.so: undefined reference to `__aeabi_unwind_cpp_pr1'*
*librtsp.so: undefined reference to `_tolower_tab_'*
*librtsp.so: undefined reference to `__cxa_pure_virtual'*
*librtsp.so: undefined reference to `_Unwind_VRS_Set'*
*librtsp.so: undefined reference to `_Unwind_GetLanguageSpecificData'*
*librtsp.so: undefined reference to `__android_log_print'*
*librtsp.so: undefined reference to `pthread_getspecific'*
*librtsp.so: undefined reference to `_Unwind_Resume'*
*librtsp.so: undefined reference to `__strlen_chk'*
*librtsp.so: undefined reference to `_ctype_'*
*librtsp.so: undefined reference to `__isthreaded'*
*librtsp.so: undefined reference to `_Unwind_Complete'*
*librtsp.so: undefined reference to `_Unwind_DeleteException'*
*librtsp.so: undefined reference to `_Unwind_GetTextRelBase'*
*librtsp.so: undefined reference to `__aeabi_uldivmod'*
*librtsp.so: undefined reference to `__swbuf'*
*librtsp.so: undefined reference to `_Unwind_GetDataRelBase'*
*librtsp.so: undefined reference to `pthread_key_delete'*
*librtsp.so: undefined reference to `__sprintf_chk'*
*librtsp.so: undefined reference to `_Unwind_GetRegionStart'*
*librtsp.so: undefined reference to `__errno'*
*librtsp.so: undefined reference to `operator new(unsigned int)'*
*librtsp.so: undefined reference to `pthread_setspecific*


[lijun@ubuntu:~/workspace$](mailto:lijun@ubuntu:~/workspace$) arm-hisiv100nptl-linux-readelf -d librtsp.so

*Dynamic section at offset 0x93c14 contains 34 entries:*
*  Tag        Type                         Name/Value*
* 0x00000003 (PLTGOT)                     0x94e34*
* 0x00000002 (PLTRELSZ)                   896 (bytes)*
* 0x00000017 (JMPREL)                     0x40a08*
* 0x00000014 (PLTREL)                     REL*
* 0x00000011 (REL)                        0x37eb8*
* 0x00000012 (RELSZ)                      35664 (bytes)*
* 0x00000013 (RELENT)                     8 (bytes)*
* 0x6ffffffa (RELCOUNT)                   4380*
* 0x00000006 (SYMTAB)                     0x114*
* 0x0000000b (SYMENT)                     16 (bytes)*
* 0x00000005 (STRTAB)                     0xe264*
* 0x0000000a (STRSZ)                      148452 (bytes)*
* 0x00000004 (HASH)                       0x32648*
* 0x00000001 (NEEDED)                     Shared library: [liblog.so]*
* 0x00000001 (NEEDED)                     Shared library: [libcutils.so]*
* 0x00000001 (NEEDED)                     Shared library: [libutils.so]*
* 0x00000001 (NEEDED)                     Shared library: [libbinder.so]*
* 0x00000001 (NEEDED)                     Shared library: [libssl.so]*
* 0x00000001 (NEEDED)                     Shared library: [libcrypto.so]*
* 0x00000001 (NEEDED)                     Shared library: [libgui.so]*
* 0x00000001 (NEEDED)                     Shared library: [libmedia.so]*
* 0x00000001 (NEEDED)                     Shared library: [libstagefright_foundation.so]*
* 0x00000001 (NEEDED)                     Shared library: [libc.so]*
* 0x00000001 (NEEDED)                     Shared library: [libstdc++.so]*
* 0x00000001 (NEEDED)                     Shared library: [libm.so]*
* 0x0000000e (SONAME)                     Library soname: [librtsp.so]*
* 0x0000001a (FINI_ARRAY)                 0x90084*
* 0x0000001c (FINI_ARRAYSZ)               4 (bytes)*
* 0x00000019 (INIT_ARRAY)                 0x90088*
* 0x0000001b (INIT_ARRAYSZ)               36 (bytes)*
* 0x00000010 (SYMBOLIC)                   0x0*
* 0x0000001e (FLAGS)                      SYMBOLIC BIND_NOW*
* 0x6ffffffb (FLAGS_1)                    Flags: NOW*
* 0x00000000 (NULL)                       0x0*


**参考:**
apk相关和loadLibrary详解 
[http://www.cnblogs.com/lijunamneg/archive/2012/11/30/2795577.html](http://www.cnblogs.com/lijunamneg/archive/2012/11/30/2795577.html)

Linux的nm查看动态和静态库中的符号 
[http://www.cnblogs.com/itech/archive/2012/09/16/2687423.html](http://www.cnblogs.com/itech/archive/2012/09/16/2687423.html)

Linux命令学习手册-readelf命令 
[http://blog.chinaunix.net/uid-9525959-id-2001833.html](http://blog.chinaunix.net/uid-9525959-id-2001833.html)

eCos编译Synthethic Target程序时无法解析__sprintf_chk的解决办法 
[http://blog.csdn.net/zoomdy/article/details/10615853](http://blog.csdn.net/zoomdy/article/details/10615853)









