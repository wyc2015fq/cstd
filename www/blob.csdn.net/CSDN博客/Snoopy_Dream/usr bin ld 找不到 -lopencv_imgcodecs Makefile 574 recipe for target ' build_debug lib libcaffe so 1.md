# /usr/bin/ld: 找不到 -lopencv_imgcodecs  Makefile:574: recipe for target '.build_debug/lib/libcaffe.so.1 - Snoopy_Dream - CSDN博客





2018年05月04日 15:54:07[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：552








```python
/usr/bin/ld: 找不到 -lopencv_imgcodecs
collect2: error: ld returned 1 exit status
Makefile:574: recipe for target '.build_debug/lib/libcaffe.so.1.0.0' failed
make: *** [.build_debug/lib/libcaffe.so.1.0.0] Error 1
```

根据提示查看opencv_imgcodecs的相关，首先查看opencv的版本。


`pkg-config --modversion opencv`
查看说我的opencv 2.9.，但是3.0.0以前的opencv没有imgcodecs包，所以重新安装opencv3.1.0，ok。

具体安装过程可参考[https://blog.csdn.net/e01528/article/details/79529905](https://blog.csdn.net/e01528/article/details/79529905)





