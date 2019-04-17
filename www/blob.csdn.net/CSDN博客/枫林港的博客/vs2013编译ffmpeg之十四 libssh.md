# vs2013编译ffmpeg之十四 libssh - 枫林港的博客 - CSDN博客





2018年07月08日 23:08:35[枫林港](https://me.csdn.net/u013601132)阅读数：46标签：[libssh																[ffmpeg																[vs2013																[编译](https://so.csdn.net/so/search/s.do?q=编译&t=blog)
个人分类：[vs2013编译ffmpeg](https://blog.csdn.net/u013601132/article/category/7785737)





# libssh

对应ffmpeg configure选项–enable-libssh。 
[ShiftMediaProject](https://github.com/ShiftMediaProject/libssh)上下载的。 

ffmpeg-3.3 configure libssh的时候出现下面这个错误：`error LNK2019: 无法解析的外部符号 __imp__sftp_init，该符号在函数 _check_sftp_init 中被引用`
sftp_init的定义是LIBSSH_API int sftp_init(sftp_session sftp)；LIBSSH_API在libssh.h里面定义的，LIBSSH_API要定义成空，sftp_init前面才不会加*_imp*前缀。将“-DLIBSSH_STATIC”放到libssh.pc里面的Cflags规则里面就可以解决这个问题，下载的libssh-master的工程里面也定义了LIBSSH_STATIC宏。

ffmpeg-3.3 configure libssh的时候出现下面的错误：

```
libgcryptd.lib(rndw32.obj) : error LNK2019: 无法解析的外部符号 __imp__GetProcessWindowStation@0，该符号在函数 __gcry_rndw32_gather_random_fast 中被引用
libgcryptd.lib(rndw32.obj) : error LNK2019: 无法解析的外部符号 __imp__GetMessagePos@0，该符号在函数 __gcry_rndw32_gather_random_fast 中被引用
libgcryptd.lib(rndw32.obj) : error LNK2019: 无法解析的外部符号 __imp__GetMessageTime@0，该符号在函数 __gcry_rndw32_gather_random_fast 中被引用
libgcryptd.lib(rndw32.obj) : error LNK2019: 无法解析的外部符号 __imp__GetClipboardOwner@0，该符号在函数 __gcry_rndw32_gather_random_fast 中被引用
libgcryptd.lib(rndw32.obj) : error LNK2019: 无法解析的外部符号 __imp__GetClipboardViewer@0，该符号在函数 __gcry_rndw32_gather_random_fast 中被引用
```

libssh要用到libgcryptd.lib，解决的办法是将user32.lib加到libssh.pc里面的Libs规则里面。

## libssh.pc

```
# libilbc.pc

prefix=../../contribute/MSVC/libssh-master
exec_prefix=${prefix}
libdir=${exec_prefix}/vs2013_build/lib/x86
includedir=${prefix}/vs2013_build/include

Name: libssh
Description: libssh
Version: 0.7.3
Libs: -L${libdir} libsshd.lib  libgcryptd.lib libgpg-errord.lib user32.lib -L../../contribute/MSVC/libgcrypt-master/vs2013_build/lib/x86 -L../../contribute/MSVC/libgpg-error-master/vs2013_build/lib/x86
Cflags: -DLIBSSH_STATIC -I${includedir}
```](https://so.csdn.net/so/search/s.do?q=vs2013&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)](https://so.csdn.net/so/search/s.do?q=libssh&t=blog)




