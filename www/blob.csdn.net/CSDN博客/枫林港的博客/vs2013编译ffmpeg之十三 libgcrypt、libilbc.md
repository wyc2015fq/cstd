# vs2013编译ffmpeg之十三 libgcrypt、libilbc - 枫林港的博客 - CSDN博客





2018年07月08日 23:07:15[枫林港](https://me.csdn.net/u013601132)阅读数：188标签：[libgcrypt																[libgpg																[ffmpeg																[vs2013																[编译](https://so.csdn.net/so/search/s.do?q=编译&t=blog)
个人分类：[vs2013编译ffmpeg](https://blog.csdn.net/u013601132/article/category/7785737)





# libgcrypt

对应ffmpeg configure选项–enable-gcrypt。 
[ShiftMediaProject](https://github.com/ShiftMediaProject/libgcrypt)上下载的。MSVC2013编译没有遇到问题。 

ffmpeg-3.0在configure的时候出现下面的错误：
```
LINK : fatal error LNK1181: 无法打开输入文件“gpg-error.lib”
ERROR: gcrypt not found
```

configure针对enabled gcrypt的规则如下：

```
if enabled gcrypt; then
    GCRYPT_CONFIG="${cross_prefix}libgcrypt-config"
    if "${GCRYPT_CONFIG}" --version > /dev/null 2>&1; then
        gcrypt_cflags=$("${GCRYPT_CONFIG}" --cflags)
        gcrypt_libs=$("${GCRYPT_CONFIG}" --libs)
        check_func_headers gcrypt.h gcry_mpi_new $gcrypt_cflags $gcrypt_libs ||
            die "ERROR: gcrypt not found"
        add_cflags $gcrypt_cflags && add_extralibs $gcrypt_libs
    else
        require2 gcrypt gcrypt.h gcry_mpi_new libgcryptd.lib
    fi
fi
```

出现这个错误的系统里面有libgcrypt-config，从而走的是第一个分支，用的是libgcrypt-config –cflags和–libs所得的参数：

```bash
$ libgcrypt-config --libs
-L/usr/win32/lib -lgcrypt -lgpg-error
```

libgcrypt-config是msys的命令，为了去掉这个依赖，正常的检测应该走else分支，直接删除libgcrypt-config。

## libgpg

libgcrypt依赖于libgpg，ShiftMediaProject上下载的[libgpg](https://github.com/ShiftMediaProject/libgpg-error)。MSVC2013编译没有遇到问题。

# libilbc

ShiftMediaProject上下载的(目前已经没有此项目了)。MSVC2013编译没有遇到问题。](https://so.csdn.net/so/search/s.do?q=vs2013&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)](https://so.csdn.net/so/search/s.do?q=libgpg&t=blog)](https://so.csdn.net/so/search/s.do?q=libgcrypt&t=blog)




