# linux下查看动态链接库so文件的依赖的相关组建 - DoubleLi - 博客园






我们很多c程序在windows下是以dll形式展现的，在linux则是以so 形式展现的。

  windows一般不会因为编译dll文件的编译器版本不同而出先dll文件不能执行。

  但是linux下，不同版本内核的linux下编译的c程序，在其他版本的linux下就容易出现无法执行的问题。主要可能是支持程序的内核相对于编译时的内核较高或者版本相对于编译时的内核较低。

  那我们如何看别人给我们提供的动态链接库文件（so后缀的）是否能在当前linux系统下可用呢。首先我们就要看他依赖的相关文件是否存在，查看命令如下：ldd file.so

假如我想看jnative的动态链接库在某个版本的linux下是否被支持，先切换到文件所在目录，然后写下如下命令：

 ldd libJNativeCpp.so

 如果正常，显示如下：

         libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x0069c000)
        libm.so.6 => /lib/tls/libm.so.6 (0x00111000)
        libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00562000)
        libc.so.6 => /lib/tls/libc.so.6 (0x00134000)
        /lib/ld-linux.so.2 (0x0097b000)
 如果不正常可能显示如下：

./libJNativeCpp.so: /lib/tls/libc.so.6: version `GLIBC_2.4' not found (required by ./JNativeCpp.so)
        libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x0047e000)
        libm.so.6 => /lib/tls/libm.so.6 (0x00111000)
        libc.so.6 => /lib/tls/libc.so.6 (0x0056e000)
        libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00c3d000)
        /lib/ld-linux.so.2 (0x0097b000)

这里是那个有名的jni第三方类库的默认的lib，上边的错误信息显示就是说我们的libJNativeCpp.so是在2.4内核下编译的，当前内核版本不支持。经过查看，我当前的linux版本的内核是2.6高于libJnativeCpp.so编译时的内核。



转自：[http://blog.csdn.net/kongqz/article/details/4027912](http://blog.csdn.net/kongqz/article/details/4027912)









