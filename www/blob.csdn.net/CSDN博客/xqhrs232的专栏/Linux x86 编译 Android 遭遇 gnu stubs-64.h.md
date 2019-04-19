# Linux x86 编译 Android 遭遇 gnu/stubs-64.h - xqhrs232的专栏 - CSDN博客
2011年06月30日 14:44:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1066
原文地址::[http://www.poemcode.net/2010/07/android-stubs-64/](http://www.poemcode.net/2010/07/android-stubs-64/)
CSDN相关网帖
编译android源码时，gcc将中定义了__x86_64__，编译不过，错误如下。我的PC是32位的，应该如何设置？
[http://topic.csdn.net/u/20100704/15/06d940c3-2ce9-4a15-a182-98a39c74f77a.html?seed=738195039&r=74129784#r_74129784](http://topic.csdn.net/u/20100704/15/06d940c3-2ce9-4a15-a182-98a39c74f77a.html?seed=738195039&r=74129784#r_74129784)
这两天心血来潮，执行完 repo sync 后，顺手来了一下 make，不料却发现了以下的错误：
host C: libclearsilver-jni <= external/clearsilver/java-jni/j_neo_util.c
In file included from /usr/include/features.h:378,
                      from /usr/include/string.h:26,
		      from external/clearsilver/java-jni/j_neo_util.c:1:
/usr/include/gnu/stubs.h:9:27: error: gnu/stubs-64.h: No such file or directory
make: *** [out/host/linux-x86/obj/SHARED_LIBRARIES/libclearsilver-jni_intermediates/j_neo_util.o] Error 1
兵来将挡，水来土掩，用关键字 android “error: gnu/stubs-64.h: no such file or directory” 来搜一下吧，发现搜索结果寥寥无几，看样子问题比较新，其中有一个 CSDN 的[链接](http://topic.csdn.net/u/20100704/15/06d940c3-2ce9-4a15-a182-98a39c74f77a.html?20065)，就顺手点了过去。帖子反映的问题和我一模一样，不过它的分析给了我一些提醒。
从错误信息入手，追踪到/usr/include/gnu/stubs.h，看到如下代码：
#if __WORDSIZE == 32# include <gnu/stubs-32.h>#elif __WORDSIZE == 64# include <gnu/stubs-64.h>#else# error "unexpected value for __WORDSIZE macro"#endif
顺着 WORDSIZE，进入/usr/include/bits/wordsize，发现：
#if defined __x86_64__# define __WORDSIZE	64# define __WORDSIZE_COMPAT32	1#else# define __WORDSIZE	32#endif
结合报错的信息，可以知道这里一定是有定义__x86_64__，执行一下 uname -a 可以确认 OS 明明是x86，为什么会__x86_64__呢？追根溯源， Android 源码或许能露出蛛丝马迹。
果不其然，从 android.git.kernel.org 的 platform/external/clearsilver.git 上，看到5天前，有个 Ying Wang 的家伙提交一个修改：Fix 64-bit clearsilver shared library issue，参看[这里](http://android.git.kernel.org/?p=platform/external/clearsilver.git;a=commit;h=d36910a8110d8377b22301274d2b5131a732a72b)，修改内容中
 java-jni/Android.mk，和出现本次问题的路径 java-jni/j_neo_util.c，同属一个父目录，看样子有点关系，查看 Android.mk 的修改内容，发现做了如下修改：
+# This forces a 64-bit build for Java6+ifneq ($(filter 1.6%,$(java_version)),)+    LOCAL_CFLAGS += -m64+    LOCAL_LDFLAGS += -m64+endif
当发现你使用Java6时，它将强制使用64-bit，我想这就能解释__x86_64__出现的原因。那么既然它要64-bit，我就满足它。首先要解决 gnu/stubs-64.h: No such file or directory，解决这个问题需要执行以下命令：
[xuhj@eniac mydroid]$ apt-get install libc6-dev-amd64
完成以后再执行 make，发现有错误：
/usr/bin/ld: cannot find -lstdc++
那就继续满足它：
[xuhj@eniac mydroid]$ apt-get install g++-multilib lib64stdc++6
还是不行，又发生下面的错误：
/usr/bin/ld: cannot find -lz
再来执行命令：
[xuhj@eniac mydroid]$ apt-get install lib64z1-dev
 //================================================================
补充我的解决方法::
   1>修改/build/core/main.mk中关于BUILD_OS及JAVA VERSION的条件判断检查----最开始我的JDK是JDK1.5.0_21
   2>修改下面4个文件----去掉m64的相关项
external/clearsilver/cgi/Android.mk,
 external/clearsilver/java-jni/Android.mk, 
external/clearsilver/util/Android.mk, 
external/clearsilver/cs/Android.mk.
   3>plainsocketimpl.java:233:method does not override a method from its superclass
      将JDK换成JDK1.6.0_22------记得导出JAVA_HOME改变PATH
   4>之所以用JDK1.5是因为JDK1.6编译doc会出问题,我只是听说
