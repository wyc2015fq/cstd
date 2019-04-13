
# levmar ndk 编译 - 小小情意 - 博客园






# [levmar ndk 编译](https://www.cnblogs.com/xiaoxiaoqingyi/p/7067096.html)
levmar是一个强大的和高效率的C/C++库，采用Levenberg - 马奎德（LM）优化算法，
主要是为了解决非线性最小二乘问题。官网是：[http://users.ics.forth.gr/~lourakis/levmar/](http://users.ics.forth.gr/~lourakis/levmar/)
在PC引用，网上有挺多的资料可以查询到，但我想应用到Android上来，google了一下，找不到太多资料，只有一个国外几年前的项目：[https://github.com/3deggi/levmar-ndk](https://github.com/3deggi/levmar-ndk)
因为项目太久远，还是采用 eclipse IDE来开发的，我尝试搬到 Android Studio 来编译，还是编译不成功，最后只有重新配置eclipse 环境。最重要是，还得使用比较旧版本的NDK来编译，我选用的NDK版本是 12b。过程中有些小错误，我就稍微调整了一下，然后按eclipse NDK的步骤编译了一遍。
项目中 的 application.mk 里有一项配置：
APP_ABI := armeab
这个是CPU 的指令集配置。一般我们在编译的时候，都会兼容多种CPU，于是改为：
APP_ABI := all
编译中，没有什么大问题，有部分警告而已，编译成功。
编译成功后，在libs目录下出现了 arm64-v8a、armeabi、armeabi-v7a、mips、mips64、x86、x86_64 指令集目录。于是，我就用真机测试一下，有部分手机通过了，有部分手机运行出错。
各种调试之后，我发现可能是不同手机的CPU 支持的指令集不一样，于是我通过以下代码输出每台手机的指令集:
String[] abis =newString[]{};
 abis= Build.SUPPORTED_ABIS;

得到结果如下：
魅族 note2:
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170622202650866-1745295268.png)
vivo Y51A
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170622202719023-480343588.png)

oppo r9s:
arm64-v8a,
armeabi-v7a,
armeabi,
当时vivo Y51A 能运行通过，而其他两台手机运行时会崩溃。然后我猜可能 是arm64-v8a 库出错了。于是我把这个文件以及里面的文件给删了， 果然，全部手机都运行通过了。
从这里大概知道：APP在获取动态库时，首先会根据CPU支持的指令集获取第一个目录下的动态库，如果获取不到，就会到下一个支持的指令集目录获取，以此类推。
当然这个项目算是编译成功了，因为目前多数手机的CPU都会支持 armeabi-v7a 和 armeabi。但我们也知道很多第三方的库都会兼容 arm64-v8a 这种指令集，这个64位的指令集应该会跑得更快。
我也尝试去重新编译 levmar 库，让它兼容 arm64-v8a 指令集。 我下载了其它版本的 NDK来尝试，因为 ndk-build 命令在 r13 中默认使用 Clang，他们将在后续版本中移除 GCC。levmar 库其实也使用到了一些 matlab 等其它语言库，我担心是这方面编译出错了。当我使用了其它版本的NDK编译后，编译是成功了，但是在 JNI 调用的时候就会报这样的错误：
06-22 16:32:40.741: A/libc(5024): Fatal signal 11 (SIGSEGV), code 1, fault addr 0x0 in tid 5024 (hvrl.levmar)
查了一下，这个错误的原因是，查不到相应 JNI 的实现方法，或者是 native 的代码执行时出错了。因为获取不到C/C++ 的错误日志，我也只能暂时放弃兼容 arm64-v8a 指令集。
levmar库是编译成功了，是一个 levmar.a 静态库， 这里 levmar 也使用到了 clapack，也编译成了 .a静态库。
我已经编译好的项目链接：[https://pan.baidu.com/s/1pLPV4oV](https://pan.baidu.com/s/1pLPV4oV)
欢迎大家关注我，或一起讨论。






