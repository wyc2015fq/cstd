# 错误:try using -rpath or -rpath-link - V__KING__的专栏 - CSDN博客





2017年12月15日 12:37:57[v__king__](https://me.csdn.net/V__KING__)阅读数：774








在使用到动态库的时候，出现错误如下：
`arm-linux/bin/ld: warning: libssl.so.0.9.8, needed by .//libcert.so, not found (try using -rpath or -rpath-link)`
我仔细查看编译命令`$(CFLAGS) –o target –L/usr/lib/ssl/ -lssl` 发现是没有问题的，但是还是出现`(try using -rpath or -rpath-link)`的错误。

从字面意义上来看，`libssl.so.0.9.8, needed by .//libcert.so, not found` 意思是libcert.so库依赖libssl.so.0.9.8库。

这里看出 `-Lssldir`和 `-lssl` 好像没有起到作用。想知道原因需要涨点姿势，如下：

### 姿势点：链接选项和路径

现代连接器在处理动态库时将链接时路径（Link-time path）和运行时路径（Run-time path）分开,用户可以通过-L指定连接时库的路径，通过-R（或-rpath）指定程序运行时库的路径，大大提高了库应用的灵活性。比如我们做嵌入式移植时`#arm-linux-gcc $(CFLAGS) –o target –L/work/lib/zlib/ -llibz-1.2.3` (`work/lib/zlib`下是交叉编译好的zlib库)，将target编译好后我们只要把zlib库拷贝到开发板的系统默认路径下即可。或者通过-rpath（或-R ）、`LD_LIBRARY_PATH`指定查找路径。

`-L`: “链接”的时候，去找的目录，也就是所有的 -lFOO 选项里的库，都会先从 -L 指定的目录去找，然后是默认的地方。编译时的-L选项并不影响环境变量LD_LIBRARY_PATH，-L只是指定了程序编译连接时库的路径，并不影响程序执行时库的路径，系统还是会到默认路径下查找该程序所需要的库，如果找不到，还是会报错，类似cannot open shared object file。

`-rpath-link`：这个也是用于“链接”的时候的，例如你显示指定的需要 libcert.so，但是 libcert.so 本身是依赖 libssl.so库 的，后者你并没有指定，而是 libcert.so 引用到它，这个时候，会先从 -rpath-link 给的路径里找。(注意动态库是运行时加载的哦)

`-rpath`: “运行”的时候，去找的目录。运行的时候，要找 .so 文件，会从这个选项里指定的地方去找。对于交叉编译，交叉编译链接器需已经配置 –with-sysroot 选项才能起作用。也就是说，-rpath指定的路径会被记录在生成的可执行程序中，用于运行时查找需要加载的动态库。-rpath-link 则只用于链接时查找。

### 综上所述

动态库中有复杂多个依赖关系时，最好加上`-Wl,-rpath` （-Wl必须加上的）。模板如下:

```
gcc -o foo foo.c -L/usr/local/lib -lfoo -Wl,-rpath=/usr/local/lib

$(CFLAGS) –o target –L/usr/local/ssl/lib -lssl -Wl,-rpath=/usr/local/ssl/lib
```

参考：[https://www.cnblogs.com/flyinggod/p/8026567.html](https://www.cnblogs.com/flyinggod/p/8026567.html)



